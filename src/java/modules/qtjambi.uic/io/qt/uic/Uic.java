/****************************************************************************
**
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
package io.qt.uic;

import java.util.Arrays;

import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QTextStream;
import io.qt.core.QXmlStreamAttributes;
import io.qt.core.QXmlStreamReader;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomCustomWidgets;
import io.qt.uic.ui4.DomHeader;
import io.qt.uic.ui4.DomInclude;
import io.qt.uic.ui4.DomIncludes;
import io.qt.uic.ui4.DomUI;

/**
 * @hidden
 */
public class Uic {
	public Uic(Driver driver) {
		this.drv = driver;
		this.opt = driver.option();
	}

	public boolean printDependencies() {
		String fileName = opt.inputFile;

		QIODevice f;
	    if (fileName.isEmpty())
	    	f = QIODevice.fromInputStream(System.in);
	    else {
	        f = new QFile(fileName);
	        if (!f.open(QIODevice.OpenModeFlag.ReadOnly))
	            return false;
	    }

	    DomUI ui = null;
	    {
	        QXmlStreamReader reader = new QXmlStreamReader(f);
	        ui = parseUiFile(reader);
	        if (ui==null)
	            return false;
	    }

	    DomIncludes includes = ui.elementIncludes();
	    if (includes!=null) {
	        for (DomInclude incl : includes.elementInclude()) {
	            String file = incl.text();
	            if (file==null || file.isEmpty())
	                continue;
	            System.out.println(file);
	        }
	    }

	    DomCustomWidgets customWidgets = ui.elementCustomWidgets();
	    if (customWidgets!=null) {
	        for (DomCustomWidget customWidget : customWidgets.elementCustomWidget()) {
	        	DomHeader header = customWidget.elementHeader();
	            if (header!=null) {
	                String file = header.text();
	                if (file==null || file.isEmpty())
	                    continue;
	                System.out.println(file);
	            }
	        }
	    }
	    ui.dispose();
	    return true;
	}

	public Driver driver()
    { return drv; }

	public QTextStream output()
    { return m_output; }

	public Option option()
    { return opt; }

	public String pixmapFunction()
    { return pixFunction; }

	public void setPixmapFunction(String f)
    { pixFunction = f; }

	public DatabaseInfo databaseInfo()
    { return info; }

	public CustomWidgetsInfo customWidgetsInfo()
    { return cWidgetsInfo; }

	public boolean write(QIODevice in, String outputDir, String language) {
		DomUI ui = null;
	    {
	        QXmlStreamReader reader = new QXmlStreamReader();
	        reader.setDevice(in);
	        ui = parseUiFile(reader);
	    }

	    if (ui==null)
	        return false;

	    double version = Double.parseDouble(ui.attributeVersion());
	    if (version < 4.0) {
	    	System.err.println("uic: File generated with too old version of Qt Designer");
	        return false;
	    }

//	    String language = ui.attributeLanguage();
	    driver().setUseIdBasedTranslations(ui.attributeIdbasedtr());

	    /*if (language!=null && !language.isEmpty() && (!language.equalsIgnoreCase("java") || !language.equalsIgnoreCase("jambi"))) {
	    	System.err.println("uic: File is not a \"java\" ui file, language="+language);
	        return false;
	    }*/

	    if (ui==null || ui.elementWidget()==null)
	        return false;
	    
	    String qualifiedClassName = ui.elementClass();
	    String[] namespaceList = qualifiedClassName.split("::");
	    String className;
	    if (namespaceList.length>1) {
	    	className = namespaceList[namespaceList.length-1];
	    	drv.option().targetPackage = String.join(".", Arrays.copyOf(namespaceList, namespaceList.length-1));
	    }else {
	    	className = qualifiedClassName;
	    }
	    
	    boolean isKotlin = "kotlin".equals(language);
	    
	    String fileSuffix;
	    if(isKotlin)
	    	fileSuffix = ".kt";
	    else
	    	fileSuffix = ".java";
	    QTextStream oldOutput = m_output;

        QFile outputFile = null;
        QIODevice iodevice;
        if (outputDir==null || outputDir.isEmpty()) {
        	iodevice = QIODevice.fromOutputStream(System.out);
        }else {
        	QFileInfo targetFile = new QFileInfo(outputDir + "/" + drv.option().targetPackage.replace('.', '/') + "/" + drv.option().prefix + className + drv.option().postfix + fileSuffix);
        	targetFile.absoluteDir().mkpath(".");
        	iodevice = outputFile = new QFile(targetFile.absoluteFilePath());
        }
    	if(iodevice.open(QIODevice.OpenModeFlag.WriteOnly)) {
    		m_output = new QTextStream(iodevice);
    	}else if(outputFile!=null) {
			outputFile.remove();
			if(outputFile.open(QIODevice.OpenModeFlag.WriteOnly))
				m_output = new QTextStream(iodevice);
			else
				return false;
    	}else {
    		return false;
    	}

	    pixFunction = ui.elementPixmapFunction();
	    if ("QPixmap::fromMimeSource".equals(pixFunction)
	        || "qPixmapFromMimeSource".equals(pixFunction)) {
	    	System.err.println(opt.messagePrefix() + "Warning: Obsolete pixmap function '" + pixFunction + "' specified in the UI file.");
	        pixFunction = null;
	    }

	    info.acceptUI(ui);
	    cWidgetsInfo.acceptUI(ui);
	    
	    if(isKotlin)
	    	new io.qt.uic.kotlin.WriteImports(this).acceptUI(ui);
	    else
	    	new io.qt.uic.java.WriteImports(this).acceptUI(ui);
	    
        new Validator(this).acceptUI(ui);
        if(isKotlin)
        	new io.qt.uic.kotlin.WriteDeclaration(this).acceptUI(ui);
        else
        	new io.qt.uic.java.WriteDeclaration(this).acceptUI(ui);
        
        m_output.dispose();
        iodevice.close();
        m_output = oldOutput;
        
        if(outputDir!=null 
        		&& !drv.option().noShellClass
        		&& ((drv.option().prefix!=null && !drv.option().prefix.isEmpty()) 
        				|| (drv.option().postfix!=null && !drv.option().postfix.isEmpty()))) {
        	iodevice = outputFile = new QFile(outputDir + "/" + drv.option().targetPackage.replace('.', '/') + "/" + className + fileSuffix);
        	if(!outputFile.exists() || drv.option().forceOutput) {
	        	if(iodevice.open(QIODevice.OpenModeFlag.WriteOnly)) {
	        		m_output = new QTextStream(iodevice);
	        	}else {
	        		return false;
	        	}
	        	if(isKotlin)
	        		new io.qt.uic.kotlin.WriteClass(this, className).acceptUI(ui);
	        	else
	        		new io.qt.uic.java.WriteClass(this, className).acceptUI(ui);
	            m_output.dispose();
	            outputFile.close();
	        	m_output = oldOutput;
        	}
        	outputFile.dispose();
        }
        return true;
	}
	

