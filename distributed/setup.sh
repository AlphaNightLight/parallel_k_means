mkdir -p obj
mkdir -p bin
mkdir -p logs

mkdir -p data
mkdir -p out
mkdir -p measures

mkdir -p plots
mkdir -p plots/out
mkdir -p plots/measures



source params.sh
header_serial="n_points,n_clusters,n_dimensions,execution_time\n"
printf >$MEASURES_SERIAL_FILE $header_serial

header_omp="omp_num_threads,n_points,n_clusters,n_dimensions,execution_time\n"
printf >$MEASURES_OMP_STATIC_STRONG_FILE $header_omp
printf >$MEASURES_OMP_STATIC_WEAK_FILE $header_omp
printf >$MEASURES_OMP_DYNAMIC_STRONG_FILE $header_omp
printf >$MEASURES_OMP_DYNAMIC_WEAK_FILE $header_omp

header_mpi="mpi_size,n_points,n_clusters,n_dimensions,execution_time\n"
printf >$MEASURES_MPI_STRONG_FILE $header_mpi
printf >$MEASURES_MPI_WEAK_FILE $header_mpi
printf >$MEASURES_MPI_ASYNCH_STRONG_FILE $header_mpi
printf >$MEASURES_MPI_ASYNCH_WEAK_FILE $header_mpi
