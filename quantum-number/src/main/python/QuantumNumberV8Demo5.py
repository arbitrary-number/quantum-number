from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo5:
    """
    Demo5 adds multiplication and division on QuantumNumberV8 values
    respecting sign bits in the signs field.

    Multiplication/division by integer factors is done by incrementing/decrementing
    the appropriate fields, plus toggling sign bits if the factor is negative.

    Expression format:
        (±a / (±b / ±c)) * (±d / (±e / ±f))
    """

    SIGN_NAMES = ['a', 'b', 'c', 'd', 'e', 'f']

    def __init__(self):
        self.qnum = QuantumNumberV8()

        # Start with some default positive values
        self.qnum.a = 10
        self.qnum.b = 2
        self.qnum.c = 1
        self.qnum.d = 5
        self.qnum.e = 3
        self.qnum.f = 1
        self.qnum.signs = 0  # all positive

    def print_state(self):
        def sign_of(bit_index):
            return '-' if (self.qnum.signs >> bit_index) & 1 else '+'

        parts = []
        for i, name in enumerate(self.SIGN_NAMES):
            sign = sign_of(i)
            val = getattr(self.qnum, name)
            parts.append(f"{sign}{val}")

        expr_str = f"({parts[0]} / ({parts[1]} / {parts[2]})) * ({parts[3]} / ({parts[4]} / {parts[5]}))"
        print(f"Quantum Number signs bitfield: {self.qnum.signs:06b}")
        print(f"Quantum Number expression: {expr_str}\n")

    def toggle_sign_bit(self, term_index):
        """Toggle the sign bit at term_index."""
        self.qnum.signs ^= (1 << term_index)

    def multiply_by(self, factor):
        """
        Multiply the quantum number by an integer factor.
        For demo simplicity, we multiply by adjusting 'e' and toggling signs accordingly.

        factor: int (can be negative)
        """
        print(f"Multiplying by {factor}")

        # Handle sign of factor
        if factor < 0:
            # Toggle signs of numerator components (a and d) to represent multiplication by negative
            self.toggle_sign_bit(0)  # a
            self.toggle_sign_bit(3)  # d
            factor = -factor

        # Multiply factor by incrementing 'e' (scaling denominator of scaling coefficient)
        # We'll just add factor to 'e' for demo purposes
        before = self.qnum.e
        self.qnum.e += factor
        print(f"e before multiply: {before}, after multiply: {self.qnum.e}")

    def divide_by(self, divisor):
        """
        Divide the quantum number by an integer divisor.
        For demo simplicity, adjust 'f' and toggle signs accordingly.

        divisor: int (can be negative)
        """
        print(f"Dividing by {divisor}")

        if divisor < 0:
            # Toggle signs of denominator components (b and e) to represent division by negative
            self.toggle_sign_bit(1)  # b
            self.toggle_sign_bit(4)  # e
            divisor = -divisor

        # Divide by incrementing 'f' (numerator of denominator of scaling coefficient)
        before = self.qnum.f
        self.qnum.f += divisor
        print(f"f before divide: {before}, after divide: {self.qnum.f}")

    def demo(self):
        print("Initial Quantum Number state:")
        self.print_state()

        self.multiply_by(3)
        self.print_state()

        self.multiply_by(-2)
        self.print_state()

        self.divide_by(5)
        self.print_state()

        self.divide_by(-4)
        self.print_state()

if __name__ == "__main__":
    demo = QuantumNumberV8Demo5()
    demo.demo()
