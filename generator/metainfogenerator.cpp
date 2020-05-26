/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

QString MetaInfoGenerator::subDirectoryForPackage(const QString &package, OutputDirectoryType type){
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
    switch (type) {
        case CppDirectory:
            {
                if(typeSystemEntry && !typeSystemEntry->qtLibrary().isEmpty())
                    if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
                        QString libName = typeSystemEntry->qtLibrary();
                        return "QtJambi" + libName.mid(2);
                    }else
                        return typeSystemEntry->qtLibrary();
                else
                    return QString(package).replace(".", "_");
            }
        case JavaDirectory:
            {
                QString pkgDir = QString(package).replace(".", "/");
                if(typeSystemEntry && !typeSystemEntry->qtLibrary().isEmpty())
                    if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
                        QString libName = typeSystemEntry->qtLibrary();
                        return "QtJambi" + libName.mid(2) + "/" + pkgDir;
                    }else
                        return typeSystemEntry->qtLibrary() + "/" + pkgDir;
                else
                    return pkgDir;
            }
    }
    return QString(); // kill nonsense warnings
}

QString MetaInfoGenerator::subDirectoryForClass(const AbstractMetaClass *cls, OutputDirectoryType type){
    Q_ASSERT(cls);
    return subDirectoryForPackage(cls->targetTypeSystem(), type);
}

QString MetaInfoGenerator::subDirectoryForClass(const AbstractMetaFunctional *cls, OutputDirectoryType type){
    Q_ASSERT(cls);
    return subDirectoryForPackage(cls->targetTypeSystem(), type);
}

void MetaInfoGenerator::generate() {
    buildSkipList();
    writeCppFile();
    writeLibraryInitializers();
}

bool MetaInfoGenerator::shouldGenerate(const TypeEntry *entry) const {
    return entry != nullptr && !entry->isIterator() && !entry->isNamespace() && !entry->isEnum() && (entry->codeGeneration() & TypeEntry::GenerateCpp);
}

bool MetaInfoGenerator::shouldGenerate(const AbstractMetaClass *cls) const {
    return (!cls->isInterface() && cls->typeEntry()->isValue() && !cls->typeEntry()->isIterator()
            && !cls->isAbstract() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp));
}

QString MetaInfoGenerator::fileNameForClass(const AbstractMetaClass *) const {
    return filenameStub() + ".cpp";
}

void MetaInfoGenerator::write(QTextStream &, const AbstractMetaClass *, int) {
    // not used
}

bool MetaInfoGenerator::generated(const AbstractMetaClass *cls) const {
    return generatedMetaInfo(cls->targetTypeSystem());
}

bool MetaInfoGenerator::generatedMetaInfo(const QString &package) const {
    return (m_skip_list.value(package, 0x0) & GeneratedMetaInfo);
}

bool MetaInfoGenerator::generatedJavaClasses(const QString &package) const {
    return (m_skip_list.value(package, 0x0) & GeneratedJavaClasses);
}

/**
 * Builds a skip list of classes that shouldn't be built.
 */
