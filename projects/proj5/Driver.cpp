//Tester class for hash tables
#include "words.h"
#include "HashTable.h"
#include <set>
#include <cstdlib>

bool isInSet(set<string>& S, const char * str) ;
void addToSet(set<string>& S, const char *str) ;
void removeFromSet(set<string>& S, const char *str) ;
void sanityCheck(HashTable& T, set<string> S) ;

int main() {
  //put some testing here
  HashTable T(107);
  T.insert("tributes") ;// 21
  T.insert("skulduggery") ;// 22
  T.insert("convulse") ;// 23
  T.insert("frothed") ;// 24
  T.insert("horrify") ;// 25
  T.insert("blackmailers") ;// 26
  T.insert("defenestrated") ;// 27
  T.insert("garrison") ;// 23 -> 28
  T.insert("lidless") ;// 22 -> 29
  T.insert("lidless");
  // T.insert("eye") ;            // 21 -> 30

  cout << "----------------------------------------------------\n" ;
  cout << "Original hash table\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
  cout << "\n\nDo some find() and remove() operations...\n\n" ;

  const char *str ;
  char *ptr ;
  bool found ;

  if( T.find(str="skulduggery") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="lidless") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="defenestrated") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="peircing") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  ptr = T.remove(str="garrison") ;
  if (ptr == NULL) {
    cout << "String " << str << " not found, not removed\n" ;
  } else {
    cout << "Removed string = " << ptr << endl ;
    free(ptr) ;
  }

  ptr = T.remove(str="infractions") ;
  if (ptr == NULL) {
    cout << "String " << str << " not found, not removed\n" ;
  } else {
    cout << "Removed string = " << ptr << endl ;
    free(ptr) ;
  }

  if( T.find(str="garrison") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="lidless") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  cout << "\n\n" ;
  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after finds and removes\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
   
  cout << "\n\nNext insert should reuse DELETED slots...\n\n" ;

  T.insert("undying") ;        // 25 -> 28 

  cout << "\n\n" ;
  cout << "----------------------------------------------------\n" ;
  cout << "Final hash table\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
  //test2
  // File: test2.cpp
  //
  // Insertions trigger rehashing.
  //
  // Clusters are moved until old hash table is empty
  // and we revert to having only 1 hash table.
  //
  HashTable T2(107);
    T2.insert("undulation") ;     //  9
    T2.insert("impertinent") ;    // 10
    T2.insert("maladies") ;       // 10 -> 11
    T2.insert("dominions") ;// 12

    T2.insert("waspish") ;        // 52
    T2.insert("wildebeest") ;     // 52 -> 53
    T2.insert("reaction") ;       // 52 -> 54

    T2.insert("pawns") ;          // 43
    T2.insert("vacuously") ;      // 43 -> 44
    T2.insert("firth") ;          // 43 -> 45
    T2.insert("manipulator") ;    // 43 -> 46
    T2.insert("dreariness") ;     // 43 -> 47

    T2.insert("insanity") ;       // 105
    T2.insert("enthronement") ;   // 105 -> 106
    T2.insert("falsifiers") ;     // 105 -> 0
    T2.insert("ignominious") ;    // 105 -> 1
    T2.insert("mummified") ;      // 105 -> 2

    T2.insert("tributes") ;// 21
    T2.insert("skulduggery") ;// 22
    T2.insert("convulse") ;// 23
    T2.insert("frothed") ;// 24
    T2.insert("horrify") ;// 25
    T2.insert("blackmailers") ;// 26
    T2.insert("defenestrated") ;// 27
    T2.insert("garrison") ;// 23 -> 28
    T2.insert("lidless") ;// 22 -> 29

    cout << "----------------------------------------------------\n" ;
    cout << "Original hash table\n" ;
    cout << "----------------------------------------------------\n" ;
    T2.dump() ;
    cout << "----------------------------------------------------\n" ;

    cout << "\n\nInserting \"eye\" should trigger rehash\n\n" ;
    T2.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


    cout << "----------------------------------------------------\n" ;
    cout << "Hash table after rehash triggered\n" ;
    cout << "----------------------------------------------------\n" ;
    T2.dump() ;
    cout << "----------------------------------------------------\n" ;

    cout << "\n\nSearch for \"manipulator\" should move cluster in slots 43-47.\n\n" ;
    T2.find("manipulator") ;            // 21 -> 30, should trigger inc. rehash

    cout << "----------------------------------------------------\n" ;
    cout << "Hash table after cluster 43-47 moved.\n" ;
    cout << "----------------------------------------------------\n" ;
    T2.dump() ;
    cout << "----------------------------------------------------\n" ;

    cout << "\n\nDo some finds, inserts and removes\n\n" ;

    const char *str2 ;
    char *ptr2 ;

    if( T2.find(str2="zip") ) {
      cout << "Found " << str2 << endl ;
    } else {
      cout << "Did not find " << str2 << endl ;
    }

    if( T.find(str2="spaceflight") ) {
      cout << "Found " << str2 << endl ;
    } else {
      cout << "Did not find " << str2 << endl ;
    }
    /*
   ptr = T.remove(str="maladies") ;
   if (ptr == NULL) {
   cout << "String " << str << " not found, not removed\n" ;
   } else {
   cout << "Removed string = " << ptr << endl ;
      free(ptr) ;
   }

   ptr = T.remove(str="wildebeest") ;
   if (ptr == NULL) {
   cout << "String " << str << " not found, not removed\n" ;
   } else {
   cout << "Removed string = " << ptr << endl ;
      free(ptr) ;
   }
    */
    if( T.find(str2="frothed") ) {
      cout << "Found " << str2 << endl ;
    } else {
      cout << "Did not find " << str2 << endl ;
    }

    T2.insert("wildcat") ;        // 18 (new table)
    T2.insert("weightlessness") ; // 69 (new table)
    T2.insert("sorceress") ;      // 33 (new table)
    T2.insert("enchantress") ;    // 33 (new table) really.


    cout << "----------------------------------------------------\n" ;
    cout << "Hash table after more insertions.\n" ;
    cout << "----------------------------------------------------\n" ;
    T2.dump() ;
    cout << "----------------------------------------------------\n" ;

    cout << "\n\nA find on \"ignominious\" + 1 more operation should cause the tables to consolidate down to one table. \n\n" ;

    if( T2.find(str2="ignominious") ) {
      cout << "Found " << str2 << endl ;
    } else {
      cout << "Did not find " << str2 << endl ;
    }

    if( T2.find(str2="reaction") ) {
      cout << "Found " << str2 << endl ;
    } else {
      cout << "Did not find " << str2 << endl ;
    }


    cout << "----------------------------------------------------\n" ;
    cout << "Hash table after wrap up.\n" ;
    cout << "----------------------------------------------------\n" ;
    T2.dump() ;
    cout << "----------------------------------------------------\n" ;
  //test3
  // File: test3.cpp
  //
  // Insertions triggers incremental rehashing
  // Further insertions make us give up on 
  // rehashing and consolidate items into a single
  // hash table.
  //
    HashTable T3(107);
    T3.insert("undulation") ;     //  9
    T3.insert("impertinent") ;    // 10
    T3.insert("maladies") ;       // 10 -> 11
    T3.insert("dominions") ;// 12

    T3.insert("waspish") ;        // 52
    T3.insert("wildebeest") ;     // 52 -> 53
    T3.insert("reaction") ;       // 52 -> 54

    T3.insert("pawns") ;          // 43
    T3.insert("vacuously") ;      // 43 -> 44
    T3.insert("firth") ;          // 43 -> 45
    T3.insert("manipulator") ;    // 43 -> 46
    T3.insert("dreariness") ;     // 43 -> 47

    T3.insert("insanity") ;       // 105
    T3.insert("enthronement") ;   // 105 -> 106
    T3.insert("falsifiers") ;     // 105 -> 0
    T3.insert("ignominious") ;    // 105 -> 1
    T3.insert("mummified") ;      // 105 -> 2

    T3.insert("tributes") ;// 21
    T3.insert("skulduggery") ;// 22
    T3.insert("convulse") ;// 23
    T3.insert("frothed") ;// 24
    T3.insert("horrify") ;// 25
    T3.insert("blackmailers") ;// 26
    T3.insert("defenestrated") ;// 27
    T3.insert("garrison") ;// 23 -> 28
    T3.insert("lidless") ;// 22 -> 29
    T3.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


    cout << "----------------------------------------------------\n" ;
    cout << "Hash table after rehash triggered\n" ;
    cout << "----------------------------------------------------\n" ;
    T3.dump() ;
    cout << "----------------------------------------------------\n" ;

    cout << "\n\nDo some insertions to make long linear probe sequence in the second table.\n\n" ;

    T3.insert("wildcat") ;        // 18 (new table)
    T3.insert("weightlessness") ; // 69 (new table)
    T3.insert("sorceress") ;      // 33 (new table)
    T3.insert("enchantress") ;    // 33 (new table) really.
    T3.insert("witchery") ;       // 67 -> 68 (new table)
    T3.insert("heliosphere") ;    // 67 -> 72 (new table)
    T3.insert("obstruct") ;       // 67 -> 73 (new table)

    cout << "----------------------------------------------------\n" ;
    cout << "Hash table insertions.\n" ;
    cout << "----------------------------------------------------\n" ;
    T3.dump() ;
    cout << "----------------------------------------------------\n" ;

    cout << "\n\nOne more insertion in slot 67 should make us give up on rehashing.\n\n" ;
    T3.insert("peripatetic") ;    // 67 -> 77 (new table)

    cout << "----------------------------------------------------\n" ;
    cout << "Hash table giving up on rehashing.\n" ;
    cout << "----------------------------------------------------\n" ;
    T3.dump() ;
    cout << "----------------------------------------------------\n" ;

  
  //test4
    HashTable T4(883) ;
    set<string> S ;
    char *str3 ;

    // insert 900 words
    for (int i=100 ; i < 1000 ; i++) {
      T4.insert(words[i]) ;  addToSet(S,words[i]) ;
    }

    // remove 100 words
    for (int i=400 ; i < 500 ; i++) {
      str3 = T4.remove(words[i]) ;  
      free(str3) ;
      removeFromSet(S,words[i]) ;
    }

    // insert 10,000 words
    for (int i=30000 ; i < 40000 ; i++) { 
      T4.insert(words[i]) ;  addToSet(S,words[i]) ;
    }

    // remove 2000 words + try to remove 2000 not in table
    for (int i=28000 ; i < 32000 ; i++) {
      str3 = T4.remove(words[i]) ;  
      free(str3) ;
      removeFromSet(S,words[i]) ;
    }

    // now do 10,000 random finds
    int r ;
    int Tcount = 0 ;
    int Scount = 0 ;
    srand(843150) ;
    for (int i=0 ; i<10000 ; i++) {
      r = rand() % numWords ;
      if ( T4.find(words[r]) ) Tcount++ ;
      if ( isInSet(S,words[r]) ) Scount++ ;
    }

    if (Tcount == Scount) {
      cout << "Passed random find() tests: "
           << "Tcount = " << Tcount << ", "
	   << "Scount = " << Scount << endl ;
    } else {
      cout << "***Failed random find() tests: "
           << "Tcount = " << Tcount << ", "
	   << "Scount = " << Scount << endl ;
    }

    sanityCheck(T4,S) ;

    //test5
    HashTable H(107) ;

    H.insert("undulation") ;     //  9
    H.insert("impertinent") ;    // 10
    H.insert("maladies") ;       // 10 -> 11
    H.insert("dominions") ;// 12

    H.insert("waspish") ;        // 52
    H.insert("wildebeest") ;     // 52 -> 53
    H.insert("reaction") ;       // 52 -> 54

    H.insert("pawns") ;          // 43
    H.insert("vacuously") ;      // 43 -> 44
    H.insert("firth") ;          // 43 -> 45
    H.insert("manipulator") ;    // 43 -> 46
    H.insert("dreariness") ;     // 43 -> 47

    H.insert("insanity") ;       // 105
    H.insert("enthronement") ;   // 105 -> 106
    H.insert("falsifiers") ;     // 105 -> 0
    H.insert("ignominious") ;    // 105 -> 1
    H.insert("mummified") ;      // 105 -> 2

    H.insert("tributes") ;// 21
    H.insert("skulduggery") ;// 22
    H.insert("convulse") ;// 23
    H.insert("frothed") ;// 24
    H.insert("horrify") ;// 25
    H.insert("blackmailers") ;// 26
    H.insert("defenestrated") ;// 27
    H.insert("garrison") ;// 23 -> 28
    H.insert("lidless") ;// 22 -> 29
    H.insert("eye") ;            // 21 -> 30, should trigger inc. rehash

    set<string> S2 ;

    addToSet(S2,"undulation") ;
    addToSet(S2,"impertinent") ;
    addToSet(S2,"maladies") ;
    addToSet(S2,"dominions") ;
    addToSet(S2,"waspish") ;
    addToSet(S2,"wildebeest") ;
    addToSet(S2,"reaction") ;
    addToSet(S2,"pawns") ;
    addToSet(S2,"vacuously") ;
    addToSet(S2,"firth") ;
    addToSet(S2,"manipulator") ;
    addToSet(S2,"dreariness") ;
    addToSet(S2,"insanity") ;
    addToSet(S2,"enthronement") ;
    addToSet(S2,"falsifiers") ;
    addToSet(S2,"ignominious") ;
    addToSet(S2,"mummified") ;
    addToSet(S2,"tributes") ;
    addToSet(S2,"skulduggery") ;
    addToSet(S2,"convulse") ;
    addToSet(S2,"frothed") ;
    addToSet(S2,"horrify") ;
    addToSet(S2,"blackmailers") ;
    addToSet(S2,"defenestrated") ;
    addToSet(S2,"garrison") ;
    addToSet(S2,"lidless") ;
    addToSet(S2,"eye") ;

    cout << "\nCheck original hash table T...\n" ;
    sanityCheck(H,S2) ;

    HashTable *T2ptr = new HashTable(H) ;
    HashTable H2(*T2ptr) ;

    cout << "\nCheck hash tables T, *T2ptr & T3 after copying...\n" ;
    sanityCheck(H,S2) ;
    sanityCheck(*T2ptr,S2) ;
    sanityCheck(H2,S2) ;

    delete T2ptr ;
    cout << "\nCheck hash tables T & T3 after deleting T2ptr...\n" ;
    sanityCheck(H,S2) ;
    sanityCheck(H2,S2) ;


    HashTable X(107) ;

    X.insert("undulation") ;     //  9
    X.insert("impertinent") ;    // 10
    X.insert("maladies") ;       // 10 -> 11
    X.insert("dominions") ;// 12

    X.insert("waspish") ;        // 52
    X.insert("wildebeest") ;     // 52 -> 53
    X.insert("reaction") ;       // 52 -> 54

    X.insert("pawns") ;          // 43
    X.insert("vacuously") ;      // 43 -> 44
    X.insert("firth") ;          // 43 -> 45
    X.insert("manipulator") ;    // 43 -> 46
    X.insert("dreariness") ;     // 43 -> 47

    X.insert("insanity") ;       // 105
    X.insert("enthronement") ;   // 105 -> 106
    X.insert("falsifiers") ;     // 105 -> 0
    X.insert("ignominious") ;    // 105 -> 1
    X.insert("mummified") ;      // 105 -> 2

    X.insert("tributes") ;// 21
    X.insert("skulduggery") ;// 22
    X.insert("convulse") ;// 23
    X.insert("frothed") ;// 24
    X.insert("horrify") ;// 25
    X.insert("blackmailers") ;// 26
    X.insert("defenestrated") ;// 27
    X.insert("garrison") ;// 23 -> 28
    X.insert("lidless") ;// 22 -> 29
    X.insert("eye") ;            // 21 -> 30, should trigger inc. rehash

    HashTable X2, X3 ;

    cout << "\nCheck original hash table X...\n" ;
    sanityCheck(X,S2) ;

    X2 = X ;
    X3 = X2 ;

    cout << "\nCheck hash tables X, X2 & X3 after assignment...\n" ;
    sanityCheck(X,S2) ;
    sanityCheck(X2,S2) ;
    sanityCheck(X3,S2) ;

    X2.insert("wildcat") ;
    X2.insert("weightlessness") ;
    X2.insert("sorceress") ;
    X2.insert("enchantress") ;
    X2.insert("witchery") ;
    X2.insert("heliosphere") ;
    X2.insert("obstruct") ;

    cout << "\nCheck hash tables X & X3 after modifying X2...\n" ;
    sanityCheck(X,S2) ;
    sanityCheck(X3,S2) ;
}


