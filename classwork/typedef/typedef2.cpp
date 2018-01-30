// File: typedef2.cpp
//
// CMSC 341 Spring 2017
//
// Another example of typedef
//

#include <iostream>
using namespace std ;

// Define a "int_ptr_t" to be pointer to int
//
typedef int *int_ptr_t ;

int main() {

  // now we can use ptr1 and ptr2 to declare pointers to int
  //
  int_ptr_t ptr1, ptr2 ;

  int m = 5 ;

  ptr1 = &m ;
  ptr2 = &m ;
  *ptr2 = 15 ;

  cout << "*ptr1 = " << *ptr1 << endl ;
  cout << "*ptr2 = " << *ptr2 << endl ;
   
  return 0 ;
}
