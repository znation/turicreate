/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>

#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include <cmath>
#include <thread>

#include "thread.hpp"
#include "vega_spec.hpp"

#include "process_wrapper.hpp"
#include "scatter.hpp"

using namespace turi;
using namespace turi::visualization;

void turi::visualization::show_scatter(const std::string& path_to_client,
                                       const gl_sarray& x,
                                       const gl_sarray& y,
                                       const std::string& xlabel,
                                       const std::string& ylabel,
                                       const std::string& title) {

  ::turi::visualization::run_thread([path_to_client, x, y, xlabel, ylabel, title]() {

    DASSERT_EQ(x.size(), y.size());

    process_wrapper ew(path_to_client);
    ew << scatter_spec(xlabel, ylabel, title);

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->mutable_data()->set_name("source_2");
    message->mutable_data()->set_progress(1.0);

    for (size_t i=0; i<x.size(); i++) {
      if (x[i].get_type() == flex_type_enum::UNDEFINED ||
          y[i].get_type() == flex_type_enum::UNDEFINED) {
        // TODO: show undefined values
        // for now, just skip them
        continue;
      }

      // for now, skip NaN/inf as well.
      if (x[i].get_type() == flex_type_enum::FLOAT &&
          !std::isfinite(x[i].get<flex_float>())) {
        continue;
      }
      if (y[i].get_type() == flex_type_enum::FLOAT &&
          !std::isfinite(y[i].get<flex_float>())) {
        continue;
      }

      auto* value = message->mutable_data()->mutable_scatter_data()->add_values();
      value->set_x(x[i]);
      value->set_y(y[i]);
    }

    ew << message;

  });

}
