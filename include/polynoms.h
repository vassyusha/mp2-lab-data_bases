#pragma once
#include <vector>
#include "cyclicList.h"

class Polynom;

class Monom {
public:
	double coef;
	int x;
	int y;
	int z;
	Monom(double coef_ = 0, std::vector<int> powers = { 0,0,0 }) {
		coef = coef_;
		x = powers[0];
		y = powers[1];
		z = powers[2];
		if (coef == 0) {
			x = 0;
			y = 0;
			z = 0;
		}
	}
	Monom(const Monom& other) {
		coef = other.coef;
		x = other.x;
		y = other.y;
		z = other.z;
		if (coef == 0) {
			x = 0;
			y = 0;
			z = 0;
		}
	}
	bool operator==(const Monom& other) const {
		return ((this->x == other.x) && (this->y == other.y) && (this->z == other.z));
	}
	bool operator!=(const Monom& other) {
		return !(*this == other);
	}
	bool operator>(const Monom& other) {
		if (this->x > other.x) return 1;
		else {
			if (this->x == other.x) {
				if (this->y > other.y) return 1;
				else {
					if (this->y == other.y) {
						if (this->z > other.z) return 1;
					}
				}
			}
		}
		return 0;
	}
	bool operator<(const Monom& other) {
		return ((*this != other) && !(*this > other));
	}
	std::vector<int> getPowers() {
		return { x,y,z };
	}
	Monom& operator=(const Monom& other) {
		coef = other.coef;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	Monom& operator+=(const Monom& other) {
		coef += other.coef;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	Monom operator-() const{
		Monom m(*this);
		m.coef = -m.coef;
		return m;
	}
	Monom & operator*=(double c) {
		coef *= c;
		return *this;
	}
	Monom& operator*=(const Monom& other) {
		coef *= other.coef;
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Monom operator*(const Monom& other) {
		Monom M(*this);
		M *= other;
		return M;
	}
	void print() {
		std::cout << coef << "x^" << x << "y^" << y << "z^" << z << " ";
	}

};
class Polynom {
	cyclicList<Monom> data;
public:
	Polynom(std::vector<Monom> monoms = std::vector<Monom>()) { //пользователь может передавать неотсортированный полином
		for (int i = 0; i < monoms.size(); i++) {
			data.insert(monoms[i]);
		}
	}
	Polynom(const Polynom& other) {
		data = other.data;
	}
	bool operator!=(const Polynom& other) {
		return (this->data != other.data);
	}
	Polynom& operator=(const Polynom& other) {
		if (*this != other) {
			data = other.data;
		}
		return *this;
	}
	bool operator==(const Polynom & other) const{
		return (this->data == other.data);
	}
	Polynom& operator+=(const Monom& m) {
		cyclicList<Monom>::Node* curr1 = this->data.HEAD();
		if (curr1 != nullptr) {
			bool flag = 0;
			if (m == curr1->value) {
				curr1->value += m;
				flag = 1;
			}
			curr1 = curr1->next;
			while (curr1 != this->data.HEAD()) {
				if (m == curr1->value) {
					curr1->value += m;
					flag = 1;
				}
				curr1 = curr1->next;
			}
			if (!flag) data.insert(m);
		}
		else data.insert(m);
		return *this;
	}
	Polynom operator+(const Monom& m) {
		Polynom P(*this);
		P += m;
		return P;
	}
	int change(cyclicList<Monom>::Node** curr1, cyclicList<Monom>::Node** curr2, cyclicList<Monom>::Node** curr3, Polynom* P) {
		Monom first = (*curr1)->value, second = (*curr2)->value;
		if (first > second) {
			*curr3 = P->data.insert_after(first, *curr3);
			*curr1 = (*curr1)->next;
			return 0; //сдвинулся первый

		}
		else {
			if ((first == second) && (first.coef + second.coef != 0)) {
				*curr3 = P->data.insert_after(Monom(first.coef + second.coef, first.getPowers()), *curr3);
				*curr1 = (*curr1)->next;
				*curr2 = (*curr2)->next;
				return 1; //сдвинулись оба
			}
			else {
				*curr3 = P->data.insert_after(second, *curr3);
				*curr2 = (*curr2)->next;
				return 2; //сдвинулся второй
			}
		}
	}
	Polynom& operator+=(const Polynom& other) {
		cyclicList<Monom>::Node* curr1 = this->data.HEAD();
		cyclicList<Monom>::Node* curr2 = other.data.HEAD();
		if ((curr1 != nullptr) && (curr2 != nullptr)) {
			Polynom P;
			cyclicList<Monom>::Node* curr3 = P.data.HEAD();
			int flag = change(&curr1, &curr2, &curr3, &P);
			if (flag == 0) while (change(&curr1, &curr2, &curr3, &P) == 0) {}
			if (flag == 2) while (change(&curr1, &curr2, &curr3, &P) == 2) {}
			while (curr1 != this->data.HEAD() && curr2 != other.data.HEAD()) { //пока какой-то не кончился 
				change(&curr1, &curr2, &curr3, &P);
			}
			while (curr1 != this->data.HEAD()) { //оставшийся 
				P.data.insert_after(curr1->value, curr3);
				curr1 = curr1->next;
				curr3 = curr3->next;
			}
			while (curr2 != other.data.HEAD()) {
				P.data.insert_after(curr2->value, curr3);
				curr2 = curr2->next;
				curr3 = curr3->next;
			}
			*this = P;
			return *this;
		}
		if ((curr1 == nullptr) && (curr2 != nullptr)) {
			*this = other;
			return *this;
		}
		return *this;
	}
	Polynom operator+(const Polynom& other)const {
		Polynom P(*this);
		P += other;
		return P;
	}
	Polynom operator-() const{ //унарный минус к полиному
		Polynom P(*this);
		P *= (-1);
		return P;
	}
	Polynom operator-(const Polynom& other) const{
		Polynom P(*this);
		Polynom tmp(other);
		P += -tmp;
		return P;
	}
	Polynom& operator*=(double c) {
		cyclicList<Monom>::Node* curr = this->data.HEAD();
		curr->value *= c;
		curr = curr->next;
		while (curr != data.HEAD()) {
			curr->value *= c;
			curr = curr->next;
		}
		return *this;
	}
	Polynom& operator*=(const Monom& M) {
		cyclicList<Monom>::Node* curr = this->data.HEAD();
		curr->value *= M;
		curr = curr->next;
		while (curr != data.HEAD()) {
			curr->value *= M;
			curr = curr->next;
		}
		//print();
		return *this;
	}
	Polynom operator*(const Monom& M) const {
		Polynom P(*this);
		P *= M;
		return P;
	}
	Polynom& operator*=(const Polynom& other) {
		if (this->data.HEAD() != nullptr && other.data.HEAD() != nullptr) {
			Polynom P;
			cyclicList<Monom>::Node* curr1 = this->data.HEAD();

			Monom m1(curr1->value);
			P += other * m1;
			curr1 = curr1->next;
			while (curr1 != this->data.HEAD()) {
				Monom m1(curr1->value);
				P += other * m1;

				curr1 = curr1->next;
			}
			*this = P;
		}
		return *this;
	}
	Polynom operator*(const Polynom& other) const {
		Polynom P(*this);
		P *= other;
		return P;
	}
	
	void print() const {
		data.print();
	}
};
