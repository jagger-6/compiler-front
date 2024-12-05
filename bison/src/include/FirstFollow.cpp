#include "FirstFollow.hpp"

// 判断一个符号是否是非终结符
bool isNonTerminal(const string &symbol, const vector<string> &nonTerminals) { return find(nonTerminals.begin(), nonTerminals.end(), symbol) != nonTerminals.end(); }

// 计算 FIRST 集合
void computeFirst(const vector<node> &rules, unordered_map<string, set<string>> &first, const vector<string> &nonTerminals) {
    bool changed;
    do {
        changed = false;
        for (const auto &rule : rules) {
            for (const string &production : rule.right) {
                string A = rule.left;
                size_t i = 0;
                while (i < production.size()) {
                    string X = string(1, production[i]);
                    if (!isNonTerminal(X, nonTerminals)) {
                        // 如果 X 是终端，则直接加入到 FIRST(A)
                        if (first[A].insert(X).second)
                            changed = true;
                        break; // 终端之后的内容不影响 FIRST 集合
                    } else {
                        // 如果 X 是非终结符，将 FIRST(X)\{$} 加入到 FIRST(A)
                        for (const string &f : first[X]) {
                            if (f != "$" && first[A].insert(f).second)
                                changed = true;
                        }
                        if (first[X].find("$") == first[X].end())
                            break; // 如果 FIRST(X) 不包含 $，则停止检查
                    }
                    ++i;
                }
                // 如果所有 X 都可以推导出 $，则将 $ 加入到 FIRST(A)
                if (i == production.size() && first[A].insert("$").second) {
                    cout << *first[A].begin();
                    changed = true;
                }
            }
        }
    } while (changed);
}

void computeFollow(const std::vector<node> &rules, const std::unordered_map<std::string, std::set<std::string>> &first, std::unordered_map<std::string, std::set<std::string>> &follow,
                   const std::string &startSymbol, const std::vector<std::string> &nonTerminals) {
    bool changed = true;

    while (changed) {
        changed = false;
        for (const auto &rule : rules) {
            for (const auto &production : rule.right) {
                size_t i = 0;
                while (i < production.size()) {
                    std::string B = "";
                    if (isupper(production[i])) {
                        // 找到整个非终结符（可能包含 '）
                        B += production[i++];
                        if (i < production.size() && production[i] == '\'') {
                            B += production[i++];
                        }
                    } else {
                        // 如果是终结符，则只取单个字符
                        B = std::string(1, production[i++]);
                    }

                    if (isNonTerminal(B, nonTerminals)) {
                        // 对于 A -> αBβ，将 FIRST(β)\{$} 加入到 FOLLOW(B)
                        std::string beta = production.substr(i);
                        if (!beta.empty()) {
                            // 使用 find 来安全检查 first 映射中是否含有 beta 键
                            auto it = first.find(beta);
                            if (it != first.end()) { // 确保键存在
                                for (const auto &f : it->second) {
                                    if (f != "$" && follow[B].insert(f).second) {
                                        changed = true;
                                    }
                                }
                                // cout << endl;
                                //  如果 β 可以推导出 $ 或不存在 β
                                if (it->second.count("$")) {
                                    for (const auto &f : follow[rule.left]) {
                                        if (follow[B].insert(f).second) {
                                            changed = true;
                                        }
                                    }
                                }
                            }
                        } else { // 如果 B 是最后一个符号或没有 β
                            // 将 FOLLOW(A) 加入到 FOLLOW(B)
                            for (const auto &f : follow[rule.left]) {
                                if (follow[B].insert(f).second) {
                                    changed = true;
                                }
                            }
                        }
                    } else if (!B.empty()) { // 如果 B 是一个终结符
                        // 查找最近的前一个非终结符，并将该终结符添加到它的 FOLLOW 集中
                        for (int k = i - 1; k >= 0; --k) {
                            string prev;
                            prev += production[k];
                            if (isNonTerminal(prev, nonTerminals)) {
                                if (follow[prev].insert(B).second) {
                                    changed = true;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void printFirst(const unordered_map<string, set<string>> &first) {
    cout << "FIRST 集合:" << endl;
    for (const auto &pair : first) {
        cout << setw(2) << left << pair.first << " -> { ";
        for (auto it = pair.second.begin(); it != pair.second.end(); ++it) {
            if (it != pair.second.begin())
                cout << ", ";
            cout << *it;
        }
        cout << " }" << endl;
    }
}

void printFollow(const unordered_map<string, set<string>> &follow) {
    cout << "\nFOLLOW 集合:" << endl;
    for (const auto &pair : follow) { // 直接遍历 follow map
        cout << setw(2) << left << pair.first << " -> { ";
        for (auto it = pair.second.begin(); it != pair.second.end(); ++it) {
            if (it != pair.second.begin())
                cout << ", ";
            cout << *it;
        }
        cout << " }" << endl;
    }
}