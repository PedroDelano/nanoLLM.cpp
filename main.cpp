#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <Eigen/Dense>

#include "src/tokenization.hpp"
#include "src/embedding.hpp"

using namespace std;
using Eigen::MatrixXd;

int main(){

  string test = "a guy walks into a bar.";
  vector<string> tokens = tokenize(test);

  for (size_t i = 0; i < tokens.size(); i++) {
    cout << tokens[i] << endl;
  }

  unordered_map<string, int> token_map = generate_token_map("data/vocab_tiny.txt");

  string tk = "love";
  int tk_id = token_encode(tk, token_map);
  cout << "Encoding for " << tk << " is " << tk_id << ". Decoded is " << token_decode(tk_id, token_map) << endl;

  MatrixXd emb_matrix = init_embeddings(6, 6);
  cout << emb_matrix << endl;

  return 0;

}
