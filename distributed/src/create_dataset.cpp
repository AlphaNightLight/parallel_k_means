#include <iostream>
#include <random>
#include <fstream>

// #define DEBUG

using namespace std;

int main(int argc, char** argv)
{
	int n_points;
	int n_dimensions;
	int n_clusters;
	float lower_bound;
	float upper_bound;
	int seed;
	ofstream fout;

	if (argc < 6 || argc > 7) {
		std::cout << "usage: " << argv[0] << " <number of datapoints> <number of clusters> <number of dimensions> <lower bound> <upper bound> [<seed>]" << std::endl;
        return EXIT_FAILURE;
	}

	n_points = atoi(argv[1]);
	n_clusters = atoi(argv[2]);
	n_dimensions = atoi(argv[3]);
	lower_bound = atof(argv[4]);
	upper_bound = atof(argv[5]);

	if (argc == 7) {
		seed = atoi(argv[6]);
	} else {
		random_device rd;
		seed = rd();
	}

	#ifdef DEBUG
		cout << "n_points: " << n_points << ", n_clusters: " << n_clusters << ", n_dimensions: "
			 << n_dimensions << ", lower_bound: " << lower_bound << ", upper_bound: "
			 << upper_bound << ", seed: " << seed << endl;
	#endif

	default_random_engine dre(seed);
	uniform_real_distribution<double> uniform(lower_bound, upper_bound);

	fout.open("../data/data.csv");

	// Note: lower_bound, upper_bound and seed are memorized just for reproducibility
	fout << "n_points,n_clusters,n_dimensions,lower_bound,upper_bound,seed" << endl;
	fout << n_points << "," << n_clusters << "," << n_dimensions << "," << lower_bound << ","
		 << upper_bound << "," << seed << endl;

	for (size_t i = 0; i < n_points; ++i) {
		for (size_t j = 0; j < n_dimensions; ++j) {
			fout << uniform(dre);
			if (j != n_dimensions - 1) {
				fout << ",";
			}
		}
		fout << endl;
	}

	fout.close();
	fout.open("../data/centroids.csv");

	fout << "n_points,n_clusters,n_dimensions,lower_bound,upper_bound,seed" << endl;
	fout << n_points << "," << n_clusters << "," << n_dimensions << "," << lower_bound << ","
		 << upper_bound << "," << seed << endl;

	for (size_t i = 0; i < n_clusters; ++i) {
		fout << i << ","; // It's the cluster index, printed to have a rapid eye comparison
		for (size_t j = 0; j < n_dimensions; ++j) {
			fout << uniform(dre);
			if (j != n_dimensions - 1) {
				fout << ",";
			}
		}
		fout << endl;
	}

	fout.close();
	return EXIT_SUCCESS;
}
