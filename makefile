bibliotecas := processos

mkfile_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

src_dir := $(mkfile_path)src/
obj_dir := $(mkfile_path)obj/
build_dir := $(mkfile_path)

bibliotecas_o := $(patsubst %,$(obj_dir)%.o,$(bibliotecas))
bibliotecas_so := $(patsubst %,$(build_dir)lib%.so,$(bibliotecas))

main := $(build_dir)main
main_c := $(patsubst $(build_dir)%,$(src_dir)%.c,$(main))
main_o := $(patsubst $(build_dir)%,$(obj_dir)%.o,$(main))

.PHONY: teste all clean cleanobj cleanbuild

all: $(main)

teste: $(main)
	valgrind --leak-check=full $^

$(obj_dir):
	mkdir $@

$(main): $(main_o) $(bibliotecas_so)
	gcc -L$(build_dir) -Wl,-rpath=$(build_dir) -Wall -o $(main) $(main_o) $(patsubst %,-l%,$(bibliotecas))

$(main_o): $(main_c) $(obj_dir)
	gcc -c -Wall -Werror $(main_c) -o $(main_o)

$(bibliotecas_o): $(obj_dir)%.o : $(src_dir)%.c
	gcc -c -Wall -Werror -fpic $^ -o $@

$(bibliotecas_so): $(build_dir)lib%.so : $(obj_dir)%.o
	gcc -shared $^ -o $@

clean: cleanobj cleanbuild

cleanobj:
	rm -f $(main_o) $(bibliotecas_o)

cleanbuild:
	rm -f $(main) $(bibliotecas_so)
