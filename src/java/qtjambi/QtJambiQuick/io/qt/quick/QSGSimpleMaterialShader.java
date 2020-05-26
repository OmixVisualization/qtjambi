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
