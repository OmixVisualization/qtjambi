# How To Deploy QtJambi Applications

You can ship your Java-based application as platform-dependent
executable by using the *QtJambi application deployer tool*. Therefore,
download **qtjambi-deployer.jar** from the release of your choice 
along with platform-dependent **qtjambi-deployer-native-X.jar** for all platforms you aim at.

In the examples below, we assume you created an application in a jar
file `my-company-app.jar` with main class `my.company.app.Main`
depending on `qtjambi-6.5.4.jar`.

Call the app deployer as shown below. Make sure the library path points
to the *Qt* libraries being `bin` on *Windows* and `lib`
on *Linux* and *macOS*.

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer app
```

Alternative way to call it:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -cp qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     io.qt.qtjambi.deployer.Main app
```


Subsequently, you specify arguments for *QtJambi deployer*
to create the platform-dependent executable. The executable is then
equipped with all arguments needed by Java to execute the program.

Possible arguments are:

  - `--executable=...` location of the platform-dependent
    *QtJambiLauncher* executable's location (optional if qtjambi-deployer-native components are in classpath)
  - `--class-path=...` class path to be used by Java to find all
    required classes relative to the executable's location
  - `--module-path=...` module path to be used by Java to find all
    required classes relative to the executable's location
  - `--library-path=...` path to *Qt* and other required
    libraries relative to the executable's location
  - `--jvm-path=...` path to Java Runtime Environment relative to the
    executable's location
  - `--autodetect-jvm` alternatively you can let the program detect Java
    automatically (minimum version required)
  - `--minversion-jvm=...` required when autodetection of Java is
    enabled
  - `--application=...` name of the generated application
  - `--filter=...` filters the program arguments. 
     All program arguments starting with the given filter are treated as JVM argument.
	 (Can be used multiple times)
  - `--dir=...` target directory to place the prepared binary
  - ...additional Java runtime arguments after `-- `

It is not possible to create executables for Android.

## Deploying on Windows

On *Windows*, the executable needs to be located in the same directory
as the `Qt6Core.dll`. We design the target directory structure as
follows:

``` shell
installation directory
    | bin
        | Qt6Core.dll
        | Qt6Gui.dll
        | Qt6Widgets.dll
        | MyCompanyApp.exe
    | plugins
        | ... (platforms, styles and others)
    | lib
        | qtjambi-6.5.4.jar
        | qtjambi-native-windows-x64-6.5.4.jar
        | my-company-app.jar
    | jre
        | ... (Java Runtime Environment)
```

To create the corresponding executable `MyCompanyApp.exe` you need to
append the following commands to the above listed *QtJambi deployer* command:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer app
     --application=MyCompanyApp
     --executable=utilities/QtJambiLauncher.exe
     --class-path=../lib/my-company-app.jar:../lib/qtjambi-6.5.4.jar:../lib/qtjambi-native-windows-x64-6.5.4.jar
     --library-path=.
     --jvm-path=../jre
     --main-class=my.company.app.Main
     --dir=output directory (may be installation directory)
     --ico=path to icons/app.ico
```

The former argument `--ico` is now depdecated. If you want to change the executable's icon use third parts software.

## Deploying on Linux

On *Linux*, the executable searches for library `libQt6Core.so` in
directory `lib` or `../lib`. We design the target directory structure as
follows:

``` shell
installation directory
    | MyCompanyApp
    | plugins
        | ... (platforms, styles and others)
    | lib
        | libQt6Core.so[.6.5.3]
        | libQt6Gui.so[.6.5.3]
        | libQt6Widgets.so[.6.5.3]
        | qtjambi-6.5.4.jar
        | qtjambi-native-linux-x64-6.5.4.jar
        | my-company-app.jar
    | jre
        | ... (Java Runtime Environment)
```

To create the corresponding executable `MyCompanyApp` you need to append
the following commands to the above listed *QtJambi deployer* command:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer app
     --application=MyCompanyApp
     --executable=utilities/QtJambiLauncher
     --class-path=lib/my-company-app.jar:lib/qtjambi-6.5.4.jar:lib/qtjambi-native-linux-x64-6.5.4.jar
     --library-path=lib
     --jvm-path=jre
     --main-class=my.company.app.Main
     --dir=output directory (may be installation directory)
```

## Deploying on macOS

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
             | lib
                | qtjambi-6.5.4.jar
                | qtjambi-native-macos-6.5.4.jar
                | my-company-app.jar
            | jre
                | ... (Java Runtime Environment)
            | Resources
            | Info.plist
            | PkgInfo
```

To create the corresponding executable `MyCompanyApp.app` you need to
append the following commands to the above listed *QtJambi deployer* command:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer app
     --application=MyCompanyApp
     --executable=utilities/QtJambiLauncher.app
     --class-path=../lib/my-company-app.jar:../lib/qtjambi-6.5.4.jar:../lib/qtjambi-native-macos-6.5.4.jar
     --library-path=../Frameworks
     --jvm-path=../jre
     --main-class=my.company.app.Main
     --dir=output directory (may be installation directory)
```
