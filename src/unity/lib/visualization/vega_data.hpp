/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef __TC_VEGA_DATA
#define __TC_VEGA_DATA

#include "histogram.hpp"
#include "transformation.hpp"

namespace turi {
  namespace visualization {
    class FlexibleType;
    class FlexImage;

    struct QueuedImage {
      size_t idx;
      std::string column;
      flex_image img;
    };

    void serialize_flex_type(const flexible_type& from, visualization::FlexibleType* to);
    void serialize_flex_type(const flex_image& from, visualization::FlexImage* to);
  }
}

#endif
