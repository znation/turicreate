/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#import "LogProxy.h"

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
    os_log_debug(LogProxy.logger, "Accessing property \"%s\" on LogProxy wrapped object %s", key.UTF8String, target.debugDescription.UTF8String);
    return [target valueForKey:key];
  }];
}

+ (JSValue *)wrap:(JSValue *)instance
    withHandler:(LogProxyHandler_t)handler {
  instance.context[@"__tmp_valueForKey"] = ^(NSObject *target, NSString *key) {
    return handler(target, key);
  };
  instance.context[@"__tmp_wrapped_object"] = instance;
  [instance.context evaluateScript:@"__tmp_wrapped_object = new Proxy(__tmp_wrapped_object, { get: __tmp_valueForKey });"];
  JSValue *ret = instance.context[@"__tmp_wrapped_object"];
  [instance.context evaluateScript:@"del __tmp_valueForKey"];
  [instance.context evaluateScript:@"del __tmp_wrapped_object"];
  return ret;
}

@end