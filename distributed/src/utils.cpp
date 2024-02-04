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

    #ifdef SHOW_DETAILS
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



void Utils::writePoints(const std::string path, const std::vector<Observation> &points,
                        const std::vector<Observation> &centroids)
{
    std::ofstream fout;
    fout.open(path);

    if (!fout.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
    }

    #ifdef SHOW_DETAILS
        std::cout<< "Writing to file " << path << std::endl;
    #endif

    fout << "n_points,n_clusters,n_dimensions" << std::endl;
    fout << points.size() << "," << centroids.size() << "," << points.at(0).getFeatureSize() << std::endl;

    fout << std::endl;
    fout << "centroid,clusterID" << std::endl;
    for (const auto &centroid : centroids) {
        for (const auto &feature : centroid.getFeatures()) {
            fout << feature << ",";
        }
        fout << centroid.getClusterID() << std::endl;
    }

    fout << std::endl;
    fout << "point,clusterID" << std::endl;
    for (const auto &point : points) {
        for (const auto &feature : point.getFeatures()) {
            fout << feature << ",";
        }
        fout << point.getClusterID() << std::endl;
    }

    fout.close();
}



bool Utils::readOutputs(const std::string path, int &n_points, int &n_clusters, int &n_dimensions,
                    std::vector<Observation> &points, std::vector<Observation> &centroids)
{
    std::ifstream fin(path);
    std::string line;
    std::istringstream iss;
    std::vector<double> features;
    int cluster_id;

    if (!fin.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return false;
    }

    points.clear();
    centroids.clear();

    getline(fin, line); // First line ignored, as it's the header
    getline(fin, line);

    iss.str(line);
    getline(iss, line, ',');
    n_points = std::stod(line);
    getline(iss, line, ',');
    n_clusters = std::stod(line);
    getline(iss, line, ',');
    n_dimensions = std::stod(line);

    getline(fin, line); // Two lines ignored, blank and second header
    getline(fin, line);

    for (size_t i = 0; i < n_clusters; ++i) {
        getline(fin, line);
        iss.str(line); iss.clear();
        features.clear();

        for (size_t j = 0; j < n_dimensions; ++j) {
            getline(iss, line, ',');
            features.emplace_back(std::stof(line));
        }

        getline(iss, line, ',');
        cluster_id = std::stod(line);
        centroids.emplace_back(features, cluster_id);
    }

    getline(fin, line); // Other wo lines ignored, blank and third header
    getline(fin, line);

    for (size_t i = 0; i < n_points; ++i) {
        getline(fin, line);
        iss.str(line); iss.clear();
        features.clear();

        for (size_t j = 0; j < n_dimensions; ++j) {
            getline(iss, line, ',');
            features.emplace_back(std::stof(line));
        }

        getline(iss, line, ',');
        cluster_id = std::stod(line);
        points.emplace_back(features, cluster_id);
    }

    fin.close();
    return true;
}