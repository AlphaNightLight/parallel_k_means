#include "../include/point.h"
#include "../include/utils.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <dirent.h>



int main(int argc, char** argv)
{
    std::string file_path_base;
    std::string folder_path_compare;
    std::string output_path;
    double tolerance;

    int n_points_base;
    int n_clusters_base;
    int n_dimensions_base;

    int n_points_compare;
    int n_clusters_compare;
    int n_dimensions_compare;

    std::vector<Observation> points_base;
    std::vector<Observation> centroids_base;
    std::vector<Observation> points_compare;
    std::vector<Observation> centroids_compare;

    int n_different_points;
    int n_different_centroids;

    std::ofstream fout;
    bool successful_read;
    std::vector<std::string> compare_files;

    if (argc != 5) {
		std::cout << "usage: " << argv[0] << " <base file path> <compare folder path> <output path> \
                     <tolerance>" << std::endl;
        return EXIT_FAILURE;
	}

    file_path_base = argv[1];
    folder_path_compare = argv[2];
    output_path = argv[3];
    tolerance = std::stof(argv[4]);

    successful_read = Utils::readOutputs(file_path_base, n_points_base, n_clusters_base, n_dimensions_base,
                                         points_base, centroids_base);
    if (!successful_read) return EXIT_FAILURE;

    if (auto compare_dir = opendir(folder_path_compare.c_str())) {
        while (auto compare_file = readdir(compare_dir)) {
            if (compare_file->d_name && compare_file->d_name[0] != '.') {
                compare_files.emplace_back(std::string(compare_file->d_name));
            }
        }
        closedir(compare_dir);
    }

    fout.open(output_path);

    fout << "Base file: " << file_path_base << std::endl;
    fout << "n_points: " << n_points_base << ", n_clusters: " << n_clusters_base << ", n_dimensions: "
         << n_dimensions_base << ", tolerance: " << tolerance << std::endl;
    fout << "Comparison with files in: " << folder_path_compare << std::endl;
    fout << std::endl;

    for (std::string compare_file : compare_files){
        successful_read = Utils::readOutputs(folder_path_compare+"/"+compare_file, n_points_compare,
                                             n_clusters_compare,n_dimensions_compare, points_compare,centroids_compare);
        if (!successful_read) return EXIT_FAILURE;
        fout << compare_file << ": ";

        if (n_dimensions_base != n_dimensions_compare) {
            fout << "DIFFERENT n_dimensions" << std::endl;
            continue;
        }
        if (n_clusters_base != n_clusters_compare) {
            fout << "DIFFERENT n_clusters" << std::endl;
            continue;
        }
        if (n_points_base > n_points_compare) {
            fout << "LESS points, ";
            continue;
        } else if (n_points_base < n_points_compare) {
            fout << "MORE points, ";
            // comparison can be done in case of more points
        }

        n_different_points = 0;
        for (size_t i = 0; i < points_base.size(); ++i) {
            if (points_base.at(i).getClusterID() != points_compare.at(i).getClusterID()) {
                ++n_different_points;
            }
        }
        if (n_different_points != 0) {
            fout << n_different_points << " DIFFERENT points, ";
        } else {
            fout << "points OK, ";
        }

        n_different_centroids = 0;
        for (size_t i = 0; i < centroids_base.size(); ++i) {
            if (distance(centroids_base.at(i), centroids_compare.at(i)) > tolerance) {
                ++n_different_centroids;
            }
        }
        if (n_different_centroids != 0) {
            fout << n_different_centroids << " DIFFERENT centroids";
        } else {
            fout << "centroids OK";
        }

        fout << std::endl;
    }

    fout << std::endl;
    fout << "No more files found in " << folder_path_compare << std::endl;
    fout.close();
    return EXIT_SUCCESS;
}
