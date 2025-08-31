class QuantumNumberLayer:
    def __init__(self, input_keys, output_keys, bases, initial_weights):
        """
        input_keys: keys for input quantum numbers (e.g., ['a', 'b', 'c'])
        output_keys: keys for output quantum numbers (e.g., ['x', 'y'])
        bases: dict of integer bases for each quantum number key
        initial_weights: dict of dicts for weights per input-output pair,
                         e.g. weights['x']['a'] = int
        """
        self.input_keys = input_keys
        self.output_keys = output_keys
        self.bases = bases
        self.weights = initial_weights  # nested dict output_key -> input_key -> int weight
        self.biases = {k: 0 for k in output_keys}  # integer biases

    def forward(self, input_qn):
        """
        input_qn: dict of quantum numbers (integer values)
        Returns output_qn: dict of integer quantum numbers
        """
        output_qn = {}
        for out_k in self.output_keys:
            total = self.biases[out_k]
            for in_k in self.input_keys:
                w = self.weights[out_k][in_k]
                v = input_qn[in_k]
                total += w * v
            # To keep quantum number valid, reduce modulo base
            base = self.bases[out_k]
            output_qn[out_k] = total % base
        return output_qn

    def backward(self, input_qn, target_qn, output_qn, learning_rate=1):
        """
        Compute error (target - output), update weights and biases as integer steps.
        No floating point math used.
        """
        errors = {}
        for k in self.output_keys:
            errors[k] = target_qn[k] - output_qn[k]

        # Update weights and biases
        for out_k in self.output_keys:
            self.biases[out_k] += learning_rate * errors[out_k]
            for in_k in self.input_keys:
                self.weights[out_k][in_k] += learning_rate * errors[out_k] * input_qn[in_k]

    def __str__(self):
        s = "QuantumNumberLayer state:\n"
        s += "Weights:\n"
        for out_k in self.output_keys:
            s += f"  {out_k}: " + ", ".join(f"{in_k}={self.weights[out_k][in_k]}" for in_k in self.input_keys) + "\n"
        s += "Biases:\n"
        for k in self.output_keys:
            s += f"  {k}: {self.biases[k]}\n"
        return s


def demo23_ml():
    print("=== Demo 23: Basic ML with Quantum Number Layer and Arbitrary Bases ===\n")

    # Setup
    input_keys = ['a', 'b', 'c']
    output_keys = ['x', 'y']
    bases = {'a': 10, 'b': 10, 'c': 10, 'x': 7, 'y': 5}  # arbitrary bases per quantum number
    initial_weights = {
        'x': {'a': 1, 'b': 2, 'c': 1},
        'y': {'a': 0, 'b': 1, 'c': 3}
    }

    qlayer = QuantumNumberLayer(input_keys, output_keys, bases, initial_weights)

    # Training data: inputs and desired outputs (all integer quantum numbers)
    training_data = [
        ({'a': 1, 'b': 2, 'c': 3}, {'x': 5, 'y': 4}),
        ({'a': 3, 'b': 1, 'c': 0}, {'x': 6, 'y': 2}),
        ({'a': 0, 'b': 0, 'c': 4}, {'x': 3, 'y': 3}),
    ]

    steps = 5
    for step in range(steps):
        print(f"--- Step {step+1} ---")
        for i, (input_qn, target_qn) in enumerate(training_data):
            output_qn = qlayer.forward(input_qn)
            print(f"Input: {input_qn}")
            print(f"Output: {output_qn}")
            print(f"Target: {target_qn}")

            qlayer.backward(input_qn, target_qn, output_qn)

        print("\nLayer state after updates:")
        print(qlayer)
        print("\n")

    print("=== Demo 23 complete: weights and biases updated with integer arithmetic only, no floating point error ===")


if __name__ == "__main__":
    demo23_ml()
