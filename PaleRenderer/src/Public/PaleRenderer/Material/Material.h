#pragma once
#include <unordered_map>
#include "PaleRenderer/Core/PaleRendererExport.h"
#include "PaleRenderer/Material/Shader.h"
#include "PaleRenderer/Material/Texture.h"

namespace PaleRdr
{
    class PALE_API IMaterial
    {
    public:
        virtual bool setShader(std::shared_ptr<IShader> vShader) = 0;
        virtual std::shared_ptr<IShader>& fetchShader() = 0;
        virtual std::vector<std::shared_ptr<ITexture>>& fetchTextureOfType(ETexture vType) = 0;
        virtual bool hasTextureOfType(ETexture vType) = 0;
        virtual bool addTextureOfType(ETexture vType, std::shared_ptr<ITexture> vTexture) = 0;
        static std::shared_ptr<IMaterial> Create();
        static std::shared_ptr<IMaterial> Create(std::shared_ptr<IShader> vShader);
    };

    class CMaterial : public IMaterial
    {
    public:
        CMaterial() = default;
        CMaterial(std::shared_ptr<IShader> vShader) : m_pShader(vShader) {}

        virtual inline bool setShader(std::shared_ptr<IShader> vShader) override { m_pShader = vShader; return true; }
        virtual inline std::shared_ptr<IShader>& fetchShader() override { return m_pShader; };
        virtual bool hasTextureOfType(ETexture vType) override;
        virtual std::vector<std::shared_ptr<ITexture>>& fetchTextureOfType(ETexture vType) override;
        virtual bool addTextureOfType(ETexture vType, std::shared_ptr<ITexture> vTexture) override;

    private:
        std::shared_ptr<IShader> m_pShader;
        std::unordered_map<ETexture, std::vector<std::shared_ptr<ITexture>>> m_TextureMap;
    };
}