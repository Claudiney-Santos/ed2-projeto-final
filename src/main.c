#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "lista.h"
#include "processos.h"
#include "parser.h"
#include "simulacao.h"

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
    if((*s)->arq)
        fclose((*s)->arq);
    if((*s)->nome)
        free((*s)->nome);
    if((*s)->l)
        liberaListaFunc(&((*s)->l), liberaTokenLista);
    free(*s);
    *s=NULL;
}

int mostraComandos(lista* l);
int carregaScript(script* arquivo);

int main(int argc, char** argv) {
    long int escolha=1;
    int err=0;
    char rodar=1;
    char input[INPUT_TAMANHO];
    char* saida;
    script* arquivo=novoScript();

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
                executarSimulacao(arquivo->l);
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
    int i=0;
    no* n=NULL;
    char* status=NULL;
    for(n=l->raiz;n;n=n->prox, i++) {
        token* t=(token*)n->val;
        printf("Comando %d: ", i);
        switch(t->cmd) {
            case iniciar:
                printf("Iniciar");
                break;
            case encerrar:
                printf("Encerrar");
                break;
            case inserirAvl:
                printf("InserirAVL %d %s %d ", t->params[0]->val.integer, t->params[1]->val.string, t->params[2]->val.integer);
                status=statusParaString(t->params[3]->val.status);
                if(status) {
                    printf("%s", status);
                    free(status);
                    status=NULL;
                } else
                    printf("%d", t->params[3]->val.status);
                break;
            case terminarAvl:
                printf("TerminarAVL %d", t->params[0]->val.integer);
                break;
            case listarAvl:
                printf("ListarAVL");
                break;
            case alterarHeap:
                printf("AlterarHeap %d %d", t->params[0]->val.integer, t->params[0]->val.integer);
                break;
            case removerHeap:
                printf("RemoverHeap");
                break;
            case listarHeap:
                printf("ListarHeap");
                break;
            case bloquearHash:
                printf("BloquearHash %d", t->params[0]->val.integer);
                break;
            case desbloquearHash:
                printf("DesbloquearHash %d", t->params[0]->val.integer);
                break;
            case executar:
                printf("Executar %d", t->params[0]->val.integer);
                break;
            case removerHash:
                printf("RemoverHash %d", t->params[0]->val.integer);
                break;
            case listarHash:
                printf("ListarHash ");
                status=statusParaString(t->params[0]->val.status);
                if(status) {
                    printf("%s", status);
                    free(status);
                    status=NULL;
                } else
                    printf("%d", t->params[0]->val.status);
                break;
            case terminar:
                printf("Terminar %d %s", t->params[0]->val.integer, t->params[1]->val.string);
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
        //return 2;
    }
    return 0;
}

