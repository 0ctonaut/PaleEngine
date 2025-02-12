#pragma once
#include <vector>
#include "PaleRenderer/OpenGL/MeshOpenGL.h"

namespace PaleRdr
{
    struct SCompMeshRenderer
    {
        SCompMeshRenderer(const std::vector<CMeshOpenGL>& meshes, const CPassOpenGL& vPass, bool vLit = true) : 
            _Meshes(meshes), _Pass(vPass), _bLit(vLit) {};

        std::vector<CMeshOpenGL> _Meshes;
        CPassOpenGL _Pass;
        bool _bLit;
    };
}