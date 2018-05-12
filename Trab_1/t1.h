#ifndef T1_H
#define T1_H

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

void rotateInPlace(float x, float y, float angle);
void display();
void mouse(int btn, int state, int x, int y);
void move();
void update(int value);
void init();

#endif
