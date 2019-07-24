/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#import <JavaScriptCore/JavaScriptCore.h>
#include <os/log.h>

typedef id (^LogProxyHandler_t)(NSObject *obj, NSString *property);

@interface LogProxy : NSObject

+ (os_log_t)logger;

/*
 * Provides a default handler (uses [obj valueForKey:property]),
 * that will log all property accesses using os_log_debug, with subsystem
 * "com.apple.turi" and component "vega_renderer".
 */
+ (JSValue *)wrap:(JSValue *)instance;

/*
 * Takes a handler to wrap the instance with; all property accesses will go
 * through this handler, and the handler should return the property value.
 */
+ (JSValue *)wrap:(JSValue *)instance
      withHandler:(LogProxyHandler_t)handler;

@end