# Quantum Teleportation Simulation with x64-Compatible Complex Quantum Circuit

## 📄 Abstract

This report presents a working simulation of the **quantum teleportation protocol** using a custom `ComplexQuantumRegister` and `ComplexQuantumGate` framework. The protocol demonstrates how an unknown qubit state can be transferred between two parties using quantum entanglement and classical communication, without physically sending the qubit.

The implementation runs entirely on a standard **x64 architecture**, highlighting full numerical compatibility, stable complex number handling, and deterministic state collapse under floating-point constraints. High-fidelity transfer is verified with computed overlap between initial and final qubit states.

---

## 🧠 Background

Quantum teleportation is a foundational protocol in quantum information theory. It enables the transmission of a qubit state from one location (Alice) to another (Bob) using:

1. An entangled EPR pair (Bell state)
2. Local operations (CNOT, Hadamard)
3. Classical communication (2 bits)
4. Conditional unitaries (Pauli X/Z gates)

---

## 🚧 x64 Compatibility Highlights

This implementation achieves the following:
- Full support for **Apache Commons Math Complex** types under 64-bit JVMs.
- **Numerical precision to 1e-10**, avoiding floating-point rounding errors in amplitude normalization.
- Deterministic **measurement collapse** based on probabilistic simulation, implemented using native `double`-precision random values.
- Memory-optimized construction of full N-qubit gates via tensor products, verified up to 3-qubit systems (8-state vector) for x64.

> ✅ No dependencies on GPU, external quantum libraries, or QASM interpreters.

---

## 🧪 Protocol Implementation

### 1. Qubit Preparation

We prepare the input qubit `|ψ⟩ = |+⟩ = (|0⟩ + |1⟩)/√2` on qubit 0:

```java
ComplexQuantumNumber plusState = new ComplexQuantumNumber();
plusState.addComponent(a, Complex.ONE.divide(new Complex(Math.sqrt(2), 0)));
plusState.addComponent(b, Complex.ONE.divide(new Complex(Math.sqrt(2), 0)));
This is encoded across a 3-qubit register as:

Copy code
|+00⟩ = (|000⟩ + |100⟩)/√2
2. Entanglement (EPR Pair)
Qubits 1 and 2 are prepared in a Bell state |Φ+⟩ = (|00⟩ + |11⟩)/√2:

java
Copy code
input = hadamardOnQubit(1).apply(input);
input = cnot(control=1, target=2).apply(input);
3. Alice's Operations
Alice entangles her qubit (0) with her half of the EPR pair (qubit 1), applies Hadamard:

java
Copy code
input = cnot(0, 1).apply(input);
input = hadamardOnQubit(0).apply(input);
4. Measurement of Qubits 0 and 1
java
Copy code
ComplexMeasurementResult result = measureQubitsComplex(input, new int[] {0, 1});
int measuredBits = result.getMeasurementOutcome();
Output example:

nginx
Copy code
Measurement on qubits 0 and 1: 10
5. Conditional Correction by Bob (Qubit 2)
java
Copy code
if ((measuredBits & 1) == 1) apply Pauli-X on qubit 2;
if ((measuredBits & 2) == 2) apply Pauli-Z on qubit 2;
These gates restore Bob’s qubit into the original unknown state |ψ⟩.

📊 Output Sample
yaml
Copy code
Measurement on qubits 0 and 1: 10
Final state on qubit 2: 
(0.7071067811865475, 0.0)*a + (0.7071067811865475, 0.0)*b

State before measurement:
Amplitude 0: (0.3535533905932737, 0.0)*a
Amplitude 1: (0.353553390593273