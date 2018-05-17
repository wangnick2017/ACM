#include "backend.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
    freopen("a.in", "r", stdin);
    Backend tmp;
    tmp.init();
    cout << tmp.add_train() << endl;
    cout << tmp.query_train() << endl;
    //cout << tmp.modify_train() << endl;
    //cout << tmp.query_train() << endl;
    cout << tmp.sale_train() << endl;
    //cout << tmp.modify_train() << endl;
    cout << tmp.query_ticket() << endl;
    return 0;
}