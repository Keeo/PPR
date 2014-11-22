git pull
rm ./log/log-*
./mpi_build.sh
mpiexec -np 2 ./ppr ./Assets/n10k5.grf

