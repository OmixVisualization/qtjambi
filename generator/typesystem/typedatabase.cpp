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
#include "handler.h"
#include "../reporthandler.h"
#include "../main.h"

static void addRemoveFunctionToTemplates(TypeDatabase *db);

TypeDatabase::TypeDatabase() : m_suppressWarnings(true), m_includeEclipseWarnings(false) {
    addType(new StringTypeEntry("QString"));

    StringTypeEntry *e = new StringTypeEntry("QLatin1String");
    e->setPreferredConversion(false);
    addType(e);

    // We need the generator to perform type conversion in C++ with the
    //  construct:
    // QString qstring = QString("string"); QStringRef(&qstring)"
    //  not with:
    // (const QStringRef &)QString("string")
    StringRefTypeEntry *sr = new StringRefTypeEntry("QStringRef");
    sr->setPreferredConversion(false);
    addType(sr);
    // TODO: Use of StringRefTypeEntry for QXmlStreamStringRef has not been tested,
    //  I am sure the previous code would cause a crash.
    sr = new StringRefTypeEntry("QXmlStreamStringRef");
    sr->setPreferredConversion(false);
    addType(sr);

    addType(new CharTypeEntry("QChar"));

    CharTypeEntry *c = new CharTypeEntry("QLatin1Char");
    c->setPreferredConversion(false);
    addType(c);

    {
        VariantTypeEntry *qvariant = new VariantTypeEntry("QVariant");
        qvariant->setCodeGeneration(TypeEntry::GenerateNothing);
        addType(qvariant);
    }

    {
        JObjectWrapperTypeEntry *wrapper = new JObjectWrapperTypeEntry("JObjectWrapper");
        wrapper->setCodeGeneration(TypeEntry::GenerateNothing);
        addType(wrapper);
    }

    addType(new ThreadTypeEntry());
    addType(new VoidTypeEntry());

    // Predefined containers...
    addType(new ContainerTypeEntry("QArray", ContainerTypeEntry::QArrayContainer));
    addType(new ContainerTypeEntry("QVector2DArray", ContainerTypeEntry::QVector2DArrayContainer));
    addType(new ContainerTypeEntry("QVector3DArray", ContainerTypeEntry::QVector3DArrayContainer));
    addType(new ContainerTypeEntry("QVector4DArray", ContainerTypeEntry::QVector4DArrayContainer));
    addType(new ContainerTypeEntry("QList", ContainerTypeEntry::ListContainer));
    addType(new PointerContainerTypeEntry("QSharedPointer", PointerContainerTypeEntry::QSharedPointer));
    addType(new PointerContainerTypeEntry("QWeakPointer", PointerContainerTypeEntry::QWeakPointer));
    addType(new ContainerTypeEntry("QStringList", ContainerTypeEntry::StringListContainer));
    addType(new ContainerTypeEntry("QByteArrayList", ContainerTypeEntry::ByteArrayListContainer));
    addType(new ContainerTypeEntry("QLinkedList", ContainerTypeEntry::LinkedListContainer));
    addType(new ContainerTypeEntry("QVector", ContainerTypeEntry::VectorContainer));
    addType(new ContainerTypeEntry("QStack", ContainerTypeEntry::StackContainer));
    addType(new ContainerTypeEntry("QSet", ContainerTypeEntry::SetContainer));
    addType(new ContainerTypeEntry("QMap", ContainerTypeEntry::MapContainer));
    addType(new ContainerTypeEntry("QHash", ContainerTypeEntry::HashContainer));
    addType(new ContainerTypeEntry("QDeclarativeListProperty", ContainerTypeEntry::QDeclarativeListPropertyContainer));
    addType(new ContainerTypeEntry("QQmlListProperty", ContainerTypeEntry::QQmlListPropertyContainer));
    addType(new ContainerTypeEntry("QPair", ContainerTypeEntry::PairContainer));
    addType(new ContainerTypeEntry("QQueue", ContainerTypeEntry::QueueContainer));
    addType(new ContainerTypeEntry("QMultiMap", ContainerTypeEntry::MultiMapContainer));

    // Custom types...
    addType(new QModelIndexTypeEntry());

    addRemoveFunctionToTemplates(this);
}

TypeDatabase *TypeDatabase::instance() {
    static TypeDatabase *db = new TypeDatabase();
    return db;
}

