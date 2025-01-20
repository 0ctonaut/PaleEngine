#pragma once
#include <filesystem>
#include "Common/Singleton.h"
class CPathManager : public Singleton<CPathManager>
{
	friend class Singleton<CPathManager>;
public:
	std::filesystem::path getRendererRoot()
	{
		return m_RendererRoot;
	}

private:
	CPathManager()
	{
		m_RendererRoot = std::filesystem::path(RENDERER_ROOT);
	}

private:
	std::filesystem::path m_RendererRoot;
};

