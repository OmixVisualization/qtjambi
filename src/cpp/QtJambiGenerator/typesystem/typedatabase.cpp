/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
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

#include "typedatabase.h"

#include <QDebug>

#include <qfile.h>
#include <qfileinfo.h>
#include <QtXml>
#include <QtCore>
#include "qmltypesystemreader.h"
#include "xmltypesystemreader.h"
#include "xmltoqmlconverter.h"
#include "../reporthandler.h"

namespace TS{

void rename(ComplexTypeEntry *e,
                       const char *signature,
                       const char * newName);

void removeFunction(ComplexTypeEntry *e, const char *signature);

void protectedAccess(ComplexTypeEntry *e, const char *signature);

void privateAccess(ComplexTypeEntry *e, const char *signature);

void injectCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args);

#if 0

void injectTargetLangCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args);

void injectCode(ComplexTypeEntry *e,
                       const QByteArray &code);

void injectTargetLangCode(ComplexTypeEntry *e,
                       const QByteArray &code);

#endif

IteratorTypeEntry* createListIterator(ContainerTypeEntry* entry, bool isMap, const QString &name = QString("const_iterator"));

TypeDatabase::TypeDatabase() :
    m_qtVersion(QLibraryInfo::version()),
    m_suppressWarnings(true),
    m_includeEclipseWarnings(true),
    m_pixmapType(nullptr),
    m_bitmapType(nullptr),
    m_qstringType(nullptr),
    m_qvariantType(nullptr),
    m_qcharType(nullptr){
}

TypeDatabase* TypeDatabase::instance(){
    static TypeDatabase _instance;
    return &_instance;
}

TypeEntry *TypeDatabase::findType(const QString &name) const {
    QList<TypeEntry *> entries = findTypes(name);
    for(TypeEntry *entry : entries) {
        //qDebug()<<"findType()"<<entry;
        if (entry &&
                (!entry->isPrimitive() ||
                    static_cast<PrimitiveTypeEntry *>(entry)->preferredTargetLangType())) {
            return entry;
        }
    }
    return nullptr;
}

SingleTypeEntryHash TypeDatabase::entries() {
    TypeEntryHash entries = allEntries();

    SingleTypeEntryHash returned;
    QList<QString> keys = entries.keys();

    for(const QString& key : keys) {
        returned[key] = findType(key);
    }

    return returned;
}

bool TypeDatabase::isSuppressedWarning(const QString &s) {
    if (!m_suppressWarnings)
        return false;

    for(const QString &_warning : m_suppressedWarnings) {
        QString warning(QString(_warning).replace("\\*", "&place_holder_for_asterisk;"));

        QStringList segs = warning.split("*", Qt::SkipEmptyParts);
        if (segs.size() == 0)
            continue ;

        int i = 0;
        auto pos = s.indexOf(QString(segs.at(i++)).replace("&place_holder_for_asterisk;", "*"));
        //qDebug() << "s == " << s << ", warning == " << segs;
        while (pos != -1) {
            if (i == segs.size())
                return true;
            pos = s.indexOf(QString(segs.at(i++)).replace("&place_holder_for_asterisk;", "*"), pos);
        }
    }

    return false;
}

PrimitiveTypeEntry *TypeDatabase::findPrimitiveType(const QString &name) const{
    QList<TypeEntry *> entries = findTypes(name);

    for(TypeEntry *entry : entries) {
        if (entry && entry->isPrimitive() && static_cast<PrimitiveTypeEntry *>(entry)->preferredTargetLangType())
            return static_cast<PrimitiveTypeEntry *>(entry);
    }

    return nullptr;
}

ComplexTypeEntry *TypeDatabase::findComplexType(const QString &name) const {
    TypeEntry *entry = findType(name);
    if (entry && entry->isComplex())
        return static_cast<ComplexTypeEntry *>(entry);
    else
        return nullptr;
}

FunctionalTypeEntry *TypeDatabase::findFunctionalType(const QString &name) const {
    TypeEntry *entry = findType(name);
    if (entry && entry->isFunctional())
        return static_cast<FunctionalTypeEntry *>(entry);
    else
        return nullptr;
}

FunctionalTypeEntry *TypeDatabase::findFunctionalTypeByUsing(const QString &containingClassName, const QString &_using) const {
    for(QList<TypeEntry *> entries : m_entries){
        for(TypeEntry *entry : entries){
            if (entry && entry->isFunctional())
            {
                FunctionalTypeEntry * fentry = static_cast<FunctionalTypeEntry *>(entry);
                if(!fentry->getUsing().isEmpty()){
                    if(( fentry->getUsing()==_using
                            || QString::fromUtf8(QMetaObject::normalizedSignature(qPrintable(fentry->getUsing())))==_using )
                            && (containingClassName.isEmpty() || fentry->name().startsWith(containingClassName+"::"))){
                        return fentry;
                    }
                }
            }
        }
    }
    return nullptr;
}

ObjectTypeEntry *TypeDatabase::findObjectType(const QString &name) const {
    TypeEntry *entry = findType(name);
    if (entry && entry->isObject())
        return static_cast<ObjectTypeEntry *>(entry);
    else
        return nullptr;
}

NamespaceTypeEntry *TypeDatabase::findNamespaceType(const QString &name) const {
    TypeEntry *entry = findType(name);
    if (entry && entry->isNamespace())
        return static_cast<NamespaceTypeEntry *>(entry);
    else
        return nullptr;
}

void TypeDatabase::addType(TypeEntry *e) {
    if(e->qualifiedCppName()=="QVariant" && !e->isQVariant()){
        m_entries["QtJambiVariant"].append(e);
        m_qvariantType = dynamic_cast<ComplexTypeEntry*>(e);
    }else if(e->qualifiedCppName()=="QString" && !e->isQString()){
        m_entries["QtJambiString"].append(e);
        m_qstringType = dynamic_cast<ComplexTypeEntry*>(e);
    }else if(e->qualifiedCppName()=="QChar" && !e->isQChar()){
        m_entries["QtJambiChar"].append(e);
        m_qcharType = dynamic_cast<ComplexTypeEntry*>(e);
    }else{
        m_entries[e->qualifiedCppName()].append(e);
    }
    QString name = e->qualifiedCppName().contains("::") ? e->qualifiedCppName().split("::").last() : e->qualifiedCppName();
    m_class_name_counter[name]++;
    if(e->type()==TypeEntry::TypeSystemType){
        TypeSystemTypeEntry* tsentry = static_cast<TypeSystemTypeEntry*>(e);
        if(!tsentry->qtLibrary().isEmpty()){
            m_typeSystemsByQtLibrary[tsentry->qtLibrary()] = tsentry;
        }else if(!tsentry->targetName().isEmpty()){
            m_typeSystemsByQtLibrary[tsentry->targetName()] = tsentry;
        }else{
            m_otherTypeSystems << tsentry;
        }
    }
    if(!m_pixmapType && e->qualifiedCppName()==QLatin1String("QPixmap")){
        m_pixmapType = dynamic_cast<ComplexTypeEntry*>(e);
    }
    if(!m_bitmapType && e->qualifiedCppName()==QLatin1String("QBitmap")){
        m_bitmapType = dynamic_cast<ComplexTypeEntry*>(e);
    }
}

