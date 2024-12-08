#include "LL1analysis.hpp"

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

            // 使用 find 检查 FOLLOW 集合是否包含 A
            auto followIt = follow.find(A);
            if (followIt == follow.end()) {
                cerr << "Error: Follow set for non-terminal '" << A << "' is not defined." << endl;
                exit(EXIT_FAILURE);
            }

            // 如果产生式为空或为$，则使用FOLLOW(A)作为lookahead
            if (production.empty() || production == "$") {
                lookaheads.insert(followIt->second.begin(), followIt->second.end());
            } else {
                while (i < production.size()) {
                    string X(1, production[i]);
                    if (!isNonTerminal(X, nonTerminals)) {
                        insertIfNew(lookaheads, X);
                        break;
                    } else {
                        // 使用 find 检查 FIRST 集合是否包含 X
                        auto firstIt = first.find(X);
                        if (firstIt == first.end()) {
                            cerr << "Error: First set for non-terminal '" << X << "' is not defined." << endl;
                            exit(EXIT_FAILURE);
                        }
                        for (const string &f : firstIt->second) {
                            if (f != "$" && insertIfNew(lookaheads, f)) {
                            }
                        }
                        if (firstIt->second.find("$") == firstIt->second.end())
                            break; // 如果 FIRST(X) 不包含 $，则停止检查
                    }
                    ++i;
                }
                // 检查最后一个符号是否能推导出 $
                if (i == production.size() || (i < production.size() && first.find(string(1, production.back())) != first.end() &&
                                               first.at(string(1, production.back())).find("$") != first.at(string(1, production.back())).end())) {
                    lookaheads.insert(followIt->second.begin(), followIt->second.end());
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

    // 收集所有的终结符和非终结符
    set<string> terminals;
    set<string> non_terminals;
    for (const auto &lhsPair : table) {
        non_terminals.insert(lhsPair.first);
        for (const auto &rhsPair : lhsPair.second) {
            terminals.insert(rhsPair.first);
        }
    }

    // 添加结束符号 #
    terminals.insert("#");

    // 打印表头
    int column_width = 10; // 设置列宽
    cout << setw(column_width) << " ";
    for (const string &terminal : terminals) {
        cout << setw(column_width) << terminal;
    }
    cout << endl;

    // 打印分割线
    cout << setfill('-') << setw(column_width * (terminals.size() + 1)) << "" << endl;
    cout << setfill(' ');

    // 打印每一行
    for (const string &non_terminal : non_terminals) {
        cout << setw(column_width) << non_terminal;
        for (const string &terminal : terminals) {
            auto it = table.find(non_terminal);
            if (it != table.end()) {
                auto production_it = it->second.find(terminal);
                if (production_it != it->second.end()) {
                    cout << setw(column_width) << production_it->second;
                } else {
                    cout << setw(column_width) << " ";
                }
            } else {
                cout << setw(column_width) << " ";
            }
        }
        cout << endl;
    }
}