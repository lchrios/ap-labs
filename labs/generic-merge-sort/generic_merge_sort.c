#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "intop.h"
//#include "strop.h"

void CopyArray(void *array1, void *array2, int arrLen, int index, 
		void Assign(void*, void*, int i, int j))
{
	int i;
	printf("Copying array...\n\t");
	for (i = 0; i < arrLen; i++)
		Assign(array1, array2, i, index+i);
	printf("Copying completed.\n");
}

void Merge(void *array, void* bottom_half, void* top_half, int arrLen,
	  int Compare(void*, void*, int, int), void Assign(void*, void*, int, int))
{
	int i, bot_i = 0, top_i = 0;
	
	printf("Merging arrays...\n");
	for (i = 0; i < arrLen; i++) // for each cell in the result array
	{
		// compare lesser values in arrays
		if (top_i == (arrLen/2 + arrLen%2) || ((bot_i < arrLen/2) && 
			Compare(bottom_half, top_half, bot_i, top_i))) {
			// if bot_i is smaller
			printf("%s > %s | bot_i smaller\n", ((char *)top_half)[top_i], ((char *)bottom_half)[bot_i]);
			Assign(array, bottom_half, i, bot_i);
			bot_i++; // next item in bottom_half
		}
		else {
			// if top_i is smaller
			printf("%s > %s | top_i smaller\n", ((char *)bottom_half)[bot_i], ((char *)top_half)[top_i]);
			Assign(array, top_half, i, top_i);
			top_i++; // next item in top_half
		}
		
	}
}

void Mergesort(void* array, int arrLen, int Size(void),
		int Compare(void*, void*, int, int), 
		void Assign(void*, void*, int, int))
{
	void* bottom_half; // of the array
	void* top_half; // of the array
	
	// array is only item
	if (arrLen == 1)
		printf("Only IT: %s", ((char *) array)[0]);
		return;
	
	// allocate memory for bottom half and top half
	bottom_half = malloc(arrLen/2 * Size());
	top_half = malloc((arrLen/2 + arrLen%2) * Size());
	
	// check memory allocations
	if (!bottom_half || !top_half) {
		printf("Could not allocate memory");
		exit(EXIT_FAILURE); // exit
	}
	
	// copy arrays
	printf("Copying first half:\n\t");
	CopyArray(bottom_half, array, arrLen/2, 0, Assign);
	printf("Copying last half:\n\t");
	CopyArray(top_half, array, (arrLen/2 + arrLen%2), arrLen/2, Assign);
	
	// sort each half recursively
	printf("Merge sorting first half...\n");
	Mergesort(bottom_half, arrLen/2, Size, Compare, Assign);
	printf("Merge sorting later half...\n");
	Mergesort(top_half, (arrLen/2 + arrLen%2), Size, Compare, Assign);
	
	// merge them
	printf("Merging both halves...\n");
	Merge(array, bottom_half, top_half, arrLen, Compare, Assign);
		
	// free allocated memory
	free(bottom_half);
	free(top_half);
}

int getFileLineLen(FILE *fileptr){
	int cnt = 0;
	char chr = getc(fileptr);

	while (chr != EOF) {
		if (chr == '\n') {
			cnt++;
		}
		chr = getc(fileptr);
	}
	fclose(fileptr);
	return cnt;
}

// compare a cell in 1 array to a cell in another array
int StrCompare(void* array1, void* array2, int i, int j) {
    return strcmp(((const char *) array1 + i), ((const char *) array2 + i));
}
// assign value from one array to another
void StrAssign(void* array1, void* array2, int i, int j) {
	((char *)array1)[i] = ((char *)array2)[j];
}
// returns sizeof(char * 64)
int StrSize(void) {
	return sizeof(char) * 64;
}
int IntCompare(void* array1, void* array2, int i, int j) {
	return (((int*)array1)[i] <= ((int*)array2)[j]);
}
// assign value from one array to another
void IntAssign(void* array1, void* array2, int i, int j) {
	((int *)array1)[i] = ((int *)array2)[j];
}
// returns sizeof(int)
int IntSize(void) {
	return sizeof(int);
}

void printArray(char *array[], int len) {
	for (int i = 0; i < len; i++) {
		printf("%s\n", array + i);
	}
}

void printIntArray(int *array, int len) {
	for (int i = 0; i < len; i++) {
		printf("%d\n", array[i]);
	}
}

int main(int argc, char *argv[])
{	
	char *location;
	char *line = NULL;
	ssize_t read;
	// Merge numbers
	if (strcmp(argv[1], "-n") == 0 && argv[2] != NULL) {
		printf("Merging numbers...\n");
		location = argv[2];
		size_t len = 256;
		 

		// read file
		FILE* fptr = fopen(argv[2], "r");

		int array[len];

		if (fptr == NULL)
			exit(EXIT_FAILURE);

		// parse file into new array
		int i = 0;
		int nLines = 0;

		while (read = getline(&line, &len, fptr) != -1){
			//printf("Line  %d readed\n", i);
			array[i] = atoi(line);
			//printf("line %d | len: %d | value: %d\n", i, read, array[i]);
			i++;
		}

		//printIntArray(array, i);

		// merge array
		printf("Beginning the merge...\n");
		Mergesort(array, sizeof(array), IntSize, IntCompare, IntAssign);

		//printIntArray(array, i);
		
		// write in document
		FILE *resf = fopen("mergedNum.txt", "w+");
		if (resf == NULL)
			exit(EXIT_FAILURE);

		for (int i = 0; i < sizeof(array); i++){
			fprintf(resf, "%d\n", atoi((const char *)array + i));
		}

		fclose(resf);

	}
	// Merge strings 
	else if (argv[1] != NULL && strncmp(argv[1], "-n", 2) != 0) {
		printf("Merging numbers...\n");

		location = argv[1];
		 

		// read file
		FILE* fptr = fopen(location, "r");
		int len = getFileLineLen(fptr);

		char *array[len];

		printArray(array, len);

		if (fptr == NULL)
			exit(EXIT_FAILURE);

		// parse file into new array
		int i = 0;
		while (fgets(line, sizeof(line), fptr) != NULL){
			array[i] = line;
			i++;
		}

		// merge array
		Mergesort(array, sizeof(array), StrSize, StrCompare, StrAssign);
		
		// write in document
		FILE *resf = fopen("mergedStr.txt", "w+");

		for (int i = 0; i < sizeof(array); i++){
			fprintf(resf, "%s\n", ((char *)array)[i]);
		}

		fclose(resf);
	}
	// Wrong input
	else {
		printf("No valid parameters");
	}

    return 0;
}