TypeEntry *TypeDatabase::findType(const QString &name) const {
    QList<TypeEntry *> entries = findTypes(name);
    foreach(TypeEntry *entry, entries) {
        //qDebug()<<"findType()"<<entry;
        if (entry != 0 &&
                (!entry->isPrimitive() ||
                 static_cast<PrimitiveTypeEntry *>(entry)->preferredTargetLangType())) {
            return entry;
        }
    }
    return 0;
}

SingleTypeEntryHash TypeDatabase::entries() {
    TypeEntryHash entries = allEntries();

    SingleTypeEntryHash returned;
    QList<QString> keys = entries.keys();

    foreach(QString key, keys) {
        returned[key] = findType(key);
    }

    return returned;
}

bool TypeDatabase::isSuppressedWarning(const QString &s) {
    if (!m_suppressWarnings)
        return false;

    foreach(const QString &_warning, m_suppressedWarnings) {
        QString warning(QString(_warning).replace("\\*", "&place_holder_for_asterisk;"));

        QStringList segs = warning.split("*", QString::SkipEmptyParts);
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

    foreach(TypeEntry *entry, entries) {
        if (entry != 0 && entry->isPrimitive() && static_cast<PrimitiveTypeEntry *>(entry)->preferredTargetLangType())
            return static_cast<PrimitiveTypeEntry *>(entry);
    }

    return 0;
}

ComplexTypeEntry *TypeDatabase::findComplexType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry != 0 && entry->isComplex())
        return static_cast<ComplexTypeEntry *>(entry);
    else
        return 0;
}

ObjectTypeEntry *TypeDatabase::findObjectType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry != 0 && entry->isObject())
        return static_cast<ObjectTypeEntry *>(entry);
    else
        return 0;
}

NamespaceTypeEntry *TypeDatabase::findNamespaceType(const QString &name) {
    TypeEntry *entry = findType(name);
    if (entry != 0 && entry->isNamespace())
        return static_cast<NamespaceTypeEntry *>(entry);
    else
        return 0;
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
    return 0;
}

PointerContainerTypeEntry *TypeDatabase::findPointerContainerType(const QString &name) {
    QString template_name = name;

    int pos = name.indexOf('<');
    if (pos > 0)
        template_name = name.left(pos);

    TypeEntry *type_entry = findType(template_name);
    if (type_entry && type_entry->isPointerContainer())
        return static_cast<PointerContainerTypeEntry *>(type_entry);
    return 0;
}

PrimitiveTypeEntry *TypeDatabase::findTargetLangPrimitiveType(const QString &java_name) {
    foreach(QList<TypeEntry *> entries, m_entries.values()) {
        foreach(TypeEntry *e, entries) {
            if (e && e->isPrimitive()) {
                PrimitiveTypeEntry *pe = static_cast<PrimitiveTypeEntry *>(e);
                if (pe->targetLangName() == java_name && pe->preferredConversion())
                    return pe;
            }
        }
    }

    return 0;
}

IncludeList TypeDatabase::extraIncludes(const QString &className) {
    ComplexTypeEntry *typeEntry = findComplexType(className);
    if (typeEntry != 0)
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

    foreach(const TypeRejection &r, m_rejections)
    if (r.class_name == class_name && r.function_name == "*" && r.field_name == "*" && r.enum_name == "*") {
        return true;
    }
    return false;
}

bool TypeDatabase::isEnumRejected(const QString &class_name, const QString &enum_name) {
    foreach(const TypeRejection &r, m_rejections) {
        if (r.enum_name == enum_name
                && (r.class_name == class_name || r.class_name == "*")) {
            return true;
        }
    }

    return false;
}

bool TypeDatabase::isFunctionRejected(const QString &class_name, const QString &function_name) {
    foreach(const TypeRejection &r, m_rejections)
    if (r.function_name == function_name &&
            (r.class_name == class_name || r.class_name == "*"))
        return true;
    return false;
}


bool TypeDatabase::isFieldRejected(const QString &class_name, const QString &field_name) {
    foreach(const TypeRejection &r, m_rejections)
    if (r.field_name == field_name &&
            (r.class_name == class_name || r.class_name == "*"))
        return true;
    return false;
}

FlagsTypeEntry *TypeDatabase::findFlagsType(const QString &name) const {
    FlagsTypeEntry *fte = (FlagsTypeEntry *) findType(name);
    return fte ? fte : (FlagsTypeEntry *) m_flags_entries.value(name);
}

QString TypeDatabase::globalNamespaceClassName(const TypeEntry * /*entry*/) {
    return QLatin1String("Global");
}

