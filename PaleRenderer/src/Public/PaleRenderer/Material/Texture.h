#pragma once
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr
{
	enum class PALE_API ETexture
	{
		Diffuse,
		Specular,
		Normal
	};

	class PALE_API ITexture
	{
	public:
		ETexture Type;
		virtual unsigned int getID() const = 0;
		static std::shared_ptr<ITexture> Create(const std::string& vPath, ETexture vType);
		static std::shared_ptr<ITexture> Create(const std::filesystem::path& vPath, ETexture vType);
	};
}