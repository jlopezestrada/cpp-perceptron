# Educational Perceptron Update Design

## Context

The repository is a small educational C++17 perceptron project. It currently exposes a reusable `Perceptron` class, a decision-boundary visualizer, a CLI demo that trains the AND gate, and dependency-free tests.

Open issues #1, #2, and #4 ask for historical documentation, technical documentation, and a perceptron capable of predicting logic-gate operations. The project will remain a focused educational resource for now. CI and GitHub Actions are out of scope.

## Goals

- Complete issue #1 by adding historical background for the perceptron.
- Complete issue #2 by adding technical details that explain the model used in this repository.
- Complete issue #4 by expanding the CLI demo from only AND to several logic gates.
- Include XOR as an explicit example of a non-linearly separable problem that a single perceptron cannot learn.
- Keep the implementation small, dependency-free, and easy to read.

## Non-Goals

- Do not add CI or GitHub Actions.
- Do not implement spam classification from issue #5 in this update.
- Do not redesign the perceptron algorithm or introduce a larger framework.
- Do not add external test, documentation, or machine-learning dependencies.
- Do not implement multilayer neural networks to solve XOR.

## Documentation Design

Add two documentation files under the existing `docs/` directory:

- `docs/perceptron-history.md`
- `docs/perceptron-technical-details.md`

The history document will explain the perceptron's origin, Frank Rosenblatt's work, early expectations, the limitations highlighted by later research, and why the perceptron remains useful as an introductory model.

The technical document will explain the binary perceptron implemented here: inputs, weights, bias, weighted sum, step activation, prediction, training update rule, learning rate, epochs, linear separability, decision boundaries, and why XOR cannot be learned by a single perceptron.

Update `README.md` to link to both documents and briefly mention that the demo now covers multiple logic gates plus the XOR limitation.

## CLI Design

Keep `Perceptron` unchanged. Expand `src/main.cpp` with small local helpers rather than adding new public classes.

The CLI will define a simple logic-gate dataset shape with:

- gate name
- training samples
- expected labels
- whether the gate is linearly separable
- short explanatory note

The demo will train and print truth tables for these linearly separable gates:

- AND
- OR
- NAND
- NOR

For XOR, the CLI will print the XOR truth table and explain that no single straight-line decision boundary separates its positive and negative samples. It will not present XOR as a successfully learned gate.

The decision-boundary visualizer will remain part of the educational output for trained two-input gates. To avoid excessive output, the CLI will print the visualization for the AND gate only, then use compact truth-table output for the remaining gates.

## Data Flow

For each linearly separable gate:

1. Create a fresh `Perceptron` with two features, learning rate `0.001`, and a shared training epoch count chosen to learn all listed linearly separable gates reliably.
2. Train it on the four binary input combinations.
3. Print each input, target label, and prediction.
4. Report whether all predictions match the target labels.

For XOR:

1. Print the truth table.
2. Explain that the positive examples `(0, 1)` and `(1, 0)` cannot be separated from `(0, 0)` and `(1, 1)` by one linear boundary.
3. Point readers to the technical documentation for more detail.

## Error Handling

The existing `try`/`catch` in `main.cpp` will remain. The demo data is static and valid, so runtime errors should only come from unexpected implementation mistakes or invalid model configuration.

Existing validation in `Perceptron` and `DecisionBoundaryVisualizer` remains unchanged.

## Testing Design

Extend the dependency-free test binary in `tests/PerceptronTests.cpp`.

Add tests that verify a fresh perceptron can learn:

- AND
- OR
- NAND
- NOR

Add a test that documents XOR as not linearly separable for the single-perceptron model. This should not depend on a flaky training failure. Prefer a deterministic helper or assertion that the XOR labels cannot be separated by any single line over a training run that merely fails after a chosen epoch count.

Keep the existing validation and visualization tests.

## Acceptance Criteria

- `README.md` links to the new history and technical documents.
- `docs/perceptron-history.md` explains the historical background clearly and concisely.
- `docs/perceptron-technical-details.md` explains the implemented perceptron and the XOR limitation.
- The CLI demonstrates AND, OR, NAND, and NOR as learnable gates.
- The CLI presents XOR as an impossible example for a single perceptron, not as a successful prediction demo.
- Tests cover the linearly separable gates and the XOR limitation.
- The project still builds with CMake and passes `ctest`.
