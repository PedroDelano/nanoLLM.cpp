#pragma once
#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;

MatrixXd init_embeddings(int n_tokens, int embedding_size);
MatrixXd embed(string s, unordered_map<string, int> token_map, MatrixXd embedding_matrix);

