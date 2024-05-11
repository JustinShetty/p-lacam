early-exit-lacam
---
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
[![CI](https://github.com/justinshetty/early-exit-lacam/actions/workflows/ci.yml/badge.svg)](https://github.com/justinshetty/early-exit-lacam/actions/workflows/ci.yml)

[LaCAM](https://github.com/kei18/lacam) modified to exit early upon meeting a solution threshold, i.e. number of agents that have reached their goals. Intended to be used iteratively to progressively make global progress.

## Building

All you need is [CMake](https://cmake.org/) (≥v3.16). The code is written in C++(17).

First, clone this repo with submodules.

```sh
git clone --recursive https://github.com/justinshetty/early-exit-lacam.git
cd lacam
```
Then, build the project.

```sh
cmake -B build && make -C build
```

## Usage

This fork introduces an additional command-line argument
```
-T, --threshold       number of reached goals necessary to terminate [default: "1"]
```
which determines the exit condition for the algorithm.
