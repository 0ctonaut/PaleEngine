#pragma once
#include <glm/glm.hpp>
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
    class PALE_API CVertex
    {
    public:
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        glm::vec3 Tangent;
        glm::vec3 BiTangent;
    };

    class PALE_API IMesh
    {
    public:
        static std::shared_ptr<IMesh> Create(
            const std::vector<CVertex>& vVertices,
            const std::vector<unsigned int>& vIndices,
            unsigned int vMaterialIndex,
            unsigned int vElementType);
        virtual unsigned int getMaterialIndex() const = 0;
        virtual unsigned int getVAO() const = 0;
        virtual unsigned int getElementType() const = 0;
        virtual unsigned int getIndiceSize() const = 0;
    };
}