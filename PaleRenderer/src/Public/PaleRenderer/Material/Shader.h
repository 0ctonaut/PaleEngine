#pragma once
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
    enum PALE_API EShaderType
    {
        Standard,
        Transparent
    };

    class CShaderOpenGL;
    class PALE_API IShader
    {
    public:
        EShaderType Type;
        virtual void use() const = 0;

        virtual void setUniform(const std::string& name, bool value) const = 0;
        virtual void setUniform(const std::string& name, int value) const = 0;
        virtual void setUniform(const std::string& name, unsigned int value) const = 0;
        virtual void setUniform(const std::string& name, float value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec2& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec3& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::vec4& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat2& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat3& value) const = 0;
        virtual void setUniform(const std::string& name, const glm::mat4& value) const = 0;

        static std::shared_ptr<IShader> Create(const std::string& vertexPath, const std::string& fragmentPath);
        static std::shared_ptr<IShader> Create(const std::filesystem::path & vertexPath, const std::filesystem::path& fragmentPath);
    };
}