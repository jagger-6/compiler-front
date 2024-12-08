#pragma once

#ifndef __ANY_H
#define __ANY_H

#include "FirstFollow.hpp"
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

// 分析栈模拟函数
void parseString(const unordered_map<string, unordered_map<string, string>> &table, const string &input);

#endif