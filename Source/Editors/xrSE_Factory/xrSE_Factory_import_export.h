#ifndef XRSE_FACTORY_IMPORT_EXPORTH
#define XRSE_FACTORY_IMPORT_EXPORTH

#ifdef XRSEFACTORY_EXPORTS
#define FACTORY_API __declspec(dllexport)
#else
#define FACTORY_API __declspec(dllimport)
#endif

namespace xrSE_Factory
{
    FACTORY_API ISE_Abstract* create_entity(LPCSTR section);
    FACTORY_API void          destroy_entity(ISE_Abstract*&);
    FACTORY_API void          initialize();
    FACTORY_API void          destroy();
}   // namespace xrSE_Factory

#endif
