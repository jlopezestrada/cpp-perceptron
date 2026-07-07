#include "Perceptron.h"
#include "DecisionBoundaryVisualizer.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template <typename Function>
void expectInvalidArgument(Function function, const std::string& message) {
    try {
        function();
    } catch (const std::invalid_argument&) {
        return;
    } catch (const std::exception& exception) {
        throw std::runtime_error(message + ": unexpected exception: " + exception.what());
    }

    throw std::runtime_error(message + ": expected std::invalid_argument");
}

void learnsAndGate() {
    Perceptron perceptron(2, 0.001);
    const std::vector<std::vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const std::vector<double> labels = {0, 0, 0, 1};

    perceptron.train(inputs, labels, 10);

    expect(perceptron.predict({0, 0}) == 0.0, "AND: 0 && 0 should be 0");
    expect(perceptron.predict({0, 1}) == 0.0, "AND: 0 && 1 should be 0");
    expect(perceptron.predict({1, 0}) == 0.0, "AND: 1 && 0 should be 0");
    expect(perceptron.predict({1, 1}) == 1.0, "AND: 1 && 1 should be 1");
}

void rejectsInvalidConstructorArguments() {
    expectInvalidArgument([] { Perceptron perceptron(0, 0.001); }, "reject zero feature count");
    expectInvalidArgument([] { Perceptron perceptron(2, 0.0); }, "reject zero learning rate");
    expectInvalidArgument([] { Perceptron perceptron(2, -0.1); }, "reject negative learning rate");
}

void rejectsInvalidTrainingData() {
    Perceptron perceptron(2, 0.001);
    const std::vector<std::vector<double>> inputs = {{0, 0}, {1, 1}};
    const std::vector<double> labels = {0, 1};

    expectInvalidArgument([&] { perceptron.train({}, {}, 1); }, "reject empty training data");
    expectInvalidArgument([&] { perceptron.train(inputs, {1}, 1); }, "reject mismatched labels");
    expectInvalidArgument([&] { perceptron.train({{1}}, {1}, 1); }, "reject wrong input dimension");
    expectInvalidArgument([&] { perceptron.train(inputs, {0, 2}, 1); }, "reject non-binary label");
    expectInvalidArgument([&] { perceptron.train(inputs, labels, 0); }, "reject zero epochs");
}

void rejectsInvalidPredictionInput() {
    Perceptron perceptron(2, 0.001);

    expectInvalidArgument([&] { perceptron.predict({1}); }, "reject short prediction input");
    expectInvalidArgument([&] { perceptron.predict({1, 0, 1}); }, "reject long prediction input");
}

void rendersDecisionBoundaryPlot() {
    Perceptron perceptron(2, 0.001);
    const std::vector<std::vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const std::vector<double> labels = {0, 0, 0, 1};

    perceptron.train(inputs, labels, 10);

    DecisionBoundaryOptions options;
    options.xMin = -0.25;
    options.xMax = 1.25;
    options.yMin = -0.25;
    options.yMax = 1.25;
    options.width = 11;
    options.height = 7;

    DecisionBoundaryVisualizer visualizer(options);
    std::ostringstream output;
    visualizer.render(perceptron, inputs, labels, output);

    const std::string plot = output.str();
    expect(plot.find('.') != std::string::npos, "plot should include class 0 region markers");
    expect(plot.find('+') != std::string::npos, "plot should include class 1 region markers");
    expect(plot.find('#') != std::string::npos, "plot should include boundary markers");
    expect(plot.find('0') != std::string::npos, "plot should include class 0 sample markers");
    expect(plot.find('1') != std::string::npos, "plot should include class 1 sample markers");
    expect(plot.find("Legend: . class 0, + class 1, # boundary, 0/1 samples") != std::string::npos,
           "plot should include legend");
}
}

int main() {
    try {
        learnsAndGate();
        rejectsInvalidConstructorArguments();
        rejectsInvalidTrainingData();
        rejectsInvalidPredictionInput();
        rendersDecisionBoundaryPlot();
    } catch (const std::exception& exception) {
        std::cerr << "Test failed: " << exception.what() << '\n';
        return 1;
    }

    std::cout << "All perceptron tests passed\n";
    return 0;
}
