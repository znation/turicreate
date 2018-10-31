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
        class Impl;
        static std::string get_url_for_plot(const Plot& plot);

    private:
        plot_map m_plots = std::make_shared<std::unordered_map<std::string, Plot>>();
        std::unique_ptr<Impl> m_impl;

        WebServer();
        ~WebServer();
        std::string add_plot(const Plot& plot);

    };

}}
