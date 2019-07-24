/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#import "LogProxy.h"

@interface LogProxy (Logging)
+ (os_log_t)logger;
@end

@implementation LogProxy

+ (os_log_t)logger {
    static os_log_t log;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
      log = os_log_create("com.apple.turi", "vega_renderer");
    });
    return log;
}

+ (JSValue *)wrap:(JSValue *)instance {
  return [LogProxy wrap:instance withHandler:^(NSObject *target, NSString *key) {
    // First off, if the JS interface for this type has this property, return it
    if ([instance hasProperty:key]) {
      JSContext *context = instance.context;
      JSValue *ret = instance[key];
      context[@"__tmp_propertyAccess"] = ret;
      if ([[context evaluateScript:@"typeof __tmp_propertyAccess"].toString isEqualToString:@"function"]) {
        // it's a method, bind it to the original target or else we'll get
        // "self type check failed for Objective-C instance method"
        ret = [ret invokeMethod:@"bind" withArguments:@[instance]];
      }
      return ret;
    }

    // Encountered a missing key here!
#ifndef NDEBUG
    NSLog(@"Missing property \"%@\" on LogProxy wrapped object %@", key, target.debugDescription);
#endif
    os_log_error(LogProxy.logger, "Missing property \"%s\" on LogProxy wrapped object %s", key.UTF8String, target.debugDescription.UTF8String);

    // This will preserve the semantics of property access without LogProxy,
    // which is to return undefined for a missing property.
    return [JSValue valueWithUndefinedInContext:instance.context];
  }];
}

+ (JSValue *)wrapObject:(NSObject *)object {
  // Assumes we have a current JSC context
  return [LogProxy wrap:[JSValue valueWithObject:object inContext:[JSContext currentContext]]];
}

+ (JSValue *)wrap:(JSValue *)instance
    withHandler:(LogProxyHandler_t)handler {
  instance.context[@"__tmp_valueForKey"] = ^(NSObject *target, NSString *key) {
    os_log_info(LogProxy.logger, "Accessing property \"%s\" on LogProxy wrapped object %s", key.UTF8String, target.debugDescription.UTF8String);
    return handler(target, key);
  };
  instance.context[@"__tmp_wrapped_object"] = instance;
  [instance.context evaluateScript:@"__tmp_wrapped_object = new Proxy(__tmp_wrapped_object, { get: __tmp_valueForKey });"];
  JSValue *ret = instance.context[@"__tmp_wrapped_object"];
  return ret;
}

@end