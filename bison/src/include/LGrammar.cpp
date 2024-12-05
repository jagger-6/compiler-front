#include "LGrammar.hpp"

vector<node> rules;
vector<string> noterminators; // 非终结符

// 使用字符串分割
void stringsplit_string(const string &str, const string split, vector<string> &res) {
    if (str == "")
        return;

    size_t pos = 0;
    while (str[pos] != '\0') {
        if (str[pos] == split[0]) {
            for (int i = 1; i < split.length(); i++) {
                if (str[pos + i] == split[i] && str[pos + i] != '\0') {
                } else {
                    cerr << "wrong grammar\n";
                    assert(false);
                }
            }
            break;
        } else {
            pos++;
        }
    }
    if (pos + split.size() <= str.size() - 1) {
        res.push_back(str.substr(0, pos));
        res.push_back(str.substr(pos + split.size()));
    } else {
        cerr << "wrong grammar\n";
        assert(false);
    }
}

// 使用字符分割
void stringsplit_char(const string &str, const char split, set<string> &res) {
    if (str == "")
        return;
    // 在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str + split;
    size_t pos = strs.find(split);

    // 若找不到内容则字符串搜索函数返回 npos
    while (pos != strs.npos) {
        string temp = strs.substr(0, pos);
        res.insert(temp);
        // 去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}

void grammar_read(const char *path) {
    fstream fin;
    string str;
    fin.open(path, ios_base::in);
    if (!fin.is_open()) {
        cerr << "Failed to open " << path << endl;
    } else {
        while (getline(fin, str)) { // 一行一行读取
            vector<string> tmp;
            set<string> prule;
            stringsplit_string(str, "->", tmp); // 以->为分隔
            stringsplit_char(tmp[1], '|', prule);
            struct node r;
            r.left = tmp[0];
            r.right = prule;
            rules.push_back(r);
        }
        getnoterminators();
        factorization();
        rules = sortRules(rules, noterminators);
        cout << "提取公共左因子后的文法为:" << endl;
        for (const auto &rule : rules) {
            cout << rule.left << " -> ";
            for (auto it = rule.right.begin(); it != rule.right.end(); ++it) {
                if (it != rule.right.begin())
                    cout << " | ";
                cout << *it;
            }
            cout << endl;
        }
        eliminatelr();
        cout << "消除左递归后的文法为:" << endl;
        for (const auto &rule : rules) {
            cout << rule.left << " -> ";
            for (auto it = rule.right.begin(); it != rule.right.end(); ++it) {
                if (it != rule.right.begin())
                    cout << " | ";
                cout << *it;
            }
            cout << endl;
        }

        fin.close();
        fin.clear();
    }
}

void getnoterminators() { // 终结符排序
    noterminators.clear();
    for (auto it : rules) {
        if (!noterminators.empty()) {
            bool flag = true;
            for (auto i : noterminators) {
                if (it.left == i) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                noterminators.push_back(it.left);
            }
        } else {
            noterminators.push_back(it.left);
        }
        for (auto IT : rules) {
            if (IT.left == it.left + "'") {
                noterminators.push_back(IT.left);
                break;
            }
        }
    }
    /*for (auto it : noterminators)
        cout << it << endl;*/
}

void factorization() {
    for (int i = 0; i < rules.size(); i++) {
        map<string, vector<string>> prefixMap; // 存储前缀和对应的后缀

        // 遍历当前规则右部，提取公共前缀
        for (const auto &right : rules[i].right) {
            // 查找当前候选项的公共前缀
            string prefix;
            bool commonPrefixFound = false;

            // 比较每对候选项，查找公共前缀
            for (const auto &otherRight : rules[i].right) {
                if (right == otherRight) {
                    continue; // 忽略自身比较
                }

                // 寻找当前候选项和其它候选项的公共前缀
                size_t len = min(right.size(), otherRight.size());
                size_t j = 0;
                while (j < len && right[j] == otherRight[j]) {
                    j++;
                }

                if (j > 0) {                     // 如果找到公共前缀
                    prefix = right.substr(0, j); // 提取公共前缀
                    commonPrefixFound = true;
                }
            }

            if (commonPrefixFound) {
                // 如果找到了公共前缀，则将当前候选项的后缀存入 map
                string suffix = right.substr(prefix.size());
                prefixMap[prefix].push_back(suffix);
            } else {
                // 如果没有公共前缀，则将候选项视为独立处理
                prefixMap[right].push_back("");
            }
        }

        // 更新当前规则右部
        rules[i].right.clear();
        string newNonTerminal = rules[i].left + "'"; // 新的非终结符

        // 遍历每个前缀及其对应的后缀列表
        for (const auto &[prefix, suffixes] : prefixMap) {
            if (suffixes.size() == 1 && suffixes[0].empty()) {
                // 如果只有一个候选项并且后缀为空，直接加入该规则
                rules[i].right.insert(prefix);
            } else {
                // 如果存在多个后缀，添加新的非终结符
                rules[i].right.insert(prefix + newNonTerminal);

                // 为新非终结符创建规则，存储所有后缀
                set<string> newRuleSet;
                for (const auto &suffix : suffixes) {
                    newRuleSet.insert(suffix.empty() ? "$" : suffix); // 空后缀替换为 $
                }
                rules.push_back({newNonTerminal, newRuleSet}); // 新的规则添加到文法中
            }
        }
    }
}

void eliminatelr() {
    // 遍历所有非终结符
    for (int i = 0; i < noterminators.size(); i++) {
        // 替换间接左递归：依次消除当前非终结符与之前非终结符的左递归
        for (int j = 0; j < i; j++) {
            int posi = -1, posj = -1;

            // 找到当前非终结符和之前非终结符的位置
            for (int k = 0; k < rules.size(); k++) {
                if (rules[k].left == noterminators[i])
                    posi = k;
                if (rules[k].left == noterminators[j])
                    posj = k;
            }

            if (posi == -1 || posj == -1)
                continue; // 如果未找到对应规则，跳过

            set<string> newRules; // 存储替换后的新规则
            for (const auto &right : rules[posi].right) {
                if (right.find(noterminators[j]) == 0) {                   // 若右部以非终结符 j 开头
                    string suffix = right.substr(noterminators[j].size()); // 提取后缀
                    for (const auto &sub : rules[posj].right) {
                        newRules.insert(sub + suffix); // 替换前缀并生成新规则
                    }
                } else {
                    newRules.insert(right); // 保留未受影响的规则
                }
            }
            rules[posi].right = newRules; // 更新规则
        }

        // 消除直接左递归
        erasedirect(i);
    }
}

void erasedirect(int posi) {
    vector<string> directRecursion, nonRecursion; // 分别存储直接递归规则和非递归规则
    string currentNonTerminal = rules[posi].left;

    // 遍历规则右部，分类存储直接递归和非递归规则
    for (const auto &right : rules[posi].right) {
        if (right.find(currentNonTerminal) == 0) {                              // 检测直接左递归
            directRecursion.push_back(right.substr(currentNonTerminal.size())); // 提取后缀
        } else {
            nonRecursion.push_back(right); // 非递归规则
        }
    }

    if (directRecursion.empty()) {
        return; // 若无直接左递归，退出
    }

    // 创建新的非终结符用于存储递归后的规则
    string newNonTerminal = currentNonTerminal + "'";
    rules[posi].right.clear(); // 清空当前规则的右部

    // 添加非递归规则，右部追加新非终结符
    for (const auto &rule : nonRecursion) {
        rules[posi].right.insert(rule + newNonTerminal);
    }

    set<string> newRules; // 存储新非终结符的规则
    for (const auto &rule : directRecursion) {
        newRules.insert(rule + newNonTerminal); // 原后缀追加新非终结符
    }
    newRules.insert("$");                        // 添加 $（空规则）
    rules.push_back({newNonTerminal, newRules}); // 将新非终结符及其规则加入文法
}

// 根据 nonTerminals 的顺序对 rules 进行排序
vector<node> sortRules(const vector<node> &rules, const vector<string> &nonTerminals) {
    // 创建一个映射来存储每个非终结符对应的规则
    unordered_map<string, vector<node>> ruleMap;

    // 将所有规则按非终结符分类
    for (const auto &rule : rules) {
        ruleMap[rule.left].push_back(rule);
    }

    // 创建一个新的规则列表，按照 nonTerminals 的顺序添加规则
    vector<node> sortedRules;
    for (const auto &nt : nonTerminals) {
        if (ruleMap.find(nt) != ruleMap.end()) {
            // 将该非终结符对应的所有规则添加到新的规则列表中
            sortedRules.insert(sortedRules.end(), ruleMap[nt].begin(), ruleMap[nt].end());
        }
    }

    return sortedRules;
}
