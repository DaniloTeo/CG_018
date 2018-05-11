all:
	gcc -o app -g ./src/*c -I./include -lm
run:
	./app
valgrind:
	valgrind --tool=memcheck --leak-check=full ./app < 01.in