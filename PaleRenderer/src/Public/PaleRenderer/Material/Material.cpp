#include "stdafx.h"
#include "PaleRenderer/Material/Material.h"

namespace PaleRdr
{
    std::shared_ptr<IMaterial> IMaterial::Create()
    {
        return std::make_shared<CMaterial>();
    }

    std::shared_ptr<IMaterial> IMaterial::Create(std::shared_ptr<IShader> vShader)
    {
        return std::make_shared<CMaterial>(vShader);
    }

    bool CMaterial::hasTextureOfType(ETexture vType)
    {
        return (m_TextureMap.find(vType) != m_TextureMap.end());
    }

    std::vector<std::shared_ptr<ITexture>>& CMaterial::fetchTextureOfType(ETexture vType)
    {
        if (m_TextureMap.find(vType) == m_TextureMap.end())
        {
            PALE_RDR_WARN("This material doesn't have type {} texture now", static_cast<int>(vType));
            m_TextureMap[vType] = std::vector<std::shared_ptr<ITexture>>();
        }
        return m_TextureMap[vType];
    }
    bool CMaterial::addTextureOfType(ETexture vType, std::shared_ptr<ITexture> vTexture)
    {
        if (m_TextureMap.find(vType) == m_TextureMap.end())
        {
            m_TextureMap[vType] = std::vector<std::shared_ptr<ITexture>>();
        }
        if (vType != vTexture->Type)
        {
            PALE_RDR_WARN("You are adding texture of type {} as type {}", static_cast<int>(vTexture->Type), static_cast<int>(vType));
        }
        m_TextureMap[vType].push_back(vTexture);
        return true;
    }

}