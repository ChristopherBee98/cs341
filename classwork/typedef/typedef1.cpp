// File: typedef1.cpp
//
// CMSC 341 Spring 2017
//
// Simple example using typedef

#include <iostream>
using namespace std ;

// define a type called "uint" that is an alias
// for "unsigned int"
//
typedef unsigned int uint ;

int main() {

  // "uint" can be used to declare variables as unsigned int
  //
  uint n ;

  unsigned int m = 5 ;

  n = m ;
  
  cout << "n = " << n << endl ;
   
  return 0 ;
}
