/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef __TC_BOXES_AND_WHISKERS
#define __TC_BOXES_AND_WHISKERS

#include <unity/lib/gl_sframe.hpp>
#include <sframe/groupby_aggregate_operators.hpp>

#include "groupby.hpp"
#include "transformation.hpp"

namespace turi {
namespace visualization {

class boxes_and_whiskers_result: public transformation_output,
                                 public groupby_quantile_result {
  public:
    virtual std::shared_ptr<Message> vega_column_data(double progress, bool sframe = false) const override;
};

// expects a gl_sframe of:
// "x": str,
// "y": int/float
class boxes_and_whiskers : public groupby<boxes_and_whiskers_result> {
};

// expects x to be str, y to be int/float
void show_boxes_and_whiskers(const std::string& path_to_client,
                              const gl_sarray& x,
                              const gl_sarray& y,
                              const std::string& xlabel,
                              const std::string& ylabel,
                              const std::string& title);

}} // turi::visualization

#endif // __TC_BOXES_AND_WHISKERS
