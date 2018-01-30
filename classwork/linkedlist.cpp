//Comments

#include <iostream>
using namespace std;

class Node {
public:
  int m_data;
  Node* next;
};

int main() {
  int counter = 0;
  Node *ptr = head;
  while (ptr != NULL) {
    counter++;
    ptr = ptr->next;
  }
  cout << counter << endl;
}
