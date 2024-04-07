#pragma once
class ENGINE_API xrGameEditorInterface
{
public:
    xrGameEditorInterface();
    virtual ~xrGameEditorInterface();
    virtual void Create()  = 0;
    virtual void Destroy() = 0;
};
