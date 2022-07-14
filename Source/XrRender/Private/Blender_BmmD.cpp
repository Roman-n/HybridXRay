// BlenderDefault.cpp: implementation of the CBlender_BmmD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#include "blender_BmmD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlender_BmmD::CBlender_BmmD	()
{
	description.CLS		= B_BmmD;
	xr_strcpy				(oT2_Name,	"$null");
	xr_strcpy				(oT2_xform,	"$null");
	description.version	= 3;
	xr_strcpy				(oR_Name,	"detail\\detail_grnd_grass");	//"$null");
	xr_strcpy				(oG_Name,	"detail\\detail_grnd_asphalt");	//"$null");
	xr_strcpy				(oB_Name,	"detail\\detail_grnd_earth");	//"$null");
	xr_strcpy				(oA_Name,	"detail\\detail_grnd_yantar");	//"$null");
}

CBlender_BmmD::~CBlender_BmmD	()
{
}

void	CBlender_BmmD::Save		(IWriter& fs )
{
}

void	CBlender_BmmD::Load		(IReader& fs, u16 version )
{
}

#if RENDER==R_R1
//////////////////////////////////////////////////////////////////////////
// R1
//////////////////////////////////////////////////////////////////////////
void	CBlender_BmmD::Compile	(CBlender_Compile& C)
{

}
#elif RENDER==R_R2

