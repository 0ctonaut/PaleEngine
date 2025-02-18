#pragma once
#include <glm/glm.hpp>
namespace PaleRdr
{
    struct SCompSkybox
    {
        SCompSkybox(const std::shared_ptr<IMesh>& vMesh, 
            const std::shared_ptr<ITexture>& vTexture,
            const std::shared_ptr<IShader>& vShader)
            : _SkyboxMesh(vMesh), _SkyboxTexture(vTexture), _SkyboxShader(vShader){};

        std::shared_ptr<IMesh> _SkyboxMesh;
        std::shared_ptr<ITexture> _SkyboxTexture;
        std::shared_ptr<IShader> _SkyboxShader;
    };
}