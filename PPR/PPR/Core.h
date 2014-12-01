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
	unsigned int waitingForworkClock_;
	std::vector<double> waitingClockVector_;
	bool workThisSent_, workLastSent_;
	bool jobDone_;


	char* initData_;
	int initDataLength_;

	void readInitData(std::string datafile);
	void distributeInitData();
	void distributeInitDataSize();
	void handleRequests();
	void processMessage(char* message, int messageLength, MPI_Status* status);
	void sendWork(MPI_Status* status);

	int nextProcessor(int processor);
	void handleStatus(EWORK ework);

	bool isWorkDone();
	void msgWorking(int& working);
public:

	Core(std::string work);

	void run();
	void finalize();
	int getProcessor();

	~Core();

};

