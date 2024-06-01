#include "hash.h"
#include <stdlib.h>

#include "utils.h"

#define PHI 161803398875

parHash* novoParHash(int chave, void* valor) {
    parHash* ph=(parHash*)malloc(sizeof(parHash));
    if(ph) {
        ph->chave=chave;
        ph->valor=valor;
    }
    return ph;
}

void* liberaParHash(parHash** ph) {
    if(!ph||!(*ph))
        return NULL;
    void* valor=(*ph)->valor;
    free(*ph);
    *ph=NULL;
    return valor;
}

hash* novoHash(size_t capacidade, int (*funcHash)(int), int (*funcColisao)(hash*, int, int)) {
    if(!funcHash||!funcColisao)
        return NULL;
    hash* h=(hash*)malloc(sizeof(hash));
    if(h) {
        h->tamanho=0;
        h->capacidade=capacidade;
        h->funcHash=funcHash;
        h->funcColisao=funcColisao;
        h->pares=(parHash**)calloc(capacidade,sizeof(parHash*));
        if(!h->pares) {
            free(h);
            h=NULL;
        }
    }
    return h;
}

void liberaHash(hash** h) {
    liberaHashFunc(h, free);
}

void liberaHashFunc(hash** h, void(*f)(void*)) {
    if(!h||!(*h))
        return;
    int i;
    for(i=0;(*h)->tamanho&&i<(*h)->capacidade;i++)
        if((*h)->pares[i]) {
            (*f)(liberaParHash(&(*h)->pares[i]));
            (*h)->tamanho--;
        }
    free((*h)->pares);
    free(*h);
    *h=NULL;
}

int expandeHash(hash* h) {
    if(!h)
        return -1;
    parHash** temp=NULL;
    size_t novaCapacidade=2*h->capacidade;
    temp=realloc(h->pares, novaCapacidade);
    if(temp) {
        h->pares=temp;
        h->capacidade=novaCapacidade;
    }
    return temp ? 0 : 1;
}

float fatorCarregamento(hash* h) {
    return h ? h->capacidade ? (float)h->tamanho/h->capacidade : 1 : -1;
}

void* defineHash(hash* h, int chave, void* valor) {
    if(!h)
        return NULL;
    int err=0;
    while(!err&&fatorCarregamento(h)>0.6)
        err=expandeHash(h);
    if(err)
        return NULL;
    int key=h->funcColisao(h, chave, 0);
    void* val=NULL;
    if(key<0||key>=h->capacidade)
        return NULL;
    else if(h->pares[key]) {
        val=h->pares[key]->valor;
        h->pares[key]->valor=valor;
        h->pares[key]->chave=chave;
    } else {
        h->pares[key]=novoParHash(chave, valor);
        h->tamanho++;
    }
    return val;
}

void* pegaHash(hash* h, int chave) {
    if(!h||!h->capacidade)
        return NULL;
    int key=h->funcColisao(h, chave, 0);
    if(key<0||key>=h->capacidade||!h->pares[key]||h->pares[key]->chave!=chave)
        return NULL;
    return h->pares[key]->valor;
}

void* removeHash(hash* h, int chave) {
    if(!h||!h->capacidade)
        return NULL;
    int key=h->funcColisao(h, chave, 0), i, knext, chaveHash;
    parHash** p=NULL;
    parHash temp;
    if(key<0||key>=h->capacidade||!h->pares[key]||h->pares[key]->chave!=chave)
        return NULL;
    chaveHash=h->funcHash(chave);
    i=0;
    do
        knext=h->funcColisao(h, chave, ++i);
    while(knext>=0&&knext<h->capacidade&&h->pares[knext]&&knext!=key&&h->funcHash(h->pares[knext]->chave)==chaveHash);

    p=&h->pares[h->funcColisao(h, chave, --i)];
    temp.chave=h->pares[key]->chave;
    temp.valor=h->pares[key]->valor;
    h->pares[key]->chave=(*p)->chave;
    h->pares[key]->valor=(*p)->valor;
    (*p)->chave=temp.chave;
    (*p)->valor=temp.valor;

    h->tamanho--;
    return liberaParHash(p);
}

int funcHash1(int chave) {
    return PHI*chave;
}

int funcHash2(int chave) {
    return chave;
}

int colisaoLinear(hash* h, int chave, int offset) {
    if(!h||!h->capacidade)
        return -1;
    int i=0, k, key=mod(h->funcHash(chave), h->capacidade), chaveHash=h->funcHash(chave), chaveHashAtual, step;
    do {
        k=(key+i)%h->capacidade;
        i++;
    } while(h->pares[k]&&h->pares[k]->chave!=chave&&(chaveHashAtual=h->funcHash(h->pares[k]->chave))==chaveHash);
    for(step=offset<0 ? -1 : 1;offset!=0&&(!h->pares[k]||h->funcHash(h->pares[k]->chave)==chaveHashAtual);i+=step,offset-=step)
        k=(key+step*i)%h->capacidade;
    return k;
}

int colisaoQuadratica(hash* h, int chave, int offset) {
    if(!h||!h->capacidade)
        return -1;
    int i=0, k, key=mod(h->funcHash(chave), h->capacidade), chaveHash=h->funcHash(chave), chaveHashAtual, step;
    do {
        k=(key+i*i)%h->capacidade;
        i++;
    } while(h->pares[k]&&h->pares[k]->chave!=chave&&(chaveHashAtual=h->funcHash(h->pares[k]->chave))==chaveHash);
    for(step=offset<0 ? -1 : 1;offset!=0&&(!h->pares[k]||h->funcHash(h->pares[k]->chave)==chaveHashAtual);i+=step,offset-=step)
        k=(key+step*i*i)%h->capacidade;
    return k;
}

