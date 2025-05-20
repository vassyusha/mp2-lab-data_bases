#pragma once
#include <iostream>

template<class Tkey, class Tvalue>
class avlTree {
public:
	class iterator;
private:
	struct Node {
		Node* left, * right, * parent;
		Tkey key;
		Tvalue value;
		int height;
		Node(const Tkey& k, const Tvalue& v, Node* p) : key(k), value(v), parent(p) {
			left = nullptr;
			right = nullptr;
			height = 1;
		}
	};
	Node * root;
	int count;
	int getHeight(Node* curr) const{
		if (curr != nullptr) return curr->height;
		return 0;
	}
	int getBalanceFactor(Node* curr) const{
		if (curr != nullptr) return (getHeight(curr->right) - getHeight(curr->left));
	}
	void updateHeight(Node * curr) {
		if (curr != nullptr) {
			curr->height=1+std::max(getHeight(curr->left), getHeight(curr->right));
		}
	}
	Node * erase(const Tkey& k, Node * curr) {
		if (!curr) throw "Tree is empty or key is not found\n";
		if (k < curr->key) {
			curr->left = erase(k, curr->left);
			return balance(curr);
		}
		if (k > curr->key) {
			curr->right = erase(k, curr->right);
			return balance(curr);
		}
		if (!curr->left && !curr->right) {
			delete curr;
			curr = nullptr;
			return curr;
		}
		if (!curr->right) { //нужна ли балансировка
			Node* tmp = curr->left;
			Node* tmp_parent = curr->parent;
			delete curr;
			tmp->parent = tmp_parent;
			return tmp;
		}
		Node* m = findMin(curr->right); //находим минимальный элемент в правом поддереве 
		curr->key = m->key;
		curr->value = m->value;
		curr->right = erase(m->key, curr->right);
		return balance(curr);
	}
	Node * findMin(Node* curr) const {
		Node* result = curr;
		while (curr) {
			result = curr;
			curr = curr->left;
		}
		return result;
	}
	Node* findMax(Node* curr) const {
		Node* result = curr;
		while (curr) {
			result = curr;
			curr = curr->right;
		}
		return result;
	}
	Node* findNext(Node* curr) const {
		Node* x = curr;
		if ((x != nullptr) && (x->right != nullptr)) {
			return findMin(x->right);
		}
		Node* y = curr->parent;
		while (y != nullptr && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}
	Node * insert(const Tkey& k, const Tvalue& v, Node * curr, Node * p) {
		if (curr == nullptr) return new Node(k, v, p);
		if (k == curr->key) throw "Try to insert element with an existing key\n";
		if (k < curr->key) curr->left = insert(k, v, curr->left,curr);
		else curr->right = insert(k, v, curr->right,curr);
		return balance(curr);
	}
	Node* balance(Node* curr) {
		if (curr != nullptr) {
			updateHeight(curr);
			if (getBalanceFactor(curr) == 2) {
				if(getHeight(curr->left)<getHeight(curr->right->left)) return big_left_rotate(curr);
				return small_left_rotate(curr);
			}
			if (getBalanceFactor(curr) == -2) {
				if (getHeight(curr->right) < getHeight(curr->left->right)) return big_right_rotate(curr);
				return small_right_rotate(curr);
			}
		}
		return curr;
	}
	Node* small_left_rotate(Node* a) {
		Node* a_parent = a->parent;
		Node* b = a->right;
		a->parent = b;
		b->parent = a_parent;
		a->right = b->left;
		b->left = a;
		updateHeight(a);
		updateHeight(b);
		return b;
	}
	Node* small_right_rotate(Node* a) {
		Node* a_parent = a->parent; 
		Node* b = a->left;
		a->parent = b;
		b->parent = a_parent;
		a->left = b->right;
		b->right = a;
		updateHeight(a);
		updateHeight(b);
		return b;
	}
	Node* big_left_rotate(Node* a) {
		a->right = small_right_rotate(a->right);
		return small_left_rotate(a);;
	}
	Node* big_right_rotate(Node* a) {
		a->left = small_left_rotate(a->left);
		return small_right_rotate(a);
	}
	Node * find(const Tkey& k, Node* curr) const{
		if (curr == nullptr) return nullptr;
		if (k == curr->key) return curr;
		if (k> curr->key) return find(k, curr->right);
		return find(k, curr->left);
	}
	bool isBalanced(Node * curr) const{
		if (curr == nullptr) return 1;
		if (abs(getBalanceFactor(curr)) > 2) return 0;
		return (isBalanced(curr->right) && isBalanced(curr->left));

	}
	size_t size(Node * curr){
		if (curr == nullptr) return count;
		size(curr->left);
		count++;
		size(curr->right);
		return count;
	}
	void print(Node* curr) const {
		if (!curr) return;
		print(curr->left);
		std::cout << curr->key << " " << curr->value << std::endl;
		print(curr->right);
	}
	void deletion(Node* curr) {
		if (!curr) return;
		deletion(curr->left);
		Node* tmp = curr->right;
		delete curr;
		deletion(tmp);
	}
	void copyLeftChild(Node * parent, Node * other) {
		if (other == nullptr) return;
		parent->left = new Node(other->key, other->value, parent);
		copyLeftChild(parent->left, other->left);
		copyRightChild(parent ->left, other->right);
	}
	void copyRightChild(Node* parent, Node* other) {
		if (other == nullptr) return;
		parent->right = new Node(other->key, other->value, parent);
		copyLeftChild(parent->right,other->left);
		copyRightChild(parent->right,other->right);
	}
public:
	avlTree() : root(nullptr) {}
	avlTree(const avlTree & other) {
		root = new Node(other.root->key, other.root->value, nullptr);
		copyLeftChild(root, other.root->left);
		copyRightChild(root, other.root->right);
	}
	avlTree& operator=(const avlTree& other) {
		deletion();
		root = new Node(other.root->key, other.root->value, nullptr);
		copyLeftChild(root, other.root->left);
		copyRightChild(root, other.root->right);
		return *this;
	}
	iterator findMin() const {
		Node* min = findMin(root);
		if (min != nullptr) return iterator(min);
		else return end();
	}
	iterator findMax() const {
		Node* max = findMax(root);
		if (max != nullptr) return iterator(max);
		else return end();
	}
	iterator findNext(const Tkey & k) const {
		Node * curr = find(k,root);
		Node * next = findNext(curr);
		if (next != nullptr) return iterator(next);
		return this->end(); 
	}
	iterator find(const Tkey& k) const{
		Node* result = this->find(k, root);
		if(result!=nullptr) return iterator(result);
		return this->end();
	}
	Tvalue& operator[](const Tkey& key) {
		if (this->find(key) != this->end()) return (*this->find(key)).value;
		else throw "Trying to use a not existing key";
	}
	Tvalue operator[](const Tkey& key) const {
		if (this->find(key) != this->end()) return (*this->find(key)).value;
		else throw "Trying to use a not existing key";
	}
	iterator insert(const Tkey& k,const Tvalue & v) {
		root = insert(k, v, this->root, this->root);
		return find(k);
	}
	iterator erase(const Tkey& k) {
		iterator next = findNext(k);
		if (next != end())
		{
			Tkey next_key = next->key;
			root = this->erase(k, this->root);
			return find(next_key);
		}
		root = this->erase(k, this->root);
		return this->end();
	}
	iterator begin() const {
		return iterator(findMin(root));
	}
	iterator end() const {
		if (findMax(root) != nullptr) return iterator(findMax(root)->right);
		else return iterator(root);
	}
	bool isBalanced() const{
		return isBalanced(root);
	}
	bool isEmpty() const {
		if (root == nullptr)  return 1;
		else return 0;
	}
	size_t size(){
		count = 0;
		return size(root);
	}
	std::pair<Tkey, Tvalue> getRoot() const{
		if(root != nullptr) return std::make_pair(root->key, root->value);
		throw "A tree is an empty\n";
	}
	void print() const{
		this->print(root);
	}
	void deletion() {
		deletion(root);
		root = nullptr;
	}
	~avlTree() {
		deletion(root);
	}
	class iterator {
		Node * it;
	public:
		iterator(Node * d) : it(d) {}
		iterator(const iterator * other) {
			it = other;
		}
		iterator operator++(int) {
			Node* x = it;
			if ((x != nullptr) && (x->right != nullptr)) {
				Node * curr = x->right;
				while (curr->left) curr = curr->left;
				it = curr; //присваивает найденный, а возвращает старый 
				return iterator(x);
			}
			Node* y = x->parent;
			while (y != nullptr && x == y->right) {
				x = y;
				y = y->parent;
			}
			Node* tmp = x;
			it = y;
			return iterator(x); //если в find next корень засунуть 
		}
		Node& operator*() { 
			if (it != nullptr) return *it;
			else throw "An attempt to dereference an empty pointer\n";
		}
		Node& operator*() const{
			if (it != nullptr) return *it;
			else throw "An attempt to dereference an empty pointer\n";
		}
		bool operator!=(const iterator& other) const {
			return this->it != other.it;
		}
		bool operator==(const iterator& other) const {
			return this->it == other.it;
		}
		Node * operator->() {
			if (it != nullptr) return it;
			else throw "An attempt to dereference an empty pointer\n";
		}
	};
};