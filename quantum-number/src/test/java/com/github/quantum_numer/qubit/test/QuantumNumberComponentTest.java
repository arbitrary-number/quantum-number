package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.QuantumNumberComponent;

public class QuantumNumberComponentTest {

	@Test
	public void testQuantumNumberComponentCombine() {
	    QuantumNumberComponent c1 = QuantumNumberComponent.a;
	    QuantumNumberComponent c2 = QuantumNumberComponent.a;

	    QuantumNumberComponent combined = QuantumNumberComponent.combine(c1, c2);
	    assertNotNull(combined);
	    assertTrue(combined.name().contains("a") && combined.name().contains("a"));
	}


}
