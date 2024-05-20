#ifndef _PROCESSOS_H_
#define _PROCESSOS_H_

typedef enum {
    pronto,
    execucao,
    bloqueado
} statusCodigo;

typedef struct {
    int codigo;
    int prioridade;
    char* nome;
    statusCodigo status;
} processo;

processo* novoProcesso(int codigo, char* nome, int prioridade, statusCodigo status);
int encerrarProcesso(processo** p);

int mudarPrioridade(processo* p, int novaPrioridade);
int mudarStatus(processo* p, statusCodigo novoStatus);

#endif
