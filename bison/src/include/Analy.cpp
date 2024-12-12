#include "Analy.hpp"
#include <iomanip>

void parseString(const unordered_map<string, unordered_map<string, string>> &table, const string &input) {
    stack<string> analysisStack;
    analysisStack.push("#");   // 结束符
    analysisStack.push(start); // 起始符号

    string remainingInput = input + "#"; // 加上结束符号
    size_t step = 1;

    cout << left; // 设置左对齐
    cout << setw(5) << "步骤" << setw(20) << "分析栈" << setw(25) << "当前输入" << setw(25) << "剩余输入串" << setw(30) << "所用产生式" << endl;

    bool result = false;

    while (!analysisStack.empty()) {
        string top = analysisStack.top();      // 栈顶元素
        char currentChar = remainingInput[0];  // 当前输入字符
        string currentCharStr(1, currentChar); // 当前字符转为字符串

        // 打印当前状态
        cout << setw(5) << step++ << setw(20);
        stack<string> tempStack = analysisStack;
        string stackContent;
        while (!tempStack.empty()) {
            stackContent = tempStack.top() + (tempStack.size() > 1 ? " " : "") + stackContent;
            tempStack.pop();
        }
        cout << setw(20) << stackContent << setw(20) << currentChar << setw(20) << remainingInput;

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
                            } else {
                                analysisStack.push(string(1, *it));
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
        cout << "分析成功！该字符串是该文法句子" << endl;
    } else {
        cout << "分析失败！该字符串不是该文法句子" << endl;
    }
}