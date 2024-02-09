export N_POINTS=65536
export N_CENTROIDS=64
export N_DIMENSIONS=8

export MAX_OMP_THREADS=64
export MAX_MPI_PROCS=256
export EPOCHS=128
export TOLERANCE=0.001

export LOWER_BOUND=-100.0
export UPPER_BOUND=100.0



data_folder=data
export DATA_FILE=$data_folder/data.csv
export CENTROIDS_FILE=$data_folder/centroids.csv

out_folder=out
export COMPARE_BASE_FILE=$out_folder/serial_out.csv
export COMPARE_FOLDER=$out_folder
export COMPARE_OUT_FILE=$out_folder/compare.txt



export OUT_SERIAL_FILE=$out_folder/serial_out.csv

export OUT_OMP_STATIC_STRONG_FILE=$out_folder/omp_static_strong_out.csv
export OUT_OMP_STATIC_WEAK_FILE=$out_folder/omp_static_weak_out.csv
export OUT_OMP_DYNAMIC_STRONG_FILE=$out_folder/omp_dynamic_strong_out.csv
export OUT_OMP_DYNAMIC_WEAK_FILE=$out_folder/omp_dynamic_weak_out.csv

export OUT_MPI_STRONG_FILE=$out_folder/mpi_strong_out.csv
export OUT_MPI_WEAK_FILE=$out_folder/mpi_weak_out.csv
export OUT_MPI_ASYNCH_STRONG_FILE=$out_folder/mpi_asynch_strong_out.csv
export OUT_MPI_ASYNCH_WEAK_FILE=$out_folder/mpi_asynch_weak_out.csv



measures_folder=measures
export MEASURES_SERIAL_FILE=$measures_folder/serial_measures.csv

export MEASURES_OMP_STATIC_STRONG_FILE=$measures_folder/omp_static_strong_measures.csv
export MEASURES_OMP_STATIC_WEAK_FILE=$measures_folder/omp_static_weak_measures.csv
export MEASURES_OMP_DYNAMIC_STRONG_FILE=$measures_folder/omp_dynamic_strong_measures.csv
export MEASURES_OMP_DYNAMIC_WEAK_FILE=$measures_folder/omp_dynamic_weak_measures.csv

export MEASURES_MPI_STRONG_FILE=$measures_folder/mpi_strong_measures.csv
export MEASURES_MPI_WEAK_FILE=$measures_folder/mpi_weak_measures.csv
export MEASURES_MPI_ASYNCH_STRONG_FILE=$measures_folder/mpi_asynch_strong_measures.csv
export MEASURES_MPI_ASYNCH_WEAK_FILE=$measures_folder/mpi_asynch_weak_measures.csv
