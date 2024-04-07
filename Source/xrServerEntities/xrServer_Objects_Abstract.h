////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_Abstract.h
//	Created 	: 19.09.2002
//  Modified 	: 18.06.2004
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "xrServer_Space.h"
#include "../xrCDB/xrCDB.h"
#include "../../xrEngine/xrISEAbstract.h"

class NET_Packet;
class CDUInterface;

#ifndef _EDITOR
#ifndef XRGAME_EXPORTS
#include "../xrSound/Sound.h"
#endif
#endif

#include "..\Editors\Public\xrEProps.h"

#ifndef XRGAME_EXPORTS
#include "DrawUtils.h"
#else
#include "DrawUtils.h"
#endif
#pragma warning(push)
#pragma warning(disable:4005)

#include "xrServer_Objects_Abstract_Base.h"
#pragma warning(pop)
