/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef TURI_CAPI_WRAPPER_STRUCTS_HPP_
#define TURI_CAPI_WRAPPER_STRUCTS_HPP_

#include <capi/TuriCreate.h>
#include <model_server/lib/variant.hpp>
#include <model_server/lib/variant_converter.hpp>
#include <core/data/flexible_type/flexible_type.hpp>
#include <model_server/lib/extensions/model_base.hpp>
#include <core/data/sframe/gl_sarray.hpp>
#include <core/data/sframe/gl_sframe.hpp>
#include <utility>

struct capi_struct_type_info {
  virtual const char* name() const = 0;
  virtual void free(const void* v) = 0;

  // TODO: memory pool implementations, etc.
};

// Each C API wrapper struct is intended to match a c++ container type that
// allows it to interface to pure C easily.  As such, each wrapper struct has the
// an instance of the wrapping type stored as the value field, and type_info to hold
// some type information associated with it for error checking and, in the 
// future, memory management.
//
// Creation of a wrapping struct should be done with new_* methods.

#define DECLARE_CAPI_WRAPPER_STRUCT(struct_name, wrapping_type)               \
  struct capi_struct_type_info_##struct_name;                                 \
                                                                              \
  /* The typeinfo executor needs to have a singleton instance. */             \
  extern capi_struct_type_info_##struct_name                                  \
      capi_struct_type_info_##struct_name##_inst;                             \
                                                                              \
  extern "C" {                                                                \
                                                                              \
  struct struct_name##_struct {                                               \
    capi_struct_type_info* type_info = nullptr;                               \
    wrapping_type value;                                                      \
  };                                                                          \
                                                                              \
  typedef struct struct_name##_struct struct_name;                            \
  }                                                                           \
                                                                              \
  struct capi_struct_type_info_##struct_name : public capi_struct_type_info { \
    const char* name() const { return #struct_name; }                         \
    void free(const void* v) {                                                \
      if (UNLIKELY(v == nullptr)) {                                           \
        return;                                                               \
      }                                                                       \
      const struct_name* vv = static_cast<const struct_name*>(v);             \
      ASSERT_TRUE(vv->type_info == this);                                     \
      ASSERT_TRUE(this == &(capi_struct_type_info_##struct_name##_inst));     \
      delete vv;                                                              \
    }                                                                         \
  };                                                                          \
                                                                              \
  static inline struct_name* new_##struct_name() {                            \
    struct_name* ret = new struct_name##_struct();                            \
    ret->type_info = &(capi_struct_type_info_##struct_name##_inst);           \
    return ret;                                                               \
  }                                                                           \
                                                                              \
  template <typename... Args>                                                 \
  static inline struct_name* new_##struct_name(Args&&... args) {              \
    struct_name* ret = new_##struct_name();                                   \
    ret->value = wrapping_type(std::forward<Args>(args)...);                  \
    return ret;                                                               \
  }

typedef std::map<std::string, turi::aggregate::groupby_descriptor_type> groupby_aggregator_map_type;

DECLARE_CAPI_WRAPPER_STRUCT(tc_error, std::string);
DECLARE_CAPI_WRAPPER_STRUCT(tc_datetime, turi::flex_date_time);
DECLARE_CAPI_WRAPPER_STRUCT(tc_flex_dict, turi::flex_dict);
DECLARE_CAPI_WRAPPER_STRUCT(tc_flex_list, turi::flex_list);
DECLARE_CAPI_WRAPPER_STRUCT(tc_flex_image, turi::flex_image);
DECLARE_CAPI_WRAPPER_STRUCT(tc_ndarray, turi::flex_nd_vec);
DECLARE_CAPI_WRAPPER_STRUCT(tc_flexible_type, turi::flexible_type);
DECLARE_CAPI_WRAPPER_STRUCT(tc_flex_enum_list, std::vector<turi::flex_type_enum>);
DECLARE_CAPI_WRAPPER_STRUCT(tc_sarray, turi::gl_sarray);
DECLARE_CAPI_WRAPPER_STRUCT(tc_sframe, turi::gl_sframe);
DECLARE_CAPI_WRAPPER_STRUCT(tc_variant, turi::variant_type);
DECLARE_CAPI_WRAPPER_STRUCT(tc_parameters, turi::variant_map_type);
DECLARE_CAPI_WRAPPER_STRUCT(tc_model, std::shared_ptr<turi::model_base>);
DECLARE_CAPI_WRAPPER_STRUCT(tc_groupby_aggregator, groupby_aggregator_map_type);
DECLARE_CAPI_WRAPPER_STRUCT(tc_plot, std::shared_ptr<turi::visualization::Plot>);

#endif
