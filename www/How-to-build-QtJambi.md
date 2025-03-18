# How To Build QtJambi From Sources

If you just want to use QtJambi for your application development you can skip these instructions 
and simply use the provided binaries from the [list of modules](www/Modules.md).
Make yourself familiar with [developing applications with QtJambi](www/First-Steps-With-QtJambi.md).

The following instructions let you build the entire QtJambi library framework from source code.
Therefore, download [QtJambi source code from Github](https://github.com/OmixVisualization/qtjambi/).

## Requirements
* [Apache Ant](https://ant.apache.org/) (min. 1.10.x)
* Java Development Kit (e.g. [OpenJDK](https://adoptopenjdk.net/), tested with Java 1.8, 11 and 21)
* Qt 5.15 or 6.x (using the Qt Online Installer)
* Minimum required Qt modules: QtCore, QtQml, QtNetwork, QtConcurrent and QtXml
* C++ compiler and make (Gcc, Clang, MSVC2022)
* XCode command line tools (macOS only)

When building for Android all required SDK and NDK components are downloaded automatically.
QtJambi build system has been tested on Linux (Ubuntu x64 and arm64, OpenSUSE x64, Fedora x64), Windows (with MSVC2022 for x64 and amd64), macOS (x64 and amd64) and FreeBSD (x64).

## Building QtJambi

If you don't need the entire set of Qt modules available in Java edit property `qtjambi.skipped.modules` in `build.properties` and exclude Qt modules you don't need.
This saves compilation time. Although QtJambi build process requires the availability of QtQml, QtNetwork, QtConcurrent and QtXml, you can skip them from being generated as QtJambi module.

On Windows open the Visual Studio command prompt for x64 architecture (to be found in: --> Start Menu --> Programs --> Visual Studio 2022 --> Visual Studio Tools --> VC).
On Linux, macOS or if you intend to use Mingw instead of MSVC open a plain terminal.

Change to directory of your QtJambi clone.

Add Apache Ant to your `PATH` variable for easy access:

`> set PATH=<path to..>\apache-ant\bin;%PATH%`

If the command line does not find the program `java` by default or if you intend to use specific Java installation add this variable:

`> set JAVA_HOME_TARGET=path to your java jdk`

Start the build process by following command:

`> ant all`

(This step may take several hours.)

QtJambi's build process will automatically detect installed Qt:

* On Windows it expects Qt to be installed in `C:\Qt`.

* On Linux and macOS it searches for Qt installation in `/opt/Qt`.

* On macOS it additionally searches in `/Library/Qt` and `/System/Qt`.

If Qt has not been found at these locations the build process searches in the user's home directory and in QtJambi's parent directory for Qt installation.

If a Qt installation has been found the build process starts building QtJambi for all Qt versions available.

If no Qt installation has been found the build process tries to detect `qmake` executable (on Linux additionally `qmake-qt5`/`qmake-qt6`).

The generated QtJambi version is based upon the Qt version linked against. Be aware that the patch version number (third) is not necessarily the same for Qt and QtJambi.

Finally, find all Java libraries in directory `<qtjambiversion>/deployment` and native bundles in `<qtjambiversion>/deployment/native`. Additionally, the platform-dependent libraries can be found in `<qtjambiversion>/deployment/platforms`.

### Additional Options

You can call ant with additional properties as listed below. Therefore use the `-D` command line argument: `ant -Dkey=value all`.

* `qt` - specify comma-separated Qt versions to be used, e.g. `-Dqt="6.5,6.7,6.8"`.
* `qtbase` - specify Qt installer's base directory, e.g. `-Dqtbase=/var/Qt`. Can be combined with `qt`.
* `qtdir` - specify Qt version and platform directory, e.g. `-Dqtdir=/var/Qt/6.8.3/macos`. This option allows multiple directories separated by path separator. (If this option is specified `qt` and `qtbase` have no effect.)
* `qmake` - specify a path to a `qmake` program to be used for building QtJambi. (If this option is specified `qt`, `qtbase` and `qtdir` have no effect.)
* `android` - specify `true` to build QtJambi for Android. Therefore, Qt for Android has to be installed. By specify `-Dandroid=only` ant skips building QtJambi for the build platform. When compiling for Android all required NDK and SDK components are downloaded automatically. Alternatively, specify the Android NDK install path with `-Dndk=...`.

All these properties could also be placed in `build.properties` file.

Alternatively, ant reads following environment variables:

* `QTVERSIONS` - may soecify comma-separated Qt versions to be used
* `QTBASE` - may specify Qt installer's base directory
* `QTDIR` - may specify Qt version and platform directory
* `ANDROID_NDK` - path to installed Android NDK

### Unit Tests

Optionally, you could create and run unit tests:

`> ant tests.generate tests.run`

Find unit test results in directory `TestResults`.

### Cross-Compilation

After building QtJambi for the running operation system you can additionally cross-compile QtJambi for other platforms.
For instance if you want to build for Linux arm:

`> ant -Dqmake=/opt/Qt/6.8.3/arm-gnueabi/bin/qmake library.native`

Just specify the third-platform cross-compiled qmake with `-Dqmake=path`.

If you are using Qt from Qt installer located at default installation path (e.g. `C:\Qt`)
and Android binaries are installed you can simply use the `-Dandroid=true` argument to 
build QtJambi for Android along with the platform-soecific binaries:

`> ant -Dandroid=true all`
