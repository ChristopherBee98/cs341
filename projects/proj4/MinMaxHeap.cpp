#ifndef _MINMAXHEAP_CPP_
#define _MINMAXHEAP_CPP_

#include "MinMaxHeap.h"

//templated standalone functions for discerning the min and max heaps
template <typename T>
bool compareHeapLT(T item1, T item2);
template <typename T>
bool compareHeapGT(T item1, T item2);

template <typename T>
MinMaxHeap<T>::MinMaxHeap(int capacity) {
  //makes it for non zero indexing
  m_capacity = capacity+1;
  m_size = 0;
  m_min = new Heap<T>;
  m_max = new Heap<T>;
  //creates a TwinData object, which contains both data and an index for its twin
  m_min->m_heapArray = new TwinData<T>*[m_capacity];
  m_max->m_heapArray = new TwinData<T>*[m_capacity];
  //sets the first element to null for inserting and deleting purposes
  m_min->m_heapArray[1] = NULL;
  m_max->m_heapArray[1] = NULL;
  m_min->otherHeap = m_max;
  m_max->otherHeap = m_min;
  m_min->truthFunc = &compareHeapLT;
  m_max->truthFunc = &compareHeapGT;
}

template <typename T>
MinMaxHeap<T>::MinMaxHeap(const MinMaxHeap<T>& other) {
  m_capacity = other.m_capacity;
  m_size = other.m_size;
  m_min = new Heap<T>;
  m_max = new Heap<T>;
  m_min->m_heapArray = new TwinData<T>*[m_capacity];
  m_max->m_heapArray = new TwinData<T>*[m_capacity];
  m_min->m_heapArray[1] = NULL;
  m_max->m_heapArray[1] = NULL;
  m_min->otherHeap = m_max;
  m_max->otherHeap = m_min;
  m_min->truthFunc = &compareHeapLT;
  m_max->truthFunc = &compareHeapGT;
  for (int i = 1; i <= m_size; i++) {
    TwinData<T>* newNode1 = new TwinData<T>(other.m_min->m_heapArray[i]->m_data, other.m_min->m_heapArray[i]->m_twinIndex);
    TwinData<T>* newNode2 = new TwinData<T>(other.m_max->m_heapArray[i]->m_data, other.m_max->m_heapArray[i]->m_twinIndex);
    m_min->m_heapArray[i] = newNode1;
    m_max->m_heapArray[i] = newNode2;
  }
}

template <typename T>
MinMaxHeap<T>::~MinMaxHeap() {
  for (int i = 1; i <= m_size; i++) {
    delete m_min->m_heapArray[i];
  }
  for (int j = 1; j <= m_size; j++) {
    delete m_max->m_heapArray[j];
  }
  //makes sure the pointers aren't pointing at anything
  delete []m_min->m_heapArray;
  delete []m_max->m_heapArray;
  delete m_min;
  delete m_max;
}

template <typename T>
const MinMaxHeap<T>& MinMaxHeap<T>::operator=(const MinMaxHeap<T>& rhs) {
  for (int i = 1; i <= m_size; i++) {
    delete m_min->m_heapArray[i];
  }
  for (int j = 1; j <= m_size; j++) {
    delete m_max->m_heapArray[j];
  }
  //makes sure the pointers aren't pointing at anything
  delete []m_min->m_heapArray;
  delete []m_max->m_heapArray;
  delete m_min;
  delete m_max;
  //made sure to deallocate the minmaxheap before assigning
  m_capacity = rhs.m_capacity;
  m_size = rhs.m_size;
  m_min = new Heap<T>;
  m_max = new Heap<T>;
  m_min->m_heapArray = new TwinData<T>*[m_capacity];
  m_max->m_heapArray = new TwinData<T>*[m_capacity];
  m_min->m_heapArray[1] = NULL;
  m_max->m_heapArray[1] = NULL;
  m_min->otherHeap = m_max;
  m_max->otherHeap = m_min;
  m_min->truthFunc = &compareHeapLT;
  m_max->truthFunc = &compareHeapGT;
  for (int i = 1; i <= m_size; i++) {
    TwinData<T>* newNode1 = new TwinData<T>(rhs.m_min->m_heapArray[i]->m_data, rhs.m_min->m_heapArray[i]->m_twinIndex);
    TwinData<T>* newNode2 = new TwinData<T>(rhs.m_max->m_heapArray[i]->m_data, rhs.m_max->m_heapArray[i]->m_twinIndex);
    m_min->m_heapArray[i] = newNode1;
    m_max->m_heapArray[i] = newNode2;
  }
  return *this;
}

template <typename T>
int MinMaxHeap<T>::size() {
  //constant time
  return m_size;
}

template <typename T>
void MinMaxHeap<T>::insert(const T& data) {
  TwinData<T>* newNode1;
  TwinData<T>* newNode2;
  //try to work this into heap class later
  if (m_min->m_heapArray[1] == NULL) {
    newNode1 = new TwinData<T>(data, 1);
    newNode2 = new TwinData<T>(data, 1);
    m_min->m_heapArray[1] = newNode1;
    m_max->m_heapArray[1] = newNode2;
    m_size++;
  } else if (m_size == m_capacity-1) {
    //throw out_of_range exception
    throw out_of_range("heap is full");
  } else {
    m_size++;
    newNode1 = new TwinData<T>(data, size());
    newNode2 = new TwinData<T>(data, size());
    m_min->m_heapArray[size()] = newNode1;
    m_max->m_heapArray[size()] = newNode2;
    m_min->insert(data, size());
    m_max->insert(data, size());
  }
}

