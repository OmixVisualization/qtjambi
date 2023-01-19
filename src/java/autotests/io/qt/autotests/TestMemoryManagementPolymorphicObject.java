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
import io.qt.autotests.generated.InvalidatorPolymorphicObjectType;
import io.qt.autotests.generated.PolymorphicObjectType;

public class TestMemoryManagementPolymorphicObject extends MemoryManagement {
    @Override
    protected void initialize() {
    }

    @Override
    protected void uninitialize() {
    }

    protected QtObject createInstanceInJava() {
        QtObject o = new PolymorphicObjectType();
        accountingForObject(o);
        return o;
    }

    protected QtObject createInstanceInNative() {
    	QtObject o = PolymorphicObjectType.newInstance();
        accountingForObject(o);
        return o;
    }

    protected void deleteLastInstance() {
        PolymorphicObjectType.deleteLastInstance();
    }
    
    QtObject temporaryObject = null;
    protected QtObject invalidateObject(QtObject obj, final boolean returnReference) {
        new InvalidatorPolymorphicObjectType() {
            @Override
            public void overrideMe(PolymorphicObjectType t) {
                if (returnReference) {
                    temporaryObject = t;
                    General.internalAccess.setJavaOwnership(temporaryObject);
                }
            }
        }.invalidateObject((PolymorphicObjectType) obj);

        QtObject tmp = temporaryObject;
        temporaryObject = null;
        return tmp;
    }

    protected String className() {
        return "PolymorphicObjectType";
    }

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
    
    @Test
    @Override
    public void dispose_NotCreatedInJava_SplitOwnership() {
    	super.dispose_NotCreatedInJava_SplitOwnership();
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
    public void invalidate_NotCreatedInJava_SplitOwnership() {
    	super.invalidate_NotCreatedInJava_SplitOwnership();
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
    public void finalize_NotCreatedInJava_SplitOwnership() {
    	super.finalize_NotCreatedInJava_SplitOwnership();
    }
    
    @Override
    @Test
    public void nativeDelete_NotCreatedInJava_SplitOwnership() {
    	super.nativeDelete_NotCreatedInJava_SplitOwnership();
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
    
    @Override
	protected boolean needsEventProcessing() {
		return true;
	}
    
    @Override
	protected WeakReference<QtObject> createInstanceInJavaWithException() {
		@SuppressWarnings("unchecked")
		WeakReference<QtObject>[] ref = new WeakReference[1];
		class MyNonInstantiable extends PolymorphicObjectType{
			MyNonInstantiable(){
				ref[0] = new WeakReference<>(this);
				throw new InstantiationError();
			}
		}
		try {
			new MyNonInstantiable();
		} catch (InstantiationError e) {
		}
		return ref[0];
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMemoryManagementPolymorphicObject.class.getName());
    }
}