void MetaInfoGenerator::buildSkipList() {
    AbstractMetaClassList classList = classes();
    for(AbstractMetaClass *cls : classList) {
        if (!m_skip_list.contains(cls->targetTypeSystem()))
            m_skip_list[cls->targetTypeSystem()] = 0x0;

        if (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
            m_skip_list[cls->targetTypeSystem()] |= GeneratedMetaInfo;

        if (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)
            m_skip_list[cls->targetTypeSystem()] |= GeneratedJavaClasses;
    }
}

static void generateInitializer(QTextStream &s, const QString &package, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent);

void MetaInfoGenerator::writeCppFile() {
    TypeEntryHash entries = TypeDatabase::instance()->allEntries();
    TypeEntryHash::iterator it;

    AbstractMetaClassList classList = classes();
    QHash<QString, FileOut *> fileHash;

    Indentor INDENT;

    // Seems continue is not supported by our foreach loop, so
    for(AbstractMetaClass *cls : classList) {

        FileOut *f = fileHash.value(cls->targetTypeSystem(), nullptr);
        if (f == nullptr && generated(cls)) {
            f = new FileOut(cppOutputDirectory() + "/" + subDirectoryForClass(cls, CppDirectory) + "/" + cppFilename());

            QSet<QString> dedupe = QSet<QString>();
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_core.h"), dedupe);
            QString typeSystemName = cls->targetTypeSystem();
            if(TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(cls->targetTypeSystem()))){
                if(typeSystemEntry->include().type!=Include::TargetLangImport){
                    writeInclude(f->stream, typeSystemEntry->include(), dedupe);
                }
                for(const Include& incl : typeSystemEntry->extraIncludes()){
                    if(incl.type!=Include::TargetLangImport){
                        writeInclude(f->stream, incl, dedupe);
                    }
                }
            }
            f->stream << endl;

            generateInitializer(f->stream, cls->targetTypeSystem(), TypeSystem::MetaInfo, CodeSnip::Position1, INDENT);

            fileHash.insert(cls->targetTypeSystem(), f);

            QString pro_file_name = subDirectoryForClass(cls, CppDirectory) + "/generated.pri";
            priGenerator->addSource(pro_file_name, cppFilename());
        }

        if (f != nullptr) {
            if(!cls->typeEntry()->isIterator() && (!cls->isFake() || !cls->enums().isEmpty()) && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0){
                f->stream << INDENT << "void initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << endl;
            }
            for(AbstractMetaFunctional* functional : cls->functionals()){
                f->stream << INDENT << "void initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_").replace("<", "_").replace(">", "_") << "();" << endl;
            }
        }
    }

    // Primitive types must be added to all packages, in case the other packages are
    // not referenced from the generated code.
    for(const QString& package : fileHash.keys()) {
        FileOut *f = fileHash.value(package, nullptr);
        if (f != nullptr) {
            for (it = entries.begin(); it != entries.end(); ++it) {
                QList<TypeEntry *> entries = it.value();
                for(TypeEntry *entry : entries) {
                    if (shouldGenerate(entry) && entry->isPrimitive()) {
                        CppImplGenerator::writeCustomStructors(f->stream, entry);
                    }
                }
            }
            generateInitializer(f->stream, package, TypeSystem::MetaInfo, CodeSnip::Position2, INDENT);

            // Initialization function: Registers meta types
            f->stream << endl
                      << INDENT << "extern \"C\" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *, void *)" << endl
                      << INDENT << "{" << endl;
            INDENTATION(INDENT)
            f->stream << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << package << "::JNI_OnLoad(JavaVM *, void *)\")" << endl;
            generateInitializer(f->stream, package, TypeSystem::MetaInfo, CodeSnip::Beginning, INDENT);
        }
    }

    {
        INDENTATION(INDENT)
        QHash<const TypeEntry *, AbstractMetaEnum *> metaEnums;
        for(AbstractMetaClass *cls : classList) {
            FileOut *f = fileHash.value(cls->targetTypeSystem(), nullptr);

            if (f != nullptr && (!cls->isFake() || !cls->enums().isEmpty()) ) {
                if (cls && !cls->typeEntry()->isIterator() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0){
                    f->stream << INDENT << "initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << endl;
                    for(AbstractMetaFunctional* functional : cls->functionals()){
                        f->stream << INDENT << "initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_") << "();" << endl;
                    }
                }
            }
        }
    }

    for(const QString& package : fileHash.keys()) {
        FileOut *f = fileHash.value(package, nullptr);
        if (f != nullptr) {
            {
                INDENTATION(INDENT)
                generateInitializer(f->stream, package, TypeSystem::MetaInfo, CodeSnip::Position3, INDENT);
                generateInitializer(f->stream, package, TypeSystem::MetaInfo, CodeSnip::Position4, INDENT);
                generateInitializer(f->stream, package, TypeSystem::MetaInfo, CodeSnip::End, INDENT);
                f->stream << INDENT << "return JNI_VERSION_10;" << endl;
            }
            f->stream << "}" << endl << endl;
            if (f->done())
                ++m_num_generated_written;
            ++m_num_generated;

            delete f;
        }
    }
}

static void generateInitializer(QTextStream &s, const QString &package, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent) {
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
    if(typeSystemEntry){
        QList<CodeSnip> snips = typeSystemEntry->snips;

        QStringList lines;
        for(const CodeSnip &snip : snips) {
            if (snip.position == pos && snip.language == language)
                lines << snip.code().split("\n");
        }
        while(!lines.isEmpty()){
            if(lines.last().trimmed().isEmpty()){
                lines.takeLast();
            }else{
                break;
            }
        }
        while(!lines.isEmpty()){
            if(lines.first().trimmed().isEmpty()){
                lines.takeFirst();
            }else{
                break;
            }
        }

        int sp = -1;
        QString spaces;
        if(!lines.isEmpty())
            s << endl;
        for(QString line : lines) {
            if(!line.isEmpty() && line[0]==QLatin1Char('\r')){
                line = line.mid(1);
            }
            if(sp<0 && line.isEmpty()){
                continue;
            }
            if(sp<0 && !QString(line).trimmed().isEmpty()){
                for(sp=0; sp<line.length(); ++sp){
                    if(line[sp]!=QLatin1Char(' ')){
                        break;
                    }
                }
                if(sp==0){
                    sp = 0;
                    for(; sp<lines[0].length(); ++sp){
                        if(lines[0][sp]!=QLatin1Char('\t')){
                            break;
                        }
                    }
                    spaces.fill(QLatin1Char('\t'), sp);
                }else{
                    spaces.fill(QLatin1Char(' '), sp);
                }
            }
            if(line.startsWith(spaces))
                line = line.mid(sp);
            s << indent << line << endl;
        }
    }
}

