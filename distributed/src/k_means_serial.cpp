#include "../include/k_means.h"

#include <iostream>
#include <vector>


double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance)
{
    auto start_time = std::chrono::high_resolution_clock::now();
  
    auto fs = points[0].getFeatureSize();

    std::vector<std::vector<double>> cumulative(centroids.size(), std::vector<double>(fs, 0.0)); // cumulative sum of features for each cluster: [clusterID][featureID]
    
    std::vector<int> counter(centroids.size(), 0); // number of points in each cluster 
    

    for (size_t e = 0; e < epochs; ++e)
    {
        auto delta = 0.0;
        auto moved = false;
        for (auto &p : points)
        {
            double dist = std::numeric_limits<double>::max();
            for (const auto &centroid : centroids)
            {
                const auto cid = centroid.getClusterID();

                const auto newDist = distance(p.getPoint(), centroid.getPoint());

                if (newDist < dist)
                {
                    p.setClusterID(cid);
                    dist = newDist;
                    for(size_t j = 0; j < fs; ++j)
                    {
                        cumulative[cid][j] += p.getFeatures(j); // update the counter when adding the pt at the cluster 
                    }
                    counter.at(cid)++;
                    moved = true;
                }
            }
        }

        if (!moved)
            break;

        for(size_t i = 0; i < centroids.size(); ++i)
        {
            auto old_centroid = centroids[i]; 

            for(size_t j = 0; j < fs; ++j)
            {
                centroids[i].setFeatures(j, counter[i] == 0 ? 0.0 : cumulative[i][j] / counter[i]);

                counter[i] = 0;  // reset 
                cumulative[i][j] = 0.0; // reset 
            }
            delta += distance(centroids[i].getPoint(), old_centroid.getPoint());
        }
        if(delta < tolerance)
            break;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( elapsed );

    return elapsed.count();
}
