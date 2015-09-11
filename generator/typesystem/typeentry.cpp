/****************************************************************************
**
** Copyright (C) 2013-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
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

#include "typeentry.h"
#include "typedatabase.h"

QString PrimitiveTypeEntry::javaObjectName() const {
    static QHash<QString, QString> table;
    if (table.isEmpty()) {
        table["boolean"] = "Boolean";
        table["byte"] = "Byte";
        table["char"] = "Character";
        table["short"] = "Short";
        table["int"] = "Integer";
        table["long"] = "Long";
        table["float"] = "Float";
        table["double"] = "Double";
    }
    Q_ASSERT(table.contains(targetLangName()));
    return table[targetLangName()];
}

QString EnumTypeEntry::jniName() const {
    return "jint";
}

QString FlagsTypeEntry::jniName() const {
    return "jint";
}

void EnumTypeEntry::addEnumValueRedirection(const QString &rejected, const QString &usedValue) {
    m_enum_redirections << EnumValueRedirection(rejected, usedValue);
}

QString EnumTypeEntry::enumValueRedirection(const QString &value) const {
    for (int i = 0; i < m_enum_redirections.size(); ++i)
        if (m_enum_redirections.at(i).rejected == value)
            return m_enum_redirections.at(i).used;
    return QString();
}

QString FlagsTypeEntry::qualifiedTargetLangName() const {
    return javaPackage() + "." + m_enum->javaQualifier() + "." + targetLangName();
}

QString EnumTypeEntry::javaQualifier() const {
    TypeEntry *te = TypeDatabase::instance()->findType(m_qualifier);
    if (te != 0)
        return te->targetLangName();
    else
        return m_qualifier;
}

QString ContainerTypeEntry::javaPackage() const {
    if (m_type == PairContainer)
        return "org.qtjambi.qt";
    if (m_type == QQmlListPropertyContainer)
        return "org.qtjambi.qt.qml";
    if (m_type == QDeclarativeListPropertyContainer)
        return "org.qtjambi.qt.declarative";  // new for QDeclarative module but not yet implemented
    return "java.util";
}

QString ContainerTypeEntry::targetLangName() const {

    switch (m_type) {
        case QArrayContainer: return "List"; // new since Qt3D
        case QVector2DArrayContainer: return "List"; // new since Qt3D
        case QVector3DArrayContainer: return "List"; // new since Qt3D
        case QVector4DArrayContainer: return "List"; // new since Qt3D
        case StringListContainer: return "List";
        case ByteArrayListContainer: return "List";
        case ListContainer: return "List";
        case LinkedListContainer: return "LinkedList";
        case VectorContainer: return "List";
        case StackContainer: return "Stack";
        case QueueContainer: return "Queue";
        case SetContainer: return "Set";
        case MapContainer: return "SortedMap";
        case MultiMapContainer: return "SortedMap";
        case HashContainer: return "HashMap";
            //     case MultiHashCollectio: return "MultiHash";
        case PairContainer: return "QPair";
        case QDeclarativeListPropertyContainer: return "QDeclarativeListProperty"; // new for QDeclarative module but not yet implemented
        case QQmlListPropertyContainer: return "QQmlListProperty"; // new for QtQml module
        default:
            qWarning("bad type... %d", m_type);
            break;
    }
    return QString();
}

QString ContainerTypeEntry::qualifiedCppName() const {
    if (m_type == StringListContainer)
        return "QStringList";
    if (m_type == ByteArrayListContainer)
        return "QByteArrayList";
    if (m_type == QArrayContainer)
        return "QArray";  // new since Qt3D
    if (m_type == QVector2DArrayContainer)
        return "QVector2DArray";  // new since Qt3D
    if (m_type == QVector3DArrayContainer)
        return "QVector3DArray";  // new since Qt3D
    if (m_type == QVector4DArrayContainer)
        return "QVector4DArray";  // new since Qt3D
    if (m_type == QDeclarativeListPropertyContainer)
        return "QDeclarativeListProperty";   // new for QDeclarative module but not yet implemented
    if (m_type == QQmlListPropertyContainer)
        return "QQmlListProperty";   // new for QDeclarative module but not yet implemented
    return ComplexTypeEntry::qualifiedCppName();
}

FunctionModificationList ComplexTypeEntry::functionModifications(const QString &signature) const {
    FunctionModificationList lst;
    for (int i = 0; i < m_function_mods.count(); ++i) {
        FunctionModification mod = m_function_mods.at(i);
        if (mod.signature == signature) {
            lst << mod;
        }
    }

    return lst;
}

FieldModification ComplexTypeEntry::fieldModification(const QString &name) const {
    for (int i = 0; i < m_field_mods.size(); ++i)
        if (m_field_mods.at(i).name == name)
            return m_field_mods.at(i);
    FieldModification mod;
    mod.name = name;
    mod.modifiers = FieldModification::Readable | FieldModification::Writable;
    return mod;
}