void MetaInfoGenerator::writeLibraryInitializers() {
    // We need to generate a library initializer in Java for all packages
    // that have generated classes in Java, and in C++ for all packages
    // that have generated metainfo.

    QList<QString> known_packages = m_skip_list.keys();
    for(const QString& package : known_packages) {

        if (generatedJavaClasses(package)) {

            FileOut fileOut(javaOutputDirectory() + "/" + subDirectoryForPackage(package, JavaDirectory) + "/QtJambi_LibraryInitializer.java");

            Indentor INDENT;
            QTextStream &s = fileOut.stream;
            s << INDENT << "package " << package << ";" << endl << endl;

            QSet<QString> dedupe = QSet<QString>();
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
            if(typeSystemEntry){
                if(typeSystemEntry->include().type==Include::TargetLangImport){
                    writeInclude(s, typeSystemEntry->include(), dedupe);
                }
                for(const Include& incl : typeSystemEntry->extraIncludes()){
                    if(incl.type==Include::TargetLangImport){
                        writeInclude(s, incl, dedupe);
                    }
                }
            }
            if(!dedupe.isEmpty()){
                s << endl;
            }
            s << INDENT << "final class QtJambi_LibraryInitializer {" << endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "static{" << endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "try {" << endl;
                    {
                        INDENTATION(INDENT)
                        generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Beginning, INDENT);
                        s << INDENT << "io.qt.QtUtilities.initializePackage(\"io.qt.internal\");" << endl;
                        generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Position1, INDENT);
                        if(typeSystemEntry){
                            for(const TypeSystemTypeEntry* entry : typeSystemEntry->requiredTypeSystems()){
                                s << INDENT << "io.qt.QtUtilities.initializePackage(\"" << entry->name() << "\");" << endl;
                            }
                            generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Position2, INDENT);
                            for(const QString& lib : typeSystemEntry->requiredQtLibraries()){
                                if(lib.startsWith("Qt")){
                                    s << INDENT << "io.qt.QtUtilities.loadQtLibrary(\"" << lib.mid(2) << "\");" << endl;
                                }else{
                                    s << INDENT << "io.qt.QtUtilities.loadUtilityLibrary(\"" << lib << "\");" << endl;
                                }
                            }
                            generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Position3, INDENT);
                            if(!typeSystemEntry->qtLibrary().isEmpty()){
                                if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                    s << INDENT << "io.qt.QtUtilities.loadQtLibrary(\"" << typeSystemEntry->qtLibrary().mid(2) << "\");" << endl;
                                }else{
                                    s << INDENT << "io.qt.QtUtilities.loadUtilityLibrary(\"" << typeSystemEntry->qtLibrary() << "\");" << endl;
                                }
                            }
                        }else{
                            generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Position2, INDENT);
                            generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Position3, INDENT);
                        }
                        generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::Position4, INDENT);
                        if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                            QString libName = typeSystemEntry->qtLibrary();
                            s << INDENT << "io.qt.QtUtilities.loadQtJambiLibrary(\"" << libName.mid(2) << "\");" << endl;
                        }else{
                            s << INDENT << "io.qt.QtUtilities.loadJambiLibrary(\"" << QString(package).replace(".", "_") << "\");" << endl;
                        }
                        generateInitializer(s, package, TypeSystem::TargetLangCode, CodeSnip::End, INDENT);
                    }
                    s << INDENT << "} catch(RuntimeException | Error t) {" << endl;
                    s << INDENT << "    throw t;" << endl;
                    s << INDENT << "} catch(Throwable t) {" << endl;
                    s << INDENT << "    throw new RuntimeException(t);" << endl;
                    s << INDENT << "}" << endl;
                }
                s << INDENT << "}" << endl;
                s << INDENT << "static void init() { };" << endl;
                s << INDENT << "private QtJambi_LibraryInitializer() { };" << endl;
            }
            s << INDENT << "}" << endl << endl;

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
}
