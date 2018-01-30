//Class definition file for hash tables
//For moving clusters, first decrement backwards until you find the first entry of the cluster,
//then move forward until you find the last entry
//When inserting, check in old table for duplicates, which counts as touching
#include "HashTable.h"

char * const HashTable::DELETED = (char *) 1;

unsigned int hashCode(const char *str) {
  unsigned int val = 0;
  const unsigned int thirtyThree = 33; //magic number from textbook
  
  int i = 0;
  while (str[i] != '\0') {
    val = val * thirtyThree + str[i];
    i++;
  }
  return val;
}

HashTable::~HashTable() {
  //deallocate memory later (use strdup() and free() in this particular project)
  for (int i = 0; i < m_sizeOrigin; i++) {
    if (m_hashArray[i] != NULL && m_hashArray[i] != DELETED) 
      free(m_hashArray[i]);
  }
  delete []m_hashArray;
  if (m_rehash == true) {
    for (int j = 0; j < m_sizeNew; j++) {
      if (m_newArray[j] != NULL && m_newArray[j] != DELETED)
	free(m_newArray[j]);
    }
    //makes sure the pointers aren't pointing at anything
    delete []m_newArray;
  }
}
//convert to binary search for table size
HashTable::HashTable(int n) {
  if (n <= 0 || n > 199999)  {
    //throw out_of_range("Out of range size");
  } else if (n <= 101) {
    m_hashArray = new char*[101];
    for (int i = 0; i < 101; i++)
      m_hashArray[i] = NULL;
    m_sizeOrigin = 101;
    m_rehash = false;
    m_cleanSlate = false;
    m_numOfItemsOrigin = 0;
    m_numOfItemsNew = 0;
    m_sizeNew = 0;
    m_hasStuff = false;
  } else {
    bool found = false;
    int index;
    int begin = 0;
    int end = numPrimes-1;
    while (found == false) {
      index = (begin+end)/2;
      if (n > primes[index-1] && n < primes[index+1]) {
	if (n == primes[index])
	  index -= 1;
	found = true;
	m_sizeOrigin = primes[index+1];
	m_hashArray = new char*[primes[index+1]];
	for (int i = 0; i < primes[index+1]; i++)
	  m_hashArray[i] = NULL;
	m_numOfItemsOrigin = 0;
	m_rehash = false;
	m_cleanSlate = false;
	m_numOfItemsNew = 0;
	m_sizeNew = 0;
	m_hasStuff = false;
      } else if (primes[index] > n) {
	end = index;
      } else if (primes[index] < n) {
	begin = index;
      } else {
	//fail safe
	found = true;
	m_hashArray = new char*[primes[numPrimes-1]];
	for (int i = 0; i < primes[numPrimes-1]; i++)
	  m_hashArray[i] = NULL;
      }
    }
  }
}
//fix so that if the other hashtable has ongoing rehashing, you force it to wrap it up, then copy
HashTable::HashTable(HashTable& other) {
  char *temp;
  if (other.m_rehash == true)
    other.reformTable();
  m_rehash = other.m_rehash;
  m_sizeOrigin = other.m_sizeOrigin;
  m_sizeNew = other.m_sizeNew;
  m_numOfItemsOrigin = other.m_numOfItemsOrigin;
  m_numOfItemsNew = other.m_numOfItemsNew;
  m_hashArray = new char*[m_sizeOrigin];
  for (int k = 0; k < m_sizeOrigin; k++) {
    m_hashArray[k] = NULL;
  }
  for (int i = 0; i < m_sizeOrigin; i++) {
    if (other.m_hashArray[i] != NULL && other.m_hashArray[i] != DELETED) {
      temp = strdup(other.m_hashArray[i]);
      m_hashArray[i] = temp;
    } else if (other.m_hashArray[i] != NULL && other.m_hashArray[i] == DELETED) {
      m_hashArray[i] = DELETED;
    }
  }
}
//same fix as the copy constructor
const HashTable& HashTable::operator= (HashTable& rhs) {
  for (int i = 0; i < m_sizeOrigin; i++) {
    if (m_hashArray[i] != NULL && m_hashArray[i] != DELETED)
      free(m_hashArray[i]);
  }
  delete []m_hashArray;
  if (m_rehash == true) {
    for (int j = 0; j < m_sizeNew; j++) {
      if (m_newArray[j] != NULL && m_newArray[j] != DELETED)
        free(m_newArray[j]);
    }
    //makes sure the pointers aren't pointing at anything
    delete []m_newArray;
  }
  char *temp;
  if (rhs.m_rehash == true)
    rhs.reformTable();
  m_rehash = rhs.m_rehash;
  m_sizeOrigin = rhs.m_sizeOrigin;
  m_sizeNew = rhs.m_sizeNew;
  m_numOfItemsOrigin = rhs.m_numOfItemsOrigin;
  m_numOfItemsNew = rhs.m_numOfItemsNew;
  m_hashArray = new char*[m_sizeOrigin];
  for (int k = 0; k < m_sizeOrigin; k++) {
    m_hashArray[k] = NULL;
  }
  for (int i = 0; i < m_sizeOrigin; i++) {
    if (rhs.m_hashArray[i] != NULL && rhs.m_hashArray[i] != DELETED) {
      temp = strdup(rhs.m_hashArray[i]);
      m_hashArray[i] = temp;
    } else if (rhs.m_hashArray[i] != NULL && rhs.m_hashArray[i] == DELETED) {
      m_hashArray[i] = DELETED;
    }
  }
  return *this;
}

