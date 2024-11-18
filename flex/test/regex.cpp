#include <iostream>
#include <regex>
#include <string>
using namespace std;

int main() {
    int pos = 0;
    std::string s("12.0e+eE+21 12.0e+2");
    string token;
    while ((isdigit(s[pos])) || s[pos] == '.' || s[pos] == 'e' || s[pos] == 'E' || s[pos] == '+' || s[pos] == '-') {
        token += s[pos];
        pos++;
    }
    cout << token << endl;
    pos--;
    cout << pos << endl;
    std::regex e("^[-+]?[0-9]+[.]([0-9]+)?([eE][-+]?[0-9]+)?$"); // 正则表达式，匹配数字和字母序列
    std::smatch match;

    if (std::regex_search(token, match, e)) {
        std::cout << "Found a match!\n";
        // 输出匹配的子串和分组
        for (size_t i = 0; i < match.size(); ++i) {
            std::cout << "Match " << i << ": " << match[i] << '\n';
        }
        pos = match[0].length() - 1;
        cout << pos;
    } else {
        std::cout << "No match found.\n";
    }

    return 0;
}