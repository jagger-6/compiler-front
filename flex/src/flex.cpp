#include "include/scanner.hpp"
#include "include/synmap.hpp"
#include <iostream>
using namespace std;

int main() {
    syntable_read("../src/include/syntable.txt");
    cout << scanner("ifx>9 x=2*x+1/3;");
    return 0;
}
