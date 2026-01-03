#ifndef IDELETE_H_
#define IDELETE_H_

namespace WmCanvas
{
class IDelete
{
public:
    IDelete() {}
    virtual void Delete() = 0;

protected:
    virtual ~IDelete() {}
};
}

#endif
