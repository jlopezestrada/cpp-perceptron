#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
class Perceptron {
private:
	int n_features = 0;
	std::vector<double> weights;
public:
	explicit Perceptron(int n_features);
	void train();
	double predict();
};

#endif