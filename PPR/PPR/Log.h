#pragma once

#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>

class Log
{
public:
	static Log& getInstance()
	{
		static Log instance;
		return instance;
	}

	void info(std::string type, std::string message);
	void ln();
	void setPrint(bool b);
	void init(int processor);

private:
	bool print_;
	std::ofstream logFile_;
	Log();
	Log(Log const&);
	void operator=(Log const&);
	~Log();
};

