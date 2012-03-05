#ifndef TRISS_DOUBLE_LINKED_LIST
#define TRISS_DOUBLE_LINKED_LIST

#include <cstdlib>

template <class T>
class List {

	private:
	class Node {
		public:
		Node(T value) {
			this -> value = value;
			prev = next = NULL;
		}
		T value;
		Node *prev, *next;
	};

	Node *head, *tail;
	int size;

	void insertBefore(T value, Node* nextNode);
    Node* getNodeAt(int position);
    T deleteNode(Node *old);

    public:
    List();
    ~List();

	class Iterator {
		public:
		Iterator(Node* head, Node* tail) {
			this -> head = head;
			this -> tail = tail;
		}

		bool hasNext();
		T next();

		private:
		Node *head, *tail;
	};

	void pushBack(T element);
	void pushFront(T element);

	T getFront();
	T getBack();

	T popFront();
	T popBack();

	int getSize();
	bool isEmpty();

	void insert(T element, int position);
	T getElementAt(int position);
	T remove(int position);

	Iterator getIterator();
};

template <class T>
void List<T> :: pushBack(T element) {
	insertBefore(element, tail);
}

template <class T>
void List<T> :: pushFront(T element) {
	insertBefore(element, head -> next);
}

template <class T>
T List<T> :: getFront() {
    return head -> next -> value;
}

template <class T>
T List<T> :: getBack() {
    return tail -> prev -> value;
}

template <class T>
T List<T> :: popFront() {
    return deleteNode(head -> next);
}

template <class T>
T List<T> :: popBack() {
    return deleteNode(tail -> prev);
}

template <class T>
int List<T> :: getSize() {
    return size;
}

template <class T>
bool List<T> :: isEmpty() {
    return size == 0;
}

template <class T>
void List<T> :: insert(T element, int position) {
	insertBefore(element, getNodeAt(position));
}

template <class T>
T List<T> :: getElementAt(int position) {
    return getNodeAt(position) -> value;
}

template <class T>
T List<T> :: remove(int position) {
    return deleteNode(getNodeAt(position));
}

template <class T>
typename List<T> :: Iterator List<T> :: getIterator() {
    Iterator it(head, tail);
    return it;
}

template <class T>
bool List<T> :: Iterator :: hasNext() {
    return head -> next != tail;
}

template <class T>
T List<T> :: Iterator :: next() {
    head = head -> next;
    return head -> value;
}

template<class T> T List<T> :: deleteNode(Node *old) {
    old -> prev -> next = old -> next;
    old -> next -> prev = old -> prev;
    T value = old-> value;
    delete old;
    --size;
    return value;
}

template <class T>
void List<T> :: insertBefore(T value, Node *nextNode) {
	Node* newElement = new Node(value);
	newElement -> next = nextNode;
	newElement -> prev = nextNode -> prev;
	newElement -> next -> prev = newElement;
	newElement -> prev -> next = newElement;
	++size;
}

template <class T>
typename List<T> :: Node* List<T> :: getNodeAt(int position) {
    Node *current = head -> next;
    for(int i = 0; i < position; ++i) {
        current = current-> next;
    }
    return current;
}

template <class T>
List<T> :: List() {
	head = new Node(NULL);
	tail = new Node(NULL);
	head -> next = tail;
	tail -> prev = head;
	size = 0;
}

template <class T>
List<T> :: ~List() {
	Node *prev = NULL, *current = head;
	while(current -> next != NULL) {
		prev = current;
		current = current -> next;
		delete prev;
	}
}

#endif
