#ifndef _HASH_H_
#define _HASH_H_

#include <stdlib.h>

typedef struct {
    int chave;
    void* valor;
} parHash;

typedef struct Hash {
    size_t capacidade;
    size_t tamanho;
    int (*funcHash)(int);
    int (*funcColisao)(struct Hash*, int, int);
    int* chaves;
    parHash** pares;
} hash;

parHash* novoParHash(int chave, void* valor);
void* liberaParHash(parHash** ph);

hash* novoHash(size_t capacidade, int (*funcHash)(int), int (*funcColisao)(hash*, int, int));
void liberaHash(hash** h);
void liberaHashFunc(hash** h, void(*f)(void*));

int expandeHash(hash* h);

float fatorCarregamento(hash* h);

void* defineHash(hash* h, int chave, void* valor);
void* pegaHash(hash* h, int chave);
void* removeHash(hash* h, int chave);

// funções de hash
int funcHash1(int chave);
int funcHash2(int chave);

// funções de colisão
int colisaoLinear(hash* h, int chave, int offset);
int colisaoQuadratica(hash* h, int chave, int offset);

#endif
