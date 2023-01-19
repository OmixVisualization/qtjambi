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

import java.lang.ref.WeakReference;

import org.junit.Test;

import io.qt.QtObject;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.InvalidatorQObjectType;
import io.qt.autotests.generated.QObjectType;
import io.qt.core.QMetaObject;

public class TestMemoryManagementQObjectType extends MemoryManagement {
    @Override
    protected void initialize() {
    }

    @Override
    protected void uninitialize() {
    }

    @Override
    protected QtObject createInstanceInJava() {
        QtObject o = new QObjectType();
        accountingForObject(o);
        return o;
    }

    @Override
    protected QtObject createInstanceInNative() {
        QtObject o = QObjectType.newInstance();
        accountingForObject(o);
        return o;
    }
    
    @Override
    protected boolean isQObject() {
        return true;
    }

    @Override
    protected boolean needsEventProcessing() {
        return true;
    }

    @Override
    protected void deleteLastInstance() {
        QObjectType.deleteLastInstance();
    }

    QtObject temporaryObject = null;
    @Override
    protected QtObject invalidateObject(QtObject obj, final boolean returnReference) {
        new InvalidatorQObjectType() {
            @Override
            public void overrideMe(QObjectType t) {
                if (returnReference) {
                    temporaryObject = t;
                    General.internalAccess.setJavaOwnership(temporaryObject);
                }
            }
        }.invalidateObject((QObjectType) obj);

        QtObject tmp = temporaryObject;
        temporaryObject = null;
        return tmp;
    }

    @Override
    protected String className() {
        return "QObjectType";
    }

    @Override
    protected boolean hasShellDestructor() {
        return true;
    }

    @Override
    protected boolean hasVirtualDestructor() {
        return true;
    }
    
    @Override
    protected boolean isSharedPointer() {
        return false;
    }

    @Override
    @Test
    public void dispose_NotCreatedInJava_CppOwnership() {
    	super.dispose_NotCreatedInJava_CppOwnership();
    }
    
    @Override
    @Test
    public void finalize_NotCreatedInJava_JavaOwnership() {
    	super.finalize_NotCreatedInJava_JavaOwnership();
    }

    @Override
    @Test
    public void dispose_NotCreatedInJava_JavaOwnership() {
    	super.dispose_NotCreatedInJava_JavaOwnership();
    }
    
    @Override
    @Test
    public void invalidate_NotCreatedInJava_JavaOwnership() {
    	super.invalidate_NotCreatedInJava_JavaOwnership();
    }
    
    @Override
    @Test
    public void invalidate_CreatedInJava_DefaultOwnership() {
    	super.invalidate_CreatedInJava_DefaultOwnership();
    }
    
    @Override
    @Test
    public void nativeDelete_NotCreatedInJava_CppOwnership() {
    	super.nativeDelete_NotCreatedInJava_CppOwnership();
    }
    
    @Test
    @Override
    public void nativeDelete_CreatedInJava_CppOwnership() {
    	super.nativeDelete_CreatedInJava_CppOwnership();
    }
    
    @Test
    @Override
    public void finalize_CreatedInJavaWithException_JavaOwnership() {
    	super.finalize_CreatedInJavaWithException_JavaOwnership();
    }
    
    class MyNonInstantiable extends QObjectType{
		MyNonInstantiable(WeakReference<QtObject>[] ref){
			ref[0] = new WeakReference<>(this);
			throw new InstantiationError();
		}
	}
    
    protected String className2() {
    	return QMetaObject.forType(MyNonInstantiable.class).className();
    }
    
    @Override
	protected WeakReference<QtObject> createInstanceInJavaWithException() {
		@SuppressWarnings("unchecked")
		WeakReference<QtObject>[] ref = new WeakReference[1];
		try {
			new MyNonInstantiable(ref);
		} catch (InstantiationError e) {
		}
		return ref[0];
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMemoryManagementQObjectType.class.getName());
    }
}
