#include "Perceptron.h"

#include <cmath>
#include <stdexcept>
#include <vector>

Perceptron::Perceptron(std::size_t n_features, double learningRate)
    : n_features(n_features), learningRate(learningRate), weights(n_features, 0.0) {
    if (n_features == 0) {
        throw std::invalid_argument("n_features must be greater than zero");
    }

    if (!std::isfinite(learningRate) || learningRate <= 0.0) {
        throw std::invalid_argument("learningRate must be a positive finite value");
    }
}

void Perceptron::train(const std::vector<std::vector<double>>& inputs, const std::vector<double>& labels, int epochs) {
    if (inputs.empty()) {
        throw std::invalid_argument("inputs cannot be empty");
    }

    if (inputs.size() != labels.size()) {
        throw std::invalid_argument("inputs and labels must contain the same number of samples");
    }

    if (epochs <= 0) {
        throw std::invalid_argument("epochs must be greater than zero");
    }

    for (std::size_t i = 0; i < inputs.size(); i++) {
        validateInput(inputs[i]);

        if (labels[i] != 0.0 && labels[i] != 1.0) {
            throw std::invalid_argument("labels must be either 0.0 or 1.0");
        }
    }

    for (int epoch = 0; epoch < epochs; epoch++) {
        for (std::size_t i = 0; i < inputs.size(); i++) {
            double weightedSum = bias;
            for (std::size_t j = 0; j < n_features; j++) {
                weightedSum += weights[j] * inputs[i][j];
            }

            double predicted = activationFunction(weightedSum);
            double error = labels[i] - predicted;

            if (error != 0) {
                for (std::size_t j = 0; j < weights.size(); j++) {
                    weights[j] += learningRate * error * inputs[i][j];
                }
                bias += learningRate * error;
            }
        }
    }
}

double Perceptron::predict(const std::vector<double>& input) const {
    validateInput(input);

    double weightedSum = bias;
    for (std::size_t i = 0; i < n_features; i++) {
        weightedSum += weights[i] * input[i];
    }

    return activationFunction(weightedSum);
}

double Perceptron::activationFunction(double weightSum) const {
    return weightSum > 0.0 ? 1.0 : 0.0;
}

void Perceptron::validateInput(const std::vector<double>& input) const {
    if (input.size() != n_features) {
        throw std::invalid_argument("input size must match n_features");
    }

    for (double value : input) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument("input values must be finite");
        }
    }
}

const std::vector<double>& Perceptron::getWeights() const {
    return weights;
}

double Perceptron::getBias() const {
    return bias;
}
