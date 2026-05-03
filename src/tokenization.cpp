/**********************************************
 *
 * Tokenization consists into a function that
 * takes a string and finds a integer in a map
 * that correspond to it.
 *
 * *********************************************/

#include <cassert>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

const string UNK = "[UNK]"; // UNKNOWN
const string SPC = "[SPC]"; // SPACE


vector<string> tokenize(string s) {
  // from a string returns the list of words (tokens)
  vector<string> tokens;
  string tk = "";
  string accent = "";

  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == ' ') {
      // flush word
      tokens.push_back(tk);
      tokens.push_back(SPC);
      tk = "";
    } else if (s[i] == '.' || s[i] == '!' || s[i] == '?' || s[i] == ',') {
      // flush word
      tokens.push_back(tk);
      tokens.push_back(SPC);
      tk = "";
      // add accent
      accent += s[i];
      tokens.push_back(accent);
      accent = "";
    } else {
      tk += s[i];
    }
  }

  if (tk != "") {
    tokens.push_back(tk);
  }

  return tokens;

}

unordered_map<string, int> generate_token_map(string fpath) {

  unordered_map<string, int> token_map = {};
  ifstream File(fpath);

  // 2. Check if the file opened successfully
  if (!File.is_open()) {
      std::cerr << "Error: Could not open the file!" << std::endl;
      return token_map;
  }

  size_t token_id = 0;
  string token;
  while(getline(File, token)){
    token_map.insert({token, token_id});
    token_id++;
  }

  // assert special tokens
  assert(token_map.count(UNK) == 1);
  assert(token_map.count(SPC) == 1);

  return token_map;

}

int token_encode(string s, unordered_map<string, int> token_map) {
  if (token_map.find(s) == token_map.end()) {
    return token_map.at(UNK);
  }
  return token_map.at(s);
}

string token_decode(int token_id, unordered_map<string, int> token_map) {
 for (const auto& [key, value]: token_map) {
    if (value == token_id) {
      return key;
    }
  }
  return UNK;
}
