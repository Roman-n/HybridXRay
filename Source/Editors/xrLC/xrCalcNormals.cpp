#include "stdafx.h"
#include "build.h"
#include "../xrLCLight/xrface.h"
#include "../xrLCLight/calculate_normals.h"
#include "../xrLCLight/xrLC_GlobalData.h"

// void
//  Performs simple cross-smooth

void CBuild::CalcNormals()
{
    calculate_normals<Vertex>::calc_normals(lc_global_data()->g_vertices(), lc_global_data()->g_faces());
    // Models
    Status("Models...");
    MU_ModelsCalculateNormals();
}
