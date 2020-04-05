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
// clang-format on

// Various namespaces that I might need
using namespace std;

// Function prototypes
void        RenderScene(void);
float       map(float value, float x1, float x2, float newX1, float newX2);
bool        ReadFile(const char* fileName, string& a);
static void CompileShaders();
static void InitializeGlutCallbacks();

// Global Variables
GLuint VBO;

int numOfComponents = 2;
int numOfVerticesX  = 10000;
// int           numOfVerticesY  = 100;
const char*   pVSFileName = "shader.vs";
const char*   pFSFileName = "shader.fs";
vector<float> vertices(numOfComponents* numOfVerticesX);

int main(int argc, char* argv[])
{
  // Variables
  int windowWidth     = 1024;
  int windowLength    = 768;
  int windowXPosition = 1500;
  int windowYPosition = 400;

  char const* windowName = "My Second Open GL Program";

  float Tmin = 0;
  float Tmax = 10000;
  float Xmin = -4.5;
  float Xmax = 4.5;
  float Ymin = -3.5;
  float Ymax = 3.5;

  float a = 11;
  float b = 10;
  for (int j = 1; j < numOfVerticesX; j++)
  {
    for (int i = 0; i < j; i += 2)
    {
      float t         = map(float(i) / numOfVerticesX, 0, 1, Tmin, Tmax);
      vertices[i]     = map(4 * sin(a / b * t), Xmin, Xmax, -1, 1);
      vertices[i + 1] = map(3 * sin(t), Ymin, Ymax, -1, 1);
    }
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

  // Callbacks. Apparently.
  InitializeGlutCallbacks();

  // Must be done after the window is created. Will error out otherwise.
  GLenum res = glewInit();
  if (res != GLEW_OK)
  {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  GLuint ShaderProgram = glCreateProgram();

  // Specifies the number of objects and the GLuint to manage them.
  // The other two functions are just setup
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(temp), temp, GL_STATIC_DRAW);

  // Sets the background color
  glClearColor(0, 0, 0, 0);

  // Sets the function that will be applied to the screen
  glutDisplayFunc(RenderScene);

  //Does all the shader work for me
  CompileShaders();

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
  glDrawArrays(GL_POINTS, 0, numOfVerticesX);  // GL_POINTS


  glDisableVertexAttribArray(0);

  // Swaps the buffer from what you were displaying to what was being edited.
  glutSwapBuffers();
}

float map(float value, float a, float b, float c, float d)
{
  return c + (value - a) * (d - c) / (b - a);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0)
  {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    exit(0);
  }

  const GLchar* p[1];
  p[0] = pShaderText;
  GLint Lengths[1];
  Lengths[0] = strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    exit(1);
  }

  glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
  GLuint ShaderProgram = glCreateProgram();

  if (ShaderProgram == 0)
  {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }

  string vs = "", fs = "";

  if (!ReadFile(pVSFileName, vs))
  {
    exit(1);
  };

  if (!ReadFile(pFSFileName, fs))
  {
    exit(1);
  };

  cout << vs << endl
       << fs;
  AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
  AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

  GLint  Success        = 0;
  GLchar ErrorLog[1024] = {0};

  glLinkProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  glUseProgram(ShaderProgram);
}

bool ReadFile(const char* fileName, string& a)
{
  ifstream f(fileName);
  for (std::string line; getline(f, line);)
  {
    a += line + "\n";
  }
  return true;
}

static void InitializeGlutCallbacks()
{
  glutDisplayFunc(RenderScene);
}
