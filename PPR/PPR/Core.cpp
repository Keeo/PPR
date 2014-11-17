#include "stdafx.h"
#include "Core.h"


Core::Core(std::string datafile)
{
	MPI_Comm_rank(MPI_COMM_WORLD, &processor_);
	MPI_Comm_size(MPI_COMM_WORLD, &cprocessor_);
	lastBotheredPc_ = rand() % cprocessor_;
	waitingForWork_ = false;
	workSent_ = false;
	jobDone_ = false;

	Log::getInstance().init(processor_);

	if (processor_ == 0) {
		readInitData(datafile);
	}

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


void Core::distributeInitData()
{
	LOG("mpi", "Sending init data size:" + std::to_string(initDataLength_));
	int ret = MPI_Bcast(&initDataLength_, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	if (ret != MPI_SUCCESS) {
		LOG("mpi", "Distribute size of work failed!");
	}

	if (processor_ != 0) {
		initData_ = new char[initDataLength_];
	}

	ret = MPI_Bcast(initData_, initDataLength_, MPI_CHAR, 0, MPI_COMM_WORLD);
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
		bool working = true;
		MPI_Send(&working, 1, MPI_C_BOOL, 1, MSG_WORKING, MPI_COMM_WORLD);
		int bestSolution = bridge_.getBestResult();
		MPI_Send(&bestSolution, 1, MPI_INTEGER, 1, MSG_BEST_FOUND, MPI_COMM_WORLD);
	}

	while (!jobDone_) {
		EWORK status = bridge_.work();
		handleStatus(status);
		handleRequests();
	}
}


void Core::handleStatus(EWORK ework)
{
	switch (ework) {
		case EWORK_OK: break;
		case EWORK_BEST_FOUND: {
			if (processor_ == 0) {
				bestResultPc_ = 0;
				jobDone_;
			}
			else {
				MPI_Send(NULL, 0, MPI_CHAR, 0, MSG_BEST_FOUND, MPI_COMM_WORLD);
			}
			break;
		} 
		

		case EWORK_OUT_OF_WORK: {
			if (waitingForWork_) break;
			waitingForWork_ = true;
			lastBotheredPc_ = nextProcessor(lastBotheredPc_);
			MPI_Send(NULL, 0, MPI_CHAR, lastBotheredPc_, MSG_WORK_REQUEST, MPI_COMM_WORLD);
			break;
		}
	}
}


void Core::handleRequests()
{
#define MAX_MESSAGE 1000
	int flag;
	int messageLength;
	char message[MAX_MESSAGE];
	MPI_Status status;
	std::vector<int> needWork;

	while (true) {
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
		if (flag) {
			MPI_Get_count(&status, MPI_CHAR, &messageLength);
			if (messageLength > MAX_MESSAGE) {
				LOG("MPI", "Received message longer than allowed");
			}
			MPI_Recv(&message, messageLength, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			processMessage(message, messageLength, &status, &needWork);
		}
		else {
			sendWorks(&needWork);
			break;
		}
	}

}


void Core::sendWorks(std::vector<int>* needWork)
{
	bool workSent = false;

	std::vector<std::vector<int>> works = bridge_.getWork(needWork->size());
	for (auto &work : works) {
		if (work.size() > 0) {
			workSent = true;
			MPI_Send(work.data(), work.size(), MPI_INTEGER, needWork->back(), MSG_WORK_SENT, MPI_COMM_WORLD);
		}
		else {
			MPI_Send(NULL, 0, MPI_CHAR, needWork->back(), MSG_WORK_NOWORK, MPI_COMM_WORLD);
		}
		needWork->pop_back();
	}
	workSent_ = workSent;
}


void Core::processMessage(char* message, int messageLength, MPI_Status* status, std::vector<int>* needWork)
{
	switch (status->MPI_TAG) {
		case MSG_WORK_REQUEST:
				needWork->push_back(status->MPI_SOURCE);
			break;

		case MSG_WORK_SENT:
				bridge_.setWork(message, messageLength);
				waitingForWork_ = false;
			break;

		case MSG_WORK_NOWORK:
				lastBotheredPc_ = nextProcessor(lastBotheredPc_);
				MPI_Send(NULL, 0, MPI_CHAR, lastBotheredPc_, MSG_WORK_REQUEST, MPI_COMM_WORLD);
			break;

		case MSG_WORKING: {
				bool working = (bool)*message;
				if (processor_ == 0 && waitingForWork_ == true && workSent_ == false && working == false) {
					jobDone_ = true;
				}
				working |= !(waitingForWork_ == true && workSent_ == false);
				MPI_Send(&working, 1, MPI_C_BOOL, nextProcessor(processor_), MSG_WORKING, MPI_COMM_WORLD);
			}
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
				MPI_Send(solution.data(), solution.size(), MPI_INTEGER, status->MPI_SOURCE, MSG_GET_SOLUTION, MPI_COMM_WORLD);
			}
			else {
				bestResultPc_ = status->MPI_SOURCE;
				jobDone_ = true;
			}

			}
			break;

		case MSG_BEST_FOUND: {
				int bestResult = bridge_.getBestResult();
				int networkResult = (int)*message;
				if (bestResult > networkResult) {
					bridge_.setBestResult(networkResult);
					bestResultPc_ = status->MPI_SOURCE;
				}
				if (bestResult < networkResult) {
					networkResult = bestResult;
					bestResultPc_ = processor_;
				}

				MPI_Send(&networkResult, 1, MPI_INTEGER, nextProcessor(status->MPI_SOURCE), MSG_BEST_FOUND, MPI_COMM_WORLD);
			}
			break;

		default:
			LOG("mpi", "WTF???");
			break;
	}
}


Core::~Core()
{
}


inline int Core::nextProcessor(int processor)
{
	return (processor + 1) % cprocessor_;
}


void Core::finalize()
{
	if (processor_ != 0) {
		return;
	}

	int messageLength;
	int* message;
	if (bestResultPc_ != 0){
		MPI_Send(NULL, 0, MPI_CHAR, bestResultPc_, MSG_GET_SOLUTION, MPI_COMM_WORLD);

		MPI_Status status;
		MPI_Probe(bestResultPc_, MSG_GET_SOLUTION, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &messageLength);
		message = new int[messageLength];
		MPI_Recv(&message, messageLength, MPI_INT, bestResultPc_, MSG_GET_SOLUTION, MPI_COMM_WORLD, &status);
	}
	else{
		std::vector<int> solution = bridge_.getSolution();
		message = solution.data();
		messageLength = solution.size();
	}

	MPI_Send(NULL, 0, MPI_CHAR, 1, MSG_FINISH, MPI_COMM_WORLD);
	std::cout << "Best solution:" << std::endl;
	for (int i = 0; i < messageLength; ++i) {
		std::cout << message[i] << std::endl;
	}
	std::cout << "Thank you for working with Reynholm Industries." << std::endl;
}
