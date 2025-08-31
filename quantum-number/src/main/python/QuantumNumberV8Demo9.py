from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo9:
    def __init__(self):
        self.features = [QuantumNumberV8() for _ in range(3)]

        self.features[0].a, self.features[0].b, self.features[0].c = 6, -2, 1
        self.features[0].signs = 0b001  # 'a' negative
        self.features[0].metadata = 1

        self.features[1].a, self.features[1].b, self.features[1].c = -5, 3, 2
        self.features[1].signs = 0b010  # 'b' negative
        self.features[1].metadata = 1

        self.features[2].a, self.features[2].b, self.features[2].c = 7, -1, 1
        self.features[2].signs = 0b100  # 'c' negative
        self.features[2].metadata = 1

    def abs_val(self, qnum: QuantumNumberV8, term: str) -> int:
        val = getattr(qnum, term)
        bit = {'a': 0, 'b': 1, 'c': 2}[term]
        return -val if ((qnum.signs >> bit) & 1) else val

    def elementwise_multiply(self, q1: QuantumNumberV8, q2: QuantumNumberV8) -> QuantumNumberV8:
        result = QuantumNumberV8()
        for term, bit in zip(['a', 'b', 'c'], [0,1,2]):
            val1 = self.abs_val(q1, term)
            val2 = self.abs_val(q2, term)
            prod = val1 * val2
            sign_bit = 1 if prod < 0 else 0
            setattr(result, term, abs(prod))
            if sign_bit:
                result.signs |= (1 << bit)
            else:
                result.signs &= ~(1 << bit)

        result.metadata = max(q1.metadata, q2.metadata) + 1
        return result

    def quantum_sigmoid(self, qnum: QuantumNumberV8, k=1):
        """
        Approximate symbolic sigmoid: a / (a + k)
        No floating point; integer division but keep it symbolic by adjusting 'a' and 'b'
        We'll simulate division by 'adding k to a' and swapping roles for demo.
        """
        result = QuantumNumberV8()
        # Preserve signs from input 'a'
        a_val = self.abs_val(qnum, 'a')
        # symbolic approximation: sigmoid ~ a/(a+k)
        result.a = a_val
        result.b = a_val + k  # denominator term

        # Signs: If input 'a' negative, result 'a' negative sign, 'b' positive sign
        if (qnum.signs & 1) != 0:
            result.signs |= 1  # 'a' negative
        # 'b' always positive for denominator

        result.metadata = qnum.metadata + 1
        return result

    def normalize_by_sum_a(self):
        total_abs_a = sum(abs(f.a) for f in self.features)
        if total_abs_a == 0:
            print("Normalization skipped: sum abs a is zero")
            return
        for i, f in enumerate(self.features):
            old_a = f.a
            f.a = f.a / total_abs_a
            f.metadata += 1
            print(f"Normalized feature[{i}] a: {old_a} -> {f.a} (metadata={f.metadata})")

    def quantum_similarity(self, q1: QuantumNumberV8, q2: QuantumNumberV8):
        # similarity = dot product on 'a' + (1 / (1 + distance))
        dot = self.abs_val(q1, 'a') * self.abs_val(q2, 'a')
        dist = abs(self.abs_val(q1, 'a') - self.abs_val(q2, 'a'))
        sim = dot + 1/(1 + dist)
        print(f"Quantum similarity between a's: dot={dot}, distance={dist}, similarity={sim}")
        return sim

    def demo(self):
        print("Initial features:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]: {f}")
        print()

        print("Elementwise multiply feature 0 and feature 1:")
        mult_result = self.elementwise_multiply(self.features[0], self.features[1])
        print("Result:", mult_result)
        print()

        print("Apply quantum sigmoid on multiply result:")
        sigmoid_result = self.quantum_sigmoid(mult_result)
        print("Sigmoid result:", sigmoid_result)
        print()

        print("Add sigmoid result to feature 2 elementwise:")
        add_result = QuantumNumberV8()
        for term in ['a', 'b', 'c']:
            v = getattr(sigmoid_result, term) + getattr(self.features[2], term)
            setattr(add_result, term, v)
        # Combine signs by XOR (just for demo)
        add_result.signs = sigmoid_result.signs ^ self.features[2].signs
        add_result.metadata = max(sigmoid_result.metadata, self.features[2].metadata) + 1
        print("Add result:", add_result)
        print()

        print("Normalizing all features including add_result:")
        self.features.append(add_result)
        self.normalize_by_sum_a()
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]: {f}")
        print()

        print("Quantum similarity between feature 0 and feature 2:")
        self.quantum_similarity(self.features[0], self.features[2])

if __name__ == "__main__":
    demo = QuantumNumberV8Demo9()
    demo.demo()
