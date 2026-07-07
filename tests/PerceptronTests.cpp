#include "Perceptron.h"
#include "DecisionBoundaryVisualizer.h"

#include <exception>
#include <iostream>
#include <limits>
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

std::vector<std::string> plotRowsBeforeLegend(const std::string& plot) {
    std::vector<std::string> rows;
    std::istringstream stream(plot);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find("Legend:") == 0) {
            break;
        }

        rows.push_back(line);
    }

    return rows;
}

bool rowsContain(const std::vector<std::string>& rows, char marker) {
    for (const std::string& row : rows) {
        if (row.find(marker) != std::string::npos) {
            return true;
        }
    }

    return false;
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
    const std::vector<std::string> plotRows = plotRowsBeforeLegend(plot);
    expect(rowsContain(plotRows, '.'), "plot rows should include class 0 region markers");
    expect(rowsContain(plotRows, '+'), "plot rows should include class 1 region markers");
    expect(rowsContain(plotRows, '#'), "plot rows should include boundary markers");
    expect(rowsContain(plotRows, '0'), "plot rows should include class 0 sample markers");
    expect(rowsContain(plotRows, '1'), "plot rows should include class 1 sample markers");
    expect(plot.find("Legend: . class 0, + class 1, # boundary, 0/1 samples") != std::string::npos,
           "plot should include legend");
}

void placesSamplesOnNearestGridCell() {
    Perceptron perceptron(2, 0.001);
    perceptron.train({{0, 0}, {1, 1}}, {0, 1}, 10);

    DecisionBoundaryOptions options;
    options.xMin = 0.0;
    options.xMax = 1.0;
    options.yMin = 0.0;
    options.yMax = 1.0;
    options.width = 3;
    options.height = 3;

    DecisionBoundaryVisualizer visualizer(options);
    std::ostringstream output;
    visualizer.render(perceptron, {{0.49, 0.49}}, {1}, output);

    const std::vector<std::string> plotRows = plotRowsBeforeLegend(output.str());
    expect(plotRows.size() == 3, "nearest-cell plot should have three rows");
    expect(plotRows[1].size() == 3, "nearest-cell plot row should have three columns");
    expect(plotRows[1][1] == '1', "in-bounds sample should be placed on nearest grid cell");
}

void skipsSamplesOutsideVisualizationBounds() {
    Perceptron perceptron(2, 0.001);
    perceptron.train({{0, 0}, {1, 1}}, {0, 1}, 10);

    DecisionBoundaryOptions options;
    options.xMin = 0.0;
    options.xMax = 1.0;
    options.yMin = 0.0;
    options.yMax = 1.0;
    options.width = 3;
    options.height = 3;

    DecisionBoundaryVisualizer visualizer(options);
    std::ostringstream output;
    visualizer.render(perceptron, {{1.5, 0.5}}, {0}, output);

    const std::vector<std::string> plotRows = plotRowsBeforeLegend(output.str());
    expect(!rowsContain(plotRows, '0'), "out-of-bounds samples should be skipped");
}

void rejectsNonTwoFeaturePerceptronForVisualization() {
    Perceptron perceptron(3, 0.001);
    DecisionBoundaryVisualizer visualizer;
    std::ostringstream output;

    expectInvalidArgument([&] { visualizer.render(perceptron, {{0, 0}}, {0}, output); },
                          "reject non-2D perceptron visualization");
}

void rejectsInvalidVisualizationOptions() {
    Perceptron perceptron(2, 0.001);
    std::ostringstream output;

    DecisionBoundaryOptions reversedBounds;
    reversedBounds.xMin = 1.0;
    reversedBounds.xMax = 0.0;
    expectInvalidArgument([&] {
        DecisionBoundaryVisualizer visualizer(reversedBounds);
        visualizer.render(perceptron, {{0, 0}}, {0}, output);
    }, "reject reversed x bounds");

    DecisionBoundaryOptions nonFiniteBounds;
    nonFiniteBounds.yMax = std::numeric_limits<double>::infinity();
    expectInvalidArgument([&] {
        DecisionBoundaryVisualizer visualizer(nonFiniteBounds);
        visualizer.render(perceptron, {{0, 0}}, {0}, output);
    }, "reject non-finite bounds");

    DecisionBoundaryOptions narrowGrid;
    narrowGrid.width = 1;
    expectInvalidArgument([&] {
        DecisionBoundaryVisualizer visualizer(narrowGrid);
        visualizer.render(perceptron, {{0, 0}}, {0}, output);
    }, "reject grid width less than 2");

    DecisionBoundaryOptions shortGrid;
    shortGrid.height = 1;
    expectInvalidArgument([&] {
        DecisionBoundaryVisualizer visualizer(shortGrid);
        visualizer.render(perceptron, {{0, 0}}, {0}, output);
    }, "reject grid height less than 2");
}

void rejectsInvalidVisualizationSamples() {
    Perceptron perceptron(2, 0.001);
    const std::vector<std::vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const std::vector<double> labels = {0, 0, 0, 1};
    perceptron.train(inputs, labels, 10);

    DecisionBoundaryVisualizer visualizer;
    std::ostringstream output;

    expectInvalidArgument([&] { visualizer.render(perceptron, {{0, 0}}, {0, 1}, output); },
                          "reject mismatched visualization labels");
    expectInvalidArgument([&] { visualizer.render(perceptron, {{0}}, {0}, output); },
                          "reject one-dimensional visualization sample");
    expectInvalidArgument([&] { visualizer.render(perceptron, {{0, 0, 0}}, {0}, output); },
                          "reject three-dimensional visualization sample");
    expectInvalidArgument([&] {
        visualizer.render(perceptron, {{std::numeric_limits<double>::quiet_NaN(), 0}}, {0}, output);
    }, "reject non-finite visualization sample");
    expectInvalidArgument([&] { visualizer.render(perceptron, {{0, 0}}, {2}, output); },
                          "reject non-binary visualization label");
}

void rejectsUntrainedFlatDecisionBoundary() {
    Perceptron perceptron(2, 0.001);
    DecisionBoundaryVisualizer visualizer;
    std::ostringstream output;

    expectInvalidArgument([&] { visualizer.render(perceptron, {{0, 0}}, {0}, output); },
                          "reject zero learned feature weights");
}
}

int main() {
    try {
        learnsAndGate();
        rejectsInvalidConstructorArguments();
        rejectsInvalidTrainingData();
        rejectsInvalidPredictionInput();
        rendersDecisionBoundaryPlot();
        placesSamplesOnNearestGridCell();
        skipsSamplesOutsideVisualizationBounds();
        rejectsNonTwoFeaturePerceptronForVisualization();
        rejectsInvalidVisualizationOptions();
        rejectsInvalidVisualizationSamples();
        rejectsUntrainedFlatDecisionBoundary();
    } catch (const std::exception& exception) {
        std::cerr << "Test failed: " << exception.what() << '\n';
        return 1;
    }

    std::cout << "All perceptron tests passed\n";
    return 0;
}
