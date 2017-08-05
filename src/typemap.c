#include <typemap.h>

MPI_Aint printdatatype( MPI_Datatype datatype, MPI_Aint prevExtentTot ) { 
	int *array_of_ints; 
	MPI_Aint *array_of_adds; 
	MPI_Datatype *array_of_dtypes; 
	int num_ints, num_adds, num_dtypes, combiner; 
	int i, j; 
 

	MPI_Type_get_envelope( datatype, &num_ints, &num_adds, &num_dtypes, &combiner ); 
	
	array_of_ints = (int *) malloc( num_ints * sizeof(int) ); 
	array_of_adds = (MPI_Aint *) malloc( num_adds * sizeof(MPI_Aint) ); 
	array_of_dtypes = (MPI_Datatype *) malloc( num_dtypes * sizeof(MPI_Datatype) );

	MPI_Aint extent, subExtent;
	MPI_Type_extent(datatype, &extent);

	switch (combiner) { 
	case MPI_COMBINER_NAMED: 
		// To print the specific type, we can match against the predefined forms.

		if (datatype == MPI_BYTE)					printf( "(MPI_BYTE, %ld)", prevExtentTot);
		else if (datatype == MPI_LB)				printf( "(MPI_LB, %ld)", prevExtentTot);
		else if (datatype == MPI_PACKED)			printf( "(MPI_PACKED, %ld)", prevExtentTot);
		else if (datatype == MPI_UB)				printf( "(MPI_UB, %ld)", prevExtentTot);
		else if (datatype == MPI_CHAR)				printf( "(MPI_CHAR, %ld)", prevExtentTot);
		else if (datatype == MPI_DOUBLE)			printf( "(MPI_DOUBLE, %ld)", prevExtentTot);
		else if (datatype == MPI_FLOAT)				printf( "(MPI_FLOAT, %ld)", prevExtentTot);
		else if (datatype == MPI_INT)				printf( "(MPI_INT, %ld)", prevExtentTot );
		else if (datatype == MPI_LONG)				printf( "(MPI_LONG, %ld)", prevExtentTot);
		else if (datatype == MPI_LONG_DOUBLE)		printf( "(MPI_LONG_DOUBLE, %ld)", prevExtentTot);
		else if (datatype == MPI_LONG_LONG)			printf( "(MPI_LONG_LONG, %ld)", prevExtentTot);
		else if (datatype == MPI_LONG_LONG_INT)		printf( "(MPI_LONG_LONG_INT, %ld)", prevExtentTot);
		else if (datatype == MPI_SHORT)				printf( "(MPI_SHORT, %ld)", prevExtentTot);
		else if (datatype == MPI_SIGNED_CHAR)		printf( "(MPI_SIGNED_CHAR, %ld)", prevExtentTot);
		else if (datatype == MPI_UNSIGNED)			printf( "(MPI_UNSIGNED, %ld)", prevExtentTot);
		else if (datatype == MPI_UNSIGNED_CHAR)		printf( "(MPI_UNSIGNED_CHAR, %ld)", prevExtentTot);
		else if (datatype == MPI_UNSIGNED_LONG)		printf( "(MPI_UNSIGNED_LONG, %ld)", prevExtentTot);
		else if (datatype == MPI_UNSIGNED_LONG_LONG)printf( "(MPI_UNSIGNED_LONG_LONG, %ld)", prevExtentTot);
		else if (datatype == MPI_UNSIGNED_SHORT)	printf( "(MPI_UNSIGNED_SHORT, %ld)", prevExtentTot);
		else if (datatype == MPI_WCHAR)				printf( "(MPI_WCHAR, %ld)", prevExtentTot);

		free( array_of_ints ); 
		free( array_of_adds ); 
		free( array_of_dtypes );

		return prevExtentTot;
		break;
	case MPI_COMBINER_DUP:
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 
		
		printdatatype( array_of_dtypes[0], prevExtentTot);

		printf(", ");

		break;
	case MPI_COMBINER_CONTIGUOUS:
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 

		MPI_Type_extent(array_of_dtypes[0], &subExtent); // no need to do in loop because same type

		for (i=0; i < array_of_ints[0]; i++) { 
			prevExtentTot = printdatatype( array_of_dtypes[0], prevExtentTot);
			prevExtentTot += subExtent;
			printf(", ");
		}

		break;
	case MPI_COMBINER_VECTOR:
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 

		MPI_Type_extent(array_of_dtypes[0], &subExtent); // no need to do in loop because same type

		printf("[");
		for (i = 0; i < array_of_ints[0]; i++) { //count
			printf( "BL : %d - ", array_of_ints[1]);
			for (j = 0; j < array_of_ints[2]; j++) { // stride
				if (j < array_of_ints[1]) { // if in blocklength
					prevExtentTot = printdatatype( array_of_dtypes[0], prevExtentTot);
					printf(", ");
				}
				prevExtentTot += subExtent;
				
			}
		}
		printf("], ");

		break;
	case MPI_COMBINER_HVECTOR:
	case MPI_COMBINER_HVECTOR_INTEGER:{
		MPI_Aint backupPrevExtent = prevExtentTot;

		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 

		MPI_Type_extent(array_of_dtypes[0], &subExtent); // no need to do in loop because same type
		
		printf("[");
		for (i = 0; i < array_of_ints[0]; i++) { //count
			printf( "BL : %d - ", array_of_ints[1]);
			for (j = 0; j < array_of_ints[1]; j++) { // blocklength
				prevExtentTot = printdatatype( array_of_dtypes[0], prevExtentTot);
				printf(", ");
				prevExtentTot += subExtent;
			}
			prevExtentTot = backupPrevExtent + array_of_adds[0]; // + stride un byte
		}
		printf("], ");

		break;
	}
	case MPI_COMBINER_INDEXED:{
		MPI_Aint tmpPrevExtent;
		int count, blocklength;
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 

		MPI_Type_extent(array_of_dtypes[0], &subExtent); // no need to do in loop because same type
		
		printf("<");
		count = array_of_ints[0];
		for (i = 0; i < count; i++) { // count
			blocklength = array_of_ints[i + 1]; // array of blocklength
			tmpPrevExtent = prevExtentTot;
			tmpPrevExtent += array_of_ints[count + 1 + i] * subExtent; // + displacement * size of block
			printf( "BL : %d - ", blocklength);
			for (j = 0; j < blocklength; j++) { // blocklength
				tmpPrevExtent = printdatatype( array_of_dtypes[0], tmpPrevExtent);
				printf(", ");
				tmpPrevExtent += subExtent;
			}
		}
		printf(">, ");

		prevExtentTot = tmpPrevExtent;

		break;
	}
	case MPI_COMBINER_HINDEXED:
	case MPI_COMBINER_HINDEXED_INTEGER:{
		MPI_Aint tmpPrevExtent;
		int count, blocklength;
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 

		MPI_Type_extent(array_of_dtypes[0], &subExtent); // no need to do in loop because same type
		
		printf("<");
		count = array_of_ints[0];
		for (i = 0; i < count; i++) { // count
			blocklength = array_of_ints[i + 1]; // array of blocklength
			tmpPrevExtent = prevExtentTot;
			tmpPrevExtent += array_of_adds[i]; // + displacement in byte
			printf( "BL : %d - ", blocklength);
			for (j = 0; j < blocklength; j++) {
				tmpPrevExtent = printdatatype( array_of_dtypes[0], tmpPrevExtent);
				printf(", ");
				tmpPrevExtent += subExtent;
			}
		}
		printf(">, ");

		prevExtentTot = tmpPrevExtent;

		break;
	}
	case MPI_COMBINER_INDEXED_BLOCK:{
		MPI_Aint tmpPrevExtent;
		int count;
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 

		MPI_Type_extent(array_of_dtypes[0], &subExtent); // no need to do in loop because same type
		
		printf("<");
		count = array_of_ints[0];
		for (i = 0; i < count; i++) { // count
			tmpPrevExtent = prevExtentTot;
			tmpPrevExtent += array_of_ints[i + 2] * subExtent; // + displacement * size of block
			printf( "BL : %d - ", array_of_ints[i + 1]);
			for (j = 0; j < array_of_ints[1]; j++) { // blocklength
				tmpPrevExtent = printdatatype( array_of_dtypes[0], tmpPrevExtent);
				printf(", ");
				tmpPrevExtent += subExtent;
			}
		}
		printf(">, ");

		prevExtentTot = tmpPrevExtent;

		break;
	}
	case MPI_COMBINER_STRUCT: 
	case MPI_COMBINER_STRUCT_INTEGER:{
		MPI_Aint tmpPrevExtent;
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes ); 
		
		printf( "{"); 
		for (i = 0; i < array_of_ints[0]; i++) { // count
			tmpPrevExtent = prevExtentTot + array_of_adds[i]; // origin + displacement
			printf( "BL : %d - ", array_of_ints[i + 1]);
			tmpPrevExtent = printdatatype( array_of_dtypes[i], tmpPrevExtent);
			tmpPrevExtent += subExtent;
			printf(", ");
		}
		printf("}, ");

		prevExtentTot = tmpPrevExtent;

		break;
	}
	case MPI_COMBINER_SUBARRAY:
		// I don't know what is interresting to display here...
		printf("... subarray not handled ...");
		break;
	case MPI_COMBINER_DARRAY:
		// Same
		printf("... darray not handled ...");
		break;
	case MPI_COMBINER_RESIZED:
		MPI_Type_get_contents( datatype, num_ints, num_adds, num_dtypes, array_of_ints, array_of_adds, array_of_dtypes );

		prevExtentTot = printdatatype( array_of_dtypes[0], prevExtentTot);

		printf(", ");

		break;
	default: 
		printf( "Unrecognized combiner type\n" ); 
	} 

	free( array_of_ints ); 
	free( array_of_adds ); 
	free( array_of_dtypes );

	return prevExtentTot;
}

void printMapDatatype(MPI_Datatype datatype) {
	MPI_Aint lb, ub;
	MPI_Type_lb(datatype, &lb);
	MPI_Type_ub(datatype, &ub);

	printf("\"(LB, %ld), ", lb);
	printdatatype(datatype, 0);
	printf("(UB, %ld)\"\n", ub);
}
