//
//  VegaRenderer.m
//  VegaRenderer
//
//  Created by Zachary Nation on 12/16/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import "VegaRenderer.h"
#import "Canvas.h"

#import <AppKit/AppKit.h>
#import <JavaScriptCore/JavaScriptCore.h>

#define Q(x) #x
#define QUOTE(x) Q(x)
#define IDENT(x) x
#define PATH(x,y) QUOTE(IDENT(x)IDENT(y))

#define VEGA_JS_H PATH(OBJROOT,/vega-4.4.0.min.js.h)
#import VEGA_JS_H
#define vega_js vega_4_4_0_min_js
#define vega_js_size vega_4_4_0_min_js_len

#define VEGALITE_JS_H PATH(OBJROOT,/vega-lite-3.0.0-rc10.min.js.h)
#import VEGALITE_JS_H
#define vegalite_js vega_lite_3_0_0_rc10_min_js
#define vegalite_js_size vega_lite_3_0_0_rc10_min_js_len

@interface VegaRenderer ()

@property NSString *spec;
@property JSContext *context;
@property VegaCGCanvas *vegaCanvas;
-(VegaCGContext *)vegaContext;

@end

@implementation VegaRenderer

-(instancetype) initWithSpec:(NSString *)spec {
    return [self initWithSpec:spec context:[[NSGraphicsContext currentContext] CGContext]];
}

-(instancetype) initWithSpec:(NSString *)spec
                     context:(CGContextRef)parentContext {
    self = [super init];
    
    // Initialize the JSContext first, so we can populate
    // the scene graph and get the width & height from spec
    JSContext *context = [[JSContext alloc] init];
    __block VegaCGCanvas *canvas = nil;
    
    // set up logging
    [context evaluateScript:@"var console = {};"];
    context[@"console"][@"log"] = ^() {
        NSArray *message = [JSContext currentArguments];
        NSLog(@"JS console log: %@", message);
    };
    context[@"console"][@"warn"] = ^() {
        NSArray *message = [JSContext currentArguments];
        NSLog(@"JS console warning: %@", message);
    };
    context[@"console"][@"error"] = ^() {
        NSArray *message = [JSContext currentArguments];
        NSLog(@"JS console error: %@", message);
        assert(false);
    };
    
    // set up error handling
    context.exceptionHandler = ^(JSContext *context, JSValue *exception) {
        NSLog(@"Unhandled exception: %@", [exception toString]);
        NSLog(@"In context: %@", [context debugDescription]);
        assert(false);
    };
    
    JSValue *canvas2 = [JSValue valueWithObject:^(double width, double height) {
        if (canvas == nil) {
            canvas = [VegaCGCanvas alloc];
        }
        canvas = [canvas initWithWidth:width height:height context:parentContext];
        return canvas;
    } inContext:context];
    canvas2[@"Image"] = [JSValue valueWithObject:^() {
        assert([[JSContext currentArguments] count] == 0);
        return [[VegaCGImage alloc] init];
    } inContext:context];
    
    JSValue *require = [JSValue valueWithObject:^(NSString *module) {
        if ([module isEqualToString:@"canvas"]) {
            return canvas2;
        }
        
        // fall through if we don't know what module it is
        NSLog(@"Called require with unknown module %@", module);
        return [JSValue valueWithNullInContext:context];
    } inContext:context];
    [context setObject:require forKeyedSubscript:@"require"];
    
    [context evaluateScript:[VegaRenderer vegaJS]];
    [context evaluateScript:[VegaRenderer vegaliteJS]];
    [context evaluateScript:[VegaRenderer vg2canvasJS]];
    JSValue* render_fn = context[@"viewRender"];
    [render_fn callWithArguments:@[spec]];
    
    assert(canvas != nil);
    
    self.spec = spec;
    self.context = context;
    self.vegaCanvas = canvas;
    
    return self;
}

- (VegaCGContext *)vegaContext {
    // Make sure we don't have any extra properties on the dictionary
    // backing the JSValue. If we do, it means we missed some property
    // implementations on the JSExport protocol for this type.
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

-(CGImageRef)CGImage {
    double scaleFactor = 2.0;
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bitsPerComponent = 8;
    CGContextRef bitmapContext = CGBitmapContextCreate(NULL, self.width * scaleFactor, self.height * scaleFactor, bitsPerComponent, 0, colorSpace, kCGImageAlphaPremultipliedLast);
    CGContextScaleCTM(bitmapContext, scaleFactor, scaleFactor);
    CGContextDrawLayerAtPoint(bitmapContext, CGPointMake(0, 0), self.vegaContext.layer);
    CGImageRef bitmapImage = CGBitmapContextCreateImage(bitmapContext);
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(bitmapContext);
    return bitmapImage;
}

-(CGLayerRef)CGLayer {
    return self.vegaContext.layer;
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
