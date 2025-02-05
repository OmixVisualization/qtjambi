/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.function.Supplier;

import io.qt.*;

/**
 * <p><b>Warning:</b> This utility class is only functional when running with the legacy OpenGL renderer of the Qt Quick scenegraph. Its usage is not recommended in new application code.</p>
 */
public class QSGSimpleMaterialComparableMaterial<State extends Object & Comparable<State>> extends QSGSimpleMaterial<State> {

	public QSGSimpleMaterialComparableMaterial(@StrictNonNull State aState, @StrictNonNull Supplier<@Nullable QSGMaterialShader> func) {
		super(aState, func);
	}

	public QSGSimpleMaterialComparableMaterial(@Nullable Class<State> type, @StrictNonNull Supplier<@Nullable QSGMaterialShader> func) {
		super(type, func);
	}

	@Override
	public int compare(@Nullable QSGMaterial other) {
		if(other instanceof QSGSimpleMaterialComparableMaterial) {
			QSGSimpleMaterialComparableMaterial<?> _other = (QSGSimpleMaterialComparableMaterial<?>)other;
			if(_other.m_type==this.m_type) {
				return state().compareTo(this.m_type.cast(_other.state()));
			}
		}
		return -1;
    }
}
