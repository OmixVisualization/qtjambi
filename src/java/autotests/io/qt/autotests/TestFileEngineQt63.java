/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.util.Optional;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.core.QFile;
import io.qt.core.QFileDevice.FileError;
import io.qt.core.QFileDevice.Permissions;
import io.qt.core.QIODevice;
import io.qt.core.Qt;
import io.qt.core.internal.QAbstractFileEngine;
import io.qt.core.internal.QAbstractFileEngineHandler;
import io.qt.gui.QPixmap;

public class TestFileEngineQt63 extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void testCustomFileEngine() {
		byte[][] result = {null};
		QAbstractFileEngineHandler handler = new QAbstractFileEngineHandler() {
			@Override
			public @Nullable QAbstractFileEngine create(@NonNull String fileName) {
				if(fileName.startsWith("/#/")) {
					return new QAbstractFileEngine(){
						
						ByteArrayOutputStream stream;

						@Override
						public boolean close() {
							System.out.println("TestFileEngine.close()");
							if(stream!=null) {
								result[0] = stream.toByteArray();
								stream = null;
								return true;
							}
							return false;
						}

						@Override
						public boolean open(QIODevice.@NonNull OpenMode openMode,
								@NonNull Optional<? extends @NonNull Permissions> permissions) {
							System.out.println("TestFileEngine.open("+openMode+")");
							if(openMode.testFlags(QFile.OpenModeFlag.WriteOnly)) {
								stream = new ByteArrayOutputStream();
								setError(FileError.NoError, null);
								return true;
							}
							return false;
						}

						@Override
						public int write(@Nullable ByteBuffer data) {
							System.out.print("TestFileEngine.write("+data.remaining()+")");
							int counter = 0;
							while(data.hasRemaining()) {
								stream.write(data.get());
								++counter;
							}
							System.out.println(" -> "+counter);
							return counter;
						}

//						@Override
						public boolean flush() {
							return true;
						}
					};
				}
				return null;
			}
		};
		try {
			QPixmap pm = new QPixmap(12800, 12800);
			pm.fill(Qt.GlobalColor.red);
			Assert.assertTrue(pm.save("/#/test.png", "png"));
//			System.out.println(result[0].length);
		}finally {
			handler.dispose();
		}
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestFileEngineQt63.class.getName());
    }
}
