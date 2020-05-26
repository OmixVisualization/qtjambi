package io.qt.quick;

import java.util.function.Supplier;

public class QSGSimpleMaterialComparableMaterial<State extends Object & Comparable<State>> extends QSGSimpleMaterial<State> {

	public QSGSimpleMaterialComparableMaterial(State aState, Supplier<QSGMaterialShader> func) {
		super(aState, func);
	}

	public QSGSimpleMaterialComparableMaterial(Class<State> type, Supplier<QSGMaterialShader> func) {
		super(type, func);
	}

	@Override
	public int compare(QSGMaterial other) {
		if(other instanceof QSGSimpleMaterialComparableMaterial) {
			QSGSimpleMaterialComparableMaterial<?> _other = (QSGSimpleMaterialComparableMaterial<?>)other;
			if(_other.m_type==this.m_type) {
				return state().compareTo(this.m_type.cast(_other.state()));
			}
		}
		return -1;
    }
}
