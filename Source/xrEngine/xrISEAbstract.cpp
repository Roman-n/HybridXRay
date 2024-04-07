#include "stdafx.h"
#include "xrISEAbstract.h"

void ISE_Abstract::FillProp(LPCSTR pref, PropItemVec& items)
{
    R_ASSERT(FALSE);
}

void ISE_Abstract::on_render(CDUInterface* du, ISE_AbstractLEOwner* owner, bool bSelected, const Fmatrix& parent, int priority, bool strictB2F)
{
    R_ASSERT(FALSE);
}

visual_data* ISE_Abstract::visual_collection() const
{
    R_ASSERT(FALSE);
    return nullptr;
}

u32 ISE_Abstract::visual_collection_size() const
{
    R_ASSERT(FALSE);
    return u32();
}

void ISE_Abstract::set_additional_info(void* info)
{
    R_ASSERT(FALSE);
}
