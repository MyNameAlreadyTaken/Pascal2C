#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <malloc.h>
#include <windows.h>
#include <vector>
#include "datetype.h"
#include "opertype.h"
#include "wordtype.h"
#include "lexicalAnalysis.h"

using namespace std;

//定义一些全局变量
static char buffer[BUFFER_SIZE];
static int readNum;
static int lastType;
static int lastWord;
static char token;
static int wrong;
static int line;
static int row;
static typeCode* keyHead, * sepHead;
static vector<typeValue> vecOfType;

//int main(int argc, char argv[])
//{
//    line = 1;
//    row = 0;
//    initTypeCode();
//    //printTypeCode(keyHead);
//    //printTypeCode(sepHead);
//    lexcial();
//    delete keyHead;
//    delete sepHead;
//    //cout << line << endl;
//}

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
    if (fp == NULL) {
        perror("Open file failed");
        system("pause");
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
            /*cout << "(";
            cout << NUM;
            cout << ", ";*/
            fout << "(";
            fout << NUM;
            fout << ", ";
            string s = "";
            for (int i = 0; i < readNum; i++) {
                s += buffer[i];
                //cout << buffer[i];
                fout << buffer[i];
            }
            const char* chr = s.c_str();
            tmp.key = (char*)malloc(readNum * sizeof(char));
            strcpy(tmp.key, chr);
            //cout << ")\n";
            fout << ")\n";
            vecOfType.push_back(tmp);
        }
        else if (lasttype == ISCHAR) {
            int temp = getValueFromKey(keyHead, buffer, readNum, STR);
            typeValue tmp = typeValue();
            tmp.value = temp;
            tmp.line = line;
            /*cout << "(";
            cout << temp;
            cout << ", ";*/
            fout << "(";
            fout << temp;
            fout << ", ";
            string s = "";
            for (int i = 0; i < readNum; i++) {
                s += buffer[i];
                //cout << buffer[i];
                fout << buffer[i];
            }
            const char* chr = s.c_str();
            tmp.key = (char*)malloc(readNum * sizeof(char));
            strcpy(tmp.key, chr);
            //cout << ")\n";
            fout << ")\n";
            vecOfType.push_back(tmp);
        }
        else if (lasttype == ISDELIMITER) {
            int temp = getValueFromSep(sepHead, buffer, 1, -1);
            if (temp != -1) {
                typeValue tmp = typeValue();
                tmp.value = temp;
                tmp.line = line;
                /*cout << "(";
                cout << temp;
                cout << ", ";*/
                fout << "(";
                fout << temp;
                fout << ", ";
                string s = "";
                for (int i = 0; i < readNum; i++) {
                    s += buffer[i];
                    //cout << buffer[i];
                    fout << buffer[i];
                }
                const char* chr = s.c_str();
                tmp.key = (char*)malloc(readNum * sizeof(char));
                strcpy(tmp.key, chr);
                //cout << ")\n";
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
        cout << "未知错误 in (";
        cout << line;
        cout << ", ";
        cout << row - 1;
        cout << ") ";
        fout << "未知错误 in (";
        fout << line;
        fout << ", ";
        fout << row - 1;
        fout << ") ";
        //printf("未知错误: line " + row + ' ');
        break;
    case INVALIDATESTR:
        cout << "非法字符串 in (";
        cout << line;
        cout << ", ";
        cout << row - 1;
        cout << ") ";
        fout << "非法字符串 in (";
        fout << line;
        fout << ", ";
        fout << row - 1;
        fout << ") ";
        break;
    case INVALIDATEFLLOWT:
        cout << "非法浮点数 in (";
        cout << line;
        cout << ", ";
        cout << row - 1;
        cout << ") ";
        fout << "非法浮点数 in (";
        fout << line;
        fout << ", ";
        fout << row - 1;
        fout << ") ";
        //printf("非法浮点数: line " + row + ' ');
        break;
    case INVALIDATEDELIMITER:
        cout << "非法分界符使用 in (";
        cout << line;
        cout << ", ";
        cout << row - 1;
        cout << ") ";
        fout << "非法分界符使用 in (";
        fout << line;
        fout << ", ";
        fout << row - 1;
        fout << ") ";
        //printf("非法分界符使用: line " + row + ' ');
        break;
    default:
        cout << "系统内部错误";
        fout << "系统内部错误";
        //printf("系统内部错误");
    }
}

