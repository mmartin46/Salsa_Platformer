// Matthew 5:28
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

template <typename T>
class Calculator
{
   private:
      T first_value;
   public:
      virtual void add(T val) = 0;  // Adds a value to the Basic_Calculator.
      virtual void add(vector<T> vals) = 0; // Adds multiple values to the Basic_Calculator.
      virtual void sub(T val) = 0;  // Subtract a value from the Basic_Calculator.
      virtual void sub(vector<T> vals) = 0; // Subtracts values from the Basic_Calculator.
      virtual void mul(T val) = 0; // Multiply a value to the Basic_Calculator.
      virtual void mul(vector<T> vals) = 0; // Multiplies values to the Basic_Calculator.
      virtual void div(T val) = 0; // Divide a value from the Basic_Calculator.
      virtual void div(vector<T> vals) = 0; // Divide values from the Basic_Calculator.
      virtual void exp(T val) = 0; // Raise the value of the Basic_Calculator by a certain power.
};


#endif