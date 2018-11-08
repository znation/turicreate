/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

// Server impl code adapted from Beast example in
// boost/beast/example/http/server/async/http_server_async.cpp

#include <logger/logger.hpp>
#include <unity/lib/visualization/server.hpp>

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// generated include files for front-end artifacts
#include <unity/lib/visualization/html/vega.h>
#include <unity/lib/visualization/html/style.h>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace qi = boost::spirit::qi;
typedef std::map<std::string, std::string> pairs_type;
using namespace turi::visualization;

// Report a failure
void
fail(boost::system::error_code ec, char const* what)
{
    logstream(LOG_ERROR) << "Web server error: " << what << ": " << ec.message() << std::endl;
}

// Parse query string from URL
// URL parsing code adapted from Spirit example in
// boost/spirit/example/qi/key_value_sequence.cpp
template <typename Iterator>
struct url_query_string 
    : qi::grammar<Iterator, pairs_type()>
{
    url_query_string()
        : url_query_string::base_type(query)
    {
        query =  pair >> *((qi::lit('&') | '&') >> pair);
        pair  =  key >> -('=' >> value);
        key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
        value = +qi::char_("a-zA-Z_0-9");
    }

    qi::rule<Iterator, pairs_type()> query;
    qi::rule<Iterator, std::pair<std::string, std::string>()> pair;
    qi::rule<Iterator, std::string()> key, value;
};

// Request handler function
template<
    class Body, class Allocator,
    class Send>
void
handle_request(
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send,
    WebServer::plot_map& m_plots)
{
    static const std::string vega_html = std::string(reinterpret_cast<char *>(html_vega_html), html_vega_html_len);
    static const std::string style_css = std::string(reinterpret_cast<char *>(html_style_css), html_style_css_len);

    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = why.to_string();
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](boost::beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + target.to_string() + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req, &send](boost::beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + what.to_string() + "'";
        res.prepare_payload();
        return send(std::move(res));
    };

    // Make sure we can handle the method= 
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head)
        return send(bad_request("Unknown HTTP-method"));

    auto const respond =
    [&req, &send, &server_error](const std::string& body, const std::string& content_type) {
        if(req.method() == http::verb::head) {
            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, content_type);
            res.content_length(body.size());
            res.keep_alive(req.keep_alive());
            return send(std::move(res));
        } else if(req.method() == http::verb::get) {
            http::response<http::string_body> res{
                std::piecewise_construct,
                std::make_tuple(body),
                std::make_tuple(http::status::ok, req.version())};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, content_type);
            res.content_length(body.size());
            res.keep_alive(req.keep_alive());
            return send(std::move(res));
        } else {
            return server_error("Unexpected request method. Expected either HEAD or GET.");
        }
    };


    try {
        auto req_target = req.target();
        if (req_target.find("/vega.html") == 0) {
            // parse the id out of the query string and use it to find the plot
            const size_t vega_html_length = sizeof("/vega.html?") - 1;
            std::string qs = req_target.substr(vega_html_length).to_string();

            std::string::iterator qs_begin = qs.begin();
            std::string::iterator qs_end = qs.end();
            url_query_string<std::string::iterator> p;
            pairs_type queries;

            if (!qi::parse(qs_begin, qs_end, p, queries))
            {
                std::stringstream ss;
                ss << "Unable to parse query string from URL ";
                ss << req_target;
                return server_error(ss.str());
            }
            if (queries.find("plot") == queries.end()) {
                std::stringstream ss;
                ss << "Expected ?plot= in query string; did not find it in URL ";
                ss << req_target;
                return server_error(ss.str());
            }
            std::string plot_id = queries.at("plot");
            if (m_plots->find(plot_id) == m_plots->end()) {
                return server_error("Expected plot " + plot_id + " was not found");
            }

            const Plot& plot = m_plots->at(plot_id);

            // load the spec and data, and format them into the HTML page
            std::string vega_spec = plot.get_spec();
            std::string vega_data = plot.get_data();
            std::string rendered_page = boost::str(boost::format(vega_html) % vega_spec % vega_data);
            return respond(rendered_page, "text/html");
        } else if (req_target == "/style.css" || req_target.find("/style.css?") == 0) {
            return respond(style_css, "text/css");
        }

        // did not match any expected URL
        logstream(LOG_ERROR) << "WebServer: unrecognized destination requested in URL:  " << req_target << std::endl;
        return send(not_found(req_target));

    } catch (const std::exception& e) {
        return server_error(e.what());
    } catch (...) {
        return server_error("Unknown exception");
    }
};


// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
private:
    // This is the C++11 equivalent of a generic lambda.
    // The function object is used to send an HTTP message.
    struct send_lambda
    {
        session& self_;

        explicit
        send_lambda(session& self)
            : self_(self)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void
        operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto sp = std::make_shared<
                http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            self_.res_ = sp;

            // Write the response
            http::async_write(
                self_.socket_,
                *sp,
                boost::asio::bind_executor(
                    self_.strand_,
                    std::bind(
                        &session::on_write,
                        self_.shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2,
                        sp->need_eof())));
        }
    };

    tcp::socket socket_;
    boost::asio::strand<
        boost::asio::io_context::executor_type> strand_;
    boost::beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;
    WebServer::plot_map& m_plots; // reference to the uuid->plot dictionary

