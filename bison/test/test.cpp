#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct node {
    string left;       // 左部
    set<string> right; // 右部
}; // 产生式
static vector<node> rules;
static vector<string> noterminators; // 非终结符

void stringsplit_string(const string &str, const string split, vector<string> &res); // 切割字符串
void stringsplit_char(const string &str, const char split, set<string> &res);        // 使用字符分割
void grammar_read(const char *path);                                                 // 读取文法文件
void getnoterminators();                                                             // 获取非终结符并排序
string myreplace(string str, string s, string t);
void factorization();
void eliminatelr();
bool allend(string str);
void erasedirect(int posi);

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
            cout << tmp[0] << " ";
            set<string>::iterator it;
            for (it = prule.begin(); it != prule.end(); it++)
                cout << *it << " ";
            cout << endl;
            struct node r;
            r.left = tmp[0];
            r.right = prule;
            rules.push_back(r);
        }
        /*for (auto i : rules) {
            cout << i.left << " " << *i.right.begin() << " ";
            cout << endl;
        }*/
        cout << "转换后的文法为:" << endl;
        for (const auto &rule : rules) {
            cout << rule.left << " -> ";
            for (auto it = rule.right.begin(); it != rule.right.end(); ++it) {
                if (it != rule.right.begin())
                    cout << " | ";
                cout << *it;
            }
            cout << endl;
        }

        getnoterminators();
        factorization();
        eliminatelr();
        cout << "转换后的文法为:" << endl;
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
    for (auto it : rules) {
        noterminators.push_back(it.left);
    }
}

string myreplace(string str, string s, string t) {
    while (str.find(s) != -1) {
        int pos = str.find(s);
        str.replace(pos, s.size(), t);
    }
    return str;
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
                    newRuleSet.insert(suffix.empty() ? "Σ" : suffix); // 空后缀替换为 ε
                }
                rules.push_back({newNonTerminal, newRuleSet}); // 新的规则添加到文法中
            }
        }
    }
}

void eliminatelr() {
    for (int i = 0; i < noterminators.size(); i++) {
        // 替换间接左递归
        for (int j = 0; j < i; j++) {
            int posi = -1, posj = -1;

            for (int k = 0; k < rules.size(); k++) {
                if (rules[k].left == noterminators[i])
                    posi = k;
                if (rules[k].left == noterminators[j])
                    posj = k;
            }

            if (posi == -1 || posj == -1)
                continue;

            set<string> newRules;
            for (const auto &right : rules[posi].right) {
                if (right.find(noterminators[j]) == 0) {
                    string suffix = right.substr(noterminators[j].size());
                    for (const auto &sub : rules[posj].right) {
                        newRules.insert(sub + suffix);
                    }
                } else {
                    newRules.insert(right);
                }
            }
            rules[posi].right = newRules;
        }

        // 消除直接左递归
        erasedirect(i);
    }
}

bool allend(string str) {
    for (int i = 0; i < str.size(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            return false;
    }
    return true;
}

void erasedirect(int posi) {
    vector<string> directRecursion, nonRecursion;
    string currentNonTerminal = rules[posi].left;

    for (const auto &right : rules[posi].right) {
        if (right.find(currentNonTerminal) == 0) {
            directRecursion.push_back(right.substr(currentNonTerminal.size()));
        } else {
            nonRecursion.push_back(right);
        }
    }

    if (directRecursion.empty()) {
        return; // 没有直接左递归
    }

    // 创建新的非终结符
    string newNonTerminal = currentNonTerminal + "'";
    rules[posi].right.clear();

    for (const auto &rule : nonRecursion) {
        rules[posi].right.insert(rule + newNonTerminal);
    }

    set<string> newRules;
    for (const auto &rule : directRecursion) {
        newRules.insert(rule + newNonTerminal);
    }
    newRules.insert("Σ"); // 添加 ε
    rules.push_back({newNonTerminal, newRules});
}

int main(int argc, char *argv[]) {
    cout << "\u03B5";
    return 0;
}