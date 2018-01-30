#include <iostream>
using namespace std;

unsigned int hashCode(const char *str) {

  unsigned int val = 0 ;
  const unsigned int thirtyThree = 33 ;  // magic number from textbook

  int i = 0 ;
  while (str[i] != '\0') {
    val = val * thirtyThree + str[i] ;
    i++ ;
  }
  return val ;
}

int main() {
  char *temp = "frothed";
  unsigned int index = hashCode(temp) % 107;
  cout << index << endl;
}
