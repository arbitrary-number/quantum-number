def format_int(n):
    return f"{n:,}"

class QuantumNumberV8:
    def __init__(self, index, a, b, c, d, e, f):
        self.index = index
        self.a = int(a)
        self.b = int(b)
        self.c = int(c)
        self.d = int(d)
        self.e = int(e)
        self.f = int(f)

        # Initialize weights scaled by 1000 (integer math)
        self.weights = {k: 1000 for k in ['a','b','c','d','e','f']}
        self.error = {k: 0 for k in self.weights}

    def forward(self):
        output = {}
        for k in ['a','b','c','d','e','f']:
            val = getattr(self, k)
            output[k] = (val * self.weights[k]) // 1000
        return output

    def backward(self, step):
        # Simulate error proportional to quantum numbers and step, scaled by 10
        self.error = {k: -(getattr(self, k) * step * 10) for k in self.weights}

    def update_weights(self):
        # Update weights by subtracting error//20 to slow growth, weights ≥ 1
        for k in self.weights:
            new_w = self.weights[k] - (self.error[k] // 20)
            self.weights[k] = max(1, new_w)

    def print_state(self, output):
        print(f"QuantumNumberV8[{self.index}] output:")
        print(", ".join(f"{k}={format_int(output[k])}" for k in output))
        print("Weights:")
        print(", ".join(f"{k}={format_int(self.weights[k])}" for k in self.weights))
        print("Error:")
        print(", ".join(f"{k}={format_int(self.error[k])}" for k in self.error))
        print()

def demo21():
    print("=== Demo 21: Integer Quantum Numbers & Scaled Weights with Controlled Updates ===\n")

    # Setup: Large quantum numbers with one large power of 10 per instance in d, e, or f
    quantum_numbers = [
        QuantumNumberV8(0, a=5, b=10, c=15, d=1_000_000_000, e=1, f=1),
        QuantumNumberV8(1, a=7, b=14, c=21, d=1, e=10_000_000_000, f=1),
        QuantumNumberV8(2, a=9, b=18, c=27, d=1, e=1, f=1_000_000_000),
    ]

    steps = 5
    for step in range(1, steps+1):
        print(f"=== Step {step} ===")

        print("Forward pass:")
        for qn in quantum_numbers:
            out = qn.forward()
            qn.print_state(out)

        print("Backward pass (calculating errors):")
        for qn in quantum_numbers:
            qn.backward(step)
            print(f"QuantumNumberV8[{qn.index}] error:")
            print(", ".join(f"{k}={format_int(qn.error[k])}" for k in qn.error))
        print()

        print("Updating weights:")
        for qn in quantum_numbers:
            before = qn.weights.copy()
            qn.update_weights()
            print(f"QuantumNumberV8[{qn.index}] weights update:")
            for k in qn.weights:
                print(f"  {k}: {format_int(before[k])} -> {format_int(qn.weights[k])}")
            print()

    print("=== Final State After All Steps ===")
    for qn in quantum_numbers:
        out = qn.forward()
        qn.print_state(out)

if __name__ == "__main__":
    demo21()
