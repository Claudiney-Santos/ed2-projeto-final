#include "avl.h"

noAvl* novoNoAvl(void* val) {
    noAvl* n=(noAvl*)malloc(sizeof(noAvl));
    if(n) {
        n->val=val;
        n->altura=n->fb=0;
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

void liberaNosAvlFunc(noAvl** n, void(*f)(void*)) {
    if(!n||!(*n)||!f)
        return;
    liberaNosAvlFunc(&(*n)->esq, f);
    liberaNosAvlFunc(&(*n)->dir, f);
    (*f)((*n)->val);
    free(*n);
    *n=NULL;
}

int insereNoAvl(noAvl** n, void* val, int(*extraiChave)(void*)) {
    if(!n)
        return -1;
    else if(!(*n)) {
        *n=novoNoAvl(val);
        return 0;
    }
    int chave=extraiChave(val), chaveAtual=extraiChave((*n)->val), alturaEsq=-1, alturaDir=-1;
    noAvl** m=chave<chaveAtual ? &(*n)->esq : &(*n)->dir;
    insereNoAvl(m, val, extraiChave);
    alturaEsq=pegaAlturaNo((*n)->esq);
    alturaDir=pegaAlturaNo((*n)->dir);
    (*n)->altura=1+(alturaEsq>alturaDir ? alturaEsq : alturaDir);
    balanceiaNos(n, m);
    return 0;
}


void* pegaNoAvl(noAvl* n, int chave, int(*extraiChave)(void*)) {
    if(!n||extraiChave)
        return NULL;
    int chaveAtual=extraiChave(n);
    noAvl* m=NULL;
    if(chaveAtual==chave)
        return n->val;
    m=chave<chaveAtual ? n->esq : n->dir;
    return pegaNoAvl(m, chave, extraiChave);
}

void* removeNoAvl(noAvl** n, int chave, int(*extraiChave)(void*)) {
    if(!n||!(*n)||extraiChave)
        return NULL;
    int chaveAtual=extraiChave(*n), alturaEsq=-1, alturaDir=-1;
    noAvl** m=NULL;
    void* val=NULL;
    if(chaveAtual==chave)
        return liberaNoAvl(n);
    m=chave<chaveAtual ? &(*n)->esq : &(*n)->dir;
    val=removeNoAvl(m, chave, extraiChave);
    alturaEsq=pegaAlturaNo((*n)->esq);
    alturaDir=pegaAlturaNo((*n)->dir);
    (*n)->altura=1+(alturaEsq>alturaDir ? alturaEsq : alturaDir);
    balanceiaNos(n, m);
    return val;
}

int balanceiaNos(noAvl** n, noAvl** m) {
    if(!n||!(*n)||!m||!(*m))
        return -1;
    else if(abs(calculaFbNo(*n))<=1)
        return 0;
    int fx=(*n)->fb;
    int fy=(*m)->fb;
    if(fx*fy<0) {
        if(fy<0)
            rotacaoDireita(m);
        else
            rotacaoEsquerda(m);
    }
    if(fx<0)
        rotacaoDireita(n);
    else
        rotacaoEsquerda(n);

    return 0;
}

void rotacaoEsquerda(noAvl** n) {
    if(!n||!(*n)||!((*n)->dir))
        return;
    noAvl* m=*n;
    *n=m->dir;
    m->dir=(*n)->esq;
    (*n)->esq=m;
}

void rotacaoDireita(noAvl** n) {
    if(!n||!(*n)||!((*n)->esq))
        return;
    noAvl* m=*n;
    *n=m->esq;
    m->esq=(*n)->dir;
    (*n)->dir=m;
}

int pegaAlturaNo(noAvl* n) {
    return n ? n->altura : -1;
}

int calculaAlturaNo(noAvl* n) {
    if(!n)
        return -1;
    int alturaEsq=calculaAlturaNo(n->esq), alturaDir=calculaAlturaNo(n->dir);
    n->altura=1+(alturaEsq>alturaDir ? alturaEsq : alturaDir);
    return n->altura;
}

int pegaFbNo(noAvl* n) {
    return n ? n->fb : 0;
}

int calculaFbNo(noAvl* n) {
    if(!n)
        return 0;
    n->fb=pegaAlturaNo(n->dir)-pegaAlturaNo(n->esq);
    return n->fb;
}

avl* novaAvl(int(*extraiChave)(void*)) {
    if(!extraiChave)
        return NULL;
    avl* a=(avl*)malloc(sizeof(avl));
    if(a) {
        a->extraiChave=extraiChave;
        a->raiz=NULL;
    }
    return a;
}

void liberaAvl(avl** a) {
    liberaAvlFunc(a, free);
}

void liberaAvlFunc(avl** a, void(*f)(void*)) {
    if(!a||!(*a)||!f)
        return;

    liberaNosAvlFunc(&(*a)->raiz, f);

    free(*a);
    *a=NULL;
}

int insereAvl(avl* a, void* val) {
    return a ? insereNoAvl(&a->raiz, val, a->extraiChave) : -1;
}

void* pegaAvl(avl* a, int chave) {
    return a ? pegaNoAvl(a->raiz, chave, a->extraiChave) : NULL;
}

void* removeAvl(avl* a, int chave) {
    return a ? removeNoAvl(&a->raiz, chave, a->extraiChave) : NULL;
}

