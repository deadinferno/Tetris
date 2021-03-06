#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>

#include "Shape.h"

#define KEY_ESCAPE 27
#define N 15.00

using namespace std;

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
}glutWindow;

glutWindow win;

bool fullscreen = false;        // для переключения в полный экран

Shape shp;

int d = N;

int upDown_x = 0;
int upDown_z = 0;
int leftRight_rotation = 45;
int r_speed = 7;
float down_speed = 0.01;
int zoom = 40;
float down = d / 2 + 2;
float movex = -1;
float movez = -1;

int score = 0;
bool gameOver = false;

bool stop = true;
int base[(int)N][(int)N][(int)N];

void *font = GLUT_BITMAP_HELVETICA_18;


void destroyer() {
	int count = 0;
	int o = 0;

	for (int k = 0; k < N; k++)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (base[i][k][j] != 0)
					count++;
			}
		}

		if (count == 225)
		{
			o++;
			score++;
			if (k > 0) k--;
		}

		if (o > 0)
		{
			for (int l = 0; l < N; l++)
			{
				for (int m = 0; m < N; m++)
				{
					if (k + o < N)
						base[l][k][m] = base[l][k + o][m];
					else
						base[l][k][m] = 0;
				}
			}
		}

		count = 0;
	}

}

void checkSides(int undox, int undoz) {
	int i, j, k;
	bool draw = false;

	if (down > -d / 2)
	{
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 3; k++)
				{//
					if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] != 0)
					{
						draw = true;
						break;
					}
					else if ((shp.enableCubes[i][k][j] == 1 && movex + i > 7) || (shp.enableCubes[i][k][j] == 1 && movez + j > 7))
					{
						draw = true;
						break;
					}
					else if (shp.enableCubes[i][k][j] == 1 && movex + i < -7)
					{
						draw = true;
						break;
					}
					else if (shp.enableCubes[i][k][2 - j] == 1 && movez + 2 - j < -7)
					{
						draw = true;
						break;
					}
				}

				if (draw)	break;
			}

			if (draw)	break;
		}

		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 3; k++)
				{
					if ((shp.enableCubes[i][k][j] == 1 && movex + i < -7))
					{
						draw = true;
						break;
					}
				}

				if (draw)	break;
			}

			if (draw)	break;
		}


		if (draw)
		{
			movex = undox;
			movez = undoz;
		}
	}
}


void putInBase() {
	int i, j, k;
	bool draw = false;
	bool redShape = false;
	bool blueShape = false;

	if (down > -d / 2)
	{
		for (k = 2; k >= 0; k--)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					if (shp.color == 1)
					{
						if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - 1 - k][(int)movez + d / 2 + j] != 0)
						{
							draw = true;
							break;
						}
					}
					else if (shp.color == 3)
					{
						if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] != 0)
						{
							base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = 0;
							//score++;
						}
					}
					else
					{
						if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - 1 - k][(int)movez + d / 2 + j] != 0
							&& base[(int)movex + d / 2 + i][(int)down + d / 2 - 2 - k][(int)movez + d / 2 + j] != 0
							&& base[(int)movex + d / 2 + i][(int)down + d / 2 - 3 - k][(int)movez + d / 2 + j] != 0)
						{
							blueShape = true;
							draw = true;
							break;
						}
						else
						{
							if (shp.enableCubes[i][k][j] == 1 && base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] != 0)
							{
								base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = 0;
								//score++;
							}
						}
					}
				}

				if (draw)	break;
			}

			if (draw)	break;
		}

		if (draw)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					for (k = 0; k < 3; k++)
					{
						if (shp.color == 1)
						{
							if (shp.enableCubes[i][k][j] == 1)
								base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = shp.color;
						}
						else if (shp.color == 2 && blueShape)
						{
							if (shp.enableCubes[i][k][j] == 1)
								base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = shp.color;
						}
						else if (shp.color == 3)
						{
							redShape = true;
						}
					}
				}
			}
		}

		if (draw && !redShape)
		{
			destroyer();
			if (down < d / 2 - 1)
			{
				shp.createShape();
				shp.initShape();
			}
			else
			{
				gameOver = true;
			}

			down = d / 2;
			movex = -1;
			movez = -1;
		}
	}

	if (down < 2 - d / 2)
	{
		score++;
		for (k = 2; k >= 0; k--)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					if ((shp.enableCubes[i][k][j] == 1 && (int)down + d / 2 - k == 0) || (draw && shp.enableCubes[i][k][j] == 1))
					{
						base[(int)movex + d / 2 + i][(int)down + d / 2 - k][(int)movez + d / 2 + j] = shp.color;
						draw = true;
					}
				}
			}
		}

		if (draw)
		{
			destroyer();
			shp.createShape();
			shp.initShape();
			down = d / 2;
			movex = -1;
			movez = -1;
		}
	}

}


