#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

typedef struct {
    void** vet;
    int prox;
    size_t tamanho;
    size_t capacidade;
    int(*extraiPrioridade)(void*);
    int(*funcPrioridade)(int,int);
} heap;

heap* novoHeap(size_t capacidadeInicial, int(*extraiPrioridade)(void*), int(*funcPrioridade)(int,int));
int expandeHeap(heap* h);
int insereHeap(heap* h, void* val);
void* removePrioritario(heap* h);
int heapifica(heap* h, int raiz);

#endif
