#ifndef WMCANVAS_WMCVFONT_H
#define WMCANVAS_WMCVFONT_H

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
} WmFontGlyphLayout;

typedef struct {
    float width;
    float ascent;
    float descent;
} WmTextMetrics;

namespace wmcanvas
{
    class WmFontStyle;
}

@interface WmFontLayout : NSObject

@property (nonatomic, strong) NSMutableData *glyphLayout;
@property (nonatomic, assign) WmTextMetrics metrics;
@property (nonatomic, assign) NSInteger glyphCount;

@end

class WmCanvasContext;

@interface WmCVFont : NSObject

@property(nonatomic,assign) WmGlyphCache *glyphCache;
@property(nonatomic,assign) GTreemap *treemap;

+ (instancetype)createGCFontWithKey:(NSString*)key;
+ (WmCVFont*)getWmCVFontWithKey:(NSString*)key;


- (void)cleanFont;

/**
 * Reset WmCVFont style with fontStyle, isStroke.
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
 * return  return WmFontLayout
 */
- (WmFontLayout *)getLayoutForString:(NSString *)string
                       withFontName:(NSString*)fontName;

- (void )drawString:(NSString*)string
       withFontName:(NSString*)fontName
         withLayout:(WmFontLayout*)fontLayout
       withPosition:(CGPoint)destPoint
            context:(WmCanvasContext *)context;

- (void)getGlyphForChar:(wchar_t)c
           withFontName:(NSString*)fontName
         withFontLayout:(WmFontLayout *)fontLayout
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
 * @param metrics           see WmTextMetrics definition
 *
 * return  adjuested text source CGPoint
 */
- (CGPoint)adjustTextPenPoint:(CGPoint)srcPoint
                   textAlign:(WmTextAlign)textAlign
                    baseLine:(GTextBaseline)baseLine
                     metrics:(WmTextMetrics)metrics;

- (NSString *)getFontNameWithCurrentScale:(wmcanvas::WmFontStyle *)fontStyle
                                  context:(WmCanvasContext *)context;

@end

#endif /* WMCANVAS_WMCVFONT_H */
