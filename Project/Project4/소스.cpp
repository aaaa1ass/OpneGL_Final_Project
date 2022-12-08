#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

char* filetobuf(const char* file);
GLvoid drawScene(GLvoid);
GLvoid drawScenee(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Reshapee(int w, int h);
void make_vertexShader();
void make_fragmentShader();
void make_shaderProgram();
void InitBuffer();

void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void idleScene();
void TimerFunction(int value);

void ReadObj(FILE* objFile);

GLint width, height;

GLchar* fragmentSource;
GLchar* vertexSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
GLuint VAO, VBO[3];

float light = 1.0;
bool left_button = false;
int start_point_x = 0;
int start_point_z = 0;
float start_vx = 0.0;
float start_vz = 0.0;
float vx = 0.0;
float vz = 0.0;

int vertex_count = 6;
int ver = 3 * vertex_count;

const GLfloat triShape[][3] = {

	{-1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, -1.0, 0.0},
	{0.0, 1.0, 0.0},

	{0.0, 0.0, -1.0},
	{0.0, 0.0, 1.0}
	
};
const GLfloat colors[][3] = {
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},

	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};
const GLfloat normals[][3] = {
	{0.0,0.0,0.0},
	{0.0,0.0,0.0},

	{0.0,0.0,0.0},
	{0.0,0.0,0.0},

	{0.0,0.0,0.0},
	{0.0,0.0,0.0}
};

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ 
	width = 1000;
	height = 1000;

	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLE" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	make_vertexShader();
	make_fragmentShader();
	make_shaderProgram();
	InitBuffer();

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(100, TimerFunction, 1);
	glutIdleFunc(idleScene);
	glViewport(0, 0, 100, 100);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);


	glutMainLoop(); // 이벤트 처리 시작 
}

glm::vec3 cameraPos = glm::vec3(-0.5, 0.5f, 1.0f); // 카메라

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f,1.0f,1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

	unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	


	glm::mat4 mTransform = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	// 축
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);

	 
	cameraPos = glm::vec3(vx, 1.0f, 1.0f + vz); // 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f + vx, 0.0f, 0.0f + vz); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 vTransform = glm::mat4(1.0f);
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);



	glm::mat4 pTransform = glm::mat4(1.0f);
	pTransform = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	pTransform = glm::translate(pTransform, glm::vec3(0.0, 0.0, -5.0));
	//pTransform = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f); //--- 투영 공간을 [-2.0, 2.0] 공간으로 설정
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLocation, light, light, light);


	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
GLvoid InitBuffer()
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * ver * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * ver * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);	
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * ver * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	glUseProgram(shaderProgram);
	unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLocation, 0.0, 0.0, 1.0);
	int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);
	
}

void make_vertexShader()
{

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	//error check
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS,&result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}


}
void make_fragmentShader()
{

	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//error check
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}


}
void make_shaderProgram()
{
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//error check
	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}

	glUseProgram(shaderProgram);

}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		start_point_x = x;
		start_point_z = y;
		left_button = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		start_vx = vx;
		start_vz = vz;
		left_button = false;
	}
	glutPostRedisplay();
}
void Motion(int x, int y)
{
	if (left_button == true)
	{
		vx = (float)(start_point_x - x) / 400 + start_vx;
		vz = (float)(start_point_z - y) / 400 + start_vz;
	}
	glutPostRedisplay();
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		exit(0);
	}

	glutPostRedisplay();
}
void SpecialKeyboard(int key, int x, int y)
{
	glutPostRedisplay();
}
void TimerFunction(int value)
{

	glutPostRedisplay();
	
	glutTimerFunc(100, TimerFunction, 1);
}
void idleScene()
{
	glutPostRedisplay();
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}





