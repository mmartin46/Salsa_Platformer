#ifndef BASIC_CALCULATOR_H
#define BASIC_CALCULATOR_H
#include "Calculator.h"

// Basic_Calculator that performs basic arithmetic.
template <typename T>
class Basic_Calculator : public Calculator<T>
{
   private:
      T first_value;
   public:
      Basic_Calculator<T>() : first_value(0) {}
      Basic_Calculator<T>(T n) : first_value(n) {}
      void add(T val);  // Adds a value to the Basic_Calculator.
      void add(vector<T> vals); // Adds multiple values to the Basic_Calculator.
      void sub(T val);  // Subtract a value from the Basic_Calculator.
      void sub(vector<T> vals); // Subtracts values from the Basic_Calculator.
      void mul(T val); // Multiply a value to the Basic_Calculator.
      void mul(vector<T> vals); // Multiplies values to the Basic_Calculator.
      void div(T val); // Divide a value from the Basic_Calculator.
      void div(vector<T> vals); // Divide values from the Basic_Calculator.
      void exp(T val); // Raise the value of the Basic_Calculator by a certain power.
      inline void neg() { this->first_value = -this->first_value; }
      inline T get_value() const { return this->first_value; } // Returns the current value in the Basic_Calculator.
      inline void set_value(T val) { this->first_value = val; }; // Sets the current value in the Basic_Calculator.
      inline void reset() { this->first_value = 0; }

      Basic_Calculator<T> operator-(const Basic_Calculator<T> &rhs);
      Basic_Calculator<T> operator+(const Basic_Calculator<T> &rhs);
      Basic_Calculator<T> operator*(const Basic_Calculator<T> &rhs);
      Basic_Calculator<T> operator/(const Basic_Calculator<T> &rhs);
      
      Basic_Calculator<T>& operator++();
      Basic_Calculator<T> operator++(int);
      Basic_Calculator<T>& operator--();
      Basic_Calculator<T> operator--(int);

      Basic_Calculator<T>& operator=(const Basic_Calculator<T> &rhs);
      bool operator==(const Basic_Calculator<T> &rhs) const; 
      bool operator!=(const Basic_Calculator<T> &rhs) const;
      bool operator<(const Basic_Calculator<T> &rhs) const;
      bool operator<=(const Basic_Calculator<T> &rhs) const;
      bool operator>=(const Basic_Calculator<T> &rhs) const;
      bool operator>(const Basic_Calculator<T> &rhs) const;
};



#endif