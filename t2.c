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

const GLfloat STEP_SIZE = 0.5f;


typedef struct vertex{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Vertex;

/**
 * funcao para ajudar a fazer vertex mais rapido
 */
Vertex* newVertex(GLfloat x, GLfloat y, GLfloat z) {
    Vertex* v = (Vertex*) malloc (1 * sizeof(Vertex));
    v -> x = x;
    v -> y = y;
    v -> z = z;
    return v;
}
/**
 * funcao para somar vertexes mais rapido
 * v recebe a soma de a e b
 */
void addVertexesv(Vertex* v, Vertex* a, Vertex* b) {
    v -> x = a -> x + b -> x;
    v -> y = a -> y + b -> y;
    v -> z = a -> z + b -> z;
}

/**
 * funcao para somar vertexes mais rapido
 * v recebe a soma de a e outro vertex que corresponde a (x, y, z)
 */
void addVertexes(Vertex* v, Vertex* a, GLfloat x, GLfloat y, GLfloat z) {
    v -> x = a -> x + x;
    v -> y = a -> y + y;
    v -> z = a -> z + z;
}

/**
 * funcao para multiplicar vertexes por escalares mais rapido
 * v recebe vertex a multiplicado pelo escalar b
 */
void mulVertex(Vertex* v, Vertex* a, int b) {
    v -> x = a -> x * b;
    v -> y = a -> y * b;
    v -> z = a -> z * b;
}

Vertex cefaloCenter;
Vertex abCenter;
Vertex* spiderCenter;
Vertex* spiderFrontDir;
Vertex* origin;
GLint ACTIVE_KEY;



//PROTOTIPOS DAS FUNCOES DE VISUALIZACAO/////////////////////////////////////////////////////////////////////////////
//DESENHO DA ARANHA
void drawAbdomen(GLfloat center_x, GLfloat center_y, GLfloat center_z);
void drawCefaloTorax(GLfloat center_x, GLfloat center_y, GLfloat center_z);
void drawLegs();
void drawSpider();
void handle_SpecialFunc(GLint key, GLint x, GLint y);
void update(int value);

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
	
	initializeVertex(&abCenter, 0.0f, 0.0f, 0.0f); //inicializacao de abCenter
	initializeVertex(&cefaloCenter, 0.0f, 0.0f, (abRadius + cefaloRadius)); //inicializacao de cefaloCenter
	spiderCenter = newVertex(0.0f, 0.0f, 0.0f);
	spiderFrontDir = newVertex(0.0f, 0.0f, 1.0f);
	origin = newVertex(0.0f, 0.0f, 0.0f);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Trabalho 2");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	glutSpecialFunc(handle_SpecialFunc);
	glutTimerFunc(25, update, 0);

	
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

void drawLeg(Vertex* d) {
	Vertex v; // vertex to draw lines
	Vertex i; // vertex that represent the vector to increment v

	glBegin(GL_LINE_STRIP);
	
		glColor3f(0.0f, 1.0f, 0.0f);

		mulVertex(&i, d, cefaloRadius);
		addVertexesv(&v, origin, &i);
		glVertex3f(v.x, v.y, v.z);

		addVertexes(&i, d, 0.0f, 1.0f, 0.0f);
		mulVertex(&i, &i, cefaloRadius * 1.2);
		addVertexesv(&v, &v, &i);
		glVertex3f(v.x, v.y, v.z);

		// mulVertex(&i, d, cefaloRadius * 2.0);
		// addVertexesv(&v, &v, &i);
		// glVertex3f(v.x, v.y, v.z);

		addVertexes(&i, d, 0.0f, -1.0f, 0.0f);
		mulVertex(&i, &i, cefaloRadius * 2.0);
		addVertexesv(&v, &v, &i);
		glVertex3f(v.x, v.y, v.z);	

	glEnd();
	
}


void drawLegs() {


	glTranslatef(spiderCenter->x + cefaloCenter.x, spiderCenter->y + cefaloCenter.y, spiderCenter->z + cefaloCenter.z);

	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);

	glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawLeg(spiderFrontDir);

	glRotatef(135.0f, 0.0f, 1.0f, 0.0f);
	
	glTranslatef(-(spiderCenter->x + cefaloCenter.x), -(spiderCenter->y + cefaloCenter.y), -(spiderCenter->z + cefaloCenter.z));
}

void drawSpider(){
	
	drawAbdomen(spiderCenter -> x + abCenter.x,
				spiderCenter -> y + abCenter.y,
				spiderCenter -> z + abCenter.z);

	drawCefaloTorax(spiderCenter -> x + cefaloCenter.x,
					spiderCenter -> y + cefaloCenter.y,
					spiderCenter -> z + cefaloCenter.z);
	
	drawLegs();
	
}

void displayCallback(){
	/** Limpa a janela APENAS uma vez */
	glClear(GL_COLOR_BUFFER_BIT);

	
	
	//bottom left viewport - free view
	glLoadIdentity();
	gluLookAt(spiderCenter -> x + 3.0, spiderCenter -> y + 2.0, spiderCenter -> z + 10.0,
				spiderCenter -> x, spiderCenter -> y, spiderCenter -> z,
				0.0, 1.0, 0.0);
	glViewport(0, 0, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();

	
	//bottom right viewport - z-axis view
	glLoadIdentity();
	gluLookAt(spiderCenter -> x + 0.0, spiderCenter -> y + 0.0, spiderCenter -> z + 10.0,
				spiderCenter -> x, spiderCenter -> y, spiderCenter -> z,
				0.0, 1.0, 0.0);
	glViewport(width/2, 0, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();

	//u left viewport - y-axis view
	glLoadIdentity();
	gluLookAt(spiderCenter -> x + 0.0, spiderCenter -> y + 10.0, spiderCenter -> z + 0.0,
				spiderCenter -> x, spiderCenter -> y, spiderCenter -> z,
				0.0, 0.0, 1.0); // up vector becomes k
	glViewport(0, height/2, width/2, height/2);
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();
	
	
	//top right viewport - x-axis view
	glLoadIdentity();
	gluLookAt(spiderCenter -> x + 10.0, spiderCenter -> y + 0.0, spiderCenter -> z + 0.0,
				spiderCenter -> x, spiderCenter -> y, spiderCenter -> z,
				0.0, 1.0, 0.0);
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

void update(int value) {

	switch (ACTIVE_KEY) {
		case GLUT_KEY_LEFT:
			addVertexes(spiderCenter, spiderCenter, STEP_SIZE, 0.0f, 0.0f);
			break;
		case GLUT_KEY_UP:
			addVertexes(spiderCenter, spiderCenter, 0.0f, 0.0f, STEP_SIZE);
			break;
		case GLUT_KEY_RIGHT:
			addVertexes(spiderCenter, spiderCenter, -STEP_SIZE, 0.0f, 0.0f);
			break;
		case GLUT_KEY_DOWN:
			addVertexes(spiderCenter, spiderCenter, 0.0f, 0.0f, -STEP_SIZE);
			break;
	}
	ACTIVE_KEY = 0;

	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

void handle_SpecialFunc(GLint key, GLint x, GLint y){
	switch (key) {
		case GLUT_KEY_LEFT:
			ACTIVE_KEY = key;
			break;
		case GLUT_KEY_UP:
			ACTIVE_KEY = key;
			break;
		case GLUT_KEY_RIGHT:
			ACTIVE_KEY = key;
			break;
		case GLUT_KEY_DOWN:
			ACTIVE_KEY = key;
			break;
	}
}