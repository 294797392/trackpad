#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <Windows.h>
#include <WinBase.h>

#include "Logger.h"
#include "MouseDispatcher.h"

int main(int argc, char **argv)
{
	log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DEFAULT"));
	log4cplus::Initializer initializer;
	log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
	Logger::Initialize();


	MouseDispatcher *dispatcher = new MouseDispatcher();
	dispatcher->Initialize();
	dispatcher->Run();

	while (1)
	{
		char line[1024] = { '\0' };
		fgets(line, sizeof(line), stdin);
	}

	return 0;
}