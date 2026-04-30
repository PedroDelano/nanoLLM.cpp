/**********************************************
 * Embedding is nothing but a lookup table
 * that finds vectors of fixed size from a
 * given integer
 ********************************************/

#include <iostream>
#include <string>
#include <unordered_map>
#include <Eigen/Dense>

#include "tokenization.hpp"

using Eigen::MatrixXd;

MatrixXd init_embeddings(int n_tokens, int embedding_size) {
  MatrixXd emb = MatrixXd::Random(n_tokens, embedding_size);
  return emb;
}

MatrixXd embed(string s, unordered_map<string, int> token_map, MatrixXd embedding_matrix) {
  int id = token_encode(s, token_map);
  return embedding_matrix.row(id);
}
