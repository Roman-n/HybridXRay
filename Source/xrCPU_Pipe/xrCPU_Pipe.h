#ifndef xrCPU_PipeH
#define xrCPU_PipeH
#pragma once

// Forward references
struct vertRender;
struct vertBoned1W;
struct vertBoned2W;
struct vertBoned3W;
struct vertBoned4W;
class CBoneInstance;
class light;
class xrDeviceInterface;

// Skinning processor specific functions
// NOTE: Destination memory is uncacheble write-combining (AGP), so avoid non-linear writes
// D: AGP,			32b aligned
// S: SysMem		non-aligned
// Bones: SysMem	64b aligned

typedef void xrSkin1W(vertRender* D, vertBoned1W* S, u32 vCount, CBoneInstance* Bones);
typedef void xrSkin2W(vertRender* D, vertBoned2W* S, u32 vCount, CBoneInstance* Bones);
typedef void xrSkin3W(vertRender* D, vertBoned3W* S, u32 vCount, CBoneInstance* Bones);
typedef void xrSkin4W(vertRender* D, vertBoned4W* S, u32 vCount, CBoneInstance* Bones);

typedef void xrPLC_calc3(int& c0, int& c1, int& c2, xrDeviceInterface& Device, Fvector* P, Fvector& N, light* L, float energy, Fvector& O);

#pragma pack(push, 8)
struct xrDispatchTable
{
    xrSkin1W*    skin1W;
    xrSkin2W*    skin2W;
    xrSkin3W*    skin3W;
    xrSkin4W*    skin4W;
    xrPLC_calc3* PLC_calc3;
};
#pragma pack(pop)

// Binder
// NOTE: Engine calls function named "_xrBindPSGP"
typedef void __cdecl xrBinder(xrDispatchTable* T, u32 dwFeatures);

#endif
