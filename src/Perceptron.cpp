#include <iostream>
#include <vector>
#include "../include/Perceptron.h"

Perceptron::Perceptron(int n_features) : n_features(n_features), weights(n_features, 0) {
	for (auto w : weights) {
		std::cout << w << std::endl;
	}
}

void Perceptron::train() {

}

double Perceptron::predict() {
	return 0.0;
}