//////////////////////////////////////////////////////////////////////////
// R2
//////////////////////////////////////////////////////////////////////////
#include "uber_deffer.h"
void	CBlender_BmmD::Compile	(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	// codepath is the same, only the shaders differ
	// ***only pixel shaders differ***
	string256				mask;
	strconcat				(sizeof(mask),mask,C.L_textures[0].c_str(),"_mask");
	switch(C.iElement) 
	{
	case SE_R2_NORMAL_HQ: 		// deffer
		uber_deffer		(C, true,	"impl","impl",false,oT2_Name[0]?oT2_Name:0,true);
		C.r_Sampler		("s_mask",	mask);
		C.r_Sampler		("s_lmap",	C.L_textures[1]);

		C.r_Sampler		("s_dt_r",	oR_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);
		C.r_Sampler		("s_dt_g",	oG_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);
		C.r_Sampler		("s_dt_b",	oB_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);
		C.r_Sampler		("s_dt_a",	oA_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);

		C.r_Sampler		("s_dn_r",	strconcat(sizeof(mask),mask,oR_Name,"_bump")	);
		C.r_Sampler		("s_dn_g",	strconcat(sizeof(mask),mask,oG_Name,"_bump") );
		C.r_Sampler		("s_dn_b",	strconcat(sizeof(mask),mask,oB_Name,"_bump") );
		C.r_Sampler		("s_dn_a",	strconcat(sizeof(mask),mask,oA_Name,"_bump") );

		C.r_End			();
		break;
	case SE_R2_NORMAL_LQ: 		// deffer
		uber_deffer		(C, false,	"base","impl",false,oT2_Name[0]?oT2_Name:0,true);
		C.r_Sampler		("s_lmap",	C.L_textures[1]);
		C.r_End			();
		break;
	case SE_R2_SHADOW:			// smap
		if (RImplementation.o.HW_smap)	C.r_Pass	("shadow_direct_base","dumb",				FALSE,TRUE,TRUE,FALSE);
		else							C.r_Pass	("shadow_direct_base","shadow_direct_base",	FALSE);
		C.r_Sampler		("s_base",	C.L_textures[0]);
		C.r_End			();
		break;
	}
}
#else
//////////////////////////////////////////////////////////////////////////
// R3
//////////////////////////////////////////////////////////////////////////
#include "uber_deffer.h"
void	CBlender_BmmD::Compile	(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	// codepath is the same, only the shaders differ
	// ***only pixel shaders differ***
	string256				mask;
	strconcat				(sizeof(mask),mask,C.L_textures[0].c_str(),"_mask");
	switch(C.iElement) 
	{
	case SE_R2_NORMAL_HQ: 		// deffer
		uber_deffer		(C, true,	"impl","impl",false,oT2_Name[0]?oT2_Name:0,true);
		//C.r_Sampler		("s_mask",	mask);
		//C.r_Sampler		("s_lmap",	C.L_textures[1]);

		//C.r_Sampler		("s_dt_r",	oR_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);
		//C.r_Sampler		("s_dt_g",	oG_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);
		//C.r_Sampler		("s_dt_b",	oB_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);
		//C.r_Sampler		("s_dt_a",	oA_Name,	false,	D3DTADDRESS_WRAP,	D3DTEXF_ANISOTROPIC,D3DTEXF_LINEAR,	D3DTEXF_ANISOTROPIC);

		//C.r_Sampler		("s_dn_r",	strconcat(sizeof(mask),mask,oR_Name,"_bump")	);
		//C.r_Sampler		("s_dn_g",	strconcat(sizeof(mask),mask,oG_Name,"_bump") );
		//C.r_Sampler		("s_dn_b",	strconcat(sizeof(mask),mask,oB_Name,"_bump") );
		//C.r_Sampler		("s_dn_a",	strconcat(sizeof(mask),mask,oA_Name,"_bump") );

		C.r_dx10Texture		("s_mask",	mask);
		C.r_dx10Texture		("s_lmap",	C.L_textures[1]);

		C.r_dx10Texture		("s_dt_r",	oR_Name);
		C.r_dx10Texture		("s_dt_g",	oG_Name);
		C.r_dx10Texture		("s_dt_b",	oB_Name);
		C.r_dx10Texture		("s_dt_a",	oA_Name);

		C.r_dx10Texture		("s_dn_r",	strconcat(sizeof(mask),mask,oR_Name,"_bump") );
		C.r_dx10Texture		("s_dn_g",	strconcat(sizeof(mask),mask,oG_Name,"_bump") );
		C.r_dx10Texture		("s_dn_b",	strconcat(sizeof(mask),mask,oB_Name,"_bump") );
		C.r_dx10Texture		("s_dn_a",	strconcat(sizeof(mask),mask,oA_Name,"_bump") );

		C.r_dx10Sampler		("smp_base");
		C.r_dx10Sampler		("smp_linear");

		C.r_Stencil		( TRUE,D3DCMP_ALWAYS,0xff,0x7f,D3DSTENCILOP_KEEP,D3DSTENCILOP_REPLACE,D3DSTENCILOP_KEEP);
		C.r_StencilRef	(0x01);

		C.r_End			();
		break;
	case SE_R2_NORMAL_LQ: 		// deffer
		uber_deffer		(C, false,	"base","impl",false,oT2_Name[0]?oT2_Name:0,true);

		//C.r_Sampler		("s_lmap",	C.L_textures[1]);

		C.r_dx10Texture		("s_lmap",	C.L_textures[1]);
		C.r_dx10Sampler		("smp_linear");


		C.r_Stencil		( TRUE,D3DCMP_ALWAYS,0xff,0x7f,D3DSTENCILOP_KEEP,D3DSTENCILOP_REPLACE,D3DSTENCILOP_KEEP);
		C.r_StencilRef	(0x01);

		C.r_End			();
		break;
	case SE_R2_SHADOW:			// smap
		//if (RImplementation.o.HW_smap)	C.r_Pass	("shadow_direct_base","dumb",	FALSE,TRUE,TRUE,FALSE);
		//else							C.r_Pass	("shadow_direct_base","shadow_direct_base",FALSE);
		C.r_Pass	("shadow_direct_base","dumb",	FALSE,TRUE,TRUE,FALSE);
		//C.r_Sampler		("s_base",C.L_textures[0]);
		C.r_dx10Texture		("s_base",C.L_textures[0]);
		C.r_dx10Sampler		("smp_base");
		C.r_dx10Sampler		("smp_linear");
		C.r_ColorWriteEnable(false, false, false, false);
		C.r_End			();
		break;
	}
}
#endif
