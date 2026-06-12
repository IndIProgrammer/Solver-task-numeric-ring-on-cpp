#include "list.h"
#include "exceptions.h"
#include <algorithm>

CircularList::CircularList() : head(nullptr), size(0) {}

CircularList::~CircularList() {
    clear();
}

void CircularList::pushBack(char val) {
    Node* newNode = new Node(val);
    if (!head) {
        head = newNode;
        head->setNext(head);
        head->setPrev(head);
    } else {
        Node* tail = head->getPrev();
        tail->setNext(newNode);
        newNode->setPrev(tail);
        newNode->setNext(head);
        head->setPrev(newNode);
    }
    size++;
}

void CircularList::clear() {
    if (!head) return;
    Node* current = head;
    for (int i = 0; i < size; ++i) {
        Node* temp = current;
        current = current->getNext();
        delete temp;
    }
    head = nullptr;
    size = 0;
}

int CircularList::getSize() const {
    return size;
}

bool CircularList::isEmpty() const {
    return size == 0;
}

void CircularList::loadFromString(const std::string& input) {
    clear();
    if (input.empty()) {
        throw InvalidInputException("Ошибка: пустая строка");
    }
    for (char c : input) {
        if (c < '0' || c > '9') {
            throw InvalidInputException("Ошибка: строка содержит не только цифры");
        }
        pushBack(c);
    }
}

std::string CircularList::extractString(Node* startNode, int length) const {
    if (isEmpty()) throw EmptyListException();
    if (length == 0) return "";

    std::string result = "";
    Node* current = startNode;
    for (int i = 0; i < length; ++i) {
        result += current->getData();
        current = current->getNext();
    }
    return result;
}

bool CircularList::isValidNumber(const std::string& s) const {
    if (s.empty()) return false;
    if (s.length() > 1 && s[0] == '0') return false;
    return true;
}

std::string CircularList::addBigNumbers(const std::string& num1, const std::string& num2) const {
    std::string result = "";
    int carry = 0;
    int i = static_cast<int>(num1.length()) - 1;
    int j = static_cast<int>(num2.length()) - 1;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += num1[i--] - '0';
        if (j >= 0) sum += num2[j--] - '0';
        carry = sum / 10;
        result += static_cast<char>((sum % 10) + '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string CircularList::findSolution() const {
    int n = size;
    if (n < 3) throw NoSolutionException();

    Node* startNode = head;

    for (int shift = 0; shift < n; ++shift) {
        for (int lenA = 1; lenA <= n - 2; ++lenA) {
            for (int lenB = 1; lenB <= n - lenA - 1; ++lenB) {
                int lenC = n - lenA - lenB;

                int maxAB = std::max(lenA, lenB);
                if (lenC < maxAB || lenC > maxAB + 1) continue;

                std::string A = extractString(startNode, lenA);

                Node* nodeB = startNode;
                for (int k = 0; k < lenA; ++k) nodeB = nodeB->getNext();
                std::string B = extractString(nodeB, lenB);

                Node* nodeC = nodeB;
                for (int k = 0; k < lenB; ++k) nodeC = nodeC->getNext();
                std::string C = extractString(nodeC, lenC);

                if (isValidNumber(A) && isValidNumber(B) && isValidNumber(C)) {
                    if (addBigNumbers(A, B) == C) {
                        return A + "+" + B + "=" + C;
                    }
                }
            }
        }
        startNode = startNode->getNext();
    }

    throw NoSolutionException();
}
