//
//  LinkedList.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef LinkedList_h
#define LinkedList_h

#include <stdexcept>

using namespace std;

template <class T>
class LinkedList {
private:
    struct Node {
        T item;
        Node* next;
        Node* prev;

        Node(const T & item, Node* next = nullptr, Node* prev = nullptr):
            item(item), next(next), prev(prev) {};
    };
    Node* head;
    Node* tail;
    size_t size;

public:
    LinkedList(): size(0), head(nullptr), tail(nullptr) {};

    LinkedList(const LinkedList& other) : size(0), head(nullptr), tail(nullptr) {
        Node* current = other.head;
        while (current != nullptr) {
            Append(current->item);
            current = current->next;
        }
    }

    T & GetFirst() const {
        if (head == nullptr) {
            throw std::out_of_range("List is empty");
        }
        return head->item;
    }

    T & GetLast() const {
        if (tail == nullptr) {
            throw std::out_of_range("List is empty");
        }
        return tail->item;
    }
    
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            Clear();
            Node* current = other.head;
            while (current != nullptr) {
                Append(current->item);
                current = current->next;
            }
        }
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    void Append(const T & item) {
        Node* node = new Node(item);
        if (size == 0) {
            tail = head = node;
        } else {
            tail -> next = node;
            node -> prev = tail;
            tail = node;
        }
        size++;
    }

    void Clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current -> next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    size_t GetLength() const{
        return size;
    }

    T& Get(const size_t & index) const{
        if (index >= size || index < 0) {
            throw out_of_range("Incorrect Index!");
        }
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current -> next;
        }
        return current -> item;
    }

    void Set(size_t index, const T & value) {
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current -> next;
        }

        current -> item = value;
    }
};

#endif /* LinkedList_h */


