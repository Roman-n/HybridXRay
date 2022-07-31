#ifndef EDetailModelH
#define EDetailModelH
#pragma once

#include "DetailModel.h"
extern ECORE_API float g_EpsSkelPositionDelta;

class CEditableObject;

class ECORE_API EDetail: public CDetail{
	friend class EDetailManager;
	friend class CDetailManager;
    friend class UIDOShuffle;

    struct EVertexIn: public fvfVertexIn
    {
        				EVertexIn	(const Fvector& _P, float _u, float _v){P.set(_P); u=_u; v=_v;};
        IC void			set			(EVertexIn& src){P.set(src.P); u=src.u; v=src.v;};
        IC void			set			(const Fvector& _P, float _u, float _v){P.set(_P); u=_u; v=_v;};
        IC BOOL			similar		(EVertexIn& V)
        {
            if (!fsimilar    (u,V.u,EPS_S))    return FALSE;
            if (!fsimilar    (v,V.v,EPS_S))    return FALSE;
            if (!P.similar   (V.P,g_EpsSkelPositionDelta))    return FALSE;
            return TRUE;
        }
        void            remapUV        (const fvfVertexIn& src, const Fvector2& offs, const Fvector2& scale, bool bRotate);
    };

    float 				m_fDensityFactor;

    // references
    xr_string			m_sRefs;
    CEditableObject*	m_pRefs;

	u16 				_AddVert		(const Fvector& p, float u, float v);
public:
						EDetail			();
	virtual             ~EDetail		();

	bool				Load            (IReader&);
	void				Save            (IWriter&);
    void                Export          (IWriter&, LPCSTR tex_name, const Fvector2& offs, const Fvector2& scale, bool rot);
    void				Export			(LPCSTR name);

	bool				Update			(LPCSTR name, float scale, CEditableObject* ref);

    LPCSTR				GetName			();
    LPCSTR				GetTextureName	();
};
DEFINE_VECTOR(EDetail*,DOVec,DOIt);

#endif //_INCDEF_DetailModel_H_