QList<TypeSystemTypeEntry*> TypeDatabase::typeSystems() const{
    QList<TypeSystemTypeEntry*> result = m_otherTypeSystems;
    result << m_typeSystemsByQtLibrary.values();
    return result;
}

void TypeDatabase::initialize(const QString &filename, const QStringList &importInputDirectoryList, const QStringList &typeystemDirectory, const QVersionNumber& qtVersion, bool generate) {
    m_qtVersion = qtVersion;
    m_entries.clear();
    m_class_name_counter.clear();
    m_rejections.clear();
    m_rebuildClasses.clear();
    m_flags_entries.clear();
    m_templates.clear();
    m_templateTypes.clear();
    m_suppressedWarnings.clear();
    m_typeSystemsByQtLibrary.clear();
    ContainerTypeEntry* stringListEntry(nullptr);
    {
        addType(new AutoTypeEntry());
        {
            StringTypeEntry *e = new StringTypeEntry("QString");
            e->setCodeGeneration(generate ? TypeEntry::GenerateAll : TypeEntry::GenerateForSubclass);
            addType(e);
        }

        Q_UNUSED(&privateAccess)
        {
            StringTypeEntry *e = new StringTypeEntry("QLatin1String");
            e->setPreferredConversion(false);
            addType(e);
        }

        if(m_qtVersion >= QVersionNumber(6,4,0)){
            StringTypeEntry *e = new StringTypeEntry("QLatin1StringView");
            e->setPreferredConversion(false);
            addType(e);
        }

        {
            StringTypeEntry *e = new StringTypeEntry("QStringView");
            e->setPreferredConversion(false);
            addType(e);
        }

        {
            StringTypeEntry *e = new StringTypeEntry("QAnyStringView");
            e->setPreferredConversion(false);
            addType(e);
        }

        {
            StringTypeEntry *e = new StringTypeEntry("QUtf8StringView");
            e->setPreferredConversion(false);
            addType(e);
        }

        if(m_qtVersion < QVersionNumber(6,0,0)){
            // We need the generator to perform type conversion in C++ with the
            //  construct:
            // QString qstring = QString("string"); QStringRef(&qstring)"
            //  not with:
            // (const QStringRef &)QString("string")
            StringTypeEntry *sr = new StringTypeEntry("QStringRef");
            sr->setPreferredConversion(false);
            addType(sr);
            // TODO: Use of StringRefTypeEntry for QXmlStreamStringRef has not been tested,
            //  I am sure the previous code would cause a crash.
            sr = new StringTypeEntry("QXmlStreamStringRef");
            sr->setPreferredConversion(false);
            addType(sr);
        }

        addType(new CharTypeEntry("QChar"));

        {
            CharTypeEntry *c = new CharTypeEntry("QLatin1Char");
            c->setPreferredConversion(false);
            addType(c);
        }

        {
            VariantTypeEntry *qvariant = new VariantTypeEntry("QVariant");
            qvariant->setTargetTypeSystem("QtCore");
            qvariant->setTargetLangPackage("io.qt.core");
            qvariant->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(qvariant);
        }

        {
            JWrapperTypeEntry *wrapper = new JWrapperTypeEntry(TypeEntry::JObjectWrapperType, "JObjectWrapper", strings_java_lang, strings_Object);
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);

            wrapper = new JWrapperTypeEntry(TypeEntry::QAndroidJniObjectType, "QAndroidJniObject", strings_java_lang, strings_Object);
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);

            wrapper = new JWrapperTypeEntry(TypeEntry::JMapWrapperType, "JMapWrapper", QLatin1String("java.util"), QLatin1String("Map"));
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);

            wrapper = new JWrapperTypeEntry(TypeEntry::JCollectionWrapperType, "JCollectionWrapper", QLatin1String("java.util"), QLatin1String("Collection"));
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);

            wrapper = new JWrapperTypeEntry(TypeEntry::JIteratorWrapperType, "JIteratorWrapper", QLatin1String("java.util"), QLatin1String("Iterator"));
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);

            wrapper = new JWrapperTypeEntry(TypeEntry::JEnumWrapperType, "JEnumWrapper", QLatin1String("java.lang"), QLatin1String("Enum"));
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);

            wrapper = new JWrapperTypeEntry(TypeEntry::JQFlagsWrapperType, "JQFlagsWrapper", QLatin1String("io.qt"), QLatin1String("QFlags"));
            wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(wrapper);
        }

        addType(new VoidTypeEntry());

        ArgumentMap args1;
        args1[1] = QLatin1String("$1");
        ArgumentMap args2 = args1;
        args2[2] = QLatin1String("$2");

        QByteArray code =
            "\nif ($1 >= __qt_this->size() || $1 < 0) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 at %2\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($1)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return;"
            "\n}";

        QByteArray code_2_args =
            "\nif ($1 >= __qt_this->size() || $1 < 0) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 at %2\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($1)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return;"
            "\n}"
            "\nif ($2 >= __qt_this->size() || $2 < 0) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 at %2\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($2)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return;"
            "\n}";

        QByteArray code_2_args_mid =
            "\nif ($1 >= __qt_this->size() || $1 < 0) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 at %2\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($1)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return $DEFAULT_VALUE_RETURN;"
            "\n}"
            "\nif ($2>=0 && $1+$2 >= __qt_this->size()) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 from %2 to %3\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($1)).arg(QString::number($1+$2)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return $DEFAULT_VALUE_RETURN;"
            "\n}";

        QByteArray code_with_return = QByteArray(code).replace("return;", "return $DEFAULT_VALUE_RETURN;");

        QByteArray code_index_length =
            "\nif ($1 < 0 || $2 < 0 || ($1 + $2) >= __qt_this->size()) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 from %2 to %3\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($1)).arg(QString::number($1+$2)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return;"
            "\n}";

        QByteArray code_non_empty =
            "\nif (__qt_this->isEmpty()) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing empty container...\").toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return;"
            "\n}";

        QByteArray code_non_empty_with_return = QByteArray(code_non_empty).replace("return;", "return $DEFAULT_VALUE_RETURN;");

        QByteArray code_two_indices =
            "\nif ($1 < 0 || $2 < 0 || $1 >= __qt_this->size() || $2 >= __qt_this->size()) {"
            "\n   JavaException::raiseIndexOutOfBoundsException(__jni_env,"
            "\n                       QString::fromLatin1(\"Accessing container of size %1 at %2 and at %3\")"
            "\n                       .arg(QString::number(__qt_this->size())).arg(QString::number($1)).arg(QString::number($2)).toLatin1() QTJAMBI_STACKTRACEINFO );"
            "\n   return;"
            "\n}";

        // Predefined containers...
        addType(new ContainerTypeEntry("std::vector", ContainerTypeEntry::std_vector));
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QList", ContainerTypeEntry::ListContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QList"));
            entry->addExtraInclude(Include(Include::IncludePath, "QtCore/QQueue"));
            removeFunction(entry, "swap(QList<T> &)");
            injectCode(entry, "at(int) const", code_with_return, args1);
            injectCode(entry, "replace(int, T)", code, args1);
            injectCode(entry, "move(int,int)", code_2_args, args2);
            injectCode(entry, "mid(int,int)const", code_2_args_mid, args2);
            injectCode(entry, "pop_back()", code_non_empty, ArgumentMap());
            injectCode(entry, "pop_front()", code_non_empty, ArgumentMap());
            injectCode(entry, "removeLast()", code_non_empty, ArgumentMap());
            injectCode(entry, "removeFirst()", code_non_empty, ArgumentMap());
            injectCode(entry, "takeLast()", code_non_empty_with_return, ArgumentMap());
            injectCode(entry, "takeFirst()", code_non_empty_with_return, ArgumentMap());
            injectCode(entry, "swap(int, int)", code_two_indices, args2);
            injectCode(entry, "swapItemsAt(int, int)", code_two_indices, args2);
            injectCode(entry, "move(int, int)", code_two_indices, args2);
            injectCode(entry, "removeAt(int)", code, args1);
            injectCode(entry, "takeAt(int)", code_with_return, args1);
            removeFunction(entry, "begin()");
            removeFunction(entry, "end()");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "crbegin() const");
            removeFunction(entry, "crend() const");
            removeFunction(entry, "rbegin()");
            removeFunction(entry, "rbegin() const");
            removeFunction(entry, "rend()");
            removeFunction(entry, "rend() const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "constFirst() const");
            removeFunction(entry, "constLast() const");
            removeFunction(entry, "constBack() const");
            removeFunction(entry, "constData() const");
            removeFunction(entry, "data() const");
            removeFunction(entry, "data()");
            removeFunction(entry, "back()");
            removeFunction(entry, "front()");
            removeFunction(entry, "first()");
            removeFunction(entry, "last()");
            removeFunction(entry, "operator[](int)");
            removeFunction(entry, "operator[](int) const");
            removeFunction(entry, "operator+(const QList<T> &) const");
            removeFunction(entry, "operator+=(const QList<T> &)");
            removeFunction(entry, "operator+=(const T&)");
            removeFunction(entry, "operator<<(const QList<T> &)");
            removeFunction(entry, "operator<<(const T&)");
            removeFunction(entry, "operator=(QList<T>)");
            removeFunction(entry, "operator=(const QList<T>&)");
            removeFunction(entry, "toVector() const");
            protectedAccess(entry, "begin() const");
            protectedAccess(entry, "end() const");
            if(qtVersion >= QVersionNumber(6,0,0)){
                rename(entry, "size() const", "longSize");
                rename(entry, "indexOf(const T &, qsizetype) const", "longIndexOf");
                rename(entry, "lastIndexOf(const T &, qsizetype) const", "lastLongIndexOf");
                removeFunction(entry, "operator[](qsizetype)");
                removeFunction(entry, "operator[](qsizetype) const");
                removeFunction(entry, "operator=(std::initializer_list<T>)");
                injectCode(entry, "at(qsizetype) const", code_with_return, args1);
                injectCode(entry, "replace(qsizetype, T)", code, args1);
                injectCode(entry, "move(qsizetype,qsizetype)", code_2_args, args2);
                injectCode(entry, "mid(qsizetype,qsizetype)const", code_2_args_mid, args2);
                injectCode(entry, "swap(qsizetype, qsizetype)", code_two_indices, args2);
                injectCode(entry, "swapItemsAt(qsizetype, qsizetype)", code_two_indices, args2);
                injectCode(entry, "move(qsizetype, qsizetype)", code_two_indices, args2);
                injectCode(entry, "removeAt(qsizetype)", code, args1);
                injectCode(entry, "takeAt(qsizetype)", code_with_return, args1);
            }
            removeFunction(entry, "append(QList::const_iterator,QList::const_iterator)");
            addType(entry);
            IteratorTypeEntry* iteratorEntry = createListIterator(entry, false);
            addType(iteratorEntry);
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QQueue"));
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QStringList"));
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QByteArrayList"));
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QModelRoleDataSpan", ContainerTypeEntry::QModelRoleDataSpanContainer);
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QBindable", ContainerTypeEntry::QBindableContainer);
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QPropertyBinding", ContainerTypeEntry::QPropertyBindingContainer);
            addType(entry);
        }
        {
            stringListEntry = new ContainerTypeEntry("QStringList", ContainerTypeEntry::StringListContainer);
            stringListEntry->setInclude(Include(Include::IncludePath, "QtCore/QStringList"));
            stringListEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QList"));
            stringListEntry->setTargetTypeSystem("io.qt.core");
            if(qtVersion < QVersionNumber(6,0,0)){
                removeFunction(stringListEntry, "contains(QLatin1String, Qt::CaseSensitivity) const");
                removeFunction(stringListEntry, "contains(QStringView, Qt::CaseSensitivity) const");
                removeFunction(stringListEntry, "operator+(const QStringList &) const");
                removeFunction(stringListEntry, "operator<<(const QString &)");
                removeFunction(stringListEntry, "operator<<(const QStringList &)");
                removeFunction(stringListEntry, "operator<<(const QList<QString> &)");
                removeFunction(stringListEntry, "operator<<(const QString &)");
                removeFunction(stringListEntry, "indexOf(QStringView, int) const");
                removeFunction(stringListEntry, "indexOf(QLatin1String, int) const");
                removeFunction(stringListEntry, "lastIndexOf(QStringView, int) const");
                removeFunction(stringListEntry, "lastIndexOf(QLatin1String, int) const");
                removeFunction(stringListEntry, "indexOf(QRegExp &, int) const");
                removeFunction(stringListEntry, "lastIndexOf(QRegExp &, int) const");
                removeFunction(stringListEntry, "operator=(const QList<QString> &)");
            }
            addType(stringListEntry);
        }
        if(qtVersion < QVersionNumber(6,0,0)){
            ContainerTypeEntry* entry = new ContainerTypeEntry("QLinkedList", ContainerTypeEntry::LinkedListContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QLinkedList"));
            removeFunction(entry, "swap(QLinkedList<T> &)");
            injectCode(entry, "at(int) const", code_with_return, args1);
            injectCode(entry, "replace(int, T)", code, args1);
            injectCode(entry, "move(int,int)", code_2_args, args2);
            injectCode(entry, "mid(int,int)const", code_2_args_mid, args2);
            injectCode(entry, "pop_back()", code_non_empty, ArgumentMap());
            injectCode(entry, "pop_front()", code_non_empty, ArgumentMap());
            injectCode(entry, "removeLast()", code_non_empty, ArgumentMap());
            injectCode(entry, "removeFirst()", code_non_empty, ArgumentMap());
            injectCode(entry, "takeLast()", code_non_empty_with_return, ArgumentMap());
            injectCode(entry, "takeFirst()", code_non_empty_with_return, ArgumentMap());
            injectCode(entry, "swap(int, int)", code_two_indices, args2);
            injectCode(entry, "swapItemsAt(int, int)", code_two_indices, args2);
            injectCode(entry, "move(int, int)", code_two_indices, args2);
            injectCode(entry, "removeAt(int)", code, args1);
            injectCode(entry, "takeAt(int)", code_with_return, args1);
            removeFunction(entry, "begin()");
            removeFunction(entry, "end()");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "crbegin() const");
            removeFunction(entry, "crend() const");
            removeFunction(entry, "rbegin()");
            removeFunction(entry, "rbegin() const");
            removeFunction(entry, "rend()");
            removeFunction(entry, "rend() const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "constFirst() const");
            removeFunction(entry, "constLast() const");
            removeFunction(entry, "constBack() const");
            removeFunction(entry, "constData() const");
            removeFunction(entry, "data() const");
            removeFunction(entry, "data()");
            removeFunction(entry, "back()");
            removeFunction(entry, "front()");
            removeFunction(entry, "first()");
            removeFunction(entry, "last()");
            removeFunction(entry, "operator[](int)");
            removeFunction(entry, "operator[](int) const");
            removeFunction(entry, "operator+(const QLinkedList<T> &) const");
            removeFunction(entry, "operator+=(const QLinkedList<T> &)");
            removeFunction(entry, "operator+=(const T&)");
            removeFunction(entry, "operator<<(const QLinkedList<T> &)");
            removeFunction(entry, "operator<<(const T&)");
            removeFunction(entry, "operator=(QLinkedList<T>)");
            removeFunction(entry, "operator=(const QLinkedList<T>&)");
            removeFunction(entry, "toVector() const");
            protectedAccess(entry, "begin() const");
            protectedAccess(entry, "end() const");
            addType(entry);
            IteratorTypeEntry* iteratorEntry = createListIterator(entry, false);
            addType(iteratorEntry);
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QLinkedList"));
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QArrayData", ContainerTypeEntry::QArrayDataContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/qarraydata.h"));
            addType(entry);
            removeFunction(entry, "data()");
            removeFunction(entry, "data() const");
            EnumTypeEntry* enumType = new EnumTypeEntry("QArrayData", "AllocationOption");
            enumType->setFlags(new FlagsTypeEntry("AllocationOptions"));
            addType(enumType);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QTypedArrayData", ContainerTypeEntry::QTypedArrayDataContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/qarraydata.h"));
            addType(entry);
            removeFunction(entry, "begin(iterator)");
            removeFunction(entry, "end(iterator)");
            removeFunction(entry, "constBegin(const_iterator) const");
            removeFunction(entry, "constEnd(const_iterator) const");
            EnumTypeEntry* enumType = new EnumTypeEntry("QArrayData", "AllocationOption");
            enumType->setFlags(new FlagsTypeEntry("AllocationOptions"));
            addType(enumType);
            IteratorTypeEntry * iteratorEntry = createListIterator(entry, false);
            iteratorEntry->setIsPointer(true);
            iteratorEntry->setInclude(Include(Include::IncludePath, "QtCore/QVector"));
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QStack"));
        }
        //if(qtVersion < QVersionNumber(6,0,0))
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QVector", ContainerTypeEntry::VectorContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QVector"));
            addType(entry);
            removeFunction(entry, "swap(QVector<T> &)");
            injectCode(entry, "at(int) const", code_with_return, args1);
            injectCode(entry, "replace(int,T)", code, args1);
            injectCode(entry, "remove(int)", code, args1);
            injectCode(entry, "move(int,int)", code_2_args, args2);
            injectCode(entry, "mid(int,int)const", code_2_args_mid, args2);
            injectCode(entry, "remove(int, int)", code_index_length, args2);
            injectCode(entry, "pop_back()", code_non_empty, ArgumentMap());
            injectCode(entry, "pop_front()", code_non_empty, ArgumentMap());
            injectCode(entry, "removeLast()", code_non_empty, ArgumentMap());
            injectCode(entry, "removeFirst()", code_non_empty, ArgumentMap());
            injectCode(entry, "takeLast()", code_non_empty_with_return, ArgumentMap());
            injectCode(entry, "takeFirst()", code_non_empty_with_return, ArgumentMap());
            injectCode(entry, "swap(int, int)", code_two_indices, args2);
            injectCode(entry, "swapItemsAt(int, int)", code_two_indices, args2);
            injectCode(entry, "removeAt(int)", code, args1);
            injectCode(entry, "takeAt(int)", code_with_return, args1);
            removeFunction(entry, "constData() const");
            removeFunction(entry, "data() const");
            removeFunction(entry, "data()");
            removeFunction(entry, "first()");
            removeFunction(entry, "front()");
            removeFunction(entry, "last()");
            removeFunction(entry, "remove(int)");
            removeFunction(entry, "operator[](int)");
            removeFunction(entry, "operator[](int) const");
            removeFunction(entry, "operator=(QVector<T>)");
            removeFunction(entry, "operator!=(const QVector<T> &) const");
            removeFunction(entry, "operator+(const QVector<T> &) const");
            removeFunction(entry, "operator+=(const QVector<T> &)");
            removeFunction(entry, "operator+=(QVector<T>)");
            removeFunction(entry, "operator+=(QVector)");
            removeFunction(entry, "operator+=(const T &)");
            removeFunction(entry, "operator+=(T &&)");
            removeFunction(entry, "operator+=(T)");
            removeFunction(entry, "operator<<(const T &)");
            removeFunction(entry, "operator<<(const QVector<T> &)");
            removeFunction(entry, "operator<<(T &&)");
            removeFunction(entry, "operator=(const QVector<T> &)");
            removeFunction(entry, "back()");
            removeFunction(entry, "back()const");
            removeFunction(entry, "begin()");
            removeFunction(entry, "end()");
            removeFunction(entry, "cbegin()const");
            removeFunction(entry, "cend()const");
            removeFunction(entry, "constFirst()const");
            removeFunction(entry, "constLast()const");
            removeFunction(entry, "constBegin()const");
            removeFunction(entry, "constEnd()const");
            removeFunction(entry, "constData()const");
            removeFunction(entry, "erase(iterator)");
            removeFunction(entry, "erase(iterator,iterator)");
            removeFunction(entry, "append(QVector::const_iterator,QVector::const_iterator)");
            removeFunction(entry, "insert(QVector::const_iterator,const T &)");
            removeFunction(entry, "insert(QVector::const_iterator,int,const T &)");
            protectedAccess(entry, "insert(int, const T &)");
            {
                FunctionModification mod;
                ArgumentModification am(0);
                am.modified_type = "void";
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("fill(const T &, int)");
                entry->addFunctionModification(mod);
            }
            {
                FunctionModification mod;
                ArgumentModification am(2);
                am.removed_default_expression = true;
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("remove(int,int)");
                entry->addFunctionModification(mod);
            }
            IteratorTypeEntry * iteratorEntry = createListIterator(entry, false);
            iteratorEntry->setInclude(Include(Include::IncludePath, "QtCore/QVector"));
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QStack"));
            addType(iteratorEntry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QStack", ContainerTypeEntry::StackContainer);
            removeFunction(entry, "swap(QStack<T> &)");
            removeFunction(entry, "top()");
            injectCode(entry, "removeLast()", code_non_empty, ArgumentMap());
            removeFunction(entry, "removeFirst()");
            rename(entry, "removeLast()", "removeLastImpl");
            protectedAccess(entry, "removeLast()");
            entry->setInclude(Include(Include::IncludePath, "QtCore/QStack"));
            entry->addExtraInclude(Include(Include::TargetLangImport, "java.util.Deque"));
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QQueue", ContainerTypeEntry::QueueContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QQueue"));
            removeFunction(entry, "head()");
            removeFunction(entry, "swap(QQueue<T> &)");
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QSet", ContainerTypeEntry::SetContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QSet"));
            removeFunction(entry, "toList()const");
            removeFunction(entry, "begin()const");
            removeFunction(entry, "end()const");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "crbegin() const");
            removeFunction(entry, "crend() const");
            removeFunction(entry, "rbegin()");
            removeFunction(entry, "rbegin() const");
            removeFunction(entry, "rend()");
            removeFunction(entry, "rend() const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "constFind(constT&) const");
            removeFunction(entry, "contains(const QSet<T> &) const");
            removeFunction(entry, "empty() const");
            protectedAccess(entry, "begin()");
            protectedAccess(entry, "end()");
            removeFunction(entry, "erase(QSet::const_iterator)");
            removeFunction(entry, "erase(QSet::iterator)");
            removeFunction(entry, "find(const T &) const");
            removeFunction(entry, "find(const T &)");
            removeFunction(entry, "swap(QSet<T> &)");
            {
                FunctionModification mod;
                ArgumentModification am(0);
                am.modified_type = "void";
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("insert(const T &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("intersect(const QSet<T> &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("subtract(const QSet<T> &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("unite(const QSet<T> &)");
                entry->addFunctionModification(mod);
            }
            removeFunction(entry, "operator!=(const QSet<T> &) const");
            removeFunction(entry, "operator&(const QSet<T> &) const");
            removeFunction(entry, "operator&=(const QSet<T> &)");
            removeFunction(entry, "operator&=(const T &)");
            removeFunction(entry, "operator+(const QSet<T> &) const");
            removeFunction(entry, "operator+=(const QSet<T> &)");
            removeFunction(entry, "operator+=(const T &)");
            removeFunction(entry, "operator-(const QSet<T> &) const");
            removeFunction(entry, "operator-=(const QSet<T> &)");
            removeFunction(entry, "operator-=(const T &)");
            removeFunction(entry, "operator<<(const T &)");
            removeFunction(entry, "operator|(const QSet<T> &) const");
            removeFunction(entry, "operator|=(const QSet<T> &)");
            removeFunction(entry, "operator|=(const T &)");
            addType(entry);
            IteratorTypeEntry * iteratorEntry = createListIterator(entry, false, "iterator");
            iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QSet"));
            addType(iteratorEntry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QMap", ContainerTypeEntry::MapContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QMap"));
            entry->addExtraInclude(Include(Include::IncludePath, "QtCore/QMultiMap"));
            removeFunction(entry, "last()");
            removeFunction(entry, "first()");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "constFind() const");
            removeFunction(entry, "constFind()");
            removeFunction(entry, "constFind(const Key &) const");
            removeFunction(entry, "constKeyValueBegin() const");
            removeFunction(entry, "erase(QMap::iterator)");
            removeFunction(entry, "find(const Key &)");
            protectedAccess(entry, "find(const Key &)const");
            removeFunction(entry, "keyBegin() const");
            removeFunction(entry, "keyEnd() const");
            removeFunction(entry, "keyValueBegin() const");
            removeFunction(entry, "keyValueEnd() const");
            removeFunction(entry, "keyValueBegin()");
            removeFunction(entry, "keyValueEnd()");
            removeFunction(entry, "lowerBound(const Key &)");
            protectedAccess(entry, "lowerBound(const Key &) const");
            removeFunction(entry, "upperBound(const Key &)");
            protectedAccess(entry, "upperBound(const Key &) const");
            removeFunction(entry, "swap(QMap<Key, T> &)");
            removeFunction(entry, "operator!=(const QMap<Key, T> &) const");
            removeFunction(entry, "operator=(const QMap<Key, T> &)");
            removeFunction(entry, "operator=(QMap<Key, T> &&)");
            removeFunction(entry, "operator[](const Key &)");
            removeFunction(entry, "operator[](const Key &)const");
            removeFunction(entry, "insert(QMap::const_iterator, const Key &, const T &)");
            removeFunction(entry, "insertMulti(QMap::const_iterator, const Key &, const T &)");
            removeFunction(entry, "uniqueKeys() const");
            removeFunction(entry, "values(const Key &) const");
            removeFunction(entry, "insertMulti(const Key &, const T &)");
            removeFunction(entry, "insertMulti(QMap::const_iterator, const Key &, const T &)");
            removeFunction(entry, "unite(const QMap<Key, T> &)");
            protectedAccess(entry, "begin()");
            protectedAccess(entry, "end()");
            protectedAccess(entry, "begin() const");
            protectedAccess(entry, "end() const");
            {
                FunctionModification mod;
                ArgumentModification am(0);
                am.modified_type = "void";
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("insert(const Key &, const T &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("insertMulti(const Key &, const T &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("intersect(const QMap<Key, T> &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("subtract(const QMap<Key, T> &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("unite(const QMap<Key, T> &)");
                entry->addFunctionModification(mod);
            }
            if(qtVersion >= QVersionNumber(6,0,0)){
                rename(entry, "size()const", "longSize");
            }
            addType(createListIterator(entry, true));
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QMultiMap", ContainerTypeEntry::MultiMapContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QMultiMap"));
            removeFunction(entry, "last()");
            removeFunction(entry, "first()");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "constFind() const");
            removeFunction(entry, "constFind()");
            removeFunction(entry, "constKeyValueBegin() const");
            removeFunction(entry, "erase(QMap::iterator)");
            removeFunction(entry, "find(const Key &)");
            protectedAccess(entry, "find(const Key &)const");
            removeFunction(entry, "find(const Key &, const T &)");
            removeFunction(entry, "find(const Key &, const T &)const");
            removeFunction(entry, "constFind(const Key &)const");
            removeFunction(entry, "constFind(const Key &, const T &)const");
            removeFunction(entry, "keyBegin() const");
            removeFunction(entry, "keyEnd() const");
            removeFunction(entry, "keyValueBegin() const");
            removeFunction(entry, "keyValueEnd() const");
            removeFunction(entry, "keyValueBegin()");
            removeFunction(entry, "keyValueEnd()");
            removeFunction(entry, "lowerBound(const Key &)");
            protectedAccess(entry, "lowerBound(const Key &) const");
            removeFunction(entry, "upperBound(const Key &)");
            protectedAccess(entry, "upperBound(const Key &) const");
            removeFunction(entry, "swap(QMap<Key, T> &)");
            removeFunction(entry, "swap(QMap &)");
            removeFunction(entry, "swap(QMultiMap<Key, T> &)");
            removeFunction(entry, "swap(QMultiMap &)");
            removeFunction(entry, "operator!=(const QMap<Key, T> &) const");
            removeFunction(entry, "operator=(const QMap<Key, T> &)");
            removeFunction(entry, "operator!=(const QMultiMap<Key, T> &) const");
            removeFunction(entry, "operator=(const QMultiMap<Key, T> &)");
            removeFunction(entry, "operator=(QMap<Key, T> &&)");
            removeFunction(entry, "operator=(const QMap &)");
            removeFunction(entry, "operator=(const QMultiMap &)");
            removeFunction(entry, "operator=(QMap &&)");
            removeFunction(entry, "operator[](const Key &)");
            removeFunction(entry, "operator[](const Key &)const");
            removeFunction(entry, "operator+(const QMultiMap<Key, T> &)const");
            removeFunction(entry, "operator+=(const QMultiMap<Key, T> &)");
            removeFunction(entry, "operator+(const QMultiMap &)const");
            removeFunction(entry, "operator+=(const QMultiMap &)");
            removeFunction(entry, "insert(QMap::const_iterator, const Key &, const T &)");
            removeFunction(entry, "insertMulti(QMap::const_iterator, const Key &, const T &)");
            removeFunction(entry, "insert(const QMap<Key, T> &)");
            rename(entry, "values()const", "allValues");
            rename(entry, "remove(const Key &)", "removeFirst");
            removeFunction(entry, "unite(const QMultiMap<Key, T> &)");
            {
                FunctionModification mod;
                ArgumentModification am(0);
                am.modified_type = "void";
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("insert(const Key &, const T &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("insertMulti(const Key &, const T &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("intersect(const QMap<Key, T> &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("subtract(const QMap<Key, T> &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("unite(const QMap<Key, T> &)");
                entry->addFunctionModification(mod);
            }
            if(qtVersion >= QVersionNumber(6,0,0)){
                removeFunction(entry, "equal_range(const Key &)const");
                rename(entry, "size()const", "longSize");
                addType(createListIterator(entry, true));
            }
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QHash", ContainerTypeEntry::HashContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QHash"));
            entry->addExtraInclude(Include(Include::IncludePath, "QtCore/QMultiHash"));
            removeFunction(entry, "swap(QHash<Key, T> &)");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "constFind() const");
            removeFunction(entry, "constFind()");
            removeFunction(entry, "uniqueKeys() const");
            removeFunction(entry, "values(const Key &) const");
            removeFunction(entry, "insertMulti(const Key &, const T &)");
            removeFunction(entry, "unite(const QHash &)");
            removeFunction(entry, "insert(const QHash &)");
            protectedAccess(entry, "find(const Key &)const");
            protectedAccess(entry, "begin() const");
            protectedAccess(entry, "end() const");
            {
                FunctionModification mod;
                ArgumentModification am(0);
                am.modified_type = "void";
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("intersect(const QHash<K, V>  &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("subtract(const QHash<K, V>  &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("unite(const QHash<K, V>  &)");
                entry->addFunctionModification(mod);
            }
            if(qtVersion >= QVersionNumber(6,0,0)){
                rename(entry, "size()const", "longSize");
            }
            addType(createListIterator(entry, true));
            addType(entry);
        }
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QMultiHash", ContainerTypeEntry::MultiHashContainer);
            entry->setInclude(Include(Include::IncludePath, "QtCore/QMultiHash"));
            removeFunction(entry, "constFind(const Key &)const");
            removeFunction(entry, "constFind(const Key &, const T &)const");
            removeFunction(entry, "constBegin() const");
            removeFunction(entry, "constEnd() const");
            removeFunction(entry, "find(const Key &)");
            removeFunction(entry, "find(const Key &, const T &)");
            removeFunction(entry, "find(const Key &, const T &)const");
            removeFunction(entry, "swap(QHash<Key, T> &)");
            removeFunction(entry, "swap(QMultiHash<Key, T> &)");
            removeFunction(entry, "swap(QHash &)");
            removeFunction(entry, "swap(QMultiHash &)");
            removeFunction(entry, "cbegin() const");
            removeFunction(entry, "cend() const");
            removeFunction(entry, "operator+(const QMultiHash<Key, T> &)const");
            removeFunction(entry, "operator+=(const QMultiHash<Key, T> &)");
            removeFunction(entry, "operator+(const QMultiHash &)const");
            removeFunction(entry, "operator+=(const QMultiHash &)");
            removeFunction(entry, "operator=(const QHash &)");
            removeFunction(entry, "operator=(const QMultiHash &)");
            removeFunction(entry, "operator=(QHash &&)");
            removeFunction(entry, "operator=(QMultiHash &&)");
            protectedAccess(entry, "begin() const");
            protectedAccess(entry, "end() const");
            protectedAccess(entry, "find(const Key &)const");
            rename(entry, "values()const", "allValues");
            {
                FunctionModification mod;
                ArgumentModification am(0);
                am.modified_type = "void";
                mod.argument_mods << am;
                mod.signature = QMetaObject::normalizedSignature("intersect(const QHash<K, V>  &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("subtract(const QHash<K, V>  &)");
                entry->addFunctionModification(mod);
                mod.signature = QMetaObject::normalizedSignature("unite(const QHash<K, V>  &)");
                entry->addFunctionModification(mod);
            }
    //        removeFunction(entry, "begin() const");
    //        removeFunction(entry, "end() const");
            if(qtVersion >= QVersionNumber(6,0,0)){
                removeFunction(entry, "equal_range(const Key &)const");
                removeFunction(entry, "keys(T)const");
                rename(entry, "size()const", "longSize");
                rename(entry, "size()const", "longSize");
                addType(createListIterator(entry, true));
            }
            addType(entry);
        }
        addType(new ContainerTypeEntry("QQmlListProperty", ContainerTypeEntry::QQmlListPropertyContainer));
        ContainerTypeEntry* pair = new ContainerTypeEntry("QPair", ContainerTypeEntry::PairContainer);
        addType(pair);
        m_entries["std::pair"].append(pair);
        addType(new ContainerTypeEntry("std::atomic", ContainerTypeEntry::std_atomic));
        addType(new ContainerTypeEntry("std::optional", ContainerTypeEntry::std_optional));
        {
            ContainerTypeEntry* entry = new ContainerTypeEntry("QDBusReply", ContainerTypeEntry::QDBusReplyContainer);
            removeFunction(entry, "operator QDBusReply::Type() const");
            addType(entry);
        }

        addType(new InitializerListTypeEntry());
        addType(new PointerContainerTypeEntry("QPointer", PointerContainerTypeEntry::QPointer));
        addType(new PointerContainerTypeEntry("QSharedPointer", PointerContainerTypeEntry::QSharedPointer));
        addType(new PointerContainerTypeEntry("QWeakPointer", PointerContainerTypeEntry::QWeakPointer));
        addType(new PointerContainerTypeEntry("QScopedPointer", PointerContainerTypeEntry::QScopedPointer));
        addType(new PointerContainerTypeEntry("std::unique_ptr", PointerContainerTypeEntry::unique_ptr));
        addType(new PointerContainerTypeEntry("std::shared_ptr", PointerContainerTypeEntry::shared_ptr));
        addType(new PointerContainerTypeEntry("std::weak_ptr", PointerContainerTypeEntry::weak_ptr));

        // Custom types...
        addType(new QMetaObjectTypeEntry());
        {
            EnumTypeEntry* etype = new EnumTypeEntry("QMetaObject", "Call");
            etype->setTargetTypeSystem("QtCore");
            etype->setTargetLangPackage("io.qt.core");
            etype->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(etype);
        }
        addType(new QMetaObjectConnectionTypeEntry());
        addType(new GLsyncTypeEntry());
        {
            ContainerTypeEntry* cronoType = new ContainerTypeEntry("std::chrono::milliseconds", ContainerTypeEntry::std_chrono);
            cronoType->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(cronoType);
            cronoType = new ContainerTypeEntry("std::chrono::nanoseconds", ContainerTypeEntry::std_chrono);
            cronoType->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(cronoType);
            cronoType = new ContainerTypeEntry("std::chrono::seconds", ContainerTypeEntry::std_chrono);
            cronoType->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(cronoType);
            cronoType = new ContainerTypeEntry("std::chrono::duration", ContainerTypeEntry::std_chrono_template);
            cronoType->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(cronoType);
            cronoType = new ContainerTypeEntry("std::chrono::time_point", ContainerTypeEntry::std_chrono_template);
            cronoType->setCodeGeneration(TypeEntry::GenerateNothing);
            addType(cronoType);
        }
    }
    if(!parseFile(filename, importInputDirectoryList, typeystemDirectory, generate)){
        std::exit(-1);
    }
    if(stringListEntry){
        if(TypeSystemTypeEntry * typeSystemTypeEntry = findTypeSystem(stringListEntry->javaPackage()))
            stringListEntry->setCodeGeneration(typeSystemTypeEntry->codeGeneration());
    }
}

bool TypeDatabase::parseFile(const QString &filename, const QStringList &importInputDirectoryList, const QStringList &typeystemDirectoryList, bool generate, bool optional) {
    const QString &filepath = resolveFilePath(filename, 1, typeystemDirectoryList);
    qDebug() << "Resolving file: " << qPrintable(filename) << " => \"" << qPrintable(filepath) << "\"";
    if(optional && filepath.trimmed().isNull()) {
        qWarning() << "Optional file: " << qPrintable(filename) << ": could not be found";
        return true;  // we're still ok
    }
    if(m_loadedTypesystems.contains(filename))
        return true;
    m_loadedTypesystems.insert(filename);

    qsizetype count = m_entries.size();

    if(filename.endsWith(".xml")){
        XmlTypeSystemReader handler(this, generate, m_qtVersion);
        handler.setImportInputDirectoryList(importInputDirectoryList);
        handler.setTypesystemsDirectoryList(typeystemDirectoryList);
        handler.parse(filepath);
        //return false;
    }else{
        QmlTypeSystemReader handler(this, generate, m_qtVersion);
        handler.setImportInputDirectoryList(importInputDirectoryList);
        handler.setTypesystemsDirectoryList(typeystemDirectoryList);
        handler.parse(filepath);
    }

    qsizetype newCount = m_entries.size();
    QString string = QString::fromLatin1("File parsed: '%1', %2 new entries")
                     .arg(filepath)
                     .arg(newCount - count);
    qDebug() << qPrintable(string);
    //ReportHandler::debugSparse(string);
    return true;
}

bool TypeDatabase::useNativeIds() const
{
    return m_useNativeIds;
}

void TypeDatabase::setUseNativeIds(bool newUseNativeIds)
{
    m_useNativeIds = newUseNativeIds;
}

const QStringList &TypeDatabase::rebuildClasses() const
{
    return m_rebuildClasses;
}

TypeSystemTypeEntry *TypeDatabase::findTypeSystem(const QString &name) const{
    TypeEntry *entry = findType(name);
    if (entry && entry->isTypeSystem())
        return static_cast<TypeSystemTypeEntry *>(entry);
    else
        return nullptr;
}

ContainerTypeEntry *TypeDatabase::findContainerType(const QString &name) const {
    QString template_name = name;

    auto pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isContainer())
        return static_cast<ContainerTypeEntry *>(type_entry);
    return nullptr;
}

IteratorTypeEntry *TypeDatabase::findIteratorType(const QString &name) const {
    QString template_name = name;

    auto pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isIterator())
        return static_cast<IteratorTypeEntry *>(type_entry);
    return nullptr;
}

IteratorTypeEntry *TypeDatabase::findIteratorType(const ComplexTypeEntry * container) const {
    for(const QList<TypeEntry *>& list : m_entries){
        for(TypeEntry * entry : list){
            if(entry->isIterator()){
                IteratorTypeEntry * ientry = static_cast<IteratorTypeEntry *>(entry);
                if(ientry->containerType()==container){
                    return ientry;
                }
            }
        }
    }
    return nullptr;
}

PointerContainerTypeEntry *TypeDatabase::findPointerContainerType(const QString &name) const {
    QString template_name = name;

    auto pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isPointerContainer())
        return static_cast<PointerContainerTypeEntry *>(type_entry);
    return nullptr;
}

