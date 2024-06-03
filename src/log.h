#ifndef _LOG_H_
#define _LOG_H_ 1

#include "lista.h"

typedef struct {
    char* nome;
    lista* logs;
} log;

log* novoLog(char* nome);
int liberaLog(log** l);

int defineNomeLog(log* l, char* nome);
int adicionaLog(log* l, char* mensagem);
int paraCadaLog(log* l, void(*f)(char*, int));
int salvarLogEmArquivo(log* l);

#endif
