#pragma once
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

using namespace std;

void wordProc(ofstream& fout, int lasttype);
void rebuildBuffer();
void initBuffer();
void lexcial();

void printErr(ofstream& fout);
void initTypeCode();
int getValueFromKey(struct typeCode* head, const char* key, int keySize, int defVal);
int getValueFromSep(struct typeCode* head, const char* key, int keySize, int defVal);
void addElement(struct typeCode* head, const char* key, int value);
void printTypeCode(struct typeCode* head);
int isSingleSep(char cTmp);
int isDoubleSep(const char* tmp);
vector<typeValue> domain();