    public boolean isButton(String className) {
    	return customWidgetsInfo().extendsOneOf(className, Arrays.asList(
    	        "QRadioButton", "QToolButton",
    	        "QCheckBox", "QPushButton",
    	        "QCommandLinkButton"
    	    ));
    }
    
    public boolean isContainer(String className) {
    	return customWidgetsInfo().extendsOneOf(className, Arrays.asList(
    	        "QStackedWidget", "QToolBox",
    	        "QTabWidget", "QScrollArea",
    	        "QMdiArea", "QWizard", "QDockWidget"
    	    ));
    }
    
    public boolean isMenu(String className) {
    	return customWidgetsInfo().extendsOneOf(className, Arrays.asList(
    	        "QMenu", "QPopupMenu"
    		    ));
    }

    private static double versionFromUiAttribute(QXmlStreamReader reader)
    {
        QXmlStreamAttributes attributes = reader.attributes();
        String versionAttribute = "version";
        if (!attributes.hasAttribute(versionAttribute))
            return 4.0;
        String version = attributes.value(versionAttribute);
        try {
			return Double.parseDouble(version);
		} catch (NumberFormatException e) {
            return 4.0;
		}
    }
    
    private DomUI parseUiFile(QXmlStreamReader reader) {
    	DomUI ui = null;

        while (!reader.atEnd()) {
            if (reader.readNext() == QXmlStreamReader.TokenType.StartElement) {
                if (reader.name().equalsIgnoreCase("ui")
                    && ui==null) {
                    double version = versionFromUiAttribute(reader);
                    if (version < 4.0) {
                    	System.err.println(String.format("QtJambi UIC: File generated with too old version of Qt Designer (%1$s)", version));
                        return null;
                    }

                    ui = new DomUI();
                    ui.read(reader);
                } else {
                    reader.raiseError("Unexpected element " + reader.name().toString());
                }
            }
        }
        if (reader.hasError()) {
            if(ui!=null)
            	ui.dispose();
            ui = null;
            System.err.println(String.format("QtJambi UIC: Error in line %1$s, column %2$s : %3$s", reader.lineNumber(), reader.columnNumber(), reader.errorString()));
        }

        return ui;
    }

    private final Driver drv;
    final QTextStream m_stdout;
    {
    	QIODevice qiodevice = QIODevice.fromOutputStream(System.out);
    	qiodevice.open(QIODevice.OpenModeFlag.WriteOnly);
    	m_stdout = new QTextStream(qiodevice);
    }
    private QTextStream m_output = m_stdout;
    private final Option opt;
    private final DatabaseInfo info = new DatabaseInfo();
    private final CustomWidgetsInfo cWidgetsInfo = new CustomWidgetsInfo();
	private String pixFunction;
}
