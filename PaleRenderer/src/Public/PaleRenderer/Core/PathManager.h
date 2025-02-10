#pragma once
#include <filesystem>
#include "PaleRenderer/Common/Singleton.h"
class CPathManager : public Singleton<CPathManager>
{
	friend class Singleton<CPathManager>;
public:
#ifdef RENDERER_ROOT
	std::filesystem::path getRendererRoot() const
	{
		return m_RendererRoot;
	}
#endif
#ifdef UI_ROOT
	std::filesystem::path getUIRoot() const
	{
		return m_UIRoot;
	}
#endif
	std::filesystem::path getRootDir() const
	{
		return m_Root;
	}

private:

	CPathManager()
	{
		m_Root = std::filesystem::path(ROOT_DIR);
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
	std::filesystem::path m_Root;

};

