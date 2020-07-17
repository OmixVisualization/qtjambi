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

#include "typedatabase.h"
#include "../customtypes.h"

#include <QDebug>

#include <qfile.h>
#include <qfileinfo.h>
#include <qxml.h>
#include <QtCore>
#include "handler.h"
#include "../reporthandler.h"
#include "../main.h"

static void rename(ComplexTypeEntry *e,
                       const char *signature,
                       const char * newName);

static void removeFunction(ComplexTypeEntry *e, const char *signature);

static void protectedAccess(ComplexTypeEntry *e, const char *signature);

static void privateAccess(ComplexTypeEntry *e, const char *signature);

static void injectCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args);

static void injectTargetLangCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args);

static IteratorTypeEntry* createListIterator(ContainerTypeEntry* entry, bool isMap, const QString &name = QString("const_iterator"));

TypeDatabase::TypeDatabase() : m_suppressWarnings(true), m_includeEclipseWarnings(false) {
    addType(new StringTypeEntry("QString"));

    Q_UNUSED(&injectTargetLangCode)
    Q_UNUSED(&privateAccess)
    {
        StringTypeEntry *e = new StringTypeEntry("QLatin1String");
        e->setPreferredConversion(false);
        addType(e);
    }

    {
        StringTypeEntry *e = new StringTypeEntry("QStringView");
        e->setPreferredConversion(false);
        addType(e);
    }

    {
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
        qvariant->setCodeGeneration(TypeEntry::GenerateNothing);
        addType(qvariant);
        EnumTypeEntry* etype = new EnumTypeEntry("QVariant", "Type");
        etype->setTargetTypeSystem("QtCore");
        etype->setTargetLangPackage("io.qt.core");
        etype->setCodeGeneration(TypeEntry::GenerateNothing);
        addType(etype);
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
    addType(new ContainerTypeEntry("QArray", ContainerTypeEntry::QArrayContainer));
    addType(new ContainerTypeEntry("QVector2DArray", ContainerTypeEntry::QVector2DArrayContainer));
    addType(new ContainerTypeEntry("QVector3DArray", ContainerTypeEntry::QVector3DArrayContainer));
    addType(new ContainerTypeEntry("QVector4DArray", ContainerTypeEntry::QVector4DArrayContainer));
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
        addType(entry);
        IteratorTypeEntry* iteratorEntry = createListIterator(entry, false);
        addType(iteratorEntry);
        iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QQueue"));
        iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QStringList"));
        iteratorEntry->addExtraInclude(Include(Include::IncludePath, "QtCore/QByteArrayList"));
    }
    {
        ContainerTypeEntry* entry = new ContainerTypeEntry("QStringList", ContainerTypeEntry::StringListContainer);
        entry->setInclude(Include(Include::IncludePath, "QtCore/QStringList"));
        entry->addExtraInclude(Include(Include::IncludePath, "QtCore/QList"));
        addType(entry);
    }
/*    {
        ContainerTypeEntry* entry = new ContainerTypeEntry("QByteArrayList", ContainerTypeEntry::ByteArrayListContainer);
        entry->setInclude(Include(Include::IncludePath, "QtCore/QByteArrayList"));
        entry->addExtraInclude(Include(Include::IncludePath, "QtCore/QList"));
        addType(entry);
    }*/
    {
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
        protectedAccess(entry, "insert(int, const T &)");
        {
            FunctionModification mod;
            ArgumentModification am(0);
            am.modified_type = "void";
            mod.argument_mods << am;
            mod.signature = QMetaObject::normalizedSignature("fill(const T &, int)");
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
//        removeFunction(entry, "begin()");
//        removeFunction(entry, "end()");
//        protectedAccess(entry, "begin()");
//        protectedAccess(entry, "end()");
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
//        addType(createListIterator(entry, true));
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
//        addType(createListIterator(entry, true));
        addType(entry);
    }
    addType(new ContainerTypeEntry("QDeclarativeListProperty", ContainerTypeEntry::QDeclarativeListPropertyContainer));
    addType(new ContainerTypeEntry("QQmlListProperty", ContainerTypeEntry::QQmlListPropertyContainer));
    addType(new ContainerTypeEntry("QPair", ContainerTypeEntry::PairContainer));
    addType(new ContainerTypeEntry("std::atomic", ContainerTypeEntry::std_atomic));
    {
        ContainerTypeEntry* entry = new ContainerTypeEntry("QDBusReply", ContainerTypeEntry::QDBusReplyContainer);
        removeFunction(entry, "operator QDBusReply::Type() const");
        addType(entry);
    }

    addType(new InitializerListTypeEntry());
    addType(new PointerContainerTypeEntry("QSharedPointer", PointerContainerTypeEntry::QSharedPointer));
    addType(new PointerContainerTypeEntry("QWeakPointer", PointerContainerTypeEntry::QWeakPointer));
    addType(new PointerContainerTypeEntry("QScopedPointer", PointerContainerTypeEntry::QScopedPointer));
    addType(new PointerContainerTypeEntry("std::unique_ptr", PointerContainerTypeEntry::unique_ptr));
    addType(new PointerContainerTypeEntry("std::shared_ptr", PointerContainerTypeEntry::shared_ptr));
    addType(new PointerContainerTypeEntry("std::weak_ptr", PointerContainerTypeEntry::weak_ptr));

    // Custom types...
    addType(new QModelIndexTypeEntry());
    addType(new QMetaObjectTypeEntry());
    {
        EnumTypeEntry* etype = new EnumTypeEntry("QMetaObject", "Call");
        etype->setTargetTypeSystem("QtCore");
        etype->setTargetLangPackage("io.qt.core");
        etype->setCodeGeneration(TypeEntry::GenerateNothing);
        addType(etype);
    }
    addType(new QMetaObjectConnectionTypeEntry());
    addType(new QMetaMethodTypeEntry());
    addType(new QMetaPropertyTypeEntry());
    addType(new QMetaEnumTypeEntry());

    {
        ObjectTypeEntry* glsync = new ObjectTypeEntry("__GLsync");
        glsync->setTargetLangName("GLsync");
        glsync->setTargetLangPackage("io.qt.gui.gl");
        glsync->setCodeGeneration(TypeEntry::GenerateNothing);
        addType(glsync);
    }
}

TypeDatabase *TypeDatabase::instance() {
    static TypeDatabase *db = new TypeDatabase();
    return db;
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
        int pos = s.indexOf(QString(segs.at(i++)).replace("&place_holder_for_asterisk;", "*"));
        //qDebug() << "s == " << s << ", warning == " << segs;
        while (pos != -1) {
            if (i == segs.size())
                return true;
            pos = s.indexOf(QString(segs.at(i++)).replace("&place_holder_for_asterisk;", "*"), pos);
        }
    }

    return false;
}

