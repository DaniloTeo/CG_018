all:
	gcc -o t1 -g t1.c -lm -lglut -lGLU -lGL
	./t1
run:
	./t1
valgrind:
	gcc -o t1 -g t1.c -lm -lglut -lGLU -lGL
	valgrind --tool=memcheck --leak-check=full ./t1