void drawBase() {

	for (int j = 0; j < N; j++)
	{
		for (int k = 0; k < N; k++)
		{
			for (int l = 0; l < N; l++)
			{
				if (base[j][k][l] != 0)
				{
					glPushMatrix();
					glTranslatef(0, 0, 0);
					glRotatef(leftRight_rotation, 0, 1, 0);
					glRotatef(upDown_x, 1, 0, 0);
					glRotatef(upDown_z, 0, 0, 1);

					if (base[j][k][l] == 1)
					{
						glColor3f(1, 1, 0);
					}
					else if (base[j][k][l] == 2)
					{
						glColor3f(0, 0, 1);
					}
					else
					{
						glColor3f(1, 0, 0);
					}

					glTranslatef(j - d / 2, k - d / 2, l - d / 2);
					//glutWireCube(1);
					glutSolidCube(1);
					glPopMatrix();
				}
			}
		}
	}

}



//--------------------------------------------------------------//
//	Вывод текста на экран										//
//--------------------------------------------------------------//
void printTxt(float x, float y, char *String) {
	char *c;

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 10, 0, 10);

	glColor3f(9.0f, 0.7f, 0.4f);
	glRasterPos2f(x, y);
	glDisable(GL_LIGHTING);

	for (c = String; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}



//--------------------------------------------------------------//
//	Рисовка	игрового поля										//
//--------------------------------------------------------------//
void display() {
	float i;
	char scr[20];
	char scre[20];


	strcpy_s(scr, "S C O R E : ");
	sprintf_s(scre, "%d", score);
	strcat_s(scr, scre);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	printTxt(1.0f, 1.0f, scr);

	glLoadIdentity();
	gluLookAt(zoom, 2, 0, 0, 0, 0, 0, 1, 0);


	glPushMatrix();

	glColor3f(0, 1, 1);
	glTranslatef(0, 0, 0);
	glRotatef(leftRight_rotation, 0, 1, 0);
	glRotatef(upDown_x, 1, 0, 0);
	glRotatef(upDown_z, 0, 0, 1);


	glColor3f(0.7, 0.6, 0.9);
	for (i = -N / 2; i <= N / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i, N / 2, -N / 2);
		glVertex3f(i, -N / 2, -N / 2);
		glEnd();
	}

	for (i = -N / 2; i <= N / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(N / 2, i, -N / 2);
		glVertex3f(-N / 2, i, -N / 2);
		glEnd();
	}

	for (i = -N / 2; i <= N / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(-N / 2, N / 2, i);
		glVertex3f(-N / 2, -N / 2, i);
		glEnd();
	}

	for (i = -N / 2; i <= N / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(-N / 2, i, N / 2);
		glVertex3f(-N / 2, i, -N / 2);
		glEnd();
	}


	glColor3f(0.9, 0.9, 0.6);
	for (i = -N / 2; i <= N / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i, -N / 2, -N / 2);
		glVertex3f(i, -N / 2, N / 2);
		glEnd();
	}

	for (i = -N / 2; i <= N / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(N / 2, -N / 2, i);
		glVertex3f(-N / 2, -N / 2, i);
		glEnd();
	}

	glPopMatrix();

	if (!gameOver)
	{
		shp.drawShape(d, movex, down, movez, leftRight_rotation, upDown_x, upDown_z);

		putInBase();
	}
	else
	{
		strcpy_s(scr, "G A M E  O V E R");
		printTxt(4.0f, 5.0f, scr);
	}

	drawBase();

	if (!stop)
	{
		down -= down_speed;
	}

	glutSwapBuffers();
	glFlush();

}



