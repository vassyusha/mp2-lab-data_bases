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

		//тут уже все переделывать надо, это анрил
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


	iterator insert(const TKey& key, const TValue& value);

	iterator find(const TKey& key) {
		Node* curr = this->root;
		while (curr && curr->value.first != key) {
			if (curr->value.first < key) curr = curr->right;
			if (curr->value.first > key) curr = curr->left;
		}
		return iterator(curr);
	}

	iterator erase(const iterator& it);
	iterator erase(const TKey& key);

	TValue& operator[](const TKey& key);
	TValue operator[](const TKey& key) const;

	friend std::ostream& operator <<(std::ostream& ostr, RBTree& tree);
};