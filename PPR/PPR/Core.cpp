#include "stdafx.h"
#include "Core.h"


Core::Core(std::string datafile)
{
	MPI_Comm_rank(MPI_COMM_WORLD, &processor_);
	MPI_Comm_size(MPI_COMM_WORLD, &cprocessor_);

	lastBotheredPc_ = nextProcessor(processor_);

	Log::getInstance().init(processor_);

	LOG("mpi", "Processor:" + std::to_string(processor_) + " cprocessor_" + std::to_string(cprocessor_) + " lastBoeheredPc:" + std::to_string(lastBotheredPc_));
	waitingForWork_ = false;
	workThisSent_ = false;
	workLastSent_ = false;
	jobDone_ = false;
	initDataLength_ = 0;
	

	if (processor_ == 0) {
		readInitData(datafile);
	}
	distributeInitDataSize();
	distributeInitData();
}


void Core::readInitData(std::string datafile)
{
	std::ifstream t;
	t.open(datafile);
	t.seekg(0, std::ios::end);
	initDataLength_ = t.tellg();

	if (initDataLength_ <= 0) {
		LOG("mpi", "Missing init data!");
	}

	t.seekg(0, std::ios::beg);
	initData_ = new char[initDataLength_];

	t.read(initData_, initDataLength_);
	t.close();
}


