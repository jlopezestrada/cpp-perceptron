#include <iostream>
#include <vector>
#include "../include/Perceptron.h"

Perceptron::Perceptron(int n_features, double learningRate) : n_features(n_features), learningRate(learningRate), weights(n_features, 0.0) {
}

void Perceptron::train(std::vector<std::vector<double>>& inputs, std::vector<double>& labels, int epochs) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        std::cout << "Epoch: " << epoch << std::endl;
        for (size_t i = 0; i < inputs.size(); i++) {
            double weightedSum = bias;
            for (int j = 0; j < n_features; j++) {
                weightedSum += weights[j] * inputs[i][j];
            }

            int predicted = activationFunction(weightedSum);
            int error = labels[i] - predicted;
            std::cout << "\t\t\tERROR: " << error << std::endl;

            if (error != 0) {
                std::cout << "\t\t\tUPDATE WEIGHTS" << std::endl;
                for (size_t j = 0; j < weights.size(); j++) {
                    weights[j] += learningRate * error * inputs[i][j];
                }
                bias += learningRate * error;
            }

            std::cout << "\tInstance " << i << ": Prediction = " << predicted << ", Actual = " << labels[i];
            if (error == 0.0) {
                std::cout << " -> OK" << std::endl;
            }
            else {
                std::cout << " -> Error" << std::endl;
            }
        }
    }
}

double Perceptron::predict() {
    // TODO
    return 0.0;
}

int Perceptron::activationFunction(double weightSum) const {
    return weightSum > 0.0 ? 1 : 0;
}