#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

typedef struct {
    void** vet;
    int raiz;
    size_t tamanho;
    int(*extraiPrioridade)(void*);
} heap;

heap novoHeap(size_t tamanhoInicial, int(*extraiPrioridade)(void*));
int insereHeap(heap* h, void* val);
void* removeMaximo(heap* h);
int heapifica(heap* h);

#endif
