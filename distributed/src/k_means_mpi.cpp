#include <mpi.h>

#include "../include/k_means.h"
#include "../include/point.h"
#include "../include/utils.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#define MASTER 0



double arrayDistance(double* x, double* y, int dim);
double** allocateMatrix(int rows, int cols);
void deAllocateMatrix(double** M, int rows, int cols);
double** pointToMatrix(std::vector<Observation> &points);
void debugPrint(std::ofstream& fout, int n_points_per_core, int n_clusters, int n_dimensions,
                double** P, double** C, double** cumulative, int* P_cluster_ID, int* C_point_counter);



double KMeans(std::vector<Observation> &points, std::vector<Observation> &centroids,
              unsigned int epochs, double tolerance)
{
    int my_rank, size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double start_time, end_time;
    start_time = MPI_Wtime();

    int n_points;
    int n_points_per_core;
    int n_dimensions;
    int n_clusters;
    int* n_broadcast;

    n_points = 0;
    n_broadcast = new int[3];

    if(my_rank == MASTER) {
        n_points = points.size();
        n_broadcast[0] = n_points / size;
        n_broadcast[1] = points.at(0).getFeatureSize();
        n_broadcast[2] = centroids.size();
    }

    MPI_Bcast(n_broadcast, 3, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    n_points_per_core = n_broadcast[0];
    n_dimensions = n_broadcast[1];
    n_clusters = n_broadcast[2];



    double** P;
    double** C;
    double** cumulative;
    int* P_cluster_ID;
    int* C_point_counter;
    int* C_point_counter_recieve; // Needed because MPI_Allreduce wants different send and recieve buffers.
    double* old_centroid;

    if(my_rank == MASTER) {
        P = pointToMatrix(points);
        C = pointToMatrix(centroids);
        P_cluster_ID = new int[n_points];
    } else {
        P = allocateMatrix(n_points_per_core, n_dimensions);
        C = allocateMatrix(n_clusters, n_dimensions);
        P_cluster_ID = new int[n_points_per_core];
    }
    cumulative = allocateMatrix(n_clusters, n_dimensions);
    C_point_counter = new int[n_clusters];
    C_point_counter_recieve = new int[n_clusters];
    old_centroid = new double[n_dimensions];

    if(my_rank == MASTER) {
        for (size_t i = 0; i < n_points; ++i) {
            int dest = i / size;
            if (dest != MASTER) {
                MPI_Send(P[i], n_dimensions, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        for (size_t i = 0; i < n_points_per_core; ++i) {
            MPI_Recv(P[i], n_dimensions, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    for (size_t i = 0; i < n_clusters; ++i) {
        MPI_Bcast(C[i], n_dimensions, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    }

    for (size_t i = 0; i < n_points_per_core; ++i) {
        P_cluster_ID[i] = 0;
    }



    /*debug*
    std::string title = "debug_";
    title += std::to_string(my_rank);
    title += ".txt";
    std::ofstream fout(title);
    fout << "I'm the mpi code " << my_rank << "/" << size << " !" << std::endl;
    /**/

    for (size_t e = 0; e < epochs; ++e) {
        double delta = 0.0;

        for (size_t cid = 0; cid < n_clusters; ++cid) {
            C_point_counter[cid] = 0;
        }

        for (size_t cid = 0; cid < n_clusters; ++cid) {
            for (size_t dm = 0; dm < n_dimensions; ++dm) {
                cumulative[cid][dm] = 0;
            }
        }

        for (size_t point = 0; point < n_points_per_core; ++point) {
            double dist = arrayDistance(P[point], C[ P_cluster_ID[point] ], n_dimensions);
            for (size_t cid = 0; cid < n_clusters; ++cid) {
                const double newDist = arrayDistance(P[point], C[cid], n_dimensions);
                if (newDist < dist) {
                    P_cluster_ID[point] = cid;
                    dist = newDist;
                }
            }

            C_point_counter[ P_cluster_ID[point] ] += 1;
            for (size_t dm = 0; dm < n_dimensions; ++dm) {
                cumulative[ P_cluster_ID[point] ][dm] += P[point][dm];
            }
        }

        /*debug*
        debugPrint(fout, n_points_per_core, n_clusters, n_dimensions, P, C,
                   cumulative, P_cluster_ID, C_point_counter);
        /**/

        MPI_Allreduce(C_point_counter,C_point_counter_recieve,n_clusters,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
        int* C_point_counter_temp;
        C_point_counter_temp = C_point_counter;
        C_point_counter = C_point_counter_recieve;
        C_point_counter_recieve = C_point_counter_temp;

        for (size_t cid = 0; cid < n_clusters; ++cid) {
            for (size_t dm = 0; dm < n_dimensions; ++dm) {
                old_centroid[dm] = C[cid][dm];
            }
            MPI_Allreduce(cumulative[cid],C[cid],n_dimensions,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
            for (size_t dm = 0; dm < n_dimensions; ++dm) {
                if (C_point_counter[cid] == 0) {
                    C[cid][dm] = 0.0;
                } else {
                    C[cid][dm] = C[cid][dm] / C_point_counter[cid];
                }
            }
            delta += arrayDistance(old_centroid,C[cid],n_dimensions);
        }

        /*debug*
        debugPrint(fout, n_points_per_core, n_clusters, n_dimensions, P, C,
                   cumulative, P_cluster_ID, C_point_counter);
        /**/

        if (delta < tolerance) break;
    }



    if(my_rank == MASTER) {
        for (size_t i = 0; i < n_points; ++i) {
            int src = i / size;
            if (src != MASTER) {
                MPI_Recv(P_cluster_ID + i, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            points.at(i).setClusterID(P_cluster_ID[i]);
        }
    } else {
        for (size_t i = 0; i < n_points_per_core; ++i) {
            MPI_Send(P_cluster_ID + i, 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
        }
    }

    if(my_rank == MASTER) {
        for (size_t cid = 0; cid < n_clusters; ++cid) {
            for (size_t dm = 0; dm < n_dimensions; ++dm) {
                centroids.at(cid).setFeatures(dm, C[cid][dm]);
            }
        }
    }

    /*debug*
    fout.close();
    /**/

    delete[] n_broadcast;

    if(my_rank == MASTER) {
        deAllocateMatrix(P, n_points, n_dimensions);
    } else {
        deAllocateMatrix(P, n_points_per_core, n_dimensions);
    }
    deAllocateMatrix(C, n_clusters, n_dimensions);
    deAllocateMatrix(cumulative, n_clusters, n_dimensions);

    delete[] P_cluster_ID;
    delete[] C_point_counter;
    delete[] C_point_counter_recieve;
    delete [] old_centroid;

    end_time = MPI_Wtime();
    return end_time - start_time;
}





double** allocateMatrix(int rows, int cols)
{
    double** M = new double*[rows];
    for (size_t i = 0; i < rows; ++i) {
        M[i] = new double[cols];
    }
    return M;
}

void deAllocateMatrix(double** M, int rows, int cols)
{
    for (size_t i = 0; i < rows; ++i) {
		delete[] M[i];
	}
	delete[] M;
}

double** pointToMatrix(std::vector<Observation> &points)
{
    int n_points = points.size();
    int n_dimensions = points.at(0).getFeatureSize();
    double** M = allocateMatrix(n_points, n_dimensions);

    for (size_t i = 0; i < n_points; ++i) {
        for (size_t j = 0; j < n_dimensions; ++j) {
            M[i][j] = points.at(i).getFeatures(j);
        }
    }

    return M;
}

double arrayDistance(double* x, double* y, int dim)
{
    // Euclidean distance
    double sum = 0.0;
    for (size_t i = 0; i < dim; ++i) {
        sum += std::pow(x[i] - y[i], 2);
    }
    return std::sqrt(sum);
}

void debugPrint(std::ofstream& fout, int n_points_per_core, int n_clusters, int n_dimensions,
                double** P, double** C, double** cumulative, int* P_cluster_ID, int* C_point_counter)
{
    fout << "*** P ***" << std::endl;
    for (size_t i = 0; i < n_points_per_core; ++i) {
        for (size_t j = 0; j < n_dimensions; ++j) {
            fout << P[i][j] << ", ";
        }
        fout << std::endl;
    }

    fout << "*** C ***" << std::endl;
    for (size_t i = 0; i < n_clusters; ++i) {
        for (size_t j = 0; j < n_dimensions; ++j) {
            fout << C[i][j] << ", ";
        }
        fout << std::endl;
    }

    fout << "*** cumulative ***" << std::endl;
    for (size_t i = 0; i < n_clusters; ++i) {
        for (size_t j = 0; j < n_dimensions; ++j) {
            fout << cumulative[i][j] << ", ";
        }
        fout << std::endl;
    }

    fout << "*** P_cluster_ID ***" << std::endl;
    for (size_t i = 0; i < n_points_per_core; ++i) {
        fout << P_cluster_ID[i] << ", ";
    }
    fout << std::endl;

    fout << "*** C_point_counter ***" << std::endl;
    for (size_t i = 0; i < n_clusters; ++i) {
        fout << C_point_counter[i] << ", ";
    }
    fout << std::endl;

    fout << "_____" << std::endl;
}
