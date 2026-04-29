/**********************************************
 * Embedding is nothing but a lookup table
 * that finds vectors of fixed size from a
 * given integer
 ********************************************/

#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;

MatrixXd init_embeddings(int n_tokens, int embedding_size) {
  MatrixXd emb = MatrixXd::Random(n_tokens, embedding_size);
  return emb;
}
