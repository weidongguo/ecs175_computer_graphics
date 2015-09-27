#include <GL/glut.h>
#include <algorithm>
#include <iostream>
float *PixelBuffer;
void display();

int main(int argc, char *argv[])
{
	//allocate new pixel buffer, need initialization!!
	PixelBuffer = new float[200 * 200 * 3];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 200*200
	glutInitWindowSize(200, 200);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");
  std::fill(PixelBuffer, PixelBuffer+200*200*3, 1); 
  glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
	//sets display function
	glutDisplayFunc(display);
  
  PixelBuffer[100 * 3 * 200 + 3 * 30 + 0] = 1;
  PixelBuffer[100 * 3 * 200 + 3 * 30 + 1] = 0;
  PixelBuffer[100 * 3 * 200 + 3 * 30 + 2] = 0;
	glutMainLoop();//main display loop, will display until terminate
	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glEnd();
	glFlush();
}
