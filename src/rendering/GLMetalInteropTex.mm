/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implemenation of class representing a texture shared between OpenGL and Metal
*/
#include "GLMetalInteropText.h"


/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implemenation of class representing a texture shared between OpenGL and Metal
*/

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "AAPLGLHeaders.h"

#ifdef TARGET_MACOS
#import <AppKit/AppKit.h>
#define PlatformGLContext NSOpenGLContext
#else // if!(TARGET_IOS || TARGET_TVOS)
#import <UIKit/UIKit.h>
#define PlatformGLContext EAGLContext
#endif // !(TARGET_IOS || TARGET_TVOS)


@interface AAPLOpenGLMetalInteropTexture : NSObject

- (nonnull instancetype)initWithMetalDevice:(nonnull id <MTLDevice>) mtlDevice
                              openGLContext:(nonnull PlatformGLContext*) glContext
                           metalPixelFormat:(MTLPixelFormat)mtlPixelFormat
                                       size:(CGSize)size;

@property (readonly, nonnull, nonatomic) id<MTLDevice> metalDevice;
@property (readonly, nonnull, nonatomic) id<MTLTexture> metalTexture;

@property (readonly, nonnull, nonatomic) PlatformGLContext *openGLContext;
@property (readonly, nonatomic) GLuint openGLTexture;

@property (readonly, nonatomic) CGSize size;

@end



typedef struct {
    int                 cvPixelFormat;
    MTLPixelFormat      mtlFormat;
    GLuint              glInternalFormat;
    GLuint              glFormat;
    GLuint              glType;
} AAPLTextureFormatInfo;

#if TARGET_IOS
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#endif

// Table of equivalent formats across CoreVideo, Metal, and OpenGL
static const AAPLTextureFormatInfo AAPLInteropFormatTable[] =
{
    // Core Video Pixel Format,               Metal Pixel Format,            GL internalformat, GL format,   GL type
    { kCVPixelFormatType_32BGRA,              MTLPixelFormatBGRA8Unorm,      GL_RGBA,           GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV },
#if TARGET_IOS
    { kCVPixelFormatType_32BGRA,              MTLPixelFormatBGRA8Unorm_sRGB, GL_RGBA,           GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV },
#else
    { kCVPixelFormatType_ARGB2101010LEPacked, MTLPixelFormatBGR10A2Unorm,    GL_RGB10_A2,       GL_BGRA,     GL_UNSIGNED_INT_2_10_10_10_REV },
    { kCVPixelFormatType_32BGRA,              MTLPixelFormatBGRA8Unorm_sRGB, GL_SRGB8_ALPHA8,   GL_BGRA,     GL_UNSIGNED_INT_8_8_8_8_REV },
    { kCVPixelFormatType_64RGBAHalf,          MTLPixelFormatRGBA16Float,     GL_RGBA,           GL_RGBA,     GL_HALF_FLOAT },
#endif
};

static const NSUInteger AAPLNumInteropFormats = sizeof(AAPLInteropFormatTable) / sizeof(AAPLTextureFormatInfo);

const AAPLTextureFormatInfo *const textureFormatInfoFromMetalPixelFormat(MTLPixelFormat pixelFormat)
{
    for(int i = 0; i < AAPLNumInteropFormats; i++) {
        if(pixelFormat == AAPLInteropFormatTable[i].mtlFormat) {
            return &AAPLInteropFormatTable[i];
        }
    }
    return NULL;
}

@implementation AAPLOpenGLMetalInteropTexture
{
    const AAPLTextureFormatInfo *_formatInfo;
    CVPixelBufferRef _CVPixelBuffer;
    CVMetalTextureRef _CVMTLTexture;

    CVOpenGLTextureCacheRef _CVGLTextureCache;
    CVOpenGLTextureRef _CVGLTexture;
    CGLPixelFormatObj _CGLPixelFormat;

    // Metal
    CVMetalTextureCacheRef _CVMTLTextureCache;

    CGSize _size;
}

