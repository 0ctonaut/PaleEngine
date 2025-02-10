#include <vector>
#include "PaleRenderer/OpenGL/MeshOpenGL.h"

namespace PaleRdr
{
    struct SCompMeshRenderer
    {
        SCompMeshRenderer(const std::vector<CMeshOpenGL>& meshes, const CPassOpenGL& vPass) : 
            _Meshes(meshes), _Pass(vPass){};

        std::vector<CMeshOpenGL> _Meshes;
        CPassOpenGL _Pass;
    };
}