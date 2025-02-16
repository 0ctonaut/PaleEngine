#include "stdafx.h"
#include "PaleRenderer/Material/Shader.h"
#include "PaleRenderer/OpenGL/ShaderOpenGL.h"

namespace PaleRdr
{

	std::shared_ptr<IShader> IShader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		return std::make_shared<CShaderOpenGL>(vertexPath, fragmentPath);
	}
	std::shared_ptr<IShader> IShader::Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		return Create(vertexPath.string(), fragmentPath.string());
	}
}
