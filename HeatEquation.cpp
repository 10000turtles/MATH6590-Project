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
#include <fstream>
#include <time.h>
#include <unistd.h>
// clang-format on

// Various namespaces that I might need
using namespace std;

// Function prototypes
void                  display(void);
void                  myInit(void);
float                 map(float value, float x1, float x2, float newX1, float newX2);
vector<vector<float>> graphFunction();
// Global Variables
GLuint VBO;

char const* windowName = "Heat Wave Equation";

int windowWidth     = 1000;
int windowLength    = 1000;
int windowXPosition = 1500;
int windowYPosition = 100;

const char*           pVSFileName = "shader.vs";
const char*           pFSFileName = "shader.fs";
vector<vector<float>> vertices;

// Driver Program
int main(int argc, char** argv)
{
  vertices = graphFunction();

  glutInit(&argc, argv);

  // Display mode which is of RGB (Red Green Blue) type
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  // Declares window size
  glutInitWindowSize(windowWidth, windowLength);

  // Declares window position which is (0, 0)
  // means lower left corner will indicate position (0, 0)
  glutInitWindowPosition(windowXPosition, windowYPosition);

  // Name to window
  glutCreateWindow(windowName);

  // Call to myInit()
  myInit();
  glutDisplayFunc(display);
  glutMainLoop();
}

void myInit(void)
{
  // Reset background color with black (since all three argument is 0.0)
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Set picture color to green (in RGB model)
  // as only argument corresponding to G (Green) is 1.0 and rest are 0.0
  glColor3f(0.1, 0.2, 0.9);

  // Set width of point to one unit
  glPointSize(1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set window size in X- and Y- direction
  gluOrtho2D(-windowWidth, windowWidth, -windowLength, windowLength);
}

// Function to display animation
void display(void)
{
  int i = 0;

  while (true)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_STRIP);

    for (int j = 0; j < vertices[i].size(); j += 2)
    {
      float c = map(vertices[i][j + 1], 0, windowLength, 0, 1);
      if (c < 0)
        c = 0;
      glColor3f(c, 0, 1 - c);
      glVertex2i(vertices[i][j], vertices[i][j + 1]);
    }

    glEnd();
    glFlush();
    i++;
    i = i % vertices.size();
    usleep(4000);
  }
}

float map(float value, float a, float b, float c, float d)
{
  return c + (value - a) * (d - c) / (b - a);
}

vector<vector<float>> graphFunction()
{
  float T0 = 0;
  float Tn = 5;
  float k  = 1.0 / 1000;

  int timeSteps = int((Tn - T0) / k);

  // Manual Programming of the wave equation
  vector<vector<float>> vertices;

  float Xmin = -10;
  float Xmax = 10;
  float h    = 1.0 / 20;

  float r = k / (h * h);

  if (r > 0.5)
  {
    cout << "ERROR: NON-CONVERGANT" << endl;
  }

  int numOfVerticesX = (Xmax - Xmin) / h;

  int   numOfComponents = 2;
  float Ymin            = -10;
  float Ymax            = 10;

  for (int i = 0; i < timeSteps; i++)
  {
    vertices.push_back(vector<float>(numOfComponents * numOfVerticesX));
  }

  // Initial Conditions

  for (int i = 0; i < numOfVerticesX * numOfComponents; i += 2)
  {
    float x            = map(i, 0, numOfComponents * numOfVerticesX, Xmin, Xmax);
    vertices[0][i]     = map(x, Xmin, Xmax, -windowWidth, windowWidth);
    vertices[0][i + 1] = map((x * x * x * 4 + 3 * x * x + 2 * x + 5) * 5 * exp(-x * x) * 2 * sin(x * x), Ymin, Ymax, -windowLength, windowLength);
  }

  // Finite Difference Equations
  for (int t = 1; t < timeSteps; t++)
  {
    float tMath = map(t, 0, timeSteps, T0, Tn);
    for (int i = 2; i < numOfVerticesX * numOfComponents - 2; i += 2)
    {
      float x            = map(i, 0, numOfComponents * numOfVerticesX, Xmin, Xmax);
      vertices[t][i]     = map(x, Xmin, Xmax, -windowWidth, windowWidth);
      vertices[t][i + 1] = (1 - 2 * r) * vertices[t - 1][i + 1] + r * vertices[t - 1][i - 1] + r * vertices[t - 1][i + 3];
    }
    vertices[t][0] = -windowWidth;
    vertices[t][1] = ((1 - 2 * r) * vertices[t - 1][1] + r * vertices[t - 1][1] + r * vertices[t - 1][3]);

    vertices[t][numOfVerticesX * numOfComponents - 2] = windowWidth;
    vertices[t][numOfVerticesX * numOfComponents - 1] = (1 - 2 * r * vertices[t - 1][numOfVerticesX * numOfComponents - 1] + r * vertices[t - 1][numOfVerticesX * numOfComponents - 3] + r * vertices[t - 1][numOfVerticesX * numOfComponents - 1]);
  }

  return vertices;
}