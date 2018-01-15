/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#include "categorical_heatmap.hpp"

#include "process_wrapper.hpp"
#include "thread.hpp"
#include "vega_data.hpp"
#include "vega_spec.hpp"

#include <parallel/lambda_omp.hpp>

#include <thread>

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>
#include <logger/assertions.hpp>
#include <logger/logger.hpp>

using namespace turi::visualization;

std::vector<categorical_heatmap_result> categorical_heatmap::split_input(size_t num_threads) {
  // TODO - do we need to do anything here? perhaps not.
  return std::vector<categorical_heatmap_result>(num_threads);
}

void categorical_heatmap::merge_results(std::vector<categorical_heatmap_result>& transformers) {
  for (const auto& other : transformers) {
    this->m_transformer->combine(other);
  }
}

std::shared_ptr<Message> categorical_heatmap_result::vega_column_data(double progress, bool) const {
  auto ret = std::make_shared<Message>();
  ret->mutable_data()->set_name("source_2");
  ret->mutable_data()->set_progress(progress);
  auto items_list = emit().get<flex_dict>();

  for (auto pair : items_list) {

    const flex_list& values = pair.first.get<flex_list>();

    // for now, skip undefined
    if (values[0].get_type() == flex_type_enum::UNDEFINED ||
        values[1].get_type() == flex_type_enum::UNDEFINED) {
      continue;
    }

    const flex_string& xValue = values[0].get<flex_string>();
    const flex_string& yValue = values[1].get<flex_string>();
    flex_int count = pair.second.get<flex_int>();

    auto* data = ret->mutable_data()->mutable_categorical_heatmap()->add_values();
    data->set_x(xValue);
    data->set_y(yValue);
    data->set_count(count);
  }

  return ret;
}


void ::turi::visualization::show_categorical_heatmap(const std::string& path_to_client,
                                                      const gl_sarray& x,
                                                      const gl_sarray& y,
                                                      const std::string& xlabel,
                                                      const std::string& ylabel,
                                                      const std::string& title) {

  ::turi::visualization::run_thread([path_to_client, x, y, xlabel, ylabel, title]() {

    DASSERT_EQ(x.size(), y.size());

    process_wrapper ew(path_to_client);
    ew << categorical_heatmap_spec(xlabel, ylabel, title);

    categorical_heatmap hm;

    gl_sframe temp_sf;
    temp_sf["x"] = x;
    temp_sf["y"] = y;
    hm.init(temp_sf);
    while (ew.good()) {
      auto result = hm.get();
      double num_rows_processed =  static_cast<double>(hm.get_rows_processed());
      double size_array = static_cast<double>(x.size());
      double percent_complete = num_rows_processed/size_array;
      ew << result->vega_column_data(percent_complete);

      if (hm.eof()) {
        break;
      }
    }

  });

}
