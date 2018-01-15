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

#include <unity/lib/image_util.hpp>

// resize string for table view
const static size_t resize_table_view = 200;

namespace turi {
namespace visualization {

void serialize_flex_type(const flexible_type& value, visualization::FlexibleType* out) {

  switch (value.get_type()) {
    case flex_type_enum::UNDEFINED:
      out->mutable_undefined_value();
      break;
    case flex_type_enum::FLOAT:
      out->set_float_value(value.get<flex_float>());
      break;
    case flex_type_enum::INTEGER:
      out->set_int_value(value.get<flex_int>());
      break;
    case flex_type_enum::IMAGE:
      {
        const size_t resized_height = 40;

        // resize to a thumbnail for immediate send
        // the user can see this one directly in the table
        flex_image img_temporary = value.get<flex_image>();
        double image_ratio = ((img_temporary.m_width*1.0)/(img_temporary.m_height*1.0));
        double calculated_width = (image_ratio * resized_height);
        size_t resized_width = static_cast<int>(calculated_width);
        flex_image img = turi::image_util::resize_image(img_temporary,
                resized_width, resized_height, img_temporary.m_channels, img_temporary.is_decoded());
        img = turi::image_util::encode_image(img);
        serialize_flex_type(img, out->mutable_image_value());
      }
      break;
    case flex_type_enum::DATETIME:
      {
        const auto& dt = value.get<flex_date_time>();
        out->mutable_datetime_value()->set_posix_timestamp(dt.posix_timestamp());
        out->mutable_datetime_value()->set_tz_15min_offset(dt.time_zone_offset());
        out->mutable_datetime_value()->set_microsecond(dt.microsecond());
      }
      break;
    case flex_type_enum::VECTOR:
      {
        auto* vec_data = out->mutable_array_value();
        for (double d : value.get<flex_vec>()) {
          vec_data->add_values(d);
        }
      }
      break;
    case flex_type_enum::LIST:
      {
        auto* list_data = out->mutable_list_value();
        for (const flexible_type& t : value.get<flex_list>()) {
          auto* list_out = list_data->add_values();
          // recursively serialize each flexible_type within the list
          serialize_flex_type(t, list_out);
        }
      }
      break;
    case flex_type_enum::STRING:
      {
        std::string truncated = value.to<std::string>();
        if(truncated.length() > resize_table_view){
          truncated.resize(resize_table_view);
        }
        out->set_str_value(truncated);
      }
      break;
    default:
      {
        std::stringstream ss;
        ss << "Unsupported dtype "
           << flex_type_enum_to_name(value.get_type())
           << "in SFrame.explore";
        throw std::runtime_error(ss.str());
      }
      break;
  }

}

void serialize_flex_type(const flex_image& img, visualization::FlexImage* out) {
  const unsigned char * image_data = img.get_image_data();
  size_t image_data_size = img.m_image_data_size;

  out->set_width(img.m_width);
  out->set_height(img.m_height);
  out->set_data(image_data, image_data_size);
  out->set_format(static_cast<visualization::FlexImage::Format>(img.m_format));
}

} // visualization
} // turi
