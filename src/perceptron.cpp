#include <iostream>
#include <vector>

class Perceptron {
private:
	int n_features;
	std::vector<double> weights;
public:
	Perceptron(int n_features) {
		this->n_features = n_features;
	}

	void train() {
		std::cout << "Training... ";
	}

	double predict() {
		std::cout << "Predict... ";
	}
};