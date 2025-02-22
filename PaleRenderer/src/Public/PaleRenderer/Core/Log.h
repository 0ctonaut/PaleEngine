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

#include <filesystem>

#define PALE_FILENAME std::filesystem::path(__FILE__).filename().string()

#define PALE_RDR_TRACE(...) \
    ::PaleRdr::CLog::getRdrLogger()->trace("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_RDR_DEBUG(...) \
    ::PaleRdr::CLog::getRdrLogger()->debug("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_RDR_INFO(...) \
    ::PaleRdr::CLog::getRdrLogger()->info("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_RDR_WARN(...) \
    ::PaleRdr::CLog::getRdrLogger()->warn("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_RDR_ERROR(...) \
    ::PaleRdr::CLog::getRdrLogger()->error("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_RDR_CRITICAL(...) \
    ::PaleRdr::CLog::getRdrLogger()->critical("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_EDITOR_TRACE(...) \
    ::PaleRdr::CLog::getEditorLogger()->trace("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_EDITOR_DEBUG(...) \
    ::PaleRdr::CLog::getEditorLogger()->debug("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_EDITOR_INFO(...) \
    ::PaleRdr::CLog::getEditorLogger()->info("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_EDITOR_WARN(...) \
    ::PaleRdr::CLog::getEditorLogger()->warn("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_EDITOR_ERROR(...) \
    ::PaleRdr::CLog::getEditorLogger()->error("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))

#define PALE_EDITOR_CRITICAL(...) \
    ::PaleRdr::CLog::getEditorLogger()->critical("[{}:{} {}] {}", PALE_FILENAME, __LINE__, __FUNCTION__, fmt::format(__VA_ARGS__))


#define PALE_RDR_ASSERT(x, msg) { if(!(x)) { PALE_RDR_ERROR("Assertion Failed: {0}", msg); PALE_DEBUGBREAK();} }
#define PALE_EDITOR_ASSERT(x, msg) { if(!(x)) { PALE_EDITOR_ERROR("Assertion Failed: {0}", msg); PALE_DEBUGBREAK();} }
