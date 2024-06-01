#include "heap.h"
#include "utils.h"

#include <string.h>

heap* novoHeap(size_t capacidadeInicial, int(*extraiPrioridade)(void*), int(*funcPrioridade)(int,int)) {
    if(!extraiPrioridade||funcPrioridade)
        return NULL;
    heap* h=(heap*)malloc(sizeof(heap));
    if(h) {
        h->capacidade=capacidadeInicial;
        h->extraiPrioridade=extraiPrioridade;
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

void* buscaHeap(heap* h, int(*ehProcurado)(void*)) {
    if(!h||!ehProcurado)
        return NULL;
    int i;
    for(i=0;i<h->tamanho;i++)
        if(ehProcurado(h->vet[i]))
            return h->vet[i];
    return NULL;
}

void* removePrioritario(heap* h) {
    if(!h||!h->tamanho)
        return NULL;
    void* val=h->vet[0];
    h->vet[0]=h->vet[--h->tamanho];
    heapifica(h,0);
    h->tamanho--;
    return val;
}

int heapifica(heap* h, int raiz) {
    if(!h)
        return -1;
    int pai=raiz, esq=2*raiz+1, dir=2*raiz+2, priorFilho;
    int priorPai, priorEsq, priorDir, priorFilhoPrior;
    void* temp=NULL;
    if(pai>=h->tamanho||esq>=h->tamanho)
        return 0;
    priorPai=h->extraiPrioridade(h->vet[pai]), priorEsq=h->extraiPrioridade(h->vet[esq]), priorDir=dir<h->tamanho ? h->extraiPrioridade(h->vet[dir]) : -1;
    if(dir<h->tamanho&&h->funcPrioridade(priorEsq, priorDir)==priorDir) {
        priorFilho=dir;
        priorFilhoPrior=priorDir;
    } else {
        priorFilho=esq;
        priorFilhoPrior=priorEsq;
    }
    if(h->funcPrioridade(priorPai, priorFilhoPrior)!=priorPai) {
        temp=h->vet[pai];
        h->vet[pai]=h->vet[priorFilho];
        h->vet[priorFilho]=temp;
    }
    heapifica(h, esq);
    heapifica(h, dir);

    return 0;
}

//void** heapOrganizado(heap* h) {
//    if(!h)
//        return NULL;
//    int i;
//    void** vet=(void**)malloc(h->tamanho*sizeof(void*));
//    if(!vet)
//        return NULL;
//    memcpy(vet, h->vet, h->tamanho*sizeof(void*));
//    mergeSort(vet, h->funcPrioridade);
//    return vet;
//}

