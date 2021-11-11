package io.qt.internal.fileengine;

final class QtJambi_LibraryUtilities {
    static{
        try {
            io.qt.QtUtilities.initializePackage("io.qt.internal");
        } catch(RuntimeException | Error t) {
            throw t;
        } catch(Throwable t) {
            throw new RuntimeException(t);
        }
    }
    static void initialize() { };
    private QtJambi_LibraryUtilities() { };
}

