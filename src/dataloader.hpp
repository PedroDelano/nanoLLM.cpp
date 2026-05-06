#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using namespace std;

typedef struct Data_s {
  vector<MatrixXd> input;
  vector<MatrixXd> output;
} Data;

Data dataloader_v1(
  string text,
  size_t batch_size,
  size_t max_length,
  size_t stride,
  unordered_map<string, int> token_map
);
