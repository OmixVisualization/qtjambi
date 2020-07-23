package io.qt.sql;

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.Objects;
import java.util.function.Supplier;

import io.qt.internal.QtJambiInternal;

public class QSqlDriverCreator<T extends QSqlDriver> extends QSqlDriverCreatorBase {
	
	private final Supplier<T> constructorHandle;

	public QSqlDriverCreator(Supplier<T> constructor) {
		constructorHandle = Objects.requireNonNull(constructor);
	}
	
	public QSqlDriverCreator(Class<T> type) {
		MethodHandle handle = findConstructor(type);
		constructorHandle = ()->{
			Object object;
			try {
				object = handle.invoke();
			} catch (RuntimeException | Error e) {
				throw e;
			} catch (Throwable e) {
				throw new RuntimeException(e);
			}
			return type.cast(object);
		};
	}
	
	private static MethodHandle findConstructor(Class<?> type){
    	if(Modifier.isAbstract(type.getModifiers())) {
        	throw new RuntimeException("Cannot construct QSqlDriverCreator for abstract widget type "+type.getName()+".");
        }
    	Constructor<?> constructor;
        try {
        	constructor = type.getDeclaredConstructor();
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing constructor "+type.getSimpleName()+"().", e);
		}
        try {
			return QtJambiInternal.getConstructorHandle(constructor);
		} catch (IllegalAccessException e) {
			throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing access to constructor "+type.getSimpleName()+"().", e);
		}
    }
	
	@Override
	public T createObject() {
		return constructorHandle.get();
	}

}
