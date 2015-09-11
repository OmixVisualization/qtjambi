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

#include "metainfogenerator.h"
#include "reporthandler.h"
#include "cppimplgenerator.h"
#include "fileout.h"

#include <QDir>
#include <QMetaType>
#include "typesystem/typedatabase.h"

MetaInfoGenerator::MetaInfoGenerator(PriGenerator *pri):
        JavaGenerator(),
        priGenerator(pri)

{
    setFilenameStub("metainfo");
}

QString MetaInfoGenerator::subDirectoryForPackage(const QString &package, OutputDirectoryType type) const {
    switch (type) {
        case CppDirectory:
            return QString(package).replace(".", "_") + "/";
        case JavaDirectory:
            return QString(package).replace(".", "/");
        default:
            return QString(); // kill nonsense warnings
    }
}

QString MetaInfoGenerator::subDirectoryForClass(const AbstractMetaClass *cls, OutputDirectoryType type) const {
    Q_ASSERT(cls);
    return subDirectoryForPackage(cls->package(), type);
}

void MetaInfoGenerator::generate() {
    buildSkipList();
    writeCppFile();
    writeHeaderFile();
    writeLibraryInitializers();
}

bool MetaInfoGenerator::shouldGenerate(const TypeEntry *entry) const {
    return entry != 0 && !entry->isNamespace() && !entry->isEnum() && (entry->codeGeneration() & TypeEntry::GenerateCpp);
}

bool MetaInfoGenerator::shouldGenerate(const AbstractMetaClass *cls) const {
    return (!cls->isInterface() && cls->typeEntry()->isValue() && !cls->isNamespace()
            && !cls->isAbstract() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp));
}

QString MetaInfoGenerator::fileNameForClass(const AbstractMetaClass *) const {
    return filenameStub() + ".cpp";
}

void MetaInfoGenerator::write(QTextStream &, const AbstractMetaClass *, int) {
    // not used
}

bool MetaInfoGenerator::generated(const AbstractMetaClass *cls) const {
    return generatedMetaInfo(cls->package());
}

bool MetaInfoGenerator::generatedMetaInfo(const QString &package) const {
    return (m_skip_list.value(package, 0x0) & GeneratedMetaInfo);
}

bool MetaInfoGenerator::generatedJavaClasses(const QString &package) const {
    return (m_skip_list.value(package, 0x0) & GeneratedJavaClasses);
}

static void metainfo_write_name_list(QTextStream &s, const char *var_name, const QList<QString> &strs,
                                     int offset, int skip) {
    s << "static const char *" << var_name << "[] = {" << endl;
    for (int i = offset; i < strs.size(); i += skip) {
        s << "    \"" << strs.at(i).toLatin1() << "\"";
        if (i < strs.size() - 1)
            s << ",";
        s << endl;
    }
    s << "};" << endl << endl;
}

void MetaInfoGenerator::writeEnums(QTextStream &s, const QString &package) {
    TypeEntryHash entries = TypeDatabase::instance()->allEntries();
    TypeEntryHash::iterator it;

    QList<QString> strs;
    for (it = entries.begin(); it != entries.end(); ++it) {
        QList<TypeEntry *> entries = it.value();
        foreach(TypeEntry *entry, entries) {
            if ((entry->isFlags() || entry->isEnum()) && entry->javaPackage() == package) {
                EnumTypeEntry *eentry = entry->isEnum() ? static_cast<EnumTypeEntry *>(entry) : static_cast<FlagsTypeEntry *>(entry)->originator();

                // The Qt flags names should map to the enum names, this is
                // required for the designer plugin to find the enum type of
                // a flags type since this functionality is not available in
                // Qt. This may be a little bit inconsistent, but it saves
                // us making yet another hash table for lookups. If it causes
                // problems, make a new one for this particular purpose.
                strs.append((eentry->javaPackage().isEmpty() ? QString() : eentry->javaPackage().replace('.', '/')  + "/")
                            + eentry->javaQualifier() + "$" + eentry->targetLangName());
                strs.append(entry->isFlags() ? static_cast<FlagsTypeEntry *>(entry)->originalName() : entry->qualifiedCppName());
            }
        }
    }

    Q_ASSERT(strs.size() % 2 == 0);

    s << "static int enum_count = " << (strs.size() / 2) << ";" << endl;
    if (strs.size() > 0) {
        metainfo_write_name_list(s, "enumJavaNames", strs, 0, 2);
        metainfo_write_name_list(s, "enumCppNames", strs, 1, 2);
    } else {
        s << "static const char **enumCppNames = 0;" << endl
        << "static const char **enumJavaNames = 0;" << endl;
    }
}

