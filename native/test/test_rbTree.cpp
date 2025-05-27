#include <gtest.h>
#include <string>
#include <chrono>
#include <random>
#include "RBTree.h"

TEST(RBTree, canMakeATree) {
	RBTree<int, double> tree;
}

TEST(RBTree, canInsertAnElementIntoAnEmptyTree) {
	RBTree<int, double> tree;
	ASSERT_NO_THROW(tree.insert(1, 1.0));
}

TEST(RBTree, canInsertAnElementIntoANonEmptyTree) {
	RBTree<int, double> tree;
	tree.insert(1, 1.0);
	ASSERT_NO_THROW(tree.insert(0, 0.0));
}

TEST(RBTree, throwsForInsertionOfAnElementWithAlreadyExistantKey) {
	RBTree<int, double> tree;
	tree.insert(1, 1.0);
	ASSERT_ANY_THROW(tree.insert(1, 0.0));
}

TEST(RBTree, canInsertALotOfElementsDuringTheNormalTime) {
	RBTree<int, double> tree;

	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; i++) {
			tree.insert(i, i);
	}
	auto end = std::chrono::steady_clock::now();

	EXPECT_LE(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(), 1);
	
}

TEST(RBTree, canInsertALotOfElementsInTheReverseOrderDuringTheNormalTime) {
	RBTree<int, double> tree;

	auto start = std::chrono::steady_clock::now();
	for (int i = 1000000; i >= 0; i--) {
		tree.insert(i, i);
	}
	auto end = std::chrono::steady_clock::now();

	EXPECT_LE(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(), 1);

}

TEST(RBTree, canInsertALotOfElementsInTheRandomOrderDuringTheNormalTime) {
	RBTree<int, double> tree;

	std::random_device rd;   
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dist(1, 1000000000);

	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; i++) {
		try {
			tree.insert(dist(gen), dist(gen));
		}
		catch (char* exception) {
			continue;
		}
		
	}
	auto end = std::chrono::steady_clock::now();

	EXPECT_LE(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(), 1);
}

TEST(RBTree, copyConstructorWorks) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) {
		tree.insert(i, i);
	}
	//tree.print();
	RBTree<int, double> tree1 = tree;

}

TEST(RBTree, canAssignTrees) {
	RBTree<int, double> tree, tree1;
	for (int i = 0; i < 10; i++) {
		tree.insert(i, i);
	}
	ASSERT_NO_THROW(tree1 = tree);

}

TEST(RBTree, canFindAnExistantElement) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) {
		tree.insert(i, i);
	}
	ASSERT_NO_THROW(tree.find(5));
	EXPECT_LE(tree.at(tree.find(5)) - 5.0, 10e-7);
}

TEST(RBTree, returnIteratorToTheEndIfFindingElementDoesntExist) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) {
		tree.insert(i, i);
	}
	ASSERT_NO_THROW(tree.find(11));
	EXPECT_EQ(tree.find(11), tree.end());
}

TEST(RBTree, canEraseTheOnlyOneElementInTheTree) {
	RBTree<int, double> tree;
	tree.insert(0, 0);
	ASSERT_NO_THROW(tree.erase(0));
	EXPECT_TRUE(tree.empty());
}

TEST(RBTree, throwsForErasureOfANonExistantElement) {
	RBTree<int, double> tree;
	tree.insert(0, 0);
	ASSERT_ANY_THROW(tree.erase(2));
}

TEST(RBTree, erasureReturnsIteratorToTheNextElement) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) tree.insert(i, i);
	RBTree<int, double>::iterator next = (tree.find(2))++;
	EXPECT_EQ(next, tree.erase(2));
}

TEST(RBTree, canEraseTheRoot) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) tree.insert(i, i);
	ASSERT_NO_THROW(tree.erase(3));
}

TEST(RBTree, canEraseTheLastElement) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) tree.insert(i, i);
	ASSERT_NO_THROW(tree.erase(9));
}

TEST(RBTree, canEraseALotOfElementsDuringTheNormalTime) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10000000; i++) {
		tree.insert(i, i);
	}
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < 10000000; i++) {
		tree.erase(i);
	}
	auto end = std::chrono::steady_clock::now();

	EXPECT_LE(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(), 1);
}

TEST(RBTree, canReturnValueOfAnExistingElement) {
	RBTree<int, double> tree;
	size_t n = 4;
	for (int i = 0; i < n; i++) tree.insert(i, i);

	ASSERT_NO_THROW(tree[2]);
}

TEST(RBTree, canReturnCorrectValueOfAnExistingElement) {
	RBTree<int, double> tree;
	size_t n = 4;
	for (int i = 0; i < n; i++) tree.insert(i, i);

	double exp = 2.0;

	EXPECT_EQ(exp, tree[2]);
}

TEST(RBTree, canReturnSmthIfTheKeyDoesntExist) {
	RBTree<int, double> tree;
	size_t n = 4;
	for (int i = 0; i < n; i++) tree.insert(i, i);

	ASSERT_NO_THROW(tree[5]);
}

TEST(RBTree, insertsTheElementWithDefaultValueIfSuchKeyDoesntExist) {
	RBTree<int, double> tree;
	size_t n = 4;
	for (int i = 0; i < n; i++) tree.insert(i, i);

	double p = tree[5];

	EXPECT_EQ(double(), p);
}

TEST(RBTree, iteratorWorksCorrectly) {
	RBTree<int, double> tree;
	for (int i = 0; i < 10; i++) tree.insert(i, i);

	auto i = tree.begin();
	double t1 = tree.at(i), t2;
	++i;
	for (; i != tree.end(); i++) {
		t2 = tree.at(i);
		EXPECT_LE(t1, t2);
		t1 = t2;
	}
	//for (auto p = tree.begin(); p != tree.end(); p++) std::cout << tree.at(p);
}
