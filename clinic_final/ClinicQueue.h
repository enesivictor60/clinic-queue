#ifndef CLINIC_QUEUE_H
#define CLINIC_QUEUE_H

#include <iostream>
#include <fstream>
#include <stdexcept>

template <typename T>
class LinkedQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
    };

    Node* frontNode;
    Node* backNode;
    int count;

public:

    LinkedQueue() {
        frontNode = nullptr;
        backNode = nullptr;
        count = 0;
    }

    ~LinkedQueue() {
        while (frontNode != nullptr) {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
    }

    LinkedQueue(const LinkedQueue& other) {
        frontNode = nullptr;
        backNode = nullptr;
        count = 0;
        Node* current = other.frontNode;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }

    LinkedQueue& operator=(const LinkedQueue& other) {
        if (this != &other) {
            while (frontNode != nullptr) {
                Node* temp = frontNode;
                frontNode = frontNode->next;
                delete temp;
            }
            frontNode = nullptr;
            backNode = nullptr;
            count = 0;

            Node* current = other.frontNode;
            while (current != nullptr) {
                enqueue(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    bool isEmpty() {
        if (count == 0) return true;
        return false;
    }

    int size() {
        return count;
    }

    void enqueue(T item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            frontNode = newNode;
            backNode = newNode;
        } else {
            backNode->next = newNode;
            backNode = newNode;
        }
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        Node* temp = frontNode;
        T item = temp->data;
        frontNode = frontNode->next;

        if (frontNode == nullptr) {
            backNode = nullptr;
        }
        delete temp;
        count--;
        return item;
    }

    T front() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return frontNode->data;
    }

    int loadFromFile(std::string filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }
        std::string line;
        int loaded = 0;
        while (std::getline(file, line)) {
            if (line.length() > 0) {
                enqueue(T::fromLine(line));
                loaded++;
            }
        }
        file.close();
        return loaded;
    }

    void saveToFile(std::string filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not save file");
        }
        Node* current = frontNode;
        while (current != nullptr) {
            file << current->data.toLine() << "\n";
            current = current->next;
        }
        file.close();
    }
};

#endif
