#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <cstddef>
#include <vector>

class Perceptron {
private:
    std::size_t n_features = 0;
    double bias = 0.0;
    double learningRate = 0.0;
    std::vector<double> weights;
    double activationFunction(double weightSum) const;
    void validateInput(const std::vector<double>& input) const;

public:
    explicit Perceptron(std::size_t n_features, double learningRate);
    void train(const std::vector<std::vector<double>>& inputs, const std::vector<double>& labels, int epochs);
    double predict(const std::vector<double>& input) const;
    const std::vector<double>& getWeights() const;
    double getBias() const;
};

#endif
