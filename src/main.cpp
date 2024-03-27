#include <iostream>
#include <vector>
#include "../include/Perceptron.h"

int main() {
	Perceptron perceptron(2, 0.001);

	// Inputs
	std::vector<std::vector<double>> inputs = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1}
	};
	
	// Labels
	std::vector<double> labels = { 0, 0, 0, 1 };

	// Train
	perceptron.train(inputs, labels, 10);

	// Prediction
	// TODO

	return 0;
}