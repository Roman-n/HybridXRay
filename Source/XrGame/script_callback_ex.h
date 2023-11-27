////////////////////////////////////////////////////////////////////////////
//	Module 		: script_callback_ex.h
//	Created 	: 06.02.2004
//  Modified 	: 11.01.2005
//	Author		: Sergey Zhemeitsev and Dmitriy Iassenev
//	Description : Script callbacks with return value
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ai_space.h"
#include "script_engine.h"

IC bool compare_safe(const luabind::object& o1, const luabind::object& o2)
{
    if ((luabind::type(o1) == LUA_TNIL) && (luabind::type(o2) == LUA_TNIL))
        return true;

    return o1 == o2;
}


template <typename _return_type> class CScriptCallbackEx
{
public:
    using return_type = _return_type;

private:
    typedef luabind::functor<_return_type> functor_type;
    typedef luabind::object                object_type;
    typedef bool (CScriptCallbackEx::*unspecified_bool_type)() const;

protected:
    functor_type m_functor;
    object_type  m_object;

private:
    IC bool empty() const;

public:
    IC CScriptCallbackEx();
    IC CScriptCallbackEx(const CScriptCallbackEx& callback);
    IC virtual ~CScriptCallbackEx();
    IC CScriptCallbackEx& operator=(const CScriptCallbackEx& callback);
    IC bool                operator==(const CScriptCallbackEx& callback) const
    {
        using luabind_obj_cr = luabind::adl::object const&;

        return compare_safe(m_object, (callback.m_object)) && (luabind_obj_cr)m_functor == (luabind_obj_cr)(callback.m_functor);
    }
    IC bool operator==(const object_type& object) const
    {
        return compare_safe(m_object, object);
    }
    IC void set(const functor_type& functor);
    IC void set(const functor_type& functor, const object_type& object);
    IC void clear();
    IC      operator unspecified_bool_type() const
    {
        return (!m_functor.is_valid() ? 0 : &CScriptCallbackEx::empty);
    }

    template<typename... TArgs>
    return_type operator()(TArgs&&... args);

    template<typename... TArgs>
    return_type operator()(TArgs&&... args) const;
};

#include "script_callback_ex_inline.h"