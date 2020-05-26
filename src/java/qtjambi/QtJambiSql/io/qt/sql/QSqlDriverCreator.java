package io.qt.sql;

import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.Objects;

public class QSqlDriverCreator<T extends QSqlDriver> extends QSqlDriverCreatorBase {
	
	public interface ConstructorHandle<T>{
		T create();
	}

	private final ConstructorHandle<T> constructorHandle;

	public QSqlDriverCreator(ConstructorHandle<T> constructor) {
		constructorHandle = Objects.requireNonNull(constructor);
	}
	
	public QSqlDriverCreator(Class<T> type) {
		long handle = findConstructor(type);
		constructorHandle = ()->construct(type, handle);
	}
	
	private static long findConstructor(Class<?> type){
    	if(Modifier.isAbstract(type.getModifiers())) {
        	throw new RuntimeException("Cannot construct QSqlDriverCreator for abstract widget type "+type.getName()+".");
        }
    	Constructor<?> constructor;
        try {
        	constructor = type.getDeclaredConstructor();
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing constructor T().", e);
		}
        return constructorHandle(constructor);
    }
	
	private static native long constructorHandle(Constructor<?> constructor);
	private static native <T> T construct(Class<T> cls, long constructorHandle);

	@Override
	public T createObject() {
		return constructorHandle.create();
	}

}
