#pragma once
#include <string>
#include <iostream>

template<class TKey, class TValue>
class RBTree {
public:
	class iterator;
	enum Color {
		RED,
		BLACK
	};

protected:
	struct Node {
		Node* left, * right, * parent;
		std::pair<TKey, TValue> value;
		Color color; // RED or BLACK
		bool isnull = false;

		Node(Node* l = nullptr, Node* r = nullptr, Node* p = nullptr, std::pair<TKey, TValue> value = std::pair<TKey, TValue>(), Color color = Color::RED, bool isnull = false) : left(l), right(r), parent(p), value(value), color(color), isnull(isnull) {}

	};

private:

	Node* root = nullptr;

	void swap(Node* first, Node* second) {
		std::pair<TKey, TValue> temp = first->value;
		first->value = second->value;
		second->value = temp;
	}

	void smallLeftRotate(Node* x) {
		Node* y = x->right;

		//B - правое поддерево x
		x->right = y->left;
		if (y->left) y->left->parent = x;

		//y - новый корень поддерева
		y->parent = x->parent;
		if (x->parent) {
			if (x->parent->right == x) x->parent->right = y;
			else x->parent->left = y;
		}
		else this->root = y;

		//x - левое поддерево y
		y->left = x;
		x->parent = y;
	}
	void smallRightRotate(Node* x) {
		Node* y = x->left;

		//B - правое поддерево x
		x->left = y->right;
		if (y->right) y->right->parent = x;

		//y - новый корень поддерева
		y->parent = x->parent;
		if (x->parent) {
			if (x->parent->right == x) x->parent->right = y;
			else x->parent->left = y;
		}
		else this->root = y;

		//x - левое поддерево y
		y->right = x;
		x->parent = y;
	}

	//relatives
	Node* P(Node* x) { //parent
		return x->parent;
	}
	Node* G(Node* x) { //granny
		if (x->parent) return x->parent->parent;
		return nullptr;
	}
	Node* U(Node* x) { //uncle
		if (x->parent && x->parent->parent) {
			if (x->parent == x->parent->parent->right) return x->parent->parent->left;
			return x->parent->parent->right;
		}
		return nullptr;
	}
	Node* S(Node* x) { //sibling
		if (x->parent) {
			if (x == x->parent->right) return x->parent->left;
			return x->parent->right;
		}
		return nullptr;
	}

	Node* insert(const TKey& key, const TValue& value, Node* curr) {
		if (!curr) {
			curr = new Node(nullptr, nullptr, nullptr, std::pair<TKey, TValue>(key, value), Color::RED);
			this->root = curr;
			return curr;
		}
		if (curr->value.first == key) throw "the element with this key already exists";
		else if (curr->value.first < key) {
			if (curr->right) return this->insert(key, value, curr->right);
			curr->right = new Node(nullptr, nullptr, curr, std::pair<TKey, TValue>(key, value), Color::RED);
			return curr->right;
		}
		else {
			if (curr->left) return this->insert(key, value, curr->left);
			curr->left = new Node(nullptr, nullptr, curr, std::pair<TKey, TValue>(key, value), Color::RED);
			return curr->left;
		}
	}

	Node* find(const TKey& key, Node* curr) {
		if (!curr || curr->value.first == key) return curr;
		if (curr->value.first < key) return this->find(key, curr->right);
		if (curr->value.first > key) return this->find(key, curr->left);
	}

	Node* next(Node* curr) {
		Node* next = curr;
		if (next->right) {
			next = next->right;
			while (next->left) next = next->left;
			return next;
		}
		if (next->parent) {
			while (next->parent && next != next->parent->left) next = next->parent;
			if (next->parent && next == next->parent->left) {
				next = next->parent;
				return next;
			}
		}
		return nullptr;
	}

