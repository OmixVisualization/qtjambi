/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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

package io.qt.tools.ant;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Properties;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.TreeMap;
import java.util.TreeSet;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;

import io.qt.tools.ant.FindCompiler.Compiler;
import io.qt.tools.ant.OSInfo.OS;

public class InitializeBuildTask extends AbstractInitializeTask {
	
	private static Properties targetProperties;

	private List<String> androidAbis = new ArrayList<>();
	private String qtVersion;
	private int qtMajorVersion;
	private int qtMinorVersion;
	private int qtPatchlevelVersion;
	private int qtJambiVersion;
	private boolean useQtFramework;
	private String qtVersionSource = "";
	private String versionSuffix; // beta4
	private String libInfix = "";
	private boolean isMinGW;

	private final List<String> generatorPreProcDefinesList = new ArrayList<>();
	private final List<String> generatorStaticLibsList = new ArrayList<>();
	private final Set<String> skippedModules = new HashSet<>();

	static class ModuleInfo {
		enum Headers{
			Public,
			Private,
			PrivateOptional
		};
		ModuleInfo(String preprocdef, String libraryName){
			this(preprocdef, libraryName, Headers.Public);
		}
		ModuleInfo(String preprocdef, String libraryName, Headers requiredHeaders) {
			this.preprocdef = preprocdef;
			this.libraryName = libraryName;
			this.requiredHeaders = requiredHeaders;
			this.isPureStaticLib = false;
		}
		ModuleInfo(String preprocdef, String libraryName, Headers requiredHeaders, boolean staticlib) {
			this.preprocdef = preprocdef;
			this.libraryName = libraryName;
			this.requiredHeaders = requiredHeaders;
			this.isPureStaticLib = staticlib;
		}

		final String preprocdef;
		final String libraryName;
		final Headers requiredHeaders;
		final boolean isPureStaticLib;
	}

	private Map<String, ModuleInfo> moduleInfos;
	Map<String, ModuleInfo> moduleInfos(){
		if(moduleInfos==null) {
			Map<String, ModuleInfo> _moduleInfos = new TreeMap<>();
			_moduleInfos.put("core", new ModuleInfo("QTJAMBI_NO_CORE", "QtCore", ModuleInfo.Headers.Private));
			_moduleInfos.put("gui", new ModuleInfo("QTJAMBI_NO_GUI", "QtGui", ModuleInfo.Headers.Private));
			_moduleInfos.put("opengl", new ModuleInfo("QTJAMBI_NO_OPENGL", "QtOpenGL"));
			_moduleInfos.put("widgets", new ModuleInfo("QTJAMBI_NO_WIDGETS", "QtWidgets", ModuleInfo.Headers.Private));
			_moduleInfos.put("qml", new ModuleInfo("QTJAMBI_NO_QML", "QtQml", ModuleInfo.Headers.Private));
			_moduleInfos.put("quick", new ModuleInfo("QTJAMBI_NO_QUICK", "QtQuick", ModuleInfo.Headers.Private));
			_moduleInfos.put("quicktest", new ModuleInfo("QTJAMBI_NO_QUICKTEST", "QtQuickTest"));
			_moduleInfos.put("quickcontrols2", new ModuleInfo("QTJAMBI_NO_QUICKCONTROLS2", "QtQuickControls2"));
			_moduleInfos.put("quickwidgets", new ModuleInfo("QTJAMBI_NO_QUICKWIDGETS", "QtQuickWidgets"));
			_moduleInfos.put("xml", new ModuleInfo("QTJAMBI_NO_XML", "QtXml"));
			_moduleInfos.put("network", new ModuleInfo("QTJAMBI_NO_NETWORK", "QtNetwork"));
			_moduleInfos.put("concurrent", new ModuleInfo("QTJAMBI_NO_CONCURRENT", "QtConcurrent"));
			_moduleInfos.put("sql", new ModuleInfo("QTJAMBI_NO_SQL", "QtSql"));
			_moduleInfos.put("printsupport", new ModuleInfo("QTJAMBI_NO_PRINTSUPPORT", "QtPrintSupport"));
			_moduleInfos.put("networkauth", new ModuleInfo("QTJAMBI_NO_NETWORKAUTH", "QtNetworkAuth"));
			_moduleInfos.put("xmlpatterns", new ModuleInfo("QTJAMBI_NO_XMLPATTERNS", "QtXmlPatterns"));
			_moduleInfos.put("virtualkeyboard", new ModuleInfo("QTJAMBI_NO_VIRTUAL_KEYBOARD", "QtVirtualKeyboard"));
			_moduleInfos.put("serialport", new ModuleInfo("QTJAMBI_NO_SERIALPORT", "QtSerialPort"));
			_moduleInfos.put("serialbus", new ModuleInfo("QTJAMBI_NO_SERIALBUS", "QtSerialBus"));
			_moduleInfos.put("sensors", new ModuleInfo("QTJAMBI_NO_SENSORS", "QtSensors", ModuleInfo.Headers.PrivateOptional));
			_moduleInfos.put("location", new ModuleInfo("QTJAMBI_NO_LOCATION", "QtLocation"));
			_moduleInfos.put("positioning", new ModuleInfo("QTJAMBI_NO_POSITIONING", "QtPositioning"));
			_moduleInfos.put("bluetooth", new ModuleInfo("QTJAMBI_NO_BLUETOOTH", "QtBluetooth"));
			_moduleInfos.put("script", new ModuleInfo("QTJAMBI_NO_SCRIPT", "QtScript"));
			_moduleInfos.put("scripttools", new ModuleInfo("QTJAMBI_NO_SCRIPTTOOLS", "QtScriptTools"));
			_moduleInfos.put("multimedia", new ModuleInfo("QTJAMBI_NO_MULTIMEDIA", "QtMultimedia"));
			_moduleInfos.put("svg", new ModuleInfo("QTJAMBI_NO_SVG", "QtSvg"));
			_moduleInfos.put("dbus", new ModuleInfo("QTJAMBI_NO_DBUS", "QtDBus", ModuleInfo.Headers.Private));
			_moduleInfos.put("test", new ModuleInfo("QTJAMBI_NO_TEST", "QtTest"));
			_moduleInfos.put("help", new ModuleInfo("QTJAMBI_NO_HELP", "QtHelp"));
			_moduleInfos.put("qt3dcore", new ModuleInfo("QTJAMBI_NO_QT3DCORE", "Qt3DCore"));
			_moduleInfos.put("qt3dextras", new ModuleInfo("QTJAMBI_NO_QT3DEXTRAS", "Qt3DExtras"));
			_moduleInfos.put("qt3drender", new ModuleInfo("QTJAMBI_NO_QT3DRENDER", "Qt3DRender"));
			_moduleInfos.put("qt3dinput", new ModuleInfo("QTJAMBI_NO_QT3DINPUT", "Qt3DInput"));
			_moduleInfos.put("qt3dquick", new ModuleInfo("QTJAMBI_NO_QT3DQUICK", "Qt3DQuick"));
			_moduleInfos.put("qt3dquickrender", new ModuleInfo("QTJAMBI_NO_QT3DQUICKRENDER", "Qt3DQuickRender"));
			_moduleInfos.put("qt3dquickscene2d", new ModuleInfo("QTJAMBI_NO_QT3DQUICKSCENE2D", "Qt3DQuickScene2D"));
			_moduleInfos.put("qt3dquickextras", new ModuleInfo("QTJAMBI_NO_QT3DQUICKEXTRAS", "Qt3DQuickExtras"));
			_moduleInfos.put("qt3dlogic", new ModuleInfo("QTJAMBI_NO_QT3DLOGIC", "Qt3DLogic"));
			_moduleInfos.put("qt3danimation", new ModuleInfo("QTJAMBI_NO_QT3DANIMATION", "Qt3DAnimation"));
			_moduleInfos.put("websockets", new ModuleInfo("QTJAMBI_NO_WEBSOCKETS", "QtWebSockets"));
			_moduleInfos.put("webchannel", new ModuleInfo("QTJAMBI_NO_WEBCHANNEL", "QtWebChannel"));
			_moduleInfos.put("webchannelquick", new ModuleInfo("QTJAMBI_NO_WEBCHANNELQUICK", "QtWebChannelQuick"));
			_moduleInfos.put("webenginecore", new ModuleInfo("QTJAMBI_NO_WEBENGINECORE", "QtWebEngineCore"));
			_moduleInfos.put("webenginewidgets", new ModuleInfo("QTJAMBI_NO_WEBENGINEWIDGETS", "QtWebEngineWidgets"));
			_moduleInfos.put("webview", new ModuleInfo("QTJAMBI_NO_WEBVIEW", "QtWebView"));
			if(qtMajorVersion==5) {
				_moduleInfos.put("uitools", new ModuleInfo("QTJAMBI_NO_UITOOLS", "QtUiTools", ModuleInfo.Headers.Public, true));
			}else {
				_moduleInfos.put("uitools", new ModuleInfo("QTJAMBI_NO_UITOOLS", "QtUiTools"));
			}
			_moduleInfos.put("bodymovin", new ModuleInfo("QTJAMBI_NO_LOTTIE", "QtBodymovin"));
			_moduleInfos.put("datavisualization", new ModuleInfo("QTJAMBI_NO_DATA_VISUALIZATION", "QtDataVisualization"));
			_moduleInfos.put("charts", new ModuleInfo("QTJAMBI_NO_CHARTS", "QtCharts"));
	//		modules.put("uiplugin",new ModuleInfo("QTJAMBI_NO_UIPLUGIN", "QtUiPlugin"));
			_moduleInfos.put("designer", new ModuleInfo("QTJAMBI_NO_DESIGNER", "QtDesigner"));
			_moduleInfos.put("ui4", new ModuleInfo("QTJAMBI_NO_UI4", "QtDesigner", ModuleInfo.Headers.Private));
			if(qtMajorVersion==5) {
				_moduleInfos.put("remoteobjects", new ModuleInfo("QTJAMBI_NO_REMOTEOBJECTS", "QtRemoteObjects", ModuleInfo.Headers.PrivateOptional));
			}else {
				_moduleInfos.put("remoteobjects", new ModuleInfo("QTJAMBI_NO_REMOTEOBJECTS", "QtRemoteObjects"));
			}
			_moduleInfos.put("gamepad", new ModuleInfo("QTJAMBI_NO_GAMEPAD", "QtGamepad"));
			_moduleInfos.put("scxml", new ModuleInfo("QTJAMBI_NO_SCXML", "QtScxml"));
			_moduleInfos.put("nfc", new ModuleInfo("QTJAMBI_NO_NFC", "QtNfc"));
			_moduleInfos.put("texttospeech", new ModuleInfo("QTJAMBI_NO_TEXTTOSPEECH", "QtTextToSpeech"));
			_moduleInfos.put("purchasing", new ModuleInfo("QTJAMBI_NO_PURCHASING", "QtPurchasing"));
			_moduleInfos.put("xcbqpa", new ModuleInfo(null, "QtXcbQpa"));
			_moduleInfos.put("pdf", new ModuleInfo("QTJAMBI_NO_PDF", "QtPdf"));
			_moduleInfos.put("pdfwidgets", new ModuleInfo("QTJAMBI_NO_PDFWIDGETS", "QtPdfWidgets"));
			_moduleInfos.put("activex", new ModuleInfo("QTJAMBI_NO_ACTIVEX", "QtAxBase", ModuleInfo.Headers.Public, true));
			_moduleInfos.put("multimediawidgets", new ModuleInfo("QTJAMBI_NO_MULTIMEDIAWIDGETS", "QtMultimediaWidgets"));
			if(qtMajorVersion==5) {
				_moduleInfos.put("winextras", new ModuleInfo("QTJAMBI_NO_WINEXTRAS", "QtWinExtras"));
				_moduleInfos.put("macextras", new ModuleInfo("QTJAMBI_NO_MACEXTRAS", "QtMacExtras"));
				_moduleInfos.put("x11extras", new ModuleInfo("QTJAMBI_NO_X11EXTRAS", "QtX11Extras"));
				_moduleInfos.put("webengine", new ModuleInfo("QTJAMBI_NO_WEBENGINE", "QtWebEngine"));
			}else if(qtMajorVersion>=6) {
				_moduleInfos.put("openglwidgets", new ModuleInfo("QTJAMBI_NO_OPENGLWIDGETS", "QtOpenGLWidgets"));
				_moduleInfos.put("svgwidgets", new ModuleInfo("QTJAMBI_NO_SVGWIDGETS", "QtSvgWidgets"));
				_moduleInfos.put("webenginequick", new ModuleInfo("QTJAMBI_NO_WEBENGINEQUICK", "QtWebEngineQuick", ModuleInfo.Headers.Private));
				_moduleInfos.put("quick3d", new ModuleInfo("QTJAMBI_NO_QUICK3D", "QtQuick3D"));
				_moduleInfos.put("statemachine", new ModuleInfo("QTJAMBI_NO_STATEMACHINE", "QtStateMachine"));
				if((qtMajorVersion==6 && qtMinorVersion>=4) || qtMajorVersion>6) {
					_moduleInfos.put("httpserver", new ModuleInfo("QTJAMBI_NO_HTTPSERVER", "QtHttpServer", ModuleInfo.Headers.Private));
					_moduleInfos.put("spatialaudio", new ModuleInfo("QTJAMBI_NO_SPATIALAUDIO", "QtSpatialAudio"));
				}
				if((qtMajorVersion==6 && qtMinorVersion>=5) || qtMajorVersion>6) {
//					up to now there is no easy way to make these modules java-able. They are actually available as raw-Java solution.
					_moduleInfos.put("grpc", new ModuleInfo("QTJAMBI_NO_GRPC", "QtGrpc"));
					_moduleInfos.put("protobuf", new ModuleInfo("QTJAMBI_NO_PROTOBUF", "QtProtobuf"));
					skippedModules.add("grpc");
					skippedModules.add("protobuf");
				}
				if((qtMajorVersion==6 && qtMinorVersion>=6) || qtMajorVersion>7) {
					_moduleInfos.put("graphs", new ModuleInfo("QTJAMBI_NO_GRAPHS", "QtGraphs"));
				}
				if((qtMajorVersion==6 && qtMinorVersion>=7) || qtMajorVersion>8) {
					_moduleInfos.put("qmlbuiltins", new ModuleInfo("QTJAMBI_NO_QMLBUILTINS", "QtQmlBuiltins", ModuleInfo.Headers.Public, true));
				}
			}
			moduleInfos = Collections.unmodifiableMap(_moduleInfos);
		}
		return moduleInfos;
	}
	
