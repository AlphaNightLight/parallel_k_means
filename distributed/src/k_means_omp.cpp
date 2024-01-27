#ifdef _OPENMP

#include "../include/k_means.h"

#include <omp.h>
#include <iostream>
#include <vector>



double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance)
{
    std::cout << "I'm the omp code!" << std::endl;
    return 0.0;
}

#else

#error "k_means_omp.cpp must be compiled with -fopenmp compiler flag"

#endif
