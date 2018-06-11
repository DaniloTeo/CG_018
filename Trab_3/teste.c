#define _USE_MATH_DEFINES // for C
#define STB_IMAGE_IMPLEMENTATION
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "stb_image.h"

int width = 800;
int height = 400;

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

void drawSkybox();
int loadCubemap();
unsigned int cubemapTexture;

void displayCallback();

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Trabalho 3");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glutDisplayFunc(displayCallback);	

    cubemapTexture = loadCubemap();

	glutMainLoop();

	return 0;
}

void drawSkybox(){	
    glEnable(GL_TEXTURE_CUBE_MAP);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 128; i+=3) {
		glTexCoord3f(skyboxVertices[i], skyboxVertices[i+1], skyboxVertices[i+2]);
		glVertex3f(skyboxVertices[i], skyboxVertices[i+1], skyboxVertices[i+2]);
	}
	glEnd();
	glDisable(GL_TEXTURE_CUBE_MAP);
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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	
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

void displayCallback(){
	/** Limpa a janela APENAS uma vez */
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	gluPerspective(90.0, width/height, 0.1, 50);

	gluLookAt(0.0, 0.0, 1.0,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

	drawSkybox();

	/** Dispara os comandos APENAS uma vez */
	glFlush();
}