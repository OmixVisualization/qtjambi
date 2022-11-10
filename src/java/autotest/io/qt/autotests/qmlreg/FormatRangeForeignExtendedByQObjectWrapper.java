package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;
import io.qt.gui.QTextLayout;
import io.qt.qml.util.QmlExtended;
import io.qt.qml.util.QmlForeign;
import io.qt.qml.util.QmlValueType;

@QtAsGadget
@QmlValueType(name="formatrange")
@QmlForeign(QTextLayout.FormatRange.class)
@QmlExtended(FormatRangeQObjectExtension.class)
public class FormatRangeForeignExtendedByQObjectWrapper{
}
