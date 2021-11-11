You can ship your Java-based application as platform-dependent
executable by using the *QtJambi application deployer tool*. Therefore,
download **qtjambi-appdeployer.jar** from the release of your choice
along with the platform-dependent **tools binaries**.

In the examples below, we assume you created an application in a jar
file `my-company-app.jar` with main class `my.company.app.Main`
depending on `qtjambi-6.1.jar`.

Call the app deployer as shown below. Make sure the library path points
to the *Qt* and *QtJambi* libraries being `bin` on *Windows* and `lib`
on *Linux* and *macOS*.

``` shell
java -cp qtjambi-appdeployer-6.1.jar:qtjambi-6.1.jar 
        -Djava.library.path=lib
        io.qt.qtjambi.appdeployer.Main
```

Subsequently, you specify arguments for *QtJambi application deployer*
to create the platform-dependent executable. The executable is then
equipped with all arguments needed by Java to execute the program.

Possible arguments are:

  - `--executable=...` location of the platform-dependent
    *QtJambiLauncher* executable's location
  - `--class-path=...` class path to be used by Java to find all
    required classes relative to the executable's location
  - `--module-path=...` module path to be used by Java to find all
    required classes relative to the executable's location
  - `--library-path=...` path to *QtJambi*, *Qt* and other required
    libraries relative to the executable's location
  - `--jvm-path=...` path to Java Runtime Environment relative to the
    executable's location
  - `--autodetect-jvm` alternatively you can let the program detect Java
    automatically (minimum version required)
  - `--minversion-jvm=...` required when autodetection of Java is
    enabled
  - `--ico=...` icon to be injected into the Windows executable (Windows
    only)
  - `--application=...` name of the generated application
  - `--dir=...` target directory to place the prepared binary
  - ...additional Java runtime arguments

# Deploying on Windows

On *Windows*, the executable needs to be located in the same directory
as the `Qt6Core.dll`. We design the target directory structure as
follows:

``` shell
installation directory
    | bin
        | Qt6Core.dll
        | Qt6Gui.dll
        | Qt6Widgets.dll
        | QtJambiCore6.dll
        | QtJambiGui6.dll
        | QtJambiWidgets6.dll
        | MyCompanyApp.exe
    | plugins
        | ... (platforms, styles and others)
    | lib
        | qtjambi-6.1.jar
        | my-company-app.jar
    | jre
        | ... (Java Runtime Environment)
```

To create the corresponding executable `MyCompanyApp.exe` you need to
append the following commands to the above listed *QtJambi application
deployer* command:

``` shell
        --application=MyCompanyApp
        --executable=utilities/QtJambiLauncher.exe
        --class-path=../lib/my-company-app.jar:../lib/qtjambi-6.1.jar
        --library-path=.
        --jvm-path=../jre
        --main-class=my.company.app.Main
        --dir=output directory (may be installation directory)
        --ico=path to icons/app.ico
```

The icon file has to provide 16,24,32,64 and 128 square pixel icons in
uncompressed RGBA color mode. It must not exceed 90 KB (92.478 Bytes).

# Deploying on Linux

On *Linux*, the executable searches for library `libQt6Core.so` in
directory `lib` or `../lib`. We design the target directory structure as
follows:

``` shell
installation directory
    | MyCompanyApp
    | plugins
        | ... (platforms, styles and others)
    | lib
        | libQt6Core.so[.6.1.0]
        | libQt6Gui.so[.6.1.0]
        | libQt6Widgets.so[.6.1.0]
        | libQtJambiCore6.so[.6.1.0]
        | libQtJambiGui6.so[.6.1.0]
        | libQtJambiWidgets6.so[.6.1.0]
        | qtjambi-6.1.jar
        | my-company-app.jar
    | jre
        | ... (Java Runtime Environment)
```

To create the corresponding executable `MyCompanyApp` you need to append
the following commands to the above listed *QtJambi application
deployer* command:

``` shell
        --application=MyCompanyApp
        --executable=utilities/QtJambiLauncher
        --class-path=lib/my-company-app.jar:lib/qtjambi-6.1.jar
        --library-path=lib
        --jvm-path=jre
        --main-class=my.company.app.Main
        --dir=output directory (may be installation directory)
```

# Deploying on macOS

On *macOS*, the executable searches for framework `QtCore.framework`
inside of the app bundle or in directory `lib`. Thus, you can place all
libraries and even Java inside of the bundle or use a directory design
similar to *Linux*. Here, we design the target directory structure as
follows:

``` shell
installation directory
    | MyCompanyApp.app
        |Contents
            | MacOS
                | MyCompanyApp
            | plugins
                | ... (platforms, styles and others)
            | Frameworks
                | QtCore.framework
                | QtGui.framework
                | QtWidgets.framework
                | libQtJambiCore6[.6.1.0].jnilib
                | libQtJambiGui6[.6.1.0].jnilib
                | libQtJambiWidgets6[.6.1.0].jnilib
             | lib
                | qtjambi-6.1.jar
                | my-company-app.jar
            | jre
                | ... (Java Runtime Environment)
            | Resources
            | Info.plist
            | PkgInfo
```

To create the corresponding executable `MyCompanyApp.app` you need to
append the following commands to the above listed *QtJambi application
deployer* command:

``` shell
        --application=MyCompanyApp
        --executable=utilities/QtJambiLauncher.app
        --class-path=../lib/my-company-app.jar:../lib/qtjambi-6.1.jar
        --library-path=../Frameworks
        --jvm-path=../jre
        --main-class=my.company.app.Main
        --dir=output directory (may be installation directory)
```