PrimitiveTypeEntry *TypeDatabase::findPrimitiveType(const QString &name) {
    QList<TypeEntry *> entries = findTypes(name);

    for(TypeEntry *entry : entries) {
        if (entry && entry->isPrimitive() && static_cast<PrimitiveTypeEntry *>(entry)->preferredTargetLangType())
            return static_cast<PrimitiveTypeEntry *>(entry);
    }

    return nullptr;
}

ComplexTypeEntry *TypeDatabase::findComplexType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry && entry->isComplex())
        return static_cast<ComplexTypeEntry *>(entry);
    else
        return nullptr;
}

FunctionalTypeEntry *TypeDatabase::findFunctionalType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry && entry->isFunctional())
        return static_cast<FunctionalTypeEntry *>(entry);
    else
        return nullptr;
}

FunctionalTypeEntry *TypeDatabase::findFunctionalTypeByUsing(const QString &containingClassName, const QString &_using) {
    for(QList<TypeEntry *> entries : m_entries){
        for(TypeEntry *entry : entries){
            if (entry && entry->isFunctional())
            {
                FunctionalTypeEntry * fentry = static_cast<FunctionalTypeEntry *>(entry);
                if(!fentry->getUsing().isEmpty()){
                    if(( fentry->getUsing()==_using
                            || QString::fromUtf8(QMetaObject::normalizedSignature(qPrintable(fentry->getUsing())))==_using )
                            && fentry->name().startsWith(containingClassName+"::")){
                        return fentry;
                    }
                }
            }
        }
    }
    return nullptr;
}

ObjectTypeEntry *TypeDatabase::findObjectType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry && entry->isObject())
        return static_cast<ObjectTypeEntry *>(entry);
    else
        return nullptr;
}

NamespaceTypeEntry *TypeDatabase::findNamespaceType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry && entry->isNamespace())
        return static_cast<NamespaceTypeEntry *>(entry);
    else
        return nullptr;
}

bool TypeDatabase::parseFile(const QString &filename, const QStringList &importInputDirectoryList, bool generate, bool optional) {
    const QString &filepath = resolveFilePath(filename, 1, importInputDirectoryList);
    qDebug() << "Resolving file: " << filename << " => " << filepath;
    if(optional && filepath.isNull()) {
        qWarning() << "Optional file: " << filename << ": could not be found";
        return true;  // we're still ok
    }
    Q_ASSERT(!filepath.isNull());
    QFile file(filepath);
    Q_ASSERT(file.exists());
    qDebug() << "Parsing file: " << filename;
    QXmlInputSource source(&file);

    int count = m_entries.size();

    QXmlSimpleReader reader;
    Handler handler(this, generate);
    handler.setImportInputDirectoryList(importInputDirectoryList);

    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);

    bool ok = reader.parse(&source, false);
    int newCount = m_entries.size();

    QString string = QString::fromLatin1("Parsed: '%1', %2 new entries")
                     .arg(filename)
                     .arg(newCount - count);
    qDebug() << string;
    //ReportHandler::debugSparse(string);

    return ok;
}

