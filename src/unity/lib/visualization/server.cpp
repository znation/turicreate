/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#include <unity/lib/visualization/server.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace http = boost::network::http;
using namespace turi::visualization;

std::string WebServer::get_url_for_plot(const Plot& plot) {
    static WebServer server;
    return server.add_plot(plot);
}

std::string WebServer::add_plot(const Plot& plot) {
    // make UUID for plot
    static auto uuid_generator = boost::uuids::random_generator();
    auto uuid = boost::lexical_cast<std::string>(uuid_generator());

    // add to dictionary with UUID
    m_plots[uuid] = plot;

    // return formatted URL
    return "http://localhost:8000/vega.html?plot=" + uuid;
}