//--------------------------------------------------------------//
//	Специальные клавиши											//
//		- обрабатывает все "специальные" нажатия, без ascii		//
//--------------------------------------------------------------//
void special_keys(int keys, int x, int y) {
	int moires = 0;

	if (leftRight_rotation < 0)
		moires = leftRight_rotation % 360 + 360;
	else
		moires = leftRight_rotation % 360;

	int undox = movex;
	int undoz = movez;

	switch (keys) {

	// F1
	case GLUT_KEY_F1:       			
		fullscreen = !fullscreen;
		if (fullscreen) glutFullScreen();
		else glutReshapeWindow(500, 500);
		break;

	// Стрелка "Влево"
	case GLUT_KEY_LEFT:
		if (!stop) {
			if (moires <= 45 || moires > 315)
			{
				if (movez < d / 2)
					movez++;
			}
			else if (moires > 135 && moires <= 225)
			{
				if (movez > -d / 2 - 2)
					movez--;
			}
			else if (moires > 45 && moires <= 135)
			{
				if (movex > -d / 2 - 2)
					movex--;
			}
			else if (moires > 225 && moires <= 315)
			{
				if (movex < d / 2)
					movex++;
			}
		}

		break;
	// Стрелка "Вправо"
	case GLUT_KEY_RIGHT:
		if (!stop) {
			if (moires <= 45 || moires > 315)
			{
				if (movez > -d / 2 - 2)
					movez--;
			}
			else if (moires > 135 && moires <= 225)
			{
				if (movez < d / 2)
					movez++;
			}
			else if (moires > 45 && moires <= 135)
			{
				if (movex < d / 2)
					movex++;
			}
			else if (moires > 225 && moires <= 315)
			{
				if (movex > -d / 2 - 2)
					movex--;
			}
		}
		break;

	// Стрелка "Вверх"
	case GLUT_KEY_UP:
		if (!stop) {
			if (moires <= 45 || moires > 315)
			{
				if (movex > -d / 2 - 2)
					movex--;
			}
			else if (moires > 135 && moires <= 225)
			{
				if (movex < d / 2)
					movex++;
			}
			else if (moires > 45 && moires <= 135)
			{
				if (movez > -d / 2 - 2)
					movez--;
			}
			else if (moires > 225 && moires <= 315)
			{
				if (movez < d / 2)
					movez++;
			}
		}
		break;

	// Стрелка "Вниз"
	case GLUT_KEY_DOWN:
		if (!stop) {
			if (moires <= 45 || moires > 315)
			{
				if (movex < d / 2)
					movex++;
			}
			else if (moires > 135 && moires <= 225)
			{
				if (movex > -d / 2 - 2)
					movex--;
			}
			else if (moires > 45 && moires <= 135)
			{
				if (movez < d / 2)
					movez++;
			}
			else if (moires > 225 && moires <= 315)
			{
				if (movez > -d / 2 - 2)
					movez--;
			}
		}
		break;

	default:
		break;
	}

	if (!stop)
		checkSides(undox, undoz);
}



