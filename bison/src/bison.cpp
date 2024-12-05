#include "include/FirstFollow.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <streambuf>
#include <unordered_set>
using namespace std;

#define grammarfile "../src/include/grammar.txt"

// 插入新元素到集合，并返回是否进行了插入
bool insertIfNew(unordered_set<string> &set, const string &item) { return set.insert(item).second; }

// 构建 LL(1) 分析表
void buildParseTable(const vector<node> &rules, const unordered_map<string, set<string>> &first, const unordered_map<string, set<string>> &follow,
                     unordered_map<string, unordered_map<string, string>> &table, const vector<string> &nonTerminals) {

    for (const auto &rule : rules) {
        for (const string &production : rule.right) {
            string A = rule.left;
            size_t i = 0;
            unordered_set<string> lookaheads;

            // 如果产生式为空或为$，则使用FOLLOW(A)作为lookahead
            if (production.empty() || production == "$") {
                // 使用 insert 方法将 FOLLOW(A) 的所有元素插入到 lookaheads 中
                lookaheads.insert(follow.at(A).begin(), follow.at(A).end());
            } else {
                while (i < production.size()) {
                    string X(1, production[i]);
                    if (!isNonTerminal(X, nonTerminals)) {
                        // 如果 X 是终端，则直接加入到 lookaheads 并停止检查
                        insertIfNew(lookaheads, X);
                        break;
                    } else {
                        // 如果 X 是非终结符，将 FIRST(X)\{$} 加入到 lookaheads
                        for (const string &f : first.at(X)) {
                            if (f != "$" && insertIfNew(lookaheads, f)) {
                            }
                        }
                        if (first.at(X).find("$") == first.at(X).end())
                            break; // 如果 FIRST(X) 不包含 $，则停止检查
                    }
                    ++i;
                }
                // 如果所有 X 都可以推导出 $ 或最后一个符号能推导出 $，则将 FOLLOW(A) 加入到 lookaheads
                if (i == production.size() || (i < production.size() && first.at(string(1, production.back())).find("$") != first.at(string(1, production.back())).end())) {
                    // 同样使用 insert 方法将 FOLLOW(A) 的所有元素插入到 lookaheads 中
                    lookaheads.insert(follow.at(A).begin(), follow.at(A).end());
                }
            }

            // 将产生式添加到解析表中
            for (const string &lookahead : lookaheads) {
                table[A][lookahead] = production;
            }
        }
    }
}

// 打印 LL(1) 分析表
void printParseTable(const unordered_map<string, unordered_map<string, string>> &table) {
    cout << "\nLL(1) 分析表:" << endl;
    for (const auto &lhsPair : table) {
        cout << setw(2) << left << lhsPair.first << ": " << endl;
        for (const auto &rhsPair : lhsPair.second) {
            cout << "  " << rhsPair.first << " -> " << rhsPair.second << endl;
        }
    }
}

/*string readfile(const char *path) {
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
}*/

int main(int argc, char *argv[]) {
    /*assert(argc == 3);
    auto input = argv[1];      // 源代码输入
    auto output = argv[2];     // 结果输出到文件中*/
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
        cout << endl << endl;
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

    return 0;
}
