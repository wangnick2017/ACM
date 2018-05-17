#include "deque.h"
#include <iostream>
int main(){
	stl::DB<int> db;
	for(int i = 1; i <= 100; ++i) db.insert(i);
}
