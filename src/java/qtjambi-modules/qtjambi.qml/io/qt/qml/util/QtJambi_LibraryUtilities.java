package io.qt.qml.util;

final class QtJambi_LibraryUtilities {
    static{
        try {
            io.qt.QtUtilities.initializePackage("io.qt.qml");
        } catch(Error t) {
            throw t;
        } catch(Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }
    private QtJambi_LibraryUtilities() throws java.lang.InstantiationError { throw new java.lang.InstantiationError("Cannot instantiate QtJambi_LibraryUtilities."); }
    
    static final io.qt.InternalAccess internal = internalAccess();

    private static native io.qt.InternalAccess internalAccess();
    
    static void initialize() { };
}

