#define _USE_MATH_DEFINES // for C

#include <math.h>
#include <GL/glut.h>



//Prototipos de desenhar aranha
void draw();
void drawSpider();
void drawAbdomen(GLfloat abCenterX, GLfloat abCenterY, GLfloat abRadius, int linesQuantity);
void drawCefaloTorax(GLfloat cefaloRadius, GLfloat cefaloCenterX, GLfloat cefaloCenterY, int linesQuantity);
void drawEyes(GLfloat rightEyeX, GLfloat leftEyeX, GLfloat rightEyeY, GLfloat leftEyeY);
void drawLegs(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity);
void drawUpperLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity);
void drawUpperRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity);
void drawBottomLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity);
void drawBottomRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity);
void drawBottomMiddleLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity);
void drawBottomMiddleRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity);
void drawUpperMiddleLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity);
void drawUpperMiddleRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity);

GLfloat abCenterX = 10.0f, abCenterY = 50.0f;


// Estrutura de dados
typedef struct _Vector {
	double x, y, z;
} Vector;

// Constantes
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Variáveis
double radian;
double speed = 10;
double rot; // Angulo de Rotação

			// Vetores
Vector pos;
Vector vel;
Vector dir;
Vector dis;
Vector mouseCoords;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -100, 100);
	//gluOrtho2D(0, 400, 0, 400);

	// Desenhando
	glColor3f(1, 0, 0);
	glTranslated(pos.x, pos.y, 0);
	glRotatef(rot, 0, 0, 1);
	drawSpider();

	glFlush();
	glutSwapBuffers();
}

void mouse(int btn, int state, int x, int y) {
	mouseCoords.x = x - SCREEN_WIDTH / 2;
	mouseCoords.y = -y + SCREEN_HEIGHT / 2;
}

void move() {
	dis.x = mouseCoords.x - pos.x;
	dis.y = mouseCoords.y - pos.y;

	// Se a magnetude for menor que a velocidade, defina as mouseCoords
	if (sqrt(dis.y*dis.y + dis.x*dis.x) < speed) {
		pos.x = mouseCoords.x;
		pos.y = mouseCoords.y;
	}
	else {
		radian = atan2(dis.y, dis.x);
		pos.x += cos(radian) * speed;
		pos.y += sin(radian) * speed;
		rot = radian * 100 / 3.14;
	}

}

void update(int value) {
	move();
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Teste");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutTimerFunc(25, update, 0);
	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	init();
	glutMainLoop();
	return 0;
}

void drawSpider() {
	int linesQuantity = 360;

	//abCenterX and abCenterY are the Abdomen's center coordinates,
	//and will be used as reference on the other parts of the spider
	GLfloat abRadius = 5.0f;
	drawAbdomen(abCenterX, abCenterY, abRadius, linesQuantity);

	GLfloat	cefaloRadius = 3.0f;
	GLfloat cefaloCenterX = (abCenterX + abRadius) + cefaloRadius;
	GLfloat cefaloCenterY = abCenterY;
	drawCefaloTorax(cefaloRadius, cefaloCenterX, cefaloCenterY, linesQuantity);

	GLfloat rightEyeX = (cefaloCenterX + 2.0f), rightEyeY = (cefaloCenterY - 1.0f);
	GLfloat leftEyeX = (cefaloCenterX + 2.0f), leftEyeY = (cefaloCenterY + 1.0f);
	drawEyes(rightEyeX, leftEyeX, rightEyeY, leftEyeY);

	drawLegs(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);
}

void drawAbdomen(GLfloat abCenterX, GLfloat abCenterY, GLfloat abRadius, int linesQuantity) {
	int i;

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < linesQuantity; i++) {
		glVertex2f(abCenterX + (abRadius * cos((i * 2 * M_PI) / linesQuantity)),
			abCenterY + (abRadius * sin((i * 2 * M_PI) / linesQuantity)));
	}
	glEnd();
}

void drawCefaloTorax(GLfloat cefaloRadius, GLfloat cefaloCenterX, GLfloat cefaloCenterY, int linesQuantity) {
	int i;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i < linesQuantity; i++) {
		glVertex2f(cefaloCenterX + (cefaloRadius * cos((i * 2 * M_PI) / linesQuantity)), cefaloCenterY + (cefaloRadius * sin((i * 2 * M_PI) / linesQuantity)));
	}
	glEnd();
}

void drawEyes(GLfloat rightEyeX, GLfloat leftEyeX, GLfloat rightEyeY, GLfloat leftEyeY) {
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex2f(rightEyeX, rightEyeY);
	glVertex2f(leftEyeX, leftEyeY);
	glEnd();
}

