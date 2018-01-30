// File: test3.cpp

#include <iostream>
using namespace std ;

#include "avl.h"

int main() {

  AVL T ;
  int answer ;

  for (int i=10 ; i < 2000000 ; i++) {
    T.insert(i) ;
  }

  cout << "Height of T = " << T.height() << endl  ;

  for (int i=10 ; i < 2000000 ; i++) {
    T.remove(i,answer) ;
  }
}
