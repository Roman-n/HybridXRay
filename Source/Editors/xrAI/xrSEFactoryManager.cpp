#include "stdafx.h"
#include "xrSEFactoryManager.h"

xrSEFactoryManager* g_SEFactoryManager = nullptr;

xrSEFactoryManager::xrSEFactoryManager()
{
    auto        a    = xrGameManager::GetGame();
    const char* Name = "xrSE_Factory.dll";
    switch (a)
    {
        case EGame::COP:
            break;
        case EGame::CS:
            Name = "xrSE_FactoryCS.dll";
            break;
        case EGame::SHOC:
            Name = "xrSE_FactorySOC.dll";
            break;
        default:
            NODEFAULT;
    }
    m_Module = LoadLibrary(Name);
    R_CHK(GetLastError());
    R_ASSERT(m_Module);
    m_pFInitialize = (void(__cdecl*)(void))GetProcAddress(m_Module, "initialize");
    R_ASSERT(m_pFInitialize);
    m_pFDestroy = (void(__cdecl*)(void))GetProcAddress(m_Module, "destroy");
    R_ASSERT(m_pFDestroy);
    m_pFCreateEntity = (ISE_Abstract * (__cdecl*)(LPCSTR)) GetProcAddress(m_Module, "create_entity");
    R_ASSERT(m_pFCreateEntity);
    m_pFDestroyEntity = (void(__cdecl*)(ISE_Abstract*&))GetProcAddress(m_Module, "destroy_entity");
    R_ASSERT(m_pFDestroyEntity);
    m_pFInitialize();
}

xrSEFactoryManager::~xrSEFactoryManager()
{
    m_pFDestroy();
    FreeLibrary(m_Module);
}

ISE_Abstract* xrSEFactoryManager::create_entity(LPCSTR section)
{
    return m_pFCreateEntity(section);
}

void xrSEFactoryManager::destroy_entity(ISE_Abstract*& abstract)
{
    m_pFDestroyEntity(abstract);
}
