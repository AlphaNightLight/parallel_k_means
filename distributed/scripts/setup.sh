mkdir -p obj
mkdir -p bin

mkdir -p data
mkdir -p out
mkdir -p measures

mkdir -p plots
mkdir -p pbs



make k_means_serial && echo k_means_serial built successfully
make k_means_omp  && echo k_means_omp built successfully
make k_means_mpi  && echo k_means_mpi built successfully
make create_dataset  && echo create_dataset built successfully



echo >$MEASURES_SERIAL_FILE n_points,n_clusters,n_dimensions,execution_time
echo >$MEASURES_OMP_FILE n_points,n_clusters,n_dimensions,execution_time
echo >$MEASURES_MPI_FILE n_points,n_clusters,n_dimensions,execution_time



(( WEAK_N_POINTS = $MAX_MPI_PROCS>$MAX_OMP_THREADS ? $N_POINTS*$MAX_MPI_PROCS : $N_POINTS*$MAX_OMP_THREADS ))
./bin/create_dataset.exe $DATA_FILE $WEAK_N_POINTS $N_DIMENSIONS $LOWER_BOUND $UPPER_BOUND && echo dataset created successfully
./bin/create_dataset.exe $CENTROIDS_FILE $N_CENTROIDS $N_DIMENSIONS $LOWER_BOUND $UPPER_BOUND && echo centroids created successfully
