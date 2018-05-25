all:
	gcc -o t2 -g t2.c -lm -lglut -lGLU -lGL
	./t2
run:
	./t2
valgrind:
	gcc -o t2 -g t2.c -lm -lglut -lGLU -lGL
	valgrind --tool=memcheck --leak-check=full ./t2
clean:
	rm t2