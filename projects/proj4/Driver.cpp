//When compiling, use driver.cpp only
//Look back at project 2 for function pointers
#include "MinMaxHeap.h"
#include "NoCopyString.h"
#include <set>

template <typename T>
void checkAgainstSet(MinMaxHeap<T>& H, multiset<T>& origS) {
  multiset<T> S ;  
  typename multiset<T>::iterator it ;

  int n = H.size() ;
  int pos ;
  int key ;

  bool passed = true ;

  S = origS ;  // make copy

  if ( n != S.size() ) {
    passed = false ;
    cout << "Heap and multiset sizes do not match!\n" ;
    cout << "   Heap size = " << n << endl ;
    cout << "   Set size  = " << S.size() << endl ;
  }


  for (int i=1 ; i <= n ; i++) {
    H.locateMin(i,key,pos) ;
    it = S.find(key) ;
    if (it == S.end()) {
      passed = false ;
      cout << "Key " << key << " in the min-heap but not in the multiset.\n" ;
    } else {
      S.erase(it) ;
    }
  }

  if (S.size() > 0) {
    cout << "There are " << S.size() 
	 << " items in multiset that not in the min-heap.\n" ;
    for (it=S.begin() ; it != S.end() ; it++) {
      cout << *it << " " ;
    }
    cout << endl ;
  }


  S = origS ;  // start over

  for (int i=1 ; i <= n ; i++) {
    H.locateMax(i,key,pos) ;
    it = S.find(key) ;
    if (it == S.end()) {
      passed = false ;
      cout << "Key " << key << " in the max-heap but not in the multiset.\n" ;
    } else {
      S.erase(it) ;
    }
  }

  if (S.size() > 0) {
    cout << "There are " << S.size() 
	 << " items in multiset that not in the max-heap.\n" ;
    for (it=S.begin() ; it != S.end() ; it++) {
      cout << *it << " " ;
    }
    cout << endl ;
  }

  if (passed) {
    cout << "Passed check against multiset\n" ;
  } else {
    cout << "***Failed check against multiset\n" ;
  }

}

template <typename T>
void sanityCheck(MinMaxHeap<T>& H) {
  int n = H.size() ;
  T minKey, maxKey, leftKey, rightKey ;
  int minPos, maxPos, pos ;
   
  bool passed=true ;

  cout << "Sanity Check minHeap...\n" ;

  for (int i=1 ; i<=n ; i++) {
    H.locateMin(i,minKey,maxPos) ; 
    H.locateMax(maxPos,maxKey,minPos) ; 
      
    if (minKey != maxKey) {
      passed = false ;
      cout << "Key mismatch at i = " << i << ": "
	   << "minKey = " << minKey << ", " 
	   << "minPos = " << minPos << ", " 
	   << "maxKey = " << maxKey << ", " 
	   << "maxPos = " << maxPos 
	   << endl ;
    }

    if (minPos != i) {
      passed = false ;
      cout << "Position mismatch at i = " << i << ": "
	   << "minKey = " << minKey << ", " 
	   << "minPos = " << minPos << ", " 
	   << "maxKey = " << maxKey << ", " 
	   << "maxPos = " << maxPos 
	   << endl ;
    }

    if (2*i <= n) {  // has left child
      H.locateMin(2*i,leftKey,pos) ;
      if (leftKey < minKey) {  // left child smaller than root 
	passed = false ;
	cout << "Bad heap condition at i = " << i << ": "
	     << "root key = " << minKey << ", " 
	     << "left key = " << leftKey 
	     << endl ;
      }
    }  

    if (2*i+1 <= n) {  // has right child
      H.locateMin(2*i+1,rightKey,pos) ;
      if (rightKey < minKey) {  // right child smaller than root 
	passed = false ;
	cout << "Bad heap condition at i = " << i << ": "
	     << "root key = " << minKey << ", " 
	     << "right key = " << rightKey 
	     << endl ;
      }
    }  

  }  // end of for (...)


  cout << "Sanity Check maxHeap...\n" ;

  for (int i=1 ; i<=n ; i++) {
    H.locateMax(i,maxKey,minPos) ; 
    H.locateMin(minPos,minKey,maxPos) ; 
      
    if (minKey != maxKey) {
      passed = false ;
      cout << "Key mismatch at i = " << i << ": "
	   << "minKey = " << minKey << ", " 
	   << "minPos = " << minPos << ", " 
	   << "maxKey = " << maxKey << ", " 
	   << "maxPos = " << maxPos 
	   << endl ;
    }

    if (maxPos != i) {
      passed = false ;
      cout << "Position mismatch at i = " << i << ": "
	   << "minKey = " << minKey << ", " 
	   << "minPos = " << minPos << ", " 
	   << "maxKey = " << maxKey << ", " 
	   << "maxPos = " << maxPos 
	   << endl ;
    }

    if (2*i <= n) {  // has left child
      H.locateMax(2*i,leftKey,pos) ;
      if (leftKey > maxKey) {  // left child bigger than root 
	passed = false ;
	cout << "Bad heap condition at i = " << i << ": "
	     << "root key = " << maxKey << ", " 
	     << "left key = " << leftKey 
	     << endl ;
      }
    }  

    if (2*i+1 <= n) {  // has right child
      H.locateMax(2*i+1,rightKey,pos) ;
      if (rightKey > maxKey) {  // right child bigger than root 
	passed = false ;
	cout << "Bad heap condition at i = " << i << ": "
	     << "root key = " << maxKey << ", " 
	     << "right key = " << rightKey 
	     << endl ;
      }
    }  
  }  // end of for (...)


  if (passed) {
    cout << "Passed sanityCheck().\n" ;
  } else {
    cout << "*** Failed sanityCheck().\n" ;
  }
}

