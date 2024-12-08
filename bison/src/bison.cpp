#include "Analy.hpp"
#include "LL1analysis.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <streambuf>
using namespace std;

#define grammarfile "../src/include/grammar.txt"

int main(int argc, char *argv[]) {
    grammar_read(grammarfile); // 文法文件

    unordered_map<string, set<string>> first;
    unordered_map<string, set<string>> follow;

    // 重新获得非终结符集合
    getnoterminators();
    rules = sortRules(rules, noterminators);
    cout << "重新排序后文法为:" << endl;
    for (const auto &rule : rules) {
        cout << rule.left << " -> ";
        for (auto it = rule.right.begin(); it != rule.right.end(); ++it) {
            if (it != rule.right.begin())
                cout << " | ";
            cout << *it;
        }
        cout << endl;
    }

    // 初始化 FIRST 集合（假设开始时为空）
    for (const auto &nt : noterminators) {
        first[nt] = {};
    }

    // 计算 FIRST 集合
    computeFirst(rules, first, noterminators);

    // 初始化 FOLLOW 集合
    for (const string &nt : noterminators) {
        follow[nt] = {};
    }

    // 初始化起始符号的 FOLLOW 集合
    follow[rules.front().left].insert("#");

    // 计算 FOLLOW 集合
    computeFollow(rules, first, follow, rules.front().left, noterminators); // 假设第一个规则的左边是起始符号

    // 打印结果或其他操作
    printFirst(first);
    printFollow(follow);

    // 构建 LL(1) 分析表
    unordered_map<string, unordered_map<string, string>> table;
    buildParseTable(rules, first, follow, table, noterminators);

    // 打印 LL(1) 分析表
    printParseTable(table);

    // 输入要分析的字符串
    string input;
    cout << "请输入要分析的字符串: ";
    cin >> input;

    // 调用分析函数
    parseString(table, input);

    return 0;
}
