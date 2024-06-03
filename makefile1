bibliotecas := utils lista processos parser avl hash heap simulacao log

mkfile_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

src_dir := $(mkfile_path)src/
obj_dir := $(mkfile_path)obj/
build_dir := $(mkfile_path)

bibliotecas_o := $(patsubst %,$(obj_dir)%.o,$(bibliotecas))

biblioteca := biblioteca
biblioteca_a := $(patsubst %,$(build_dir)lib%.a,$(biblioteca))

main_c := $(src_dir)main.c
main_o := $(patsubst $(src_dir)%.c,$(obj_dir)%.o,$(main_c))
main := $(patsubst $(src_dir)%.c,$(build_dir)%,$(main_c))

.PHONY: teste all clean cleanobj cleanbuild

all: $(main)

teste: $(main)
	valgrind -s --track-origins=yes --leak-check=full --show-leak-kinds=all $^

$(obj_dir) $(build_dir):
	mkdir $@

$(main): $(build_dir) $(main_o) $(biblioteca_a)
	gcc $(main_o) -L$(build_dir) -l$(biblioteca) -o $@

$(main_o): $(obj_dir) $(main_c)
	gcc -c -Wall -Werror $(main_c) -o $(main_o)

$(bibliotecas_o): $(obj_dir)%.o : $(src_dir)%.c
	gcc -c -Wall -Werror $^ -o $@

$(biblioteca_a): $(bibliotecas_o)
	ar rcs $@ $^

clean: cleanobj cleanbuild

cleanobj:
	rm -f $(main_o) $(bibliotecas_o)

cleanbuild:
	rm -f $(main) $(biblioteca_a)

