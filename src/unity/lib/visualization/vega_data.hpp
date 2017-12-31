/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef __TC_VEGA_DATA
#define __TC_VEGA_DATA

#include <string>
#include <flexible_type/flexible_type.hpp>
#include <unity/lib/visualization/transformation.hpp>

namespace turi {
  namespace visualization {
    namespace vega_data {

      struct Image {
        size_t idx;
        std::string column;
        flex_image img;
      };

      std::shared_ptr<Message> create_sframe_spec(
        size_t i,
        size_t num_rows,
        flex_type_enum type,
        std::string title,
        const std::shared_ptr<transformation_output>& result,
        double percent_complete
      );

    };
  }
}

#endif