void drawLegs(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity) {
	drawUpperLeftLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);
	drawUpperRightLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);

	drawBottomLeftLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);
	drawBottomRightLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);

	drawBottomMiddleLeftLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);
	drawBottomMiddleRightLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);

	drawUpperMiddleLeftLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);
	drawUpperMiddleRightLeg(cefaloCenterX, cefaloCenterY, cefaloRadius, linesQuantity);
}

void drawUpperLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity) {
	//PERNA ESQ

	//Leg base
	GLfloat upperLeftBaseLegX = cefaloCenterX + (cefaloRadius * cos((60 * 2 * M_PI) / linesQuantity));
	GLfloat upperLeftBaseLegY = cefaloCenterY + (cefaloRadius * sin((60 * 2 * M_PI) / linesQuantity));

	//Leg Cot
	GLfloat	upperLeftCotLegX = cefaloCenterX + 10.0f;
	GLfloat upperLeftCotLegY = cefaloCenterY + 5.0f;

	//Leg ext
	GLfloat	upperLeftExtLegX = cefaloCenterX + 10.0f + 3.0f;
	GLfloat upperLeftExtLegY = cefaloCenterY + 5.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(upperLeftBaseLegX, upperLeftBaseLegY); //base 
	glVertex2f(upperLeftCotLegX, upperLeftCotLegY); //cotovelo
	glVertex2f(upperLeftExtLegX, upperLeftExtLegY); //extremidade
	glEnd();

}

void drawUpperRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity) {
	//Leg Base
	GLfloat upperRightLegBaseX = cefaloCenterX + (cefaloRadius * cos((300 * 2 * M_PI) / linesQuantity));
	GLfloat upperRightLegBaseY = cefaloCenterY + (cefaloRadius * sin((300 * 2 * M_PI) / linesQuantity));

	//Leg Cot
	GLfloat	upperRightLegCotX = cefaloCenterX + 10.0f;
	GLfloat upperRightLegCotY = cefaloCenterY - 5.0f;

	//Leg Ext
	GLfloat	upperRightLegExtX = cefaloCenterX + 10.0f + 3.0f;
	GLfloat upperRightLegExtY = cefaloCenterY - 5.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(upperRightLegBaseX, upperRightLegBaseY); //base 
	glVertex2f(upperRightLegCotX, upperRightLegCotY); //cotovelo
	glVertex2f(upperRightLegExtX, upperRightLegExtY); //extremidade
	glEnd();
}

void drawBottomLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity) {
	//PERNA ESQ
	GLfloat	bottomLeftBaseLegX = cefaloCenterX + (cefaloRadius * cos((120 * 2 * M_PI) / linesQuantity));
	GLfloat bottomLeftBaseLegY = cefaloCenterY + (cefaloRadius * sin((120 * 2 * M_PI) / linesQuantity));

	GLfloat	bottomLeftCotLegX = cefaloCenterX - 3.0f;
	GLfloat bottomLeftCotLegY = cefaloCenterY + 5.0f;

	GLfloat	bottomLeftExtLegX = cefaloCenterX - 10.0f - 3.0f;
	GLfloat bottomLeftExtLegY = cefaloCenterY + 6.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(bottomLeftBaseLegX, bottomLeftBaseLegY); //base 
	glVertex2f(bottomLeftCotLegX, bottomLeftCotLegY); //cotovelo
	glVertex2f(bottomLeftExtLegX, bottomLeftExtLegY); //extremidade
	glEnd();
}

void drawBottomRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, int linesQuantity) {
	//PERNA DIR
	GLfloat	bottomRightBaseLegX = cefaloCenterX + (cefaloRadius * cos((240 * 2 * M_PI) / linesQuantity));
	GLfloat bottomRightBaseLegY = cefaloCenterY + (cefaloRadius * sin((240 * 2 * M_PI) / linesQuantity));

	GLfloat	bottomRightCotLegX = cefaloCenterX - 3.0f;
	GLfloat bottomRightCotLegY = cefaloCenterY - 5.0f;

	GLfloat	bottomRightExtLegX = cefaloCenterX - 10.0f - 3.0f;
	GLfloat bottomRightExtLegY = cefaloCenterY - 6.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(bottomRightBaseLegX, bottomRightBaseLegY); //base 
	glVertex2f(bottomRightCotLegX, bottomRightCotLegY); //cotovelo
	glVertex2f(bottomRightExtLegX, bottomRightExtLegY); //extremidade
	glEnd();
}

void drawBottomMiddleLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity) {
	//PERNA ESQ
	GLfloat	bottomMiddleLeftBaseLegX = cefaloCenterX + (cefaloRadius * cos((100 * 2 * M_PI) / linesQuantity));
	GLfloat bottomMiddleLeftBaseLegY = cefaloCenterY + (cefaloRadius * sin((100 * 2 * M_PI) / linesQuantity));

	GLfloat	bottomMiddleLeftCotLegX = cefaloCenterX - 1.0f;
	GLfloat bottomMiddleLeftCotLegY = cefaloCenterY + 6.0f;

	GLfloat	bottomMiddleLeftExtLegX = cefaloCenterX - 10.0f;
	GLfloat bottomMiddleLeftExtLegY = cefaloCenterY + 7.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(bottomMiddleLeftBaseLegX, bottomMiddleLeftBaseLegY); //base 
	glVertex2f(bottomMiddleLeftCotLegX, bottomMiddleLeftCotLegY); //cotovelo
	glVertex2f(bottomMiddleLeftExtLegX, bottomMiddleLeftExtLegY); //extremidade
	glEnd();
}

void drawBottomMiddleRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity) {
	//PERNA DIR
	GLfloat	bottomMiddleRightBaseLegX = cefaloCenterX + (cefaloRadius * cos((260 * 2 * M_PI) / linesQuantity));
	GLfloat bottomMiddleRightBaseLegY = cefaloCenterY + (cefaloRadius * sin((260 * 2 * M_PI) / linesQuantity));

	GLfloat	bottomMiddleRightCotLegX = cefaloCenterX - 1.0f;
	GLfloat bottomMiddleRightCotLegY = cefaloCenterY - 6.0f;

	GLfloat	bottomMiddleRightExtLegX = cefaloCenterX - 10.0f;
	GLfloat bottomMiddleRightExtLegY = cefaloCenterY - 7.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(bottomMiddleRightBaseLegX, bottomMiddleRightBaseLegY); //base 
	glVertex2f(bottomMiddleRightCotLegX, bottomMiddleRightCotLegY); //cotovelo
	glVertex2f(bottomMiddleRightExtLegX, bottomMiddleRightExtLegY); //extremidade
	glEnd();
}

void drawUpperMiddleLeftLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity) {
	//PERNA ESQ
	GLfloat	upperMiddleLeftBaseLegX = cefaloCenterX + (cefaloRadius * cos((80 * 2 * M_PI) / linesQuantity));
	GLfloat upperMiddleLeftBaseLegY = cefaloCenterY + (cefaloRadius * sin((80 * 2 * M_PI) / linesQuantity));

	GLfloat	upperMiddleLeftCotLegX = cefaloCenterX;
	GLfloat upperMiddleLeftCotLegY = cefaloCenterY + 7.0f;

	GLfloat	upperMiddleLeftExtLegX = cefaloCenterX - 10.0f;
	GLfloat upperMiddleLeftExtLegY = cefaloCenterY + 8.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(upperMiddleLeftBaseLegX, upperMiddleLeftBaseLegY); //base 
	glVertex2f(upperMiddleLeftCotLegX, upperMiddleLeftCotLegY); //cotovelo
	glVertex2f(upperMiddleLeftExtLegX, upperMiddleLeftExtLegY); //extremidade
	glEnd();
}

void drawUpperMiddleRightLeg(GLfloat cefaloCenterX, GLfloat cefaloCenterY, GLfloat cefaloRadius, GLfloat linesQuantity) {
	//PERNA DIR
	GLfloat	upperMiddleRightBaseLegX = cefaloCenterX + (cefaloRadius * cos((280 * 2 * M_PI) / linesQuantity));
	GLfloat upperMiddleRightBaseLegY = cefaloCenterY + (cefaloRadius * sin((280 * 2 * M_PI) / linesQuantity));

	GLfloat	upperMiddleRightCotLegX = cefaloCenterX;
	GLfloat upperMiddleRightCotLegY = cefaloCenterY - 7.0f;

	GLfloat	upperMiddleRightExtLegX = cefaloCenterX - 10.0f;
	GLfloat upperMiddleRightExtLegY = cefaloCenterY - 8.0f;

	glBegin(GL_LINE_STRIP);
	glVertex2f(upperMiddleRightBaseLegX, upperMiddleRightBaseLegY); //base 
	glVertex2f(upperMiddleRightCotLegX, upperMiddleRightCotLegY); //cotovelo
	glVertex2f(upperMiddleRightExtLegX, upperMiddleRightExtLegY); //extremidade
	glEnd();
}