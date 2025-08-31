def format_int(n):
    # Just print integer with commas for readability
    return f"{n:,}"

class QuantumNumberV8:
    def __init__(self, index, a, b, c, d, e, f):
        # Quantum numbers are integers only
        self.index = index
        self.a = int(a)
        self.b = int(b)
        self.c = int(c)
        self.d = int(d)
        self.e = int(e)
        self.f = int(f)
        # Initialize weights as integers (scaled by 1000 for example)
        self.weights = {
            'a': 1000,
            'b': 1000,
            'c': 1000,
            'd': 1000,
            'e': 1000,
            'f': 1000
        }
        self.error = {k: 0 for k in self.weights}

    def forward(self):
        # Forward pass: output quantum numbers multiplied by weights, integer math only
        output = {}
        for k in ['a','b','c','d','e','f']:
            # multiply quantum number by weight and divide by 1000 (to simulate scaling)
            output[k] = (getattr(self, k) * self.weights[k]) // 1000
        return output

    def backward(self, errors):
        # Backward pass: simple error propagation, just store errors here
        self.error = errors

    def update_weights(self):
        # Update weights based on error, weights stay integers
        # For simplicity, subtract error // 10 (scaled adjustment)
        for k in self.weights:
            # Update weight with integer arithmetic and clip >0
            new_weight = self.weights[k] - self.error[k] // 10
            self.weights[k] = max(1, new_weight)  # weights never below 1

    def print_state(self, output):
        print(f"QuantumNumberV8[{self.index}] output: " + ", ".join(
            f"{k}={format_int(output[k])}" for k in output
        ))
        print(f"QuantumNumberV8[{self.index}] weights: " + ", ".join(
            f"{k}={format_int(self.weights[k])}" for k in self.weights
        ))
        print(f"QuantumNumberV8[{self.index}] error: " + ", ".join(
            f"{k}={format_int(self.error[k])}" for k in self.error
        ))

def demo20():
    print("=== Demo 20: Integer Quantum Numbers with explicit powers of 10 ===\n")

    # Example QuantumNumberV8 instances with d, e, f holding one large power of 10 each, others 1
    quantum_numbers = [
        QuantumNumberV8(0, a=7, b=14, c=21, d=1_000_000_000, e=1, f=1),
        QuantumNumberV8(1, a=11, b=22, c=33, d=1, e=10_000_000_000, f=1),
        QuantumNumberV8(2, a=13, b=26, c=39, d=1, e=1, f=1_000_000_000),
    ]

    # Run 3 training steps
    for step in range(1,4):
        print(f"=== Training Step {step} ===\n")

        # Forward pass
        print("--- Forward Pass ---")
        outputs = []
        for qn in quantum_numbers:
            out = qn.forward()
            qn.print_state(out)
            outputs.append(out)
            print()

        # Dummy errors: difference from some target (simulate)
        print("--- Backward Pass ---")
        for qn in quantum_numbers:
            # Create dummy error for demo: error = quantum numbers * -step (arbitrary)
            error = {k: -(getattr(qn, k) * step * 10) for k in qn.weights}
            qn.backward(error)
            print(f"QuantumNumberV8[{qn.index}] error: " + ", ".join(f"{k}={format_int(error[k])}" for k in error))
        print()

        # Update weights
        print("--- Weights Update ---")
        for qn in quantum_numbers:
            before = qn.weights.copy()
            qn.update_weights()
            print(f"QuantumNumberV8[{qn.index}] weights update:")
            for k in qn.weights:
                print(f"  {k}: {format_int(before[k])} -> {format_int(qn.weights[k])}")
        print()

if __name__ == "__main__":
    demo20()
