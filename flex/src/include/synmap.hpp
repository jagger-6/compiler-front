#pragma once

#ifndef __SYN_H
#define __SYN_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void stringsplit(const string &str, const char split, vector<string> &res); // 切割字符串
void syntable_read(const char *path);                                       // 读取种别码文件

#endif