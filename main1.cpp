#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include "LCRS_BalancedTree.h"

using namespace std;

class MyKey : public Key {
public:
    MyKey(unsigned k) : key(k) {}

    MyKey(MyKey const &mk) : key(mk.key) {}

    ~MyKey() {}

    MyKey *clone() const {
        return new MyKey(*this);
    }

    unsigned get_key() { return key; }

    void set_key(unsigned k) { key = k; }

    bool operator<(const Key &rhs) const { return key < ((MyKey &) rhs).key; }

    void print() const { cout << key; }

private:
    unsigned key;
};


class MyValue : public Value {

public:
    MyValue(string val) : value(val) {}

    ~MyValue() {}

    MyValue(MyValue const &v) { value = v.value; }

    MyValue *clone() const {
        return new MyValue(*this);
    }

    string get_value() { return value; }

    void set_value(string val) { value = val; }

    void print() const { cout << value << ","; }

private:
    string value;
};


int main() {

    MyKey mk1(0);
    MyKey mk2(100);
    LCRS_BalancedTree x(&mk1, &mk2);
    for (int i = 0; i < 9; i++) {
        MyKey mk1(i + 1);
        MyValue mv1(string(1, '0' + i + 1));
        x.Insert(&mk1, &mv1);
    }
    for (int i = 0; i < 9; i++) {
        MyKey mk1(i + 1);
        x.Delete(&mk1);
    }
    return 0;
}
