#ifndef _LISTA_H_
#define _LISTA_H_ 1

#include <stdlib.h>

typedef struct No {
    void* val;
    struct No* prox;
    struct No* ant;
} no;

typedef struct {
    no* raiz;
    no* ultimo;
    size_t tamanho;
} lista;

no* novoNo(void* val);
void* liberaNo(no** n);

lista* novaLista();
void liberaLista(lista** l);
void liberaListaFunc(lista** l, void(*f)(void*));

int insereRaiz(lista* l, void* val);
int insereUltimo(lista* l, void* val);
int inserePosicao(lista* l, void* val, int indice);
void* pegaRaiz(lista* l);
void* pegaUltimo(lista* l);
void* pegaPosicao(lista* l, int indice);
void* removeRaiz(lista* l);
void* removeUltimo(lista* l);
void* removePosicao(lista* l, int indice);

void paraCada(lista* l, void(*f)(void*, int, lista*));
void mapeiaLista(lista* l, void* (*f)(void*));

#endif
