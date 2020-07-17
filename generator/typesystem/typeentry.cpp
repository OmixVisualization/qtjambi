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

#include <QtCore/QSet>
#include "typeentry.h"
#include "codesnip.h"
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
        table["void"] = "Void";
    }
    Q_ASSERT(table.contains(targetLangName()));
    return table[targetLangName()];
}

QString EnumTypeEntry::jniName() const {
    switch(m_size){
    case 8: return "jbyte";
    case 16: return "jshort";
    case 64: return "jlong";
    default: break;
    }
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
    return javaPackage().isEmpty() ? (
                m_enum->javaQualifier().isEmpty() ? targetLangName()
                                                  : m_enum->javaQualifier() + "." + targetLangName()
            ) : (
                m_enum->javaQualifier().isEmpty() ? javaPackage() + "." + targetLangName()
                                                  : javaPackage() + "." + m_enum->javaQualifier() + "." + targetLangName()
            );
}

QString FlagsTypeEntry::qualifiedTargetLangJNIName() const {
    QString pkg = javaPackage().replace('.', '/');
    return pkg.isEmpty() ? (
                m_enum->javaQualifier().isEmpty() ? targetLangName()
                                                  : m_enum->javaQualifier() + "$" + targetLangName()
            ) : (
                m_enum->javaQualifier().isEmpty() ? pkg + "/" + targetLangName()
                                                  : pkg + "/" + m_enum->javaQualifier() + "$" + targetLangName()
            );
}

QString FunctionalTypeEntry::javaQualifier() const {
    if(!m_qualifier_type)
        m_qualifier_type = TypeDatabase::instance()->findType(m_qualifier);
    if (m_qualifier_type){
        if(m_qualifier_type->isVariant())
            return "QVariant";
        if(m_qualifier_type->designatedInterface()){
            return m_qualifier_type->designatedInterface()->targetLangName();
        }
        return m_qualifier_type->targetLangName();
    }else
        return m_qualifier;
}

QString EnumTypeEntry::javaQualifier() const {
    if(!m_qualifier_type)
        m_qualifier_type = TypeDatabase::instance()->findType(m_qualifier);
    if (m_qualifier_type){
        if(m_qualifier_type->isVariant())
            return "QVariant";
        if(m_qualifier_type->designatedInterface()){
            return m_qualifier_type->designatedInterface()->targetLangName();
        }
        return m_qualifier_type->targetLangName();
    }else
        return m_qualifier;
}

