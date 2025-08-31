from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo11:
    def __init__(self):
        self.features = []
        q0 = QuantumNumberV8()
        q0.signs = 0b001
        q0.a = 3
        q0.b = 2
        q0.c = 1
        q0.metadata = 1
        self.features.append(q0)

        q1 = QuantumNumberV8()
        q1.signs = 0b010
        q1.a = -4
        q1.b = 1
        q1.c = 2
        q1.metadata = 1
        self.features.append(q1)

        q2 = QuantumNumberV8()
        q2.signs = 0b100
        q2.a = 5
        q2.b = -3
        q2.c = 1
        q2.metadata = 1
        self.features.append(q2)

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

        self.learning_rate = 0.05
        self.momentum = 0.9
        # Initialize velocity for momentum as zero QuantumNumberV8 instances matching weights
        self.velocity = [QuantumNumberV8() for _ in self.weights]

    def multiply_feature_weight(self, feature, weight):
        # Element-wise multiply a,b,c terms and combine signs and metadata
        result = QuantumNumberV8()
        result.a = feature.a * weight.a
        result.b = feature.b * weight.b
        result.c = feature.c * weight.c
        result.signs = feature.signs | weight.signs
        result.metadata = max(feature.metadata, weight.metadata)
        return result

    def weighted_sum(self):
        # Weighted sum of features * weights on a,b,c terms with combined signs and metadata
        total = QuantumNumberV8()
        total.a = 0
        total.b = 0
        total.c = 0
        total.signs = 0
        total.metadata = 0
        for f, w in zip(self.features, self.weights):
            mul = self.multiply_feature_weight(f, w)
            total.a += mul.a
            total.b += mul.b
            total.c += mul.c
            total.signs |= mul.signs
            total.metadata = max(total.metadata, mul.metadata)
        return total

    def relu_activation(self, qnum):
        # Simple ReLU on terms a,b,c and adjust signs accordingly
        for term in ['a', 'b', 'c']:
            val = getattr(qnum, term)
            if val < 0:
                setattr(qnum, term, 0)
                # Clear any sign bit related to this term (just example: bit 0 for 'a', 1 for 'b', 2 for 'c')
                qnum.signs &= ~(1 << ['a','b','c'].index(term))
        qnum.metadata += 1  # Track activation applied
        return qnum

    def gradient_step(self, output, target):
        error = target - output.a  # Use 'a' term error for simplicity
        print(f"\nGradient step, error = {error:.4f}")

        for i, (f, w, v) in enumerate(zip(self.features, self.weights, self.velocity)):
            # Calculate gradient for each term (a,b,c)
            grad_a = error * f.a
            grad_b = error * f.b
            grad_c = error * f.c

            # Momentum update
            v.a = self.momentum * v.a + self.learning_rate * grad_a
            v.b = self.momentum * v.b + self.learning_rate * grad_b
            v.c = self.momentum * v.c + self.learning_rate * grad_c

            # Update weights
            w.a += v.a
            w.b += v.b
            w.c += v.c

            print(f"Weight[{i}] updated a={w.a:.4f}, b={w.b:.4f}, c={w.c:.4f}")

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

        updated_output = self.weighted_sum()
        updated_output = self.relu_activation(updated_output)
        print(f"\nUpdated weighted sum output after gradient step and activation: {updated_output}")

if __name__ == "__main__":
    demo = QuantumNumberV8Demo11()
    demo.run()
