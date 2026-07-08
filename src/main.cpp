#include "DecisionBoundaryVisualizer.h"
#include "Perceptron.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr double kLearningRate = 0.001;
constexpr int kTrainingEpochs = 25;

struct LogicGate {
    std::string name;
    std::vector<double> labels;
    bool linearlySeparable = true;
    std::string note;
};

const std::vector<std::vector<double>>& binaryInputs() {
    static const std::vector<std::vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    return inputs;
}

std::vector<LogicGate> logicGates() {
    return {
        {"AND", {0, 0, 0, 1}, true, "Outputs 1 only when both inputs are 1."},
        {"OR", {0, 1, 1, 1}, true, "Outputs 1 when at least one input is 1."},
        {"NAND", {1, 1, 1, 0}, true, "Outputs the inverse of AND."},
        {"NOR", {1, 0, 0, 0}, true, "Outputs the inverse of OR."},
        {"XOR", {0, 1, 1, 0}, false, "Outputs 1 when exactly one input is 1."}
    };
}

int asBinary(double value) {
    return value == 0.0 ? 0 : 1;
}

void printExpectedTruthTable(const LogicGate& gate) {
    const std::vector<std::vector<double>>& inputs = binaryInputs();

    std::cout << "Input | Target\n";
    std::cout << "------+-------\n";

    for (std::size_t i = 0; i < inputs.size(); i++) {
        std::cout << "  " << asBinary(inputs[i][0]) << " " << asBinary(inputs[i][1])
                  << " |   " << asBinary(gate.labels[i]) << '\n';
    }
}

void demonstrateLearnableGate(const LogicGate& gate, bool showDecisionBoundary) {
    const std::vector<std::vector<double>>& inputs = binaryInputs();
    Perceptron perceptron(2, kLearningRate);
    perceptron.train(inputs, gate.labels, kTrainingEpochs);

    bool allPredictionsMatch = true;

    std::cout << "\n" << gate.name << " gate\n";
    std::cout << gate.note << '\n';
    std::cout << "Linearly separable: yes\n";
    std::cout << "Input | Target | Prediction\n";
    std::cout << "------+--------+-----------\n";

    for (std::size_t i = 0; i < inputs.size(); i++) {
        const double prediction = perceptron.predict(inputs[i]);
        allPredictionsMatch = allPredictionsMatch && prediction == gate.labels[i];

        std::cout << "  " << asBinary(inputs[i][0]) << " " << asBinary(inputs[i][1])
                  << " |   " << asBinary(gate.labels[i])
                  << "    |     " << asBinary(prediction) << '\n';
    }

    std::cout << "Result: " << (allPredictionsMatch ? "learned" : "not learned") << "\n";

    if (showDecisionBoundary) {
        std::cout << "\nDecision boundary for " << gate.name << ":\n";
        DecisionBoundaryVisualizer visualizer;
        visualizer.render(perceptron, inputs, gate.labels, std::cout);
    }
}

void explainNonLinearlySeparableGate(const LogicGate& gate) {
    std::cout << "\n" << gate.name << " gate\n";
    std::cout << gate.note << '\n';
    std::cout << "Linearly separable: no\n";
    printExpectedTruthTable(gate);
    std::cout << "Result: not learnable by a single perceptron\n";
    std::cout << "Why: the 1 labels are on opposite corners of the input square, "
              << "so one straight decision boundary cannot separate them from the 0 labels.\n";
}
}

int main() {
    try {
        std::cout << "\n========================================\n";
        std::cout << "   C++ Perceptron - Logic Gate Demo\n";
        std::cout << "========================================\n";

        const std::vector<LogicGate> gates = logicGates();
        bool boundaryRendered = false;

        for (const LogicGate& gate : gates) {
            if (gate.linearlySeparable) {
                demonstrateLearnableGate(gate, !boundaryRendered);
                boundaryRendered = true;
            } else {
                explainNonLinearlySeparableGate(gate);
            }
        }

        std::cout << "\nRead docs/perceptron-technical-details.md for the training rule and XOR limitation.\n";
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << '\n';
        return 1;
    }

    return 0;
}
