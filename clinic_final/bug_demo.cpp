#include <iostream>
using namespace std;

struct Node { int id; Node* next; };

struct BuggyQueue {
    Node* front = nullptr;
    Node* back = nullptr;

    void enqueue(int id) {
        Node* n = new Node{id, nullptr};
        if (back == nullptr) front = back = n;
        else { back->next = n; back = n; }
    }

    void dequeue() {
        Node* old = front;
        front = front->next;

        delete old;
    }
};

int main() {
    BuggyQueue q;
    q.enqueue(1);
    q.dequeue();
    q.enqueue(2);
    cout << "done\n";
    return 0;
}
