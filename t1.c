/*
Danilo da Costa Telles Téo	9293626


*/


#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void draw();

void mouse(int button, int state, int x, int y);

int main(int argc, char *argv[]) {
	
	// inicializa o glut;
	glutInit(&argc, argv);

	// utilizando o buffer duplo
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);


	// define uma janela de 800x600 e a posiciona no inicio da tela do computador
	glutInitWindowSize(900, 900);
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

	return 0;
}



void draw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//define o fundo da tela para branco
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);




	//DESENHA ABDOMEN
	int i;
	int qtd_retas = 360;

	GLfloat centroab_x = 10.0f, centroab_y = 50.0f;
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






	glutSwapBuffers();
	glFlush();


}

void mouse(int button, int state, int x, int y) {}