#pragma once
#include <glm/glm.hpp>
namespace PaleRdr
{
    struct SCompSkybox
    {
        SCompSkybox(const std::shared_ptr<IMesh>& vMesh, 
            const std::shared_ptr<IShader>& vShader,
            const std::shared_ptr<ITexture>& vSkyboxTexture,
            const std::shared_ptr<ITexture>& vIrradiance,
            const std::shared_ptr<ITexture>& vPrefilterMap,
            const std::shared_ptr<ITexture>& vBRDFLUT)
            : _SkyboxMesh(vMesh), _SkyboxShader(vShader),
            _SkyboxTexture(vSkyboxTexture),
            _Irradiance(vIrradiance),
            _PrefilterMap(vPrefilterMap),
            _BRDFLUT(vBRDFLUT)
        {};

        std::shared_ptr<IMesh> _SkyboxMesh;
        std::shared_ptr<IShader> _SkyboxShader;
        std::shared_ptr<ITexture> _SkyboxTexture;
        std::shared_ptr<ITexture> _Irradiance;
        std::shared_ptr<ITexture> _PrefilterMap;
        std::shared_ptr<ITexture> _BRDFLUT;
    };
}