bool isInSet(set<string>& S, const char * str) {
  set<string>::iterator it ;

  it = S.find(str) ;
  if (it != S.end() ) return true ;
  return false ;
}


void addToSet(set<string>& S, const char *str) {
  pair< set<string>::iterator, bool>  p ;

  p = S.insert(str) ;
}


void removeFromSet(set<string>& S, const char *str) {
  S.erase(str) ;
}


void sanityCheck(HashTable& T, set<string> S) {
   
  int Tsize ;
  const char * ONE = (char *) 1 ;

  // Check hash table size vs set size
  //
  if ( T.isRehashing() ) {
    Tsize = T.size(0) + T.size(1) ;
  } else {
    Tsize = T.size(0) ;
  }

  if ( Tsize == S.size() ) {
    cout << "Sets sizes are both " << Tsize << endl ;
  } else {
    cout << "Sets sizes are different:  " 
	 << "T size = " << Tsize << ", "
	 << "S.size() = " << S.size() << endl ;
  }

  // Construct STL set<string> for all strings in the hash table
  //
  set<string> Tset ;
  const char *str ;

  int n = T.tableSize(0) ;
  for (int i=0 ; i < n ; i++) {
    str = T.at(i,0) ;
    if (str != NULL && str != ONE) addToSet(Tset,str) ;
  }
  if ( T.isRehashing() ) {
    n = T.tableSize(1) ;
    for (int i=0 ; i < n ; i++) {
      str = T.at(i,1) ;
      if (str != NULL && str != ONE) addToSet(Tset,str) ;
    }
  }

  set<string>::iterator it ; 

  int inSnotT = 0 ;
  int inTnotS = 0 ;

  for (it = S.begin() ; it != S.end() ; it++) {
    if (! isInSet(Tset, it->c_str()) ) inSnotT++ ;
  }

  for (it = Tset.begin() ; it != Tset.end() ; it++) {
    if (! isInSet(S, it->c_str()) ) inTnotS++ ;
  }

  if (inSnotT == 0 && inTnotS == 0) {
    cout << "Passed set equality test\n" ;
  } else {
    cout << "***Failed set equality test\n" ;
    cout << "   " << inSnotT << " words in set S but not in hash table T.\n" ;
    cout << "   " << inTnotS << " words in hash table T but not in set S.\n" ;
  }
}

