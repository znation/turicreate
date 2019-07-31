/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#import "LogProxy.h"
#import "LogProxyHandler.h"

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
  return [LogProxy wrap:instance withHandler:[[LogProxyHandler alloc] init]];
}

+ (JSValue *)wrapObject:(NSObject *)object {
  // Assumes we have a current JSC context
  return [LogProxy wrap:[JSValue valueWithObject:object inContext:[JSContext currentContext]]];
}

+ (JSValue *)wrap:(JSValue *)instance
   withHandler:(id<LogProxyHandling>)handler {
  instance.context[@"__tmp_wrapped_object"] = instance;
  instance.context[@"__tmp_handler"] = handler;
  return [instance.context evaluateScript:@"new Proxy(__tmp_wrapped_object, __tmp_handler);"];
}

+ (id)unwrap:(id)object {
  // First unwrap JSValues into Objective C object values
  if ([object isKindOfClass:JSValue.class]) {
    JSValue *value = (JSValue *)object;
    if (value.isObject) {
      object = value.toObject;
    }
  }

  // Then test for NSDictionary, which is what new Proxy() shows
  // up as in Objective C.
  if ([object isKindOfClass:NSDictionary.class]) {
    NSDictionary *dict = (NSDictionary *)object;

    // If it has __instance on it, assume we put it there and
    // that's what we want to return.
    id instance = [dict objectForKey:@"__instance"];
    if (instance != nil) {
      return instance;
    }
  }

  // It doesn't appear to be a Proxy object, or at least not
  // one wrapped by LogProxy. Return it as is.
  return object;
}

@end