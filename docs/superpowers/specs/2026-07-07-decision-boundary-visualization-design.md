# Decision Boundary Visualization Design

## Context

Issue #3 asks for a method or class that shows the decision boundary separating different classes. The project is a small C++17 educational perceptron implementation with one library target, one CLI demo, and a lightweight custom test binary. The current demo trains a two-feature perceptron on the logical AND gate.

The perceptron already exposes `predict`, `getWeights`, and `getBias`, which are sufficient to visualize the learned two-dimensional decision boundary without changing training internals.

## Goals

- Add a dependency-free ASCII visualization for two-feature perceptrons.
- Provide the feature as a reusable library class, not as logic embedded only in `main.cpp`.
- Show predicted class regions, the learned decision boundary, and training samples.
- Keep the default plot useful for the current AND-gate demo while allowing callers to configure bounds and grid size.
- Preserve the existing CMake, CLI, and test style.

## Non-Goals

- Do not support higher-dimensional visualization.
- Do not add external plotting, image, GUI, or terminal UI dependencies.
- Do not redesign perceptron training or prediction.
- Do not add a new test framework.

## Architecture

Add a small library-level component beside `Perceptron`:

- `include/DecisionBoundaryVisualizer.h`
- `src/DecisionBoundaryVisualizer.cpp`

`DecisionBoundaryVisualizer` depends on the public `Perceptron` API. `Perceptron` remains focused on model behavior and does not depend on visualization code.

The existing `cpp-perceptron` executable will train the AND-gate model as it does today, print the truth table, then call the visualizer to print the ASCII plot. CMake will compile the new source into the existing `perceptron` library so both the CLI and tests can use it.

## Public API

Expose a compact API:

```cpp
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
};
```

Default bounds frame the current AND-gate inputs with a small margin. Callers can override bounds and grid size through `DecisionBoundaryOptions`.

## Rendering Behavior

The renderer is deterministic:

1. Validate options, sample dimensions, labels, and that the perceptron exposes exactly two finite weights.
2. For each grid coordinate, map the character cell to an `(x, y)` point inside the configured bounds.
3. Use `perceptron.predict({x, y})` to choose the region marker: `.` for class `0` and `+` for class `1`.
4. Use the learned equation `w0*x + w1*y + bias = 0` to mark cells near the boundary as `#`. A cell is a boundary cell when `abs(w0*x + w1*y + bias)` is less than or equal to `0.5 * (abs(w0) * xStep + abs(w1) * yStep)`, where `xStep` and `yStep` are the configured grid spacing values.
5. Overlay training samples last as `0` or `1`, so examples remain visible over regions and boundary cells.
6. Print rows from high `y` to low `y`, followed by a legend: `. class 0`, `+ class 1`, `# boundary`, and `0/1 samples`.

Training samples outside the configured bounds are not plotted. Samples inside the bounds map to the nearest grid cell.

## Error Handling

The visualizer throws `std::invalid_argument` for invalid use:

- The perceptron does not expose exactly two weights.
- Bounds are non-finite or min/max are not ordered.
- Grid width or height is less than `2`.
- Samples and labels have different sizes.
- A sample is not two-dimensional or contains non-finite values.
- A label is not `0.0` or `1.0`.
- Both learned feature weights are zero, so there is no meaningful two-dimensional boundary to draw.

The CLI keeps its existing top-level `try/catch`, so visualization errors are reported as clean `Error: ...` messages.

## Testing

Extend the existing custom test binary:

- Train the existing AND perceptron, render a small deterministic plot, and assert that the output contains sample markers, region markers, boundary markers, and the legend.
- Verify rejection of non-two-feature perceptrons.
- Verify rejection of invalid plot options.
- Verify rejection of mismatched sample/label counts.
- Verify rejection of bad sample dimensions, non-finite samples, and non-binary labels.

The tests should avoid brittle full-plot snapshots. The core class receives direct behavior coverage; the CLI remains covered by successful build integration.

## Success Criteria

- Running `./build/cpp-perceptron` prints the existing AND-gate output plus an ASCII decision boundary visualization.
- The visualization includes class regions, an approximate boundary, sample markers, and a legend.
- The feature is usable through `DecisionBoundaryVisualizer` from library consumers.
- Existing tests still pass, and new visualizer tests cover rendering and validation behavior.
