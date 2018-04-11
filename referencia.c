/*
	Jorge Hideki Motokubo Halker	9292945
	Leonardo de Souza Lemes			8941126
	Marly da Cruz Claudio			⁠⁠⁠8936885
	Rodrigo Valim Maciel			⁠⁠⁠9278149
*/

#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*
	ao iniciar o programa, 3 objetos sao colocados em posicoes aleatorias da tela
	clique com o botao esquerdo ou direito do mouse em algum ponto da tela
	os 3 objetos comecarao a se mover em linha reta, passando pelo ponto escolhido
	ao sairem da tela, os objetos voltam para a posicao inicial e repetem o trajeto, como especificado no pdf
*/

// as coordenadas estao armazenadas da seguinte maneira, counter clockwise
/*
	triangulo:
			[0]		[1]
		[0]	0.0		0.0 - indica vertice1(0.0, 0.0)
		[1]	10.0	0.0 - indica vertice2(10.0, 0.0)
		[2]	5.0		10.0 - indica vertice3(5.0, 10.0)
*/

// os vetores triCenter e triDir possuem duas dimensoes, onde a primeira representa a coordenada x e a segunda representa a coordenada y
// analogamente para os outros vetores do quadrado e pentagono

// para melhor compreensao do codigo leia na ordem: main, getInitialPosition, mouse, tick e draw

// valores de lambda que definem a velocidade do triangulo, quadrado e do pentagono
GLfloat lambda[3] = {0.1f, 0.3f, 0.5f};

// coordenadas do triangulo
GLfloat tri[3][2];
// centro do triangulo
GLfloat triCenter[2];
// vetor diretor do triangulo
GLfloat triDir[2];

// coordenadas do quadrado
GLfloat qua[4][2];
// centro do quadrado
GLfloat quaCenter[2];
// vetor diretor do quadrado
GLfloat quaDir[2];

// coordenadas do pentagono
GLfloat pen[5][2];
// centro do pentagono
GLfloat penCenter[2];
// vetor diretor do pentagono
GLfloat penDir[2];

// coordenadas do mouse
GLfloat mouseX;
GLfloat mouseY;

char flag = 0; // indica se os objetos devem comecar a se mover
char count = 0; // explicado abaixo

void draw() {
	// carrega as matrizes de transformacao
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// define a cor de fundo
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// TRIANGULO VERMELHO
	// checa se o triangulo saiu da tela
	if ((triCenter[0] + lambda[0] * triDir[0] < -15.0f) || (triCenter[0] + lambda[0] * triDir[0] > 115.0f) || (triCenter[1] + lambda[0] * triDir[1] < -15.0f) || (triCenter[1] + lambda[0] * triDir[1] > 115.0f)) {
		// saiu da tela, reseta lambda para voltar a posicao inicial
		lambda[0] = 0.1f;
	}
	// coloca a matriz de transformacao na pilha (isto serve para aplicar a translacao apenas para este objeto e nao para os outros)
	glPushMatrix();
	if (flag) {
		// inicia o movimento
		glTranslatef(lambda[0] * triDir[0], lambda[0] * triDir[1], 0.0f);
	}
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
		// desenha o triangulo
		for (int i = 0; i < 3; i++) {
			glVertex2f(tri[i][0], tri[i][1]);
		}
	glEnd();
	glPopMatrix(); // tira a matriz de transformacao da pilha (os objetos que estao entre o push e o pop sofrem a transformacao)

	// QUADRADO VERDE
	// comentarios analogos aos do triangulo
	if ((quaCenter[0] + lambda[1] * quaDir[0] < -15.0f) || (quaCenter[0] + lambda[1] * quaDir[0] > 115.0f) || (quaCenter[1] + lambda[1] * quaDir[1] < -15.0f) || (quaCenter[1] + lambda[1] * quaDir[1] > 115.0f)) {
		lambda[1] = 0.3f;
	}
	glPushMatrix();
	if (flag) {
		glTranslatef(lambda[1] * quaDir[0], lambda[1] * quaDir[1], 0.0f);
	}
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glVertex2f(qua[i][0], qua[i][1]);
		}
	glEnd();
	glPopMatrix();

	// PENTAGONO AZUL
	// comentario analogos aos do triangulo
	if ((penCenter[0] + lambda[2] * penDir[0] < -15.0f) || (penCenter[0] + lambda[2] * penDir[0] > 115.0f) || (penCenter[1] + lambda[2] * penDir[1] < -15.0f) || (penCenter[1] + lambda[2] * penDir[1] > 115.0f)) {
		lambda[2] = 0.5f;
	}
	glPushMatrix();
	if (flag) {
		glTranslatef(lambda[2] * penDir[0], lambda[2] * penDir[1], 0.0f);
	}
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
		for (int i = 0; i < 5; i++) {
			glVertex2f(pen[i][0], pen[i][1]);
		}
	glEnd();
	glPopMatrix();

	if (flag) {
		// incrementa os lambdas se o movimento tiver iniciado
		lambda[0] += 0.1f;
		lambda[1] += 0.3f;
		lambda[2] += 0.5f;
	}

	glutSwapBuffers(); // troca os buffers (necessario pois estamos utilizando double buffer)
	glFlush();
}

// tick de tempo para redesenhar, 10ms
void tick(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, tick, 0);
}


