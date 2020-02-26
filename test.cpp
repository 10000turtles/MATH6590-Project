#include <GL/freeglut.h>
#include <stdlib.h>

int   x, y;
float r, g, b;

void idle()
{
  x = rand() % 640;
  y = rand() % 480;

  r = 0;  //(float)((rand() % 9)) / 8;
  g = 1;  //(float)((rand() % 9)) / 8;
  b = 0;  //(float)((rand() % 9)) / 8;

  glutPostRedisplay();
}

void magic_dots(void)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 20.0, 0.0, 400.0);

  glColor3f(r, g, b);

  glBegin(GL_POINTS);
  glVertex2i(x, y);
  glEnd();

  glFlush();
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowPosition(2000, 400);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Randomly generated points");
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glutDisplayFunc(magic_dots);
  glutIdleFunc(idle);
  glutMainLoop();

  return 0;
}