public:
    // Take ownership of the socket
    explicit
    session(
        tcp::socket socket,
        WebServer::plot_map& plots)
        : socket_(std::move(socket))
        , strand_(socket_.get_executor())
        , lambda_(*this)
        , m_plots(plots)
    {
    }

    // Start the asynchronous operation
    void
    run()
    {
        do_read();
    }

    void
    do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Read a request
        http::async_read(socket_, buffer_, req_,
            boost::asio::bind_executor(
                strand_,
                std::bind(
                    &session::on_read,
                    shared_from_this(),
                    std::placeholders::_1,
                    std::placeholders::_2)));
    }

    void
    on_read(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if(ec == http::error::end_of_stream)
            return do_close();

        if(ec)
            return fail(ec, "read");

        // Send the response
        handle_request(std::move(req_), lambda_, m_plots);
    }

    void
    on_write(
        boost::system::error_code ec,
        std::size_t bytes_transferred,
        bool close)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        if(close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // We're done with the response so delete it
        res_ = nullptr;

        // Read another request
        do_read();
    }

    void
    do_close()
    {
        // Send a TCP shutdown
        boost::system::error_code ec;
        socket_.shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    WebServer::plot_map& m_plots; // reference to the uuid->plot dictionary

public:
    listener(
        boost::asio::io_context& ioc,
        tcp::endpoint endpoint,
        WebServer::plot_map& plots)
        : acceptor_(ioc)
        , socket_(ioc)
        , m_plots(plots)
    {
        boost::system::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            boost::asio::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {
        if(! acceptor_.is_open())
            return;
        do_accept();
    }

    void
    do_accept()
    {
        acceptor_.async_accept(
            socket_,
            std::bind(
                &listener::on_accept,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_accept(boost::system::error_code ec)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(
                std::move(socket_),
                m_plots)->run();
        }

        // Accept another connection
        do_accept();
    }
};

static unsigned short find_port() {
    for (unsigned short port=8000; port<=9000; port++) {
        logstream(LOG_DEBUG) << "WebServer: checking port " << port << std::endl;
        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0) {
            if (errno == EMFILE) {
                log_and_throw("Unable to open a port between 8000 and 9000 (inclusive) to host Turi Create visualizations: too many open file handles.");
            }
            log_and_throw("Unable to open a port between 8000 and 9000 (inclusive) to host Turi Create visualizations: unknown error.");
        }
        int optval = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(inet_addr("0.0.0.0"));
        int error = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
        if (error == 0) {
            return port;
        } else {
            logstream(LOG_DEBUG) << "WebServer: return was " << error << ", error was " << errno << " (" << std::strerror(errno) << ")" << std::endl;
        }

    }
    log_and_throw("Unable to open a port between 8000 and 9000 (inclusive) to host Turi Create visualizations: all ports seem to be in use.");
}

class WebServer::Impl {
public:
    unsigned short m_port;
    const static size_t NUM_THREADS = 6; // handle requests on 6 threads - TODO optimize this number
    boost::asio::io_context m_ioc{NUM_THREADS}; 
    std::vector<std::thread> m_threads; // listener threads
    std::shared_ptr<listener> m_listener = nullptr;

    Impl(plot_map plots) {
        logstream(LOG_DEBUG) << "WebServer: starting WebServer::Impl\n";
        m_port = find_port();
        const auto address = boost::asio::ip::make_address("127.0.0.1");

        // Create and launch a listening port
        m_listener = std::make_shared<listener>(
            m_ioc,
            tcp::endpoint{address, m_port},
            plots);
        m_listener->run();

        // Run the ioc on background threads
        for (size_t i=0; i<NUM_THREADS; i++) {
            m_threads.push_back(std::thread([&]() {
                m_ioc.run();
            }));
        }

        logstream(LOG_DEBUG) << "WebServer: finished starting WebServer::Impl\n";
    }
    ~Impl() {
        logstream(LOG_DEBUG) << "WebServer: destroying WebServer::Impl\n";
        m_ioc.stop();
        for (auto& thread : m_threads) {
            thread.join();
        }
        logstream(LOG_DEBUG) << "WebServer: finished destroying WebServer::Impl\n";
    }
};

WebServer::WebServer() : m_impl(new Impl(m_plots)) {
    logstream(LOG_DEBUG) << "WebServer: starting WebServer\n";
}
WebServer::~WebServer() {
    logstream(LOG_DEBUG) << "WebServer: destroying WebServer\n";
}

std::string WebServer::get_url_for_plot(const Plot& plot) {
    static WebServer server;
    return server.add_plot(plot);
}

std::string WebServer::add_plot(const Plot& plot) {
    // make UUID for plot
    static auto uuid_generator = boost::uuids::random_generator();
    auto uuid = uuid_generator();
    auto uuid_str = boost::lexical_cast<std::string>(uuid);

    // add to dictionary with UUID
    (*m_plots)[uuid_str] = plot;

    // return formatted URL
    std::string port_str = std::to_string(m_impl->m_port);
    return "http://localhost:" + port_str + "/vega.html?plot=" + uuid_str;
}