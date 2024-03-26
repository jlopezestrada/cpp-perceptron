#include <iostream>
#include <vector>
#include "../include/Perceptron.h"

int main() {
	// Initializate Perceptron class
	Perceptron perceptron(8);

	// Inputs
	std::vector<std::vector<double>> inputs = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1}
	};
	
	// AND function labels
	std::vector<int> labels = { 0, 1, 1, 1 };

	// Training of the perceptron
	int epocs;
	std::cout << "Number of epocs: ";
	std::cin >> epocs;
	for (int i = 0; i < epocs; i++) {
		std::cout << "Epoch: " << i << std::endl;
		for (size_t j = 0; j < inputs.size(); j++) {
			// perceptron.train(inputs[j], labels[j]); TODO
			std::cout << "\t - Training process for input: " << inputs[j][0] << " label: " << labels[j] << std::endl;
			// more information...
		}
	}

	// Output of the trained perceptron
	std::cout << "Input values: " << inputs[0][0] << ", " << inputs[0][1] << std::endl;
	std::cout << "Label value: 0" << std::endl;
	// std::cout << "Predicted output: " << perceptron.predict(inputs[0]); TODO
	std::cout << "Predicted output: 0";

	return 0;
}