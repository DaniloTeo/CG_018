all:
	gcc -o t3 -g t3.c -lm -lglut -lGLU -lGL
	./t3
run:
	./t3
valgrind:
	gcc -o t3 -g t3.c -lm -lglut -lGLU -lGL
	valgrind --tool=memcheck --leak-check=full ./t3
clean:
	rm t3