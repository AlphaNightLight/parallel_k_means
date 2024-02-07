CC := g++
CFLAGS := -std=c++11 -O3 #-Wextra -Werror -Wall
OMPFLAGS := -fopenmp
MPI_CC := mpicxx

BIN_FOLDER := bin
OBJ_FOLDER := obj
SRC_FOLDER := src

shared_files := $(OBJ_FOLDER)/point.o $(OBJ_FOLDER)/utils.o



all : k_means_serial k_means_omp_static k_means_omp_dynamic k_means_mpi k_means_mpi_asynch create_dataset

clean :
	rm -rf $(OBJ_FOLDER)/* $(BIN_FOLDER)/* logs/*

clean_everything :
	rm -rf $(OBJ_FOLDER)/* $(BIN_FOLDER)/* logs/* data/* out/* measures/* plots/*

k_means_serial k_means_omp_static k_means_omp_dynamic k_means_mpi k_means_mpi_asynch create_dataset compare_results : % : $(BIN_FOLDER)/%.exe



# OBJs
$(OBJ_FOLDER)/point.o $(OBJ_FOLDER)/utils.o $(OBJ_FOLDER)/main_serial.o $(OBJ_FOLDER)/k_means_serial.o $(OBJ_FOLDER)/compare_results.o : $(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.cpp
	$(CC) -c $(CFLAGS) $^ -o $@

$(OBJ_FOLDER)/k_means_omp_static.o $(OBJ_FOLDER)/k_means_omp_dynamic.o $(OBJ_FOLDER)/main_omp.o : $(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.cpp
	$(CC) -c $(CFLAGS) $(OMPFLAGS) $^ -o $@

$(OBJ_FOLDER)/k_means_mpi.o $(OBJ_FOLDER)/k_means_mpi_asynch.o $(OBJ_FOLDER)/main_mpi.o : $(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.cpp
	$(MPI_CC) -c $(CFLAGS) $^ -o $@

$(OBJ_FOLDER)/point_mpi.o : $(SRC_FOLDER)/point.cpp
	$(MPI_CC) -c $(CFLAGS) $^ -o $@

$(OBJ_FOLDER)/utils_mpi.o : $(SRC_FOLDER)/utils.cpp
	$(MPI_CC) -c $(CFLAGS) $^ -o $@



# Executables
$(BIN_FOLDER)/k_means_serial.exe : $(shared_files) $(OBJ_FOLDER)/main_serial.o $(OBJ_FOLDER)/k_means_serial.o
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_FOLDER)/k_means_omp_static.exe $(BIN_FOLDER)/k_means_omp_dynamic.exe : $(BIN_FOLDER)/%.exe : $(shared_files) $(OBJ_FOLDER)/main_omp.o $(OBJ_FOLDER)/%.o
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $@

$(BIN_FOLDER)/k_means_mpi.exe $(BIN_FOLDER)/k_means_mpi_asynch.exe : $(BIN_FOLDER)/%.exe : $(OBJ_FOLDER)/point_mpi.o $(OBJ_FOLDER)/utils_mpi.o $(OBJ_FOLDER)/main_mpi.o $(OBJ_FOLDER)/%.o
	$(MPI_CC) $(CFLAGS) $^ -o $@

$(BIN_FOLDER)/create_dataset.exe : $(SRC_FOLDER)/create_dataset.cpp
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_FOLDER)/compare_results.exe : $(shared_files) $(OBJ_FOLDER)/compare_results.o
	$(CC) $(CFLAGS) $^ -o $@
