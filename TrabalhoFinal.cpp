/*
*
* Computação Gráfica - 2021.2
* Trabalho Final
* Alunos: Lucas Ribeiro Penedo e Breno Antunes
*
*/

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592654

using namespace std;

char* filenameTexMetal = (char*)"./metalTexture.bmp";
char* filenameTexRobotEye = (char*)"./RobotEyeTexture.bmp";

GLuint _textureIdMetal;
GLuint _textureIdSphere;
GLuint _textureIdCylinder;
GLuint _textureIdRobotEye;
GLUquadric* quadSphere;
GLUquadric* quadCylinder;

bool textureOn = true;

float diameterCylinder = 0.25;
float diameterSphere = 0.4;
float sizeArm = 2.75;
float sizeClampPart = 0.7;
float diameterBase = 5.5;
float heightBase = 2.0;

float diameterHead = 4.0;
float diameterEye = 2.5;

float eyeDistance = 20.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;

float angleLeftArmX = 0.0;
float angleLeftArmY = 0.0;
float angleLeftArmZ = 0.0;
float angleLeftClampX = 0.0;
float angleLeftClampY = 0.0;
float angleLeftClampZ = 0.0;
float angleLeftClamp = 0.0;
float openLeftClamp = 0.0;
float angleRightArmX = 0.0;
float angleRightArmY = 0.0;
float angleRightArmZ = 0.0;
float angleRightClampX = 0.0;
float angleRightClampY = 0.0;
float angleRightClampZ = 0.0;
float angleRightClamp = 0.0;
float openRightClamp = 0.0;

GLfloat lightposition[4] = { 0.0f, 0.0f, -10.0f, 0.0f};
GLfloat luzAmbiente[4] = { 0.2, 0.2, 0.2, 0.0 };

//defines light source
void initLighting(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char* filename) {
	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
												//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
		0,						//0 for now
		GL_RGB,					//Format OpenGL uses for image
		theTexMap.GetNumCols(),	//Width 
		theTexMap.GetNumRows(),	//Height
		0,						//The border of the image
		GL_RGB,					//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	//The actual pixel data
	return textureId; //Returns the id of the texture
}

void initRendering(void) {
	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();

	_textureIdMetal = loadTexture(filenameTexMetal);
	_textureIdCylinder = _textureIdMetal;
	_textureIdSphere = _textureIdMetal;

	_textureIdRobotEye = loadTexture(filenameTexRobotEye);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case 'q':
		if(angleLeftArmX<75) angleLeftArmX += 3;
		glutPostRedisplay();
		break;
	case 'a':
		if(angleLeftArmX>-75) angleLeftArmX -= 3;
		glutPostRedisplay();
		break;
	case 'w':
		angleLeftArmZ = (angleLeftArmZ<=360)? angleLeftArmZ+3 : angleLeftArmZ-360+3;
		glutPostRedisplay();
		break;
	case 's':
		angleLeftArmZ = (angleLeftArmZ>=0)? angleLeftArmZ-3 : angleLeftArmZ+360-3;
		glutPostRedisplay();
		break;
	case 'e':
		if(angleLeftClampX<120) angleLeftClampX += 3;
		glutPostRedisplay();
		break;
	case 'd':
		if(angleLeftClampX>-120) angleLeftClampX -= 3;
		glutPostRedisplay();
		break;
	case 'r':
		angleLeftClampZ = (angleLeftClampZ<=360)? angleLeftClampZ+3 : angleLeftClampZ-360+3;
		glutPostRedisplay();
		break;
	case 'f':
		angleLeftClampZ = (angleLeftClampZ>=0)? angleLeftClampZ-3 : angleLeftClampZ+360-3;
		glutPostRedisplay();
		break;
	case 't':
		if (openLeftClamp < 60) openLeftClamp += 3;
		glutPostRedisplay();
		break;
	case 'g':
		if (openLeftClamp > 0) openLeftClamp -= 3;
		glutPostRedisplay();
		break;
	case 'y':
		angleLeftClamp = (angleLeftClamp<=360)? angleLeftClamp+3 : angleLeftClamp-360+3;
		glutPostRedisplay();
		break;
	case 'h':
		angleLeftClamp = (angleLeftClamp>=0)? angleLeftClamp-3 : angleLeftClamp+360-3;
		glutPostRedisplay();
		break;
	case 'Q':
		if(angleRightArmX<75) angleRightArmX += 3;
		glutPostRedisplay();
		break;
	case 'A':
		if(angleRightArmX>-75) angleRightArmX -= 3;
		glutPostRedisplay();
		break;
	case 'W':
		angleRightArmZ = (angleRightArmZ<=360)? angleRightArmZ+3 : angleRightArmZ-360+3;
		glutPostRedisplay();
		break;
	case 'S':
		angleRightArmZ = (angleRightArmZ>=0)? angleRightArmZ-3 : angleRightArmZ+360-3;
		glutPostRedisplay();
		break;
	case 'E':
		if(angleRightClampX<120) angleRightClampX += 3;
		glutPostRedisplay();
		break;
	case 'D':
		if(angleRightClampX>-120) angleRightClampX -= 3;
		glutPostRedisplay();
		break;
	case 'R':
		angleRightClampZ = (angleRightClampZ<=360)? angleRightClampZ+3 : angleRightClampZ-360+3;
		glutPostRedisplay();
		break;
	case 'F':
		angleRightClampZ = (angleRightClampZ>=0)? angleRightClampZ-3 : angleRightClampZ+360-3;
		glutPostRedisplay();
		break;
	case 'T':
		if (openRightClamp < 60) openRightClamp += 3;
		glutPostRedisplay();
		break;
	case 'G':
		if (openRightClamp > 0) openRightClamp -= 3;
		glutPostRedisplay();
		break;
	case 'Y':
		angleRightClamp = (angleRightClamp<=360)? angleRightClamp+3 : angleRightClamp-360+3;
		glutPostRedisplay();
		break;
	case 'H':
		angleRightClamp = (angleRightClamp>=0)? angleRightClamp-3 : angleRightClamp+360-3;
		glutPostRedisplay();
		break;

	case '1':
		if(viewAngleZ<180) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case '2':
		if(viewAngleZ>0) viewAngleZ -= 3;
		glutPostRedisplay();
		break;
	case '3':
		viewAngleX = (viewAngleX>0)? viewAngleX-3 : viewAngleX+360-3;
		glutPostRedisplay();
		break;
	case '4':
		viewAngleX = (viewAngleX<360)? viewAngleX+3 : viewAngleX-360+3;
		glutPostRedisplay();
		break;
	case '5':
		if(eyeDistance<30) eyeDistance += 1;
		glutPostRedisplay();
		break;
	case '6':
		if(eyeDistance>10) eyeDistance -= 1;
		glutPostRedisplay();
		break;
	case '7':
		textureOn = !textureOn;
		glutPostRedisplay();
		break;
	}
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
}

