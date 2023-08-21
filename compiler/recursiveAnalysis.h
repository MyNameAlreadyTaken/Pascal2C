#pragma once

#include <map>
#include "lexicalAnalysis.cpp"

static vector<typeValue> vec = domain();
static int i = 0;
static unordered_map<string, set<int> > first = getFirst();
//static unordered_map<string, set<int> > myFirst = getFirst();
static string filename;
static string buf = "";
static bool isMain = true;
static int level = 0;
static bool io = false;
static int tag = 0;

inline void addRroundb() {
    if (buf[buf.size() - 1] == ' ')
        buf.pop_back();
    buf.push_back(')');
}

inline void addSemicolon() {
    if (buf[buf.size() - 1] == ' ')
        buf.pop_back();
    buf += ";\n";
}

inline void printTag() {
    buf += string(tag, '\t');
}

void program();
void proghead();
void block();
void consexpl();
void consdefi();
void varexpl();
void conssuff();
void vardefi();
void varsuff();
void procdefi();
void typeil();
void procedh();
void procsuff();
void assipro();
void sentence();
void suffix();
void ifsent();
void read();
void whilsent();
void idsuff();
void Write();
void compsent();
void Exprsuff();
void sentsuff();
void Conditio();
void termsuff();
void Express();
void term();
void Factsuff();
void argument();
void Factor();
void addoper();
void Muloper();
void respoper();
void match(int i);
void match();
