#include "heap.h"
#include "utils.h"

#include <string.h>

heap* novoHeap(size_t capacidadeInicial, void*(*funcPrioridade)(void*,void*)) {
    if(!funcPrioridade)
        return NULL;
    heap* h=(heap*)malloc(sizeof(heap));
    if(h) {
        h->capacidade=capacidadeInicial;
        h->funcPrioridade=funcPrioridade;
        h->tamanho=0;
        h->vet=(void**)malloc(capacidadeInicial*sizeof(void*));
        if(!h->vet) {
            free(h);
            h=NULL;
        }
    }

    return h;
}

int liberaHeap(heap** h) {
    return liberaHeapFunc(h, free);
}
int liberaHeapFunc(heap** h, void(*f)(void*)) {
    if(!h||!(*h))
        return -1;
    int i;
    for(i=0;i<(*h)->tamanho;i++)
        (*f)((*h)->vet[i]);
    free((*h)->vet);
    free(*h);
    *h=NULL;
    return 0;
}

int expandeHeap(heap* h) {
    if(!h)
        return -1;
    size_t novaCapacidade=2*h->capacidade;
    void** temp=realloc(h->vet, novaCapacidade);
    if(temp) {
        h->vet=temp;
        h->capacidade=novaCapacidade;
    }
    return temp ? 0 : 1;
}

int insereHeap(heap* h, void* val) {
    if(!h)
        return -1;
    int err=0;
    while(!err&&h->tamanho>=h->capacidade)
        err=expandeHeap(h);
    if(!err) {
        h->vet[h->tamanho++]=val;
        heapifica(h,0);
    }
    return err;
}

void* buscaHeap(heap* h, int(*ehProcurado)(void*, void*), void* procura) {
    if(!h||!ehProcurado)
        return NULL;
    int i;
    for(i=0;i<h->tamanho;i++)
        if(ehProcurado(procura, h->vet[i]))
            return h->vet[i];
    return NULL;
}

void* removeHeap(heap* h, int(*ehProcurado)(void*, void*), void* procura) {
    if(!h||!ehProcurado||!h->tamanho)
        return NULL;
    int i;
    void* val=NULL;
    for(i=0;i<h->tamanho;i++)
        if(ehProcurado(procura, h->vet[i])) {
            val=h->vet[i];
            h->vet[i]=h->vet[--h->tamanho];
            heapifica(h,0);
        }
    return val;
}

void* removePrioritario(heap* h) {
    if(!h||!h->tamanho)
        return NULL;
    void* val=h->vet[0];
    h->vet[0]=h->vet[--h->tamanho];
    heapifica(h,0);
    return val;
}

int heapifica(heap* h, int raiz) {
    if(!h)
        return -1;
    int pai=raiz, esq=2*raiz+1, dir=2*raiz+2, priorFilho;
    void* temp=NULL;
    if(pai>=h->tamanho||esq>=h->tamanho)
        return 0;
    priorFilho=dir<h->tamanho&&h->funcPrioridade(h->vet[esq],h->vet[dir])==h->vet[dir] ? dir : esq;
    if(h->funcPrioridade(h->vet[pai], h->vet[priorFilho])==h->vet[priorFilho]) {
        temp=h->vet[pai];
        h->vet[pai]=h->vet[priorFilho];
        h->vet[priorFilho]=temp;
    }
    heapifica(h, esq);
    heapifica(h, dir);

    return 0;
}

void** heapOrganizado(heap* h) {
    if(!h)
        return NULL;
    void** vet=(void**)malloc(h->tamanho*sizeof(void*));
    if(!vet)
        return NULL;
    memcpy(vet, h->vet, h->tamanho*sizeof(void*));
    mergesort(vet, 0, h->tamanho-1, h->funcPrioridade);
    return vet;
}

