//Header file for hash table
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <iostream>
#include "string.h"
#include "stdlib.h"
#include <cstring>
#include "primes.h"
using namespace std;

class HashTable {
 public:
  HashTable(int n=101);
  ~HashTable();
  HashTable(HashTable& other);
  const HashTable& operator= (HashTable& rhs);
  void insert(const char *str);
  bool find(const char *str);
  char * remove(const char *str);
  bool isRehashing();
  int tableSize(int table=0);
  int size(int table=0);
  const char * at(int index, int table=0);
  void dump();
  void lastResort();
  void createNewTable();
  void moveCluster(int index);
  void reformTable();
 private:
  static char* const DELETED;
  bool m_rehash;
  bool m_cleanSlate;
  int m_sizeOrigin;
  int m_sizeNew;
  char **m_hashArray;
  char **m_newArray;
  char **emergencyArray;
  int m_numOfItemsOrigin;
  int m_numOfItemsNew;
  int emergencySize;
  int emergencyItems;
  bool m_hasStuff;
};

#endif
