#include "../include/utils.h"

#include <vector>
#include <fstream>
#include <sstream>



std::vector<Observation> Utils::readPoints(const std::string path, unsigned int requested_points,
                                           unsigned int requested_dimensions, bool is_centroid)
{
    unsigned int n_points;
    unsigned int n_dimensions;
    std::vector<Observation> points;
    std::ifstream fin(path);
    std::string line;
    std::istringstream iss;
    std::vector<double> features;

    if (!fin.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return points;
    }

    getline(fin, line); // First line ignored, as it's the header
    getline(fin, line);

    iss.str(line);
    getline(iss, line, ',');
    n_points = std::stod(line);
    getline(iss, line, ',');
    n_dimensions = std::stod(line);

    if ( n_points < requested_points) {
        std::cerr << "Error, requested " << requested_points << " points, but " << path 
                  << " contains only " << n_points<< std::endl;
        return points;
    }
    if (requested_points == 0) { // Special case to request all
        requested_points = n_points;
    }

    if ( n_dimensions < requested_dimensions) {
        std::cerr << "Error, requested " << requested_dimensions << " dimensions, but " << path 
                  << " contains poits with only " << n_dimensions << std::endl;
        return points;
    }
    if (requested_dimensions == 0) { // Special case to request all
        requested_dimensions = n_dimensions;
    }

    #ifdef DEBUG
        std::cout<< "Reading file " << path << std::endl;
    #endif

    for (size_t i = 0; i < requested_points; ++i) {
        getline(fin, line);
        iss.str(line); iss.clear();
        features.clear();

        for (size_t j = 0; j < requested_dimensions; ++j) {
            getline(iss, line, ',');
            features.emplace_back(std::stof(line));
        }

        if (is_centroid) {
            points.emplace_back(features, i);
        } else {
            points.emplace_back(features, -1);
        }
    }

    fin.close();
    return points;
}



void Utils::writePoints(const std::string path, const std::vector<Observation>& p,
                        unsigned int n_clusters)
{
    std::ofstream fout;
    fout.open(path);

    if (!fout.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
    }

    #ifdef DEBUG
        std::cout<< "Writing to file " << path << std::endl;
    #endif

    fout << "n_points,n_clusters,n_dimensions" << std::endl;
    fout << p.size() << "," << n_clusters << "," << p.at(0).getFeatureSize() << std::endl;

    for (const auto &point : p) {
        for (const auto &feature : point.getFeatures()) {
            fout << feature << ",";
        }
        fout << point.getClusterID() << std::endl;
    }

    fout.close();
}
