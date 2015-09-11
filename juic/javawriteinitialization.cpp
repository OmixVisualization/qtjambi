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

#include "javawriteinitialization.h"
#include "driver.h"
#include "ui4.h"
#include "javautils.h"
#include "utils.h"
#include "uic.h"
#include "databaseinfo.h"
#include "globaldefs.h"

#include <QTextStream>
#include <QtDebug>

#include <limits.h>


static QString enumify(const QString &value, const char *enumName)
{
    bool isNumber = false;
    int i = value.toInt(&isNumber);
    if (isNumber)
        return QString::fromLatin1("%1.resolve(%2)").arg(enumName).arg(i);
    return QString::fromLatin1("%1.%2").arg(enumName).arg(value);
}



namespace Java {

WriteInitialization::WriteInitialization(Uic *uic)
    : driver(uic->driver()), output(uic->output()), option(uic->option()),
      m_defaultMargin(INT_MIN), m_defaultSpacing(INT_MIN),
      delayedOut(&m_delayedInitialization, QIODevice::WriteOnly),
      refreshOut(&m_refreshInitialization, QIODevice::WriteOnly),
      actionOut(&m_delayedActionInitialization, QIODevice::WriteOnly)
{
    this->uic = uic;
}

void WriteInitialization::acceptUI(DomUI *node)
{
    m_registeredImages.clear();
    m_actionGroupChain.push(0);
    m_widgetChain.push(0);
    m_layoutChain.push(0);

    acceptLayoutDefault(node->elementLayoutDefault());
    acceptLayoutFunction(node->elementLayoutFunction());

    if (node->elementCustomWidgets())
        TreeWalker::acceptCustomWidgets(node->elementCustomWidgets());

    if (node->elementImages())
        TreeWalker::acceptImages(node->elementImages());

    m_stdsetdef = true;
    if (node->hasAttributeStdSetDef())
        m_stdsetdef = node->attributeStdSetDef();

    QString className = node->elementClass() + option.postfix;
    m_generatedClass = className;

    QString varName = escapeVariableName(driver->findOrInsertWidget(node->elementWidget()));
    m_registeredWidgets.insert(varName, node->elementWidget()); // register the main widget

    QString widgetClassName = node->elementWidget()->attributeClass();

    output << option.indent << "public void " << "setupUi(" << widgetClassName << " " << varName << ")\n"
           << option.indent << "{\n";

    // Increase indent...
    QString oldIndent = option.indent;
    option.indent += QString(4, ' ');

    acceptWidget(node->elementWidget());

    for (int i=0; i<m_buddies.size(); ++i) {
        const Buddy &b = m_buddies.at(i);

        if (!m_registeredWidgets.contains(b.objName)) {
            fprintf(stderr, "'%s' isn't a valid widget\n", b.objName.toLatin1().data());
            continue;
        } else if (!m_registeredWidgets.contains(b.buddy)) {
            fprintf(stderr, "'%s' isn't a valid widget\n", b.buddy.toLatin1().data());
            continue;
        }
        output << option.indent << b.objName << ".setBuddy(" << b.buddy << ");\n";
    }

    if (node->elementTabStops())
        acceptTabStops(node->elementTabStops());

    if (m_delayedActionInitialization.size())
        output << "\n" << m_delayedActionInitialization;

    output << option.indent << "retranslateUi(" << varName << ");\n";

    if (node->elementConnections())
        acceptConnections(node->elementConnections());

    if (!m_delayedInitialization.isEmpty())
        output << "\n" << m_delayedInitialization << "\n";

    if (option.autoConnection)
        output << "\n" << option.indent << varName << ".connectSlotsByName();\n";

    option.indent = oldIndent;

    output << option.indent << "} // setupUi\n\n";

    output << option.indent << "void " << "retranslateUi(" << widgetClassName << " " << varName << ")\n"
           << option.indent << "{\n"
           << m_refreshInitialization
           << option.indent << "} // retranslateUi\n\n";

    m_layoutChain.pop();
    m_widgetChain.pop();
    m_actionGroupChain.pop();
}

void WriteInitialization::acceptWidget(DomWidget *node)
{
    QString className = node->attributeClass();
    QString varName = escapeVariableName(driver->findOrInsertWidget(node));
    m_registeredWidgets.insert(varName, node); // register the current widget

    QString parentWidget, parentClass;
    if (m_widgetChain.top()) {
        parentWidget = escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top()));
        parentClass = m_widgetChain.top()->attributeClass();
    }

    QString savedParentWidget = parentWidget;

    if (uic->isContainer(parentClass))
        parentWidget.clear();

    if (m_widgetChain.size() != 1)
        output << option.indent << varName << " = new " << uic->customWidgetsInfo()->realClassName(className) << "(" << parentWidget << ");\n";

    parentWidget = savedParentWidget;

    if (uic->customWidgetsInfo()->extends(className, QLatin1String("QComboBox"))) {
        initializeComboBox(node);
    } else if (uic->customWidgetsInfo()->extends(className, QLatin1String("QListWidget"))) {
        initializeListWidget(node);
    } else if (uic->customWidgetsInfo()->extends(className, QLatin1String("QTreeWidget"))) {
        initializeTreeWidget(node);
    } else if (uic->customWidgetsInfo()->extends(className, QLatin1String("QTableWidget"))) {
        initializeTableWidget(node);
    }

    if (uic->isButton(className)) {
        QHash<QString, DomProperty*> attributes = propertyMap(node->elementAttribute());
        if (DomProperty *prop = attributes.value(QLatin1String("buttonGroup"))) {
            QString groupName = toString(prop->elementString());
            if (!m_buttonGroups.contains(groupName)) {
                m_buttonGroups.insert(groupName, escapeVariableName(driver->findOrInsertName(groupName)));
                QString g = m_buttonGroups.value(groupName);
                output << option.indent << "QButtonGroup " << g << " = new QButtonGroup(" << m_generatedClass << ");\n";
            }

            QString g = m_buttonGroups.value(groupName);
            output << option.indent << g << ".addButton(" << varName << ");\n";
        }
    }

    writeProperties(varName, className, node->elementProperty());

    if (uic->customWidgetsInfo()->extends(className, QLatin1String("QMenu")) && parentWidget.size()) {
        initializeMenu(node, parentWidget);
    }

    if (node->elementLayout().isEmpty())
        m_layoutChain.push(0);

    m_widgetChain.push(node);
    m_layoutChain.push(0);
    TreeWalker::acceptWidget(node);
    m_layoutChain.pop();
    m_widgetChain.pop();

    QHash<QString, DomProperty*> attributes = propertyMap(node->elementAttribute());

    QString title = QLatin1String("Page");
    if (DomProperty *ptitle = attributes.value(QLatin1String("title"))) {
        title = toString(ptitle->elementString());
    }

    QString label = QLatin1String("Page");
    if (DomProperty *plabel = attributes.value(QLatin1String("label"))) {
        label = toString(plabel->elementString());
    }

    int id = -1;
    if (DomProperty *pid = attributes.value(QLatin1String("id"))) {
        id = pid->elementNumber();
    }

    if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QMainWindow"))
            || uic->customWidgetsInfo()->extends(parentClass, QLatin1String("Q3MainWindow"))) {

        if (uic->customWidgetsInfo()->extends(className, QLatin1String("QMenuBar"))) {
            if (!uic->customWidgetsInfo()->extends(parentClass, QLatin1String("Q3MainWindow")))
                output << option.indent << parentWidget << ".setMenuBar(" << varName <<");\n";
        } else if (uic->customWidgetsInfo()->extends(className, QLatin1String("QToolBar"))) {
            QString area;
            if (DomProperty *pstyle = attributes.value(QLatin1String("toolBarArea"))) {
                area += pstyle->elementEnum();
                if (area.isEmpty())
                    area += QString::number(pstyle->elementNumber());
            }

            area = enumify(area, "org.qtjambi.qt.core.Qt.ToolBarArea");
            output << option.indent << parentWidget << ".addToolBar(" << area << ", "
                   << varName << ");\n";
        } else if (uic->customWidgetsInfo()->extends(className, QLatin1String("QDockWidget"))) {
            QString area;
            if (DomProperty *pstyle = attributes.value(QLatin1String("dockWidgetArea"))) {
                area += QString::number(pstyle->elementNumber());
            }

            area = enumify(area, "org.qtjambi.qt.core.Qt.DockWidgetArea");
            output << option.indent << parentWidget << ".addDockWidget(" << area << ", "
                   << varName << ");\n";
        } else if (uic->customWidgetsInfo()->extends(className, QLatin1String("QStatusBar"))) {
            output << option.indent << parentWidget << ".setStatusBar(" << varName << ");\n";
        } else if (className == QLatin1String("QWidget")) {
            output << option.indent << parentWidget << ".setCentralWidget(" << varName << ");\n";
        }
    }

    if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QStackedWidget"))) {
        output << option.indent << parentWidget << ".addWidget(" << varName << ");\n";
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QToolBar"))) {
        output << option.indent << parentWidget << ".addWidget(" << varName << ");\n";
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("Q3WidgetStack"))) {
        output << option.indent << parentWidget << ".addWidget(" << varName << ", " << id << ");\n";
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QDockWidget"))) {
        output << option.indent << parentWidget << ".setWidget(" << varName << ");\n";
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QSplitter"))) {
        output << option.indent << parentWidget << ".addWidget(" << varName << ");\n";
    } else if(uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QScrollArea"))) {
        output << option.indent << parentWidget << ".setWidget(" << varName << ");\n";
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QToolBox"))) {
        QString icon;
        if (DomProperty *picon = attributes.value(QLatin1String("icon"))) {
            icon += QLatin1String(", ") + pixCall(picon);
        }

        output << option.indent << parentWidget << ".addItem(" << varName << icon << ", " << trCall(label) << ");\n";

        refreshOut << option.indent << parentWidget << ".setItemText("
                   << parentWidget << ".indexOf(" << varName << "), " << trCall(label) << ");\n";

        if (DomProperty *ptoolTip = attributes.value(QLatin1String("toolTip"))) {
            refreshOut << option.indent << parentWidget << ".setItemToolTip("
                       << parentWidget << ".indexOf(" << varName << "), " << trCall(ptoolTip->elementString()) << ");\n";
        }
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("QTabWidget"))) {
        QString icon;
        if (DomProperty *picon = attributes.value(QLatin1String("icon"))) {
            icon += QLatin1String(", ") + pixCall(picon);
        }

        output << option.indent << parentWidget << ".addTab(" << varName << icon << ", " << trCall(title) << ");\n";

        refreshOut << option.indent << parentWidget << ".setTabText("
                   << parentWidget << ".indexOf(" << varName << "), " << trCall(title) << ");\n";

        if (DomProperty *ptoolTip = attributes.value(QLatin1String("toolTip"))) {
            refreshOut << option.indent << parentWidget << ".setTabToolTip("
                       << parentWidget << ".indexOf(" << varName << "), " << trCall(ptoolTip->elementString()) << ");\n";
        }
    } else if (uic->customWidgetsInfo()->extends(parentClass, QLatin1String("Q3Wizard"))) {
        output << option.indent << parentWidget << ".addPage(" << varName << ", " << trCall(title) << ");\n";

        refreshOut << option.indent << parentWidget << ".setTitle("
                   << varName << ", " << trCall(title) << ");\n";

    }

    if (node->elementLayout().isEmpty())
        m_layoutChain.pop();
}