	static class QtContent {
		QtContent(String query, String dir) {
			this.query = query;
			this.dir = dir;
		}

		final String query;
		final String dir;
	}
	private final static Map<String,QtContent> qtContents;
	static{
		Map<String,QtContent> _qtContents = new TreeMap<>();
		_qtContents.put("qtjambi.qtdir", new QtContent("QT_INSTALL_PREFIX", null));
		_qtContents.put(Constants.BINDIR, new QtContent("QT_INSTALL_BINS", "bin"));
		_qtContents.put(Constants.TOOLS_BINDIR, new QtContent("QT_HOST_BINS", "bin"));
		_qtContents.put(Constants.TOOLS_LIBDIR, new QtContent("QT_HOST_LIBS", "lib"));
		_qtContents.put(Constants.LIBDIR, new QtContent("QT_INSTALL_LIBS", "lib"));
		_qtContents.put(Constants.LIBEXECDIR, new QtContent("QT_INSTALL_LIBEXECS", "libexec"));
		_qtContents.put(Constants.INCLUDEDIR, new QtContent("QT_INSTALL_HEADERS", "include"));
		_qtContents.put(Constants.PLUGINSDIR, new QtContent("QT_INSTALL_PLUGINS", "plugins"));
		_qtContents.put(Constants.DOCSDIR, new QtContent("QT_INSTALL_DOCS", null));
		_qtContents.put(Constants.QMLMODULESDIR, new QtContent("QT_INSTALL_QML", "qml"));
		_qtContents.put(Constants.TRANSLATIONSDIR, new QtContent("QT_INSTALL_TRANSLATIONS", "translations"));
		_qtContents.put(Constants.RESOURCESDIR, new QtContent("QT_INSTALL_DATA", "resources"));
		qtContents = Collections.unmodifiableMap(_qtContents);
	}

	// did somebody mention something about methods never being longer than 10
	// lines?
	@SuppressWarnings("unused")
	public void executeInitialize() throws BuildException {
		if(targetProperties!=null) {
			for(Object key : targetProperties.keySet()) {
				if(targetProperties.get(key)!=null) {
					propertyHelper.setProperty(key.toString(), targetProperties.get(key), true);
					getProject().log(this, "--> "+key+"=" + propertyHelper.getProperty(key.toString()), Project.MSG_INFO);
				}
			}
		}
		if(!"true".equals(AntUtil.getPropertyAsString(propertyHelper, "build.properties.loaded"))){
			Properties properties = new Properties();
			try (FileInputStream stream = new FileInputStream(new File("build.properties"))) {
				properties.load(stream);
			} catch (IOException e) {
				getProject().log("reading build.properties", e, Project.MSG_ERR);
			}
			for(Object key : properties.keySet()) {
				propertyHelper.setNewProperty(key.toString(), properties.get(key));
			}
		}
		String skip = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.skipped.modules");
		if (skip != null) {
			for (String s : skip.split(",")) {
				if("core".equals(s)
						|| "gui".equals(s)
						|| "widgets".equals(s))
					throw new BuildException("It's not possible to exclude module "+s+" from bying generated.");
				skippedModules.add(s.trim());
			}
		}

		String sourceValue;

		String sep = AntUtil.getPropertyAsString(propertyHelper, "sep");
		if (sep == null || sep.isEmpty()) {
			sep = File.separator;
			sourceValue = " (auto-detect)";
		} else {
			sourceValue = null;
		}
		mySetProperty(-1, "sep", sourceValue, sep, true);

		String psep = AntUtil.getPropertyAsString(propertyHelper, "psep");
		if (psep == null || psep.isEmpty()) {
			psep = File.pathSeparator;
			sourceValue = " (auto-detect)";
		} else {
			sourceValue = null;
		}
		mySetProperty(-1, "psep", sourceValue, psep, true);

		{
			String value = AntUtil.getPropertyAsString(propertyHelper, "jnicheck"); // used here
			if (value == null || value.isEmpty()) {
				value = System.getenv("JNICHECK");
				AntUtil.setProperty(propertyHelper, "jnicheck", Boolean.toString(value != null));
			}
			value = AntUtil.getPropertyAsString(propertyHelper, "test"); // used here
			if (value == null || value.isEmpty()) {
				value = AntUtil.getPropertyAsString(propertyHelper, "TEST");
			}
			if (value == null || value.isEmpty()) {
				value = System.getenv("TEST");
			}
			if (value != null && !value.isEmpty()) {
				AntUtil.setProperty(propertyHelper, "test", value);
				getProject().log(this, "test=" + value, Project.MSG_INFO);
			}
		}
		
		String s;
		FindCompiler finder = new FindCompiler(this.propertyHelper, this);
		Compiler compiler;
		{
			Compiler detectedCompiler = null;
			String compilerError = null;
			try {
				detectedCompiler = finder.decideCompiler();
			} catch (BuildException e) {
				compilerError = e.getMessage();
			}
			String compilerValue = AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER);
			if (compilerValue == null || compilerValue.isEmpty()) {
				sourceValue = " (available compilers: " + finder.getAvailableCompilers() + "; auto-detected)";
				compilerValue = (detectedCompiler==null ? "" : detectedCompiler.toString());
				compiler = detectedCompiler;
			} else {
				if ("help".equals(compilerValue) || "?".equals(compilerValue)) {
					Compiler[] values = Compiler.values();
					s = Arrays.toString(values);
					throw new BuildException(Constants.COMPILER + " valid values: " + s);
				}
				compiler = Compiler.resolve(compilerValue);
				sourceValue = " (available compilers: " + finder.getAvailableCompilers() + "; detected: "
						+ (detectedCompiler==null ? "none" : detectedCompiler) + ")";
			}
			isMinGW = compiler==Compiler.MinGW || compiler==Compiler.MinGW_W64;
			mySetProperty(-1, Constants.COMPILER, sourceValue, compilerValue, true); // report value
			if(compilerError!=null)
				mySetProperty(-1, "qtjambi.compiler.error", sourceValue, compilerError, true);
		}

		String qtQmake = AntUtil.getPropertyAsString(propertyHelper, "qmake");

		Map<String,String> qmakeQuery;
		if(qtQmake!=null && !qtQmake.isEmpty()) {
			sourceValue = " (from property:qmake)";
			mySetProperty(-1, Constants.QMAKE, sourceValue, new File(qtQmake).getName(), true);
			mySetProperty(-1, Constants.QMAKE_ABSPATH, sourceValue, new File(qtQmake).getAbsolutePath(), true);
			qmakeQuery = QMakeTask.query(this, qtQmake);
		}else {
			String QTDIR = AntUtil.getPropertyAsString(propertyHelper, "QTDIR"); // used here
			if (QTDIR == null || QTDIR.isEmpty()) {
				QTDIR = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
			}
			if (QTDIR == null || QTDIR.isEmpty()) {
				QTDIR = System.getenv("QTDIR");
			}
			if (QTDIR != null && !QTDIR.isEmpty()) {
				File qtdirDir = new File(QTDIR);
				if (qtdirDir.isDirectory()) {
					QTDIR = qtdirDir.getAbsolutePath();
					getProject().log(this, "QTDIR is set: " + prettyValue(QTDIR), Project.MSG_INFO);
				} else {
					QTDIR = null;
				}
			}
			if (QTDIR == null || QTDIR.isEmpty()) {
				String value = AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR);
				if (value != null && !value.isEmpty()) {
					File binDir = new File(value);
					if (binDir.isDirectory()) {
						QTDIR = binDir.getParent();
					} else {
						throw new BuildException("No QTDIR specified.");
					}
				} else {
					throw new BuildException("No QTDIR specified.");
				}
			}
			if (QTDIR != null && !QTDIR.isEmpty()) {
				AntUtil.setProperty(propertyHelper, "QTDIR", QTDIR);
				AntUtil.setProperty(propertyHelper, "qtjambi.qtdir", QTDIR);
				AntUtil.setProperty(propertyHelper, Constants.BINDIR, new File(QTDIR, "bin").getAbsolutePath());
			}
			sourceValue = null;
			qtQmake = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE);
			String detectedQmake = QMakeTask.executableName();
			if (qtQmake == null || qtQmake.isEmpty()) {
				qtQmake = detectedQmake;
				sourceValue = " (default)";
			} else if (!qtQmake.equals(detectedQmake)) {
				sourceValue = " (WARNING expected " + detectedQmake + ")";
			}
			mySetProperty(-1, Constants.QMAKE, sourceValue, qtQmake, true);

			String detectedQmakeAbsolutePath = QMakeTask.resolveExecutableAbsolutePath(getProject(), qtQmake);

			String qtQmakeAbspath = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE_ABSPATH);
			if (qtQmakeAbspath == null || qtQmakeAbspath.isEmpty()) {
				qtQmakeAbspath = detectedQmakeAbsolutePath;
				if (qtQmakeAbspath != null)
					sourceValue = " (auto-detected)";
				else
					sourceValue = " (WARNING not found)";
			} else {
				if (!qtQmakeAbspath.equals(detectedQmakeAbsolutePath))
					sourceValue = " (WARNING detected at " + detectedQmakeAbsolutePath + ")";
			}
			mySetProperty(-1, Constants.QMAKE_ABSPATH, sourceValue, qtQmakeAbspath, true);
			qmakeQuery = QMakeTask.query(this, qtQmakeAbspath);
		}
		sourceValue = " (from qmake -query)";
