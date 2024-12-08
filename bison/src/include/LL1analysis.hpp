#pragma once

#ifndef __ANY_H
#define __ANY_H

#include "FirstFollow.hpp"
#include <unordered_set>

bool insertIfNew(unordered_set<string> &set, const string &item);
void buildParseTable(const vector<node> &rules, const unordered_map<string, set<string>> &first, const unordered_map<string, set<string>> &follow,
                     unordered_map<string, unordered_map<string, string>> &table, const vector<string> &nonTerminals);
void printParseTable(const unordered_map<string, unordered_map<string, string>> &table);

#endif