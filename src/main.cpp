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
	
	// Labels AND
	std::vector<double> labels = { 0, 0, 0, 1 };

	// Train
	perceptron.train(inputs, labels, 10);

	// Prediction
	std::vector<double> userInput;
	userInput.resize(2);
	std::cout << "Enter your values (x y): ";
	std::cin >> userInput[0] >> userInput[1];

	std::cout << "Prediction of [" << userInput[0] << " " << userInput[1] << "]: " << perceptron.predict(userInput) << std::endl;

	return 0;
}