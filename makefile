bibliotecas := processos

mkfile_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
bibliotecas_o := $(patsubst %,%.o,$(bibliotecas))
bibliotecas_so := $(patsubst %,lib%.so,$(bibliotecas))

teste: main
	valgrind --leak-check=full ./$^

main: main.o $(bibliotecas_so)
	gcc -L$(mkfile_path) -Wl,-rpath=$(mkfile_path) -Wall -o ./main ./main.o $(patsubst %,-l%,$(bibliotecas))

main.o: main.c
	gcc -c -Wall -Werror main.c -o main.o

$(bibliotecas_o): %.o : %.c
	gcc -c -Wall -Werror -fpic $^ -o $@

$(bibliotecas_so): lib%.so : %.o
	gcc -shared $^ -o $@

clean:
	rm -f main main.o $(bibliotecas_o) $(bibliotecas_so)

