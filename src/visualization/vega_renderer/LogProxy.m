/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#import "LogProxy.h"
#import "LogProxyHandler.h"

@implementation LogProxy

+ (JSValue *)wrap:(JSValue *)instance {
  return [LogProxy wrap:instance withGetHandler:^NSObject*(NSObject *target, NSString *key) {
    return nil;
  } setHandler:^BOOL(NSObject *target, NSString *key, NSObject *value) {
    return FALSE;
  }];
}

+ (JSValue *)wrapObject:(NSObject *)object {
  // Assumes we have a current JSC context
  return [LogProxy wrap:[JSValue valueWithObject:object inContext:[JSContext currentContext]]];
}

+ (JSValue *)wrap:(JSValue *)instance
   withGetHandler:(LogProxyGetHandler_t)getHandler
       setHandler:(LogProxySetHandler_t)setHandler {
  instance.context[@"__tmp_wrapped_object"] = instance;
  instance.context[@"__tmp_wrapped_object_original"] = instance;
  instance.context[@"__tmp_handler"] = [[LogProxyHandler alloc] init];
  [instance.context evaluateScript:@"__tmp_wrapped_object = new Proxy(__tmp_wrapped_object_original, __tmp_handler);"];
  JSValue *ret = instance.context[@"__tmp_wrapped_object"];
  return ret;
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