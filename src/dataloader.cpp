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
#include "dataloader.hpp"

using Eigen::MatrixXd;
using namespace std;

Data dataloader_v1(
  string text,
  size_t batch_size,
  size_t max_length,
  size_t stride,
  unordered_map<string, int> token_map
) {


  vector<string> tokens = tokenize(text);
  assert(tokens.size() > max_length);
  assert(tokens.size() > batch_size);
  assert(stride > 0);

  int curr_batch = 0;

  vector<MatrixXd> input;
  vector<MatrixXd> output;

  MatrixXd batch_in(batch_size, max_length);
  MatrixXd batch_out(batch_size, max_length);

  batch_in.setZero();
  batch_out.setZero();

  for (size_t window = 0; window < (tokens.size() - max_length); window += stride) {
    for (size_t i = 0; i < max_length; i++) {
      size_t in_idx = window + i;
      size_t tgt_idx = window + i + 1;
      batch_in(curr_batch, i)  = token_encode(tokens.at(in_idx),  token_map);
      batch_out(curr_batch, i) = token_encode(tokens.at(tgt_idx), token_map);
    }
    curr_batch++;

    if (curr_batch == batch_size) {
      input.push_back(batch_in);
      output.push_back(batch_out);
      curr_batch = 0;
      batch_in.setZero();
      batch_out.setZero();
    }

    assert(curr_batch < batch_size);

  }

  Data data;
  data.input = input;
  data.output = output;
  return data;

}


