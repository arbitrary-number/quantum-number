from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo14:
    def __init__(self):
        self.features = []
        self.weights = []

        # Initialize features
        q0 = QuantumNumberV8()
        q0.signs = 1
        q0.metadata = 1
        q0.a = 2
        q0.b = 1
        q0.c = 0
        self.features.append(q0)

        q1 = QuantumNumberV8()
        q1.signs = 2
        q1.metadata = 1
        q1.a = -1
        q1.b = 3
        q1.c = 1
        self.features.append(q1)

        q2 = QuantumNumberV8()
        q2.signs = 4
        q2.metadata = 1
        q2.a = 4
        q2.b = -2
        q2.c = 2
        self.features.append(q2)

        # Initialize weights
        w0 = QuantumNumberV8()
        w0.signs = 0
        w0.metadata = 1
        w0.a = 0.5
        w0.b = 1.0
        w0.c = -0.5
        self.weights.append(w0)

        w1 = QuantumNumberV8()
        w1.signs = 1
        w1.metadata = 1
        w1.a = -1.5
        w1.b = 0.5
        w1.c = 1.0
        self.weights.append(w1)

        w2 = QuantumNumberV8()
        w2.signs = 2
        w2.metadata = 1
        w2.a = 2.0
        w2.b = -1.0
        w2.c = 0.5
        self.weights.append(w2)

    def weighted_sum(self):
        output = QuantumNumberV8()
        output.signs = 0
        output.metadata = 0
        output.a = 0
        output.b = 0
        output.c = 0

        for f, w in zip(self.features, self.weights):
            output.a += f.a * w.a
            output.b += f.b * w.b
            output.c += f.c * w.c
            output.signs |= (f.signs | w.signs)
            output.metadata = max(output.metadata, f.metadata, w.metadata)

        return output

    def relu_activation(self, qnum):
        # Apply ReLU individually to a,b,c terms
        for attr in ['a','b','c']:
            val = getattr(qnum, attr)
            if val < 0:
                setattr(qnum, attr, 0)
                # Clear the bit for that attr in signs (assuming a=bit0, b=bit1, c=bit2)
                qnum.signs &= ~(1 << ['a','b','c'].index(attr))
                qnum.metadata += 1
        return qnum

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

if __name__ == "__main__":
    demo = QuantumNumberV8Demo14()
    demo.run()
