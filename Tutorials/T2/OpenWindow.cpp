// g++ *.cpp -lglut -lGLEW -lGLU -lGL -lSDL2 -o out.exe

// The includes must be included in THIS specific order or else it breaks

// clang-format off
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <iostream>
// clang-format on

// Various namespaces that I might need
using namespace std;

// Function prototypes
void  RenderScene(void);
float map(float value, float x1, float x2, float newX1, float newX2);

// Global Variables
GLuint        VBO;
int           numOfComponents = 2;
int           numOfVertices   = 10000;
vector<float> vertices(numOfComponents* numOfVertices);

int main(int argc, char* argv[])
{
  // Variables
  int         windowWidth     = 1024;
  int         windowLength    = 768;
  int         windowXPosition = 1500;
  int         windowYPosition = 400;
  char const* windowName      = "My Second Open GL Program";

  for (int i = 0; i < numOfVertices; i++)
  {
    float x                           = map(float(i) / numOfVertices, 0, 1, -10, 10);
    vertices[numOfComponents * i]     = map(x, -10, 10, -1, 1);
    vertices[numOfComponents * i + 1] = map(3 * sin(x), -10, 10, -1, 1);
    cout << x << " " << vertices[numOfComponents * i] << " " << vertices[numOfComponents * i + 1] << endl;
  }

  // Converting vector into array
  float temp[vertices.size()];
  copy(vertices.begin(), vertices.end(), temp);

  // Allows for easy command line arguments.
  glutInit(&argc, argv);

  // Allows for double buffering.
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  // Various window options
  glutInitWindowSize(windowWidth, windowLength);
  glutInitWindowPosition(windowXPosition, windowYPosition);
  glutCreateWindow(windowName);

  // Must be done after the window is created. Will error out otherwise.
  GLenum res = glewInit();
  if (res != GLEW_OK)
  {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  // Specifies the number of objects and the GLuint to manage them.
  // The other two functions are just setup
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(temp), temp, GL_STATIC_DRAW);

  // Sets the background color
  glClearColor(0, 0, 0, 0);

  // Sets the function that will be applied to the screen
  glutDisplayFunc(RenderScene);

  // Goes to the main loop of OpenGL
  glutMainLoop();
}

void RenderScene(void)
{
  // Clears the screen
  glClear(GL_COLOR_BUFFER_BIT);

  glEnableVertexAttribArray(0);

  // Binds our buff buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // The inputs are, starting value, number of components, type of each value, normalized data bool, various offsets I can google
  glVertexAttribPointer(0, numOfComponents, GL_FLOAT, GL_FALSE, 0, 0);

  // Draws our points to the screen
  glDrawArrays(GL_LINE_STRIP, 0, numOfVertices);  // GL_POINTS

  glDisableVertexAttribArray(0);

  // Swaps the buffer from what you were displaying to what was being edited.
  glutSwapBuffers();
}

float map(float value, float a, float b, float c, float d)
{
  return c + (value - a) * (d - c) / (b - a);
}