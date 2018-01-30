// File: test3.cpp

#include <iostream>
using namespace std ;

#include "bst.h"

int main() {

  BST T ;

  T.insert(5) ;
  T.insert(7) ;
  T.insert(3) ;

  T.dump() ;
  T.inorder() ; cout << endl ;

  BSTNode *ptr ;

  ptr = T.removeMaxNode() ;
  cout << "*** returned node ***\n" ;
  ptr->dump() ;
  cout << "*********************\n" ;
  T.dump() ;
  T.inorder() ; cout << endl ;
  delete ptr ;

  ptr = T.removeMaxNode() ;
  cout << "*** returned node ***\n" ;
  ptr->dump() ;
  cout << "*********************\n" ;
  T.dump() ;
  T.inorder() ; cout << endl ;
  delete ptr ;

}
