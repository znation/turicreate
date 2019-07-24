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

@protocol TestExportInterface<JSExport>
@property NSString * expected;
@end

@interface TestExport : NSObject<TestExportInterface>
@property NSString * expected;
@end

@implementation TestExport
@synthesize expected;

- (instancetype)init {
    self = [super init];
    self.expected = @"this is expected.";
    return self;
}

@end

void LogProxyTests::test_no_logging_on_expected_property_access() {
    JSContext *context = [[JSContext alloc] init];
    TestExport *original = [[TestExport alloc] init];
    JSValue *v = [JSValue valueWithObject:original inContext:context];
    JSValue *wrapped = [LogProxy wrap:v];
    TS_ASSERT_DIFFERS(wrapped, nil);
    NSString *str = @"this is not expected.";
    str = wrapped[@"expected"].toString;

    // Expect a defined property to give back the correct result through the wrapper
    TS_ASSERT_DIFFERS(str, nil);
    std::string expected = "this is expected.";
    std::string actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);
}

void LogProxyTests::test_logging_on_unexpected_property_access() {
    JSContext *context = [[JSContext alloc] init];
    TestExport *original = [[TestExport alloc] init];
    JSValue *v = [JSValue valueWithObject:original inContext:context];
    JSValue *wrapped = [LogProxy wrap:v];
    TS_ASSERT_DIFFERS(wrapped, nil);
    NSString *str = @"this is not expected.";

    // Expect accessing a missing property to throw an exception?
    //str = wrapped[@"unexpected"].toString;

    // Expect str to be unmodified
    TS_ASSERT_DIFFERS(str, nil);
    std::string expected = "this is not expected.";
    std::string actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);
}