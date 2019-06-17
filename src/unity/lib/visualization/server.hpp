/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#include <unity/lib/visualization/plot.hpp>

#include <memory>

namespace turi {
namespace visualization {

    class WebServer {
    public:
        typedef std::shared_ptr<std::unordered_map<std::string, Plot>> plot_map;

        static WebServer& get_instance();
        ~WebServer();

        // Generates and returns the URL to a given Plot object.
        // Spins up the web server lazily, if needed.
        static std::string get_url_for_plot(const Plot& plot);

        // Sets the static URL path for the web server. Files underneath this
        // directory will be served as static content from the HTTP root.
        // Typically this is set to a directory containing an index.html.
        static void set_static_url_directory(const std::string& directory);

    private:
        WebServer();

        class Impl;
        plot_map m_plots = std::make_shared<std::unordered_map<std::string, Plot>>();
        std::unique_ptr<Impl> m_impl;
        std::string m_static_url_directory;

        std::string add_plot(const Plot& plot);

    };

}}
