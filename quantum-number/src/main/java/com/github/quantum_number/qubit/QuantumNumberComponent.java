package com.github.quantum_number.qubit;
import java.util.Objects;

public class QuantumNumberComponent {
    public static final QuantumNumberComponent a = new QuantumNumberComponent("a");
    public static final QuantumNumberComponent b = new QuantumNumberComponent("b");
    public static final QuantumNumberComponent c = new QuantumNumberComponent("c");
    public static final QuantumNumberComponent d = new QuantumNumberComponent("d");
    public static final QuantumNumberComponent e = new QuantumNumberComponent("e");
    public static final QuantumNumberComponent f = new QuantumNumberComponent("f");
    public static final QuantumNumberComponent g = new QuantumNumberComponent("g");

    private final String label;

    public QuantumNumberComponent(String label) {
        this.label = label;
    }

    public String getLabel() {
        return label;
    }

    // Combines two components by concatenating their labels
    public static QuantumNumberComponent combine(QuantumNumberComponent c1, QuantumNumberComponent c2) {
        return new QuantumNumberComponent(c1.label + c2.label);
    }

    @Override
    public String toString() {
        return label;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof QuantumNumberComponent)) return false;
        QuantumNumberComponent that = (QuantumNumberComponent) o;
        return Objects.equals(label, that.label);
    }

    @Override
    public int hashCode() {
        return Objects.hash(label);
    }

	public String name() {
		return label;
	}
}
