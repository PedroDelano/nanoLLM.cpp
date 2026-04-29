# Roadmap

A C++/Eigen project that ends with **a small GPT pretrained from
scratch on commodity hardware**. Raschka's *Build a Large Language
Model (From Scratch)* is the conceptual reference, but this roadmap is
organised around shippable milestones, not book chapters. No
open-weight loading anywhere — the model in `nanollm` is one we
trained ourselves.

The phases below are sequential: each one ends with something that
runs end-to-end on its own. Don't start phase N+1 until phase N's
"done means" line is true.

---

## Phase 0 — Project foundations

**Done means:** `make test` runs a trivial unit test, the source tree
has room to grow, and the model size we're targeting is written down.

- [ ] Pick the target model size. Realistic CPU-trainable budget:
      `emb=128–256`, `heads=4`, `layers=4–6`, `ctx=128–256`, vocab
      determined by tokenizer choice. Larger configs stay supported by
      the code but won't be trained on this machine.
- [ ] Lay out source: `src/`, `include/nanollm/`, `tests/`, `data/`,
      `tools/`. Update the `Makefile` to glob `src/**/*.cpp`.
- [ ] Tensor conventions: settle on `Eigen::MatrixXf` everywhere,
      shape comments in `(batch, time, ...)` order, document in
      `include/nanollm/tensor.h`.
- [ ] Test harness: pick a tiny header-only framework (doctest is
      easy) and add `make test`.
- [ ] Replace `main.cpp` with a CLI skeleton: `nanollm <subcmd>` with
      stubs for `tokenize`, `train`, `generate`, `eval`.

## Phase 1 — Data pipeline

**Done means:** running `nanollm tokenize` on a text file produces
`(input_ids, target_ids)` mini-batches end-to-end, and a round-trip
`encode → decode` is byte-exact on ASCII test strings.

- [ ] Pick the corpus. Candidates, from smallest to largest:
      "The Verdict" short story (~5k tokens, debug only) → TinyStories
      (~500MB) → a Wikipedia subset. Pick **one** to optimise for and
      note why.
- [ ] Pick the tokenizer. Two options — choose one before writing
      code:
      - [ ] **A) Byte-level BPE, trained ourselves.** Vocab ~8k. More
        work but keeps the project self-contained.
      - [ ] **B) Byte-level fallback only** (vocab=256). Trivial to
        implement, longer sequences, fine for tiny models.
- [ ] Implement the chosen tokenizer in `src/tokenizer/`.
- [ ] Special tokens: at least `<|endoftext|>`. `<|unk|>` only if BPE
      can fail to cover input.
- [ ] Sliding-window dataset: `(token_stream, max_length, stride) →
      (x, y)` where `y = x` shifted by 1. Stream from disk, don't load
      the whole corpus into RAM.
- [ ] Mini-batch iterator with a configurable seed, drop-last, and a
      train/val split.
- [ ] Token + learned positional embeddings as `Eigen::MatrixXf` of
      shape `(vocab, emb)` and `(ctx, emb)`; final input = sum.

## Phase 2 — Forward pass

**Done means:** `nanollm generate --random-init` runs a forward pass
through the full model and emits (gibberish) text via greedy decoding.
Shapes are checked against a Python/PyTorch reference to ~1e-5.

- [ ] `LayerNorm` (per-token mean/var, learnable affine, `eps=1e-5`).
- [ ] `GELU` (tanh approximation).
- [ ] `Linear` layer with bias toggle.
- [ ] `FeedForward`: `Linear(emb, 4*emb) → GELU → Linear(4*emb, emb)`.
- [ ] Causal multi-head attention with the **single-matmul** layout
      (one `W_qkv`, reshape to `(B, heads, T, head_dim)`), upper-tri
      mask, attention dropout. Don't build `std::vector<Head>`.
- [ ] `TransformerBlock` in pre-LN order: LN → MHA → residual → LN →
      FFN → residual.
- [ ] `GPTModel` = embeddings + `n_layers` blocks + final LN + output
      head. Tied input/output embedding by default.
