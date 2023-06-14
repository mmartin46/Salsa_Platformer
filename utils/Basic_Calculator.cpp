#include "Basic_Calculator.h"

template <typename T>
Basic_Calculator<T> Basic_Calculator<T>::operator*(const Basic_Calculator<T> &rhs)
{
   this->mul(rhs.get_value());
   return *this; 
}

template <typename T>
Basic_Calculator<T> Basic_Calculator<T>::operator/(const Basic_Calculator<T> &rhs)
{
   this->div(rhs.get_value());
   return *this;
}
template <typename T>
Basic_Calculator<T>& Basic_Calculator<T>::operator=(const Basic_Calculator<T> &rhs)
{
   if (this != &rhs)
   {
      this->set_value(rhs.get_value());
   }
   return *this;
}

template <typename T>
Basic_Calculator<T>& Basic_Calculator<T>::operator++()
{
   this->add(1);
   return *this;
}
template <typename T>
Basic_Calculator<T> Basic_Calculator<T>::operator++(int)
{
   Basic_Calculator<T> copy = *this;
   copy.add(1);
   return copy;  
}

template <typename T>
Basic_Calculator<T>& Basic_Calculator<T>::operator--()
{
   this->sub(1);
   return *this;
}
template <typename T>
Basic_Calculator<T> Basic_Calculator<T>::operator--(int)
{
   Basic_Calculator<T> copy = *this;
   copy.sub(1);
   return copy;
}



/* EQUIVALENCE */


template <typename T>
bool Basic_Calculator<T>::operator==(const Basic_Calculator<T> &rhs) const
{
   if (this->get_value() != rhs.get_value())
   {
      return false;
   }
   return true;
}
template <typename T>
bool Basic_Calculator<T>::operator!=(const Basic_Calculator<T> &rhs) const
{
   return !(*this == rhs);
}
template <typename T>
bool Basic_Calculator<T>::operator<(const Basic_Calculator<T> &rhs) const
{
   if (this->get_value() >= rhs.get_value())
   {
      return false;
   }
   return true;   
}
template <typename T>
bool Basic_Calculator<T>::operator>=(const Basic_Calculator<T> &rhs) const
{
   return !(*this < rhs);
}
template <typename T>
bool Basic_Calculator<T>::operator>(const Basic_Calculator<T> &rhs) const
{
   if (this->get_value() <= rhs.get_value())
   {
      return false;
   }
   return true;   
}
template <typename T>
bool Basic_Calculator<T>::operator<=(const Basic_Calculator<T> &rhs) const
{
   return !(*this > rhs);
}





// Only subtracts calculator values of the same type.
template <typename T>
Basic_Calculator<T> Basic_Calculator<T>::operator-(const Basic_Calculator<T> &rhs)
{
   Basic_Calculator<T> cal(*this);
   cal.sub(rhs.get_value());
   return cal;
}

// Only adds calculator values of the same type.
template <typename T>
Basic_Calculator<T> Basic_Calculator<T>::operator+(const Basic_Calculator<T> &rhs)
{
   Basic_Calculator<T> cal(*this);
   cal.add(rhs.get_value());
   return cal;
}


// Raise the value of the Basic_Calculator by a certain power.
template <typename T>
void Basic_Calculator<T>::exp(T val)
{
   this->first_value = static_cast<T>(pow(static_cast<double>(this->first_value), static_cast<double>(val)));
}
// Divide a value from the Basic_Calculator.
template <typename T>
void Basic_Calculator<T>::div(T val)
{
   this->first_value /= val;
}
// Divide values from the Basic_Calculator.
// Params: vector<T>
template <typename T>
void Basic_Calculator<T>::div(vector<T> vals)
{
   typename vector<T>::const_pointer ptr, end = vals.data() + vals.size();
   for (T *ptr = vals.data(); ptr < end; ++ptr)
   {
      this->first_value /= *ptr;
   }
}

/* MULTIPLICATION */

// Multiply a value to the Basic_Calculator.
template <typename T>
void Basic_Calculator<T>::mul(T val)
{
   this->first_value *= val;
}
// Multiplies values to the Basic_Calculator.
// Params: vector<T>
template <typename T>
void Basic_Calculator<T>::mul(vector<T> vals)
{
   typename vector<T>::const_pointer ptr, end = vals.data() + vals.size();
   for (T *ptr = vals.data(); ptr < end; ++ptr)
   {
      this->first_value *= *ptr;
   }
}

/* ADDITION */

// Adds a value to the Basic_Calculator.
template <typename T>
void Basic_Calculator<T>::add(T val)
{
   this->first_value += val;
}

// Adds multiple values to the Basic_Calculator.
// Params: vector<T>
template <typename T>
void Basic_Calculator<T>::add(vector<T> vals)
{
   typename vector<T>::const_pointer ptr, end = vals.data() + vals.size();
   for (T *ptr = vals.data(); ptr < end; ++ptr)
   {
      this->first_value += *ptr;
   }
}

/* SUBTRACTION */

// Subtract a value from the Basic_Calculator.
template <typename T>
void Basic_Calculator<T>::sub(T val)
{
   this->first_value -= val;
}
// Subtracts values from the Basic_Calculator.
// Params: vector<T>
template <typename T>
void Basic_Calculator<T>::sub(vector<T> vals)
{
   typename vector<T>::const_pointer ptr, end = vals.data() + vals.size();
   for (T *ptr = vals.data(); ptr < end; ++ptr)
   {
      this->first_value -= *ptr;
   }
}


/*
// FACTORY METHOD
template <typename T>
class Calculator_Factory
{
   private:
      Calculator<T>* calc;
   public:
      Calculator<T>* getCalculator(string s);
      ~Calculator_Factory<T>();
};

// Generates a calculator.
template <typename T>
Calculator<T>* Calculator_Factory<T>::getCalculator(string s)
{
   if (s == "BASIC")
   {
      calc = new Basic_Calculator<T>();
   }
   else
   {
      calc = NULL;
   }
   return calc;
}

template <typename T>
Calculator_Factory<T>::~Calculator_Factory<T>()
{
   if (calc)
   {
      delete calc;
      calc = NULL;
   }
}
*/