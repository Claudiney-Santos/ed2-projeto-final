#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processos.h"

void chomp(char* str) {
    str[strcspn(str,"\n")]=0;
}

int main() {
    int codigo=1, prioridade;
    statusCodigo status=pronto;
    char* nome=(char*)malloc(21*sizeof(char));

    printf("Informe o nome do processo: ");
    fgets(nome, 21, stdin);
    chomp(nome);

    printf("Informa a prioridade: ");
    scanf("%d", &prioridade);

    processo* p=novoProcesso(codigo, nome, prioridade, status);

    printf("PROCESSOS:\nCodigo: %d\nNome: %s\nPrioridade: %d\nStatus: %d\n", p->codigo, p->nome, p->prioridade, p->status);

    encerrarProcesso(&p);

    return 0;
}
