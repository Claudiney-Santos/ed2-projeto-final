#include "log.h"

#include <stdio.h>
#include <string.h>

log* novoLog(char* nome) {
    if(!nome)
        return NULL;
    int err=0;
    log* l=(log*)malloc(sizeof(log));
    if(l) {
        l->nome=(char*)malloc((strlen(nome)+1)*sizeof(char));
        l->logs=novaLista();
        if(l->nome)
            strcpy(l->nome, nome);
        else
            err=1;
        if(!l->logs)
            err=1;
        if(err) {
            if(l->nome)
                free(l->nome);
            free(l);
            l=NULL;
        }
    }

    return l;
}

int liberaLog(log** l) {
    if(!l||!(*l))
        return -1;
    free((*l)->nome);
    liberaLista(&(*l)->logs);
    free(*l);
    *l=NULL;
    return 0;
}

int defineNomeLog(log* l, char* nome) {
    if(!l||!nome)
        return -1;
    char* str=(char*)malloc((strlen(nome)+1)*sizeof(char));
    if(str)
        strcpy(str, nome);
    else
        return 1;
    if(l->nome)
        free(l->nome);
    l->nome=str;
    return 0;
}

int adicionaLog(log* l, char* mensagem) {
    if(!l)
        return -1;

    char* msg=(char*)malloc((strlen(mensagem)+1)*sizeof(char));
    if(!msg)
        return 1;
    strcpy(msg, mensagem);
    insereUltimo(l->logs, (void*)msg);

    return 0;
}

int paraCadaLog(log* l, void(*f)(char*, int)) {
    if(!l)
        return -1;
    int i;
    no* n=l->logs->raiz;
    for(i=0;n;n=n->prox)
        (*f)((char*)n->val, i++);
    return 0;
}

int salvarLogEmArquivo(log* l) {
    if(!l)
        return -1;
    char* nome=(char*)malloc((strlen(l->nome)+5)*sizeof(char));
    char* msg=NULL;
    FILE* arquivo=NULL;
    no* n=l->logs->raiz;
    if(!nome)
        return 1;
    sprintf(nome, "%s.log", l->nome);
    arquivo=fopen(nome, "w");
    //printf("%s\n---\n", nome);
    free(nome);
    for(;n;n=n->prox) {
        msg=(char*)n->val;
        //printf("%s\n", msg);
        fprintf(arquivo, "%s\n", msg);
    }
    fclose(arquivo);
    return 0;
}

