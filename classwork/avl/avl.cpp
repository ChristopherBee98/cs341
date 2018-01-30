// File: avl.cpp

#include <iostream>
#include <string>
#include <assert.h>
using namespace std ;

#include "avl.h"


AVL::AVL() {
  nptr = NULL ;
}


AVL::AVL(const AVL& other) {
  if ( other.empty() ) {
    nptr = NULL ;
  } else {
    nptr = new AVLNode( *(other.nptr) ) ;
  }
}


AVL::~AVL() {
  //   cout << "AVL destructor, this = " << this << endl ;
  delete nptr ;
}


int AVL::height() const {
  return nptr ? nptr->height : 0 ;
}



void AVL::updateHeight() {
  int leftHeight = nptr->left.height() ;
  int rightHeight = nptr->right.height() ;
  nptr->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight) ;
}


void AVL::fixImbalance() {

  if ( empty() ) return ;

  AVLNode *root, *Lptr, *Rptr ; 
  AVLNode *LLptr, *LRptr, *RLptr, *RRptr ;
  AVLNode *LRLptr, *LRRptr, *RLLptr, *RLRptr ;
  int L_height, R_height, LL_height, LR_height, RL_height, RR_height ;

  L_height = nptr->left.height() ;
  R_height = nptr->right.height() ;

  assert( (L_height <= R_height + 2) && (R_height <= L_height + 2) ) ;

  if (L_height > R_height + 1 ) {
   
    root = nptr ;
    Lptr = root->left.nptr ;
    Rptr = root->right.nptr ;

    assert(Lptr != NULL) ;

    LLptr = Lptr->left.nptr ;
    LRptr = Lptr->right.nptr ;

    LL_height = Lptr->left.height() ; 
    LR_height = Lptr->right.height() ; 

    if ( LL_height >= LR_height ) {   // must break tie for LL

      // LL grandchild is tallest
      // Do "single" rotation

      nptr = Lptr ;
      nptr->right.nptr = root ;
      root->left.nptr = LRptr ;

      nptr->right.updateHeight() ;
      updateHeight() ;

    } else {

      // LR grandchild is tallest
      // Do "double" rotation

      assert(LRptr != NULL) ;

      LRLptr = LRptr->left.nptr ;
      LRRptr = LRptr->right.nptr ;

      nptr = LRptr ;
      nptr->left.nptr = Lptr ;
      nptr->right.nptr = root ;

      Lptr->right.nptr = LRLptr ;
      root->left.nptr = LRRptr ;

      nptr->left.updateHeight() ;
      nptr->right.updateHeight() ;
      updateHeight() ;

    }


  } else if (R_height > L_height + 1) {

    root = nptr ;
    Lptr = root->left.nptr ;
    Rptr = root->right.nptr ;

    assert(Rptr != NULL) ;

    RLptr = Rptr->left.nptr ;
    RRptr = Rptr->right.nptr ;

    RL_height = Rptr->left.height() ; 
    RR_height = Rptr->right.height() ; 

    if ( RR_height >= RL_height ) {  // must break tie for RR

      // RR grandchild is tallest
      // Do "single" rotation

      nptr = Rptr ;
      nptr->left.nptr = root ;
      root->right.nptr = RLptr ;

      nptr->left.updateHeight() ;
      updateHeight() ;

    } else {

      // RL grandchild is tallest
      // Do "double" rotation

      assert(RLptr != NULL) ;

      RLLptr = RLptr->left.nptr ;
      RLRptr = RLptr->right.nptr ;


      nptr = RLptr ;
      nptr->left.nptr = root ;
      nptr->right.nptr = Rptr ;

      root->right.nptr = RLLptr ;

      Rptr->left.nptr = RLRptr ;

      nptr->left.updateHeight() ;
      nptr->right.updateHeight() ;
      updateHeight() ;

    }

  } else {
    updateHeight() ;
  }
   
}



DTYPE *AVL::find(DTYPE x) {

  if ( empty() ) return NULL ;

  if (nptr->data == x) return &(nptr->data) ;

  if (nptr->data < x) return nptr->right.find(x) ;

  return  nptr->left.find(x) ;
}



