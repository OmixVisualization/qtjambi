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
package io.qt.internal;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectStreamConstants;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Parameter;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Deque;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.NavigableMap;
import java.util.Queue;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QtMetaType;
import io.qt.QtObjectInterface;
import io.qt.QtPointerType;
import io.qt.QtReferenceType;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QDataStream;
import io.qt.core.QDebug;
import io.qt.core.QHash;
import io.qt.core.QIODevice;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QQueue;
import io.qt.core.QSet;
import io.qt.core.QStack;
import io.qt.core.QStringList;
import io.qt.core.QVariant;
import io.qt.core.Qt;

/**
 * @hidden
 */
public final class MetaTypeUtility {
	static {
		QtJambi_LibraryUtilities.initialize();
	}

	
	private MetaTypeUtility() {
		throw new RuntimeException();
	}
	
	private static final class ReadWriteHandles{
		private ReadWriteHandles(BiConsumer<Object,QDataStream> writeTo, BiConsumer<Object,QDataStream> readFrom, Supplier<?> constructor) {
			super();
			this.writeTo = writeTo;
			this.readFrom = readFrom;
			this.constructor = constructor;
		}
		final BiConsumer<Object,QDataStream> writeTo;
		final BiConsumer<Object,QDataStream> readFrom;
		final Supplier<?> constructor;
	}
	private static final class DataStreamFunctions{
		@SuppressWarnings("unchecked")
		private DataStreamFunctions(BiConsumer<QDataStream, ?> datastreamInFn,
				Function<QDataStream, ?> datastreamOutFn) {
			super();
			this.datastreamInFn = (java.util.function.BiConsumer<QDataStream, Object>)datastreamInFn;
			this.datastreamOutFn = (java.util.function.Function<QDataStream, Object>)datastreamOutFn;
		}
		final java.util.function.BiConsumer<QDataStream, Object> datastreamInFn;
		final java.util.function.Function<QDataStream, Object> datastreamOutFn;
	}
	private static final Map<Class<?>, ReadWriteHandles> readWriteHandles;
	private static final Map<Class<?>, DataStreamFunctions> dataStreamFunctions;
	private static final Map<Class<?>, java.util.function.BiConsumer<QDebug, Object>> debugStreamFunctions;
	static {
		dataStreamFunctions = Collections.synchronizedMap(new HashMap<>());
		debugStreamFunctions = Collections.synchronizedMap(new HashMap<>());
		readWriteHandles = Collections.synchronizedMap(new HashMap<>());
	}
	
	public static <T> void registerDataStreamOperators(int metaType, Class<?> classType, java.util.function.BiConsumer<QDataStream, T> datastreamInFn, java.util.function.Function<QDataStream, T> datastreamOutFn) {
		boolean isCustomValueType = isCustomValueType(metaType);
		if(!isObjectWrapperType(metaType)) {
			if(isCustomValueType) {
				if(metaTypeHasDataStreamOperators(metaType)) {
					throw new RuntimeException(String.format("Datastream operators already exist for type %1$s.", classType.getTypeName()));
				}
			}else if(classType.isArray())
				throw new IllegalArgumentException(String.format("Unable to register datastream operators for type %1$s.", classType.getTypeName()));
			else
				throw new IllegalArgumentException(String.format("Unable to register datastream operators for type %1$s (%2$s).", classType.getTypeName(), new QMetaType(metaType).name()));
		}
		DataStreamFunctions functions = dataStreamFunctions.computeIfAbsent(classType, cls->new DataStreamFunctions(datastreamInFn, datastreamOutFn));
		if(functions.datastreamInFn!=datastreamInFn || functions.datastreamOutFn!=datastreamOutFn)
			throw new RuntimeException(String.format("Datastream operators already exist for type %1$s.", classType.getTypeName()));
		if(isCustomValueType)
			registerCustomDataStreamOperators(metaType);
	}
	
