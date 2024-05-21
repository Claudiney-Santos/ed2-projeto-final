#include "processos.h"
#include <stdlib.h>

processo* novoProcesso(int codigo, char* nome, int prioridade, statusCodigo status) {
    processo* p=(processo*)malloc(sizeof(processo));
    if(p) {
        p->codigo=codigo;
        p->nome=nome;
        p->prioridade=prioridade;
        p->status=status;
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

