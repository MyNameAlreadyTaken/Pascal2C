#define _CRT_SECURE_NO_WARNINGS

#include "recursiveAnalysis.h"

void program() {  //程序
	proghead();
	block();
	match(DOT);
	printTag();
	buf += "return 0;\n}\n";
}

void proghead() { //程序首部
	match(PROGRAM);
	match(STR);
	filename = vec[i - 1].key;
	match(SEMICOLON);
}

void block() { //分程序
	++level;
	consexpl();
	if (buf[buf.size() - 1] != '\n')
		buf.push_back('\n');
	varexpl();
	if (!(buf[buf.size() - 1] == '\n' && buf[buf.size() - 2] == '\n'))
		buf.push_back('\n');
	procdefi();
	if (!(buf[buf.size() - 1] == '\n' && buf[buf.size() - 2] == '\n'))
		buf.push_back('\n');
	if (isMain && level == 1) {
		buf += "int main() {\n";
		isMain = false;
		++tag;
	}
	compsent();
	--level;
}

void consexpl() { //常量说明部分
	if (vec[i].value == CONST) {
		match(CONST);
		printTag();
		buf += "const int ";
		consdefi();
		conssuff();
		match(SEMICOLON);
		addSemicolon();
		// buf += ";\n";
	}
	else
		match();
}

void consdefi() { //常量定义
	match(STR);
	buf += vec[i - 1].key;
	buf.push_back(' ');
	match(ASSIGN);
	buf += "= ";
	match(NUM);
	buf += vec[i - 1].key;
}

void varexpl() { //变量说明部分
	if (vec[i].value == VAR) {
		match(VAR);
		printTag();
		vardefi();
		varsuff();
	}
	else
		match();
}

void conssuff() { //常量定义后缀
	if (vec[i].value == COMMA) {
		match(COMMA);
		buf += ", ";
		consdefi();
		conssuff();
	}
	else
		match();
}

void vardefi() { //变量定义
	printTag();
	buf += "<type> ";
	match(STR);
	buf += vec[i - 1].key;
	idsuff();
	match(COLON);
	typeil();
	string type = vec[i - 1].key;
	if (type == "INTEGER")
		buf.replace(buf.find("<type>"), 6, "int");
	else if (type == "REAL")
		buf.replace(buf.find("<type>"), 6, "short");
	match(SEMICOLON);
	addSemicolon();
	//buf += ";\n";
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
		printTag();
		buf += "return;\n";
		--tag;
		printTag();
		buf += "}\n";
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
	printTag();
	buf += "void ";
	match(STR);
	buf += vec[i - 1].key;
	buf.push_back('(');
	argument();
	match(SEMICOLON);
	buf += "{\n";
	++tag;
}

void procsuff() { //过程说明部分后缀
	if (first["procedh"].find(vec[i].value) != first["procedh"].end()) {
		procedh();
		block();
		match(SEMICOLON);
		printTag();
		buf += "return;\n";
		--tag;
		printTag();
		buf += "}\n";
		procsuff();
	}
	else
		match();
}

void assipro() { //赋值或调用语句
	match(STR);
	printTag();
	buf += vec[i - 1].key;
	buf.push_back(' ');
	match(VAL);
	buf += "= ";
	suffix();
	addSemicolon();
	//buf += ";\n";
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
	else if (first["write"].find(vec[i].value) != first["write"].end())
		Write();
	else if (first["compsent"].find(vec[i].value) != first["compsent"].end())
		compsent();
	else
		match();
}

void suffix() { //后缀
	if (vec[i].value == LROUNDB) {
		match(LROUNDB);
		buf.push_back('(');
		Express();
		match(RROUNDB);
		addRroundb();
		buf.push_back(' ');
	}
	else
		Express();
}

void ifsent() { //条件语句
	match(IF);
	printTag();
	buf += "if (";
	Conditio();
	addRroundb();
	buf += " {\n";
	match(THEN);
	++tag;
	sentence();
	--tag;
	printTag();
	buf += "}\n";
}

void read() { //读语句
	match(READ);
	printTag();
	buf += "scanf";
	match(LROUNDB);
	buf.push_back('(');
	match(STR);
	buf += vec[i - 1].key;
	idsuff();
	match(RROUNDB);
	addRroundb();
	buf += ";\n";
	if (!io) {
		buf = "#include <stdio.h>\n\n" + buf;
		io = true;
	}
}

void whilsent() { //当型循环语句
	match(WHILE);
	printTag();
	buf += "while (";
	Conditio();
	addRroundb();
	buf += " {\n";
	match(DO);
	++tag;
	sentence();
	--tag;
	printTag();
	buf += "}\n";
}

void idsuff() { //标识符后缀
	if (vec[i].value == COMMA) {
		match(COMMA);
		buf += ", ";
		match(STR);
		buf += vec[i - 1].key;
		idsuff();
	}
	else
		match();
}

void Write() { // 写语句
	match(WRITE);
	printTag();
	buf += "printf";
	match(LROUNDB);
	buf.push_back('(');
	Express();
	Exprsuff();
	match(RROUNDB);
	addRroundb();
	buf += ";\n";
	if (!io) {
		buf = "#include <stdio.h>\n\n" + buf;
		io = true;
	}
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
		buf += ", ";
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
		buf += "+ ";
		term();
		termsuff();
	}
	else if (vec[i].value == SUB) {
		match(SUB);
		buf += "- ";
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
	if (first["muloper"].find(vec[i].value) != first["muloper"].end()) {
		Muloper();
		Factor();
		Factsuff();
	}
	else
		match();
}