void WriteInitialization::acceptLayout(DomLayout *node)
{
    QString className = node->attributeClass();
    QString varName = escapeVariableName(driver->findOrInsertLayout(node));

    QHash<QString, DomProperty*> properties = propertyMap(node->elementProperty());

    bool isGroupBox = false;

    if (m_widgetChain.top()) {
        QString parentWidget = m_widgetChain.top()->attributeClass();

        if (!m_layoutChain.top() && (uic->customWidgetsInfo()->extends(parentWidget, QLatin1String("Q3GroupBox"))
                        || uic->customWidgetsInfo()->extends(parentWidget, QLatin1String("Q3ButtonGroup")))) {
            QString parent = escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top()));

            isGroupBox = true;

            // special case for group box

            int margin = m_defaultMargin;
            int spacing = m_defaultSpacing;

            if (properties.contains(QLatin1String("margin")))
                margin = properties.value(QLatin1String("margin"))->elementNumber();

            if (properties.contains(QLatin1String("spacing")))
                spacing = properties.value(QLatin1String("spacing"))->elementNumber();

            output << option.indent << parent << ".setColumnLayout(0, Qt.Vertical);\n";

            if (spacing != INT_MIN) {
                QString value = QString::number(spacing);
                if (!m_spacingFunction.isEmpty() && spacing == m_defaultSpacing)
                    value = m_spacingFunction + QLatin1String("()");

                output << option.indent << parent << ".layout().setSpacing(" << value << ");\n";
            }

            if (margin != INT_MIN) {
                QString value = QString::number(margin);
                if (!m_marginFunction.isEmpty() && margin == m_defaultMargin)
                    value = m_marginFunction + QLatin1String("()");

                output << option.indent << parent << ".layout().setMargin(" << value << ");\n";
            }
        }
    }

    output << option.indent << varName << " = new " << className << "(";

    if (isGroupBox) {
        output << escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top())) << ".layout()";
    } else if (!m_layoutChain.top()) {
        output << escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top()));
    }

    output << ");\n";

    QList<DomProperty*> layoutProperties = node->elementProperty();

    if (isGroupBox) {
        output << option.indent << varName << ".setAlignment(Qt.AlignTop);\n";
    } else {
        int margin = m_defaultMargin;
        int spacing = m_defaultSpacing;

        if (properties.contains(QLatin1String("margin"))) {
            DomProperty *p = properties.value(QLatin1String("margin"));
            Q_ASSERT(p != 0);

            margin = properties.value(QLatin1String("margin"))->elementNumber();
            layoutProperties.removeAt(layoutProperties.indexOf(p));
        }

        if (properties.contains(QLatin1String("spacing"))) {
            DomProperty *p = properties.value(QLatin1String("spacing"));
            Q_ASSERT(p != 0);

            spacing = properties.value(QLatin1String("spacing"))->elementNumber();
            layoutProperties.removeAt(layoutProperties.indexOf(p));
        }

        if (spacing != INT_MIN) {
            QString value = QString::number(spacing);
            if (!m_spacingFunction.isEmpty() && spacing == m_defaultSpacing)
                value = m_spacingFunction + QLatin1String("()");

            output << option.indent << varName << ".setSpacing(" << value << ");\n";
        }

        if (margin != INT_MIN) {
            QString value = QString::number(margin);
            if (!m_marginFunction.isEmpty() && margin == m_defaultMargin)
                value = m_marginFunction + QLatin1String("()");

            output << option.indent << varName << ".setMargin(" << value << ");\n";
        }
    }

    writeProperties(varName, className, layoutProperties);

    m_layoutChain.push(node);
    TreeWalker::acceptLayout(node);
    m_layoutChain.pop();
}

