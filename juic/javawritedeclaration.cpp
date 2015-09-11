/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "javawritedeclaration.h"
#include "javawriteinitialization.h"
#include "driver.h"
#include "ui4.h"
#include "uic.h"
#include "databaseinfo.h"
#include "customwidgetsinfo.h"
#include "javautils.h"

#include <QTextStream>
#include <QSet>

#define JUIC_NO_EMBEDDED_ICON

extern bool generate_java_main_function;

namespace Java {

WriteDeclaration::WriteDeclaration(Uic *uic)
    : driver(uic->driver()), output(uic->output()), option(uic->option())
{
    this->uic = uic;
}

void WriteDeclaration::acceptUI(DomUI *node)
{
    QString qualifiedClassName = node->elementClass() + option.postfix;
    QString className = qualifiedClassName;

    QString varName = driver->findOrInsertWidget(node->elementWidget());
    QString widgetName = node->elementWidget()->attributeClass();

    QString package = driver->option().javaPackage;

    if (!package.isEmpty())
        output << "package " << package << ";\n\n";

    output << "import org.qtjambi.qt.core.*;\n"
           << "import org.qtjambi.qt.gui.*;\n"
           << "import org.qtjambi.qt.widgets.*;\n"
           << "\n";

    if (DomCustomWidgets *customWidgets = node->elementCustomWidgets()) {
        QList<DomCustomWidget *> list = customWidgets->elementCustomWidget();
        QSet<QString> usedImports;
        usedImports.insert("org.qtjambi.qt.core");
        usedImports.insert("org.qtjambi.qt.gui");
        usedImports.insert("org.qtjambi.qt.widgets");

        for (int i=0; i<list.size(); ++i) {
            DomCustomWidget *customWidget = list.at(i);
            if (customWidget != 0 && customWidget->hasElementHeader()) {
                QString import = customWidget->elementHeader()->text();
                if (!import.isEmpty() && !usedImports.contains(import)) {
                    output << "import " << import << ".*;\n";
                    usedImports.insert(import);
                }
            }
        }

        output << "\n";
    }

    QString widgetClassName = node->elementWidget()->attributeClass();
    output << "public class " << option.prefix << className
           << " implements org.qtjambi.qt.QUiForm<" << widgetClassName << ">" << "\n"
           << "{\n";

    TreeWalker::acceptWidget(node->elementWidget());

    output << "\n"
           << option.indent << "public " << option.prefix << className << "() { super(); }\n"
            << "\n";

    WriteInitialization(uic).acceptUI(node);

    if (generate_java_main_function) {
        QString uiName = option.prefix + className;
        output << "    public static void main(String args[]) {" << endl
               << "        QApplication.initialize(args);" << endl
               << "        " << uiName << " ui = new " << uiName << "();" << endl
               << "        " << widgetName << " widget = new " << widgetName << "();" << endl
               << "        ui.setupUi(widget);" << endl
               << "        widget.show();" << endl
               << "        QApplication.exec();" << endl
               << "    }" << endl;
    }

    output << "}\n\n";
}

void WriteDeclaration::acceptWidget(DomWidget *node)
{
    QString className = QLatin1String("QWidget");
    if (node->hasAttributeClass())
        className = node->attributeClass();

    output << option.indent << "public " << uic->customWidgetsInfo()->realClassName(className) << " "
           << escapeVariableName(driver->findOrInsertWidget(node)) << ";\n";

    TreeWalker::acceptWidget(node);
}

void WriteDeclaration::acceptLayout(DomLayout *node)
{
    QString className = QLatin1String("QLayout");
    if (node->hasAttributeClass())
        className = node->attributeClass();

    output << option.indent << "public " << className << " "
           << escapeVariableName(driver->findOrInsertLayout(node)) << ";\n";

    TreeWalker::acceptLayout(node);
}

void WriteDeclaration::acceptSpacer(DomSpacer *node)
{
    output << option.indent << "public QSpacerItem "
           << escapeVariableName(driver->findOrInsertSpacer(node)) << ";\n";

    TreeWalker::acceptSpacer(node);
}

void WriteDeclaration::acceptActionGroup(DomActionGroup *node)
{
    output << option.indent << "public QActionGroup "
           << escapeVariableName(driver->findOrInsertActionGroup(node)) << ";\n";

    TreeWalker::acceptActionGroup(node);
}

void WriteDeclaration::acceptAction(DomAction *node)
{
    output << option.indent << "public QAction "
           << escapeVariableName(driver->findOrInsertAction(node)) << ";\n";

    TreeWalker::acceptAction(node);
}

} // namespace Java
