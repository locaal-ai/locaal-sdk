#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <string>

class Logger {
public:
	enum class Level { DEBUG, INFO, WARNING, ERROR };

	using LogCallback = std::function<void(Level, const std::string &)>;

	static void setLogCallback(LogCallback callback);
	static void Logger::log(Level level, const std::string &format, ...);

private:
	static LogCallback s_logCallback;
	static std::string getLevelString(Level level);
};

#endif // LOGGER_H
