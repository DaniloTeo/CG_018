#define _USE_MATH_DEFINES // for C

#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

int width = 800;
int height = 800;

const GLfloat abRadius = 2.0f;
const int abStacks = 20;
const int abSlices = 20;

const GLfloat cefaloRadius = 1.0f;
const int cefaloStacks = 20;
const int cefaloSlices = 20;




typedef struct vertex{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Vertex;

Vertex cefaloCenter;
Vertex abCenter;



//PROTOTIPOS DAS FUNCOES DE VISUALIZACAO/////////////////////////////////////////////////////////////////////////////
//DESENHO DA ARANHA
void drawAbdomen(GLfloat center_x, GLfloat center_y, GLfloat center_z);
void drawCefaloTorax(GLfloat center_x, GLfloat center_y, GLfloat center_z);
void drawUpperLeftLeg(int linesQuantity) ;
void drawUpperRightLeg(int linesQuantity) ;
void drawBottomLeftLeg(int linesQuantity) ;
void drawBottomRightLeg(int linesQuantity) ;
void drawBottomMiddleLeftLeg(int linesQuantity) ;
void drawBottomMiddleRightLeg(int linesQuantity) ;
void drawUpperMiddleLeftLeg(int linesQuantity) ;
void drawUpperMiddleRightLeg(int linesQuantity) ;
void drawLegs(int linesQuantity) ;
void drawSpider();

//DESENHO DO GRID - CODIGOS FORNECIDOS PELO MONITOR DIEGO CINTRA
/**
 * @desc Draw grid plane - adapted from Song Ho Ahn matrix projection program (http://www.songho.ca/opengl/gl_transform.html).
 * @param {float} size Defines grid size.
 * @param {float} step Specifies grid steps, similar to tiles on the floor.
 */
void drawGrid(float size, float step);

//DESENHO DOS EIXOS - CODIGOS FORNECIDOS PELO MONITOR DIEGO CINTRA
/**
 * @desc Desenha eixos de um sistema de coordenadas.
 * @param {float*} basePoint Ponto de origem de um sistema de coordenadas.
 * @param {float*} i Primeiro versor.
 * @param {float*} j Segundo versor.
 * @param {float*} k Terceiro versor.
 */
void drawAxes(Vertex *basePoint, Vertex *i, Vertex *j, Vertex *k);

/**
 * @desc Desenha as coordenadas globais.
 */
void drawWCAxes();

//DEFINICAO DAS VIEWPORTS
void displayCallback();
void reshapeCallback(int w, int h);


//FUNCAO DE INICIALIZACAO DA ESTRUTURA VERTEX
void initializeVertex(Vertex *v, GLfloat x, GLfloat y, GLfloat z);

/////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Trabalho 2");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	
	glutMainLoop();
	return 0;
}

/////////////////////FUNCOES DE DESENHO E DE DEFINICAO DE VIEWPORTS//////////////////////////////////


void drawGrid(float size, float step){
    float i;
    glColor3f(0.3, 0.3, 0.3);
    for(i = 0; i < size; i = i + step)    {
        /** Translate to point in x-axis to draw line parallel to z-axis */
        glTranslatef(i, 0.0, 0.0);
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, size);
        glEnd();
        glTranslatef(-i, 0.0, 0.0);
        /** Translate to point in z-axis to draw line parallel to x-axis */
        glTranslatef(0.0, 0.0, i);
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(size, 0.0, 0.0);
        glEnd();
        glTranslatef(0.0, 0.0, -i);
    }
}



void drawAxes(Vertex *basePoint, Vertex *i, Vertex *j, Vertex *k){
  float currentColor[4];
  /** Armazena cor atual */
  glGetFloatv(GL_CURRENT_COLOR, currentColor);	
  /** Desenha versores */
  glColor3f(1.0, 0.0, 0.0); //x-axis
  glBegin(GL_LINES);
  	glVertex3f(basePoint->x, basePoint->y, basePoint->z);
  	glVertex3f(i->x, i->y, i->z);
  glEnd();
  glColor3f(0.0, 1.0, 0.0); //y-axis
  glBegin(GL_LINES);
  	glVertex3f(basePoint->x, basePoint->y, basePoint->z);
  	glVertex3f(j->x, j->y, j->z);
  glEnd();
  glColor3f(0.0, 0.0, 1.0); //z-axis
  glBegin(GL_LINES);
    glVertex3f(basePoint->x, basePoint->y, basePoint->z);
    glVertex3f(k->x, k->y, k->z);
  glEnd();
  /** Retorna para cor anterior */
  glColor3f(currentColor[0], currentColor[1], currentColor[2]);
}