	@SuppressWarnings("unchecked")
	public static <T> void registerDebugStreamOperator(int metaType, Class<?> classType, java.util.function.BiConsumer<QDebug, T> debugstreamFn) {
		boolean isCustomValueType = isCustomValueType(metaType);
		if(!isObjectWrapperType(metaType)) {
			if(isCustomValueType) {
				if(metaTypeHasDebugStreamOperator(metaType)) {
					throw new RuntimeException(String.format("Debug stream operator already exists for type %1$s.", classType.getTypeName()));
				}
			}else if(classType.isArray())
				throw new IllegalArgumentException(String.format("Unable to register debug stream operator for type %1$s.", classType.getTypeName()));
			else
				throw new IllegalArgumentException(String.format("Unable to register debug stream operator for type %1$s (%2$s).", classType.getTypeName(), classType.getTypeName()));
		}
		java.util.function.BiConsumer<QDebug,Object> fun = debugStreamFunctions.computeIfAbsent(classType, c->(java.util.function.BiConsumer<QDebug,Object>)debugstreamFn);
		if(fun!=debugstreamFn)
			throw new RuntimeException(String.format("Debug stream operator already exists for type %1$s.", classType.getTypeName()));
		if(isCustomValueType)
			registerCustomDebugStreamOperator(metaType);
	}
	
	public static native boolean registerConverter(int metaType1, Class<?> classType1, int metaType2, Class<?> classType2, java.util.function.Function<?,?> converterFn);
	
	private static native boolean isObjectWrapperType(int metaType);
	
	//Qt6 only!
	private static native boolean isCustomValueType(int metaType);
	private static native boolean metaTypeHasDataStreamOperators(int metaType);
	private static native boolean metaTypeHasDebugStreamOperator(int metaType);
	private static native void registerCustomDataStreamOperators(int metaType);
	private static native void registerCustomDebugStreamOperator(int metaType);
	
	private static final int RWH_MAGIC = 0x01010101;
	private static final int NULL_MAGIC = 0x02020202;
	private static final int DSF_MAGIC = 0x03030303;
	private static final int BYTE_ARRAY_MAGIC = 0x04040400;
	private static final int SHORT_ARRAY_MAGIC = 0x04040401;
	private static final int INT_ARRAY_MAGIC = 0x04040402;
	private static final int LONG_ARRAY_MAGIC = 0x04040403;
	private static final int FLOAT_ARRAY_MAGIC = 0x04040404;
	private static final int DOUBLE_ARRAY_MAGIC = 0x04040405;
	private static final int BOOLEAN_ARRAY_MAGIC = 0x04040406;
	private static final int CHAR_ARRAY_MAGIC = 0x04040407;
	private static final int OBJECT_ARRAY_MAGIC = 0x04040408;
	private static final int OBJECT_ARRAY_MAGIC_V2 = 0x04040409;

	private static ReadWriteHandles getReadWriteHandles(Class<?> _cls) {
		return readWriteHandles.computeIfAbsent(_cls, cls ->{
			try {
				Constructor<?> constructor = cls.getDeclaredConstructor();
				Method writeTo;
				try {
					writeTo = cls.getMethod("writeTo", QDataStream.class);
				} catch (Throwable e) {
					writeTo = cls.getDeclaredMethod("writeTo", QDataStream.class);
				}
				Method readFrom;
				try {
					readFrom = cls.getMethod("readFrom", QDataStream.class);
				} catch (Throwable e) {
					readFrom = cls.getDeclaredMethod("readFrom", QDataStream.class);
				}
				if(!Modifier.isStatic(writeTo.getModifiers()) && !Modifier.isStatic(readFrom.getModifiers())) {
					return new ReadWriteHandles(ReflectionUtility.methodInvocationHandler.streamIO(writeTo), 
							ReflectionUtility.methodInvocationHandler.streamIO(readFrom), 
							ReflectionUtility.methodInvocationHandler.getFactory0(constructor));
				}
			} catch (Throwable e) {
			}
			return null;
		});
	}

