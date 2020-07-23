package io.qt.internal;

import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Field;
import java.util.Optional;
import java.util.function.Supplier;
import io.qt.core.QPair;

public class RetroHelper {
	private static final StackWalker stackWalker = StackWalker.getInstance(java.util.Collections.singleton(StackWalker.Option.RETAIN_CLASS_REFERENCE));
	private static final Supplier<Class<?>> callerClassProvider = stackWalker::getCallerClass;
	private static final Supplier<QPair<Class<?>, String>> callerClassProvider2 = ()->{
		Optional<StackWalker.StackFrame> stackFrame = stackWalker.walk(stream->stream.limit(4).skip(3).findFirst());
		if(stackFrame.isPresent()) {
			return new QPair<>(
				stackFrame.get().getDeclaringClass(),
				stackFrame.get().getMethodName());
		}else {
			return null;
		}
	};
	
	static boolean trySetAccessible(Field f) {
		return f.trySetAccessible();
	}
	
	static AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
		return actualType.getAnnotatedOwnerType();
	}
	
	static Supplier<Class<?>> callerClassProvider(){
		return callerClassProvider;
	}
	
	static Supplier<QPair<Class<?>, String>> classAccessChecker(){
		return callerClassProvider2;
	}
}
