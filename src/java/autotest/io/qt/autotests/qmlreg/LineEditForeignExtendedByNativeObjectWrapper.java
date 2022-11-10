package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;
import io.qt.qml.util.*;
import io.qt.widgets.QLineEdit;

@QtAsGadget
@QmlForeign(QLineEdit.class)
@QmlNamedElement(name="QLineEdit")
@QmlExtended(LineEditQtObjectExtension.class)
public class LineEditForeignExtendedByNativeObjectWrapper{}