void MetaInfoGenerator::writeSignalsAndSlots(QTextStream &s, const QString &package) {
    AbstractMetaClassList classes = this->classes();

    QList<QString> strs;
    foreach(AbstractMetaClass *cls, classes) {
        if (cls->package() == package) {
            AbstractMetaFunctionList functions = cls->functions();
            foreach(AbstractMetaFunction *f, functions) {
                if (f->implementingClass() == cls && (f->isSignal() || f->isSlot())) {

                    AbstractMetaArgumentList arguments = f->arguments();
                    int numOverloads = arguments.size();
                    for (int i = arguments.size() - 1; i >= 0; --i) {
                        if (arguments.at(i)->defaultValueExpression().isEmpty()) {
                            numOverloads = arguments.size() - i - 1;
                            break;
                        }
                    }

                    for (int i = 0; i <= numOverloads; ++i) {
                        Option option = Option(SkipAttributes | SkipReturnType | SkipName);
                        QString qtName;
                        {

                            QTextStream qtNameStream(&qtName);
                            CppGenerator::writeFunctionSignature(qtNameStream, f, 0, QString(),
                                                                 Option(option | OriginalName | NormalizeAndFixTypeSignature | OriginalTypeDescription),
                                                                 QString(), QStringList(), arguments.size() - i);
                        }
                        qtName = f->implementingClass()->qualifiedCppName() + "::" + qtName;
                        qtName = QMetaObject::normalizedSignature(qtName.toLatin1().constData());

                        QString javaFunctionName = functionSignature(f, 0, 0, option, arguments.size() - (f->isSignal() ? 0 : i));
                        QString javaObjectName = f->isSignal()
                                                 ? f->name()
                                                 : javaFunctionName;

                        javaFunctionName = f->implementingClass()->fullName() + "." + javaFunctionName;
                        javaObjectName   = f->implementingClass()->fullName() + "." + javaObjectName;

                        QString javaSignature = "(";
                        for (int j = 0; j < (arguments.size() - (f->isSignal() ? 0 : i)); ++j)  {
                            AbstractMetaArgument *arg = arguments.at(j);
                            javaSignature += jni_signature(arg->type(), SlashesAndStuff);
                        }
                        javaSignature += ")" + jni_signature(f->type(), SlashesAndStuff);

                        strs.append(qtName);
                        strs.append(javaFunctionName);
                        strs.append(javaObjectName);
                        strs.append(javaSignature);
                    }
                }
            }
        }
    }

    Q_ASSERT(strs.size() % 4 == 0);

    s << "static int sns_count = " << (strs.size() / 4) << ";" << endl;
    if (strs.size() > 0) {
        metainfo_write_name_list(s, "qtNames", strs, 0, 4);
        metainfo_write_name_list(s, "javaFunctionNames", strs, 1, 4);
        metainfo_write_name_list(s, "javaObjectNames", strs, 2, 4);
        metainfo_write_name_list(s, "javaSignatures", strs, 3, 4);
    } else {
        s << "static const char **qtNames = 0;" << endl
        << "static const char **javaFunctionNames = 0;" << endl
        << "static const char **javaObjectNames = 0;" << endl
        << "static const char **javaSignatures = 0;" << endl;
    }
}

void MetaInfoGenerator::writeRegisterSignalsAndSlots(QTextStream &s) {
    s << "    for (int i=0;i<sns_count; ++i) {" << endl
    << "        registerQtToJava(qtNames[i], javaFunctionNames[i]);" << endl
    << "        if (getQtName(javaObjectNames[i]).length() < QByteArray(qtNames[i]).size())" << endl
    << "            registerJavaToQt(javaObjectNames[i], qtNames[i]);" << endl
    << "        registerJavaSignature(qtNames[i], javaSignatures[i]);" << endl
    << "    }" << endl;
}

void MetaInfoGenerator::writeRegisterEnums(QTextStream &s) {
    s << "    for (int i=0;i<enum_count; ++i) {" << endl
    << "        registerQtToJava(enumCppNames[i], enumJavaNames[i]);" << endl
    << "        registerJavaToQt(enumJavaNames[i], enumCppNames[i]);" << endl
    << "    }" << endl;
}

/**
 * Builds a skip list of classes that shouldn't be built.
 */
