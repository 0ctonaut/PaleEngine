#pragma once
#include "PaleRenderer/Core/PaleRendererExport.h"

namespace PaleRdr 
{
	class PALE_API CLog
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> getRdrLogger() { return m_RdrLogger; }
		inline static std::shared_ptr<spdlog::logger> getEditorLogger() { return m_EditorLogger; }

	private:
		inline static std::shared_ptr<spdlog::logger> m_RdrLogger;
		inline static std::shared_ptr<spdlog::logger> m_EditorLogger;
	};
}

#if defined(PALE_PLATFORM_WINDOWS)
#define PALE_DEBUGBREAK() __debugbreak()
#endif

#define PALE_RDR_TRACE(...)		        ::PaleRdr::CLog::getRdrLogger()->trace(__VA_ARGS__)
#define PALE_RDR_DEBUG(...)		        ::PaleRdr::CLog::getRdrLogger()->debug(__VA_ARGS__)
#define PALE_RDR_INFO(...)		        ::PaleRdr::CLog::getRdrLogger()->info(__VA_ARGS__)
#define PALE_RDR_WARN(...)		        ::PaleRdr::CLog::getRdrLogger()->warn(__VA_ARGS__)
#define PALE_RDR_ERROR(...)		        ::PaleRdr::CLog::getRdrLogger()->error(__VA_ARGS__)
#define PALE_RDR_CRITICAL(...)	        ::PaleRdr::CLog::getRdrLogger()->critical(__VA_ARGS__)
#define PALE_RDR_ASSERT(x, msg) { if(!(x)) { PALE_RDR_ERROR("Assertion Failed: {0}", msg); PALE_DEBUGBREAK();} }

#define PALE_EDITOR_TRACE(...)			::PaleRdr::CLog::getEditorLogger()->trace(__VA_ARGS__)
#define PALE_EDITOR_DEBUG(...)			::PaleRdr::CLog::getEditorLogger()->debug(__VA_ARGS__)
#define PALE_EDITOR_INFO(...)			::PaleRdr::CLog::getEditorLogger()->info(__VA_ARGS__)
#define PALE_EDITOR_WARN(...)			::PaleRdr::CLog::getEditorLogger()->warn(__VA_ARGS__)
#define PALE_EDITOR_ERROR(...)			::PaleRdr::CLog::getEditorLogger()->error(__VA_ARGS__)
#define PALE_EDITOR_CRITICAL(...)		::PaleRdr::CLog::getEditorLogger()->critical(__VA_ARGS__)
#define PALE_EDITOR_ASSERT(x, msg) { if(!(x)) { PALE_EDITOR_ERROR("Assertion Failed: {0}", msg); PALE_DEBUGBREAK();} }
