// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop
#if NET_TEXTURE_LOADER
#include "..\..\External\RedImage\RedImageTool\RedImage.hpp"
using namespace RedImageTool;
#endif
#pragma warning(disable:4995)
#include "directx\d3dx9.h"
#pragma warning(default:4995)

#include "directx\D3DX10Tex.h"

#include "../Private/dxRenderDeviceRender.h"

// #include "std_classes.h"
// #include "xr_avi.h"

void fix_texture_name(LPSTR fn)
{
    LPSTR _ext = strext(fn);
    if (_ext && (0 == stricmp(_ext, ".tga") || 0 == stricmp(_ext, ".dds") || 0 == stricmp(_ext, ".bmp") || 0 == stricmp(_ext, ".ogm")))
    {
        *_ext = 0;
    }
}

int get_texture_load_lod(LPCSTR fn)
{
    CInifile::Sect&   sect  = pSettings->r_section("reduce_lod_texture_list");
    CInifile::SectCIt it_   = sect.Data.begin();
    CInifile::SectCIt it_e_ = sect.Data.end();

    ENGINE_API bool   is_enough_address_space_available();
    static bool       enough_address_space_available = is_enough_address_space_available();

    CInifile::SectCIt it                             = it_;
    CInifile::SectCIt it_e                           = it_e_;

    for (; it != it_e; ++it)
    {
        if (strstr(fn, it->first.c_str()))
        {
            if (psTextureLOD < 1)
            {
                if (enough_address_space_available)
                    return 0;
                else
                    return 1;
            }
            else if (psTextureLOD < 3)
                return 1;
            else
                return 2;
        }
    }

    if (psTextureLOD < 2)
    {
        //		if ( enough_address_space_available )
        return 0;
        //		else
        //			return 1;
    }
    else if (psTextureLOD < 4)
        return 1;
    else
        return 2;
}

u32 calc_texture_size(int lod, u32 mip_cnt, size_t orig_size)
{
    if (1 == mip_cnt)
        return orig_size;

    int   _lod = lod;
    float res  = float(orig_size);

    while (_lod > 0)
    {
        --_lod;
        res -= res / 1.333f;
    }
    return iFloor(res);
}

const float _BUMPHEIGH = 8.f;
//////////////////////////////////////////////////////////////////////
// Utility pack
//////////////////////////////////////////////////////////////////////
IC u32      GetPowerOf2Plus1(u32 v)
{
    u32 cnt = 0;
    while (v)
    {
        v >>= 1;
        cnt++;
    };
    return cnt;
}
IC void Reduce(int& w, int& h, int& l, int& skip)
{
    while ((l > 1) && skip)
    {
        w /= 2;
        h /= 2;
        l -= 1;

        skip--;
    }
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
}

IC void Reduce(UINT& w, UINT& h, int l, int skip)
{
    while ((l > 1) && skip)
    {
        w /= 2;
        h /= 2;
        l -= 1;

        skip--;
    }
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
}

