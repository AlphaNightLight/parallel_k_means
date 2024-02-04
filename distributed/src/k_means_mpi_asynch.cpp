#include <mpi.h>

#include "../include/k_means.h"

#include <iostream>
#include <vector>



double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance)
{
    int my_rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    std::cout << "I'm the asynchronous mpi code " << my_rank << "/" << size << " !" << std::endl;

    return 0.0;
}