//--------------------------------------------------------------//
//	Обычные клавиши												//
//		- обрабатывает все "нормальные" нажатия, с ascii		//
//--------------------------------------------------------------//
void keyboard(unsigned char key, int mousePositionX, int mousePositionY)
{
	int moires = 0;

	if (leftRight_rotation < 0)
		moires = leftRight_rotation % 360 + 360;
	else
		moires = leftRight_rotation % 360;

	// S
	switch (key)
	{
	case 's':
		if (stop)
			stop = false;
		else
			stop = true;

		break;

	// I
	case 'i':
		if (moires <= 45 || moires > 315)
		{
			upDown_z += r_speed;
		}
		else if (moires > 135 && moires <= 225)
		{
			upDown_z -= r_speed;
		}
		else if (moires > 45 && moires <= 135)
		{
			upDown_x -= r_speed;
		}
		else if (moires > 225 && moires <= 315)
		{
			upDown_x += r_speed;
		}
		break;

	// K
	case 'k':
		if (moires <= 45 || moires > 315)
		{
			upDown_z -= r_speed;
		}
		else if (moires > 135 && moires <= 225)
		{
			upDown_z += r_speed;
		}
		else if (moires > 45 && moires <= 135)
		{
			upDown_x += r_speed;
		}
		else if (moires > 225 && moires <= 315)
		{
			upDown_x -= r_speed;
		}

		break;

	// Минус
	case '-':
		if (down_speed > 0.01 && !stop)
			down_speed -= 0.005;
		break;

	// Равно
	case '=':
		if (down_speed <= 0.10 && !stop)
			down_speed += 0.005;
		break;

	// F
	case 'f':
		if (zoom > 2)
			zoom -= 2;
		break;

	// D
	case 'd':
		if (zoom < 100)
			zoom += 2;
		break;

	// J
	case 'j':
		leftRight_rotation += r_speed;
		break;

	// L
	case 'l':
		leftRight_rotation -= r_speed;
		break;

	// X
	case 'x':
		if (!stop)
			shp.rotateShape_x();
		break;

	// C
	case 'c':
		if (!stop)
			shp.rotateShape_y();
		break;

	// V
	case 'v':
		if (!stop)
			shp.rotateShape_z();
		break;

	// Escape
	case KEY_ESCAPE:
		exit(0);
		break;

	default:
		break;
	}
}



//--------------------------------------------------------------//
//	Инициализация												//
//--------------------------------------------------------------//
void initialize()
{
	// Устанавливаем текущей матрицу проекции
	glMatrixMode(GL_PROJECTION);
	
	// Устанавливаем область видимости
	glViewport(0, 0, win.width, win.height);
	
	//glMatrixMode(GL_PROJECTION);

	// Сбрасываем текущую матрицу, т.е. заменяя единичной
	glLoadIdentity();
	GLfloat aspect = (GLfloat)win.width / win.height;

	// gluPerspective — set up a perspective projection matrix
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);

	glMatrixMode(GL_MODELVIEW); // Устанавливаем видовую матрицу
	glShadeModel(GL_SMOOTH);
	
	glClearDepth(1.0f); // Задает значение глубины, использующееся при очистке глубины буфера
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// glHint — specify implementation-specific hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// геморрой с освещением
	GLfloat amb_light[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat specular[] = { 0.5, 0.5, 0.6, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


//------------------------------------------------------//
//	main()												//
//		- Инициализирует GLUT и создает окно			//
//		- Назначет функции обратного вызова				//
//		- Запускает цикл приложения						//
//------------------------------------------------------//
int main(int argc, char** argv) {
	// set window values
	win.width = 640;
	win.height = 480;
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			for (int k = 0; k < d; k++)
			{
				base[i][j][k] = 0;
			}
		}
	}

	shp.createShape();
	shp.initShape();

	
	glutInit(&argc, argv);							// Инициализация glut

	/*	GLUT_RGB - Bit mask to select an RGB mode window
		GLUT_DOUBLE - Bit mask to select a double buffered window
		GLUT_DEPTH - Bit mask to select a window with a depth buffer	*/
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  		

	glutInitWindowSize(win.width, win.height);		// Размер окна
	glutCreateWindow("Tetris 3D");					// Создаем окно и задаем заголовок
	glutDisplayFunc(display);                       // Коллбэк для текущего окна
	glutIdleFunc(display);							// Коллбэк для простаивания, т.е. когда нет событий
	glutKeyboardFunc(keyboard);						// Коллбэк для обычных клавиш
	glutSpecialFunc(special_keys);                  // Коллбэк для специальных клавиш

	initialize();
	glutMainLoop();                                 // Инициализация главного цикла

	return 0;
}
