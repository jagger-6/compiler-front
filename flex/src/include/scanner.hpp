#pragma once

#ifndef __SCN_H
#define __SCN_H

#include "synmap.hpp"

string scanner(string program);               // 扫描器
bool isreserve(string token);                 // 判断是否为关键字
bool iscommends(string program, int &pos);    // 判断是否为注释以及注释处理
bool ishexadecimal(string program, int &pos); // 判断是否为16进制
bool ischarinrange(char ch);                  // 判断字符是否为a到f或A到F
bool isidentifier(char tmp);                  // 判断是否为标识符
string doubletoken(string program, int &pos); // 双精度常量处理
string stringtoken(string program, int &pos); // 字符常量处理
void linenum(char temp);                      // 行号自增

#endif