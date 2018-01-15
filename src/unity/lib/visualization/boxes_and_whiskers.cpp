/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#include "boxes_and_whiskers.hpp"

#include "process_wrapper.hpp"
#include "thread.hpp"
#include "vega_data.hpp"
#include "vega_spec.hpp"

#include <parallel/lambda_omp.hpp>

#include <cmath>
#include <thread>

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>
#include <logger/assertions.hpp>
#include <logger/logger.hpp>

using namespace turi::visualization;

static const std::string x_name = "x";
static const std::string y_name = "y";

std::shared_ptr<Message> boxes_and_whiskers_result::vega_column_data(double progress, bool) const {
  auto ret = std::make_shared<Message>();
  ret->mutable_data()->set_name("source_2");
  ret->mutable_data()->set_progress(progress);
  std::unordered_map<flexible_type, flexible_type> grouped = get_grouped();

  for (const auto& pair : grouped) {
    // if x is missing, nothing to plot -- skip for now
    // TODO: eventually we should probably have an "undefined" bin
    if (pair.first.get_type() == flex_type_enum::UNDEFINED) {
      continue;
    }

    const auto& xValue = pair.first.get<flex_string>();
    const auto& yValues = pair.second.get<flex_vec>();

    // TODO - for now, skip values with nan/inf
    bool isFinite = true;
    for (const auto& yValue : yValues) {
      if (!std::isfinite(yValue)) {
        isFinite = false;
      }
    }
    if (!isFinite) {
      continue;
    }

    auto* data = ret->mutable_data()->mutable_boxes_and_whiskers()->add_values();
    data->set_name(xValue);
    data->set_min(yValues[0]);
    data->set_lower_quartile(yValues[1]);
    data->set_median(yValues[2]);
    data->set_upper_quartile(yValues[3]);
    data->set_max(yValues[4]);
  }

  return ret;
}

void ::turi::visualization::show_boxes_and_whiskers(const std::string& path_to_client,
                                                    const gl_sarray& x,
                                                    const gl_sarray& y,
                                                    const std::string& xlabel,
                                                    const std::string& ylabel,
                                                    const std::string& title) {

  ::turi::visualization::run_thread([path_to_client, x, y, xlabel, ylabel, title]() {

    DASSERT_EQ(x.size(), y.size());


    process_wrapper pw(path_to_client);
    pw << boxes_and_whiskers_spec(xlabel, ylabel, title);

    boxes_and_whiskers bw;

    gl_sframe temp_sf;
    temp_sf[x_name] = x;
    temp_sf[y_name] = y;
    bw.init(temp_sf);
    while (pw.good()) {
      auto result = bw.get();
      double num_rows_processed =  static_cast<double>(bw.get_rows_processed());
      double size_array = static_cast<double>(x.size());
      double percent_complete = num_rows_processed/size_array;
      pw << result->vega_column_data(percent_complete);

      if (bw.eof()) {
        break;
      }
    }

  });

}
