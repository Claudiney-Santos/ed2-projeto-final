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
int encerraProcesso(processo** p);

int mudaPrioridade(processo* p, int novaPrioridade);
int mudaStatus(processo* p, statusCodigo novoStatus);

void mostraProcesso(processo* p);

char* statusParaString(statusCodigo status);

#endif
