// g++ *.cpp -lglut -lGLEW -lGLU -lGL -lSDL2 -o out.exe

// The includes must be included in THIS specific order or else it breaks

// clang-format off
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
// clang-format on

// Possible other libraries that I might need lol

// #include <GLFW/glfw3.h>
// #include <SDL2/SDL.h>
// #include <cstdlib>
// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>

using namespace std;
using namespace glm;

void RenderScene(void);

int main(int argc, char* argv[])
{
  //VARIABLES
  int         windowWidth     = 1024;
  int         windowLength    = 768;
  int         windowXPosition = 1500;
  int         windowYPosition = 400;
  char const* windowName      = "My First Open GL Program";

  // clang-format off
  static const GLfloat triangleData[] = {
      0.0,  0.8,  0.0,
     -0.8, -0.8,  0.0,
      0.8, -0.8,  0.0,
  };

  static const GLfloat rectData[] = {
      0.0,  0.0,  0.0,
      0.0,  0.4,  0.0,
      0.4,  0.0,  0.0,
      0.4,  0.4,  0.0,
  };
  // clang-format on

  // Allows for easy command line arguments.
  glutInit(&argc, argv);

  // Allows for double buffering.
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  // Various window options
  glutInitWindowSize(windowWidth, windowLength);
  glutInitWindowPosition(windowXPosition, windowYPosition);
  glutCreateWindow(windowName);

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

  // Swaps the buffer from what you were displaying to what was being edited.
  glutSwapBuffers();
}