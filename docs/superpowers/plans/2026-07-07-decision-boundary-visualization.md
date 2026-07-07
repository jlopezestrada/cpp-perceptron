# Decision Boundary Visualization Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a dependency-free ASCII decision boundary visualizer for two-feature perceptrons and display it in the existing AND-gate CLI demo.

**Architecture:** Add `DecisionBoundaryVisualizer` as a focused library component beside `Perceptron`. It renders a deterministic ASCII plot to `std::ostream` using the perceptron's public `predict`, `getWeights`, and `getBias` APIs; the CLI only constructs the visualizer and calls `render` after training.

**Tech Stack:** C++17, CMake 3.16+, standard library only, existing custom C++ test executable, git conventional commits.

## Global Constraints

- C++ standard remains C++17.
- Do not add external plotting, image, GUI, terminal UI, or test dependencies.
- Support only two-feature perceptrons for visualization.
- Preserve the existing CMake, CLI, and test style.
- Do not redesign perceptron training or prediction.
- Throw `std::invalid_argument` for invalid visualizer inputs.
- Keep commits focused and use conventional commit messages.

---

## File Structure

- Create `include/DecisionBoundaryVisualizer.h`: public options struct and visualizer class declaration.
- Create `src/DecisionBoundaryVisualizer.cpp`: validation, coordinate mapping, boundary detection, sample overlay, and stream rendering.
- Modify `CMakeLists.txt`: compile the new source into the existing `perceptron` library.
- Modify `tests/PerceptronTests.cpp`: include the new header and add focused rendering/validation tests to the existing custom test binary.
- Modify `src/main.cpp`: include the visualizer and render the decision boundary after printing the learned truth table.

---

### Task 1: Add Visualizer API And Build Integration

**Files:**
- Create: `include/DecisionBoundaryVisualizer.h`
- Create: `src/DecisionBoundaryVisualizer.cpp`
- Modify: `CMakeLists.txt:17`
- Modify: `tests/PerceptronTests.cpp:1-8, 68-73`

**Interfaces:**
- Consumes: `Perceptron::predict(const std::vector<double>&) const`, `Perceptron::getWeights() const`, `Perceptron::getBias() const`
- Produces: `struct DecisionBoundaryOptions` with members `xMin`, `xMax`, `yMin`, `yMax`, `width`, `height`
- Produces: `class DecisionBoundaryVisualizer` with `explicit DecisionBoundaryVisualizer(DecisionBoundaryOptions options = {})` and `void render(const Perceptron& perceptron, const std::vector<std::vector<double>>& samples, const std::vector<double>& labels, std::ostream& output) const`

- [ ] **Step 1: Write the failing API/build test**

Modify `tests/PerceptronTests.cpp` to include the new header and add a test that constructs the visualizer and calls `render`.

```cpp
#include "DecisionBoundaryVisualizer.h"
```

Add `<sstream>` to the standard includes:

```cpp
#include <sstream>
```

Add this test function inside the anonymous namespace after `rejectsInvalidPredictionInput()`:

```cpp
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
```

Call it from `main()` after `rejectsInvalidPredictionInput();`:

```cpp
        rendersDecisionBoundaryPlot();
```

- [ ] **Step 2: Run the test to verify it fails**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`

Expected: build fails because `DecisionBoundaryVisualizer.h` does not exist.

- [ ] **Step 3: Add the public header**

Create `include/DecisionBoundaryVisualizer.h`:

```cpp
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
```

- [ ] **Step 4: Add minimal implementation and CMake wiring**

Create `src/DecisionBoundaryVisualizer.cpp` with the smallest implementation that compiles and emits all markers required by the current test:

```cpp
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
```

Modify `CMakeLists.txt` line 17 from:

```cmake
add_library(perceptron src/Perceptron.cpp)
```

to:

```cmake
add_library(perceptron src/Perceptron.cpp src/DecisionBoundaryVisualizer.cpp)
```

- [ ] **Step 5: Run tests to verify the API/build task passes**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`

Expected: build succeeds and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 6: Commit Task 1**

Run:

```bash
git add CMakeLists.txt include/DecisionBoundaryVisualizer.h src/DecisionBoundaryVisualizer.cpp tests/PerceptronTests.cpp
git commit -m "feat(boundary): add visualizer api"
```

---

### Task 2: Implement Deterministic Rendering And Validation

**Files:**
- Modify: `src/DecisionBoundaryVisualizer.cpp`
- Modify: `tests/PerceptronTests.cpp`

