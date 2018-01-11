/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>

#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include "vega_spec.hpp"

namespace turi {
namespace visualization {

static void apply_padding(double amount, vega::BoundingBox* to) {
  to->set_left(amount);
  to->set_right(amount);
  to->set_top(amount);
  to->set_bottom(amount);
}

std::shared_ptr<Message> histogram_spec(const std::string& title,
                                  const std::string& xlabel,
                                  const std::string& ylabel,
                                  double sizeMultiplier) {
  // TODO: Convert back to Vega-Lite

  size_t width = static_cast<size_t>(600.0 * sizeMultiplier);
  size_t height = static_cast<size_t>(400.0 * sizeMultiplier);

  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;
}

std::shared_ptr<Message> categorical_spec(size_t length_list,
                                    const std::string& title,
                                    const std::string& xlabel,
                                    const std::string& ylabel,
                                    double sizeMultiplier) {

  // TODO: Convert back to Vega-Lite

  size_t width = static_cast<size_t>(600.0 * sizeMultiplier);
  size_t height = static_cast<size_t>(std::max(static_cast<double>(length_list) * 16.0, static_cast<double>(length_list) * 25.0 * sizeMultiplier));

  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;
}


std::shared_ptr<Message> summary_view_spec(size_t length_elements, double sizeMultiplier){
  size_t height = static_cast<size_t>((300.0 * sizeMultiplier * length_elements) + 80.0);

  return "{ \"$schema\":\"https:\/\/vega.github.io\/schema\/vega\/v3.0.json\", \"metadata\":{ \"bubbleOpts\":{ \"showAllFields\":false, \"fields\":[ { \"field\":\"left\" }, { \"field\":\"right\" }, { \"field\":\"count\" }, { \"field\":\"label\" } ] } }, \"width\":800, \"height\":"+std::to_string(height)+", \"padding\":0, \"data\":[ { \"name\":\"pts_store\" }, { \"name\": \"source_2\" }, { \"name\":\"data_2\", \"source\":\"source_2\", \"transform\":[ { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+66\", \"as\":\"c_main_background\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+43\", \"as\":\"c_top_bar\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+59\", \"as\":\"c_top_title\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+58\", \"as\":\"c_top_type\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+178\", \"as\":\"c_rule\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+106\", \"as\":\"c_num_rows\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+130\", \"as\":\"c_num_unique\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+154\", \"as\":\"c_missing\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+105\", \"as\":\"c_num_rows_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+130\", \"as\":\"c_num_unique_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+154\", \"as\":\"c_missing_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+195\", \"as\":\"c_frequent_items\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+218\", \"as\":\"c_first_item\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+235\", \"as\":\"c_second_item\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+252\", \"as\":\"c_third_item\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+269\", \"as\":\"c_fourth_item\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+286\", \"as\":\"c_fifth_item\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+200\", \"as\":\"c_mean\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+220\", \"as\":\"c_min\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+240\", \"as\":\"c_max\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+260\", \"as\":\"c_median\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+280\", \"as\":\"c_stdev\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+198\", \"as\":\"c_mean_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+218\", \"as\":\"c_min_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+238\", \"as\":\"c_max_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+258\", \"as\":\"c_median_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+278\", \"as\":\"c_stdev_val\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+106\", \"as\":\"graph_offset\" }, { \"type\":\"formula\", \"expr\":\"toNumber(datum[\\\"a\\\"])*300+132\", \"as\":\"graph_offset_categorical\" }, { \"type\":\"formula\", \"expr\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?false:true\", \"as\":\"c_clip_val\" }, { \"type\":\"formula\", \"expr\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?250:0\", \"as\":\"c_width_numeric_val\" }, { \"type\":\"formula\", \"expr\":\"(toString(datum[\\\"type\\\"]) == \\\"str\\\")?false:true\", \"as\":\"c_clip_val_cat\" }, { \"type\":\"formula\", \"expr\":\"(toString(datum[\\\"type\\\"]) == \\\"str\\\")?250:0\", \"as\":\"c_width_numeric_val_cat\" } ] } ], \"marks\":[ { \"encode\":{ \"enter\":{ \"x\":{ \"value\":0 }, \"width\":{ \"value\":734 }, \"y\":{ \"value\":0 }, \"height\":{ \"value\":366 }, \"clip\":{ \"value\":0 }, \"fill\":{ \"value\":\"#ffffff\" }, \"fillOpacity\":{ \"value\":0 }, \"stroke\":{ \"value\":\"#000000\" }, \"strokeWidth\":{ \"value\":0 } } }, \"marks\":[ { \"encode\":{ \"enter\":{ \"x\":{ \"value\":0 }, \"width\":{ \"value\":734 }, \"y\":{ \"value\":0 }, \"height\":{ \"value\":366 }, \"clip\":{ \"value\":0 }, \"fill\":{ \"value\":\"#ffffff\" }, \"fillOpacity\":{ \"value\":0 }, \"stroke\":{ \"value\":\"#000000\" }, \"strokeWidth\":{ \"value\":0 } } }, \"scales\":[ ], \"axes\":[ ], \"marks\":[ { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":33 }, \"width\":{ \"value\":700 }, \"y\":{ \"value\":66 }, \"height\":{ \"value\":250 }, \"fill\":{ \"value\":\"#FEFEFE\" }, \"fillOpacity\":{ \"value\":1 }, \"stroke\":{ \"value\":\"#DEDEDE\" }, \"strokeWidth\":{ \"value\":0.5 } }, \"update\":{ \"x\":{ \"value\":33 }, \"y\":{ \"field\":\"c_main_background\" } } }, \"type\":\"rect\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":33 }, \"width\":{ \"value\":700 }, \"y\":{ \"value\":43 }, \"height\":{ \"value\":30 }, \"fill\":{ \"value\":\"#F5F5F5\" }, \"fillOpacity\":{ \"value\":1 }, \"stroke\":{ \"value\":\"#DEDEDE\" }, \"strokeWidth\":{ \"value\":0.5 } }, \"update\":{ \"x\":{ \"value\":33 }, \"y\":{ \"field\":\"c_top_bar\" } } }, \"type\":\"rect\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":720 }, \"y\":{ \"value\":58 }, \"text\":{ \"signal\":\"\'\'+datum[\\\"type\\\"]\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#595859\" } }, \"update\":{ \"y\":{ \"field\":\"c_top_type\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":44 }, \"y\":{ \"value\":59 }, \"text\":{ \"signal\":\"\'\'+datum[\\\"title\\\"]\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":15 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#9B9B9B\" } }, \"update\":{ \"y\":{ \"field\":\"c_top_title\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":178 }, \"stroke\":{ \"value\":\"#EDEDEB\" }, \"strokeWidth\":{ \"value\":1 }, \"strokeCap\":{ \"value\":\"butt\" }, \"x2\":{ \"value\":720 }, \"y2\":{ \"value\":178 } }, \"update\":{ \"y\":{ \"field\":\"c_rule\" }, \"y2\":{ \"field\":\"c_rule\" } } }, \"type\":\"rule\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":106 }, \"text\":{ \"value\":\"Num. Rows:\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_num_rows\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":130 }, \"text\":{ \"value\":\"Num. Unique:\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_num_unique\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":154 }, \"text\":{ \"value\":\"Missing:\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_missing\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":105 }, \"text\":{ \"signal\":\"toString(format(datum[\\\"num_row\\\"], \\\",\\\"))\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#5A5A5A\" } }, \"update\":{ \"y\":{ \"field\":\"c_num_rows_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":130 }, \"text\":{ \"signal\": \"toString(format(datum[\\\"num_unique\\\"], \\\",\\\"))\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#5A5A5A\" } }, \"update\":{ \"y\":{ \"field\":\"c_num_unique_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":154 }, \"text\":{ \"signal\": \"toString(format(datum[\\\"num_missing\\\"], \\\",\\\"))\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":12 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#5A5A5A\" } }, \"update\":{ \"y\":{ \"field\":\"c_missing_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"str\\\")? \\\"Frequent Items\\\":\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"bold\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_frequent_items\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":520 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 1) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][0][\\\"label\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_first_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":520 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 2) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][1][\\\"label\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_second_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":520 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 3) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][2][\\\"label\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_third_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":520 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 4) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][3][\\\"label\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_fourth_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":520 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 5) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][4][\\\"label\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_fifth_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 1) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][0][\\\"count\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#7A7A7A\" } }, \"update\":{ \"y\":{ \"field\":\"c_first_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 2) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][1][\\\"count\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#7A7A7A\" } }, \"update\":{ \"y\":{ \"field\":\"c_second_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 3) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][2][\\\"count\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#7A7A7A\" } }, \"update\":{ \"y\":{ \"field\":\"c_third_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 4) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][3][\\\"count\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#7A7A7A\" } }, \"update\":{ \"y\":{ \"field\":\"c_fourth_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"((datum[\\\"categorical\\\"].length >= 5) && (toString(datum[\\\"type\\\"]) == \\\"str\\\"))? toString(datum[\\\"categorical\\\"][4][\\\"count\\\"]):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#7A7A7A\" } }, \"update\":{ \"y\":{ \"field\":\"c_fifth_item\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":200 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")? \\\"Mean:\\\":\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"clip\":{ \"value\":true }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"bold\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_mean\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":220 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")? \\\"Min:\\\":\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"bold\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_min\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":240 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")? \\\"Max:\\\":\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"bold\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_max\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":260 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")? \\\"Median:\\\":\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"bold\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_median\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":500 }, \"y\":{ \"value\":280 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")? \\\"St. Dev:\\\":\\\"\\\"\" }, \"align\":{ \"value\":\"left\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":11 }, \"fontWeight\":{ \"value\":\"bold\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#4A4A4A\" } }, \"update\":{ \"y\":{ \"field\":\"c_stdev\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":198 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?toString(format(datum[\\\"mean\\\"], \\\",\\\")):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#6A6A6A\" } }, \"update\":{ \"y\":{ \"field\":\"c_mean_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":218 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?toString(format(datum[\\\"min\\\"], \\\",\\\")):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#6A6A6A\" } }, \"update\":{ \"y\":{ \"field\":\"c_min_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":238 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?toString(format(datum[\\\"max\\\"], \\\",\\\")):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#6A6A6A\" } }, \"update\":{ \"y\":{ \"field\":\"c_max_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":258 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?toString(format(datum[\\\"median\\\"], \\\",\\\")):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#6A6A6A\" } }, \"update\":{ \"y\":{ \"field\":\"c_median_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"data\":\"data_2\" }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":700 }, \"y\":{ \"value\":278 }, \"text\":{ \"signal\":\"(toString(datum[\\\"type\\\"]) == \\\"integer\\\" || toString(datum[\\\"type\\\"]) == \\\"float\\\")?toString(format(datum[\\\"stdev\\\"], \\\",\\\")):\\\"\\\"\" }, \"align\":{ \"value\":\"right\" }, \"baseline\":{ \"value\":\"middle\" }, \"dx\":{ \"value\":0, \"offset\":0 }, \"dy\":{ \"value\":0, \"offset\":0 }, \"angle\":{ \"value\":0 }, \"font\":{ \"value\":\"AvenirNext-Medium\" }, \"fontSize\":{ \"value\":10 }, \"fontWeight\":{ \"value\":\"normal\" }, \"fontStyle\":{ \"value\":\"normal\" }, \"fill\":{ \"value\":\"#6A6A6A\" } }, \"update\":{ \"y\":{ \"field\":\"c_stdev_val\" } } }, \"type\":\"text\" }, { \"from\":{ \"facet\":{ \"name\":\"new_data\", \"data\":\"data_2\", \"field\":\"numeric\" } }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":120 }, \"width\":{ \"value\":250 }, \"y\":{ \"field\":\"graph_offset\" }, \"height\":{ \"value\":150 }, \"fill\":{ \"value\":\"#ffffff\" }, \"fillOpacity\":{ \"value\":0 }, \"stroke\":{ \"value\":\"#000000\" }, \"strokeWidth\":{ \"value\":0 } }, \"update\":{ \"clip\":{ \"field\":\"c_clip_val\" }, \"width\":{ \"field\":\"c_width_numeric_val\" } } }, \"type\":\"group\", \"scales\":[ { \"name\":\"x\", \"type\":\"linear\", \"domain\":{ \"data\":\"new_data\", \"fields\":[ \"left\", \"right\" ], \"sort\":true }, \"range\":[ 0, { \"signal\":\"width\" } ], \"nice\":true, \"zero\":true }, { \"name\":\"y\", \"type\":\"linear\", \"domain\":{ \"data\":\"new_data\", \"field\":\"count\" }, \"range\":[ { \"signal\":\"height\" }, 0 ], \"nice\":true, \"zero\":true } ], \"axes\":[ { \"title\":\"Values\", \"scale\":\"x\", \"labelOverlap\":true, \"orient\":\"bottom\", \"tickCount\":{ \"signal\":\"ceil(width\/40)\" }, \"zindex\":1 }, { \"scale\":\"x\", \"domain\":false, \"grid\":true, \"labels\":false, \"maxExtent\":0, \"minExtent\":0, \"orient\":\"bottom\", \"tickCount\":{ \"signal\":\"ceil(width\/40)\" }, \"ticks\":false, \"zindex\":0, \"gridScale\":\"y\" }, { \"title\":\"Count\", \"scale\":\"y\", \"labelOverlap\":true, \"orient\":\"left\", \"tickCount\":{ \"signal\":\"ceil(height\/40)\" }, \"zindex\":1 }, { \"scale\":\"y\", \"domain\":false, \"grid\":true, \"labels\":false, \"maxExtent\":0, \"minExtent\":0, \"orient\":\"left\", \"tickCount\":{ \"signal\":\"ceil(height\/40)\" }, \"ticks\":false, \"zindex\":0, \"gridScale\":\"x\" } ], \"style\":\"cell\", \"signals\":[ { \"name\":\"width\", \"update\":\"250\" }, { \"name\":\"height\", \"update\":\"150\" } ], \"marks\":[ { \"name\":\"marks\", \"type\":\"rect\", \"style\":[ \"rect\" ], \"from\":{ \"data\":\"new_data\" }, \"encode\":{ \"hover\":{ \"fill\":{ \"value\":\"#7EC2F3\" } }, \"update\":{ \"x\":{ \"scale\":\"x\", \"field\":\"left\" }, \"x2\":{ \"scale\":\"x\", \"field\":\"right\" }, \"y\":{ \"scale\":\"y\", \"field\":\"count\" }, \"y2\":{ \"scale\":\"y\", \"value\":0 }, \"fill\":{ \"value\":\"#108EE9\" } } } } ] }, { \"from\":{ \"facet\":{ \"name\":\"data_5\", \"data\":\"data_2\", \"field\":\"categorical\" } }, \"encode\":{ \"enter\":{ \"x\":{ \"value\":170 }, \"width\":{ \"value\":250 }, \"y\":{ \"field\":\"graph_offset_categorical\" }, \"height\":{ \"value\":150 }, \"fill\":{ \"value\":\"#ffffff\" }, \"fillOpacity\":{ \"value\":0 }, \"stroke\":{ \"value\":\"#000000\" }, \"strokeWidth\":{ \"value\":0 } }, \"update\":{ \"clip\":{ \"field\":\"c_clip_val_cat\" }, \"width\":{ \"field\":\"c_width_numeric_val_cat\" } } }, \"type\":\"group\", \"style\":\"cell\", \"signals\":[ { \"name\":\"unit\", \"value\":{ }, \"on\":[ { \"events\":\"mousemove\", \"update\":\"isTuple(group()) ? group() : unit\" } ] }, { \"name\":\"pts\", \"update\":\"data(\\\"pts_store\\\").length && {count: data(\\\"pts_store\\\")[0].values[0]}\" }, { \"name\":\"pts_tuple\", \"value\":{ }, \"on\":[ { \"events\":[ { \"source\":\"scope\", \"type\":\"click\" } ], \"update\":\"datum && item().mark.marktype !== \'group\' ? {unit: \\\"\\\", encodings: [\\\"x\\\"], fields: [\\\"count\\\"], values: [datum[\\\"count\\\"]]} : null\", \"force\":true } ] }, { \"name\":\"pts_modify\", \"on\":[ { \"events\":{ \"signal\":\"pts_tuple\" }, \"update\":\"modify(\\\"pts_store\\\", pts_tuple, true)\" } ] } ], \"marks\":[ { \"name\":\"marks\", \"type\":\"rect\", \"style\":[ \"bar\" ], \"from\":{ \"data\":\"data_5\" }, \"encode\":{ \"hover\":{ \"fill\":{ \"value\":\"#7EC2F3\" } }, \"update\":{ \"x\":{ \"scale\":\"x\", \"field\":\"count\" }, \"x2\":{ \"scale\":\"x\", \"value\":0 }, \"y\":{ \"scale\":\"y\", \"field\":\"label\" }, \"height\":{ \"scale\":\"y\", \"band\":true }, \"fill\":{ \"value\":\"#108EE9\" } } } } ], \"scales\":[ { \"name\":\"x\", \"type\":\"linear\", \"domain\":{ \"data\":\"data_5\", \"field\":\"count\" }, \"range\":[ 0, 250 ], \"nice\":true, \"zero\":true }, { \"name\":\"y\", \"type\":\"band\", \"domain\":{ \"data\":\"data_5\", \"field\":\"label\", \"sort\":{ \"op\":\"mean\", \"field\":\"label_idx\", \"order\":\"descending\" } }, \"range\":[ 150, 0 ], \"paddingInner\":0.1, \"paddingOuter\":0.05 } ], \"axes\":[ { \"orient\":\"top\", \"scale\":\"x\", \"labelOverlap\":true, \"tickCount\":{ \"signal\":\"ceil(width\/40)\" }, \"title\":\"Count\", \"zindex\":1 }, { \"orient\":\"top\", \"scale\":\"x\", \"domain\":false, \"grid\":true, \"labels\":false, \"maxExtent\":0, \"minExtent\":0, \"tickCount\":{ \"signal\":\"ceil(width\/40)\" }, \"ticks\":false, \"zindex\":0, \"gridScale\":\"y\" }, { \"scale\":\"y\", \"labelOverlap\":true, \"orient\":\"left\", \"title\":\"Label\", \"zindex\":1 } ] } ], \"type\":\"group\" } ], \"type\":\"group\" } ], \"config\":{ \"axis\":{ \"labelFont\":\"HelveticaNeue-Light\", \"labelFontSize\":7, \"labelPadding\":10, \"labelColor\":\"#595959\", \"titleFont\":\"HelveticaNeue-Light\", \"titleFontWeight\":\"normal\", \"titlePadding\":9, \"titleFontSize\":12, \"titleColor\":\"#595959\" }, \"axisY\":{ \"minExtent\":30 }, \"style\":{ \"rect\":{ \"stroke\":\"rgba(200, 200, 200, 0.5)\" }, \"group-title\":{ \"fontSize\":20, \"font\":\"HelveticaNeue-Light\", \"fontWeight\":\"normal\", \"fill\":\"#595959\" } } } }";
}