void WriteInitialization::acceptSpacer(DomSpacer *node)
{
    QHash<QString, DomProperty *> properties = propertyMap(node->elementProperty());
    QString varName = escapeVariableName(driver->findOrInsertSpacer(node));

    DomSize *sizeHint = properties.contains(QLatin1String("sizeHint"))
                        ? properties.value(QLatin1String("sizeHint"))->elementSize() : 0;

    QString sizeType = properties.contains(QLatin1String("sizeType"))
        ? properties.value(QLatin1String("sizeType"))->elementEnum()
        : QString::fromLatin1("org.qtjambi.qt.widgets.QSizePolicy.Policy.Expanding");

    QString orientation = properties.contains(QLatin1String("orientation"))
        ? properties.value(QLatin1String("orientation"))->elementEnum() : QString();

    bool isVspacer = orientation.endsWith(QLatin1String("Vertical"));

    if (sizeType.contains(QLatin1String("::"))) {
        QStringList lst = sizeType.split(QLatin1String("::"));
        if (lst.size() == 2 && lst.at(0) == QLatin1String("QSizePolicy")) {
            sizeType = QLatin1String("org.qtjambi.qt.widgets.QSizePolicy.Policy.") + lst.at(1);
        } else {
            fprintf(stderr, "Malformed enum value %s, C++ syntax not allowed\n",
                    qPrintable(sizeType));
            return;
        }
    }

    output << option.indent << varName << " = new QSpacerItem(";

    if (sizeHint)
        output << sizeHint->elementWidth() << ", " << sizeHint->elementHeight() << ", ";

    QString minimum = QLatin1String("org.qtjambi.qt.widgets.QSizePolicy.Policy.Minimum");

    if (isVspacer)
        output << minimum << ", " << sizeType << ");\n";
    else
        output << sizeType << ", " << minimum << ");\n";

    TreeWalker::acceptSpacer(node);
}

void WriteInitialization::acceptLayoutItem(DomLayoutItem *node)
{
    TreeWalker::acceptLayoutItem(node);

    DomLayout *layout = m_layoutChain.top();

    if (!layout)
        return;

    QString varName = escapeVariableName(driver->findOrInsertLayoutItem(node));
    QString layoutName = escapeVariableName(driver->findOrInsertLayout(layout));

    QString opt;
    if (layout->attributeClass() == QLatin1String("QGridLayout")) {
        int row = node->attributeRow();
        int col = node->attributeColumn();

        int rowSpan = 1;
        if (node->hasAttributeRowSpan())
            rowSpan = node->attributeRowSpan();

        int colSpan = 1;
        if (node->hasAttributeColSpan())
            colSpan = node->attributeColSpan();

        opt = QString::fromLatin1(", %1, %2, %3, %4").arg(row).arg(col).arg(rowSpan).arg(colSpan);
    }

    QString method = QLatin1String("addItem");
    switch (node->kind()) {
        case DomLayoutItem::Widget:
            method = QLatin1String("addWidget");
            break;
        case DomLayoutItem::Layout:
            method = QLatin1String("addLayout");
            break;
        case DomLayoutItem::Spacer:
            method = QLatin1String("addItem");
            break;
        case DomLayoutItem::Unknown:
            Q_ASSERT( 0 );
            break;
    }

    output << "\n" << option.indent << layoutName << "." << method << "(" << varName << opt << ");\n\n";
}

void WriteInitialization::acceptActionGroup(DomActionGroup *node)
{
    QString actionName = escapeVariableName(driver->findOrInsertActionGroup(node));
    QString varName = escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top()));

    if (m_actionGroupChain.top())
        varName = driver->findOrInsertActionGroup(m_actionGroupChain.top());

    output << option.indent << actionName << " = new QActionGroup(" << varName << ");\n";
    writeProperties(actionName, QLatin1String("QActionGroup"), node->elementProperty());

    m_actionGroupChain.push(node);
    TreeWalker::acceptActionGroup(node);
    m_actionGroupChain.pop();
}

void WriteInitialization::acceptAction(DomAction *node)
{
    if (node->hasAttributeMenu())
        return;

    QString actionName = escapeVariableName(driver->findOrInsertAction(node));
    m_registeredActions.insert(actionName, node);
    QString varName = escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top()));

    if (m_actionGroupChain.top())
        varName = escapeVariableName(driver->findOrInsertActionGroup(m_actionGroupChain.top()));

    output << option.indent << actionName << " = new QAction(" << varName << ");\n";
    writeProperties(actionName, QLatin1String("QAction"), node->elementProperty());
}

void WriteInitialization::acceptActionRef(DomActionRef *node)
{
    QString actionName = node->attributeName();
    bool isSeparator = actionName == QLatin1String("separator");
    bool isMenu = false;

    QString varName = escapeVariableName(driver->findOrInsertWidget(m_widgetChain.top()));

    if (actionName.isEmpty() || !m_widgetChain.top()) {
        return;
    } else if (driver->actionGroupByName(actionName)) {
        return;
    } else if (DomWidget *w = driver->widgetByName(actionName)) {
        isMenu = uic->isMenu(w->attributeClass());
        bool inQ3ToolBar = uic->customWidgetsInfo()->extends(m_widgetChain.top()->attributeClass(), QLatin1String("Q3ToolBar"));
        if (!isMenu && inQ3ToolBar) {
            actionOut << option.indent << actionName << ".setParent(null);\n";
            actionOut << option.indent << actionName << ".setParent(" << varName << ");\n";
            return;
        }
    } else if (!(driver->actionByName(actionName) || isSeparator)) {
        fprintf(stderr, "Warning: action `%s' not declared\n", actionName.toLatin1().data());
        return;
    }

    if (m_widgetChain.top() && isSeparator) {
        // separator is always reserved!
        actionOut << option.indent << varName << ".addSeparator();\n";
        return;
    }

    if (isMenu)
        actionName += QLatin1String(".menuAction()");

    actionOut << option.indent << varName << ".addAction(" << actionName << ");\n";
}

