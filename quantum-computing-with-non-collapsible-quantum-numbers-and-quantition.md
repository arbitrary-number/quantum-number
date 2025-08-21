<!--
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright (c) Arbitrary Number Project Team. All rights reserved.
-->
# Quantum Computing with Non-Collapsible Numbers and Quantition

**Author:** Arbitrary Number Project Team
**Date:** 2025-08-22  
**Status:** Draft  

---

## Abstract

We propose a novel algebraic framework for quantum computing in which quantum states and quantum gates are represented by *non-collapsible nested fractional numbers* extended to allow division by zero as a valid operation. In this framework, *quantition*—an additive transformation defined on these non-collapsible numbers—unifies the representation of quantum states and operations, allowing numbers to act as quantum gates and vice versa. This approach potentially encodes inherent error correction and reversibility via algebraic constraints, offering a fresh perspective on fault-tolerant quantum computation.

---

## Introduction

Quantum computing traditionally models qubits as vectors in complex Hilbert spaces, and quantum gates as unitary operators acting on these vectors. Error correction is typically layered on top using stabilizer codes or topological methods. Here, we explore a fundamentally different algebraic model based on:

- **Non-collapsible nested fractional numbers** with components allowing division by zero in a controlled and consistent manner.
- An operation called **quantition**, which acts as an additive transformation within this algebra.
- A duality where **quantum states and quantum gates coexist as algebraic elements** and can transform into each other via quantition.

---

## Algebraic Framework

### Non-Collapsible Numbers

Quantum states are encoded as complex nested fractions of the form:

\[
\frac{(\pm a \pm gi)}{\left(\frac{\pm b \pm hi}{\pm c \pm ii}\right)} \div \frac{\left(\frac{\pm d \pm ji}{\pm e \pm ki}\right)}{\pm f \pm li}
\]

where \(a,b,c,d,e,f,g,h,i,j,k,l \in \mathbb{R}\) or \(\mathbb{C}\), and division by zero is defined as a valid, non-collapsible entity in this algebra. These numbers represent ordinals from \(1\) to \(2^{256}\), providing a rich, high-dimensional encoding space.

### Quantition: Additive Transformations

A **quantum transformation** (gate) \(T_2\) acts on a source state \(T_1\) to produce a target state \(T_3\) via:

\[
T_3 = T_1 + T_2
\]

The inverse transformation is given by:

\[
T_1 = T_3 - T_2
\]

where addition and subtraction are defined component-wise on the nested fractional components within the non-collapsible algebra.

### States and Operations Duality

Because \(T_2\) itself is a non-collapsible number, **quantum gates and quantum states inhabit the same algebraic space**. Through quantition, numbers can become operations and operations can be interpreted as numbers, enabling a self-referential and highly unified computational model.

---

## Properties and Implications

- **Invertibility:** All transformations are invertible by applying the opposite sign to \(T_2\).
- **Error Correction Potential:** Non-collapsibility serves as an invariant, potentially allowing detection and correction of errors that would otherwise collapse the algebraic structure.
- **Fault Tolerance:** The algebraic constraints could encode fault tolerance inherently, avoiding complex error correction overhead.
- **Universality:** Composition of quantition operations could generate universal quantum computation primitives (subject to further investigation).
- **Measurement:** Collapse corresponds to a breakdown of non-collapsibility, providing an algebraic signature of measurement or decoherence.

---

## Open Questions and Future Work

- Formalizing the algebraic rules for addition, subtraction, and division by zero in this context.
- Exploring the exact mapping between these algebraic states and physical qubit implementations.
- Defining measurement operators and procedures within the algebra.
- Investigating universality and computational complexity.
- Simulating simple quantum circuits within this algebraic model.

---

## Conclusion

This report outlines an ambitious, novel framework for quantum computation leveraging non-collapsible numbers and quantition. By merging quantum states and operations into a single algebraic structure that supports division by zero, this model opens intriguing avenues for fault-tolerant and universal quantum computing beyond conventional Hilbert space methods.

---

*End of report.*
