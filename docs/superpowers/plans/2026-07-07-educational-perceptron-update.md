# Educational Perceptron Update Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Expand the repository into a polished educational perceptron resource by adding history docs, technical docs, a multi-gate CLI demo, and tests for learnable gates plus the XOR limitation.

**Architecture:** Keep `Perceptron` and `DecisionBoundaryVisualizer` unchanged. Add documentation under `docs/`, expand `src/main.cpp` with local logic-gate demo helpers, and extend the existing dependency-free test binary with logic-gate coverage and a deterministic XOR non-separability proof.

**Tech Stack:** C++17, CMake, standard library only, custom C++ test binary, Markdown documentation.

## Global Constraints

- Do not add CI or GitHub Actions.
- Do not implement spam classification from issue #5 in this update.
- Do not redesign the perceptron algorithm or introduce a larger framework.
- Do not add external test, documentation, or machine-learning dependencies.
- Do not implement multilayer neural networks to solve XOR.
- Keep `Perceptron` unchanged.
- Print the decision-boundary visualization for the AND gate only.
- Use learning rate `0.001` for logic-gate demos and tests.

---

## File Structure

- Create `docs/perceptron-history.md`: historical background for issue #1.
- Create `docs/perceptron-technical-details.md`: model math, training rule, decision boundary, and XOR limitation for issue #2.
- Modify `README.md`: mention the expanded demo and link to the new docs.
- Modify `tests/PerceptronTests.cpp`: add reusable logic-gate test helpers, tests for AND/OR/NAND/NOR, and a deterministic XOR convex-hull intersection test.
- Modify `src/main.cpp`: replace the single AND-only interactive demo with an automatic educational demo for AND/OR/NAND/NOR and an XOR explanation.
- Leave `CMakeLists.txt`, `include/Perceptron.h`, `src/Perceptron.cpp`, `include/DecisionBoundaryVisualizer.h`, and `src/DecisionBoundaryVisualizer.cpp` unchanged.

---

### Task 1: Add Perceptron Documentation

**Files:**
- Create: `docs/perceptron-history.md`
- Create: `docs/perceptron-technical-details.md`
- Modify: `README.md`

**Interfaces:**
- Consumes: current README project overview and build/run/test commands.
- Produces: stable documentation links used by README and CLI text: `docs/perceptron-history.md`, `docs/perceptron-technical-details.md`.

- [ ] **Step 1: Create the historical background document**

Create `docs/perceptron-history.md` with this content:

```markdown
# Historical Background of the Perceptron

The perceptron is one of the earliest computational models of a neuron and one of the foundations of machine learning. Frank Rosenblatt introduced it in the late 1950s while researching pattern recognition systems. The original goal was ambitious: build a machine that could learn classification rules from examples instead of being programmed with explicit rules.

Rosenblatt's perceptron combined a simple weighted sum with a threshold decision. If the weighted input exceeded the threshold, the perceptron activated; otherwise, it did not. This made the model easy to explain, easy to implement, and useful for binary classification tasks where the classes can be separated by a straight line, plane, or hyperplane.

Early perceptron research generated significant excitement because it suggested that machines could adapt from data. The model could learn simple visual and logical patterns, and it offered a concrete training rule for updating weights after mistakes.

The limitations became clearer in later work. A single perceptron can only learn linearly separable patterns. Problems such as XOR cannot be solved by one perceptron because no single straight decision boundary separates the two classes. Marvin Minsky and Seymour Papert's work on perceptrons emphasized these limitations and contributed to a period of reduced enthusiasm for neural-network research.

The perceptron remained important because its core ideas did not disappear. Weighted inputs, bias terms, activation functions, loss from prediction errors, and iterative learning all appear in modern neural networks. Multilayer networks extend the idea by combining many simple units, which allows them to model non-linear relationships that a single perceptron cannot represent.

This repository uses the perceptron as an educational starting point. It is small enough to read in one sitting while still showing the essential mechanics behind binary classification and decision boundaries.
```

- [ ] **Step 2: Create the technical details document**

Create `docs/perceptron-technical-details.md` with this content:

````markdown
# Technical Details of the Perceptron

This repository implements a binary perceptron. The model receives a fixed-size vector of numeric inputs and predicts one of two labels: `0` or `1`.

