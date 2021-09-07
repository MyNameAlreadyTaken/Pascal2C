#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "lexicalAnalysis.h"

using namespace std;

static vector<typeValue> vec = domain();
static int i = 0;
static map<string, set<int>> first;

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


void program() {  //程序
	proghead();
	block();
	match(DOT);
}

void proghead() { //程序首部
	match(PROGRAM);
	match(STR);
	match(SEMICOLON);
}

void block() { //分程序
	consexpl();
	varexpl();
	procdefi();
	compsent();
}

void consexpl() { //常量说明部分
	if (vec[i].value == CONST) {
		match(CONST);
		consdefi();
		conssuff();
		match(SEMICOLON);
	}
	else
		match();
}

void consdefi() { //常量定义
	match(STR);
	match(ASSIGN);
	match(NUM);
}

void varexpl() { //变量说明部分
	if (vec[i].value == VAR) {
		match(VAR);
		vardefi();
		varsuff();
	}
	else
		match();
}

void conssuff() { //常量定义后缀
	if (vec[i].value == COMMA) {
		match(COMMA);
		consdefi();
		conssuff();
	}
	else
		match();
}

void vardefi() { //变量定义
	match(STR);
	idsuff();
	match(COLON);
	typeil();
	match(SEMICOLON);
}

void varsuff() { //变量定义后缀
	if (first["vardefi"].find(vec[i].value) != first["vardefi"].end()) {
		vardefi();
		varsuff();
	}
	else
		match();
}

void procdefi() { //过程说明部分
	if (first["procedh"].find(vec[i].value) != first["procedh"].end()) {
		procedh();
		block();
		match(SEMICOLON);
		procsuff();
	}
	else
		match();
}

void typeil() { //类型
	if (vec[i].value == INTEGER)
		match(INTEGER);
	else
		match(REAL);
}

void procedh() { //过程首部
	match(PROCEDURE);
	match(STR);
	argument();
	match(SEMICOLON);
}

void procsuff() { //过程说明部分后缀
	if (first["procedh"].find(vec[i].value) != first["procedh"].end()) {
		procedh();
		block();
		match(SEMICOLON);
		procsuff();
	}
	else
		match();
}

void assipro() { //赋值或调用语句
	match(STR);
	match(VAL);
	suffix();
}

void sentence() { //语句
	if (first["assipro"].find(vec[i].value) != first["assipro"].end())
		assipro();
	else if (first["ifsent"].find(vec[i].value) != first["ifsent"].end())
		ifsent();
	else if (first["whilsent"].find(vec[i].value) != first["whilsent"].end())
		whilsent();
	else if (first["read"].find(vec[i].value) != first["read"].end())
		read();
	else if (first["Write"].find(vec[i].value) != first["Write"].end())
		Write();
	else if (first["compsent"].find(vec[i].value) != first["compsent"].end())
		compsent();
	else
		match();
}

void suffix() { //后缀
	if (vec[i].value == LROUNDB) {
		match(LROUNDB);
		Express();
		match(RROUNDB);
	}
	else
		Express();
}

void ifsent() { //条件语句
	match(IF);
	Conditio();
	match(THEN);
	sentence();
}

void read() { //读语句
	match(READ);
	match(LROUNDB);
	match(STR);
	idsuff();
	match(RROUNDB);
}

void whilsent() { //当型循环语句
	match(WHILE);
	Conditio();
	match(DO);
	sentence();
}

void idsuff() { //标识符后缀
	if (vec[i].value == COMMA) {
		match(COMMA);
		match(STR);
		idsuff();
	}
	else
		match();
}

void Write() { // 写语句
	match(WRITE);
	match(LROUNDB);
	Express();
	Exprsuff();
	match(RROUNDB);
}

void compsent() { //复合语句
	match(BEGIN);
	sentence();
	sentsuff();
	match(END);
}

void Exprsuff() { //表达式后缀
	if (vec[i].value == COMMA) {
		match(COMMA);
		Express();
		Exprsuff();
	}
	else
		match();
}

void sentsuff() { //语句后缀
	if (vec[i].value == SEMICOLON) {
		match(SEMICOLON);
		sentence();
		sentsuff();
	}
	else
		match();
}

void Conditio() { //条件
	if (vec[i].value == ODD) {
		match(ODD);
		Express();
	}
	else {
		Express();
		respoper();
		Express();
	}
}

void termsuff() { //项后缀
	if (first["addoper"].find(vec[i].value) != first["addoper"].end()) {
		addoper();
		term();
		termsuff();
	}
	else
		match();
}

