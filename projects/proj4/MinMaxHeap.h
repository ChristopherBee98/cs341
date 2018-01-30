#ifndef _MINMAXHEAP_H_
#define _MINMAXHEAP_H_

#include <iostream>
#include <stdlib.h>
using namespace std;

template <typename T>
class TwinData {
  template <class A>
    friend class MinMaxHeap;
  template <class B>
    friend class Heap;
  
 public:
  TwinData();
  ~TwinData();
  TwinData(T data, int twinIndex);
 private:
  T m_data;
  int m_twinIndex;
};

template <typename T>
class Heap {
  template <class A>
    friend class MinMaxHeap;
  template <class B>
    friend class TwinData;
 public:
  Heap();
  ~Heap();
  void updateIndex(TwinData<T>* temp1, TwinData<T>* temp2, int pos);
  void insert(T data, int size);
  void deleteObject(int prevSize, int index);
 private:
  Heap* otherHeap;
  TwinData<T>** m_heapArray;
  bool (*truthFunc) (T, T);
};

template <class T>
class MinMaxHeap {

 public:
  MinMaxHeap(int capacity);

  MinMaxHeap(const MinMaxHeap<T>& other);

  ~MinMaxHeap();

  const MinMaxHeap<T>& operator=(const MinMaxHeap<T>& rhs);

  int size();

  void insert(const T& data);

  T deleteMin();

  T deleteMax();

  void dump();

  void locateMin(int pos, T& data, int& index);

  void locateMax(int pos, T& data, int& index);
 private:
  int m_capacity, m_size;
  Heap<T>* m_min;
  Heap<T>* m_max;
};

#include "MinMaxHeap.cpp"
#endif
