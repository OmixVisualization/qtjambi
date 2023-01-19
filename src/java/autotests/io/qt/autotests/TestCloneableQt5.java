/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.QHash_int;
import io.qt.autotests.generated.QList_int;
import io.qt.autotests.generated.QMap_int;
import io.qt.autotests.generated.QQueue_int;
import io.qt.autotests.generated.QSet_int;
import io.qt.autotests.generated.QStack_int;
import io.qt.autotests.generated.QVector_int;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QHash;
import io.qt.core.QLibraryInfo;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QPoint;
import io.qt.core.QPointF;
import io.qt.core.QQueue;
import io.qt.core.QVector;
import io.qt.gui.QColor;
import io.qt.gui.QLinearGradient;
import io.qt.gui.QTouchEvent;
import io.qt.core.QRegExp;

public class TestCloneableQt5 extends ApplicationInitializer {
    @Test
    public void run_clone_QRegExp() {
        QRegExp org = new QRegExp();
        QRegExp clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QRegExp clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

	@Test
	public void run_clone_QTouchEvent_TouchPoint() {
		QTouchEvent.TouchPoint org = new QTouchEvent.TouchPoint();
		org.setPos(new QPointF(5,6));
		QTouchEvent.TouchPoint clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTouchEvent.TouchPoint clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.pos().x(), 5.);
		assertEquals((Object)clone.pos().y(), 6.);
		assertEquals(clone.pos(), clone2.pos());
	}

	
}
