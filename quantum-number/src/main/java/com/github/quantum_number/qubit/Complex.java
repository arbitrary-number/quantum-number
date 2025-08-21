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

public final class Complex {
    private final double re;
    private final double im;

    public static final Complex ZERO = new Complex(0, 0);
    public static final Complex ONE = new Complex(1, 0);

    public Complex(double re, double im) {
        this.re = re;
        this.im = im;
    }

    public double real() {
        return re;
    }

    public double imag() {
        return im;
    }

    public Complex add(Complex other) {
        return new Complex(re + other.re, im + other.im);
    }

    public Complex subtract(Complex other) {
        return new Complex(re - other.re, im - other.im);
    }

    public Complex multiply(Complex other) {
        return new Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }

    public Complex multiply(double scalar) {
        return new Complex(re * scalar, im * scalar);
    }

    public Complex divide(Complex other) {
        double denom = other.re * other.re + other.im * other.im;
        return new Complex(
            (re * other.re + im * other.im) / denom,
            (im * other.re - re * other.im) / denom
        );
    }

    public Complex conjugate() {
        return new Complex(re, -im);
    }

    public double magnitude() {
        return Math.sqrt(re * re + im * im);
    }

    public Complex normalize() {
        double mag = magnitude();
        if (mag == 0) return ZERO;
        return new Complex(re / mag, im / mag);
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Complex)) return false;
        Complex c = (Complex) obj;
        final double EPS = 1e-10;
        return Math.abs(re - c.re) < EPS && Math.abs(im - c.im) < EPS;
    }

    @Override
    public int hashCode() {
        return Double.hashCode(re) ^ Double.hashCode(im);
    }

    @Override
    public String toString() {
        return String.format("(%f%+fi)", re, im);
    }
}
