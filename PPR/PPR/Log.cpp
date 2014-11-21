#include "stdafx.h"
#include "Log.h"


Log::Log()
{
	
}

void Log::init(int processor)
{
	processor_ = processor;
	std::stringstream ss;
	ss << time(0);
	std::string str = ss.str();
	logFile_.open("log/log-" + str + "-" + std::to_string(processor) + ".txt");
	if (!logFile_.is_open()) {
		std::cerr << "[ERR] Log file cant be open!" << std::endl;
	}

	logFile_ << "[Greetings][cpu " << processor_ << "]" << std::endl << std::endl;
	print_ = true;
}


Log::~Log()
{
	logFile_.close();
}


void Log::info(std::string type, std::string message)
{
	if ("incoming" != type) return;

	logFile_ << "(" << processor_ << ")[" << type << "] " << message << std::endl;
	if (print_) std::cout << "(" << processor_ << ")[" << type << "] " << message << std::endl;
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
