#pragma once
#ifndef __K_MEANS_H__
#define __K_MEANS_H__

#include "point.h"
#include "utils.h"
#include <vector>

// namespace KMeans
// {
//     // double costFunction(const std::vector<Observation> &points, const std::vector<Observation> &centroids);
//     // void assignCluster(Observation &point, const std::vector<Observation> &centroids);
//     // void updateCentroids(const std::vector<Observation> &initPoints, std::vector<Observation> &centroids);
//     double fit(std::vector<Observation> &points, std::vector<Observation> &centroids,
//                                     double tolerance, unsigned int epochs);
// }
double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance);

#endif // __K_MEANS_H__
