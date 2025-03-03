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
	Node* first = nullptr;
	Node* HEAD() {
		return first;
	}
	Node* HEAD() const {
		return first;
	}
	cyclicList(int n = 0) {
		if (n >= 0) {
			if (n > 0) {
				Node* tmp = new Node(first, T()); //a pointer to the first created Node
				first = tmp;
				n--;
				while (n) {
					Node* newNode = new Node(first, T());
					first = newNode;
					n--;
				}
				tmp->next = first; //the first created Node point to the head 
			}
		}
		else throw "Incorrect size of list\n";

	}
	cyclicList(const cyclicList& other) {
		Node* curr2 = other.first;
		if (curr2 != nullptr) {
			Node* curr1 = new Node(this->first, curr2->value);
			first = curr1;
			Node* tmpOther = curr2;  //the head of the other list
			Node* tmpThis = curr1; //the head of this list
			curr2 = curr2->next;
			while (curr2 != tmpOther) {
				Node* nextNode = new Node(nullptr, curr2->value);
				curr1->next = nextNode;
				curr1 = nextNode;
				curr2 = curr2->next;
			}
			curr1->next = tmpThis; //the end of the new list points to the head 
		}
		else throw "another list is empty\n";
	}
	Node& operator[](int index) {
		if (first != nullptr) {
			Node* tmpFirst = first;
			Node* curr = first;
			int i = 0;
			while ((i != index) && (curr->next != tmpFirst)) {
				curr = curr->next;
				i++;
			}
			if (i == index) {
				return *curr;
			}
			else throw "incorrect index\n";
		}
		else throw "list is empty\n";
	}
	cyclicList& operator=(const cyclicList& other) {
		if (this->first != nullptr) {
			Node* tmpFirst = this->first;
			Node* curr = this->first->next;
			this->first = curr;
			while (curr != tmpFirst) {
				curr = first->next;
				delete first;
				first = curr;
			}
			delete curr;
		}
		this->first = nullptr;
		Node* curr2 = other.first;
		if (curr2 != nullptr) {
			Node* curr1 = new Node(this->first, curr2->value);
			first = curr1;
			Node* tmpOther = curr2;  //the head of the other list
			Node* tmpThis = curr1; //the head of this list
			curr2 = curr2->next;
			while (curr2 != tmpOther) {
				Node* nextNode = new Node(nullptr, curr2->value);
				curr1->next = nextNode;
				curr1 = nextNode;
				curr2 = curr2->next;
			}
			curr1->next = tmpThis; //the end of the new list points to the head 
		}
		return *this;
	}
	size_t size() const {
		size_t count = 0;
		if (this->first != nullptr) {
			Node* tmpFirst = first;
			Node* curr = first;
			count++;
			while (curr->next != tmpFirst) {
				count++;
				curr = curr->next;
			}
		}
		return count;
	}
	void erase_after(Node* prev) {
		if (this->first != nullptr) {
			if (prev->next != first) {
				Node* tmp = prev->next->next;
				delete prev->next;
				prev->next = tmp;
			}
			else {
				erase_front();
			}
		}
	}
	void erase_front() {
		if (this->first != nullptr) {
			if (first->next != first) {
				Node* tmp = first->next;
				first->value = first->next->value;
				first->next = first->next->next;
				delete tmp;

			}
			else {      //случай, если в массиве один элемент
				delete first;
				first = nullptr;
			}
		}
	}
	void erase_back() {
		if (first != nullptr) {
			if (first->next != first) {
				Node* tmpFirst = first;
				Node* curr = first;
				while (curr->next->next != tmpFirst) {
					curr = curr->next;
				}
				erase_after(curr);
			}
			else {
				delete first;
				first = nullptr;
			}
		}
		else throw "an empty list\n";
	}
	Node* insert_after(const T& value, Node* prev) {
		if (prev != nullptr) {
			prev->next = new Node(prev->next, value);
			return prev->next;
		}
		first = new Node(first, value);
		first->next = first;
		return first;
	}
	Node* insert(T data) {
		if (first != nullptr) {
			Node* curr = first;
			if (first->next != first) {
				if (first->value < data) { //случай, если нужно вставлять перед первым элементом 
					Node* newNode = new Node(first->next, first->value);
					first->next = newNode;
					first->value = data;
					return first;
				}
				while ((data < curr->next->value) && (curr->next != first)) {
					curr = curr->next;
				}
				Node* newNode = new Node(curr->next, data);
				curr->next = newNode;
				return newNode;
			}
			else { //случай, если в списке один элемент
				if (first->value < data) {
					Node* newNode = new Node(first->next, first->value);
					first->next = newNode;
					first->value = data;
					return first;
				}
				else {
					first->next = new Node(first, data);
					return first->next;
				}
			}
		}
		else {
			first = new Node(first, data);
			first->next = first;
			return first;
		}
	}
	void print() const {
		if (first != nullptr) {
			Node* tmpFirst = first;
			first->value.print();
			Node* curr = first->next;
			while (curr != tmpFirst) {
				curr->value.print();
				curr = curr->next;
			}
			std::cout << std::endl;
		}
		else throw "list is empty\n";
	}
	bool operator==(const cyclicList<T>& other) const{
		if (this->size() != other.size()) {
			return 0;
		}
		if (first != nullptr) {
			Node* curr1 = first;
			Node* curr2 = other.first;
			if ((curr1->value != curr2->value) || (curr1->value.coef != curr2->value.coef)) return 0;
			curr1 = curr1->next;
			curr2 = curr2->next;
			while (curr1 != first) {
				if ((curr1->value != curr2->value)||(curr1->value.coef!=curr2->value.coef)) {
					return 0;
				}
				curr1 = curr1->next;
				curr2 = curr2->next;
			}
		}
		return 1;
	}
	bool operator!=(const cyclicList<T>& other) {
		return !(*this == other);
	}
	~cyclicList() {
		if (first != nullptr) {
			Node* tmpFirst = first;
			Node* curr = first->next;
			first = curr;
			while (curr != tmpFirst) {
				curr = first->next;
				delete first;
				first = curr;
			}
			delete curr;
		}
	}
};