void WriteInitialization::writeProperties(const QString &varName,
                                          const QString &className,
                                          const QList<DomProperty*> &lst)
{
    bool isTopLevel = m_widgetChain.count() == 1;

    if (uic->customWidgetsInfo()->extends(className, QLatin1String("QAxWidget"))) {
        QHash<QString, DomProperty*> properties = propertyMap(lst);
        if (properties.contains(QLatin1String("control"))) {
            DomProperty *p = properties.value(QLatin1String("control"));
            output << option.indent << varName << ".setControl("
                   << javaFixString(toString(p->elementString())) << ");\n";
        }
    }

    DomWidget *buttonGroupWidget = findWidget(QLatin1String("Q3ButtonGroup"));

    output << option.indent << varName << ".setObjectName(" << javaFixString(varName) << ");\n";

    int leftMargin, topMargin, rightMargin, bottomMargin;
    leftMargin = topMargin = rightMargin = bottomMargin = -1;

    for (int i=0; i<lst.size(); ++i) {
        DomProperty *p = lst.at(i);
        QString propertyName = p->attributeName();
        QString propertyValue;

        // special case for the property `geometry'
        if (isTopLevel && propertyName == QLatin1String("geometry") && p->elementRect()) {
            DomRect *r = p->elementRect();
            int w = r->elementWidth();
            int h = r->elementHeight();
            output << option.indent << varName << ".resize(new QSize(" << w << ", " << h << ").expandedTo("
                << varName << ".minimumSizeHint()));\n";
            continue;
        } else if (propertyName == QLatin1String("buttonGroupId") && buttonGroupWidget) { // Q3ButtonGroup support
            output << option.indent << escapeVariableName(driver->findOrInsertWidget(buttonGroupWidget)) << ".insert("
                   << varName << ", " << p->elementNumber() << ");\n";
            continue;
        } else if (propertyName == QLatin1String("currentRow") // QListWidget::currentRow
                    && uic->customWidgetsInfo()->extends(className, QLatin1String("QListWidget"))) {
            delayedOut << option.indent << varName << ".setCurrentRow("
                       << p->elementNumber() << ");\n";
            continue;
        } else if (propertyName == QLatin1String("currentIndex") // set currentIndex later
                    && (uic->customWidgetsInfo()->extends(className, QLatin1String("QComboBox"))
                    || uic->customWidgetsInfo()->extends(className, QLatin1String("QStackedWidget"))
                    || uic->customWidgetsInfo()->extends(className, QLatin1String("QTabWidget"))
                    || uic->customWidgetsInfo()->extends(className, QLatin1String("QToolBox")))) {
            delayedOut << option.indent << varName << ".setCurrentIndex("
                       << p->elementNumber() << ");\n";
            continue;
        } else if (propertyName == QLatin1String("control") // ActiveQt support
                    && uic->customWidgetsInfo()->extends(className, QLatin1String("QAxWidget"))) {
            // already done ;)
            continue;
        } else if (propertyName == QLatin1String("database")
                    && p->elementStringList()) {
            // Sql support
            continue;
        } else if (propertyName == QLatin1String("frameworkCode")
                    && p->kind() == DomProperty::Bool) {
            // Sql support
            continue;
        } else if (propertyName == QLatin1String("orientation")
                    && uic->customWidgetsInfo()->extends(className, QLatin1String("Line"))) {
            // Line support
            QString shape = QLatin1String("QFrame.Shape.HLine");
            if (p->elementEnum() == QLatin1String("Qt::Vertical"))
                shape = QLatin1String("QFrame.Shape.VLine");

            output << option.indent << varName << ".setFrameShape(" << shape << ");\n";
            continue;

        } else if (propertyName == QLatin1String("leftMargin")
                   && p->kind() == DomProperty::Number) {
            leftMargin = p->elementNumber();
            continue;
        } else if (propertyName == QLatin1String("topMargin") && p->kind() == DomProperty::Number) {
            topMargin = p->elementNumber();
            continue;
        } else if (propertyName == QLatin1String("rightMargin")
                   && p->kind() == DomProperty::Number) {
            rightMargin = p->elementNumber();
            continue;
        } else if (propertyName == QLatin1String("bottomMargin")
                   && p->kind() == DomProperty::Number) {
            bottomMargin = p->elementNumber();
            continue;
        } else if (propertyName == QLatin1String("buddy") &&
                   uic->customWidgetsInfo()->extends(className, QLatin1String("QLabel"))) {
            QString name = p->elementCstring();
            if (name.isEmpty())
                name = p->elementString()->text();
            m_buddies.append(Buddy(varName, name));
            continue;
        } else if (propertyName == QLatin1String("icon") &&
                   uic->customWidgetsInfo()->extends(className, QLatin1String("QWidget"))) {
            output << option.indent << varName << ".setWindowIcon(" << pixCall(p) << ");\n";
            continue;
        }



        bool stdset = m_stdsetdef;
        if (p->hasAttributeStdset())
            stdset = p->attributeStdset();

        QString setFunction;
        if (stdset) {
            setFunction = QLatin1String(".set")
                + propertyName.left(1).toUpper()
                + propertyName.mid(1)
                + QLatin1String("(");
        } else {
            setFunction = QLatin1String(".setProperty(\"")
                + propertyName
                + QLatin1String("\", ");
        }

        switch (p->kind()) {
    enum Kind {




        Char, Url,
        Brush };

        case DomProperty::Bool: {
            propertyValue = p->elementBool();
            break;
        }
        case DomProperty::Color: {
            DomColor *c = p->elementColor();
            propertyValue = QString::fromLatin1("new QColor(%1, %2, %3, %4)")
                  .arg(c->elementRed())
                  .arg(c->elementGreen())
                  .arg(c->elementBlue())
                  .arg(c->attributeAlpha()); }
            break;
        case DomProperty::Cstring:
            propertyValue = javaFixString(p->elementCstring());
            break;
        case DomProperty::Cursor:
            propertyValue = QString::fromLatin1("new QCursor(%1)")
                            .arg(p->elementCursor());
            break;
        case DomProperty::Enum:
            propertyValue = p->elementEnum();
            if (propertyValue.contains(QLatin1String("::")))
                fprintf(stderr, "Malformed enum value %s, C++ syntax not allowed\n",
                        qPrintable(propertyValue));
            break;
        case DomProperty::Locale: {
             const DomLocale *locale = p->elementLocale();
             propertyValue = QString::fromLatin1("new QLocale(QLocale.Language.%1, QLocale.Country.%2)")
                             .arg(locale->attributeLanguage()).arg(locale->attributeCountry());
            break;
        }
        case DomProperty::Set: {
            propertyValue = p->elementSet();

            if (propertyValue.contains(QLatin1String("::"))) {
                fprintf(stderr, "Malformed QFlags value %s, C++ syntax not allowed\n",
                        qPrintable(propertyValue));
            }

            QStringList vals = propertyValue.split(QLatin1Char('|'), QString::SkipEmptyParts);

            // These values are printed duplicate into the .jui file
            // by designer, but are not supported by Jambi...
            vals.removeAll(QLatin1String("org.qtjambi.qt.core.Qt.AlignmentFlag.AlignTrailing"));
            vals.removeAll(QLatin1String("org.qtjambi.qt.core.Qt.AlignmentFlag.AlignLeading"));

            QString flagsName;
            if (vals.size()) {
                int dotPos = vals.at(0).lastIndexOf(QLatin1Char('.'));
                if (dotPos >= 0)
                    flagsName = vals.at(0).left(dotPos);
            }

            propertyValue = QString::fromLatin1("%1.createQFlags(%2)").arg(flagsName).arg(vals.join(","));

            // Specialcase with a "toInt" conversion
            if (className == QLatin1String("QGroupBox")
                && propertyName == QLatin1String("alignment"))
                propertyValue = propertyValue + ".value()";

            }
            break;
        case DomProperty::Font: {
            DomFont *f = p->elementFont();
            QString fontName = driver->unique(QLatin1String("font"));
            output << option.indent << "QFont " << fontName << " = new QFont();\n";

            if (f->hasElementFamily() && !f->elementFamily().isEmpty()) {
                output << option.indent << fontName << ".setFamily("
                       << javaFixString(f->elementFamily()) << ");\n";
            }
            if (f->hasElementPointSize() && f->elementPointSize() > 0) {
                output << option.indent << fontName << ".setPointSize(" << f->elementPointSize()
                    << ");\n";
            }
            if (f->hasElementBold()) {
                output << option.indent << fontName << ".setBold("
                    << (f->elementBold() ? "true" : "false") << ");\n";
            }
            if (f->hasElementItalic()) {
                output << option.indent << fontName << ".setItalic("
                    <<  (f->elementItalic() ? "true" : "false") << ");\n";
            }
            if (f->hasElementUnderline()) {
                output << option.indent << fontName << ".setUnderline("
                    << (f->elementUnderline() ? "true" : "false") << ");\n";
            }
            if (f->hasElementWeight() && f->elementWeight() > 0) {
                output << option.indent << fontName << ".setWeight("
                    << f->elementWeight() << ");" << endl;
            }
            if (f->hasElementStrikeOut()) {
                output << option.indent << fontName << ".setStrikeOut("
                    << (f->elementStrikeOut() ? "true" : "false") << ");\n";
            }
            if (f->hasElementKerning()) {
                output << option.indent << fontName << ".setKerning("
                    << (f->elementKerning() ? "true" : "false") << ");\n";
            }
            if (f->hasElementAntialiasing()) {
                output << option.indent << fontName << ".setStyleStrategy("
                    << (f->elementAntialiasing()
                        ? "QFont.StyleStrategy.PreferDefault"
                        : "QFont.StyleStrategy.NoAntialias") << ");\n";
            }

            propertyValue = fontName;
            break;
        }
        case DomProperty::IconSet:
            propertyValue = pixCall(p);
            break;

        case DomProperty::Pixmap:
            propertyValue = pixCall(p);
            break;

        case DomProperty::Palette: {
            // ### port me
            DomPalette *pal = p->elementPalette();
            QString paletteName = driver->unique(QLatin1String("palette"));
            output << option.indent << "QPalette " << paletteName << "= new QPalette();\n";

            writeColorGroup(pal->elementActive(), QLatin1String("QPalette.ColorGroup.Active"), paletteName);
            writeColorGroup(pal->elementInactive(), QLatin1String("QPalette.ColorGroup.Inactive"), paletteName);
            writeColorGroup(pal->elementDisabled(), QLatin1String("QPalette.ColorGroup.Disabled"), paletteName);

            propertyValue = paletteName;
            break;
        }
        case DomProperty::Point: {
            DomPoint *po = p->elementPoint();
            propertyValue = QString::fromLatin1("new QPoint(%1, %2)")
                            .arg(po->elementX()).arg(po->elementY());
            break;
        }
        case DomProperty::PointF: {
            const DomPointF *pof = p->elementPointF();
            propertyValue = QString::fromLatin1("new QPointF(%1, %2)")
                            .arg(pof->elementX()).arg(pof->elementY());
            break;
        }

        case DomProperty::Rect: {
            DomRect *r = p->elementRect();
            propertyValue = QString::fromLatin1("new QRect(%1, %2, %3, %4)")
                            .arg(r->elementX()).arg(r->elementY())
                            .arg(r->elementWidth()).arg(r->elementHeight());
            break;
        }
        case DomProperty::RectF: {
            const DomRectF *rf = p->elementRectF();
            propertyValue = QString::fromLatin1("new QRectF(%1, %2, %3, %4)")
                            .arg(rf->elementX()).arg(rf->elementY())
                            .arg(rf->elementWidth()).arg(rf->elementHeight());
            break;
        }

        case DomProperty::SizePolicy: {
            DomSizePolicy *sp = p->elementSizePolicy();
            QString spName = driver->unique(QLatin1String("sizePolicy"));

            QString vSizeType, hSizeType;

            if (sp->hasAttributeVSizeType() && sp->hasAttributeHSizeType()) {
                const char *name = "org.qtjambi.qt.widgets.QSizePolicy.Policy.%1";
                vSizeType = QString::fromLatin1(name).arg(sp->attributeVSizeType());
                hSizeType = QString::fromLatin1(name).arg(sp->attributeHSizeType());
            } else {
                const char *name = "org.qtjambi.qt.widgets.QSizePolicy.Policy.resolve(%1)";
                vSizeType = QString::fromLatin1(name).arg(sp->elementVSizeType());
                hSizeType = QString::fromLatin1(name).arg(sp->elementHSizeType());
            }

            output << option.indent << "QSizePolicy " << spName << " = "
                   << "new QSizePolicy(" << hSizeType << ", " << vSizeType << ");\n";
            output << option.indent << spName << ".setHorizontalStretch"
                   << "((byte)" << sp->elementHorStretch() << ");\n";
            output << option.indent << spName << ".setVerticalStretch"
                   << "((byte)" << sp->elementVerStretch() << ");\n";
            output << option.indent << spName << QString::fromLatin1(
                ".setHeightForWidth(%1.sizePolicy().hasHeightForWidth());\n")
                .arg(varName);

            propertyValue = spName;
            break;
        }
        case DomProperty::Size: {
             DomSize *s = p->elementSize();
              propertyValue = QString::fromLatin1("new QSize(%1, %2)")
                             .arg(s->elementWidth()).arg(s->elementHeight());
            break;
        }
        case DomProperty::SizeF: {
            const DomSizeF *sf = p->elementSizeF();
             propertyValue = QString::fromLatin1("new QSizeF(%1, %2)")
                            .arg(sf->elementWidth()).arg(sf->elementHeight());
            break;
        }
        case DomProperty::String: {
            if (propertyName == QLatin1String("objectName")) {
                QString v = p->elementString()->text();
                if (v == varName)
                    break;

                // ### qWarning("Deprecated: the property `objectName' is different from the variable name");
            }

            if (p->elementString()->hasAttributeNotr()
                    && toBool(p->elementString()->attributeNotr())) {
                propertyValue = javaFixString(p->elementString()->text());
            } else {
                propertyValue = trCall(p->elementString());
            }
            break;
        }
        case DomProperty::Number:
            propertyValue = QString::number(p->elementNumber());
            break;
        case DomProperty::UInt:
            propertyValue = QString::number(p->elementUInt());
            break;
        case DomProperty::Float:
            propertyValue = QString::number(p->elementFloat());
            break;
        case DomProperty::Double:
            propertyValue = QString::number(p->elementDouble());
            break;
        case DomProperty::Char:
            propertyValue = p->elementChar()->elementUnicode();
            break;
        case DomProperty::Url: {
            const DomUrl* u = p->elementUrl();
            propertyValue = QString::fromLatin1("new QUrl(%1)")
                            .arg(javaFixString(u->elementString()->text()));
            break;
        }
        case DomProperty::CursorShape:
            propertyValue = QString::fromLatin1("new QCursor(Qt.CursorShape.%1)")
                            .arg(p->elementCursorShape());
            break;
        case DomProperty::Brush: {
            DomBrush *brush = p->elementBrush();
            const QString brushName = driver->unique(QLatin1String("brush"));
            writeBrush(brush, brushName);
            propertyValue = brushName;
            break;
        }
        case DomProperty::ULongLong:
        case DomProperty::LongLong:
            propertyValue = QString::number(p->elementLongLong());
            break;
        case DomProperty::Date: {
            DomDate *d = p->elementDate();
            propertyValue = QString::fromLatin1("new QDate(%1, %2, %3)")
                            .arg(d->elementYear())
                            .arg(d->elementMonth())
                            .arg(d->elementDay());
            break;
        }
        case DomProperty::Time: {
            DomTime *t = p->elementTime();
            propertyValue = QString::fromLatin1("new QTime(%1, %2, %3)")
                            .arg(t->elementHour())
                            .arg(t->elementMinute())
                            .arg(t->elementSecond());
            break;
        }
        case DomProperty::DateTime: {
            DomDateTime *dt = p->elementDateTime();
            propertyValue = QString::fromLatin1("new QDateTime(new QDate(%1, %2, %3), new QTime(%4, %5, %6))")
                            .arg(dt->elementYear())
                            .arg(dt->elementMonth())
                            .arg(dt->elementDay())
                            .arg(dt->elementHour())
                            .arg(dt->elementMinute())
                            .arg(dt->elementSecond());
            break;
        }

        case DomProperty::StringList:
            propertyValue = QLatin1String("java.util.Arrays.asList(new String[] {");
            if (p->elementStringList()->elementString().size()) {
                QStringList lst = p->elementStringList()->elementString();
                for (int i=0; i<lst.size(); ++i) {
                    if (i > 0)
                        propertyValue += QLatin1String(", ");
                    propertyValue += javaFixString(lst.at(i));
                }
            }
            propertyValue += QLatin1String("})");
            break;

        case DomProperty::Unknown:
            break;

        }



        if (propertyValue.size()) {
            QTextStream *o = &output;

            if (p->kind() == DomProperty::String
                    && (!p->elementString()->hasAttributeNotr() || !toBool(p->elementString()->attributeNotr())))
                o = &refreshOut;

            (*o) << option.indent << varName << setFunction << propertyValue;
            (*o) << ");\n";
        }
    }

    if (leftMargin != -1 || topMargin != -1 || rightMargin != -1 || bottomMargin != -1) {
        QString objectName = varName;
        output << option.indent << objectName << QLatin1String(".setContentsMargins(")
               << QString::number(leftMargin) << QLatin1String(", ")
               << QString::number(topMargin) << QLatin1String(", ")
               << QString::number(rightMargin) << QLatin1String(", ")
               << QString::number(bottomMargin) << QLatin1String(");\n");
    }

}

