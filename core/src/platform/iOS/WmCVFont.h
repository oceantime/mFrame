#ifndef GCANVAS_GCVFONT_H
#define GCANVAS_GCVFONT_H

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <CoreText/CoreText.h>
#include "WmTextDefine.h"
#include "WmGlyphCache.h"
#include "WmTreemap.h"

typedef struct {
    float x, y, w, h;
    CGSize advances;
    unsigned short textureIndex;
    float tx, ty, tw, th;
} GFontGlyphInfo;


typedef struct {
    float xpos;
    WmGlyphs info;
} GFontGlyphLayout;

typedef struct {
    float width;
    float ascent;
    float descent;
} GTextMetrics;

namespace wmcanvas
{
    class WmFontStyle;
}

@interface GFontLayout : NSObject

@property (nonatomic, strong) NSMutableData *glyphLayout;
@property (nonatomic, assign) GTextMetrics metrics;
@property (nonatomic, assign) NSInteger glyphCount;

@end

class WmCanvasContext;

@interface GCVFont : NSObject

@property(nonatomic,assign) WmGlyphCache *glyphCache;
@property(nonatomic,assign) GTreemap *treemap;

+ (instancetype)createGCFontWithKey:(NSString*)key;
+ (GCVFont*)getGCVFontWithKey:(NSString*)key;


- (void)cleanFont;

/**
 * Reset GCVFont style with fontStyle, isStroke.
 *
 * @param fontStyle     fontStyle name
 * @param isStroke      stroke flag
 */
- (void)resetWithFontStyle:(wmcanvas::WmFontStyle *)fontStyle isStroke:(BOOL)isStroke context:(WmCanvasContext *)context;

/**
 * Get getLayoutForString.
 *
 * @param string        input string
 *
 * return  return GFontLayout
 */
- (GFontLayout *)getLayoutForString:(NSString *)string
                       withFontName:(NSString*)fontName;

- (void )drawString:(NSString*)string
       withFontName:(NSString*)fontName
         withLayout:(GFontLayout*)fontLayout
       withPosition:(CGPoint)destPoint
            context:(WmCanvasContext *)context;

- (void)getGlyphForChar:(wchar_t)c
           withFontName:(NSString*)fontName
         withFontLayout:(GFontLayout *)fontLayout
            withOffsetX:(int*)x;

/**
 * Create font texture with FontGlyph info
 *
 * @param glyph         CGGlyph
 * @param font          CTFontRef
 * @param fontName     NSString
 * @param glyphInfo     GFontGlyphInfo
 *
 * return  font textureId
 */
- (void)createGlyph:(CGGlyph)glyph
           withFont:(CTFontRef)font
       withFontName:(NSString*)fontName
          glyphInfo:(WmGlyphs *)glyphInfo;


/**
 * Adjust text source point
 *
 * @param srcPoint          source point to adjust
 * @param textAlign         see WmTextAlign definition
 * @param baseLine          see GTextBaseline definition
 * @param metrics           see GTextMetrics definition
 *
 * return  adjuested text source CGPoint
 */
- (CGPoint)adjustTextPenPoint:(CGPoint)srcPoint
                   textAlign:(WmTextAlign)textAlign
                    baseLine:(GTextBaseline)baseLine
                     metrics:(GTextMetrics)metrics;

- (NSString *)getFontNameWithCurrentScale:(wmcanvas::WmFontStyle *)fontStyle
                                  context:(WmCanvasContext *)context;

@end

#endif /* GCANVAS_GCVFONT_H */
