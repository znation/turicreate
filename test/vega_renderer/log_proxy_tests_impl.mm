/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#import <JavaScriptCore/JavaScriptCore.h>
#import <visualization/vega_renderer/LogProxy.h>
#import <visualization/vega_renderer/JSConsole.h>

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
    [JSConsole attachToJavaScriptContext:context];
    TestExport *original = [[TestExport alloc] init];
    JSValue *v = [JSValue valueWithObject:original inContext:context];
    JSValue *wrapped = [LogProxy wrap:v];
    TS_ASSERT_DIFFERS(wrapped, nil);

    // Expect a defined property to give back the correct result through the wrapper
    NSString *str = wrapped[@"expected"].toString;
    TS_ASSERT_DIFFERS(str, nil);
    std::string expected = "this is expected.";
    std::string actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);

    // Expect setting a defined property to retain it
    wrapped[@"expected"] = @"this is the new value.";
    str = wrapped[@"expected"].toString;
    TS_ASSERT_DIFFERS(str, nil);
    expected = "this is the new value.";
    actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);
}

void LogProxyTests::test_logging_on_unexpected_property_access() {
    JSContext *context = [[JSContext alloc] init];
    [JSConsole attachToJavaScriptContext:context];
    TestExport *original = [[TestExport alloc] init];
    JSValue *v = [JSValue valueWithObject:original inContext:context];

    // Set up the wrapper to expect exactly what we are about to test
    JSValue *wrapped = [LogProxy wrap:v withGetHandler:^(NSObject *target, NSString *key) {
        TS_ASSERT(![v hasProperty:key]);
        return [JSValue valueWithUndefinedInContext:context];
    } setHandler:^BOOL(NSObject *target, NSString *key, NSObject *value) {
        TS_ASSERT_EQUALS(key.UTF8String, "undeclared");
        TS_ASSERT(![v hasProperty:key]);
        return FALSE;
    }];
    TS_ASSERT_DIFFERS(wrapped, nil);

    // Expect accessing a missing property to return undefined, but it will also
    // pass through the above wrapper and make assertions there.
    NSString *str = wrapped[@"unexpected"].toString;
    TS_ASSERT_DIFFERS(str, nil);
    std::string expected = "undefined";
    std::string actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);

    // Expect setting a missing property to do nothing,
    // and not retain the value
    wrapped[@"undeclared"] = @"this is the new value.";
    str = wrapped[@"undeclared"].toString;
    TS_ASSERT_DIFFERS(str, nil);
    expected = "undefined";
    actual = str.UTF8String;
    TS_ASSERT_EQUALS(expected, actual);
}