void drawCylinder(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCone(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}

void drawDisk(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdSphere);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawHead() {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdSphere);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameterHead, 40.0, 40.0);

	glColor3f(0.0f, 0.0f, 0.0f);
	glPushMatrix();

	gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameterHead+0.01, diameterHead+0.01, 0.2, 40.0, 0.2*30.0);
	gluCylinder(quadCylinder, diameterHead+0.01, diameterHead+0.01, -0.2, 40.0, 0.2*30.0);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadCylinder, diameterHead+0.01, diameterHead+0.01, 0.2, 40.0, 0.2*30.0);
	gluCylinder(quadCylinder, diameterHead+0.01, diameterHead+0.01, -0.2, 40.0, 0.2*30.0);

	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void drawEye() {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdRobotEye);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameterEye, 40.0, 40.0);
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	eyeX = eyeDistance * cos(viewAngleZ * PI / 180) * cos(viewAngleX * PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ * PI / 180) * sin(viewAngleX * PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ * PI / 180);
	if (viewAngleZ < 90)
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 4.0, 0.0, 0.0, 1.0);
	else
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 4.0, 0.0, 0.0, -1.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	//drawing color
	glColor3f(1.0f, 1.0f, 1.0f);

	//draws the base
	drawDisk(0.0, diameterBase);
	drawCylinder(diameterBase, 0.25);
	glTranslatef(0.0f, 0.0f, 0.25);
	drawDisk(0.0, diameterBase);

	//draws head
	glTranslatef(0.0f, 0.0f, heightBase+diameterHead);
	drawHead();

	glPushMatrix();

	glTranslatef(diameterHead*0.465, 0.0f, 0.0f);
	glRotatef(75.0, 0.0f, 0.0f, 1.0f);
	drawEye();

	glPopMatrix();
	glPushMatrix();

	//draws left arm
	glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, diameterHead);
	drawSphere(diameterSphere*2);

	glRotatef(-angleLeftArmZ, 0.0f, 0.0f, 1.0f);
	glRotatef(angleLeftArmX, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, diameterSphere);
	drawCylinder(diameterCylinder, sizeArm);

	glTranslatef(0.0f, 0.0f, sizeArm);
	drawSphere(diameterSphere);

	glRotatef(-angleLeftClampZ, 0.0f, 0.0f, 1.0f);
	glRotatef(angleLeftClampX, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder, sizeArm);

	glTranslatef(0.0f, 0.0f, sizeArm);
	drawSphere(diameterSphere);

	//draws left clamp
	glRotatef(-angleLeftClamp, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glRotatef(openLeftClamp+60.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 2*diameterSphere/3);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(-60.0, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(-60.0, 1.0f, 0.0f, 0.0f);
	drawCone(diameterCylinder/3, sizeClampPart*1.2);

	glPopMatrix();

	glRotatef(-openLeftClamp-60.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 2*diameterSphere/3);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(60.0, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(60.0, 1.0f, 0.0f, 0.0f);
	drawCone(diameterCylinder/3, sizeClampPart*1.2);

	glPopMatrix();
	glPushMatrix();

	//draws right arm
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, diameterHead);
	drawSphere(diameterSphere*2);
	glTranslatef(0.0f, 0.0f, diameterSphere);

	glRotatef(angleRightArmZ, 0.0f, 0.0f, 1.0f);
	glRotatef(-angleRightArmX, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder, sizeArm);

	glTranslatef(0.0f, 0.0f, sizeArm);
	drawSphere(diameterSphere);

	glRotatef(angleRightClampZ, 0.0f, 0.0f, 1.0f);
	glRotatef(-angleRightClampX, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder, sizeArm);

	glTranslatef(0.0f, 0.0f, sizeArm);
	drawSphere(diameterSphere);

	//draws right clamp
	glRotatef(angleRightClamp, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glRotatef(-openRightClamp-60.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 2*diameterSphere/3);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(60.0, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(60.0, 1.0f, 0.0f, 0.0f);
	drawCone(diameterCylinder/3, sizeClampPart*1.2);

	glPopMatrix();

	glRotatef(openRightClamp+60.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 2*diameterSphere/3);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(-60.0, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder/3, sizeClampPart);

	glTranslatef(0.0f, 0.0f, sizeClampPart);
	drawSphere(diameterSphere/3);

	glRotatef(-60.0, 1.0f, 0.0f, 0.0f);
	drawCone(diameterCylinder/3, sizeClampPart*1.2);

	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Garra");

	initLighting();
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}