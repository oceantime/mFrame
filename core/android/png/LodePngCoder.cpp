#include <cstdlib>
#include "LodePngCoder.h"
extern "C" {
#include "thirdparty/lodepng.h"
}
#include "../support/Log.h"

namespace wmcanvas {
LodePngCoder::LodePngCoder() {}

LodePngCoder::~LodePngCoder() {}

int LodePngCoder::Init() { return 0; }

int LodePngCoder::UnInit() { return 0; }

bool LodePngCoder::IsSupportEncode() { return true; }

bool LodePngCoder::IsSupportDecode() { return true; }

int LodePngCoder::Decode(const char *filePath, unsigned char **outputBuffer,
                         unsigned int *outputWidth,
                         unsigned int *outputHeight)
{
    unsigned int uw = 0, uh = 0;
    unsigned char *buffer = nullptr;
    int error = lodepng_decode32_file(&buffer, &uw, &uh, filePath);

    if (outputWidth) *outputWidth = uw;
    if (outputHeight) *outputHeight = uh;
    if (outputBuffer) *outputBuffer = buffer;

    if (error)
    {
        LOG_I("[LodePngCoder] fail to decode, error:%d, %s", error,
              lodepng_error_text(error));
    }

    return error;
}

int LodePngCoder::Decode(const unsigned char *inBuffer,
                         unsigned int bufferLen, unsigned char **outputBuffer,
                         unsigned int *output_width,
                         unsigned int *output_height)
{
    unsigned int uw = 0, uh = 0;
    unsigned char *buffer = nullptr;
    int error = lodepng_decode32(&buffer, &uw, &uh, inBuffer, bufferLen);

    if (output_width) *output_width = uw;
    if (output_height) *output_height = uh;
    if (outputBuffer) *outputBuffer = buffer;

    if (error)
    {
        LOG_I("[LodePngCoder] fail to decode, error:%d, %s", error,
              lodepng_error_text(error));
    }

    return error;
}

int LodePngCoder::Encode(const char *filename, const unsigned char *image,
                         unsigned w, unsigned h)
{
    if (!filename || !image) return -1;

    int res = lodepng_encode32_file(filename, image, w, h);
    if (res != 0)
    {
        LOG_I("[LodePngCoder] fail to encode png, error:%d, %s", res,
              lodepng_error_text(res));
    }

    return res;
}

int LodePngCoder::Encode(unsigned char** out, int* outsize, const unsigned char* image, unsigned w, unsigned h) {

  if (!image) return -1;

  int res = lodepng_encode32(out,(size_t *)outsize, image, w, h);
  if (res != 0) {
    LOG_I( "[LodePngCoder] fail to encode png, error:%d, %s", res, lodepng_error_text(res));
  }

  return res;
}

void LodePngCoder::FreeBuffer(unsigned char *buffer) { free(buffer); }
}
