#pragma once
#include "Level.h"
class CLevelEditor: public CLevel
{
public:
    CLevelEditor();
    virtual ~CLevelEditor();
    virtual BOOL net_Start(LPCSTR op_server, LPCSTR op_client);
};
add_to_type_list(CLevelEditor)
#undef script_type_list
#define script_type_list save_type_list(CLevelEditor)
