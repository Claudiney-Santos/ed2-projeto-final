#ifndef _PARSER_H_
#define _PARSER_H_

#define MAX_CHAR_LIDOS_POR_LINHA 80

#include "lista.h"
#include "processos.h"
#include <stdio.h>

typedef enum {
    iniciar,
    encerrar,
    inserirAvl,
    terminarAvl,
    listarAvl,
    alterarHeap,
    removerHeap,
    listarHeap,
    bloquearHash,
    desbloquearHash,
    executar,
    removerHash,
    listarHash,
    terminar
} comandosToken;

typedef enum {
    integer,
    string,
    status
} paramTokenType;

typedef union {
    int integer;
    char* string;
    statusCodigo status;
} paramTokenVal;

typedef struct {
    paramTokenType type;
    paramTokenVal val;
} paramToken;

typedef struct {
    comandosToken cmd;
    paramToken** params;
    size_t paramLen;
} token;

token* criaToken(comandosToken cmd);
void liberaToken(token** t);
void liberaTokenLista(void* val);

int defineParamInteger(token* t, int indice, int val);
int defineParamString(token* t, int indice, char* val);
int defineParamStatus(token* t, int indice, statusCodigo val);

lista* tokenizaArquivo(FILE* arquivo);

#endif
