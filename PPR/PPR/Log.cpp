#include "stdafx.h"
#include "Log.h"


Log::Log()
{
	std::stringstream ss;
	ss << time(0);
	std::string str = ss.str();
	logFile_.open("log/log-" + str + ".txt");
	if (!logFile_.is_open()) {
		std::cerr << "[ERR] Log file cant be open!" << std::endl;
	}

	logFile_ << "[Greetings]" << std::endl << std::endl;
	print_ = true;
}


Log::~Log()
{
	logFile_.close();
}


void Log::info(std::string type, std::string message)
{
	logFile_ << "[" << type << "] " << message << std::endl;
	if (print_) std::cout << "[" << type << "] " << message << std::endl;
}


void Log::ln()
{
	logFile_ << std::endl;
	if (print_) std::cout << std::endl;
}


void Log::setPrint(bool enabled)
{
	print_ = enabled;
}
