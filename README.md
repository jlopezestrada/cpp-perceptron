![GitHub last commit](https://img.shields.io/github/last-commit/jlopezestrada/cpp-perceptron)
![GitHub issues](https://img.shields.io/github/issues/jlopezestrada/cpp-perceptron)

# C++ Perceptron
C++ Perceptron (*cpp-perceptron*) is a small educational implementation of a binary perceptron in C++. The included demo trains models for linearly separable logic gates (`AND`, `OR`, `NAND`, and `NOR`) and explains why `XOR` cannot be learned by a single perceptron.

## Perceptron
The perceptron was one of the earliest types of artificial neural networks and was developed in the 1950s. It can be used for binary classification tasks where the input data is linearly separable. With the advent of more advanced neural network architectures, the perceptron has been largely superseded, but it remains an important building block in neural network development.

<p align="center"><img src="https://upload.wikimedia.org/wikipedia/commons/8/8c/Perceptron_moj.png" width="500px"></p>

## Documentation
- [Historical background of the perceptron](docs/perceptron-history.md)
- [Technical details of the perceptron](docs/perceptron-technical-details.md)

## Build
This project uses CMake and requires a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/cpp-perceptron
```

## Test
```bash
ctest --test-dir build --output-on-failure
```

# License
![GitHub](https://img.shields.io/github/license/jlopezestrada/cpp-perceptron)
