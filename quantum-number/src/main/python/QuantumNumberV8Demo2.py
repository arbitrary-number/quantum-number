from QuantumNumberV8 import QuantumNumberV8

class QuantumNumberV8Demo2:
    """
    Demo symbolic multiplication and division by modifying 'e' via addition/subtraction.
    Does NOT compute numeric values — purely symbolic manipulations.
    """

    def __init__(self):
        self.qnum = QuantumNumberV8()
        # Initialize with example symbolic numbers
        self.qnum.a = 10
        self.qnum.b = 2
        self.qnum.c = 1
        self.qnum.d = 5
        self.qnum.e = 3
        self.qnum.f = 1

    def multiply_by_factor_via_e(self, factor: int):
        """
        Multiply by a factor by adding (scaling) 'e'.
        For symbolic demo, multiplication by factor means:
        e_new = e_old + (factor - 1)
        (assuming factor >=1)
        """
        print(f"Multiplying by {factor} via addition on e (before): e = {self.qnum.e}")
        self.qnum.e += (factor - 1)
        print(f"After multiply, e = {self.qnum.e}")

    def divide_by_factor_via_e(self, factor: int):
        """
        Divide by a factor by subtracting from 'e'.
        For symbolic demo, division by factor means:
        e_new = max(e_old - (factor - 1), 1)
        to avoid zero or negative denominator.
        """
        print(f"Dividing by {factor} via subtraction on e (before): e = {self.qnum.e}")
        self.qnum.e = max(self.qnum.e - (factor - 1), 1)
        print(f"After divide, e = {self.qnum.e}")

    def print_state(self):
        print("Current symbolic QuantumNumberV8 state:")
        print(self.qnum)


if __name__ == "__main__":
    demo = QuantumNumberV8Demo2()
    demo.print_state()

    demo.multiply_by_factor_via_e(3)  # multiply by 3
    demo.print_state()

    demo.divide_by_factor_via_e(2)    # divide by 2
    demo.print_state()
