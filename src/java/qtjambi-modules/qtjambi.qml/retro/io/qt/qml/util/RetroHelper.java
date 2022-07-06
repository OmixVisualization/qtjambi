package io.qt.qml.util;

import io.qt.internal.QtJambiInternal;

final class RetroHelper {
	private RetroHelper() {throw new RuntimeException();}
	static Package getDefinedPackage(ClassLoader cl, String pkg) {
        try {
			return cl.getDefinedPackage(pkg);
		} catch (Throwable e) {}
        try {
            return (Package)QtJambiInternal.invokeMethod(ClassLoader.class.getDeclaredMethod("getPackage", String.class), cl, pkg);
		} catch (Throwable e) {}
        try {
        	for(java.lang.Package _pkg : java.lang.Package.getPackages()) {
        		if(_pkg.getName().equals(pkg))
        			return _pkg;
        	}
		} catch (Throwable e) {}
        try {
			return cl.loadClass(pkg.replace("/", ".")+".package-info").getPackage();
		} catch (Throwable e) {}
		return null;
    }
}