// funcao para tratar eventos do mouse
void mouse(int button, int state, int x, int y) {	
	// detecta clique esquerdo e direito
	if ((button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) || (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)) {
		count++; // conta se estamos tratando de um primeiro clique ou um n clique
		lambda[0] = 0.1f;
		lambda[1] = 0.3f;
		lambda[2] = 0.5f;
		flag = 1;
		mouseX = (x / (double) glutGet(GLUT_WINDOW_WIDTH)) * 100.0f;
		mouseY = (1.0f - (y / (double) glutGet(GLUT_WINDOW_HEIGHT))) * 100.0f;

		// calcula o vetor diretor nao normalizado
		GLfloat t1 = mouseX - triCenter[0];
		GLfloat t2 = mouseY - triCenter[1];

		GLfloat q1 = mouseX - quaCenter[0];
		GLfloat q2 = mouseY - quaCenter[1];

		GLfloat p1 = mouseX - penCenter[0];
		GLfloat p2 = mouseY - penCenter[1];

		// normaliza o vetor diretor
		triDir[0] = t1 / (sqrt(pow(t1, 2) + pow(t2, 2)));
		triDir[1] = t2 / (sqrt(pow(t1, 2) + pow(t2, 2)));

		quaDir[0] = q1 / (sqrt(pow(q1, 2) + pow(q2, 2)));
		quaDir[1] = q2 / (sqrt(pow(q1, 2) + pow(q2, 2)));

		penDir[0] = p1 / (sqrt(pow(p1, 2) + pow(p2, 2)));
		penDir[1] = p2 / (sqrt(pow(p1, 2) + pow(p2, 2)));

		// checa se estamos tratando de um clique que nao seja o primeiro, caso seja verdade, finaliza a recursao do clique anterior
		if (count > 1) {
			count = 1;
		}
		else {
			glutTimerFunc(10, tick, 0);
		}
	}
}

// define as coordenadas iniciais dos 3 objetos
void getInitialPosition() {
	srand(time(NULL));

	// gera 3 pontos aleatorios e os utiliza como base para construir o triangulo, o quadrado e o pentagono
	// note que apesar de guardarmos o valor do ponto aleatorio na variavel que indica o centro
	// ele nao representa o centro do objeto ainda
	triCenter[0] = (GLfloat) (rand() % 101);
	triCenter[1] = (GLfloat) (rand() % 101);

	quaCenter[0] = (GLfloat) (rand() % 101);
	quaCenter[1] = (GLfloat) (rand() % 101);

	penCenter[0] = (GLfloat) (rand() % 101);
	penCenter[1] = (GLfloat) (rand() % 101);

	// constroi o triangulo com base no ponto aleatorio gerado
	tri[0][0] = triCenter[0] - 10.0f;
	tri[0][1] = triCenter[1] - 10.0f;
	tri[1][0] = triCenter[0] + 10.0f;
	tri[1][1] = triCenter[1] - 10.0f;
	tri[2][0] = triCenter[0];
	tri[2][1] = triCenter[1] + 10.0f;

	// calcula o ponto medio do triangulo
	triCenter[0] = 0.0f;
	triCenter[1] = 0.0f;
	for (int i = 0; i < 3; i++) {
		triCenter[0] += tri[i][0];
		triCenter[1] += tri[i][1];
	}
	triCenter[0] /= 3.0f;
	triCenter[1] /= 3.0f;

	// constroi o quadrado com base no ponto aleatorio gerado
	qua[0][0] = quaCenter[0] - 10.0f;
	qua[0][1] = quaCenter[1] - 10.0f;
	qua[1][0] = quaCenter[0] + 10.0f;
	qua[1][1] = quaCenter[1] - 10.0f;
	qua[2][0] = quaCenter[0] + 10.0f;
	qua[2][1] = quaCenter[1] + 10.0f;
	qua[3][0] = quaCenter[0] - 10.0f;
	qua[3][1] = quaCenter[1] + 10.0f;

	// calcula o ponto medio do quadrado
	quaCenter[0] = 0.0f;
	quaCenter[1] = 0.0f;
	for (int i = 0; i < 4; i++) {
		quaCenter[0] += qua[i][0];
		quaCenter[1] += qua[i][1];
	}
	quaCenter[0] /= 4.0f;
	quaCenter[1] /= 4.0f;

	// constroi o pentagono com base no ponto aleatorio gerado
	pen[0][0] = penCenter[0] - 5.0f;
	pen[0][1] = penCenter[1] - 10.0f;
	pen[1][0] = penCenter[0] + 5.0f;
	pen[1][1] = penCenter[1] - 10.0f;
	pen[2][0] = penCenter[0] + 10.0f;
	pen[2][1] = penCenter[1];
	pen[3][0] = penCenter[0];
	pen[3][1] = penCenter[1] + 10.0f;
	pen[4][0] = penCenter[0] - 10.0f;
	pen[4][1] = penCenter[1];

	// calcula o ponto medio do pentagono
	penCenter[0] = 0.0f;
	penCenter[1] = 0.0f;
	for (int i = 0; i < 5; i++) {
		penCenter[0] += pen[i][0];
		penCenter[1] += pen[i][1];
	}
	penCenter[0] /= 5.0f;
	penCenter[1] /= 5.0f;
}

int main(int argc, char *argv[]) {
	// inicializa o glut;
	glutInit(&argc, argv);

	// utilizando o buffer duplo
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// define as posicoes iniciais dos 3 objetos
	getInitialPosition();

	// define uma janela de 800x600 e a posiciona no inicio da tela do computador
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Trabalho 1");

	// define as funcoes de desenho na tela e de evento do mouse
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);

	// sistema de coordenadasf
	// 0 <= x <= 100
	// 0 <= y <= 100
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);

	// loop para a main
	glutMainLoop();
}