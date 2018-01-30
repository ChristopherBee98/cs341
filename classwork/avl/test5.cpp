// File: test5.cpp

#include <iostream>
using namespace std ;

#include "bst.h"

int main() {

  BST T ;

  T.insert(14) ;

  T.insert(7) ;
  T.insert(3) ; T.insert(1) ; T.insert(4) ;
  T.insert(10) ; T.insert(9) ; T.insert(12) ;

  T.insert(25) ;
  T.insert(18) ; T.insert(15) ; T.insert(21) ;
  T.insert(32) ; T.insert(29) ; T.insert(44) ;

  T.dump() ;
  T.inorder2() ; cout << endl ;

  int n, answer ;

  n = 21  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

  n = 18  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

  n = 7  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

  n = 13  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

  n = 29  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

  n = 32  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

  n = 14  ;
  cout << "Remove " << n << endl ;
  if ( T.remove(n,answer) ) { 
    cout << "answer = " << answer << endl ;
  } else {
    cout << n << " not found\n" ;
  }
  T.inorder2() ; cout << endl ;

}
