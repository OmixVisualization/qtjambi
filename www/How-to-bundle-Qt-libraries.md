# How To Bundle Qt

You can bundle Qt libraries as platform-dependent jar files self-extracting at startup by using the *QtJambi application deployer tool*.
Therefore, download **qtjambi-deployer.jar** from the release of your choice.
If you want to create bundles for Android you also need platform-dependent **qtjambi-deployer-native-android-X.jar**.

Call Qt deployer as shown below. Make sure the library path points
to the *Qt* libraries being `bin` on *Windows* and `lib`
on *Linux* and *macOS*.

``` shell
java -Djava.library.path=<path to Qt libraries>
     -p qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     -m qtjambi.deployer qt
```

Alternative way to call it:

``` shell
java -Djava.library.path=<path to Qt libraries>
     -cp qtjambi-6.5.4.jar:qtjambi-deployer-6.5.4.jar
     io.qt.qtjambi.deployer.Main qt
```

*QtJambi deployer* autodetects Qt installation on your computer and creates jar bundles for each of the Qt libraries along with their specific plugins.
E.g. the output files for Windows look this way:

- `qt-lib-core-native-windows-x64-6.5.3.jar`
- `qt-lib-gui-native-windows-x64-6.5.3.jar`
- `qt-lib-qml-native-windows-x64-6.5.3.jar`
- ...

Place these libraries next to the **qtjambi-native-X.jar** files.

## Additional Options

Possible arguments are:

  - `--platform=...` Target platform, e.g. `android-arm64`, `windows-x64`, `linux-x64`, `macos`. 
  - `-d`, `--target-directory=...` Directory where to place the generated bundle files
  - `--condifuration=...` Either `debug` or `release` (default), wether to bundle release libraries or debug libraries.
  - `--qtdir=...` path to *Qt* version, e.g. `C:\Qt\6.5.3\msvc2019_64`
  - `--qtbase=...` alternatively path to *Qt* installation, e.g. `C:\Qt`.
  - `--target-version=` version to be bundles
  - `--utilities=...` path to Android utilities (only for android bundles). E.g. `--utilities=utilities/QtAndroidBindings.jar`.
  - `--utilities-location=...` alternatively, location of Android utilities (only for android bundles)
