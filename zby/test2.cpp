#include <bits/stdc++.h>
#include "DB.hpp"
using namespace std;

int main() {
	DB<int, int[60]> tDB;
	tDB.init("test");
	int a[60];
	for (int i = 0; i < 60; i++) a[i] = i;
	tDB.insert(1, a);
	auto ret = tDB.find(1);
	for (int i = 0; i < 60; i++) printf("%d ", (*ret.first)[i]);
	return 0;
}