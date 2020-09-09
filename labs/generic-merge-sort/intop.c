#include "intop.h"


// compare a cell in 1 array to a cell in another array
int IntCompare(void* array1, void* array2, int i, int j)
{
	return (((int*)array1)[i] <= ((int*)array2)[j]);
}
// assign value from one array to another
void IntAssign(void* array1, void* array2, int i, int j)
{
	((int*)array1)[i] = ((int*)array2)[j];
}

// returns sizeof(int)
int IntSize(void)
{
	return sizeof(int);
}