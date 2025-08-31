# QuantumNumberV8Demo12.py

class QuantumNumberV8:
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
        return (f"QuantumNumberV8(signs={self.signs}, metadata={self.metadata},\n"
                f"  a={self.a}, b={self.b}, c={self.c}, d={self.d}, e={self.e}, f={self.f},\n"
                f"  left={self.left}, right={self.right}, up={self.up}, down={self.down},\n"
                f"  in_={self.in_}, out={self.out})")

# Demo12 class implementing your example workflow
class QuantumNumberV8Demo12:
    def __init__(self):
        # Initialize feature vectors
        self.features = []

        q0 = QuantumNumberV8()
        q0.signs = 0b001
        q0.a = 1
        q0.b = 2
        q0.c = 1
        q0.metadata = 1
        self.features.append(q0)

        q1 = QuantumNumberV8()
        q1.signs = 0b010
        q1.a = -1
        q1.b = 0
        q1.c = 2
        q1.metadata = 1
        self.features.append(q1)

        q2 = QuantumNumberV8()
        q2.signs = 0b100
        q2.a = 3
        q2.b = -1
        q2.c = 1
        q2.metadata = 1
        self.features.append(q2)

        # Initialize weights similarly
        self.weights = []

        w0 = QuantumNumberV8()
        w0.signs = 0b000
        w0.a = 1
        w0.b = 1
        w0.c = 1
        w0.metadata = 1
        self.weights.append(w0)

        w1 = QuantumNumberV8()
        w1.signs = 0b001
        w1.a = -1
        w1.b = 1
        w1.c = 2
        w1.metadata = 1
        self.weights.append(w1)

        w2 = QuantumNumberV8()
        w2.signs = 0b010
        w2.a = 2
        w2.b = -1
        w2.c = 1
        w2.metadata = 1
        self.weights.append(w2)

    def weighted_sum(self):
        # Sum feature * weight (only on 'a' terms here, extend as needed)
        output = QuantumNumberV8()
        output.signs = 0
        output.metadata = 0
        output.a = 0

        for f, w in zip(self.features, self.weights):
            output.a += f.a * w.a
            output.signs |= (f.signs | w.signs)
            output.metadata = max(output.metadata, f.metadata, w.metadata)

        return output

    def relu_activation(self, qnum):
        # Simple ReLU: if a < 0, zero it and clear sign bit on 'a'
        if qnum.a < 0:
            qnum.a = 0
            qnum.signs &= ~0b001  # Clear sign bit for 'a'
            qnum.metadata += 1  # Update metadata to indicate modification
        return qnum

    def gradient_step(self, output, target):
        error = target - output.a
        print(f"Gradient step, error = {error:.4f}")

        # Update weights a,b,c by a fraction of error times corresponding feature terms
        learning_rate = 0.05
        for i in range(len(self.weights)):
            self.weights[i].a += learning_rate * error * self.features[i].a
            self.weights[i].b += learning_rate * error * self.features[i].b
            self.weights[i].c += learning_rate * error * self.features[i].c
            print(f"Weight[{i}] updated a={self.weights[i].a:.4f}, b={self.weights[i].b:.4f}, c={self.weights[i].c:.4f}")

    def run(self):
        print("Initial features:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]: {f}")

        print("\nInitial weights:")
        for i, w in enumerate(self.weights):
            print(f"Weight[{i}]: {w}")

        output = self.weighted_sum()
        print(f"\nWeighted sum output before activation: {output}")

        output = self.relu_activation(output)
        print(f"Weighted sum output after ReLU activation: {output}")

        target_value = 10
        print(f"\nTarget value: {target_value}")

        self.gradient_step(output, target_value)

        output = self.weighted_sum()
        output = self.relu_activation(output)
        print(f"\nUpdated weighted sum output after gradient step and activation: {output}")

if __name__ == "__main__":
    demo = QuantumNumberV8Demo12()
    demo.run()