void initializeVertex(Vertex *v, GLfloat x, GLfloat y, GLfloat z){
	v->x = x;
	v->y = y;
	v->z = z;
}



void drawWCAxes(){
	Vertex basePoint, i, j, k;
	initializeVertex(&basePoint, 0.0f, 0.0f, 0.0f);
	initializeVertex(&i, 5.0f, 0.0f, 0.0f);
	initializeVertex(&j, 0.0f, 5.0f, 0.0f);
	initializeVertex(&k, 0.0f, 0.0f, 5.0f);
	
	drawAxes(&basePoint, &i, &j, &k);
}


void drawAbdomen(GLfloat center_x, GLfloat center_y, GLfloat center_z){
	glPushMatrix();
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(center_x, center_y, center_z); 
		glutSolidSphere(abRadius, abSlices, abStacks); //esfera com centro (0,0,0)
	
	glPopMatrix();
}

void drawCefaloTorax(GLfloat center_x, GLfloat center_y, GLfloat center_z){
	glPushMatrix();
		
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(center_x, center_y, center_z); 
		glutSolidSphere(cefaloRadius, cefaloSlices, cefaloStacks); //esfera com centro (0,0,0)
	
	glPopMatrix();
}


void drawUpperLeftLeg(int linesQuantity) {
	//PERNA ESQ
	Vertex upperLeftBaseLeg;
	Vertex upperLeftCotLeg;
	Vertex upperLeftExtLeg;


	//Leg base
	upperLeftBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((45 * 2 * M_PI) / linesQuantity));
	upperLeftBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((45 * 2 * M_PI) / linesQuantity));
	upperLeftBaseLeg.z = cefaloCenter.z;

	//Leg Cot
	upperLeftCotLeg.x = cefaloCenter.x + 2.0f;
	upperLeftCotLeg.y = cefaloCenter.y + 1.5f;
	upperLeftCotLeg.z = cefaloCenter.z + 2.0f;

	//Leg ext
	upperLeftExtLeg.x = cefaloCenter.x + 2.0f + 1.0f;
	upperLeftExtLeg.y = cefaloCenter.y - 1.0f;
	upperLeftExtLeg.z = cefaloCenter.z + 2.0f;	

	//glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(upperLeftBaseLeg.x, upperLeftBaseLeg.y, upperLeftBaseLeg.z); //base 
	glVertex3f(upperLeftCotLeg.x, upperLeftCotLeg.y, upperLeftCotLeg.z); //cotovelo
	glVertex3f(upperLeftExtLeg.x, upperLeftExtLeg.y, upperLeftExtLeg.z); //extremidade
	glEnd();
}


void drawUpperRightLeg(int linesQuantity) {
	//PERNA DIR
	Vertex upperRightBaseLeg;
	Vertex upperRightCotLeg;
	Vertex upperRightExtLeg;


	//Leg base
	upperRightBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((135 * 2 * M_PI) / linesQuantity));
	upperRightBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((135 * 2 * M_PI) / linesQuantity));
	upperRightBaseLeg.z = cefaloCenter.z;

	//Leg Cot
	upperRightCotLeg.x = cefaloCenter.x - 2.0f;
	upperRightCotLeg.y = cefaloCenter.y + 1.5f;
	upperRightCotLeg.z = cefaloCenter.z + 2.0f;

	//Leg ext
	upperRightExtLeg.x = cefaloCenter.x - (2.0f + 1.0f);
	upperRightExtLeg.y = cefaloCenter.y - 1.0f;
	upperRightExtLeg.z = cefaloCenter.z + 2.0f;	

	//glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(upperRightBaseLeg.x, upperRightBaseLeg.y, upperRightBaseLeg.z); //base 
	glVertex3f(upperRightCotLeg.x, upperRightCotLeg.y, upperRightCotLeg.z); //cotovelo
	glVertex3f(upperRightExtLeg.x, upperRightExtLeg.y, upperRightExtLeg.z); //extremidade
	glEnd();
}

