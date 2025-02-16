#include "stdafx.h"
#include "PaleRenderer/Mesh/Mesh.h"
#include "PaleRenderer/OpenGL/MeshOpenGL.h"

namespace PaleRdr
{
    std::shared_ptr<IMesh> IMesh::Create(
        const std::vector<CVertex>& vVertices, 
        const std::vector<unsigned int>& vIndices,
        unsigned int vMaterialIndex,
        unsigned int vElementType)
    {
        return std::make_shared<CMeshOpenGL>(
            vVertices, vIndices, vMaterialIndex, vElementType);
    }
}