teste: main
	valgrind --leak-check=full ./main

main:
	gcc -o ./main main.c