InitializerListTypeEntry *TypeDatabase::findInitializerListType(const QString &name) const {
    QString template_name = name;

    auto pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isInitializerList())
        return static_cast<InitializerListTypeEntry *>(type_entry);
    return nullptr;
}

PrimitiveTypeEntry *TypeDatabase::findTargetLangPrimitiveType(const QString &java_name) const {
    for(QList<TypeEntry *> entries : m_entries) {
        for(TypeEntry *e : entries) {
            if (e && e->isPrimitive()) {
                PrimitiveTypeEntry *pe = static_cast<PrimitiveTypeEntry *>(e);
                if (pe->targetLangName() == java_name && pe->preferredConversion())
                    return pe;
            }
        }
    }

    return nullptr;
}

IncludeList TypeDatabase::extraIncludes(const QString &className) {
    ComplexTypeEntry *typeEntry = findComplexType(className);
    if (typeEntry)
        return typeEntry->extraIncludes();
    else
        return IncludeList();
}

void TypeDatabase::addRejection(const QString &class_name, const QString &function_name,
                                const QString &field_name, const QString &enum_name) {
    TypeRejection r;
    r.class_name = class_name;
    r.function_name = function_name;
    r.field_name = field_name;
    r.enum_name = enum_name;

    m_rejections << r;
}

