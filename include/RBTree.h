#pragma once
#include <vector>
#include <string>

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

		Node(Node* l, Node* r, Node p, std::pair<TKey, TValue> value, Color color) : key(key), left(l), right(r), parent(p), value(value), color(color) {}
	};

private:

	Node* root;

	void swap(iterator first, iterator second) {
		std::pair<TKey, TValue> temp = *(first);
		*(first) = *(second);
		*(second) = temp;
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

		//x - левое поддерево y
		y->right = x;
		x->parent = y;
	}

	void bigLeftRotate(Node* x) {
		this->smallRightRotate(x->right);
		this->smallLeftRotate(x);
	}
	void bigRightRotate(Node* x) {
		this->smallLeftRotate(x->left);
		this->smallRightRotate(x);
	}

	//relatives
	Node* P(x) { //parent
		return x->parent;
	}
	Node* G(x) { //granny
		if (x->parent) return x->parent->parent;
		return nullptr;
	}
	Node* U(x) { //uncle
		if (x->parent && x->parent->parent) {
			if (x->parent == x->parent->parent->right) return x->parent->parent->left;
			return x->parent->parent->right;
		}
		return nullptr;
	}
	Node* S(x) { //sibling
		if (x->parent) {
			if (x == x->parent->right) return x->parent->left;
			return x->parent->right;
		}
		return nullptr;
	}

	Node* insert(const TKey& key, const TValue& value, Node* curr) {
		if(!curr) curr = new Node(nullptr, nullptr, nullptr, std::pair<TKey, TValue>(key, value), Color::RED);
		else if (curr->value.first < key) {
			if (curr->right) return this->insert(key, value, curr->right);
			curr->right = new Node(nullptr, nullptr, curr, std::pair<TKey, TValue>(key, value), Color::RED);
		}
		else {
			if (curr->value.first == key) throw "the element with this key already exists";
			if (curr->left) return this->insert(key, value, curr->left);
			curr->left = new Node(nullptr, nullptr, curr, std::pair<TKey, TValue>(key, value), Color::RED);
		}
		return curr;
	}

	void balanceInsert(Node* x) {
		if (!(x->parent)) {
			x->color = Color::BLACK;
			return;
		}
		if (this->P(x)->color == Color::BLACK) return;
		if (this->U(x)->color == Color::RED) {
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

public:

	class iterator {
	private:
		Node* it = nullptr;

	public:
		iterator(Node* it) : it(it) {}

		Node& operator*() { return *(this->it); }
		Node operator*() const { return *(this->it); }

		bool operator!=(const iterator& other) const { return (this->it != other.it); }
		bool operator==(const iterator& other) const { return !((*this) != other); }

		iterator& operator++() {
			if (this->it->right) {
				this->it = this->it->right;
				while (this->it->left) this->it = this->it->left;
				return (*this);
			}
			if (this->it->parent) {
				while (this->it->parent && this->it != this->it->parent->left) this->it = this->it->parent;
				if (this->it->parent && this->it == this->it->parent->left) {
					this->it = this->it->parent;
					return (*this);
				}
			}
			while (this->it->right) this->it = this->it->right;
			this->it = this->it->right;
			return (*this);
		}
		iterator operator++(int) {
			iterator i = (*this);
			++(*this);
			return i;
		}

	};

	iterator begin() {
		Node* ll = this->root;
		while (ll->left) ll = ll->left;
		return iterator(ll);
	}
	iterator end() {
		Node* rr = this->root;
		while (rr->right) rr = rr->right;
		rr = rr->right;
		return iterator(rr);
	}


	iterator insert(const TKey& key, const TValue& value) {
		Node* x = this->insert(key, value, this->root);
		this->balanceInsert(x);
		return iterator(x);
	}

	iterator find(const TKey& key) {
		Node* curr = this->root;
		while (curr && curr->value.first != key) {
			if (curr->value.first < key) curr = curr->right;
			if (curr->value.first > key) curr = curr->left;
		}
		return iterator(curr);
	}

	iterator erase(const TKey& key);

	TValue& operator[](const TKey& key);
	TValue operator[](const TKey& key) const;

	friend std::ostream& operator <<(std::ostream& ostr, RBTree& tree);
};