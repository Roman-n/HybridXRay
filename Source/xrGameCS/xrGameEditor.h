#include "../xrEngine/xrGameEditorInterface.h"
#ifndef SHIPPING
class xrGameEditor: public xrGameEditorInterface
{
public:
    xrGameEditor();
    virtual ~xrGameEditor();
    virtual void Create();
    virtual void Destroy();
};
#endif
