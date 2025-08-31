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

class QuantumNumberV8Demo18:
    def __init__(self):
        self.grid_size = 3
        self.learning_rate = 0.001
        self.nodes = []
        self.weights = []
        self.targets = []

        # Initialize nodes with increasing coefficients for demo
        for i in range(self.grid_size ** 2):
            # a-f coefficients initialized distinctively to illustrate updates
            node = QuantumNumberV8(
                a=i+1, b=(i+1)*2, c=(i+1)*3,
                d=(i+1)*4, e=(i+1)*5, f=(i+1)*6
            )
            # weights initialized all 1.0 for demo
            weight = QuantumNumberV8(a=1.0, b=1.0, c=1.0, d=1.0, e=1.0, f=1.0)
            self.nodes.append(node)
            self.weights.append(weight)
            # targets set to fixed QuantumNumber with all coefficients = 10.0
            self.targets.append(QuantumNumberV8(a=10, b=10, c=10, d=10, e=10, f=10))

        # Link neighbors in grid (up/down/left/right)
        for r in range(self.grid_size):
            for c in range(self.grid_size):
                idx = r * self.grid_size + c
                node = self.nodes[idx]
                if c > 0:
                    node.left = self.nodes[idx - 1]
                if c < self.grid_size - 1:
                    node.right = self.nodes[idx + 1]
                if r > 0:
                    node.up = self.nodes[idx - self.grid_size]
                if r < self.grid_size - 1:
                    node.down = self.nodes[idx + self.grid_size]

    def forward(self):
        print("\n--- Forward Pass ---")
        self.outputs = []
        for idx, node in enumerate(self.nodes):
            # Sum neighbors + self coefficients
            summed = node.weighted_sum_neighbors()
            # Multiply each coefficient by corresponding weight coeff
            w = self.weights[idx]
            out = QuantumNumberV8(
                a=summed.a * w.a,
                b=summed.b * w.b,
                c=summed.c * w.c,
                d=summed.d * w.d,
                e=summed.e * w.e,
                f=summed.f * w.f
            )
            # ReLU only on 'a' for now
            if out.a < 0:
                out.a = 0
            self.outputs.append(out)
            print(f"Node[{idx}] output: a={out.a:.3f}, b={out.b:.3f}, c={out.c:.3f}, "
                  f"d={out.d:.3f}, e={out.e:.3f}, f={out.f:.3f}")

    def backward(self):
        print("\n--- Backward Pass ---")
        self.errors = []
        for idx, (output, target) in enumerate(zip(self.outputs, self.targets)):
            # Compute error per coefficient (target - output)
            err = QuantumNumberV8(
                a=target.a - output.a,
                b=target.b - output.b,
                c=target.c - output.c,
                d=target.d - output.d,
                e=target.e - output.e,
                f=target.f - output.f,
            )
            self.errors.append(err)
            print(f"Node[{idx}] error: a={err.a:.3f}, b={err.b:.3f}, c={err.c:.3f}, "
                  f"d={err.d:.3f}, e={err.e:.3f}, f={err.f:.3f}")

    def update_weights(self):
        print("\n--- Weights Update ---")
        for idx, err in enumerate(self.errors):
            w = self.weights[idx]
            old = (w.a, w.b, w.c, w.d, w.e, w.f)
            # Gradient descent weight update per coefficient
            w.a += self.learning_rate * err.a
            w.b += self.learning_rate * err.b
            w.c += self.learning_rate * err.c
            w.d += self.learning_rate * err.d
            w.e += self.learning_rate * err.e
            w.f += self.learning_rate * err.f
            print(f"Weight[{idx}] update: "
                  f"a {old[0]:.3f}->{w.a:.3f}, "
                  f"b {old[1]:.3f}->{w.b:.3f}, "
                  f"c {old[2]:.3f}->{w.c:.3f}, "
                  f"d {old[3]:.3f}->{w.d:.3f}, "
                  f"e {old[4]:.3f}->{w.e:.3f}, "
                  f"f {old[5]:.3f}->{w.f:.3f}")

    def train_step(self):
        self.forward()
        self.backward()
        self.update_weights()


if __name__ == "__main__":
    demo = QuantumNumberV8Demo18()
    for step in range(3):
        print(f"\n=== Training Step {step + 1} ===")
        demo.train_step()