static void removeFunction(ComplexTypeEntry *e, const char *signature) {
    FunctionModification mod;
    mod.signature = QMetaObject::normalizedSignature(signature);
    mod.removal = TypeSystem::All;

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


static void addRemoveFunctionToTemplates(TypeDatabase *db) {
    ContainerTypeEntry *qvector = db->findContainerType(QLatin1String("QVector"));
    removeFunction(qvector, "constData() const");
    removeFunction(qvector, "data() const");
    removeFunction(qvector, "data()");
    removeFunction(qvector, "first()");
    removeFunction(qvector, "last()");
    removeFunction(qvector, "operator[](int)");
    removeFunction(qvector, "operator[](int) const");
    removeFunction(qvector, "operator=(QVector<T>)");

    ContainerTypeEntry *qlist = db->findContainerType(QLatin1String("QList"));
    removeFunction(qlist, "constData() const");
    removeFunction(qlist, "data() const");
    removeFunction(qlist, "data()");
    removeFunction(qlist, "back()");
    removeFunction(qlist, "front()");
    removeFunction(qlist, "first()");
    removeFunction(qlist, "last()");
    removeFunction(qlist, "operator[](int)");
    removeFunction(qlist, "operator[](int) const");
    removeFunction(qlist, "operator=(QList<T>)");

    ContainerTypeEntry *qqueue = db->findContainerType(QLatin1String("QQueue"));
    removeFunction(qqueue, "head() const");

    ArgumentMap args1;
    args1[1] = QLatin1String("$1");
    ArgumentMap args2 = args1;
    args2[2] = QLatin1String("$2");

    QByteArray code =
        "\nif ($1 >= __qt_this->size() || $1 < 0) {"
        "\n   __jni_env->ThrowNew(__jni_env->FindClass(\"java/lang/IndexOutOfBoundsException\"),"
        "\n                       QString::fromLatin1(\"Accessing container of size %3 at %4\")"
        "\n                       .arg(__qt_this->size()).arg($1).toLatin1());"
        "\n   return;"
        "\n}";

    QByteArray code_with_return = QByteArray(code).replace("return;", "return 0;");

    QByteArray code_index_length =
        "\nif ($1 < 0 || $2 < 0 || ($1 + $2) >= __qt_this->size()) {"
        "\n   __jni_env->ThrowNew(__jni_env->FindClass(\"java/lang/IndexOutOfBoundsException\"),"
        "\n                       QString::fromLatin1(\"Accessing container of size %3 from %4 to %5\")"
        "\n                       .arg(__qt_this->size()).arg($1).arg($1+$2).toLatin1());"
        "\n   return;"
        "\n}";

    QByteArray code_non_empty =
        "\nif (__qt_this->isEmpty()) {"
        "\n   __jni_env->ThrowNew(__jni_env->FindClass(\"java/lang/IndexOutOfBoundsException\"),"
        "\n                       QString::fromLatin1(\"Accessing empty container...\").toLatin1());"
        "\n   return;"
        "\n}";

    QByteArray code_two_indices =
        "\nif ($1 < 0 || $2 < 0 || $1 >= __qt_this->size() || $2 >= __qt_this->size()) {"
        "\n   __jni_env->ThrowNew(__jni_env->FindClass(\"java/lang/IndexOutOfBoundsException\"),"
        "\n                       QString::fromLatin1(\"Accessing container of size %3 at %4 and at %5\")"
        "\n                       .arg(__qt_this->size()).arg($1).arg($2).toLatin1());"
        "\n   return;"
        "\n}";

    { // QVector safty...
        injectCode(qvector, "at(int) const", code_with_return, args1);
        injectCode(qvector, "replace(int,T)", code, args1);
        injectCode(qvector, "remove(int)", code, args1);
        injectCode(qvector, "remove(int, int)", code_index_length, args2);
        injectCode(qvector, "pop_back()", code_non_empty, ArgumentMap());
        injectCode(qvector, "pop_front()", code_non_empty, ArgumentMap());
    }

    { // QList safty...
        injectCode(qlist, "at(int) const", code_with_return, args1);
        injectCode(qlist, "replace(int, T)", code, args1);
        injectCode(qlist, "pop_back()", code_non_empty, ArgumentMap());
        injectCode(qlist, "pop_front()", code_non_empty, ArgumentMap());
        injectCode(qlist, "swap(int, int)", code_two_indices, args2);
        injectCode(qlist, "move(int, int)", code_two_indices, args2);
        injectCode(qlist, "removeAt(int)", code, args1);
        injectCode(qlist, "takeAt(int)", code_with_return, args1);
    }

}

