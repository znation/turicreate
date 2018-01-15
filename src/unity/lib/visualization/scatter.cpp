/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>
#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include "scatter.hpp"

#include "process_wrapper.hpp"
#include "thread.hpp"
#include "vega_spec.hpp"

#include <cmath>
#include <thread>

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

    process_wrapper pw(path_to_client);
    pw << scatter_spec(xlabel, ylabel, title);

    auto data_message = std::make_shared<Message>();
    data_message->mutable_data()->set_name("source_2");
    data_message->mutable_data()->set_progress(1.0);

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

      auto *data = data_message->mutable_data()->mutable_scatter()->add_values();
      data->set_x(x[i]);
      data->set_y(y[i]);
    }

    pw << data_message;

  });

}
