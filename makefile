mkfile_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

teste: main
	valgrind --leak-check=full ./$^

main: main.o libprocessos.so
	gcc -L$(mkfile_path) -Wl,-rpath=$(mkfile_path) -Wall -o ./main ./main.o -lprocessos

main.o: main.c
	gcc -c -Wall -Werror main.c -o main.o

processos.o: processos.c
	gcc -c -Wall -Werror -fpic ./processos.c -o ./processos.o

libprocessos.so: processos.o
	gcc -shared ./processos.o -o libprocessos.so

clean:
	rm -f main main.o processos.o libprocessos.so

