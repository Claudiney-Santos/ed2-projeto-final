#include "avl.h"
#include <stdlib.h>

noAvl* novoNoAvl(void* val) {
    noAvl* n=(noAvl*)malloc(sizeof(noAvl));
    if(n) {
        n->val=val;
        n->altura=0;
        n->esq=n->dir=NULL;
    }
    return n;
}

void* liberaNoAvl(noAvl** n) {
    if(!n||!(*n))
        return NULL;
    void* val=(*n)->val;
    free(*n);
    *n=NULL;
    return val;
}

int pegaAltura(noAvl* n) {
    return n ? n->altura : -1;
}

int calculaAlturaSimples(noAvl* n) {
    if(!n)
        return -1;
    n->altura=1+max(pegaAltura(n->esq), pegaAltura(n->dir));
    return n->altura;
}

int pegaBalanceamento(noAvl* n) {
    return n ? pegaAltura(n->dir) - pegaAltura(n->esq) : 0;
}

int insereAvlNo(noAvl** n, void* val, int(*extraiChave)(void*)) {
    if(!n)
        return 0;
    else if(!(*n)) {
        *n=novoNoAvl(val);
        return 1;
    }
    int chave=extraiChave(val), chaveN=extraiChave(*n), fb;
    noAvl** m=chave<chaveN ? &((*n)->esq) : &((*n)->dir);
    if(chave==chaveN)
        return 0;
    else if(insereAvl(m, val, extraiChave)) {
        calculaAltura(*n);
        fb=pegaBalanceamento(*n);
        if(abs(fb)>1) {
            
        }
    }
}

avl* novaAvl(int (*extraiChave)(void*)) {
    avl* a=NULL;
    if(extraiChave)
        a=(avl*)malloc(sizeof(avl));
    if(a) {
        a->raiz=NULL;
        a->extraiChave=extraiChave;
    }
    return a;
}

void liberaAvl(avl** a) {
    free(*a);
    *a=NULL;
}

int insereAvl(avl* a, void* val) {
    if(!a)
        return -1;
    else if(!a->raiz) {
        a->raiz=novoNoAvl(val);
        return 0;
    }
    int chave=extraiChave(val), chaveN;
    noAvl* l=NULL;
    noAvl* m=NULL;
    noAvl** n=&(a->raiz);
    while(*n) {
        chaveN=a->extraiChave(*n);
        if(chave==chaveN)
            return 1;
        l=m;
        m=n;
        n=chave<chaveN ? &((*n)->esq) : &((*n)->dir);
    }
    *n=novoNoAvl(val);
}

void* pegaAvl(avl* a, int chave);
void* removeAvl(avl* a, int chave);
void listaAvl(avl* a);