void WriteInitialization::writeBrush(const DomBrush *brush, const QString &brushName)
{
    QString style = QLatin1String("SolidPattern");
    if (brush->hasAttributeBrushStyle())
        style = brush->attributeBrushStyle();

    if (style == QLatin1String("LinearGradientPattern") ||
            style == QLatin1String("RadialGradientPattern") ||
            style == QLatin1String("ConicalGradientPattern")) {
        const DomGradient *gradient = brush->elementGradient();
        const QString gradientType = gradient->attributeType();
        const QString gradientName = driver->unique(QLatin1String("gradient"));
        if (gradientType == QLatin1String("LinearGradient")) {
            output << option.indent << "QLinearGradient " << gradientName
                   << " = new QLinearGradient(" << gradient->attributeStartX()
                   << ", " << gradient->attributeStartY()
                   << ", " << gradient->attributeEndX()
                   << ", " << gradient->attributeEndY() << ");\n";
        } else if (gradientType == QLatin1String("RadialGradient")) {
            output << option.indent << "QRadialGradient " << gradientName
                     << " = new QRadialGradient(" << gradient->attributeCentralX()
                     << ", " << gradient->attributeCentralY()
                     << ", " << gradient->attributeRadius()
                     << ", " << gradient->attributeFocalX()
                     << ", " << gradient->attributeFocalY() << ");\n";
        } else if (gradientType == QLatin1String("ConicalGradient")) {
            output << option.indent << "QConicalGradient " << gradientName
                     << " = new QConicalGradient(" << gradient->attributeCentralX()
                     << ", " << gradient->attributeCentralY()
                     << ", " << gradient->attributeAngle() << ");\n";
        }

        output << option.indent << gradientName << ".setSpread(QGradient.Spread."
               << gradient->attributeSpread() << ");\n";

        if (gradient->hasAttributeCoordinateMode()) {
            output << gradientName << ".setCoordinateMode(QGradient.CoordinateMode."
                   << gradient->attributeCoordinateMode() << ");\n";
        }

       const  QList<DomGradientStop *> stops = gradient->elementGradientStop();
        QListIterator<DomGradientStop *> it(stops);
        while (it.hasNext()) {
            const DomGradientStop *stop = it.next();
            DomColor *color = stop->elementColor();
            output << option.indent << gradientName << ".setColorAt("
                   << stop->attributePosition() << ", "
                   << domColor2QString(color) << ");\n";
        }
        output << option.indent << "QBrush " << brushName << " = new QBrush("
               << gradientName << ");\n";
    } else if (style == QLatin1String("TexturePattern")) {
        qWarning("juic: Brushes with TexturePattern not supported yet");
        /*const DomProperty *property = brush->elementTexture();
        const QString iconValue = iconCall(property);

        output << option.indent << "QBrush " << brushName << " = new QBrush("
        << iconValue << ");\n";*/
    } else {
        DomColor *color = brush->elementColor();
        output << option.indent << "QBrush " << brushName << " = new QBrush("
               << domColor2QString(color) << ");\n";

        output << option.indent << brushName << ".setStyle("
            << "Qt.BrushStyle." << style << ");\n";
    }
}

