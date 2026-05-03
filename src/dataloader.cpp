/*
 *
 * A dataloader receives text as input and 
 * returns an iterator of N batches, of X words each
 *
 * */

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>

#include "tokenization.hpp"

using Eigen::MatrixXd;
using namespace std;

vector<MatrixXd> dataloader_v1(
  string text,
  size_t batch_size,
  size_t max_length,
  unordered_map<string, int> token_map,
  MatrixXd embedding_matrix
) {

  vector<string> tokens = tokenize(text);
  assert(tokens.size() > max_length);
  assert(tokens.size() > batch_size);

  vector<MatrixXd> batch;
  size_t index;
  int curr_token_id;

  for (size_t window = 0; window < (tokens.size() - max_length); window++) {
    index = 0;
    for (size_t i = 0; i < max_length; i++) {
      index = window + i;
      cout << window  << " / " << i << " ---> " << index << " = " << tokens.at(index) << " => " << token_decode(token_encode(tokens.at(index), token_map), token_map) << endl;
      curr_token_id = token_encode(tokens.at(index), token_map);
      batch.push_back(embedding_matrix.row(curr_token_id));
    }
    cout << endl;
  }

  return batch;

}


