#include "parser.h"

#include "utils.h"
#include <string.h>

token* criarToken(comandosToken cmd) {
    token* t=(token*)malloc(sizeof(token));
    if(t) {
        t->cmd=cmd;
        switch(cmd) {
            case iniciar:
            case encerrar:
            case listarAvl:
            case listarHeap:
            case removerHeap:
                t->paramLen=0;
                break;
            case terminarAvl:
            case bloquearHash:
            case desbloquearHash:
            case executar:
            case removerHash:
            case listarHash:
                t->paramLen=1;
                break;
            case alterarHeap:
            case terminar:
                t->paramLen=2;
                break;
            case inserirAvl:
                t->paramLen=4;
                break;
        }
        t->params=(paramToken**)calloc(t->paramLen,sizeof(paramToken*));
    }
    return t;
}

void liberaToken(token** t) {
    if(!t||!(*t))
        return;
    int i;
    for(i=0;i<(*t)->paramLen&&(*t)->params[i];i++) {
        if((*t)->params[i]->type==string)
            free((*t)->params[i]->val.string);
        free((*t)->params[i]);
    }
    free(*t);
    *t=NULL;
}

void liberaTokenLista(void* val) {
    token* t=(token*)val;
    liberaToken(&t);
}

int defineParamInteger(token* t, int indice, int val) {
    if(!t)
        return 1;
    else if(t->paramLen<=indice)
        return 2;
    t->params[indice]=(paramToken*)malloc(sizeof(paramToken));
    if(!t->params[indice])
        return 3;
    t->params[indice]->val.integer=val;
    return 0;
}

int defineParamString(token* t, int indice, char* val) {
    if(!t)
        return 1;
    else if(t->paramLen<=indice)
        return 2;
    t->params[indice]=(paramToken*)malloc(sizeof(paramToken));
    if(!t->params[indice])
        return 3;
    t->params[indice]->val.string=(char*)malloc((strlen(val)+1)*sizeof(char));
    strcpy(t->params[indice]->val.string, val);
    return 0;
}

int defineParamStatus(token* t, int indice, statusCodigo val) {
    if(!t)
        return 1;
    else if(t->paramLen<=indice)
        return 2;
    t->params[indice]=(paramToken*)malloc(sizeof(paramToken));
    if(!t->params[indice])
        return 3;
    t->params[indice]->val.status=val;
    return 0;
}

lista* tokenizaArquivo(FILE* arquivo) {
    if(!arquivo)
        return NULL;

    int paramQnt=0, currentParam=0, err=0, linhaNum, colunaNum;
    char linha[MAX_CHAR_LIDOS_POR_LINHA];
    char* palavra=NULL;
    lista* l=novaLista();
    token* t=NULL;

    linhaNum=-1;
    while(!err&&fgets(linha, MAX_CHAR_LIDOS_POR_LINHA, arquivo)) {
        linhaNum++;
        if(!strncmp(linha, "//", 2))
            continue;
        chomp(linha);
        lowerString(linha);
        palavra=strtok(linha," ");
        colunaNum=-1;
        while(!err&&palavra) {
            colunaNum++;
            if(!strcmp(palavra, "iniciar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(iniciar);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "encerrar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(encerrar);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "inseriravl")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(inserirAvl);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "terminaravl")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(terminarAvl);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "listaravl")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(listarAvl);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "alterarheap")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(alterarHeap);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "removerheap")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(removerHeap);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "listarheap")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(listarHeap);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "bloquearhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(bloquearHash);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "desbloquearhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(desbloquearHash);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "executar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(executar);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "removerhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(removerHash);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "listarhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(listarHash);
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "terminar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(terminar);
                paramQnt=t->paramLen;
            } else {
                if(!paramQnt) {
                    err=2;
                    continue;
                }
                currentParam=t->paramLen-paramQnt;
                if(isNumeric(palavra))
                    defineParamInteger(t, currentParam, atoi(palavra));
                else if(isAlphabetic(palavra)) {
                    if(!strcmp(palavra, "pronto"))
                        defineParamStatus(t, currentParam, pronto);
                    else if(!strcmp(palavra, "execucao"))
                        defineParamStatus(t, currentParam, execucao);
                    else if(!strcmp(palavra, "bloqueado"))
                        defineParamStatus(t, currentParam, bloqueado);
                    else {
                        defineParamString(t, currentParam, palavra);
                    }
                } else {
                    err=3;
        return NULL;
                    continue;
                }
                paramQnt--;
                if(!paramQnt) {
                    insereUltimo(l, (void*)t);
                    t=NULL;
                }
            }
            palavra=strtok(NULL," ");
        }
    }
    if(err) {
        printf("Ocorreu um erro na linha %d, na palavra %d:\n", linhaNum, colunaNum);
        switch(err) {
            case 1:
                printf("Era esperado um parametro, porem foi recebido um comando\n");
                break;
            case 2:
                printf("Era esperado um comando, porem foi recebido um parametro\n");
                break;
            case 3:
                printf("Era esperado um parametro, porem foi recebido uma string invalida (nem numerica, nem alfabetica)\n");
                break;
        }
        if(t)
            liberaToken(&t);
        liberaListaFunc(&l, liberaTokenLista);
    }
    return l;
}

