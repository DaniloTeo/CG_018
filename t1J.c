#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void draw();

void mouse(int button, int state, int x, int y);

void rot_aranha(int x, int y);

void tick(int value);

float angle = 0.0;

float coef_mov = 0.5;

float mouse_x, mouse_y;

float pos_aranha[2] = {10.0, 50.0};
float dir_aranha[2] = {0.0, 0.0};

short moving = 0;

int main(int argc, char *argv[]) {
	
	// inicializa o glut;
	glutInit(&argc, argv);

	// utilizando o buffer duplo
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);


	// define uma janela de 900x900 e a posiciona no inicio da tela do computador
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Trabalho 1");

	// define as funcoes de desenho na tela e de evento do mouse
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ativa movimento para ponto clicado
  	glutPassiveMotionFunc(rot_aranha); // pega angulo de rotacao

	// sistema de coordenadasf
	// 0 <= x <= 100
	// 0 <= y <= 100
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);

	glutTimerFunc(10, tick, 0);
	// loop para a main
	glutMainLoop();

	return 0;
}

float sysGLToOrthoX(float x){
	float coordGL;
	coordGL = (x / (double) glutGet(GLUT_WINDOW_WIDTH)) * 100.0f;
	return coordGL;
}

float sysGLToOrthoY(float y){
	float coordGL;
	coordGL = (1.0f - (y / (double) glutGet(GLUT_WINDOW_HEIGHT))) * 100.0f;
	return coordGL;
}

float orthoToSysGLX(float x){
	float coordOrtho;
	coordOrtho = x * glutGet(GLUT_WINDOW_WIDTH) / 100.0f;
	return coordOrtho;
}

float orthoToSysGLY(float y){
	float coordOrtho;
	coordOrtho = ((y-1.0)/100.0) * (double) glutGet(GLUT_WINDOW_HEIGHT);
	return coordOrtho;
}

