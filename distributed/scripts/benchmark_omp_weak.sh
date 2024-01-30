# OMP weak scaling
for ((i = 1 ; i <= $MAX_OMP_THREADS ; i*=2))
do
	((j = $i * $N_POINTS))
	export OMP_NUM_THREADS=$i; ./bin/k_means_omp.exe $DATA_FILE $j $CENTROIDS_FILE $N_CENTROIDS $N_DIMENSIONS $OUT_OMP_FILE $MEASURES_OMP_FILE $EPOCHS $TOLERANCE
done
