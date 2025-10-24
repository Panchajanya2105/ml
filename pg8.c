#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char* argv[]) {
 int rank, size;
 const int elements_per_proc = 2; // Number of elements per process
 MPI_Init(&argc, &argv); // Initialize MPI
 MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get process rank
 MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes
 int total_elements = elements_per_proc * size;
 int *data = NULL;
 if (rank == 0) {
 // Root initializes an array of data
 data = (int *)malloc(sizeof(int) * total_elements);
 for (int i = 0; i < total_elements; i++) {
 data[i] = i + 1;
 }
 printf("Root process has data to scatter:\n");
 for (int i = 0; i < total_elements; i++) {
 printf("%d ", data[i]);
 }
 printf("\n");
 }
 // Each process receives elements_per_proc integers
 int *sub_data = (int *)malloc(sizeof(int) * elements_per_proc);
 // Scatter the data from root to all processes
 MPI_Scatter(data, elements_per_proc, MPI_INT,
 sub_data, elements_per_proc, MPI_INT,
 0, MPI_COMM_WORLD);
 // Each process modifies the received data (e.g., multiply by 2)
 for (int i = 0; i < elements_per_proc; i++) {
 sub_data[i] *= 2;
 }
 // Gather the modified data back to root
 MPI_Gather(sub_data, elements_per_proc, MPI_INT,
 data, elements_per_proc, MPI_INT,
 0, MPI_COMM_WORLD);
 // Root prints the gathered result
 if (rank == 0) {
 printf("\nRoot process received modified data from all processes:\n");
 for (int i = 0; i < total_elements; i++) {
 printf("%d ", data[i]);
 }
 printf("\n");
 free(data);
 }
 free(sub_data);
 MPI_Finalize();
 return 0;
}
