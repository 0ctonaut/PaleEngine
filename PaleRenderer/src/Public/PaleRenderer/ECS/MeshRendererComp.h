#pragma once
#include <vector>
#include <unordered_map>
#include "PaleRenderer/Mesh/Mesh.h"
#include "PaleRenderer/Material/Material.h"

namespace PaleRdr
{
    struct SCompMeshRenderer
    {
        SCompMeshRenderer(const std::vector<std::shared_ptr<IMesh>>& vMeshes,
            const std::shared_ptr<IShader> vShader,
            bool vLit = true) :
            _Meshes(vMeshes), _bLit(vLit)
        {
            _Materials[0] = IMaterial::Create(vShader);
        };

        SCompMeshRenderer(const std::vector<std::shared_ptr<IMesh>>& vMeshes,
            const std::unordered_map<int, std::shared_ptr<IMaterial>> & vMaterials,
            bool vLit = true) : _Meshes(vMeshes), _bLit(vLit), _Materials(vMaterials)
        {
            
        };

        std::vector<std::shared_ptr<IMesh>> _Meshes;
        std::unordered_map<int, std::shared_ptr<IMaterial>> _Materials;
        bool _bLit;
    };
}