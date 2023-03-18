/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

class QQuickTest___{
    @QtUninvokable
    public static int quickTestMainWithSetup(String[] args, String name, java.util.function.Supplier<io.qt.core.QObject> setupSupplier){
        io.qt.core.QObject setup = setupSupplier.get();
        return quickTestMainWithSetup(args, name, null, QtJambi_LibraryUtilities.internal.checkedNativeId(setup));
    }
    
    @QtUninvokable
    public static int quickTestMainWithSetup(String[] args, String name, Class<? extends io.qt.core.QObject> setupType)
    {
        io.qt.core.QObject setup;
        try {
            try {
                setup = setupType.getConstructor().newInstance();
            } catch (java.lang.reflect.InvocationTargetException e) {
                throw e.getTargetException();
            }
        } catch (RuntimeException | Error e) {
            throw e;
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
        return quickTestMainWithSetup(args, name, null, QtJambi_LibraryUtilities.internal.checkedNativeId(setup));
    }
    
    @QtUninvokable
    public static int quickTestMainWithSetup(String[] args, String name, io.qt.core.QObject setup){
        return quickTestMainWithSetup(args, name, null, QtJambi_LibraryUtilities.internal.checkedNativeId(setup));
    }
    
    @QtUninvokable
    public static int quickTestMainWithSetup(String[] args, String name, String sourceDir, java.util.function.Supplier<io.qt.core.QObject> setupSupplier){
        io.qt.core.QObject setup = setupSupplier.get();
        return quickTestMainWithSetup(args, name, sourceDir, QtJambi_LibraryUtilities.internal.checkedNativeId(setup));
    }
    
    @QtUninvokable
    public static int quickTestMainWithSetup(String[] args, String name, String sourceDir, Class<? extends io.qt.core.QObject> setupType)
    {
        io.qt.core.QObject setup;
        try {
            try {
                setup = setupType.getConstructor().newInstance();
            } catch (java.lang.reflect.InvocationTargetException e) {
                throw e.getTargetException();
            }
        } catch (RuntimeException | Error e) {
            throw e;
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
        return quickTestMainWithSetup(args, name, sourceDir, QtJambi_LibraryUtilities.internal.checkedNativeId(setup));
    }
    
    @QtUninvokable
    public static int quickTestMainWithSetup(String[] args, String name, String sourceDir, io.qt.core.QObject setup){
        return quickTestMainWithSetup(args, name, sourceDir, QtJambi_LibraryUtilities.internal.checkedNativeId(setup));
    }
    
    @QtUninvokable
    public static int quickTestMain(String[] args, String name, String sourceDir){
        return quickTestMainWithSetup(args, name, sourceDir, 0);
    }
    
    @QtUninvokable
    public static int quickTestMain(String[] args, String name){
        return quickTestMainWithSetup(args, name, null, 0);
    }
    
    @QtUninvokable
    private static native int quickTestMainWithSetup(String[] args, String name, String sourceDir, long setupId);
}// class
