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

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

public abstract class QSGSimpleMaterialShader<State> extends QSGMaterialShader {
	
	public QSGSimpleMaterialShader(Class<State> type) {
		super();
		m_type = type;
	}

	@Override
	public void initialize() {
        super.initialize();
        if(isOpenGL()) {
	        m_id_matrix = program().uniformLocation(uniformMatrixName());
	        if (m_id_matrix < 0) {
	        	System.err.println(String.format("QSGSimpleMaterialShader does not implement 'uniform highp mat4 %1$s;' in its vertex shader", uniformMatrixName()));
	        }
	
	        String opacity = uniformOpacityName();
	        if (opacity!=null) {
	            m_id_opacity = program().uniformLocation(uniformOpacityName());
	            if (m_id_opacity < 0) {
	            	System.err.println(String.format("QSGSimpleMaterialShader does not implement 'uniform lowp float %1$s' in its fragment shader", uniformOpacityName()));
	            }
	        } else {
	            m_id_opacity = -1;
	        }
        }
        resolveUniforms();
    }

    public final String uniformMatrixName() { return "qt_Matrix"; }
    public final String uniformOpacityName() { return "qt_Opacity"; }

    @Override
    public void updateState(RenderState state, QSGMaterial newMaterial, QSGMaterial oldMaterial) {
    	if(isOpenGL()) {
	        if (state.isMatrixDirty())
	            program().setUniformValue(m_id_matrix, state.combinedMatrix());
	        if (state.isOpacityDirty() && m_id_opacity >= 0)
	            program().setUniformValue(m_id_opacity, state.opacity());
    	}
        QSGSimpleMaterial<?> newSimpleMaterial = (QSGSimpleMaterial<?>)newMaterial;
        if(newSimpleMaterial.m_type!=m_type) {
        	return;
        }
        State ns = m_type.cast(newSimpleMaterial.state());
        State old = null;
        if (oldMaterial!=null) {
            QSGSimpleMaterial<?> oldSimpleMaterial = (QSGSimpleMaterial<?>)oldMaterial;
            if(oldSimpleMaterial.m_type!=m_type) {
            	return;
            }
            old = m_type.cast(oldSimpleMaterial.state());
        }
        updateState(ns, old);
    }
    

    public abstract void updateState(State newState, State oldState);

    public void resolveUniforms() {}

    public abstract List<String> attributes();

    @Override
    public String[] attributeNames()
    {
    	List<String> attributes = attributes();
        return attributes.toArray(new String[attributes.size()]);
    }

    private final Class<State> m_type;
    private int m_id_matrix;
    private int m_id_opacity;

    private ByteBuffer m_attribute_name_data;
    private ArrayList<String> m_attribute_pointers = new ArrayList<>();
    
    private static native boolean isOpenGL();
}
