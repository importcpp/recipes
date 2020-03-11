#include "KString.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{

    // MyString s1;
    // cout << s1 << endl;
    // MyString s2("hello ");
    // cout << s2 << endl;
    // MyString s3 = "world";
    // cout << s3 << endl;
    // MyString s4(3, '!');
    // cout << s4 << endl;
    // MyString s5 = s1 + s2 + s3 + s4;
    // cout << s5 << endl;

    // MyString s6(s5);
    // cout << s6 << endl;
    MyString s6 = "Hi !!!";
    MyString s7;
    s7 = s6;
    MyString s77 = s6;
    cout << s7 << endl;
    cout << s7[2] << endl;

    cin >> s7;
    cout << s7 << endl;

    cout << "===============Test Sort==================" << endl;
    MyString s8("Ab");
    MyString s9("Ac");
    cout << "s8 < s9: " << (s8 < s9) << endl;

    vector<MyString> v = {"Bbd", "Abc", "Abd", "Abd", "AAAA", "AAc", "aaa"};
    sort(v.begin(), v.end());

    cout << "===Print Vector====" << endl;
    for(auto& e:v){
        cout << e << endl;
    }
    cout << "===Print Vector====" << endl;
    cout << "===============Test Sort==================" << endl;
    return 0;
}
