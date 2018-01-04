/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>

#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include "vega_data.hpp"

namespace turi {
namespace visualization {
namespace vega_data {

/*
vega_data::vega_data() {
  m_spec << "{\"data_spec\": {\"name\": \"source_2\", \"values\": [";
}

std::string vega_data::get_data_spec(double progress) {
  m_spec << "], \"progress\": "+std::to_string(progress)+" }}\n";
  return m_spec.str();
}
*/

std::shared_ptr<Message> create_sframe_data(
    size_t i,
    size_t num_rows,
    flex_type_enum type,
    std::string title,
    const std::shared_ptr<transformation_output>& result,
    double percent_complete
) {
  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;

  /*
  // TODO this must be protobuf -> JSON
  ss << "{\"a\": " << std::to_string(i) << ",";
  ss << "\"title\": " << title << ",";
  ss << "\"num_row\": " << num_rows << ",";

  switch (type) {
    case flex_type_enum::INTEGER:
    case flex_type_enum::FLOAT:
    case flex_type_enum::STRING:
    {
      auto h_result = std::dynamic_pointer_cast<sframe_transformation_output>(result);
      ss << h_result->vega_summary_data();
      ss << "}";
      break;
    }
    default:
      throw std::runtime_error("Unexpected dtype. SFrame plot expects int, float or str dtypes.");
  }
  return ss.str();
  */
}

/*
vega_data& vega_data::operator<<(const std::string& vega_string) {
  if (!m_has_spec) {
    m_spec << vega_string;
    m_has_spec = true;
  } else {
    m_spec << "," << vega_string;
  }
  return *this;
}
*/

} // vega_data
} // visualization
} // turi
