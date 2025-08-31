from QuantumNumberV8 import QuantumNumberV8

# Assuming QuantumNumberV8 class is defined as you provided above

def sum_all_coefficients(qnum):
    # Sum all coefficients of qnum and all connected pointers
    nodes = [qnum, qnum.left, qnum.right, qnum.up, qnum.down, qnum.in_, qnum.out]
    total = 0
    for node in nodes:
        if node:
            total += node.a + node.b + node.c + node.d + node.e + node.f
    return total


# Create main node
main_node = QuantumNumberV8()
main_node.a = 1_000_000_000_000  # large integer part for demo
main_node.b = 2
main_node.c = 3
main_node.d = 4
main_node.e = 5
main_node.f = 6

# Create linked nodes with various coefficients
main_node.left = QuantumNumberV8()
main_node.left.a = 10
main_node.left.b = 20

main_node.right = QuantumNumberV8()
main_node.right.c = 30
main_node.right.d = 40

main_node.up = QuantumNumberV8()
main_node.up.e = 50

main_node.down = QuantumNumberV8()
main_node.down.f = 60

main_node.in_ = QuantumNumberV8()
main_node.in_.a = 7
main_node.in_.b = 8
main_node.in_.c = 9

main_node.out = QuantumNumberV8()
main_node.out.d = 11
main_node.out.e = 12
main_node.out.f = 13

# Print main node and neighbors
print("Main Node:")
print(main_node)
print("\nNeighbors:")
print(f"Left:  {main_node.left}")
print(f"Right: {main_node.right}")
print(f"Up:    {main_node.up}")
print(f"Down:  {main_node.down}")
print(f"In:    {main_node.in_}")
print(f"Out:   {main_node.out}")

# Calculate and print total sum of coefficients
total_sum = sum_all_coefficients(main_node)
print(f"\nTotal sum of coefficients (a,b,c,d,e,f) over all nodes: {total_sum}")