void HashTable::insert(const char *str) {
  //inserts a cstring into the hash table
  double check;
  if (m_rehash == true) {
    double total = m_numOfItemsOrigin + m_numOfItemsNew;
    if (m_numOfItemsOrigin/total < .03) {
      m_rehash = false;
      reformTable();
    }
  }
  if (m_rehash == true) {
    bool isMatch = false;
    int checkCluster = 1;
    int index1 = hashCode(str) % m_sizeNew;
    int index2 = hashCode(str) % m_sizeOrigin;
    if (m_hashArray[index2] != NULL) {
      int k = hashCode(str) % m_sizeOrigin;
      while (m_hashArray[k] != NULL) {
	if (m_hashArray[k] != DELETED) {
	  if (strcmp(m_hashArray[k], str) == 0) {
	    isMatch = true;
	  }
	}
	k++;
	if (k == m_sizeOrigin)
	  k = 0;
      }
      moveCluster(index2);
    }
    if (m_newArray[index1] != NULL) {
      int i = hashCode(str) % m_sizeNew;
      while (m_newArray[i] != NULL && m_newArray[i] != DELETED) {
        if (m_newArray[i] != DELETED) {
          if (strcmp(m_newArray[i], str) == 0) {
            isMatch = true;
          }
        }
        i++;
        checkCluster++;
        if (checkCluster >= 10)
          m_cleanSlate = true;
        if (i == m_sizeNew)
          i = 0;
      }
      if (isMatch == false) {
        char *temp = strdup(str);
        m_newArray[i] = temp;
        m_numOfItemsNew++;
	check = m_sizeNew;
        if (m_numOfItemsNew >= (check/2))
          m_cleanSlate = true;
      }
    } else {
      char *temp = strdup(str);
      m_newArray[index1] = temp;
      m_numOfItemsNew++;
      check = m_sizeNew;
      if (m_numOfItemsNew >= (check/2))
        m_cleanSlate = true;
    }
  } else {
    bool isMatch = false;
    int checkCluster = 1;
    int index = hashCode(str) % m_sizeOrigin;
    if (m_hashArray[index] != NULL) {
      int i = hashCode(str) % m_sizeOrigin;
      while (m_hashArray[i] != NULL && m_hashArray[i] != DELETED) {
	if (m_hashArray[i] != DELETED) {
	  if (strcmp(m_hashArray[i], str) == 0) {
	    isMatch = true;
	  }
	}
	i++;
	checkCluster++;
	if (checkCluster >= 10)
	  m_rehash = true;
	if (i == m_sizeOrigin)
	  i = 0;
      }
      if (isMatch == false) {
	char *temp = strdup(str);
	m_hashArray[i] = temp;
	m_numOfItemsOrigin++;
	check = m_sizeOrigin;
	if (m_numOfItemsOrigin >= (check/2))
	  m_rehash = true;
      }
    } else {
      char *temp = strdup(str);
      m_hashArray[index] = temp;
      m_numOfItemsOrigin++;
      check = m_sizeOrigin;
      if (m_numOfItemsOrigin >= (check/2))
	m_rehash = true;
    }
    if (m_rehash == true) {
      createNewTable();
      moveCluster(index);
    }
  }
  lastResort();
}
//when touching during rehashing, check for each clusters hash value to know the original point of the cluster
bool HashTable::find(const char *str) {
  bool temp = false;
  if (m_rehash == true) {
    double total = m_numOfItemsOrigin + m_numOfItemsNew;
    if (m_numOfItemsOrigin/total < .03) {
      m_rehash = false;
      reformTable();
    }
  }
  int checkCluster = 1;
  int index = hashCode(str) % m_sizeOrigin;
  int i = index;
  while (m_hashArray[i] != NULL) {
    if (m_hashArray[i] != DELETED) {
      if (strcmp(m_hashArray[i], str) == 0)
	temp = true;
    }
    i++;
    checkCluster++;
    //move create table somewhere else
    if (checkCluster >= 10) {
      m_rehash = true;
      createNewTable();
    }
    if (i == m_sizeOrigin)
      i = 0;
  }
  if (m_rehash == true)
    moveCluster(index);
  checkCluster = 1;
  if (m_rehash == true) {
    index = hashCode(str) % m_sizeNew;
    while (m_newArray[index] != NULL) {
      if (m_newArray[index] != DELETED) {
	if (strcmp(m_newArray[index], str) == 0)
	  temp = true;
      }
      index++;
      checkCluster++;
      if (checkCluster >= 10) {
	m_cleanSlate = true;
      }
      if (index == m_sizeNew)
	index = 0;
    }
  }
  lastResort();
  return temp;
}
char * HashTable::remove(const char *str) {
  if (m_rehash == true) {
    double total = m_numOfItemsOrigin + m_numOfItemsNew;
    if (m_numOfItemsOrigin/total < .03) {
      m_rehash = false;
      reformTable();
    }
  }
  char *temp = NULL;
  int checkCluster = 1;
  int index = hashCode(str) % m_sizeOrigin;
  int i = index;
  while (m_hashArray[i] != NULL) {
    if (m_hashArray[i] != DELETED) {
      if (strcmp(m_hashArray[i], str) == 0) {
	temp = m_hashArray[i];
	m_hashArray[i] = DELETED;
	m_numOfItemsOrigin--;
      }
    }
    i++;
    checkCluster++;
    if (checkCluster >= 10) {
      m_rehash = true;
      createNewTable();
    }
    if (i == m_sizeOrigin)
      i = 0;
  }
  if (m_rehash == true) {
    moveCluster(index);
  }
  checkCluster = 1;
  if (m_rehash == true) {
    index = hashCode(str) % m_sizeNew;
    while (m_newArray[index] != NULL) {
      if (m_newArray[index] != DELETED) {
	if (strcmp(m_newArray[index], str) == 0) {
	  temp = m_newArray[index];
	  m_newArray[index] = DELETED;
	  m_numOfItemsNew--;
	}
      }
      index++;
      checkCluster++;
      if (checkCluster >= 10) {
	m_cleanSlate = true;
      }
      if (index == m_sizeNew)
	index = 0;
    }
  }
  lastResort();
  return temp;
}

