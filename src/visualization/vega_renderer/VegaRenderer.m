/* Copyright © 2019 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#import "VegaRenderer.h"
#import "JSCanvas.h"
#import "JSConsole.h"
#import "JSDocument.h"

#import <AppKit/AppKit.h>
#import <JavaScriptCore/JavaScriptCore.h>

#define Q(x) #x
#define QUOTE(x) Q(x)
#define IDENT(x) x
#define PATH(x,y) QUOTE(IDENT(x)IDENT(y))

#ifdef NDEBUG

// release mode, use minified JS
#define VEGA_JS_H PATH(OBJROOT,/vega-5.4.0.min.js.h)
#import VEGA_JS_H
#define vega_js vega_5_4_0_min_js
#define vega_js_size vega_5_4_0_min_js_len

#define VEGALITE_JS_H PATH(OBJROOT,/vega-lite-3.3.0.min.js.h)
#import VEGALITE_JS_H
#define vegalite_js vega_lite_3_3_0_min_js
#define vegalite_js_size vega_lite_3_3_0_min_js_len

#else

// debug mode, use unminified JS
#define VEGA_JS_H PATH(OBJROOT,/vega-5.4.0.js.h)
#import VEGA_JS_H
#define vega_js vega_5_4_0_js
#define vega_js_size vega_5_4_0_js_len

#define VEGALITE_JS_H PATH(OBJROOT,/vega-lite-3.3.0.js.h)
#import VEGALITE_JS_H
#define vegalite_js vega_lite_3_3_0_js
#define vegalite_js_size vega_lite_3_3_0_js_len

#endif

@interface VegaRenderer ()

@property (strong) NSString *spec;
@property (strong) JSContext *context;
@property (strong) VegaCGCanvas *vegaCanvas;
-(VegaCGContext *)vegaContext;

@end

@implementation VegaRenderer 

-(instancetype) initWithSpec:(NSString *)spec {
    return [self initWithSpec:spec context:[[NSGraphicsContext currentContext] CGContext]];
}

-(instancetype) initWithSpec:(NSString *)spec
                     context:(CGContextRef)parentContext {
    self = [super init];
    self.spec = spec;

    // Initialize the JSContext first, so we can populate
    // the scene graph and get the width & height from spec
    self.context = [[JSContext alloc] init];

    __unsafe_unretained typeof(self) weakSelf = self;

    @autoreleasepool {

        // set up logging
        [JSConsole attachToJavaScriptContext:self.context];

        VegaCGCanvas* vegaCanvas = [[VegaCGCanvas alloc] initWithContext:parentContext];
        weakSelf.vegaCanvas = vegaCanvas;

        VegaJSDocument* document = [[VegaJSDocument alloc] initWithCanvas:vegaCanvas];
        self.context[@"document"] = document;

        // set up Image type
        self.context[@"Image"] = [JSValue valueWithObject:^() {
            assert([[JSContext currentArguments] count] == 0);
            return [[VegaCGImage alloc] init];
        } inContext:self.context];

        [self.context evaluateScript:[VegaRenderer vegaJS]];
        [self.context evaluateScript:[VegaRenderer vegaliteJS]];
        [self.context evaluateScript:[VegaRenderer vg2canvasJS]];
        JSValue* render_fn = self.context[@"viewRender"];
        [render_fn callWithArguments:@[self.spec]];

        assert(self.vegaCanvas!= nil);

    }

    return self;
}

- (VegaCGContext *)vegaContext {
    // Make sure we don't have any extra properties on the dictionary
    // backing the JSValue. If we do, it means we missed some property
    // implementations on the JSExport protocol for this type.
    if (self.vegaCanvas == nil) {
        return nil;
    }
    JSValue *jsVegaCanvas = [JSValue valueWithObject:self.vegaCanvas inContext:self.context];
    NSDictionary *jsVegaCanvasExtraProps = jsVegaCanvas.toDictionary;
    if (jsVegaCanvasExtraProps.count > 0) {
        NSLog(@"Encountered extra properties on canvas: %@", jsVegaCanvasExtraProps);
        assert(false);
    }

    JSValue *jsVegaContext = [jsVegaCanvas invokeMethod:@"getContext" withArguments:@[@"2d"]];
    NSDictionary *jsVegaContextExtraProps = jsVegaContext.toDictionary;
    if (jsVegaContextExtraProps.count > 0) {
        NSLog(@"Encountered extra properties on canvas context: %@", jsVegaContextExtraProps);
        assert(false);
    }

    VegaCGContext *vegaContext = [self.vegaCanvas getContext:@"2d"];
    assert(vegaContext == jsVegaContext.toObject);
    return vegaContext;
}

-(NSUInteger)width {
    return self.vegaCanvas.context.width;
}

-(NSUInteger)height {
    return self.vegaCanvas.context.height;
}

-(CGLayerRef)CGLayer {
    return self.vegaContext.layer;
}

-(CGImageRef)CGImage {
    double scaleFactor = 2.0;
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bitsPerComponent = 8;
    CGContextRef bitmapContext = CGBitmapContextCreate(NULL, self.width * scaleFactor, self.height * scaleFactor, bitsPerComponent, 0, colorSpace, kCGImageAlphaPremultipliedLast);
    CGContextScaleCTM(bitmapContext, scaleFactor, scaleFactor);
    CGLayerRef layer = self.vegaContext.layer;
    CGContextDrawLayerAtPoint(bitmapContext, CGPointMake(0, 0), layer);
    CGImageRef image = CGBitmapContextCreateImage(bitmapContext);
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(bitmapContext);
    return image;
}

+ (NSString*)vg2canvasJS {
    return @
    "function viewRender(spec) {"
    "  if (typeof spec === 'string') {"
    "    spec = JSON.parse(spec);"
    "  }"
    "  if (spec['$schema'].startsWith('https://vega.github.io/schema/vega-lite/')) {"
    "    spec = vl.compile(spec).spec;"
    "  }"
    "  return new vega.View(vega.parse(spec), {"
    "    logLevel: vega.Warn,"
    "    renderer: 'canvas'"
    "  })"
    "  .initialize()"
    "  .runAsync()"
    "  .then(view => {"
    "    view.toCanvas().then(canvas => {"
    "    })"
    "    .catch(err => { console.error(err); });"
    "  })"
    "  .catch(err => { console.error(err); });"
    "}";
}

+ (NSString*)vegaJS {

    NSData *data = [NSData dataWithBytes:vega_js length:vega_js_size];
    NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return str;
}

+ (NSString*)vegaliteJS {

    NSData *data = [NSData dataWithBytes:vegalite_js length:vegalite_js_size];
    NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return str;
}

@end