void TW_Save(ID3DTexture2D* T, LPCSTR name, LPCSTR prefix, LPCSTR postfix)
{
    string256 fn;
    strconcat(sizeof(fn), fn, name, "_", prefix, "-", postfix);
    for (int it = 0; it < int(xr_strlen(fn)); it++)
        if ('\\' == fn[it])
            fn[it] = '_';
    string256 fn2;
    strconcat(sizeof(fn2), fn2, "debug\\", fn, ".dds");
    Log("* debug texture save: ", fn2);
#ifdef USE_DX11
    R_CHK(D3DX11SaveTextureToFile(HW.pContext, T, D3DX11_IFF_DDS, fn2));
#else
    R_CHK(D3DX10SaveTextureToFile(T, D3DX10_IFF_DDS, fn2));
#endif
}
/*
ID3DTexture2D*	TW_LoadTextureFromTexture
(
 ID3DTexture2D*		t_from,
 D3DFORMAT&				t_dest_fmt,
 int						levels_2_skip,
 u32&					w,
 u32&					h
 )
{
    // Calculate levels & dimensions
    ID3DTexture2D*		t_dest			= NULL;
    D3DSURFACE_DESC			t_from_desc0	;
    R_CHK					(t_from->GetLevelDesc	(0,&t_from_desc0));
    int levels_exist		= t_from->GetLevelCount();
    int top_width			= t_from_desc0.Width;
    int top_height			= t_from_desc0.Height;
    Reduce					(top_width,top_height,levels_exist,levels_2_skip);

    // Create HW-surface
    if (D3DX_DEFAULT==t_dest_fmt)	t_dest_fmt = t_from_desc0.Format;
    R_CHK					(D3DXCreateTexture(
        HW.pDevice,
        top_width,top_height,
        levels_exist,0,t_dest_fmt,
        D3DPOOL_MANAGED,&t_dest
        ));

    // Copy surfaces & destroy temporary
    ID3DTexture2D* T_src= t_from;
    ID3DTexture2D* T_dst= t_dest;

    int		L_src			= T_src->GetLevelCount	()-1;
    int		L_dst			= T_dst->GetLevelCount	()-1;
    for (; L_dst>=0; L_src--,L_dst--)
    {
        // Get surfaces
        IDirect3DSurface9		*S_src, *S_dst;
        R_CHK	(T_src->GetSurfaceLevel	(L_src,&S_src));
        R_CHK	(T_dst->GetSurfaceLevel	(L_dst,&S_dst));

        // Copy
        R_CHK	(D3DXLoadSurfaceFromSurface(S_dst,NULL,NULL,S_src,NULL,NULL,D3DX_FILTER_NONE,0));

        // Release surfaces
        _RELEASE				(S_src);
        _RELEASE				(S_dst);
    }

    // OK
    w						= top_width;
    h						= top_height;
    return					t_dest;
}

template	<class _It>
IC	void	TW_Iterate_1OP
(
 ID3DTexture2D*		t_dst,
 ID3DTexture2D*		t_src,
 const _It				pred
 )
{
    DWORD mips							= t_dst->GetLevelCount();
    R_ASSERT							(mips == t_src->GetLevelCount());
    for (DWORD i = 0; i < mips; i++)	{
        D3DLOCKED_RECT				Rsrc,Rdst;
        D3DSURFACE_DESC				desc,descS;

        t_dst->GetLevelDesc			(i, &desc);
        t_src->GetLevelDesc			(i, &descS);
        VERIFY						(desc.Format==descS.Format);
        VERIFY						(desc.Format==D3DFMT_A8R8G8B8);
        t_src->LockRect				(i,&Rsrc,0,0);
        t_dst->LockRect				(i,&Rdst,0,0);
        for (u32 y = 0; y < desc.Height; y++)	{
            for (u32 x = 0; x < desc.Width; x++)	{
                DWORD&	pSrc	= *(((DWORD*)((BYTE*)Rsrc.pBits + (y * Rsrc.Pitch)))+x);
                DWORD&	pDst	= *(((DWORD*)((BYTE*)Rdst.pBits + (y * Rdst.Pitch)))+x);
                pDst			= pred(pDst,pSrc);
            }
        }
        t_dst->UnlockRect			(i);
        t_src->UnlockRect			(i);
    }
}
template	<class _It>
IC	void	TW_Iterate_2OP
(
 ID3DTexture2D*		t_dst,
 ID3DTexture2D*		t_src0,
 ID3DTexture2D*		t_src1,
 const _It				pred
 )
{
    DWORD mips							= t_dst->GetLevelCount();
    R_ASSERT							(mips == t_src0->GetLevelCount());
    R_ASSERT							(mips == t_src1->GetLevelCount());
    for (DWORD i = 0; i < mips; i++)	{
        D3DLOCKED_RECT				Rsrc0,Rsrc1,Rdst;
        D3DSURFACE_DESC				desc,descS0,descS1;

        t_dst->GetLevelDesc			(i, &desc);
        t_src0->GetLevelDesc		(i, &descS0);
        t_src1->GetLevelDesc		(i, &descS1);
        VERIFY						(desc.Format==descS0.Format);
        VERIFY						(desc.Format==descS1.Format);
        VERIFY						(desc.Format==D3DFMT_A8R8G8B8);
        t_src0->LockRect			(i,&Rsrc0,	0,0);
        t_src1->LockRect			(i,&Rsrc1,	0,0);
        t_dst->LockRect				(i,&Rdst,	0,0);
        for (u32 y = 0; y < desc.Height; y++)	{
            for (u32 x = 0; x < desc.Width; x++)	{
                DWORD&	pSrc0	= *(((DWORD*)((BYTE*)Rsrc0.pBits + (y * Rsrc0.Pitch)))+x);
                DWORD&	pSrc1	= *(((DWORD*)((BYTE*)Rsrc1.pBits + (y * Rsrc1.Pitch)))+x);
                DWORD&	pDst	= *(((DWORD*)((BYTE*)Rdst.pBits  + (y * Rdst.Pitch)))+x);
                pDst			= pred(pDst,pSrc0,pSrc1);
            }
        }
        t_dst->UnlockRect			(i);
        t_src0->UnlockRect			(i);
        t_src1->UnlockRect			(i);
    }
}

IC u32 it_gloss_rev		(u32 d, u32 s)	{	return	color_rgba	(
    color_get_A(s),		// gloss
    color_get_B(d),
    color_get_G(d),
    color_get_R(d)		);
}
IC u32 it_gloss_rev_base(u32 d, u32 s)	{
    u32		occ		= color_get_A(d)/3;
    u32		def		= 8;
    u32		gloss	= (occ*1+def*3)/4;
    return	color_rgba	(
        gloss,			// gloss
        color_get_B(d),
        color_get_G(d),
        color_get_R(d)
        );
}
IC u32 it_difference	(u32 d, u32 orig, u32 ucomp)	{	return	color_rgba(
    128+(int(color_get_R(orig))-int(color_get_R(ucomp)))*2,		// R-error
    128+(int(color_get_G(orig))-int(color_get_G(ucomp)))*2,		// G-error
    128+(int(color_get_B(orig))-int(color_get_B(ucomp)))*2,		// B-error
    128+(int(color_get_A(orig))-int(color_get_A(ucomp)))*2	);	// A-error
}
IC u32 it_height_rev	(u32 d, u32 s)	{	return	color_rgba	(
    color_get_A(d),					// diff x
    color_get_B(d),					// diff y
    color_get_G(d),					// diff z
    color_get_R(s)	);				// height
}
IC u32 it_height_rev_base(u32 d, u32 s)	{	return	color_rgba	(
    color_get_A(d),					// diff x
    color_get_B(d),					// diff y
    color_get_G(d),					// diff z
    (color_get_R(s)+color_get_G(s)+color_get_B(s))/3	);	// height
}
*/
#if NET_TEXTURE_LOADER
DXGI_FORMAT Convert(RedImage& Image)
{
    switch (Image.GetFormat())
    {
        case RedTexturePixelFormat::R8:
            return DXGI_FORMAT::DXGI_FORMAT_A8_UNORM;
            break;
        case RedTexturePixelFormat::R8G8:
            return DXGI_FORMAT::DXGI_FORMAT_R8G8_UNORM;
            break;
        case RedTexturePixelFormat::R8G8B8:
            Image.Convert(RedTexturePixelFormat::R8G8B8A8);
            return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case RedTexturePixelFormat::R8G8B8A8:
            return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case RedTexturePixelFormat::R32F:
            return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
            break;
        case RedTexturePixelFormat::R32G32F:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
            break;
        case RedTexturePixelFormat::R32G32B32F:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case RedTexturePixelFormat::R32G32B32A32F:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        case RedTexturePixelFormat::BC1:
            return DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM;
            break;
        case RedTexturePixelFormat::BC1a:
            return DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM;
            break;
        case RedTexturePixelFormat::BC2:
            return DXGI_FORMAT::DXGI_FORMAT_BC2_UNORM;
            break;
        case RedTexturePixelFormat::BC3:
            return DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM;
            break;
        case RedTexturePixelFormat::BC4:
            return DXGI_FORMAT::DXGI_FORMAT_BC4_UNORM;
            break;
        case RedTexturePixelFormat::BC5:
            return DXGI_FORMAT::DXGI_FORMAT_BC5_UNORM;
            break;
        case RedTexturePixelFormat::BC6:
#ifndef USE_DX11
            R_ASSERT(!"BC6 DX10 не поддерживает");
#endif
            return DXGI_FORMAT::DXGI_FORMAT_BC6H_SF16;
            break;
        case RedTexturePixelFormat::BC7:
#ifndef USE_DX11
            R_ASSERT(!"BC7 DX10 не поддерживает");
#endif
            return DXGI_FORMAT::DXGI_FORMAT_BC7_UNORM;
            break;
        default:
            R_ASSERT(!"Неизвестный формат текстуры");
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
            break;
    }
}
ID3DBaseTexture* CRender::texture_load(LPCSTR fRName, u32& ret_msize, bool bStaging)
{
    static bool bAllowStaging = !strstr(Core.Params, "-no_staging");
    bStaging &= bAllowStaging;
    ID3DTexture2D* Texture2D = NULL;
    ID3DTexture3D* Texture3D = NULL;
    R_ASSERT(fRName);
    R_ASSERT(fRName[0]);
    string_path fn;
    // make file name
    string_path fname;
    xr_strcpy(fname, fRName);   //. andy if (strext(fname)) *strext(fname)=0;
    fix_texture_name(fname);
    IReader* S = NULL;
    if (FS.exist(fn, "$level$", fname, ".dds"))
        goto _DDS;
    if (FS.exist(fn, "$game_saves$", fname, ".dds"))
        goto _DDS;
    if (FS.exist(fn, "$game_textures$", fname, ".dds"))
        goto _DDS;
    // if (FS.exist(fn,"$game_textures$",fname,	".dds")	&& strstr(fname,"_bump"))	goto _BUMP;
    bool        IsBump = false;
    DXGI_FORMAT Format;
    if (strstr(fname, "_bump"))
    {
        if (!FS.exist(fn, "$game_textures$", fname, ".dds"))
        {
            Msg("! Fallback to default bump map: %s", fname);
            //////////////////
            if (strstr(fname, "_bump#"))
            {
                R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump#", ".dds"), "ed_dummy_bump#");
            }
            if (strstr(fname, "_bump"))
            {
                R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump", ".dds"), "ed_dummy_bump");
            }
        }
        IsBump = true;
    }
    else
    {
        if (!FS.exist(fn, "$level$", fname, ".dds"))
            if (!FS.exist(fn, "$game_saves$", fname, ".dds"))
                if (!FS.exist(fn, "$game_textures$", fname, ".dds"))
                {
                    string_path temp;
                    R_ASSERT(FS.exist(temp, "$game_textures$", "ed\\ed_not_existing_texture", ".dds"));
                    R_ASSERT(xr_strcmp(temp, fn));
                    xr_strcpy(fn, temp);
                }
    }
Load:
{
    S = FS.r_open(fn);
    R_ASSERT2(S, fn);
    RedImage Image;
    if (!Image.LoadFromMemory(S->pointer(), S->length()))
    {
        Msg("! Can't get image info for texture '%s'", fn);
        FS.r_close(S);
        string_path temp;
        R_ASSERT(FS.exist(temp, "$game_textures$", "ed\\ed_not_existing_texture", ".dds"));
        R_ASSERT(xr_strcmp(temp, fn));
        xr_strcpy(fn, temp);
        goto Load;
    }
    /*if (strstr(fn, "d:\\gamedev\\engine\\xrayengine\\xrayengine\\gamedata\\textures\\ui\\ui_font_graff_22_1024.dds"))
    {
        Image.SaveToPng("test.png");
    }*/
    FS.r_close(S);

    Format    = Convert(Image);

    ret_msize = Image.GetSizeInMemory();
    if (Image.IsCubeMap())
    {
        R_ASSERT(IsBump == false);
        R_ASSERT(Image.GetDepth() == 6);
        D3D_TEXTURE2D_DESC Desc = {};
        Desc.Width              = Image.GetWidth();
        Desc.Height             = Image.GetHeight();
        Desc.MipLevels          = Image.GetMips();
        Desc.ArraySize          = 6;
        Desc.Format             = Format;
        Desc.SampleDesc.Count   = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.Usage              = D3D_USAGE_DEFAULT;
        Desc.BindFlags          = D3D_BIND_SHADER_RESOURCE;
        Desc.CPUAccessFlags     = 0;
#ifdef USE_DX11
        Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
#else
        Desc.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;
#endif
        xr_vector<D3D_SUBRESOURCE_DATA> SubResources;
        u8*                             Pointer = (u8*)(*Image);
        for (UINT i = 0; i < 6; i++)
        {
            for (UINT a = 0; a < Desc.MipLevels; a++)
            {
                size_t               MipW        = RedTextureUtils::GetMip(Desc.Width, a);
                size_t               MipH        = RedTextureUtils::GetMip(Desc.Height, a);
                D3D_SUBRESOURCE_DATA SubResource = {};
                SubResource.pSysMem              = Pointer;
                SubResource.SysMemPitch          = static_cast<UINT>(RedTextureUtils::GetSizeWidth(MipW, Image.GetFormat()));
                SubResource.SysMemSlicePitch     = static_cast<UINT>(RedTextureUtils::GetSizeDepth(MipW, MipH, Image.GetFormat()));
                Pointer += SubResource.SysMemSlicePitch;
                SubResources.push_back(SubResource);
            }
        }
        R_CHK(HW.pDevice->CreateTexture2D(&Desc, SubResources.data(), &Texture2D));
        return Texture2D;
    }
    else if (Image.GetDepth() > 1)
    {
        D3D_TEXTURE3D_DESC Desc = {};
        Desc.Width              = Image.GetWidth();
        Desc.Height             = Image.GetHeight();
        Desc.MipLevels          = Image.GetMips();
        Desc.Depth              = Image.GetDepth();
        ;
        Desc.Format         = Format;
        Desc.Usage          = D3D_USAGE_DEFAULT;
        Desc.BindFlags      = D3D_BIND_SHADER_RESOURCE;
        Desc.CPUAccessFlags = 0;
        Desc.MiscFlags      = 0;
        xr_vector<D3D_SUBRESOURCE_DATA> SubResources;
        u8*                             Pointer = (u8*)(*Image);
        for (UINT i = 0; i < Image.GetDepth(); i++)
        {
            for (UINT a = 0; a < Desc.MipLevels; a++)
            {
                size_t               MipW        = RedTextureUtils::GetMip(Desc.Width, a);
                size_t               MipH        = RedTextureUtils::GetMip(Desc.Height, a);
                D3D_SUBRESOURCE_DATA SubResource = {};
                SubResource.pSysMem              = Pointer;
                SubResource.SysMemPitch          = static_cast<UINT>(RedTextureUtils::GetSizeWidth(MipW, Image.GetFormat()));
                SubResource.SysMemSlicePitch     = static_cast<UINT>(RedTextureUtils::GetSizeDepth(MipW, MipH, Image.GetFormat()));
                Pointer += SubResource.SysMemSlicePitch;
                SubResources.push_back(SubResource);
            }
        }
        R_CHK(HW.pDevice->CreateTexture3D(&Desc, SubResources.data(), &Texture3D));
        return Texture3D;
    }
    else
    {
        D3D_TEXTURE2D_DESC Desc = {};
        Desc.Width              = Image.GetWidth();
        Desc.Height             = Image.GetHeight();
        Desc.MipLevels          = Image.GetMips();
        Desc.ArraySize          = Image.GetDepth();
        ;
        Desc.Format             = Format;
        Desc.SampleDesc.Count   = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.Usage              = D3D_USAGE_DEFAULT;
        Desc.BindFlags          = D3D_BIND_SHADER_RESOURCE;
        Desc.CPUAccessFlags     = 0;
        Desc.MiscFlags          = 0;
        if (bStaging)
        {
            Desc.Usage          = D3D_USAGE_STAGING;
            Desc.BindFlags      = 0;
            Desc.CPUAccessFlags = D3D_CPU_ACCESS_WRITE;
        }
        xr_vector<D3D_SUBRESOURCE_DATA> SubResources;
        u8*                             Pointer = (u8*)(*Image);
        for (UINT i = 0; i < Image.GetDepth(); i++)
        {
            for (UINT a = 0; a < Desc.MipLevels; a++)
            {
                size_t               MipW        = RedTextureUtils::GetMip(Desc.Width, a);
                size_t               MipH        = RedTextureUtils::GetMip(Desc.Height, a);
                D3D_SUBRESOURCE_DATA SubResource = {};
                SubResource.pSysMem              = Pointer;
                SubResource.SysMemPitch          = static_cast<UINT>(RedTextureUtils::GetSizeWidth(MipW, Image.GetFormat()));
                SubResource.SysMemSlicePitch     = static_cast<UINT>(RedTextureUtils::GetSizeDepth(MipW, MipH, Image.GetFormat()));
                Pointer += SubResource.SysMemSlicePitch;
                SubResources.push_back(SubResource);
            }
        }
        R_CHK(HW.pDevice->CreateTexture2D(&Desc, SubResources.data(), &Texture2D));
        return Texture2D;
    }
}

//  Moved here just to avoid warning
#ifdef USE_DX11
    D3DX11_IMAGE_INFO IMG;
#else
    D3DX10_IMAGE_INFO IMG;
#endif
    ZeroMemory(&IMG, sizeof(IMG));

    ID3DBaseTexture* pTexture2D     = NULL;
    size_t           img_size       = 0;
    int              img_loaded_lod = 0;
    u32              mip_cnt        = u32(-1);

_DDS:
{
    // Load and get header
    S = FS.r_open(fn);
#ifdef DEBUG
    Msg("* Loaded: %s[%d]b", fn, S->length());
#endif   // DEBUG
    img_size = S->length();
    R_ASSERT(S);
#ifdef USE_DX11
    R_CHK2(D3DX11GetImageInfoFromMemory(S->pointer(), S->length(), 0, &IMG, 0), fn);
#else
    R_CHK2(D3DX10GetImageInfoFromMemory(S->pointer(), S->length(), 0, &IMG, 0), fn);
#endif
    if (IMG.MiscFlags & D3D_RESOURCE_MISC_TEXTURECUBE)
        goto _DDS_CUBE;
    else
        goto _DDS_2D;

_DDS_CUBE:
{
    //	Inited to default by provided default constructor
#ifdef USE_DX11
    D3DX11_IMAGE_LOAD_INFO LoadInfo;
#else
    D3DX10_IMAGE_LOAD_INFO LoadInfo;
#endif
    LoadInfo.Usage = D3D_USAGE_IMMUTABLE;
    if (bStaging)
    {
        LoadInfo.Usage          = D3D_USAGE_STAGING;
        LoadInfo.BindFlags      = 0;
        LoadInfo.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
    }
    else
    {
        LoadInfo.Usage     = D3D_USAGE_DEFAULT;
        LoadInfo.BindFlags = D3D_BIND_SHADER_RESOURCE;
    }
    LoadInfo.pSrcInfo = &IMG;

#ifdef USE_DX11
    R_CHK(D3DX11CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0));
#else
    R_CHK(D3DX10CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0));
#endif
    FS.r_close(S);
    // OK
    mip_cnt   = IMG.MipLevels;
    ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
    return pTexture2D;
}
_DDS_2D:
{
    // Check for LMAP and compress if needed
    strlwr(fn);
    img_loaded_lod = get_texture_load_lod(fn);
    //	Inited to default by provided default constructor
#ifdef USE_DX11
    D3DX11_IMAGE_LOAD_INFO LoadInfo;
#else
    D3DX10_IMAGE_LOAD_INFO LoadInfo;
#endif
    // LoadInfo.FirstMipLevel = img_loaded_lod;
    LoadInfo.Width  = IMG.Width;
    LoadInfo.Height = IMG.Height;

    if (img_loaded_lod)
    {
        Reduce(LoadInfo.Width, LoadInfo.Height, IMG.MipLevels, img_loaded_lod);
    }
    LoadInfo.Usage = D3D_USAGE_IMMUTABLE;
    if (bStaging)
    {
        LoadInfo.Usage          = D3D_USAGE_STAGING;
        LoadInfo.BindFlags      = 0;
        LoadInfo.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
    }
    else
    {
        LoadInfo.Usage     = D3D_USAGE_DEFAULT;
        LoadInfo.BindFlags = D3D_BIND_SHADER_RESOURCE;
    }
    LoadInfo.pSrcInfo = &IMG;

#ifdef USE_DX11
    R_CHK2(D3DX11CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0), fn);
