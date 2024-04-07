#pragma once

class UIMoveToForm: public xrUI
{
public:
    UIMoveToForm();
    virtual void Draw();

private:
    void    CLBOk();
    void    CLBCancel();
    void    CLBReset();
    Fvector m_DesiredCameraPosition;
    Fvector m_BackupCameraPosition;
};