	void balanceInsert(Node* x) {
		if (!(x->parent)) {
			x->color = Color::BLACK;
			return;
		}
		if (this->P(x)->color == Color::BLACK) return;
		if (this->U(x) && this->U(x)->color == Color::RED) {
			this->P(x)->color = Color::BLACK;
			this->U(x)->color = Color::BLACK;
			this->G(x)->color = Color::RED;
			balanceInsert(this->G(x));
			return;
		}
		if (this->P(x) == this->G(x)->left) {
			if (x == this->P(x)->right) {
				x = this->P(x);
				this->smallLeftRotate(x);
			}
			this->G(x)->color = Color::RED;
			this->P(x)->color = Color::BLACK;
			this->smallRightRotate(this->G(x));
			return;
		}
		if (this->P(x) == this->G(x)->right) {
			if (x == this->P(x)->left) {
				x = this->P(x);
				this->smallRightRotate(x);
			}
			this->G(x)->color = Color::RED;
			this->P(x)->color = Color::BLACK;
			this->smallLeftRotate(this->G(x));
			return;
		}
	}

	void balanceDelete(Node* x) {
		if (!(this->P(x)) || x->color == Color::RED) {
			x->color = Color::BLACK;
			return;
		}
		if (this->S(x)->color == Color::RED) {
			this->P(x)->color = Color::RED;
			this->S(x)->color = Color::BLACK;
			if (x == this->P(x)->left) this->smallLeftRotate(this->P(x));
			else this->smallRightRotate(this->P(x));
		}
		if (this->S(x)->color == Color::BLACK && this->P(x)->color == Color::BLACK && (!this->S(x)->left || this->S(x)->left->color == Color::BLACK) && (!this->S(x)->right || this->S(x)->right->color == Color::BLACK)) {
			this->S(x)->color = Color::RED;
			this->balanceDelete(this->P(x));
			return;
		}
		if (this->S(x)->color == Color::BLACK && this->P(x)->color == Color::RED && (!this->S(x)->left || this->S(x)->left->color == Color::BLACK) && (!this->S(x)->right || this->S(x)->right->color == Color::BLACK)) {
			this->S(x)->color = Color::RED;
			this->P(x)->color = Color::BLACK;
			return;
		}
		if (x == this->P(x)->left) {
			if ((this->S(x)->left && this->S(x)->left->color == Color::RED) && (!this->S(x)->right || this->S(x)->right->color == Color::BLACK)) {
				this->S(x)->color = Color::RED;
				this->S(x)->left->color == Color::BLACK;
				this->smallRightRotate(this->S(x)); 
			}
			this->S(x)->color = this->P(x)->color; 
			this->P(x)->color = BLACK;
			if(this->S(x)->right) this->S(x)->right->color = BLACK;
			this->smallLeftRotate(this->P(x));
			return;
		}
		else if (x == this->P(x)->right) {
			if ((this->S(x)->right && this->S(x)->right->color == Color::RED) && (!this->S(x)->left || this->S(x)->left->color == Color::BLACK)) {
				this->S(x)->color = Color::RED;
				this->S(x)->right->color == Color::BLACK;
				this->smallLeftRotate(this->S(x));
			}
			this->S(x)->color = this->P(x)->color;
			this->P(x)->color = BLACK;
			if(this->S(x)->left)this->S(x)->left->color = BLACK;
			this->smallRightRotate(this->P(x));
			return;
		}

	}

	void clear(Node* x) {
		if (x) {
			if (x->left) this->clear(x->left);
			if(x->right) this->clear(x->right);
		}
		delete x;
		return;
	}

	void assign(Node* target, Node* other) {
		target->value = other->value;
		target->color = other->color;
		if (other->left) {
			target->left = new Node(nullptr, nullptr, target);
			this->assign(target->left, other->left);
		}
		if (other->right) {
			target->right = new Node(nullptr, nullptr, target);
			this->assign(target->right, other->right);
		}
		return;
	}


public:

	class iterator {
	private:
		Node* it = nullptr;
		RBTree* owner = nullptr;

	public:
		iterator(Node* it, RBTree* owner) : it(it), owner(owner) {}

		Node* operator*() { return this->it; }

		bool operator!=(const iterator& other) const { return (this->it != other.it); }
		bool operator==(const iterator& other) const { return !((*this) != other); }