void Express() { //表达式
	if (vec[i].value == ADD) {
		match(ADD);
		term();
		termsuff();
	}
	else if (vec[i].value == SUB) {
		match(SUB);
		term();
		termsuff();
	}
	else {
		term();
		termsuff();
	}
}

void term() { //项
	Factor();
	Factsuff();
}

void Factsuff() { //因子后缀
	if (first["Muloper"].find(vec[i].value) != first["Muloper"].end()) {
		Muloper();
		Factor();
		Factsuff();
	}
	else
		match();
}

void argument() { //参数部分
	match(LROUNDB);
	match(STR);
	match(COLON);
	typeil();
	match(RROUNDB);
}

void Factor() { //因子
	if (vec[i].value == STR)
		match(STR);
	else if (vec[i].value == NUM)
		match(NUM);
	else {
		match(LROUNDB);
		Express();
		match(RROUNDB);
	}
}

void addoper() { //加型运算符
	if (vec[i].value == ADD)
		match(ADD);
	else
		match(SUB);
}

void Muloper() { //乘型运算符
	if (vec[i].value == MUL)
		match(MUL);
	else
		match(DEV);
}

void respoper() { //关系运算符
	if (vec[i].value == ASSIGN)
		match(ASSIGN);
	else if (vec[i].value == NE)
		match(NE);
	else if (vec[i].value == LT)
		match(LT);
	else if (vec[i].value == LE)
		match(LE);
	else if (vec[i].value == GT)
		match(GT);
	else
		match(GE);
}

void match(int n) {
	if (i < vec.size()) {
		if (vec[i].value == n) {
			i += 1;
			return;
		}
		else {
			cout << "该程序不符合PASCAL文法。" << endl;
			if (i == 0)
				cout << "第1行出现错误。" << endl;
			else
				cout << "第" << vec[i - 1].line << "行出现错误。" << endl;
			exit(1);
		}
	}
	else {
		cout << "该程序不符合PASCAL文法。" << endl;
		cout << "第" << vec[vec.size() - 1].line << "行出现错误。" << endl;
		exit(1);
	}
}

void match() {
	return;
}

void setFirst() {
	first["program"] = set<int>({ PROGRAM });
	first["proghead"] = set<int>({ PROGRAM });
	first["block"] = set<int>({ CONST, PROCEDURE, BEGIN });
	first["consexpl"] = set<int>({ CONST, 0 });
	first["consdefi"] = set<int>({ STR });
	first["varexpl"] = set<int>({ VAR, 0 });
	first["conssuff"] = set<int>({ COMMA, 0 });
	first["vardefi"] = set<int>({ STR });
	first["varsuff"] = set<int>({ STR });
	first["procdefi"] = set<int>({ PROCEDURE, 0 });
	first["typeil"] = set<int>({ INTEGER, REAL });
	first["procedh"] = set<int>({ PROCEDURE });
	first["procsuff"] = set<int>({ PROCEDURE, 0 });
	first["assipro"] = set<int>({ STR });
	first["sentence"] = set<int>({ STR, IF, WHILE, READ, WRITE, BEGIN, 0 });
	first["suffix"] = set<int>({ ADD, SUB, STR, NUM, LROUNDB });
	first["ifsent"] = set<int>({ IF });
	first["read"] = set<int>({ READ });
	first["whilsent"] = set<int>({ WHILE });
	first["idsuff"] = set<int>({ COMMA, 0 });
	first["Write"] = set<int>({ WRITE });
	first["compsent"] = set<int>({ BEGIN });
	first["Exprsuff"] = set<int>({ COMMA, 0 });
	first["sentsuff"] = set<int>({ SEMICOLON, 0 });
	first["Conditio"] = set<int>({ ADD, SUB, STR, NUM, LROUNDB, ODD });
	first["termsuff"] = set<int>({ ADD, SUB, 0 });
	first["Express"] = set<int>({ ADD, SUB, STR, NUM, LROUNDB });
	first["term"] = set<int>({ STR, NUM, LROUNDB });
	first["Factsuff"] = set<int>({ MUL, DEV, 0 });
	first["argument"] = set<int>({ LROUNDB });
	first["Factor"] = set<int>({ STR, NUM, LROUNDB });
	first["addoper"] = set<int>({ ADD, SUB });
	first["Muloper"] = set<int>({ MUL, DEV });
	first["respoper"] = set<int>({ ASSIGN, NE, LT, LE, GT, GE });
	return;
}

int main() {
	setFirst();
	program();
	cout << "该程序符合PASCAL文法。" << endl;
	return 0;
}
