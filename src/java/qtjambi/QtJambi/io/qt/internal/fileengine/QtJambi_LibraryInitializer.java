package io.qt.internal.fileengine;

final class QtJambi_LibraryInitializer {
    static{
        try {
            io.qt.QtUtilities.initializePackage("io.qt.internal");
        } catch(RuntimeException | Error t) {
            throw t;
        } catch(Throwable t) {
            throw new RuntimeException(t);
        }
    }
    static void init() { };
    private QtJambi_LibraryInitializer() { };
}

