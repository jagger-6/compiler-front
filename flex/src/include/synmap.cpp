#include "synmap.hpp"

map<string, int> valuemap;

// 使用字符分割
void stringsplit(const string &str, const char split, vector<string> &res) {
    if (str == "")
        return;
    // 在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str + split;
    size_t pos = strs.find(split);

    // 若找不到内容则字符串搜索函数返回 npos
    while (pos != strs.npos) {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        // 去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
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
            /*cout << tmp.front() << endl;
            cout << tmp.back() << endl;*/
            valuemap.insert({tmp.front(), stoi(tmp.back())});
            // cout << str << endl;
        }
    }
    /*for (map<string, int>::iterator it = valuemap.begin(); it != valuemap.end(); ++it) {
        cout << it->first << " => " << it->second << endl;
    }*/
    fin.close();
    fin.clear();
}
