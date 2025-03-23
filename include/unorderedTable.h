#pragma once
#include <vector>
#include <string>

template<class TKey, class TValue>
class unorderedTable {
private:
	std::vector<std::pair<TKey, TValue>> data;

	void swap(const iterator& first, const iterator& second) {
		std::pair<TKey, TValue> temp = *(first);
		*(first) = *(second);
		*(second) = temp;
	}

public:

	size_t size() { return this->data->size(); }

	class iterator {
	private:
		std::pair<TKey, TValue>* it;

	public:
		iterator(std::pair<TKey, TValue>* it): it(it){}

		std::vector<std::pair<TKey, TValue>>& operator*() { return *(this->it); }
		std::vector<std::pair<TKey, TValue>> operator*() const { return *(this->it); }

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
		bool operator>=(const iterator& other) const { return !((*this) < iter); }
		bool operator<=(const iterator& other) const { return !((*this) > iter); }

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
			iterator i(this->it + n, this->arr);
			return i;
		}

		iterator& operator-=(int n) {
			this->it -= n;
			return *this;
		}
		iterator operator-(int n) const {
			iterator i(this->it - n, this->arr);
			return i;
		}

	};

	iterator begin() { return this->data; }
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
		if (it >= this->end()) throw "you are trying to erase an element outside the table";
		swap(it, this->end() - 1);
		this->data.pop_back();
		return it;
	}
	iterator erase(const TKey& key) {
		iterator it = this->find(key);
		if (it != this->end()) return this->erase(it);
		return it;
	}

	TValue& operator[](const TKey& key) { return (*(this->find(key)).second; }
	TValue operator[](const TKey& key) const { return (*(this->find(key)).second; }

	friend std::ostream& operator <<(std::ostream& ostr, unorderedTable& table) {
		ostr << "|   KEY    |    VALUE   |\n";
		for (iterator it = table.begin(); it != table.end(); ++it) ostr << "|       " << (*it).first << "       |       " << (*it).second << "      |\n";
		return ostr;
	}
};