#else
    R_CHK2(D3DX10CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0), fn);
#endif
    FS.r_close(S);
    mip_cnt   = IMG.MipLevels;
    // OK
    ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
    return pTexture2D;
}
}
}
#else
ID3DBaseTexture* CRender::texture_load(LPCSTR fRName, u32& ret_msize, bool bStaging)
{
//  Moved here just to avoid warning
#ifdef USE_DX11
    D3DX11_IMAGE_INFO IMG;
#else
    D3DX10_IMAGE_INFO IMG;
#endif
    ZeroMemory(&IMG, sizeof(IMG));

    //	Staging control
    static bool bAllowStaging = !strstr(Core.Params, "-no_staging");
    bStaging &= bAllowStaging;

    ID3DBaseTexture* pTexture2D = NULL;
    // IDirect3DCubeTexture9*	pTextureCUBE	= NULL;
    string_path      fn;
    // u32						dwWidth,dwHeight;
    size_t           img_size       = 0;
    int              img_loaded_lod = 0;
    // D3DFORMAT				fmt;
    u32              mip_cnt        = u32(-1);
    // validation
    R_ASSERT(fRName);
    R_ASSERT(fRName[0]);

    // make file name
    string_path fname;
    xr_strcpy(fname, fRName);   //. andy if (strext(fname)) *strext(fname)=0;
    fix_texture_name(fname);
    IReader* S = NULL;
    if (!FS.exist(fn, "$game_textures$", fname, ".dds") && strstr(fname, "_bump"))
        goto _BUMP_from_base;
    if (FS.exist(fn, "$level$", fname, ".dds"))
        goto _DDS;
    if (FS.exist(fn, "$game_saves$", fname, ".dds"))
        goto _DDS;
    if (FS.exist(fn, "$game_textures$", fname, ".dds"))
        goto _DDS;

#ifdef _EDITOR
    ELog.Msg(mtError, "! Can't find texture '%s'", fname);
    return 0;
#else

    Msg("! Can't find texture '%s'", fname);
    R_ASSERT(FS.exist(fn, "$game_textures$", "ed\\ed_not_existing_texture", ".dds"));
    goto _DDS;

    //	Debug.fatal(DEBUG_INFO,"Can't find texture '%s'",fname);

#endif

_DDS:
{
    // Load and get header

    S = FS.r_open(fn);
#ifdef DEBUG
    Msg("* Loaded: %s[%d]b", fn, S->length());
#endif   // DEBUG
    img_size = S->length();
    R_ASSERT(S);
    // R_CHK2					(D3DXGetImageInfoFromFileInMemory	(S->pointer(),S->length(),&IMG), fn);
#ifdef USE_DX11
    R_CHK2(D3DX11GetImageInfoFromMemory(S->pointer(), S->length(), 0, &IMG, 0), fn);
#else
    R_CHK2(D3DX10GetImageInfoFromMemory(S->pointer(), S->length(), 0, &IMG, 0), fn);
#endif
    // if (IMG.ResourceType	== D3DRTYPE_CUBETEXTURE)			goto _DDS_CUBE;
    if (IMG.MiscFlags & D3D_RESOURCE_MISC_TEXTURECUBE)
        goto _DDS_CUBE;
    else
        goto _DDS_2D;

_DDS_CUBE:
{
    // R_CHK(D3DXCreateCubeTextureFromFileInMemoryEx(
    //	HW.pDevice,
    //	S->pointer(),S->length(),
    //	D3DX_DEFAULT,
    //	IMG.MipLevels,0,
    //	IMG.Format,
    //	D3DPOOL_MANAGED,
    //	D3DX_DEFAULT,
    //	D3DX_DEFAULT,
    //	0,&IMG,0,
    //	&pTextureCUBE
    //	));

    //	Inited to default by provided default constructor
#ifdef USE_DX11
    D3DX11_IMAGE_LOAD_INFO LoadInfo;
#else
    D3DX10_IMAGE_LOAD_INFO LoadInfo;
#endif
    LoadInfo.Usage = D3D_USAGE_IMMUTABLE;
    if (bStaging)
    {
        LoadInfo.Usage          = D3D_USAGE_STAGING;
        LoadInfo.BindFlags      = 0;
        LoadInfo.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
    }
    else
    {
        LoadInfo.Usage     = D3D_USAGE_DEFAULT;
        LoadInfo.BindFlags = D3D_BIND_SHADER_RESOURCE;
    }

    LoadInfo.pSrcInfo = &IMG;

#ifdef USE_DX11
    R_CHK(D3DX11CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0));
#else
    R_CHK(D3DX10CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0));
#endif

    FS.r_close(S);

    // OK
    mip_cnt   = IMG.MipLevels;
    ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
    return pTexture2D;
}
_DDS_2D:
{
    // Check for LMAP and compress if needed
    strlwr(fn);

    // Load   SYS-MEM-surface, bound to device restrictions
    // ID3DTexture2D*		T_sysmem;
    // R_CHK2(D3DXCreateTextureFromFileInMemoryEx
    //	(
    //	HW.pDevice,S->pointer(),S->length(),
    //	D3DX_DEFAULT,D3DX_DEFAULT,
    //	IMG.MipLevels,0,
    //	IMG.Format,
    //	D3DPOOL_SYSTEMMEM,
    //	D3DX_DEFAULT,
    //	D3DX_DEFAULT,
    //	0,&IMG,0,
    //	&T_sysmem
    //	), fn);

    img_loaded_lod = get_texture_load_lod(fn);

    //	Inited to default by provided default constructor
#ifdef USE_DX11
    D3DX11_IMAGE_LOAD_INFO LoadInfo;
#else
    D3DX10_IMAGE_LOAD_INFO LoadInfo;
#endif
    // LoadInfo.FirstMipLevel = img_loaded_lod;
    LoadInfo.Width  = IMG.Width;
    LoadInfo.Height = IMG.Height;

    if (img_loaded_lod)
    {
        Reduce(LoadInfo.Width, LoadInfo.Height, IMG.MipLevels, img_loaded_lod);
    }

    LoadInfo.Usage = D3D_USAGE_IMMUTABLE;
    if (bStaging)
    {
        LoadInfo.Usage          = D3D_USAGE_STAGING;
        LoadInfo.BindFlags      = 0;
        LoadInfo.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
    }
    else
    {
        LoadInfo.Usage     = D3D_USAGE_DEFAULT;
        LoadInfo.BindFlags = D3D_BIND_SHADER_RESOURCE;
    }
    LoadInfo.pSrcInfo = &IMG;

#ifdef USE_DX11
    R_CHK2(D3DX11CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0), fn);
#else
    R_CHK2(D3DX10CreateTextureFromMemory(HW.pDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0), fn);
#endif
    FS.r_close(S);
    mip_cnt   = IMG.MipLevels;
    // OK
    ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
    return pTexture2D;
}
}

_BUMP_from_base:
{
    // Msg			("! auto-generated bump map: %s",fname);
    Msg("! Fallback to default bump map: %s", fname);
    //////////////////
    if (strstr(fname, "_bump#"))
    {
        R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump#", ".dds"), "ed_dummy_bump#");
        S = FS.r_open(fn);
        R_ASSERT2(S, fn);
        img_size = S->length();
        goto _DDS_2D;
    }
    if (strstr(fname, "_bump"))
    {
        R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump", ".dds"), "ed_dummy_bump");
        S = FS.r_open(fn);

        R_ASSERT2(S, fn);

        img_size = S->length();
        goto _DDS_2D;
    }
    //////////////////
}

    return 0;
}
#endif
