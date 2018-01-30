//File: Driver.cpp
//
//CMSC 341 Spring 2017
//
//Contains main driver for graph project
//

#include <iostream>
#include "Graph.h"
using namespace std;

int main() {
  
  // G has 5 vertices numbered 0 thru 4
  Graph G(5) ;
  
  // add some edges
  G.addEdge(3,4) ;
  G.addEdge(1,4) ;
  G.addEdge(0,3) ;
  G.addEdge(0,4) ;
  G.addEdge(0,1) ;
  G.addEdge(1,2) ;
  G.addEdge(2,4) ;

  // dump out data structure
  G.dump() ;
  // Test neighbor iterator
  //
  Graph::NbIterator nit ;

  cout << "\nThe neighbors of vertex 4 are:\n" ;
  for (nit = G.nbBegin(4); nit != G.nbEnd(4) ; nit++) {
    cout << *nit << " " ;
  }
  cout << endl ;


  // Test edge iterator
  //
  Graph::EgIterator eit ;
  pair<int,int> edge ;

  cout << "\nThe edges in the graph are:\n" ;
  for (eit = G.egBegin() ; eit != G.egEnd() ; eit++) {

    edge = *eit ;   // get current edge

    // the two data members of a pair are first and second
    //
    cout << "(" << edge.first << ", " << edge.second << ") " ;

  }
  cout << endl ;
  cout << endl ;

  Graph* Gptr = new Graph(5) ;

  // add some edges
  Gptr->addEdge(3,4) ;
  Gptr->addEdge(1,4) ;
  Gptr->addEdge(0,3) ;
  Gptr->addEdge(0,4) ;
  Gptr->addEdge(0,1) ;
  Gptr->addEdge(1,2) ;
  Gptr->addEdge(2,4) ;


  // dump out data structure
  Gptr->dump() ;

  // make a copy using copy constructor
  Graph* Gptr2 = new Graph(*Gptr) ;
  Gptr2->dump() ;

  // get rid off original graph
  // check if new graph is still there
  delete Gptr ;
  Gptr2->dump() ;

  // Make another graph and dump

  Graph G3(3) ;
  G3.addEdge(0,1) ;
  G3.addEdge(0,2) ;
  G3.addEdge(1,2) ;
  G3.dump() ;

  // Copy over G3 and dump
  G3 = *Gptr2 ;
  G3.dump() ;

  delete Gptr2 ;

  //out of range exceptions
  /*
  try {
    Graph G4(-1);
    G3.addEdge(-1,0);
  }
  catch (string outOfBounds) {
    cout << outOfBounds << endl;
  }
  */
}
