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
    auto start_time = std::chrono::high_resolution_clock::now();

    int n_points = points.size();
    int n_dimensions = points.at(0).getFeatureSize();
    int n_clusters = centroids.size();

    // cumulative sum of features for each cluster: [clusterID][featureID]
    std::vector<std::vector<double>> cumulative(n_clusters, std::vector<double>(n_dimensions, 0.0));
    std::vector<int> counter(n_clusters, 0); // number of points in each cluster
    Observation old_centroid;

    #pragma omp parallel for schedule(dynamic, static_cast<int>(points.size()))
    for (auto &p : points) 
    {
        p.setClusterID(0);
    }

    for (size_t e = 0; e < epochs; ++e) 
    {
        double delta = 0.0;

        #pragma omp parallel for schedule(dynamic, static_cast<int>(counter.size()))
        for (size_t cid = 0; cid < n_clusters; ++cid) 
        {
            counter.at(cid) = 0;
        }

        #pragma omp parallel for schedule(dynamic, static_cast<int>(cumulative.size()))
        for (size_t cid = 0; cid < n_clusters; ++cid) 
        {
            for (size_t dm = 0; dm < n_dimensions; ++dm) 
            {
                cumulative.at(cid).at(dm) = 0;
            }
        }

        #pragma omp parallel for schedule(dynamic, static_cast<int>(points.size())) reduction(+:delta)
        for (auto &p : points) 
        {
            double dist = distance(p.getPoint(), centroids.at( p.getClusterID() ));
            for (const auto &c : centroids) 
            {
                const double newDist = distance(p.getPoint(), c.getPoint());
                if (newDist < dist) 
                {
                    p.setClusterID( c.getClusterID() );
                    dist = newDist;
                }
            }

            counter.at( p.getClusterID() ) += 1;
            for (size_t dm = 0; dm < n_dimensions; ++dm) 
            {
                cumulative.at( p.getClusterID() ).at(dm) += p.getFeatures(dm);
            }
        }


        #pragma omp parallel for schedule(dynamic, static_cast<int>(centroids.size())) reduction(+:delta)
        for(auto &c : centroids) 
        {
            old_centroid = c;
            const int cid = c.getClusterID();

            size_t dm;
            #pragma omp parallel for schedule(dynamic, dm)
            for(dm = 0; dm < n_dimensions; ++dm) 
            {
                if (counter.at(cid) == 0) 
                {
                    c.setFeatures(dm, 0.0);
                } else 
                {
                    double newValue = cumulative.at(cid).at(dm) / counter.at(cid);
                    c.setFeatures(dm, newValue);
                }
            }
            delta += distance(old_centroid.getPoint(),c.getPoint());
        }

        if (delta < tolerance) break;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( elapsed );

    return elapsed.count();
}



#else

#error "k_means_omp.cpp must be compiled with -fopenmp compiler flag"

#endif