QString ContainerTypeEntry::javaPackage() const {
    if (m_type == PairContainer)
        return "io.qt.core";
    if (m_type == QDBusReplyContainer)
        return "io.qt.dbus";
    if (m_type == QQmlListPropertyContainer)
        return "io.qt.qml";
    if (m_type == QDeclarativeListPropertyContainer)
        return "io.qt.declarative";  // new for QDeclarative module but not yet implemented
    if (m_type == QArrayContainer
            || m_type == QVector2DArrayContainer
            || m_type == QVector3DArrayContainer
            || m_type == QVector4DArrayContainer)
        return "io.qt.declarative3d";
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
        case InitializerListContainer: return "List";
        case LinkedListContainer: return "LinkedList";
        case VectorContainer: return "List";
        case QArrayDataContainer: return "Collection";
        case QTypedArrayDataContainer: return "Collection";
        case StackContainer: return "Deque";
        case QueueContainer: return "Queue";
        case SetContainer: return "Set";
        case MapContainer: return "NavigableMap";
        case QDBusReplyContainer: return "QDBusReply";
        case MultiMapContainer: return "NavigableMap";
        case HashContainer: return "Map";
        case MultiHashContainer: return "Map";
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

QString ContainerTypeEntry::collectionWrapperClass()const{
    switch (m_type) {
        case ListContainer: return "QtJambiListObject";
        case QArrayDataContainer: return "QtJambiCollectionObject";
        case QTypedArrayDataContainer: return "QtJambiCollectionObject";
        case LinkedListContainer: return "QtJambiLinkedListObject";
        case VectorContainer: return "QtJambiVectorObject";
        case StackContainer: return "QtJambiStackObject";
        case QueueContainer: return "QtJambiQueueObject";
        case SetContainer: return "QtJambiSetObject";
        case MapContainer: return "QtJambiMapObject";
        case HashContainer: return "QtJambiHashObject";
        case MultiHashContainer: return "QtJambiMultiHashObject";
        case MultiMapContainer: return "QtJambiMultiMapObject";
        default:
            break;
    }
    return QString();
}

QString ContainerTypeEntry::qualifiedCppName() const {
    if (m_type == StringListContainer)
        return "QStringList";
    if (m_type == InitializerListContainer)
        return "std::initializer_list";
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

QString IteratorTypeEntry::targetLangName() const {
//    return (m_containerType ? m_containerType->targetLangName()+"$" : "") + ComplexTypeEntry::targetLangName();
    return "QIterator";
}

QString IteratorTypeEntry::javaPackage() const {
//    return m_containerType ? m_containerType->javaPackage() : ComplexTypeEntry::javaPackage();
    return "io.qt.core";
}

const QString& IteratorTypeEntry::qualifiedCppContainerName() const {
    return m_qualifiedCppContainerName;
}

QString IteratorTypeEntry::qualifiedCppName() const {
    if(!m_qualifiedCppContainerName.isEmpty()){
        return m_qualifiedCppContainerName+"::"+ComplexTypeEntry::qualifiedCppName().split("::").last();
    }else{
        return m_containerType ? m_containerType->qualifiedCppName() + "::" + ComplexTypeEntry::qualifiedCppName().split("::").last() : ComplexTypeEntry::qualifiedCppName();
    }
}

QString IteratorTypeEntry::iteratorName() const {
    return ComplexTypeEntry::qualifiedCppName();
}

IteratorTypeEntry* IteratorTypeEntry::clone(const ComplexTypeEntry* containerType, const QString& qualifiedCppContainerName) const {
    IteratorTypeEntry* entry = new IteratorTypeEntry(ComplexTypeEntry::qualifiedCppName(), qualifiedCppContainerName, containerType, m_isPointer);
    if(containerType->typeFlags()==ComplexTypeEntry::ContainerType){
        entry->setCodeGeneration(GenerateForSubclass);
    }else{
        entry->setCodeGeneration(containerType->codeGeneration());
    }
    entry->setCodeGeneration(GenerateNothing);
//    entry->setInclude(include());
//    entry->setTypeFlags(typeFlags());
//    entry->setPreferredConversion(preferredConversion());
    entry->setDefaultSuperclass(defaultSuperclass());
    entry->setImplements(implements());
//    entry->setPolymorphicIdValue(polymorphicIdValue());
//    entry->setExpensePolicy(expensePolicy());
    entry->setExtraIncludes(extraIncludes());
//    entry->setIsPolymorphicBase(isPolymorphicBase());
//    entry->setLookupName(lookupName());
    entry->setTargetLangName(ComplexTypeEntry::targetLangName());
    return entry;
}

bool ComplexTypeEntry::hasFunctionCodeInjections(const QString &methodSignature, TypeSystem::Language language, const QSet<CodeSnip::Position>& positions) const{
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == methodSignature) {
            if (mod.snips.count() <= 0)
                continue ;
            for(const CodeSnip& snip : mod.snips) {
                if (!positions.contains(snip.position))
                    continue;

                if (!(snip.language & language))
                    continue;

                if(!snip.code().isEmpty())
                    return true;
            }
        }
    }
    return false;
}

QList<ArgumentModification> ComplexTypeEntry::functionArgumentModifications(const QString &signature) const {
    QList<ArgumentModification> lst;
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == signature) {
            lst << mod.argument_mods;
        }
    }
    return lst;
}

FunctionModificationList ComplexTypeEntry::functionModifications(const QString &signature) const {
    FunctionModificationList lst;
    for (const FunctionModification& mod : m_function_mods) {
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
    if(isInterface()){
        return static_cast<const InterfaceTypeEntry*>(this)->origin()->fieldModification(name);
    }
    FieldModification mod;
    mod.name = name;
    mod.modifiers = FieldModification::Readable | FieldModification::Writable;
    return mod;
}

QString ComplexTypeEntry::qualifiedCppName() const {
    if(designatedInterface()){
        return designatedInterface()->qualifiedCppName();
    }
    return m_qualified_cpp_name;
}

bool ObjectTypeEntry::isValueOwner() const
{
    return m_is_value_owner;
}

void ObjectTypeEntry::setIsValueOwner(bool is_value_owner)
{
    m_is_value_owner = is_value_owner;
}