void drawBottomLeftLeg(int linesQuantity) {
	//PERNA ESQ
	Vertex bottomLeftBaseLeg;
	Vertex bottomLeftCotLeg;
	Vertex bottomLeftExtLeg;


	//Leg base
	bottomLeftBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((45 * 2 * M_PI) / linesQuantity));
	bottomLeftBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((45 * 2 * M_PI) / linesQuantity));
	bottomLeftBaseLeg.z = cefaloCenter.z - 1.0f;

	//Leg Cot
	bottomLeftCotLeg.x = cefaloCenter.x + 2.0f;
	bottomLeftCotLeg.y = cefaloCenter.y + 1.5f;
	bottomLeftCotLeg.z = cefaloCenter.z - 2.0f;

	//Leg ext
	bottomLeftExtLeg.x = cefaloCenter.x + 2.0f + 1.0f;
	bottomLeftExtLeg.y = cefaloCenter.y - 1.0f;
	bottomLeftExtLeg.z = cefaloCenter.z - 2.0f;	

	//glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(bottomLeftBaseLeg.x, bottomLeftBaseLeg.y, bottomLeftBaseLeg.z); //base 
	glVertex3f(bottomLeftCotLeg.x, bottomLeftCotLeg.y, bottomLeftCotLeg.z); //cotovelo
	glVertex3f(bottomLeftExtLeg.x, bottomLeftExtLeg.y, bottomLeftExtLeg.z); //extremidade
	glEnd();
}

void drawBottomRightLeg(int linesQuantity) {
	//PERNA DIR
	Vertex bottomRightBaseLeg;
	Vertex bottomRightCotLeg;
	Vertex bottomRightExtLeg;


	//Leg base
	bottomRightBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((135 * 2 * M_PI) / linesQuantity));
	bottomRightBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((135 * 2 * M_PI) / linesQuantity));
	bottomRightBaseLeg.z = cefaloCenter.z - 1.0f;

	//Leg Cot
	bottomRightCotLeg.x = cefaloCenter.x - 2.0f;
	bottomRightCotLeg.y = cefaloCenter.y + 1.5f;
	bottomRightCotLeg.z = cefaloCenter.z - 2.0f;

	//Leg ext
	bottomRightExtLeg.x = cefaloCenter.x - (2.0f + 1.0f);
	bottomRightExtLeg.y = cefaloCenter.y - 1.0f;
	bottomRightExtLeg.z = cefaloCenter.z - 2.0f;	

	//glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(bottomRightBaseLeg.x, bottomRightBaseLeg.y, bottomRightBaseLeg.z); //base 
	glVertex3f(bottomRightCotLeg.x, bottomRightCotLeg.y, bottomRightCotLeg.z); //cotovelo
	glVertex3f(bottomRightExtLeg.x, bottomRightExtLeg.y, bottomRightExtLeg.z); //extremidade
	glEnd();
}

void drawBottomMiddleLeftLeg(int linesQuantity) {
	//PERNA ESQ
	Vertex bottomMiddleLeftBaseLeg;
	Vertex bottomMiddleLeftCotLeg;
	Vertex bottomMiddleLeftExtLeg;


	//Leg base
	bottomMiddleLeftBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((45 * 2 * M_PI) / linesQuantity));
	bottomMiddleLeftBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((45 * 2 * M_PI) / linesQuantity));
	bottomMiddleLeftBaseLeg.z = cefaloCenter.z - 0.5f;

	//Leg Cot
	bottomMiddleLeftCotLeg.x = cefaloCenter.x + 2.0f;
	bottomMiddleLeftCotLeg.y = cefaloCenter.y + 1.5f;
	bottomMiddleLeftCotLeg.z = cefaloCenter.z - 1.5f;

	//Leg ext
	bottomMiddleLeftExtLeg.x = cefaloCenter.x + (2.0f + 1.0f);
	bottomMiddleLeftExtLeg.y = cefaloCenter.y - 1.0f;
	bottomMiddleLeftExtLeg.z = cefaloCenter.z - 1.5f;	

	//glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(bottomMiddleLeftBaseLeg.x, bottomMiddleLeftBaseLeg.y, bottomMiddleLeftBaseLeg.z); //base 
	glVertex3f(bottomMiddleLeftCotLeg.x, bottomMiddleLeftCotLeg.y, bottomMiddleLeftCotLeg.z); //cotovelo
	glVertex3f(bottomMiddleLeftExtLeg.x, bottomMiddleLeftExtLeg.y, bottomMiddleLeftExtLeg.z); //extremidade
	glEnd();
}

