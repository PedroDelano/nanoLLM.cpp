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

MatrixXd embed_token(string s, unordered_map<string, int> token_map, MatrixXd embedding_matrix) {
  int id = token_encode(s, token_map);
  return embedding_matrix.row(id);
}

MatrixXd pos_embed(size_t num_tokens, size_t embedding_size) {
  MatrixXd pos(num_tokens, embedding_size);
  for (size_t i = 0; i < num_tokens; i++) {
    for (size_t j = 0; j < embedding_size; j++) {
      pos(i, j) = i;
    }
  }
  return pos;
}
