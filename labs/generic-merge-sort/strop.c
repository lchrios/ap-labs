#include "intop.h"
#include <string.h>


// compare a cell in 1 array to a cell in another array
int StrCompare(void* array1, void* array2, int i, int j)
{
    return strcmp(((char *)array1)[i], ((char *)array2)[j]);
}
// assign value from one array to another
void StrAssign(void* array1, void* array2, int i, int j)
{
	((char *)array1)[i] = ((char *)array2)[j];
}

// returns sizeof(int)
int StrSize(void)
{
	return sizeof(char) * 64;
}