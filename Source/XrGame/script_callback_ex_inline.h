////////////////////////////////////////////////////////////////////////////
//	Module 		: script_callback_ex_inline.h
//	Created 	: 06.02.2004
//  Modified 	: 11.01.2005
//	Author		: Sergey Zhemeitsev and Dmitriy Iassenev
//	Description : Script callbacks with return value inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION template <typename _return_type>
#define CSScriptCallbackEx CScriptCallbackEx<_return_type>

TEMPLATE_SPECIALIZATION
IC CSScriptCallbackEx::CScriptCallbackEx() {}

TEMPLATE_SPECIALIZATION
IC CSScriptCallbackEx::~CScriptCallbackEx() {}

TEMPLATE_SPECIALIZATION
IC void CSScriptCallbackEx::clear()
{
    m_functor.~functor_type();
    new (&m_functor) functor_type();

    m_object.~object_type();
    new (&m_object) object_type();
}

TEMPLATE_SPECIALIZATION
IC CSScriptCallbackEx::CScriptCallbackEx(const CScriptCallbackEx& callback)
{
    clear();
    *this = callback;
}

TEMPLATE_SPECIALIZATION
IC CSScriptCallbackEx& CSScriptCallbackEx::operator=(const CScriptCallbackEx& callback)
{
    clear();

    if (callback.m_functor.is_valid() && callback.m_functor.interpreter())
        m_functor = callback.m_functor;

    if (callback.m_object.is_valid() && callback.m_object.interpreter())
        m_object = callback.m_object;

    return (*this);
}

TEMPLATE_SPECIALIZATION
IC void CSScriptCallbackEx::set(const functor_type& functor)
{
    clear();
    m_functor = functor;
}

TEMPLATE_SPECIALIZATION
IC void CSScriptCallbackEx::set(const functor_type& functor, const object_type& object)
{
    clear();

    m_functor = functor;
    m_object  = object;
}

TEMPLATE_SPECIALIZATION
IC bool CSScriptCallbackEx::empty() const
{
    return (!!m_functor.interpreter());
}


#if XRAY_EXCEPTIONS
#define process_error                                                                      \
    catch (luabind::error & e)                                                             \
    {                                                                                      \
        ai().script_engine().print_output(ai().script_engine().lua(), "", LUA_ERRRUN); \
    }
#else
#define process_error
#endif


TEMPLATE_SPECIALIZATION
template<typename... TArgs>
_return_type CSScriptCallbackEx::operator()(TArgs&&... args)
{
    try
    {
        try
        {
            if (m_functor)
            {
                VERIFY(m_functor.is_valid());
                if (m_object)
                {
                    VERIFY(m_object.is_valid());
                    return _return_type(
                        luabind::call_function<_return_type>(m_functor, m_object, std::forward<TArgs>(args)...));
                }
                return _return_type(luabind::call_function<_return_type>(m_functor, std::forward<TArgs>(args)...));
            }
        }
        process_error catch (std::exception&)
        {
            ai().script_engine().print_output(ai().script_engine().lua(), "", 1);
        }
    }
    catch (...)
    {
        clear();
    }
    return _return_type(0);
}

template <>
template<typename... TArgs>
void CScriptCallbackEx<void>::operator()(TArgs&&... args)
{
    try
    {
        try
        {
            if (m_functor)
            {
                VERIFY(m_functor.is_valid());
                if (m_object)
                {
                    VERIFY(m_object.is_valid());
                    luabind::call_function<void>(m_functor, m_object, std::forward<TArgs>(args)...);
                }
                luabind::call_function<void>(m_functor, std::forward<TArgs>(args)...);
            }
        }
        process_error catch (std::exception&)
        {
            ai().script_engine().print_output(ai().script_engine().lua(), "", 1);
        }
    }
    catch (...)
    {
        clear();
    }

}

TEMPLATE_SPECIALIZATION
template<typename... TArgs>
_return_type CSScriptCallbackEx::operator()(TArgs&&... args) const
{
    try
    {
        try
        {
            if (m_functor)
            {
                VERIFY(m_functor.is_valid());
                if (m_object)
                {
                    VERIFY(m_object.is_valid());
                    return _return_type(
                        luabind::call_function<_return_type>(m_functor, m_object, std::forward<TArgs>(args)...));
                }
                return _return_type(luabind::call_function<_return_type>(m_functor, std::forward<TArgs>(args)...));
            }
        }
        process_error catch (std::exception&)
        {
            ai().script_engine().print_output(ai().script_engine().lua(), "", 1);
        }
    }
    catch (...)
    {
        const_cast<CScriptCallbackEx*>(this)->clear();
    }
    return _return_type(0);
}

template<>
template<typename... TArgs>
void CScriptCallbackEx<void>::operator()(TArgs&&... args) const
{
    try
    {
        try
        {
            if (m_functor)
            {
                VERIFY(m_functor.is_valid());
                if (m_object)
                {
                    VERIFY(m_object.is_valid());
                    luabind::call_function<void>(m_functor, m_object, std::forward<TArgs>(args)...);
                }
                luabind::call_function<void>(m_functor, std::forward<TArgs>(args)...);
            }
        }
        process_error catch (std::exception&)
        {
            ai().script_engine().print_output(ai().script_engine().lua(), "", 1);
        }
    }
    catch (...)
    {
        const_cast<CScriptCallbackEx*>(this)->clear();
    }
}


#undef process_error


#undef TEMPLATE_SPECIALIZATION
#undef CSScriptCallbackEx
