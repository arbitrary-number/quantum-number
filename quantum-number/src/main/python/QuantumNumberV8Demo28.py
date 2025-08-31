class QuantumNumberV8:
    """
    Represents a symbolic, exact numeric unit designed for
    infinite-precision arithmetic across the full number line.

    Structure modeled as:
    Σ (±a / (±b / ±c)) * (±d / (±e / ±f))

    Each of a,b,c,d,e,f is an arbitrary-precision integer in Python.

    Explicit carry management through the left pointer is mandatory.
    """

    def __init__(self):
        self.signs = 0
        self.metadata = 0

        self.a = 0
        self.b = 0
        self.c = 0
        self.d = 0
        self.e = 0
        self.f = 0

        self.left = None
        self.right = None
        self.up = None
        self.down = None
        self.in_ = None
        self.out = None

    def __repr__(self):
        def ptr_id(ptr):
            return hex(id(ptr)) if ptr else "None"
        return (
            f"QNum(a={self.a},b={self.b},c={self.c},d={self.d},e={self.e},f={self.f},"
            f" left={ptr_id(self.left)})"
        )

    def add(self, other):
        """Add other QuantumNumberV8 to self with explicit carry on left pointer."""
        carry = QuantumNumberV8()
        carry_flag = False

        for field in ['a', 'b', 'c', 'd', 'e', 'f']:
            val = getattr(self, field) + getattr(other, field)
            if val > 10:  # simple carry threshold
                carry_flag = True
                setattr(self, field, val - 10)
                setattr(carry, field, 1)
            else:
                setattr(self, field, val)
                setattr(carry, field, 0)

        if carry_flag:
            if self.left:
                self.left.add(carry)
            else:
                self.left = carry

    def scale_by_int(self, scalar):
        """Scale all fields by an integer scalar, handling carry."""
        carry = QuantumNumberV8()
        carry_flag = False
        for field in ['a', 'b', 'c', 'd', 'e', 'f']:
            val = getattr(self, field) * scalar
            if val > 10:
                carry_flag = True
                setattr(self, field, val % 10)
                setattr(carry, field, val // 10)
            else:
                setattr(self, field, val)
                setattr(carry, field, 0)

        if carry_flag:
            if self.left:
                self.left.add(carry)
            else:
                self.left = carry


class QuantumLayer:
    def __init__(self, inputs, outputs):
        # weights: dict output -> dict input -> QuantumNumberV8
        self.weights = {o: {i: QuantumNumberV8() for i in inputs} for o in outputs}
        self.biases = {o: QuantumNumberV8() for o in outputs}

        # Initialize weights and biases with small fractional values (e.g. a=1, others 0)
        for o in outputs:
            for i in inputs:
                self.weights[o][i].a = 1  # fraction 1 (a=1)
            self.biases[o].a = 1  # fraction 1 bias

    def forward(self, input_vector):
        # input_vector: dict input_name -> integer value
        output = {}
        for o, weight_dict in self.weights.items():
            sum_qnum = QuantumNumberV8()
            for i, qnum in weight_dict.items():
                # Multiply qnum by input value (scale by int)
                tmp = QuantumNumberV8()
                tmp.a, tmp.b, tmp.c, tmp.d, tmp.e, tmp.f = qnum.a, qnum.b, qnum.c, qnum.d, qnum.e, qnum.f
                tmp.scale_by_int(input_vector.get(i, 0))
                sum_qnum.add(tmp)
            sum_qnum.add(self.biases[o])
            output[o] = sum_qnum
        return output

    def train_step(self, input_vector, target_vector, learning_rate=1):
        # Compute output
        output = self.forward(input_vector)

        # Calculate error and update weights and biases exactly
        for o in self.weights:
            error = QuantumNumberV8()
            # error = target - output (simplified for demo, only a field used)
            error.a = target_vector[o] - output[o].a

            # Update weights
            for i in self.weights[o]:
                delta = QuantumNumberV8()
                delta.a = error.a * input_vector.get(i, 0) * learning_rate
                self.weights[o][i].add(delta)

            # Update bias
            bias_delta = QuantumNumberV8()
            bias_delta.a = error.a * learning_rate
            self.biases[o].add(bias_delta)

def demo28():
    inputs = ['a', 'b', 'c']
    outputs = ['x', 'y']
    layer = QuantumLayer(inputs, outputs)

    # Example training data (integer inputs and targets)
    training_data = [
        ({'a': 1, 'b': 2, 'c': 3}, {'x': 5, 'y': 4}),
        ({'a': 3, 'b': 1, 'c': 0}, {'x': 6, 'y': 2}),
        ({'a': 0, 'b': 0, 'c': 4}, {'x': 3, 'y': 3}),
    ]

    for epoch in range(3):
        print(f"=== Epoch {epoch+1} ===")
        for inp, target in training_data:
            print("Input:", inp)
            out = layer.forward(inp)
            print("Output before update:", {k: v.a for k, v in out.items()})
            print("Target:", target)

            layer.train_step(inp, target, learning_rate=1)

            out_after = layer.forward(inp)
            print("Output after update:", {k: v.a for k, v in out_after.items()})
            print("Weights and biases states:")
            for o in outputs:
                for i in inputs:
                    print(f"weight[{o}][{i}]:", layer.weights[o][i])
                print(f"bias[{o}]:", layer.biases[o])
            print("-" * 30)

if __name__ == "__main__":
    demo28()
