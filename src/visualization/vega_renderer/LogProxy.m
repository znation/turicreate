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
  return [LogProxy wrap:instance withGetHandler:^(NSObject *target, NSString *key) {

    // First off, if the JS interface for this type has this property, return it
    JSValue *ret = instance[key];
    if (ret != nil && !ret.isUndefined) {
      JSContext *context = instance.context;
      context[@"__tmp_propertyAccess"] = ret;
      if ([[context evaluateScript:@"typeof __tmp_propertyAccess"].toString isEqualToString:@"function"]) {
        // it's a method, bind it to the original target or else we'll get
        // "self type check failed for Objective-C instance method"
        ret = [ret invokeMethod:@"bind" withArguments:@[instance]];
      }

      // Make sure the wrapping is applied recursively on objects
      if (ret.isObject) {
        ret = [LogProxy wrap:ret];
      }

      return ret;
    }

    // Encountered a missing key here!
#ifndef NDEBUG
    NSLog(@"Get for missing property \"%@\" on LogProxy wrapped object %@", key, target.debugDescription);
    assert(false);
#endif
    os_log_error(LogProxy.logger, "Get for missing property \"%s\" on LogProxy wrapped object %s", key.UTF8String, target.debugDescription.UTF8String);

    // This will preserve the semantics of property access without LogProxy,
    // which is to return undefined for a missing property.
    return [JSValue valueWithUndefinedInContext:instance.context];

  } setHandler:^BOOL(NSObject *target, NSString *key, NSObject *value) {

    assert(![key isEqualToString:@"__instance"]);

    // First off, if the JS interface for this type has this property, use it
    if ([instance hasProperty:key]) {
      instance[key] = value;
      return TRUE;
    }

    // Encountered a missing key here!
#ifndef NDEBUG
    NSLog(@"Set for missing property \"%@\" on LogProxy wrapped object %@ to value %@", key, target.debugDescription, value.debugDescription);
    assert(false);
#endif
    os_log_error(LogProxy.logger, "Set for missing property \"%s\" on LogProxy wrapped object %s to value %s", key.UTF8String, target.debugDescription.UTF8String, value.debugDescription.UTF8String);

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
  __weak JSValue * weak_instance = instance;
  instance.context[@"__tmp_valueForKey"] = ^(NSObject *target, NSString *key) {
    NSLog(@"Zach was here 1");
    if ([key isEqualToString:@"__instance"]) {
      return (NSObject *)weak_instance;
    }
    os_log_info(LogProxy.logger, "Getting property \"%s\" on LogProxy wrapped object %s", key.UTF8String, target.debugDescription.UTF8String);
    return getHandler(target, key);
  };
  instance.context[@"__tmp_setValueForKey"] = ^BOOL(NSObject *target, NSString *key, NSObject *value) {
    // TODO why does this not get called???
    NSLog(@"Zach was here 2");
    os_log_info(LogProxy.logger, "Setting property \"%s\" on LogProxy wrapped object %s to value %s", key.UTF8String, target.debugDescription.UTF8String, value.debugDescription.UTF8String);
    return setHandler(target, key, value);
  };
  instance.context[@"__tmp_wrapped_object"] = instance;
  instance.context[@"__tmp_wrapped_object_original"] = instance;
  [instance.context evaluateScript:@""
    "__tmp_wrapped_object = new Proxy(__tmp_wrapped_object_original, {"
    "    get: __tmp_valueForKey,"
    "    set: __tmp_setValueForKey,"
    "});"
    "Object.defineProperty(__tmp_wrapped_object, '__instance', {"
    "  enumerable: true,"
    "  value: __tmp_wrapped_object_original,"
    "});"
  ];
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