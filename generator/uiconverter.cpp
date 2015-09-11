/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "uiconverter.h"
#include "metajava.h"

#include "reporthandler.h"

#include <QtCore/QFileInfo>
#include <QtXml/QDomDocument>

void UiConverter::convertToJui(const QString &uiFile, const QString &customWidgetFiles) {
    ReportHandler::setContext(QLatin1String("UiConverter to .jui"));

    traverseCustomWidgets(customWidgetFiles);

    QFileInfo fileInfo(uiFile);

    if (!fileInfo.exists()) {
        ReportHandler::warning(QString::fromLatin1("Ui File %1 doesn't exist...\n").arg(uiFile));
        return;
    }

    if (fileInfo.suffix() != QLatin1String("ui")) {
        ReportHandler::warning(QString::fromLatin1("File doesn't have .ui extension: %1")
                               .arg(uiFile));
        return;
    }

    QString juiFile = fileInfo.absolutePath() + QLatin1Char('/') + fileInfo.baseName()
                      + QLatin1String(".jui");

    QFile inputFile(uiFile);

    if (!inputFile.open(QFile::ReadOnly | QFile::Text)) {
        ReportHandler::warning(QString::fromLatin1("Could not open '%1' for reading").arg(uiFile));
        return;
    }

    QDomDocument dom;
    QString error;
    if (!dom.setContent(&inputFile, false, &error)) {
        ReportHandler::warning(QString::fromLatin1("Xml loading %1 failed: %2")
                               .arg(uiFile).arg(error));
        inputFile.close();
        return;
    }
    inputFile.close();

    QDomNodeList customWidgets = dom.documentElement().elementsByTagName("customwidget");
    for (int i = 0; i < customWidgets.size(); ++i) {
        QDomNode customWidget = customWidgets.at(i);

        QDomElement el = customWidget.toElement();
        fixCustomWidgetNode(el, &dom);
    }

    traverse(dom.documentElement(), &dom);

    QFile outputFile(juiFile);
    if (!outputFile.open(QFile::WriteOnly | QFile::Text)) {
        ReportHandler::warning(QString::fromLatin1("Could not open '%1' for writing")
                               .arg(juiFile));
        return;
    }

    outputFile.write(dom.toByteArray());
    outputFile.close();
}

void UiConverter::traverseCustomWidgetFile(const QString &customWidgetFile) {
    if (customWidgetFile.isEmpty())
        return;

    QFile file(customWidgetFile);
    if (!file.open(QIODevice::ReadOnly)) {
        ReportHandler::warning("Can't read custom widget file '"
                               + customWidgetFile
                               + "'");
        return;
    }

    QXmlStreamReader reader(&file);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "qt-jambi-custom-widget") {
            QXmlStreamAttributes attributes = reader.attributes();
            QString className = attributes.value("class").toString();

            int pos = className.lastIndexOf(".");
            m_custom_widgets.insertMulti(className.mid(pos + 1), CustomWidget(className, 0));
        }
    }

    if (reader.hasError()) {
        ReportHandler::warning("Error when parsing custom widget file '"
                               + customWidgetFile
                               + "': "
                               + reader.errorString());
    }
}

void UiConverter::traverseCustomWidgets(const QString &customWidgetFiles) {
#ifdef Q_OS_WIN32
    char separator = ';';
#else
    char separator = ':';
#endif

    QStringList customWidgets = customWidgetFiles.split(separator);
    foreach(QString customWidget, customWidgets)
        traverseCustomWidgetFile(customWidget);
}

void UiConverter::traverse(QDomNode node, QDomDocument *doc) {
    if (node.isNull())
        return;

    QDomElement element = node.toElement();
    if (!element.isNull()) {
        if (element.nodeName() == QLatin1String("ui"))
            fixUiNode(element, doc);
        else if (element.nodeName() == QLatin1String("set"))
            fixSetNode(element, doc);
        else if (element.nodeName() == QLatin1String("enum"))
            fixEnumNode(element, doc);
        else if (element.nodeName() == QLatin1String("connection"))
            fixConnectionNode(element, doc);
        else if (element.nodeName() == QLatin1String("widget"))
            fixWidgetNode(element, doc);
    }

    QDomNodeList list = node.childNodes();
    for (int i = 0; i < list.size(); ++i)
        traverse(list.at(i), doc);
}


