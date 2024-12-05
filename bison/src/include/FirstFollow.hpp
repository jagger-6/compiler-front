#pragma once

#ifndef __FF_H
#define __FF_H

#include "LGrammar.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_map>

bool isNonTerminal(const string &symbol, const vector<string> &nonTerminals);
void computeFirst(const vector<node> &rules, unordered_map<string, set<string>> &first, const vector<string> &nonTerminals);
void computeFollow(const vector<node> &rules, const unordered_map<string, set<string>> &first, unordered_map<string, set<string>> &follow, const string &startSymbol,
                   const vector<string> &nonTerminals);
void printFirst(const unordered_map<string, set<string>> &first);
void printFollow(const unordered_map<string, set<string>> &follow);

#endif