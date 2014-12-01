#include "stdafx.h"
#include "Core.h"


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	if (argc == 1) {
		std::cout << "[INF] Run with graph set: ./ppr ../../Assets/n10k5.grf";
		return 1;
	}

	unsigned int runTime = std::clock();

	Core c(argv[1]);
	c.run();
	c.finalize();
		
	double time = double(std::clock() - runTime) / CLOCKS_PER_SEC;
	if (c.getProcessor() == 1) {
		Log::getInstance().info("Time", "Total elapsed time:" + std::to_string(time));
	}

	MPI_Finalize();
	return 0;
}

