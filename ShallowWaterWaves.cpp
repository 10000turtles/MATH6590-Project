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
float tolerance = 0.01;
float friction = 0.99;

char const* windowName = "Shallow Water Waves";

int windowWidth     = 1000;
int windowLength    = 1000;
int windowXPosition = 0;
int windowYPosition = 0;

const char*           pVSFileName = "shader.vs";
const char*           pFSFileName = "shader.fs";
vector<vector<float>> vertices;
vector<vector<float>> bottom;

class Complex
{
 public:
  float r;
  float i;
  Complex(float r, float i)
  {
    this->r = r;
    this->i = i;
  }
  Complex()
  {
    r = 0;
    i = 0;
  }
  void operator=(Complex a)
  {
    this->r = a.r;
    this->i = a.i;
  }
  Complex operator*(Complex a)
  {
    Complex temp;
    temp.r = this->r * a.r - this->i * a.i;
    temp.i = this->r * a.i + this->i * a.r;
    return temp;
  }
  void operator+=(Complex a)
  {
    this->r += a.r;
    this->i += a.i;
  }
  Complex operator/(float a)
  {
    Complex temp;
    temp.r = this->r / a;
    temp.i = this->i / a;
    return temp;
  }
  Complex operator+(float a)
  {
    Complex temp;
    temp.r = this->r + a;
    temp.i = this->i;
    return temp;
  }
};

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
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // Set picture color to green (in RGB model)
  // as only argument corresponding to G (Green) is 1.0 and rest are 0.0
  glColor3f(0.1, 0.2, 0.9);

  // Set width of point to one unit
  glPointSize(40.0);
  glLineWidth(3.0);
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
    glBegin(GL_LINE_STRIP);

    for (int j = 0; j < bottom[i].size(); j += 2)
    {
      float c = map(bottom[i][j + 1], 0, windowLength, 0, 1);
      if (c < 0)
        c = 0;
      glColor3f(0, 0, 0);
      glVertex2i(bottom[i][j], bottom[i][j + 1]);
    }

    glEnd();
    glFlush();
    i++;
    i = i % vertices.size();
    usleep(10000);
  }
}

float map(float value, float a, float b, float c, float d)
{
  return c + (value - a) * (d - c) / (b - a);
}

std::ostream& operator<<(std::ostream& ostr, const Complex& e)
{
  ostr << e.r << " + " << e.i << "i";
  return ostr;
}

std::ostream& operator<<(std::ostream& ostr, Complex* e)
{
  ostr << *e;
  return ostr;
}

Complex expi(float a)
{
  return Complex(cos(a), sin(a));
}

void fft(vector<Complex>& nums, int c)
{
  vector<Complex> ans;

  for (int j = 0; j < c; j++)
  {
    ans.push_back(Complex());
  }

  for (int q = 0; q < c; q++)
  {
    Complex sum = Complex(0, 0);
    for (int k = 0; k < c; k++)
    {
      Complex temp = expi(-2 * M_PI * (float(k * q) / c));
      //cout << p << " " << q << " " << j << " " << k << "   " << nums[j][k] << endl;
      sum += temp * nums[k];
    }
    ans[q]  = sum;
  }
  for (int q = 0; q < c; q++)
  {
    if(abs(ans[q].r) < tolerance)
    {
      ans[q].r = 0;
    }
    if(abs(ans[q].i) < tolerance)
    {
      ans[q].i = 0;
    }
    nums[q] = ans[q];
  }
}

void ifft(vector<Complex>& nums, int c)
{
  vector<Complex> ans;

  for (int j = 0; j < c; j++)
  {
    ans.push_back(Complex());
  }

  for (int q = 0; q < c; q++)
  {
    Complex sum = Complex(0, 0);
    for (int k = 0; k < c; k++)
    {
      Complex temp = expi(2 * M_PI * (float(k * q) / c));
      //cout << q << " " << k << "   " << temp << endl;
      sum += temp * nums[k];
    }
    Complex temp(1 / float(c), 0);
    ans[q]  = sum * temp;
  }
  for (int q = 0; q < c; q++)
  {
    if(abs(ans[q].r) < tolerance)
    {
      ans[q].r = 0;
    }
    if(abs(ans[q].i) < tolerance)
    {
      ans[q].i = 0;
    }
    nums[q] = ans[q];
  }
}

vector<float> RHS_h(vector<Complex> u, float dt, float B, vector<Complex> k)
{
  vector<float> ans;
  fft(u, u.size());
  for (int i = 0; i < u.size(); i++)
  {
    u[i] = u[i] * k[i];
  }
  ifft(u, u.size());
  for (int i = 0; i < u.size(); i++)
  {

    ans.push_back(-u[i].r * dt / B);
  }
  return ans;
}