## Model

For an input vector `x`, weights `w`, and bias `b`, the perceptron computes a weighted sum:

```text
z = b + w0*x0 + w1*x1 + ... + wn*xn
```

The activation function converts that weighted sum into a binary prediction:

```text
prediction = 1 when z > 0
prediction = 0 when z <= 0
```

The bias shifts the decision boundary. Without a bias term, every boundary must pass through the origin, which is too restrictive for many simple classification tasks.

## Training Rule

Training loops over labeled examples for a fixed number of epochs. For each example, the model predicts a label and compares it with the target label:

```text
error = target - prediction
```

When the prediction is wrong, the weights and bias are updated:

```text
wi = wi + learning_rate * error * xi
b  = b  + learning_rate * error
```

The learning rate controls the update size. A small learning rate makes small corrections. A larger learning rate changes the model faster, but it can also make training less stable.

## Decision Boundary

For two input features, the perceptron's decision boundary is a line:

```text
w0*x + w1*y + b = 0
```

Points on one side of the line predict class `1`; points on the other side predict class `0`. The ASCII decision-boundary visualizer in this repository samples a two-dimensional grid and marks the learned regions, boundary, and training examples.

For more than two input features, the same equation describes a hyperplane instead of a line.

## Linear Separability

A single perceptron can learn a binary classification problem only when the classes are linearly separable. That means one line, plane, or hyperplane can separate all examples of class `0` from all examples of class `1`.

The logic gates `AND`, `OR`, `NAND`, and `NOR` are linearly separable. A single perceptron can learn them from the four binary input combinations.

The `XOR` gate is not linearly separable:

| x0 | x1 | XOR |
|----|----|-----|
| 0  | 0  | 0   |
| 0  | 1  | 1   |
| 1  | 0  | 1   |
| 1  | 1  | 0   |

The positive examples `(0, 1)` and `(1, 0)` lie on one diagonal of the unit square. The negative examples `(0, 0)` and `(1, 1)` lie on the other diagonal. Any line that tries to separate one diagonal from the other fails because the two class regions cross in the middle.

The same conclusion can be written as a contradiction. A single perceptron that learns XOR would need these four conditions:

```text
b <= 0              for input (0, 0) -> 0
b + w0 > 0          for input (1, 0) -> 1
b + w1 > 0          for input (0, 1) -> 1
b + w0 + w1 <= 0    for input (1, 1) -> 0
```

Adding the two positive conditions gives:

```text
2b + w0 + w1 > 0
```

Adding the two negative conditions gives:

```text
2b + w0 + w1 <= 0
```

Both cannot be true at the same time, so one perceptron cannot represent XOR.

## What This Project Demonstrates

- How a perceptron makes binary predictions.
- How weights and bias change during training.
- How simple linearly separable logic gates can be learned.
- Why XOR requires a more expressive model, such as a multilayer neural network.
````

- [ ] **Step 3: Update README overview and docs links**

In `README.md`, replace line 5 with this paragraph:

```markdown
C++ Perceptron (*cpp-perceptron*) is a small educational implementation of a binary perceptron in C++. The included demo trains models for linearly separable logic gates (`AND`, `OR`, `NAND`, and `NOR`) and explains why `XOR` cannot be learned by a single perceptron.
```

After the existing `## Perceptron` section image, add this section before `## Build`:

```markdown
## Documentation
- [Historical background of the perceptron](docs/perceptron-history.md)
- [Technical details of the perceptron](docs/perceptron-technical-details.md)
```

- [ ] **Step 4: Verify documentation links and wording**

Run: `git diff -- README.md docs/perceptron-history.md docs/perceptron-technical-details.md`

Expected: diff shows two new Markdown files, the README overview mentions `AND`, `OR`, `NAND`, `NOR`, and `XOR`, and the README links point to `docs/perceptron-history.md` and `docs/perceptron-technical-details.md`.

- [ ] **Step 5: Commit documentation changes**

Run:

```bash
git status --short -- README.md docs/perceptron-history.md docs/perceptron-technical-details.md
git diff -- README.md docs/perceptron-history.md docs/perceptron-technical-details.md
git add README.md docs/perceptron-history.md docs/perceptron-technical-details.md
git commit -m "docs(perceptron): add educational background"
```

