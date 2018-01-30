// File: Graph.cpp
//
// CMSC 341 Spring 2017
//
// Definition file for Graph class
//

#include "Graph.h"
#include <iostream>

// Graph constructor, must give number of vertices
Graph::Graph(int n) {
  //throws an exception if the graph isn't valid
  if (n <= 0) {
    throw std::out_of_range("Graph can't be size negative or 0.");
  }
  m_size = n;
  m_adjLists = new AdjListNode*[n];
  //makes sure the list has a value after it's initialized
  for (int i = 0; i < m_size; i++)
    m_adjLists[i] = NULL;
}

//Graph copy constructor (does not need to copy in order)
Graph::Graph(const Graph& G) {
  m_size = G.m_size;
  m_adjLists = new AdjListNode*[G.m_size];
  for (int j = 0; j < m_size; j++)
    m_adjLists[j] = NULL;
  AdjListNode* currentPtr;
  AdjListNode* newNode;
  for (int i = 0; i < m_size; i++) {
    //checks if the vertex has any neighbors at all, otherwise it skips
    while (G.m_adjLists[i] != NULL) {
      currentPtr = m_adjLists[i];
      newNode = new AdjListNode;
      newNode->next = NULL;
      newNode->m_vertex = G.m_adjLists[i]->m_vertex;
      if (m_adjLists[i] == NULL) {
        m_adjLists[i] = newNode;
      } else {
        while (currentPtr->next != NULL) {
          currentPtr = currentPtr->next;
        }
        currentPtr->next = newNode;
      }
      G.m_adjLists[i] = G.m_adjLists[i]->next;
    }
  }
}

// Graph destructor
Graph::~Graph() {
  AdjListNode* currentPtr;
  for (int i = 0; i < m_size; i++) {
    currentPtr = m_adjLists[i];
    while (currentPtr != NULL) {
      //goes through and deletes/deallocates the data from the linked list
      m_adjLists[i] = m_adjLists[i]->next;
      delete currentPtr;
      currentPtr = m_adjLists[i];
    }
    m_adjLists[i] = NULL;
  }
  //makes sure the pointers aren't pointing at anything
  delete []m_adjLists;
}

// Graph assignment operator
const Graph& Graph::operator= (const Graph& rhs) {
  if (this == &rhs) {
    return *this;
  } else {
    AdjListNode* currentPtr;
    for (int i = 0; i < m_size; i++) {
      currentPtr = m_adjLists[i];
      //deallocates previous memory to prevent memory leaks
      while (currentPtr != NULL) {
	m_adjLists[i] = m_adjLists[i]->next;
	delete currentPtr;
	currentPtr = m_adjLists[i];
      }
      m_adjLists[i] = NULL;
    }
    delete [] m_adjLists;
    m_size = rhs.m_size;
    m_adjLists = new AdjListNode*[rhs.m_size];
    for (int k = 0; k < m_size; k++)
      m_adjLists[k] = NULL;
    AdjListNode* newNode;
    for (int i = 0; i < m_size; i++) {
      while (rhs.m_adjLists[i] != NULL) {
	currentPtr = m_adjLists[i];
	newNode = new AdjListNode;
	newNode->next = NULL;
	newNode->m_vertex = rhs.m_adjLists[i]->m_vertex;
	if (m_adjLists[i] == NULL) {
	  m_adjLists[i] = newNode;
	} else {
	  while (currentPtr->next != NULL) {
	    currentPtr = currentPtr->next;
	  }
	  currentPtr->next = newNode;
	}
	//moves along until the end of that vertex's list
	rhs.m_adjLists[i] = rhs.m_adjLists[i]->next;
      }
    }
  return *this;
  }
}

// return number of vertices
int Graph::size() {
  return m_size;
}

// add edge between u and v
void Graph::addEdge(int u, int v) {
  //checks if the edge is valid
  if ((u < 0 || u >= m_size) || (v < 0 || v >= m_size)) {
    throw std::out_of_range("u or v are either negative or out of range.");
  }
  AdjListNode* newNode1 = new AdjListNode;
  //sets the placeholder pointer to the head node pointer
  AdjListNode* currentPtr = m_adjLists[v];
  newNode1->m_vertex = u;
  if (m_adjLists[v] == NULL) {
    m_adjLists[v] = newNode1;
  //traverses the linked list until it reaches the end
  } else {
    newNode1->next = m_adjLists[v];
    m_adjLists[v] = newNode1;
  }
  //proceeds to add the other vertex with the value
  AdjListNode* newNode2 = new AdjListNode;
  currentPtr = m_adjLists[u];
  newNode2->m_vertex = v;
  if (m_adjLists[u] == NULL) {
    m_adjLists[u] = newNode2;
  } else {
    newNode2->next = m_adjLists[u];
    m_adjLists[u] = newNode2;
  }
}

