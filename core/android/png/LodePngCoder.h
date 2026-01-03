#ifndef LODE_PNG_CODER_H_
#define LODE_PNG_CODER_H_
#include "IPngCoder.h"

namespace WmCanvas
{
class LodePngCoder : public IPngCoder
{
public:
    LodePngCoder();
    virtual ~LodePngCoder();

    virtual int Init();
    virtual int UnInit();

    virtual bool IsSupportEncode();
    virtual bool IsSupportDecode();

    virtual int Decode(const char *filePath, unsigned char **outputBuffer,
                       unsigned int *outputWidth, unsigned int *outputHeight);
    virtual int Decode(const unsigned char *inBuffer, unsigned int bufferLen,
                       unsigned char **outputBuffer, unsigned int *w,
                       unsigned int *h);
    virtual int Encode(const char *filename, const unsigned char *image,
                       unsigned w, unsigned h);
	virtual int Encode(unsigned char** out, int* outsize, const unsigned char* image, unsigned w, unsigned h);
    virtual void FreeBuffer(unsigned char *buffer);
};
}

#endif /* LODE_PNG_CODER_H_ */
