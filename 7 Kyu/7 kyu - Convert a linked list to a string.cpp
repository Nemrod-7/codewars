#include <iostream>
#include <sstream>
#include <string>

class Node {
  public:
    int data;
    Node* next;

  Node(int data, Node* next = nullptr)   {
    this->data = data;
    this->next = next;
  }
};

std::string stringify (Node* list) ;

int main () {

    using namespace std;

    cout << stringify(new Node(0, new Node(1, new Node(4, new Node(9, new Node(16)))))) << endl;

    return 0;
}

std::string stringify (Node* list) {

    std::ostringstream os;

    while (list) {
        os << list->data << " -> ";
        list = list->next;
    }

    os << "nullptr";
    return os.str();
}
