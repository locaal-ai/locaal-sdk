#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <string>
#include <sstream>

/**
 * @class Logger
 * @brief A logging utility class that provides various logging levels and supports custom log callbacks.
 *
 * The Logger class allows logging messages at different levels (DEBUG, INFO, WARNING, ERROR_LOG).
 * It supports setting a custom log callback function and log level. It also provides a stream-based
 * logging interface through the nested LogStream class.
 */
class Logger {
public:
	enum class Level { DEBUG = 0, INFO, WARNING, ERROR_LOG };

	using LogCallback = std::function<void(Level, const std::string &)>;

	/**
	 * @brief Sets the callback function for logging.
	 * 
	 * This function allows the user to specify a custom callback function
	 * that will be used for logging purposes. The provided callback function
	 * should match the LogCallback type signature.
	 * 
	 * @param callback The callback function to be used for logging.
	 */
	static void setLogCallback(LogCallback callback);

	/**
	 * @brief Sets the logging level for the logger.
	 * 
	 * This function allows you to specify the level of logging detail that should be captured.
	 * 
	 * @param level The logging level to set. This can be one of the predefined levels such as DEBUG, INFO, WARN, ERROR, etc.
	 */
	static void setLogLevel(Level level);

	/**
	 * @brief Logs a message with a specified log level.
	 * 
	 * This function logs a formatted message to the logging system. The message
	 * format and additional arguments follow the printf-style formatting.
	 * 
	 * @param level The log level indicating the severity of the log message.
	 * @param format The format string for the log message.
	 * @param ... Additional arguments for the format string.
	 */
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

	/**
	 * @brief Creates a LogStream object with the specified logging level.
	 * 
	 * This function initializes a LogStream object that can be used to log messages
	 * at the given logging level. The LogStream object provides a convenient way to
	 * format and output log messages.
	 * 
	 * @param level The logging level for the LogStream object. This determines the
	 * severity or importance of the log messages that will be handled by the stream.
	 * 
	 * @return A LogStream object configured with the specified logging level.
	 */
	static LogStream stream(Level level) { return LogStream(level); }

private:
	static LogCallback s_logCallback;
	static Level s_logLevel;
	static std::string getLevelString(Level level);
};

#endif // LOGGER_H
