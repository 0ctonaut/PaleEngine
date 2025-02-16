#pragma once
#include "PaleRenderer/Material/Shader.h"

namespace PaleRdr
{
    //namespace glm
    //{
    //    struct vec2;
    //    struct vec3;
    //    struct vec4;
    //    struct mat2;
    //    struct mat3;
    //    struct mat4;
    //}

    class CShaderOpenGL : public IShader
    {
    public:
        CShaderOpenGL(const char* vertexPath, const char* fragmentPath);
        CShaderOpenGL(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        CShaderOpenGL(const CShaderOpenGL& vOther);
        ~CShaderOpenGL();

        virtual void use() const override;

        virtual void setUniform(const std::string& name, bool value) const override;
        virtual void setUniform(const std::string& name, int value) const override;
        virtual void setUniform(const std::string& name, unsigned int value) const override;
        virtual void setUniform(const std::string& name, float value) const override;
        virtual void setUniform(const std::string& name, const glm::vec2& value) const override;
        virtual void setUniform(const std::string& name, const glm::vec3& value) const override;
        virtual void setUniform(const std::string& name, const glm::vec4& value) const override;
        virtual void setUniform(const std::string& name, const glm::mat2& value) const override;
        virtual void setUniform(const std::string& name, const glm::mat3& value) const override;
        virtual void setUniform(const std::string& name, const glm::mat4& value) const override;
    public:
        unsigned int ID;

    private:
        void __checkCompileErrors(GLuint shader, std::string type);
        int getUniformLocation(const std::string& name) const;
    };
}