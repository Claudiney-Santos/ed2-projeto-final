#include "heap.h"

heap* novoHeap(size_t capacidadeInicial, int(*extraiPrioridade)(void*), int(*funcPrioridade)(int,int)) {
    if(!extraiPrioridade||funcPrioridade)
        return NULL;
    heap* h=(heap*)malloc(sizeof(heap));
    if(h) {
        h->capacidade=capacidadeInicial;
        h->extraiPrioridade=extraiPrioridade;
        h->funcPrioridade=funcPrioridade;
        h->prox=h->tamanho=0;
        h->vet=(void**)malloc(capacidadeInicial*sizeof(void*));
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
        h->vet[h->prox++]=val;
        h->tamanho++;
        heapifica(h,0);
    }
    return err;
}
void* removePrioritario(heap* h) {
    if(!h||!h->tamanho)
        return NULL;
    void* val=h->vet[0];
    h->vet[0]=h->vet[--h->prox];
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
    if(pai>=h->prox||esq>=h->prox)
        return 0;
    priorPai=h->extraiPrioridade(h->vet[pai]), priorEsq=h->extraiPrioridade(h->vet[esq]), priorDir=dir<h->prox ? h->extraiPrioridade(h->vet[dir]) : -1;
    if(dir<h->prox&&h->funcPrioridade(priorEsq, priorDir)==priorDir) {
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