**Interfaces:**
- Consumes: `DecisionBoundaryVisualizer::render(...)` from Task 1
- Produces: deterministic ASCII rendering with region markers `.`, `+`, boundary marker `#`, sample overlays `0`/`1`, and legend `Legend: . class 0, + class 1, # boundary, 0/1 samples`
- Produces: `std::invalid_argument` validation for invalid options, non-2D perceptrons, mismatched labels, invalid samples, invalid labels, and zero learned feature weights

- [ ] **Step 1: Add validation tests that must fail against the Task 1 stub**

Add `<limits>` to `tests/PerceptronTests.cpp` includes:

```cpp
#include <limits>
```

Add these test functions inside the anonymous namespace after `rendersDecisionBoundaryPlot()`:

```cpp
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
```

Call these tests from `main()` after `rendersDecisionBoundaryPlot();`:

```cpp
        rejectsNonTwoFeaturePerceptronForVisualization();
        rejectsInvalidVisualizationOptions();
        rejectsInvalidVisualizationSamples();
        rejectsUntrainedFlatDecisionBoundary();
```

- [ ] **Step 2: Run tests to verify validation tests fail**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`

Expected: tests fail with a message containing `expected std::invalid_argument`.

- [ ] **Step 3: Replace the stub with full rendering and validation**

Replace all contents of `src/DecisionBoundaryVisualizer.cpp` with:

```cpp
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
```

- [ ] **Step 4: Run tests to verify validation and rendering pass**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`

Expected: build succeeds and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 5: Run formatting/compile warning verification through the normal build**

Run: `cmake --build build`

Expected: build exits 0 with no `-Wall -Wextra -pedantic` warnings.

- [ ] **Step 6: Commit Task 2**

Run:

```bash
git add src/DecisionBoundaryVisualizer.cpp tests/PerceptronTests.cpp
git commit -m "feat(boundary): render ascii decision boundary"
```

---

### Task 3: Show The Visualization In The CLI Demo

**Files:**
- Modify: `src/main.cpp`
- Optionally Modify: `README.md`

**Interfaces:**
- Consumes: `DecisionBoundaryVisualizer` and `DecisionBoundaryOptions` from Tasks 1 and 2
- Produces: CLI output that prints the existing truth table, then a titled ASCII decision boundary plot before the interactive prompt

- [ ] **Step 1: Add a CLI smoke test command before changing the CLI**

Run: `printf "1 1\n" | ./build/cpp-perceptron`

Expected before this task: output does not contain `Decision boundary:`. Keep this result as the baseline that the CLI does not yet expose the feature.

- [ ] **Step 2: Integrate the visualizer into `src/main.cpp`**

Add the new include at the top:

```cpp
#include "DecisionBoundaryVisualizer.h"
#include "Perceptron.h"
```

After the truth-table loop and before `std::vector<double> userInput(2);`, insert:

```cpp
        std::cout << "\nDecision boundary:" << std::endl;
        DecisionBoundaryVisualizer visualizer;
        visualizer.render(perceptron, inputs, labels, std::cout);
```

- [ ] **Step 3: Build and test the CLI integration**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`

Expected: build succeeds and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 4: Verify CLI output includes the visualization**

Run: `printf "1 1\n" | ./build/cpp-perceptron`

Expected: output contains `Decision boundary:`, multiple plot rows containing `.`, `+`, `#`, `0`, and `1`, and `Legend: . class 0, + class 1, # boundary, 0/1 samples` before the interactive prediction result.

- [ ] **Step 5: Update README only if the CLI output description is now misleading**

If `README.md` still accurately says the demo trains the model to learn the logical AND gate, do not change it. If adding a short mention is desired, add this sentence after line 5:

```markdown
The demo also prints an ASCII decision boundary for the learned two-feature model.
```

If `README.md` is changed, include it in the Task 3 commit. If not changed, leave it unstaged.

- [ ] **Step 6: Commit Task 3**

Run:

```bash
git add src/main.cpp README.md
git commit -m "feat(cli): show decision boundary visualization"
```

If `README.md` was not changed, use:

```bash
git add src/main.cpp
git commit -m "feat(cli): show decision boundary visualization"
```

---

## Final Verification

- [ ] Run: `git status --short`

Expected: no unrelated changes staged. If there are uncommitted intentional changes, inspect them with `git diff` before proceeding.

- [ ] Run: `cmake --build build`

Expected: exit 0 with no compiler warnings.

- [ ] Run: `ctest --test-dir build --output-on-failure`

Expected: `100% tests passed, 0 tests failed out of 1`.

- [ ] Run: `printf "1 1\n" | ./build/cpp-perceptron`

Expected: output includes `Decision boundary:` and `Legend: . class 0, + class 1, # boundary, 0/1 samples`, then predicts `1` for `[1, 1]`.

- [ ] Run: `git log --oneline -5`

Expected: the three feature commits appear above the planning/spec commits.
