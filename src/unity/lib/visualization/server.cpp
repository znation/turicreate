/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#include <unity/lib/visualization/server.hpp>
#include <unity/lib/visualization/thread.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace http = boost::network::http;
using namespace turi::visualization;

class handler_type;
typedef http::server<handler_type> http_server;

class handler_type {
public:
    void operator() (http_server::request const &request,
                     http_server::connection_ptr const &connection) {
        static http_server::response_header headers[] = {{"Connection", "close"},
                                                    {"Content-Type", "text/plain"}};
        std::cerr << "DEBUG: handling connection\n";
        connection->set_status(http_server::connection::status_t::ok);
        connection->set_headers(boost::make_iterator_range(headers, headers + 2));
        std::string msg = "Hello, World!";
        connection->write(msg, [](std::error_code ec) {
            std::cerr << "DEBUG: finished writing to connection object. Error code was " << ec << std::endl;
        });
        std::cerr << "DEBUG: returning from handling connection\n";
    }

    void log(http_server::string_type const &info) {
        // TODO use turi logging
        std::cerr << "ERROR: " << info << '\n';
    }
};

class WebServer::Impl {
public:
    handler_type m_handler;
    http_server::options m_options;
    std::unique_ptr<http_server> m_server = nullptr;
    Impl() : m_handler(), m_options(m_handler) {
        std::cerr << "DEBUG: starting WebServer::Impl\n";
        m_options.thread_pool(
            std::make_shared<boost::network::utils::thread_pool>());
        m_server.reset(new http_server(m_options.address("localhost").port("8000")));
        run_thread([this]() {
            std::cerr << "DEBUG: server `run` starting\n";
            m_server->run();
            std::cerr << "DEBUG: server `run` completed\n";
        });
        std::cerr << "DEBUG: finished starting WebServer::Impl\n";
    }
    ~Impl() {
        std::cerr << "DEBUG: destroying WebServer::Impl\n";
    }
};

WebServer::WebServer() : m_impl(new Impl()) {}
WebServer::~WebServer() {
    std::cerr << "DEBUG: destroying WebServer\n";
}

std::string WebServer::get_url_for_plot(const Plot& plot) {
    std::cerr << "DEBUG: getting url for plot\n";
    static WebServer server;
    std::cerr << "DEBUG: past WebServer constructor\n";
    return server.add_plot(plot);
}

std::string WebServer::add_plot(const Plot& plot) {
    // make UUID for plot
    static auto uuid_generator = boost::uuids::random_generator();
    auto uuid = uuid_generator();
    auto uuid_str = boost::lexical_cast<std::string>(uuid);

    // add to dictionary with UUID
    m_plots[uuid_str] = plot;

    // return formatted URL
    return "http://localhost:8000/vega.html?plot=" + uuid_str;
}
