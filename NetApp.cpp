/*
We are given a linked List
[1 -> 2 -> 3 -> 4]
[4 -> 3 -> 2 -> 1]

LinkedList -> val(int), *next(ListList)

cur = 1

1->next = 2

2->next = 1

nextElement = 2

prev = 
cur = 1
next = 2

4 3 2 -> 1

cur


*/
#include<bits/stdc++.h>

using namespace std;

class LinkedList {
private:
public:
    int value;
    LinkedList* next;

    LinkedList(int value = 0, LinkedList* next = nullptr): value(value), next(next) {}
};

LinkedList* reverseLinkedList(LinkedList* head) {
    if(head == nullptr) return nullptr;
    LinkedList* cur = head;
    LinkedList* nextPtr = head->next;
    LinkedList* tail = reverseLinkedList(nextPtr);
    nextPtr->next = cur;
    return tail;
}

/*
2 3

3 2 1

*/


int main() {
    LinkedList* firstNode = new LinkedList(1, nullptr);
    LinkedList* secondNode = new LinkedList(2, nullptr);
    LinkedList* thirdNode = new LinkedList(3, nullptr);
    firstNode->next = secondNode;
    secondNode->next = thirdNode;
    while(firstNode != nullptr) {
        cout << firstNode->value << " ";
        firstNode = firstNode->next;
    }
    LinkedList* tailNode = reverseLinkedList(firstNode);
    while(tailNode != nullptr) {
        cout << tailNode->value << " ";
        tailNode = tailNode->next;
    }
    return 0;
}