#define _CRT_SECURE_NO_WARNINGS

#include "lexicalAnalysis.h"

using namespace std;

// int main(int argc, char** argv) {
//     getPreserved();
//     line = 1;
//     row = 0;
//     lexcial();
//     //cout << line << endl;
//     return 0;
// }

void rebuildBuffer() {
    int i = 0;
    for (i = 0; i < readNum; i++)
        buffer[i] = '\0';
    readNum = 0;
    wrong = 0;
    lastType = ISSEPRATE;
}

void initBuffer() {
    int i = 0;
    for (i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = '\0';
    readNum = 0;
}

void lexcial() {
    FILE* fp = fopen("res//test.txt", "r");
    ofstream fout("res//temp.txt");
    readNum = 0;
    lastType = ISSEPRATE;
    if (fp == nullptr) {
        perror("Open file failed");
        return;
    }
    do {
        token = (char)fgetc(fp);
        row += 1;
        if (isalpha(token)) {
            switch (lastType) {
            case ISCHAR:
            case ISSEPRATE:
                if (readNum == 20) {
                    wrong = INVALIDATESTR;
                    printErr(fout);
                }
                buffer[readNum] = token;
                readNum++;
                lastType = ISCHAR;
                break;
            case ISDIGIT:
                if (readNum == 20) {
                    wrong = INVALIDATESTR;
                    printErr(fout);
                }
                buffer[readNum] = token;
                readNum++;
                wrong = INVALIDATESTR;
                lastType = ISCHAR;
                break;
            case ISDELIMITER:
                wordProc(fout, lastType);
                rebuildBuffer();
                buffer[readNum] = token;
                readNum++;
                lastType = ISCHAR;
                break;
            case ISFLLOWT:
                if (readNum == 20) {
                    wrong = INVALIDATESTR;
                    printErr(fout);
                }
                buffer[readNum] = token;
                readNum++;
                wrong = INVALIDATESTR;
                lastType = ISCHAR;
                break;
            default:
                wrong = UNKNOWWRONG;
                lastType = ISCHAR;
            }
        }
        else if (isdigit(token)) {
            switch (lastType) {
            case ISCHAR:
                if (readNum == 20) {
                    wrong = INVALIDATESTR;
                    printErr(fout);
                }
                buffer[readNum] = token;
                readNum++;
                break;
            case ISSEPRATE:
            case ISDIGIT:
                if (readNum == 20) {
                    wrong = INVALIDATESTR;
                    printErr(fout);
                }
                buffer[readNum] = token;
                readNum++;
                lastType = ISDIGIT;
                break;
            case ISFLLOWT:
                if (readNum == 20) {
                    wrong = INVALIDATESTR;
                    printErr(fout);
                }
                buffer[readNum] = token;
                readNum++;
                break;
            case ISDELIMITER:
                wordProc(fout, lastType);
                rebuildBuffer();
                buffer[readNum] = token;
                readNum++;
                lastType = ISDIGIT;
                break;
            default:
                wrong = UNKNOWWRONG;
                lastType = ISCHAR;
            }
        }
        else if (isspace(token)) {
            wordProc(fout, lastType);
            if (token == '\n') {
                line += 1;
                row = 0;
            }
            lastType = ISSEPRATE;
        }
        else if (token == '.') {
            switch (lastType) {
            case ISCHAR:
                wordProc(fout, lastType);
                rebuildBuffer();
                buffer[readNum] = '.';
                readNum++;
                lastType = ISDELIMITER;
                break;
                /*buffer[readNum] = '.';
                readNum++;
                break;*/
            case ISSEPRATE:
            case ISDIGIT:
                buffer[readNum] = '.';
                readNum++;
                lastType = ISFLLOWT;
                break;
            case ISDELIMITER:
                wordProc(fout, lastType);
                rebuildBuffer();
                buffer[readNum] = '.';
                readNum++;
                lastType = ISFLLOWT;
                break;
            case ISFLLOWT:
                buffer[readNum] = '.';
                readNum++;
                wrong = INVALIDATEFLLOWT;
                lastType = ISCHAR;
                break;
            default:
                wrong = UNKNOWWRONG;
                lastType = ISCHAR;
            }
        }
        else {
            switch (lastType) {
            case ISCHAR:
            case ISDIGIT:
            case ISFLLOWT:
            case ISSEPRATE:
                wordProc(fout, lastType);
                rebuildBuffer();
                buffer[readNum] = token;
                readNum++;
                lastType = ISDELIMITER;
                if (isSingleSep(token))
                    wordProc(fout, lastType);
                break;
            case ISDELIMITER:
                buffer[readNum] = token;
                readNum++;
                lastType = ISDELIMITER;
                if (readNum == 2) {
                    if (buffer[0] == '/' && buffer[1] == '*') {
                        token = (char)fgetc(fp);
                        if (token == '\n') {
                            line += 1;
                            row = 0;
                        }
                        else
                            row += 1;
                        while (token != EOF) {
                            while (token != EOF && token != '*') {
                                token = (char)fgetc(fp);
                                if (token == '\n') {
                                    line += 1;
                                    row = 0;
                                }
                                else
                                    row += 1;
                            }
                            if (token == EOF) {
                                printf("程序中可能存在未闭合的注释，不能完成词法分析\n");
                                system("pause");
                                exit(0);
                            }
                            token = fgetc(fp);
                            if (token == '\n') {
                                line += 1;
                                row = 0;
                            }
                            else
                                row += 1;
                            if (token == '/')
                                break;
                        }
                        rebuildBuffer();
                    }
                    else if (isDoubleSep(buffer)) {
                        wordProc(fout, lastType);
                        rebuildBuffer();
                    }
                }
                if (readNum > DELIMITER_SIZE)
                    wrong = INVALIDATEDELIMITER;
                break;
            default:
                wrong = UNKNOWWRONG;
                lastType = ISCHAR;
            }
        }
    } while (token != EOF);
    wordProc(fout, lastType);
    fclose(fp);
    fout.close();
    return;
}

void wordProc(ofstream &fout, int lasttype) {
    if (readNum > 0) {
        if (wrong) {
            printErr(fout);
            for (int i = 0; i < readNum; i++)
                cout << buffer[i];
            cout << endl;
            for (int i = 0; i < readNum; i++)
                fout << buffer[i];
        }
        else if (lasttype == ISFLLOWT || lasttype == ISDIGIT) {
            typeValue tmp = typeValue();
            tmp.value = NUM;
            tmp.line = line;
            fout << "(" << NUM << ", ";
            string s = "";
            for (int i = 0; i < readNum; i++) {
                s += buffer[i];
                fout << buffer[i];
            }
            const char* chr = s.c_str();
            tmp.key = (char*)malloc(readNum * sizeof(char));
            strcpy(tmp.key, chr);
            fout << ")\n";
            vecOfType.push_back(tmp);
        }
        else if (lasttype == ISCHAR) {
            int temp = getValueFromKey(buffer, readNum, STR);
            typeValue tmp = typeValue();
            tmp.value = temp;
            tmp.line = line;
            fout << "(" << temp << ", ";
            string s = "";
            for (int i = 0; i < readNum; i++) {
                s += buffer[i];
                fout << buffer[i];
            }
            const char* chr = s.c_str();
            tmp.key = (char*)malloc(readNum * sizeof(char));
            strcpy(tmp.key, chr);
            fout << ")\n";
            vecOfType.push_back(tmp);
        }
        else if (lasttype == ISDELIMITER) {
            int temp = getValueFromSep(buffer, 1, -1);
            if (temp != -1) {
                typeValue tmp = typeValue();
                tmp.value = temp;
                tmp.line = line;
                fout << "(" << temp << ", ";
                string s = "";
                for (int i = 0; i < readNum; i++) {
                    s += buffer[i];
                    fout << buffer[i];
                }
                const char* chr = s.c_str();
                tmp.key = (char*)malloc(readNum * sizeof(char));
                strcpy(tmp.key, chr);
                fout << ")\n";
                vecOfType.push_back(tmp);
            }
        }
        else {
            cout << "(unknow, ";
            fout << "(unknow, ";
            for (int i = 0; i < readNum; i++) {
                cout << buffer[i];
                fout << buffer[i];
            }
            cout << ")\n";
            fout << ")\n";
        }
        rebuildBuffer();
    }
    return;
}

void printErr(ofstream &fout) {
    switch (wrong) {
    case UNKNOWWRONG:
        cout << "未知错误 in (" << line << ", " << row - 1 << ") ";
        fout << "未知错误 in (" << line << ", " << row - 1 << ") ";
        //printf("未知错误: line " + row + ' ');
        break;
    case INVALIDATESTR:
        cout << "非法字符串 in (" << line << ", " << row - 1 << ") ";
        fout << "非法字符串 in (" << line << ", " << row - 1 << ") ";
        break;
    case INVALIDATEFLLOWT:
        cout << "非法浮点数 in (" << line << ", " << row - 1 << ") ";
        fout << "非法浮点数 in (" << line << ", " << row - 1 << ") ";
        //printf("非法浮点数: line " + row + ' ');
        break;
    case INVALIDATEDELIMITER:
        cout << "非法分界符使用 in (" << line << ", " << row - 1 << ") ";
        fout << "非法分界符使用 in (" << line << ", " << row - 1 << ") ";
        //printf("非法分界符使用: line " + row + ' ');
        break;
    default:
        cout << "系统内部错误";
        fout << "系统内部错误";
        //printf("系统内部错误");
    }
    return;
}

int getValueFromKey(const char* key, int keySize, int defVal) {
    //struct typeCode* tmp;
    int i = 0;
    char* cTmp = new char[keySize];
    //tmp = head;
    for (i = 0; i < keySize; i++) {
        if (key[i] >= 97 && key[i] <= 122)
            cTmp[i] = key[i] - 32;
        else
            cTmp[i] = key[i];
    }
    cTmp[i] = '\0';
    if (mp.find(cTmp) == mp.end())
        return defVal;
    else
        return mp[cTmp];
}

int getValueFromSep(const char* key, int keySize, int defVal) {
    if (mp.find(key) == mp.end())
        return defVal;
    else
        return mp[key];
}

void printTypeCode() {
    for (unordered_map<string, int>::iterator it = mp.begin(); it != mp.end(); it++)
        cout << it->first << "--" << it->second << endl;
    return;
}

bool isSingleSep(const char cTmp) {
    if (cTmp == '+' || cTmp == '-' || cTmp == '*' || cTmp == ';' || cTmp == '(' || cTmp == ')' || cTmp == '[' || cTmp == ']' || cTmp == '{' || cTmp == '}')
        return true;
    else
        return false;
}

bool isDoubleSep(const char* cTmp) {
    if (strcmp(cTmp, "<>") || strcmp(cTmp, "<=") || strcmp(cTmp, ">=") || strcmp(cTmp, "==") || strcmp(cTmp, ":="))
        return true;
    else
        return false;
}

vector<typeValue> domain() {
    getPreserved();
    line = 1;
    row = 0;
    lexcial();
    return vecOfType;
}
