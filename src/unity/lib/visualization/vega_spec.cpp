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
  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;
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
  std::shared_ptr<Message> ret = std::make_shared<Message>();
  DASSERT_TRUE(false);
  return ret;
}

} // visualization
} // turi
