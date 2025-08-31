from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo4:
    """
    Demo4 illustrates how the sign bits in QuantumNumberV8.signs control
    the sign of each component (a,b,c,d,e,f) in the expression:
        (±a / (±b / ±c)) * (±d / (±e / ±f))

    The signs field is a bitfield where each bit corresponds to sign of each term:
    bit 0 = sign of a (0=+, 1=-)
    bit 1 = sign of b
    bit 2 = sign of c
    bit 3 = sign of d
    bit 4 = sign of e
    bit 5 = sign of f

    We'll show how toggling each bit flips the sign of that term.
    """

    SIGN_NAMES = ['a', 'b', 'c', 'd', 'e', 'f']

    def __init__(self):
        self.qnum = QuantumNumberV8()

        # Initialize values to non-zero for demo clarity
        self.qnum.a = 10
        self.qnum.b = 2
        self.qnum.c = 1
        self.qnum.d = 5
        self.qnum.e = 3
        self.qnum.f = 1

        # Initially all positive (sign bits 0)
        self.qnum.signs = 0

    def print_state(self):
        def sign_of(bit_index):
            return '-' if (self.qnum.signs >> bit_index) & 1 else '+'

        # Build string showing each component with its sign
        parts = []
        for i, name in enumerate(self.SIGN_NAMES):
            sign = sign_of(i)
            val = getattr(self.qnum, name)
            parts.append(f"{sign}{val}")

        expr_str = f"({parts[0]} / ({parts[1]} / {parts[2]})) * ({parts[3]} / ({parts[4]} / {parts[5]}))"
        print(f"Quantum Number signs bitfield: {self.qnum.signs:06b}")
        print(f"Quantum Number expression: {expr_str}")

    def toggle_sign(self, term_index):
        """
        Toggle the sign bit of a given term (0-5).
        """
        self.qnum.signs ^= (1 << term_index)

    def demo(self):
        print("Initial Quantum Number state (all positive):")
        self.print_state()

        print("\nToggling signs one by one:")
        for i, name in enumerate(self.SIGN_NAMES):
            print(f"\nToggle sign of '{name}':")
            self.toggle_sign(i)
            self.print_state()

        print("\nToggle all signs back to positive:")
        self.qnum.signs = 0
        self.print_state()


if __name__ == "__main__":
    demo = QuantumNumberV8Demo4()
    demo.demo()