bool TypeDatabase::isClassRejected(const QString &class_name) {
    if (!m_rebuildClasses.isEmpty())
        return !m_rebuildClasses.contains(class_name);

    if(!m_rejectedClasses.contains(class_name)){
        m_rejectedClasses[class_name] = false;
        for(const TypeRejection &r : m_rejections){
            if(r.function_name.isEmpty() && r.field_name.isEmpty() && r.enum_name.isEmpty()){
                if(r.class_name.contains("*")){
                    QRegularExpression exp(QRegularExpression::wildcardToRegularExpression(r.class_name));
                    if(exp.match(class_name).hasMatch()){
                        m_rejectedClasses[class_name] = true;
                        break;
                    }
                }else if (r.class_name == class_name) {
                    m_rejectedClasses[class_name] = true;
                    break;
                }
            }
        }
    }
    return m_rejectedClasses[class_name];
}

bool TypeDatabase::isEnumRejected(const QString &class_name, const QString &enum_name) {
    if(!m_rejectedClasses.contains(class_name + "::" + enum_name)){
        m_rejectedClasses[class_name + "::" + enum_name] = false;
        for(const TypeRejection &r : m_rejections) {
            if(r.function_name.isEmpty() && r.field_name.isEmpty()) {
                if(r.class_name == class_name || r.class_name == "*"){
                    if(r.enum_name.contains("*")){
                        QRegularExpression exp(QRegularExpression::wildcardToRegularExpression(r.enum_name));
                        if(exp.match(enum_name).hasMatch()){
                            m_rejectedClasses[class_name + "::" + enum_name] = true;
                            break;
                        }
                    }else if (r.enum_name == enum_name) {
                        m_rejectedClasses[class_name + "::" + enum_name] = true;
                        break;
                    }
                }
            }
        }
    }
    return m_rejectedClasses[class_name + "::" + enum_name];
}

