#ifdef _OPENMP

#include "../include/k_means.h"
#include "../include/point.h"
#include "../include/utils.h"

#include <omp.h>
#include <iostream>
#include <vector>



double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance)
{
    double start_time, end_time;
    start_time = omp_get_wtime();

    int n_points = points.size();
    int n_dimensions = points.at(0).getFeatureSize();
    int n_clusters = centroids.size();

    // cumulative sum of features for each cluster: [clusterID][featureID]
    std::vector<std::vector<double>> cumulative(n_clusters, std::vector<double>(n_dimensions, 0.0));
    std::vector<int> counter(n_clusters, 0); // number of points in each cluster
    Observation old_centroid;

    #pragma omp parallel for schedule(static, static_cast<int>(points.size()))
    for (size_t point = 0; point < n_points; ++point)
    {
        points.at(point).setClusterID(0);
    }

    for (size_t e = 0; e < epochs; ++e) 
    {
        double delta = 0.0;

        #pragma omp parallel for schedule(static, static_cast<int>(counter.size()))
        for (size_t cid = 0; cid < n_clusters; ++cid) 
        {
            counter.at(cid) = 0;
        }

        #pragma omp parallel for schedule(static, static_cast<int>(cumulative.size()))
        for (size_t cid = 0; cid < n_clusters; ++cid) 
        {
            for (size_t dm = 0; dm < n_dimensions; ++dm) 
            {
                cumulative.at(cid).at(dm) = 0;
            }
        }

        #pragma omp parallel for schedule(static, static_cast<int>(points.size())) reduction(+:delta)
        for (size_t point = 0; point < n_points; ++point) 
        {
            double dist = distance(points.at(point).getPoint(), centroids.at( points.at(point).getClusterID() ));
            for (const auto &c : centroids) 
            {
                const double newDist = distance(points.at(point).getPoint(), c.getPoint());
                if (newDist < dist) 
                {
                    points.at(point).setClusterID( c.getClusterID() );
                    dist = newDist;
                }
            }

            counter.at( points.at(point).getClusterID() ) += 1;
            for (size_t dm = 0; dm < n_dimensions; ++dm) 
            {
                cumulative.at( points.at(point).getClusterID() ).at(dm) += points.at(point).getFeatures(dm);
            }
        }


        #pragma omp parallel for schedule(static, static_cast<int>(centroids.size())) reduction(+:delta)
        for(size_t cid = 0; cid < n_clusters; ++cid) 
        {
            old_centroid = centroids.at(cid);

            size_t dm;
            #pragma omp parallel for schedule(static, dm)
            for(dm = 0; dm < n_dimensions; ++dm) 
            {
                if (counter.at(cid) == 0) 
                {
                    centroids.at(cid).setFeatures(dm, 0.0);
                } else 
                {
                    double newValue = cumulative.at(cid).at(dm) / counter.at(cid);
                    centroids.at(cid).setFeatures(dm, newValue);
                }
            }
            delta += distance(old_centroid.getPoint(),centroids.at(cid).getPoint());
        }

        if (delta < tolerance) break;
    }

    end_time = omp_get_wtime();
    return end_time - start_time;
}



#else

#error "k_means_omp.cpp must be compiled with -fopenmp compiler flag"

#endif