void MetaInfoGenerator::buildSkipList() {
    AbstractMetaClassList classList = classes();
    foreach(AbstractMetaClass *cls, classList) {
        if (!m_skip_list.contains(cls->package()))
            m_skip_list[cls->package()] = 0x0;

        if (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
            m_skip_list[cls->package()] |= GeneratedMetaInfo;

        if (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)
            m_skip_list[cls->package()] |= GeneratedJavaClasses;
    }
}

QStringList MetaInfoGenerator::writePolymorphicHandler(QTextStream &s, const QString &package,
        const AbstractMetaClassList &classes) {
    QStringList handlers;
    foreach(AbstractMetaClass *cls, classes) {
        const ComplexTypeEntry *centry = cls->typeEntry();
        if (!centry->isPolymorphicBase())
            continue;

        AbstractMetaClassList classList = this->classes();
        bool first = true;
        foreach(AbstractMetaClass *clazz, classList) {
            if (clazz->package() == package && clazz->inheritsFrom(cls)) {
                if (!clazz->typeEntry()->polymorphicIdValue().isEmpty()) {
                    // On first find, open the function
                    if (first) {
                        first = false;

                        QString handler = jni_signature(cls->fullName(), Underscores);
                        handlers.append(handler);

                        s << "static bool polymorphichandler_" << handler
                        << "(const void *ptr, const char **class_name, const char **package)" << endl
                        << "{" << endl
                        << "    Q_ASSERT(ptr != 0);" << endl
                        << "    " << cls->qualifiedCppName() << " *object = ("
                        << cls->qualifiedCppName() << " *)ptr;" << endl;
                    }

                    // For each, add case label
                    s << "    if ("
                    << clazz->typeEntry()->polymorphicIdValue().replace("%1", "object")
                    << ") {" << endl
                    << "        *class_name = \"" << clazz->name() << "\";" << endl
                    << "        *package    = \"" << clazz->package().replace(".", "/") << "/\";" << endl
                    << "        return true;" << endl
                    << "    }" << endl;
                } else {
                    QString warning = QString("class '%1' inherits from polymorphic class '%2', but has no polymorphic id set")
                                      .arg(clazz->name())
                                      .arg(cls->name());

                    ReportHandler::warning(warning);
                }
            }
        }

        // Close the function if it has been opened
        if (!first) {
            s << "    return false;" << endl
            << "}" << endl;
        }
    }

    return handlers;
}

// This is only needed when qtJambiDebugTools() is set
void MetaInfoGenerator::writeNameLiteral(QTextStream &s, const TypeEntry *entry, const QString &fileName) {
    static QSet<QString> used;

    const QString key (fileName + ":" + entry->name());
    const bool exists = used.contains(key);
    if (exists)		/* If it already exists comment out the repeats */
        s << "/* ";
    if (!exists) {
        // This can not be made 'static' or 'const' due to compile failure
        s << "char __name_" << QString(entry->name()).replace(':', '_').replace(' ', '_') << "[] = \"" << entry->name() << "\";";
        used.insert(key);
    }
    if (exists)
        s << " */";
    s << endl;
}

void MetaInfoGenerator::writeCppFile() {
    TypeEntryHash entries = TypeDatabase::instance()->allEntries();
    TypeEntryHash::iterator it;

    AbstractMetaClassList classes_with_polymorphic_id;
    AbstractMetaClassList classList = classes();
    QHash<QString, FileOut *> fileHash;

    // Seems continue is not supported by our foreach loop, so
    foreach(AbstractMetaClass *cls, classList) {

        FileOut *f = fileHash.value(cls->package(), 0);
        if (f == 0 && generated(cls)) {
            f = new FileOut(cppOutputDirectory() + "/" + subDirectoryForClass(cls, CppDirectory) + "/" + cppFilename());

            writeIncludeStatements(f->stream, classList, cls->package());
            f->stream << endl;

            // Write the generic destructors and constructors
            f->stream
            << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl
            << "template <typename T, const char *NAME>" << endl
            << "#else /* QTJAMBI_DEBUG_TOOLS */" << endl
            << "template <typename T>" << endl
            << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl
            << "static void qtjambiGenericDeleter(T *t)" << endl
            << "{" << endl
            << "    delete t;" << endl
            << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl
            << "    qtjambi_increase_destructorFunctionCalledCount(QString::fromLatin1(NAME));" << endl
            << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl
            << "}" << endl
            << endl
#if QT_VERSION >= 0x050000
            << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl
            << "template <typename T, const char *NAME>" << endl
            << "#else /* QTJAMBI_DEBUG_TOOLS */" << endl
            << "template <typename T>" << endl
            << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl
            << "static void qtjambiGenericDestructor(void *t)" << endl
            << "{" << endl
            << "    static_cast<T*>(t)->~T();" << endl
            << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl
            << "    qtjambi_increase_destructorFunctionCalledCount(QString::fromLatin1(NAME));" << endl
            << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl
            << "}" << endl
            << endl
#endif
            << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl
#if QT_VERSION >= 0x050000
            << "template <typename T>" << endl
            << "static void *qtjambiGenericConstructor(void *where, const void *t)" << endl
            << "{" << endl
            << "    if (t)" << endl
            << "        return new (where) T(*static_cast<const T*>(t));" << endl
            << "    return new (where) T;" << endl
            << "}" << endl
            << endl
#endif
            << "template <typename T>" << endl
            << "static void *qtjambiGenericCreator(const T *t)" << endl
            << "{" << endl
            << "    if (t)" << endl
            << "        return new T(*static_cast<const T *>(t));" << endl
            << "    return new T();" << endl
            << "}" << endl
            << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl
            << endl;

            fileHash.insert(cls->package(), f);

            QString pro_file_name = cls->package().replace(".", "_") + "/" + cls->package().replace(".", "_") + ".pri";
            priGenerator->addSource(pro_file_name, cppFilename());
        }

        if (!(cls->attributes() & AbstractMetaAttributes::Fake)) {
            if (f != 0) {
                if (cls->typeEntry()->isObject()
                        && !cls->typeEntry()->isQObject()
                        && !cls->isInterface()) {
                    writeDestructors(f->stream, cls);
                }
                writeCustomStructors(f->stream, cls->typeEntry());
            }

            if (cls->typeEntry()->isPolymorphicBase())
                classes_with_polymorphic_id.append(cls);
        }

//        if (qtJambiDebugTools()) {
            if (cls->typeEntry()->isValue() && shouldGenerate(cls->typeEntry())) {
                f->stream << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl;
                writeNameLiteral(f->stream, cls->typeEntry(), f->name());
                f->stream << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
            }
//        }
    }

    QHash<QString, QStringList> handlers_to_register;
    foreach(QString package, fileHash.keys()) {
        FileOut *f = fileHash.value(package, 0);
        if (f != 0) {
            writeSignalsAndSlots(f->stream, package);
            writeEnums(f->stream, package);
            handlers_to_register[package] = writePolymorphicHandler(f->stream, package, classes_with_polymorphic_id);
        }
    }

    TypeEntry * declarativeListPropertyTypeEntry = Q_NULLPTR;
    TypeEntry * qmlListPropertyTypeEntry = Q_NULLPTR;

    // Primitive types must be added to all packages, in case the other packages are
    // not referenced from the generated code.
    foreach(FileOut *f, fileHash.values()) {
        for (it = entries.begin(); it != entries.end(); ++it) {
            QList<TypeEntry *> entries = it.value();
            foreach(TypeEntry *entry, entries) {
                if(entry->isContainer()){
                    ContainerTypeEntry* containerTypeEntry = dynamic_cast<ContainerTypeEntry*>(entry);
                    if(containerTypeEntry->type()==ContainerTypeEntry::QQmlListPropertyContainer){
                        qmlListPropertyTypeEntry = containerTypeEntry;
                    }else if(containerTypeEntry->type()==ContainerTypeEntry::QDeclarativeListPropertyContainer){
                        declarativeListPropertyTypeEntry = containerTypeEntry;
                    }
                }
                if (shouldGenerate(entry) && entry->isPrimitive()) {
                    writeCustomStructors(f->stream, entry);
//                    if (qtJambiDebugTools()) {
                        f->stream << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl;
                        writeNameLiteral(f->stream, entry, f->name());
                        f->stream << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
//                    }
                }
            }
        }

        // Initialization function: Registers meta types
        writeInitializationFunctionName(f->stream, fileHash.key(f, ""), true);
        f->stream << endl << "{" << endl;
        for (it = entries.begin(); it != entries.end(); ++it) {
            QList<TypeEntry *> entries = it.value();
            foreach(TypeEntry *entry, entries) {
                if (entry &&
                        ((shouldGenerate(entry) && entry->isPrimitive())
                         || entry->isString()
                         || entry->isStringRef()
                         || entry->isChar())) {
                    writeInitialization(f->stream, entry, 0);
                }
            }
        }
        writeRegisterSignalsAndSlots(f->stream);
        writeRegisterEnums(f->stream);
    }

    if(qmlListPropertyTypeEntry){
        FileOut *f = fileHash.value("org.qtjambi.qt.qml", 0);
        if(f){
             f->stream << "registerQtToJava(\"QQmlListProperty\", \"org/qtjambi/qt/qml/QQmlListProperty\");" << endl;
             f->stream << "registerJavaToQt(\"org/qtjambi/qt/qml/QQmlListProperty\", \"QQmlListProperty\");" << endl;
             f->stream << "qRegisterMetaType<QtjambiQmlListProperty>(\"QQmlListProperty\");" << endl;
        }
    }
    if(declarativeListPropertyTypeEntry){
        FileOut *f = fileHash.value("org.qtjambi.qt.declarative", 0);
        if(f){
            f->stream << "registerQtToJava(\"QDeclarativeListProperty\", \"org/qtjambi/qt/declarative/QDeclarativeListProperty\");" << endl;
            f->stream << "registerJavaToQt(\"org/qtjambi/qt/declarative/QQmlListProperty\", \"QDeclarativeListProperty\");" << endl;
            f->stream << "qRegisterMetaType<QtjambiDeclarativeListProperty>(\"QDeclarativeListProperty\");" << endl;
        }
    }

    foreach(AbstractMetaClass *cls, classList) {
        FileOut *f = fileHash.value(cls->package(), 0);

        if (f != 0) {
            writeInitialization(f->stream, cls->typeEntry(), cls, shouldGenerate(cls));
        }
    }

    foreach(QString package, fileHash.keys()) {
        FileOut *f = fileHash.value(package, 0);
        if (f != 0) {
            foreach(QString handler, handlers_to_register.value(package, QStringList())) {
                f->stream << "    qtjambi_register_polymorphic_id(\"" << handler << "\","
                << "polymorphichandler_" << handler << ");" << endl;
            }

            f->stream << "}" << endl << endl;
            if (f->done())
                ++m_num_generated_written;
            ++m_num_generated;

            delete f;
        }
    }
}

void MetaInfoGenerator::writeHeaderFile() {
    AbstractMetaClassList classList = classes();
    QHash<QString, bool> fileHash;

    foreach(AbstractMetaClass *cls, classList) {
        bool hasGenerated = fileHash.value(cls->package(), false);
        if (!hasGenerated && generated(cls)) {
            FileOut file(cppOutputDirectory() + "/" + subDirectoryForClass(cls, CppDirectory) + "/" + headerFilename());
            file.stream << "#ifndef " << filenameStub().toUpper() << "_H" << endl;
            file.stream << "#define " << filenameStub().toUpper() << "_H" << endl << endl;
            writeInitializationFunctionName(file.stream, cls->package(), true);
            file.stream << ";" << endl << "#endif" << endl << endl;

            fileHash.insert(cls->package(), true);

            QString pro_file_name = cls->package().replace(".", "_") + "/" + cls->package().replace(".", "_") + ".pri";
            priGenerator->addHeader(pro_file_name, headerFilename());

            if (file.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
}

void MetaInfoGenerator::writeCodeBlock(QTextStream &s, const QString &code) {
    QStringList lines = code.split('\n');
    QString indent;
    foreach(QString str, lines) {
        s << "    " << indent << str.trimmed() << endl;
        if (!str.trimmed().endsWith(";") && !str.trimmed().isEmpty())
            indent = "    ";
        else
            indent = "";
    }
}

const AbstractMetaClass* MetaInfoGenerator::lookupClassWithPublicDestructor(const AbstractMetaClass *cls) {
    while (cls != 0) {
        if (cls->hasPublicDestructor()) {
            return cls;
        } else {
            cls = cls->baseClass();
        }
    }
    return 0;
}

void MetaInfoGenerator::writeDestructors(QTextStream &s, const AbstractMetaClass *cls) {
    // We can only delete classes with public destructors
    const AbstractMetaClass *clsWithPublicDestructor = lookupClassWithPublicDestructor(cls);
    if (clsWithPublicDestructor != 0) {
        const ComplexTypeEntry *entry = cls->typeEntry();
        if ((entry->codeGeneration() & TypeEntry::GenerateCode) != 0) {
            s   << "static void deleter_" << entry->javaPackage().replace(".", "_")  << "_"
            << entry->lookupName().replace(".", "_").replace("$", "_") << "(void *ptr)" << endl
            << "{" << endl
            << "    delete reinterpret_cast<" << clsWithPublicDestructor->qualifiedCppName() << " *>(ptr);" << endl;

//            if (qtJambiDebugTools()) {
                s << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl
                << "    qtjambi_increase_destructorFunctionCalledCount(QString::fromLatin1(\"" << cls->name() << "\"));" << endl
                << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
//            }

            s   << "}" << endl << endl;
        }
    }
}

void MetaInfoGenerator::writeCustomStructors(QTextStream &s, const TypeEntry *entry) {
    if (!entry->preferredConversion())
        return ;

    CustomFunction customCreator = entry->customCreator();
    CustomFunction customDeleter = entry->customDeleter();
    CustomFunction customConstructor = entry->customConstructor();
    CustomFunction customDestructor = entry->customDestructor();

    if (!customCreator.name.isEmpty()) {
        s << "// Custom constructor and destructor for " << entry->qualifiedCppName() << endl
        << "static void *" << customCreator.name << "("
        << "const " << entry->qualifiedCppName() << " *" << customCreator.param_name
        << ")" << endl
        << "{" << endl;
        writeCodeBlock(s, customCreator.code());
        s << "}" << endl << endl;

#if QT_VERSION >= 0x050000
        if(customConstructor.name.isEmpty()){
            s << "static void * " << customCreator.name + "_construct_ ("
            << "void* where, const void * __in_var"
            << ")" << endl
            << "{" << endl
            << "    const " << entry->qualifiedCppName() << "* " << customCreator.param_name << " = static_cast<" << "const " << entry->qualifiedCppName() << "*>(__in_var);";
            writeCodeBlock(s, QString(customCreator.code()).replace(QString("new %1").arg(entry->qualifiedCppName()), QString("new(where) %1").arg(entry->qualifiedCppName())));
            s << "}" << endl << endl;
        }else{
            s << "static void * " << customConstructor.name << "("
            << "void* " << customConstructor.where_name << ", const void * __in_var"
            << ")" << endl
            << "{" << endl
            << "    const " << entry->qualifiedCppName() << "* " << customConstructor.param_name << " = static_cast<" << "const " << entry->qualifiedCppName() << "*>(__in_var);";
            writeCodeBlock(s, customConstructor.code());
            s << "}" << endl << endl;
        }
#endif
    }

    if (!customDeleter.name.isEmpty()) {
        s << "static void " << customDeleter.name << "("
        << "const " << entry->qualifiedCppName() << " *" << customDeleter.param_name
        << ")" << endl
        << "{" << endl;
        writeCodeBlock(s, customDeleter.code());
        s << "}" << endl << endl;

    }
#if QT_VERSION >= 0x050000
    if (!customDestructor.name.isEmpty()) {
        s << "static void " << customDestructor.name << "("
        << "const void * __in_var"
        << ")" << endl
        << "{" << endl
        << "    const " << entry->qualifiedCppName() << "* " << customConstructor.param_name << " = static_cast<" << "const " << entry->qualifiedCppName() << "*>(__in_var);";
        writeCodeBlock(s, customDestructor.code());
        s << "}" << endl << endl;
    }
#endif
}

static void generateInitializer(QTextStream &s, const QString &package, CodeSnip::Position pos) {
    TypeSystemTypeEntry * typeSystemEntry = (TypeSystemTypeEntry *) TypeDatabase::instance()->findType(package);
    if(typeSystemEntry){
        QList<CodeSnip> snips = typeSystemEntry->snips;

        foreach(const CodeSnip &snip, snips) {
            if (snip.position == pos)
                s << snip.code();
        }
    }
}

void MetaInfoGenerator::writeLibraryInitializers() {
    // from cppimplgenerator.cpp
    extern QString jni_function_signature(QString package,
                                              QString class_name,
                                              const QString &function_name,
                                              const QString &return_type,
                                              const QString &mangled_arguments = QString(),
                                              uint options = CppImplGenerator::StandardJNISignature);

    // We need to generate a library initializer in Java for all packages
    // that have generated classes in Java, and in C++ for all packages
    // that have generated metainfo.

    QList<QString> known_packages = m_skip_list.keys();
    foreach(QString package, known_packages) {
        if (generatedMetaInfo(package)) { // write cpp file

            FileOut fileOut(cppOutputDirectory() + "/" + subDirectoryForPackage(package, CppDirectory) + "/qtjambi_libraryinitializer.cpp");

            QString signature = jni_function_signature(package, "QtJambi_LibraryInitializer",
                                "__qt_initLibrary", "void");
            QTextStream &s = fileOut.stream;
            s << "#include \"metainfo.h\"" << endl
            << "#include <qtjambi/qtjambi_global.h>" << endl << endl
            << signature << "(JNIEnv *, jclass)" << endl
            << "{" << endl
            << "    ";
            writeInitializationFunctionName(s, package, false);
            s << ";" << endl
            << "}" << endl << endl;

            QString pro_file_name = QString(package).replace(".", "_");

            priGenerator->addSource(pro_file_name + "/" + pro_file_name + ".pri", "qtjambi_libraryinitializer.cpp");

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }

        if (generatedJavaClasses(package)) {

            FileOut fileOut(javaOutputDirectory() + "/" + subDirectoryForPackage(package, JavaDirectory) + "/QtJambi_LibraryInitializer.java");

            QTextStream &s = fileOut.stream;
            s << "package " << package << ";" << endl << endl
            << "class QtJambi_LibraryInitializer" << endl
            << "{" << endl
            << "    static {" << endl
            << "        try {" << endl;

            generateInitializer(s, package, CodeSnip::Beginning);

            s << "            org.qtjambi.qt.Utilities.loadJambiJniLibrary(\""
            << QString(package).replace(".", "_") << "\");" << endl;

            if (generatedMetaInfo(package))
                s << "            __qt_initLibrary();" << endl;

            generateInitializer(s, package, CodeSnip::End);

            s << "        } catch(Throwable t) {" << endl;
            // we encapsulate this in try/catch so every individual *.java doesn't have to
            //  and the user gets some feedback about problems.
            s << "            t.printStackTrace();" << endl;
            s << "        }" << endl;
            s << "    }" << endl;

            if (generatedMetaInfo(package))
                s << "    private native static void __qt_initLibrary();" << endl;

            s << "    static void init() { };" << endl
            << "}" << endl << endl;

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
}

void MetaInfoGenerator::writeInclude(QTextStream &s, const Include &inc, QSet<QString> &dedupe) {
    if (inc.name.isEmpty())
        return;

    QString incString;
    if (inc.type == Include::LocalPath)
        incString = "\"" + inc.name + "\"";
    else
        incString = "<" + inc.name + ">";

    if (dedupe.contains(incString))
        return;    // no need to emit this #include it is already in the generared file

    dedupe.insert(incString);

    s << "#include " << incString << endl;
}

void MetaInfoGenerator::writeIncludeStatements(QTextStream &s, const AbstractMetaClassList &classList,
        const QString &package) {
    QSet<QString> dedupe = QSet<QString>();

    writeInclude(s, Include(Include::LocalPath, headerFilename()), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QMetaType"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QString"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QLatin1String"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QHash"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QReadWriteLock"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QReadLocker"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QWriteLocker"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_cache.h"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_core.h"), dedupe);

    if(package=="org.qtjambi.qt.qml"){
        writeInclude(s, Include(Include::IncludePath, "qtjambi_qml/qqmllistproperty.h"), dedupe);
    }
    if(package=="org.qtjambi.qt.declarative"){
        writeInclude(s, Include(Include::IncludePath, "qtjambi_declarative/qdeclarativelistproperty.h"), dedupe);
    }
//    if (qtJambiDebugTools()) {
        s << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl;
        writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambidebugtools_p.h"), dedupe);
        s << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
//    }

    s << endl;

    foreach(AbstractMetaClass *cls, classList) {
        if (generated(cls) && !cls->isInterface() && cls->package() == package) {
            const ComplexTypeEntry *ctype = cls->typeEntry();

            Include inc = ctype->include();
            writeInclude(s, inc, dedupe);
        }
    }
}

void MetaInfoGenerator::writeInitializationFunctionName(QTextStream &s, const QString &package, bool fullSignature) {
    if (fullSignature)
        s << "void ";
    s << "__metainfo_init_" << QString(package).replace(".", "_") << "()";
}

void MetaInfoGenerator::writeInitialization(QTextStream &s, const TypeEntry *entry, const AbstractMetaClass *cls,
        bool registerMetaType) {

    if (entry->codeGeneration() == TypeEntry::GenerateForSubclass)
        return;

    if (cls && cls->attributes() & AbstractMetaAttributes::Fake)
        return;

    QString creatorName = entry->customCreator().name;
    QString deleterName = entry->customDeleter().name;
#if QT_VERSION < 0x050000
    QString constructorName = "";
    QString destructorName = "";
#else
    QString constructorName = entry->customConstructor().name;
    QString destructorName = entry->customDestructor().name;

    bool useGenericDeleter = false;
    bool useGenericDestructor = false;

    if(!creatorName.isEmpty() && constructorName.isEmpty()){
        constructorName = creatorName + "_construct_";
    }
#endif


    /*
    // currently no use for this
    QString type_flags = entry->typeFlags();
    if(type_flags.isEmpty()){
        type_flags = "QMetaType::NeedsConstruction | QMetaType::NeedsDestruction | QMetaType::MovableType | QMetaType::WasDeclaredAsMetaType";
    }
    */

    bool reasonQtJambiDebugToolsOnly = false;
    if (qtJambiDebugTools()) {
        if (creatorName.isEmpty() && deleterName.isEmpty() && constructorName.isEmpty() && destructorName.isEmpty())
            reasonQtJambiDebugToolsOnly = true;		// ifdef the emitter

        if (creatorName.isEmpty())
            creatorName = "qtjambiGenericCreator<" + entry->qualifiedCppName() + ">";
        if (deleterName.isEmpty()){
            deleterName = "qtjambiGenericDeleter<" + entry->qualifiedCppName() + ">";
            useGenericDeleter = true;
        }

#if QT_VERSION >= 0x050000
        if (constructorName.isEmpty())
            constructorName = "qtjambiGenericConstructor<" + entry->qualifiedCppName() + ">";

        if (destructorName.isEmpty()){
            destructorName = "qtjambiGenericDestructor<" + entry->qualifiedCppName() + ">";
            useGenericDestructor = true;
        }
#endif
    }else{
        if (!creatorName.isEmpty() && deleterName.isEmpty()){
            deleterName = "qtjambiGenericDeleter<" + entry->qualifiedCppName() + ">";
            useGenericDeleter = true;
        }
#if QT_VERSION >= 0x050000
        if (!constructorName.isEmpty() && destructorName.isEmpty()){
            destructorName = "qtjambiGenericDestructor<" + entry->qualifiedCppName() + ">";
            useGenericDestructor = true;
        }
#endif
    }


    if (creatorName.isEmpty() != constructorName.isEmpty()) {
        ReportHandler::warning(QString("specify either no custom functions, or both "
                                       "creator and constructor for type '%1'").arg(entry->name()));
    }

    QString javaPackage = entry->javaPackage();

    QString javaName =  entry->lookupName();
    if (!javaPackage.isEmpty()) {
        javaName.prepend(javaPackage.replace(".", "/") + "/");
    }


    if (entry->isComplex()) {
        const ComplexTypeEntry *centry = static_cast<const ComplexTypeEntry *>(entry);
        if (centry->typeFlags() & ComplexTypeEntry::DeleteInMainThread)
            s << "    registerObjectDeletionPolicy(\"" << javaName << "\", ObjectDeletionPolicyDeleteInMainThread);" << endl;
    }

    QString qtName = entry->qualifiedCppName();
    if ((!entry->isInterface())
            && (!entry->isPrimitive() || ((PrimitiveTypeEntry *) entry)->preferredTargetLangType()))
        s << "    registerQtToJava(\"" << qtName << "\", \"" << javaName << "\");" << endl;

    if (!entry->preferredConversion())
        return ;

    s << "    registerJavaToQt(\"" << javaName << "\", \"" << qtName << "\");" << endl;
    if (entry->isComplex() && entry->isObject() && !((ComplexTypeEntry *)entry)->isQObject() && !entry->isInterface()) {
        QString patchedName = QString(javaName).replace("/", "_").replace("$", "_");

        if (lookupClassWithPublicDestructor(cls))
            s << "    registerDeleter(\"" << javaName << "\", deleter_" << patchedName << ");" << endl;
    }

    if (!registerMetaType)
        return ;

    int metaType = QMetaType::type(entry->name().toLocal8Bit().constData());
#if QT_VERSION < 0x050000
    if (metaType != QMetaType::Void)
#else
    if (metaType != QMetaType::UnknownType)
#endif
        return ;
    if (entry->name() == "QStringRef")	// Special case, this does not have metaType number
        return ;

    // This variable exists so that the emitted code is the same with QTJAMBI_DEBUG_TOOLS
    // is not defined during compilation.
    bool emittedRegisterType = false;
    if (!creatorName.isEmpty() && !deleterName.isEmpty()) {
        QString registerTypeFunctionName = QString("QMetaType::registerType");
        if (reasonQtJambiDebugToolsOnly)
            s << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl;
        s << "    " << registerTypeFunctionName << "(\"" << entry->qualifiedCppName() << "\"," << endl
#if QT_VERSION < 0x050000
        << "                            reinterpret_cast<QMetaType::Destructor>( "
#else
        << "                            reinterpret_cast<QMetaType::Deleter>( "
#endif
        // This cast is needed by GCC 4.4.4 at least, to infer the correct type from the context
        << "(void (*)(" << entry->qualifiedCppName() << " *)) ";
        if(useGenericDeleter){
            s << endl << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl;
            QString choppedDeleterName(deleterName);
            choppedDeleterName.chop(1);
            s << "                                                                   ";
            s << choppedDeleterName << ", __name_" << QString(entry->name()).replace(":", "_").replace(' ', '_') << ">" << endl;
            s << "#else /* QTJAMBI_DEBUG_TOOLS */" << endl;
            s << "                                                                   ";
        }
        s << deleterName;
        if(useGenericDeleter){
            s << endl << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
            s << "                           ";
        }
        s << " )," << endl
#if QT_VERSION < 0x050000
        << "                            reinterpret_cast<QMetaType::Constructor>( "
#else
        << "                            reinterpret_cast<QMetaType::Creator>( "
#endif
        // This cast is needed by GCC 4.4.4 at least, to infer the correct type from the context
        << "(void *(*)(const " << entry->qualifiedCppName() << " *)) "
        << creatorName
#if QT_VERSION < 0x050000
        << " )" << endl
#else
        << " )," << endl
        << "                            reinterpret_cast<QMetaType::Destructor>( (void (*)(" << entry->qualifiedCppName() << " *)) ";
        if(useGenericDestructor){
            s << endl << "#if defined(QTJAMBI_DEBUG_TOOLS)" << endl;
            s << "                                                                   ";
            QString choppedDestructorName(destructorName);
            choppedDestructorName.chop(1);
            s << choppedDestructorName << ", __name_" << QString(entry->name()).replace(":", "_").replace(' ', '_') << ">" << endl;
            s << "#else /* QTJAMBI_DEBUG_TOOLS */" << endl;
            s << "                                                                   ";
        }
        s << destructorName;
        if(useGenericDestructor){
            s << endl << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
            s << "                            ";
        }
        s << ")," << endl
        << "                            reinterpret_cast<QMetaType::Constructor>( (void *(*)(const " << entry->qualifiedCppName() << " *)) " << constructorName << "),"  << endl
        << "                            sizeof(" << entry->qualifiedCppName() << ")," << endl
        << "                            QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<" << entry->qualifiedCppName() << ">::Flags)," << endl
        << "                            QtPrivate::MetaObjectForType<" << entry->qualifiedCppName() << ">::value()" << endl
#endif
        << "    );" << endl;
        emittedRegisterType = true;
        if (reasonQtJambiDebugToolsOnly)
            s << "#else /* QTJAMBI_DEBUG_TOOLS */" << endl;
    }

    if(reasonQtJambiDebugToolsOnly || !emittedRegisterType) {
        // Look for default constructor, required for qRegisterMetaType
        if (cls != 0) {
            AbstractMetaFunctionList functions = cls->queryFunctions(AbstractMetaClass::WasPublic | AbstractMetaClass::Constructors);

            bool hasDefaultConstructor = false;
            foreach(AbstractMetaFunction *function, functions) {
                // Default constructor has to be present
                if (function->wasPublic() && function->actualMinimumArgumentCount() == 0)
                    hasDefaultConstructor = true;
            }

            if (!hasDefaultConstructor) {
                ReportHandler::warning(QString("Value type '%1' is missing a default constructor. "
                                               "The resulting C++ code will not compile. If necessary, use <custom-creator>, <custom-deleter>, <custom-constructor> and "
                                               "<custom-destructor> tags to provide the constructors.").arg(cls->fullName()));
            }

        }
        s << "    qRegisterMetaType<" << entry->qualifiedCppName() << ">(\"" << entry->qualifiedCppName() << "\");" << endl;
    }

    if (!creatorName.isEmpty() && !deleterName.isEmpty()) {	// PARANOID
        if (reasonQtJambiDebugToolsOnly)
            s << "#endif /* QTJAMBI_DEBUG_TOOLS */" << endl;
    }
}
