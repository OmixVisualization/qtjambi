package io.qt.internal;

import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Field;
import java.util.function.Supplier;

import io.qt.core.QPair;

public class RetroHelper {
	
	static boolean trySetAccessible(Field f) {
		try {
			f.setAccessible(true);
		} catch (Throwable e) {
		}
		return f.isAccessible();
	}
	
	static AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
		return null;
	}
	
	@SuppressWarnings("deprecation")
	private static Supplier<Class<?>> callerClassProvider = ()->sun.reflect.Reflection.getCallerClass(4);
	
	private static Supplier<QPair<Class<?>, String>> classAccessChecker = ()->{
		StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
		if(stackTrace.length>4) {
			try {
				return new QPair<Class<?>, String>(Class.forName(stackTrace[4].getClassName()), stackTrace[4].getMethodName());
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
		}
		return null;
	};
	
	static Supplier<Class<?>> callerClassProvider(){
		return callerClassProvider;
	}
	
	static Supplier<QPair<Class<?>, String>> classAccessChecker(){
		return classAccessChecker;
	}
}