std::shared_ptr<Message> scatter_spec(std::string x_name, std::string y_name, std::string title_name) {
  if (x_name.empty()) {
    x_name = "X";
  }
  if (y_name.empty()) {
    y_name = "Y";
  }
  if (title_name.empty()) {
    title_name = x_name + " vs " + y_name;
  }

  std::shared_ptr<Message> ret = std::make_shared<Message>();
  auto* spec = ret->mutable_spec()->mutable_vega_spec();
  spec->set_autosize_str("pad");
  apply_padding(5, spec->mutable_padding());
  spec->set_width(640);
  spec->set_height(400);
  spec->mutable_title()->set_text(title_name);
  spec->mutable_title()->set_offset(30);
  spec->add_data()->set_name("source_2");
  {
    // marks
    auto* mark = spec->add_marks();
    mark->set_name("marks");
    mark->set_type("symbol");
    mark->add_style("circle");
    mark->mutable_from()->set_data("source_2");
    {
      auto* encode = mark->mutable_encode();
      auto* fillValue = (*encode)["hover"].mutable_fill()->mutable_valuereference();
      fillValue->mutable_value()->set_string_value("#7EC2F3");
      auto& update = (*encode)["update"];
      update.mutable_x()->mutable_valuereference()->set_field("x");
      update.mutable_x()->mutable_valuereference()->set_scale("x");
      update.mutable_y()->mutable_valuereference()->set_field("y");
      update.mutable_y()->mutable_valuereference()->set_scale("y");
      update.mutable_fill()->mutable_valuereference()->mutable_value()->set_string_value("#108EE9");
      update.mutable_shape()->mutable_valuereference()->mutable_value()->set_string_value("circle");
      update.mutable_opacity()->mutable_valuereference()->mutable_value()->set_number_value(0.7);
    }
  }
  {
    // scales
    auto* scaleX = spec->add_scales();
    scaleX->set_name("x");
    scaleX->set_type("linear");
    scaleX->mutable_domaindatareference()->set_data("source_2");
    scaleX->mutable_domaindatareference()->add_field("x");
    {
      auto* begin = scaleX->mutable_rangeliteral()->add_values();
      begin->set_number_value(0);
      auto* end = scaleX->mutable_rangeliteral()->add_values();
      auto* fields = end->mutable_struct_value()->mutable_fields();
      (*fields)["signal"].set_string_value("width");
    }
    scaleX->set_nicebool(true);
    scaleX->set_zero(true);

    auto* scaleY = spec->add_scales();
    scaleY->set_name("y");
    scaleY->set_type("linear");
    scaleY->mutable_domaindatareference()->set_data("source_2");
    scaleY->mutable_domaindatareference()->add_field("y");
    {
      auto* begin = scaleY->mutable_rangeliteral()->add_values();
      auto* fields = begin->mutable_struct_value()->mutable_fields();
      (*fields)["signal"].set_string_value("width");
      auto* end = scaleY->mutable_rangeliteral()->add_values();
      end->set_number_value(0);
    }
    scaleY->set_nicebool(true);
    scaleY->set_zero(true);
  }
  {
    // axes
    {
      auto* axisX1 = spec->add_axes();
      axisX1->set_scale("x");
      axisX1->set_labeloverlapflag(true);
      axisX1->set_orient("bottom");
      auto* fields = axisX1->mutable_tickcount()->mutable_struct_value()->mutable_fields();
      (*fields)["signal"].set_string_value("ceil(width/40)");
      axisX1->set_title(title_name);
      axisX1->set_zindex(1);
    }
    {
      auto* axisX2 = spec->add_axes();
      axisX2->set_domain(false);
      axisX2->set_grid(true);
      axisX2->set_labels(false);
      axisX2->set_maxextent(0);
      axisX2->set_minextent(0);
      axisX2->set_orient("bottom");
      auto* fields = axisX1->mutable_tickcount()->mutable_struct_value()->mutable_fields();
      (*fields)["signal"].set_string_value("ceil(width/40)");
      axisX2->set_ticks(false);
      axisX2->set_zindex(0);
    }
  }
  return ret;
}

