// File: avl.h

#ifndef _AVL_H_
#define _AVL_H_

#include <iostream>
#include <stdlib.h>

#define DTYPE int

class AVLNode ;

class AVL {

  // friend class AVLNode ;
  friend ostream& operator<< (ostream& os, const AVL& T) ;

 public:

  AVL() ;
  AVL(const AVL& other) ;
  ~AVL() ;

  // inline member functions
  //
  bool empty() const { return nptr == NULL ; }  
  int height() const ;

  DTYPE* find(DTYPE x) ;
  void insert(DTYPE x) ;

  bool removeMax(DTYPE &answer) ;
  bool remove(DTYPE x, DTYPE &answer) ;

  void inorder() ;
  void inorder2() ;   // with parentheses
  void inorder3() ;   // with parentheses & height

  const AVL& operator=(const AVL& rhs) ;

  void dump() ;

 private:
  AVLNode *nptr ;

  void updateHeight() ;
  void fixImbalance() ;
  AVLNode *removeMaxNode() ;   // should be private

} ;


ostream& operator<< (ostream& os, const AVL& T) ;


class AVLNode {

  friend class AVL ;

 public:
  AVLNode() ;
  AVLNode(DTYPE x) ;

  ~AVLNode() ;

  void dump() ;

 private:
  DTYPE data ;
  int height ;
  AVL left, right ;
} ;



#endif
