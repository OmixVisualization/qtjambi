package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;
import io.qt.core.QList;
import io.qt.qml.util.QmlAnonymous;
import io.qt.qml.util.QmlForeign;
import io.qt.qml.util.QmlSequencialContainer;
import io.qt.widgets.QLineEdit;

@QtAsGadget
@QmlSequencialContainer(QLineEdit.class)
@QmlForeign(QList.class)
@QmlAnonymous
public class LineEditContainerForeign {
}
