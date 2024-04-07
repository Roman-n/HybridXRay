////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_Abstract.h
//	Created 	: 19.09.2002
//  Modified 	: 18.06.2004
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects
////////////////////////////////////////////////////////////////////////////

#ifndef xrServer_Objects_AbstractH
#define xrServer_Objects_AbstractH

#include "xrServer_Space.h"
#ifdef XRGAME_EXPORTS
#include "../xrCDB/xrCDB.h"
#else   // XRGAME_EXPORTS
#include "../xrCDB/xrCDB.h"
#endif   // XRGAME_EXPORTS
#include "ShapeData.h"

class NET_Packet;
class CDUInterface;

#ifndef _EDITOR
#ifndef XRGAME_EXPORTS
#include "../xrSound/Sound.h"
#endif
#endif

#include "../Editors/Public/xrEProps.h"
#include "DrawUtils.h"

#include "../xrServerEntities/xrServer_Objects_Abstract_Base.h"

#endif   // xrServer_Objects_AbstractH
