//Binary search tree definition file
//if errors make no sense, do LazyBST::BSTNode* for return a pointer to the node
//When finished with implementation, make sure to change ints to DTYPE that is defined in .h file (#define DTYPE int)
//Make sure to allow no duplicates, and to look over remove function

#include <iostream>
#include <string>
#include <assert.h>
using namespace std ;

#include "LazyBST.h"


LazyBST::LazyBST() {
  nptr = NULL ;
}


LazyBST::LazyBST(const LazyBST& other) {
  //checks for other tree if it's empty
  if ( other.empty() ) {
    nptr = NULL ;
  } else {
    nptr = new BSTNode( *(other.nptr) ) ;
  }
}


LazyBST::~LazyBST() {
  //   cout << "BST destructor, this = " << this << endl ;
  delete nptr ;
}


bool LazyBST::find(int key) {

  if ( empty() ) return false ;

  if (nptr->data == key) return true ;

  if (nptr->data < key) return nptr->right.find(key) ;
  //recursively calls find until the key is found or not found
  return  nptr->left.find(key) ;
}


void LazyBST::inorder() {

  if ( empty() ) return ;

  cout << "(" ;

  nptr->left.inorder() ;

  cout << nptr->data << ":" << nptr->height << ":" << nptr->size;

  nptr->right.inorder() ;

  cout << ")" ;

  return  ;
}


void LazyBST::inorder2() {

  if ( empty() ) return ;

  cout << "( " ;
  nptr->left.inorder2() ;

  cout << nptr->data << " " ;

  nptr->right.inorder2() ;
  cout << ") " ;

  return  ;
}



void LazyBST::insert(int key) {
  int dummy;
  if ( empty() ) {

    nptr = new BSTNode(key) ;
    nptr->height = 0;
    nptr->size = 1;

  } else {
    //dummy does nothing
    dummy = insertHelp(key, 0);
}
}

int LazyBST::insertHelp(int key, int heightCheck) {
  int increment;
  if (nptr != NULL) {
    if (nptr->height >= 4) {
      if (nptr->left.nptr->size >= 2*(nptr->right.nptr->size) || nptr->right.nptr->size <= 2*(nptr->left.nptr->size)) {
	int* array = new int[nptr->size];
	nptr = rebalance(array, 0, true);
      }
    }
  }

  if ( empty() ) {

    nptr = new BSTNode(key) ;
    nptr->height = 0;
    nptr->size = 1;
    return 1;

  } else if (nptr->data < key) {  // doesn't allow duplicates
    increment = nptr->right.insertHelp(key, heightCheck+1) ;
    nptr->size += 1;
    //makes sure if the height needs updating in a particular line of nodes/subtrees
    if (increment == 2) {
      if (nptr->height == heightCheck) {
	return 2;
      } else {
	nptr->height += 1;
	return 2;
      }
    } else {
      if (!nptr->left.empty() && !nptr->right.empty()) {
	return 1;
      } else {
	nptr->height += 1;
	return 2;
    }
    }
  } else if (nptr->data > key) {
    increment = nptr->left.insertHelp(key, heightCheck+1) ;
    nptr->size += 1;
    if (increment == 2) {
      if(nptr->height == heightCheck) {
	return 2;
      } else {
	nptr->height += 1;
	return 2;
      }
    } else {
      if (!nptr->left.empty() && !nptr->right.empty()) {
	return 1;
      } else {
	nptr->height += 1;
	return 2;
    }
    }
  } else {
    return 0;
  }
}


BSTNode *LazyBST::removeMaxNode() {


  if (empty()) return NULL ;
  
  if (! nptr->right.empty() ) {
    return nptr->right.removeMaxNode() ;
  }

  BSTNode *answer = nptr ;
  nptr = nptr->left.nptr ;  // don't do deep BST copy!

  answer->left.nptr = NULL ;    // must unlink from tree o.w.
  answer->right.nptr = NULL ;   // destructor will kill subtrees

  return answer ;
   
}



bool LazyBST::remove(int key) {
  int returnValue;
  if ( empty() ) return false ;

  returnValue = removeHelp(key, 0);
  
  if (returnValue == 0)
    return false;
  else
    return true;
}

int LazyBST::removeHelp(int key, int heightCheck) {
  int answer;
  int isGone;

  if (nptr->height >= 4) {
    if (nptr->left.nptr->size >= 2*(nptr->right.nptr->size) || nptr->right.nptr->size <= 2*(nptr->left.nptr->size)) {
      int* array = new int[nptr->size];
      nptr = rebalance(array, 0, true);
    }
  }

  if (empty()) return 0;

  if (nptr->data < key) {
    isGone = nptr->right.remove(key);
    if (isGone == 2) {  
      nptr->size -= 1;
      if (nptr->height == heightCheck) {
	return isGone;
      } else {
	nptr->height -= 1;
	return isGone;
      }
    } else if (isGone == 1) {
      nptr->size -= 1;
      if (!nptr->left.empty() && !nptr->right.empty()) {
	nptr->height -= 1;
	isGone = 2;
	return isGone;
      }
    }
    return isGone;
  }
  if (nptr->data > key) {
    isGone = nptr->left.remove(key);
    if (isGone == 2) {
      nptr->size -= 1;
      if (nptr->height == heightCheck) {
	return isGone;
      } else {
	nptr->height -= 1;
	return isGone;
      }
    } else if (isGone == 1) {
      nptr->size -= 1;
      if (!nptr->left.empty() && !nptr->right.empty()) {
        nptr->height -= 1;
        isGone = 2;
	return isGone;
      }
    }
    return isGone;
  }
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
  return 1 ;


}

