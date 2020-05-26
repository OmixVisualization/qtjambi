package io.qt.quick;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.function.Supplier;

public class QSGSimpleMaterial<State> extends QSGMaterial {
	@SuppressWarnings("unchecked")
	public QSGSimpleMaterial(State aState, Supplier<QSGMaterialShader> func) {
		m_type = (Class<State>)aState.getClass();
		m_state = aState;
		m_func = Objects.requireNonNull(func);
    }

	public QSGSimpleMaterial(Class<State> type, Supplier<QSGMaterialShader> func) {
		m_type = type;
		m_state = null;
		m_func = Objects.requireNonNull(func);
    }

	@Override
	public QSGMaterialShader createShader() { return m_func.get(); }

	@Override
	public QSGMaterialType type() { return m_types.computeIfAbsent(m_type, cls->new QSGMaterialType()); }

	public final State state() { return m_state; }

	final Class<State> m_type;
	private final State m_state;
	private final Supplier<QSGMaterialShader> m_func;
	private final static Map<Class<?>, QSGMaterialType> m_types = new HashMap<>();
}
