#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <Eigen/Dense>

#include "src/tokenization.hpp"
#include "src/embedding.hpp"
#include "src/dataloader.hpp"

using namespace std;
using Eigen::MatrixXd;

int main(){

  string test = "a guy walks into a bar.";
  vector<string> tokens = tokenize(test);

  for (size_t i = 0; i < tokens.size(); i++) {
    cout << tokens[i] << endl;
  }

  unordered_map<string, int> token_map = generate_token_map("data/vocab_large.txt");

  string tk = "love";
  int tk_id = token_encode(tk, token_map);
  cout << "Encoding for " << tk << " is " << tk_id << ". Decoded is " << token_decode(tk_id, token_map) << endl;

  const int EMBED_SIZE = 4;
  const int VOCAB_SIZE = token_map.size();
  MatrixXd embedding_matrix = init_embeddings(VOCAB_SIZE, EMBED_SIZE);

  MatrixXd emb = embed_token(tk, token_map, embedding_matrix);
  cout << emb << endl;

  MatrixXd pos = pos_embed(2, EMBED_SIZE);
  cout << pos << endl;

  const string TEXT = "the fox jumps over the lazy dog. the fox jumps over the lazy dog. the fox jumps over the lazy dog.";
  const size_t STRIDE = 1;
  const size_t BATCH_SIZE = 4;
  const size_t MAX_LENGTH = 4;
  Data data = dataloader_v1(TEXT, BATCH_SIZE, MAX_LENGTH, STRIDE, token_map);

  for (size_t i = 0; i < data.output.size(); i++) {
    cout << "=============================" << endl;
    cout << data.input.at(i) << endl;
    cout << "=============================" << endl;
    cout << data.output.at(i) << endl;
    cout << "=============================" << endl;
    cout << endl;
  }

  return 0;

}
