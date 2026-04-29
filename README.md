# nanoLLM.cpp

A C++ port of the LLM-from-scratch journey laid out in
**"Build a Large Language Model (from Scratch)"** by Sebastian Raschka
(Manning, October 2024).

The book walks through designing, pretraining, and fine-tuning a small
GPT-style language model in Python with PyTorch. This repository is a
study project that re-implements the same ideas in modern C++ using
[Eigen](https://eigen.tuxfamily.org/) for linear algebra.

## Goals

- Re-create each chapter's milestone in C++ instead of Python.
- Keep dependencies minimal — only the C++ standard library and Eigen.
- Stay readable: the point is to learn the internals, not to ship a
  production runtime.

## Roadmap

Following the structure of the book:

1. Working with text data — tokenization, vocabularies, datasets.
2. Coding attention mechanisms — self-attention, multi-head, masked.
3. Implementing a GPT model from scratch.
4. Pretraining on an unlabeled corpus.
5. Fine-tuning for text classification.
6. Fine-tuning to follow instructions.

Each milestone will live in its own subdirectory as the project grows.

## Requirements

- A C++17-capable compiler (e.g. `g++` 9+ or `clang++` 10+).
- GNU Make.
- [Eigen 5.0.0](https://gitlab.com/libeigen/eigen) (header-only).

Eigen is **not** vendored in the repo — see below.

## Getting Eigen

The Makefile expects Eigen to be extracted into `./eigen-5.0.0/` at the
project root. If it isn't there, the build will stop with a message
pointing here.

Two ways to get it:

**Automatic** (requires `curl` and `tar`):

```bash
make fetch-eigen
```

**Manual**: download
<https://gitlab.com/libeigen/eigen/-/archive/5.0.0/eigen-5.0.0.tar.gz>
and extract the archive at the repo root so that `eigen-5.0.0/Eigen/`
exists.

## Build & run

```bash
make          # checks for Eigen, then compiles main.cpp -> ./main
./main        # runs the current example
make clean    # removes the built binary
```

The current `main.cpp` is just a sanity check that builds a 2x2
`Eigen::MatrixXd` and prints it.

## Layout

```
nanoLLM.cpp/
├── Makefile        # build + Eigen check + fetch helper
├── main.cpp        # current entry point / scratch pad
├── README.md
└── eigen-5.0.0/    # Eigen headers (gitignored)
```

## Reference

Raschka, Sebastian. *Build a Large Language Model (from Scratch)*.
Manning, 2024. <https://www.manning.com/books/build-a-large-language-model-from-scratch>
