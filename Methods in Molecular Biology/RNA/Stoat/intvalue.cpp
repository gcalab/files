/*****************************************************************************
 *                                                                           *
 *  This function returns the PAUP Matrix related integer of the float F.    *
 *                                                                           *
 *                                                                STOAT 2007 *
 *****************************************************************************/
  
#include <iostream>  // Fix for std::cout and std::endl
#include "in2ch.h"   // Ensure this is the correct header for in2ch()

char intvalue(float f, int cs, float fmin, float fmax)   
{  
  float a = 0; 
  char  r ='0';
  int   x = 0;
  float y = 0;
  int   i = 0;

  // Linear increase number;
  a = (fmax - fmin)/(cs);
  
  for(i=0; i < cs; ++i)
    {
      // The equation.
      y = fmin + a*i;
      // The interval test.
      if(f >= y  && f < y + a) {
	x = i;
      }
    }
  // This statement is not included in the above test.
  if(f == fmax) x = cs - 1;
  
  // Finds the corresponding char.
  r = in2ch(x);
  
  // Prints a warning if we end up with a value greater than fmax.
  if (fmin + a*(x) > fmax) {
    std::cout << " WARNING : Value of character statement is bigger than fmax!"
	      << std::endl;
  }
  return(r);
}
