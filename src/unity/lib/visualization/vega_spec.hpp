/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef __TC_VEGA_SPEC
#define __TC_VEGA_SPEC

#include <string>
#include <sstream>

namespace turi {
  namespace visualization {
    class Message;
    std::shared_ptr<Message> histogram_spec(const std::string& title, const std::string& xlabel, const std::string& ylabel, double sizeMultiplier = 1.0);
    std::shared_ptr<Message> categorical_spec(size_t length_list, const std::string& title, const std::string& xlabel, const std::string& ylabel, double sizeMultiplier = 1.0);
    std::shared_ptr<Message> summary_view_spec(size_t length_elements, double sizeMultiplier = 1.0);
    std::shared_ptr<Message> scatter_spec(std::string x_name = "", std::string y_name = "", std::string title_name = "");
    std::shared_ptr<Message> heatmap_spec(const std::string& x_name = "", const std::string& y_name = "", const std::string& title_name = "");
    std::shared_ptr<Message> categorical_heatmap_spec(const std::string& x_name = "", const std::string& y_name = "", const std::string& title_name = "");
    std::shared_ptr<Message> boxes_and_whiskers_spec(const std::string& x_name = "", const std::string& y_name = "", const std::string& title_name = "");
  }
}

#endif // __TC_VEGA_SPEC
