CC := g++
CFLAGS := -std=c++11 -O3 -Wextra -Werror -Wall
OMPFLAGS := -fopenmp
MPI_CC := mpicxx

BIN_FOLDER := bin
OBJ_FOLDER := obj
SRC_FOLDER := src



all : k_means_serial k_means_omp k_means_mpi create_dataset

clean :
	rm -rf $(OBJ_FOLDER)/* $(BIN_FOLDER)/*

clean_everything :
	rm -rf $(OBJ_FOLDER)/* $(BIN_FOLDER)/* data/* out/* measures/* plots/*



# OBJs
$(OBJ_FOLDER)/point.o $(OBJ_FOLDER)/utils.o $(OBJ_FOLDER)/main.o $(OBJ_FOLDER)/k_means_serial.o : $(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.cpp
	@mkdir -p $(OBJ_FOLDER)
	$(CC) -c $(CFLAGS) $^ -o $@

$(OBJ_FOLDER)/k_means_omp.o : $(SRC_FOLDER)/k_means_omp.cpp
	@mkdir -p $(OBJ_FOLDER)
	$(CC) -c $(CFLAGS) $(OMPFLAGS) $^ -o $@

$(OBJ_FOLDER)/k_means_mpi.o : $(SRC_FOLDER)/k_means_mpi.cpp
	@mkdir -p $(OBJ_FOLDER)
	$(MPI_CC) -c $^ -o $@



# Executables
shared_files = $(OBJ_FOLDER)/point.o $(OBJ_FOLDER)/utils.o $(OBJ_FOLDER)/main.o

k_means_serial : $(shared_files) $(OBJ_FOLDER)/k_means_serial.o
	@mkdir -p $(BIN_FOLDER)
	$(CC) $(CFLAGS) $^ -o $(BIN_FOLDER)/$@.exe

k_means_omp : $(shared_files) $(OBJ_FOLDER)/k_means_omp.o
	@mkdir -p $(BIN_FOLDER)
	$(CC) $(CFLAGS) $^ -o $(BIN_FOLDER)/$@.exe

k_means_mpi : $(shared_files) $(OBJ_FOLDER)/k_means_mpi.o
	@mkdir -p $(BIN_FOLDER)
	$(MPI_CC) $^ -o $(BIN_FOLDER)/$@.exe

create_dataset : $(SRC_FOLDER)/create_dataset.cpp
	@mkdir -p $(BIN_FOLDER)
	$(CC) -c $(CFLAGS) $^ -o $(BIN_FOLDER)/$@.exe
