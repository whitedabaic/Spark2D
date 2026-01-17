#pragma once
#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>

#define SPARK_LOG(x, ...) SPARK_LOGGER::Logger::GetInstance().Log(x, __VA_ARGS__);
#define SPARK_WARN(x, ...) SPARK_LOGGER::Logger::GetInstance().Warn(x, __VA_ARGS__);
#define SPARK_ERROR(x, ...) SPARK_LOGGER::Logger::GetInstance().Error(std::source_location::current(), x, __VA_ARGS__);
#define SPARK_INIT_LOGS(console, retain) SPARK_LOGGER::Logger::GetInstance().Init(console, retain);

namespace SPARK_LOGGER
{
	struct LogEntry
	{
		enum class LogType { INFO, WARN, ERR, NONE };
		LogType type{ LogType::INFO };
		std::string log{""};
	};

	class Logger
	{
	private:
		std::vector<LogEntry> m_LogEntries;
		bool m_bLogAdded{ false }, m_bInitialized{ false }, m_bConsoleLog{ true }, m_bRetainLogs{ true };

		Logger() = default;

		struct LogTime
		{
			std::string day, dayNumber, month, year, time;
			LogTime(const std::string& date);
		};

		std::string CurrentDateTime();

	public:
		static Logger& GetInstance();

		~Logger() = default;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		void Init(bool consoleLog = true, bool retainLogs = true);

		template<typename... Args>
		void Log(const std::string& message, Args&&... args);

		template<typename... Args>
		void Warn(const std::string& message, Args&&... args);

		template<typename... Args>
		void Error(std::source_location location, const std::string& message, Args&&... args);
	};
}

#include "Logger.inl"