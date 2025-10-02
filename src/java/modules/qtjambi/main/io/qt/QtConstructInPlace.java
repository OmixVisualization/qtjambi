package io.qt;

import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.function.Consumer;

import io.qt.core.QMetaObject;

/**
 * Single constructor argument for in-place construction, i.e. constructors being called
 * via {@link QMetaObject#static_metacall(io.qt.core.QMetaObject.Call, int, Object[])} or QML.
 * @see QMetaObject.Call#ConstructInPlace
 */
public final class QtConstructInPlace{
	@NativeAccess
    private QtConstructInPlace(long native_id) {
        this.native_id = native_id;
    }
	private final long native_id;
	public void initialize(@StrictNonNull QtObject instance) {
		initialize(native_id, Objects.requireNonNull(instance));
	}
	public void initWithArguments(@StrictNonNull QtObject instance, int... indexes) {
		initWithArguments(native_id, Objects.requireNonNull(instance), indexes);
	}
	public int parameterCount() {
		return parameterCount(native_id);
	}
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public <T> T argumentAt(int index, @StrictNonNull Class<T> type) {
		if(type.isPrimitive()) {
			if(type==int.class)
				type = (Class)Integer.class;
			if(type==long.class)
				type = (Class)Long.class;
			if(type==short.class)
				type = (Class)Short.class;
			if(type==float.class)
				type = (Class)Float.class;
			if(type==double.class)
				type = (Class)Double.class;
			if(type==byte.class)
				type = (Class)Byte.class;
			if(type==boolean.class)
				type = (Class)Boolean.class;
			if(type==char.class)
				type = (Class)Character.class;
		}
		return type.cast(argumentAt(native_id, index));
	}
	public Class<?> parameterTypeAt(int index) {
		return parameterTypeAt(native_id, index);
	}
	public QtConstructInPlace asArguments(int @NonNull... indexes) {
		return asArguments(native_id, indexes);
	}
	public QtConstructInPlace asArguments(Consumer<ArgumentMapper> argumentMapper) {
		ArgumentMapper mapper = new ArgumentMapper();
		argumentMapper.accept(mapper);
		return mapper.indexes==null ? this : asArguments(native_id, mapper.indexes);
	}
	/**
	 * Argument mapper.
	 */
	public final class ArgumentMapper{
    	private int[] indexes;
    	public boolean matches(@NonNull Class<?> @StrictNonNull... types) {
    		return QtConstructInPlace.this.matches(types);
    	}
    	public void map(int...indexes) {
    		this.indexes = indexes;
    	}
	}
	public boolean matches(@NonNull Class<?> @StrictNonNull... types) {
		if(parameterCount()==types.length) {
			boolean matches = true;
			for (int i = 0; i < types.length; i++) {
				if(parameterTypeAt(i)!=types[i]) {
					matches = false;
					break;
				}
			}
			return matches;
		}
		return false;
	}
	/**
	 * Argument stream.
	 */
	public final class Stream{
		private final QtArgument.Stream stream;
		Stream(Class<?> type){
			stream = new QtArgument.Stream(type);
		}
		public @StrictNonNull Stream add(Object value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(int value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(long value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(short value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(byte value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(double value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(float value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(char value) {
			stream.add(value);
			return this;
		}
		public @StrictNonNull Stream add(boolean value) {
			stream.add(value);
			return this;
		}
		public <T> @StrictNonNull Stream add(@StrictNonNull Class<? super T> type, T value) {
			stream.add(type, value);
			return this;
		}
		public @StrictNonNull Stream addIndex(int index) {
			stream.addIndexes(index);
			return this;
		}
		public @StrictNonNull Stream addIndexes(int index, int @NonNull... indexes) {
			stream.addIndexes(index, indexes);
			return this;
		}
		public @StrictNonNull Stream begin(Class<?> type) {
			stream.begin(type);
			return this;
		}
		public void initialize(@StrictNonNull QtObject instance) {
			initFromStream(native_id, Objects.requireNonNull(instance), stream.arguments());
    	}
	}
	public @StrictNonNull Stream begin(@NonNull Class<?> type) {
		return new Stream(type);
	}
	private static native void initialize(long native_id, QtObject instance);
	private static native void initWithArguments(long native_id, QtObject instance, int[] indexes);
	private static native void initFromStream(long native_id, QtObject instance, Map<Class<?>, List<Map.Entry<Object,Object>>> arguments);
	private static native int parameterCount(long native_id);
	private static native Object argumentAt(long native_id, int index);
	private static native Class<?> parameterTypeAt(long native_id, int index);
	private static native QtConstructInPlace asArguments(long native_id, int[] indexes);
}