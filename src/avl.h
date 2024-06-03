#ifndef _AVL_H_
#define _AVL_H_ 1

#include "log.h"

#include <stdlib.h>

typedef struct NoAvl {
    size_t altura;
    int fb;
    struct NoAvl* esq;
    struct NoAvl* dir;
    void* val;
} noAvl;

typedef struct {
    noAvl* raiz;
    int (*extraiChave)(void*);
    log* registro;
} avl;

noAvl* novoNoAvl(void* val);
void* liberaNoAvl(noAvl** n);
void liberaNosAvlFunc(noAvl** n, void(*f)(void*));
int insereNoAvl(noAvl** n, void* val, int(*extraiChave)(void*));
void* pegaNoAvl(noAvl* n, int chave, int(*extraiChave)(void*));
void* removeNoAvl(noAvl** n, int chave, int(*extraiChave)(void*));

int balanceiaNos(noAvl** n, noAvl** m);
void rotacaoEsquerda(noAvl** n);
void rotacaoDireita(noAvl** n);

int pegaAlturaNo(noAvl* n);
int calculaAlturaNo(noAvl* n);

int pegaFbNo(noAvl* n);
int calculaFbNo(noAvl* n);

avl* novaAvl(int(*extraiChave)(void*));
void liberaAvl(avl** a);
void liberaAvlFunc(avl** a, void(*f)(void*));

int insereAvl(avl* a, void* val);
void* pegaAvl(avl* a, int chave);
void* removeAvl(avl* a, int chave);

#endif
