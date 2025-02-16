#include "stdafx.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "PaleRenderer/Core/Log.h"

namespace PaleRdr
{
	void CLog::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_RdrLogger = spdlog::stdout_color_mt("RDR");
		m_RdrLogger->set_level(spdlog::level::trace);

		m_EditorLogger = spdlog::stdout_color_mt("EDITOR");
		m_EditorLogger->set_level(spdlog::level::trace);
	}
}