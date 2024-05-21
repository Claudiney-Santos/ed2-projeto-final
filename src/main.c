#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "processos.h"
#include "lista.h"

void iniciaProcesso(lista* l);
void veProcesso(lista* l);
void veProcessos(lista* l);
void removeProcesso(lista* l);

void encerraProcessos(void* val, int indice, lista* _) {
    processo* p=(processo*)val;
    free(p->nome);
    p->nome=NULL;
}

int main() {
    lista* l=novaLista();
    if(!l)
        return 1;

    int escolha=-1;
    while(escolha) {
        printf("1) Iniciar processo\n2) Ver processo\n3) Ver processos\n4) Remover processo\n0) Sair\n\nEscolha: ");
        scanf("%d", &escolha);
        printf("\n");
        switch(escolha) {
            case 1:
                iniciaProcesso(l);
                break;
            case 2:
                veProcesso(l);
                break;
            case 3:
                veProcessos(l);
                break;
            case 4:
                removeProcesso(l);
                break;
            case 0:
                break;
            default:
                printf("Escolha invalida!\n");
        }
        printf("\n");
    }

    paraCada(l, encerraProcessos);
    liberaLista(&l);

    return 0;
}

void iniciaProcesso(lista* l) {
    if(!l)
        return;
    int codigo=0, prioridade=0, status=0;
    char* nome=(char*)malloc(21*sizeof(char));

    printf("Informe o codigo: ");
    scanf("%d", &codigo);

    printf("Informe o nome: ");
    fgets(nome, 21, stdin);
    chomp(nome);

    printf("Informe a prioridade: ");
    scanf("%d", &prioridade);

    do {
        printf("Status:\n1) PRONTO\n2) EXECUCAO\n3) BLOQUEADO\n\nInforme: ");
        scanf("%d", &status);
    } while(status<0||status>3);

    insereUltimo(l,
        (void*)novoProcesso(codigo, nome, prioridade, (statusCodigo)status));
}

void veProcesso(lista* l) {
    if(!l)
        return;
    int indice=0;
    processo* p=NULL;
    printf("Informe o indice: ");
    scanf("%d", &indice);
    p=(processo*)pegaPosicao(l, indice);
    printf("\n");
    if(p)
        printf("Processo:\nCodigo: %d\nNome: \"%s\"\nPrioridade: %d\nStatus: %d\n", p->codigo, p->nome, p->prioridade, p->status);
    else
        printf("Processo nao encontrado!\n");
    printf("\n");
}

void mostraProcessos(void* val, int indice, lista* _) {
    processo* p=(processo*)val;
    if(indice)
        printf("===\n");
    printf("Processo %d:\n", indice);
    printf("Codigo: %d\nNome: \"%s\"\nPrioridade: %d\nStatus: %d\n", p->codigo, p->nome, p->prioridade, p->status);
}

void veProcessos(lista* l) {
    if(!l)
        return;
    paraCada(l, mostraProcessos);
    printf("\n");
}

void removeProcesso(lista* l) {
    if(!l)
        return;
    int indice=0;
    processo* p=NULL;
    printf("Informe o indice: ");
    scanf("%d", &indice);

    p=(processo*)removePosicao(l, indice);

    if(p) {
        printf("Removido o processo %d:\n", indice);
        printf("Codigo: %d\nNome: \"%s\"\nPrioridade: %d\nStatus: %d\n", p->codigo, p->nome, p->prioridade, p->status);
        encerraProcesso(&p);
    } else
        printf("Nao foi possivel remover esse processo\n");
}
