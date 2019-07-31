/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#import <JavaScriptCore/JavaScriptCore.h>
#include <os/log.h>

typedef JSValue * (^LogProxyGetHandler_t)(JSValue *obj, NSString *property);
typedef BOOL (^LogProxySetHandler_t)(JSValue *obj, NSString *property, JSValue *value);

@interface LogProxy : NSObject

/*
 * Provides a default handler (uses [obj valueForKey:property]),
 * that will log all property accesses using os_log_info, and will log missing
 * properties with os_log_error, using subsystem "com.apple.turi" and component
 * "vega_renderer".
 */
+ (JSValue *)wrap:(JSValue *)instance;

/*
 * Provides a default handler (uses [obj valueForKey:property]),
 * that will log all property accesses using os_log_info, and will log missing
 * properties with os_log_error, using subsystem "com.apple.turi" and component
 * "vega_renderer".
 */
+ (JSValue *)wrapObject:(NSObject *)object;

/*
 * Takes a handler to wrap the instance with; all property accesses will go
 * through this handler, and the handler should return the property value.
 */
+ (JSValue *)wrap:(JSValue *)instance
   withGetHandler:(LogProxyGetHandler_t)getHandler
       setHandler:(LogProxySetHandler_t)setHandler;


/*
 * Takes a LogProxy, or any other object type.
 * If object is a LogProxy wrapper, returns the wrapped object.
 * Otherwise, returns the object passed in.
 */
+ (id)unwrap:(id)object;

/*
 * A preconfigured log object for use with os_log methods.
 */
+ (os_log_t)logger;

@end