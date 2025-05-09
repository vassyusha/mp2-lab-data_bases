#pragma once
#include <vector>
#include <string>
#include <random>


template<class TKey, class TValue>
class hashTable {
public:
	class iterator;
private:
	std::vector<std::vector<std::pair<TKey, TValue>>> data;

	size_t capacity = 0;
	size_t size = 0;

	int a = 0, b = 0;
	int mod; 

	size_t hash(const std::string& s) const{
		size_t res = 0;
		for (int i = 0; i < s.size(); i++) {
			res += s[i] * (1 << i);
		}
		res %= this->mod;
		res %= capacity;
		return res;
	}

	size_t hash(size_t num) const {
		return (a * num + b) % mod % capacity;
	}

	void swap(iterator first, iterator second) {
		std::pair<TKey, TValue> temp = *(first);
		*(first) = *(second);
		*(second) = temp;
	}

	void resize() {
		hashTable<TKey, TValue> tmp(pow(this->size, 2));
		for (auto it = this->begin(); it != this->end(); it++) {
			tmp.insert((*it).first, (*it).second);
		}
		*this = tmp;
	}

public:

	//size_t size() { return this->data.size(); }
	hashTable(size_t cap = 7) {
		this->capacity = cap;
		this->data = std::vector<std::vector<std::pair<TKey, TValue>>>(capacity);

		std::random_device rd;  
		std::mt19937 gen(rd()); 
		std::uniform_int_distribution<> dist(1, this->capacity*2);

		this->a = dist(gen);
		this->b = dist(gen);
		this->mod = pow(capacity, 2) * 2 - 1;
	}

	class iterator {
	private:
		hashTable* owner;
		size_t cell_pos; //по какому индексу по хэшу хранится
		size_t num_pos; //какой по номеру среди совпавших
		

	public:
		iterator(hashTable* owner, size_t pos, size_t num): owner(owner), cell_pos(pos), num_pos(num){}

		std::pair<TKey, TValue>& operator*() { 
			if ((*this) == this->owner->end()) throw "you cant get the end element";
			return this->owner->data[cell_pos][num_pos]; 
		}
		std::pair<TKey, TValue> operator*() const { 
			if ((*this) == this->owner->end()) throw "you cant get the end element";
			return this->owner->data[cell_pos][num_pos]; 
		}

		iterator& operator++() {
			if (this->num_pos < this->owner->data[this->cell_pos].size() - 1) this->num_pos++;
			else {
				this->cell_pos++;
				while (this->cell_pos < this->owner->capacity && this->owner->data[this->cell_pos].size() == 0) this->cell_pos++;
				/*if (this->owner->data[cell_pos].size() == 0) */this->num_pos = 0;
				/*else return this->owner->end();*/
			}
			return (*this);
		}
		iterator operator++(int) {
			iterator i = *this;
			++(*this);
			return i;
		}

		bool operator!=(const iterator& other) const { return !((*this) == other); }
		bool operator==(const iterator& other) const { return (this->owner == other.owner && this->cell_pos == other.cell_pos && this->num_pos == other.num_pos); }
		bool operator>(const iterator& other) const { 
			if (this->owner != other.owner) throw "you cant compare iterators of different tables";
			if (this->cell_pos > other.cell_pos) return true;
			if (this->cell_pos == other.cell_pos && this->num_pos > other.num_pos) return true;
			return false;
		}
		bool operator<(const iterator& other) const { 
			if (this->owner != other.owner) throw "you cant compare iterators of different tables";
			if (this->cell_pos < other.cell_pos) return true;
			if (this->cell_pos == other.cell_pos && this->num_pos < other.num_pos) return true;
			return false;
		}
		bool operator>=(const iterator& other) const { return !((*this) < other); }
		bool operator<=(const iterator& other) const { return !((*this) > other); }

		iterator& operator--() {
			if (this->num_pos > 0) this->num_pos--;
			while (this->cell_pos >= 0 && this->owner->data[cell_pos].size() == 0) this->cell_pos--;
			if (this->owner->data[cell_pos].size() != 0) this->num_pos = this->owner->data[cell_pos].size()-1;
			else return this->owner->begin();
			return *this;
		}
		iterator operator--(int) {
			iterator i = *this;
			--(*this);
			return i;
		}

		size_t getCellPos() const { return this->cell_pos; }
		size_t getNumPos() const { return this->num_pos; }
	};

	iterator begin() {
		if (this->empty()) return this->end();
		int cell_pos = 0;
		while (this->data[cell_pos].size() == 0) cell_pos++;
		return iterator(this, cell_pos, 0);
	}
	iterator end() {
		return iterator(this, this->capacity, 0);
	}

	iterator insert(const TKey& key, const TValue& value) {
		if (this->find(key) != this->end()) throw "the element with this key already exists";

		if ((double)(this->size + 1) / this->capacity > 0.6) this->resize();

		this->size++;
		size_t h = this->hash(key);
		//std::cout << h << "\n";
		this->data[h].push_back(std::pair<TKey, TValue>(key, value));
		return iterator(this, h, this->data[h].size() - 1);
	}

	iterator find(const TKey& key) {
		size_t h = this->hash(key);
		if (this->data[h].size() != 0) {
			size_t i = 0;
			while (i != data[h].size() && data[h][i].first != key) i++;
			if (i != data[h].size() && data[h][i].first == key) return iterator(this, h, i);
		}
		return this->end();
	}

	iterator erase(const TKey& key) {
		iterator it = this->find(key);
		if (it == this->end()) throw "you are trying to erase a non-existant element";

		size_t cell_pos = it.getCellPos(), num_pos = it.getNumPos();
		this->swap(it, iterator(this, cell_pos, this->data[cell_pos].size() - 1));
		this->size--;
		it++;
		this->data[cell_pos].pop_back();
		return it;
	}

	TValue& operator[](const TKey& key){
		if (this->find(key) == this->end()) {
			return (*this->insert(key, TValue())).second;
		}
		return (*(this->find(key))).second; 
	}
	TValue operator[](const TKey& key) const{
		if (this->find(key) == this->end()) throw "an element with such key doesnt exist";
		return (*(this->find(key))).second;
	}

	bool empty() { return (this->size == 0); }
	size_t getCapacity() const { return this->capacity; }

	friend std::ostream& operator <<(std::ostream& ostr, hashTable& table) {
		ostr << "|   KEY    |    VALUE   |\n";
		for (iterator it = table.begin(); it != table.end(); ++it) ostr << "|       " << (*it).first << "       |       " << (*it).second << "      |\n";
		return ostr;
	}

	void print() {
		for (int i = 0; i < this->capacity; i++) {
			std::cout << "|";
			for(int j = 0; j < this->data[i].size(); j++) std::cout << "|	" << data[i][j].first << "	" << data[i][j].second << "	|";
			std::cout << "|\n";
		}
	}

};