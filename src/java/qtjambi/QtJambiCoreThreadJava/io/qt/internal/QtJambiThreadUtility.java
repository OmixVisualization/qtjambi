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
package io.qt.internal;

import io.qt.core.QObject;
import io.qt.core.QThread;

/**
 * This utility class allows source code compatibility for test cases between two different QThread implementations
 * either as QObject subclass or as java.lang.Thread subclass. The latter implies a patched Qt library to be used.
 */
public class QtJambiThreadUtility {
    public static QThread.Wrapper currentThreadObject() {
        return QThread.currentQThread();
    }
    
    public static Thread javaThread(QThread thread) {
        return thread;
    }
    
    public static QObject.PrivateSignal0 currentThreadStartedSignal(){
        return currentThreadObject().started;
    }
    
    public static QObject.PrivateSignal0 currentThreadFinishedSignal(){
        return currentThreadObject().finished;
    }
}
