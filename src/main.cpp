#include "DecisionBoundaryVisualizer.h"
#include "Perceptron.h"

#include <exception>
#include <iostream>
#include <vector>

int main() {
    try {
        std::cout << "\n========================================" << std::endl;
        std::cout << "   C++ Perceptron - Logic Gate Demo" << std::endl;
        std::cout << "========================================\n" << std::endl;

        Perceptron perceptron(2, 0.001);

        const std::vector<std::vector<double>> inputs = {
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1}
        };
        const std::vector<double> labels = {0, 0, 0, 1};

        std::cout << "Training Model (Logical AND)..." << std::endl;
        perceptron.train(inputs, labels, 10);

        std::cout << "\nTraining Complete.\n" << std::endl;
        std::cout << "Learned truth table:" << std::endl;

        for (std::size_t i = 0; i < inputs.size(); i++) {
            std::cout << "[" << inputs[i][0] << ", " << inputs[i][1] << "] -> "
                      << perceptron.predict(inputs[i]) << " (target " << labels[i] << ")" << std::endl;
        }

        std::cout << "\nDecision boundary:" << std::endl;
        DecisionBoundaryVisualizer visualizer;
        visualizer.render(perceptron, inputs, labels, std::cout);

        std::vector<double> userInput(2);
        std::cout << "\nTest the model:" << std::endl;
        std::cout << "Enter two values (0 or 1) separated by space: ";

        if (!(std::cin >> userInput[0] >> userInput[1])) {
            std::cerr << "Invalid input. Please enter two numeric values." << std::endl;
            return 1;
        }

        if ((userInput[0] != 0.0 && userInput[0] != 1.0) || (userInput[1] != 0.0 && userInput[1] != 1.0)) {
            std::cerr << "Invalid input. Values must be either 0 or 1." << std::endl;
            return 1;
        }

        double result = perceptron.predict(userInput);
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Prediction for [" << userInput[0] << ", " << userInput[1] << "]: "
                  << result << std::endl;
        std::cout << "----------------------------------------\n" << std::endl;
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }

    return 0;
}
