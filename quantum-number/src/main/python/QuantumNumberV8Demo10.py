from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo10:
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

        self.learning_rate = 0.1

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
        # Weighted sum of features * weights on 'a' terms with signs
        total = QuantumNumberV8()
        total.a = 0
        total.signs = 0
        total.metadata = 0
        for f, w in zip(self.features, self.weights):
            mul = self.multiply_feature_weight(f, w)
            total.a += mul.a
            total.signs |= mul.signs
            total.metadata = max(total.metadata, mul.metadata)
        return total

    def gradient_step(self, output, target):
        # Simple gradient descent update for weights based on difference
        error = target - output.a
        print(f"Error: {error}")

        for i, (f, w) in enumerate(zip(self.features, self.weights)):
            grad = error * f.a
            w.a += self.learning_rate * grad
            print(f"Updated weight[{i}].a: {w.a:.4f}")

    def run(self):
        print("Initial features:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]: {f}")

        print("\nInitial weights:")
        for i, w in enumerate(self.weights):
            print(f"Weight[{i}]: {w}")

        output = self.weighted_sum()
        print(f"\nWeighted sum output: {output}")

        target_value = 15
        print(f"\nTarget value: {target_value}")

        self.gradient_step(output, target_value)

        updated_output = self.weighted_sum()
        print(f"\nUpdated weighted sum output: {updated_output}")

if __name__ == "__main__":
    demo = QuantumNumberV8Demo10()
    demo.run()
