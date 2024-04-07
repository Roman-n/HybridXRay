#include "stdafx.h"
#include "xrGameManager.h"
#include "xrAPI.h"

const EGamePath GCurrentPath = []
{ 
  if (strstr(GetCommandLine(), "-soc_14") || strstr(GetCommandLine(), "-soc_10004"))
  {
    return EGamePath::SHOC_10004;
  }
  else if (strstr(GetCommandLine(), "-soc"))
  {
    return EGamePath::SHOC_10006;
  }
  else if (strstr(GetCommandLine(), "-cs"))
  {
    return EGamePath::CS_1510;
  }
  else
  {
    return EGamePath::COP_1602;
  }
}();

EGame xrGameManager::GetGame()
{
    switch (GCurrentPath)
    {
        case EGamePath::COP_1602:
            return EGame::COP;
        case EGamePath::CS_1510:
            return EGame::CS;
        case EGamePath::SHOC_10006:
        case EGamePath::SHOC_10004:
            return EGame::SHOC;
        default:
            return EGame::COP;
    }
}

EGamePath xrGameManager::GetPath()
{
    return GCurrentPath;
}