void draw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//define o fundo da tela para branco
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	// Desloca a aranha pra posicao do mouse caso tenha clicado
	if(moving){
		pos_aranha[0] += dir_aranha[0]*0.5;
		pos_aranha[1] += dir_aranha[1]*0.5;
		//glTranslatef(pos_aranha[0], pos_aranha[1], 0.0);
		glTranslatef(coef_mov * dir_aranha[0], coef_mov * dir_aranha[1], 0.0);
		coef_mov += 0.5;
		if (abs(pos_aranha[0] - mouse_x) < 0.5 || abs(pos_aranha[1] - mouse_y) < 0.5){
			moving = 0;
			coef_mov = 0.1;
		}
		//printf("mouse_x %f mouse_y %f pos_aranha_x %f pos_aranha_y %f\n", mouse_x, mouse_y, pos_aranha[0], pos_aranha[1]);
	} else {
		// Aranha olha para o mouse
		//printf("%f %f\n", orthoToSysGLX(pos_aranha[0]), orthoToSysGLY(pos_aranha[1]));
		//glTranslatef(orthoToSysGLX(pos_aranha[0]), orthoToSysGLY(pos_aranha[1]), 0.0);
		glRotatef(10, 0.0, 0.0, 1.0);	
		//glTranslatef(-orthoToSysGLX(pos_aranha[0]), -orthoToSysGLY(pos_aranha[1]),0.0);
	}

	//DESENHA ABDOMEN
	int i;
	int qtd_retas = 360;

	GLfloat centroab_x = pos_aranha[0], centroab_y = pos_aranha[1];
	GLfloat raioab = 5.0f;
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		for(i = 0; i < qtd_retas; i++) {
			glVertex2f(centroab_x + (raioab * cos((i * 2* M_PI)/qtd_retas)), centroab_y + (raioab * sin((i * 2* M_PI)/qtd_retas)));
		}

	glEnd();

	//DESENHA CEFALOTORAX
	GLfloat	raioce = 3.0f;
	GLfloat centroce_x = (centroab_x + raioab) + raioce;
	GLfloat centroce_y = centroab_y;

	glBegin(GL_LINE_LOOP);
		for(i = 0; i < qtd_retas; i++){
			glVertex2f(centroce_x + (raioce * cos((i * 2* M_PI)/qtd_retas)), centroce_y + (raioce * sin((i * 2* M_PI)/qtd_retas)));
		}

	glEnd();

	//DESENHA OLHOS
	GLfloat xd = (centroce_x + 2.0f), yd = (centroce_y - 1.0f);
	GLfloat xe = (centroce_x + 2.0f), ye = (centroce_y + 1.0f);

	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex2f(xd, yd);
		glVertex2f(xe, ye);

	glEnd();


	//DESENHA PERNAS SUPERIORES-----------------------------------------------------------------------------------------------------

	//PERNA DIR
	GLfloat pernasup_d_base_x = centroce_x + (raioce * cos((300 * 2* M_PI)/qtd_retas)), pernasup_d_base_y = centroce_y + (raioce * sin((300 * 2* M_PI)/qtd_retas));
	GLfloat	pernasup_d_cot_x = centroce_x + 10.0f, pernasup_d_cot_y = centroce_y - 5.0f;
	GLfloat	pernasup_d_ext_x = centroce_x + 10.0f + 3.0f, pernasup_d_ext_y = centroce_y - 5.0f;

	//PERNA ESQ
	GLfloat pernasup_e_base_x = centroce_x + (raioce * cos((60 * 2* M_PI)/qtd_retas)), pernasup_e_base_y = centroce_y + (raioce * sin((60 * 2* M_PI)/qtd_retas));
	GLfloat	pernasup_e_cot_x = centroce_x + 10.0f, pernasup_e_cot_y = centroce_y + 5.0f;
	GLfloat	pernasup_e_ext_x = centroce_x + 10.0f + 3.0f, pernasup_e_ext_y = centroce_y + 5.0f;

	glBegin(GL_LINE_STRIP);
		//dir
		glVertex2f(pernasup_d_base_x, pernasup_d_base_y); //base 
		glVertex2f(pernasup_d_cot_x, pernasup_d_cot_y); //cotovelo
		glVertex2f(pernasup_d_ext_x, pernasup_d_ext_y); //extremidade
	glEnd();

	glBegin(GL_LINE_STRIP);
		//esq
		glVertex2f(pernasup_e_base_x, pernasup_e_base_y); //base 
		glVertex2f(pernasup_e_cot_x, pernasup_e_cot_y); //cotovelo
		glVertex2f(pernasup_e_ext_x, pernasup_e_ext_y); //extremidade
	glEnd();


	//DESENHA PERNAS INFERIORES--------------------------------------------------------------------------------------------------------

	//PERNA DIR
	GLfloat	pernainf_d_base_x = centroce_x + (raioce * cos((240 * 2* M_PI)/qtd_retas)) , pernainf_d_base_y = centroce_y + (raioce * sin((240 * 2* M_PI)/qtd_retas));
	GLfloat	pernainf_d_cot_x = centroce_x - 3.0f , pernainf_d_cot_y = centroce_y - 5.0f ;
	GLfloat	pernainf_d_ext_x = centroce_x - 10.0f - 3.0f , pernainf_d_ext_y = centroce_y - 6.0f;

	//PERNA ESQ
	GLfloat	pernainf_e_base_x = centroce_x + (raioce * cos((120 * 2* M_PI)/qtd_retas)) , pernainf_e_base_y  = centroce_y + (raioce * sin((120 * 2* M_PI)/qtd_retas));
	GLfloat	pernainf_e_cot_x = centroce_x - 3.0f , pernainf_e_cot_y = centroce_y + 5.0f ;
	GLfloat	pernainf_e_ext_x = centroce_x - 10.0f - 3.0f , pernainf_e_ext_y = centroce_y + 6.0f;

	glBegin(GL_LINE_STRIP);
		//dir
		glVertex2f(pernainf_d_base_x, pernainf_d_base_y); //base 
		glVertex2f(pernainf_d_cot_x, pernainf_d_cot_y); //cotovelo
		glVertex2f(pernainf_d_ext_x, pernainf_d_ext_y); //extremidade
	glEnd();

	glBegin(GL_LINE_STRIP);
		//esq
		glVertex2f(pernainf_e_base_x, pernainf_e_base_y); //base 
		glVertex2f(pernainf_e_cot_x, pernainf_e_cot_y); //cotovelo
		glVertex2f(pernainf_e_ext_x, pernainf_e_ext_y); //extremidade
	glEnd();

	//DESENHA PERNAS MID-INF---------------------------------------------------------------------------------------------------------
	//PERNA DIR
	GLfloat	pernamidinf_d_base_x = centroce_x + (raioce * cos((260 * 2* M_PI)/qtd_retas)) , pernamidinf_d_base_y = centroce_y + (raioce * sin((260 * 2* M_PI)/qtd_retas));
	GLfloat	pernamidinf_d_cot_x = centroce_x - 1.0f , pernamidinf_d_cot_y = centroce_y - 6.0f ;
	GLfloat	pernamidinf_d_ext_x = centroce_x - 10.0f , pernamidinf_d_ext_y = centroce_y - 7.0f;

	//PERNA ESQ
	GLfloat	pernamidinf_e_base_x = centroce_x + (raioce * cos((100 * 2* M_PI)/qtd_retas)) , pernamidinf_e_base_y  = centroce_y + (raioce * sin((100 * 2* M_PI)/qtd_retas));
	GLfloat	pernamidinf_e_cot_x = centroce_x - 1.0f , pernamidinf_e_cot_y = centroce_y + 6.0f ;
	GLfloat	pernamidinf_e_ext_x = centroce_x - 10.0f  , pernamidinf_e_ext_y = centroce_y + 7.0f;

	glBegin(GL_LINE_STRIP);
		//dir
		glVertex2f(pernamidinf_d_base_x, pernamidinf_d_base_y); //base 
		glVertex2f(pernamidinf_d_cot_x, pernamidinf_d_cot_y); //cotovelo
		glVertex2f(pernamidinf_d_ext_x, pernamidinf_d_ext_y); //extremidade
	glEnd();

	glBegin(GL_LINE_STRIP);
		//esq
		glVertex2f(pernamidinf_e_base_x, pernamidinf_e_base_y); //base 
		glVertex2f(pernamidinf_e_cot_x, pernamidinf_e_cot_y); //cotovelo
		glVertex2f(pernamidinf_e_ext_x, pernamidinf_e_ext_y); //extremidade
	glEnd();

	// glBegin(GL_POINTS);
		
	// 	//dir
	// 	glVertex2f(centroce_x + (raioce * cos((280 * 2* M_PI)/qtd_retas)), centroce_y + (raioce * sin((280 * 2* M_PI)/qtd_retas)));
	// 	glVertex2f(centroce_x - 0.0f, centroce_y - 7.0f);
	// 	glVertex2f(centroce_x - 10.0f - 3.0f, centroce_y - 8.0f);

	// 	//esq
	// 	glVertex2f(centroce_x + (raioce * cos((80 * 2* M_PI)/qtd_retas)), centroce_y + (raioce * sin((80 * 2* M_PI)/qtd_retas)));
	// 	glVertex2f(centroce_x - 0.0f, centroce_y + 7.0f);
	// 	glVertex2f(centroce_x - 10.0f - 3.0f, centroce_y + 8.0f);



	// glEnd();

	//DESENHA PERNAS MID-SUP-----------------------------------------------------------------------------------------------------
	//PERNA DIR
	GLfloat	pernamidsup_d_base_x = centroce_x + (raioce * cos((280 * 2* M_PI)/qtd_retas)) , pernamidsup_d_base_y = centroce_y + (raioce * sin((280 * 2* M_PI)/qtd_retas));
	GLfloat	pernamidsup_d_cot_x = centroce_x  , pernamidsup_d_cot_y = centroce_y - 7.0f ;
	GLfloat	pernamidsup_d_ext_x = centroce_x - 10.0f , pernamidsup_d_ext_y = centroce_y - 8.0f;

	//PERNA ESQ
	GLfloat	pernamidsup_e_base_x = centroce_x + (raioce * cos((80 * 2* M_PI)/qtd_retas)) , pernamidsup_e_base_y  = centroce_y + (raioce * sin((80 * 2* M_PI)/qtd_retas));
	GLfloat	pernamidsup_e_cot_x = centroce_x  , pernamidsup_e_cot_y = centroce_y + 7.0f ;
	GLfloat	pernamidsup_e_ext_x = centroce_x - 10.0f , pernamidsup_e_ext_y = centroce_y + 8.0f;

	glBegin(GL_LINE_STRIP);
		//dir
		glVertex2f(pernamidsup_d_base_x, pernamidsup_d_base_y); //base 
		glVertex2f(pernamidsup_d_cot_x, pernamidsup_d_cot_y); //cotovelo
		glVertex2f(pernamidsup_d_ext_x, pernamidsup_d_ext_y); //extremidade
	glEnd();

	glBegin(GL_LINE_STRIP);
		//esq
		glVertex2f(pernamidsup_e_base_x, pernamidsup_e_base_y); //base 
		glVertex2f(pernamidsup_e_cot_x, pernamidsup_e_cot_y); //cotovelo
		glVertex2f(pernamidsup_e_ext_x, pernamidsup_e_ext_y); //extremidade
	glEnd();


	glPopMatrix();



	glutSwapBuffers();
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON){
		moving = 1;
	}
}


void rot_aranha(int x, int y) {
	if(!moving){
		// pegar angulo entre mouse e aranha
		mouse_x = sysGLToOrthoX(x);
		mouse_y = sysGLToOrthoY(y);
		
		GLfloat t1 = mouse_x - pos_aranha[0];
		GLfloat t2 = mouse_y - pos_aranha[1];
		dir_aranha[0] = t1 / (sqrt(pow(t1, 2) + pow(t2, 2)));
		dir_aranha[1] = t2 / (sqrt(pow(t1, 2) + pow(t2, 2)));

		angle = atan2(dir_aranha[1], dir_aranha[0])*180/M_PI;
		if(angle < 0){
			angle += 360;
		}
		//printf("dir x %f dir y %f angle %f\n", dir_aranha[0], dir_aranha[1], angle);
	}
}

void tick(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, tick, 0);
}