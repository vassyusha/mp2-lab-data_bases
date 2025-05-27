#pragma once
#include <vector>
#include <string>

template<class TKey, class TValue>
class unorderedTable {
public:
	class iterator;
private:
	std::vector<std::pair<TKey, TValue>> data;

	void swap(iterator first, iterator second) {
		std::pair<TKey, TValue> temp = *(first);
		*(first) = *(second);
		*(second) = temp;
	}

public:

	size_t size() { return this->data.size(); }

	class iterator {
	private:
		std::pair<TKey, TValue>* it;

	public:
		iterator(std::pair<TKey, TValue>* it): it(it){}

		std::pair<TKey, TValue>& operator*() { return *(this->it); }
		std::pair<TKey, TValue> operator*() const { return *(this->it); }

		iterator& operator++() {
			this->it++;
			return *this;
		}
		iterator operator++(int) {
			iterator i = *this;
			++(*this);
			return i;
		}

		bool operator!=(const iterator& other) const { return (this->it != other.it); }
		bool operator==(const iterator& other) const { return !((*this) != other); }
		bool operator>(const iterator& other) const { return (this->it > other.it); }
		bool operator<(const iterator& other) const { return (this->it < other.it); }
		bool operator>=(const iterator& other) const { return !((*this) < other); }
		bool operator<=(const iterator& other) const { return !((*this) > other); }

		iterator& operator--() {
			this->it--;
			return *this;
		}
		iterator operator--(int) {
			iterator i = *this;
			--(*this);
			return i;
		}

		iterator& operator+=(int n) {
			this->it += n;
			return *this;
		}
		iterator operator+(int n) const {
			iterator i(this->it + n);
			return i;
		}

		iterator& operator-=(int n) {
			this->it -= n;
			return *this;
		}
		iterator operator-(int n) const {
			iterator i(this->it - n);
			return i;
		}

	};

	iterator begin() { return iterator(this->data.data()); }
	iterator end() { return this->begin() + this->size(); }

	iterator insert(const TKey& key, const TValue& value) {
		if (this->find(key) != end()) throw "the element with this key already exists";
		this->data.push_back(std::pair<TKey, TValue>(key, value));
		return this->end()-1;
	}

	iterator find(const TKey& key) {
		for (iterator it = this->begin(); it != this->end(); ++it) {
			if ((*it).first == key) return it;
		}
		return this->end();
	}

	iterator erase(const iterator& it) {
		if (it >= this->end()) throw "you are trying to erase a non-existant element";
		swap(it, this->end() - 1);
		this->data.pop_back();
		return it;
	}
	iterator erase(const TKey& key) {
		iterator it = this->find(key);
		return this->erase(it);
	}

	TValue& operator[](const TKey& key) {
		if (this->find(key) == this->end()) {
			this->data.push_back(std::pair<TKey, TValue>(key, TValue()));
			return (*(this->end() - 1)).second;
		}
		return (*(this->find(key))).second; 
	}
	TValue operator[](const TKey& key) const {
		if (this->find(key) == this->end()) throw "an element with such key doesnt exist";
		return (*(this->find(key))).second;
	}

	friend std::ostream& operator <<(std::ostream& ostr, unorderedTable& table) {
		ostr << "|   KEY    |    VALUE   |\n";
		for (iterator it = table.begin(); it != table.end(); ++it) ostr << "|       " << (*it).first << "       |       " << (*it).second << "      |\n";
		return ostr;
	}
};