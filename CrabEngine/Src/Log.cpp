#include "CrabEnginePch.h"
#include "Log.h"
#include <sstream>

namespace crab
{
	void Log::Init()
	{
		// 비동기 스레드 풀
		spdlog::init_thread_pool(8192, 1);

		// 콘솔 싱크
		auto consoleSink = MakeRef<spdlog::sinks::stdout_color_sink_mt>();

		// 파일 이름 생성
		auto now = std::chrono::system_clock::now();
		std::time_t current_time = std::chrono::system_clock::to_time_t(now);
		std::tm local_time{};
		localtime_s(&local_time, &current_time);

		std::ostringstream oss = {};
		oss << "logs/" << std::put_time(&local_time, "%Y-%m-%d_%H-%M-%S") << ".txt";

		// 파일 싱크
		std::string fileName = oss.str();
		auto fileSink = MakeRef<spdlog::sinks::basic_file_sink_mt>(fileName.c_str(), true);

		// 싱크 별 포맷 설정
		consoleSink->set_pattern("[%H:%M:%S] [%^%l%$] %v");
		fileSink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

		// 비동기 로거 설정
		m_logger = MakeRef<spdlog::async_logger>(
			"async_multi_logger",
			spdlog::sinks_init_list{ consoleSink, fileSink },
			spdlog::thread_pool(),
			spdlog::async_overflow_policy::block
		);

		// 로거 등록
		spdlog::register_logger(m_logger);

		// 로그 레벨 설정
		SetLogLevel(eLogLevel::Trace);
		Log::Info("Log initialize done.");

	}

	void Log::Shutdown()
	{
		spdlog::shutdown();
	}
}
