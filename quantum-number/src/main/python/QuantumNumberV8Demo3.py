from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo3:
    """
    Advanced math demo with QuantumNumberV8.

    Demonstrates:
    - Multiplication and division by factors via e and f increments.
    - Scaling via d and a increments.
    - Non-collapsing exact operations in Quantum Number form.
    """

    def __init__(self):
        self.qnum = QuantumNumberV8()
        # Initialize base Quantum Number:
        # (a/(b/c)) * (d/(e/f)) = (10 / (2 / 1)) * (5 / (3 / 1))
        self.qnum.a = 10
        self.qnum.b = 2
        self.qnum.c = 1
        self.qnum.d = 5
        self.qnum.e = 3
        self.qnum.f = 1

    def multiply_by_factor_e(self, factor: int):
        """
        Multiply Quantum Number by factor by increasing 'e' (denominator of scaling).
        """
        print(f"Multiply by {factor} by incrementing 'e' (before): e = {self.qnum.e}")
        self.qnum.e += (factor - 1)
        print(f"After multiplication, e = {self.qnum.e}")

    def divide_by_factor_f(self, factor: int):
        """
        Divide Quantum Number by factor by increasing 'f' (numerator of denominator in scaling).
        """
        print(f"Divide by {factor} by incrementing 'f' (before): f = {self.qnum.f}")
        self.qnum.f += (factor - 1)
        print(f"After division, f = {self.qnum.f}")

    def scale_up_d(self, amount: int):
        """
        Scale Quantum Number up by incrementing 'd' (scaling numerator).
        """
        print(f"Scale up by {amount} via 'd' (before): d = {self.qnum.d}")
        self.qnum.d += amount
        print(f"After scaling up, d = {self.qnum.d}")

    def scale_down_a(self, amount: int):
        """
        Scale down Quantum Number by decrementing 'a' (base numerator).
        """
        print(f"Scale down by {amount} via 'a' (before): a = {self.qnum.a}")
        self.qnum.a = max(self.qnum.a - amount, 1)  # avoid zero or negative a
        print(f"After scaling down, a = {self.qnum.a}")

    def print_state(self):
        print("Quantum Number state:")
        print(self.qnum)


if __name__ == "__main__":
    demo = QuantumNumberV8Demo3()
    demo.print_state()

    demo.multiply_by_factor_e(4)   # multiply by 4 via e
    demo.print_state()

    demo.divide_by_factor_f(2)     # divide by 2 via f
    demo.print_state()

    demo.scale_up_d(3)             # scale numerator d up by 3
    demo.print_state()

    demo.scale_down_a(5)           # scale base numerator a down by 5
    demo.print_state()