ContainerTypeEntry *TypeDatabase::findContainerType(const QString &name) {
    QString template_name = name;

    int pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isContainer())
        return static_cast<ContainerTypeEntry *>(type_entry);
    return nullptr;
}

IteratorTypeEntry *TypeDatabase::findIteratorType(const QString &name) {
    QString template_name = name;

    int pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isIterator())
        return static_cast<IteratorTypeEntry *>(type_entry);
    return nullptr;
}

IteratorTypeEntry *TypeDatabase::findIteratorType(const ComplexTypeEntry * container) {
    for(const QList<TypeEntry *>& list : m_entries.values()){
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

PointerContainerTypeEntry *TypeDatabase::findPointerContainerType(const QString &name) {
    QString template_name = name;

    int pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isPointerContainer())
        return static_cast<PointerContainerTypeEntry *>(type_entry);
    return nullptr;
}

InitializerListTypeEntry *TypeDatabase::findInitializerListType(const QString &name) {
    QString template_name = name;

    int pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isInitializerList())
        return static_cast<InitializerListTypeEntry *>(type_entry);
    return nullptr;
}

PrimitiveTypeEntry *TypeDatabase::findTargetLangPrimitiveType(const QString &java_name) {
    for(QList<TypeEntry *> entries : m_entries.values()) {
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
    if (!m_rebuild_classes.isEmpty())
        return !m_rebuild_classes.contains(class_name);

    for(const TypeRejection &r : m_rejections){
        if(r.function_name == "*" && r.field_name == "*" && r.enum_name == "*"){
            if(r.class_name.contains("*")){
                QRegExp exp(r.class_name, Qt::CaseSensitive, QRegExp::Wildcard);
                if(exp.exactMatch(class_name)){
                    return true;
                }
            }else if (r.class_name == class_name) {
                return true;
            }
        }
    }
    return false;
}

bool TypeDatabase::isEnumRejected(const QString &class_name, const QString &enum_name) {
    for(const TypeRejection &r : m_rejections) {
        if(r.function_name == "*" && r.field_name == "*") {
            if (r.enum_name == enum_name
                    && (r.class_name == class_name || r.class_name == "*")) {
                return true;
            }
        }
    }

    return false;
}

bool TypeDatabase::isFunctionRejected(const QString &class_name, const QString &function_name) {
    for(const TypeRejection &r : m_rejections){
        if(r.enum_name == "*" && r.field_name == "*"){
            if (r.function_name == function_name &&
                (r.class_name == class_name || r.class_name == "*"))
                return true;
        }
    }
    return false;
}


bool TypeDatabase::isFieldRejected(const QString &class_name, const QString &field_name) {
    for(const TypeRejection &r : m_rejections){
        if(r.function_name == "*" && r.enum_name == "*") {
            if (r.field_name == field_name &&
                    (r.class_name == class_name || r.class_name == "*"))
                return true;
        }
    }
    return false;
}

FlagsTypeEntry *TypeDatabase::findFlagsType(const QString &name) const {
    FlagsTypeEntry *fte = reinterpret_cast<FlagsTypeEntry *>(findType(name));
    return fte ? fte : reinterpret_cast<FlagsTypeEntry *>(m_flags_entries.value(name));
}

QString TypeDatabase::globalNamespaceClassName() {
    return QLatin1String("package_global");
}

static void removeFunction(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.removal = TypeSystem::All;

    e->addFunctionModification(mod);
}

static void protectedAccess(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.modifiers = Modification::Protected;
    e->addFunctionModification(mod);
}

static void privateAccess(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.modifiers = Modification::Private;
    e->addFunctionModification(mod);
}

static void injectCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args) {
    CodeSnip snip;
    snip.language = TypeSystem::NativeCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    snip.argumentMap = args;

    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.snips << snip;
    mod.modifiers = Modification::CodeInjection;
    e->addFunctionModification(mod);
}

static void injectTargetLangCode(ComplexTypeEntry *e,
                       const char *signature,
                       const QByteArray &code,
                       const ArgumentMap &args) {
    CodeSnip snip;
    snip.language = TypeSystem::TargetLangCode;
    snip.position = CodeSnip::Beginning;
    snip.addCode(QString::fromLatin1(code));
    snip.argumentMap = args;

    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.snips << snip;
    mod.modifiers = Modification::CodeInjection;
    e->addFunctionModification(mod);
}

static void rename(ComplexTypeEntry *e,
                       const char *signature,
                       const char * newName) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.renamedToName = newName;
    mod.modifiers = Modification::Rename;
    e->addFunctionModification(mod);
}


static IteratorTypeEntry* createListIterator(ContainerTypeEntry* entry, bool isMap, const QString &name){
    IteratorTypeEntry* iteratorEntry = new IteratorTypeEntry(name, entry);
    iteratorEntry->setTargetLangName(isMap ? "QMapIterator" : "QIterator");
    iteratorEntry->setExtraIncludes(entry->extraIncludes());
    iteratorEntry->setInclude(entry->include());
    return iteratorEntry;
}

