#include <gtest.h>
#include <string>
#include "hashTable.h"

TEST(hashTable, canJustExist) {
	hashTable<int, double> map;
}

TEST(hashTable, canInsertAnElement) {
	hashTable<int, double> map;
	ASSERT_NO_THROW(map.insert(1, 2.0));
}

TEST(hashTable, canInsertSeveralElements) {
	hashTable<int, double> map;
	map.insert(1, 2.0);
	ASSERT_NO_THROW(map.insert(2, 4.0));
}

TEST(hashTable, throwsForInsertionOfTheAlreadyExistantElement) {
	hashTable<int, double> map;
	size_t st_cap = map.getCapacity();
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	ASSERT_ANY_THROW(map.insert(3,3));
}

TEST(hashTable, resizesToAvoidCollisions) {
	hashTable<int, double> map;
	size_t st_cap = map.getCapacity();
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	EXPECT_LT(st_cap, map.getCapacity());
}

TEST(hashTable, canFindAnExistantElement) {
	hashTable<int, double> map;
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	ASSERT_NO_THROW(map.find(3));
}

TEST(hashTable, canCorrectlyFindAnExistantElement) {
	hashTable<int, double> map;
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	EXPECT_EQ(3.0, (*map.find(3)).second);
}

TEST(hashTable, findingANonExistantElementReturnsIteratorToTheEnd) {
	hashTable<int, double> map;
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	EXPECT_EQ(map.end(), map.find(6));
}

TEST(hashTable, canEraseAnExistantElement) {
	hashTable<int, double> map;
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	ASSERT_NO_THROW(map.erase(3));
}

TEST(hashTable, erasureReturnsIteratorToTheNextElement) {
	hashTable<int, double> map;
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}
	auto it = map.find(3);
	it++;
	EXPECT_EQ(it, map.erase(3));
}

TEST(hashTable, throwsForErasureOfANonExistantElement) {
	hashTable<int, double> map;
	for (int i = 0; i < 5; i++) {
		map.insert(i, i);
	}

	ASSERT_ANY_THROW(map.erase(6));
}

TEST(hashTable, canReturnValueOfAnExistingElement) {
	hashTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_NO_THROW(a[2]);
}

TEST(hashTable, canReturnCorrectValueOfAnExistingElement) {
	hashTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	double exp = 2.0;

	EXPECT_EQ(exp, a[2]);
}

TEST(hashTable, canReturnSmthIfTheKeyDoesntExist) {
	hashTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_NO_THROW(a[5]);
}

TEST(hashTable, insertsTheElementWithDefaultValueIfSuchKeyDoesntExist) {
	hashTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	double p = a[5];

	EXPECT_EQ(double(), p);
}

TEST(hashTable, someTestsForStringKey) {
	hashTable<std::string, double> map;
	std::vector<std::string> keys = { "1st", "2nd", "3hd", "4th", "5th" };

	for (int i = 0; i < 5; i++) {
		ASSERT_NO_THROW(map.insert(keys[i], i));
	}
	
}