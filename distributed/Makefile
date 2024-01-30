CC := g++
CFLAGS := -std=c++11 -O3 #-Wextra -Werror -Wall
OMPFLAGS := -fopenmp
MPI_CC := mpicxx

BIN_FOLDER := bin
OBJ_FOLDER := obj
SRC_FOLDER := src



all : k_means_serial k_means_omp k_means_mpi create_dataset

clean :
	rm -rf $(OBJ_FOLDER)/* $(BIN_FOLDER)/* pbs/*

clean_everything :
	rm -rf $(OBJ_FOLDER)/* $(BIN_FOLDER)/* pbs/* data/* out/* measures/* plots/*



# OBJs
$(OBJ_FOLDER)/point.o $(OBJ_FOLDER)/utils.o $(OBJ_FOLDER)/main.o $(OBJ_FOLDER)/k_means_serial.o : $(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.cpp
	$(CC) -c $(CFLAGS) $^ -o $@

$(OBJ_FOLDER)/k_means_omp.o : $(SRC_FOLDER)/k_means_omp.cpp
	$(CC) -c $(CFLAGS) $(OMPFLAGS) $^ -o $@

$(OBJ_FOLDER)/k_means_mpi.o : $(SRC_FOLDER)/k_means_mpi.cpp
	$(MPI_CC) -c $(CFLAGS) $^ -o $@



# Executables
shared_files = $(OBJ_FOLDER)/point.o $(OBJ_FOLDER)/utils.o $(OBJ_FOLDER)/main.o

k_means_serial : $(shared_files) $(OBJ_FOLDER)/k_means_serial.o
	$(CC) $(CFLAGS) $^ -o $(BIN_FOLDER)/$@.exe

k_means_omp : $(shared_files) $(OBJ_FOLDER)/k_means_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $(BIN_FOLDER)/$@.exe

k_means_mpi : $(shared_files) $(OBJ_FOLDER)/k_means_mpi.o
	$(MPI_CC) $(CFLAGS) $^ -o $(BIN_FOLDER)/$@.exe

create_dataset : $(SRC_FOLDER)/create_dataset.cpp
	$(CC) $(CFLAGS) $^ -o $(BIN_FOLDER)/$@.exe
