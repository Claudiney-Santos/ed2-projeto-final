#include "processos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

processo* novoProcesso(int codigo, char* nome, int prioridade, statusCodigo status) {
    if(!nome)
        return NULL;
    processo* p=(processo*)malloc(sizeof(processo));
    if(p) {
        p->codigo=codigo;
        p->prioridade=prioridade;
        p->status=status;
        p->nome=(char*)malloc((strlen(nome)+1)*sizeof(char));
        if(p->nome)
            strcpy(p->nome, nome);
        else {
            free(p);
            p=NULL;
        }
    }
    return p;
}

int encerraProcesso(processo** p) {
    if(!p||!(*p))
        return 1;
    if((*p)->nome)
        free((*p)->nome);
    free(*p);
    *p=NULL;
    return 0;
}

int mudaPrioridade(processo* p, int novaPrioridade) {
    if(!p)
        return 1;
    p->prioridade=novaPrioridade;
    return 0;
}

int mudaStatus(processo* p, statusCodigo novoStatus) {
    if(!p)
        return 1;
    p->status=novoStatus;
    return 0;
}

void mostraProcesso(processo* p) {
    if(!p)
        return;
    char* status=statusParaString(p->status);
    printf("Codigo: %d\nNome: %s\nPrioridade: %d\nStatus: %s", p->codigo, p->nome, p->prioridade, status);
    free(status);
}

char* statusParaString(statusCodigo status) {
    char* str=NULL;
    switch(status) {
        case pronto:
            str=(char*)malloc(7*sizeof(char));
            strcpy(str, "PRONTO");
            break;
        case execucao:
            str=(char*)malloc(9*sizeof(char));
            strcpy(str, "EXECUCAO");
            break;
        case bloqueado:
            str=(char*)malloc(10*sizeof(char));
            strcpy(str, "BLOQUEADO");
            break;
    }
    return str;
}

