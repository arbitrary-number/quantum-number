from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo8:
    def __init__(self):
        # Setup 3 example feature vectors with signs & metadata
        self.features = [QuantumNumberV8() for _ in range(3)]

        # Initialize with arbitrary data and signs (bit0=a, bit1=b, bit2=c ...)
        self.features[0].a, self.features[0].b, self.features[0].c = 5, -3, 1
        self.features[0].signs = 0b000001  # 'a' negative
        self.features[0].metadata = 1

        self.features[1].a, self.features[1].b, self.features[1].c = -7, 2, 1
        self.features[1].signs = 0b000010  # 'b' negative
        self.features[1].metadata = 1

        self.features[2].a, self.features[2].b, self.features[2].c = 4, -1, 2
        self.features[2].signs = 0b000100  # 'c' negative
        self.features[2].metadata = 1

    def abs_val(self, qnum: QuantumNumberV8, term: str) -> int:
        """Get absolute value of a term, applying sign from signs bitfield."""
        val = getattr(qnum, term)
        bit = {'a': 0, 'b': 1, 'c': 2}[term]
        sign_bit_set = (qnum.signs >> bit) & 1
        return -val if sign_bit_set else val

    def normalize_features(self):
        """Normalize each feature vector's 'a' term by sum of abs of 'a' in all features."""
        total_abs_a = sum(abs(f.a) for f in self.features)
        if total_abs_a == 0:
            print("Normalization skipped: total absolute 'a' term is zero")
            return

        for i, f in enumerate(self.features):
            old_a = f.a
            normalized_a = f.a / total_abs_a  # Keep sign by dividing actual value
            f.a = normalized_a
            f.metadata += 1
            print(f"Normalized feature[{i}] 'a': {old_a} -> {normalized_a} (metadata={f.metadata})")

    def elementwise_add(self, q1: QuantumNumberV8, q2: QuantumNumberV8) -> QuantumNumberV8:
        """Element-wise addition of a,b,c terms, managing signs bitwise by XOR."""
        result = QuantumNumberV8()
        for term, bit in zip(['a', 'b', 'c'], [0,1,2]):
            val1 = self.abs_val(q1, term)
            val2 = self.abs_val(q2, term)
            sum_val = val1 + val2
            sign_bit = 0 if sum_val >= 0 else 1
            setattr(result, term, abs(sum_val))
            # Set or clear sign bit accordingly
            if sign_bit:
                result.signs |= (1 << bit)
            else:
                result.signs &= ~(1 << bit)

        # Combine metadata as max + 1 (operation count)
        result.metadata = max(q1.metadata, q2.metadata) + 1
        return result

    def dot_product_full_terms(self):
        """
        Dot product across all features on a,b,c terms:
        sum_i sum_terms (feature_i.term * feature_i.term),
        combine sign bits with XOR for all terms.
        """
        output = QuantumNumberV8()
        max_metadata = 0
        for i, f in enumerate(self.features):
            partial_sum = 0
            combined_signs = 0
            for term, bit in zip(['a','b','c'], [0,1,2]):
                val = self.abs_val(f, term)
                partial_sum += val * val  # Square as example
                # XOR sign bits (term-level)
                if (f.signs >> bit) & 1:
                    combined_signs ^= (1 << bit)
            output.a += partial_sum
            output.signs ^= combined_signs
            max_metadata = max(max_metadata, f.metadata)

            print(f"Feature[{i}] squared sum: {partial_sum}, combined signs: {bin(combined_signs)}")

        output.metadata = max_metadata + 1
        return output

    def batch_sum(self, qnums):
        """Sum a list of QuantumNumberV8 elementwise, similar to elementwise_add chained."""
        total = QuantumNumberV8()
        for q in qnums:
            total = self.elementwise_add(total, q)
        print(f"Batch sum metadata: {total.metadata}")
        return total

    def quantum_number_distance(self, q1: QuantumNumberV8, q2: QuantumNumberV8):
        """Sum of absolute differences of 'a' terms (sign-aware)."""
        diff = abs(self.abs_val(q1, 'a') - self.abs_val(q2, 'a'))
        print(f"Quantum Number distance (|a1 - a2|): {diff}")
        return diff

    def demo(self):
        print("Initial features:")
        for i, f in enumerate(self.features):
            print(f"Feature[{i}]: {f}")
        print()

        print("Normalizing features by sum of absolute 'a' terms...")
        self.normalize_features()
        for i, f in enumerate(self.features):
            print(f"Feature[{i}] after normalization: {f}")
        print()

        print("Adding feature 0 and feature 1 element-wise...")
        add_result = self.elementwise_add(self.features[0], self.features[1])
        print("Result:", add_result)
        print()

        print("Computing full dot product on a,b,c terms squared sum...")
        dot_result = self.dot_product_full_terms()
        print("Dot product result:", dot_result)
        print()

        print("Batch sum of all features...")
        batch_result = self.batch_sum(self.features)
        print("Batch sum result:", batch_result)
        print()

        print("Quantum number distance between feature 0 and 2:")
        self.quantum_number_distance(self.features[0], self.features[2])

if __name__ == "__main__":
    demo = QuantumNumberV8Demo8()
    demo.demo()