void UiConverter::fixUiNode(QDomElement el, QDomDocument *) {
    el.setAttribute("language", "jambi");
}

void UiConverter::fixCustomWidgetNode(QDomElement el, QDomDocument *) {
    QDomNodeList classes = el.elementsByTagName("class");
    if (classes.size() < 1) {
        ReportHandler::warning("Custom widget missing 'class' child");
        return;
    }

    QDomNodeList extendss = el.elementsByTagName("extends");
    if (extendss.size() < 1) {
        ReportHandler::warning("Custom widget missing 'extends' child");
        return;
    }

    QString className = classes.at(0).toElement().text();
    QString extends = extendss.at(0).toElement().text();

    AbstractMetaClass *javaClass = m_java_classes.findClass(extends);
    if (javaClass == 0) {
        ReportHandler::warning("Couldn't find super class for custom widget: '" + extends + "'");
        return;
    }

    QList<CustomWidget> fullNames = m_custom_widgets.values(className);
    if (fullNames.size() == 0) {
        ReportHandler::warning("Couldn't find custom widget entry for '" + className + "'."
                               " You need to specify this class in a custom widget file and"
                               " pass the file name on the command line using the --custom-widgets option.");
        return;
    }

    if (fullNames.size() > 1) {
        ReportHandler::warning("More than custom widget type matching '" + className + "'. "
                               + "Will use first seen entry: '" + fullNames.at(0).first + "'");
    }

    QString fullName = fullNames.at(0).first;
    classes.at(0).namedItem("#text").toText().setData(fullName);

    QMap<QString, CustomWidget>::iterator it;
    for (it = m_custom_widgets.begin(); it != m_custom_widgets.end(); ++it) {
        if (it.key() == className)
            (*it).second = javaClass;
    }
}

void UiConverter::fixSetNode(QDomElement el, QDomDocument *) {
    QStringList cppSet = el.firstChild().nodeValue().split(QLatin1Char('|'));

    QStringList javaSet;
    for (int i = 0; i < cppSet.size(); ++i)
        javaSet << translateEnumValue(cppSet.at(i));

    el.firstChild().setNodeValue(javaSet.join(QLatin1String("|")));
}


void UiConverter::fixEnumNode(QDomElement el, QDomDocument *) {
    QDomNode valueNode = el.firstChild();
    if (valueNode.isNull()) {
        ReportHandler::warning(QString::fromLatin1("Bad enum value at '%1'").arg(el.nodeValue()));
        return;
    }

    QString cppEnumValue = valueNode.nodeValue();
    QString javaEnumValue = translateEnumValue(cppEnumValue);
    valueNode.setNodeValue(javaEnumValue);
}


void UiConverter::fixConnectionNode(QDomElement el, QDomDocument *) {
    QString senderName = el.namedItem("sender").firstChild().nodeValue();
    AbstractMetaClass *senderClass = m_named_widgets[senderName];
    if (!senderClass) {
        ReportHandler::warning(QString::fromLatin1("sender unknown '%1'").arg(senderName));
        return;
    }
    QDomNode signalSignatureNode = el.namedItem("signal").toElement().firstChild();
    QString signalSignature = signalSignatureNode.nodeValue();
    const AbstractMetaFunction *signalFunction = findFunction(senderClass,
            signalSignature,
            SignalSearch);
    if (!signalFunction) {
        ReportHandler::warning(QString::fromLatin1("Signal not found '%1' in '%2'")
                               .arg(signalSignature).arg(senderClass->qualifiedCppName()));
        return;
    }
    signalSignatureNode.setNodeValue(signalFunction->modifiedName());

    QString receiverName = el.namedItem("receiver").firstChild().nodeValue();
    AbstractMetaClass *receiverClass = m_named_widgets[receiverName];
    if (!receiverClass) {
        ReportHandler::warning(QString::fromLatin1("receiver unknown '%1'").arg(receiverName));
        return;
    }

    QDomNode slotSignatureNode = el.namedItem("slot").firstChild();
    QString slotSignature = slotSignatureNode.nodeValue();
    const AbstractMetaFunction *slotFunction = findFunction(receiverClass, slotSignature, SlotSearch);
    if (!signalFunction) {
        ReportHandler::warning(QString::fromLatin1("Slot not found '%1' in '%2'")
                               .arg(slotSignature).arg(receiverClass->qualifiedCppName()));
        return;
    }

    slotSignatureNode.setNodeValue(slotFunction->targetLangSignature(true));
}


