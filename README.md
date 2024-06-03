# ATIVIDADE DE PRÁTICA 2

# Observação:
Esse projeto foi programado com o Linux em mente. Ele foi criado e testado no Ubuntu (WSL).

## Pré-requisitos para a compilação
- `gcc`
- `make`

### O que é o `make`?
`make` é uma ferramenta criada para o automatização do processo de compilação de um projeto. Esse projeto a usa por conta do processo metódico relacionado com a compilação de múltiplos headers numa única biblioteca.
Desse modo, com o script `makefile` já feito, ele comprime as seguintes linhas de código:
```
mkdir obj/
gcc -c -Wall -Werror src/main.c -o obj/main.o
gcc -c -Wall -Werror src/utils.c -o obj/utils.o
gcc -c -Wall -Werror src/lista.c -o obj/lista.o
gcc -c -Wall -Werror src/processos.c -o obj/processos.o
gcc -c -Wall -Werror src/parser.c -o obj/parser.o
gcc -c -Wall -Werror src/avl.c -o obj/avl.o
gcc -c -Wall -Werror src/hash.c -o obj/hash.o
gcc -c -Wall -Werror src/heap.c -o obj/heap.o
gcc -c -Wall -Werror src/simulacao.c -o obj/simulacao.o
gcc -c -Wall -Werror src/log.c -o obj/log.o
ar rcs libbiblioteca.a obj/utils.o obj/lista.o obj/processos.o obj/parser.o obj/avl.o obj/hash.o obj/heap.o obj/simulacao.o obj/log.o
gcc obj/main.o -L. -lbiblioteca -o main
```
No seguinte comando:
```
make all
```

### Como instalar o `make`?
Os seguintes comandos funcionam tanto em Linux nativo, quanto WSL
Ubuntu/Debian:
    `sudo apt-get install make`
Fedora/RedHat/CentOS:
    `sudo dnf install make`
Fedora/RedHat/CentOS (legado):
    `sudo yum install make`

## Compilando
Com o terminal no diretório do código fonte, execute o comando:
```
make all
```
Esse comando irá o gerar arquivo `./main` o qual pode ser executado