		iterator& operator++() {
			this->it = owner->next(it);
			return (*this);
		}
		iterator operator++(int) {
			iterator i = (*this);
			++(*this);
			return i;
		}


	};

	RBTree() {
		this->root = nullptr;
	}

	//правило 3-х, еще конструтор копирования и присваивания
	RBTree(const RBTree& other) {
		if (!other.root) return;
		this->root = new Node;
		this->assign(this->root, other.root);
	}

	RBTree& operator=(const RBTree& other) {
		this->clear(this->root);
		if (!other.root) return (*this);
		this->root = new Node;
		this->assign(this->root, other.root);
		return (*this);
	}

	~RBTree() {
		this->clear(this->root);
		this->root = nullptr;
	}

	iterator begin() {
		Node* ll = this->root;
		while (ll->left) ll = ll->left;
		return iterator(ll, this);
	}
	iterator end() {
		return iterator(nullptr, this);
	}

	iterator insert(const TKey& key, const TValue& value) {
		Node* x = this->insert(key, value, this->root);
		this->balanceInsert(x);
		return iterator(x, this);
	}

	iterator find(const TKey& key) {
		return iterator(this->find(key, this->root), this);
	}
	iterator erase(const TKey& key) {

		Node* curr = this->find(key, this->root);
		if (!curr) throw "you are trying to erase a non-existant element";

		Node* y, * x;//y - вершина, с которой свапнем удаляемую; x - потенциальный правый ребенок y
		if (!curr->left || !curr->right) y = curr; //если y - лист или у нее только 1 ребенок, то ее же и будем удалять
		else y = this->next(curr);//в ином случае находим вершину, на которую будем менять

		if (y->left) x = y->left; //в случае, если единственный лист - левый, то его и будем подтягивать
		else x = y->right; //в ином случае соответственно подтягиваем правый
		if (x) x->parent = y->parent; //прикрепляем х к новому родителю
		else x = new Node(nullptr, nullptr, y->parent, std::pair<TKey, TValue>(), Color::BLACK, true);

		if (y->parent) { //если не корень, то родителю тоже сообщаем о новых детях
			if (y == y->parent->left) y->parent->left = x;
			else y->parent->right = x;
		}
		else this->root = x;

		if (y != curr) swap(curr, y); //меняем значения удаляемой вершины и той, с которой можем безобидно поменять

		Color yColor = y->color;
		delete y;
		if (yColor == Color::BLACK) balanceDelete(x);
		if (x->isnull) {
			if (x->parent) {
				if (x == x->parent->left)x->parent->left = nullptr;
				else x->parent->right = nullptr;
			}
			else {
				this->root = nullptr;
				curr = nullptr;
			}
			delete x;
		}
		return iterator(curr, this);

	}

	TValue& operator[](const TKey& key) {
		if (this->find(key) == this->end()) {
			this->insert(key, TValue());
		}
		return (*(this->find(key)))->value.second;
	}
	TValue operator[](const TKey& key) const {
		if (this->find(key) == this->end()) throw "an element with such key doesnt exist";
		return (*(this->find(key)))->value.second;
	}

	TValue& at(iterator it) {
		return (*it)->value.second;
	}
	TValue at(iterator it) const{
		return (*it)->value.second;
	}

	bool empty() {
		if (!this->root) return true;
		return false;
	}

	void printBT(const std::string& prefix, const Node* node, bool isLeft)
	{
		if (node != nullptr)
		{
			std::cout << prefix;
			std::cout << (isLeft ? "|--" : "+--");
			// print the value of the node
			if (node->color == Color::RED) std::cout << "\033[31m";
			//else std::cout << termocolor::white;
			std::cout << node->value.first << "\033[0m" << std::endl;
			// enter the next tree level - left and right branch
			printBT(prefix + (isLeft ? "|   " : "    "), node->left, true);
			printBT(prefix + (isLeft ? "|   " : "    "), node->right, false);
		}
	}
	void printBT(const Node* node)
	{
		printBT("", node, false);
	}

	void print() {
		printBT(this->root);
	}

	friend std::ostream& operator <<(std::ostream& ostr, RBTree& tree);
};