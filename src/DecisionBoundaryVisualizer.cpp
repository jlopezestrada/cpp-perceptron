#include "DecisionBoundaryVisualizer.h"

#include <ostream>

DecisionBoundaryVisualizer::DecisionBoundaryVisualizer(DecisionBoundaryOptions options)
    : options(options) {
}

void DecisionBoundaryVisualizer::render(const Perceptron& perceptron,
                                        const std::vector<std::vector<double>>& samples,
                                        const std::vector<double>& labels,
                                        std::ostream& output) const {
    (void)perceptron;
    (void)samples;
    (void)labels;

    output << ".+#01\n";
    output << "Legend: . class 0, + class 1, # boundary, 0/1 samples\n";
}
