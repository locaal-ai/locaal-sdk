#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <string>

class Logger {
public:
	enum class Level { DEBUG = 0, INFO, WARNING, ERROR_LOG };

	using LogCallback = std::function<void(Level, const std::string &)>;

	static void setLogCallback(LogCallback callback);
	static void setLogLevel(Level level);
	static void Logger::log(Level level, const std::string &format, ...);
	// set log level

private:
	static LogCallback s_logCallback;
	static Level s_logLevel;
	static std::string getLevelString(Level level);
};

#endif // LOGGER_H