- (nonnull instancetype)initWithMetalDevice:(nonnull id <MTLDevice>) metalevice
                              openGLContext:(nonnull PlatformGLContext *) glContext
                           metalPixelFormat:(MTLPixelFormat)mtlPixelFormat
                                       size:(CGSize)size
{
    self = [super init];
    if(self)
    {
        _formatInfo =
            textureFormatInfoFromMetalPixelFormat(mtlPixelFormat);

        NSAssert(_formatInfo, @"Metal Format supplied not supported in this sample");

        _size = size;
        _metalDevice = metalevice;
        _openGLContext = glContext;

        _CGLPixelFormat = _openGLContext.pixelFormat.CGLPixelFormatObj;

        NSDictionary* cvBufferProperties = @{
            (__bridge NSString*)kCVPixelBufferOpenGLCompatibilityKey : @YES,
            (__bridge NSString*)kCVPixelBufferMetalCompatibilityKey : @YES,
        };
        CVReturn cvret = CVPixelBufferCreate(kCFAllocatorDefault,
                                size.width, size.height,
                                _formatInfo->cvPixelFormat,
                                (__bridge CFDictionaryRef)cvBufferProperties,
                                &_CVPixelBuffer);

        NSAssert(cvret == kCVReturnSuccess, @"Failed to create CVPixelBuffer");
        
        [self createGLTexture];
        [self createMetalTexture];
    }
    return self;
}



/**
 On macOS, create an OpenGL texture and retrieve an OpenGL texture name using the following steps, and as annotated in the code listings below:
 */
- (void)createGLTexture
{
    CVReturn cvret;
    // 1. Create an OpenGL CoreVideo texture cache from the pixel buffer.
    cvret  = CVOpenGLTextureCacheCreate(
                    kCFAllocatorDefault,
                    nil,
                    _openGLContext.CGLContextObj,
                    _CGLPixelFormat,
                    nil,
                    &_CVGLTextureCache);
    
    NSAssert(cvret == kCVReturnSuccess, @"Failed to create OpenGL Texture Cache");
    
    // 2. Create a CVPixelBuffer-backed OpenGL texture image from the texture cache.
    cvret = CVOpenGLTextureCacheCreateTextureFromImage(
                    kCFAllocatorDefault,
                    _CVGLTextureCache,
                    _CVPixelBuffer,
                    nil,
                    &_CVGLTexture);
    
    NSAssert(cvret == kCVReturnSuccess, @"Failed to create OpenGL Texture From Image");
    
    // 3. Get an OpenGL texture name from the CVPixelBuffer-backed OpenGL texture image.
    _openGLTexture = CVOpenGLTextureGetName(_CVGLTexture);
}


/**
 Create a Metal texture from the CoreVideo pixel buffer using the following steps, and as annotated in the code listings below:
 */
- (void)createMetalTexture
{
    CVReturn cvret;
    // 1. Create a Metal Core Video texture cache from the pixel buffer.
    cvret = CVMetalTextureCacheCreate(
                    kCFAllocatorDefault,
                    nil,
                    _metalDevice,
                    nil,
                    &_CVMTLTextureCache);

    NSAssert(cvret == kCVReturnSuccess, @"Failed to create Metal texture cache");
    
    // 2. Create a CoreVideo pixel buffer backed Metal texture image from the texture cache.
    
    cvret = CVMetalTextureCacheCreateTextureFromImage(
                    kCFAllocatorDefault,
                    _CVMTLTextureCache,
                    _CVPixelBuffer, nil,
                    _formatInfo->mtlFormat,
                    _size.width, _size.height,
                    0,
                    &_CVMTLTexture);
    
    NSAssert(cvret == kCVReturnSuccess, @"Failed to create CoreVideo Metal texture from image");
    
    // 3. Get a Metal texture using the CoreVideo Metal texture reference.
    _metalTexture = CVMetalTextureGetTexture(_CVMTLTexture);
    
    NSAssert(_metalTexture, @"Failed to create Metal texture CoreVideo Metal Texture");
}

@end


AAPLOpenGLMetalInteropTexture *InteropTex;



int init_interop(void *glContext){

    
}











