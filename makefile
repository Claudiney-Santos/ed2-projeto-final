bibliotecas := utils lista processos parser avl hash heap #simulacao

mkfile_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

src_dir := $(mkfile_path)src/
obj_dir := $(mkfile_path)obj/
build_dir := $(mkfile_path)

bibliotecas_o := $(patsubst %,$(obj_dir)%.o,$(bibliotecas))
bibliotecas_so := $(patsubst %,$(build_dir)lib%.so,$(bibliotecas))

main_c := $(src_dir)main.c
main_o := $(patsubst $(src_dir)%.c,$(obj_dir)%.o,$(main_c))
main := $(patsubst $(src_dir)%.c,$(build_dir)%,$(main_c))

simulacao_c := $(src_dir)simulacao.c
simulacao_o := $(patsubst $(src_dir)%.c,$(obj_dir)%.o,$(simulacao_c))
simulacao_so := $(patsubst $(src_dir)%.c,$(build_dir)lib%.so,$(simulacao_c))

simulacao_dependencias := processos parser lista avl hash heap
simulacao_dependencias_o := $(patsubst %,$(obj_dir)%.o,$(simulacao_dependencias))
simulacao_dependencias_so := $(patsubst %,$(build_dir)lib%.so,$(simulacao_dependencias))

.PHONY: teste all clean cleanobj cleanbuild

all: $(main)

teste: $(main)
	valgrind -s --track-origins=yes --leak-check=full --show-leak-kinds=all $^

$(obj_dir) $(build_dir):
	mkdir $@

$(simulacao_o): $(simulacao_c)
	gcc -c -Wall -Werror -fpic $^ -o $@ -I$(build_dir)

$(simulacao_so): $(simulacao_o) $(simulacao_dependencias_so)
	gcc -L$(build_dir) -Wl,-rpath=$(build_dir) -shared -o $@ $(simulacao_o) $(patsubst %,-l%,$(simulacao_dependencias))

$(main): $(build_dir) $(main_o) $(bibliotecas_so) $(simulacao_so)
	gcc -L$(build_dir) -Wl,-rpath=$(build_dir) -Wall -o $(main) $(main_o) $(patsubst $(build_dir)lib%.so,-l%,$(bibliotecas_so) $(simulacao_so))

$(main_o): $(obj_dir) $(main_c)
	gcc -c -Wall -Werror $(main_c) -o $(main_o)

$(bibliotecas_o): $(obj_dir)%.o : $(src_dir)%.c
	gcc -c -Wall -Werror -fpic $^ -o $@

$(bibliotecas_so): $(build_dir)lib%.so : $(obj_dir)%.o
	gcc -shared $^ -o $@

clean: cleanobj cleanbuild

cleanobj:
	rm -f $(main_o) $(bibliotecas_o) $(simulacao_o)

cleanbuild:
	rm -f $(main) $(bibliotecas_so) $(simulacao_so)
