#define _USE_MATH_DEFINES // for C
#define STB_IMAGE_IMPLEMENTATION
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"
#include <opencv2/opencv.hpp>

int width = 800;
int height = 400;

// FOG
int fogToggle = 1;
static GLint fogMode;


int view_width = 800;
int view_height = 400;

const GLfloat abRadius = 2.0f;
const int abStacks = 20;
const int abSlices = 20;

const GLfloat cefaloRadius = 1.0f;
const int cefaloStacks = 20;
const int cefaloSlices = 20;

const GLfloat STEP_SIZE = 0.5f;

const int REFRESH_DELAY = 200;


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
void mulVertex(Vertex* v, Vertex* a, float b) {
    v -> x = a -> x * b;
    v -> y = a -> y * b;
    v -> z = a -> z * b;
}

void rotateVertexOnY(Vertex* v, Vertex* a, float b) {
	GLfloat aux_x = a -> x;
    v -> x =  cos(M_PI * -b / 180.0) * aux_x + sin(M_PI * -b / 180.0) * a -> z;
    v -> y = a -> y;
    v -> z = -sin(M_PI * -b / 180.0) * aux_x + cos(M_PI * -b / 180.0) * a -> z;
}

Vertex cefaloCenter;
Vertex abCenter;
Vertex* spiderCenter;
Vertex* spiderFrontDir;
Vertex* origin;
GLint ACTIVE_KEY;
GLint LAST_KEY;
GLfloat ROT; //clockwise!
int STATE;
cv::Mat hello1;
cv::Mat hello2;
GLuint texName[2];

char textures_faces[6][23] = {
	"orbital-element_rt.tga",
	"orbital-element_lf.tga",
	"orbital-element_up.tga",
	"orbital-element_dn.tga",
	"orbital-element_bk.tga",
	"orbital-element_ft.tga",
};

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


//PROTOTIPOS DAS FUNCOES DE VISUALIZACAO/////////////////////////////////////////////////////////////////////////////
//DESENHO DA ARANHA
void drawAbdomen(GLfloat center_x, GLfloat center_y, GLfloat center_z);
void drawCefaloTorax(GLfloat center_x, GLfloat center_y, GLfloat center_z);
void drawLegs();
void drawSpider();
void handle_SpecialFunc(GLint key, GLint x, GLint y);
void handleFog(unsigned char key, int x, int y);
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

/**
 * @desc Desenha a skybox.
 */
void drawSkybox();
int loadCubemap();
unsigned int cubemapTexture;

//DEFINICAO DAS VIEWPORTS
void displayCallback();
void reshapeCallback(int w, int h);


//FUNCAO DE INICIALIZACAO DA ESTRUTURA VERTEX
void initializeVertex(Vertex *v, GLfloat x, GLfloat y, GLfloat z);

/////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv){

	
	
	initializeVertex(&abCenter, 0.0f, 0.0f, -(abRadius + cefaloRadius)); //inicializacao de abCenter
	initializeVertex(&cefaloCenter, 0.0f, 0.0f, 0.0f); //inicializacao de cefaloCenter
	spiderCenter = newVertex(0.0f, 0.0f, (abRadius + cefaloRadius));
	spiderFrontDir = newVertex(0.0f, 0.0f, 1.0f);
	origin = newVertex(0.0f, 0.0f, 0.0f);
	STATE = 0;
	ROT = 0.0f;

	hello1 = cv::imread("hellokittytexture1.jpg");
	hello2 = cv::imread("hellokittytexture2.jpg");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Trabalho 3");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	glutSpecialFunc(handle_SpecialFunc);
	glutKeyboardFunc(handleFog);
	glutTimerFunc(REFRESH_DELAY, update, 0);

    cubemapTexture = loadCubemap();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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
    glColor3f(0.0, 0.0, 0.0);
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

void drawSkybox(){		
    glColor3f(1.0, 1.0, 1.0);
	glTranslatef(50,50,50);
	glScalef(100.0f,100.0f,100.0f);    
    glEnable(GL_TEXTURE_CUBE_MAP);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 128; i+=3) {
		glTexCoord3f(skyboxVertices[i], skyboxVertices[i+1], skyboxVertices[i+2]);
		glVertex3f(skyboxVertices[i], skyboxVertices[i+1], skyboxVertices[i+2]);
	}
	glEnd();
	glDisable(GL_TEXTURE_CUBE_MAP);
	glScalef(1/100.0f,1/100.0f,1/100.0f);
	glTranslatef(-50,-50,-50);
}

