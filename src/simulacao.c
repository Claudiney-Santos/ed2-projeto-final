#include "simulacao.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void* funcPrioridadeHeap(void* a, void* b);
int extraiCodigo(void* val);
int buscandoNoHeap(void* procura, void* val);
void listaProcessosNoAvl(avl* a, noAvl* n);
void listaProcessosAvl(avl* a);
void listaProcessosHeap(heap* h);
void listaHash(hash* h, statusCodigo status);
void encerraProcessoGenerico(void* val);

double calculaTempo(clock_t* tempo);

int adicionarProcessoAvl(token* t, avl* a, clock_t* tempo, char* msg) {
    double delta;
    processo *p=novoProcesso(t->params[0]->val.integer, t->params[1]->val.string, t->params[2]->val.integer, t->params[3]->val.status);

    calculaTempo(tempo);
    insereAvl(a, (void*)p);
    delta=calculaTempo(tempo);
    if(msg) {
        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
        adicionaLog(a->registro, msg);
    }

    return 0;
}
int adicionarProcessoHeap(token* t, heap* h, clock_t* tempo, char* msg) {
    double delta;
    processo* p=novoProcesso(t->params[0]->val.integer, t->params[1]->val.string, t->params[2]->val.integer, t->params[3]->val.status);

    calculaTempo(tempo);
    insereHeap(h, (void*)p);
    delta=calculaTempo(tempo);
    if(msg) {
        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
        adicionaLog(h->registro, msg);
    }

    return 0;
}
int adicionarProcessoHash(token* t, hash* h, clock_t* tempo, char* msg) {
    double delta;
    processo* p=novoProcesso(t->params[0]->val.integer, t->params[1]->val.string, t->params[2]->val.integer, t->params[3]->val.status);
    processo* q=NULL;
    calculaTempo(tempo);
    q=defineHash(h, p->codigo, (void*)p);
    delta=calculaTempo(tempo);
    if(q)
        encerraProcesso(&q);
    if(msg) {
        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
        adicionaLog(h->registro, msg);
    }

    return 0;
}

double calculaTempo(clock_t* tempo) {
    double delta=0.0;
    if(*tempo) {
        delta=(double)(clock()-*tempo)/CLOCKS_PER_SEC;
        *tempo=0;
    } else
        *tempo=clock();
    return delta;
}

