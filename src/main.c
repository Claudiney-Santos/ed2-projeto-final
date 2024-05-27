#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "lista.h"
#include "parser.h"

#define INPUT_TAMANHO 50

typedef struct {
    FILE* arq;
    char* nome;
    lista* l;
} script;

script* novoScript() {
    script* s=(script*)malloc(sizeof(script));
    if(s) {
        s->arq=NULL;
        s->nome=(char*)malloc(INPUT_TAMANHO*sizeof(script));
        s->nome[0]=0;
        s->l=NULL;
    }
    return s;
}

void liberaScript(script** s) {
    if(!s||!(*s))
        return;
    fclose((*s)->arq);
    free((*s)->nome);
    liberaListaFunc(&((*s)->l), liberaTokenLista);
    free(*s);
    *s=NULL;
}

int mostraComandos(lista* l);
int carregaScript(script* arquivo);

void encerraProcessoLista(void* val) {
    processo* p=(processo*)val;
    encerraProcesso(&p);
}

int main(int argc, char** argv) {
    long int escolha=1;
    int err=0;
    char input[INPUT_TAMANHO];
    char* saida;
    script* arquivo=novoScript();
    char rodar=1;

    while(rodar&&!err) {
        if(arquivo->l)
            printf("Arquivo carregado: \"%s\"\n", arquivo->nome);
        else
            printf("Nenhum arquivo carregado\n");
        printf("===\n");
        printf("1) Iniciar simulacao\n2) Ver script carregado\n3) Carregar script\n0) Sair\n\nEscolha: ");
        fgets(input, INPUT_TAMANHO, stdin);
        escolha=strtol(input, &saida, 10);
        if(!strcmp(input,saida))
            continue;
        switch(escolha) {
            case 0:
                rodar=0;
                break;
            case 1:
                break;
            case 2:
                err=mostraComandos(arquivo->l);
                break;
            case 3:
                err=carregaScript(arquivo);
                break;
        }
        printf("\n===\n");
    }
    liberaScript(&arquivo);
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

int carregaScript(script* arquivo) {
    if(!arquivo)
        return 1;
    else if(arquivo->arq)
        fclose(arquivo->arq);
    if(arquivo->l)
        liberaListaFunc(&(arquivo->l), liberaTokenLista);
    printf("Informe o nome do arquivo: ");
    fgets(arquivo->nome, INPUT_TAMANHO, stdin);
    chomp(arquivo->nome);
    arquivo->arq=fopen(arquivo->nome, "r");
    if(!arquivo->arq) {
        printf("Nao foi possivel abrir o arquivo \"%s\"\n", arquivo->nome);
        arquivo->nome[0]=0;
        return 0;
    }
    arquivo->l=tokenizaArquivo(arquivo->arq);
    if(!arquivo->l) {
        fclose(arquivo->arq);
        arquivo->arq=NULL;
        arquivo->nome[0]=0;
        return 2;
    }
    return 0;
}

