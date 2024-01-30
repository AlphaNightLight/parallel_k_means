# MPI strong scaling
for ((i = 1 ; i <= $MAX_MPI_PROCS ; i*=2))
do
	mpiexec -np $i ./bin/k_means_mpi.exe $DATA_FILE $N_POINTS $CENTROIDS_FILE $N_CENTROIDS $N_DIMENSIONS $OUT_MPI_FILE $MEASURES_MPI_FILE $EPOCHS $TOLERANCE
done
