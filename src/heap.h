#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

typedef struct {
    void** vet;
    int prox;
    size_t tamanho;
    size_t capacidade;
    int(*extraiPrioridade)(void*);
} heap;

heap* novoHeap(size_t tamanhoInicial, int(*extraiPrioridade)(void*));
int expandeHeap(heap* h);
int insereHeap(heap* h, void* val);
void* removeMaximo(heap* h);
int heapifica(heap* h, int raiz);

#endif
