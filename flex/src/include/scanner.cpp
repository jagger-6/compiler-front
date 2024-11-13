#include "scanner.hpp"
#include <cassert>

extern map<string, int> valuemap;

map<string, int> symbolmap;

string scanner(string program) {
    string str = "";
    string token = "";
    int pos = -1;
    char tmp = program[0];
    bool isnotes = false;
    while (tmp != '\0') {
        token = "";
        tmp = program[++pos];
        while (tmp != '\0' && (tmp == ' ' || tmp == '\n')) {
            pos++;
            tmp = program[pos];
        }
        switch (tmp) {
        case 'a':
        case 'A':
        case 'b':
        case 'B':
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'h':
        case 'H':
        case 'i':
        case 'I':
        case 'j':
        case 'J':
        case 'k':
        case 'K':
        case 'l':
        case 'L':
        case 'm':
        case 'M':
        case 'n':
        case 'N':
        case 'o':
        case 'O':
        case 'p':
        case 'P':
        case 'q':
        case 'Q':
        case 'r':
        case 'R':
        case 's':
        case 'S':
        case 't':
        case 'T':
        case 'u':
        case 'U':
        case 'v':
        case 'V':
        case 'w':
        case 'W':
        case 'x':
        case 'X':
        case 'y':
        case 'Y':
        case 'z':
        case 'Z':
            while (isdigit(tmp) || isalpha(tmp) || tmp == '_') {
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
            if (ishexadecimal(program, pos)) {
                token = "0x";
                char temp = program[++pos];
                while (isdigit(temp)) {
                    token += temp;
                    temp = program[++pos];
                }
                pos--;
                symbolmap.insert({token, valuemap["NUM"]});
                str += "(" + to_string(valuemap["NUM"]) + "," + token + ")" + "\n";
                break;
            } else {
                pos--;
            }
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
            isnotes = iscommends(program, pos);
            if (isnotes == false)
                str += "(" + to_string(valuemap["/"]) + "," + "/" + ")" + "\n";
            break;

        case '(':
            str += "(" + to_string(valuemap["("]) + "," + "(" + ")" + "\n";
            break;

        case ')':
            str += "(" + to_string(valuemap[")"]) + "," + ")" + ")" + "\n";
            break;

        case '[':
            str += "(" + to_string(valuemap["["]) + "," + "[" + ")" + "\n";
            break;

        case ']':
            str += "(" + to_string(valuemap["]"]) + "," + "]" + ")" + "\n";
            break;

        case '{':
            str += "(" + to_string(valuemap["{"]) + "," + "{" + ")" + "\n";
            break;

        case '}':
            str += "(" + to_string(valuemap["}"]) + "," + "}" + ")" + "\n";
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
            cout << tmp;
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

bool iscommends(string program, int &pos) {
    bool isnotes = false;
    if (program[++pos] == '/') {
        isnotes = true;
        pos++;
        while (program[pos] != '\n' && program[pos] != '\0') {
            pos++;
        }
    } else {
        pos--;
    }
    return isnotes;
}

bool ishexadecimal(string program, int &pos) {
    pos++;
    if (program[pos] == 'x' || program[pos] == 'X')
        return true;
    return false;
}