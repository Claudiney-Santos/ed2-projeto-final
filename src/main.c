#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lista.h"

void mostrarComandos(lista* l);

int main(int argc, char** argv) {
    char rodar=1;
    int escolha=1;
    FILE* arquivo=NULL;
    lista* l=NULL;

    l=novaLista();
    while(rodar) {
        printf("");
    }
    return 0;
}

void mostrarComandos(lista* l) {
    if(!l||!l->raiz) {
        printf("Nao ha comandos salvos!\n");
        return;
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

}

