
#include "SolutionManager.h"
#include "RBTree.h"
#include "hashTable.h"

int main() {

	SolutionManager<hashTable<std::string, Polynom>> m;
	m.manage();
	
	return 0;
}