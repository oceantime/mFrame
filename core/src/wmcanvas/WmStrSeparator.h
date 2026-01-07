#ifndef WMCANVAS_WMSTRSEPARATOR_H
#define WMCANVAS_WMSTRSEPARATOR_H

namespace wmcanvas
{
class WmStrSeparator
{
public:
    static const short PARSESEPSTRMAXCOUNT = 50;
    static const short PARSELOCALBUFFERSIZE = 480;

    WmStrSeparator();
    short SepStrBySpace(char *str, short maxCount = -1);
    short SepStrByCharArray(char *str, const char *byteArray, short byteCount,
                            short maxCount = -1);
    char **GetSepArray() { return (char **)mPointers; }

private:
    char *mPointers[PARSELOCALBUFFERSIZE];
};

}

#endif /* WMCANVAS_WMSTRSEPARATOR_H */
