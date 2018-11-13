/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at
 * https://opensource.org/licenses/BSD-3-Clause
 */
#include <capi/TuriCreate.h>
#include <capi/impl/capi_error_handling.hpp>
#include <capi/impl/capi_initialization_internal.hpp>
#include <capi/impl/capi_wrapper_structs.hpp>
#include <export.hpp>
#include <flexible_type/flexible_type.hpp>
#include <unity/lib/visualization/show.hpp>

static turi::flexible_type optional_str(const char *str) {
    if (str == nullptr) {
        return turi::FLEX_UNDEFINED;
    }
    return turi::flex_string(str);
}

extern "C" {

EXPORT tc_plot* tc_plot_create_1d(const tc_sarray* sa, const char* title,
                                  const char* x_axis_title,
                                  const char* y_axis_title,
                                  const tc_parameters*, tc_error** error) {
    ERROR_HANDLE_START();
    turi::ensure_server_initialized();

  CHECK_NOT_NULL(error, sa, "sarray", NULL);

  std::shared_ptr<turi::model_base> plot =
      sa->value.plot(optional_str(title), optional_str(x_axis_title), optional_str(y_axis_title));
  return new_tc_plot(
      std::dynamic_pointer_cast<turi::visualization::Plot>(plot));

    ERROR_HANDLE_END(error, NULL);
}

EXPORT tc_plot* tc_plot_create_2d(const tc_sarray* sa_x, const tc_sarray* sa_y,
                                  const char* title, const char* x_axis_title,
                                  const char* y_axis_title,
                                  const tc_parameters*, tc_error** error) {
  ERROR_HANDLE_START();
  turi::ensure_server_initialized();

  CHECK_NOT_NULL(error, sa_x, "sarray_x", NULL);
  CHECK_NOT_NULL(error, sa_y, "sarray_y", NULL);

  std::shared_ptr<turi::model_base> plot = turi::visualization::plot(
      sa_x->value, sa_y->value,
      optional_str(x_axis_title), optional_str(y_axis_title), optional_str(title));
  return new_tc_plot(
      std::dynamic_pointer_cast<turi::visualization::Plot>(plot));

  ERROR_HANDLE_END(error, NULL);
}

EXPORT tc_plot* tc_plot_create_sframe_summary(const tc_sframe* sf,
                                              const tc_parameters* params,
                                              tc_error** error) {
    ERROR_HANDLE_START();
    turi::ensure_server_initialized();

    CHECK_NOT_NULL(error, sf, "sframe", NULL);

    std::shared_ptr<turi::model_base> plot = sf->value.plot();
    return new_tc_plot(std::dynamic_pointer_cast<turi::visualization::Plot>(plot));
    
    ERROR_HANDLE_END(error, NULL);
}

EXPORT tc_flexible_type* tc_plot_get_vega_spec(const tc_plot* plot,
                                               const tc_parameters*,
                                               tc_error** error) {
    ERROR_HANDLE_START();
    turi::ensure_server_initialized();

    CHECK_NOT_NULL(error, plot, "plot", NULL);

    std::string vega_spec = plot->value->get_spec();
    return tc_ft_create_from_string(vega_spec.data(), vega_spec.size(), error);

    ERROR_HANDLE_END(error, NULL);
}

EXPORT tc_flexible_type* tc_plot_get_next_data(const tc_plot* plot,
                                               const tc_parameters*,
                                               tc_error** error) {
    ERROR_HANDLE_START();
    turi::ensure_server_initialized();

    CHECK_NOT_NULL(error, plot, "plot", NULL);

    std::string vega_data = plot->value->get_next_data();
    return tc_ft_create_from_string(vega_data.data(), vega_data.size(), error);

    ERROR_HANDLE_END(error, NULL);
}

EXPORT bool tc_plot_finished_streaming(const tc_plot* plot,
                                       const tc_parameters*, tc_error** error) {
    ERROR_HANDLE_START();
    turi::ensure_server_initialized();

    CHECK_NOT_NULL(error, plot, "plot", NULL);
    return plot->value->finished_streaming();

    ERROR_HANDLE_END(error, NULL);
}

EXPORT tc_flexible_type* tc_plot_get_url(const tc_plot* plot, const tc_parameters* params, tc_error** error) {
    ERROR_HANDLE_START();
    turi::ensure_server_initialized();

    CHECK_NOT_NULL(error, plot, "plot", NULL);

    std::string url = plot->value->get_url();
    return tc_ft_create_from_string(url.data(), url.size(), error);

    ERROR_HANDLE_END(error, NULL);
}
}