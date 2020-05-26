/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

import QtQuick 2.0
Rectangle {
    id: item
    property int testProperty: 0
    signal testIntSignal(int i)
    signal testBoolSignal(bool i)
    signal testRealSignal(real r)
    signal testPointSignal(point p)
    signal testVarSignal(var v)
    signal testRectangleSignal(Rectangle i)
    signal testVar2Signal(var v)
    signal testStringSignal(string strg)
    signal testFunctionCall(var arg)
    function testFunction(arg) {
        testFunctionCall(arg);
    }
    onTestPropertyChanged: {
        testIntSignal(testProperty);
        testBoolSignal(testProperty>0);
        testRealSignal(testProperty);
        testPointSignal(Qt.point(testProperty,testProperty));
        testStringSignal("string "+testProperty);
        testVarSignal(Qt.rect(0, 0, testProperty, testProperty));
    }
}
