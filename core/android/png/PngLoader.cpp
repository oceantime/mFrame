#include "PngLoader.h"
#include "LodePngCoder.h"

namespace WmCanvas
{
PngLoader::PngLoader()
{
#if ENABLE_LODE_PNG
    mCoders.push_back(new LodePngCoder());
#endif
}

PngLoader::~PngLoader()
{
    PngCoderVector::iterator index;
    for (index = mCoders.begin(); index != mCoders.end(); ++index)
    {
        delete (*index);
    }
    mCoders.clear();
}

PngLoader &PngLoader::Instance()
{
    static PngLoader loader;
    return loader;
}

int PngLoader::Init()
{
    // TODO:
    return 0;
}

int PngLoader::UnInit()
{
    // TODO:
    return 0;
}

int PngLoader::DecodePng(const char *filePath, unsigned char **outputBuffer,
                         unsigned int *w, unsigned int *h)
{
    size_t index, count = mCoders.size();

    for (index = 0; index < count; ++index)
    {
        if (!mCoders[index]->IsSupportDecode()) continue;

        if (mCoders[index]->Decode(filePath, outputBuffer, w, h) == 0)
            return 0;
    }

    return -1;
}

int PngLoader::DecodePng(const unsigned char *inBuffer,
                         unsigned int bufferLen, unsigned char **outputBuffer,
                         unsigned int *w, unsigned int *h)
{
    size_t index, count = mCoders.size();

    for (index = 0; index < count; ++index)
    {
        if (!mCoders[index]->IsSupportDecode()) continue;

        if (mCoders[index]->Decode(inBuffer, bufferLen, outputBuffer, w,
                                   h) == 0)
            return 0;
    }

    return -1;
}

int PngLoader::EncodePng(const char *filename, const unsigned char *image,
                         unsigned w, unsigned h)
{
    size_t index, count = mCoders.size();

    for (index = 0; index < count; ++index)
    {
        if (!mCoders[index]->IsSupportEncode()) continue;

        if (mCoders[index]->Encode(filename, image, w, h) == 0) return 0;
    }

    return -1;
}

int PngLoader::EncodePng(unsigned char** out, int* outsize, const unsigned char* image, unsigned w, unsigned h) {
  size_t index, count = mCoders.size();

  for (index = 0; index < count; ++index) {
    if (!mCoders[index]->IsSupportEncode())
      continue;

    if (mCoders[index]->Encode(out, outsize,image, w, h) == 0)
      return 0;
  }

  return -1;
}

void PngLoader::FreeBuffer(unsigned char **buffer) { free(buffer); }
}
