#ifndef DECISION_BOUNDARY_VISUALIZER_H
#define DECISION_BOUNDARY_VISUALIZER_H

#include "Perceptron.h"

#include <cstddef>
#include <iosfwd>
#include <vector>

struct DecisionBoundaryOptions {
    double xMin = -0.25;
    double xMax = 1.25;
    double yMin = -0.25;
    double yMax = 1.25;
    std::size_t width = 41;
    std::size_t height = 21;
};

class DecisionBoundaryVisualizer {
public:
    explicit DecisionBoundaryVisualizer(DecisionBoundaryOptions options = {});

    void render(const Perceptron& perceptron,
                const std::vector<std::vector<double>>& samples,
                const std::vector<double>& labels,
                std::ostream& output) const;

private:
    DecisionBoundaryOptions options;
};

#endif
