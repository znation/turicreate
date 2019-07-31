/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#import "LogProxy.h"
#import "LogProxyHandler.h"

#include <os/log.h>

@implementation LogProxyHandler

- (JSValue *)getPropertyOnObject:(JSValue *)instance
                            named:(NSString *)key {
    os_log_info(LogProxy.logger, "Getting property \"%s\" on LogProxy wrapped object %s", key.UTF8String, instance.debugDescription.UTF8String);

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
    NSLog(@"Get for missing property \"%@\" on LogProxy wrapped object %@", key, instance.debugDescription);
#endif
    os_log_error(LogProxy.logger, "Get for missing property \"%s\" on LogProxy wrapped object %s", key.UTF8String, instance.debugDescription.UTF8String);

    // This will preserve the semantics of property access without LogProxy,
    // which is to return undefined for a missing property.
    return [JSValue valueWithUndefinedInContext:instance.context];
}

- (BOOL)setPropertyOnObject:(JSValue *)instance
                        named:(NSString *)key
                    toValue:(JSValue *)value {

    os_log_info(LogProxy.logger, "Setting property \"%s\" on LogProxy wrapped object %s to value \"%s\"", key.UTF8String, instance.debugDescription.UTF8String, value.debugDescription.UTF8String);

    // First off, if the JS interface for this type has this property, use it
    if ([instance hasProperty:key]) {
      instance[key] = value;
      return TRUE;
    }

    // Encountered a missing key here!
#ifndef NDEBUG
    NSLog(@"Set for missing property \"%@\" on LogProxy wrapped object %@ to value %@", key, instance.debugDescription, value.debugDescription);
#endif
    os_log_error(LogProxy.logger, "Set for missing property \"%s\" on LogProxy wrapped object %s to value %s", key.UTF8String, instance.debugDescription.UTF8String, value.debugDescription.UTF8String);

    return FALSE;
}

@end