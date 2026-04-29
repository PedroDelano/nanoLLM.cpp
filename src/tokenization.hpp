#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

#include "tokenization.cpp"

using namespace std;

string decode(int token_id, unordered_map<string, int> token_map);
int encode(string s, unordered_map<string, int> token_map);
unordered_map<string, int> generate_token_map(string fpath);
vector<string> tokenize(string s);

