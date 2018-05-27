#include <cstdio>
#include <string>
#include "backend.hpp"

bool eqv(const char *a, const char *b) {
    int len1 = strlen(a); int len2 = strlen(b);
    if (len1 != len2) return 0;
    for(int i = 0; i < len1; i++) if (a[i] != b[i]) return 0;
    return 1;
}

int main(){
    //freopen("a.in", "r", stdin);
    //freopen("a.out", "w", stdout);
    Backend tmp;
    tmp.init();
    char op[50];
    while (1) {
        scanf("%s", op);
        if (eqv(op, "register")) printf("%d\n", tmp.regist());
        else if (eqv(op, "login")) printf("%d\n", tmp.login());
        else if (eqv(op, "query_profile")) {
            int x; scanf("%d", &x);
            printf("%s\n", tmp.query_profile(x));
        }
        else if (eqv(op, "modify_profile")) printf("%d\n", tmp.modify_profile());
        else if (eqv(op, "modify_privilege")) {
            int id1, id2, pri;
            scanf("%d%d%d", &id1, &id2, &pri);
            printf("%d\n", tmp.modify_privilege(id1, id2, pri));
        }
        else if (eqv(op, "add_train")) printf("%d\n", tmp.add_train());
        else if (eqv(op, "modify_train")) printf("%d\n", tmp.modify_train());
        else if (eqv(op, "sale_train")) printf("%d\n", tmp.sale_train());
        else if (eqv(op, "delete_train")) printf("%d\n", tmp.delete_train());
        else if (eqv(op, "query_train")) printf("%s", tmp.query_train());
        else if (eqv(op, "query_ticket")) printf("%s", tmp.query_ticket());
        else if (eqv(op, "buy_ticket")) printf("%d\n", tmp.buy_ticket());
        else if (eqv(op, "query_order")) printf("%s", tmp.query_order());
        else if (eqv(op, "refund_ticket")) printf("%d\n", tmp.refund_ticket());
        else if (eqv(op, "query_transfer")) printf("%s", tmp.query_transfer());
        else if (eqv(op, "clean")) {
            tmp.clear();
            printf("1\n");
        }
        else if (eqv(op, "exit")) {
            printf("BYE\n");
            break;
        }
    }
    return 0;
}
