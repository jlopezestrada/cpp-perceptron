# Logic Gate Graphs Flag Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a `--graphs` CLI flag that renders decision-boundary graphs for every linearly separable logic gate while keeping the no-argument demo concise.

**Architecture:** Keep the existing `Perceptron` and `DecisionBoundaryVisualizer` APIs unchanged. Implement minimal argument parsing inside `src/main.cpp`, then use the parsed demo mode to decide whether to render only the first graph or every linearly separable gate graph.

**Tech Stack:** C++17, CMake, standard library only, custom C++ test binary, command-level CLI verification.

## Global Constraints

- Do not change `Perceptron` training or prediction.
- Do not change `DecisionBoundaryVisualizer`.
- Do not add CI or GitHub Actions.
- Do not add external CLI parsing or testing dependencies.
- Do not implement a multilayer model or graph for `XOR`.
- Preserve the current no-argument behavior: render only the `AND` graph.
- Add a `--graphs` CLI flag that renders decision-boundary graphs for all linearly separable gates.
- Keep `XOR` explanatory only and do not render a graph for it.
- Add a short `--help` output that documents available options.
- Reject unknown arguments with usage text and a non-zero exit code.
- Multiple arguments are invalid and should show usage with exit code `1`.

---

## File Structure

- Modify `src/main.cpp`: add local CLI mode parsing, usage output, and graph-rendering mode selection.
- Do not modify `include/Perceptron.h`, `src/Perceptron.cpp`, `include/DecisionBoundaryVisualizer.h`, or `src/DecisionBoundaryVisualizer.cpp`.
- Do not add new dependencies or CI files.
- Do not add process-spawning tests to `tests/PerceptronTests.cpp`; verify CLI behavior with command-level checks instead.

---

### Task 1: Add `--graphs` CLI Mode

**Files:**
- Modify: `src/main.cpp`

**Interfaces:**
- Consumes: `DecisionBoundaryVisualizer::render(const Perceptron&, const std::vector<std::vector<double>>&, const std::vector<double>&, std::ostream&) const`.
- Consumes: `Perceptron::train(const std::vector<std::vector<double>>&, const std::vector<double>&, int)` and `Perceptron::predict(const std::vector<double>&) const`.
- Produces: `enum class DemoMode { Default, AllGraphs, Help }`, `programName(int argc, char* argv[])`, `printUsage(const std::string&, std::ostream&)`, and `parseArguments(int argc, char* argv[], DemoMode&, std::ostream&)` in the anonymous namespace of `src/main.cpp`.

- [ ] **Step 1: Replace `src/main.cpp` with the flag-aware CLI**

Replace the full contents of `src/main.cpp` with:

```cpp
#include "DecisionBoundaryVisualizer.h"
#include "Perceptron.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace {
constexpr double kLearningRate = 0.001;
constexpr int kTrainingEpochs = 25;

enum class DemoMode {
    Default,
    AllGraphs,
    Help
};

struct LogicGate {
    std::string name;
    std::vector<double> labels;
    bool linearlySeparable = true;
    std::string note;
};

std::string programName(int argc, char* argv[]) {
    if (argc > 0 && argv[0] != nullptr) {
        return argv[0];
    }

    return "cpp-perceptron";
}

void printUsage(const std::string& executable, std::ostream& output) {
    output << "Usage: " << executable << " [--graphs|--help]\n\n";
    output << "Options:\n";
    output << "  --graphs  Show decision-boundary graphs for AND, OR, NAND, and NOR.\n";
    output << "  --help    Show this help message.\n";
}

bool parseArguments(int argc, char* argv[], DemoMode& mode, std::ostream& errorOutput) {
    mode = DemoMode::Default;
    const std::string executable = programName(argc, argv);

    if (argc == 1) {
        return true;
    }

    if (argc == 2) {
        const std::string argument = argv[1];
        if (argument == "--graphs") {
            mode = DemoMode::AllGraphs;
            return true;
        }

        if (argument == "--help") {
            mode = DemoMode::Help;
            return true;
        }

        errorOutput << "Unknown option: " << argument << "\n\n";
        printUsage(executable, errorOutput);
        return false;
    }

    errorOutput << "Invalid argument count.\n\n";
    printUsage(executable, errorOutput);
    return false;
}

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

int main(int argc, char* argv[]) {
    try {
        DemoMode mode = DemoMode::Default;
        const std::string executable = programName(argc, argv);

        if (!parseArguments(argc, argv, mode, std::cerr)) {
            return 1;
        }

        if (mode == DemoMode::Help) {
            printUsage(executable, std::cout);
            return 0;
        }

        std::cout << "\n========================================\n";
        std::cout << "   C++ Perceptron - Logic Gate Demo\n";
        std::cout << "========================================\n";

        const std::vector<LogicGate> gates = logicGates();
        const bool renderAllGraphs = mode == DemoMode::AllGraphs;
        bool boundaryRendered = false;

        for (const LogicGate& gate : gates) {
            if (gate.linearlySeparable) {
                const bool showDecisionBoundary = renderAllGraphs || !boundaryRendered;
                demonstrateLearnableGate(gate, showDecisionBoundary);
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

- [ ] **Step 2: Build and run the existing test suite**

Run: `cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure`

Expected: configure succeeds, build succeeds, and CTest reports `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 3: Verify default CLI output remains concise**

