#include "synmap.hpp"

map<string, int> valuemap;

void syntable_read(const char *path) {
    fstream fin;
    string str;
    fin.open(path, ios_base::in);
    if (!fin.is_open()) {
        cerr << "Failed to open " << path << endl;
    } else {
        while (getline(fin, str))
            cout << str << endl;
    }
    fin.close();
    fin.clear();
}
