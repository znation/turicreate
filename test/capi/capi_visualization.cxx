/* Copyright © 2018 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#define BOOST_TEST_MODULE
#include <boost/test/unit_test.hpp>
#include <util/test_macros.hpp>

#include <capi/TuriCreate.h>
#include "capi_utils.hpp"
#include <unity/lib/visualization/plot.hpp>
#include <unity/lib/visualization/show.hpp>

using namespace turi;
using namespace turi::visualization;

/*
 * Test streaming visualization C API
 * High level goal: ensure the same output between the C API
 *                  and the equivalent C++ Plot object method(s).
 * Non-goal (for this test): ensure that the resulting values are correct.
 * C++ unit tests should cover the correctness of the results.
 */
class capi_test_visualization {
    private:
        tc_sarray* m_sa_int;
        tc_sarray* m_sa_float;
        tc_sarray* m_sa_str;

    public:
        capi_test_visualization() {
            tc_error* error = NULL;
            tc_flex_list* fl = NULL;

            std::vector<int64_t> v = {0,1,2,3,4,5};
            fl = make_flex_list_int(v);
            m_sa_int = tc_sarray_create_from_list(fl, &error);
            CAPI_CHECK_ERROR(error);

            std::vector<double> v2 = {0.0, 0.8, -1.0, -0.4, 1.0};
            fl = make_flex_list_double(v2);
            m_sa_float = tc_sarray_create_from_list(fl, &error);
            CAPI_CHECK_ERROR(error);

            std::vector<std::string> v3 = {"foo", "bar", "baz", "qux", "baz", "baz"};
            fl = make_flex_list_string(v3);
            m_sa_str = tc_sarray_create_from_list(fl, &error);
            CAPI_CHECK_ERROR(error);
        }

        ~capi_test_visualization() {
            tc_release(m_sa_int);
            tc_release(m_sa_float);
            tc_release(m_sa_str);
        }

        void test_1d_plots() {

            // numeric histogram (int)
            std::shared_ptr<model_base> expected_obj_base = m_sa_int->value.plot("", "foo", "bar", "baz");
            std::shared_ptr<Plot> expected_obj = std::dynamic_pointer_cast<Plot>(expected_obj_base);
            std::string expected_spec = expected_obj->get_spec();
            tc_error *error = nullptr;
            tc_plot *actual_obj = tc_plot_create_1d(m_sa_int, "foo", "bar", "baz", nullptr, &error);
            CAPI_CHECK_ERROR(error);
            tc_flexible_type *actual_spec_ft = tc_plot_get_vega_spec(actual_obj, nullptr, &error);
            CAPI_CHECK_ERROR(error);
            const char *actual_spec_data = tc_ft_string_data(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            size_t actual_spec_length = tc_ft_string_length(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            std::string actual_spec(actual_spec_data, actual_spec_length);
            TS_ASSERT_EQUALS(actual_spec, expected_spec);
            while (!expected_obj->finished_streaming()) {
                TS_ASSERT_EQUALS(tc_plot_finished_streaming(actual_obj, &error), false);
                CAPI_CHECK_ERROR(error);
                std::string expected_data = expected_obj->get_next_data();
                tc_flexible_type *actual_data_ft = tc_plot_get_next_data(actual_obj, &error);
                CAPI_CHECK_ERROR(error);
                const char *actual_data_data = tc_ft_string_data(actual_data_ft, &error);
                CAPI_CHECK_ERROR(error);
                size_t actual_data_length = tc_ft_string_length(actual_data_ft, &error);
                CAPI_CHECK_ERROR(error);
                std::string actual_data(actual_data_data, actual_data_length);
                TS_ASSERT_EQUALS(actual_data, expected_data);
            }
            TS_ASSERT_EQUALS(tc_plot_finished_streaming(actual_obj, &error), true);
            CAPI_CHECK_ERROR(error);

            // numeric histogram (float)
            expected_obj_base = m_sa_float->value.plot("", "foo", "bar", "baz");
            expected_obj = std::dynamic_pointer_cast<Plot>(expected_obj_base);
            expected_spec = expected_obj->get_spec();
            error = nullptr;
            actual_obj = tc_plot_create_1d(m_sa_float, "foo", "bar", "baz", &error);
            CAPI_CHECK_ERROR(error);
            actual_spec_ft = tc_plot_get_vega_spec(actual_obj, &error);
            CAPI_CHECK_ERROR(error);
            actual_spec_data = tc_ft_string_data(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            actual_spec_length = tc_ft_string_length(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            actual_spec = std::string(actual_spec_data, actual_spec_length);
            TS_ASSERT_EQUALS(actual_spec, expected_spec);

            // categorical histogram (str)
            expected_obj_base = m_sa_str->value.plot("", "foo", "bar", "baz");
            expected_obj = std::dynamic_pointer_cast<Plot>(expected_obj_base);
            expected_spec = expected_obj->get_spec();
            error = nullptr;
            actual_obj = tc_plot_create_1d(m_sa_str, "foo", "bar", "baz", &error);
            CAPI_CHECK_ERROR(error);
            actual_spec_ft = tc_plot_get_vega_spec(actual_obj, &error);
            CAPI_CHECK_ERROR(error);
            actual_spec_data = tc_ft_string_data(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            actual_spec_length = tc_ft_string_length(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            actual_spec = std::string(actual_spec_data, actual_spec_length);
            TS_ASSERT_EQUALS(actual_spec, expected_spec);

        }

        void test_2d_plots() {

            // numeric x numeric small (scatter plot)
            /*
            std::shared_ptr<model_base> expected_obj_base = plot("", m_sa_int->value, m_sa_float->value, "foo", "bar", "baz");
            std::shared_ptr<Plot> expected_obj = std::dynamic_pointer_cast<Plot>(expected_obj_base);
            std::string expected_spec = expected_obj->get_spec();
            tc_error *error = nullptr;
            tc_plot *actual_obj = tc_plot_create_1d(m_sa_int, "foo", "bar", "baz", &error);
            CAPI_CHECK_ERROR(error);
            tc_flexible_type *actual_spec_ft = tc_plot_get_vega_spec(actual_obj, &error);
            CAPI_CHECK_ERROR(error);
            const char *actual_spec_data = tc_ft_string_data(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            size_t actual_spec_length = tc_ft_string_length(actual_spec_ft, &error);
            CAPI_CHECK_ERROR(error);
            std::string actual_spec(actual_spec_data, actual_spec_length);
            TS_ASSERT_EQUALS(actual_spec, expected_spec);
            */
            

            // numeric x numeric large (continuous heat map)
            // TODO test this case?
            // It may not be worth unit testing here since the data must be
            // relatively large.

            // numeric x categorical (boxes and whiskers)

            // categorical x categorical (discrete heat map)

            // 
        }

        void test_sframe_summary_plot() {
        }
};

BOOST_FIXTURE_TEST_SUITE(_capi_test_visualization, capi_test_visualization)
BOOST_AUTO_TEST_CASE(test_1d_plots) {
  capi_test_visualization::test_1d_plots();
}
BOOST_AUTO_TEST_SUITE_END()