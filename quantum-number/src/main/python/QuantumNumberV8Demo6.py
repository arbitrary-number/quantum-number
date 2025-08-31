from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo6:
    def __init__(self):
        # Create a vector of 3 QuantumNumberV8 numbers, simulating features
        self.features = [QuantumNumberV8() for _ in range(3)]
        self.weights = [QuantumNumberV8() for _ in range(3)]
        self.output = QuantumNumberV8()

        # Initialize features with some values and signs
        self.features[0].a, self.features[0].b, self.features[0].c = 10, 2, 1
        self.features[0].d, self.features[0].e, self.features[0].f = 5, 3, 1
        self.features[0].signs = 0b000001  # negative 'a' term

        self.features[1].a, self.features[1].b, self.features[1].c = 6, 1, 1
        self.features[1].d, self.features[1].e, self.features[1].f = 7, 2, 1
        self.features[1].signs = 0b000100  # negative 'd' term

        self.features[2].a, self.features[2].b, self.features[2].c = 8, 3, 1
        self.features[2].d, self.features[2].e, self.features[2].f = 4, 1, 1
        self.features[2].signs = 0b001000  # negative 'e' term

        # Initialize weights, all positive, simple scaling factors
        for w in self.weights:
            w.a, w.b, w.c = 1, 1, 1
            w.d, w.e, w.f = 1, 1, 1
            w.signs = 0  # all positive

    def multiply(self, qnum: QuantumNumberV8, factor: int, sign_flip=False):
        """
        Multiply QuantumNumberV8 by a factor via additive scaling on e,
        flipping signs if requested.
        """
        before = qnum.e
        qnum.e += factor
        if sign_flip:
            # Flip sign bit of 'a' (bit 0)
            qnum.signs ^= 0b000001
        print(f"Multiply by {'-' if sign_flip else ''}{factor}: e before={before}, after={qnum.e}, signs={bin(qnum.signs)}")

    def divide(self, qnum: QuantumNumberV8, factor: int, sign_flip=False):
        """
        Divide QuantumNumberV8 by a factor via additive scaling on f,
        flipping signs if requested.
        """
        before = qnum.f
        qnum.f += factor
        if sign_flip:
            # Flip sign bit of 'd' (bit 3)
            qnum.signs ^= 0b0001000
        print(f"Divide by {'-' if sign_flip else ''}{factor}: f before={before}, after={qnum.f}, signs={bin(qnum.signs)}")

    def weighted_sum(self):
        """
        Compute output = sum(features[i] * weights[i]) element-wise.
        Multiplication is symbolic: multiply 'a' terms and add signs accordingly.
        """
        # Reset output
        self.output = QuantumNumberV8()

        for i, (f, w) in enumerate(zip(self.features, self.weights)):
            # Symbolic multiplication:
            # Multiply 'a' terms
            prod_a = f.a * w.a
            # Combine signs XOR for 'a' term
            prod_signs = f.signs ^ w.signs

            # Accumulate to output 'a' and signs
            self.output.a += prod_a
            self.output.signs ^= prod_signs

            print(f"Feature[{i}] * Weight[{i}]: a={f.a}*{w.a}={prod_a}, signs={bin(prod_signs)}")

        print("Weighted sum output 'a':", self.output.a)
        print("Weighted sum output signs bitfield:", bin(self.output.signs))

    def demo(self):
        print("Initial feature vectors:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]:", f)
        print()

        # Multiply features[0] by 2 and flip sign on 'a'
        self.multiply(self.features[0], 2, sign_flip=True)
        # Divide features[1] by 3 without sign flip
        self.divide(self.features[1], 3, sign_flip=False)
        # Multiply features[2] by 1 with sign flip on 'a'
        self.multiply(self.features[2], 1, sign_flip=True)

        print("\nFeatures after multiplication/division:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]:", f)
        print()

        print("Perform weighted sum of features:")
        self.weighted_sum()
        print("\nOutput Quantum Number state:")
        print(self.output)


if __name__ == "__main__":
    demo = QuantumNumberV8Demo6()
    demo.demo()
