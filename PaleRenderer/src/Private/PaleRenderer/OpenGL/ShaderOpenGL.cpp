#include "stdafx.h"
#include "PaleRenderer/OpenGL/ShaderOpenGL.h"

namespace PaleRdr
{
	CShaderOpenGL::CShaderOpenGL(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            PALE_RDR_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", e.what());
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        __checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        __checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        __checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    CShaderOpenGL::CShaderOpenGL(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) :
        CShaderOpenGL(vertexPath.string().c_str(), fragmentPath.string().c_str())
    {
    }

    CShaderOpenGL::CShaderOpenGL(const CShaderOpenGL& vOther)
    {
        ID = vOther.ID;
    }

    CShaderOpenGL::~CShaderOpenGL()
    {
        glDeleteProgram(ID);
    }

    void CShaderOpenGL::use() const
    {
        glUseProgram(ID);
    }

    void CShaderOpenGL::setUniform(const std::string& name, bool value) const {
        glUniform1i(getUniformLocation(name), static_cast<int>(value));
    }

    void CShaderOpenGL::setUniform(const std::string& name, int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    void CShaderOpenGL::setUniform(const std::string& name, unsigned int value) const {
        glUniform1ui(getUniformLocation(name), value);
    }

    void CShaderOpenGL::setUniform(const std::string& name, float value) const {
        glUniform1f(getUniformLocation(name), value);
    }

    void CShaderOpenGL::setUniform(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(getUniformLocation(name), 1, &value[0]);
    }

    void CShaderOpenGL::setUniform(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(getUniformLocation(name), 1, &value[0]);
    }

    void CShaderOpenGL::setUniform(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(getUniformLocation(name), 1, &value[0]);
    }

    void CShaderOpenGL::setUniform(const std::string& name, const glm::mat2& value) const {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void CShaderOpenGL::setUniform(const std::string& name, const glm::mat3& value) const {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void CShaderOpenGL::setUniform(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void CShaderOpenGL::__checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                PALE_RDR_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {} {}", type, infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                PALE_RDR_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {} {}", type, infoLog);
            }
        }
    }
    int CShaderOpenGL::getUniformLocation(const std::string& name) const
    {
        return glGetUniformLocation(ID, name.c_str());
    }
}