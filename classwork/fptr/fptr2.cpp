// File: fptr2.cpp
//
// CMSC341 Spring 2017
//
// Using typedef and function pointers
//

#include <iostream>
using namespace std ;

// Define "function_ptr_t" to be the type of pointers
// to functions that take an int parameter and returns int.
//
typedef int (*function_ptr_t)(int) ;


// An example function
//
int add3(int x) {
  return  x + 3 ;
}


// Another function that has same
// signature as add3()
//
int add5(int x) {
  return x + 5 ;
}

int main() {

  // Declare function pointer using typedef
  // defined function pointer type
  //
  function_ptr_t fptr ;

  int n ;

  // use fptr to invoke add3
  //
  fptr = &add3 ; 
  n = fptr(8) ;

  cout << "n = " << n << endl ;   // should be 11

  // use fptr to invoke add5
  //
  fptr = &add5 ;
  n = fptr(8) ;

  cout << "n = " << n << endl ;   // should be 13

  return 0 ;
}
