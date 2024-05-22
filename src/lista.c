#include "lista.h"
#include "utils.h"

no* novoNo(void* val) {
    no* n=(no*)malloc(sizeof(no));
    if(n) {
        n->val=val;
        n->prox=n->ant=NULL;
    }
    return n;
}

void* liberaNo(no** n) {
    if(!n||!(*n))
        return NULL;
    void* val=(*n)->val;
    free(*n);
    *n=NULL;
    return val;
}

lista* novaLista() {
    lista* l=(lista*)malloc(sizeof(lista));
    if(l) {
        l->raiz=l->ultimo=NULL;
        l->tamanho=0;
    }
    return l;
}

void liberaLista(lista** l) {
    liberaListaFunc(l, free);
}

void liberaListaFunc(lista** l, void(*f)(void*)) {
    if(!l||!(*l))
        return;

    if(f)
        while((*l)->tamanho)
            (*f)(removeRaiz(*l));

    free(*l);
    *l=NULL;
}

int insereRaiz(lista* l, void* val) {
    if(!l)
        return 1;
    no* n=novoNo(val);
    if(!n)
        return 2;

    if(l->tamanho)
        l->raiz->ant=n;
    else
        l->ultimo=n;

    n->prox=l->raiz;
    l->raiz=n;
    l->tamanho++;
    return 0;
}

int insereUltimo(lista* l, void* val) {
    if(!l)
        return 1;
    no* n=novoNo(val);
    if(!n)
        return 2;

    if(l->tamanho)
        l->ultimo->prox=n;
    else
        l->raiz=n;

    n->ant=l->ultimo;
    l->ultimo=n;
    l->tamanho++;
    return 0;
}

int inserePosicao(lista* l, void* val, int indice) {
    if(!l)
        return 1;
    indice=mod(indice,l->tamanho+1);
    if(!indice)
        return insereRaiz(l, val);
    else if(indice==l->tamanho)
        return insereUltimo(l, val);
    no* n=novoNo(val);
    no* m=NULL;
    if(!n)
        return 2;
    if(indice<=l->tamanho/2)
        for(m=l->raiz;indice>1;indice--)
            m=m->prox;
    else
        for(m=l->ultimo;indice<l->tamanho;indice++)
            m=m->ant;
    n->prox=m->prox;
    n->prox->ant=n;
    m->prox=n;
    m->prox->ant=m;
    l->tamanho++;
    return 0;
}

void* pegaRaiz(lista* l) {
    return l&&l->raiz ? l->raiz->val : NULL;
}

void* pegaUltimo(lista* l) {
    return l&&l->ultimo ? l->ultimo->val : NULL;
}

void* pegaPosicao(lista* l, int indice) {
    if(!l)
        return NULL;
    indice=mod(indice,l->tamanho);
    if(!indice)
        return pegaRaiz(l);
    else if(indice==l->tamanho-1)
        return pegaUltimo(l);

    no* n=NULL;
    if(indice<l->tamanho/2) {
        for(n=l->raiz;indice>0;indice--)
            n=n->prox;
    } else
        for(n=l->ultimo;indice<l->tamanho-1;indice++)
            n=n->ant;
    return n->val;
}

void* removeRaiz(lista* l) {
    if(!l||!l->raiz)
        return NULL;
    no* n=l->raiz;
    l->raiz=n->prox;
    if(l->tamanho==1)
        l->ultimo=NULL;
    else
        l->raiz->ant=NULL;
    l->tamanho--;
    return liberaNo(&n);
}

void* removeUltimo(lista* l) {
    if(!l||!l->ultimo)
        return NULL;
    no* n=l->ultimo;
    l->ultimo=n->ant;
    if(l->tamanho==1)
        l->raiz=NULL;
    else if(l->tamanho>1)
        l->ultimo->prox=NULL;
    l->tamanho--;
    return liberaNo(&n);
}

void* removePosicao(lista* l, int indice) {
    if(!l||!l->tamanho)
        return NULL;
    indice=mod(indice,l->tamanho+1);
    if(!indice)
        return removeRaiz(l);
    else if(indice==l->tamanho)
        return removeUltimo(l);

    no* n=NULL;
    no* m=NULL;
    if(indice<=l->tamanho/2)
        for(m=l->raiz;indice>1;indice--)
            m=m->prox;
    else
        for(m=l->ultimo;indice<l->tamanho;indice++)
            m=m->ant;
    n=m->prox;
    m->prox=n->prox;
    m->prox->ant=m;
    l->tamanho--;
    return liberaNo(&n);
}

void paraCada(lista* l, void(*f)(void*, int, lista*)) {
    if(!l||!f)
        return;
    no* n=NULL;
    int i=0;
    for(n=l->raiz;n;n=n->prox) {
        (*f)(n->val, i, l);
        i++;
    }
}

void mapeiaLista(lista* l, void* (*f)(void*)) {
    if(!l||!f)
        return;
    no* n=NULL;
    for(n=l->raiz;n;n=n->prox)
        n->val=(*f)(n->val);
}
