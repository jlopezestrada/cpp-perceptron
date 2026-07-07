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
