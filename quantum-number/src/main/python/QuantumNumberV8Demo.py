from QuantumNumberV8 import QuantumNumberV8  # assuming your class is in quantum_number_v8.py


class QuantumNumberV8Demo:
    def __init__(self):
        # Create nodes
        self.root = QuantumNumberV8()
        self.left = QuantumNumberV8()
        self.right = QuantumNumberV8()
        self.up = QuantumNumberV8()
        self.down = QuantumNumberV8()
        self.in_ = QuantumNumberV8()
        self.out = QuantumNumberV8()

        # Setup pointers (like your assembly example)
        self.root.left = self.left
        self.root.right = self.right
        self.root.up = self.up
        self.root.down = self.down
        self.root.in_ = self.in_
        self.root.out = self.out

        # Initialize some example values
        self.root.a = 10
        self.root.b = 2
        self.root.c = 0  # symbolic zero allowed

        self.left.a = 1
        self.left.b = 1
        self.left.c = 1

        self.right.a = 3
        self.right.b = 6
        self.right.c = 9

    def print_structure(self):
        print("Root node:")
        print(self.root)
        print("\nLeft node:")
        print(self.root.left)
        print("\nRight node:")
        print(self.root.right)


if __name__ == "__main__":
    demo = QuantumNumberV8Demo()
    demo.print_structure()