void AVL::inorder() {

  if ( empty() ) return ;

  nptr->left.inorder() ;

  cout << nptr->data << " " ;

  nptr->right.inorder() ;

  return  ;
}


void AVL::inorder2() {

  if ( empty() ) return ;

  cout << "( " ;
  nptr->left.inorder2() ;

  cout << nptr->data << " " ;

  nptr->right.inorder2() ;
  cout << ") " ;

  return  ;
}


void AVL::inorder3() {

  if ( empty() ) return ;

  cout << "( " ;
  nptr->left.inorder3() ;

  cout << nptr->data << ":" << height() << " " ;

  nptr->right.inorder3() ;
  cout << ") " ;

  return  ;
}



void AVL::insert(DTYPE x) {
  if ( empty() ) {

    nptr = new AVLNode(x) ;

  } else if (nptr->data <= x) {  // allows duplicates

    nptr->right.insert(x) ;

  } else {

    nptr->left.insert(x) ;

  }

  fixImbalance() ;

}



AVLNode *AVL::removeMaxNode() {

  AVLNode *answer ;

  if ( empty() ) return NULL ;
  
  if (! nptr->right.empty() ) {

    answer = nptr->right.removeMaxNode() ;

  } else {

    answer = nptr ;
    nptr = nptr->left.nptr ;  // don't do deep copy!

    answer->left.nptr = NULL ;    // must unlink from tree o.w.
    answer->right.nptr = NULL ;   // destructor will kill subtrees
  }

  fixImbalance() ;
  return answer ;
   
}



bool AVL::remove(DTYPE x, DTYPE& answer) {
 
  if ( empty() ) return false ;
   
  bool success ;

  if (nptr->data < x) {
    success = nptr->right.remove(x, answer) ;
    fixImbalance() ;
    return success ;
  } 
   
  if (nptr->data > x) {
    success = nptr->left.remove(x, answer) ;
    fixImbalance() ;
    return success ;
  } 


  AVLNode *temp = nptr ;

  // otherwise, nptr->data == x
  if ( nptr->left.empty() ) {
    nptr = nptr->right.nptr ;

  } else if ( nptr->right.empty() ) {
    nptr = nptr->left.nptr ;

  } else {
    AVLNode *pred = nptr->left.removeMaxNode() ;
    assert( pred != NULL ) ;

    nptr = pred ;
    pred->left.nptr = temp->left.nptr ;
    pred->right.nptr = temp->right.nptr ;

  }

  answer = temp->data ;

  temp->left.nptr = NULL ;    // must unlink from tree o.w.
  temp->right.nptr = NULL ;   // destructor will kill subtrees
  delete temp ;

  fixImbalance() ;
  return true ;
}



bool AVL::removeMax(DTYPE &answer) {

  AVLNode *temp = removeMaxNode() ;

  if (temp == NULL) {
    return false ;
  } else {
    answer = temp->data ;
    delete temp ;
    return true ;
  }

}



const AVL& AVL::operator=(const AVL& rhs) {

  if (this == &rhs) return *this ;
   
  delete nptr ;

  if ( rhs.empty() ) {
    nptr = NULL ;
  } else {
    nptr = new AVLNode( *(rhs.nptr) ) ;
  }

  return *this ;
}



void AVL::dump() {


  //   cout << "***** AVL::dump()\n" ;
  //   cout << "      this = " << this << endl ; 
  //   cout << "      nptr = " << nptr << endl ; 

  if ( !empty() ) nptr->dump() ;
}


ostream& operator<< (ostream& os, const AVL& T) {
  os << T.nptr ; 
  return os ;
}


AVLNode::AVLNode() :
  height(0) {

  // do nothing else
}


AVLNode::AVLNode(DTYPE x) 
  : data(x), height(0) {

  // do nothing
}


AVLNode::~AVLNode() {

  // cout << "AVLNode destructor, this = " << this << endl ;

  // do nothing else

}


void AVLNode::dump() {
  cout << "===== AVLNode::dump()\n" ;
  cout << "      this = " << this << endl ; 
  cout << "      left = " << left << endl ; 
  cout << "      right= " << right << endl ; 
  cout << "      data = " << data << endl ;
  cout << "      height = " << height << endl ;
  left.dump() ;
  right.dump() ;
}