template <typename T>
T MinMaxHeap<T>::deleteMin() {
  if (m_min->m_heapArray[1] == NULL) {
    //make this into a out_of_range exception
    throw out_of_range("heap is empty");
  } else {
    T key = m_min->m_heapArray[1]->m_data;
    int prevSize = m_size;
    m_size--;
    m_max->deleteObject(prevSize, m_min->m_heapArray[1]->m_twinIndex);
    m_min->deleteObject(prevSize, 1);
    return key;
  }
  return NULL;
}

template <typename T>
T MinMaxHeap<T>::deleteMax() {
  if (m_min->m_heapArray[1] == NULL) {
    //make this into a out_of_range exception
    throw out_of_range("heap is empty");
  } else {
    T key = m_max->m_heapArray[1]->m_data;
    int prevSize = m_size;
    m_size--;
    m_min->deleteObject(prevSize, m_max->m_heapArray[1]->m_twinIndex);
    m_max->deleteObject(prevSize, 1);
    return key;
  }
  //if the heap is empty, no value is returned
  return NULL;
}

template <typename T>
void MinMaxHeap<T>::dump() {
  cout << "... MinMaxHeap::dump() ..." << endl;
  cout << endl;
  cout << "----------Min----------" << endl;
  cout << "size = " << m_size << ", capacity = " << m_capacity-1 << endl;
  for (int i = 1; i <= m_size; i++) {
    cout << "Heap[" << i << "] = (" << m_min->m_heapArray[i]->m_data << ", " << m_min->m_heapArray[i]->m_twinIndex << ")" << endl;
  }
  cout << "----------Max----------" << endl;
  cout << "size = " << m_size << ", capacity = " << m_capacity-1 << endl;
  for (int i = 1; i <= m_size; i++) {
    cout << "Heap[" << i << "] = (" << m_max->m_heapArray[i]->m_data << ", " << m_max->m_heapArray[i]->m_twinIndex << ")" << endl;
  }
  cout << "-----------------------" << endl;
}

template <typename T>
void MinMaxHeap<T>::locateMin(int pos, T& data, int& index) {
  data = m_min->m_heapArray[pos]->m_data;
  index = m_min->m_heapArray[pos]->m_twinIndex;
}

template <typename T>
void MinMaxHeap<T>::locateMax(int pos, T& data, int& index) {
  data = m_max->m_heapArray[pos]->m_data;
  index = m_max->m_heapArray[pos]->m_twinIndex;
}

template <typename T>
Heap<T>::Heap() {
  m_heapArray = NULL;
  otherHeap = NULL;
  truthFunc = NULL;
}

template <typename T>
Heap<T>::~Heap() {
}

//running time: O(logn)
template <typename T>
void Heap<T>::insert(T data, int size) {
  TwinData<T>* temp;
  TwinData<T>* temp1;
  TwinData<T>* temp2;
  int pos = size;
  while (pos > 1 && truthFunc(data, m_heapArray[pos/2]->m_data)) {
    temp1 = m_heapArray[pos];
    temp2 = m_heapArray[pos/2];
    temp = m_heapArray[pos];
    m_heapArray[pos] = m_heapArray[pos/2];
    m_heapArray[pos/2] = temp;
    //updates the index for each entry of the heap
    otherHeap->updateIndex(temp1, temp2, pos);
    pos /= 2;
  }
}

//running time: O(log n)
template <typename T>
void Heap<T>::deleteObject(int prevSize, int index) {
  //implement O(logn) time
  //use twin index to remove other heaps node
  //convert comparisons to truthFunc
  int pos = index;
  int child = index*2;
  delete m_heapArray[index];
  TwinData<T>* temp = m_heapArray[prevSize];
  prevSize--;  
  while (child <= prevSize) {
      if (child < prevSize && truthFunc(m_heapArray[child+1]->m_data, m_heapArray[child]->m_data)) {
        child++;
      }
      if (truthFunc(m_heapArray[child]->m_data, temp->m_data)) {
        m_heapArray[pos] = m_heapArray[child];
        //updates the index by swapping with the child
        otherHeap->m_heapArray[m_heapArray[child]->m_twinIndex]->m_twinIndex = pos;
      } else {
        break;
      }
      pos = child;
      child *= 2;
  }
  m_heapArray[pos] = temp;
  otherHeap->m_heapArray[temp->m_twinIndex]->m_twinIndex = pos;
}

template <typename T>
TwinData<T>::TwinData() {
  m_data = 0;
  m_twinIndex = 0;
}

//sets up the twindata object
template <typename T>
TwinData<T>::TwinData(T data, int twinIndex) {
  m_data = data;
  m_twinIndex = twinIndex;
}

template <typename T>
TwinData<T>::~TwinData() {
}

template <typename T>
void Heap<T>::updateIndex(TwinData<T>* temp1, TwinData<T>* temp2, int pos) {
  //otherHeap is pointer to the max/min heap
  //need to check if the twinindex is still valid
  //otherHeap->m_heapArray[temp2->m_twinIndex]->m_data
  if (temp1->m_data != temp2->m_data) {
    int temp = m_heapArray[temp1->m_twinIndex]->m_twinIndex;
    m_heapArray[temp1->m_twinIndex]->m_twinIndex = m_heapArray[temp2->m_twinIndex]->m_twinIndex;
    m_heapArray[temp2->m_twinIndex]->m_twinIndex = temp;
  }
}

template <typename T>
bool compareHeapLT(T item1, T item2) {
  if (item1 < item2)
    return true;
  else
    return false;
}

template <typename T>
bool compareHeapGT(T item1, T item2) {
  if (item1 > item2)
    return true;
  else
    return false;
}
#endif