- [ ] Parameter-count helper; print on construction.
- [ ] Greedy `generate(prompt_ids, max_new_tokens)` that crops to
      `ctx_len` and appends `argmax` of the last-row logits.
- [ ] Numerical parity test: same weights/inputs in PyTorch and in
      `nanollm`, output diff `< 1e-5`. This catches almost every silent
      bug in phases 3 and 4.

## Phase 3 — Backprop and a tiny training run

**Done means:** training on a few-thousand-token toy corpus
("The Verdict") drives loss down convincingly and `nanollm generate`
emits text that's at least loop-coherent.

- [ ] Pick the autograd strategy:
      - [ ] **A) Hand-written backward passes** for each op
        (Linear, LayerNorm, GELU, softmax+CE fused, attention,
        embedding, residual). Smallest dependency footprint.
      - [ ] **B) Tape-based autograd** in `src/autograd/`. More
        upfront work, less code-per-op once it's running.
      Document the choice in a `notes/autograd.md`.
- [ ] Implement the backward path for every op used in phase 2, with a
      finite-difference gradcheck per op.
- [ ] Cross-entropy loss over `(B, T, vocab)` logits and `(B, T)`
      targets, fused with softmax for numerical stability.
- [ ] AdamW (`betas=(0.9, 0.95)`, `eps=1e-8`, `weight_decay=0.1`,
      decoupled).
- [ ] Training loop: forward → loss → backward → step, with periodic
      validation-loss eval and logging to a CSV.
- [ ] Train on "The Verdict" until train loss < 1.0 with a small
      config (e.g. `emb=128, layers=4`). Capture a sample generation
      and the loss curve in `notes/runs/00-verdict.md`.

## Phase 4 — Real pretraining run

**Done means:** a multi-hour CPU training run on the corpus chosen in
phase 1 produces a checkpoint that generates locally coherent prose.

- [ ] LR schedule: linear warmup → cosine decay to ~10% of peak.
- [ ] Gradient clipping by global norm (default `1.0`).
- [ ] Checkpointing: save weights + optimizer state + step number to
      a simple binary format (`magic | header_json_len | header_json |
      tensor blobs`); reload restores training exactly.
- [ ] Resumable training (`nanollm train --resume path/to/ckpt`).
- [ ] Sampling for generation: temperature scaling and top-k. Combine
      into one `generate` path with `eos_id` early-stopping.
- [ ] Run the actual training. Targets to hit: stable loss curve,
      val-loss tracks train-loss, sample text shows real word
      structure. Document the config and result in
      `notes/runs/01-pretrain.md`.
- [ ] Stretch: KV-cache for inference so `generate` is `O(T)` instead
      of `O(T^2)` per token.

## Phase 5 — Performance (optional, do only if Phase 4 is too slow)

**Done means:** a benchmarked speedup vs. the Phase-4 baseline,
measured with `nanollm bench`.

- [ ] Benchmark harness: tokens/sec for forward and forward+backward.
- [ ] OpenMP / `Eigen::setNbThreads` tuning; pin threads.
- [ ] Block-friendly matmul layouts; check Eigen is hitting BLAS.
- [ ] FP16 / BF16 weights for inference only (training stays FP32).
- [ ] Stretch: a hand-rolled fused softmax+attention kernel for the
      hot path.

---

## Decisions to make before phase 1 starts

1. **Corpus**: TinyStories is the sweet spot for CPU pretraining —
   small models actually learn coherent English on it. Pick this
   unless there's a reason not to.
2. **Tokenizer**: byte-level BPE (option A) is the more interesting
   exercise and matches what real GPTs do. Byte-only (option B) is the
   "ship it faster" path.
3. **Autograd**: hand-written backward (option A) is enough for this
   project's scope and avoids building a framework you'd then have to
   maintain. Pick a tape only if you want the framework as a goal in
   itself.

## Explicitly out of scope

- Loading any pretrained weights (OpenAI / HF / etc.).
- GPU support.
- Multi-machine training.
- Mixed-precision training (inference-only FP16 is fine in Phase 5).
- Instruction / RLHF fine-tuning.
- Classification fine-tuning. Easy to add later if interesting, but
  not a goal.
