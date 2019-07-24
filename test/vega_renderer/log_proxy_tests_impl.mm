/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#import <JavaScriptCore/JavaScriptCore.h>
#import <visualization/vega_renderer/LogProxy.h>

#include <boost/test/unit_test.hpp>
#include <core/util/test_macros.hpp>
#include "log_proxy_tests.hpp"

#include <iostream>

@protocol TestExportInterface<JSExport>
@property (readonly) NSString * expected;
@end

@interface TestExport : NSObject<TestExportInterface>
@end

@implementation TestExport

- (NSString *)expected {
    return @"this is expected.";
}

@end

void LogProxyTests::test_no_logging_on_expected_property_access() {
    std::cout << "Starting no logging test" << std::endl;
    JSContext *context = [[JSContext alloc] init];
    TestExport *original = [[TestExport alloc] init];
    JSValue *v = [JSValue valueWithObject:original inContext:context];
    JSValue *wrapped = [LogProxy wrap:v];
    TS_ASSERT_DIFFERS(wrapped, nil);
    NSString *str = @"this is not expected.";
    BOOST_CHECK_NO_THROW(
        str = v[@"expected"].toString;
    );
    TS_ASSERT_DIFFERS(str, nil);
    std::string expected = "this is expected.";
    std::string actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);
    std::cout << "Finished no logging test" << std::endl;
}

void LogProxyTests::test_logging_on_unexpected_property_access() {

}

void LogProxyTests::test_logging_on_unexpected_function_call() {

}