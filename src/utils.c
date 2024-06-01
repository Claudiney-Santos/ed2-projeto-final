#include "utils.h"
#include <string.h>
#include <stdlib.h>

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

int mergesort(void** vet, int first, int last, void*(*priorityFunc)(void*,void*)) {
    if(!vet)
        return -1;
    else if(first>=last)
        return 0;
    int length=last-first+1, err=0, i, I, j, J, k;
    void** vet2=(void**)malloc(length*sizeof(void*));
    if(!vet2)
        return 2;
    i=first;
    I=first+length/2-1;
    j=I+1;
    J=last;
    err=mergesort(vet, i, I, priorityFunc);
    if(!err)
        err=mergesort(vet, j, J, priorityFunc);
    if(err)
        return err;
    for(k=0;k<length;k++) {
        if(i<=I&&j<=J)
            vet2[k]=priorityFunc(vet[i], vet[j])==vet[i] ? vet[i++] : vet[j++];
        else if(i<=I)
            vet2[k]=vet[i++];
        else
            vet2[k]=vet[j++];
    }
    for(k=0,i=first;k<length;k++,i++)
        vet[i]=vet2[k];
    free(vet2);

    return 0;
}