std::shared_ptr<Message> heatmap_spec(const std::string& x_name, const std::string& y_name, const std::string& title_name) {
  std::string escapedX = x_name;
  if (escapedX.empty()) {
    escapedX = "X";
  }
  std::string escapedY = y_name;
  if (escapedY.empty()) {
    escapedY = "Y";
  }
  std::string escapedTitle = title_name;
  if (escapedTitle.empty()) {
    escapedTitle = x_name + " vs " + y_name;
  }

  // HACK: Initialized with data to correctly render vega spec
  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;
}

std::shared_ptr<Message> categorical_heatmap_spec(const std::string& x_name, const std::string& y_name, const std::string& title_name) {
  std::string escapedX = x_name;
  if (escapedX.empty()) {
    escapedX = "X";
  }
  std::string escapedY = y_name;
  if (escapedY.empty()) {
    escapedY = "Y";
  }
  std::string escapedTitle = title_name;
  if (escapedTitle.empty()) {
    escapedTitle = x_name + " vs " + y_name;
  }

  // HACK: Initialized with data to correctly render vega spec
  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;
}

std::shared_ptr<Message> boxes_and_whiskers_spec(const std::string& x_name, const std::string& y_name, const std::string& title_name) {
  std::string escapedX = x_name;
  if (escapedX.empty()) {
    escapedX = "X";
  }
  std::string escapedY = y_name;
  if (escapedY.empty()) {
    escapedY = "Y";
  }
  std::string escapedTitle = title_name;
  if (escapedTitle.empty()) {
    escapedTitle = x_name + " vs " + y_name;
  }
  return "{\"$schema\":\"https://vega.github.io/schema/vega/v3.0.json\",\"autosize\":\"pad\",\"metadata\": {\"bubbleOpts\":{\"showAllFields\": false, \"fields\": [{\"field\": \"x\"},{\"field\": \"min\"},{\"field\": \"lower quartile\"},{\"field\": \"median\"},{\"field\": \"upper quartile\"},{\"field\": \"max\"}]}},\"title\":{\"text\":" + escapedTitle + ",\"offset\":30},\"padding\":5,\"height\":480,\"style\":\"cell\",\"data\":[{\"name\":\"source_2\"}],\"signals\":[{\"name\":\"x_step\",\"value\":42},{\"name\":\"width\",\"update\":\"bandspace(domain('x').length, 0.1, 0.1) * x_step\"}],\"marks\":[{\"name\":\"layer_0_marks\",\"type\":\"rule\",\"style\":[\"boxWhisker\"],\"from\":{\"data\":\"source_2\"},\"encode\":{\"update\":{\"x\":{\"scale\":\"x\",\"field\":\"x\",\"band\":0.5},\"y\":{\"scale\":\"y\",\"field\":\"min\"},\"y2\":{\"scale\":\"y\",\"field\":\"lower quartile\"},\"stroke\":{\"value\":\"black\"}}}},{\"name\":\"layer_1_marks\",\"type\":\"rule\",\"style\":[\"boxWhisker\"],\"from\":{\"data\":\"source_2\"},\"encode\":{\"update\":{\"x\":{\"scale\":\"x\",\"field\":\"x\",\"band\":0.5},\"y\":{\"scale\":\"y\",\"field\":\"upper quartile\"},\"y2\":{\"scale\":\"y\",\"field\":\"max\"},\"stroke\":{\"value\":\"black\"}}}},{\"name\":\"layer_2_marks\",\"type\":\"rect\",\"style\":[\"box\"],\"from\":{\"data\":\"source_2\"},\"encode\":{\"hover\": {\"fill\": {\"value\": \"#7EC2F3\"}}, \"update\":{\"xc\":{\"scale\":\"x\",\"field\":\"x\",\"band\":0.5},\"width\":{\"value\":20},\"y\":{\"scale\":\"y\",\"field\":\"lower quartile\"},\"y2\":{\"scale\":\"y\",\"field\":\"upper quartile\"},\"fill\":{\"value\":\"#108EE9\"}}}},{\"name\":\"layer_3_marks\",\"type\":\"rect\",\"style\":[\"boxMid\"],\"from\":{\"data\":\"source_2\"},\"encode\":{\"update\":{\"xc\":{\"scale\":\"x\",\"field\":\"x\",\"band\":0.5},\"yc\":{\"scale\":\"y\",\"field\":\"median\"},\"width\":{\"value\":20},\"height\":{\"value\":1},\"fill\":{\"value\":\"black\"},\"opacity\":{\"value\":0.7}}}}],\"scales\":[{\"name\":\"x\",\"type\":\"band\",\"domain\":{\"fields\":[{\"data\":\"source_2\",\"field\":\"x\"}]},\"range\":{\"step\":{\"signal\":\"x_step\"}},\"paddingInner\":0.1,\"paddingOuter\":0.05},{\"name\":\"y\",\"type\":\"linear\",\"domain\":{\"fields\":[{\"data\":\"source_2\",\"field\":\"min\"},{\"data\":\"source_2\",\"field\":\"lower quartile\"},{\"data\":\"source_2\",\"field\":\"upper quartile\"},{\"data\":\"source_2\",\"field\":\"max\"},{\"data\":\"source_2\",\"field\":\"lower quartile\"},{\"data\":\"source_2\",\"field\":\"upper quartile\"},{\"data\":\"source_2\",\"field\":\"median\"}],\"sort\":true},\"range\":[{\"signal\":\"height\"},0],\"nice\":true,\"zero\":true}],\"axes\":[{\"scale\":\"x\",\"labelOverlap\":true,\"orient\":\"bottom\",\"title\":" + escapedX + ",\"zindex\":1,\"encode\":{\"labels\":{\"update\":{\"angle\":{\"value\":270},\"align\":{\"value\":\"right\"},\"baseline\":{\"value\":\"middle\"}}}}},{\"title\":" + escapedY + ",\"scale\":\"y\",\"labelOverlap\":true,\"orient\":\"left\",\"tickCount\":{\"signal\":\"ceil(height/40)\"},\"zindex\":1},{\"scale\":\"y\",\"domain\":false,\"grid\":true,\"labels\":false,\"maxExtent\":0,\"minExtent\":0,\"orient\":\"left\",\"tickCount\":{\"signal\":\"ceil(height/40)\"},\"ticks\":false,\"zindex\":0,\"gridScale\":\"x\"}],\"config\":{\"axis\":{\"labelFont\":\"HelveticaNeue\",\"labelFontSize\":12,\"labelPadding\":10,\"labelColor\":\"rgba(0,0,0,0.65)\",\"titleFont\":\"HelveticaNeue-Medium\",\"titleFontWeight\":\"normal\",\"titlePadding\":30,\"titleFontSize\":15,\"titleColor\":\"rgba(0,0,0,0.65)\"},\"axisY\":{\"minExtent\":30},\"style\":{\"rect\":{\"stroke\":\"rgba(200, 200, 200, 0.5)\"},\"group-title\":{\"fontSize\":29,\"font\":\"HelveticaNeue\",\"fontWeight\":\"normal\",\"fill\":\"rgba(0,0,0,0.65)\"}}}}";
}

vega_spec::vega_spec() {
  m_spec << "{\"vega_spec\":";
}

vega_spec::~vega_spec() {
}

std::string vega_spec::get_spec() {
  m_spec << "}\n";
  return m_spec.str();
}

vega_spec& vega_spec::operator<<(const std::string& vega_string) {
  m_spec << vega_string;
  return *this;
}

} // visualization
} // turi
