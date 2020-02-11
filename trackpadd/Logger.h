#pragma once


#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/hierarchy.h>
#include <log4cplus/initializer.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

#define LOGD(log) 	LOG4CPLUS_INFO(Logger::logger, log);
#define LOGI(log) 	LOG4CPLUS_INFO(Logger::logger, log);
#define LOGW(log) 	LOG4CPLUS_INFO(Logger::logger, log);
#define LOGE(log) 	LOG4CPLUS_INFO(Logger::logger, log);

class Logger
{
public:
	static log4cplus::Logger logger;

public:
	static void Initialize();
};