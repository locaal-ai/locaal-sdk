#include "logger.h"

#include <iostream>
#include <sstream>
#include <stdarg.h>

Logger::LogCallback Logger::s_logCallback = nullptr;

void Logger::setLogCallback(LogCallback callback)
{
	s_logCallback = callback;
}

void Logger::log(Level level, const std::string &format, ...)
{
	// Default logging behavior
	va_list args;
	va_start(args, format);
	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
	va_end(args);
	std::stringstream ss;
	ss << "[ " << getLevelString(level) << " ] " << buffer;

	if (s_logCallback) {
		s_logCallback(level, ss.str());
	} else {
		// Default logging behavior
		std::cout << ss.str() << std::endl;
	}
}

std::string Logger::getLevelString(Level level)
{
	switch (level) {
	case Level::DEBUG:
		return "DEBUG";
	case Level::INFO:
		return "INFO";
	case Level::WARNING:
		return "WARNING";
	case Level::ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}
