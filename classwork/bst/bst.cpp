// File: bst.cpp

#include <iostream>
#include <string>
#include <assert.h>
using namespace std ;

#include "bst.h"


BST::BST() {
  nptr = NULL ;
}


BST::BST(const BST& other) {
  if ( other.empty() ) {
    nptr = NULL ;
  } else {
    nptr = new BSTNode( *(other.nptr) ) ;
  }
}


BST::~BST() {
  //   cout << "BST destructor, this = " << this << endl ;
  delete nptr ;
}


DTYPE *BST::find(DTYPE x) {

  if ( empty() ) return NULL ;

  if (nptr->data == x) return &(nptr->data) ;

  if (nptr->data < x) return nptr->right.find(x) ;

  return  nptr->left.find(x) ;
}



void BST::inorder() {

  if ( empty() ) return ;

  nptr->left.inorder() ;

  cout << nptr->data << " " ;

  nptr->right.inorder() ;

  return  ;
}



void BST::inorder2() {

  if ( empty() ) return ;

  cout << "( " ;
  nptr->left.inorder2() ;

  cout << nptr->data << " " ;

  nptr->right.inorder2() ;
  cout << ") " ;

  return  ;
}



void BST::insert(DTYPE x) {
  if ( empty() ) {

    nptr = new BSTNode(x) ;

  } else if (nptr->data <= x) {  // allows duplicates

    nptr->right.insert(x) ;

  } else {

    nptr->left.insert(x) ;

  }
}



BSTNode *BST::removeMaxNode() {


  if ( empty() ) return NULL ;
  
  if (! nptr->right.empty() ) {
    return nptr->right.removeMaxNode() ;
  }

  BSTNode *answer = nptr ;
  nptr = nptr->left.nptr ;  // don't do deep BST copy!

  answer->left.nptr = NULL ;    // must unlink from tree o.w.
  answer->right.nptr = NULL ;   // destructor will kill subtrees

  return answer ;
   
}



bool BST::remove(DTYPE x, DTYPE& answer) {
 
  if ( empty() ) return false ;


  if (nptr->data < x) return nptr->right.remove(x, answer) ;

  if (nptr->data > x) return  nptr->left.remove(x, answer) ;

  BSTNode *temp ;

  // otherwise, nptr->data == x
  if ( nptr->left.empty() ) {

    temp = nptr ;
    nptr = nptr->right.nptr ;

  } else if ( nptr->right.empty() ) {

    temp = nptr ;
    nptr = nptr->left.nptr ;

  } else {

    BSTNode *pred = nptr->left.removeMaxNode() ;
    assert( pred != NULL ) ;

    temp = nptr ;
    nptr = pred ;
    pred->left.nptr = temp->left.nptr ;
    pred->right.nptr = temp->right.nptr ;

  }

  answer = temp->data ;
  temp->left.nptr = NULL ;    // must unlink from tree o.w.
  temp->right.nptr = NULL ;   // destructor will kill subtrees
  delete temp ;
  return true ;


}



bool BST::removeMax(DTYPE &answer) {

  BSTNode *temp = removeMaxNode() ;

  if (temp == NULL) {
    return false ;
  } else {
    answer = temp->data ;
    delete temp ;
    return true ;
  }

}



const BST& BST::operator=(const BST& rhs) {

  if (this == &rhs) return *this ;
   
  delete nptr ;

  if ( rhs.empty() ) {
    nptr = NULL ;
  } else {
    nptr = new BSTNode( *(rhs.nptr) ) ;
  }

  return *this ;
}



void BST::dump() {


  cout << "***** BST::dump()\n" ;
  cout << "      this = " << this << endl ; 
  cout << "      nptr = " << nptr << endl ; 

  if ( !empty() ) nptr->dump() ;
}


ostream& operator<< (ostream& os, const BST& T) {
  os << T.nptr ; 
  return os ;
}


BSTNode::BSTNode() {
  // do nothing
}


BSTNode::BSTNode(DTYPE x) 
  : data(x) {

  // do nothing
}


BSTNode::~BSTNode() {

  // cout << "BSTNode destructor, this = " << this << endl ;

  // do nothing else

}


void BSTNode::dump() {
  cout << "===== BSTNode::dump()\n" ;
  cout << "      this = " << this << endl ; 
  cout << "      left = " << left << endl ; 
  cout << "      right= " << right << endl ; 
  cout << "      data = " << data << endl ;
  left.dump() ;
  right.dump() ;
}

