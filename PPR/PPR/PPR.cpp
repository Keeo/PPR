#include "stdafx.h"
#include "Core.h"


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	if (argc == 1) {
		std::cout << "[INF] Run with graph set: ./ppr ../../Assets/n10k5.grf";
		return 1;
	}

	Core c(argv[1]);
	c.run();
	c.finalize();
		
	MPI_Finalize();
	return 0;
}

