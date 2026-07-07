# Historical Background of the Perceptron

The perceptron is one of the earliest computational models of a neuron and one of the foundations of machine learning. Frank Rosenblatt introduced it in the late 1950s while researching pattern recognition systems. The original goal was ambitious: build a machine that could learn classification rules from examples instead of being programmed with explicit rules.

Rosenblatt's perceptron combined a simple weighted sum with a threshold decision. If the weighted input exceeded the threshold, the perceptron activated; otherwise, it did not. This made the model easy to explain, easy to implement, and useful for binary classification tasks where the classes can be separated by a straight line, plane, or hyperplane.

Early perceptron research generated significant excitement because it suggested that machines could adapt from data. The model could learn simple visual and logical patterns, and it offered a concrete training rule for updating weights after mistakes.

The limitations became clearer in later work. A single perceptron can only learn linearly separable patterns. Problems such as XOR cannot be solved by one perceptron because no single straight decision boundary separates the two classes. Marvin Minsky and Seymour Papert's work on perceptrons emphasized these limitations and contributed to a period of reduced enthusiasm for neural-network research.

The perceptron remained important because its core ideas did not disappear. Weighted inputs, bias terms, activation functions, loss from prediction errors, and iterative learning all appear in modern neural networks. Multilayer networks extend the idea by combining many simple units, which allows them to model non-linear relationships that a single perceptron cannot represent.

This repository uses the perceptron as an educational starting point. It is small enough to read in one sitting while still showing the essential mechanics behind binary classification and decision boundaries.
