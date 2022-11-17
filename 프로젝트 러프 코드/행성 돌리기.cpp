#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int, int, int, int);
void Keyboard(unsigned char, int, int);
void TimerFunction(int);


float fXNum = 0.0;
float fYNum = 0.0;
float fZNum = 0.0;

float fSNum = 0.0;
float fWNum = 0.0;
float fANum = 0.0;
float fDNum = 0.0;

float fMainNum = 0.0;
float fOneNum = 0.0;
float fTwoNum = 0.0;
float fThrNum = 0.0;

int iOneNum = 90;
int iTwoNum = 90;
int iThrNum = 90;

bool bSolidWire = true;

void vSphereM(int, int, int, bool);
void vTorusM(int, int, int, int, bool);
void isortho(bool o);

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("Sample"); 
	glutDisplayFunc(drawScene); 
	glutMouseFunc(Mouse);
	glutTimerFunc(100, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

// 윈도우 출력 함수
GLvoid drawScene(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT); 
	//-------------------------------------------------------------
	//전체 회전
	glPushMatrix(); //Save
	glRotatef(fXNum, 1.0, 0.0, 0.0);
	glRotatef(fYNum, 0.0, 1.0, 0.0);
	glRotatef(fZNum, 0.0, 0.0, 1.0);
	glTranslated(fANum, fDNum, fSNum);
	//-------------------------------------------------------------
	//중심 회전
	glPushMatrix(); //Save
	glRotated(90, 1.0, 0, 0);
	glRotated(fMainNum, 0, 0, 1.0);
	glTranslated(0, 0, 0);
	glColor3f(1.0, 0.0, 0.0);
	vSphereM(50, 50, 10, bSolidWire);
	glPopMatrix(); //Load
	//-------------------------------------------------------------
	glPushMatrix();
	glRotated(iOneNum, 1.0, 0, 0);
	glColor3f(0.0, 1.0, 0.0);
	vTorusM(2, 200, 5, 30, bSolidWire);
	//-------------------------------------------------------------
	glRotated(fOneNum, 0, 0, 1.0);
	glTranslated(200, 0, 0);
	vSphereM(30, 30, 30, bSolidWire);
	//-------------------------------------------------------------
	glColor3f(0.0, 0.0, 1.0);
	vTorusM(2, 50, 5, 30, bSolidWire);
	//-------------------------------------------------------------
	glRotated(fOneNum, 0, 0, 1.0);
	glTranslated(50, 0, 0);
	vSphereM(10, 10, 10, bSolidWire);
	glPopMatrix();
	//-------------------------------------------------------------


	//-------------------------------------------------------------
	glPushMatrix();
	glRotated(iTwoNum, 1.0, 1.0, 0);
	glColor3f(0.0, 1.0, 1.0);
	vTorusM(2, 200, 5, 30, bSolidWire);
	//-------------------------------------------------------------
	glRotated(fTwoNum, 0, 0, 1.0);
	glTranslated(200, 0, 0);
	vSphereM(30, 30, 30, bSolidWire);
	//-------------------------------------------------------------
	glColor3f(0.0, 0.0, 1.0);
	vTorusM(2, 50, 5, 30, bSolidWire);
	//-------------------------------------------------------------
	glRotated(fTwoNum, 0, 0, 1.0);
	glTranslated(50, 0, 0);
	vSphereM(10, 10, 10, bSolidWire);
	glPopMatrix();
	//-------------------------------------------------------------


	//-------------------------------------------------------------
	glPushMatrix();
	glRotated(iThrNum, 1.0, -1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	vTorusM(2, 200, 5, 30, bSolidWire);
	//-------------------------------------------------------------
	glRotated(fThrNum, 0, 0, 1.0);
	glTranslated(200, 0, 0);
	vSphereM(30, 30, 30, bSolidWire);
	//-------------------------------------------------------------
	glColor3f(0.0, 0.0, 1.0);
	vTorusM(2, 50, 5, 30, bSolidWire);
	//-------------------------------------------------------------
	glRotated(fThrNum, 0, 0, 1.0);
	glTranslated(50, 0, 0);
	vSphereM(10, 10, 10, bSolidWire);
	glPopMatrix();
	//-------------------------------------------------------------

	glPopMatrix(); //Load

	glFlush(); // 화면에 출력하기

}

void TimerFunction(int value) {
	glutPostRedisplay(); 
	fMainNum += 5.0;
	fOneNum += 5.0;
	fTwoNum += 3.0;
	fThrNum += 1.0;
	glutTimerFunc(100, TimerFunction, 1); 
}

void Keyboard(unsigned char key, int x, int y) {
	printf("InPut Key = %c\n", key);

	if (key == 'x')
		fXNum += 5.0;
	if (key == 'X')
		fXNum -= 5.0;

	if (key == 'y')
		fYNum += 5.0;
	if (key == 'Y')
		fYNum -= 5.0;

	if (key == 'z')
		fZNum += 5.0;
	if (key == 'Z')
		fZNum -= 5.0;

	if (key == '1')
		iTwoNum += 10;

	if (key == '2')
		iThrNum += 10;

	if (key == '3')
		iOneNum += 10;

	if (key == 'm')
		bSolidWire = true;

	if (key == 'M')
		bSolidWire = false;

	
	if (key == 's')
		fSNum += 1.0;
	if (key == 'S')
		fSNum -= 1.0;
	if (key == 'a')
		fANum += 1.0;
	if (key == 'A')
		fANum -= 1.0;
	if (key == 'd')
		fDNum += 1.0;
	if (key == 'D')
		fDNum -= 1.0;
	

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("Left = (%d, %d)\n", x, y);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		printf("Right = (%d, %d)\n", x, y);
	}
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, w / h, 1.0, 1500.0);

	glTranslatef(0, 0, -500);

	glMatrixMode(GL_MODELVIEW);
}

void vSphereM(int a, int b, int c, bool d) {
	if (d == true)
		glutWireSphere(a, b, c);		//가운데 중심 원
	else
		glutSolidSphere(a, b, c);		//가운데 중심 원
}

void vTorusM(int a, int b, int c, int d, bool e) {
	if (e == true)
		glutWireTorus(a, b, c, d);		//2번째 안에 고리
	else
		glutSolidTorus(a, b, c, d);		//2번째 안에 고리
}