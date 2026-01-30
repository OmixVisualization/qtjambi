/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assume.*;

import org.junit.*;

import io.qt.*;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.qml.*;
import io.qt.quick.*;
import io.qt.webchannelquick.*;
import io.qt.webengine.core.*;
import io.qt.webengine.quick.*;
import io.qt.widgets.*;

public class TestWebChannelOnEngine extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
        if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS)
    			&& "x86_64".equals(System.getProperty("os.arch").toLowerCase())
    			&& QLibraryInfo.version().compareTo(new QVersionNumber(6,5,4))>=0) {
    		Assert.assertFalse("env SYSTEM_VERSION_COMPAT=0 required to run WebEngine on macOS x86_64", "10.16".equals(General.stringSysctlByName("kern.osproductversion")));
    	}
        Assume.assumeFalse("Cannot run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
        Assume.assumeFalse("Cannot run on Windows MINGW", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows) && !QLibraryInfo.build().contains("MSVC"));
//        QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.OpenGLRhi);
    	QtUtilities.initializePackage("io.qt.webengine.widgets");
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
        ApplicationInitializer.testInitializeWithWidgets();
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	boolean found = false;
    	try {
			Class<?> cls = Class.forName("io.qt.webengine.widgets.QWebEngineView");
			if (cls != null)
				found = true;
		} catch (ClassNotFoundException e) {
		}
    	assumeTrue("QWebEngineView not available.", found);
    	QtWebEngineQuick.initialize();
    	assumeTrue("global share context not available.", QOpenGLContext.globalShareContext()!=null);
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
    }
    
    static class WebApp extends QObject {
    	@QtInvokable
    	void sendToObject(String msg) {
    		System.out.println("sendToObject("+msg+")");
    	}
    	
    	public final Signal0 optionChanged = new Signal0();
    	private final QJsonObject option = new QJsonObject();
    	{
    		option.set("color", "red");
    	}
    	
    	@QtInvokable
    	public void changeColor(String color) {
    		option.set("color", color);
    		optionChanged.emit();
    	}

		public QJsonObject getOption() {
			return option.clone();
		}

		public void setOption(QJsonObject option) {
			this.option.assign(option);
			optionChanged.emit();
		}
    };

    @Test
    public void test() {
        QQmlWebChannel.staticMetaObject.hashCode();
        QQuickWindow.staticMetaObject.hashCode();
    	QFile apiFile = new QFile(":/qtwebchannel/qwebchannel.js");
    	String script = "";
    	if(apiFile.open(QIODeviceBase.OpenModeFlag.ReadOnly)) {
    		script += apiFile.readAll();
    		apiFile.close();
    	}
    	QQmlApplicationEngine engine = new QQmlApplicationEngine();
    	QQuickItem.staticMetaObject.hashCode();
    	engine.loadData(new QByteArray("import QtQuick;import QtQuick.Window;import QtWebEngine\nWindow{"
    			+ "Row{anchors.fill: parent;"
    			+ "WebEngineView{id: devtools;width: parent.width / 2;height: parent.height}"
    			+ "WebEngineView{objectName: 'QQuickWebEngineView';devToolsView: devtools;width: parent.width / 2;height: parent.height;"
    			+ "function doRunJavaScript(code,consumer){"
    			+ "runJavaScript(code, value => consumer.accept(value));"
    			+ "}"
    			+ "}"
    			+ "}"
    			+ "}"));
    	QList<QObject> rootObjects = engine.rootObjects();
        Assert.assertTrue(rootObjects.get(0)!=null);
        QQuickWindow window = (QQuickWindow)rootObjects.get(0);
        QQuickItem webEngineView = window.findChild(QQuickItem.class, "QQuickWebEngineView");
        QWebEngineSettings settings = QtWebEngineQuick.toWebEngineSettings((QObject)webEngineView.property("settings"));
        settings.setAttribute(QWebEngineSettings.WebAttribute.JavascriptEnabled, true);
        settings.setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineScriptCollection webChannelScriptCollection = QtWebEngineQuick.toWebEngineScriptCollection((QObject)webEngineView.property("userScripts"));
    	QWebEngineScript webChannelScript = new QWebEngineScript();
    	script += "\n"
    			+ ";\n"
    			+ "if (typeof qt != 'undefined') { new QWebChannel(qt.webChannelTransport, function(channel) {\n"
    			+ "    window.$object = channel.objects.$object;\n"
    			+ "});}";
    	webChannelScript.setSourceCode(script);
    	webChannelScript.setRunsOnSubFrames(true);
    	webChannelScript.setName("webapp");
    	webChannelScript.setWorldId(QWebEngineScript.ScriptWorldId.MainWorld.value());
    	webChannelScript.setInjectionPoint(QWebEngineScript.InjectionPoint.DocumentCreation);
    	webChannelScriptCollection.insert(webChannelScript);
    	
        QQmlWebChannel webChannel = (QQmlWebChannel)webEngineView.property("webChannel");
        webChannel.setPropertyUpdateInterval(-1);
        WebApp object = new WebApp();
        webChannel.registerObject("$object", object);
    	Object received[] = {null};
    	AssertionError errors[] = {null};
        QObject consumer = new QObject() {
			@QtInvokable
			public void accept(QJSValue value) {
				received[0] = value.toVariant();
			}
        };
    	QMetaMethod mtd = webEngineView.metaObject().method("doRunJavaScript", Object.class, Object.class);
        QMetaObject.findSignal(webEngineView, "loadingChanged", QWebEngineLoadingInfo.class).connect(info->{
        	switch(info.getStatus()) {
			case LoadFailedStatus:
				QApplication.quit();
				break;
			case LoadSucceededStatus:
			{
        		mtd.invoke(webEngineView, "test()", consumer);
        		QApplication.processEvents();
        		QTimer.singleShot(500, ()->{
        			try {
						Assert.assertEquals("red", received[0]);
					} catch (AssertionError e) {
						errors[0] = e;
						QApplication.quit();
					}
        			if(errors[0]==null) {
        				object.changeColor("blue");
        				mtd.invoke(webEngineView, "test()", consumer);
        				QTimer.singleShot(500, QApplication::quit);
        			}
        		});
        	}
				break;
			default:
				break;
        	
        	}
        });
    	webEngineView.setProperty("url", new QUrl("qrc:io/qt/autotests/webchanneltest.html"));
        window.show();
        QTimer.singleShot(20000, QApplication::quit);
        QApplication.exec();
        window.dispose();
        engine.dispose();
        if(errors[0]==null) {
        	Assert.assertEquals("blue", received[0]);
        }
        if(errors[0]!=null)
        	throw errors[0];
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebChannelOnEngine.class.getName());
    }
}
