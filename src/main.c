#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "parser.h"

#define INPUT_TAMANHO 50

int mostraComandos(lista* l);
int carregaScript(FILE** arquivo, lista** l);

void encerraProcessoLista(void* val) {
    processo* p=(processo*)val;
    encecerraProcesso(&p);
}

int main(int argc, char** argv) {
    long int escolha=1;
    int err=0;
    char input[INPUT_TAMANHO];
    char nomeArquivo[INPUT_TAMANHO];
    char* saida;
    FILE* arquivo=NULL;
    lista* l=NULL;
    char rodar=1;

    while(rodar&&!err) {
        if(arquivo)
            printf("Arquivo carregado: \"%s\"\n", nomeArquivo);
        else
            printf("Nenhum arquivo carregado\n");
        printf("===\n")
        printf("1) Iniciar simulacao\n2) Ver script carregado\n3) Carregar script\n0) Sair\n\nEscolha: ");
        fgets(input, INPUT_TAMANHO, stdin);
        escolha=strtol(input, &saida, 10);
        if(!strcmp(input,saida))
            continue;
        switch(escolha) {
            case 1:
                break;
            case 2:
                err=mostraComandos(l);
                break;
            case 3:
                err=carregaScript(&arquivo, &l);
        }
    }
    return 0;
}

int mostraComandos(lista* l) {
    if(!l||!l->raiz) {
        printf("Nao ha comandos salvos!\n");
        return 0;
    }
    no* n=NULL;
    int i=0;
    for(n=l->raiz;n;n=n->prox, i++) {
        token* t=(token*)n->val;
        if(i)
            printf("===\n");
        printf("Comando %d: ", i);
        switch(t->cmd) {
            case iniciar:
                printf("iniciar");
                break;
            case encerrar:
                printf("encerrar");
                break;
            case inserirAvl:
                printf("inserirAVL");
                break;
            case terminarAvl:
                printf("terminarAVL");
                break;
            case listarAvl:
                printf("listarAVL");
                break;
            case alterarHeap:
                printf("alterarHeap");
                break;
            case removerHeap:
                printf("removerHeap");
                break;
            case listarHeap:
                printf("listarHeap");
                break;
            case bloquearHash:
                printf("bloquearHash");
                break;
            case desbloquearHash:
                printf("desbloquearHash");
                break;
            case executar:
                printf("executar");
                break;
            case removerHash:
                printf("removerHash");
                break;
            case listarHash:
                printf("listarHash");
                break;
            case terminar:
                printf("terminar");
                break;
        }
        printf("\n");
    }
    return 0;
}

int carregaScript(FILE** arquivo, lista** l) {
    char input[INPUT_TAMANHO];
    if(!arquivo||!l)
        return 1;
    else if(*arquivo)
        fclose(*arquivo);
    if(*l)
        liberaListaFunc(l, encerraProcessoLista);
    printf("Informe o nome do arquivo: ");
    fgets(input, INPUT_TAMANHO, stdin);
    chomp(input);
    *arquivo=fopen(input, "r");
    if(!(*arquivo)) {
        printf("Nao foi possivel abrir o arquivo \"%s\"\n", input);
        return 2;
    }
    *l=tokenizaLista(*arquivo);
    if(!l) {
        fclose(*arquivo);
        *arquivo=NULL;
        return 3;
    }
    return 0;
}

