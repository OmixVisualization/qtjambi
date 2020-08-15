package io.qt.qml.util;

final class RetroHelper {
	private RetroHelper() {throw new RuntimeException();}
	static Package getDefinedPackage(ClassLoader cl, String pkg) {
        return cl.getDefinedPackage(pkg);
    }
}