int loadCubemap(){
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int width, height, nrChannels;
	unsigned char *data;  	
	for(GLuint i = 0; i < 6; i++) {		
		if(i == 2)
			stbi_set_flip_vertically_on_load(1); 
		else
			stbi_set_flip_vertically_on_load(0);
	    data = stbi_load(textures_faces[i], &width, &height, &nrChannels, 0);
	    glTexImage2D(
	        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
	        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	    );
		stbi_image_free(data);
	}

	return texture;
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

		
		
		// glColor3f(1.0f, 1.0f, 1.0f);
		// glTranslatef(center_x, center_y, center_z); 
		// glutSolidSphere(abRadius, abSlices, abStacks); //esfera com centro (0,0,0)

		glGenTextures(1, texName);
		glBindTexture(GL_TEXTURE_3D, texName[0]);
		
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		// glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		// glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		// glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, hello2.cols, hello2.rows, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, hello2.ptr());

		glEnable(GL_TEXTURE_3D);

		GLUquadric *quadObj = gluNewQuadric();

		gluQuadricTexture(quadObj, GL_TRUE);
		gluSphere(quadObj, abRadius, abSlices, abStacks);
		gluDeleteQuadric(quadObj);

		glDisable(GL_TEXTURE_3D);
			
	glPopMatrix();
}

void drawCefaloTorax(GLfloat center_x, GLfloat center_y, GLfloat center_z){
	glPushMatrix();

		glGenTextures(1, texName);
		glBindTexture(GL_TEXTURE_3D, texName[0]);
		
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		// glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		// glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
		// glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, hello2.cols, hello2.rows, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, hello2.ptr());
		

		glEnable(GL_TEXTURE_3D);

		GLUquadric *quadObj = gluNewQuadric();

		gluQuadricTexture(quadObj, GL_TRUE);
		gluSphere(quadObj, cefaloRadius, cefaloSlices, cefaloStacks);
		gluDeleteQuadric(quadObj);

		glDisable(GL_TEXTURE_3D);


		// glColor3f(1.0f, 0.0f, 0.0f);
		// glTranslatef(center_x, center_y, center_z); 
		// glutSolidSphere(cefaloRadius, cefaloSlices, cefaloStacks); //esfera com centro (0,0,0)
	
	glPopMatrix();
}

void drawContractedLeg(Vertex* d) {
	Vertex v; // vertex to draw lines
	Vertex i; // vertex that represent the vector to increment v

	glBegin(GL_LINE_STRIP);
	
		glColor3f(0.0f, 1.0f, 0.0f);

		mulVertex(&i, d, cefaloRadius);
		addVertexesv(&v, origin, &i);
		glVertex3f(v.x, v.y, v.z);

		addVertexes(&i, d, 0.0f, 1.0f, 0.0f);
		mulVertex(&i, &i, cefaloRadius);
		addVertexesv(&v, &v, &i);
		glVertex3f(v.x, v.y, v.z);

		// mulVertex(&i, d, cefaloRadius * 2.0);
		// addVertexesv(&v, &v, &i);
		// glVertex3f(v.x, v.y, v.z);

		addVertexes(&i, d, 0.0f, -1.0f, 0.0f);
		mulVertex(&i, &i, cefaloRadius * 2);
		addVertexesv(&v, &v, &i);
		glVertex3f(v.x, v.y, v.z);	

	glEnd();
	
}

void drawExtendedLeg(Vertex* d) {
	Vertex v; // vertex to draw lines
	Vertex i; // vertex that represent the vector to increment v

	glBegin(GL_LINE_STRIP);
	
		glColor3f(0.0f, 1.0f, 0.0f);

		mulVertex(&i, d, cefaloRadius);
		addVertexesv(&v, origin, &i);
		glVertex3f(v.x, v.y, v.z);

		mulVertex(&i, d, 4.0);
		addVertexes(&v, &i, 0.0f, -1.0f, 0.0f);
		glVertex3f(v.x, v.y, v.z);


	glEnd();
	
}


void drawLegs0() {

	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);

	glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);

	glRotatef(135.0f, 0.0f, 1.0f, 0.0f);		
}

void drawLegs1() {

	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	drawExtendedLeg(spiderFrontDir);
	glRotatef(40.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(40.0f, 0.0f, 1.0f, 0.0f);
	drawExtendedLeg(spiderFrontDir);

	glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);

	glRotatef(135.0f, 0.0f, 1.0f, 0.0f);
}

void drawLegs2() {

	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);

	glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	drawExtendedLeg(spiderFrontDir);
	glRotatef(-40.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
	drawContractedLeg(spiderFrontDir);
	glRotatef(-40.0f, 0.0f, 1.0f, 0.0f);
	drawExtendedLeg(spiderFrontDir);

	glRotatef(135.0f, 0.0f, 1.0f, 0.0f);
}

