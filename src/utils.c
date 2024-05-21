#include "utils.h"
#include <string.h>

void chomp(char* str) {
    str[strcspn(str,"\n")]=0;
}

int mod(int a, int b) {
    return b ? (a%b+b)%b : 0;
}
