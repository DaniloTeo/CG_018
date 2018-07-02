all:
	g++ -std=c++11 -o t3 -g t3.c -lm -lglut -lGLU -lGL `pkg-config --cflags --libs opencv`
	./t3
run:
	./t3
valgrind:
	g++ -std=c++11 -o t3 -g t3.c -lm -lglut -lGLU -lGL `pkg-config --cflags --libs opencv`
	valgrind --tool=memcheck --leak-check=full ./t3
clean:
	rm t3
