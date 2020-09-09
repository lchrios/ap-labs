#ifndef _INTOP_H_
#define _INTOP_H_

// compare a cell in 1 array to a cell in another array
extern int IntCompare(void* array1, void* array2, int i, int j);

// assign value from one array to another
extern void IntAssign(void* array1, void* array2, int i, int j);

// returns sizeof(int)
extern int IntSize(void);

#endif