Expected: commit succeeds and includes only the README and the two new documentation files.

---

### Task 2: Add Logic-Gate Learning And XOR Tests

**Files:**
- Modify: `tests/PerceptronTests.cpp`

**Interfaces:**
- Consumes: `Perceptron::train(const std::vector<std::vector<double>>&, const std::vector<double>&, int)` and `Perceptron::predict(const std::vector<double>&) const`.
- Produces: test helpers `binaryLogicInputs`, `expectLearnsLogicGate`, `lineSegmentsIntersect`, and tests `learnsLinearlySeparableLogicGates`, `xorClassesAreNotLinearlySeparable`.

- [ ] **Step 1: Add shared binary logic input helper**

In `tests/PerceptronTests.cpp`, inside the anonymous namespace after `expectInvalidArgument`, add:

```cpp
const std::vector<std::vector<double>>& binaryLogicInputs() {
    static const std::vector<std::vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    return inputs;
}

void expectLearnsLogicGate(const std::string& name, const std::vector<double>& labels) {
    Perceptron perceptron(2, 0.001);
    const std::vector<std::vector<double>>& inputs = binaryLogicInputs();

    perceptron.train(inputs, labels, 25);

    for (std::size_t i = 0; i < inputs.size(); i++) {
        expect(perceptron.predict(inputs[i]) == labels[i], name + ": prediction should match target label");
    }
}
```

- [ ] **Step 2: Replace the AND-only learning test with multi-gate coverage**

Replace the existing `learnsAndGate()` function with:

```cpp
void learnsLinearlySeparableLogicGates() {
    expectLearnsLogicGate("AND", {0, 0, 0, 1});
    expectLearnsLogicGate("OR", {0, 1, 1, 1});
    expectLearnsLogicGate("NAND", {1, 1, 1, 0});
    expectLearnsLogicGate("NOR", {1, 0, 0, 0});
}
```

In `main()` at the bottom of the test file, replace:

```cpp
learnsAndGate();
```

with:

```cpp
learnsLinearlySeparableLogicGates();
```

- [ ] **Step 3: Add deterministic XOR non-separability helpers**

In `tests/PerceptronTests.cpp`, inside the anonymous namespace after `expectLearnsLogicGate`, add:

```cpp
struct Point {
    double x = 0.0;
    double y = 0.0;
};

double crossProduct(const Point& origin, const Point& first, const Point& second) {
    return (first.x - origin.x) * (second.y - origin.y) -
           (first.y - origin.y) * (second.x - origin.x);
}

bool hasOppositeOrientationOrTouches(double first, double second) {
    return (first <= 0.0 && second >= 0.0) || (first >= 0.0 && second <= 0.0);
}

bool lineSegmentsIntersect(const Point& firstStart,
                           const Point& firstEnd,
                           const Point& secondStart,
                           const Point& secondEnd) {
    const double firstSide = crossProduct(firstStart, firstEnd, secondStart);
    const double secondSide = crossProduct(firstStart, firstEnd, secondEnd);
    const double thirdSide = crossProduct(secondStart, secondEnd, firstStart);
    const double fourthSide = crossProduct(secondStart, secondEnd, firstEnd);

    return hasOppositeOrientationOrTouches(firstSide, secondSide) &&
           hasOppositeOrientationOrTouches(thirdSide, fourthSide);
}
```

- [ ] **Step 4: Add the XOR limitation test**

In `tests/PerceptronTests.cpp`, after `learnsLinearlySeparableLogicGates()`, add:

```cpp
void xorClassesAreNotLinearlySeparable() {
    const Point positiveA{0.0, 1.0};
    const Point positiveB{1.0, 0.0};
    const Point negativeA{0.0, 0.0};
    const Point negativeB{1.0, 1.0};

    expect(lineSegmentsIntersect(positiveA, positiveB, negativeA, negativeB),
           "XOR positive and negative convex hulls intersect, so one line cannot separate them");
}
```

In `main()` at the bottom of the test file, call the new test immediately after `learnsLinearlySeparableLogicGates();`:

```cpp
learnsLinearlySeparableLogicGates();
xorClassesAreNotLinearlySeparable();
```

