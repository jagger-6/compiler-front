#include "include/scanner.hpp"
#include "include/synmap.hpp"
#include <cassert>
#include <iostream>
#include <streambuf>
using namespace std;

string readfile(const char *path) {
    // 读取文件
    string str = "";
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Failed to open " << path << endl;
    } else {
        // 读取文件所有内容
        string s((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
        str = s;
    }
    fin.close();
    fin.clear();
    return str;
}

void writefile(string str, const char *path) {
    // 输出到文件
    fstream fout;
    fout.open(path, ios_base::out);
    if (!fout.is_open()) {
        cerr << "Failed to open " << path << endl;
    } else {
        fout << str << endl;
    }
    fout.close();
    fout.clear();
}

int main(int argc, char *argv[]) {
    assert(argc == 3);
    auto input = argv[1];                         // 源代码输入
    auto output = argv[2];                        // 结果输出到文件中
    syntable_read("../src/include/syntable.txt"); // 种别码文件
    writefile(scanner(readfile(input)), output);
    return 0;
}