//		Constants.TOOLS_BINDIR
		if(AntUtil.getPropertyAsString(propertyHelper, "QTDIR")==null)
			AntUtil.setProperty(propertyHelper, "QTDIR", AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir"));
		if (!decideQtVersion(qmakeQuery))
			throw new BuildException("Unable to determine Qt version.");

		String qtVersionShort = String.format("%1$s.%2$s", qtMajorVersion, qtMinorVersion);
		InputStream inStream = null;
		try {
			inStream = new FileInputStream("releases.properties");
			Properties releases = new Properties();
			releases.load(inStream);
			String value = releases.getProperty(qtVersionShort, "0");
			try {
				qtJambiVersion = Integer.parseInt(value);
			} catch (NumberFormatException e) {
				qtJambiVersion = 0;
			}
		} catch (FileNotFoundException e) {
			// Acceptable failure
		} catch (IOException e) {
			throw new BuildException(e);
		} finally {
			if (inStream != null) {
				try {
					inStream.close();
				} catch (IOException e) {
					throw new BuildException(e);
				}
			}
		}
		sourceValue = " (auto-detected)";
		mySetProperty(-1, "qtjambi.patchversion", sourceValue, "" + qtJambiVersion, true); // report value
		String qtjambiFullVersion = qtVersionShort + "." + qtJambiVersion;
		mySetProperty(-1, "qtjambi.jar.version", sourceValue, qtjambiFullVersion, true); // report
																												// value
		String detectedOsname = OSInfo.crossOSArchName();
		String osname = AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME);
		if (osname == null || osname.isEmpty()) {
			sourceValue = " (auto-detected)";
			osname = detectedOsname;
			if("android".equals(detectedOsname)) {
				if(androidAbis.size()==1) {
					switch(androidAbis.get(0)) {
					case "arm64-v8a":
						osname += "-arm64";
						OSInfo.setCrossOSArchName(osname);
						break;
					case "armeabi-v7a":
						osname += "-arm";
						OSInfo.setCrossOSArchName(osname);
						break;
					case "x86":
						osname += "-x86";
						OSInfo.setCrossOSArchName(osname);
						break;
					case "x86_64":
						osname += "-x64";
						OSInfo.setCrossOSArchName(osname);
						break;
					}
				}else {
					String QTDIR = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
					if(QTDIR!=null) {
						String androidSpec = new File(QTDIR).getName();
						switch(androidSpec) {
						case "android_arm64_v8a":
							osname += "-arm64";
							OSInfo.setCrossOSArchName(osname);
							break;
						case "android_armv7":
							osname += "-arm";
							OSInfo.setCrossOSArchName(osname);
							break;
						case "android_x86":
							osname += "-x86";
							OSInfo.setCrossOSArchName(osname);
							break;
						case "android_x86_64":
							osname += "-x64";
							OSInfo.setCrossOSArchName(osname);
							break;
						}
					}else {
						OSInfo.setCrossOSArchName(null);
					}
				}
			}else {
				OSInfo.setCrossOSArchName(null);
			}
		} else {
			if ("help".equals(osname) || "?".equals(osname)) {
				OSInfo.OS[] values = OSInfo.OS.values();
				s = Arrays.toString(values);
				throw new BuildException(Constants.OSNAME + " valid values: " + s);
			}
			sourceValue = " (detected: " + detectedOsname + ")";
		}
		mySetProperty(-1, Constants.OSNAME, sourceValue, osname, true); // report value
		
		if(compiler!=null && compiler.name().endsWith("_arm64")) {
			qmakeQuery = new TreeMap<>(qmakeQuery);
			qmakeQuery.put("QMAKE_SPEC", qmakeQuery.get("QMAKE_XSPEC"));
			qmakeQuery.put("QT_HOST_BINS", qmakeQuery.get("QT_INSTALL_BINS"));
			qmakeQuery.put("QT_HOST_LIBS", qmakeQuery.get("QT_INSTALL_LIBS"));
			qmakeQuery.put("QT_HOST_LIBEXECS", qmakeQuery.get("QT_INSTALL_LIBEXECS"));
			qmakeQuery.put("QT_HOST_PREFIX", qmakeQuery.get("QT_INSTALL_PREFIX"));
			qmakeQuery = Collections.unmodifiableMap(qmakeQuery);
		}
		if (!decideQMAKE_XSPEC(qmakeQuery))
			throw new BuildException("Unable to determine QMAKE_XSPEC");
		for(Map.Entry<String, QtContent> entry : qtContents.entrySet()) {
			String value = AntUtil.getPropertyAsString(propertyHelper, entry.getKey());
			if(value==null || value.isEmpty()) {
				value = qmakeQuery.get(entry.getValue().query);
				switch(entry.getKey()) {
				case Constants.RESOURCESDIR:
					value += "/resources";
					break;
				}
				mySetProperty(-1, entry.getKey(), sourceValue, value, true);
			}
		}

		String vsredistdirValue = AntUtil.getPropertyAsString(propertyHelper, Constants.VSREDISTDIR);
		if (vsredistdirValue != null && !vsredistdirValue.isEmpty()) {
			sourceValue = null; // " (environment variable $" + "VSREDISTDIR" + ")"; // not strictly true it
								// could be set by ant cmdline
			mySetProperty(-1, Constants.VSREDISTDIR_PACKAGE, sourceValue, vsredistdirValue, true); // report value

			String vsredistdirPackageValue = AntUtil.getPropertyAsString(propertyHelper, Constants.VSREDISTDIR_PACKAGE);
			if (vsredistdirPackageValue == null) {
				sourceValue = " (default value; feature need explicit enabling)";
				vsredistdirPackageValue = "false";
			} else {
				sourceValue = null;
			}
			mySetProperty(-1, Constants.VSREDISTDIR_PACKAGE, sourceValue, vsredistdirPackageValue, true); // report
																											// value
		} else {
			mySetProperty(0, Constants.VSREDISTDIR_PACKAGE, null, "false", true); // silently set false
		}

		String CROSS_COMPILE = System.getenv("CROSS_COMPILE"); // used here
		if (CROSS_COMPILE != null && !CROSS_COMPILE.isEmpty())
			getProject().log(this, "CROSS_COMPILE is set: " + prettyValue(CROSS_COMPILE), Project.MSG_INFO);

		s = null;
		switch (OSInfo.os()) {
		case Linux:
			s = OSInfo.K_LINUX;
			break;
		case Windows:
			s = OSInfo.K_WINDOWS;
			break;
		case Android:
			s = OSInfo.K_ANDROID;
			break;
		case MacOS:
			s = OSInfo.K_MACOS;
			mySetProperty(0, Constants.QTJAMBI_CONFIG_ISMACOSX, " (set by init)", "true", true);
			if (qtMajorVersion >= 6 && qtMinorVersion >= 2
					&& AntUtil.getProperty(propertyHelper, "qtjambi.pluginloader.build.from.source") == null) {
				AntUtil.setProperty(propertyHelper, "qtjambi.pluginloader.build.from.source", "true");
			}
			break;
		default:
			break;
		}
		if (s != null)
			AntUtil.setProperty(propertyHelper, Constants.OSPLATFORM, s);

		s = null;

		{
			Properties props = System.getProperties();
			Enumeration<?> e = props.propertyNames();
			while (e.hasMoreElements()) {
				String k = (String) e.nextElement();
				Object v = System.getProperty(k);
				getProject().log("systemProperty[" + k + "] = " + v, Project.MSG_VERBOSE);
			}
		}

		String javaOsArch = System.getProperty("os.arch"); // arm|
		if ("arm".equals(javaOsArch)) {
			// FIXME get LE or BE
			s = "arm";
		} else {
			if (osname.endsWith("64"))
				s = "x86_64";
			else
				s = "i386";
		}
		if (s != null)
			AntUtil.setProperty(propertyHelper, Constants.OSCPU, s);

		finder.checkCompilerDetails();

		mySetProperty(-1, Constants.EXEC_STRIP, null, null, true); // report value

		String javaHomeTarget = decideJavaHomeTarget();
		if (javaHomeTarget == null)
			throw new BuildException(
					"Unable to determine JAVA_HOME_TARGET, setup environment variable JAVA_HOME (or JAVA_HOME_TARGET) or edit build.properties");

		String javaOsarchTarget = decideJavaOsarchTarget();
		if (javaOsarchTarget == null) {
			if (OSInfo.os() != OSInfo.OS.MacOS) // On MacOSX there is no sub-dir inside the JDK include directory that
												// contains jni.h
				throw new BuildException(
						"Unable to determine JAVA_OSARCH_TARGET, setup environment variable JAVA_OSARCH_TARGET or edit build.properties");
		}

		String javaOscpu = decideJavaOscpu();
		if (javaOscpu == null)
			throw new BuildException(
					"Unable to determine JAVA_OSCPU, setup environment variable JAVA_OSCPU or edit build.properties");

		String javaOscpuTarget = decideJavaOscpuTarget();
		if (javaOscpuTarget == null)
			throw new BuildException(
					"Unable to determine JAVA_OSCPU_TARGET, setup environment variable JAVA_OSCPU_TARGET or edit build.properties");

		String generatorQtDir = AntUtil.getPropertyAsString(propertyHelper, "tools.qtdir");
		if (generatorQtDir != null && !generatorQtDir.isEmpty()) {
			final String[] emitB = { Constants.TOOLS_BINDIR, Constants.TOOLS_LIBDIR };
			for (String emit : emitB) {
				sourceValue = null;
				String value = AntUtil.getPropertyAsString(propertyHelper, emit);
				File dir = null;
				if (value != null && !value.isEmpty()) {
					dir = new File(value);
				} else if ((value == null || value.isEmpty()) && generatorQtDir != null) {
					dir = resolveDirectoryViaQTDIR(generatorQtDir, emit);
					if (dir != null) {
						value = dir.getAbsolutePath();
						sourceValue = " (from qtjambi.generator.qtdir)";
					}
				}
				if (dir != null) {
					if (dir.isDirectory() == false)
						sourceValue = " (WARNING: path does not exist or is not a directory)";
				}
				mySetProperty(-1, emit, sourceValue, value, true);
			}
		} else {
			{
				sourceValue = null;
				String toolsBindir = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_BINDIR);
				if (toolsBindir == null || toolsBindir.isEmpty()) {
					String bindir = AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR);
					if (bindir != null && !bindir.isEmpty()) {
						sourceValue = " (inherited from ${" + Constants.BINDIR + "})";
						toolsBindir = bindir;
					}
				}
				mySetProperty(-1, Constants.TOOLS_BINDIR, sourceValue, toolsBindir, true); // report value
			}
			{
				sourceValue = null;
				String toolsLibdir = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_LIBDIR);
				if (toolsLibdir == null || toolsLibdir.isEmpty()) {
					String libdir = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
					if (libdir != null && !libdir.isEmpty()) {
						sourceValue = " (inherited from ${" + Constants.LIBDIR + "})";
						toolsLibdir = libdir;
					}
				}
				mySetProperty(-1, Constants.TOOLS_LIBDIR, sourceValue, toolsLibdir, true); // report value
			}
		}
		{
			sourceValue = null;
			String toolsQmake = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKE);
			String toolsBindir = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_BINDIR);
			String binDirProp = Constants.TOOLS_BINDIR;
			if (toolsQmake == null || toolsQmake.isEmpty()) {
				if(toolsBindir==null) {
					binDirProp = Constants.BINDIR;
					toolsQmake = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE);
					sourceValue = " (inherited from ${" + Constants.QMAKE + "})";
				}else {
					sourceValue = " (auto-detected)";
					toolsQmake = QMakeTask.executableName();
				}
			}
			mySetProperty(-1, Constants.TOOLS_QMAKE, sourceValue, toolsQmake, true); // report value

			// This method needs to look in Constants.TOOLS_BINDIR not Constants.BINDIR

			String toolsQmakeAbspath = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKE_ABSPATH);
			if (toolsQmakeAbspath == null || toolsQmakeAbspath.isEmpty()) {
				toolsQmakeAbspath = QMakeTask.resolveExecutableAbsolutePath(getProject(), toolsQmake, binDirProp);
				if (toolsQmakeAbspath != null)
					sourceValue = " (auto-detected)";
				else
					sourceValue = " (WARNING not found)";
			} else {
				try {
					String detectedQmakeAbsolutePath = QMakeTask.resolveExecutableAbsolutePath(getProject(), toolsQmake, binDirProp);
					if (!toolsQmakeAbspath.equals(detectedQmakeAbsolutePath))
						sourceValue = " (WARNING detected at " + detectedQmakeAbsolutePath + ")";
				} catch (Throwable e) {
				}
			}
			mySetProperty(-1, Constants.TOOLS_QMAKE_ABSPATH, sourceValue, toolsQmakeAbspath, true);
		}

		List<String> privateModules = new ArrayList<>();
		privateModules.add("QtCore");
		privateModules.add("QtGui");
		privateModules.add("QtDesigner");
		privateModules.add("QtRemoteObjects");
		privateModules.add("QtQuick");
		{
			String pm = AntUtil.getPropertyAsString(propertyHelper, Constants.PRIVATE_MODULES);
			if(pm!=null)
				privateModules.addAll(Arrays.asList(pm.split(",")));
		}
		if (OSInfo.os() == OSInfo.OS.MacOS) {
			String wantedSdk = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_MACOSX_MAC_SDK);
			detectMacosxSdk(wantedSdk);
			String generatorIncludepaths = AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_GENERATE_FRAMEWORKS);
			if(generatorIncludepaths==null || generatorIncludepaths.isEmpty()) {
				AntUtil.setProperty(propertyHelper, Constants.MAC_OS_GENERATE_FRAMEWORKS, "true");
			}
		}
		{
			List<String> includePaths = new ArrayList<>();
			String generatorIncludepaths = AntUtil.getPropertyAsString(propertyHelper, Constants.GENERATOR_INCLUDEPATHS);
			if (OSInfo.os() == OSInfo.OS.MacOS && useQtFramework) {
				String libdir = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
				for (File frameworkDir : new File(libdir).listFiles()) {
					if (frameworkDir.isDirectory() && frameworkDir.getName().endsWith(".framework")) {
						String libName = frameworkDir.getName();
						libName = libName.substring(0, libName.length() - 10);
						File headers = new File(frameworkDir, "/Versions/" + qtMajorVersion + "/Headers");
						if (!headers.isDirectory())
							headers = new File(frameworkDir, "/Versions/A/Headers");
						if (!headers.isDirectory())
							headers = new File(frameworkDir, "/Versions/Current/Headers");
						if (!headers.isDirectory())
							headers = new File(frameworkDir, "/Headers");
						if (privateModules.contains(libName) && !includePaths.contains(headers.getAbsolutePath()
								+ "/" + qtMajorVersion + "." + qtMinorVersion + "." + qtPatchlevelVersion)) {
							includePaths.add(headers.getAbsolutePath() + "/" + qtMajorVersion + "." + qtMinorVersion
									+ "." + qtPatchlevelVersion);
						}
					}
				}
			} else {
				String includedir = AntUtil.getPropertyAsString(propertyHelper, Constants.INCLUDEDIR);
				for (String privateModule : privateModules) {
					if (!includePaths.contains(new File(new File(includedir),
							privateModule + "/" + qtMajorVersion + "." + qtMinorVersion + "." + qtPatchlevelVersion)
									.getAbsolutePath()))
						includePaths.add(new File(new File(includedir),
								privateModule + "/" + qtMajorVersion + "." + qtMinorVersion + "." + qtPatchlevelVersion)
										.getAbsolutePath());
				}
			}
			if (generatorIncludepaths != null) {
				for (String path : generatorIncludepaths
						.split("[;" + (File.pathSeparatorChar == ';' ? "" : File.pathSeparator) + "]")) {
					if (!includePaths.contains(path))
						includePaths.add(path);
				}
			}
			String path = propertyHelper.replaceProperties(null, "${basedir}/src/cpp", null);
			if (!includePaths.contains(path))
				includePaths.add(path);
			AntUtil.setProperty(propertyHelper, Constants.GENERATOR_INCLUDEPATHS,
					String.join(File.pathSeparator, includePaths));
			getProject().log(this,
					Constants.GENERATOR_INCLUDEPATHS + ": " + String.join(File.pathSeparator, includePaths),
					Project.MSG_INFO);
		}
		if (qtMajorVersion >= 5) {
			AntUtil.setProperty(propertyHelper, "qt.feature.QT_NO_STYLE_PLASTIQUE", "true");
		}

		s = String.valueOf(qtVersion);
		getProject().log(this, Constants.QT_VERSION + " is " + s + qtVersionSource, Project.MSG_VERBOSE);
		AntUtil.setProperty(propertyHelper, Constants.QT_VERSION, s);
		AntUtil.setProperty(propertyHelper, Constants.VERSION, s); // this won't overwrite existing value
		{
			String docs = AntUtil.getPropertyAsString(propertyHelper, Constants.DOCSDIR);
			if(docs==null) {
				String QTDIR = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
				File tryPath = new File(new File(new File(QTDIR).getParentFile().getParentFile(), "Docs"), "Qt-" + s);
				if(tryPath.isDirectory())
					mySetProperty(-1, Constants.DOCSDIR, " (auto-detected)", tryPath.getAbsolutePath(), true);
			}
		}

		s = String.valueOf(qtMajorVersion);
		getProject().log(this, Constants.QT_VERSION_MAJOR + " is " + s, Project.MSG_VERBOSE);
		AntUtil.setProperty(propertyHelper, Constants.QT_VERSION_MAJOR, s);
		AntUtil.setProperty(propertyHelper, Constants.QT_VERSION_MAJOR_NEXT, String.valueOf(qtMajorVersion + 1));
		AntUtil.setProperty(propertyHelper, Constants.QT_VERSION_MINOR, String.valueOf(qtMinorVersion));
		AntUtil.setProperty(propertyHelper, Constants.QT_VERSION_MINOR_NEXT, String.valueOf(qtMinorVersion + 1));
		AntUtil.setProperty(propertyHelper, Constants.QT_VERSION_PATCHLEVEL, String.valueOf(qtPatchlevelVersion));
		decideAlternativeJavaHomesTarget();

		versionSuffix = AntUtil.getPropertyAsString(propertyHelper, Constants.SUFFIX_VERSION);
		mySetProperty(-1, Constants.SUFFIX_VERSION, null, null, true); // report

		String canonVersionSuffix;
		if (versionSuffix != null)
			canonVersionSuffix = versionSuffix;
		else
			canonVersionSuffix = "";
		String bundleVersionMode = AntUtil.getPropertyAsString(propertyHelper, Constants.BUNDLE_VERSION_MODE);
		if (bundleVersionMode != null) {
			if (bundleVersionMode.equals("auto-suffix-date")) {
				SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
				s = qtVersion + "." + sdf.format(new Date()) + canonVersionSuffix;
			}
		} else {
			s = qtVersion + canonVersionSuffix;
		}
		mySetProperty(-1, Constants.BUNDLE_VERSION, null, s, true);
		analyzeLibinfix();

		{ // Need to detect the version of Qt we a reworking with before deciding
			// configuration
			String wantedConfiguration = AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIGURATION);
			detectConfiguration(wantedConfiguration); // all the magic is in here now
		}
		
		String force = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_FORCE_DEBUG_INFO);
		if(force==null || !"true".equalsIgnoreCase(force) || !"false".equalsIgnoreCase(force)) {
			switch (OSInfo.crossOS()) {
			case Android:
			case IOS:
				AntUtil.setProperty(propertyHelper, Constants.QTJAMBI_FORCE_DEBUG_INFO, "false");
				break;
			default:
				AntUtil.setProperty(propertyHelper, Constants.QTJAMBI_FORCE_DEBUG_INFO, "true");
				break;
			}
		}

		// Moved until after auto-detect of configuration is complete
		s = null;
		if (Constants.CONFIG_RELEASE.equals(getConfiguration()))
			s = ""; // empty
		else if (Constants.CONFIG_DEBUG.equals(getConfiguration()))
			s = "-debug";
		else if (Constants.CONFIG_TEST.equals(getConfiguration()))
			s = "-test";
		else
			s = ""; // empty (debug_and_release)
		if (s != null)
			AntUtil.setProperty(propertyHelper, Constants.CONFIGURATION_DASH, s);
		s = null;
		if (Constants.CONFIG_RELEASE.equals(getConfiguration()))
			s = ""; // empty
		else if (Constants.CONFIG_DEBUG.equals(getConfiguration()))
			s = ".debug";
		else if (Constants.CONFIG_TEST.equals(getConfiguration()))
			s = ".test";
		else
			s = ""; // empty (debug_and_release)
		if (s != null)
			AntUtil.setProperty(propertyHelper, Constants.CONFIGURATION_OSGI, s);

		{
			sourceValue = null;
			String value = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_DEBUG_TOOLS);
			if (value == null) {
				if (isConfigurationDebug() || isConfigurationTest())
					value = Boolean.TRUE.toString();
				else
					value = Boolean.FALSE.toString();
				sourceValue = " (auto-configured based on ${" + Constants.CONFIGURATION + "})";
			}
			mySetProperty(-1, Constants.QTJAMBI_DEBUG_TOOLS, sourceValue, value, true);
		}

		{
			sourceValue = null;
			String qmakeTargetDefault = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE_TARGET_DEFAULT);
			if (qmakeTargetDefault == null) {
				// We only need to override the default when the Qt SDK is debug_and_release but
				// we are only building the project for one kind.
//              if(Constants.CONFIG_RELEASE.equals(configuration))
//                  qmakeTargetDefault = configuration;
//              else if(Constants.CONFIG_DEBUG.equals(configuration))
//                  qmakeTargetDefault = configuration;
				if (Constants.CONFIG_DEBUG_AND_RELEASE.equals(getConfiguration()))
					qmakeTargetDefault = "all";
				else
					qmakeTargetDefault = ""; // must be set to empty string (avoid ant subst ${qmake.target.default})
			}
			mySetProperty(-1, Constants.QMAKE_TARGET_DEFAULT, sourceValue, qmakeTargetDefault, true); // report value
		}

		mySetProperty(-1, Constants.QTJAMBI_SONAME_VERSION_MAJOR, " (taken from qt version)", "" + qtMajorVersion,
				true);
		mySetProperty(-1, Constants.QTJAMBI_SONAME_VERSION_MINOR, " (taken from qt version)", "" + qtMinorVersion,
				true);

		String cachekeyVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.CACHEKEY);
		String cachekeyVersionSource = " (already set)";
		if (cachekeyVersion == null || cachekeyVersion.isEmpty()) { // auto-configure
			cachekeyVersionSource = " (set by init)";
			cachekeyVersion = propertyHelper.replaceProperties(null,
					"${qtjambi.compiler}${qtjambi.configuration.dash}-${DSTAMP}-${TSTAMP}", null);
		}
		mySetProperty(-1, Constants.CACHEKEY, cachekeyVersionSource, cachekeyVersion, true);

		String qtjambiQtLibdir = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
		if (qtjambiQtLibdir != null && !qtjambiQtLibdir.isEmpty()) {
			sourceValue = null;
			s = null;
//            s = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR);
			if (OSInfo.os() == OSInfo.OS.MacOS) {
				s = doesQtLibExistDir(qtjambiQtLibdir, "Resources/qt_menu.nib");
				if (s == null)
					s = doesQtLibExistDir(qtjambiQtLibdir, "qt_menu.nib");
				if (s == null)
					s = doesQtLibExistDir(qtjambiQtLibdir, "../src/gui/mac/qt_menu.nib");
				if (s == null)
					s = doesQtLibExistDir(qtjambiQtLibdir, "../qtbase/src/plugins/platforms/cocoa/qt_menu.nib");
				// FIXME: auto-detect, directory from source, directory from QtSDK on MacOSX,
				// directory from framework on MacOSX

				if (s != null)
					sourceValue = " (auto-detected)";
			}
			if (s == null) {
				if (OSInfo.os() == OSInfo.OS.MacOS)
					sourceValue = " (WARNING you should resolve this for targetting MacOSX)";
				else
					sourceValue = " (expected for non-MacOSX platform)";
				s = "";
			}
			mySetProperty(-1, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR, sourceValue, s, true);
		}
		String qtmenudir = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR);
		if (qtmenudir == null || qtmenudir.isEmpty())
			AntUtil.setProperty(propertyHelper, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR, "", false);

		// The 'qreal' Qt C++ type is 'float' on ARM platforms and 'double' on all
		// others
		String qrealtype = "double";
		String qrealSource = " (the default)";
		if ("arm".equals(javaOsArch)) {
			qrealtype = "float";
			qrealSource = " (${" + Constants.OSCPU + "} is \"arm\")";
		}
		mySetProperty(-1, Constants.QTJAMBI_QREALTYPE, qrealSource, qrealtype, true);

		if (detectQtDsoExistAndSetProperty("qtjambi.core", "QtCore", libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null)) {
			Set<String> availableModules = new TreeSet<>();
			Map<String, ModuleInfo> moduleInfos = moduleInfos();
			for (String module : moduleInfos.keySet()) {
				switch(module) {
				case "core": break;
				default:
					if (detectQtDsoExistAndSetProperty("qtjambi." + module, moduleInfos.get(module).libraryName, libInfix,
							qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, module))
						availableModules.add(module);
					break;
				}
			}
		}else {
			String filename;
			if (useQtFramework) {
				filename = CreateNativeDeploymentTask.formatQtNameAsFramework("QtCore", null, false, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
			} else {
				filename = CreateNativeDeploymentTask.formatQtName("QtCore", null, false, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, false);
			}
			throw new BuildException("Unable to detect "+filename+" library.");
		}
		// Other build information sanity testing and warning

		String generatorIncludepaths = AntUtil.getPropertyAsString(propertyHelper, Constants.GENERATOR_INCLUDEPATHS);
		if (generatorIncludepaths != null) {
			// Validate the settings
			sourceValue = null;
			mySetProperty(-1, Constants.GENERATOR_INCLUDEPATHS, sourceValue, generatorIncludepaths, true);

			prettifyPathSeparator(generatorIncludepaths, true);
		}
		// On Windows it is usual to configure the compile include directory via envvar
		String INCLUDE = System.getenv("INCLUDE");
		if (INCLUDE != null) {
			getProject().log(this, "INCLUDE is set: " + INCLUDE, Project.MSG_INFO);
			prettifyPathSeparator(INCLUDE, false);
		}

		switch(OSInfo.os()) {
			case Linux: { // Check we have libQtCore.so.4 in one of the paths in LD_LIBRARY_PATH
				String LD_LIBRARY_PATH = System.getenv("LD_LIBRARY_PATH");
				getProject().log(this,
						"LD_LIBRARY_PATH is set: " + ((LD_LIBRARY_PATH == null) ? "<notset>" : LD_LIBRARY_PATH),
						Project.MSG_INFO);
				if (LD_LIBRARY_PATH != null) {
					String[] sA = LD_LIBRARY_PATH.split(File.pathSeparator); // should regex escape it
					String filename = CreateNativeDeploymentTask.formatQtName("QtCore", libInfix, isConfigurationDebug(), qtMajorVersion,
							qtMinorVersion, qtPatchlevelVersion);
					int found = 0;
					for (String element : sA) {
						File testDir = new File(element);
						if (testDir.isDirectory() == false)
							getProject().log(this, " WARNING: LD_LIBRARY_PATH directory does not exit: " + element,
									Project.MSG_INFO);
						File testFile = new File(element, filename);
						if (testFile.isFile()) {
							getProject().log(this,
									"   FOUND: LD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(),
									Project.MSG_INFO);
							found++;
						}
					}
	
					// FIXME: Refactor this duplicate code later (we look for !debug here but don't
					// WARNING is we dont find it)
					filename = CreateNativeDeploymentTask.formatQtName("QtCore", libInfix, !isConfigurationDebug(), qtMajorVersion,
							qtMinorVersion, qtPatchlevelVersion);
					found = 0;
					for (String element : sA) {
						File testDir = new File(element);
						// we already warned about non-existing directory here
						File testFile = new File(testDir, filename);
						if (testFile.isFile()) {
							getProject().log(this,
									"  XFOUND: LD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(),
									Project.MSG_INFO);
							found++;
						}
					}
				} else { // Maybe we should check to see if (QTDIR != null) before warning
				}
			}
			break;
			case Windows: { // Check we have QtCore4.dll/Qt5Core.dll in one of the paths in PATH
				String PATH = System.getenv("PATH");
				getProject().log(this, "PATH is set: " + ((PATH == null) ? "<notset>" : PATH), Project.MSG_INFO);
				if (PATH != null) {
					String[] sA = PATH.split(File.pathSeparator); // should regex escape it
					String filename = CreateNativeDeploymentTask.formatQtName("QtCore", libInfix, isConfigurationDebug(), qtMajorVersion,
							qtMinorVersion, qtPatchlevelVersion);
					int found = 0;
					for (String element : sA) {
						File testDir = new File(element);
						if (testDir.isDirectory() == false)
							getProject().log(this, " WARNING: PATH directory does not exit: " + element, Project.MSG_INFO);
						File testFile = new File(element, filename);
						if (testFile.isFile()) {
							getProject().log(this,
									"   FOUND: PATH directory contains QtCore: " + testFile.getAbsolutePath(),
									Project.MSG_INFO);
							found++;
						}
					}
	
					// FIXME: Refactor this duplicate code later (we look for !debug here but don't
					// WARNING is we dont find it)
					filename = CreateNativeDeploymentTask.formatQtName("QtCore", libInfix, !isConfigurationDebug(), qtMajorVersion,
							qtMinorVersion, qtPatchlevelVersion);
					found = 0;
					for (String element : sA) {
						File testDir = new File(element);
						// we already warned about non-existing directory here
						File testFile = new File(testDir, filename);
						if (testFile.isFile()) {
							getProject().log(this,
									"  XFOUND: PATH directory contains QtCore: " + testFile.getAbsolutePath(),
									Project.MSG_INFO);
							found++;
						}
					}
				}
			}
			break;
			case MacOS: { // Check we have libQtCore.4.dylib in one of the paths in
													// DYLD_LIBRARY_PATH
				String DYLD_LIBRARY_PATH = System.getenv("DYLD_LIBRARY_PATH");
				getProject().log(this,
						"DYLD_LIBRARY_PATH is set: " + ((DYLD_LIBRARY_PATH == null) ? "<notset>" : DYLD_LIBRARY_PATH),
						Project.MSG_INFO);
				if (DYLD_LIBRARY_PATH != null) {
					String[] sA = DYLD_LIBRARY_PATH.split(File.pathSeparator); // should regex escape it
					String filename;
					if (useQtFramework) {
						filename = CreateNativeDeploymentTask.formatQtNameAsFramework("QtCore", libInfix, isConfigurationDebug(),
								qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
					} else {
						filename = CreateNativeDeploymentTask.formatQtName("QtCore", libInfix, isConfigurationDebug(), qtMajorVersion,
								qtMinorVersion, qtPatchlevelVersion);
					}
					int found = 0;
					for (String element : sA) {
						File testDir = new File(element);
						File testFile = new File(element, filename);
						if (testFile.isFile()) {
							getProject().log(this,
									"   FOUND: DYLD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(),
									Project.MSG_INFO);
							found++;
						}
					}
	
					// FIXME: Refactor this duplicate code later (we look for !debug here but don't
					// WARNING is we dont find it)
					if (useQtFramework) {
						filename = CreateNativeDeploymentTask.formatQtNameAsFramework("QtCore", libInfix, !isConfigurationDebug(),
								qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
					} else {
						filename = CreateNativeDeploymentTask.formatQtName("QtCore", libInfix, !isConfigurationDebug(), qtMajorVersion,
								qtMinorVersion, qtPatchlevelVersion);
					}
					found = 0;
					for (String element : sA) {
						File testDir = new File(element);
						// we already warned about non-existing directory here
						File testFile = new File(testDir, filename);
						if (testFile.isFile()) {
							getProject().log(this,
									"  XFOUND: DYLD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(),
									Project.MSG_INFO);
							found++;
						}
					}
				}
			}
			break;
			default: break;
		}

		// Setup some properties to be silently set to an empty string for safe ANT
		// substitution
		{
			final String[] emptyStringIfNotSetA = { Constants.QTJAMBI_CONFIG_JUMPTABLE,
					Constants.QTJAMBI_GENERATOR_JUMPTABLE };
			for (String emptyStringIfNotSet : emptyStringIfNotSetA) {
				String value = AntUtil.getPropertyAsString(propertyHelper, emptyStringIfNotSet);
				if (value == null)
					value = ""; // set this empty-string default here
				mySetProperty(0, emptyStringIfNotSet, " (default)", value, true); // silent
			}
		}

		// Sanity checking, the goal here is to abort the build process early
		// on so the developer doesn't waste their time getting an error
		// message later in the build process.
		if (generatorIncludepaths == null) // ${generator.includepaths}
			throw new BuildException("ERROR: " + Constants.GENERATOR_INCLUDEPATHS
					+ " property is not configured, please ensure you read and edit build.properties");
		
		mySetProperty(-1, "TODAY", null, new SimpleDateFormat("yyyy-MM-dd").format(new Date()), false);
		
		File buildDir = new File(new File(qtjambiFullVersion), "build");
		mySetProperty(-1, "outputDir", null, buildDir.getAbsolutePath(), true);
		mySetProperty(-1, "deploymentdir", null, new File(new File(qtjambiFullVersion), "deployment").getAbsolutePath(), true);
		mySetProperty(-1, "java.outdir", null, new File(buildDir, "java").getAbsolutePath(), true);
		mySetProperty(-1, "java.outsrcdir", null, new File(buildDir, "java-src").getAbsolutePath(), true);
		File platformBuildDir = new File(buildDir, osname);
		mySetProperty(-1, "qtjambi.builddir", null, new File(platformBuildDir, "qtjambi").getAbsolutePath(), true);
		if(Objects.equals(OSInfo.osArchName(), OSInfo.crossOSArchName())) {
			mySetProperty(-1, "generator.builddir", null, new File(new File(platformBuildDir, "qtjambi"), "QtJambiGenerator").getAbsolutePath(), true);
		}else {
			mySetProperty(-1, "generator.builddir", null, new File(new File(platformBuildDir, "generator"), "QtJambiGenerator").getAbsolutePath(), true);
		}
		mySetProperty(-1, "plugins.builddir", null, new File(platformBuildDir, "plugins").getAbsolutePath(), true);
		
		File generatorOutputdir = new File(buildDir, "generator");
		mySetProperty(-1, "generator.outputdir", null, generatorOutputdir.getAbsolutePath(), true);
		configureGenerator(generatorOutputdir);
		
		mySetProperty(-1, Constants.GENERATOR_PREPROC_DEFINES, null, listJoinToString(generatorPreProcDefinesList, ","), true);
		mySetProperty(-1, Constants.GENERATOR_STATICLIBS, null, listJoinToString(generatorStaticLibsList, ","), true);
		
		int threads = 1;
		{
			String qtjambiJunitThreads = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.junit.threads");
			try {
				threads = Integer.parseInt(qtjambiJunitThreads);
			} catch (Throwable e) {
				if (!"false".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "parallel"))) {
					AntUtil.setProperty(propertyHelper, "qtjambi.junit.threads",
							"" + (threads = Runtime.getRuntime().availableProcessors()));
				} else {
					AntUtil.setProperty(propertyHelper, "qtjambi.junit.threads", "1");
				}
			}
		}
		{
			String qtjambiJunitThreads = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.junit.timeout");
			try {
				Integer.parseInt(qtjambiJunitThreads);
			} catch (Throwable e) {
				AntUtil.setProperty(propertyHelper, "qtjambi.junit.timeout", "" + (360000 + 60000 * threads));
			}
		}
	}

	public static String listToString(Collection<?> list) {
		StringBuffer sb = new StringBuffer();
		sb.append("[");
		boolean first = true;
		for (Object o : list) {
			if (first)
				first = false;
			else
				sb.append(", ");
			sb.append("\"");
			sb.append(o.toString());
			sb.append("\"");
		}
		sb.append("]");
		return sb.toString();
	}

	public static String listJoinToString(Collection<?> list, CharSequence delim) {
		StringBuffer sb = new StringBuffer();
		boolean first = true;
		for (Object o : list) {
			if (first)
				first = false;
			else
				sb.append(delim);
			sb.append(o.toString());
		}
		return sb.toString();
	}

	private String detectConfiguration(String wantedConfiguration) {
		String dsoName = "QtCore";
		boolean isStatic = false;
		String dsoPath = doesQtLibExist(dsoName, libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null, Boolean.FALSE, false);
		if (dsoPath == null)
			dsoPath = doesQtBinExist(dsoName, libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null, Boolean.FALSE, false);
		if (dsoPath == null) {
			dsoPath = doesQtStaticLibExist(dsoName, libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null, Boolean.FALSE, false);
			isStatic = dsoPath != null;
		}
		String dsoDebugPath = doesQtLibExist(dsoName, libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null, Boolean.TRUE, false);
		if (dsoDebugPath == null)
			dsoDebugPath = doesQtBinExist(dsoName, libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null, Boolean.TRUE, false);
		if (dsoDebugPath == null) {
			dsoDebugPath = doesQtStaticLibExist(dsoName, libInfix, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, null, Boolean.TRUE, false);
			isStatic = dsoDebugPath != null;
		}
		String thisConfiguration = null;
		String sourceValue = null;
		if (wantedConfiguration != null) {
			// if we parsed *.prl we error on invalid configurations against it
			if (Constants.CONFIG_DEBUG_AND_RELEASE.equals(wantedConfiguration)) {
				thisConfiguration = wantedConfiguration;
			} else if (Constants.CONFIG_RELEASE.equals(wantedConfiguration)) {
				thisConfiguration = wantedConfiguration;
			} else if (Constants.CONFIG_DEBUG.equals(wantedConfiguration)) {
				thisConfiguration = wantedConfiguration;
			} else if (Constants.CONFIG_TEST.equals(wantedConfiguration)) {
				thisConfiguration = wantedConfiguration;
			} else {
				throw new BuildException(Constants.CONFIGURATION + " could not be set to " + wantedConfiguration
						+ " try one of " + Constants.CONFIG_RELEASE + ", " + Constants.CONFIG_DEBUG + ", "
						+ Constants.CONFIG_DEBUG_AND_RELEASE + ", " + Constants.CONFIG_TEST);
			}
			sourceValue = "default";
		}else if(isStatic) {
			thisConfiguration = Constants.CONFIG_DEBUG_AND_RELEASE;
			sourceValue = "detected";
		}else if(dsoPath!=null && dsoDebugPath!=null){
			switch (OSInfo.crossOS()) {
			case Windows:
				thisConfiguration = Constants.CONFIG_DEBUG_AND_RELEASE;
				sourceValue = "detected";
				String compiler = (String)PropertyHelper.getProperty(getProject(), Constants.COMPILER);
				if(compiler!=null && !compiler.startsWith("msvc")) {
					thisConfiguration = Constants.CONFIG_RELEASE;
					sourceValue = "detected";
				}
				break;
			default:
				thisConfiguration = Constants.CONFIG_RELEASE;
				sourceValue = "detected";
				break;
			}
		}else if(dsoDebugPath!=null) {
			thisConfiguration = Constants.CONFIG_DEBUG;
			sourceValue = "detected";			
		}else if(dsoPath!=null) {
			thisConfiguration = Constants.CONFIG_RELEASE;
			sourceValue = "detected";
		} else {
			thisConfiguration = Constants.CONFIG_DEBUG_AND_RELEASE; // this is the configuration of last resort
			sourceValue = "default";
		}
		setConfiguration(thisConfiguration);
		{
        	String debugBundles = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.debug.bundles");
        	if(debugBundles==null || debugBundles.isEmpty()) {
        		AntUtil.setProperty(propertyHelper, "qtjambi.debug.bundles", "false");
        	}else {
        		AntUtil.setProperty(propertyHelper, "qtjambi.debug.bundles", ""+Boolean.parseBoolean(debugBundles));
        	}
        }
		mySetProperty(-1, Constants.CONFIGURATION, " (" + sourceValue + ")", thisConfiguration, true);
		return thisConfiguration;
	}

	// wantedSdk maybe "MacOSX10.5.sdk" or "/Developer/SDKs/MacOSX10.5.sdk"
	private String detectMacosxSdk(String wantedSdk) {
		if (OSInfo.os() == OSInfo.OS.MacOS == false || qtMajorVersion >= 5)
			return null;

		// QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
		String sourceValueAvailable = null;
		Map<String, File> foundMap = new HashMap<String, File>();
		List<String> filenameList = new ArrayList<String>();
		File sdkDir = new File("/Developer/SDKs");
		if (sdkDir.isDirectory()) {
			File[] listFiles = sdkDir.listFiles();
			for (File f : listFiles) {
				if (f.isDirectory()) {
					String name = f.getName();
					foundMap.put(name, f);
					filenameList.add(name);
				}
			}
			Collections.sort(filenameList, Collections.reverseOrder());

			// build available string
			StringBuilder sb = new StringBuilder("available: ");
			boolean first = true;
			for (String n : filenameList) {
				if (first)
					first = false;
				else
					sb.append(", ");
				sb.append(n);
			}
			sourceValueAvailable = sb.toString();
		}

		boolean foundValue = false; // null is a valid value
		String sourceValue = "";
		String thisValue = null;
		File wantedSdkFile = null;
		if (wantedSdk != null) {
			if (wantedSdk.length() > 0) {
				wantedSdkFile = new File(wantedSdk); // absolute ?
				if (wantedSdkFile.isDirectory() == false) {
					wantedSdkFile = new File("/Developer/SDKs", wantedSdk); // relative?
					if (wantedSdkFile.isDirectory() == false)
						wantedSdkFile = null;
				}
				if (wantedSdkFile != null) {
					thisValue = wantedSdkFile.getAbsolutePath(); // invalidate it (maybe we should allow it and warn it
																	// does not exist)
					foundValue = true;
				}
			} else {
				if (wantedSdk.length() == 0)
					thisValue = null; // treat empty-string as null
				foundValue = true; // null is the value we want
			}
		}
		if (foundValue == false) {
			if (foundMap.isEmpty())
				throw new BuildException(
						"Directory \"/Developer/SDKs\" does not exist, did you install XCode on your Macosx ?");
			sourceValue = "picked default";
			thisValue = foundMap.get(filenameList.get(0)).getAbsolutePath();
			foundValue = true;
		}

		if (foundValue) {
			if (sourceValueAvailable != null) {
				if (sourceValue.length() > 0) // isEmpty() is Java1.6 :(
					sourceValue += "; ";
				sourceValue += sourceValueAvailable;
			}
			mySetProperty(-1, Constants.QTJAMBI_MACOSX_MAC_SDK, " (" + sourceValue + ")", thisValue, true);
		}

		return thisValue;
	}

	private void prettifyPathSeparator(String generatorIncludepaths, boolean allowRecurse) {
		StringTokenizer tokenizer = new StringTokenizer(generatorIncludepaths, File.pathSeparator);
		while (tokenizer.hasMoreTokens()) {
			String path = tokenizer.nextToken();
			path = propertyHelper.replaceProperties(null, path, null); // one round of substitution
			if (allowRecurse && path.indexOf(File.pathSeparatorChar) >= 0) {
				prettifyPathSeparator(path, false); // this is due to ${env.INCLUDE}
				continue;
			}
			File dir = new File(path);
			String msg = dir.getAbsolutePath();
			if (dir.isDirectory()) {
				getProject().log(this, "   FOUND: " + msg, Project.MSG_INFO);
			} else {
				getProject().log(this, " MISSING: " + msg, Project.MSG_WARN);
			}
		}
	}

	private File resolveDirectoryViaQTDIR(String qtdir, String attrName) {
		File tryPath = null;

		if (attrName.equals(Constants.BINDIR)) {
			tryPath = new File(qtdir, "bin"); // $QTDIR/bin
		} else if (attrName.equals(Constants.LIBDIR)) {
			tryPath = new File(qtdir, "lib"); // $QTDIR/lib
		} else if (attrName.equals(Constants.TOOLS_BINDIR)) {
			tryPath = new File(qtdir, "bin"); // $QTDIR/bin
		} else if (attrName.equals(Constants.TOOLS_LIBDIR)) {
			tryPath = new File(qtdir, "lib"); // $QTDIR/lib
		} else if (attrName.equals(Constants.LIBEXECDIR)) {
			tryPath = new File(qtdir, "libexec"); // $QTDIR/libexec
		} else if (attrName.equals(Constants.INCLUDEDIR)) {
			tryPath = new File(qtdir, "include"); // $QTDIR/include
		} else if (attrName.equals(Constants.PLUGINSDIR)) {
			tryPath = new File(qtdir, "plugins"); // $QTDIR/plugins
		} else if (attrName.equals(Constants.QMLMODULESDIR)) {
			tryPath = new File(qtdir, "qml"); // $QTDIR/qml
		} else if (attrName.equals(Constants.IMPORTSDIR)) {
			tryPath = new File(qtdir, "imports"); // $QTDIR/imports
		} else if (attrName.equals(Constants.TRANSLATIONSDIR)) {
			tryPath = new File(qtdir, "translations"); // $QTDIR/translations
		} else if (attrName.equals(Constants.RESOURCESDIR)) {
			tryPath = new File(qtdir, "resources"); // $QTDIR/resources
		} else if (attrName.equals(Constants.QMAKE)) {
			String qmakeExecName = QMakeTask.executableName();
			tryPath = new File(qtdir, "bin" + File.separator + qmakeExecName); // $QTDIR/bin
			if(!tryPath.isFile() && qmakeExecName.endsWith(".exe")) {
				tryPath = new File(qtdir, "bin" + File.separator + qmakeExecName.replace(".exe", ".bat"));
			}
		}

		if (tryPath.isDirectory())
			return tryPath;
		return null;
	}

	private boolean parseQtVersion(String versionString) {
		if (versionString == null || versionString.isEmpty())
			return false;

		// Remove leading, remove trailing whitespace
		versionString = Util.stripLeadingAndTrailingWhitespace(versionString);

		{
			// Check for valid character set "[0-9\.]+"
			final int len = versionString.length();
			for (int i = 0; i < len; i++) {
				char c = versionString.charAt(i);
				if ((c >= '0' && c <= '9') || c == '.')
					continue;
				return false;
			}
		}

		// Check for non-empty
		final int len = versionString.length();
		if (len == 0)
			return false;

		// Check for [0-9\.] and no double dots, no leading/trailing dots.
		if (versionString.charAt(0) == '.' || versionString.charAt(len - 1) == '.')
			return false;

		if (versionString.indexOf("..") >= 0)
			return false;

		// Split
		String[] versionParts = versionString.split("\\.");

		String tmpQtVersion = null;
		Integer tmpQtMajorVersion = null;
		Integer tmpQtMinorVersion = null;
		Integer tmpQtPatchlevelVersion = null;

		try {
			tmpQtVersion = versionString;
			if (versionParts.length < 1)
				return false;
			tmpQtMajorVersion = Integer.valueOf(versionParts[0]);
			if (versionParts.length < 2)
				return false;
			tmpQtMinorVersion = Integer.valueOf(versionParts[1]);
			if (versionParts.length < 3)
				tmpQtPatchlevelVersion = 0;
			else
				tmpQtPatchlevelVersion = Integer.valueOf(versionParts[2]);
		} catch (NumberFormatException e) {
			return false;
		}

		// Ok we happy
		qtVersion = tmpQtVersion;
		qtMajorVersion = tmpQtMajorVersion.intValue();
		qtMinorVersion = tmpQtMinorVersion.intValue();
		qtPatchlevelVersion = tmpQtPatchlevelVersion.intValue();
		return true;
	}

	private boolean decideQtVersion(Map<String,String> qmakeQuery) {
		boolean versionFound = false;

		String tmpQtVersion = null;

		if (!versionFound) {
			tmpQtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION);
			if (parseQtVersion(tmpQtVersion)) {
				versionFound = true;
				qtVersionSource = " (${" + Constants.QT_VERSION + "})";
			}
		}

		if (!versionFound) {
			tmpQtVersion = qmakeQuery.get("QT_VERSION");
			if (tmpQtVersion != null) {
				if (parseQtVersion(tmpQtVersion)) {
					versionFound = true;
					qtVersionSource = " (qmake -query QT_VERSION)";
				}
			}
		}

		// This is last method as it is the target we are trying to set and also
		// debatable if it should be here at all. Maybe the only use is with maybe
		// supporting really older Qt which does not allow: qmake -query
		if (!versionFound) {
			tmpQtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.VERSION);
			if (parseQtVersion(tmpQtVersion)) {
				versionFound = true;
				qtVersionSource = " (${" + Constants.VERSION + "})";
			}
		}

		return versionFound;
	}

	private boolean decideQMAKE_XSPEC(Map<String,String> qmakeQuery) {
		boolean specFound = false;

		String qmakeXspecSource = "";
		String qmakeSpec = null;
		String qmakeXSpec = null;

		if (!specFound) {
			qmakeXSpec = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_SPEC);
			if (parseQtVersion(qmakeXSpec)) {
				specFound = true;
				qmakeXspecSource = " (${" + Constants.QT_SPEC + "})";
			}
		}

		if (!specFound) {
			qmakeSpec = qmakeQuery.get("QMAKE_SPEC");
			qmakeXSpec = qmakeQuery.get("QMAKE_XSPEC");
			if (qmakeXSpec != null) {
				specFound = true;
				mySetProperty(-1, Constants.QT_SPEC, qmakeXspecSource, qmakeXSpec, true);
				mySetProperty(-1, Constants.TOOLS_QMAKESPEC, qmakeXspecSource, qmakeSpec, true);
				if (!qmakeXSpec.equals(qmakeSpec)) {
					OSInfo.setQMakeXSpec(qmakeXSpec);
				}
			}else {
				mySetProperty(-1, Constants.QT_SPEC, qmakeXspecSource, qmakeSpec, true);
				mySetProperty(-1, Constants.TOOLS_QMAKESPEC, qmakeXspecSource, qmakeSpec, true);
				specFound = qmakeSpec!=null;
			}
		}else {
			mySetProperty(-1, Constants.TOOLS_QMAKESPEC, qmakeXspecSource, qmakeSpec, true);
		}
		return specFound;
	}

	private void configureGenerator(File generatorOutputdir) {
		String qtsources = AntUtil.getPropertyAsString(propertyHelper, "qtsources");
		if(qtsources==null || qtsources.isEmpty()) {
			String qtdir = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
			if (new File(new File(qtdir),
					".." + File.separator + "Src" + File.separator + "qtbase").isDirectory()
					&& new File(new File(qtdir),
							".." + File.separator + "Src" + File.separator + "qtbase")
									.listFiles().length > 0) {
				qtsources = new File(new File(qtdir), ".." + File.separator + "Src").getAbsolutePath();
				AntUtil.setProperty(propertyHelper, "qtsources", qtsources, false);
			}
		}
		String generatorExtraIncludes = AntUtil.getPropertyAsString(propertyHelper, "generator.extra.includes");
		String version = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION);
		File libPath = new File(AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR));
		File includePath = new File(AntUtil.getPropertyAsString(propertyHelper, Constants.INCLUDEDIR));
		Map<String, ModuleInfo> moduleInfos = moduleInfos();
		List<String> modules = new ArrayList<>();
		File headersdir = new File(generatorOutputdir, "missing-headers");
		for (String module : moduleInfos.keySet()) {
			ModuleInfo info = moduleInfos.get(module);
			if (Boolean.parseBoolean(AntUtil.getPropertyAsString(propertyHelper, "qtjambi." + module + ".any.true"))) {
				String qtjambiLibName = "QtJambi"+info.libraryName.substring(2);
				if (Boolean.parseBoolean(
						AntUtil.getPropertyAsString(propertyHelper, "qtjambi." + module + ".staticlib"))
						|| info.isPureStaticLib) {
					generatorStaticLibsList.add(info.libraryName);
				}
				if(info.preprocdef!=null) {
					if(!modules.contains(qtjambiLibName))
						modules.add(qtjambiLibName);
				}
				boolean optional = false;
				switch(info.requiredHeaders) {
				case PrivateOptional:
					optional = true;
				case Private:
					File privateHeaders = new File(includePath + "/" + info.libraryName + "/" + version + "/" + info.libraryName + "/private");
					if((!privateHeaders.exists() || !privateHeaders.isDirectory()) 
							&& OSInfo.crossOS()==OSInfo.OS.MacOS && useQtFramework) {
						if(qtMajorVersion==5)
							privateHeaders = new File(libPath, info.libraryName+".framework/Versions/5/Headers/" + version + "/" + info.libraryName + "/private");
						else
							privateHeaders = new File(libPath, info.libraryName+".framework/Versions/A/Headers/" + version + "/" + info.libraryName + "/private");
					}
					if(!privateHeaders.exists() || !privateHeaders.isDirectory()) {
						if(OSInfo.crossOS()==OSInfo.OS.Linux) {
							File dir = new File(includePath, info.libraryName);
							if(dir.isDirectory()) {
								String versionX = String.format("%1$s.%2$s.", qtMajorVersion, qtMinorVersion);
								File _dir = null;
								for(File f : dir.listFiles()) {
									if(f.isDirectory() && f.getName().startsWith(versionX)) {
										_dir = f;
										break;
									}
								}
								if(_dir!=null) {
									dir = new File(_dir, info.libraryName);
									dir = new File(dir, "private");
									if(dir.exists() && dir.isDirectory()) {
										if(!modules.contains(qtjambiLibName))
											modules.add(qtjambiLibName);
										break;
									}
								}
							}
						}
						if(optional) {
							generatorPreProcDefinesList.add(info.preprocdef+"_PRIVATE");
						}else {
							switch(module) {
							case "core":
							case "gui":
							case "widgets":
								throw new BuildException("Private headers missing for module "+info.libraryName+". Expecting: "+privateHeaders.getAbsolutePath());
							default:
								break;
							}
							generatorPreProcDefinesList.add(info.preprocdef);
						}
						getProject().log(this, "qtjambi." + module + ": private headers missing. Expecting: "+privateHeaders.getAbsolutePath(), Project.MSG_WARN);
					}else {
						if(info.preprocdef!=null) {
							if(!modules.contains(qtjambiLibName+"-private"))
								modules.add(qtjambiLibName+"-private");
						}
					}
					break;
				default:
					break;
				}
			} else if (info.preprocdef != null) {
				switch (module) {
				case "activex":
					if (skippedModules.contains(module) || qtsources==null || qtsources.isEmpty()) {
						generatorPreProcDefinesList.add(info.preprocdef);
					} else {
						if(qtMajorVersion==5) {
							File includeDir = new File(new File(new File(qtsources), "qtactiveqt"), "include");
							if(includeDir.isDirectory()) {
								for(File moduleDir : includeDir.listFiles()) {
									if(moduleDir.isDirectory() && !moduleDir.getName().startsWith(".")) {
										File moduleHeaders = new File(headersdir, moduleDir.getName());
										if(!moduleHeaders.isDirectory()) {
											moduleHeaders.mkdirs();
											for(File header : moduleDir.listFiles()) {
												if(header.isFile()) {
													File target = new File(moduleHeaders, header.getName());
													if(header.getName().endsWith(".h")) {
														for(File subdir : new File(new File(new File(new File(qtsources), "qtactiveqt"), "src"), "activeqt").listFiles()) {
															if(subdir.isDirectory()) {
																File header2 = new File(subdir, header.getName());
																if(header2.isFile()) {
																	header = header2;
																	break;
																}
															}
														}
													}
													try {
														Files.copy(header.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING);
													} catch (IOException e) {
														e.printStackTrace();
													}
												}
											}
										}
									}
								}
								String qtjambiLibName = "QtJambi"+info.libraryName.substring(2);
								if (Boolean.parseBoolean(
										AntUtil.getPropertyAsString(propertyHelper, "qtjambi." + module + ".staticlib"))
										|| info.isPureStaticLib) {
									generatorStaticLibsList.add(info.libraryName);
								}
								if(!modules.contains(qtjambiLibName))
									modules.add(qtjambiLibName);
							}else {
								generatorPreProcDefinesList.add(info.preprocdef);
							}
						}else {
							File srcDir = new File(new File(new File(new File(qtsources), "qtactiveqt"), "src"), "activeqt");
							File cmakeFile = new File(srcDir, "CMakeLists.txt");
							if(cmakeFile.isFile()) {
								try {
									String cmakeSpec = new String(Files.readAllBytes(cmakeFile.toPath()), "UTF-8");
									int idx = cmakeSpec.indexOf("qt_internal_add_module");
									if(idx>0) {
										idx = cmakeSpec.indexOf('(', idx);
										if(idx>0) {
											int idx2 = cmakeSpec.indexOf(')', idx);
											String qt_internal_add_module = cmakeSpec.substring(idx+1, idx2);
											if(qt_internal_add_module.contains("HEADER_MODULE") && qt_internal_add_module.contains("SOURCES")) {
												idx = qt_internal_add_module.indexOf("MODULE_INCLUDE_NAME ");
												if(idx>0) {
													idx = qt_internal_add_module.indexOf(' ', idx);
													idx2 = qt_internal_add_module.indexOf('\n', idx);
													String includeName = qt_internal_add_module.substring(idx+1, idx2).trim();
													idx = qt_internal_add_module.indexOf("SOURCES", idx2);
													if(idx>0) {
														File axcontainerHeaders = new File(headersdir, "QtAxContainer");
														if(!axcontainerHeaders.isDirectory()) {
															axcontainerHeaders.mkdirs();
														}
														File moduleHeaders = new File(headersdir, includeName);
														if(!moduleHeaders.isDirectory()) {
															moduleHeaders.mkdirs();
														}
														try(PrintStream out1 = new PrintStream(new File(moduleHeaders, qtMajorVersion>6 || qtMinorVersion>=5 ? "QtActiveQt" : "ActiveQt"));
																PrintStream out2 = new PrintStream(new File(axcontainerHeaders, "QtAxContainer"))){
															String[] sources = qt_internal_add_module.substring(idx+7).trim().split("[\\s]+");
															for(String srcentry : sources) {
																if(srcentry.endsWith(".h") && !srcentry.endsWith("_p.h")) {
																	if(File.separatorChar!='/')
																		srcentry = srcentry.replace('/', File.separatorChar);
																	File srcFile = new File(srcDir, srcentry);
																	File target = new File(moduleHeaders, srcFile.getName());
																	Files.copy(srcFile.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING);
																	target = new File(axcontainerHeaders, srcFile.getName());
																	Files.copy(srcFile.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING);
																	out1.print("#include \"");
																	out1.print(srcFile.getName());
																	out1.println("\"");
																	out2.print("#include \"");
																	out2.print(srcFile.getName());
																	out2.println("\"");
																}
															}
														}
													}
												}
											}
										}
									}
									String qtjambiLibName = "QtJambi"+info.libraryName.substring(2);
									if (Boolean.parseBoolean(
											AntUtil.getPropertyAsString(propertyHelper, "qtjambi." + module + ".staticlib"))
											|| info.isPureStaticLib) {
										generatorStaticLibsList.add(info.libraryName);
									}
									if(!modules.contains(qtjambiLibName))
										modules.add(qtjambiLibName);
								} catch (IOException e) {
									e.printStackTrace();
								}
							}else {
								generatorPreProcDefinesList.add(info.preprocdef);
							}
						}
					}
					break;
				case "macextras":
				case "x11extras":
				case "winextras":
					if (skippedModules.contains(module) || qtMajorVersion>5 || qtsources==null || qtsources.isEmpty()) {
						generatorPreProcDefinesList.add(info.preprocdef);
					} else {
						File includeDir = new File(new File(new File(qtsources), "qt" + module), "include");
						if(includeDir.isDirectory()) {
							for(File moduleDir : includeDir.listFiles()) {
								if(moduleDir.isDirectory() && !moduleDir.getName().startsWith(".")) {
									File moduleHeaders = new File(headersdir, moduleDir.getName());
									if(!moduleHeaders.isDirectory()) {
										moduleHeaders.mkdirs();
										for(File header : moduleDir.listFiles()) {
											if(header.isFile()) {
												File target = new File(moduleHeaders, header.getName());
												if(header.getName().endsWith(".h")) {
													File header2 = new File(new File(new File(new File(new File(qtsources), "qt" + module), "src"), module), header.getName());
													if(header2.isFile())
														header = header2;
												}
												try {
													Files.copy(header.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING);
												} catch (IOException e) {
													e.printStackTrace();
												}
											}
										}
									}
								}
							}
							String qtjambiLibName = "QtJambi"+info.libraryName.substring(2);
							if (Boolean.parseBoolean(
									AntUtil.getPropertyAsString(propertyHelper, "qtjambi." + module + ".staticlib"))
									|| info.isPureStaticLib) {
								generatorStaticLibsList.add(info.libraryName);
							}
							if(!modules.contains(qtjambiLibName))
								modules.add(qtjambiLibName);
						}else {
							generatorPreProcDefinesList.add(info.preprocdef);
						}
					}
					break;
				default:
					generatorPreProcDefinesList.add(info.preprocdef);
				}
			}
		}
		AntUtil.setProperty(propertyHelper, Constants.QTJAMBI_MODULES, String.join(",", modules), false);
		if(((qtMajorVersion==6 && qtMinorVersion>=5) || qtMajorVersion>6) && qtsources!=null) {
			if(!headersdir.isDirectory()) {
				headersdir.mkdirs();
			}
			File opengldir = new File(headersdir, "QtOpenGL");
			if(!opengldir.isDirectory()) {
				File originalIncludeDir = new File(includePath, "QtOpenGL");
				if((!originalIncludeDir.exists() || !originalIncludeDir.isDirectory()) 
						&& OSInfo.crossOS()==OSInfo.OS.MacOS && useQtFramework) {
					originalIncludeDir = new File(libPath, "QtOpenGL.framework/Versions/A/Headers");
				}
				File moduleDir = new File(new File(qtsources), "qtbase" + File.separator + "src" + File.separator + "opengl");
				if (moduleDir.isDirectory()) {
					for(File file : moduleDir.listFiles(f->f.getName().startsWith("qopenglfunctions_") && f.getName().endsWith(".h"))) {
						if(!new File(originalIncludeDir, file.getName()).exists()) {
							try {
								opengldir.mkdirs();
								Files.copy(file.toPath(), new File(opengldir, file.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
							} catch (IOException e) {
								e.printStackTrace();
							}
						}
					}
				}
			}
			File coredir = new File(headersdir, "QtCore");
			if(!coredir.isDirectory()) {
				File originalIncludeDir = new File(includePath, "QtCore");
				if((!originalIncludeDir.exists() || !originalIncludeDir.isDirectory()) 
						&& OSInfo.crossOS()==OSInfo.OS.MacOS && useQtFramework) {
					originalIncludeDir = new File(libPath, "QtCore.framework/Versions/A/Headers");
				}
				File moduleDir = new File(new File(qtsources), "qtbase" + File.separator + "src" + File.separator + "corelib" + File.separator + "kernel");
				if (moduleDir.isDirectory()) {
					File file = new File(moduleDir, "qpermissions.h");
					if(file.exists() && !new File(originalIncludeDir, file.getName()).exists()) {
						try {
							coredir.mkdirs();
							Files.copy(file.toPath(), new File(coredir, file.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}
			}
			File guidir = new File(headersdir, "QtGui");
			if(!guidir.isDirectory()) {
				File originalIncludeDir = new File(includePath, "QtGui");
				if((!originalIncludeDir.exists() || !originalIncludeDir.isDirectory()) 
						&& OSInfo.crossOS()==OSInfo.OS.MacOS && useQtFramework) {
					originalIncludeDir = new File(libPath, "QtGui.framework/Versions/A/Headers");
				}
				File moduleDir = new File(new File(qtsources), "qtbase" + File.separator + "src" + File.separator + "gui" + File.separator + "platform" + File.separator + "darwin");
				if (moduleDir.isDirectory()) {
					File file = new File(moduleDir, "qutimimeconverter.h");
					if(file.exists() && !new File(originalIncludeDir, file.getName()).exists()) {
						try {
							guidir.mkdirs();
							Files.copy(file.toPath(), new File(guidir, file.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}
				moduleDir = new File(new File(qtsources), "qtbase" + File.separator + "src" + File.separator + "gui" + File.separator + "platform" + File.separator + "windows");
				if (moduleDir.isDirectory()) {
					File file = new File(moduleDir, "qwindowsmimeconverter.h");
					if(file.exists() && !new File(originalIncludeDir, file.getName()).exists()) {
						try {
							guidir.mkdirs();
							Files.copy(file.toPath(), new File(guidir, file.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}
				moduleDir = new File(new File(qtsources), "qtbase" + File.separator + "src" + File.separator + "gui" + File.separator + "vulkan");
				if (moduleDir.isDirectory()) {
					File file = new File(moduleDir, "qvulkaninstance.h");
					if(file.exists() && !new File(originalIncludeDir, file.getName()).exists()) {
						try {
							guidir.mkdirs();
							Files.copy(file.toPath(), new File(guidir, file.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}
			}
		}
		if(headersdir.isDirectory()) {
			if (generatorExtraIncludes == null)
				generatorExtraIncludes = "";
			if (!generatorExtraIncludes.isEmpty())
				generatorExtraIncludes += ";";
			generatorExtraIncludes += headersdir.getAbsolutePath();
		}
		if(generatorExtraIncludes!=null) {
			AntUtil.setProperty(propertyHelper, "generator.extra.includes", generatorExtraIncludes, false);
		}
	}

	private String doesQtLibExistDir(String librarydir, String name) {
		File dir = new File(librarydir, name);
		if (dir.exists() && dir.isDirectory())
			return dir.getAbsolutePath();
		return null;
	}

	private String doesQtDsoExist(String name, String infix, int majorVersion, int minorVersion, int patchVersion,
			String librarydir, Boolean debugValue, boolean prl, boolean staticLib, String propName) {
		if ((infix == null || infix.isEmpty()) && OSInfo.crossOSArchName().equalsIgnoreCase("android") && !androidAbis.isEmpty()) {
			infix = "_" + androidAbis.get(0);
		}
		String path = null;

		if (librarydir != null) {
			path = librarydir;
		} else if (propName != null) {
			String propValue = AntUtil.getPropertyAsString(propertyHelper, propName);
			if (propValue != null && !propValue.isEmpty())
				path = propValue;
		}

		boolean thisDebug = isConfigurationDebug();
		if (debugValue != null)
			thisDebug = debugValue.booleanValue();
		String filename;
		if (prl) {
			if (useQtFramework && name.startsWith("Qt")) {
				filename = CreateNativeDeploymentTask.formatQtPrlNameAsFramework(name, infix, thisDebug);
			} else {
				filename = CreateNativeDeploymentTask.formatQtPrlName(name, infix, thisDebug, qtMajorVersion, qtMinorVersion,
						qtPatchlevelVersion);
			}
		} else if (useQtFramework && name.startsWith("Qt") && !staticLib) {
			filename = CreateNativeDeploymentTask.formatQtNameAsFramework(name, infix, thisDebug, majorVersion, minorVersion,
					patchVersion);
		} else {
			filename = CreateNativeDeploymentTask.formatQtName(name, infix, thisDebug, majorVersion, minorVersion, patchVersion,
					staticLib);
		}
		if(path==null)
			return null;
		File testForFile = new File(new File(path), filename);
		getProject().log(this, "Checking " + testForFile + " " + testForFile.exists(), Project.MSG_VERBOSE);
		if (testForFile.exists()) {
			try {
				return testForFile.getCanonicalPath();
			} catch (IOException e) {
				return testForFile.getAbsolutePath();
			}
		}else if(OSInfo.crossOS()==OS.Linux && !staticLib){
			filename = CreateNativeDeploymentTask.formatQtName(name, infix, thisDebug, majorVersion, minorVersion, -1,
					staticLib);
			testForFile = new File(new File(path), filename);
			if(testForFile.exists()) {
				if(Files.isSymbolicLink(testForFile.toPath())) {
					try {
						testForFile = Files.readSymbolicLink(testForFile.toPath()).toFile();
					} catch (Exception e) {
					}
				}
				try {
					return testForFile.getCanonicalPath();
				} catch (IOException e) {
					return testForFile.getAbsolutePath();
				}
			}
		}
		return null;
	}

	private String doesQtBinExist(String name, String infix, int majorVersion, int minorVersion, int patchVersion,
			String librarydir, Boolean debugValue, boolean prl) {
		return doesQtDsoExist(name, infix, majorVersion, minorVersion, patchVersion, librarydir, debugValue, prl, false,
				Constants.BINDIR);
	}

	private String doesQtLibExist(String name, String infix, int majorVersion, int minorVersion, int patchVersion,
			String librarydir, Boolean debugValue, boolean prl) {
		return doesQtDsoExist(name, infix, majorVersion, minorVersion, patchVersion, librarydir, debugValue, prl, false,
				Constants.LIBDIR);
	}

	private String doesQtStaticLibExist(String name, String infix, int majorVersion, int minorVersion, int patchVersion,
			String librarydir, Boolean debugValue, boolean prl) {
		return doesQtDsoExist(name, infix, majorVersion, minorVersion, patchVersion, librarydir, debugValue, prl, true,
				Constants.LIBDIR);
	}

	static class LibResult {
		private LibResult(String path, boolean isStatic) {
			this.path = path;
			this.isStatic = isStatic;
		}

		final String path;
		final boolean isStatic;
	}

	private LibResult doesQtLibOrBinExist(String name, String infix, int majorVersion, int minorVersion,
			int patchVersion, Boolean debugValue) {
		boolean isStatic = false;
		String path = doesQtLibExist(name, infix, majorVersion, minorVersion, patchVersion, null, debugValue, false);
		if (path == null)
			path = doesQtBinExist(name, infix, majorVersion, minorVersion, patchVersion, null, debugValue, false);
		if (path == null) {
			path = doesQtStaticLibExist(name, infix, majorVersion, minorVersion, patchVersion, null, debugValue, false);
			isStatic = path != null;
		}
		return path != null ? new LibResult(path, isStatic) : null;
	}

	private boolean detectQtDsoExistAndSetProperty(String propName, String name, String infix, int majorVersion,
			int minorVersion, int patchVersion, String addQtConfig, String altLibdir) {
		if (addQtConfig != null) {
			if (skippedModules.contains(addQtConfig)) {
				return false;
			} else if (addQtConfig.startsWith("qt3d") && skippedModules.contains("qt3d")) {
				return false;
			} else if (addQtConfig.startsWith("multimedia") && skippedModules.contains("multimedia")) {
				return false;
			} else if (addQtConfig.startsWith("webengine") && skippedModules.contains("webengine")) {
				return false;
			} else if (addQtConfig.startsWith("pdf") && skippedModules.contains("pdf")) {
				return false;
			} else if (addQtConfig.startsWith("quick") && (skippedModules.contains("quick") || skippedModules.contains("qml"))) {
				return false;
			} else if (addQtConfig.startsWith("svg") && skippedModules.contains("svg")) {
				return false;
			} else if ((addQtConfig.equals("opengl") || addQtConfig.startsWith("pdf")) && qtMajorVersion < 6) {
				return false;
			}
		}
		// We detect both non-debug and debug forms manually
		// On Linux the debug and the release are the same DSO name, so we have to look
		// at configuration before detecting which one to detect first
		LibResult dsoPath;
		LibResult dsoDebugPath;
		if (Constants.CONFIG_DEBUG.equals(getConfiguration())) {
			dsoDebugPath = doesQtLibOrBinExist(name, infix, majorVersion, minorVersion, patchVersion, Boolean.TRUE);
			dsoPath = doesQtLibOrBinExist(name, infix, majorVersion, minorVersion, patchVersion, Boolean.FALSE);
			// if(dsoDebugPath != null && dsoDebugPath.equals(dsoPath))
			// dsoPath = null; // same name on linux it can only be one kind
		} else {
			dsoPath = doesQtLibOrBinExist(name, infix, majorVersion, minorVersion, patchVersion, Boolean.FALSE);
			dsoDebugPath = doesQtLibOrBinExist(name, infix, majorVersion, minorVersion, patchVersion, Boolean.TRUE);
			if (dsoDebugPath == null && dsoPath != null && isMinGW) {
				File file = new File(dsoPath.path);
				String _name = file.getName() + ".debug";
				file = new File(file.getParentFile().getParentFile(), "lib");
				file = new File(file, _name);
				if (file.exists()) {
					AntUtil.setProperty(propertyHelper, propName + ".debug.true", "true");
					dsoDebugPath = dsoPath;
				}
			}
			// if(dsoPath != null && dsoPath.equals(dsoDebugPath))
			// dsoDebugPath = null; // same name on linux it can only be one kind
		}

		String dsoDebugFilename = null;
		String sourceValue = null;
		if (dsoDebugPath != null && !dsoDebugPath.isStatic) { // ideally we want to resolve and display the relative
																// path difference
			dsoDebugFilename = new File(dsoDebugPath.path).getName();
			sourceValue = " (found debug DSO: " + dsoDebugFilename + ")";
		}

		mySetProperty(((dsoPath != null || dsoDebugPath == null) ? -1 : 0), propName, sourceValue,
				dsoPath != null ? dsoPath.path : null, true); // silent if null
		if (dsoPath != null) { // we do this because the setting is used for both the Qt library itself and the
								// JNI linkage DSO
			AntUtil.setProperty(propertyHelper, propName + ".true", "true"); // true or not-set
			AntUtil.setProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
		}

		sourceValue = null;
		if (Constants.CONFIG_DEBUG_AND_RELEASE.equals(getConfiguration()) && dsoPath == null && dsoDebugPath != null
				&& !dsoDebugPath.isStatic)
			sourceValue = " (no release DSO found)";
		mySetProperty(((dsoPath != null || dsoDebugPath == null) ? 0 : -1), propName + ".debug", sourceValue,
				dsoDebugPath != null ? dsoDebugPath.path : null, true); // silent unless dsoPath==null
		if (dsoDebugPath != null) { // we do this because the setting is used for both the Qt library itself and the
									// JNI linkage DSO
			AntUtil.setProperty(propertyHelper, propName + ".debug.true", "true"); // true or not-set
			AntUtil.setProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
		}
		if ((dsoDebugPath != null && dsoDebugPath.isStatic) || (dsoPath != null && dsoPath.isStatic)) {
			AntUtil.setProperty(propertyHelper, propName + ".staticlib", "true");
			if (dsoPath != null && dsoPath.path != null && dsoDebugPath == null) {
				if (dsoPath.path.endsWith(".a")) {
					Path path = new File(dsoPath.path).toPath();
					File link = new File(dsoPath.path.substring(0, dsoPath.path.length() - 2) + "_debug.a");
					if (!link.exists()) {
						try {
							Files.createLink(link.toPath(), path);
						} catch (IOException e) {
							getProject().log(this, "unable to create symbolic link to " + link, e, Project.MSG_WARN);
						}
					}
				}
			}
		}

		return dsoPath != null || dsoDebugPath != null;
	}

	private boolean detectQtDsoExistAndSetProperty(String propName, String name, String infix, int majorVersion,
			int minorVersion, int patchVersion, String addQtConfig) {
		return detectQtDsoExistAndSetProperty(propName, name, infix, majorVersion, minorVersion, patchVersion,
				addQtConfig, null);
	}

	public static boolean findInString(String haystack, String needle, char delimChar) {
		final int nLen = needle.length();
		final int hLen = haystack.length();
		int o = 0;
		boolean found = false;
		while (o < hLen) {
			int stringOffset = haystack.indexOf(needle, o);
			if (stringOffset < 0)
				break;
			if (stringOffset == 0 || haystack.charAt(stringOffset - 1) == delimChar) {
				if (hLen <= stringOffset + nLen || haystack.charAt(stringOffset + nLen) == delimChar) {
					// found
					found = true;
					break;
				}
			}
			o = stringOffset + nLen;
		}
		return found;
	}

	private String decideQtLibDso(String attrName, String name, String infix, int majorVersion, int minorVersion,
			int patchlevelVersion, boolean verboseFlag) {
		String path = doesQtLibExist(name, infix, majorVersion, minorVersion, patchlevelVersion, null, Boolean.FALSE,
				false);
		if (verboseFlag && path != null)
			getProject().log(this, attrName + ": " + path, Project.MSG_INFO);
		return path;
	}

	// unused
	@SuppressWarnings("unused")
	private String decideQtLibDso(String attrName, String name, String infix, int majorVersion, int minorVersion,
			int patchlevelVersion) {
		boolean verbose = getVerboseLevel() > 0;
		return decideQtLibDso(attrName, name, infix, majorVersion, minorVersion, patchlevelVersion, verbose);
	}

	// unused
	@SuppressWarnings("unused")
	private String decideQtLibDso(String attrName, String name, String infix, int[][] tryVersionA,
			Boolean verboseBoolean) {
		boolean thisVerbose = getVerboseLevel() > 0;
		if (verboseBoolean != null)
			thisVerbose = verboseBoolean.booleanValue();

		if (tryVersionA == null)
			return decideQtLibDso(attrName, name, infix, -1, -1, -1, thisVerbose); // run at least once

		String rv = null;
		for (int[] tryVersion : tryVersionA) {
			if (tryVersion != null && tryVersion.length == 3)
				rv = decideQtLibDso(attrName, name, infix, tryVersion[0], tryVersion[1], tryVersion[2], thisVerbose);
			else
				rv = decideQtLibDso(attrName, name, infix, -1, -1, -1, thisVerbose);
			if (rv != null)
				return rv;
		}
		return rv;
	}

	private void analyzeLibinfix() {
		if (qtMajorVersion > 4) {
			File libraryPath;
			if (OSInfo.crossOS() == OSInfo.OS.Windows) {
				libraryPath = new File(AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR));
			} else {
				libraryPath = new File(AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR));
			}

			String libName = "Qt" + qtMajorVersion + "Gui";

			String[] libSuffixes;
			boolean isAndroid = false;
			switch (OSInfo.crossOS()) {
			case Windows:
				libSuffixes = new String[] { "d.dll", ".dll" };
				break;
			case MacOS:
				libSuffixes = new String[] { "_debug." + qtVersion + ".dylib", "." + qtVersion + ".dylib" };
				libName = "lib" + libName;
				break;
			case Android:
				libSuffixes = new String[] { ".so" };
				libName = "lib" + libName;
				isAndroid = true;
				break;
			case IOS:
				libSuffixes = new String[] { ".a" };
				libName = "lib" + libName;
				break;
			case Linux:
			case Unknown:
			default:
				libSuffixes = new String[] { ".so." + qtVersion };
				libName = "lib" + libName;
				break;
			}

			String availableAbis = "";
			if(libraryPath.isDirectory()) {
				loop: for (String libSuffix : libSuffixes) {
					if (!new File(libraryPath, libName + libSuffix).exists()) {
						for (File libfile : libraryPath.listFiles()) {
							String fileName = libfile.getName();
							if (libfile.isFile() && fileName.startsWith(libName) && fileName.endsWith(libSuffix)) {
								fileName = fileName.substring(0, fileName.length() - libSuffix.length());
								fileName = fileName.substring(libName.length());
								if (isAndroid) {
									fileName = fileName.substring(1);
									availableAbis += " " + fileName;
									androidAbis.add(fileName);
								} else {
									libInfix = fileName;
									break loop;
								}
							} else if (OSInfo.os() == OSInfo.OS.MacOS && libfile.isDirectory()
									&& fileName.equals("QtGui.framework")) {
								mySetProperty(-1, Constants.MAC_OS_USE_FRAMEWORK, " (as detected in lib directory)", "true", true);
								useQtFramework = true;
								break loop;
							}
						}
					}
				}
			}
			if (isAndroid) {
				mySetProperty(-1, Constants.QTJAMBI_ABIS, " (taken from library name)", availableAbis.trim(), true);
			}
			mySetProperty(-1, Constants.QTJAMBI_QT_LIBINFIX, " (taken from library name)", libInfix, true);
		}
	}

	public static void setTargetProperties(Properties targetProperties) {
		InitializeBuildTask.targetProperties = targetProperties;
	}
}
