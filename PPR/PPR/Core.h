#pragma once

#include "Bridge.h"

class Core
{
	Bridge bridge_;


	int processor_;
	int cprocessor_;
	int lastBotheredPc_;
	int bestResultPc_;
	bool waitingForWork_;
	bool workSent_;
	bool jobDone_;


	char* initData_;
	int initDataLength_;

	void readInitData(std::string datafile);
	void distributeInitData();
	void distributeInitDataSize();
	void handleRequests();
	void processMessage(char* message, int messageLength, MPI_Status* status, std::vector<int>* needWork);
	void sendWorks(std::vector<int>* needWork);

	int nextProcessor(int processor);
	void handleStatus(EWORK ework);
public:

	Core(std::string work);

	void run();
	void finalize();

	~Core();

};

