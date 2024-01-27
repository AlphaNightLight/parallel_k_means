#include <mpi.h>

#include "../include/k_means.h"

#include <iostream>
#include <vector>



double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance)
{
    std::cout << "I'm the mpi code!" << std::endl;
    return 0.0;
}
