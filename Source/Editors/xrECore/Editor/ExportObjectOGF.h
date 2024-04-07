//---------------------------------------------------------------------------

#ifndef ExportObjectOGFH
#define ExportObjectOGFH

#include "../../xrETools/PropSlimTools.h"
//---------------------------------------------------------------------------
const int              clpOGFMX = 48, clpOGFMY = 16, clpOGFMZ = 48;
extern ECORE_API float g_EpsSkelPositionDelta;
//---------------------------------------------------------------------------
// refs
class CEditableObject;
class CEditableMesh;
class CSurface;
class CInifile;

struct SOGFVert
{
    Fvector  P;
    Fvector  N;
    Fvector2 UV;
    Fvector  T;
    Fvector  B;
    SOGFVert()
    {
        P.set(0, 0, 0);
        N.set(0, 1, 0);
        UV.set(0.f, 0.f);
        T.set(0, 0, 0);
        B.set(0, 0, 0);
    }
    void set(Fvector const& p, Fvector const& n, Fvector2 const& uv)
    {
        P.set(p);
        N.set(n);
        UV.set(uv);
    }
    BOOL similar_pos(SOGFVert& V)
    {
        return P.similar(V.P, g_EpsSkelPositionDelta);
    }
    BOOL similar(SOGFVert& V)
    {
        if (!P.similar(V.P, g_EpsSkelPositionDelta))
            return FALSE;
        if (!UV.similar(V.UV, EPS_S))
            return FALSE;
        if (!N.similar(V.N, g_EpsSkelPositionDelta))
            return FALSE;
        return TRUE;
    }
};

struct SOGFFace
{
    WORD v[3];
};

DEFINE_VECTOR(SOGFVert, OGFVertVec, OGFVertIt);
DEFINE_VECTOR(SOGFFace, OGFFaceVec, OGFFaceIt);

class CObjectOGFCollectorPacked
{
public:
    // Fobb m_OBB;
    Fbox                    m_Box;

    OGFVertVec              m_Verts;
    OGFFaceVec              m_Faces;

    // Progressive
    ArbitraryList<VIPM_SWR> m_SWR;   // The records of the collapses.

    Fvector                 m_VMmin, m_VMscale;
    U32Vec                  m_VM[clpOGFMX + 1][clpOGFMY + 1][clpOGFMZ + 1];
    Fvector                 m_VMeps;

    u16                     VPack(SOGFVert& V);
    void                    ComputeBounding();
    void                    OptimizeTextureCoordinates();

public:
    CObjectOGFCollectorPacked(const Fbox& bb, int apx_vertices, int apx_faces);
    void    CalculateTB();
    void    MakeProgressive();
    void    MakeStripify();
    IC bool check(SOGFFace& F)
    {
        if ((F.v[0] == F.v[1]) || (F.v[0] == F.v[2]) || (F.v[1] == F.v[2]))
            return false;
        else
            return true;
    }
    IC bool add_face(SOGFVert& v0, SOGFVert& v1, SOGFVert& v2, bool hq)
    {
        if (m_Verts.size() >= 0xffff || m_Faces.size() >= 0xffff)
            return false;
        if (!hq && ((v0.P.similar(v1.P, EPS) || v0.P.similar(v2.P, EPS) || v1.P.similar(v2.P, EPS))))
        {
            // ELog.Msg(mtError, "! ..Degenerate face found. Removed.");
            Msg("! ..Degenerate face found. Removed.");
            return true;
        }
        SOGFFace F;
        u16      v;
        F.v[0] = VPack(v0);
        F.v[1] = VPack(v1);
        F.v[2] = VPack(v2);

        if (hq || check(F))
            m_Faces.push_back(F);
        else
        {
            // ELog.Msg(mtError, "! ..Duplicate(degenerate) face found. Removed.");
            Msg("! ..Duplicate(degenerate) face found. Removed.");
            return true;
        }
        return true;
    }
    IC OGFVertVec& getV_Verts()
    {
        return m_Verts;
    }
    IC OGFFaceVec& getV_Faces()
    {
        return m_Faces;
    }
    IC SOGFVert* getVert()
    {
        return &m_Verts.front();
    }
    IC u32 getVS()
    {
        return m_Verts.size();
    }
    IC u32 getTS()
    {
        return m_Faces.size();
    }
};
//----------------------------------------------------
DEFINE_VECTOR(CObjectOGFCollectorPacked*, COGFCPVec, COGFCPIt)

