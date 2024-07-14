#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H

#include <string>
#include <queue>
#include <memory>
#include <semaphore.h>

#include "src/common/config.h"
#include "src/common/mutex.h"
#include "src/net/timer_event.h"

namespace MyTinyRPC {

	#define DEBUGLOG(str, ...) \
	if (MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() && MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() <= MyTinyRPC::Debug) \
	{ \
		MyTinyRPC::Logger::GetGlobalLogger()->pushLog(MyTinyRPC::LogEvent(MyTinyRPC::LogLevel::Debug).toString() \
		+ "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + MyTinyRPC::formatString(str, ##__VA_ARGS__) + "\n");\
	} \

	#define INFOLOG(str, ...) \
	if (MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() <= MyTinyRPC::Info) \
	{ \
		MyTinyRPC::Logger::GetGlobalLogger()->pushLog(MyTinyRPC::LogEvent(MyTinyRPC::LogLevel::Info).toString() \
		+ "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + MyTinyRPC::formatString(str, ##__VA_ARGS__) + "\n");\
	} \

	#define ERRORLOG(str, ...) \
	if (MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() <= MyTinyRPC::Error) \
	{ \
		MyTinyRPC::Logger::GetGlobalLogger()->pushLog(MyTinyRPC::LogEvent(MyTinyRPC::LogLevel::Error).toString() \
		+ "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + MyTinyRPC::formatString(str, ##__VA_ARGS__) + "\n");\
	} \

	#define APPDEBUGLOG(str, ...) \
	if (MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() <= MyTinyRPC::Debug) \
	{ \
		MyTinyRPC::Logger::GetGlobalLogger()->pushAppLog(MyTinyRPC::LogEvent(MyTinyRPC::LogLevel::Debug).toString() \
		+ "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + MyTinyRPC::formatString(str, ##__VA_ARGS__) + "\n");\
	} \

	#define APPINFOLOG(str, ...) \
	if (MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() <= MyTinyRPC::Info) \
	{ \
		MyTinyRPC::Logger::GetGlobalLogger()->pushAppLog(MyTinyRPC::LogEvent(MyTinyRPC::LogLevel::Info).toString() \
		+ "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + MyTinyRPC::formatString(str, ##__VA_ARGS__) + "\n");\
	} \

	#define APPERRORLOG(str, ...) \
	if (MyTinyRPC::Logger::GetGlobalLogger()->getLogLevel() <= MyTinyRPC::Error) \
	{ \
		MyTinyRPC::Logger::GetGlobalLogger()->pushAppLog(MyTinyRPC::LogEvent(MyTinyRPC::LogLevel::Error).toString() \
		+ "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + MyTinyRPC::formatString(str, ##__VA_ARGS__) + "\n");\
	} \

	enum LogLevel {
		Unknown = 0,
		Debug = 1,
		Info = 2,
		Error = 3
	};

	class AsyncLogger {

		public:
			typedef std::shared_ptr<AsyncLogger> s_ptr;
			AsyncLogger(const std::string& file_name, const std::string& file_path, int max_size);
			void stop();
			void flush();// 刷新到磁盘
			void pushLogBuffer(std::vector<std::string>& vec);
			static void* Loop(void*);
			pthread_t m_thread;

		private:
			// m_file_path/m_file_name_yyyymmdd.0
			std::queue<std::vector<std::string>> m_buffer;
			std::string m_file_name;    // 日志输出文件名字
			std::string m_file_path;    // 日志输出路径
			int m_max_file_size {0};    // 日志单个文件最大大小, 单位为字节

			sem_t m_sempahore;
			pthread_cond_t m_condtion;  // 条件变量
			Mutex m_mutex;

			std::string m_date;   // 当前打印日志的文件日期
			FILE* m_file_hanlder {NULL};   // 当前打开的日志文件句柄

			bool m_reopen_flag {false};
			int m_no {0};   // 日志文件序号
			bool m_stop_flag {false};
	};

	class Logger {
		public:
			typedef std::shared_ptr<Logger> s_ptr;

			Logger(LogLevel level, int type = 1);
			void pushLog(const std::string& msg);
			void pushAppLog(const std::string& msg);
			void init();
			void log();

			LogLevel getLogLevel() const {
				return m_set_level;
			}
			AsyncLogger::s_ptr getAsyncAppLopger() {
				return m_asnyc_app_logger;
			}
			AsyncLogger::s_ptr getAsyncLopger() {
				return m_asnyc_logger;
			}

			void syncLoop();
			void flush();
			static Logger* GetGlobalLogger();
			static void InitGlobalLogger(int type = 1);

		private:
			LogLevel m_set_level;  // 日志级别限制
			std::vector<std::string> m_buffer;
			std::vector<std::string> m_app_buffer;
			Mutex m_mutex;
			Mutex m_app_mutex;
			// m_file_path/m_file_name_yyyymmdd.1
			std::string m_file_name;    // 日志输出文件名字
			std::string m_file_path;    // 日志输出路径
			int m_max_file_size {0};    // 日志单个文件最大大小

			AsyncLogger::s_ptr m_asnyc_logger;
			AsyncLogger::s_ptr m_asnyc_app_logger;
			TimerEvent::s_ptr m_timer_event;

			int m_type {0};
	};


	class LogEvent {
		public:
			LogEvent(LogLevel level) : m_level(level) {}

			std::string getFileName() const {
				return m_file_name;  
			}
			LogLevel getLogLevel() const {
				return m_level;
			}
			std::string toString();

		private:
			std::string m_file_name;  // 文件名
			int32_t m_file_line;  // 行号
			int32_t m_pid;  // 进程号
			int32_t m_thread_id;  // 线程号
			LogLevel m_level;     //日志级别
	};


	std::string LogLevelToString(LogLevel level);

	LogLevel StringToLogLevel(const std::string& log_level);

	template<typename... Args>
	std::string formatString(const char* str, Args&&... args) {

		int size = snprintf(nullptr, 0, str, args...);

		std::string result(size, '\0');
		if (size > 0) {
			result.resize(size);
			snprintf(&result[0], size + 1, str, args...);
		}

		return result;
	}
}

#endif