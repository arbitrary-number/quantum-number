# Secp256k1ECCV2 - Clean Elliptic Curve Implementation

## Overview

This document describes the new clean implementation of secp256k1 elliptic curve operations in `Secp256k1ECCV2.java` and its corresponding JUnit 5 test suite in `Secp256k1ECCV2Test.java`.

## Implementation Goals

- **Clean Architecture**: New implementation without legacy dependencies
- **Mathematical Accuracy**: Precise secp256k1 curve operations
- **UTF-8 Support**: Proper encoding for mathematical symbols in logging
- **Comprehensive Testing**: JUnit 5 tests demonstrating G, 2G, and 3G coordinates
- **Educational Value**: Clear demonstration of elliptic curve point arithmetic

## Key Features

### Secp256k1ECCV2.java

#### Core Components

1. **Curve Parameters**
   - Prime modulus P: `0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F`
   - Curve coefficients A=0, B=7
   - Generator point G coordinates
   - Order N of the curve

2. **Point Class**
   - Immutable point representation with x, y coordinates
   - Support for point at infinity
   - Hexadecimal string representation for coordinates

3. **Elliptic Curve Operations**
   - Point addition (including point doubling)
   - Scalar multiplication using double-and-add algorithm
   - Curve validation (point-on-curve check)

4. **Coordinate Calculations**
   - `calculate2G()`: Computes G + G (point doubling)
   - `calculate3G()`: Computes 2G + G (point addition)
   - Verification against scalar multiplication

#### Mathematical Operations

```java
// Point Addition Formula
// If P ≠ Q: (x3, y3) = ((y2-y1)/(x2-x1))^2 - x1 - x2, (y2-y1)/(x2-x1) * (x1 - x3) - y1
// If P = Q: (x3, y3) = ((3x1^2 + a)/(2y1))^2 - 2x1, ((3x1^2 + a)/(2y1)) * (x1 - x3) - y1
```

### Secp256k1ECCV2Test.java

#### Test Coverage

1. **Generator Point Validation**
   - Verifies G is on the secp256k1 curve
   - Confirms G is not the point at infinity

2. **2G Coordinate Calculation**
   - Tests point doubling: 2G = G + G
   - Validates result is on curve
   - Verifies consistency with scalar multiplication

3. **3G Coordinate Calculation**
   - Tests point addition: 3G = 2G + G
   - Validates result is on curve
   - Verifies consistency with scalar multiplication

4. **Arithmetic Consistency**
   - Confirms 3G = G + G + G
   - Validates point addition vs scalar multiplication equivalence

5. **Coordinate Progression**
   - Demonstrates distinct coordinates for G, 2G, 3G
   - Shows proper elliptic curve behavior

#### UTF-8 Logging Features

- Mathematical symbols: ∈ (element of), ✓ (check mark), ✗ (cross mark)
- Subscript notation: G₁, G₂, G₃
- Unicode symbols for visual clarity
- Proper console encoding configuration

## Usage Examples

### Basic Coordinate Demonstration

```java
// Calculate and display coordinates
Secp256k1ECCV2.Point g = Secp256k1ECCV2.G;
Secp256k1ECCV2.Point g2 = Secp256k1ECCV2.calculate2G();
Secp256k1ECCV2.Point g3 = Secp256k1ECCV2.calculate3G();

// Output:
// G = (0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,
//      0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8)
// 2G = (0xC6047F9441ED7D6D3045406E95C07CD85C778E4B8CEF3CA7ABAC09B95C709EE5,
//       0x1AE168FEA63DC339A3C58419466CEAEEF7F632653266D0E1236431A950CFE52A)
// 3G = (0xF9308A019258C31049344F85F89D5229B531C845836F99B08601F113BCE036F9,
//       0x388F7B0F632DE8140FE337E62A37F3566500A99934C2231B6CB9FD7584B8E672)
```

### Running Tests

```bash
# Run specific test class
mvn test -Dtest=Secp256k1ECCV2Test

# Run with verbose output
mvn test -Dtest=Secp256k1ECCV2Test -DforkCount=1 -DreuseForks=false
```

## Technical Details

### Curve Parameters (secp256k1)

- **Field**: Prime field ℤ/pℤ where p = 2^256 - 2^32 - 977
- **Equation**: y² = x³ + 7 (mod p)
- **Order**: N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
- **Cofactor**: 1

### Implementation Notes

1. **BigInteger Usage**: All calculations use Java's BigInteger for arbitrary precision arithmetic
2. **Modular Arithmetic**: All operations performed modulo P for field elements
3. **Point at Infinity**: Properly handled in addition formulas
4. **Immutable Points**: Point objects are immutable for thread safety

### Security Considerations

- Implementation follows standard elliptic curve arithmetic
- No timing attacks addressed (educational implementation)
- Coordinates displayed in hexadecimal for verification
- Point validation ensures all results are on the curve

## Verification

The implementation has been verified to produce correct secp256k1 coordinates:

- G coordinates match the secp256k1 standard
- 2G and 3G calculations verified against scalar multiplication
- All points confirmed to lie on the elliptic curve
- UTF-8 encoding properly configured for mathematical symbols

## Future Enhancements

- ECDSA signature generation/verification
- Batch point operations
- Montgomery ladder for constant-time scalar multiplication
- Additional curve parameter support

## Dependencies

- Java 17+
- JUnit 5 (for testing)
- Standard Java BigInteger library

## Files Created

1. `quantum-number/src/main/java/com/github/quantum_number/v8/Secp256k1ECCV2.java`
2. `quantum-number/src/test/java/com/github/quantum_number/v8/Secp256k1ECCV2Test.java`
3. `SECP256K1_ECC_V2_IMPLEMENTATION.md` (this documentation)

This implementation provides a clean, educational demonstration of secp256k1 elliptic curve operations with proper coordinate calculations for G, 2G, and 3G.
