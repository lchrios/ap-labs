#ifndef _STROP_H_
#define _STROP_H_

// compare a cell in 1 array to a cell in another array
extern int StrCompare(void* array1, void* array2, int i, int j);

// assign value from one array to another
extern void StrAssign(void* array1, void* array2, int i, int j);

// returns sizeof(int)
extern int StrSize(void);

#endif