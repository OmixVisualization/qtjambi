package io.qt.designer.util;

final class QtJambi_LibraryUtilities {
    static{
        try {
            io.qt.QtUtilities.initializePackage("io.qt.designer");
        } catch(RuntimeException | Error t) {
            throw t;
        } catch(Throwable t) {
            throw new RuntimeException(t);
        }
    }

    static void initialize() { };

    private QtJambi_LibraryUtilities() throws java.lang.InstantiationError { throw new java.lang.InstantiationError("Cannot instantiate QtJambi_LibraryUtilities."); }
}

