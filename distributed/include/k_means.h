#pragma once
#ifndef __K_MEANS_H__
#define __K_MEANS_H__

#include "point.h"
#include <vector>

double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance);

#endif // __K_MEANS_H__
