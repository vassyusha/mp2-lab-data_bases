#include <gtest.h>
#include <hash_table2.h>
#include <utility>

#define count 100

template <class T>
class hastTable2Fixture :public ::testing::Test {
public:
	using Tkey = typename T::first_type;
	using Tvalue = typename T::second_type;
	hastTable2<Tkey, Tvalue> table= hastTable2<Tkey, Tvalue>(count);
protected:
	int COUNT = 20;
	void SetUp() override {
		for (int i = 0; i < COUNT; i++) table.insert(static_cast<Tkey>(i), static_cast<Tvalue>(i * 3));
	}
};

using hastTable2FixtureStrings = hastTable2Fixture<std::pair<std::string, int>>;

using MyTypes = ::testing::Types< std::pair<int, double>>;
TYPED_TEST_CASE(hastTable2Fixture, MyTypes);

TEST(hastTable2, can_insert_one_element)
{
	int size = 20;
	hastTable2<int, int> T(size);

	T.insert(5, 4);

	EXPECT_EQ(1,T.size());
}
TEST(hastTable2, insertion_return_right_iterator)
{
	int size = 10;
	hastTable2<int, int> T(size);
	T.insert(5, 4);

	EXPECT_EQ(1, (*T.find(5)).key==5);
}
TEST(hastTable2, hash_table_can_do_repacking)
{
	int size = 10;
	hastTable2<int, int> T(size);
	bool flag = 1;
	for (int i = 0; i <size; i++) T.insert(i, 3); 
	for (int i = 0; i <size; i++) { //может найти любой элемент в переупаковонной хэш-таблице
		if ((*T.find(i)).key != i) {
			flag = 0;
			break;
		}
	}
	EXPECT_EQ(1, flag);
}
TEST(hastTable2, hash_table_can_insert_with_collisions)
{
	int size = 10;
	hastTable2<int, int> T(size);

	for (int i = 0; i < 5; i++) T.insert(i, 3);
	T.insert(10, 3);

	EXPECT_EQ(T.getCollisionsNumber(), 2);
}
TEST(hastTable2, hash_table_can_insert_with_a_lot_of_collisions)
{
	int size = 10;
	hastTable2<int, int> T(size);
	for (int i = 0; i < 8; i++) T.insert(i, 3);
	T.insert(11, 3);

	EXPECT_EQ(T.getCollisionsNumber(), 7);
}
TEST(hastTable2, hash_table_can_insert_some_elements_with_repacking)
{
	int size = 10;
	hastTable2<int, int> T(size);
	for (int i = 0; i < 9; i++) T.insert(i, 3);
	for (int i = 9; i < 40; i++) T.insert(i, 3);

	EXPECT_EQ(T.getCollisionsNumber(), 0);
}
TEST(hastTable2, hash_table_can_use_operator_square_brackets)
{
	int size = 10;
	hastTable2<int, int> T(size);
	for (int i = 0; i < 9; i++) T.insert(i, 3);
	T[2] = 5;
	EXPECT_EQ(T[2], 5);
}
TEST(hastTable2, operator_square_brackets_thow_exception_if_we_try_to_find_a_not_existing_key)
{
	int size = 10;
	hastTable2<int, int> T(size);
	for (int i = 0; i < 9; i++) T.insert(i, 3);
	ASSERT_ANY_THROW(T[20]);
}
TEST(hastTable2, hash_table_stressed_test)
{
	hastTable2<int, int> T(10);
	for (int i = 0; i < 1000; i++) T.insert(i, 3);
	EXPECT_EQ(T.getCollisionsNumber(), 0);
}
TEST(hastTable2, hash_table_can_insert_an_element_with_string_key)
{
	int size = 10;
	hastTable2<std::string, int> T(size);
	ASSERT_NO_THROW(T.insert("something", 3));
}
TEST(hastTable2, hash_table_can_insert_some_elements_with_string_key)
{
	int size = 10;
	hastTable2<std::string, int> T(size);
	std::string s = "1";
	for (int i= 0; i < 8; i++) {
		T.insert(s, 3);
		s += std::to_string(i);
	}
	EXPECT_EQ(8, T.size());
}
TEST(hastTable2, hash_table_can_insert_some_elements_with_string_key_with_rapacking)
{
	int size = 8;
	hastTable2<std::string, int> T(size);
	std::string s = "1";
	for (int i = 0; i < size; i++) {
		T.insert(s, 3);
		s += std::to_string(i);
	}
	EXPECT_EQ(size, T.size());
}
TEST(hastTable2, hash_table_can_erase_an_element_with_string_key)
{
	int size = 8;
	hastTable2<std::string, int> T(size);
	std::string s = "1";
	for (int i = 0; i < size; i++) {
		T.insert(s, 3);
		s += std::to_string(i);
	}
	ASSERT_NO_THROW(T.erase("101"));
}
TEST(hastTable2, hash_table_can_erase_an_element_with_string_key_and_size_will_be_right)
{
	int size = 8;
	hastTable2<std::string, int> T(size);
	std::string s = "1";
	for (int i = 0; i < size; i++) {
		T.insert(s, 3);
		s += std::to_string(i);
	}
	ASSERT_NO_THROW(T.size(),size-1); 
}
TEST(hastTable2, hash_table_can_erase_some_elements_with_string_key)
{
	int size = 8;
	hastTable2<std::string, int> T(size);
	std::string s = "1";
	for (int i = 0; i < size; i++) {
		T.insert(s, 3);
		s += std::to_string(i);
	}
	T.erase("101");
	T.erase("1012");
	T.erase("1012345");
	T.erase("1");
	EXPECT_EQ(T.size(), size - 4);
}
TYPED_TEST(hastTable2Fixture, can_get_the_right_size) {
	EXPECT_EQ(COUNT, table.size());
}
TYPED_TEST(hastTable2Fixture, find_return_iterator_to_the_end_if_we_try_to_find_not_existing_key) {
	EXPECT_EQ(table.end(), table.find(static_cast<Tkey>(34)));
}
TYPED_TEST(hastTable2Fixture, find_return_iterator_to_the_right_element) {
	EXPECT_EQ(9, (*table.find(static_cast<Tkey>(3))).value);
}
TYPED_TEST(hastTable2Fixture, can_erase_all_elements) {
	for (int i = 0; i < COUNT; i++) {
		ASSERT_NO_THROW(table.erase(static_cast<Tkey>(i)));
	}
	EXPECT_EQ(0, table.size());
}
TYPED_TEST(hastTable2Fixture, can_erase_some_elements) {
	for (int i = 0; i < COUNT/2; i++) {
		ASSERT_NO_THROW(table.erase(static_cast<Tkey>(i)));
	}
	EXPECT_EQ(COUNT/2, table.size());
}
TYPED_TEST(hastTable2Fixture, can_erase_some_elements_and_insert_them_back) {
	for (int i = COUNT / 2; i < COUNT; i++) table.erase(static_cast<Tkey>(i));
	for (int i = COUNT / 2; i < COUNT; i++) table.insert(static_cast<Tkey>(i), static_cast<Tvalue>(i * 3));
	EXPECT_EQ(COUNT,table.size());
}
TYPED_TEST(hastTable2Fixture, can_erase_some_elements_and_insert_some_element_after_that_with_collisions) {
	for (int i = COUNT / 2; i < COUNT; i++) table.erase(static_cast<Tkey>(i));
	for (int i = COUNT * 2; i < COUNT * 6; i++) table.insert(static_cast<Tkey>(i), static_cast<Tvalue>(i * 3));
	EXPECT_EQ((table.size()==90), 1);
}
TYPED_TEST(hastTable2Fixture, can_erase_all_elements_on_another_order) {
	for (int i = COUNT-1; i>=0; i--) {
		ASSERT_NO_THROW(table.erase(static_cast<Tkey>(i)));
	}
	EXPECT_EQ(0, table.size());
}
TYPED_TEST(hastTable2Fixture, can_not_erase_not_existing_key) {
	ASSERT_ANY_THROW(table.erase(static_cast<Tkey>(22)));
}
TYPED_TEST(hastTable2Fixture, iterator_can_use_operator_postfix_increment) {
	hastTable2<Tkey, Tvalue>::iterator it(table.find(static_cast<Tkey>(0)));
	it++;
	EXPECT_EQ((*it).key, static_cast<Tkey>(1));
}
TYPED_TEST(hastTable2Fixture, iterator_can_use_operator_postfix_increment_in_the_right_way) {
	hastTable2<Tkey, Tvalue>::iterator it(table.find(static_cast<Tkey>(0)));
	ASSERT_NO_THROW(it++);
}
TYPED_TEST(hastTable2Fixture, iterator_can_use_operator_postfix_increment_for_last_element) {
	hastTable2<Tkey,Tvalue>::iterator it(table.find(static_cast<Tkey>(19)));
	ASSERT_NO_THROW(it++);
}
TYPED_TEST(hastTable2Fixture, iterator_can_use_operator_postfix_increment_for_last_element_in_the_right_way) {
	hastTable2<Tkey, Tvalue>::iterator it(table.find(static_cast<Tkey>(19)));
	it++;
	EXPECT_EQ(it, table.end());
}
TYPED_TEST(hastTable2Fixture, iterator_can_use_operator_pref_increment) {
	hastTable2<Tkey, Tvalue>::iterator it(table.find(static_cast<Tkey>(0)));
	EXPECT_EQ((*++it).key, static_cast<Tkey>(1));
}
TYPED_TEST(hastTable2Fixture, iterator_can_bypass_all_the_elements) {
	for (int i = 100; i < 138; i++) { 
		table.insert(static_cast<Tkey>(i), static_cast<Tvalue>(i) * 4); }
	table.insert(static_cast<Tkey>(138), static_cast<Tvalue>(138) * 4);
	for (hastTable2<Tkey,Tvalue>::iterator it = table.begin(); it < table.end(); it++) {
		ASSERT_NO_THROW((*it));
	}
}