#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include "typemap.h"

#include <signal.h>
#include <stddef.h>

void debug() {
	raise(SIGINT);
}

typedef struct car_s {
		int shifts;
		int topSpeed;
} car;

int main(int argc, char **argv) {

	/*
	 * Init MPI
	 */
	int rank, worldSize;
	MPI_Init ( &argc, &argv );
	MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
	MPI_Comm_size ( MPI_COMM_WORLD, &worldSize );


	/*
	 * Init global stuff that we gonna need
	 */
	MPI_Datatype resized, duplicate, contiguous, rescont, vect, hvect, index, hindex;
	MPI_Datatype index_block, mpi_car_type; //sub_matrix;

	int count;
	int blocklength;
	int stride;
	MPI_Aint hstride;
	int * blocklengths;
	int * displacements;
	MPI_Aint * hdisplacements;
	MPI_Datatype * types;



	/*
	 * ########################################
	 *                  TESTS
	 * ########################################
	 */
	// resize
	printf("resize\n");

	MPI_Type_create_resized(MPI_INT, -3, 9, &resized);
	MPI_Type_commit(&resized);
	printMapDatatype(resized);

	printf("\n");


	// duplicate
	printf("duplicate\n");

	MPI_Type_dup(MPI_INT, &duplicate);
	MPI_Type_commit(&duplicate);
	printMapDatatype(duplicate);

	printf("\n");

	// contiguous
	printf("contiguous\n");
	count = 2;

	MPI_Type_contiguous(count, MPI_INT, &contiguous);  
	MPI_Type_commit(&contiguous);
	printMapDatatype(contiguous);

	printf("\n");


	// contiguous + resize
	printf("contiguous + resize\n");
	count = 2;
	MPI_Type_contiguous(count, resized, &rescont);  
	MPI_Type_commit(&rescont);
	printMapDatatype(rescont);

	printf("\n");


	// vector
	printf("vector\n");
	count = 2;
	blocklength = 3;
	stride = 5;

	MPI_Type_vector(count, blocklength, stride, MPI_INT, &vect);
	MPI_Type_commit(&vect);
	printMapDatatype(vect);

	printf("\n");


	// hvector
	printf("hvector\n");\
	count = 2;
	blocklength = 3;
	hstride = 5 * sizeof(int);

	MPI_Type_hvector(count, blocklength, hstride, MPI_INT, &hvect);
	MPI_Type_commit(&hvect);
	printMapDatatype(hvect);

	printf("\n");


	// indexed
	printf("indexed\n");
	count = 3;
	blocklengths = (int *) malloc( count * sizeof(int) );
	displacements = (int *) malloc( count * sizeof(int) );
	blocklengths[0] = 4;
	blocklengths[1] = 3;
	blocklengths[2] = 1;
	displacements[0] = 0;
	displacements[1] = 5;
	displacements[2] = 11;

	MPI_Type_indexed(3, blocklengths, displacements, MPI_INT, &index);
	MPI_Type_commit(&index);
	printMapDatatype(index);

	free(blocklengths);
	free(displacements);
	printf("\n");


	// hindexed
	printf("hindexed\n");
	count = 3;
	blocklengths = (int *) malloc( count * sizeof(int) );
	blocklengths[0] = 4;
	blocklengths[1] = 3;
	blocklengths[2] = 1;
	hdisplacements = (MPI_Aint *) malloc( count * sizeof(MPI_Aint) );
	hdisplacements[0] = 0 * sizeof(int);
	hdisplacements[1] = 5 * sizeof(int);
	hdisplacements[2] = 11 * sizeof(int);

	MPI_Type_hindexed(3, blocklengths, hdisplacements, MPI_INT, &hindex);
	MPI_Type_commit(&hindex);
	printMapDatatype(hindex);

	free(blocklengths);
	free(hdisplacements);
	printf("\n");


	// indexed block
	printf("indexed block\n");
	count = 3;
	blocklength = 4;
	displacements = (int *) malloc( count * sizeof(int) );
	displacements[0] = 0;
	displacements[1] = 5;
	displacements[2] = 11;

	MPI_Type_create_indexed_block(count, blocklength, displacements, MPI_INT, &index_block);
	MPI_Type_commit(&index_block);
	printMapDatatype(index_block);

	free(displacements);
	printf("\n");


	// structure
	printf("structure\n");
	count = 2;
	blocklengths = (int *) malloc( 2 * sizeof(int) );
	blocklengths[0] = 1;
	blocklengths[1] = 1;
	hdisplacements = (MPI_Aint *) malloc( 2 * sizeof(MPI_Aint) );
	hdisplacements[0] = offsetof(car, shifts);
	hdisplacements[1] = offsetof(car, topSpeed);
	types = (MPI_Datatype *) malloc( 2 * sizeof(int) );
	types[0] = MPI_INT;
	types[1] = MPI_INT;

	MPI_Type_create_struct(count, blocklengths, hdisplacements, types, &mpi_car_type);
	MPI_Type_commit(&mpi_car_type);
	printMapDatatype(mpi_car_type);

	free(blocklengths);
	free(hdisplacements);
	free(types);
	printf("\n");


	// //subarray
	// printf("subarray\n");
	// int nb_dims = 2;
	// int size_y = 10;
	// int size_x = 10;
	// int * sizes = (int *) malloc( nb_dims * sizeof(int) );
	// int * sub_sizes = (int *) malloc( nb_dims * sizeof(int) );
	// int * start = (int *) malloc( nb_dims * sizeof(int) );

	// sizes[0] = size_y;
	// sizes[1] = size_x;
	// sub_sizes[0] = 3;
	// sub_sizes[1] = 4;
	// start[0] = 2;
	// start[1] = 4;

	// int ** matrix = (int **) malloc ( size_y * sizeof(int *) );
	// for (int i = 0; i < size_y; ++i) {
	// 	matrix[i] = (int *) malloc( size_x * sizeof(int) );
	// 	for (int j = 0; j < size_x; j++) {
	// 		matrix[i][j] = i + j;
	// 	}
	// }

	// MPI_Type_create_subarray(nb_dims, sizes, sub_sizes, start, MPI_ORDER_C, MPI_INT, &sub_matrix); //only handle c for the moment
	// MPI_Type_commit(&sub_matrix);
	// printMapDatatype(sub_matrix);

	// for (int i = 0; i < size_y; ++i) {
	// 	free(matrix[i]);
	// }
	// free(matrix);
	// free(sizes);
	// free(sub_sizes);
	// free(start);
	// printf("\n");



	/*
	 * Proper close
	 */
	MPI_Type_free(&resized);
	MPI_Type_free(&duplicate);
	MPI_Type_free(&contiguous);
	MPI_Type_free(&rescont);
	MPI_Type_free(&vect);
	MPI_Type_free(&hvect);
	MPI_Type_free(&index);
	MPI_Type_free(&hindex);
	MPI_Type_free(&index_block);
	MPI_Type_free(&mpi_car_type);
	// MPI_Type_free(&sub_matrix);


	MPI_Finalize();

	return 0;
}
