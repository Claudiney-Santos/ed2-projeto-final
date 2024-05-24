#include "heap.h"

heap novoHeap(size_t tamanhoInicial, int(*extraiPrioridade)(void*)) {
    if(!extraiPrioridade)
        return NULL;
    heap* h=(heap*)malloc(sizeof(heap));
    if(h) {
        h->raiz=-1;
        h->tamanho=tamanhoInicial;
        h->extraiPrioridade=extraiPrioridade;
        h->vet=(void**)malloc(sizeof(void*));
    }
    return h;
}

int insereHeap(heap* h, void* val) {
    
}
void* removeMaximo(heap* h);
int heapifica(heap* h);