bool TypeDatabase::isFunctionRejected(const QString &class_name, const QString &function_name) {
    for(const TypeRejection &r : m_rejections){
        if(r.enum_name.isEmpty() && r.field_name.isEmpty()){
            if(r.class_name == class_name || r.class_name == "*"){
                if(r.function_name.contains("*")){
                    QRegularExpression exp(QRegularExpression::wildcardToRegularExpression(r.function_name));
                    if(exp.match(function_name).hasMatch()){
                        return true;
                    }
                }else if (r.function_name == function_name) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool TypeDatabase::isFieldRejected(const QString &class_name, const QString &field_name) {
    for(const TypeRejection &r : m_rejections){
        if(r.function_name.isEmpty() && r.enum_name.isEmpty()) {
            if(r.class_name == class_name || r.class_name == "*"){
                if(r.field_name.contains("*")){
                    QRegularExpression exp(QRegularExpression::wildcardToRegularExpression(r.field_name));
                    if(exp.match(field_name).hasMatch()){
                        return true;
                    }
                }else if (r.field_name == field_name) {
                    return true;
                }
            }
        }
    }
    return false;
}

FlagsTypeEntry *TypeDatabase::findFlagsType(const QString &name) const {
    FlagsTypeEntry *fte = reinterpret_cast<FlagsTypeEntry *>(findType(name));
    return fte ? fte : reinterpret_cast<FlagsTypeEntry *>(m_flags_entries.value(name));
}

void removeFunction(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.removal = TS::All;

    e->addFunctionModification(mod);
}

void protectedAccess(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.modifiers = Modification::Protected;
    e->addFunctionModification(mod);
}

void privateAccess(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.modifiers = Modification::Private;
    e->addFunctionModification(mod);
}

void injectCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args) {
    CodeSnip snip;
    snip.language = TS::NativeCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    snip.argumentMap = args;

    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.snips << snip;
    mod.modifiers = Modification::CodeInjection;
    e->addFunctionModification(mod);
}

#if 0
void injectTargetLangCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args) {
    CodeSnip snip;
    snip.language = TS::TargetLangCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    snip.argumentMap = args;

    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.snips << snip;
    mod.modifiers = Modification::CodeInjection;
    e->addFunctionModification(mod);
}

void injectCode(ComplexTypeEntry *e,
                       const QByteArray &code) {
    CodeSnip snip;
    snip.language = TS::NativeCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    e->addCodeSnip(snip);
}

void injectTargetLangCode(ComplexTypeEntry *e,
                       const QByteArray &code) {
    CodeSnip snip;
    snip.language = TS::TargetLangCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    e->addCodeSnip(snip);
}
#endif

void rename(ComplexTypeEntry *e,
                       const char *signature,
                       const char * newName) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.renamedToName = newName;
    mod.modifiers = Modification::Rename;
    e->addFunctionModification(mod);
}


IteratorTypeEntry* createListIterator(ContainerTypeEntry* entry, bool isMap, const QString &name){
    IteratorTypeEntry* iteratorEntry = new IteratorTypeEntry(name, entry);
    iteratorEntry->setTargetLangName(isMap ? "QAssociativeIterator" : "QSequentialIterator");
    iteratorEntry->setExtraIncludes(entry->extraIncludes());
    iteratorEntry->setInclude(entry->include());
    return iteratorEntry;
}

}

QString resolveFilePath(QString fileName, int opts, const QStringList &list) {
    if(!fileName.contains(".") && !fileName.contains("/")){
        QFileInfo fileinfoAbs(":/io/qtjambi/generator/typesystem/"+fileName+".qml");
        if(fileinfoAbs.exists()){
            return fileinfoAbs.absoluteFilePath();
        }
        fileName = fileName+".qml";
    }
    if(fileName.startsWith(":"))
        return fileName;
    QFileInfo fileinfoAbs(fileName);
    if(fileinfoAbs.exists()) {
        return fileinfoAbs.absoluteFilePath();
    }

    for(const QString &s : list) {
        QDir dir(s);
        if(!dir.exists()) {
            if(opts)
                qDebug() << "Absolute path: " << fileName << " in " << dir << ": No such directory";
            continue;
        }
        QFileInfo fileinfo(dir, fileName);
        if(fileinfo.isFile())
            return fileinfo.absoluteFilePath().trimmed();
        if(opts)
            qDebug() << "Absolute path: " << fileName << " in " << dir << ": No such file";
    }
    return QString();
}
