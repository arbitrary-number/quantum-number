from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo7:
    def __init__(self):
        # Initialize features and weights vectors, like previous demo
        self.features = [QuantumNumberV8() for _ in range(3)]
        self.weights = [QuantumNumberV8() for _ in range(3)]
        self.output = QuantumNumberV8()

        # Setup initial values and signs (bitfield little endian: a=bit0, b=bit1, c=bit2, ...)
        self.features[0].a, self.features[0].b, self.features[0].c = 12, 3, 1
        self.features[0].d, self.features[0].e, self.features[0].f = 4, 2, 1
        self.features[0].signs = 0b000001  # negative 'a'

        self.features[1].a, self.features[1].b, self.features[1].c = 7, 2, 1
        self.features[1].d, self.features[1].e, self.features[1].f = 5, 3, 1
        self.features[1].signs = 0b000010  # negative 'b'

        self.features[2].a, self.features[2].b, self.features[2].c = 9, 4, 1
        self.features[2].d, self.features[2].e, self.features[2].f = 3, 1, 1
        self.features[2].signs = 0b000100  # negative 'c'

        # Initialize weights as all positive ones with metadata as version 1
        for w in self.weights:
            w.a, w.b, w.c = 1, 1, 1
            w.d, w.e, w.f = 1, 1, 1
            w.signs = 0  # all positive
            w.metadata = 1  # version or tag

    def multiply(self, qnum: QuantumNumberV8, factor: int, sign_flip=False):
        before = qnum.e
        qnum.e += factor
        if sign_flip:
            # Flip sign bit for 'a' (bit 0)
            qnum.signs ^= 0b000001
        # Update metadata: increment version count
        qnum.metadata += 1
        print(f"Multiply by {'-' if sign_flip else ''}{factor}: e before={before}, after={qnum.e}, signs={bin(qnum.signs)}, metadata={qnum.metadata}")

    def divide(self, qnum: QuantumNumberV8, factor: int, sign_flip=False):
        before = qnum.f
        qnum.f += factor
        if sign_flip:
            # Flip sign bit for 'd' (bit 3)
            qnum.signs ^= 0b0001000
        qnum.metadata += 1
        print(f"Divide by {'-' if sign_flip else ''}{factor}: f before={before}, after={qnum.f}, signs={bin(qnum.signs)}, metadata={qnum.metadata}")

    def symbolic_dot_product(self):
        """
        Symbolic dot product between features and weights:
        sum over i of (features[i].a * weights[i].a),
        combine sign bits with XOR,
        accumulate metadata as max of involved components + 1.
        """
        self.output = QuantumNumberV8()
        max_metadata = 0

        for i, (f, w) in enumerate(zip(self.features, self.weights)):
            prod_a = f.a * w.a
            prod_signs = f.signs ^ w.signs
            self.output.a += prod_a
            self.output.signs ^= prod_signs
            max_metadata = max(max_metadata, f.metadata, w.metadata)

            print(f"Dot Product [{i}]: {f.a} * {w.a} = {prod_a}, signs={bin(prod_signs)}, max_metadata={max_metadata}")

        # Set output metadata as max + 1 to indicate new derived state
        self.output.metadata = max_metadata + 1
        print(f"Output metadata set to {self.output.metadata}")

    def relu_activation(self, qnum: QuantumNumberV8):
        """
        Simple symbolic ReLU: zero out 'a' term if negative (sign bit 0 set),
        clear that sign bit accordingly.
        """
        if qnum.signs & 0b000001:  # negative 'a'
            print(f"ReLU activation: 'a' term negative, zeroing out 'a' (was {qnum.a}) and clearing sign bit")
            qnum.a = 0
            qnum.signs &= ~0b000001  # clear bit 0
            # Increase metadata due to transformation
            qnum.metadata += 1
        else:
            print("ReLU activation: 'a' term positive, no change")

    def demo(self):
        print("Initial feature vectors:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]:", f)
        print()

        # Step 1: Multiply first feature by 3 with sign flip on 'a'
        self.multiply(self.features[0], 3, sign_flip=True)

        # Step 2: Divide second feature by 2 without sign flip
        self.divide(self.features[1], 2, sign_flip=False)

        # Step 3: Multiply third feature by 1 with sign flip on 'a'
        self.multiply(self.features[2], 1, sign_flip=True)

        print("\nFeatures after multiply/divide:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]:", f)
        print()

        print("Perform symbolic dot product of features and weights:")
        self.symbolic_dot_product()
        print("\nOutput before activation:")
        print(self.output)
        print()

        print("Apply symbolic ReLU activation to output:")
        self.relu_activation(self.output)
        print("\nOutput after activation:")
        print(self.output)

if __name__ == "__main__":
    demo = QuantumNumberV8Demo7()
    demo.demo()