bool LazyBST::removeMax(int &answer) {

  BSTNode *temp = removeMaxNode() ;

  if (temp == NULL) {
    return false ;
  } else {
    answer = temp->data ;
    delete temp ;
    return true ;
  }

}

BSTNode *LazyBST::rebalance(int rebalancedArray[], int sizeOfArray, bool firstStack) {
  //rebalance tree
  //checks if it's the first run through, in which case it grabs an array of the entire tree that needs rebalancing
  if (firstStack == true) {
    rebalancedArray = rebalanceArray(rebalancedArray, sizeOfArray);
    //deletes the tree so it doesn't overwrite any existing memory
    delete nptr;
  }
  if (sizeOfArray == 1) {
    nptr = new BSTNode(rebalancedArray[0]);
    nptr->size = 1;
    nptr->height = 0;
    nptr->left.nptr = NULL;
    nptr->right.nptr = NULL;
    delete[] rebalancedArray;
    return nptr;
  } else if (sizeOfArray == 2){
    nptr = new BSTNode(rebalancedArray[0]);
    nptr->size = 2;
    nptr->height = 1;
    nptr->left.nptr = NULL;
    nptr->right.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2), 1), (sizeOfArray/2), false);
  } else if (sizeOfArray == 3){
    nptr = new BSTNode(rebalancedArray[1]);
    nptr->size = 3;
    nptr->height = 1;
    nptr->left.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2), 0), (sizeOfArray/2), false);
    nptr->right.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2), 2), (sizeOfArray/2), false);
  } else if(sizeOfArray % 2 == 0) {
    nptr = new BSTNode(rebalancedArray[(sizeOfArray/2)-1]);
    nptr->size = sizeOfArray;
    nptr->height = (sizeOfArray/2)-1;
    nptr->left.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2)-1, 0), (sizeOfArray/2)-1, false);
    nptr->right.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2), (sizeOfArray/2)), (sizeOfArray/2), false);
  } else {
    nptr = new BSTNode(rebalancedArray[(sizeOfArray/2)]);
    nptr->size = sizeOfArray;
    nptr->height = (sizeOfArray/2)-1;
    nptr->left.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2), 0), (sizeOfArray/2), false);
    nptr->right.rebalance(ConstructArray(rebalancedArray, (sizeOfArray/2), (sizeOfArray/2)+1), (sizeOfArray/2), false);
  }
  delete[] rebalancedArray;
  return nptr;
}

int* LazyBST::rebalanceArray(int rebalancedArray[], int &sizeOfArray) {
  if (empty()) return NULL;
  nptr->left.rebalanceArray(rebalancedArray, sizeOfArray);
  rebalancedArray[sizeOfArray] = nptr->data;
  sizeOfArray++;
  nptr->right.rebalanceArray(rebalancedArray, sizeOfArray);

  return rebalancedArray;
}

int* LazyBST::ConstructArray(int array[], int size, int pos) {
  int i = 0;
  int *newArray = new int[size];
  while (i < size) {
    newArray[i] = array[pos+i];
    i++;
  }
  return newArray;
}

bool LazyBST::locate(const char *position, int& key) {
  //returns whether there is a node in a position of the LazyBST (const char * is like an array, but read only)
  bool check = false;
  int size = 0;
  int i = 0;
  string realPosition = "";
  string temp;
  while (check == false) {
    size++;
    if (position[i] == '\0') {
      check = true;
    } else {
      i++;
    }
  }
  for (int j = 0; j < size; j++)
    realPosition += position[j];
  if (size == 1 && nptr != NULL) {
    key = nptr->data;
    return true;
  }
  if (position[0] == 'L' && !nptr->left.empty()) {
    temp = realPosition.substr(1, size);
    return nptr->left.locate(temp.c_str(), key);
  } else if (position[0] == 'R' && !nptr->right.empty()) {
    temp = realPosition.substr(1, size);
    return nptr->right.locate(temp.c_str(), key);
  } else {
    return false;
  }
}


const LazyBST& LazyBST::operator=(const LazyBST& rhs) {

  if (this == &rhs) return *this ;
   
  delete nptr ;

  if ( rhs.empty() ) {
    nptr = NULL ;
  } else {
    nptr = new BSTNode( *(rhs.nptr) ) ;
  }

  return *this ;
}



void LazyBST::dump() {


  cout << "***** BST::dump()\n" ;
  cout << "      this = " << this << endl ; 
  cout << "      nptr = " << nptr << endl ; 

  if ( !empty() ) nptr->dump() ;
}


ostream& operator<< (ostream& os, const LazyBST& T) {
  os << T.nptr ; 
  return os ;
}


BSTNode::BSTNode() {
  // do nothing
}


BSTNode::BSTNode(int key) 
  : data(key) {

  // do nothing
}


BSTNode::~BSTNode() {

  //cout << "BSTNode destructor, this = " << this << endl ;

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
