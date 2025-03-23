#pragma once
#include <iostream>
template<class T>
struct cyclicList {
	struct Node {
		Node* next;
		T value;
		Node(Node* n, T data) {
			next = n;
			value = data;
		}
	};
	Node* first;
	Node* HEAD() {
		return first;
	}
	Node* HEAD() const {
		return first;
	}
	cyclicList(int n = 0) {
		if (n >= 0) {
			Node* tmp = new Node(first, T()); //фиктивная нода
			first = tmp;
			while (n > 0) {
				Node* newNode = new Node(first, T());
				first = newNode;
				n--;
			}
			tmp->next = first; //the first created Node point to the head 
		}
		else throw"incorrect size of list\n";
	}
	cyclicList(const cyclicList& other) {
		Node* curr2 = other.first;
		Node* curr1 = new Node(this->first, curr2->value);
		first = curr1;

		curr2 = curr2->next;
		while (curr2 != other.HEAD()) {
			Node* nextNode = new Node(nullptr, curr2->value);
			curr1->next = nextNode;
			curr1 = nextNode;
			curr2 = curr2->next;
		}
		curr1->next = first; //the end of the new list points to the head 
	}
	Node& operator[](int index) {
		Node* curr = first->next;
		int i = 0;
		while ((i != index) && (curr->next != this->HEAD())) {
			curr = curr->next;
			i++;
		}
		if (i == index) return *curr;
		else throw "incorrect index\n";
	}
	cyclicList& operator=(const cyclicList& other) {
		if (this->first != nullptr) {
			Node* curr = this->first->next;
			first = curr;
			while (curr != this->HEAD()) {
				curr = first->next;
				delete first;
				first = curr;
			}
			delete curr;
		}
		Node* curr2 = other.first;
		Node* curr1 = new Node(this->first, curr2->value);
		first = curr1;

		curr2 = curr2->next;
		while (curr2 != other.HEAD()) {
			Node* nextNode = new Node(nullptr, curr2->value);
			curr1->next = nextNode;
			curr1 = nextNode;
			curr2 = curr2->next;
		}
		curr1->next = first; //the end of the new list points to the head 
		return *this;
	}
	size_t size() const {
		size_t count = 0;
		Node* curr = first->next;
		while (curr != this->HEAD()) {
			count++;
			curr = curr->next;
		}
		return count;
	}
	void erase_after(Node* prev) {
		if (prev->next != first) {
			Node* tmp = prev->next->next;
			delete prev->next;
			prev->next = tmp;
		}
		else {
			erase_front();
		}
	}
	void erase_front() { //проверить случай, если в массиве один элемент
		if (first->next != first) {
			Node* tmp = first->next;
			first->next = first->next->next;
			delete tmp;
		}
	}
	void erase_back() {
		if (first->next != first) {
			Node* curr = first;
			while (curr->next->next != this->HEAD()) {
				curr = curr->next;
			}
			erase_after(curr);
		}
	}
	Node* insert_after(const T& value, Node* prev) {
		prev->next = new Node(prev->next, value);
		return prev->next;
	}
	Node* insert(T data) {
		Node* curr = first;
		if (first->next != first) {
			while ((data < curr->next->value) && (curr->next != this->HEAD())) {
				curr = curr->next;
			}
			Node* newNode = new Node(curr->next, data);
			curr->next = newNode;
			return newNode;
		}
		else {
			Node* newNode = new Node(first->next, data);  //случай, если в списке 0 элементов
			first->next = newNode;
			return first->next;
		}
	}
	void print() const {
		Node* curr = first->next;
		while (curr != this->HEAD()) {
			curr->value.print();
			curr = curr->next;
		}
		std::cout << std::endl;
	}
	friend std::ostream& operator <<(std::ostream& ostr, cyclicList& c) {
		Node* curr = c.first->next;
		while (curr != this->HEAD()) {
			ostr << curr->value;
			curr = curr->next;
		}
		ostr << std::endl;
		return ostr;
	}
	bool operator==(const cyclicList<T>& other) const {
		if (this->size() != other.size()) {
			return 0;
		}
		Node* curr1 = first->next;
		Node* curr2 = other.first->next;
		while (curr1 != this->HEAD()) {
			if ((curr1->value != curr2->value) || (curr1->value.coef != curr2->value.coef)) {
				return 0;
			}
			curr1 = curr1->next;
			curr2 = curr2->next;
		}
		return 1;
	}
	bool operator!=(const cyclicList<T>& other) {
		return !(*this == other);
	}
	~cyclicList() {
		Node* curr = first->next;
		first = curr;
		while (curr != this->HEAD()) {
			curr = first->next;
			delete first;
			first = curr;
		}
		delete this->HEAD();
	}
};
