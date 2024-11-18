#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void stringsplit(const string str, const char split, vector<string> &res);
void syntable_read(const char *path);

map<string, int> valuemap;

// 使用字符分割
void stringsplit(const string str, const char split, vector<string> &res) {
    if (str == "")
        return;
    // 在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str + split;
    size_t pos = strs.find(split);
    int i = 1;

    // 若找不到内容则字符串搜索函数返回 npos
    while (i != 3) {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        // 去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
        i++;
    }
}

void syntable_read(const char *path) {
    fstream fin;
    string str;
    fin.open(path, ios_base::in);
    if (!fin.is_open()) {
        cerr << "Failed to open " << path << endl;
    } else {
        while (getline(fin, str)) {
            vector<string> tmp;
            stringsplit(str, ' ', tmp);
            cout << tmp.front() << endl;
            cout << tmp.back() << endl;
            // cout << tmp.back().length() << endl;
            //  valuemap.insert({tmp.front(), stoi(string(tmp.back()))});
            cout << str << endl;
        }
    }
    /*for (map<string, int>::iterator it = valuemap.begin(); it != valuemap.end(); ++it) {
        cout << it->first << " => " << it->second << endl;
    }*/
    fin.close();
    fin.clear();
}

int main() {
    // syntable_read("./flex/src/include/syntable.txt");
    regex pattern("^[-+]?[0-9]+[.]([0-9]+)?([eE][-+]?[0-9]+)?$");
    std::smatch results;
    string str("12.0e+2");
    if (regex_search(str, results, pattern))
        cout << results[0];
    return 0;
}
