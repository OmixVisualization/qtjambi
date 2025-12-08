![QtJambi - UI Framework for Java](https://github.com/OmixVisualization/qtjambi/raw/master/www/images/QtJambi_eyecatcher.png "QtJambi - UI Framework for Java")

# QtJambi - UI Framework for Java

## Overview

QtJambi is a UI framework for Java and Kotlin based upon [Qt](https://www.qt.io).
It makes the entire Qt framework available by providing Java wrappers for all C++ types and functions in the Qt API.
Like Qt framework, QtJambi is modular and provides different libraries for various application fields.
The basic module `qtjambi` provides bindings for native modules `QtCore`, `QtGui` and `QtWidgets` which are a good starting point to create sophisticated user interfaces.
The modules `qtjambi-qml`and `qtjambi-quick` allow you to devine user interfaces in QML language based upon `QtQuick`.
Further modules deal with concurrency, SQL and XML processing, statemachines, OpenGL, web content and multimedia integration, printsupport, visualization, connectivity, special hardware and many more.

## Availability

[![Maven Central](https://maven-badges.sml.io/sonatype-central/io.qtjambi/qtjambi/badge.svg)](https://central.sonatype.com/artifact/io.qtjambi/qtjambi)
[![Release (latest by date)](https://img.shields.io/github/v/release/OmixVisualization/qtjambi)](https://github.com/OmixVisualization/qtjambi/releases)

QtJambi is available for Java 11 and higher using Qt6 in Java on Windows, Android Linux and macOS. 
Most Qt modules are available as QtJambi module as [listed here](https://www.qtjambi.io/doc/Modules.md). All modules are published as Maven Artifact.
By default, the native components of only essential modules are made available on Maven. Please make a request via **[Issues](../../issues)** if you require native components not yet available.
Depending on the used Qt version you need the corresponding [QtJambi version](https://www.qtjambi.io/doc/Versions.md).

## Support

QtJambi is an open source project making the power of Qt available for your Java projects free of charge. However, this project is causing ongoing costs. 
Please consider supporting this project by donation. Your support is highly appreciated.

[Sponsor this project](https://github.com/sponsors/OmixVisualization)

## History

QtJambi for Qt4 and has been originally developed by Trolltech, the predecessor of The Qt Company. 
The successive open source project provides bindings of Qt5 and Qt6 releases for latest Java JDK versions.

The original idea for QtJambi was and still is to generated the the Java bindings by inspection of the C++ header files of the Qt implementation.
The necessary JNI (Java Native Interface) code and `.java` files are automatically generated. The QtJambi kernel cares for object lifecycle and native memory management.

## Further Reading

* [QtJambi Documentation](https://www.qtjambi.io/doc)

## Licenses

```
            GNU LESSER GENERAL PUBLIC LICENSE

 QtJambi is Copyright (C) 1992-2009 Nokia. All rights reserved.
            Copyright (C) 2009-2025 Dr. Peter Droste, 
                                    Omix Visualization GmbH & Co. KG. 
                                    All rights reserved.

 You may use, distribute and copy QtJambi under the terms of 
 GNU Lesser General Public License version 2.1.
```

```
            GNU GENERAL PUBLIC LICENSE

 QtJambi is Copyright (C) 1992-2009 Nokia. All rights reserved.
            Copyright (C) 2009-2025 Dr. Peter Droste, 
                                    Omix Visualization GmbH & Co. KG. 
                                    All rights reserved.

 You may use, distribute and copy QtJambi under the terms of
 GNU Lesser General Public License version 3. That license references
 the General Public License version 3. Other
 portions of the QtJambi may be licensed directly under this license.
```

* [GNU Lesser General Public License version 2.1](LICENSE.LGPLv21)
* [General Public License version 3](LICENSE.GPLv3)
