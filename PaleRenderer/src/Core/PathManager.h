#pragma once
#include <filesystem>
#include "Common/Singleton.h"
class CPathManager : public Singleton<CPathManager>
{
	friend class Singleton<CPathManager>;
public:
#ifdef RENDERER_ROOT
	std::filesystem::path getRendererRoot()
	{
		return m_RendererRoot;
	}
#endif
#ifdef UI_ROOT
	std::filesystem::path getUIRoot()
	{
		return m_UIRoot;
	}
#endif

private:

	CPathManager()
	{
#ifdef RENDERER_ROOT
		m_RendererRoot = std::filesystem::path(RENDERER_ROOT);
#endif

#ifdef UI_ROOT
		m_UIRoot = std::filesystem::path(UI_ROOT);
#endif
	}
#ifdef RENDERER_ROOT
	std::filesystem::path m_RendererRoot;
#endif
#ifdef UI_ROOT
	std::filesystem::path m_UIRoot;
#endif
};

