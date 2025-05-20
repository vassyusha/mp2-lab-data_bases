#pragma once
#include <iostream>
#include <vector>
#include <string>

enum class Status { Free, Busy, Deleted };

int gcd(int a, int b) {
	if (a == 0) return b;
	if (b == 0) return a;
	if (a == b) return a;
	if (a > b) return gcd(a - b, b);
	return gcd(a, b - a);
}
bool isMutuallySimple(int x, int y) {
	if (gcd(x, y) == 1) return 1;
	else return 0;
}
int findSimpleNumber(int a) {
	for (int i = 2; i < 2 * a; i++) {
		if (isMutuallySimple(i, a)) return i;
	}
}
template<class Tkey, class Tvalue>
class hastTable2 {
public:class iterator;
private:
	class function;
	struct Record;

	std::vector<Record> data;
	int capacity;
	int count;
	function h;
	int collisionsNumber = 0;
	struct Record {
		Status state; 
		Tkey key;
		Tvalue value;
		Record(){ state = Status::Free; }
		Record(Status st) { state = st; }
		Record(const Tkey& k, const Tvalue& v) : key(k), value(v) {
			state = Status::Free;
		}
	};
	class function {
		int a, b, count;
	public:
		function(int a_=0, int b_=0, int count_=0) {
			a = a_;
			b = b_;
			count = count_;
		}
		int operator()(int k, int i){
			int h1 = (a * k + b) % count;
			if(i==0)  return h1;
			int h2 = 1 + k % a;
			return (h1+h2*i) % count;
		}
		int operator()(const std::string & key_, int flag)  {
			    const std::string& key = key_.c_str();
				int len = key.size();
				const unsigned int m = 0x5bd1e995;
				const unsigned int seed = 0;
				const int r = 24;

				unsigned int h = seed ^ static_cast<unsigned int>(len);

				const unsigned char * data = (const unsigned char*)key.c_str();
				unsigned int k = 0;
				while (len >= 4)
				{
					k = data[0];
					k |= data[1] << 8;
					k |= data[2] << 16;
					k |= data[3] << 24;

					k *= m;
					k ^= k >> r;
					k *= m;

					h *= m;
					h ^= k;

					data += 4;
					len -= 4;
				}
				switch (len)
				{
				case 3: h ^= data[2] << 16;
				case 2: h ^= data[1] << 8;
				case 1:
					h ^= data[0];
					h *= m;
				};
				h ^= h >> 13;
				h *= m;
				h ^= h >> 15;
				if(flag==0) return h%count;
				return (h + flag) % count;
		}
	};
	Record * insert(const Tkey& k, const Tvalue& v, int index, int num) {
		if((this->data[index].state==Status::Busy)&&(this->data[index].key==k)) throw "Try to insert an existing key\n";
		if ((this->data[index].state == Status::Deleted) && (num == -1)) num = index;
		if ((this->data[index].state == Status::Free) && (num == -1)) 
		{
			count++;
			this->data[index].state = Status::Busy;
			this->data[index].key = k;
			this->data[index].value = v;
			return &data[index];
		}
		if ((this->data[index].state == Status::Free) && (num > -1)) {
			count++;
			this->data[num].state = Status::Busy;
			this->data[num].key = k;
			this->data[num].value = v;
			return &data[num];
		}
		collisionsNumber++;
		int hesh = h(k, collisionsNumber);
		return this->insert(k, v, hesh, num);
	}
	Record * find(const Tkey& k, int index){
		if ((this->data[index].state == Status::Busy) && (this->data[index].key==k)) {
			return &data[index];
		}
		if (this->data[index].state == Status::Free) {
			return &data[index]; //долшли до свободной и не нашли нужный 
		}
		collisionsNumber++;
		int hesh = h(k, collisionsNumber);
		return this->find(k,hesh);
	}
	Record* erase(const Tkey& k, int index) {
		if ((this->data[index].state == Status::Busy) && (this->data[index].key == k)) {
			this->data[index].state = Status::Deleted;
			count--;
			if (index + 1 < capacity) return &data[index + 1];
			return &data[0];
		}
		if (this->data[index].state == Status::Free) {
			throw "Element with this key doesn't exist\n";
		}
		collisionsNumber++;
		int hesh = h(k, collisionsNumber);
		return this->erase(k,hesh);

	}
	friend std::ostream& operator <<(std::ostream& ostr, hastTable2& table) {
		ostr << "|   KEY    |    VALUE   |\n";
		for (iterator it = table.begin(); it < table.end(); it++) 
			if((*it).state == Status::Busy) ostr << "|       " << (*it).key << "       |       " << (*it).value << "      |\n";
		return ostr;
	}
	void repacking() {
		hastTable2<Tkey, Tvalue> tmp(*this);
		capacity = capacity * capacity;
		data = std::vector<Record>(capacity);
		count = 0;
		this->h= function(findSimpleNumber(capacity), 4, capacity);
		for (hastTable2<Tkey, Tvalue>::iterator it = tmp.begin(); it < tmp.end(); it++) {
			this->insert((*it).key, (*it).value);
		}
	}
public:
	int getCollisionsNumber() {
		return collisionsNumber;
	}
	hastTable2(const int &  m) : capacity(m) {
		count = 0;
		h = function(findSimpleNumber(capacity), 4, capacity);
		data = std::vector<Record>(capacity);
	}
	iterator insert(const Tkey & k, const Tvalue & v) {
		collisionsNumber = 0;
		if (count > 0.8 * capacity) {
			repacking();
		}
		return iterator(this,this->insert(k, v, h(k,0), -1));
	}
	iterator erase(const Tkey& k) {
		collisionsNumber = 0;
		return iterator(this,this->erase(k,h(k, 0)));
	}
	iterator find(const Tkey& k){
		collisionsNumber = 0;
		if ((*(this->find(k, h(k, 0)))).state != Status::Free) return iterator(this,this->find(k,h(k, 0)));
		return this->end();
	}
	Tvalue& operator[](const Tkey& key) {
		if (this->find(key) != this->end()) return (*this->find(key)).value;
		else throw "Trying to use a not existing key";
	}
	Tvalue operator[](const Tkey& key) const{
		if (this->find(key) != this->end()) return (*this->find(key)).value;
		else throw "Trying to use a not existing key";
	}
	iterator begin() {
		int i = 0;
		while ((i< capacity) && (this->data[i].state == Status::Free)) i++;
		if (i<capacity) return iterator(this,&data[i]);
		return iterator(this, this->data.data());
	}
	iterator end() {
		iterator it(this->begin());
		iterator result(it); 
		while (it<=iterator(this,&data[capacity - 1])) {
			if ((*it).state == Status::Busy) result = it;
			it++;
		}
		return ++result;
	}
	int size() {
		return count; 
	}
	int getCapacity() { return capacity; }
	class iterator {
		Record * it;
		hastTable2* table; 
	public:
		iterator(hastTable2 * table_, Record * data) {
			table = table_;
			it = data;
		}
		iterator operator++(int) {
			Record* tmp = it;
			while (it <= &(table->data[table->capacity - 1])) {
				if ((*it).state == Status::Busy && (*it).key != (*tmp).key) return iterator(table,tmp);
				it++;
			}
			return iterator(table,tmp);
		}
		iterator operator++() {
			Record * tmp = it;
			while (it <= &(table->data[table->capacity - 1])) {
				if ((*it).state == Status::Busy && (*it).key != (*tmp).key) break;
				it++;
			}
			return iterator(table, it);
		}
		Record & operator*() {
			return *it;
		}
		Record & operator->() {
			return *it;
		}
		bool operator<(const iterator & other) const{
			return this->it <other.it;
		}
		bool operator<=(const iterator& other) const {
			return this->it <= other.it;
		}
		bool operator==(const iterator& other) const {
			return this->it == other.it;
		}
		bool operator!=(const iterator& other) const {
			return this->it != other.it;
		}
	};
};
