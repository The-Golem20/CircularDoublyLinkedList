#pragma once

template <typename T>
class ListNode {
private:
    class Node {
    public:
        T data;
        Node* next;
        Node* prev;

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}

        Node(const T& value, Node* nextNode, Node* prevNode)
            : data(value), next(nextNode), prev(prevNode) {
        }
    };

    Node* head;
    size_t size;

public:
    ListNode() : head(nullptr), size(0) {}


    ListNode(std::initializer_list<T> initList) : head(nullptr), size(0) {
        for (const auto& item : initList) {
            pushBack(item);
        }
    }

    ~ListNode() {
        clear();
    }

    ListNode(const ListNode& other) : head(nullptr), size(0) {
        Node* current = other.head;
        if (current) {
            do {
                pushBack(current->data);
                current = current->next;
            } while (current != other.head);
        }
    }

    ListNode(ListNode&& other) noexcept : head(other.head), size(other.size) {
        other.head = nullptr;
        other.size = 0;
    }

    size_t getSize() const { return size; }

    bool isEmpty() const { return head == nullptr; }

    class Iterator {
    private:
        Node* current;
        Node* listHead;
        bool firstPass;
    public:
        Iterator(Node* start, Node* head) : current(start), listHead(head), firstPass(true) {}

        T& operator*() {
            if (!current) throw std::runtime_error("Dereferencing invalid iterator");
            return current->data;
        }

        Iterator& operator++() {
            if (!current) throw std::runtime_error("Incrementing invalid iterator");
            current = current->next;
            if (current == listHead) firstPass = false;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            if (!current) throw std::runtime_error("Decrementing invalid iterator");
            current = current->prev;
            if (current == listHead) firstPass = true;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current && firstPass == other.firstPass;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() {
        if (!head) {
            return Iterator(nullptr, nullptr);
        }
        return Iterator(head, head);
    }

    Iterator end() {
        if (!head) {
            return Iterator(nullptr, nullptr);
        }
        return Iterator(head, head);
    }

    ListNode& operator=(const ListNode& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            if (current) {
                do {
                    pushBack(current->data);
                    current = current->next;
                } while (current != other.head);
            }
        }
        return *this;
    }

    ListNode& operator=(ListNode&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            size = other.size;
            other.head = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ListNode operator+(const ListNode& other) const {
        ListNode result;
        Node* current = head;
        if (current) {
            do {
                result.pushBack(current->data);
                current = current->next;
            } while (current != head);
        }

        current = other.head;
        if (current) {
            do {
                result.pushBack(current->data);
                current = current->next;
            } while (current != other.head);
        }
        return result;
    }

    bool operator==(const ListNode& other) const {
        if (size != other.size) return false;

        Node* current1 = head;
        Node* current2 = other.head;

        for (size_t i = 0; i < size; ++i) {
            if (current1->data != current2->data) return false;
            current1 = current1->next;
            current2 = current2->next;
        }
        return true;
    }

    bool operator!=(const ListNode& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const ListNode& list) {
        if (list.isEmpty()) {
            os << "[]";
            return os;
        }

        os << "[";
        Node* current = list.head;
        do {
            os << current->data;
            if (current->next != list.head) {
                os << ", ";
            }
            current = current->next;
        } while (current != list.head);
        os << "]";

        return os;
    }

    friend std::istream& operator>>(std::istream& is, ListNode& list) {
        list.clear();
        T value;

        while (is >> value) {
            list.pushBack(value);
        }

        is.clear();

        return is;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void pushFront(const T& value) {
        Node* newNode = new Node(value);

        if (isEmpty()) {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else {
            Node* tail = head->prev;

            newNode->next = head;
            newNode->prev = tail;
            head->prev = newNode;
            tail->next = newNode;

            head = newNode;
        }
        size++;
    }

    void pushBack(const T& value) {
        Node* newNode = new Node(value);

        if (isEmpty()) {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else {
            Node* tail = head->prev;

            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        size++;
    }
    
    T popFront() {
        if (isEmpty()) {
            throw std::runtime_error("List is empty");
        }

        T value = head->data;

        if (head->next == head) {
            delete head;
            head = nullptr;
        }
        else {
            Node* newHead = head->next;
            Node* tail = head->prev;

            tail->next = newHead;
            newHead->prev = tail;

            delete head;
            head = newHead;
        }

        size--;
        return value;
    }

    T popBack() {
        if (isEmpty()) {
            throw std::runtime_error("List is empty");
        }

        Node* tail = head->prev;
        T value = tail->data;

        if (head->next == head) {
            delete head;
            head = nullptr;
        }
        else {
            Node* newTail = tail->prev;

            newTail->next = head;
            head->prev = newTail;

            delete tail;
        }

        size--;
        return value;
    }

    void insertAfter(const T& target, const T& value) {
        Node* current = findNode(target);
        if (!current) {
            throw std::runtime_error("Target not found");
        }

        Node* newNode = new Node(value);

        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;

        size++;
    }

    void remove(const T& value) {
        if (isEmpty()) return;

        Node* current = head;
        do {
            if (current->data == value) {
                if (current == head) {
                    head = (head->next == head) ? nullptr : head->next;
                }

                current->prev->next = current->next;
                current->next->prev = current->prev;

                delete current;
                size--;
                return;
            }
            current = current->next;
        } while (current != head);
    }

    private:
        Node* findNode(const T& value) {
            if (isEmpty()) return nullptr;

            Node* current = head;
            do {
                if (current->data == value) {
                    return current;
                }
                current = current->next;
            } while (current != head);

            return nullptr;
        }
};