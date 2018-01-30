// File: bst.h

#ifndef _BST_H_
#define _BST_H_

#include <iostream>
#include <stdlib.h>

#define DTYPE int

class BSTNode ;

class BST {

// friend class BSTNode ;
friend ostream& operator<< (ostream& os, const BST& T) ;

public:

BST() ;
BST(const BST& other) ;
~BST() ;

bool empty() const { return nptr == NULL ; }  // inline

DTYPE* find(DTYPE x) ;
void insert(DTYPE x) ;

bool removeMax(DTYPE &answer) ;
BSTNode *removeMaxNode() ;   // should be private
bool remove(DTYPE x, DTYPE &answer) ;

void inorder() ;
void inorder2() ;   // with parentheses

const BST& operator=(const BST& rhs) ;

void dump() ;

private:
BSTNode *nptr ;


} ;


ostream& operator<< (ostream& os, const BST& T) ;


class BSTNode {

friend class BST ;

public:
BSTNode() ;
BSTNode(DTYPE x) ;

~BSTNode() ;

void dump() ;

private:
DTYPE data ;
BST left, right ;
} ;



#endif