QString WriteInitialization::domColor2QString(DomColor *c)
{
    return QString::fromLatin1("new QColor(%1, %2, %3)")
        .arg(c->elementRed())
        .arg(c->elementGreen())
        .arg(c->elementBlue());
}

QString WriteInitialization::domBrush2QString(DomBrush *c)
{
    return domColor2QString(c->elementColor());
}

void WriteInitialization::writeColorGroup(DomColorGroup *colorGroup, const QString &group, const QString &paletteName)
{
    if (!colorGroup)
        return;

    const char *roleNames[] = {
        "WindowText",
        "Button",
        "Light",
        "Midlight",
        "Dark",
        "Mid",
        "Text",
        "BrightText",
        "ButtonText",
        "Base",
        "Window",
        "Shadow",
        "Highlight",
        "HighlightedText",
        "Link",
        "LinkVisited",
        "AlternateBase",
        "NoRole"
    };

    // Old format
    QList<DomColor*> colors = colorGroup->elementColor();
    for (int i=0; i<colors.size(); ++i) {
        DomColor *color = colors.at(i);

        output << option.indent << paletteName << ".setColor(" << group
               << ", QPalette.ColorRole." << roleNames[i]
               << ", " << domColor2QString(color)
               << ");\n";
    }

    // New format
    QList<DomColorRole*> roles = colorGroup->elementColorRole();
    for (int i=0; i<roles.size(); ++i) {
        DomColorRole *role = roles.at(i);
        output << option.indent << paletteName << ".setColor(" << group
               << ", QPalette.ColorRole." << role->attributeRole()
               << ", " << domBrush2QString(role->elementBrush())
               << ");\n";
    }
}

