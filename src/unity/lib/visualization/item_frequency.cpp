/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>
#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include "item_frequency.hpp"
#include "vega_spec.hpp"

using namespace turi::visualization;

std::vector<item_frequency_result> item_frequency::split_input(size_t num_threads) {
  // TODO - do we need to do anything here? perhaps not.
  return std::vector<item_frequency_result>(num_threads);
}

void item_frequency::merge_results(std::vector<item_frequency_result>& transformers) {
  for (const auto& other : transformers) {
    this->m_transformer->combine(other);
  }
}

void item_frequency_result::add_element_simple(const flexible_type& flex) {
  groupby_operators::frequency_count::add_element_simple(flex);

  /*
   * add element to summary stats
   */
  m_count.add_element_simple(flex);
  m_count_distinct.add_element_simple(flex);
  m_non_null_count.add_element_simple(flex);
}

void item_frequency_result::combine(const group_aggregate_value& other) {
  groupby_operators::frequency_count::combine(other);

  /* combine summary stats */
  auto item_frequency_other = dynamic_cast<const item_frequency_result&>(other);
  m_count.combine(item_frequency_other.m_count);
  m_count_distinct.combine(item_frequency_other.m_count_distinct);
  m_non_null_count.combine(item_frequency_other.m_non_null_count);
}

std::shared_ptr<Message> item_frequency_result::vega_summary_data(double progress, size_t column_idx, const std::string& column_title, size_t num_rows) const {
  std::shared_ptr<Message> ret = vega_column_data(progress, true /*sframe*/);
  flex_int num_missing = m_count.emit() - m_non_null_count.emit();
  auto *data = ret->mutable_data()->mutable_item_frequency();
  data->set_type("str");
  data->set_num_unique(m_count_distinct.emit());
  data->set_num_missing(num_missing);
  data->set_column_idx(column_idx);
  data->set_column_title(column_title);
  data->set_num_rows(num_rows);
  return ret;
}

std::shared_ptr<Message> item_frequency_result::vega_column_data(double progress, bool sframe) const {
  auto ret = std::make_shared<Message>();
  ret->mutable_data()->set_name("source_2");
  ret->mutable_data()->set_progress(progress);

  auto items_list = emit().get<flex_dict>();
  size_t size_list;

  if(sframe){
    size_list = std::min(10UL, items_list.size());
  }else{
    size_list = std::min(200UL, items_list.size());
  }

  std::sort(items_list.begin(), items_list.end(), [](const std::pair<turi::flexible_type,flexible_type> &left, const std::pair<turi::flexible_type,flexible_type> &right) {
    DASSERT_EQ(left.second.get_type(), flex_type_enum::INTEGER);
    DASSERT_EQ(right.second.get_type(), flex_type_enum::INTEGER);

    if (left.second == right.second) {
      // ignore undefined (always sort lower -- it'll get ignored later)
      if (left.first.get_type() == flex_type_enum::UNDEFINED ||
          right.first.get_type() == flex_type_enum::UNDEFINED) {
        return false;
      }

      DASSERT_EQ(left.first.get_type(), flex_type_enum::STRING);
      DASSERT_EQ(right.first.get_type(), flex_type_enum::STRING);

      // if count is equal, sort ascending by label
      return right.first > left.first;
    }
    // sort descending by count
    return left.second > right.second;
  });

  for(size_t i=0; i<size_list; i++) {

    const auto& pair = items_list[i];
    const auto& flex_value = pair.first;
    if (flex_value.get_type() == flex_type_enum::UNDEFINED) {
      // skip missing values for now
      continue;
    }

    DASSERT_TRUE(flex_value.get_type() == flex_type_enum::STRING);
    const auto& value = flex_value.get<flex_string>();

    size_t count = pair.second.get<flex_int>();
    auto* data = ret->mutable_data()->mutable_item_frequency()->add_values();
    data->set_label(value);
    data->set_label_idx(i);
    data->set_count(count);
  }

  return ret;
}
