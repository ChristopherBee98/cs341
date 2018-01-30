// File: fptr1.cpp
// 
// CMSC 341 Spring 2017
//
// Simple use of function pointers
//

#include <iostream>
using namespace std ;

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

  // Declare function pointer named "fptr" that can 
  // point to functions that take an int parameter
  // and returns an int.
  //
  int (*fptr)(int ) ;

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
