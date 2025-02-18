#include<bits/stdc++.h>
using namespace std;// 把它補成4位
string add_zero(string str, int n) {
    while (str.length() < n) {
        str = "0" + str;
    }
    return str;
}
