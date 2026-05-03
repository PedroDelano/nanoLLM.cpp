#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using namespace std;

vector<MatrixXd> dataloader_v1(
  string text,
  size_t batch_size,
  size_t max_length,
  unordered_map<string, int> token_map,
  MatrixXd embedding_matrix
);
