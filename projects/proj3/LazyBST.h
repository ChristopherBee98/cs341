//Binary tree header file

#ifndef _BST_H_
#define _BST_H_

#include <iostream>
#include <stdlib.h>

class BSTNode ;

class LazyBST {

  // friend class BSTNode ;
  friend std::ostream& operator<< (std::ostream& os, const LazyBST& T) ;

 public:

  LazyBST() ;
  ~LazyBST() ;
  LazyBST(const LazyBST& other) ;

  bool empty() const { return nptr == NULL ; }  // inline

  bool find(int key) ;
  void insert(int key) ;
  int insertHelp(int key, int heightCheck) ;

  bool removeMax(int &answer) ;
  BSTNode *removeMaxNode() ;   // should be private
  bool remove(int key) ;
  int removeHelp(int key, int heightCheck) ;

  void inorder() ;
  void inorder2() ;   // with parentheses
  BSTNode *rebalance(int rebalancedArray[], int sizeOfArray, bool firstStack) ;
  int* rebalanceArray(int rebalancedArray[], int &sizeOfArray);
  int* ConstructArray(int array[], int size, int pos);
  bool locate(const char *position, int& key) ;

  const LazyBST& operator=(const LazyBST& rhs) ;

  void dump() ;

 private:
  BSTNode *nptr ;


} ;


std::ostream& operator<< (std::ostream& os, const LazyBST& T) ;


class BSTNode {

  friend class LazyBST ;

 public:
  BSTNode() ;
  ~BSTNode() ;
  BSTNode(int key) ;

  void dump() ;

 private:
  int data ;
  LazyBST left, right ;
  int height, size;
} ;



#endif
