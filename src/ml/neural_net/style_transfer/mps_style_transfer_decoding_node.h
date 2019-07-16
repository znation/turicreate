#import <Accelerate/Accelerate.h>
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <MetalPerformanceShaders/MetalPerformanceShaders.h> 

#import <ml/neural_net/mps_descriptor_utils.h>


NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.14))
@interface TCMPSStyleTransferDecodingNode : NSObject 

@property (nonatomic) MPSNNImageNode *output;

- (instancetype) initWithParameters:(NSString *)name
                          inputNode:(MPSNNImageNode *)inputNode
                             device:(id<MTLDevice>)dev
                           cmdQueue:(id<MTLCommandQueue>)cmdQ
                         descriptor:(TCMPSDecodingDescriptor *)descriptor
                        initWeights:(NSDictionary<NSString *, NSData *> *) weights;

- (MPSNNImageNode *) backwardPass:(MPSNNImageNode *) inputNode;

@end

NS_ASSUME_NONNULL_END