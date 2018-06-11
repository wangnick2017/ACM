#include "fvector.h"
#include <iostream>
using namespace std;
int main(){
	fvector<int, int> t;
	t.init("bryan");
        for(int i = 0;i < int(1e5); ++i){
		t.insert(i, i);
                t.modify(i, i + 100);
	}

        for(int i = 0; i <= int(1e5); ++i){
                cout << t.find(i - 1) << "\t";
	}
}
