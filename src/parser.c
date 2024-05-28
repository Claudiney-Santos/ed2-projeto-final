#include "parser.h"

#include "utils.h"
#include <string.h>

token* criarToken(comandosToken cmd) {
    int i;
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
        if(t->params) {
            for(i=0;i<t->paramLen;i++) {
                t->params[i]=(paramToken*)malloc(sizeof(paramToken));
                if(!t->params[i]) {
                    while(i--)
                        free(t->params[i]);
                    free(t->params);
                    free(t);
                    t=NULL;
                    break;
                }
            }
            switch(cmd) {
                case iniciar:
                case encerrar:
                case listarAvl:
                case listarHeap:
                case removerHeap:
                    break;
                case terminarAvl:
                case bloquearHash:
                case desbloquearHash:
                case executar:
                case removerHash:
                    t->params[0]->type=integer;
                    break;
                case listarHash:
                    t->params[0]->type=status;
                    break;
                case alterarHeap:
                    t->params[0]->type=integer;
                    t->params[1]->type=integer;
                    break;
                case terminar:
                    t->params[0]->type=integer;
                    t->params[1]->type=string;

                    t->params[1]->val.string=NULL;
                    break;
                case inserirAvl:
                    t->params[0]->type=integer;
                    t->params[1]->type=string;
                    t->params[2]->type=integer;
                    t->params[3]->type=status;

                    t->params[1]->val.string=NULL;
                    break;
            }
        } else {
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
        if((*t)->params[i]->type==string&&(*t)->params[i]->val.string)
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
    else if(t->params[indice]->type!=integer)
        return 3;
    t->params[indice]->val.integer=val;
    return 0;
}

int defineParamString(token* t, int indice, char* val) {
    if(!t)
        return -1;
    else if(indice>=t->paramLen)
        return 1;
    else if(t->params[indice]->type!=string)
        return 3;
    t->params[indice]->val.string=(char*)malloc((strlen(val)+1)*sizeof(char));
    if(!t->params[indice]->val.string)
        return 2;
    strcpy(t->params[indice]->val.string, val);
    return 0;
}

int defineParamStatus(token* t, int indice, statusCodigo val) {
    if(!t)
        return -1;
    else if(indice>=t->paramLen)
        return 1;
    else if(t->params[indice]->type!=status)
        return 3;
    t->params[indice]->val.status=val;
    return 0;
}

char* pegaTipoParamTokenType(paramTokenType t) {
    return t==integer ? "integer" : t==string ? "string" : t==status ? "status" : "indefinido";
}

lista* tokenizaArquivo(FILE* arquivo) {
    if(!arquivo)
        return NULL;

    int paramQnt=0, currentParam=0, err=0, errT=0, linhaNum, colunaNum;
    paramTokenType tipoEsperado, tipoRecebido;
    char linha[MAX_CHAR_LIDOS_POR_LINHA];
    char* palavra=NULL;
    lista* l=novaLista();
    token* t=NULL;

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
                    //printf("Palavra magica: \"%s\"\n", palavra);
                    err=2;
                    continue;
                }
                currentParam=t->paramLen-paramQnt;
                errT=0;
                tipoEsperado=t->params[currentParam]->type;
                tipoRecebido=status;
                if(isNumeric(palavra)) {
                    errT=defineParamInteger(t, currentParam, atoi(palavra));
                    tipoRecebido=integer;
                } else if(isAlphabetic(palavra)) {
                    if(!strcmp(palavra, "pronto"))
                        errT=defineParamStatus(t, currentParam, pronto);
                    else if(!strcmp(palavra, "execucao"))
                        errT=defineParamStatus(t, currentParam, execucao);
                    else if(!strcmp(palavra, "bloqueado"))
                        errT=defineParamStatus(t, currentParam, bloqueado);
                    else {
                        errT=defineParamString(t, currentParam, palavra);
                        tipoRecebido=string;
                    }
                } else {
                    err=3;
                    tipoRecebido=string;
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
                    case 3:
                        err=5;
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
            case 5:
                printf("Parametro com tipo incorreto\nTipo esperado: %s\nTipo recebido: %s\n", pegaTipoParamTokenType(tipoEsperado), pegaTipoParamTokenType(tipoRecebido));
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