void drawSpider(){
	
	glTranslatef(spiderCenter->x, spiderCenter->y, spiderCenter->z);
	glRotatef(-ROT, 0.0f, 1.0f, 0.0f);

	glTranslatef(abCenter.x, abCenter.y, abCenter.z);
	drawAbdomen(0.0f, 0.0f, 0.0f);
	glTranslatef(-(abCenter.x), -(abCenter.y), -(abCenter.z));
	
	drawCefaloTorax(0.0f, 0.0f, 0.0f);

	switch(STATE) {
		case 0:
			drawLegs0();
			break;
		case 1:
			drawLegs1();
			break;
		case 2:
			drawLegs2();
			break;
		
	}
	

	glRotatef(ROT, 0.0f, 1.0f, 0.0f);
	glTranslatef(-(spiderCenter->x), -(spiderCenter->y), -(spiderCenter->z));
}

void displayCallback(){
	/** Limpa a janela APENAS uma vez */
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vertex abPos; // center of the view
	Vertex v; // eye pos
	// addVertexes(&abPos, spiderCenter,
	// 			cos(M_PI * -ROT / 180.0) * abCenter.x + sin(M_PI * -ROT / 180.0) * abCenter.z,
	// 			abCenter.y,
	// 			-sin(M_PI * -ROT / 180.0) * abCenter.x + cos(M_PI * -ROT / 180.0) * abCenter.z);
	addVertexesv(&abPos, spiderCenter, &abCenter);

	//bottom left viewport - free view
	glLoadIdentity();
	addVertexes(&v, &abPos, 3.0, 2.0, 10.0);

	gluLookAt(v.x, v.y, v.z,
				abPos.x, abPos.y, abPos.z,
				0.0, 1.0, 0.0);
	glViewport(0, 0, view_width/2, view_height);
	drawSkybox();
	drawGrid(100.0, 1.0);
	drawWCAxes();
	drawSpider();

	//u left viewport - y-axis view
	glLoadIdentity();
	addVertexes(&v, &abPos, 0.0, 10.0, 0.0);
	gluLookAt(v.x, v.y, v.z,
				abPos.x, abPos.y, abPos.z,
				0.0, 0.0, 1.0); // up vector becomes k
	glViewport(width/2, 0, view_width/2, view_height);
	drawSkybox();
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
	gluPerspective(80.0, (GLfloat) width/(GLfloat) height, 1.0, 250.0);
	glMatrixMode(GL_MODELVIEW);
}

int isArrowKey(GLint key) {
	return key == GLUT_KEY_LEFT || key == GLUT_KEY_UP || key == GLUT_KEY_RIGHT || key == GLUT_KEY_DOWN;
}

void update(int value) {

	
	Vertex v;
	
	switch (ACTIVE_KEY) {
		case GLUT_KEY_LEFT:
			// addVertexes(spiderCenter, spiderCenter, STEP_SIZE, 0.0f, 0.0f);
			ROT += -45.0f;
			STATE = 0;
			break;
		case GLUT_KEY_UP:

			initializeVertex(&v, 0.0f, 0.0f, STEP_SIZE);
			rotateVertexOnY(&v, &v, ROT);
			// printf("%f, %f\n", v.x, v.z);
			addVertexesv(spiderCenter, spiderCenter, &v);
			// ROT = 0.0f;
			STATE += 1;
			break;
		case GLUT_KEY_RIGHT:
			// addVertexes(spiderCenter, spiderCenter, -STEP_SIZE, 0.0f, 0.0f);
			ROT += 45.0f;
			STATE  = 0;
			break;
		case GLUT_KEY_DOWN:
			
			initializeVertex(&v, 0.0f, 0.0f, -STEP_SIZE);
			rotateVertexOnY(&v, &v, ROT);
			// printf("%f, %f\n", v.x, v.z);
			addVertexesv(spiderCenter, spiderCenter, &v);
			// ROT = 180.0f;
			STATE += 1;
			break;
	}
	if (STATE == 3)
		if (isArrowKey(LAST_KEY)) {
			ACTIVE_KEY = LAST_KEY;
			STATE = 1;			
		}	
		else
			STATE = 0;
			
	LAST_KEY = 0;
	
	if (STATE == 0)
		ACTIVE_KEY = 0;

	glutPostRedisplay();
	glutTimerFunc(REFRESH_DELAY, update, 0);
}

void handle_SpecialFunc(GLint key, GLint x, GLint y){

	LAST_KEY = key;

	if (STATE == 0) { // lock ACTIVE_KEY while spider is moving
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
}

void handleFog(unsigned char key, int x, int y){
	if(key == 'n' || key == 'N'){
		if(fogToggle == 1){
			glEnable(GL_FOG);
			{
				GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0};

				fogMode = GL_EXP;
				glFogi(GL_FOG_MODE, fogMode);
				glFogfv(GL_FOG_COLOR, fogColor);
				glFogf(GL_FOG_DENSITY, 0.1);
				glFogf(GL_FOG_START, 1.0);
				glFogf(GL_FOG_END, 10.0);
			}
			glClearColor(0.5, 0.5, 0.5, 1.0); /* fog color */
			fogToggle = 0;
		}
		else if(fogToggle == 0){
			glDisable(GL_FOG);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			fogToggle = 1;
		}
	}
}