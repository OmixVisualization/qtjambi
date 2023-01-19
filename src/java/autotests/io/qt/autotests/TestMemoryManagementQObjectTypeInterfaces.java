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
import io.qt.autotests.generated.InvalidatorQObjectType;
import io.qt.autotests.generated.QObjectType;
import io.qt.core.QFactoryInterface;
import io.qt.core.QRunnable;

public class TestMemoryManagementQObjectTypeInterfaces extends MemoryManagement {
    @Override
    protected void initialize() {
    }

    @Override
    protected void uninitialize() {
    }

    @Override
    protected QtObject createInstanceInJava() {
        QtObject o = new QObjectTypeInterface();
        accountingForObject(o);
        return o;
    }

    @Override
    protected QtObject createInstanceInNative() {
    	throw new UnsupportedOperationException();
    }
    
    @Override
    protected boolean isQObject() {
        return true;
    }
	
	@Override
	protected WeakReference<QtObject> createInstanceInJavaWithException() {
		throw new UnsupportedOperationException();
	}

    @Override
    protected boolean needsEventProcessing() {
        return true;
    }
    
    @Override
    protected boolean isSharedPointer() {
        return false;
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
        return "io::qt::autotests::QObjectTypeInterface";
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
    protected boolean hasInterfaces() {
    	return true;
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
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMemoryManagementQObjectTypeInterfaces.class.getName());
    }
}

class QObjectTypeInterface extends QObjectType implements QFactoryInterface, QRunnable{
	@Override
	public List<String> keys() {
		return Collections.emptyList();
	}

	@Override
	public void run() {
	}
}