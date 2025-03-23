#include "ordered_table.h"
#include <gtest.h>

TEST(orderedTable, can_insert_an_element_in_ordered_table)
{
	orderedTable<double, double> table;
	ASSERT_NO_THROW(table.insert(4.5,7.9));
}
TEST(orderedTable, can_insert_some_elements_in_ordered_table)
{
	orderedTable<double, double> table;
	size_t N = 10;

	for (int i = 0; i <= N; i++) table.insert(1.5*i, i);

	EXPECT_EQ((*(table.end()-1)).second, 10.);
}
TEST(orderedTable, can_insert_some_elements_in_ordered_table_reversed)
{
	orderedTable<double, double> table;
	size_t N = 10;

	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ((*(table.begin())).second, 1.);
}
TEST(orderedTable, can_insert_some_elements_in_ordered_table_in_alternating_order)
{
	orderedTable<double, double> table;
	size_t N = 10;

	for (int i = N; i > 0; i--) {
		if (i % 2 == 0) table.insert(1.5 * i, i); 
		else table.insert(15 - i * 1.5, i);
	}
	
	EXPECT_EQ((*(table.begin())).second, 9.);
}
TEST(orderedTable, can_erase_one_element_in_the_end)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);
	
	table.erase(15.);

	EXPECT_EQ((*(table.end()-1)).second, 9.);
}
TEST(orderedTable, can_insert_one_element_and_erase_one_element_in_the_end)
{
	orderedTable<double, double> table;
	size_t N = 10;
	table.insert(1.5, 1.5);

	table.erase(1.5);

	EXPECT_EQ(table.size(),0);
}
TEST(orderedTable, can_erase_element_from_the_centre)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	ASSERT_NO_THROW(table.erase(4.5));
}
TEST(orderedTable, erase_return_right_iterator_on_the_next_element)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ((*table.erase(4.5)).first,6.); //нужный ли указатель возвращает erase
}
TEST(orderedTable, can_erase_all_elements)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);
	
	for (int i = N; i > 0; i--) table.erase(1.5 * i);

	EXPECT_EQ(table.size(), 0); 
}
TEST(orderedTable, can_erase_all_elements_in_other_order)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	for (int i = 1; i<=N; i++) table.erase(1.5 * i);

	EXPECT_EQ(table.size(), 0);
}
TEST(orderedTable, can_erase_some_elements_in_other_order)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	for (int i = 1; i <= 5; i++) table.erase(1.5 * i);

	EXPECT_EQ((*table.begin()).first, 6.*1.5);
}
TEST(orderedTable, erase_some_elements_return_right_iterator)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	for (int i = 1; i < 5; i++) table.erase(1.5 * i);
	
	EXPECT_EQ((*table.erase(1.5 * 5)).second, 6);
}
TEST(orderedTable, can_find_an_element)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ((*table.find(1.5 * 5)).second, 5);
}
TEST(orderedTable, can_find_the_first_element)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ((*table.find(1.5 * 1)).second, 1);
}
TEST(orderedTable, can_find_the_last_element)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ((*table.find(1.5 * 10)).second, 10);
}
TEST(orderedTable, the_square_brackets_operator_return_right_value)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(table[4.5], 3);
}
TEST(orderedTable, the_square_brackets_operator_return_right_values_in_cycle_for)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);
	int flag = 1;

	for (int i = N; i > 0; i--) {
		if (table[1.5 * i] != i) {
			flag = 0;
			break;
		}
	}

	EXPECT_EQ(1, flag);
}
TEST(orderedTable, insert_erase_insert_some_elements)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);
	for (int i = N; i >6; i--) table.erase(1.5 * i);

	for (int i = 10; i < 20; i++) table.insert(1.5 * i,i);
	
	EXPECT_EQ((*(table.end()-1)).first,1.5*19.);
}
TEST(orderedTable, inserting_return_right_iterator)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);
	
	EXPECT_EQ((*(table.insert(1.3 *4 , 4))).first, 1.3 * 4);
}
TEST(orderedTable, inserting_in_the_end_return_right_iterator)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ((*(table.insert(1.3 * 15, 4))).first, 1.3 * 15);
}
TEST(orderedTable, erasing_last_element_return_right_iterator)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(table.end(),table.erase(1.5 * 10));
}
TEST(orderedTable, can_not_erase_not_existing_element)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(table.erase(1.3*6), table.end());
}
TEST(orderedTable, return_iterator_on_the_end_if_we_try_to_find_non_existent_element)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(table.find(1.3 * 6),table.end());
}
TEST(orderedTable, can_use_postfix_increment_in_iterator)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(table.find(1.5 * 6)++, table.find(1.5 * 6));
}
TEST(orderedTable, can_use_prefix_increment_in_iterator)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(++table.find(1.5 * 6), table.find(1.5 * 7));
}
TEST(orderedTable, iterator_can_add_const_to_himself)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	EXPECT_EQ(table.find(1.5 * 6)+3, table.find(1.5 * 9));
}
TEST(orderedTable, can_not_insert_element_with_existing_key)
{
	orderedTable<double, double> table;
	size_t N = 10;
	for (int i = N; i > 0; i--) table.insert(1.5 * i, i);

	ASSERT_ANY_THROW(table.insert(1.5 * 4, 4));
}
