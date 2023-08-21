#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "datetype.h"
#include "opertype.h"
#include "wordtype.h"

using namespace std;

static unordered_map<string, int> mp;
static unordered_multimap<string, vector<string> > sentences;

unordered_map<string, set<int> > getFirst();
void getSentence(string);
void getPreserved();
void traceback(unordered_multimap<string, vector<string> >&, const string&);
