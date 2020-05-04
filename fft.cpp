#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
//clang-format on
using namespace std;

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
};

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

vector<vector<Complex>> fft2(vector<vector<Complex>> nums, int r, int c)
{
  vector<vector<Complex>> ans;
  for (int i = 0; i < r; i++)
  {
    ans.push_back(vector<Complex>(c));
    for (int j = 0; j < c; j++)
    {
      ans[i].push_back(Complex());
    }
  }
  for (int p = 0; p < r; p++)
  {
    for (int q = 0; q < c; q++)
    {
      Complex sum = Complex(0, 0);
      for (int j = 0; j < r; j++)
      {
        for (int k = 0; k < c; k++)
        {
          Complex temp = expi(-2 * M_PI * (float(j * p) / r + float(k * q) / c));
          //cout << p << " " << q << " " << j << " " << k << "   " << nums[j][k] << endl;
          sum += temp * nums[j][k];
        }
      }
      ans[p][q] = sum;
    }
  }
  return ans;
}

vector<vector<Complex>> ifft2(vector<vector<Complex>> nums, int r, int c)
{
  vector<vector<Complex>> ans;
  for (int i = 0; i < r; i++)
  {
    ans.push_back(vector<Complex>(c));
    for (int j = 0; j < c; j++)
    {
      ans[i].push_back(Complex());
    }
  }
  for (int p = 0; p < r; p++)
  {
    for (int q = 0; q < c; q++)
    {
      Complex sum = Complex(0, 0);
      for (int j = 0; j < r; j++)
      {
        for (int k = 0; k < c; k++)
        {
          Complex temp = expi(2 * M_PI * (float(j * p) / r + float(k * q) / c));
          //cout << p << " " << q << " " << j << " " << k << "   " << nums[j][k] << endl;
          sum += temp * nums[j][k];
        }
      }
      ans[p][q] = sum * Complex(1 / float(r * c), 0);
    }
  }
  return ans;
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
      //cout  << q << " " << k << "   " << nums[k] << " " << temp << " " << temp * nums[k] << endl;
      sum += temp * nums[k];
    }
    ans[q]  = sum;

  }
  for (int q = 0; q < c; q++)
  {
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
    nums[q] = ans[q];
  }
}


int main()
{
  int RowCol = 32;
  vector<Complex> nums;
  for (int j = 0; j < RowCol; j++)
  {
    nums.push_back(Complex(1,0));
  }

  fft(nums, RowCol);

  for (int q = 0; q < RowCol; q++)
  {
    cout << nums[q] << "\n";
  }
  cout << endl;

}
