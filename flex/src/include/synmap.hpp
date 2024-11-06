#pragma once

#ifndef __SYN_H
#define __SYN_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void stringsplit(const string &str, const char split, vector<string> &res);
void syntable_read(const char *path);

#endif