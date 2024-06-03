#include "avl.h"
#include <stdio.h>

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
    if(f)
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

void* removeMenorFilho(noAvl** n) {
    if(!n||!*n)
        return NULL;
    noAvl* m=NULL;
    if((*n)->esq)
        m=removeMenorFilho(&(*n)->esq);
    else {
        m=*n;
        *n=m->dir;
        m->dir=NULL;
    }
    return m;
}

void* removeMaiorFilho(noAvl** n) {
    if(!n||!*n)
        return NULL;
    noAvl* m=NULL;
    if((*n)->dir)
        m=removeMenorFilho(&(*n)->dir);
    else {
        m=*n;
        *n=m->esq;
        m->esq=NULL;
    }
    return m;
}

void* pegaNoAvl(noAvl* n, int chave, int(*extraiChave)(void*)) {
    if(!n||extraiChave)
        return NULL;
    int chaveAtual=extraiChave(n->val);
    noAvl* m=NULL;
    if(chaveAtual==chave)
        return n->val;
    m=chave<chaveAtual ? n->esq : n->dir;
    return pegaNoAvl(m, chave, extraiChave);
}

void* removeNoAvl(noAvl** n, int chave, int(*extraiChave)(void*)) {
    if(!n||!(*n)||!extraiChave)
        return NULL;

    int chaveAtual=extraiChave((*n)->val), alturaEsq=-1, alturaDir=-1;
    noAvl** m=NULL;
    noAvl* o=NULL;
    void* val=NULL;
    if(chaveAtual==chave) {
        o=(*n)->esq ? removeMaiorFilho(&(*n)->esq) : removeMenorFilho(&(*n)->dir);
        if(o) {
            o->dir=(*n)->dir;
            o->esq=(*n)->esq;
        }
        val=liberaNoAvl(n);
        *n=o;
        if(o) {
            alturaEsq=calculaAlturaNo(&(*n)->esq);
            alturaDir=calculaAlturaNo(&(*n)->dir);
            (*n)->altura=1+(alturaEsq>alturaDir ? alturaEsq : alturaDir);
        }
        return val;
    }
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

int calculaAlturaNo(noAvl** n) {
    if(!n||!(*n))
        return -1;
    int alturaEsq=calculaAlturaNo(&(*n)->esq), alturaDir=calculaAlturaNo(&(*n)->dir);
    (*n)->altura=1+(alturaEsq>alturaDir ? alturaEsq : alturaDir);
    if(abs(calculaFbNo(*n))>1)
        balanceiaNos(n, (*n)->fb>0 ? &(*n)->dir : &(*n)->esq);
    return (*n)->altura;
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
    int err=0;
    avl* a=(avl*)malloc(sizeof(avl));
    if(a) {
        a->raiz=NULL;
        a->extraiChave=extraiChave;
        a->registro=novoLog("AVL");
        if(!a->registro)
            err=1;
        if(err) {
            if(a->registro)
                liberaLog(&a->registro);
            free(a);
            a=NULL;
        }
    }
    return a;
}

void liberaAvl(avl** a) {
    liberaAvlFunc(a, free);
}

void liberaAvlFunc(avl** a, void(*f)(void*)) {
    if(!a||!(*a))
        return;

    liberaNosAvlFunc(&(*a)->raiz, f);

    if((*a)->registro)
        liberaLog(&(*a)->registro);

    free(*a);
    *a=NULL;
}

int insereAvl(avl* a, void* val) {
    if(!a)
        return -1;
    int err=insereNoAvl(&a->raiz, val, a->extraiChave);
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return 1;
    if(err)
        sprintf(msg, "Nao foi possivel adicionar o elemento com chave %d a AVL", a->extraiChave(val));
    else
        sprintf(msg, "O elemento com chave %d foi adicionado com sucesso a AVL", a->extraiChave(val));
    adicionaLog(a->registro, msg);
    free(msg);
    return err;
}

void* pegaAvl(avl* a, int chave) {
    if(!a)
        return NULL;
    void* val=pegaNoAvl(a->raiz, chave, a->extraiChave);
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return NULL;
    if(val)
        sprintf(msg, "O elemento com chave %d foi recuperado com sucesso", chave);
    else
        sprintf(msg, "Houve uma falha ao tentar recuperar o elemento com chave %d", chave);
    adicionaLog(a->registro, msg);
    free(msg);
    return val;
}

void* removeAvl(avl* a, int chave) {
    if(!a)
        return NULL;
    void* val=removeNoAvl(&a->raiz, chave, a->extraiChave);
    char* msg=(char*)malloc(256*sizeof(char));
    if(msg) {
        if(val)
            sprintf(msg, "A chave %d foi removida com sucesso da AVL", chave);
        else
            sprintf(msg, "Houve uma falha ao tentar remover a chave %d da AVL", chave);
        adicionaLog(a->registro, msg);
        free(msg);
    }
    return val;
}