void initTypeCode() {
    keyHead = new typeCode();
    sepHead = new typeCode();

    addElement(keyHead, "PROGRAM", PROGRAM);
    addElement(keyHead, "CONST", CONST);
    addElement(keyHead, "VAR", VAR);
    addElement(keyHead, "INTEGER", INTEGER);
    addElement(keyHead, "LONG", LONG);
    addElement(keyHead, "PROCEDURE", PROCEDURE);
    addElement(keyHead, "IF", IF);
    addElement(keyHead, "THEN", THEN);
    addElement(keyHead, "WHILE", WHILE);
    addElement(keyHead, "DO", DO);
    addElement(keyHead, "READ", READ);
    addElement(keyHead, "WRITE", WRITE);
    addElement(keyHead, "BEGIN", BEGIN);
    addElement(keyHead, "END", END);
    addElement(keyHead, "ODD", ODD);
    addElement(keyHead, "REAL", REAL);

    addElement(sepHead, "=", ASSIGN);
    addElement(sepHead, ":=", VAL);
    addElement(sepHead, ">=", GE);
    addElement(sepHead, "<=", LE);
    addElement(sepHead, "<>", NE);

    addElement(sepHead, "<", LT);
    addElement(sepHead, ">", GT);
    addElement(sepHead, "(", LROUNDB);
    addElement(sepHead, ")", RROUNDB);
    addElement(sepHead, "+", ADD);
    addElement(sepHead, "-", SUB);
    addElement(sepHead, "*", MUL);
    addElement(sepHead, "/", DEV);
    addElement(sepHead, ".", DOT);
    addElement(sepHead, ",", COMMA);
    addElement(sepHead, ":", COLON);
    addElement(sepHead, ";", SEMICOLON);
    addElement(sepHead, "#", WELL);
}

void addElement(struct typeCode* head, const char* key, int value) {
    typeCode* tmp = new typeCode();
    tmp->key = new char[20];
    strcpy(tmp->key, key);
    tmp->value = value;
    tmp->next = head->next;
    head->next = tmp;
    return;
}

int getValueFromKey(struct typeCode* head, const char* key, int keySize, int defVal) {
    struct typeCode* tmp;
    int i = 0;
    char* cTmp = new char[keySize];
    tmp = head;
    for (i = 0; i < keySize; i++) {
        if (key[i] >= 97 && key[i] <= 122)
            cTmp[i] = key[i] - 32;
        else
            cTmp[i] = key[i];
    }
    cTmp[i] = '\0';
    //printf("%s+++++++++++%d,%d\n",cTmp,'A','a');
    while (tmp->next != NULL) {
        tmp = tmp->next;
        if (strcmp(cTmp, tmp->key) == 0)
            return tmp->value;
    }
    return defVal;
}

int getValueFromSep(struct typeCode* head, const char* key, int keySize, int defVal) {
    struct typeCode* tmp;
    tmp = head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
        if (strcmp(key, tmp->key) == 0)
            return tmp->value;
    }
    return defVal;
}

void printTypeCode(struct typeCode* head) {
    struct typeCode* tmp;
    tmp = head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
        printf("%s--%d\n", tmp->key, tmp->value);
    }
    return;
}

int isSingleSep(char cTmp) {
    if (cTmp == '+' || cTmp == '-' || cTmp == '*' || cTmp == ';' || cTmp == '(' || cTmp == ')' || cTmp == '[' || cTmp == ']' || cTmp == '{' || cTmp == '}')
        return 1;
    else
        return 0;
}

int isDoubleSep(const char* cTmp) {
    if (strcmp(cTmp, "<>") || strcmp(cTmp, "<=") || strcmp(cTmp, ">=") || strcmp(cTmp, "==") || strcmp(cTmp, ":="))
        return 1;
    else
        return 0;
}

vector<typeValue> domain() {
    line = 1;
    row = 0;
    initTypeCode();
    lexcial();
    delete keyHead;
    delete sepHead;
    return vecOfType;
}