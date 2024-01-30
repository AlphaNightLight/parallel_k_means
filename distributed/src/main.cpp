#include "../include/point.h"
#include "../include/utils.h"
#include "../include/k_means.h"

#include <iostream>
#include <vector>
#include <fstream>

#define SHOW_DETAILS



int main(int argc, char** argv)
{
    std::string dataset_path;
    int n_points;
    std::string centroids_path;
    int n_clusters;
    int n_dimensions;

    std::string output_path;
    std::string measures_path;
    int epochs;
    double tolerance;

    std::vector<Observation> points;
    std::vector<Observation> centroids;

    double execution_time;
    std::ofstream fout;

    if (argc != 10) {
		std::cout << "usage: " << argv[0] << " <dataset path> <number of datapoints> <centroids path> \
                     <number of clusters> <number of dimensions> <output path> \
				     <measurements path> <epochs> <tolerance>" << std::endl;
        return EXIT_FAILURE;
	}

    dataset_path = argv[1];
    n_points = std::stod(argv[2]);
    centroids_path = argv[3];
    n_clusters = std::stod(argv[4]);
    n_dimensions = std::stod(argv[5]);

    output_path = argv[6];;
    measures_path = argv[7];
    epochs = std::stod(argv[8]);
    tolerance = std::stof(argv[9]);

    points = Utils::readPoints(dataset_path,n_points,n_dimensions,false);
    centroids = Utils::readPoints(centroids_path,n_clusters,n_dimensions,true);

    #ifdef SHOW_DETAILS
        std::cout << "Dimensions: " << n_dimensions << ", epochs: " << epochs
                  << ", tolerance: " << tolerance << std::endl;
        std::cout << points.size() << " points read from " << dataset_path << std::endl;
        std::cout << centroids.size() << " centroids read from " << centroids_path << std::endl;
    #endif



    execution_time = KMeans(points, centroids, epochs, tolerance);
    // Please note: KMeans() modifies points and centroids, as they are passed by reference.



    #ifdef SHOW_DETAILS
        std::cout << "The clustering function took " << execution_time << " seconds" << std::endl;
        std::cout << "Writing results in "  << output_path << " and time measurements in "
                  << measures_path << std::endl;
    #endif

    Utils::writePoints(output_path, points, centroids);

    fout.open(measures_path, std::ios_base::app);

    // Header structure: n_points,n_clusters,n_dimensions,execution_time
    fout << n_points << "," << n_clusters << "," << n_dimensions << ","
         << execution_time << std::endl;

    // Note: in output files we print centroids.size() as we're interested in the actual number of
    // final clusters the program found, while in measure files we print n_clusters as we're
    // interested in the initial parameter we gave to the program.

    fout.close();

    #ifdef SHOW_DETAILS
        std::cout << argv[0] << " terminated successfully!" << std::endl;
    #endif

    return EXIT_SUCCESS;
}