void drawBottomMiddleRightLeg(int linesQuantity) {
	//PERNA DIR
	Vertex bottomMiddleRightBaseLeg;
	Vertex bottomMiddleRightCotLeg;
	Vertex bottomMiddleRightExtLeg;


	//Leg base
	bottomMiddleRightBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((135 * 2 * M_PI) / linesQuantity));
	bottomMiddleRightBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((135 * 2 * M_PI) / linesQuantity));
	bottomMiddleRightBaseLeg.z = cefaloCenter.z - 0.5f;

	//Leg Cot
	bottomMiddleRightCotLeg.x = cefaloCenter.x - 2.0f;
	bottomMiddleRightCotLeg.y = cefaloCenter.y + 1.5f;
	bottomMiddleRightCotLeg.z = cefaloCenter.z - 1.5f;

	//Leg ext
	bottomMiddleRightExtLeg.x = cefaloCenter.x - (2.0f + 1.0f);
	bottomMiddleRightExtLeg.y = cefaloCenter.y - 1.0f;
	bottomMiddleRightExtLeg.z = cefaloCenter.z - 1.5f;	

	//glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(bottomMiddleRightBaseLeg.x, bottomMiddleRightBaseLeg.y, bottomMiddleRightBaseLeg.z); //base 
	glVertex3f(bottomMiddleRightCotLeg.x, bottomMiddleRightCotLeg.y, bottomMiddleRightCotLeg.z); //cotovelo
	glVertex3f(bottomMiddleRightExtLeg.x, bottomMiddleRightExtLeg.y, bottomMiddleRightExtLeg.z); //extremidade
	glEnd();
}

void drawUpperMiddleLeftLeg(int linesQuantity) {
	//PERNA ESQ
	Vertex upperMiddleLeftBaseLeg;
	Vertex upperMiddleLeftCotLeg;
	Vertex upperMiddleLeftExtLeg;


	//Leg base
	upperMiddleLeftBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((45 * 2 * M_PI) / linesQuantity));
	upperMiddleLeftBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((45 * 2 * M_PI) / linesQuantity));
	upperMiddleLeftBaseLeg.z = cefaloCenter.z - 0.25f;

	//Leg Cot
	upperMiddleLeftCotLeg.x = cefaloCenter.x + 2.0f;
	upperMiddleLeftCotLeg.y = cefaloCenter.y + 1.5f;
	upperMiddleLeftCotLeg.z = cefaloCenter.z - 0.5f;

	//Leg ext
	upperMiddleLeftExtLeg.x = cefaloCenter.x + (2.0f + 1.0f);
	upperMiddleLeftExtLeg.y = cefaloCenter.y - 1.0f;
	upperMiddleLeftExtLeg.z = cefaloCenter.z - 0.5f;	

	//glLineWidth(3);
	glPointSize(5);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(upperMiddleLeftBaseLeg.x, upperMiddleLeftBaseLeg.y, upperMiddleLeftBaseLeg.z); //base 
	glVertex3f(upperMiddleLeftCotLeg.x, upperMiddleLeftCotLeg.y, upperMiddleLeftCotLeg.z); //cotovelo
	glVertex3f(upperMiddleLeftExtLeg.x, upperMiddleLeftExtLeg.y, upperMiddleLeftExtLeg.z); //extremidade
	glEnd();
}


