#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

typedef struct {
    void** vet;
    size_t tamanho;
    size_t capacidade;
    void*(*funcPrioridade)(void*,void*);
} heap;

heap* novoHeap(size_t capacidadeInicial, void*(*funcPrioridade)(void*,void*));
int liberaHeap(heap** h);
int liberaHeapFunc(heap** h, void(*f)(void*));
int expandeHeap(heap* h);
int insereHeap(heap* h, void* val);
void* buscaHeap(heap* h, int(*ehProcurado)(void*, void*), void* procura);
void* removeHeap(heap* h, int(*ehProcurado)(void*, void*), void* procura);
void* removePrioritario(heap* h);
int heapifica(heap* h, int raiz);
void** heapOrganizado(heap* h);

#endif
