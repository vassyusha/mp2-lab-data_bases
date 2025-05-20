#include <gtest.h>
#include <avl_tree.h>
#include <utility>

#define count 1000
template <class T>
class avlTreeFixture :public ::testing::Test {
public:
	using Tkey = typename T::first_type;
	using Tvalue = typename T::second_type;
	avlTree<Tkey, Tvalue> Tree;
protected:
	int COUNT = 20;
	void SetUp() override {
		for (Tkey i = 0; i < COUNT; i++) Tree.insert(static_cast<Tkey>(i), static_cast<Tvalue>(i * 3));
	}
};

using MyTypes = ::testing::Types<std::pair<int, double>>;
TYPED_TEST_CASE(avlTreeFixture, MyTypes);

TEST(avlTree, can_insert_one_element)
{
	avlTree<int, int> T;

	T.insert(5, 4);

	EXPECT_EQ(5, T.getRoot().first);
}
TEST(avlTree, can_use_a_small_left_rotate)
{
	avlTree<int, int> T;

	T.insert(10, 4);
	T.insert(7, 4);
	T.insert(13, 4);
	T.insert(12, 4);
	T.insert(14, 4);
	T.insert(15, 4);

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_use_a_small_right_rotate)
{
	avlTree<int, int> T;

	T.insert(10, 4);
	T.insert(8, 4);
	T.insert(15, 4);
	T.insert(6, 4);
	T.insert(9, 4);
	T.insert(2, 4);

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_use_a_big_left_rotate)
{
	avlTree<int, int> T;

	T.insert(10, 4);
	T.insert(9, 4);
	T.insert(20, 4);
	T.insert(19, 4);
	T.insert(21, 4);
	T.insert(14, 4);

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_use_a_big_right_rotate)
{
	avlTree<int, int> T;

	T.insert(10, 4);
	T.insert(5, 4);
	T.insert(21, 4);
	T.insert(6, 4);
	T.insert(4, 4);
	T.insert(8, 4);

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_find_min_in_the_tree_with_one_element)
{
	avlTree<int, int> T;

	T.insert(5, 4);

	EXPECT_EQ(5, T.findMin()->key);
}
TEST(avlTree, specific_test)
{
	avlTree<int, int> T;

	T.insert(5, 4);
	T.insert(3, 4);
	T.insert(10, 4);
	T.insert(2, 4);
	T.insert(4, 4);
	T.insert(8, 4);
	T.insert(12, 4);
	T.insert(1, 4);
	T.insert(6, 4);
	T.insert(9, 4);
	T.insert(11, 4);
	T.insert(13, 4);
	T.insert(14, 4);
	T.insert(7, 4);
	T.erase(5);

	EXPECT_EQ(1,T.isBalanced());
}
TEST(avlTree, can_find_miax_in_the_tree_with_one_element)
{
	avlTree<int, int> T;

	T.insert(5, 4);

	EXPECT_EQ(5, T.findMax()->key);
}
TEST(avlTree, can_get_a_right_size_of_tree) 
{
	avlTree<int, int> T;
	size_t size = 5;

	for (int i = 0; i < size; i++) T.insert(i, i * 3);

	EXPECT_EQ(size, T.size());
}
TEST(avlTree, can_insert_million_elements)
{
	avlTree<int,int> T;

	for (int i = 0; i < count; i++) T.insert(i, i * 3);

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_insert_million_elements_in_the_reverse_order)
{
	avlTree<int, int> T;

	for (int i = count; i >= 0; i--)T.insert(i, i * 2);   
	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_erase_million_elements_in_the_order_they_are_added)
{
	avlTree<int, int> T;
	for (int i = 0; i < count; i++) {
		T.insert(i, i * 3);
	}
	for (int i = 0; i < count; i++) {
		T.erase(i);
	}

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, can_erase_million_elements_in_the_reverse_order)
{
	avlTree<int, int> T;
	for (int i = 0; i < count; i++) {
		T.insert(i, i * 3);
	}
	for (int i = count-1; i>=0; i--) {
		T.erase(i);
	}

	EXPECT_EQ(1, T.isBalanced());
}
TEST(avlTree, avlTree_can_use_operator_square_brackets)
{
	int size = 10;
	avlTree<int, int> T;
	for (int i = 0; i < 9; i++) T.insert(i, 3);
	T[5] = 10;
	EXPECT_EQ(10, T[5]);
}
TEST(avlTree, avlTree_method_find_return_iterator_to_the_end_if_we_try_to_find_not_existing_key)
{
	int size = 10;
	avlTree<int, int> T;
	for (int i = 0; i < 9; i++) T.insert(i, 3);
	EXPECT_EQ(T.find(20), T.end());
}
TEST(avlTree, operator_square_brackets_thow_exception_if_we_try_to_find_a_not_existing_key)
{
	int size = 10;
	avlTree<int, int> T;
	for (int i = 0; i < 9; i++) T.insert(i, 3);
	ASSERT_ANY_THROW(T[20]);
}
TEST(avlTree, can_erase_less_elements_than_there_was_added)
{
	avlTree<int, int> T;
	for (int i = 0; i < count; i++) {
		T.insert(i, i * 3);
	}
	for (int i = 0; i < count / 2;i++){
		T.erase(i);
	}
	EXPECT_EQ(1, T.isBalanced());
}

TYPED_TEST(avlTreeFixture, can_get_the_right_size) {
	EXPECT_EQ(COUNT, Tree.size());
}

