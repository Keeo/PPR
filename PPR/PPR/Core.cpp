#include "stdafx.h"
#include "Core.h"


Core::Core(std::string datafile)
{
	MPI_Comm_rank(MPI_COMM_WORLD, &processor_);
	MPI_Comm_size(MPI_COMM_WORLD, &cprocessor_);

	srand(processor_);
	lastBotheredPc_ = rand() % cprocessor_;
	Log::getInstance().init(processor_);

	LOG("mpi", "Processor:" + std::to_string(processor_) + " cprocessor_" + std::to_string(cprocessor_) + " lastBoeheredPc:" + std::to_string(lastBotheredPc_));
	waitingForWork_ = false;
	workSent_ = false;
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
		bool working = true;
		MPI_Send(&working, 1, MPI_C_BOOL, 1, MSG_WORKING, MPI_COMM_WORLD);
		int bestSolution = bridge_.getBestResult();
		MPI_Send(&bestSolution, 1, MPI_INT, 1, MSG_BEST_FOUND, MPI_COMM_WORLD);
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
			LOG("mpi", "Dosla prace, sending request to: " + std::to_string(lastBotheredPc_));
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

	while (true) {
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
		if (flag) {
			MPI_Get_count(&status, MPI_CHAR, &messageLength);
			if (messageLength > MAX_MESSAGE) {
				LOG("MPI", "Received message longer than allowed");
			}
			MPI_Recv(&message, messageLength, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
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
		workSent_ = true;
		int byteSize = (work.size() * sizeof(MPI_INT)) / sizeof(MPI_CHAR);

		LOG("mpi", "Sending work to:" + std::to_string(status->MPI_SOURCE) + " byteSize:" + std::to_string(byteSize));
		MPI_Send(work.data(), byteSize, MPI_CHAR, status->MPI_SOURCE, MSG_WORK_SENT, MPI_COMM_WORLD);
	}
	else {
		LOG("mpi", "Sending work_nowork to:" + std::to_string(status->MPI_SOURCE));
		MPI_Send(NULL, 0, MPI_CHAR, status->MPI_SOURCE, MSG_WORK_NOWORK, MPI_COMM_WORLD);
	}
}

void Core::processMessage(char* message, int messageLength, MPI_Status* status)
{
	LOG("mpi", "Prisel packet od: " + std::to_string(status->MPI_SOURCE) + " tag:" + to_string((MSG)status->MPI_TAG) + " size:" + std::to_string(messageLength));

	switch (status->MPI_TAG) {
		case MSG_WORK_REQUEST:
			sendWork(status);
			break;

		case MSG_WORK_SENT:
			if (messageLength > 0){
				LOG("mpi", "Prisla prace.");
				bridge_.setWork(message, messageLength);
				waitingForWork_ = false;
			}
			else {
				LOG("mpi", "Prijata prace byla bez dat.");
			}
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
				MPI_Send(solution.data(), solution.size(), MPI_INT, status->MPI_SOURCE, MSG_GET_SOLUTION, MPI_COMM_WORLD);
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

				MPI_Send(&networkResult, 1, MPI_INT, nextProcessor(processor_), MSG_BEST_FOUND, MPI_COMM_WORLD);
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
	int p = (processor + 1) % cprocessor_;
	return p == processor_ ? ((processor + 2) % cprocessor_) : p;
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
