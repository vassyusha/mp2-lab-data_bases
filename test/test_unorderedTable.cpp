#include <gtest.h>
#include <string>
#include "unorderedTable.h"

TEST(UnorderedTable, canInsertAnElementIntoAnEmptyTable) {
	unorderedTable<std::string, double> a;
	ASSERT_NO_THROW(a.insert("zero", 0.0));
}

TEST(UnorderedTable, cantInsertAnElementWithAlreadyExistedKey) {
	unorderedTable<std::string, double> a;
	a.insert("zero", 0.0);
	ASSERT_ANY_THROW(a.insert("zero", 1.0));
}

TEST(UnorderedTable, canInsertAnElementIntoANotEmptyTable) {
	unorderedTable<std::string, double> a;
	a.insert("zero", 0.0);
	ASSERT_NO_THROW(a.insert("first", 1.0));
}

TEST(UnorderedTable, anElementInsertsInTheEnd) {
	unorderedTable<std::string, double> a;
	a.insert("zero", 0.0);
	unorderedTable<std::string, double>::iterator it = a.insert("first", 1.0);
	EXPECT_EQ(it+1, a.end());
}


TEST(UnorderedTable, canFindAnExistingElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_NO_THROW(a.find(2));
}

TEST(UnorderedTable, canCorrectlyFindAnExistingElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);
	std::pair<int, double> exp(2, 2.0);

	EXPECT_EQ(*(a.find(2)), exp);
}

TEST(UnorderedTable, returnsIteratorToTheEndWhenAnElementDoesntExist) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	EXPECT_EQ(a.find(5), a.end());
}

TEST(UnorderedTable, canCorrectlyFindTheFirstElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);
	std::pair<int, double> exp(0, 0.0);

	EXPECT_EQ(*(a.find(0)), exp);
}

TEST(UnorderedTable, canCorrectlyFindTheLastElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);
	std::pair<int, double> exp(3, 3.0);

	EXPECT_EQ(*(a.find(3)), exp);
}


TEST(UnorderedTable, canEraseAnElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_NO_THROW(a.erase(2));
}

TEST(UnorderedTable, canEraseTheOnlyOneElement) {
	unorderedTable<int, double> a;
	a.insert(1, 2.0);

	ASSERT_NO_THROW(a.erase(1));
}

TEST(UnorderedTable, cantEraseNonExistantElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_ANY_THROW(a.erase(5));
}

TEST(UnorderedTable, returnsIteratorToTheNextElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	std::pair<int, double> exp = *(a.find(2) + 1);
	

	EXPECT_EQ(exp, *(a.erase(2)));
}

TEST(UnorderedTable, erasureOfTheLastElementReturnsIteratorToTheEnd) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	EXPECT_EQ(a.end(), a.erase(3));
}


TEST(UnorderedTable, canReturnValueOfAnExistingElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_NO_THROW(a[2]);
}

TEST(UnorderedTable, canReturnCorrectValueOfAnExistingElement) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	double exp = 2.0;

	EXPECT_EQ(exp, a[2]);
}

TEST(UnorderedTable, canReturnSmthIfTheKeyDoesntExist) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	ASSERT_NO_THROW(a[5]);
}

TEST(UnorderedTable, insertsTheElementWithDefaultValueIfSuchKeyDoesntExist) {
	unorderedTable<int, double> a;
	size_t n = 4;
	for (int i = 0; i < n; i++) a.insert(i, i);

	double p = a[5];

	EXPECT_EQ(double(), p);
}