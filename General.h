#ifndef __GENERAL__
#define __GENERAL__

#include <stdio.h>

#define MAX_STR_LEN 255

// new types
typedef void* DATA;					// a type for data (easy to change)
typedef enum { False, True } BOOL;	// a boolean type

char*	getStrExactName(const char* msg);
char*	myGets(char* buffer, int size);
void getStringFromBinaryFile(char* buffer, FILE* pFile);
char*	getDynStr(char* str);
char**	splitCharsToWords(char* str,int* pCount,int* pTotalLength);
char*  myGetsFile(char* buffer, int size, FILE* pFile);
void    generalArrayFunction(void* array, int numElements, int elementSize, void(*f)(void* element));

#endif
