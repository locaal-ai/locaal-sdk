#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <string>
#include <sstream>

class Logger {
public:
	enum class Level { DEBUG = 0, INFO, WARNING, ERROR_LOG };

	using LogCallback = std::function<void(Level, const std::string &)>;

	static void setLogCallback(LogCallback callback);
	static void setLogLevel(Level level);
	static void Logger::log(Level level, const std::string format, ...);

	class LogStream {
	public:
		LogStream(Level level) : m_level(level) {}

		template<typename T> LogStream &operator<<(const T &value)
		{
			m_stream << value;
			return *this;
		}

		~LogStream()
		{
			if (Logger::s_logCallback) {
				Logger::s_logCallback(m_level, m_stream.str());
			}
		}

	private:
		Level m_level;
		std::ostringstream m_stream;
	};

	static LogStream stream(Level level) { return LogStream(level); }

private:
	static LogCallback s_logCallback;
	static Level s_logLevel;
	static std::string getLevelString(Level level);
};

#endif // LOGGER_H
