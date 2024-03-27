#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
class Perceptron {
private:
	int n_features = 0;
	double bias = 0.0;
	double learningRate = 0.0;
	std::vector<double> weights;
	int activationFunction(double weightSum) const;

public:
	explicit Perceptron(int n_features, double learningRate);
	void train(std::vector<std::vector<double>>& inputs, std::vector<double>& labels, int epochs);
	double predict();
};

#endif