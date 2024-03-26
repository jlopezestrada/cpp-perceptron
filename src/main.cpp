#include <iostream>
#include <vector>
#include "../include/Perceptron.h"

int main() {
	// Initializate Perceptron class
	Perceptron perceptron(2);

	// Inputs
	std::vector<std::vector<double>> input = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1}
	};
	
	// Labels
	std::vector<double> labels = { 0, 0, 0, 1 };

	// Train
	perceptron.train(input, labels, 10);

	// Prediction
	// TODO

	return 0;
}