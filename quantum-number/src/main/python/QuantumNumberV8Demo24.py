class QuantumNumber:
    def __init__(self, digits=None, base=10, sign=1):
        self.base = base
        self.sign = sign  # 1 for positive, -1 for negative
        if digits is None:
            self.digits = [0]
        else:
            self.digits = digits
            self._normalize()

    def _normalize(self):
        # Remove leading zeros
        while len(self.digits) > 1 and self.digits[-1] == 0:
            self.digits.pop()
        # If zero, sign always positive
        if len(self.digits) == 1 and self.digits[0] == 0:
            self.sign = 1

    def to_int(self):
        val = 0
        for i in reversed(range(len(self.digits))):
            val = val * self.base + self.digits[i]
        return val * self.sign

    def add(self, value):
        # Add integer value (can be negative)
        if value == 0:
            return
        current_val = self.to_int()
        new_val = current_val + value
        self.sign = 1 if new_val >= 0 else -1
        abs_val = abs(new_val)
        self.digits = []
        if abs_val == 0:
            self.digits = [0]
            self.sign = 1
        else:
            while abs_val > 0:
                self.digits.append(abs_val % self.base)
                abs_val //= self.base
        self._normalize()

    def __repr__(self):
        prefix = '-' if self.sign < 0 else ''
        return prefix + ''.join(str(d) for d in reversed(self.digits))


class QuantumNumberLayer:
    def __init__(self, input_keys, output_keys, base=10):
        self.base = base
        self.input_keys = input_keys
        self.output_keys = output_keys
        # Initialize weights and biases as QuantumNumbers (start at zero)
        self.weights = {o: {i: QuantumNumber([0], base) for i in input_keys} for o in output_keys}
        self.biases = {o: QuantumNumber([0], base) for o in output_keys}

    def forward(self, inp):
        out = {}
        for o in self.output_keys:
            total = self.biases[o].to_int()
            for i in self.input_keys:
                total += self.weights[o][i].to_int() * inp.get(i, 0)
            # Result as QuantumNumber (for consistency)
            out[o] = QuantumNumber([], self.base)
            out[o].add(total)
        return out

    def train_step(self, inp, target, learning_rate=1):
        output = self.forward(inp)
        print(f"Input: {inp}")
        print(f"Output before update: {{'{self.output_keys[0]}': {output[self.output_keys[0]]}, '{self.output_keys[1]}': {output[self.output_keys[1]]}}}")
        print(f"Target: {target}")

        for o in self.output_keys:
            error = target[o] - output[o].to_int()
            # Update weights and bias by error * input * learning rate
            for i in self.input_keys:
                delta = int(error * inp.get(i, 0) * learning_rate)
                if delta != 0:
                    print(f"Updating weight[{o}][{i}] by {delta}")
                    self.weights[o][i].add(delta)
            delta_bias = int(error * learning_rate)
            if delta_bias != 0:
                print(f"Updating bias[{o}] by {delta_bias}")
                self.biases[o].add(delta_bias)
        print()

    def print_state(self):
        print("Layer state after updates:")
        print("Weights:")
        for o in self.output_keys:
            print(f"  {o}: ", end="")
            for i in self.input_keys:
                print(f"{i}={self.weights[o][i]}", end=" ")
            print()
        print("Biases:")
        for o in self.output_keys:
            print(f"  {o}: {self.biases[o]}")
        print()


def demo24():
    input_keys = ['a', 'b', 'c']
    output_keys = ['x', 'y']
    layer = QuantumNumberLayer(input_keys, output_keys, base=10)

    training_data = [
        ({'a': 1, 'b': 2, 'c': 3}, {'x': 5, 'y': 4}),
        ({'a': 3, 'b': 1, 'c': 0}, {'x': 6, 'y': 2}),
        ({'a': 0, 'b': 0, 'c': 4}, {'x': 3, 'y': 3}),
    ]

    steps = 5
    for step in range(1, steps + 1):
        print(f"--- Step {step} ---")
        for inp, target in training_data:
            layer.train_step(inp, target, learning_rate=1)
        layer.print_state()

    print("=== Demo 24 complete: explicit carry propagation managed for all weight updates ===")

if __name__ == "__main__":
    demo24()
