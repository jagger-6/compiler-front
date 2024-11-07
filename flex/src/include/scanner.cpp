#include "scanner.hpp"
#include <cassert>

extern map<string, int> valuemap;

map<string, int> symbolmap;

string scanner(string program) {
    string str = "";
    string token = "";
    int pos = -1;
    char tmp = program[0];
    while (tmp != '\0') {
        token = "";
        tmp = program[++pos];
        while (tmp != '\0' && tmp == ' ') {
            pos++;
            tmp = program[pos];
        }
        switch (tmp) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            while (isdigit(tmp) || isalpha(tmp)) {
                token += tmp;
                tmp = program[++pos];
            }
            pos--;
            if (isreserve(token)) {
                str += "(" + to_string(valuemap[token]) + "," + token + ")" + "\n";
            } else {
                symbolmap.insert({token, valuemap["id"]});
                str += "(" + to_string(valuemap["id"]) + "," + token + ")" + "\n";
            }
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            while (isdigit(tmp)) {
                token += tmp;
                tmp = program[++pos];
            }
            pos--;
            symbolmap.insert({token, valuemap["NUM"]});
            str += "(" + to_string(valuemap["NUM"]) + "," + token + ")" + "\n";
            break;

        case '+':
            str += "(" + to_string(valuemap["+"]) + "," + "+" + ")" + "\n";
            break;

        case '-':
            str += "(" + to_string(valuemap["-"]) + "," + "-" + ")" + "\n";
            break;

        case '*':
            str += "(" + to_string(valuemap["*"]) + "," + "*" + ")" + "\n";
            break;

        case '/':
            str += "(" + to_string(valuemap["/"]) + "," + "/" + ")" + "\n";
            break;

        case ',':
            str += "(" + to_string(valuemap[","]) + "," + "," + ")" + "\n";
            break;

        case ':':
            str += "(" + to_string(valuemap[":"]) + "," + ":" + ")" + "\n";
            break;

        case ';':
            str += "(" + to_string(valuemap[";"]) + "," + ";" + ")" + "\n";
            break;

        case '>':
            pos++;
            if (program[pos] == '=') {
                str += "(" + to_string(valuemap[">="]) + "," + ">=" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap[">"]) + "," + ">" + ")" + "\n";
            }
            break;

        case '<':
            pos++;
            if (program[pos] == '=') {
                str += "(" + to_string(valuemap["<="]) + "," + "<=" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap["<"]) + "," + "<" + ")" + "\n";
            }
            break;

        case '=':
            pos++;
            if (program[pos] == '=') {
                str += "(" + to_string(valuemap["=="]) + "," + "==" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap["="]) + "," + "=" + ")" + "\n";
            }
            break;

        default:
            if (tmp == '\0')
                break;
            assert(false);
        }
    }
    return str;
}

bool isreserve(string token) {
    if (valuemap.count(token) && token != "id")
        return true;
    return false;
}