class ECORE_API CExportObjectOGF
{
    struct SSplit
    {
        Fbox                       apx_box;

        COGFCPVec                  m_Parts;
        CObjectOGFCollectorPacked* m_CurrentPart;

        Fbox                       m_Box;
        CSurface*                  m_Surf;
        u16                        m_id;
        u16                        m_sort_id;
        shared_str                 m_Shader;
        shared_str                 m_Texture;

        // Progressive
        void                       AppendPart(int apx_vertices, int apx_faces);
        void                       SavePart(IWriter& F, CObjectOGFCollectorPacked* part);
        void                       Save(IWriter& F, int& chunk_id);

        void                       CalculateTB()
        {
            for (COGFCPIt it = m_Parts.begin(); it != m_Parts.end(); it++)
                (*it)->CalculateTB();
        }

        bool SplitStats(u32& id, size_t& verts_, size_t& faces_, bool silent)
        {
            for (COGFCPIt it = m_Parts.begin(); it != m_Parts.end(); it++)
            {
                u32 verts = (*it)->m_Verts.size();
                u32 faces = (*it)->m_Faces.size();
                verts_ += verts;
                faces_ += faces;

                if (!silent)
                {
                    if (faces == 0 || verts == 0)
                    {
                        Msg("& ..Empty split found (Texture: %s).", *m_Texture);
                        return false;
                    }
                    else
                    {
                        Msg("# ..Split %d: [Faces: %d, Verts: %d, Shader/Texture: '%s'/'%s']", id, faces, verts, *m_Shader, *m_Texture);
                    }
                    id++;
                }
            }
            return true;
        }

        void MakeProgressive();
        void MakeStripify();
        SSplit(CSurface* surf, const Fbox& bb);
        ~SSplit();
        void ComputeBounding()
        {
            m_Box.invalidate();
            for (COGFCPIt it = m_Parts.begin(); it != m_Parts.end(); it++)
            {
                CObjectOGFCollectorPacked* part = *it;
                part->ComputeBounding();
                m_Box.merge(part->m_Box);
            }
        }
        bool GetVertexBound()
        {
            u32 verts = 0;
            for (COGFCPIt it = m_Parts.begin(); it != m_Parts.end(); it++)
                verts += (*it)->m_Verts.size();

            return verts < 60000;
        }
    };
    DEFINE_VECTOR(SSplit*, SplitVec, SplitIt);
    SplitVec         m_Splits;
    CEditableObject* m_Source;
    Fbox             m_Box;
    //----------------------------------------------------
    //	void 	ComputeOBB			(Fobb &B, FvectorVec& V);
    SSplit*          FindSplit(CSurface* surf, u16 surf_id);
    void             ComputeBounding()
    {
        m_Box.invalidate();
        for (SplitIt it = m_Splits.begin(); it != m_Splits.end(); it++)
        {
            (*it)->ComputeBounding();
            m_Box.merge((*it)->m_Box);
        }
    }
    bool PrepareMESH(CEditableMesh* mesh);
    bool Prepare(bool gen_tb, CEditableMesh* mesh);

public:
    CExportObjectOGF(CEditableObject* object);
    ~CExportObjectOGF();
    bool Export(IWriter& F, bool gen_tb = true, CEditableMesh* mesh = NULL);
    bool ExportAsSimple(IWriter& F);
    bool ExportAsWavefrontOBJ(IWriter& F, LPCSTR fn);
    void DetectSmoothType(CEditableMesh* mesh, xr_vector<CEditableMesh*> mesh_vec);
};

#endif
