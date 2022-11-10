package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;
import io.qt.qml.util.QmlExtended;
import io.qt.qml.util.QmlForeign;
import io.qt.qml.util.QmlValueType;

@QtAsGadget
@QmlValueType(name="pointf")
@QmlForeign(PointF.class)
@QmlExtended(PointFInterface.class)
public class PointFForeignExtendedByJavaMirror{
}
