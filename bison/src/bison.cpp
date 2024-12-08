#include "include/LL1analysis.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <streambuf>
using namespace std;

#define grammarfile "../src/include/grammar.txt"

// 分析栈模拟函数
void parseString(const unordered_map<string, unordered_map<string, string>> &table, const string &input) {
    stack<string> analysisStack;
    analysisStack.push("#");   // 结束符
    analysisStack.push(start); // 起始符号

    string remainingInput = input + "#"; // 加上结束符号
    size_t step = 1;

    cout << "步骤\t分析栈\t当前输入\t剩余输入串\t所用产生式" << endl;

    bool result = false;

    while (!analysisStack.empty()) {
        string top = analysisStack.top();      // 栈顶元素
        char currentChar = remainingInput[0];  // 当前输入字符
        string currentCharStr(1, currentChar); // 当前字符转为字符串

        // 打印当前状态
        cout << step++ << "\t";
        stack<string> tempStack = analysisStack;
        string stackContent;
        while (!tempStack.empty()) {
            stackContent = tempStack.top() + stackContent;
            tempStack.pop();
        }
        cout << stackContent << "\t\t" << currentChar << "\t\t" << remainingInput << "\t\t";

        // 栈顶符号是终结符或 #
        if (top == "#" || !isNonTerminal(top, noterminators)) { // 判断是否为非终结符
            if (top == "#" && remainingInput == "#") {
                result = true;
            }
            if (top[0] == currentChar) { // 匹配成功
                analysisStack.pop();
                remainingInput = remainingInput.substr(1);
                cout << "匹配" << endl;
            } else {
                cout << "错误：符号不匹配！" << endl;
                break;
            }
        }
        // 栈顶符号是非终结符
        else {
            auto rowIt = table.find(top); // 查找栈顶非终结符对应的表行
            if (rowIt != table.end()) {
                auto colIt = rowIt->second.find(currentCharStr); // 查找当前输入符号对应的列
                if (colIt != rowIt->second.end()) {
                    string production = colIt->second;
                    analysisStack.pop();     // 弹出当前非终结符
                    if (production != "$") { // 空串不需要入栈
                        for (auto it = production.rbegin(); it != production.rend(); ++it) {
                            if ((it + 1) != production.rend() && *it == '\'' && isupper(*(it + 1))) {
                                string tmp = string(1, *(it + 1));
                                tmp += "'";
                                analysisStack.push(tmp);
                                it++;
                                // cout << tmp << endl;
                            } else {
                                analysisStack.push(string(1, *it));
                                // cout << *it << " " << endl;
                            }
                        }
                    }
                    cout << top << " -> " << production << endl;
                } else {
                    cout << "错误：当前输入符号未找到匹配的产生式！" << endl;
                    break;
                }
            } else {
                cout << "错误：栈顶符号未找到匹配的行！" << endl;
                break;
            }
        }
    }

    if (result) {
        cout << "分析成功！" << endl;
    } else {
        cout << "分析失败！" << endl;
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

    // 输入要分析的字符串
    string input;
    cout << "请输入要分析的字符串: ";
    cin >> input;

    // 调用分析函数
    parseString(table, input);

    return 0;
}
