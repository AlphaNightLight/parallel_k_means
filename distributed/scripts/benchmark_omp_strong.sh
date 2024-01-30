# OMP strong scaling
for ((i = 1 ; i <= $MAX_OMP_THREADS ; i*=2))
do
	export OMP_NUM_THREADS=$i; ./bin/k_means_omp.exe $DATA_FILE $N_POINTS $CENTROIDS_FILE $N_CENTROIDS $N_DIMENSIONS $OUT_OMP_FILE $MEASURES_OMP_FILE $EPOCHS $TOLERANCE
done
