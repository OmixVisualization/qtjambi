package io.qt.gui.vulkan;

import io.qt.*;


/**
 * <p>A specialized QList for QVulkanLayer and QVulkanExtension</p>
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qvulkaninfovector.html">QVulkanInfoVector&lt;T&gt;</a></code></p>
 */
public class QVulkanInfoVector<T> extends io.qt.core.QList<T>
{

	static {
        QtJambi_LibraryUtilities.initialize();
    }
	
    @SafeVarargs
	private QVulkanInfoVector(@Nullable Class<T> elementType, T @StrictNonNull... elements) {
		super(elementType, elements);
	}

	private QVulkanInfoVector(@Nullable Class<T> elementType) {
		super(elementType);
	}
	
	public static QVulkanInfoVector<QVulkanLayer> of(QVulkanLayer... layers){
		return new QVulkanInfoVector<>(QVulkanLayer.class, layers);
	}
	
	public static QVulkanInfoVector<QVulkanExtension> of(QVulkanExtension... extensions){
		return new QVulkanInfoVector<>(QVulkanExtension.class, extensions);
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qvulkaninfovector.html#contains">QVulkanInfoVector&lt;T&gt;::<wbr/>contains(QByteArray)const</a></code></p>
     * @param name
     * @return
     */
    @QtUninvokable
    public final boolean contains(io.qt.core.@NonNull QByteArray name){
    	Class<?> type = this.elementMetaType().javaType();
    	if(type==QVulkanLayer.class) {
	    	for(T element : this) {
	    		if(((QVulkanLayer)element).name().equals(name))
	    			return true;
	    	}
    	}
    	if(type==QVulkanExtension.class) {
	    	for(T element : this) {
	    		if(((QVulkanExtension)element).name().equals(name))
	    			return true;
	    	}
    	}
        return false;
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qvulkaninfovector.html#contains-1">QVulkanInfoVector&lt;T&gt;::<wbr/>contains(QByteArray,<wbr/>int)const</a></code></p>
     * @param name
     * @param minVersion
     * @return
     */
    @QtUninvokable
    public final boolean contains(io.qt.core.@NonNull QByteArray name, int minVersion){
    	Class<?> type = this.elementMetaType().javaType();
    	if(type==QVulkanLayer.class) {
	    	for(T element : this) {
	    		if(((QVulkanLayer)element).version() >= minVersion && ((QVulkanLayer)element).name().equals(name))
	    			return true;
	    	}
    	}
    	if(type==QVulkanExtension.class) {
	    	for(T element : this) {
	    		if(((QVulkanExtension)element).version() >= minVersion && ((QVulkanExtension)element).name().equals(name))
	    			return true;
	    	}
    	}
        return false;
    }
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
    @NativeAccess
    protected QVulkanInfoVector(QPrivateConstructor p) { super(p); } 
    
    
    /**
     * <p>Overloaded function for {@link #contains(io.qt.core.QByteArray)}.</p>
     */
    @QtUninvokable
    public final boolean contains(byte @NonNull[] name) {
        return contains(new io.qt.core.QByteArray(name));
    }
    
    /**
     * <p>Overloaded function for {@link #contains(io.qt.core.QByteArray, int)}.</p>
     */
    @QtUninvokable
    public final boolean contains(byte @NonNull[] name, int minVersion) {
        return contains(new io.qt.core.QByteArray(name), minVersion);
    }
}
