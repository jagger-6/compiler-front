#include "scanner.hpp"
#include <cassert>
#include <regex>

extern map<string, int> valuemap; // 种别码表

map<string, int> symbolmap; // 符号表
int line = 1;               // 行号，用于定位此法错误位置

string scanner(string program) {
    string str = ""; // 输出结果
    string token = "";
    int pos = -1; // 当前字符串索引
    char tmp = program[0];
    bool isnotes = false;
    while (tmp != '\0' && tmp != EOF) { // 读到文件末尾
        token = "";
        tmp = program[++pos];
        while (tmp != '\0' && (tmp == ' ' || tmp == '\n')) { // 遇到字符串结尾、空格、换行符则跳过
            linenum(tmp);
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
            while (isidentifier(tmp)) { // 标识符处理
                token += tmp;
                tmp = program[++pos];
            }
            pos--;
            if (isreserve(token)) { // 关键字
                str += "(" + to_string(valuemap[token]) + "," + token + ")" + "\n";
            } else {
                symbolmap.insert({token, valuemap["id"]}); // 加入到符号表
                str += "(" + to_string(valuemap["id"]) + "," + token + ")" + "\n";
            }
            break;

        case '0':
            if (ishexadecimal(program, pos)) { // 十六进制处理
                token = "0x";
                char temp = program[++pos];
                while (isdigit(temp) || ischarinrange(temp)) {
                    token += temp;
                    temp = program[++pos];
                }
                if (isalpha(temp) && !ischarinrange(temp)) { // 合法十六进制判断
                    cerr << "line:" << line << " " << token + temp << " is invalid " << endl;
                    assert(false);
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
            if (isalpha(program[pos + 1])) { // 非法标识符处理（以数字开头，如1z）
                string errn = to_string(tmp - '0');
                errn += program[pos + 1];
                cerr << "line:" << line << " " << errn << " is invalid " << endl;
                assert(false);
            }

            if ((token = doubletoken(program, pos)) != "") { // 双精度常量处理
                symbolmap.insert({token, valuemap["NUM"]});
                str += "(" + to_string(valuemap["NUM"]) + "," + token + ")" + "\n";
                break;
            }
            while (isdigit(tmp)) { // 普通数
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
            isnotes = iscommends(program, pos); // 注释
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
            if (program[pos] == '=') { //>=
                str += "(" + to_string(valuemap[">="]) + "," + ">=" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap[">"]) + "," + ">" + ")" + "\n";
            }
            break;

        case '<':
            pos++;
            if (program[pos] == '=') { //<=
                str += "(" + to_string(valuemap["<="]) + "," + "<=" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap["<"]) + "," + "<" + ")" + "\n";
            }
            break;

        case '=':
            pos++;
            if (program[pos] == '=') { //==
                str += "(" + to_string(valuemap["=="]) + "," + "==" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap["="]) + "," + "=" + ")" + "\n";
            }
            break;

        case '!':
            pos++;
            if (program[pos] == '=') { //!=
                str += "(" + to_string(valuemap["!="]) + "," + "!=" + ")" + "\n";
            } else {
                pos--;
                str += "(" + to_string(valuemap["!"]) + "," + "!" + ")" + "\n";
            }
            break;

        case '"':
            pos++;
            token = stringtoken(program, pos); // 字符串常量处理
            symbolmap.insert({token, valuemap["STRING"]});
            str += "(" + to_string(valuemap["STRING"]) + "," + token + ")" + "\n";
            break;

        default:
            if (tmp == '\0' || tmp == EOF)
                break;
            // 非法字符处理
            cerr << "line:" << line << " invalid token:" << tmp << endl;
            assert(false);
        }
    }
    return str;
}

bool isreserve(string token) {
    if (valuemap.count(token) && token != "id") // 排除id为正常标识符却被识别成关键字的情况
        return true;
    return false;
}

bool iscommends(string program, int &pos) {
    bool isnotes = false;
    int backup = pos; // 备份当前索引位置
    int Line = line;
    pos++;
    if (program[pos] == '/') { // 行注释
        isnotes = true;
        pos++;
        while (program[pos] != '\n' && program[pos] != '\0') {
            pos++;
        }
        linenum(program[pos]);
    } else if (program[pos] == '*') { // 块注释
        while ((program[pos] != '*' || program[pos + 1] != '/') && program[pos] != '\0') {
            linenum(program[pos]);
            pos++;
        }
        if (program[pos] == '*' && program[pos + 1] == '/') {
            isnotes = true;
            pos++;
        } else {
            pos = backup;
            cerr << "line:" << Line << " not a completed commends" << endl;
            assert(false);
        }
    } else {
        pos = backup;
    }
    return isnotes;
}

bool ishexadecimal(string program, int &pos) {
    pos++;
    if (program[pos] == 'x' || program[pos] == 'X')
        return true;
    return false;
}

bool ischarinrange(char ch) {
    int asc = int(ch);
    return (asc >= 'a' && asc <= 'f') || (asc >= 'A' && asc <= 'F');
}

bool isidentifier(char tmp) {
    // 以字母开头，数字、字母以及下划线组成的标识符
    return (isdigit(tmp) || isalpha(tmp) || tmp == '_');
}

string doubletoken(string program, int &pos) {
    string token;
    int backup = pos;
    // 先读取满足双精度常量的字符串（有数字、.、E、e、+、-即可）
    while ((isdigit(program[pos])) || program[pos] == '.' || program[pos] == 'e' || program[pos] == 'E' || program[pos] == '+' || program[pos] == '-') {
        token += program[pos];
        pos++;
    }
    linenum(program[pos]);
    std::regex e("^[-+]?[0-9]+[.]([0-9]+)?([eE][-+]?[0-9]+)?$"); // 正则表达式判断是否为合法的双精度常量
    std::smatch match;

    if (std::regex_search(token, match, e)) {
        // 输出匹配的子串和分组
        /*for (size_t i = 0; i < match.size(); ++i) {
            std::cout << "Match " << i << ": " << match[i] << '\n';
        }*/
        pos = backup + match[0].length();
    } else {
        pos = backup;
        return "";
    }
    return token;
}

string stringtoken(string program, int &pos) {
    string token;
    int backup = pos;
    int Line = line;
    while (program[pos] != '\n' && program[pos] != '"' && program[pos] != '\0') { // 遇到这些字符时停止
        token += program[pos];
        pos++;
    }
    linenum(program[pos]);
    if (program[pos] == '"') {
        return token;
    } else {
        pos = backup;
        cerr << "line:" << Line << " " << token << "is not a completed string" << endl;
        assert(false);
        return "";
    }
}

void linenum(char temp) {
    // 遇到换行符，行号自增
    if (temp == '\n')
        line++;
}