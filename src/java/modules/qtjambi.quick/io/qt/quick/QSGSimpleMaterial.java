/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

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