bool HashTable::isRehashing() {
  if (m_rehash == true)
    return true;
  else
    return false;
}

int HashTable::tableSize(int table) {
  if (m_rehash == true) {
    if (table == 0) {
      return m_sizeOrigin;
    } else {
      return m_sizeNew;
    }
  } else {
    return m_sizeOrigin;
  }
}

int HashTable::size(int table) {
  if (table == 0)
    return m_numOfItemsOrigin;
  else
    return m_numOfItemsNew;
}

const char * HashTable::at(int index, int table) {
  if (table == 0) {
    if (index < 0 || index >= m_sizeOrigin) {
      //throw out_of_range("Out of range index");
    }
    return m_hashArray[index];
  } else {
    if (index < 0 || index >= m_sizeNew) {
      //throw out_of_range("Out of range size");
    }
    return m_newArray[index];
  }
}

void HashTable::dump() {
  //dump out contents of hash table
  int temp;
  cout << "Hash Table #1" << endl;
  cout << "Size of table: " << m_sizeOrigin << endl;
  cout << "Number of items: " << m_numOfItemsOrigin << endl;
  for (int i = 0; i < m_sizeOrigin; i++) {
    if (m_hashArray[i] != NULL && m_hashArray[i] != DELETED) {
      temp = hashCode(m_hashArray[i]) % m_sizeOrigin;
      cout << "H1[" << i << "] = " << m_hashArray[i] << " (" << temp << ") \n";
    } else if (m_hashArray[i] == DELETED) {
      cout << "H1[" << i << "] = " << "DELETED \n";
    } else {
      cout << "H1[" << i << "] = " << "\n";
    }
  }
  if (m_rehash == true) {
    cout << endl;
    cout << "Hash Table #2" << endl;
    cout << "Size of table: " << m_sizeNew << endl;
    cout << "Number of items: " << m_numOfItemsNew << endl;
    for (int j = 0; j < m_sizeNew; j++) {
      if (m_newArray[j] != NULL && m_newArray[j] != DELETED) {
	temp = hashCode(m_newArray[j]) % m_sizeNew;
	cout << "H2[" << j << "] = " << m_newArray[j] << " (" << temp << ") \n";
      } else if (m_newArray[j] == DELETED) {
	cout << "H2[" << j << "] = " << "DELETED \n";
      } else {
	cout << "H2[" << j << "] = " << "\n";
      }
    }
  }
}