TYPED_TEST(avlTreeFixture, can_get_the_root) {
	ASSERT_NO_THROW(Tree.getRoot());
}
TYPED_TEST(avlTreeFixture, can_not_get_the_root_in_the_empty_tree) {
	for (int i = 0; i < COUNT; i++) Tree.erase(i);
	ASSERT_ANY_THROW(Tree.getRoot());
}

TYPED_TEST(avlTreeFixture, can_erase_some_elements) {
	for (int i = 0; i < COUNT / 2; i++) ASSERT_NO_THROW(Tree.erase(i));
	EXPECT_EQ(1, Tree.isBalanced());
}
TYPED_TEST(avlTreeFixture, can_erase_some_elements_in_reverse_order) {
	for (int i = COUNT / 2+1; i>=0; i--) ASSERT_NO_THROW(Tree.erase(i));
	EXPECT_EQ(1, Tree.isBalanced());
}
TYPED_TEST(avlTreeFixture, return_the_right_pointer_to_the_begin) {
	EXPECT_EQ(static_cast<Tkey>(0), Tree.begin()->key);
}
TYPED_TEST(avlTreeFixture, can_find_min) {
	EXPECT_EQ(static_cast<Tkey>(0), Tree.findMin()->key);
}
TYPED_TEST(avlTreeFixture, can_find_max) {
	EXPECT_EQ(static_cast<Tkey>(19), Tree.findMax()->key);
}
TYPED_TEST(avlTreeFixture, can_find_next_element) {
	EXPECT_EQ(static_cast<Tkey>(15), Tree.findNext(14)->key);
}
TYPED_TEST(avlTreeFixture, can_find_next_element_for_the_last_element) {
	EXPECT_EQ(Tree.end(), Tree.findNext(19));
}
TYPED_TEST(avlTreeFixture, can_find_next_element_for_the_root) {
	EXPECT_EQ(static_cast<Tkey>(8),Tree.findNext(Tree.getRoot().first)->key);
}
TYPED_TEST(avlTreeFixture, can_find_next_element_for_the_last_element_and_can_not_get_a_key) {
	ASSERT_ANY_THROW(Tree.findNext(19)->key);
}
TYPED_TEST(avlTreeFixture, return_a_pointer_to_the_end_when_we_try_to_find_non_existing_key) {
	EXPECT_EQ(Tree.find(20),Tree.end());
}
TYPED_TEST(avlTreeFixture, erasing__of_last_element_returns_a_pointer_to_the_end) {
	EXPECT_EQ(Tree.erase(19), Tree.end());
}
TYPED_TEST(avlTreeFixture, can_dedicate_an_empty_tree) {
	for (Tkey i = 0; i < static_cast<Tkey>(20); i++) {
		Tree.erase(i);
	}
	EXPECT_EQ(Tree.isEmpty(), 1);
}
TYPED_TEST(avlTreeFixture, can_dedicate_an_not_empty_tree) {
	for (Tkey i = 0; i < static_cast<Tkey>(19); i++) {
		Tree.erase(i);
	}
	EXPECT_EQ(Tree.isEmpty(), 0);
}
TYPED_TEST(avlTreeFixture, can_use_a_finction_of_deletion) {
	Tree.deletion();
	EXPECT_EQ(Tree.isEmpty(), 1);
}
TYPED_TEST(avlTreeFixture, erasing_return_a_pointer_to_the_next_element_for_any_element) { //пробует удалить каждый элемент и проверяет какой итератор возвращает 
	int flag = 1;
	for (Tkey i = 0;i < static_cast<Tkey>(19); i++) {
		if (Tree.erase(i)->key != static_cast<Tkey>(i+1)) {
			flag = 0;
			break;
		}
		Tree.insert(i,i*5);
	}
	EXPECT_EQ(flag,1);
}
TYPED_TEST(avlTreeFixture, iterator_can_use_postfix_incriment) {
	int flag = 1; 
	Tkey i = 0;
	for (avlTree<Tkey, Tvalue>::iterator it = Tree.begin(); it !=Tree.end(); it++) {
		if (static_cast<Tkey>(it->key) != i) {
			flag = 0;
			break;
		}
		i++;
	}
	EXPECT_EQ(flag, 1);
}
TYPED_TEST(avlTreeFixture, can_use_copy_constructor) {
	avlTree<Tkey, Tvalue> T(Tree);

	int flag = 1;
	avlTree<Tkey, Tvalue>::iterator it1 = Tree.begin();
	avlTree<Tkey, Tvalue>::iterator it2 = T.begin();
	while ((it1 != Tree.end()) || (it2 != T.end())) {
		if (static_cast<Tkey>(it1->key) != static_cast<Tkey>(it2->key) || static_cast<Tkey>(it1->value) != static_cast<Tkey>(it2->value)) {
			flag = 0;
			break;
		}
		it1++;
		it2++;
	}
	EXPECT_EQ(flag, 1);
}
TYPED_TEST(avlTreeFixture, can_use_the_assignment_operator) {
	avlTree<Tkey, Tvalue> T;

	T = Tree;
	int flag = 1;
	avlTree<Tkey, Tvalue>::iterator it1 = Tree.begin();
	avlTree<Tkey, Tvalue>::iterator it2 = T.begin();
	while ((it1 != Tree.end()) || (it2 != T.end())) {
		if (static_cast<Tkey>(it1->key) != static_cast<Tkey>(it2->key) || static_cast<Tkey>(it1->value) != static_cast<Tkey>(it2->value)) {
			flag = 0;
			break;
		}
		it1++;
		it2++;
	}
	EXPECT_EQ(flag, 1);
}