void Core::distributeInitDataSize()
{
	int ret = MPI_Bcast(&initDataLength_, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (ret != MPI_SUCCESS) {
		LOG("mpi", "Distribute size of work failed!");
	}

	if (initDataLength_ == 0) {
		LOG("mpi", "Missing init data!");
	}

	if (processor_ != 0) {
		initData_ = new char[initDataLength_];
	}
}


void Core::distributeInitData()
{
	LOG("mpi", "Sending init data size:" + std::to_string(initDataLength_));
	int ret = MPI_Bcast(initData_, initDataLength_, MPI_CHAR, 0, MPI_COMM_WORLD);
	if (ret != MPI_SUCCESS) {
		LOG("mpi", "Distribute work failed!");
	}

	bridge_.initData(initData_, initDataLength_, processor_ == 0);
	
	delete[] initData_;
	initDataLength_ = -1;	
}


void Core::run()
{
	if (processor_ == 0) {
		int out[3];
		out[0] = bridge_.getBestResult();
		out[1] = processor_;
		out[2] = MSG_WORKING;
		LOG("Core", "Inicializuji status packet s:" + std::to_string(out[0]));
		MPI_Send(out, 3, MPI_INT, 1, MSG_STATUS, MPI_COMM_WORLD);
	}

	while (!jobDone_) {
		EWORK status = bridge_.work();
		handleStatus(status);
		handleRequests();
		LOG("Core", "Loop done workThisSent:" + std::to_string(workThisSent_) + " workLastSent:" + std::to_string(workLastSent_) + " waitingForWork:" + std::to_string(waitingForWork_));
	}
}


void Core::handleStatus(EWORK ework)
{
	switch (ework) {
		case EWORK_OK: 
			break;

		case EWORK_BEST_FOUND: {
			if (processor_ == 0) {
				bestResultPc_ = 0;
				jobDone_;
			}
			else {
				MPI_Send(NULL, 0, MPI_CHAR, 0, MSG_BEST_FOUND, MPI_COMM_WORLD);
			}
		} 
		break;
		
		case EWORK_OUT_OF_WORK: {
			if (!waitingForWork_) {
				waitingForWork_ = true;
				waitingForworkClock_ = std::clock();
				lastBotheredPc_ = nextProcessor(lastBotheredPc_);
				LOG("mpi", "Dosla prace, sending request to: " + std::to_string(lastBotheredPc_));
				MPI_Send(NULL, 0, MPI_CHAR, lastBotheredPc_, MSG_WORK_REQUEST, MPI_COMM_WORLD);
			}
		}
		break;
	}
}


void Core::handleRequests()
{
#define MAX_MESSAGE 1000
	int flag;
	int messageLength;
	char message[MAX_MESSAGE];
	MPI_Status status;

	while (!jobDone_) {
		if (waitingForWork_) {
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			flag = true;
		}
		else{
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
		}
		
		if (flag) {
			MPI_Get_count(&status, MPI_CHAR, &messageLength);
			if (messageLength > MAX_MESSAGE) {
				LOG("MPI", "Received message longer than allowed");
				exit(85);
			}

			LOG("Core", "Preparing for message from: " + std::to_string(status.MPI_SOURCE) + " tag:" + msg_to_string((MSG)status.MPI_TAG) + " size:" + std::to_string(messageLength));
			MPI_Recv(&message, messageLength, MPI_CHAR, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
			processMessage(message, messageLength, &status);
		}
		else {
			break;
		}
	}

}


void Core::sendWork(MPI_Status* status)
{
	std::vector<int> work = bridge_.getWork();

	if (work.size() > 0) {
		workThisSent_ = true;
		LOG("mpi", "Sending work to:" + std::to_string(status->MPI_SOURCE) + " size:" + std::to_string(work.size()));
		MPI_Send(work.data(), work.size(), MPI_INT, status->MPI_SOURCE, MSG_WORK_SENT, MPI_COMM_WORLD);
	}
	else {
		LOG("mpi", "Sending NOWORK to:" + std::to_string(status->MPI_SOURCE));
		MPI_Send(NULL, 0, MPI_BYTE, status->MPI_SOURCE, MSG_WORK_NOWORK, MPI_COMM_WORLD);
	}
}

void Core::processMessage(char* message, int messageLength, MPI_Status* status)
{
	LOG("incoming", "Prisel packet od: " + std::to_string(status->MPI_SOURCE) + " tag:" + msg_to_string((MSG)status->MPI_TAG) + " size:" + std::to_string(messageLength));

	switch (status->MPI_TAG) {
		case MSG_WORK_REQUEST:
			sendWork(status);
			break;

		case MSG_WORK_SENT:
			if (messageLength > 0){
				LOG("mpi", "Prisla prace.");

				bridge_.setWork(message, messageLength);
				waitingForWork_ = false;
				Log::getInstance().info("loop", "Waiting for work :" + std::to_string((double(std::clock() - waitingForworkClock_) / CLOCKS_PER_SEC)));
			}
			else {
				LOG("mpi", "Prijata prace byla bez dat.");
				exit(71);
			}
			break;

		case MSG_WORK_NOWORK:
				lastBotheredPc_ = nextProcessor(lastBotheredPc_);
				LOG("Core", "NOWORK preparing another packet for:" + std::to_string(lastBotheredPc_));
				MPI_Send(NULL, 0, MPI_CHAR, lastBotheredPc_, MSG_WORK_REQUEST, MPI_COMM_WORLD);
			break;

		case MSG_FINISH:{
			if (processor_ != cprocessor_ - 1) {
				MPI_Send(NULL, 0, MPI_CHAR, nextProcessor(processor_), MSG_FINISH, MPI_COMM_WORLD);
			}

			jobDone_ = true;
			}
			break;

		case MSG_GET_SOLUTION: {
			if (status->MPI_SOURCE == 0){
				std::vector<int> solution = bridge_.getSolution();
				LOG("Core", "Providing best solution path, steps:" + std::to_string(solution.size()));
				MPI_Send(solution.data(), solution.size(), MPI_INT, status->MPI_SOURCE, MSG_GET_SOLUTION, MPI_COMM_WORLD);
			}
			else {
				LOG("Core", "Get best solution asked frome not 0-processor!");
				exit(72);
			}

			}
			break;

		case MSG_STATUS: {
			int bestResult = bridge_.getBestResult();
			int& networkResult = *(int*)message;
			int& networkPc = *(((int*)message) + 1);
			int& working = *(((int*)message) + 2);
			LOG("Core", "Best result looping in packet:" + std::to_string(networkResult) + " from pc_:" + std::to_string(networkPc));
			if (bestResult > networkResult) {
				bridge_.setBestResult(networkResult);
				bestResultPc_ = networkPc;
			}
			if (bestResult < networkResult) {
				networkResult = bestResult;
				bestResultPc_ = processor_;
				networkPc = processor_;
			}

			msgWorking(working);
			LOG("status", "Sending status packet.");
			MPI_Send(&networkResult, 3, MPI_INT, nextProcessor(processor_), MSG_STATUS, MPI_COMM_WORLD);
		}

		default:
			LOG("mpi", "WTF???");
			break;
	}
}


void Core::msgWorking(int& working)
{
	if (working == MSG_NOT_WORKING) {
		if (processor_ == 0) {
			jobDone_ = true;
		}
		else{
			working = isWorkDone() ? MSG_NOT_WORKING : MSG_WORKING;
		}
	}
	else {
		if (processor_ == 0) {
			working = isWorkDone() ? MSG_NOT_WORKING : MSG_WORKING;
		}
	}
	workLastSent_ = workThisSent_;
	workThisSent_ = false;
}

Core::~Core()
{
}


inline int Core::nextProcessor(int processor)
{
	int p = (processor + 1) % cprocessor_;
	return p == processor_ ? ((processor + 2) % cprocessor_) : p;
}


void Core::finalize()
{
	if (processor_ != 0) {
		return;
	}

	int messageLength;
	int message[100];

	LOG("Core", "Finalizing solution will provide:" + std::to_string(bestResultPc_));
	if (bestResultPc_ != 0) {
		LOG("Core", "Asking for best solution.");
		MPI_Send(NULL, 0, MPI_INT, bestResultPc_, MSG_GET_SOLUTION, MPI_COMM_WORLD);

		MPI_Status status;
		LOG("Core", "Waiting for response.");
		MPI_Probe(bestResultPc_, MSG_GET_SOLUTION, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &messageLength);
		
		LOG("Core", "Incoming data size:" + std::to_string(messageLength));
		MPI_Recv(&message, messageLength, MPI_INT, bestResultPc_, MSG_GET_SOLUTION, MPI_COMM_WORLD, &status);

		LOG("Core", "Best solution received.");
	}
	else{
		std::vector<int> solution = bridge_.getSolution();
		messageLength = solution.size();
		memcpy(message, solution.data(), messageLength * sizeof(int));
	}

	MPI_Send(NULL, 0, MPI_CHAR, 1, MSG_FINISH, MPI_COMM_WORLD);

	std::stringstream ss;
	ss << "Best solution:" << std::endl;
	for (int i = 0; i < messageLength; ++i) {
		ss << std::to_string(message[i]) << ", ";
	}
	LOG("WIN", ss.str());
	std::cout << ss.str() << std::endl;
	std::cout << "Thank you for working with Reynholm Industries." << std::endl;
}


bool Core::isWorkDone()
{
	return waitingForWork_ == true && workThisSent_ == false && workLastSent_ == false;
}

