#ifndef LIST_H
#define LIST_H

#include <string>

class Node {
private:
    char data;
    Node* next;
    Node* prev;

public:
    explicit Node(char val) : data(val), next(nullptr), prev(nullptr) {}

    char getData() const { return data; }
    Node* getNext() const { return next; }
    Node* getPrev() const { return prev; }

    void setNext(Node* n) { next = n; }
    void setPrev(Node* p) { prev = p; }
};

class CircularList {
private:
    Node* head;
    int size;

    bool isValidNumber(const std::string& s) const;
    std::string addBigNumbers(const std::string& num1, const std::string& num2) const;

public:
    CircularList();
    ~CircularList();

    void pushBack(char val);
    void clear();
    int getSize() const;
    bool isEmpty() const;

    void loadFromString(const std::string& input);
    std::string extractString(Node* startNode, int length) const;
    std::string findSolution() const;
};

#endif
