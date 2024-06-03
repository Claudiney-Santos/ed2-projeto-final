#ifndef _UTILS_H_
#define _UTILS_H_ 1

void upperString(char* str);
void lowerString(char* str);

void chomp(char* str);
char isNumeric(char* str);
char isAlphabetic(char* str);
char isAlphanumeric(char* str);

int mod(int a, int b);

int mergesort(void** vet, int first, int last, void*(*priorityFunc)(void*,void*));

#endif
