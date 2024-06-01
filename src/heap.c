#include "heap.h"

heap* novoHeap(size_t capacidadeInicial, int(*extraiPrioridade)(void*)) {
    if(!extraiPrioridade)
        return NULL;
    heap* h=(heap*)malloc(sizeof(heap));
    if(h) {
        h->capacidade=capacidadeInicial;
        h->extraiPrioridade=extraiPrioridade;
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
void* removeMaximo(heap* h) {
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
    int pai=raiz, esq=2*raiz+1, dir=2*raiz+2, maior;
    int prior, priorEsq, priorDir, maiorPrior;
    void* temp=NULL;
    if(pai>=h->prox||esq>=h->prox)
        return 0;
    prior=h->extraiPrioridade(h->vet[raiz]), priorEsq=h->extraiPrioridade(h->vet[esq]), priorDir=dir<h->prox ? h->extraiPrioridade(h->vet[dir]) : -1;
    if(priorEsq>priorDir) {
        maior=esq;
        maiorPrior=priorEsq;
    } else {
        maior=dir;
        maiorPrior=priorDir;
    }
    if(prior<maiorPrior) {
        temp=h->vet[pai];
        h->vet[pai]=h->vet[maior];
        h->vet[maior]=temp;
    }
    heapifica(h, esq);
    heapifica(h, dir);

    return 0;
}