//for rehashing
void HashTable::createNewTable() {
  //create second hash table for rehashing here
  if (m_hasStuff == false) {
    m_sizeNew = m_numOfItemsOrigin*4;
    int index;
    if (m_sizeNew <= 101) {
      m_newArray = new char*[101];
      for (int i = 0; i < 101; i++)
	m_newArray[i] = NULL;
    } else {
      int begin = 0;
      int end = numPrimes-1;
      bool found = false;
      while (found == false) {
	index = (begin+end)/2;
	if (m_sizeNew > primes[index-1] && m_sizeNew < primes[index+1]) {
	  if (m_sizeNew == primes[index])
	    index -= 1;
	  found = true;
	  m_sizeNew = primes[index+1];
	  m_newArray = new char*[primes[index+1]];
	  for (int i = 0; i < primes[index+1]; i++)
	    m_newArray[i] = NULL;
	} else if (primes[index] > m_sizeNew) {
	  end = index;
	} else if (primes[index] < m_sizeNew) {
	  begin = index;
	} else {
	  //fail safe
	  found = true;
	  m_newArray = new char*[primes[numPrimes-1]];
	  for (int i = 0; i < primes[numPrimes-1]; i++)
	    m_newArray[i] = NULL;
	}
      }
    }
    m_hasStuff = true;
  }
}

