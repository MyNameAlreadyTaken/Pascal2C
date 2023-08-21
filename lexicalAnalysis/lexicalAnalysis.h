#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/malloc.h>
#include <vector>
#include <unordered_map>
#include "datetype.h"
#include "opertype.h"
#include "wordtype.h"
#include "first.cpp"

using namespace std;

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

void wordProc(ofstream& fout, int lasttype);
void rebuildBuffer();
void initBuffer();
void lexcial();

void printErr(ofstream& fout);
int getValueFromKey(const char* key, int keySize, int defVal);
int getValueFromSep(const char* key, int keySize, int defVal);
void printTypeCode(struct typeCode* head);
bool isSingleSep(const char cTmp);
bool isDoubleSep(const char* tmp);
vector<typeValue> domain();
