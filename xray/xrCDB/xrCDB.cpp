// xrCDB.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "xrCDB.h"

using namespace CDB;
using namespace Opcode;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  u32  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}

// Model building
MODEL_Base::MODEL_Base	(size_t triSize) :
#ifdef PROFILE_CRITICAL_SECTIONS
	cs(MUTEX_PROFILE_ID(MODEL_Base)),
#endif // PROFILE_CRITICAL_SECTIONS
	m_triSize(triSize)
{
	tree		= 0;
	tris		= 0;
	tris_count	= 0;
	verts		= 0;
	verts_count	= 0;
	status		= S_INIT;
}
MODEL_Base::~MODEL_Base()
{
	syncronize	();		// maybe model still in building
	status		= S_INIT;
	xr_delete	(tree);
	xr_free		(tris);		tris_count = 0;
	xr_free		(verts);	verts_count= 0;
}

struct	BTHREAD_params
{
	MODEL_Base*				M;
	Fvector*			V;
	int					Vcnt;
	void*				T;
	int					Tcnt;
	build_callback*		BC;
	void*				BCP;
};

void	MODEL_Base::build_thread		(void *params)
{
	_initialize_cpu_thread		();
	FPU::m64r					();
	BTHREAD_params	P			= *( (BTHREAD_params*)params );
	P.M->cs.Enter				();
	P.M->build_internal			(P.V,P.Vcnt,P.T,P.Tcnt,P.BC,P.BCP);
	P.M->status					= S_READY;
	P.M->cs.Leave				();
	//Msg						("* xrCDB: cform build completed, memory usage: %d K",P.M->memory()/1024);
}

void	MODEL_Base::build			(Fvector* V, int Vcnt, void* T, int Tcnt, build_callback* bc, void* bcp)
{
	R_ASSERT					(S_INIT == status);
    R_ASSERT					((Vcnt>=4)&&(Tcnt>=2));

	_initialize_cpu_thread		();
#ifdef _EDITOR    
	build_internal				(V,Vcnt,T,Tcnt,bc,bcp);
#else
	if(!strstr(Core.Params, "-mt_cdb"))
	{
		build_internal				(V,Vcnt,T,Tcnt,bc,bcp);
		status						= S_READY;
	}else
	{
		BTHREAD_params				P = { this, V, Vcnt, T, Tcnt, bc, bcp };
		thread_spawn				(build_thread,"CDB-construction",0,&P);
		while						(S_INIT	== status)	Sleep	(5);
	}
#endif
}

void	MODEL_Base::build_internal	(Fvector* V, int Vcnt, void* T, int Tcnt, build_callback* bc, void* bcp)
{
	// verts
	verts_count	= Vcnt;
	verts		= xr_alloc<Fvector>	(verts_count);
	CopyMemory	(verts,V,verts_count*sizeof(Fvector));
	
	// tris
	tris_count	= Tcnt;
	tris		= allocTris(tris_count);
	CopyMemory	(tris,T,tris_count*m_triSize);

	// callback
	if (bc)		bc	(verts,Vcnt,tris,Tcnt,bcp);

	// Release data pointers
	status		= S_BUILD;
	
	// Allocate temporary "OPCODE" tris + convert tris to 'pointer' form
	u32*		temp_tris	= xr_alloc<u32>	(tris_count*3);
	if (0==temp_tris)	{
		xr_free		(verts);
		xr_free		(tris);
		return;
	}
	u32*		temp_ptr	= temp_tris;
	for (int i=0; i<tris_count; i++)
	{
		TRI_Base* tri = (TRI_Base*)((char*)tris + i * m_triSize);
		*temp_ptr++	= tri->verts[0];
		*temp_ptr++	= tri->verts[1];
		*temp_ptr++	= tri->verts[2];
	}
	
	// Build a non quantized no-leaf tree
	OPCODECREATE	OPCC;
	OPCC.NbTris		= tris_count;
	OPCC.NbVerts	= verts_count;
	OPCC.Tris		= (unsigned*)temp_tris;
	OPCC.Verts		= (Point*)verts;
	OPCC.Rules		= SPLIT_COMPLETE | SPLIT_SPLATTERPOINTS | SPLIT_GEOMCENTER;
	OPCC.NoLeaf		= true;
	OPCC.Quantized	= false;
	// if (Memory.debug_mode) OPCC.KeepOriginal = true;

	tree			= xr_new<OPCODE_Model> ();
	if (!tree->Build(OPCC)) {
		xr_free		(verts);
		xr_free		(tris);
		xr_free		(temp_tris);
		return;
	};

	// Free temporary tris
	xr_free			(temp_tris);
	return;
}

u32 MODEL_Base::memory	()
{
	if (S_BUILD==status)	{ Msg	("! xrCDB: model still isn't ready"); return 0; }
	u32 V					= verts_count*sizeof(Fvector);
	u32 T					= tris_count *m_triSize;
	return tree->GetUsedBytes()+V+T+sizeof(*this)+sizeof(*tree);
}

// This is the constructor of a class that has been exported.
// see xrCDB.h for the class definition
COLLIDER_Base::COLLIDER_Base(size_t triSize) : _triSize(triSize)
{ 
	ray_mode		= 0;
	box_mode		= 0;
	frustum_mode	= 0;
}

COLLIDER_Base::~COLLIDER_Base()
{
}

size_t COLLIDER_Base::triSize() const
{
	return _triSize;
}
