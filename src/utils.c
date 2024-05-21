#include "utils.h"
#include <string.h>

void upperString(char* str) {
    if(!str)
        return;
    int i;
    for(i=0;str[i];i++)
        if(str[i]>='a'&&str[i]<='z')
            str[i]-=32;
}

void lowerString(char* str) {
    if(!str)
        return;
    int i;
    for(i=0;str[i];i++)
        if(str[i]>='A'&&str[i]<='Z')
            str[i]+=32;
}

void chomp(char* str) {
    if(!str)
        return;
    str[strcspn(str,"\n")]=0;
}

char isNumeric(char* str) {
    if(!str)
        return 0;
    int i;
    for(i=0;str[i];i++)
        if(str[i]<'0'||str[i]>'9')
            return 0;
    return 1;
}

char isAlphabetic(char* str) {
    if(!str)
        return 0;
    int i;
    for(i=0;str[i];i++)
        if(str[i]<'A'||str[i]>'z'||(str[i]>'Z'&&str[i]<'a'))
            return 0;
    return 1;
}

char isAlphanumeric(char* str) {
    if(!str)
        return 0;
    int i;
    for(i=0;str[i];i++)
        if(!(
            (str[i]>='0'&&str[i]<='9') ||
            (str[i]>='A'&&str[i]<='Z') ||
            (str[i]>='a'&&str[i]<='z')
            ))
            return 0;
    return 1;
}

int mod(int a, int b) {
    return b ? (a%b+b)%b : 0;
}
