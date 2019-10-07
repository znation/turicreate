/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at
 * https://opensource.org/licenses/BSD-3-Clause
 */

#include <boost/gil/gil_all.hpp>

#include <core/data/image/image_type.hpp>

#include <core/data/sframe/gl_sframe.hpp>
#include <toolkits/object_detection/one_shot_object_detection/util/parameter_sampler.hpp>

namespace turi {
namespace one_shot_object_detection {
namespace data_augmentation {

flex_image create_synthetic_image(
    const boost::gil::rgba8_image_t::const_view_t &starter_image_view,
    const boost::gil::rgb8_image_t::const_view_t &background_view,
    ParameterSampler &parameter_sampler);

}  // namespace data_augmentation
}  // namespace one_shot_object_detection
}  // namespace turi
