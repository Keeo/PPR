git pull
rm ./log/log-*
./mpi_no_build.sh
mpiexec -np 2 ./ppr_no_debug ./Assets/n10k5.grf