void argument() { //参数部分
	match(LROUNDB);
	buf += "<type> ";
	match(STR);
	buf += vec[i - 1].key;
	match(COLON);
	typeil();
	string type = vec[i - 1].key;
	if (type == "INTEGER")
		buf.replace(buf.find("<type>"), 6, "int");
	else if (type == "REAL")
		buf.replace(buf.find("<type>"), 6, "short");
	match(RROUNDB);
	addRroundb();
	buf.push_back(' ');
}

void Factor() { //因子
	if (vec[i].value == STR) {
		match(STR);
		buf += vec[i - 1].key;
		buf.push_back(' ');
	}
	else if (vec[i].value == NUM) {
		match(NUM);
		buf += vec[i - 1].key;
		buf.push_back(' ');
	}
	else {
		match(LROUNDB);
		buf.push_back('(');
		Express();
		match(RROUNDB);
		addRroundb();
		buf.push_back(' ');
	}
}

void addoper() { //加型运算符
	if (vec[i].value == ADD) {
		match(ADD);
		buf += "+ ";
	}
	else {
		match(SUB);
		buf += "- ";
	}
}

void Muloper() { //乘型运算符
	if (vec[i].value == MUL) {
		match(MUL);
		buf += "* ";
	}
	else {
		match(DEV);
		buf += "/ ";
	}
}

void respoper() { //关系运算符
	if (vec[i].value == ASSIGN) {
		match(ASSIGN);
		buf += "== ";
	}
	else if (vec[i].value == NE) {
		match(NE);
		buf += "!= ";
	}
	else if (vec[i].value == LT) {
		match(LT);
		buf += "< ";
	}
	else if (vec[i].value == LE) {
		match(LE);
		buf += "<= ";
	}
	else if (vec[i].value == GT) {
		match(GT);
		buf += "> ";
	}
	else {
		match(GE);
		buf += ">= ";
	}
}

void match(int n) {
	if (i < vec.size()) {
		if (vec[i].value == n) {
			++i;
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

// void setFirst() {
// 	first["program"] = set<int>({ PROGRAM });
// 	first["proghead"] = set<int>({ PROGRAM });
// 	first["block"] = set<int>({ CONST, VAR, PROCEDURE, BEGIN });
// 	first["consexpl"] = set<int>({ CONST, 0 });
// 	first["consdefi"] = set<int>({ STR });
// 	first["varexpl"] = set<int>({ VAR, 0 });
// 	first["conssuff"] = set<int>({ COMMA, 0 });
// 	first["vardefi"] = set<int>({ STR });
// 	first["varsuff"] = set<int>({ STR, 0 });
// 	first["procdefi"] = set<int>({ PROCEDURE, 0 });
// 	first["typeil"] = set<int>({ INTEGER, REAL });
// 	first["procedh"] = set<int>({ PROCEDURE });
// 	first["procsuff"] = set<int>({ PROCEDURE, 0 });
// 	first["assipro"] = set<int>({ STR });
// 	first["sentence"] = set<int>({ STR, IF, WHILE, READ, WRITE, BEGIN, 0 });
// 	first["suffix"] = set<int>({ ADD, SUB, STR, NUM, LROUNDB, 0 });
// 	first["ifsent"] = set<int>({ IF });
// 	first["read"] = set<int>({ READ });
// 	first["whilsent"] = set<int>({ WHILE });
// 	first["idsuff"] = set<int>({ COMMA, 0 });
// 	first["write"] = set<int>({ WRITE });
// 	first["compsent"] = set<int>({ BEGIN });
// 	first["exprsuff"] = set<int>({ COMMA, 0 });
// 	first["sentsuff"] = set<int>({ SEMICOLON, 0 });
// 	first["condition"] = set<int>({ ADD, SUB, STR, NUM, LROUNDB, ODD });
// 	first["termsuff"] = set<int>({ ADD, SUB, 0 });
// 	first["express"] = set<int>({ ADD, SUB, STR, NUM, LROUNDB });
// 	first["term"] = set<int>({ STR, NUM, LROUNDB });
// 	first["factsuff"] = set<int>({ MUL, DEV, 0 });
// 	first["argument"] = set<int>({ LROUNDB });
// 	first["factor"] = set<int>({ STR, NUM, LROUNDB });
// 	first["addoper"] = set<int>({ ADD, SUB });
// 	first["muloper"] = set<int>({ MUL, DEV });
// 	first["respoper"] = set<int>({ ASSIGN, NE, LT, LE, GT, GE });

// 	for (auto it = first.begin(); it != first.end(); ++it) {
// 		if (myFirst.find(it->first) == myFirst.end()) {
// 			cout << "wrong " << it->first << endl;
// 		}
// 		else {
// 			if (it->second != myFirst[it->first]) {
// 				cout << "wrong " << it->first << endl;
// 			}

// 		}
// 	}
// 	return;
// }

int main() {
	//setFirst();
	program();
	ofstream fout(".//res//" + filename + ".txt");
	cout << "该程序符合PASCAL文法。" << endl;
	cout << buf;
	fout << buf;
	return 0;
}
