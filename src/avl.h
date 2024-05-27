#ifndef _AVL_H_
#define _AVL_H_

typedef struct NoAvl {
    void* val;
    int altura;
    struct NoAvl esq;
    struct NoAvl dir;
} noAvl;

typedef struct {
    noAvl* raiz;
    int (*extraiChave)(void*);
} avl;

noAvl* novoNoAvl(void* val);
void* liberaNoAvl(noAvl** n);

int insereAvlNo(noAvl** n, void* val, int(*extraiChave)(void*));

avl* novaAvl(int (*extraiChave)(void*));
void liberaAvl(avl** a);

int insereAvl(avl* a, void* val);
void* pegaAvl(avl* a, int chave);
void* removeAvl(avl* a, int chave);
void listaAvl(avl* a);


#endif
