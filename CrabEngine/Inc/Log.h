#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace crab
{
	enum class eLogLevel
	{
		Trace = SPDLOG_LEVEL_TRACE,
		Debug = SPDLOG_LEVEL_DEBUG,
		Info = SPDLOG_LEVEL_INFO,
		Warn = SPDLOG_LEVEL_WARN,
		Error = SPDLOG_LEVEL_ERROR,
		Critical = SPDLOG_LEVEL_CRITICAL,
		Off = SPDLOG_LEVEL_OFF
	};

	class Log
	{
		friend class Application;
	public:
		template <typename... Args>
		FORCE_INLINE static void Trace(fmt::format_string<Args...> fmt, Args&&... args)
		{
			m_logger->trace(fmt, std::forward<Args>(args)...);
		}

		template <typename... Args>
		FORCE_INLINE static void Info(fmt::format_string<Args...> fmt, Args&&... args)
		{
			m_logger->info(fmt, std::forward<Args>(args)...);
		}

		template <typename... Args>
		FORCE_INLINE static void Warn(fmt::format_string<Args...> fmt, Args&&... args)
		{
			m_logger->warn(fmt, std::forward<Args>(args)...);
		}


		template <typename... Args>
		FORCE_INLINE static void Error(fmt::format_string<Args...> fmt, Args&&... args)
		{
			m_logger->error(fmt, std::forward<Args>(args)...);
		}

		template <typename... Args>
		FORCE_INLINE static void Debug(fmt::format_string<Args...> fmt, Args&&... args)
		{
			m_logger->debug(fmt, std::forward<Args>(args)...);
		}


	private:
		static void Init();
		static void Shutdown();
		static void SetLogLevel(eLogLevel in_level) { m_logger->set_level((spdlog::level::level_enum)in_level); }

		inline static Ref<spdlog::async_logger> m_logger = nullptr;
	};

}
