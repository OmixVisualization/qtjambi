/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.uic;

import io.qt.QtUtilities;
import io.qt.core.QCommandLineOption;
import io.qt.core.QCommandLineParser;
import io.qt.core.QCoreApplication;
import io.qt.core.QDir;
import io.qt.core.QList;
import io.qt.core.QScopeGuard;
import io.qt.core.Qt;

public class Main {

	public static void main(String[] args) {
		Qt.qSetGlobalQHashSeed(0);
		QCoreApplication.setApplicationName("uic");
		QCoreApplication.setApplicationVersion(QtUtilities.qtjambiVersion().toString());
		QCoreApplication.initialize(args);
		QCoreApplication.setApplicationName("QtJambi UIC");
		try(QScopeGuard guard = new QScopeGuard(QCoreApplication::shutdown)){
			Driver driver = new Driver();
			QCommandLineParser parser = new QCommandLineParser();
		    parser.setSingleDashWordOptionMode(QCommandLineParser.SingleDashWordOptionMode.ParseAsLongOptions);
		    parser.setApplicationDescription(String.format("QtJambi User Interface Compiler version %1$s", QCoreApplication.applicationVersion()));
		    parser.addHelpOption();
		    parser.addVersionOption();
	
		    QCommandLineOption dependenciesOption = new QCommandLineOption(QList.of("d", "dependencies"), "dependencies");
		    dependenciesOption.setDescription("Display the dependencies.");
		    parser.addOption(dependenciesOption);
	
		    QCommandLineOption forceOption = new QCommandLineOption(QList.of("f", "force"), "force");
		    forceOption.setDescription("Force all source files to be written.");
		    parser.addOption(forceOption);
		    
		    QCommandLineOption outputOption = new QCommandLineOption(QList.of("o", "output"), "output");
		    outputOption.setDescription("Place the output into <dir>");
		    outputOption.setValueName("dir");
		    parser.addOption(outputOption);
		    
		    QCommandLineOption packageOption = new QCommandLineOption(QList.of("p", "package"), "package");
		    packageOption.setDescription("Place the output into <package>");
		    packageOption.setValueName("package");
		    parser.addOption(packageOption);
	
		    QCommandLineOption noAutoConnectionOption = new QCommandLineOption(QList.of("a", "no-autoconnection"));
		    noAutoConnectionOption.setDescription("Do not generate a call to QMetaObject.connectSlotsByName().");
		    parser.addOption(noAutoConnectionOption);
	
		    QCommandLineOption postfixOption = new QCommandLineOption("postfix");
		    postfixOption.setDescription("Postfix to add to all generated classnames.");
		    postfixOption.setValueName("postfix");
		    parser.addOption(postfixOption);
	
		    QCommandLineOption translateOption = new QCommandLineOption(QList.of("tr", "translate"));
		    translateOption.setDescription("Use <function> for i18n.");
		    translateOption.setValueName("function");
		    parser.addOption(translateOption);
	
		    QCommandLineOption importOption = new QCommandLineOption(QList.of("i", "imports"));
		    importOption.setDescription("Add imports to comma-separated packages and/or classes.");
		    importOption.setValueName("imports");
		    parser.addOption(importOption);
	
		    QCommandLineOption generatorOption = new QCommandLineOption(QList.of("g", "generator"));
		    generatorOption.setDescription("Select generator.");
		    generatorOption.setValueName("c++|python|java");
		    generatorOption.setDefaultValue("java");
		    generatorOption.setFlags(QCommandLineOption.Flag.HiddenFromHelp);
		    parser.addOption(generatorOption);
	
		    QCommandLineOption connectionsOption = new QCommandLineOption(QList.of("c", "connections"));
		    connectionsOption.setDescription("Connection syntax.");
		    connectionsOption.setValueName("pmf|string");
		    parser.addOption(connectionsOption);
	
		    QCommandLineOption idBasedOption = new QCommandLineOption("idbased");
		    idBasedOption.setDescription("Use id based function for i18n");
		    parser.addOption(idBasedOption);
	
		    parser.addPositionalArgument("[uifile]", "Input file (*.ui), otherwise stdin.");
	
		    parser.process(QCoreApplication.arguments());
		    
		    driver.option().dependencies = parser.isSet(dependenciesOption);
		    driver.option().outputDir = QDir.fromNativeSeparators(parser.value(outputOption));
		    driver.option().targetPackage = parser.value(packageOption).replace('/', '.');
		    driver.option().autoConnection = !parser.isSet(noAutoConnectionOption);
		    driver.option().idBased = parser.isSet(idBasedOption);
		    driver.option().postfix = parser.value(postfixOption);
		    driver.option().translateFunction = parser.value(translateOption);
		    driver.option().imports = QDir.fromNativeSeparators(parser.value(importOption));
		    driver.option().forceOutput = parser.isSet(forceOption);
		    if (parser.isSet(connectionsOption)) {
		        String value = parser.value(connectionsOption);
		        if (value.equals("pmf"))
		            driver.option().forceMemberFnPtrConnectionSyntax = true;
		        else if (value.equals("string"))
		            driver.option().forceStringConnectionSyntax = true;
		    }
	
		    if (parser.isSet(generatorOption)) {
		        if (parser.value(generatorOption).compareTo("python") == 0) {
		        	System.err.println("QtJambi UIC could not generate python code. Use Qt's native UIC tool instead.");
		        	System.exit(-1);
		        }
		        if (parser.value(generatorOption).compareTo("c++") == 0
		        		|| parser.value(generatorOption).compareTo("cpp") == 0
		        		|| parser.value(generatorOption).compareTo("cplusplus") == 0)
		        	System.err.println("QtJambi UIC could not generate c++ code. Use Qt's native UIC tool instead.");
		        	System.exit(-1);
		    }
	
		    String inputFile = null;
		    if (!parser.positionalArguments().isEmpty())
		        inputFile = parser.positionalArguments().at(0);
	
		    if (driver.option().dependencies) {
		        driver.printDependencies(inputFile);
		        return;
		    }
	
		    driver.uic(inputFile, driver.option().outputDir);
		}
	}

}
	