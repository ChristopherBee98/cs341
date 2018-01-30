//Comments
#include <iostream>
using namespace std;

int main() {
  int m = 3, n = 2, *p;
  p = &n;
  *p = n + m;
  p = &m;
  m = *p - n;
  cout << " m = " << m << ", ";
  cout << " n = " << n << ", ";
  cout << " *p = " << *p << endl;
}
