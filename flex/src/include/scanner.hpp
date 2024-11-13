#pragma once

#ifndef __SCN_H
#define __SCN_H

#include "synmap.hpp"

string scanner(string program);
bool isreserve(string token);
bool iscommends(string program, int &pos);
bool ishexadecimal(string program, int &pos);

#endif