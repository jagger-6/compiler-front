#pragma once

#ifndef __SCN_H
#define __SCN_H

#include "synmap.hpp"

string scanner(string program);
bool isreserve(string token);
bool iscommends(string program, int &pos);
bool ishexadecimal(string program, int &pos);
bool ischarinrange(char ch);
bool isidentifier(char tmp);
string doubletoken(string program, int &pos);
string stringtoken(string program, int &pos);
void linenum(char temp);

#endif