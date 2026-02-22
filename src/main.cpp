#include <iostream>
#include <vector>
#include "../include/Perceptron.h"

int main() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   C++ Perceptron - Logic Gate Demo" << std::endl;
    std::cout << "========================================\n" << std::endl;

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

    std::cout << "Training Model (Logical AND)..." << std::endl;

	// Train
	perceptron.train(inputs, labels, 10);

    std::cout << "\nTraining Complete.\n" << std::endl;

	// Prediction
	std::vector<double> userInput;
	userInput.resize(2);
	std::cout << "Test the model:" << std::endl;
    std::cout << "Enter two values (0 or 1) separated by space: ";
	std::cin >> userInput[0] >> userInput[1];

    double result = perceptron.predict(userInput);
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Prediction for [" << userInput[0] << ", " << userInput[1] << "]: " 
              << "\033[1m" << result << "\033[0m" << std::endl;
	std::cout << "----------------------------------------\n" << std::endl;

	return 0;
}