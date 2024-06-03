#include "hash.h"

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define PHI 161803
//398875

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
    int err=0;
    hash* h=(hash*)malloc(sizeof(hash));
    if(h) {
        h->tamanho=0;
        h->capacidade=capacidade;
        h->funcHash=funcHash;
        h->funcColisao=funcColisao;
        h->chaves=(int*)malloc(capacidade*sizeof(int));
        h->pares=(parHash**)calloc(capacidade,sizeof(parHash*));
        h->registro=novoLog("Hash");
        if(!h->chaves||!h->pares||!h->registro)
            err=1;
        if(err) {
            if(h->chaves)
                free(h->chaves);
            if(h->pares)
                free(h->pares);
            if(h->registro)
                liberaLog(&h->registro);
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
    void* val=NULL;
    while((*h)->tamanho) {
        val=removeHash(*h, (*h)->chaves[0]);
        if(f)
            (*f)(val);
    }

    if((*h)->registro)
        liberaLog(&(*h)->registro);
    if((*h)->pares)
        free((*h)->pares);
    if((*h)->chaves)
        free((*h)->chaves);
    free(*h);
    *h=NULL;
}

int expandeHash(hash* h) {
    if(!h)
        return -1;
    int err=0;
    size_t novaCapacidade=2*h->capacidade;
    int* tempInt=NULL;
    parHash** tempPar=NULL;
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return 2;

    tempPar=realloc(h->pares, novaCapacidade);
    tempInt=realloc(h->chaves, novaCapacidade);
    if(!tempPar||!tempInt)
        err=1;
    if(err) {
        sprintf(msg, "Foi tentado aumentar a capacidade do hash de %lu para %lu, porem houve uma falha", h->capacidade, novaCapacidade);
        adicionaLog(h->registro, msg);
        if(tempPar)
            h->pares=realloc(tempPar, h->capacidade);
        if(tempInt)
            h->chaves=realloc(tempInt, h->capacidade);
    } else {
        sprintf(msg, "A capacidade do hash foi aumentada de %lu para %lu", h->capacidade, novaCapacidade);
        adicionaLog(h->registro, msg);
        h->pares=tempPar;
        h->chaves=tempInt;
        h->capacidade=novaCapacidade;
    }
    free(msg);
    return err;
}

float fatorCarregamento(hash* h) {
    return h ? h->capacidade ? (float)h->tamanho/h->capacidade : 1 : -1;
}

void* defineHash(hash* h, int chave, void* valor) {
    if(!h)
        return NULL;
    int err=0, key, i, keyOriginal;
    void* val=NULL;
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return NULL;

    while(!err&&fatorCarregamento(h)>0.6)
        err=expandeHash(h);
    if(err) {
        free(msg);
        return NULL;
    }
    keyOriginal=mod(h->funcHash(chave), h->capacidade);

    key=h->funcColisao(h, chave, 0);
    if(key!=keyOriginal) {
            sprintf(msg, "Houve uma colisao ao tentar definir a chave %d para a posicao %d, sua nova possivel posicao eh %d", chave, keyOriginal, key);
            adicionaLog(h->registro, msg);
    }
    if(key<0||key>=h->capacidade) {
        sprintf(msg, "Ao tentar definir a chave %d, os algoritmos de hash e/ou colisao resultaram na chave invalida %d", chave, key);
        adicionaLog(h->registro, msg);
        free(msg);
        return NULL;
    } else if(h->pares[key]) {
        val=h->pares[key]->valor;
        h->pares[key]->valor=valor;
        if(h->pares[key]->chave!=chave) {
            sprintf(msg, "A chave %d foi definida na posicao %d, sobreescrevendo a chave %d, que havia sido anteriormente definida", chave, key, h->pares[key]->chave);
            adicionaLog(h->registro, msg);
            for(i=0;i<h->tamanho;i++)
                if(h->chaves[i]==h->pares[key]->chave) {
                    h->chaves[i]=chave;
                    break;
                }
            h->pares[key]->chave=chave;
        } else {
            sprintf(msg, "A chave %d foi definida na posicao %d, sobreescrevendo o valor que estava anteriormente definido", chave, key);
            adicionaLog(h->registro, msg);
        }
    } else {
        sprintf(msg, "A chave %d foi definida na posicao %d", chave, key);
        adicionaLog(h->registro, msg);
        h->pares[key]=novoParHash(chave, valor);
        h->chaves[h->tamanho++]=chave;
    }
    free(msg);
    return val;
}

void* pegaHash(hash* h, int chave) {
    if(!h||!h->capacidade)
        return NULL;
    int key=h->funcColisao(h, chave, 0);
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return NULL;
    if(key<0||key>=h->capacidade||!h->pares[key]||h->pares[key]->chave!=chave) {
        sprintf(msg, "Houve uma falha ao tentar acessar a chave %d do hash", chave);
        adicionaLog(h->registro, msg);
        free(msg);
        return NULL;
    }
    sprintf(msg, "A chave %d foi acessada com sucesso", chave);
    adicionaLog(h->registro, msg);
    free(msg);
    return h->pares[key]->valor;
}

void* removeHash(hash* h, int chave) {
    if(!h||!h->capacidade)
        return NULL;
    int key=h->funcColisao(h, chave, 0), i=0, knext=-1, chaveHash=-1, chaveHashAtual=-1;
    parHash temp;
    parHash** p=NULL;
    char* msg=(char*)malloc(256*sizeof(char));
    if(!msg)
        return NULL;
    if(key<0||key>=h->capacidade||!h->pares[key]||h->pares[key]->chave!=chave) {
        sprintf(msg, "Houve uma falha ao tentar remover a chave %d do hash", chave);
        adicionaLog(h->registro, msg);
        free(msg);
        return NULL;
    }
    chaveHash=mod(h->funcHash(chave), h->capacidade);
    i=0;
    do {
        knext=h->funcColisao(h, chave, ++i);
        chaveHashAtual=h->pares[knext] ? mod(h->funcHash(h->pares[knext]->chave), h->capacidade) : -1;
    } while(knext>=0&&knext<h->capacidade&&h->pares[knext]&&knext!=key&&chaveHashAtual==chaveHash);

    p=&h->pares[h->funcColisao(h, chave, --i)];
    temp.chave=h->pares[key]->chave;
    temp.valor=h->pares[key]->valor;
    h->pares[key]->chave=(*p)->chave;
    h->pares[key]->valor=(*p)->valor;
    (*p)->chave=temp.chave;
    (*p)->valor=temp.valor;

    if(i)
        sprintf(msg, "A chave %d foi removida com sucesso da posicao %d, e a chave %d foi realocada para essa posicao", chave, key, h->pares[key]->chave);
    else 
        sprintf(msg, "A chave %d foi removida com sucesso da posicao %d", chave, key);
    adicionaLog(h->registro, msg);
    free(msg);

    for(i=0;i<h->tamanho;i++)
        if(h->chaves[i]==chave) {
            h->chaves[i]=h->chaves[h->tamanho-1];
            break;
        }
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
    if(!h||h->capacidade==0)
        return -1;
    int i=0, k=-1, key=mod(h->funcHash(chave), h->capacidade), chaveHash=mod(h->funcHash(chave), h->capacidade), chaveHashAtual=-1, step=offset<0 ? -1 : 1;
    do {
        k=(key+i)%h->capacidade;
        i++;
        chaveHashAtual=h->pares[k] ? mod(h->funcHash(h->pares[k]->chave), h->capacidade) : -1;
    } while(h->pares[k]&&h->pares[k]->chave!=chave&&chaveHashAtual==chaveHash);
    for(;offset!=0&&(!h->pares[k]||chaveHashAtual==chaveHash);i+=step,offset-=step) {
        chaveHashAtual=h->pares[k] ? mod(h->funcHash(h->pares[k]->chave), h->capacidade) : -1;
        k=(key+step*i)%h->capacidade;
    }
    return k;
}

int colisaoQuadratica(hash* h, int chave, int offset) {
    if(!h||!h->capacidade)
        return -1;
    int i=0, k, key=mod(h->funcHash(chave), h->capacidade), chaveHash=mod(h->funcHash(chave), h->capacidade), chaveHashAtual=-1, step=offset<0 ? -1 : 1;
    do {
        k=(key+i*i)%h->capacidade;
        i++;
        chaveHashAtual=h->pares[k] ? mod(h->funcHash(h->pares[k]->chave), h->capacidade) : -1;
    } while(h->pares[k]&&h->pares[k]->chave!=chave&&chaveHashAtual==chaveHash);
    for(;offset!=0&&(!h->pares[k]||chaveHashAtual==chaveHash);i+=step,offset-=step) {
        chaveHashAtual=h->pares[k] ? mod(h->funcHash(h->pares[k]->chave), h->capacidade) : -1;
        k=(key+step*i*i)%h->capacidade;
    }
    return k;
}

