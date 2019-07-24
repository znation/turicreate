/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

struct LogProxyTests {
    static void test_no_logging_on_expected_property_access();
    static void test_logging_on_unexpected_property_access();
    static void test_logging_on_unexpected_function_call();
};