Run:

```bash
./build/cpp-perceptron > /tmp/opencode/cpp-perceptron-default.txt
rg -c "Decision boundary for" /tmp/opencode/cpp-perceptron-default.txt
rg "Decision boundary for AND:" /tmp/opencode/cpp-perceptron-default.txt
```

Expected: `rg -c` prints `1`, and the output contains `Decision boundary for AND:`.

- [ ] **Step 4: Verify `--graphs` renders every linearly separable graph and no XOR graph**

Run:

```bash
./build/cpp-perceptron --graphs > /tmp/opencode/cpp-perceptron-graphs.txt
rg -c "Decision boundary for" /tmp/opencode/cpp-perceptron-graphs.txt
rg "Decision boundary for AND:" /tmp/opencode/cpp-perceptron-graphs.txt
rg "Decision boundary for OR:" /tmp/opencode/cpp-perceptron-graphs.txt
rg "Decision boundary for NAND:" /tmp/opencode/cpp-perceptron-graphs.txt
rg "Decision boundary for NOR:" /tmp/opencode/cpp-perceptron-graphs.txt
if rg "Decision boundary for XOR:" /tmp/opencode/cpp-perceptron-graphs.txt; then exit 1; fi
```

Expected: `rg -c` prints `4`; the output contains graph headings for `AND`, `OR`, `NAND`, and `NOR`; the final `if` command exits successfully because there is no `XOR` graph heading.

- [ ] **Step 5: Verify `--help` exits successfully without running the demo**

Run:

```bash
./build/cpp-perceptron --help > /tmp/opencode/cpp-perceptron-help.txt
rg "Usage:" /tmp/opencode/cpp-perceptron-help.txt
rg -- "--graphs" /tmp/opencode/cpp-perceptron-help.txt
if rg "AND gate" /tmp/opencode/cpp-perceptron-help.txt; then exit 1; fi
```

Expected: usage output includes `Usage:` and `--graphs`; the final `if` command exits successfully because help output does not run the demo.

- [ ] **Step 6: Verify invalid arguments fail with usage**

Run:

```bash
./build/cpp-perceptron --unknown > /tmp/opencode/cpp-perceptron-invalid.txt 2>&1
status=$?
test "$status" -eq 1
rg "Unknown option: --unknown" /tmp/opencode/cpp-perceptron-invalid.txt
rg "Usage:" /tmp/opencode/cpp-perceptron-invalid.txt
./build/cpp-perceptron --graphs --help > /tmp/opencode/cpp-perceptron-invalid-count.txt 2>&1
status=$?
test "$status" -eq 1
rg "Invalid argument count" /tmp/opencode/cpp-perceptron-invalid-count.txt
rg "Usage:" /tmp/opencode/cpp-perceptron-invalid-count.txt
```

Expected: both invalid invocations exit with status `1` and print usage text.

- [ ] **Step 7: Review the diff**

Run: `git diff -- src/main.cpp`

Expected: only `src/main.cpp` changed; no `Perceptron`, visualizer, dependency, CI, or test files changed.

- [ ] **Step 8: Commit the CLI flag**

Run:

```bash
git status --short -- src/main.cpp
git add src/main.cpp
git commit -m "feat(cli): add graphs option"
```

Expected: commit succeeds and includes only `src/main.cpp`.