void drawUpperMiddleRightLeg(int linesQuantity) {
	//PERNA DIR
	Vertex upperMiddleRightBaseLeg;
	Vertex upperMiddleRightCotLeg;
	Vertex upperMiddleRightExtLeg;


	//Leg base
	upperMiddleRightBaseLeg.x = cefaloCenter.x + (cefaloRadius * cos((135 * 2 * M_PI) / linesQuantity));
	upperMiddleRightBaseLeg.y = cefaloCenter.y + (cefaloRadius * sin((135 * 2 * M_PI) / linesQuantity));
	upperMiddleRightBaseLeg.z = cefaloCenter.z - 0.25f;

	//Leg Cot
	upperMiddleRightCotLeg.x = cefaloCenter.x - 2.0f;
	upperMiddleRightCotLeg.y = cefaloCenter.y + 1.5f;
	upperMiddleRightCotLeg.z = cefaloCenter.z - 0.5f;

	//Leg ext
	upperMiddleRightExtLeg.x = cefaloCenter.x - (2.0f + 1.0f);
	upperMiddleRightExtLeg.y = cefaloCenter.y - 1.0f;
	upperMiddleRightExtLeg.z = cefaloCenter.z - 0.5f;	

	//glLineWidth(3);
	glPointSize(5);
	glBegin(GL_LINE_STRIP);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(upperMiddleRightBaseLeg.x, upperMiddleRightBaseLeg.y, upperMiddleRightBaseLeg.z); //base 
	glVertex3f(upperMiddleRightCotLeg.x, upperMiddleRightCotLeg.y, upperMiddleRightCotLeg.z); //cotovelo
	glVertex3f(upperMiddleRightExtLeg.x, upperMiddleRightExtLeg.y, upperMiddleRightExtLeg.z); //extremidade
	glEnd();
}


void drawLegs(int linesQuantity) {

	//rotateInPlace(cefaloCenterX, cefaloCenterY, legRotation*legState); //comentei pq n sei oq  faz

	drawUpperRightLeg(linesQuantity);
	drawUpperLeftLeg(linesQuantity);
	drawBottomMiddleLeftLeg(linesQuantity);
	drawBottomMiddleRightLeg(linesQuantity);

	//rotateInPlace(cefaloCenterX, cefaloCenterY, -legRotation * legState); //comentei pq n sei oq faz

	//rotateInPlace(cefaloCenterX, cefaloCenterY, -legRotation * legState); //comentei pq n sei oq faz

	drawBottomLeftLeg(linesQuantity);
	drawBottomRightLeg(linesQuantity);
	drawUpperMiddleLeftLeg(linesQuantity);
	drawUpperMiddleRightLeg(linesQuantity);

//	rotateInPlace(cefaloCenterX, cefaloCenterY, legRotation*legState);

}

void drawSpider(){
	initializeVertex(&abCenter, 0.0f, 0.0f, 0.0f); //inicializacao de abCenter
	initializeVertex(&cefaloCenter, 0.0f, 0.0f, (abRadius + cefaloRadius)); //inicializacao de cefaloCenter


	drawAbdomen(abCenter.x, abCenter.y, abCenter.z);
	drawCefaloTorax(cefaloCenter.x, cefaloCenter.y, cefaloCenter.z);
	drawLegs(360);
	
}

void displayCallback(){
	/** Limpa a janela APENAS uma vez */
	glClear(GL_COLOR_BUFFER_BIT);

	
	
	//bottom left viewport - free view
	glLoadIdentity();
	gluLookAt(3.0, 2.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glViewport(0, 0, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();

	
	//bottom right viewport - z-axis view
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glViewport(width/2, 0, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();

	//u left viewport - y-axis view
	glLoadIdentity();
	gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glViewport(0, height/2, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();
	
	
	//top right viewport - x-axis view
	glLoadIdentity();
	gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glViewport(width/2, height/2, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();


	

	/** Dispara os comandos APENAS uma vez */
	glFlush();
}


void reshapeCallback(int w, int h){
	/** Atualiza os valores da janela */
	width = w;
	height = h;
	/** Define o volume de vista */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}