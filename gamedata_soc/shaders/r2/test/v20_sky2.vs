//
// Generated by Microsoft (R) D3DX9 Shader Compiler 9.07.239.0000
//
//   fxc /nologo /Tvs_2_0 /Emain /Zpr /Fctest\v20_sky2.vs sky2.vs
//
//
// Parameters:
//
//   row_major float4x4 m_WVP;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   m_WVP        c0       4
//

    vs_2_0
    dcl_position v0
    dcl_color v1
    dcl_texcoord v2
    dcl_texcoord1 v3
    dp4 oPos.x, c0, v0
    dp4 oPos.y, c1, v0
    dp4 oPos.z, c2, v0
    dp4 oPos.w, c3, v0
    mov oD0, v1
    mov oT0.xyz, v2
    mov oT1.xyz, v3

// approximately 7 instruction slots used
