#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include "src/tokenization.hpp"

using namespace std;

int main(){

  string test = "a guy walks into a bar.";
  vector<string> tokens = tokenize(test);

  for (size_t i = 0; i < tokens.size(); i++) {
    cout << tokens[i] << endl;
  }

  unordered_map<string, int> token_map = generate_token_map("data/vocab_tiny.txt");

  string tk = "love";
  int tk_id = encode(tk, token_map);
  cout << "Encoding for " << tk << " is " << tk_id << ". Decoded is " << decode(tk_id, token_map) << endl;

  return 0;

}
