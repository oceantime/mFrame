#ifndef WMCANVAS_ENCODE_H
#define WMCANVAS_ENCODE_H

namespace wmcanvas
{
////////////////////////////base64 encoding////////////////////////////
int Base64EncodeLen(int inputLen);
int Base64EncodeBuf(char *szDest, const char *szSrc, int &iBufSize);
int Base64DecodeBuf(char *szDest, const char *szSrc, int &iBufSize);
}

#endif /* WMCANVAS_ENCODE_H */