vector<float> RHS_u(vector<float> h, vector<Complex> u, float dt, float B, float g, vector<Complex> k)
{
  vector<float>   ans;
  vector<Complex> temp;
  for (int i = 0; i < u.size(); i++)
  {
    // if (abs(h[i]) - 0.00005 <= 0)
    // {
    //   cout << "Helloooooo" << endl;
    // }
    temp.push_back(((u[i] * u[i]) / h[i]) + (g * h[i] * h[i] / 2));
  }

  fft(temp, temp.size());
  for (int i = 0; i < temp.size(); i++)
  {
    temp[i] = temp[i] * k[i];
  }
  ifft(temp, temp.size());

  for (int i = 0; i < temp.size(); i++)
  {
    ans.push_back(-temp[i].r * dt / B);
  }
  return ans;
}

vector<vector<float>> graphFunction()
{
  float g = 9.81;

  float T0 = 0;
  float Tn = 6;
  float dt = 1.0/400;

  int timeSteps = int((Tn - T0) / dt);

  // Manual Programming of the wave equation
  vector<vector<float>> vertices;
  vector<vector<float>> etaB;
  vector<vector<float>> X;
  vector<vector<float>> h;

  float Xmin = -10;
  float Xmax = 10;
  int numOfVerticesX  = 200;
  float dx = (Xmax-Xmin)/numOfVerticesX;

  float B = (Xmax - Xmin) / (2 * M_PI);

  int   numOfComponents = 2;
  float Ymin            = 0;
  float Ymax            = 20;

  for (int i = 0; i < timeSteps; i++)
  {
    vertices.push_back(vector<float>(numOfComponents * numOfVerticesX));
    bottom.push_back(vector<float>(numOfComponents * numOfVerticesX));
    etaB.push_back(vector<float>(numOfVerticesX));
    X.push_back(vector<float>(numOfVerticesX));
    h.push_back(vector<float>(numOfVerticesX));
  }

  // Initial Conditions

  for (int i = 0; i < numOfVerticesX; i++)
  {
    float x = map(i, 0,numOfVerticesX, Xmin, Xmax);
    for (int t = 0; t < timeSteps; t++)
    {
      X[t][i] = x;
      etaB[t][i] = 0.5+7*exp(-x*x);
    }

    h[0][i] = 5.8 + 3*exp(-(x+5)*(x+5)/10);
  }
  for (int t = 0; t < timeSteps; t++)
  {
    etaB[t][0] = 10;
    etaB[t][numOfVerticesX-1] = 10;
  }
  for (int i = 0; i < numOfVerticesX * numOfComponents; i += 2)
  {
    vertices[0][i]     = map(X[0][i / 2], Xmin, Xmax, -windowWidth, windowWidth);
    vertices[0][i + 1] = map(h[0][i / 2] , Ymin, Ymax, -windowLength, windowLength);
  }
  for (int i = 0; i < numOfVerticesX *numOfComponents; i+=2)
  {
    for (int t = 0; t < timeSteps; t++)
    {
      bottom[t][i] = map(X[0][i / 2], Xmin, Xmax, -windowWidth, windowWidth);
      bottom[t][i+1] = map(etaB[0][i / 2] , Ymin, Ymax, -windowLength, windowLength);
    }
  }

  vector<Complex> k;
  vector<Complex> u;

  for (int i = 0; i < numOfVerticesX / 2; i++)
  {
    k.push_back(Complex(0, i));
  }

  k.push_back(Complex(0, 0));

  for (int i = -numOfVerticesX / 2 + 1; i < 0; i++)
  {
    k.push_back(Complex(0, i));
  }

  for (int i = 0; i < numOfVerticesX; i++)
  {
    u.push_back(Complex(0, 0));
  }

  for (int t = 1; t < timeSteps; t++)
  {
    cout << "t: " << t << endl;
    vector<float> temp = RHS_h(u, dt, B, k);
    for (int i = 0; i < numOfVerticesX; i++)
    {
      h[t][i] = h[t-1][i] + temp[i];

      //cout << "h[" << t << "][" << i << "]: " << h[t][i] << endl;
    }
    temp = RHS_u(h[t], u, dt, B, g, k);
    for (int i = 0; i < numOfVerticesX; i++)
    {
      //cout << "u["<< i<<"]: " << u[i] << endl;
      u[i].r += temp[i];
      u[i].r *= friction;
    }

    for (int i = 0; i < numOfVerticesX * numOfComponents; i += 2)
    {
      vertices[t][i]     = map(X[t][i / 2], Xmin, Xmax, -windowWidth, windowWidth);
      vertices[t][i + 1] = map(h[t][i / 2] , Ymin, Ymax, -windowLength, windowLength);
      if(h[t][i/2] <= etaB[t][i/2])
      {
        u[i/2].r = 0;
        // h[t][i/2] = 1;
        //vertices[t][i] = map(h[t][i / 2] , Ymin, Ymax, -windowLength, windowLength);
        vertices[t][i + 1] = map(etaB[t][i / 2] , Ymin, Ymax, -windowLength, windowLength);
      }

    }
  }
  return vertices;
}