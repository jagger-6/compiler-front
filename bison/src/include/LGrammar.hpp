#pragma once

#ifndef __LG_H
#define __LG_H

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

struct node {
    string left;       // 左部
    set<string> right; // 右部
}; // 产生式

extern vector<node> rules;
extern vector<string> noterminators; // 非终结符
extern string start;                 // 文法开始符

void stringsplit_string(const string &str, const string split, vector<string> &res);   // 切割字符串
void stringsplit_char(const string &str, const char split, set<string> &res);          // 使用字符分割
void grammar_read(const char *path);                                                   // 读取文法文件
void getnoterminators();                                                               // 获取非终结符并排序
void factorization();                                                                  // 提取公共左因子
void eliminatelr();                                                                    // 消除间接左递归
void erasedirect(int posi);                                                            // 消除直接左递归
vector<node> sortRules(const vector<node> &rules, const vector<string> &nonTerminals); // 重新对产生式排序

#endif