int executarSimulacao(lista* tokens) {
    if(!tokens)
        return -1;
    size_t capacidadeInicial=101;

    double delta=0;
    clock_t tempo=0;

    int err=0, i=0;

    char executando=0;
    int* k=(int*)malloc(sizeof(int));

    char* msg=(char*)malloc(256*sizeof(char));

    char* temp=NULL;
    no* n=NULL;
    token* t=NULL;

    avl* a=novaAvl(extraiCodigo);
    hash* ha[4];
    heap* he=novoHeap(capacidadeInicial, funcPrioridadeHeap);

    processo* p=NULL;
    //processo* q=NULL;

    ha[0]=novoHash(capacidadeInicial, funcHash1, colisaoLinear);
    ha[1]=novoHash(capacidadeInicial, funcHash1, colisaoQuadratica);
    ha[2]=novoHash(capacidadeInicial, funcHash2, colisaoLinear);
    ha[3]=novoHash(capacidadeInicial, funcHash2, colisaoQuadratica);

    if(!k||!msg||!a||!he||!ha[0]||!ha[1]||!ha[2]||!ha[3])
        err=1;

    for(i=0;!err&&i<4;i++) {
        sprintf(msg, "HashFuncHash%sColisao%s", i<2 ? "1" : "2", i%2 ? "Quadratica" : "Linear");
        err=defineNomeLog(ha[i]->registro, msg);
    }

    for(n=tokens->raiz;n&&!err;n=n->prox) {
        t=(token*)n->val;
        if(!executando&&t->cmd!=iniciar)
            continue;
        switch(t->cmd) {
            case iniciar:
                executando=1;
                break;
            case encerrar:
                executando=0;
                break;
            case inserirAvl:
                adicionarProcessoAvl(t, a, &tempo, msg);
                adicionarProcessoHeap(t, he, &tempo, msg);
                for(i=0;i<4;i++)
                    adicionarProcessoHash(t, ha[i], &tempo, msg);

                p=NULL;
                break;
            //case terminarAvl:
            //case removerHash:
                //calculaTempo(&tempo);
                //p=(processo*)removeAvl(a, t->params[0]->val.integer);
                //delta=calculaTempo(&tempo);
                //if(msg) {
                //    sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                //    adicionaLog(a->registro, msg);
                //}

                //for(i=0;i<4;i++) {
                //    calculaTempo(&tempo);
                //    p=(processo*)removeHash(ha[i], t->params[0]->val.integer);
                //    delta=calculaTempo(&tempo);
                //    if(msg) {
                //        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                //        adicionaLog(ha[i]->registro, msg);
                //    }

                //    encerraProcesso(&p);
                //    p=NULL;
                //}

                //encerraProcesso(&p);
                //p=NULL;
                //break;
            case listarAvl:
                calculaTempo(&tempo);
                listaProcessosAvl(a);
                delta=calculaTempo(&tempo);
                if(msg) {
                    sprintf(msg, "A listagem de todos os elementos foi feita em %g segundos", delta);
                    adicionaLog(a->registro, msg);
                }
                break;
            case alterarHeap:
                *k=t->params[0]->val.integer;
                calculaTempo(&tempo);
                p=(processo*)buscaHeap(he, buscandoNoHeap, (void*)k);
                //p=(processo*)pegaAvl(a, t->params[0]->val.integer);
                i=p ? p->prioridade : 0;
                if(p) {
                    mudaPrioridade(p, t->params[1]->val.integer);
                    heapifica(he, 0);
                }
                delta=calculaTempo(&tempo);
                if(msg) {
                    if(p)
                        sprintf(msg, "O processo com codigo %d teve sua prioridade (%d) alterada para %d, isso foi feito em %g segundos", p->codigo, i, p->prioridade, delta);
                    else
                        sprintf(msg, "Ocorreu algum erro ao tentar buscar o processo com codigo %d no heap", *k);
                    adicionaLog(he->registro, msg);
                }
                p=NULL;
                break;
            case removerHeap:
                calculaTempo(&tempo);
                p=(processo*)removePrioritario(he);
                delta=calculaTempo(&tempo);
                if(msg) {
                    if(p)
                        sprintf(msg, "O processo com codigo %d foi removido em %g segundos", p->codigo, delta);
                    else
                        sprintf(msg, "Ocorreu algum problema ao tentar remover o elemento prioritario");
                    adicionaLog(he->registro, msg);
                }
                encerraProcesso(&p);

                //if(p) {
                //    calculaTempo(&tempo);
                //    p=removeAvl(a,p->codigo);
                //    delta=calculaTempo(&tempo);
                //    if(msg) {
                //        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                //        adicionaLog(a->registro, msg);
                //    }
                //    encerraProcesso(&p);

                //    for(i=0;i<4;i++) {
                //        calculaTempo(&tempo);
                //        p=removeHash(ha[i],p->codigo);
                //        delta=calculaTempo(&tempo);
                //        if(msg) {
                //            sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                //            adicionaLog(ha[i]->registro, msg);
                //        }
                //        encerraProcesso(&p);
                //    }
                //}

                p=NULL;
                break;
            case listarHeap:
                calculaTempo(&tempo);
                listaProcessosHeap(he);
                delta=calculaTempo(&tempo);
                if(msg) {
                    sprintf(msg, "A listagem de todos os elementos foi feita em %g segundos", delta);
                    adicionaLog(he->registro, msg);
                }
                break;
            case bloquearHash:
                for(i=0;i<4;i++) {
                    calculaTempo(&tempo);
                    p=(processo*)pegaHash(ha[i], t->params[0]->val.integer);
                    if(p) {
                        temp=p->status==bloqueado ? "BLOQUEADO" : p->status==pronto ? "PRONTO" : "EXECUTANDO";
                        mudaStatus(p, bloqueado);
                    }
                    delta=calculaTempo(&tempo);
                    if(msg) {
                        //sprintf(msg, "O processo com codigo %d passou do estado %s para BLOQUEADO, isso foi feito em %g segundos", p->codigo, temp, delta);
                        if(p)
                            sprintf(msg, "O processo com codigo %d passou para o estado BLOQUEADO, isso foi feito em %g segundos", p->codigo, delta);
                        else
                            sprintf(msg, "Ocorreu um erro ao tentar mudar o estado do processo com codigo %d", t->params[0]->val.integer);
                        adicionaLog(ha[i]->registro, msg);
                    }
                    p=NULL;
                }
                break;
            case desbloquearHash:
                for(i=0;i<4;i++) {
                    calculaTempo(&tempo);
                    p=(processo*)pegaHash(ha[i], t->params[0]->val.integer);
                    if(p) {
                        temp=p->status==bloqueado ? "BLOQUEADO" : p->status==pronto ? "PRONTO" : "EXECUTANDO";
                        mudaStatus(p, pronto);
                    }
                    delta=calculaTempo(&tempo);
                    if(msg) {
                        if(p)
                            sprintf(msg, "O processo com codigo %d passou do estado %s para PRONTO, isso foi feito em %g segundos", p->codigo, temp, delta);
                        else
                            sprintf(msg, "Ocorreu um erro ao tentar mudar o estado do processo com codigo %d", t->params[0]->val.integer);
                        adicionaLog(ha[i]->registro, msg);
                    }
                    p=NULL;
                }
                break;
            case executar:
                for(i=0;i<4;i++) {
                    calculaTempo(&tempo);
                    p=(processo*)pegaHash(ha[i], t->params[0]->val.integer);
                    if(p) {
                        temp=p->status==bloqueado ? "BLOQUEADO" : p->status==pronto ? "PRONTO" : "EXECUTANDO";
                        mudaStatus(p, executando);
                    }
                    delta=calculaTempo(&tempo);
                    if(msg) {
                        if(p)
                            sprintf(msg, "O processo com codigo %d passou do estado %s para EXECUTANDO, isso foi feito em %g segundos", p->codigo, temp, delta);
                        else
                            sprintf(msg, "Ocorreu um erro ao tentar mudar o estado do processo com codigo %d", t->params[0]->val.integer);
                        adicionaLog(ha[i]->registro, msg);
                    }
                    p=NULL;
                }
                break;
            case listarHash:
                for(i=0;i<4;i++) {
                    calculaTempo(&tempo);
                    listaHash(ha[i], t->params[0]->val.status);
                    delta=calculaTempo(&tempo);
                    if(msg) {
                        sprintf(msg, "A listagem de todos os elementos com status %s foi feita em %g segundos", t->params[0]->val.status==bloqueado ? "BLOQUEADO" : t->params[0]->val.status==pronto ? "PRONTO" : "EXECUTANDO" , delta);
                        adicionaLog(ha[i]->registro, msg);
                    }
                }
                break;
            case terminarAvl:
                calculaTempo(&tempo);
                p=removeAvl(a,t->params[0]->val.integer);
                delta=calculaTempo(&tempo);
                if(msg) {
                    sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                    adicionaLog(a->registro, msg);
                }
                encerraProcesso(&p);
                break;
            case removerHash:
                for(i=0;i<4;i++) {
                    calculaTempo(&tempo);
                    p=removeHash(ha[i],t->params[0]->val.integer);
                    delta=calculaTempo(&tempo);
                    if(msg) {
                        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                        adicionaLog(ha[i]->registro, msg);
                    }
                    encerraProcesso(&p);
                }
                break;
            case terminar:
                if(!k) {
                    err=2;
                    break;
                }
                *k=t->params[0]->val.integer;

                calculaTempo(&tempo);
                p=removeHeap(he, buscandoNoHeap, k);
                delta=calculaTempo(&tempo);
                if(msg) {
                    //sprintf(msg, "O processo com codigo %d e prioridade %d foi removido do heap em %g segundos", p->codigo, p->prioridade, delta);
                    sprintf(msg, "O processo anterior foi removido do heap em %g segundos", delta);
                    adicionaLog(he->registro, msg);
                }
                encerraProcesso(&p);

                calculaTempo(&tempo);
                p=removeAvl(a,t->params[0]->val.integer);
                delta=calculaTempo(&tempo);
                if(msg) {
                    sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                    adicionaLog(a->registro, msg);
                }
                encerraProcesso(&p);

                for(i=0;i<4;i++) {
                    calculaTempo(&tempo);
                    p=removeHash(ha[i],t->params[0]->val.integer);
                    delta=calculaTempo(&tempo);
                    if(msg) {
                        sprintf(msg, "A ultima operacao foi executada em %g segundos", delta);
                        adicionaLog(ha[i]->registro, msg);
                    }
                    encerraProcesso(&p);
                }
                break;
        }
    }
    if(k)
        free(k);

    if(!err) {
        salvarLogEmArquivo(a->registro);
        printf("Os arquivos \"%s.log\"", a->registro->nome);
        for(i=0;i<4;i++) {
            salvarLogEmArquivo(ha[i]->registro);
            printf(", \"%s.log\"", ha[i]->registro->nome);
        }
        salvarLogEmArquivo(he->registro);
        printf(" e \"%s.log\" foram salvos!\n", he->registro->nome);
    }

    if(msg)
        free(msg);
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

void listaProcessosNoAvl(avl* a, noAvl* n) {
    if(!n||!a)
        return;
    processo* p=(processo*)n->val;
    char* msg=NULL;
    char* status=NULL;
    listaProcessosNoAvl(a, n->esq);
    if(p) {
        msg=(char*)malloc(256*sizeof(char));
        status=p->status==bloqueado ? "BLOQUEADO" : p->status==pronto ? "PRONTO" : "EXECUTANDO";
        sprintf(msg, "===\nCodigo: %d\nNome: %s\nPrioridade: %d\nStatus: %s\n===", p->codigo, p->nome, p->prioridade, status);
        adicionaLog(a->registro, msg);
        free(msg);
    }
    listaProcessosNoAvl(a, n->dir);
}

void listaProcessosAvl(avl* a) {
    if(!a)
        return;
    else if(!a->raiz) {
        adicionaLog(a->registro, "Nao foi possivel listar processos na arvore AVL, pois ela esta vazia!");
    } else {
        adicionaLog(a->registro, "Listando processos na AVL:");
        listaProcessosNoAvl(a, a->raiz);
    }
}

void listaProcessosHeap(heap* h) {
    if(!h)
        return;
    int i;
    void** heapOrg=heapOrganizado(h);
    char* msg=NULL;
    char* status=NULL;
    processo* p=NULL;
    adicionaLog(h->registro, "Listando processos no Heap:");
    for(i=0;i<h->tamanho;i++) {
        p=(processo*)heapOrg[i];
        msg=(char*)malloc(256*sizeof(char));
        status=p->status==bloqueado ? "BLOQUEADO" : p->status==pronto ? "PRONTO" : "EXECUTANDO";
        sprintf(msg, "===\nCodigo: %d\nNome: %s\nPrioridade: %d\nStatus: %s\n===", p->codigo, p->nome, p->prioridade, status);
        adicionaLog(h->registro, msg);
        free(msg);
    }
    free(heapOrg);
    heapOrg=NULL;
}

void listaHash(hash* h, statusCodigo status) {
    if(!h)
        return;
    int i;
    processo* p=NULL;
    char* msg=NULL;
    char* statusStr=status==bloqueado ? "BLOQUEADO" : status==pronto ? "PRONTO" : "EXECUTANDO";
    for(i=0;i<h->tamanho;i++) {
        p=(processo*)pegaHash(h, h->chaves[i]);
        if(p->status==status) {
            msg=(char*)malloc(256*sizeof(char));
            sprintf(msg, "===\nCodigo: %d\nNome: %s\nPrioridade: %d\nStatus: %s\n===", p->codigo, p->nome, p->prioridade, statusStr);
            adicionaLog(h->registro, msg);
            free(msg);
        }
    }
}

void encerraProcessoGenerico(void* val) {
    processo* p=(processo*)val;
    encerraProcesso(&p);
}