	@SuppressWarnings("deprecation")
	@NativeAccess
	private static Object readSerializableJavaObject(final QDataStream s) throws ClassNotFoundException, IOException {
		Object res = null;
		boolean isSerialized = true;
		s.startTransaction();
		{
			int ch1 = s.readByte();
	        int ch2 = s.readByte();
			int ch3 = s.readByte();
	        int ch4 = s.readByte();
	        short magic = (short)((ch1 << 8) + (ch2 & 0xFF));
	        short version = (short)((ch3 << 8) + (ch4 & 0xFF));
			if(magic!=ObjectStreamConstants.STREAM_MAGIC
					|| version!=ObjectStreamConstants.STREAM_VERSION) {
				isSerialized = false;
			}
		}
		s.rollbackTransaction();
		
		if(isSerialized) {
			if(s.device()!=null && s.device().isReadable()) {
				try (ObjectInputStream in = new ObjectInputStream(QIODevice.toInputStream(s.device()))) {
					res = in.readObject();
				}
			}else {
				try (ObjectInputStream in = new ObjectInputStream(new InputStream() {
					@Override
					public int read() throws IOException {
						return s.readByte();
					}
				})) {
					res = in.readObject();
				}
			}
		}else {
			Class<?> cls;
			int length;
			switch(s.readInt()) {
			case BYTE_ARRAY_MAGIC:
				length = s.readInt();
				{
					byte[] array = new byte[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readByte();
					}
					res = array;
				}
				break;
			case SHORT_ARRAY_MAGIC:
				length = s.readInt();
				{
					short[] array = new short[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readShort();
					}
					res = array;
				}
				break;
			case INT_ARRAY_MAGIC:
				length = s.readInt();
				{
					int[] array = new int[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readInt();
					}
					res = array;
				}
				break;
			case LONG_ARRAY_MAGIC:
				length = s.readInt();
				{
					long[] array = new long[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readLong();
					}
					res = array;
				}
				break;
			case FLOAT_ARRAY_MAGIC:
				length = s.readInt();
				{
					float[] array = new float[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readFloat();
					}
					res = array;
				}
				break;
			case DOUBLE_ARRAY_MAGIC:
				length = s.readInt();
				{
					double[] array = new double[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readDouble();
					}
					res = array;
				}
				break;
			case BOOLEAN_ARRAY_MAGIC:
				length = s.readInt();
				{
					boolean[] array = new boolean[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readBoolean();
					}
					res = array;
				}
				break;
			case CHAR_ARRAY_MAGIC:
				length = s.readInt();
				{
					char[] array = new char[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readChar();
					}
					res = array;
				}
				break;
			case OBJECT_ARRAY_MAGIC:
				length = s.readInt();
				{
					cls = Class.forName(s.readString());
					res = Array.newInstance(cls, length);
					for (int i = 0; i < length; i++) {
						Array.set(res, i, QVariant.loadObject(s));
					}
				}
				break;
			case OBJECT_ARRAY_MAGIC_V2:
				length = s.readInt();
				{
					cls = Class.forName(s.readString());
					res = Array.newInstance(cls, length);
					for (int i = 0; i < length; i++) {
						QVariant variant = new QVariant();
						variant.load(s);
						Array.set(res, i, variant.value());
						variant.dispose();
					}
				}
				break;
			case RWH_MAGIC:
				cls = Class.forName(s.readString());
				ReadWriteHandles readWriteHandles = getReadWriteHandles(cls);
				if(readWriteHandles!=null) {
					try {
						res = readWriteHandles.constructor.get();
						readWriteHandles.readFrom.accept(res, s);
					} catch (Error | Exception e) {
						throw e;
					} catch (Throwable e) {
						throw new IOException(e);
					}
				}
				break;
			case DSF_MAGIC:
				cls = Class.forName(s.readString());
				DataStreamFunctions functions = dataStreamFunctions.get(cls);
				if(functions!=null) {
					try {
						res = functions.datastreamOutFn.apply(s);
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (Throwable e) {
						throw new IOException(e);
					}
				}
				break;
			case NULL_MAGIC:
				break;
			default:
				break;
			}
		}
		return res;
	}
	
	@NativeAccess
	private static void writeSerializableJavaObject(QDataStream s, Object o) throws IOException {
		if(o==null) {
			s.writeInt(NULL_MAGIC);
			return;
		}
		Class<?> objectClass = ClassAnalyzerUtility.getClass(o);
		if(objectClass.isArray()) {
			if(objectClass==byte[].class) {
				byte[] array = (byte[])o;
				s.writeInt(BYTE_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==short[].class) {
				short[] array = (short[])o;
				s.writeInt(SHORT_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==int[].class) {
				int[] array = (int[])o;
				s.writeInt(INT_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==long[].class) {
				long[] array = (long[])o;
				s.writeInt(LONG_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==float[].class) {
				float[] array = (float[])o;
				s.writeInt(FLOAT_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==double[].class) {
				double[] array = (double[])o;
				s.writeInt(DOUBLE_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==boolean[].class) {
				boolean[] array = (boolean[])o;
				s.writeInt(BOOLEAN_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else if(objectClass==char[].class) {
				char[] array = (char[])o;
				s.writeInt(CHAR_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.append(array[i]);
				}
			}else {
				Object[] array = (Object[])o;
				s.writeInt(OBJECT_ARRAY_MAGIC_V2);
				s.writeString(objectClass.getComponentType().getName());
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					QVariant variant = new QVariant(array[i]);
					variant.save(s);
					variant.dispose();
				}
			}
		}else {
			DataStreamFunctions functions = dataStreamFunctions.get(objectClass);
			if(functions!=null) {
				try {
					s.writeInt(DSF_MAGIC);
					s.writeString(objectClass.getName());
					functions.datastreamInFn.accept(s, o);
				} catch (RuntimeException | Error e) {
					throw e;
				} catch (Throwable e) {
					throw new IOException(e);
				}
			}else {
				ReadWriteHandles readWriteHandles = getReadWriteHandles(objectClass);
				if(readWriteHandles!=null) {
					try {
						s.writeInt(RWH_MAGIC);
						s.writeString(objectClass.getName());
						readWriteHandles.writeTo.accept(o, s);
					} catch (Error | Exception e) {
						throw e;
					} catch (Throwable e) {
						throw new IOException(e);
					}
				}else if(s.device()!=null && s.device().isWritable()) {
					try(ObjectOutputStream out = new ObjectOutputStream(QIODevice.toOutputStream(s.device()))) {
						out.writeObject(o);
					}
				}else {
					ByteArrayOutputStream bos = new ByteArrayOutputStream();
					try(ObjectOutputStream out = new ObjectOutputStream(bos)) {
						out.writeObject(o);
					}
					s.writeRawData(bos.toByteArray());
				}
			}
		}
	}
	
	@NativeAccess
	private static void debugObject(QDebug s, Object o) {
		if(o==null) {
			s.append("null");
		}else{
			Class<?> objectClass = ClassAnalyzerUtility.getClass(o);
			if(objectClass.isArray()) {
				if(objectClass==byte[].class) {
					byte[] array = (byte[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==short[].class) {
					short[] array = (short[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==int[].class) {
					int[] array = (int[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==long[].class) {
					long[] array = (long[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==float[].class) {
					float[] array = (float[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==double[].class) {
					double[] array = (double[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==boolean[].class) {
					boolean[] array = (boolean[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==char[].class) {
					char[] array = (char[])o;
					s.append(Arrays.toString(array));
				}else {
					Object[] array = (Object[])o;
					s.append(Arrays.toString(array));
				}
			}else {
				java.util.function.BiConsumer<QDebug,Object> function = debugStreamFunctions.get(objectClass);
				if(function!=null) {
					try {
						function.accept(s, o);
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (Throwable e) {
						throw new RuntimeException(e);
					}
				}else {
					s.append(o.toString());
				}
			}
		}
	}
	

	static native int registerRefMetaType(int id, boolean isPointer, boolean isReference);

	static native int findMetaType(String name);
	
	static int registerMetaType(Parameter parameter) {
		AnnotatedElement annotatedParameterType = null;
    	if(ClassAnalyzerUtility.useAnnotatedType)
    		annotatedParameterType = parameter.getAnnotatedType();
    	return registerMetaType(parameter.getType(), parameter.getParameterizedType(), annotatedParameterType, false, false);
	}

	public static int registerMetaType(Class<?> clazz, Type genericType, AnnotatedElement annotatedType, boolean isPointer, boolean isReference) {
		QtUtilities.initializePackage(clazz);
		QtReferenceType referenceType = null;
		QtPointerType pointerType = null;
		if(annotatedType!=null) {
			referenceType = annotatedType.getAnnotation(QtReferenceType.class);
			if(referenceType!=null)
				isReference = !referenceType.isConst();
			pointerType = annotatedType.getAnnotation(QtPointerType.class);
			if(pointerType!=null && !isPointer)
				isPointer = true;
			QtMetaType metaTypeDecl = annotatedType.getAnnotation(QtMetaType.class);
			if(metaTypeDecl!=null) {
				int id;
				if(metaTypeDecl.id()!=0) {
					id = metaTypeDecl.id();
				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
					id = metaTypeDecl.type().value();
				}else {
					id = findMetaType(metaTypeDecl.name());
					if(id==0) {
						if(metaTypeDecl.name().isEmpty())
							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
						else
							throw new IllegalArgumentException("Unable to detect meta type "+metaTypeDecl.name());
					}
				}
				if(isPointer) {
					id = registerRefMetaType(id, true, false);
				}else if(isReference){
					id = registerRefMetaType(id, false, true);
				}
				return id;
			}
		}
		if (genericType instanceof ParameterizedType) {
			ParameterizedType parameterizedType = (ParameterizedType) genericType;
			Type[] actualTypeArguments = parameterizedType.getActualTypeArguments();
			AnnotatedElement[] actualAnnotatedTypeArguments = null;
			if(ClassAnalyzerUtility.useAnnotatedType) {
				if(annotatedType instanceof java.lang.reflect.AnnotatedParameterizedType) {
					actualAnnotatedTypeArguments = ((java.lang.reflect.AnnotatedParameterizedType)annotatedType).getAnnotatedActualTypeArguments();
				}
			}
			if (parameterizedType.getRawType() instanceof Class<?>) {
				if (isQQmlListProperty((Class<?>) parameterizedType.getRawType())) {
					String listPropertyName = internalTypeNameOfClass(clazz, genericType);
					int id = findMetaType(listPropertyName);
					if (id != QMetaType.Type.UnknownType.value()) {
						return id;
					} else {
						return registerQmlListProperty(listPropertyName);
					}
				} else if ((AbstractMetaObjectUtility.isListType(clazz)
						|| List.class==parameterizedType.getRawType()
						|| Collection.class==parameterizedType.getRawType()
						|| Deque.class==parameterizedType.getRawType()
						|| Queue.class==parameterizedType.getRawType()
						|| Set.class==parameterizedType.getRawType())
							&& actualTypeArguments.length == 1) {
					if(List.class==parameterizedType.getRawType()) {
						if (actualTypeArguments[0] == String.class) {
							return QMetaType.Type.QStringList.value();
						} else if (actualTypeArguments[0] == QByteArray.class) {
							return QMetaType.Type.QByteArrayList.value();
						} else if (actualTypeArguments[0] instanceof Class<?>) {
							if (QMetaType.qMetaTypeId((Class<?>) actualTypeArguments[0]) == QMetaType.Type.QVariant.value()) {
								return QMetaType.Type.QVariantList.value();
							}
						}
					}
					int elementType = 0;
					if(actualTypeArguments[0] instanceof Class)
						elementType = registerMetaType((Class<?>)actualTypeArguments[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[0]).getRawType() instanceof Class)
						elementType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[0]).getRawType(), actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								elementType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
						}
					}
					if(elementType!=0) {
						int cotainerMetaType;
						if(AbstractMetaObjectUtility.isListType(clazz)) {
							cotainerMetaType = QMetaType.qRegisterMetaType(clazz, new QMetaType(elementType));
						}else if(Deque.class==parameterizedType.getRawType()) {
							cotainerMetaType = QMetaType.qRegisterMetaType(QStack.class, new QMetaType(elementType));
						}else if(Queue.class==parameterizedType.getRawType()) {
							cotainerMetaType = QMetaType.qRegisterMetaType(QQueue.class, new QMetaType(elementType));
						}else if(Set.class==parameterizedType.getRawType()) {
							cotainerMetaType = QMetaType.qRegisterMetaType(QSet.class, new QMetaType(elementType));
						}else {
							cotainerMetaType = QMetaType.qRegisterMetaType(QList.class, new QMetaType(elementType));
						}
						cotainerMetaType = registerRefMetaType(cotainerMetaType, isPointer, isReference);
						return cotainerMetaType;
					}
				} else if ((
							NavigableMap.class==parameterizedType.getRawType()
							|| Map.class==parameterizedType.getRawType()
							|| QMap.class==parameterizedType.getRawType()
							|| QMultiMap.class==parameterizedType.getRawType()
							|| QHash.class==parameterizedType.getRawType()
							|| QMultiHash.class==parameterizedType.getRawType()
							|| QPair.class==parameterizedType.getRawType()
						)
						&& actualTypeArguments.length == 2) {
					if (actualTypeArguments[0] == String.class
							&& actualTypeArguments[1] instanceof Class<?>) {
						if(NavigableMap.class==parameterizedType.getRawType()) {
							if (QMetaType.qMetaTypeId((Class<?>) actualTypeArguments[1]) == QMetaType.Type.QVariant.value()) {
								return QMetaType.Type.QVariantMap.value();
							}
						}
						if(Map.class==parameterizedType.getRawType()) {
							if (QMetaType.qMetaTypeId((Class<?>) actualTypeArguments[1]) == QMetaType.Type.QVariant.value()) {
								return QMetaType.Type.QVariantHash.value();
							}
						}
					}
					int keyType = 0;
					if(actualTypeArguments[0] instanceof Class)
						keyType = registerMetaType((Class<?>)actualTypeArguments[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[0]).getRawType() instanceof Class)
						keyType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[0]).getRawType(), actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								keyType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
						}
					}
					int valueType = 0;
					if(actualTypeArguments[1] instanceof Class)
						valueType = registerMetaType((Class<?>)actualTypeArguments[1], actualTypeArguments[1], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[1] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[1]).getRawType() instanceof Class)
						valueType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[1]).getRawType(), actualTypeArguments[1], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[1] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[1]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								valueType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[1], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
						}
					}
					if(keyType!=0 && valueType!=0) {
						int cotainerMetaType;
						if(NavigableMap.class==parameterizedType.getRawType()) {
							cotainerMetaType = QMetaType.qRegisterMetaType(QMap.class, new QMetaType(keyType), new QMetaType(valueType));
						}else if(Map.class==parameterizedType.getRawType()) {
							cotainerMetaType = QMetaType.qRegisterMetaType(QHash.class, new QMetaType(keyType), new QMetaType(valueType));
						}else {
							cotainerMetaType = QMetaType.qRegisterMetaType(clazz, new QMetaType(keyType), new QMetaType(valueType));
						}
						cotainerMetaType = registerRefMetaType(cotainerMetaType, isPointer, isReference);
						return cotainerMetaType;
					}
				}
			}
		}
		return registerRefMetaType(QMetaType.qRegisterMetaType(clazz), isPointer, isReference);
	}

	public static int objectMetaTypeId(Object o) {
		if (o == null) {
			return QMetaType.Type.Nullptr.value();
		} else {
			return QMetaType.qMetaTypeId(ClassAnalyzerUtility.getClass(o));
		}
	}

	public static int nextMetaTypeId(Class<?> clazz) {
		int id = QMetaType.Type.UnknownType.value();
		if (QtObjectInterface.class.isAssignableFrom(clazz)) {
			QtUtilities.initializePackage(clazz);
			id = QMetaType.qMetaTypeId(clazz);
			if (QMetaType.Type.UnknownType.value() == id) {
				if (!clazz.isInterface())
					id = nextMetaTypeId(clazz.getSuperclass());
				if (QMetaType.Type.UnknownType.value() == id) {
					for (Class<?> iclass : clazz.getInterfaces()) {
						id = nextMetaTypeId(iclass);
						if (QMetaType.Type.UnknownType.value() != id) {
							break;
						}
					}
				}
			}
		} else {
			id = QMetaType.qMetaTypeId(clazz);
		}
		return id;
	}

	public static String internalNameOfArgumentType(Class<? extends Object> cls) {
		return internalTypeNameOfClass(cls, cls);
	}

	public static String internalTypeNameOfClass(Class<? extends Object> cls, Type genericType) {
		try {
			if (isQQmlListProperty(cls) && genericType instanceof ParameterizedType) {
				ParameterizedType ptype = (ParameterizedType) genericType;
				Type actualTypes[] = ptype.getActualTypeArguments();
				if (actualTypes.length == 1 && actualTypes[0] instanceof Class<?>) {
					String argumentName = internalTypeNameOfClass((Class<?>) actualTypes[0], actualTypes[0]);
					if (argumentName.endsWith("*")) {
						argumentName = argumentName.substring(0, argumentName.length() - 1);
					}
					return "QQmlListProperty<" + argumentName + ">";
				}
			}else if (( cls==QMap.class
						|| cls==QHash.class
						|| cls==QMultiMap.class
						|| cls==QMultiHash.class
						|| cls==QPair.class
						|| cls==Map.class
						|| cls==NavigableMap.class) && genericType instanceof ParameterizedType) {
				ParameterizedType ptype = (ParameterizedType) genericType;
				Type actualTypes[] = ptype.getActualTypeArguments();
				if (actualTypes.length == 2) {
					Type keyType = actualTypes[0];
					if(actualTypes[0] instanceof ParameterizedType)
						keyType = ((ParameterizedType) actualTypes[0]).getRawType();
					else if(actualTypes[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>) actualTypes[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof ParameterizedType)
								keyType = ((ParameterizedType) bounds[0]).getRawType();
							else
								keyType = bounds[0];
						}
					}
					Type valueType = actualTypes[1];
					if(actualTypes[1] instanceof ParameterizedType)
						valueType = ((ParameterizedType) actualTypes[1]).getRawType();
					else if(actualTypes[1] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>) actualTypes[1]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof ParameterizedType)
								valueType = ((ParameterizedType) bounds[0]).getRawType();
							else
								valueType = bounds[0];
						}
					}
					if(keyType instanceof Class && valueType instanceof Class) {
						String keyName = internalTypeNameOfClass((Class<?>) keyType, actualTypes[0]);
						String valueName = internalTypeNameOfClass((Class<?>) valueType, actualTypes[1]);
						if(keyType==actualTypes[0])
							QMetaType.qRegisterMetaType((Class<?>) keyType);
						if(valueType==actualTypes[1])
							QMetaType.qRegisterMetaType((Class<?>) valueType);
						if(cls==NavigableMap.class) {
							return String.format("QMap<%1$s,%2$s>", keyName, valueName);
						}else if(cls==Map.class) {
							return String.format("QHash<%1$s,%2$s>", keyName, valueName);
						}else {
							return String.format("%1$s<%2$s,%3$s>", cls.getSimpleName(), keyName, valueName);
						}
					}
				}
			}else if ((
					AbstractMetaObjectUtility.isListType(cls)
					|| cls==Collection.class
					|| cls==Queue.class
					|| cls==Deque.class
					|| cls==List.class
					|| cls==Set.class
					) && genericType instanceof ParameterizedType) {
				ParameterizedType ptype = (ParameterizedType) genericType;
				Type actualTypes[] = ptype.getActualTypeArguments();
				if (actualTypes.length == 1) {
					Type elementType = actualTypes[0];
					if(actualTypes[0] instanceof ParameterizedType)
						elementType = ((ParameterizedType) actualTypes[0]).getRawType();
					else if(actualTypes[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>) actualTypes[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof ParameterizedType)
								elementType = ((ParameterizedType) bounds[0]).getRawType();
							else
								elementType = bounds[0];
						}
					}
					if(elementType instanceof Class) {
						String elementName = internalTypeNameOfClass((Class<?>) elementType, actualTypes[0]);
						if(elementType==actualTypes[0])
							QMetaType.qRegisterMetaType((Class<?>) elementType);
						if(cls==Collection.class
								|| cls==Queue.class
								|| cls==Deque.class
								|| cls==List.class
								|| cls==Set.class) {
							if(cls==Set.class) {
								return String.format("QSet<%1$s>", elementName);
							}else if(cls==Queue.class) {
								return String.format("QQueue<%1$s>", elementName);								
							}else if(cls==Deque.class) {
								return String.format("QStack<%1$s>", elementName);
							}else {
								return String.format("QList<%1$s>", elementName);
							}
						}else {
							return String.format("%1$s<%2$s>", cls.getSimpleName(), elementName);
						}
					}
				}
			}
			String result = internalTypeNameByClass(cls);
			boolean isEnumOrFlags = Enum.class.isAssignableFrom(cls) || QFlags.class.isAssignableFrom(cls);
			if (isEnumOrFlags && "JObjectWrapper".equals(result) && cls.getDeclaringClass() != null
					&& (QObject.class.isAssignableFrom(cls.getDeclaringClass())
							|| Qt.class.isAssignableFrom(cls.getDeclaringClass()))) {
				if (ClassAnalyzerUtility.isGeneratedClass(cls.getDeclaringClass())) {
					result = internalTypeNameOfClass(cls.getDeclaringClass(), null).replace("*", "")
																		+ "::" + cls.getSimpleName();
				}
			}
			return result;
		} catch (Throwable t) {
			java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
			return "";
		}
	}
	
	native static String internalTypeNameByClass(Class<?> cls);
	
	public native static String internalTypeName(String s, ClassLoader classLoader);

	private static Class<?> qmlListPropertiesClass;
	private static boolean qmlListPropertiesClassResolved;

	static boolean isQQmlListProperty(Class<? extends Object> cls) {
		if (!qmlListPropertiesClassResolved) {
			qmlListPropertiesClassResolved = true;
			Class<?> _qmlListPropertiesClass = null;
			try {
				_qmlListPropertiesClass = Class.forName("io.qt.qml.QQmlListProperty");
			} catch (Exception e) {
			}
			qmlListPropertiesClass = _qmlListPropertiesClass;
		}
		return qmlListPropertiesClass != null && qmlListPropertiesClass == cls;
	}
	
	static native int registerQmlListProperty(String type);
	
	@NativeAccess
	private static Object analyzeExpectedTemplateName(java.lang.Class<?> clazz, io.qt.core.QMetaType... instantiations) {
		QtUtilities.initializePackage(clazz);
		Object[] typeParameters = clazz.getTypeParameters();
        if(typeParameters.length>0) {
        	if(typeParameters.length!=(instantiations!=null ? instantiations.length : 0)) {
        		if(instantiations==null || instantiations.length==0) {
    				return null;
        		}
        		throw new IllegalArgumentException("Number of instantiations does not correspond to number of type parameters.");
        	}
        	for (QMetaType instantiation : instantiations) {
        		if(instantiation.id()==0)
        			throw new IllegalArgumentException("Invalid instantiation.");
        	}
        	switch(typeParameters.length) {
        	case 1:
        		if(instantiations[0].id()!=QMetaType.Type.Void.value()) {
        			if(clazz==QList.class && instantiations[0].id()==QMetaType.Type.QString.value()) {
        				return QMetaType.Type.QStringList;
        			}else if(clazz==QList.class && instantiations[0].id()==QMetaType.Type.QByteArray.value()) {
        				return QMetaType.Type.QByteArrayList;
        			}else if(clazz==QList.class && instantiations[0].id()==QMetaType.Type.QVariant.value()) {
        				return QMetaType.Type.QVariantList;
        			}else if(clazz.getName().startsWith("io.qt.core.Q") && AbstractMetaObjectUtility.isSequentialContainer(clazz)) {
        				return String.format("%1$s<%2$s>", clazz.getSimpleName(), instantiations[0].name());
        			}else if(clazz==java.util.Set.class) {
        				return String.format("QSet<%1$s>", instantiations[0].name());
        			}else if(clazz==java.util.Queue.class) {
        				return String.format("QQueue<%1$s>", instantiations[0].name());
        			}else if(clazz==java.util.Deque.class) {
        				return String.format("QStack<%1$s>", instantiations[0].name());
        			}else if(clazz.isInterface() && java.util.List.class.isAssignableFrom(clazz)) {
        				return String.format("QList<%1$s>", instantiations[0].name());
        			}
        		}
        		break;
        	case 2:
        		if(instantiations[0].id()!=QMetaType.Type.Void.value() && instantiations[1].id()!=QMetaType.Type.Void.value()) {
        			if(clazz==QMap.class
    					&& instantiations[0].id()==QMetaType.Type.QString.value()
    					&& instantiations[1].id()==QMetaType.Type.QVariant.value()) {
        				return QMetaType.Type.QVariantMap;
        			}else if(clazz==QHash.class
						&& instantiations[0].id()==QMetaType.Type.QString.value()
						&& instantiations[1].id()==QMetaType.Type.QVariant.value()) {
	    				return QMetaType.Type.QVariantHash;
	    			}else if(clazz.getName().startsWith("io.qt.core.Q") 
	    					&& (QMap.class.isAssignableFrom(clazz) || QMultiMap.class.isAssignableFrom(clazz)
    							 || QHash.class.isAssignableFrom(clazz) || QMultiHash.class.isAssignableFrom(clazz))) {
        				return String.format("%1$s<%2$s,%3$s>", clazz.getSimpleName(), instantiations[0].name(), instantiations[1].name());
        			}else if(clazz==java.util.Map.class) {
        				return String.format("QHash<%1$s,%2$s>", instantiations[0].name(), instantiations[1].name());
        			}else if(clazz==java.util.TreeMap.class) {
        				return String.format("QMap<%1$s,%2$s>", instantiations[0].name(), instantiations[1].name());
        			}else if(clazz==java.util.HashMap.class) {
        				return String.format("QHash<%1$s>", instantiations[0].name(), instantiations[1].name());
        			}else if(clazz.isInterface() && java.util.NavigableMap.class.isAssignableFrom(clazz)) {
        				return String.format("QMap<%1$s,%2$s>", instantiations[0].name(), instantiations[1].name());
        			}
        		}
        		break;
        	}
        	if(io.qt.QtObjectInterface.class.isAssignableFrom(clazz)) {
        		String name = clazz.getSimpleName();
        		QStringList _instantiations = new QStringList();
        		for (QMetaType instantiation : instantiations) {
        			_instantiations.add(instantiation.name().toString());
        		}
        		return String.format("%1$s<%2$s>", name, _instantiations.join(','));
        	}
        }else {
        	if(instantiations!=null && instantiations.length>0) {
        		throw new IllegalArgumentException("Type "+clazz.getName()+" does not accept instantiations.");
        	}
        }
        return null;
	}
}
