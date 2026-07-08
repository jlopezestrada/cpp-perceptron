# Logic Gate Graphs Flag Design

## Context

The current CLI demo trains `AND`, `OR`, `NAND`, and `NOR`, explains why `XOR` is not linearly separable, and renders one ASCII decision-boundary graph for `AND`. The `DecisionBoundaryVisualizer` already supports any trained two-feature perceptron, so it can render the other linearly separable logic gates without changing the visualizer or `Perceptron`.

The user wants an option to see graphs for the rest of the truth tables while keeping the default output concise.

## Goals

- Add a `--graphs` CLI flag that renders decision-boundary graphs for all linearly separable gates.
- Preserve the current no-argument behavior: render only the `AND` graph.
- Keep `XOR` explanatory only and do not render a graph for it.
- Add a short `--help` output that documents available options.
- Reject unknown arguments with usage text and a non-zero exit code.
- Keep the implementation dependency-free and focused in `src/main.cpp`.

## Non-Goals

- Do not change `Perceptron` training or prediction.
- Do not change `DecisionBoundaryVisualizer`.
- Do not add CI or GitHub Actions.
- Do not add external CLI parsing or testing dependencies.
- Do not implement a multilayer model or graph for `XOR`.

## CLI Behavior

The executable will accept these forms:

```text
./build/cpp-perceptron
./build/cpp-perceptron --graphs
./build/cpp-perceptron --help
```

Default behavior remains concise:

- Train and print truth tables for `AND`, `OR`, `NAND`, and `NOR`.
- Render only the `AND` decision-boundary graph.
- Print the `XOR` truth table and non-linearly separable explanation.

With `--graphs`:

- Train and print truth tables for `AND`, `OR`, `NAND`, and `NOR`.
- Render a decision-boundary graph for each linearly separable gate: `AND`, `OR`, `NAND`, and `NOR`.
- Print the `XOR` truth table and explanation without a graph.

With `--help`:

- Print usage and options.
- Return success without training or rendering graphs.

With any unknown argument:

- Print an error and usage text.
- Return exit code `1`.

## Implementation Design

Change `main()` to `int main(int argc, char* argv[])` and add a small local argument parser in `src/main.cpp`.

Use an enum or simple mode value with three outcomes:

- default demo mode
- all-graphs demo mode
- help mode

Update `demonstrateLearnableGate` to keep taking a boolean that decides whether to render the graph. In the main gate loop, compute that boolean as:

- `true` for every linearly separable gate when `--graphs` is present
- `true` only for the first linearly separable gate when no flag is present
- `false` for `XOR` because it is handled by `explainNonLinearlySeparableGate`

Keep parsing minimal. The CLI only accepts zero arguments or one supported flag. Multiple arguments are invalid and should show usage with exit code `1`.

## Testing And Verification

Extend tests only if the current custom C++ test setup can cover the new behavior without introducing process-spawning infrastructure. The most pragmatic verification is command-level:

- Build and run CTest.
- Run `./build/cpp-perceptron` and verify it prints only one `Decision boundary for` section.
- Run `./build/cpp-perceptron --graphs` and verify it prints graph sections for `AND`, `OR`, `NAND`, and `NOR`, and no graph for `XOR`.
- Run `./build/cpp-perceptron --help` and verify it prints usage and exits successfully.
- Run `./build/cpp-perceptron --unknown` and verify it prints usage and exits with code `1`.

## Acceptance Criteria

- `./build/cpp-perceptron` keeps the existing concise behavior with only the `AND` graph.
- `./build/cpp-perceptron --graphs` renders graphs for `AND`, `OR`, `NAND`, and `NOR`.
- `XOR` remains explanatory only and is never rendered as a decision-boundary graph.
- `./build/cpp-perceptron --help` documents `--graphs`.
- Unknown arguments return a non-zero exit code and show usage.
- CMake build and CTest still pass.
