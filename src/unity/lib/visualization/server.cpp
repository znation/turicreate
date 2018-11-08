/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#include <unity/lib/visualization/server.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

using namespace turi::visualization;

static int find_port() {
    for (int port=8000; port<=9000; port++) {
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
    std::string m_port;

    Impl(plot_map plots) {
        logstream(LOG_DEBUG) << "WebServer: starting WebServer::Impl\n";
        m_port = std::to_string(find_port());
        logstream(LOG_DEBUG) << "WebServer: finished starting WebServer::Impl\n";
    }
    ~Impl() {
        logstream(LOG_DEBUG) << "WebServer: destroying WebServer::Impl\n";
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
    return "http://localhost:" + m_impl->m_port + "/vega.html?plot=" + uuid_str;
}