//for when rehashing fails
void HashTable::lastResort() {
  char *fakeEntry;
  char **fakeTable;
  int index2;
  if (m_cleanSlate == true) {
    emergencySize = (m_numOfItemsOrigin+m_numOfItemsNew)*4;
    int index;
    int begin = 0;
    int end = numPrimes-1;
    if (emergencySize <= 101) {
      emergencyArray = new char*[101];
      for (int i = 0; i < 101; i++)
	emergencyArray[i] = NULL;
    } else {
      bool found = false;
      while (found == false) {
	//infinite loop for some reason
	//fix binary search algorithm
	index = (begin+end)/2;
	if (emergencySize > primes[index-1] && emergencySize < primes[index+1]) {
	  if (emergencySize == primes[index])
	    index -= 1;
	  found = true;
	  emergencySize = primes[index+1];
	  emergencyArray = new char*[primes[index+1]];
	  for (int i = 0; i < primes[index+1]; i++)
	    emergencyArray[i] = NULL;
	} else if (primes[index] > emergencySize) {
	  end = index;
	} else if (primes[index] < emergencySize) {
	  begin = index;
	} else {
	  //fail safe
	  found = true;
	  emergencyArray = new char*[primes[numPrimes-1]];
	  for (int i = 0; i < primes[numPrimes-1]; i++)
	    emergencyArray[i] = NULL;
	}
      }
    }
    for (int j = 0; j < m_sizeOrigin; j++) {
      if (m_hashArray[j] != NULL && m_hashArray[j] != DELETED) {
	index2 = hashCode(m_hashArray[j]) % emergencySize;
	while (emergencyArray[index2] != NULL) {
	  index2 += 1;
	  if (index2 == emergencySize)
	    index2 = 0;
	}
	fakeEntry = strdup(m_hashArray[j]);
	emergencyArray[index2] = fakeEntry;
	free(m_hashArray[j]);
      }
      m_hashArray[j] = NULL;
    }
    for (int k = 0; k < m_sizeNew; k++) {
      if (m_newArray[k] != NULL && m_newArray[k] != DELETED) {
	index2 = hashCode(m_newArray[k]) % emergencySize;
	while (emergencyArray[index2] != NULL) {
	  index2 += 1;
	  if (index2 == emergencySize)
	    index2 = 0;
	}
	fakeEntry = strdup(m_newArray[k]);
	emergencyArray[index2] = fakeEntry;
	free(m_newArray[k]);
      }
      m_newArray[k] = NULL;
    }
    emergencyItems = m_numOfItemsOrigin + m_numOfItemsNew;
    fakeTable = m_hashArray;
    m_hashArray = emergencyArray;
    emergencyArray = fakeTable;
    m_sizeOrigin = emergencySize;
    m_numOfItemsOrigin = emergencyItems;
    m_sizeNew = 0;
    m_numOfItemsNew = 0;
    emergencySize = 0;
    emergencyItems = 0;
    delete []emergencyArray;
    delete []m_newArray;
    m_rehash = false;
    m_cleanSlate = false;
    m_hasStuff = false;
  }
}

//for moving clusters during touch
//causing infinite loop when remove comes here (Fix)
void HashTable::moveCluster(int index) {
  int begin, end;
  char *temp;
  int newIndex;
  begin = index;
  end = index;
  if (m_hashArray[index] != NULL) {
    while (m_hashArray[begin] != NULL) {
      begin--;
      if (begin == -1)
	begin = m_sizeOrigin-1;
    }
    begin++;
    if (begin == m_sizeOrigin)
      begin = 0;
    while (m_hashArray[end] != NULL) {
      end++;
      if (end == m_sizeOrigin)
	end = 0;
    }
    end--;
    if (end == -1)
      end = m_sizeOrigin-1;
    while (begin != end+1) {
      if (m_hashArray[begin] != DELETED) {
	newIndex = hashCode(m_hashArray[begin]) % m_sizeNew;
	while (m_newArray[newIndex] != NULL) {
	  newIndex += 1;
	  if (newIndex == m_sizeNew)
	    newIndex = 0;
	}
	temp = strdup(m_hashArray[begin]);
	m_newArray[newIndex] = temp;
	free(m_hashArray[begin]);
	m_hashArray[begin] = NULL;
	m_numOfItemsOrigin--;
	m_numOfItemsNew++;
      }
      begin++;
      if (begin == m_sizeOrigin)
	begin = 0;
    }
  }
}
//forms one table again
void HashTable::reformTable() {
  int index;
  char *temp;
  char **temp2;
  for (int i = 0; i < m_sizeOrigin; i++) {
    if (m_hashArray[i] != NULL && m_hashArray[i] != DELETED) {
      index = hashCode(m_hashArray[i]) % m_sizeNew;
      while (m_newArray[index] != NULL) {
	index += 1;
	if (index == m_sizeNew)
	  index = 0;
      }
      temp = strdup(m_hashArray[i]);
      m_newArray[index] = temp;
      free(m_hashArray[i]);
    }
    m_hashArray[i] = NULL;
  }
  temp2 = m_hashArray;
  m_hashArray = m_newArray;
  m_newArray = temp2;
  m_sizeOrigin = m_sizeNew;
  m_numOfItemsOrigin = m_numOfItemsNew + m_numOfItemsOrigin;
  m_sizeNew = 0;
  m_numOfItemsNew = 0;
  delete []m_newArray;
  m_rehash = false;
  m_hasStuff = false;
}
