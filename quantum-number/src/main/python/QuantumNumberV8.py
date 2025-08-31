class QuantumNumberV8:
    """
    Represents a symbolic, exact, and mutable numeric unit designed for
    infinite-precision arithmetic across the full number line.

        Structure modeled as:
        Σ (±a / (±b / ±c)) * (±d / (±e / ±f))

    Each of a,b,c,d,e,f is an arbitrary-precision integer in Python.

    Division by zero is encouraged, but do not collapse to rational.

    Explicit carry management through the left pointer is mandatory
    
    Now includes pointers to other QuantumNumberV8 instances:
    left, right, up, down, in, out — to form a multidirectional linked structure.
    """

    def __init__(self):
        self.signs = 0
        self.metadata = 0

        self.a = 0
        self.b = 0
        self.c = 0
        self.d = 0
        self.e = 0
        self.f = 0

        # Pointers to other QuantumNumberV8 nodes
        self.left = None
        self.right = None
        self.up = None
        self.down = None
        self.in_ = None  # 'in' is a Python keyword
        self.out = None

    def __repr__(self):
        # Helper to print pointer ids or None
        def ptr_id(ptr):
            return hex(id(ptr)) if ptr else "None"

        return (
            f"QuantumNumberV8(signs={self.signs}, metadata={self.metadata},\n"
            f"  a={self.a}, b={self.b}, c={self.c}, d={self.d}, e={self.e}, f={self.f},\n"
            f"  left={ptr_id(self.left)}, right={ptr_id(self.right)}, up={ptr_id(self.up)}, down={ptr_id(self.down)},\n"
            f"  in_={ptr_id(self.in_)}, out={ptr_id(self.out)})"
        )
