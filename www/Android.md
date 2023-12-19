# How to Create Android Apps

You can use QtJambi to develop Qt-based applications for Android. Therefore, use *Android Studio* as IDE.

Start your software project with "No Activity" template. Specify your intended application name and package name. 
You can develop your application in Java or Kotlin. As minimum SDK version choose API 26 or higher.
(If you need to support lower API versions you'll have exclude certain features).

The new project consists of several directories, most important `manifests`, `java`, and `res`.

## Manifest

The next step is to adapt the `AndroidManifest.xml` specification to be found in `manifests`.
Add the following activity to the `<application>` tag:

```xml
	<activity
		android:name="org.qtproject.qt.android.bindings.QtActivity"
		android:exported="true">
		<intent-filter>
			<action android:name="android.intent.action.MAIN" />
			<category android:name="android.intent.category.LAUNCHER" />
		</intent-filter>
		<meta-data
			android:name="android.app.lib_name"
			android:value="QtJambi" />
	</activity>
```

(If you are using Qt5 the path is `org.qtproject.qt5.android.bindings.QtActivity`)

## Resource Entries

Next step is to add several resources to the project that are needed by Qt library.
Therefore, go to directory `res/values` and open the file `strings.xml`.
Add the following entries to the `<resources>` tag:

```xml
    <array name="qt_libs">
        <item>armeabi-v7a;c++_shared</item>
        <item>arm64-v8a;c++_shared</item>
        <item>x86_64;c++_shared</item>
        <item>x86;c++_shared</item>
    </array>
    <array name="bundled_libs"></array>
    <array name="load_local_libs"></array>
    <string name="static_init_classes"></string>
    <string name="use_local_qt_libs">1</string>
    <string name="bundle_local_qt_libs">1</string>
```

## Dependencies

A critical resource for QtJambi is available Qt library. Download *Qt installer* and install Qt binaries for Android.
Android projects need Qt to be available as jar bundles.
Therefore, follow the instructions [here](How-to-bundle-Qt-libraries.md) to create Qt bundles.
Copy the required bundles to the app folder of your software project, e.g. for ABI arm64-v8a:

- `qt-lib-core-native-android-arm64-6.5.4.jar`
- `qt-lib-core-native-android-common-6.5.4.jar`
- `qt-lib-gui-native-android-arm64-6.5.4.jar`
- `qt-lib-widgets-native-android-arm64-6.5.4.jar`

Several libraries have a `native-android-common` component.

The next step is to add all required QtJambi libraries to the project.
Therefore, open the file `app/build.gradle`. 
Add the following lines to the `dependencies` section (for ABI arm64-v8a):

```gradle
    implementation 'io.qtjambi:qtjambi:6.5.4'
    implementation 'io.qtjambi:qtjambi-native-android-arm64:6.5.4'
    implementation files('qt-lib-core-native-android-arm64-6.5.3.jar',
                         'qt-lib-core-native-android-common-6.5.3.jar',
                         'qt-lib-gui-native-android-arm64-6.5.3.jar',
                         'qt-lib-widgets-native-android-arm64-6.5.3.jar')
```

Exchange `android-arm64` by `android-arm` (armeabi-v7a), `android-x86` (x86) or `android-x64` (x86_64).

In case you add further qtjambi modules you might need to add the following code to the `android` section:

```gradle
    packagingOptions{
        exclude 'META-INF/INDEX.LIST'
    }
```

## Implementation

Now, your Android project's configuration is finished. The next step is to develop the application.
QtJambi expects a start-up class in the application's top level package called `Main`. 
I.e. if your application package is `com.myapp` then create the Java class `com.myapp.Main` containing a static main method:

```java
package com.myapp;

import io.qt.widgets.*;

public class Main{
    public static void main(String[] args) {
        QApplication.initialize(args);
        QMessageBox.information(null, "QtJambi", "Hello World!");
        QApplication.shutdown();
    }
}
```

Alternatively, if you decide to use *Kotlin*, create a Kotlin file called `main` in your application package:

```kotlin
package com.myapp

import io.qt.widgets.*

fun main(args : Array<String>){
    QApplication.initialize(args)
    QMessageBox.information(null, "QtTest", "Hello World!")
    QApplication.shutdown()
}
```

[Read more](Characteristics-of-QtJambi.md) about how to develop with QtJambi.
