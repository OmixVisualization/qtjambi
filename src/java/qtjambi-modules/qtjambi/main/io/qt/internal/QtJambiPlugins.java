/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.lang.invoke.MethodHandle;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.function.Supplier;
import java.util.logging.Logger;

import io.qt.NativeAccess;
import io.qt.QtObjectInterface;
import io.qt.core.QByteArray;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonDocument.FromJsonResult;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonParseError.ParseError;
import io.qt.core.QObject;
import io.qt.core.internal.QtPluginMetaData;

public final class QtJambiPlugins {
	private QtJambiPlugins() {throw new RuntimeException();}
	
	private static class PluginClassLoader extends URLClassLoader {
		public PluginClassLoader() {
			super(new URL[0]);
		}

		protected void addURLs(Collection<URL> urls) {
			for (URL url : urls) {
				super.addURL(url);
				QtJambiResources.addSearchPath(url.toString());
			}
		}
	}
	private static final PluginClassLoader pluginClassLoader = new PluginClassLoader();

	public static native void qRegisterPluginInterface(Class<? extends QtObjectInterface> iface);

	public static Class<? extends QtObjectInterface> qRegisteredPluginInterface(QByteArray iid) {
		return qRegisteredPluginInterface(QtJambiInternal.internalAccess.nativeId(iid));
	}

	public static native Class<? extends QtObjectInterface> qRegisteredPluginInterface(long iid);

	public static void qRegisterStaticPluginFunction(QObject instance, QJsonObject metaData) {
		if (metaData == null) {
			metaData = loadMetaDataFromClass(instance.getClass());
		}
		qRegisterStaticPluginFunctionInstance(QtJambiInternal.internalAccess.nativeId(instance), QtJambiInternal.internalAccess.nativeId(metaData));
	}

	public static void qRegisterStaticPluginFunction(Class<? extends QObject> pluginClass, QJsonObject metaData) {
		try {
			if (metaData == null) {
				metaData = loadMetaDataFromClass(pluginClass);
			}
			qRegisterStaticPluginFunctionClass(pluginClass, QtJambiInternal.internalAccess.nativeId(metaData));
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Exception e) {
			throw new RuntimeException("Unable to register plugin " + pluginClass.getName(), e);
		}
	}

	private static native void qRegisterStaticPluginFunctionClass(Class<? extends QObject> pluginClass, long metaData);

	private static native void qRegisterStaticPluginFunctionInstance(long instance, long metaData);

	private static native void qRegisterStaticPluginFunction(Supplier<Class<? extends QObject>> classSupplier,
			String className, String iid, long metaData, long pluginInfo);

	private static QJsonObject loadMetaDataFromClass(Class<? extends QObject> pluginClass) {
		QtPluginMetaData pluginMetaData = pluginClass.getAnnotation(QtPluginMetaData.class);
		if (pluginMetaData != null) {
			if (!pluginMetaData.file().isEmpty()) {
				QFile pluginMetaDataFile = new QFile("classpath:" + pluginClass.getPackage().getName().replace('.', '/')
						+ "/" + pluginMetaData.file());
				if (pluginMetaDataFile.exists() && pluginMetaDataFile.open(QIODevice.OpenModeFlag.ReadOnly)) {
					FromJsonResult result = QJsonDocument.fromJson(pluginMetaDataFile.readAll());
					if (result.document != null)
						return result.document.object();
				} else {
					try {
						InputStream stream = pluginClass.getClassLoader().getResourceAsStream(
								pluginClass.getPackage().getName().replace('.', '/') + "/" + pluginMetaData.file());
						if (stream != null) {
							ByteArrayOutputStream bas = new ByteArrayOutputStream();
							try {
								byte[] buffer = new byte[1024];
								int length = stream.read(buffer);
								while (length > 0) {
									bas.write(buffer, 0, length);
									length = stream.read(buffer);
								}
							} finally {
								stream.close();
							}
							FromJsonResult result = QJsonDocument.fromJson(new QByteArray(bas.toByteArray()));
							if (result.error == null || result.error.error() == ParseError.NoError)
								return result.document.object();
							else
								throw new IOException(result.error.errorString());
						}
					} catch (IOException t) {
						Logger.getLogger("internal").throwing(QtJambiInternal.class.getName(), "qRegisterPlugin", t);
					}
				}
			} else if (!pluginMetaData.json().isEmpty()) {
				FromJsonResult result = QJsonDocument.fromJson(new QByteArray(pluginMetaData.json()));
				if (result.document != null)
					return result.document.object();
			}
		}
		return new QJsonObject();
	}

	@NativeAccess
	private static QObject loadPluginInstance(String libPath, String className, String pluginName) throws Throwable {
		QFileInfo libFile = new QFileInfo(QDir.fromNativeSeparators(libPath));
		if (libFile.exists()) {
			List<URL> urls = new ArrayList<URL>();
			QDir dir = libFile.dir();
			if (dir.exists(pluginName + ".jar")) {
				QFileInfo jarFile = new QFileInfo(dir.filePath(pluginName + ".jar"));
				if (jarFile.isFile()) {
					urls.add(new File(QDir.toNativeSeparators(jarFile.absoluteFilePath())).toURI().toURL());
				}
			} else {
				QDir subdir = new QDir(dir.filePath(pluginName));
				if (subdir.exists()) {
					for (String jar : subdir.entryList(Arrays.asList("*.jar"), QDir.Filter.Files)) {
						urls.add(new File(QDir.toNativeSeparators(subdir.absoluteFilePath(jar))).toURI().toURL());
					}
					if (!urls.isEmpty()) {
						System.setProperty("java.library.path", System.getProperty("java.library.path", "")
								+ File.pathSeparatorChar + QDir.toNativeSeparators(subdir.absolutePath()));
					}
				}
			}
			if (!urls.isEmpty()) {
				pluginClassLoader.addURLs(urls);
				Class<?> foundClass = pluginClassLoader.loadClass(className);
				MethodHandle constructor = QtJambiInternal.privateLookup(foundClass)
						.unreflectConstructor(foundClass.getDeclaredConstructor());
				QObject result = (QObject) constructor.invoke();
				QtJambiInternal.internalAccess.setCppOwnership(result);
				return result;
			}
		}
		return null;
	}

	public static native String getInterfaceIID(Class<?> cls);
}
