#include "simulacao.h"

#include <stdlib.h>
#include <stdio.h>

void* funcPrioridadeHeap(void* a, void* b);
int extraiCodigo(void* val);
int buscandoNoHeap(void* procura, void* val);
void listaProcessosNoAvl(noAvl* n);
void listaProcessosAvl(avl* a);
void listaProcessosHeap(heap* h);
void listaHash(hash* h, statusCodigo status);
void encerraProcessoGenerico(void* val);

int executarSimulacao(lista* tokens) {
    if(!tokens)
        return -1;
    size_t capacidadeInicial=101;
    int err=0, i;
    char executando=0;
    int* k=NULL;
    no* n=NULL;
    token* t=NULL;

    avl* a=novaAvl(extraiCodigo);
    hash* ha[4];
    heap* he=novoHeap(capacidadeInicial, funcPrioridadeHeap);

    processo* p=NULL;

    ha[0]=novoHash(capacidadeInicial, funcHash1, colisaoLinear);
    ha[1]=novoHash(capacidadeInicial, funcHash1, colisaoQuadratica);
    ha[2]=novoHash(capacidadeInicial, funcHash2, colisaoLinear);
    ha[3]=novoHash(capacidadeInicial, funcHash2, colisaoQuadratica);

    for(n=tokens->raiz;n&&!err;n=n->prox) {
        t=(token*)n->val;
        if(!executando) {
            if(t->cmd==iniciar)
                executando=1;
            else
                continue;
        }
        switch(t->cmd) {
            case iniciar:
                err=1;
                break;
            case encerrar:
                executando=0;
                break;
            case inserirAvl:
                p=novoProcesso(t->params[0]->val.integer, t->params[1]->val.string, t->params[2]->val.integer, t->params[3]->val.status);
                insereAvl(a, (void*)p);
                insereHeap(he, (void*)p);
                for(i=0;i<4;i++)
                    defineHash(ha[i], p->codigo, (void*)p);
                p=NULL;
                break;
            case terminarAvl:
                p=(processo*)removeAvl(a, t->params[0]->val.integer);
                encerraProcesso(&p);
                p=NULL;
                break;
            case listarAvl:
                listaProcessosAvl(a);
                break;
            case alterarHeap:
                k=(int*)malloc(sizeof(int));
                if(!k) {
                    err=2;
                    break;
                }
                *k=t->params[0]->val.integer;
                p=(processo*)buscaHeap(he, buscandoNoHeap, (void*)k);
                mudaPrioridade(p, t->params[1]->val.integer);
                p=NULL;
                break;
            case removerHeap:
                p=(processo*)removePrioritario(he);
                encerraProcesso(&p);
                p=NULL;
                break;
            case listarHeap:
                listaProcessosHeap(he);
                break;
            case bloquearHash:
                for(i=0;i<4;i++) {
                    p=(processo*)pegaHash(ha[i], t->params[0]->val.integer);
                    mudaStatus(p, bloqueado);
                    p=NULL;
                }
                break;
            case desbloquearHash:
                for(i=0;i<4;i++) {
                    p=(processo*)pegaHash(ha[i], t->params[0]->val.integer);
                    mudaStatus(p, pronto);
                    p=NULL;
                }
                break;
            case executar:
                for(i=0;i<4;i++) {
                    p=(processo*)pegaHash(ha[i], t->params[0]->val.integer);
                    mudaStatus(p, executando);
                    p=NULL;
                }
                break;
            case removerHash:
                for(i=0;i<4;i++) {
                    p=(processo*)removeHash(ha[i], t->params[0]->val.integer);
                    encerraProcesso(&p);
                    p=NULL;
                }
                break;
            case listarHash:
                listaHash(ha[0], t->params[0]->val.status);
                break;
            case terminar:
                k=(int*)malloc(sizeof(int));
                if(!k) {
                    err=2;
                    break;
                }
                *k=t->params[0]->val.integer;
                removeHeap(he, buscandoNoHeap, k);
                free(k);
                p=removeAvl(a,t->params[0]->val.integer);
                for(i=0;i<4;i++)
                    removeHash(ha[i],t->params[0]->val.integer);
                encerraProcesso(&p);
                break;
        }
    }

    liberaAvlFunc(&a, encerraProcessoGenerico);
    liberaHeapFunc(&he, encerraProcessoGenerico);
    for(i=0;i<4;i++)
        liberaHashFunc(&ha[i], encerraProcessoGenerico);

    return err;
}

void* funcPrioridadeHeap(void* a, void* b) {
    processo* p=(processo*)a;
    processo* q=(processo*)b;
    return p->prioridade>q->prioridade ? a : b;
}

int extraiCodigo(void* val) {
    processo* p=(processo*)val;
    return p ? p->codigo : -1;
}

int buscandoNoHeap(void* procura, void* val) {
    processo* p=(processo*)val;
    return p ? p->codigo==*(int*)procura : 0;
}

void listaProcessosNoAvl(noAvl* n) {
    if(!n)
        return;
    processo* p=(processo*)n->val;
    listaProcessosNoAvl(n->esq);
    if(p) {
        mostraProcesso(p);
        printf("===\n");
    }
    listaProcessosNoAvl(n->dir);
}

void listaProcessosAvl(avl* a) {
    if(!a)
        return;
    else if(!a->raiz)
        printf("A arvore AVL esta vazia!");
    else
        listaProcessosNoAvl(a->raiz);
}

void listaProcessosHeap(heap* h) {
    if(!h)
        return;
    int i;
    void** heapOrg=heapOrganizado(h);
    for(i=0;i<h->tamanho;i++) {
        mostraProcesso((processo*)heapOrg[i]);
        printf("===\n");
    }
    free(heapOrg);
    heapOrg=NULL;
}

void listaHash(hash* h, statusCodigo status) {
    if(!h)
        return;
    int i;
    processo* p=NULL;
    for(i=0;i<h->tamanho;i++) {
        p=(processo*)pegaHash(h, h->chaves[i]);
        if(p->status==status) {
            mostraProcesso(p);
            printf("===\n");
        }
    }
}

void encerraProcessoGenerico(void* val) {
    processo* p=(processo*)val;
    encerraProcesso(&p);
}