void WriteInitialization::acceptCustomWidget(DomCustomWidget *node)
{
    Q_UNUSED(node);
}

void WriteInitialization::acceptCustomWidgets(DomCustomWidgets *node)
{
    Q_UNUSED(node);
}

void WriteInitialization::acceptTabStops(DomTabStops *tabStops)
{
    QString lastName;

    QStringList l = tabStops->elementTabStop();
    for (int i=0; i<l.size(); ++i) {
        QString name = l.at(i);

        if (!m_registeredWidgets.contains(name)) {
            fprintf(stderr, "'%s' isn't a valid widget\n", name.toLatin1().data());
            continue;
        }

        if (i == 0) {
            lastName = name;
            continue;
        } else if (name.isEmpty() || lastName.isEmpty()) {
            continue;
        }

        output << option.indent << "QWidget.setTabOrder(" << lastName << ", " << name << ");\n";

        lastName = name;
    }
}

void WriteInitialization::acceptLayoutDefault(DomLayoutDefault *node)
{
    m_defaultMargin = INT_MIN;
    m_defaultSpacing = INT_MIN;

    if (!node)
        return;

    if (node->hasAttributeMargin())
        m_defaultMargin = node->attributeMargin();

    if (node->hasAttributeSpacing())
        m_defaultSpacing = node->attributeSpacing();
}

void WriteInitialization::acceptLayoutFunction(DomLayoutFunction *node)
{
    m_marginFunction.clear();
    m_spacingFunction.clear();

    if (!node)
        return;

    if (node->hasAttributeMargin())
        m_marginFunction = node->attributeMargin();

    if (node->hasAttributeSpacing())
        m_spacingFunction = node->attributeSpacing();
}

void WriteInitialization::initializeTreeWidgetItems(const QString &className, const QString &varName, const QList<DomItem *> &items)
{
    if (items.isEmpty())
        return;

    // items
    for (int i=0; i<items.size(); ++i) {
        DomItem *item = items.at(i);

        QString itemName = driver->unique(QLatin1String("__item"));
        refreshOut << "\n";
        refreshOut << option.indent << "QTreeWidgetItem " << itemName << " = new QTreeWidgetItem(" << varName << ");\n";

        int textCount = 0;
        QList<DomProperty*> properties = item->elementProperty();
        for (int i=0; i<properties.size(); ++i) {
            DomProperty *p = properties.at(i);
            if (p->attributeName() == QLatin1String("text"))
                refreshOut << option.indent << itemName << ".setText(" << textCount++ << ", "
                           << trCall(p->elementString()) << ");\n";

            if (p->attributeName() == QLatin1String("icon") && textCount > 0)
                refreshOut << option.indent << itemName << ".setIcon(" << textCount - 1 << ", "
                           << pixCall(p) << ");\n";
        }

        if (item->elementItem().size())
            initializeTreeWidgetItems(className, itemName, item->elementItem());
    }
}

QString WriteInitialization::pixCall(DomProperty *p) const
{
    Q_ASSERT(p->kind() == DomProperty::IconSet || p->kind() == DomProperty::Pixmap);

    QString pixFunc = uic->pixmapFunction();

    QString type, s;
    if (p->kind() == DomProperty::IconSet) {
        type = QLatin1String("QIcon");
        s = p->elementIconSet()->text();
        pixFunc = "new QPixmap";
    } else {
        type = QLatin1String("QPixmap");
        s = p->elementPixmap()->text();
    }

    if (s.isEmpty())
        return "new " + type + QLatin1String("()");
    else if (findImage(s) != 0)
        return QLatin1String("icon(") + s + QLatin1String("_ID)");

    return "new " + type + QLatin1String("(") + pixFunc + QLatin1String("(") + javaFixString(s) + QLatin1String(")") + QLatin1String(")");
}

void WriteInitialization::initializeComboBox(DomWidget *w)
{
    QString varName = escapeVariableName(driver->findOrInsertWidget(w));
    QString className = w->attributeClass();

    QList<DomItem*> items = w->elementItem();

    if (items.isEmpty())
        return;

    refreshOut << option.indent << varName << ".clear();\n";

    for (int i=0; i<items.size(); ++i) {
        DomItem *item = items.at(i);

        QHash<QString, DomProperty*> properties = propertyMap(item->elementProperty());
        DomProperty *text = properties.value(QLatin1String("text"));
        DomProperty *pixmap = properties.value(QLatin1String("icon"));
        if (!(text || pixmap))
            continue;

        refreshOut << option.indent << varName << ".addItem(";

        if (pixmap != 0) {
            refreshOut << pixCall(pixmap);

            if (text)
                refreshOut << ", ";
        }

        refreshOut << trCall(text->elementString()) << ");\n";
    }
}

void WriteInitialization::initializeListWidget(DomWidget *w)
{
    QString varName = escapeVariableName(driver->findOrInsertWidget(w));
    QString className = w->attributeClass();

    QList<DomItem*> items = w->elementItem();

    if (items.isEmpty())
        return;

    refreshOut << option.indent << varName << ".clear();\n";

    // items
    for (int i=0; i<items.size(); ++i) {
        DomItem *item = items.at(i);

        QString itemName = driver->unique(QLatin1String("__item"));
        refreshOut << "\n";
        refreshOut << option.indent << "QListWidgetItem " << itemName << " = new QListWidgetItem(" << varName << ");\n";

        QList<DomProperty*> properties = item->elementProperty();
        for (int i=0; i<properties.size(); ++i) {
            DomProperty *p = properties.at(i);

            if (p->attributeName() == QLatin1String("text"))
                refreshOut << option.indent << itemName << ".setText(" << trCall(p->elementString()) << ");\n";

            if (p->attributeName() == QLatin1String("icon"))
                refreshOut << option.indent << itemName << ".setIcon(" << pixCall(p) << ");\n";
        }
    }
}

void WriteInitialization::initializeTreeWidget(DomWidget *w)
{
    QString varName = escapeVariableName(driver->findOrInsertWidget(w));
    QString className = w->attributeClass();

    // columns
    QList<DomColumn*> columns = w->elementColumn();
    for (int i=0; i<columns.size(); ++i) {
        DomColumn *column = columns.at(i);

        QHash<QString, DomProperty*> properties = propertyMap(column->elementProperty());
        DomProperty *text = properties.value(QLatin1String("text"));
        DomProperty *icon = properties.value(QLatin1String("icon"));

        QString txt = trCall(text->elementString());
        refreshOut << option.indent << varName << ".headerItem().setText(" << i << ", " << txt << ");\n";

        if (icon != 0 && icon->elementIconSet()) {
            output << option.indent << varName << ".headerItem().setIcon("
                   << i << ", " << pixCall(icon) << ");\n";
        }
    }

    if (w->elementItem().size()) {
        refreshOut << option.indent << varName << ".clear();\n";

        initializeTreeWidgetItems(className, varName, w->elementItem());
    }
}

