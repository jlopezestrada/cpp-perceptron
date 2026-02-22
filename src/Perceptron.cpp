#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "../include/Perceptron.h"

Perceptron::Perceptron(int n_features, double learningRate) : n_features(n_features), learningRate(learningRate), weights(n_features, 0.0) {
}

void Perceptron::train(std::vector<std::vector<double>>& inputs, std::vector<double>& labels, int epochs) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        // Reset formatting flags at the start of each epoch
        std::cout.unsetf(std::ios_base::floatfield);
        
        std::cout << "\n\033[1;36mEpoch " << std::setw(2) << (epoch + 1) << "/" << epochs << "\033[0m" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << " Input      | Prediction | Target | Status" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        int correctCount = 0;

        for (size_t i = 0; i < inputs.size(); i++) {
            double weightedSum = bias;
            for (int j = 0; j < n_features; j++) {
                weightedSum += weights[j] * inputs[i][j];
            }

            int predicted = activationFunction(weightedSum);
            int error = labels[i] - predicted;

            if (error != 0) {
                for (size_t j = 0; j < weights.size(); j++) {
                    weights[j] += learningRate * error * inputs[i][j];
                }
                bias += learningRate * error;
            } else {
                correctCount++;
            }

            // Format Input String
            std::stringstream ss;
            ss << "[";
            for(size_t j=0; j < inputs[i].size(); j++) {
                ss << inputs[i][j] << (j < inputs[i].size()-1 ? ", " : "");
            }
            ss << "]";

            // Print Row with fixed widths
            std::cout << " " << std::left << std::setw(11) << ss.str() << "| ";
            
            // Prediction Column
            std::cout << "    " << predicted << "      | "; 
            
            // Target Column
            std::cout << "   " << (int)labels[i] << "    | ";

            if (error == 0.0) {
                std::cout << "\033[32m[OK]\033[0m" << std::endl;
            }
            else {
                std::cout << "\033[31m[FAIL]\033[0m" << std::endl;
            }
        }
        
        double accuracy = (double)correctCount / inputs.size() * 100.0;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << " Accuracy: " << std::fixed << std::setprecision(1) << accuracy << "%" << std::endl;
    }
}

double Perceptron::predict(std::vector<double> input) {
    double weightedSum = bias;
    for (int i = 0; i < n_features; i++) {
        weightedSum += weights[i] * input[i];
    }
    return activationFunction(weightedSum);
}

double Perceptron::activationFunction(double weightSum) const {
    return weightSum > 0.0 ? 1.0 : 0.0;
}

std::vector<double> Perceptron::getWeights() {
    return weights;
}

double Perceptron::getBias() {
    return bias;
}