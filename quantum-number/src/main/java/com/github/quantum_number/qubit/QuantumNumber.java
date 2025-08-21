/*
 * Copyright (c) Arbitrary Number Project Team. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
package com.github.quantum_number.qubit;

import java.util.Random;

public class QuantumNumber {
    // Complex nested structure (real and imaginary parts)
    public double a, b, c, d, e, f;
    public double g, h, i, j, k, l;

    public QuantumNumber(double a, double g, double b, double h, double c, double i,
                         double d, double j, double e, double k, double f, double l) {
        this.a = a; this.g = g;
        this.b = b; this.h = h;
        this.c = c; this.i = i;
        this.d = d; this.j = j;
        this.e = e; this.k = k;
        this.f = f; this.l = l;
    }

    // Copy constructor
    public QuantumNumber(QuantumNumber other) {
        this(other.a, other.g, other.b, other.h, other.c, other.i,
             other.d, other.j, other.e, other.k, other.f, other.l);
    }

    // Quantition addition (ψ' = ψ + T)
    public QuantumNumber add(QuantumNumber t) {
        return new QuantumNumber(
            this.a + t.a, this.g + t.g,
            this.b + t.b, this.h + t.h,
            this.c + t.c, this.i + t.i,
            this.d + t.d, this.j + t.j,
            this.e + t.e, this.k + t.k,
            this.f + t.f, this.l + t.l
        );
    }

    // Quantition inverse (for subtraction)
    public QuantumNumber inverse() {
        return new QuantumNumber(
            -a, -g, -b, -h, -c, -i,
            -d, -j, -e, -k, -f, -l
        );
    }

    public QuantumNumber subtract(QuantumNumber t) {
        return this.add(t.inverse());
    }

    // Simulated measurement (basic magnitude heuristic)
    public double measureProbability() {
        double mag0 = a * a + g * g;
        double mag1 = d * d + j * j;
        return mag0 / (mag0 + mag1 + 1e-12);  // Avoid div by zero
    }

    public int measure() {
        double p0 = measureProbability();
        return new Random().nextDouble() < p0 ? 0 : 1;
    }

    public QuantumNumber normalize() {
        // Sum of squares of all 12 components (real and imaginary parts)
        double sumSquares =
            a * a + g * g +
            b * b + h * h +
            c * c + i * i +
            d * d + j * j +
            e * e + k * k +
            f * f + l * l;

        double norm = Math.sqrt(sumSquares);

        if (norm == 0) return this; // Avoid divide by zero

        return new QuantumNumber(
            a / norm, g / norm,
            b / norm, h / norm,
            c / norm, i / norm,
            d / norm, j / norm,
            e / norm, k / norm,
            f / norm, l / norm
        );
    }


    @Override
    public String toString() {
        return String.format(
            "((%.2f + %.2fi) / ((%.2f + %.2fi) / (%.2f + %.2fi))) ÷ ((%.2f + %.2fi) / ((%.2f + %.2fi) / (%.2f + %.2fi)))",
            a, g, b, h, c, i, d, j, e, k, f, l
        );
    }
}