// print out data structure for debugging
void Graph::dump() {
  AdjListNode* currentPtr;
  std::cout << "Dump out graph (size = " << m_size << "):" << std::endl;
  for (int i = 0; i < m_size; i++) {
    currentPtr = m_adjLists[i];
    std::cout << "[" << i << "]: ";
    while (currentPtr != NULL) {
      //prints out the elements of the vertex
      std::cout << currentPtr->m_vertex << " ";
      currentPtr = currentPtr->next;
    }
    //new line for next vertex
    std::cout << std::endl;
  }
}

// Edge Iterator constructor, isEnd true to create end iterator
Graph::EgIterator::EgIterator(Graph *Gptr, bool isEnd) {
  m_Gptr = Gptr;
  //checks for a begin or end iterator
  if (Gptr != NULL) {
    if (isEnd == false) {
      m_source = 0;
      m_where = m_Gptr->m_adjLists[m_source];
    }
    else {
      m_source = m_Gptr->size();
      m_where = NULL;
    }
  }
}

// Compare iterators, only makes sense to compare with end iterator
bool Graph::EgIterator::operator!= (const EgIterator& rhs) {
  //checks for the source and whether it's equal
  if (m_source != rhs.m_source)
    return true;
  else
    return false;
}
     
// Move iterator to next printable edge (post increment)
void Graph::EgIterator::operator++(int dummy) {
   if (m_where->next == NULL) {
     m_source++;
     m_where = m_Gptr->m_adjLists[m_source];
     //makes sure the edge isn't repeated numerous times
     while ((m_source > m_where->m_vertex) && (m_source != m_Gptr->size())) {
      if (m_where->next == NULL) {
	m_source++;
	if (m_source < m_Gptr->size())
	  m_where = m_Gptr->m_adjLists[m_source];
      } else {
	m_where = m_where->next;
      }
    }
  } else {
    m_where = m_where->next;
    while ((m_source > m_where->m_vertex) && (m_source != m_Gptr->size())) {
      if (m_where->next == NULL) {
	m_source++;
	if (m_source < m_Gptr->size())
	  m_where = m_Gptr->m_adjLists[m_source];
      } else {
	m_where = m_where->next;
      }
    }
  }
}

// return edge at iterator location
std::pair<int,int> Graph::EgIterator::operator*() {
  //another exception for the iterator being out of bounds
  if (m_where == NULL) {
    throw std::out_of_range("EgIterator dereference error.");
  }
  return std::pair<int, int>(m_source, m_where->m_vertex);
}

// Make an initial edge Iterator
Graph::EgIterator Graph::egBegin() {
  //this acting as the pointer to the super class of a Graph
  EgIterator beginIterator(this);
  return beginIterator;
}

// Make an end iterator for edge iterator
Graph::EgIterator Graph::egEnd() {
  EgIterator endIterator(this, true);
  return endIterator;
}

// Constructor for iterator for vertices adjacent to vertex v
Graph::NbIterator::NbIterator(Graph *Gptr, int v, bool isEnd) {
  m_Gptr = Gptr;
  m_source = v;
  if (isEnd == true) {
    AdjListNode* currentPtr = Gptr->m_adjLists[v];
    while (currentPtr != NULL) {
      currentPtr = currentPtr->next;
    }
    m_where = currentPtr;
  }
  else {
    if (Gptr != NULL)
      m_where = Gptr->m_adjLists[v];
  }
}
// Compare iterators, only makes sense to compare with end iterator
bool Graph::NbIterator::operator!= (const NbIterator& rhs) {
  //checks where the neighbor iterator is at and if it's at the end
  if (m_where != rhs.m_where)
    return true;
  else
    return false;
}

// Move iterator to next neighbor
void Graph::NbIterator::operator++(int dummy) {
  m_where = m_where->next;
}

// Return neighbor at current iterator position
int Graph::NbIterator::operator*() {
  //exception for any null shenanigans involving the neighbor iterator
  if (m_where == NULL) {
    throw std::out_of_range("NbIterator dereference error.");
  }
  return m_where->m_vertex;
}

// Make an initial neighbor iterator
Graph::NbIterator Graph::nbBegin(int v) {
  NbIterator beginIterator(this, v);
  return beginIterator;
}

// Make an end neighbor iterator
Graph::NbIterator Graph::nbEnd(int v) {
  NbIterator endIterator(this, v, true);
  return endIterator;
}

// construct a node 
Graph::AdjListNode::AdjListNode(int v, AdjListNode *ptr) {
  m_vertex = v;
  next = ptr;
}      

/*
 private:
    Graph *m_Gptr ;         // pointer to Graph using this iterator
    int m_source ;          // source vertex of current location
    AdjListNode *m_where ;  // location in linked list of iterator
  } ;
 private:
    Graph *m_Gptr ;          // which graph?
    int m_source ;           // neighbor of which vertex?
    AdjListNode *m_where ;   // location in linked list
  } ;
private:

    int m_vertex ;        // holds one neighbor vertex
    AdjListNode * next ;  // next node
  } ;


  // pointer to a dynamically allocated array of linked lists
  // of AdjListNodes
  AdjListNode ** m_adjLists ;

  // number of vertices in this graph
  int m_size ;

} ;
*/
