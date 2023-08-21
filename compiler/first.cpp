#include "first.h"

void getPreserved() {
    if (!mp.empty())
        return;
    mp.insert(pair<string, int>("PROGRAM", PROGRAM));
    mp.insert(pair<string, int>("CONST", CONST));
    mp.insert(pair<string, int>("VAR", VAR));
    mp.insert(pair<string, int>("INTEGER", INTEGER));
    mp.insert(pair<string, int>("LONG", LONG));
    mp.insert(pair<string, int>("PROCEDURE", PROCEDURE));
    mp.insert(pair<string, int>("IF", IF));
    mp.insert(pair<string, int>("THEN", THEN));
    mp.insert(pair<string, int>("WHILE", WHILE));
    mp.insert(pair<string, int>("DO", DO));
    mp.insert(pair<string, int>("READ", READ));
    mp.insert(pair<string, int>("WRITE", WRITE));
    mp.insert(pair<string, int>("BEGIN", BEGIN));
    mp.insert(pair<string, int>("END", END));
    mp.insert(pair<string, int>("ODD", ODD));
    mp.insert(pair<string, int>("REAL", REAL));

    mp.insert(pair<string, int>("=", ASSIGN));
    mp.insert(pair<string, int>(":=", VAL));
    mp.insert(pair<string, int>(">=", GE));
    mp.insert(pair<string, int>("<=", LE));
    mp.insert(pair<string, int>("<>", NE));

    mp.insert(pair<string, int>("<", LT));
    mp.insert(pair<string, int>(">", GT));
    mp.insert(pair<string, int>("(", LROUNDB));
    mp.insert(pair<string, int>(")", RROUNDB));
    mp.insert(pair<string, int>("+", ADD));
    mp.insert(pair<string, int>("-", SUB));
    mp.insert(pair<string, int>("*", MUL));
    mp.insert(pair<string, int>("/", DEV));
    mp.insert(pair<string, int>(".", DOT));
    mp.insert(pair<string, int>(",", COMMA));
    mp.insert(pair<string, int>(":", COLON));
    mp.insert(pair<string, int>(";", SEMICOLON));
    mp.insert(pair<string, int>("#", WELL));
    return;
}

void traceback(unordered_map<string, set<int> >& first, const string& head) {
    if (first.find(head) == first.end())
        first[head] = set<int>();
    else
        return;
    vector<pair <pair<string, int>, unordered_multimap<string, vector<string> >::iterator > > nexts;
    auto range = sentences.equal_range(head);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second[0] == "@")
            first[head].insert(0);
        else if (mp.find(it->second[0]) != mp.end())
            first[head].insert(mp[it->second[0]]);
        else if (it->second[0] == "STR") {
            first[head].insert(44);
        }
        else if (it->second[0] == "NUM") {
            first[head].insert(43);
        }
        else {
            pair<string, int> p(it->second[0], 0);
            nexts.push_back(pair<pair<string, int>, unordered_multimap<string, vector<string> >::iterator >(p, it));
        }
    }
    int i = 0;
    while (i < nexts.size()) { //be care of recursion
        traceback(first, nexts[i].first.first);
        if (first[nexts[i].first.first].find(0) != first[nexts[i].first.first].end()) {
            unordered_multimap<string, vector<string> >::iterator it = nexts[i].second;
            int index = nexts[i].first.second + 1;
            if (it->second.size() != index) {
                string str = it->second[index];
                if (mp.find(str) != mp.end()) {
                    first[head].insert(mp[str]);
                }
                else if (str == "STR") {
                    first[head].insert(44);
                }
                else if (str == "NUM") {
                    first[head].insert(43);
                }
                else {
                    pair<string, int> p(str, index);
                    nexts.push_back(pair<pair<string, int>, unordered_multimap<string, vector<string> >::iterator >(p, it));
                }
            }
        }
        ++i;
    }
    for (auto next : nexts) {
        if (first[next.first.first].find(0) == first[next.first.first].end())
            first[head].insert(first[next.first.first].begin(), first[next.first.first].end());
        else if (next.first.second == next.second->second.size() - 1) {
            first[head].insert(first[next.first.first].begin(), first[next.first.first].end());
        }
        else {
            set<int> st = first[next.first.first];
            st.erase(0);
            first[head].insert(st.begin(), st.end());
        }
    }
    return;
}

unordered_map<string, set<int> > getFirst() {
    getPreserved();
    unordered_map<string, set<int> > first;
    ifstream fin(".//res//bnf.txt");
    string str;
    while (!fin.eof()) {
        getline(fin, str);
        getSentence(str);
    }
    for (auto it = sentences.begin(); it != sentences.end(); ++it) {
        traceback(first, it->first);
    }
    return first;
}

void getSentence(string str) {
    if (str.size() == 0)
        return;
    int i = str.find(' ');
    string head = str.substr(0, i);
    i += 4;
    string temp = "";
    vector<string> tail;
    str.push_back(' ');
    while (i < str.size()) {
        if (str[i] == ' ' || str[i] == '\n') {
            if (temp != "") {
                tail.push_back(temp);
                temp = "";
            }
        }
        else if (str[i] == '|') {
            sentences.insert({ head, tail });
            tail.clear();
        }
        else {
            temp.push_back(str[i]);
        }
        i++;
    }
    sentences.insert({ head, tail });
    return;
}

// int main() {
//     unordered_map<string, set<int> > first = getFirst();

//     for (auto it = sentences.begin(); it != sentences.end(); ++it) {
//         cout << it->first << " -> ";
//         for (auto temp : it->second)
//             cout << temp << " ";
//         cout << endl;
//     }
//     cout << "-----------------------------------------------------------" << endl;
//     for (auto it = first.begin(); it != first.end(); ++it) {
//         cout << it->first << " : ";
//         for (auto i : it->second)
//             cout << i << " ";
//         cout << endl;
//     }
//     return 0;
// }
