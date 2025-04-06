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
		if (!curr) {
			curr = new Node(nullptr, nullptr, nullptr, std::pair<TKey, TValue>(key, value), Color::RED);
			this->root = curr;
			return curr;
		}
		if (curr->value.first == key) throw "the element with this key already exists";
		else if (curr->value.first < key) {
			if (curr->right) return this->insert(key, value, curr->right);
			curr->right = new Node(nullptr, nullptr, curr, std::pair<TKey, TValue>(key, value), Color::RED);
		}
		else {
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
		if (this->S(x)->color == Color::BLACK && this->P(x)->color == Color::BLACK && this->S(x)->left->color == Color::BLACK && this->S(x)->right->color == Color::BLACK) {
			this->S(x)->color = Color::RED;
			this->balanceDelete(this->P(x));
			return;
		}
		if (this->S(x)->color == Color::BLACK && this->P(x)->color == Color::RED && this->S(x)->left->color == Color::BLACK && this->S(x)->right->color == Color::BLACK) {
			this->S(x)->color = Color::RED;
			this->P(x)->color = Color::BLACK;
			return;
		}
		if (x == this->P(x)->left) {
			if (this->S(x)->left->color == Color::RED && this->S(x)->right->color == Color::BLACK) { 
				this->S(x)->color = Color::RED;
				this->S(x)->left->color == Color::BLACK;
				this->smallRightRotate(this->S(x)); 
			}
			this->S(x)->color = this->P(x)->color; 
			this->P(x)->color = BLACK;
			this->S(x)->right->color = BLACK;
			this->smallLeftRotate(this->P(x));
			return;
		}
		else if (x == this->P(x)->right) {
			if (this->S(x)->right->color == Color::RED && this->S(x)->left->color == Color::BLACK) {
				this->S(x)->color = Color::RED;
				this->S(x)->right->color == Color::BLACK;
				this->smallLeftRotate(this->S(x));
			}
			this->S(x)->color = this->P(x)->color;
			this->P(x)->color = BLACK;
			this->S(x)->left->color = BLACK;
			this->smallRightRotate(this->P(x));
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
	iterator erase(const TKey& key) {

		Node* curr = *(this->find(key));
		if (!curr) throw "you are trying to erase a non-existant element";

		Node* y, x;//y - вершина, с которой свапнем удаляемую; x - потенциальный правый ребенок y
		if (!curr->left || !curr->right) y = curr; //если y - лист или у нее только 1 ребенок, то ее же и будем удалять
		else y = *(++(iterator(curr))); //в ином случае находим вершину, на которую будем менять

		if (y->left) x = y->left; //в случае, если единственный лист - левый, то его и будем подтягивать
		else x = y->right; //в ином случае соответственно подтягиваем правый
		x->parent = y->parent; //прикрепляем х к новому родителю

		if (y->parent) { //если не корень, то родителю тоже сообщаем о новых детях
			if (y == y->parent->left) x = y->parent->left;
			else x = y->parent->right;
		}
		else this->root = x;

		if (y != curr) swap(curr, y); //меняем значения удаляемой вершины и той, с которой можем безобидно поменять

		Color yColor = y->color;
		delete y;
		if (yColor == Color::BLACK) balanceDelete(x);
		return iterator(curr);

	}


	TValue& operator[](const TKey& key);
	TValue operator[](const TKey& key) const;

	friend std::ostream& operator <<(std::ostream& ostr, RBTree& tree);
};