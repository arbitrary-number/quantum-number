class QuantumNumberV8:
    """
    QuantumNumberV8 from your definition.
    Simplified __init__ for demo, with optional initial values.
    """

    def __init__(self, signs=0, metadata=0, a=0, b=0, c=0, d=0, e=0, f=0):
        self.signs = signs
        self.metadata = metadata
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f

        self.left = None
        self.right = None
        self.up = None
        self.down = None
        self.in_ = None
        self.out = None

    def __repr__(self):
        def ptr_id(ptr):
            return hex(id(ptr)) if ptr else "None"
        return (f"QuantumNumberV8(signs={self.signs}, metadata={self.metadata},\n"
                f"  a={self.a}, b={self.b}, c={self.c}, d={self.d}, e={self.e}, f={self.f},\n"
                f"  left={ptr_id(self.left)}, right={ptr_id(self.right)}, up={ptr_id(self.up)}, down={ptr_id(self.down)},\n"
                f"  in_={ptr_id(self.in_)}, out={ptr_id(self.out)})")

    def copy(self):
        return QuantumNumberV8(self.signs, self.metadata, self.a, self.b, self.c, self.d, self.e, self.f)

    def add(self, other):
        """Elementwise add coefficients a-f"""
        self.a += other.a
        self.b += other.b
        self.c += other.c
        self.d += other.d
        self.e += other.e
        self.f += other.f

    def multiply_scalar(self, scalar):
        """Multiply all coefficients by scalar"""
        self.a *= scalar
        self.b *= scalar
        self.c *= scalar
        self.d *= scalar
        self.e *= scalar
        self.f *= scalar

    def relu(self):
        """Apply ReLU on 'a' coeff only for simplicity"""
        if self.a < 0:
            self.a = 0

    def weighted_sum_neighbors(self):
        """Sum self + neighbors (up, down, left, right) elementwise"""
        result = self.copy()
        for neighbor in [self.up, self.down, self.left, self.right]:
            if neighbor:
                result.add(neighbor)
        return result

class QuantumNumberV8Demo17:
    def __init__(self):
        self.grid_size = 3
        self.learning_rate = 0.01
        self.nodes = []
        self.weights = []
        self.targets = []

        # Create grid nodes and weights
        for i in range(self.grid_size ** 2):
            node = QuantumNumberV8(a=i+1)  # Node's initial 'a' increases 1..9
            weight = QuantumNumberV8(a=1.0)  # Init weights to 1.0
            self.nodes.append(node)
            self.weights.append(weight)
            self.targets.append(5.0)  # Target output for each node, arbitrary 5.0

        # Link nodes up/down/left/right in 3x3 grid
        for r in range(self.grid_size):
            for c in range(self.grid_size):
                idx = r * self.grid_size + c
                node = self.nodes[idx]
                # left neighbor
                if c > 0:
                    node.left = self.nodes[idx - 1]
                # right neighbor
                if c < self.grid_size - 1:
                    node.right = self.nodes[idx + 1]
                # up neighbor
                if r > 0:
                    node.up = self.nodes[idx - self.grid_size]
                # down neighbor
                if r < self.grid_size - 1:
                    node.down = self.nodes[idx + self.grid_size]

    def forward(self):
        print("\n--- Forward Pass ---")
        self.outputs = []
        for idx, node in enumerate(self.nodes):
            # Weighted sum neighbors and self
            neighbor_sum = node.weighted_sum_neighbors()
            # Multiply by weight
            w = self.weights[idx]
            weighted_output = neighbor_sum.copy()
            weighted_output.multiply_scalar(w.a)  # simple scalar weight
            # Apply ReLU
            weighted_output.relu()
            self.outputs.append(weighted_output)
            print(f"Node[{idx}] weighted output a={weighted_output.a:.4f}")

    def backward(self):
        print("\n--- Backward Pass ---")
        self.errors = []
        for idx, output in enumerate(self.outputs):
            error = self.targets[idx] - output.a
            self.errors.append(error)
            print(f"Node[{idx}] error = {error:.4f}")

    def update_weights(self):
        print("\n--- Weight Update ---")
        for idx, error in enumerate(self.errors):
            gradient = error  # dLoss/dOutput simplified
            old_weight = self.weights[idx].a
            self.weights[idx].a += self.learning_rate * gradient
            print(f"Weight[{idx}] updated: {old_weight:.4f} -> {self.weights[idx].a:.4f}")

    def train_step(self):
        self.forward()
        self.backward()
        self.update_weights()

if __name__ == "__main__":
    demo = QuantumNumberV8Demo17()
    # Run 5 training steps
    for step in range(5):
        print(f"\n=== Training Step {step + 1} ===")
        demo.train_step()
