/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef __TC_ITEM_FREQUENCY
#define __TC_ITEM_FREQUENCY

#include <unity/lib/gl_sarray.hpp>
#include <sframe/groupby_aggregate_operators.hpp>

#include "transformation.hpp"

namespace turi {
namespace visualization {

class item_frequency_result: public sframe_transformation_output,
                             public ::turi::groupby_operators::frequency_count {
  public:
    virtual void add_element_simple(const flexible_type& flex) override;
    virtual void combine(const group_aggregate_value& other) override;
    virtual std::shared_ptr<Message> vega_column_data(double progress, bool sframe = false) const override;
    virtual std::shared_ptr<Message> vega_summary_data(double progress, size_t column_idx, const std::string& column_title, size_t num_rows) const override;

    // also store and compute basic summary stats
    groupby_operators::count m_count; // num rows
    groupby_operators::count_distinct m_count_distinct; // num unique
    groupby_operators::non_null_count m_non_null_count; // (inverse) num missing
};

typedef transformation<gl_sarray, item_frequency_result, 5000000> item_frequency_parent;

class item_frequency : public item_frequency_parent {
  public:
    virtual std::vector<item_frequency_result> split_input(size_t num_threads) override;
    virtual void merge_results(std::vector<item_frequency_result>& transformers) override;
};

}} // turi::visualization

#endif // __TC_ITEM_FREQUENCY
