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
import java.util.Collections;
import java.util.List;

import org.junit.Test;

import io.qt.QtObject;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.InvalidatorPolymorphicOwnedObjectType;
import io.qt.autotests.generated.PolymorphicOwnedObjectType;
import io.qt.core.QFactoryInterface;
import io.qt.core.QRunnable;

public class TestMemoryManagementPolymorphicObjectInterfaces extends MemoryManagement {
    @Override
    protected void initialize() {
    }

    @Override
    protected void uninitialize() {
    }

    protected QtObject createInstanceInJava() {
        QtObject o = new PolymorphicOwnedObjectTypeInterface();
        accountingForObject(o);
        return o;
    }

    protected QtObject createInstanceInNative() {
    	throw new UnsupportedOperationException();
    }
	
	@Override
	protected WeakReference<QtObject> createInstanceInJavaWithException() {
		throw new UnsupportedOperationException();
	}

    protected void deleteLastInstance() {
        PolymorphicOwnedObjectType.deleteLastInstance();
    }
    
    QtObject temporaryObject = null;
    protected QtObject invalidateObject(QtObject obj, final boolean returnReference) {
        new InvalidatorPolymorphicOwnedObjectType() {
            @Override
            public void overrideMe(PolymorphicOwnedObjectType t) {
                if (returnReference) {
                    temporaryObject = t;
                    General.internalAccess.setJavaOwnership(temporaryObject);
                }
            }
        }.invalidateObject((PolymorphicOwnedObjectType) obj);

        QtObject tmp = temporaryObject;
        temporaryObject = null;
        return tmp;
    }

    protected String className() {
        return "PolymorphicOwnedObjectType";
    }

    protected boolean hasShellDestructor() {
        return true;
    }

    @Override
    protected boolean hasVirtualDestructor() {
        return true;
    }
    
    @Override
    protected boolean hasInterfaces() {
    	return true;
	}
    
    @Override
    protected boolean isSharedPointer() {
        return false;
    }
    
    @Test
    @Override
    public void nativeDelete_CreatedInJava_CppOwnership() {
    	super.nativeDelete_CreatedInJava_CppOwnership();
    }
    
    @Override
    @Test
    public void invalidate_CreatedInJava_DefaultOwnership() {
    	super.invalidate_CreatedInJava_DefaultOwnership();
    }
    
    @Override
	protected boolean needsEventProcessing() {
		return true;
	}
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMemoryManagementPolymorphicObjectInterfaces.class.getName());
    }
}

class PolymorphicOwnedObjectTypeInterface extends PolymorphicOwnedObjectType implements QFactoryInterface, QRunnable{
	@Override
	public List<String> keys() {
		return Collections.emptyList();
	}

	@Override
	public void run() {
	}
}