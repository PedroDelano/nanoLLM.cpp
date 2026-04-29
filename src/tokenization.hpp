#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

string token_decode(int token_id, unordered_map<string, int> token_map);
int token_encode(string s, unordered_map<string, int> token_map);
unordered_map<string, int> generate_token_map(string fpath);
vector<string> tokenize(string s);

