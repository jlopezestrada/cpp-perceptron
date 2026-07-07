#include "DecisionBoundaryVisualizer.h"

#include <cmath>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void validateOptions(const DecisionBoundaryOptions& options) {
    if (!std::isfinite(options.xMin) || !std::isfinite(options.xMax) ||
        !std::isfinite(options.yMin) || !std::isfinite(options.yMax)) {
        throw std::invalid_argument("decision boundary bounds must be finite");
    }

    if (options.xMin >= options.xMax || options.yMin >= options.yMax) {
        throw std::invalid_argument("decision boundary min bounds must be less than max bounds");
    }

    if (options.width < 2 || options.height < 2) {
        throw std::invalid_argument("decision boundary width and height must be at least 2");
    }
}

void validateSamples(const std::vector<std::vector<double>>& samples, const std::vector<double>& labels) {
    if (samples.size() != labels.size()) {
        throw std::invalid_argument("decision boundary samples and labels must contain the same number of entries");
    }

    for (std::size_t i = 0; i < samples.size(); i++) {
        if (samples[i].size() != 2) {
            throw std::invalid_argument("decision boundary samples must contain exactly two values");
        }

        if (!std::isfinite(samples[i][0]) || !std::isfinite(samples[i][1])) {
            throw std::invalid_argument("decision boundary sample values must be finite");
        }

        if (labels[i] != 0.0 && labels[i] != 1.0) {
            throw std::invalid_argument("decision boundary labels must be either 0.0 or 1.0");
        }
    }
}

std::size_t nearestColumn(double x, const DecisionBoundaryOptions& options, double xStep) {
    const double column = std::round((x - options.xMin) / xStep);
    return static_cast<std::size_t>(column);
}

std::size_t nearestRowFromBottom(double y, const DecisionBoundaryOptions& options, double yStep) {
    const double row = std::round((y - options.yMin) / yStep);
    return static_cast<std::size_t>(row);
}
}

DecisionBoundaryVisualizer::DecisionBoundaryVisualizer(DecisionBoundaryOptions options)
    : options(options) {
}

void DecisionBoundaryVisualizer::render(const Perceptron& perceptron,
                                        const std::vector<std::vector<double>>& samples,
                                        const std::vector<double>& labels,
                                        std::ostream& output) const {
    validateOptions(options);
    validateSamples(samples, labels);

    const std::vector<double>& weights = perceptron.getWeights();
    if (weights.size() != 2) {
        throw std::invalid_argument("decision boundary visualization requires a two-feature perceptron");
    }

    if (!std::isfinite(weights[0]) || !std::isfinite(weights[1]) || !std::isfinite(perceptron.getBias())) {
        throw std::invalid_argument("decision boundary weights and bias must be finite");
    }

    if (weights[0] == 0.0 && weights[1] == 0.0) {
        throw std::invalid_argument("decision boundary cannot be drawn when both feature weights are zero");
    }

    const double xStep = (options.xMax - options.xMin) / static_cast<double>(options.width - 1);
    const double yStep = (options.yMax - options.yMin) / static_cast<double>(options.height - 1);
    const double boundaryThreshold = 0.5 * (std::abs(weights[0]) * xStep + std::abs(weights[1]) * yStep);

    std::vector<std::string> grid(options.height, std::string(options.width, '.'));

    for (std::size_t row = 0; row < options.height; row++) {
        const double y = options.yMax - static_cast<double>(row) * yStep;

        for (std::size_t column = 0; column < options.width; column++) {
            const double x = options.xMin + static_cast<double>(column) * xStep;
            const double prediction = perceptron.predict({x, y});
            char marker = prediction == 1.0 ? '+' : '.';

            const double boundaryValue = weights[0] * x + weights[1] * y + perceptron.getBias();
            if (std::abs(boundaryValue) <= boundaryThreshold) {
                marker = '#';
            }

            grid[row][column] = marker;
        }
    }

    for (std::size_t i = 0; i < samples.size(); i++) {
        const double x = samples[i][0];
        const double y = samples[i][1];
        if (x < options.xMin || x > options.xMax || y < options.yMin || y > options.yMax) {
            continue;
        }

        const std::size_t column = nearestColumn(x, options, xStep);
        const std::size_t rowFromBottom = nearestRowFromBottom(y, options, yStep);
        const std::size_t row = options.height - 1 - rowFromBottom;
        grid[row][column] = labels[i] == 1.0 ? '1' : '0';
    }

    for (const std::string& row : grid) {
        output << row << '\n';
    }

    output << "Legend: . class 0, + class 1, # boundary, 0/1 samples\n";
}