void UiConverter::fixWidgetNode(QDomElement el, QDomDocument *) {
    QString className = el.attribute(QLatin1String("class"));
    QList<CustomWidget> customWidgetNames = m_custom_widgets.values(className);
    QString customWidgetName = customWidgetNames.size() > 0 ? customWidgetNames.at(0).first : QString();

    AbstractMetaClass *javaClass = customWidgetName.isEmpty() ? m_java_classes.findClass(className) : customWidgetNames.at(0).second;
    if (!javaClass) {
        ReportHandler::warning(QString::fromLatin1("Class '%1' is unknown").arg(className));
        return;
    }

    if (!customWidgetName.isEmpty())
        el.setAttribute(QLatin1String("class"), customWidgetName);
    else if (javaClass->package() != QLatin1String("org.qtjambi.qt.widgets"))
        el.setAttribute(QLatin1String("class"), javaClass->fullName());

    m_named_widgets.insert(el.attribute(QLatin1String("name")), javaClass);
}


QString UiConverter::translateEnumValue(const QString &cppEnumValue) {
    if (!cppEnumValue.contains(QLatin1String("::"))) {
        ReportHandler::warning(QString::fromLatin1("Expected '::' in enum value '%1'")
                               .arg(cppEnumValue));
        return QString();
    }

    QStringList names = cppEnumValue.split(QLatin1String("::"));
    AbstractMetaClass *javaClass = m_java_classes.findClass(names.at(0));

    if (!javaClass) {
        ReportHandler::warning(QString::fromLatin1("Class '%1' is unknown").arg(names.at(0)));
        return QString();
    }

    AbstractMetaEnum *javaEnum = javaClass->findEnumForValue(names.at(1));
    if (!javaEnum) {
        ReportHandler::warning(QString::fromLatin1("Enum value '%1' was not found in '%2'")
                               .arg(names.at(1)).arg(names.at(0)));
        return QString();
    }

    AbstractMetaEnumValueList enumValues = javaEnum->values();
    AbstractMetaEnumValue *enumValue = enumValues.find(names.at(1));
    int value = enumValue->value();

    if (javaEnum->typeEntry()->isEnumValueRejected(enumValue->name())) {
        for (int i = 0; i < enumValues.size(); ++i) {
            AbstractMetaEnumValue *ev = enumValues.at(i);
            if (ev->value() == value) {
                enumValue = ev;
                break;
            }
        }
    }

    return javaEnum->fullName() + QLatin1String(".") + enumValue->name();
}

const AbstractMetaFunction *UiConverter::findFunction(AbstractMetaClass *javaClass,
        const QString &signature,
        SearchType type) {
    AbstractMetaFunctionList senderFunctions = javaClass->functions();
    foreach(const AbstractMetaFunction *f, senderFunctions) {
        if (type == SignalSearch && !f->isSignal())
            continue;

        QString fsig = f->minimalSignature();


        int pos = 0;
        while (pos < signature.length()
                && fsig.constData()[pos] == signature.constData()[pos]) ++pos;

        if (pos == signature.length()
                || (type == SignalSearch
                    && pos == signature.length() - 1
                    && signature.constData()[pos] == QLatin1Char(')'))) {
            return f;
        }
    }

    return 0;
}