void WriteInitialization::initializeTableWidget(DomWidget *w)
{
    QString varName = escapeVariableName(driver->findOrInsertWidget(w));
    QString className = w->attributeClass();

    refreshOut << option.indent << varName << ".clear();\n";
    // columns
    QList<DomColumn *> columns = w->elementColumn();
    refreshOut << option.indent << varName << ".setColumnCount("
                << columns.size() << ");\n";
    for (int i = 0; i < columns.size(); ++i) {
        DomColumn *column = columns.at(i);

        QHash<QString, DomProperty*> properties = propertyMap(column->elementProperty());
        DomProperty *text = properties.value(QLatin1String("text"));
        DomProperty *icon = properties.value(QLatin1String("icon"));
        if (text || icon) {
            QString itemName = driver->unique(QLatin1String("__colItem"));
            refreshOut << "\n";
            refreshOut << option.indent << "QTableWidgetItem "
                           << itemName << " = new QTableWidgetItem();\n";

            if (text && text->attributeName() == QLatin1String("text"))
                refreshOut << option.indent << itemName << ".setText("
                           << trCall(text->elementString()) << ");\n";

            if (icon && icon->attributeName() == QLatin1String("icon"))
                refreshOut << option.indent << itemName << ".setIcon("
                           << pixCall(icon) << ");\n";
            refreshOut << option.indent << varName << ".setHorizontalHeaderItem("
                           << i << ", " << itemName << ");\n";
        }
    }

    // rows
    QList<DomRow *> rows = w->elementRow();
    refreshOut << option.indent << varName << ".setRowCount("
                << rows.size() << ");\n";
    for (int i = 0; i < rows.size(); ++i) {
        DomRow *row = rows.at(i);

        QHash<QString, DomProperty*> properties = propertyMap(row->elementProperty());
        DomProperty *text = properties.value(QLatin1String("text"));
        DomProperty *icon = properties.value(QLatin1String("icon"));
        if (text || icon) {
            QString itemName = driver->unique(QLatin1String("__rowItem"));
            refreshOut << "\n";
            refreshOut << option.indent << "QTableWidgetItem "
                           << itemName << " = new QTableWidgetItem();\n";

            if (text && text->attributeName() == QLatin1String("text"))
                refreshOut << option.indent << itemName << ".setText("
                           << trCall(text->elementString()) << ");\n";

            if (icon && icon->attributeName() == QLatin1String("icon"))
                refreshOut << option.indent << itemName << ".setIcon("
                           << pixCall(icon) << ");\n";
            refreshOut << option.indent << varName << ".setVerticalHeaderItem("
                           << i << ", " << itemName << ");\n";
        }
    }

    // items
    QList<DomItem *> items = w->elementItem();
    for (int i = 0; i < items.size(); ++i) {
        DomItem *item = items.at(i);
        if (item->hasAttributeRow() && item->hasAttributeColumn()) {
            QHash<QString, DomProperty*> properties = propertyMap(item->elementProperty());
            DomProperty *text = properties.value(QLatin1String("text"));
            DomProperty *icon = properties.value(QLatin1String("icon"));
            if (text || icon) {
                QString itemName = driver->unique(QLatin1String("__item"));
                refreshOut << "\n";
                refreshOut << option.indent << "QTableWidgetItem "
                    << itemName << " = new QTableWidgetItem();\n";

                if (text && text->attributeName() == QLatin1String("text"))
                    refreshOut << option.indent << itemName << ".setText("
                        << trCall(text->elementString()) << ");\n";

                if (icon && icon->attributeName() == QLatin1String("icon"))
                    refreshOut << option.indent << itemName << ".setIcon("
                        << pixCall(icon) << ");\n";
                refreshOut << option.indent << varName << ".setItem("
                    << item->attributeRow() << ", "
                    << item->attributeColumn() << ", "
                    << itemName << ");\n";
            }
        }
    }
}

QString WriteInitialization::trCall(const QString &str, const QString &commentHint) const
{
    if (str.isEmpty())
        return QLatin1String("\"\"");

    QString result;
    const QString comment = commentHint.isEmpty() ? QString::fromLatin1("null") : javaFixString(commentHint);

    result = QLatin1String("org.qtjambi.qt.core.QCoreApplication.translate(\"");
    result += m_generatedClass;
    result += QLatin1Char('"');
    result += QLatin1String(", ");

    result += javaFixString(str);
    result += QLatin1String(", ");
    result += comment;

    result += QLatin1Char(')');
    return result;
}

void WriteInitialization::initializeMenu(DomWidget *w, const QString &/*parentWidget*/)
{
    QString menuName = escapeVariableName(driver->findOrInsertWidget(w));
    QString menuAction = menuName + QLatin1String("Action");

    DomAction *action = driver->actionByName(menuAction);
    if (action && action->hasAttributeMenu()) {
        output << option.indent << menuAction << " = " << menuName << ".menuAction();\n";
    }
}

QString WriteInitialization::trCall(DomString *str) const
{
    return trCall(toString(str), str->attributeComment());
}

bool WriteInitialization::isValidObject(const QString &name) const
{
    return m_registeredWidgets.contains(name)
        || m_registeredActions.contains(name);
}

void WriteInitialization::acceptConnection(DomConnection *connection)
{
    if (isValidObject(connection->elementSender()) && isValidObject(connection->elementReceiver())) {
        QString senderClassName;
        if (DomWidget *w = m_registeredWidgets.value(connection->elementSender())) {
            senderClassName = w->attributeClass();
        } else if (m_registeredActions.value(connection->elementSender())) {
            senderClassName = QLatin1String("QAction");
        }

        QString signalName, slotSignature;

        QString signal = connection->elementSignal();

        // Java connection, no type mapping...
        int ltPos = signal.indexOf(QLatin1Char('<'));
        if (ltPos >= 1 || !signal.contains(QLatin1Char('('))) {
            signalName = ltPos >= 1 ? signal.left(ltPos) : signal;
            slotSignature = connection->elementSlot();

        // C++ signature on connection...
        } else {
            fprintf(stderr,
                    "Malformed signal/slot connection '%s::%s -> %s::%s', C++ syntax not allowed",
                    qPrintable(senderClassName),
                    qPrintable(connection->elementSignal()),
                    qPrintable(connection->elementReceiver()),
                    qPrintable(connection->elementSlot()));
        }

        if (signalName.isEmpty () || slotSignature.isEmpty()) {
            fprintf(stderr, "Connection rejected: %s::%s -> %s::%s\n",
                    qPrintable(senderClassName),
                    qPrintable(connection->elementSignal()),
                    qPrintable(connection->elementReceiver()),
                    qPrintable(connection->elementSlot()));
            return;
        }

        output << option.indent << connection->elementSender() << "." << signalName << ".connect("
               << connection->elementReceiver() << ", " << javaFixString(slotSignature) << ");\n";
    }
}

DomImage *WriteInitialization::findImage(const QString &name) const
{
    return m_registeredImages.value(name);
}

DomWidget *WriteInitialization::findWidget(const QLatin1String &widgetClass)
{
    for (int i = m_widgetChain.count() - 1; i >= 0; --i) {
        DomWidget *widget = m_widgetChain.at(i);

        if (widget && uic->customWidgetsInfo()->extends(widget->attributeClass(), widgetClass))
            return widget;
    }

    return 0;
}

void WriteInitialization::acceptImage(DomImage *image)
{
    if (!image->hasAttributeName())
        return;

    m_registeredImages.insert(image->attributeName(), image);
}

} // namespace Java
