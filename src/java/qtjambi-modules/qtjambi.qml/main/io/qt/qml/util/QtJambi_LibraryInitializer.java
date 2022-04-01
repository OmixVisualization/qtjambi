package io.qt.qml.util;

final class QtJambi_LibraryInitializer {
    static{
        try {
            io.qt.QtUtilities.initializePackage("io.qt.qml");
        } catch(Error t) {
            throw t;
        } catch(Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }
    static void init() { };
    private QtJambi_LibraryInitializer() { };
}

