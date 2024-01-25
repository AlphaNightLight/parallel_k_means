#include <iostream>
#include <random>
#include <fstream>

// #define DEBUG

int main(int argc, char** argv)
{
	int n_points;
	int n_dimensions;
	float lower_bound;
	float upper_bound;
	int seed;
	std::ofstream fout;

	if (argc < 6 || argc > 7) {
		std::cout << "usage: " << argv[0] << " <path> <number of datapoints> \
					 <number of dimensions> <lower bound> <upper bound> [<seed>]" << std::endl;
        return EXIT_FAILURE;
	}

	n_points = std::stod(argv[2]);
	n_dimensions = std::stod(argv[3]);
	lower_bound = std::stof(argv[4]);
	upper_bound = std::stof(argv[5]);

	if (argc == 7) {
		seed = std::stod(argv[6]);
	} else {
		std::random_device rd;
		seed = rd();
	}

	#ifdef DEBUG
		std::cout << "n_points: " << n_points << ", n_dimensions: " << n_dimensions
			 << ", lower_bound: " << lower_bound << ", upper_bound: " << upper_bound
			 << ", seed: " << seed << std::endl;
	#endif

	std::default_random_engine dre(seed);
	std::uniform_real_distribution<double> uniform(lower_bound, upper_bound);

	fout.open(argv[1]);
	if (!fout.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

	// Note: lower_bound, upper_bound and seed are memorized just for reproducibility
	fout << "n_points,n_dimensions,lower_bound,upper_bound,seed" << std::endl;
	fout << n_points << "," << n_dimensions << "," << lower_bound << ","
		 << upper_bound << "," << seed << std::endl;

	for (size_t i = 0; i < n_points; ++i) {
		for (size_t j = 0; j < n_dimensions; ++j) {
			fout << uniform(dre);
			if (j != n_dimensions - 1) {
				fout << ",";
			}
		}
		fout << std::endl;
	}

	fout.close();
	return EXIT_SUCCESS;
}