- [ ] **Step 5: Run the test binary through CTest**

Run: `cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure`

Expected: build succeeds and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 6: Commit test changes**

Run:

```bash
git status --short -- tests/PerceptronTests.cpp
git diff -- tests/PerceptronTests.cpp
git add tests/PerceptronTests.cpp
git commit -m "test(perceptron): cover logic gate separability"
```

Expected: commit succeeds and includes only `tests/PerceptronTests.cpp`.

---

### Task 3: Expand The CLI Logic-Gate Demo

**Files:**
- Modify: `src/main.cpp`

**Interfaces:**
- Consumes: `Perceptron`, `DecisionBoundaryVisualizer`, and the new documentation path `docs/perceptron-technical-details.md` referenced in terminal text.
- Produces: an automatic CLI demo for `AND`, `OR`, `NAND`, `NOR`, plus an explanatory `XOR` section.

- [ ] **Step 1: Replace `src/main.cpp` with the multi-gate demo**

Replace the full contents of `src/main.cpp` with:

```cpp
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
```

- [ ] **Step 2: Build and test the CLI change**

Run: `cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure`

Expected: build succeeds and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 3: Run the demo and inspect output**

Run: `./build/cpp-perceptron`

Expected output contains these strings:

```text
AND gate
OR gate
NAND gate
NOR gate
XOR gate
Linearly separable: no
Result: not learnable by a single perceptron
Read docs/perceptron-technical-details.md
```

Expected output includes one decision-boundary plot, under `Decision boundary for AND:`.

- [ ] **Step 4: Commit CLI changes**

Run:

```bash
git status --short -- src/main.cpp
git diff -- src/main.cpp
git add src/main.cpp
git commit -m "feat(cli): demonstrate logic gates"
```

Expected: commit succeeds and includes only `src/main.cpp`.

---

### Task 4: Final Verification

**Files:**
- Verify: `README.md`
- Verify: `docs/perceptron-history.md`
- Verify: `docs/perceptron-technical-details.md`
- Verify: `tests/PerceptronTests.cpp`
- Verify: `src/main.cpp`

**Interfaces:**
- Consumes: all changes from Tasks 1 through 3.
- Produces: evidence that issues #1, #2, and #4 are implemented without CI, GitHub Actions, spam classification, or larger framework work.

- [ ] **Step 1: Run the complete build and test command**

Run: `cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure`

Expected: configure succeeds, build succeeds, and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 2: Run the CLI demo**

Run: `./build/cpp-perceptron`

Expected output contains:

```text
AND gate
OR gate
NAND gate
NOR gate
XOR gate
Result: not learnable by a single perceptron
```

- [ ] **Step 3: Verify documentation files and README links**

Run: `git grep -n "perceptron-history\|perceptron-technical-details\|XOR\|NAND\|NOR" README.md docs/perceptron-history.md docs/perceptron-technical-details.md`

Expected: output includes README links to both documentation files, technical documentation that mentions XOR, and README text that mentions NAND and NOR.

- [ ] **Step 4: Verify no out-of-scope CI files were added**

Run: `git status --short -- README.md docs/perceptron-history.md docs/perceptron-technical-details.md tests/PerceptronTests.cpp src/main.cpp .github && git ls-files ".github/*"`

Expected: the path-scoped `git status --short` command prints no product-code changes after all task commits, and `git ls-files ".github/*"` prints no tracked files.

- [ ] **Step 5: Review recent commits**

Run: `git log --oneline -5`

Expected: recent commits include these messages:

```text
feat(cli): demonstrate logic gates
test(perceptron): cover logic gate separability
docs(perceptron): add educational background
docs(perceptron): add educational update design
```

- [ ] **Step 6: Prepare issue closure notes**

Use these notes when closing or commenting on the GitHub issues:

```text
Issue #1: Added docs/perceptron-history.md with historical background covering Rosenblatt, early expectations, limitations, and why perceptrons remain useful educational models.

Issue #2: Added docs/perceptron-technical-details.md with the model equation, activation function, training rule, decision boundary, linear separability, and XOR limitation.

Issue #4: Expanded the CLI demo to train AND, OR, NAND, and NOR, and to show XOR as a non-linearly separable example that a single perceptron cannot learn.
```
