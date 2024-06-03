#include "heap.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>

heap* novoHeap(size_t capacidadeInicial, void*(*funcPrioridade)(void*,void*)) {
    if(!funcPrioridade)
        return NULL;
    int err=0;
    heap* h=(heap*)malloc(sizeof(heap));
    if(h) {
        h->capacidade=capacidadeInicial;
        h->funcPrioridade=funcPrioridade;
        h->tamanho=0;
        h->vet=(void**)malloc(capacidadeInicial*sizeof(void*));
        h->registro=novoLog("Heap");
        if(!h->vet||!h->registro)
            err=1;
        if(err) {
            if(h->vet)
                free(h->vet);
            if(h->registro)
                liberaLog(&h->registro);
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
        if(f)
            (*f)((*h)->vet[i]);
    if((*h)->registro)
        liberaLog(&(*h)->registro);
    free((*h)->vet);
    free(*h);
    *h=NULL;
    return 0;
}

int expandeHeap(heap* h) {
    if(!h)
        return -1;
    size_t novaCapacidade=2*h->capacidade;
    char* msg=(char*)malloc(256*sizeof(char));
    void** temp=NULL;
    if(!msg)
        return 1;
    temp=realloc(h->vet, novaCapacidade);
    if(temp) {
        sprintf(msg, "A capacidade do heap foi expandida de %lu para %lu", h->capacidade, novaCapacidade);
        h->vet=temp;
        h->capacidade=novaCapacidade;
    } else
        sprintf(msg, "Nao foi possivel expandir a capacidade do heap de %lu para %lu", h->capacidade, novaCapacidade);
    adicionaLog(h->registro, msg);
    free(msg);
    return temp ? 0 : 1;
}

int insereHeap(heap* h, void* val) {
    if(!h)
        return -1;
    int err=0;
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return 1;
    while(!err&&h->tamanho>=h->capacidade)
        err=expandeHeap(h);
    if(err)
        sprintf(msg, "Nao foi possivel inserir um elemento ao heap");
    else {
        sprintf(msg, "Foi inserido um novo elemento ao heap");
        h->vet[h->tamanho++]=val;
        heapifica(h,0);
    }
    adicionaLog(h->registro, msg);
    free(msg);
    return err;
}

void* buscaHeap(heap* h, int(*ehProcurado)(void*, void*), void* procura) {
    if(!h||!ehProcurado)
        return NULL;
    int i;
    void* val=NULL;
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return NULL;
    else if(!h->tamanho) {
        sprintf(msg, "Nao foi possivel encontrar o elemento desejado do heap porque o heap esta vazio");
        adicionaLog(h->registro, msg);
        free(msg);
        return NULL;
    }
    for(i=0;i<h->tamanho;i++)
        if(ehProcurado(procura, h->vet[i]))
            val=h->vet[i];
    if(val)
        sprintf(msg, "Foi encontrado o elemento procurado no heap");
    else
        sprintf(msg, "Nao foi possivel encontrar o elemento procurado no heap");
    adicionaLog(h->registro, msg);
    free(msg);
    return val;
}

void* removeHeap(heap* h, int(*ehProcurado)(void*, void*), void* procura) {
    if(!h||!ehProcurado)
        return NULL;
    int i;
    void* val=NULL;
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return NULL;
    else if(!h->tamanho) {
        sprintf(msg, "Nao foi possivel remover o elemento desejado do heap, porque o heap esta vazio");
        adicionaLog(h->registro, msg);
        free(msg);
        return NULL;
    }
    for(i=0;i<h->tamanho;i++)
        if(ehProcurado(procura, h->vet[i])) {
            val=h->vet[i];
            h->vet[i]=h->vet[--h->tamanho];
            heapifica(h,0);
        }
    if(val)
        sprintf(msg, "Foi removido o elemento desejado no heap");
    else
        sprintf(msg, "Nao foi possivel remover o elemento desejado do heap");
    adicionaLog(h->registro, msg);
    free(msg);
    return val;
}

void* removePrioritario(heap* h) {
    if(!h)
        return NULL;
    char* msg=(char*)malloc(256*sizeof(char));
    void* val=NULL;
    if(!msg)
        return NULL;
    else if(h->tamanho) {
        sprintf(msg, "Foi removido com sucesso o elemento prioritario");
        val=h->vet[0];
        h->vet[0]=h->vet[--h->tamanho];
        heapifica(h,0);
    } else
        sprintf(msg, "Nao foi possivel remover o elemento prioritario porque o heap esta vazio");
    adicionaLog(h->registro, msg);
    free(msg);
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

    if(!raiz) {
        temp=(void*)malloc(256*sizeof(char));
        sprintf((char*)temp, "O heap foi heapificado");
        adicionaLog(h->registro, (char*)temp);
        free(temp);
    }

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

