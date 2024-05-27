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
        if(!t->params) {
            free(t);
            t=NULL;
        }
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
        (*t)->params[i]=NULL;
    }
    free((*t)->params);
    (*t)->params=NULL;
    free(*t);
    *t=NULL;
}

void liberaTokenLista(void* val) {
    token* t=(token*)val;
    liberaToken(&t);
}

int defineParamInteger(token* t, int indice, int val) {
    if(!t)
        return -1;
    else if(indice>=t->paramLen)
        return 1;
    t->params[indice]=(paramToken*)malloc(sizeof(paramToken));
    if(!t->params[indice])
        return 2;
    t->params[indice]->type=integer;
    t->params[indice]->val.integer=val;
    return 0;
}

int defineParamString(token* t, int indice, char* val) {
    if(!t)
        return -1;
    else if(indice>=t->paramLen)
        return 1;
    t->params[indice]=(paramToken*)malloc(sizeof(paramToken));
    if(!t->params[indice])
        return 2;
    t->params[indice]->type=string;
    t->params[indice]->val.string=(char*)malloc((strlen(val)+1)*sizeof(char));
    if(!t->params[indice]->val.string) {
        free(t->params[indice]);
        t->params[indice]=NULL;
        return 2;
    }
    strcpy(t->params[indice]->val.string, val);
    return 0;
}

int defineParamStatus(token* t, int indice, statusCodigo val) {
    if(!t)
        return -1;
    else if(indice>=t->paramLen)
        return 1;
    t->params[indice]=(paramToken*)malloc(sizeof(paramToken));
    if(!t->params[indice])
        return 2;
    t->params[indice]->type=status;
    t->params[indice]->val.status=val;
    return 0;
}

lista* tokenizaArquivo(FILE* arquivo) {
    if(!arquivo)
        return NULL;

    int paramQnt=0, currentParam=0, err=0, errT=0, linhaNum, colunaNum;
    char linha[MAX_CHAR_LIDOS_POR_LINHA];
    char* palavra=NULL;
    lista* l=novaLista();
    token* t=NULL;

    //int err=0, linhaNum=-1, colunaNum=-1;
    //char linha[MAX_CHAR_LIDOS_POR_LINHA];
    //char* palavra=NULL;
    //token* t=NULL;
    //lista* l=novaLista();

    if(!l)
        err=4;

    linhaNum=0;
    while(!err&&fgets(linha, MAX_CHAR_LIDOS_POR_LINHA, arquivo)) {
        linhaNum++;
        if(!strncmp(linha, "//", 2))
            continue;
        chomp(linha);
        lowerString(linha);
        palavra=strtok(linha," ");
        colunaNum=0;
        while(!err&&palavra) {
            colunaNum++;
            if(!strcmp(palavra, "iniciar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(iniciar);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "encerrar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(encerrar);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "inseriravl")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(inserirAvl);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "terminaravl")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(terminarAvl);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "listaravl")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(listarAvl);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "alterarheap")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(alterarHeap);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "removerheap")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(removerHeap);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "listarheap")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(listarHeap);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "bloquearhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(bloquearHash);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "desbloquearhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(desbloquearHash);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "executar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(executar);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "removerhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(removerHash);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "listarhash")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(listarHash);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else if(!strcmp(palavra, "terminar")) {
                if(paramQnt) {
                    err=1;
                    continue;
                }
                t=criarToken(terminar);
                if(!t) {
                    err=4;
                    continue;
                }
                paramQnt=t->paramLen;
            } else {
                if(!paramQnt) {
                    printf("Palavra magica: \"%s\"\n", palavra);
                    err=2;
                    continue;
                }
                currentParam=t->paramLen-paramQnt;
                errT=0;
                if(isNumeric(palavra))
                    errT=defineParamInteger(t, currentParam, atoi(palavra));
                else if(isAlphabetic(palavra)) {
                    if(!strcmp(palavra, "pronto"))
                        errT=defineParamStatus(t, currentParam, pronto);
                    else if(!strcmp(palavra, "execucao"))
                        errT=defineParamStatus(t, currentParam, execucao);
                    else if(!strcmp(palavra, "bloqueado"))
                        errT=defineParamStatus(t, currentParam, bloqueado);
                    else
                        errT=defineParamString(t, currentParam, palavra);
                } else {
                    err=3;
                    continue;
                }
                switch(errT) {
                    case -1:
                        err=-1;
                        break;
                    case 1:
                        err=-1;
                        break;
                    case 2:
                        err=4;
                        break;
                    default:
                        err=errT;
                        break;
                }
                if(err)
                    continue;
                paramQnt--;
            }
            if(!paramQnt) {
                insereUltimo(l, (void*)t);
                t=NULL;
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
            case 4:
                printf("Nao ha memoria suficiente para alocacao\n");
                break;
            default:
                printf("Erro desconhecido, codigo %d\n", err);
                break;
        }
        if(t) {
            printf("Ultimo comando registrado: %d\n", t->cmd);
            liberaToken(&t);
        } else if(l)
            printf("Ultimo comando registrado: %d\n", ((token*)pegaUltimo(l))->cmd);
        liberaListaFunc(&l, liberaTokenLista);
    }
    return l;
}

int validaListaToken(lista* l) {
    if(!l)
        return -1;
    int err=0;
    no* n=NULL;
    token* t=NULL;
    char cmd[20];
    for(n=l->raiz;!err&&n;n=n->prox) {
        t=(token*)n->val;
        switch(t->cmd) {
            case iniciar://t->paramLen=0
            case encerrar:
            case listarAvl:
            case listarHeap:
            case removerHeap:
                break;
            case terminarAvl://t->paramLen=1
                strcpy(cmd, "TerminarAVL");
            case bloquearHash:
                strcpy(cmd, "BloquearHash");
            case desbloquearHash:
                strcpy(cmd, "DesbloquearHash");
            case executar:
                strcpy(cmd, "Executar");
            case removerHash:
                strcpy(cmd, "RemoverHash");
                if(t->params[0]->type!=integer)
                    err=1;
                break;
            case listarHash:
                strcpy(cmd, "ListarHash");
                if(t->params[0]->type!=status)
                    err=1;
                break;
            case alterarHeap://t->paramLen=2
                strcpy(cmd, "AlterarHeap");
                if(t->params[0]->type!=integer)
                    err=1;
                else if(t->params[1]->type!=integer)
                    err=2;
                break;
            case terminar:
                strcpy(cmd, "Terminar");
                if(t->params[0]->type!=integer)
                    err=1;
                else if(t->params[1]->type!=string)
                    err=2;
                break;
            case inserirAvl://t->paramLen=4
                strcpy(cmd, "InserirAVL");
                if(t->params[0]->type!=integer)
                    err=1;
                else if(t->params[1]->type!=string)
                    err=2;
                else if(t->params[2]->type!=integer)
                    err=3;
                else if(t->params[3]->type!=status)
                    err=4;
                break;
        }
    }
    if(!err)
        printf("O comando %s recebeu o tipo incorreto em seu %d parametro\n", cmd, err);
    return err;
}