int main() {
  
  MinMaxHeap<int> H(25);
  H.insert(5);
  H.insert(10);
  H.insert(7);
  H.insert(9);
  H.insert(6);
  H.insert(2);
  H.insert(16);
  H.insert(12);
  H.insert(11);
  H.insert(16);
  cout << "Initial MinMax Heap\n" ;
  H.insert(8); 
  cout << "After inserting 8\n" ;
  H.dump() ;

  sanityCheck(H) ;
  
  MinMaxHeap<int> *Gptr = new MinMaxHeap<int>(H) ;
  cout << "copied MinMax Heap\n" ;
  Gptr->dump();
  delete Gptr;

  MinMaxHeap<int> K1(11);
  K1.insert(29) ;
  K1.insert(24) ;
  K1.insert(23) ;
  K1.insert(22) ;
  K1.insert(28) ;
  K1.insert(26) ;
  K1.insert(30) ;
  K1.insert(21) ;
  K1.insert(25) ;
  K1.insert(27) ;

  K1.dump();
  MinMaxHeap<int> K2(4);
  K2.insert(43);
  K2.insert(42);
  K2.insert(44);
  K2.insert(41);

  K2.dump();
  K2 = K1;

  K2.dump();

  int answer ;
  H.dump();
  answer = H.deleteMin() ;
  cout << "\n\nMin item " << answer << " removed from MinMax Heap\n" ;

  H.dump() ;
  sanityCheck(H) ;
  answer = H.deleteMax() ;
  cout << "\n\nMax item " << answer << " removed from MinMax Heap\n" ;

  H.dump() ;
  sanityCheck(H) ;
  /*
  MinMaxHeap<NoCopyString> H(9) ;

  const char* myData[9] = {
    "jkl", "yz_", "abc",
    "mno", "vwx", "pqr",
    "ghi", "def", "stu" 
  } ;

  H.insert(myData[0]) ;
  H.insert(myData[1]) ;
  H.insert(myData[2]) ;
  H.insert(myData[3]) ;
  H.insert(myData[4]) ;
  H.insert(myData[5]) ;
  H.insert(myData[6]) ;
  H.insert(myData[7]) ;
  H.insert(myData[8]) ;

  H.deleteMax();
  cout << "After deletion of max" << endl;
  H.dump();
  */
  //H.dump() ;
  //sanityCheck(H) ;
  /*
  srand(4178012351) ;

  MinMaxHeap<int> H(1000) ;
  multiset<int> S ;
  multiset<int>::iterator it ;
  int key ;
  //bool passed ;

  for (int i=0 ; i < 500 ; i++) {
    key = rand() % 100 ;
    // cout << "Insert key = " << key << endl ;
    H.insert(key) ;
    S.insert(key) ;
  }
  sanityCheck(H) ;
  checkAgainstSet(H,S) ;
  */
  MinMaxHeap<int> D(25);
  try {
    D.deleteMin();
    for (int i = 1; i <= 26; i++)
      D.insert(i);
  }
  catch(string error) {
    cout << error << endl;
  }
}
