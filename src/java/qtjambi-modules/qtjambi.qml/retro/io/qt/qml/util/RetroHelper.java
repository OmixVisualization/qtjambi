package io.qt.qml.util;

final class RetroHelper {
	private RetroHelper() {throw new RuntimeException();}
	static Package getDefinedPackage(ClassLoader cl, String pkg) {
        try {
			return cl.getDefinedPackage(pkg);
		} catch (Throwable e) {
			return null;
		}
    }
}
