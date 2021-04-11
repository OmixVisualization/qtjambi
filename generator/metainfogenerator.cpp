/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

QString MetaInfoGenerator::subDirectoryForPackage(const QString &typeSystem, const QString &package, OutputDirectoryType type){
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(typeSystem));
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
    return subDirectoryForPackage(cls->targetTypeSystem(), cls->package(), type);
}

QString MetaInfoGenerator::subDirectoryForClass(const AbstractMetaFunctional *cls, OutputDirectoryType type){
    Q_ASSERT(cls);
    return subDirectoryForPackage(cls->targetTypeSystem(), cls->package(), type);
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

/**
 * Builds a skip list of classes that shouldn't be built.
 */
void MetaInfoGenerator::buildSkipList() {
    for(AbstractMetaClass *cls : m_classes) {
        if(cls->isFake()){
            for(AbstractMetaEnum* _cls : cls->enums()){
                if (!m_packageGenerationPolicies.contains(_cls->targetTypeSystem()))
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] = 0x0;

                if (_cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] |= GeneratedMetaInfo;

                if (_cls->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] |= GeneratedJavaClasses;
                    if(_cls->targetTypeSystem()!=_cls->package()){
                        if (!m_packageGenerationPolicies.contains(_cls->package()))
                            m_packageGenerationPolicies[_cls->package()] = 0x0;
                        m_packageGenerationPolicies[_cls->package()] |= GeneratedJavaClasses;
                        m_typeSystemByPackage[_cls->package()] = _cls->targetTypeSystem();
                    }
                }
            }
            for(AbstractMetaFunctional* _cls : cls->functionals()){
                if (!m_packageGenerationPolicies.contains(_cls->targetTypeSystem()))
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] = 0x0;

                if (_cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] |= GeneratedMetaInfo;

                if (_cls->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] |= GeneratedJavaClasses;
                    if(_cls->targetTypeSystem()!=_cls->package()){
                        if (!m_packageGenerationPolicies.contains(_cls->package()))
                            m_packageGenerationPolicies[_cls->package()] = 0x0;
                        m_packageGenerationPolicies[_cls->package()] |= GeneratedJavaClasses;
                        m_typeSystemByPackage[_cls->package()] = _cls->targetTypeSystem();
                    }
                }
            }
            for(AbstractMetaClass *_cls : cls->enclosedClasses()) {
                if (!m_packageGenerationPolicies.contains(_cls->targetTypeSystem()))
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] = 0x0;

                if (_cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] |= GeneratedMetaInfo;

                if (_cls->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
                    m_packageGenerationPolicies[_cls->targetTypeSystem()] |= GeneratedJavaClasses;
                    if(_cls->targetTypeSystem()!=_cls->package()){
                        if (!m_packageGenerationPolicies.contains(_cls->package()))
                            m_packageGenerationPolicies[_cls->package()] = 0x0;
                        m_packageGenerationPolicies[_cls->package()] |= GeneratedJavaClasses;
                        m_typeSystemByPackage[_cls->package()] = _cls->targetTypeSystem();
                    }
                }
            }
        }
        if (!m_packageGenerationPolicies.contains(cls->targetTypeSystem()))
            m_packageGenerationPolicies[cls->targetTypeSystem()] = 0x0;

        if (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
            m_packageGenerationPolicies[cls->targetTypeSystem()] |= GeneratedMetaInfo;

        if (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
            m_packageGenerationPolicies[cls->targetTypeSystem()] |= GeneratedJavaClasses;
            if(cls->targetTypeSystem()!=cls->package()){
                if (!m_packageGenerationPolicies.contains(cls->package()))
                    m_packageGenerationPolicies[cls->package()] = 0x0;
                m_packageGenerationPolicies[cls->package()] |= GeneratedJavaClasses;
                m_typeSystemByPackage[cls->package()] = cls->targetTypeSystem();
            }
        }
    }
}

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent);

void MetaInfoGenerator::writeCppFile() {
    const TypeEntryHash& entries = TypeDatabase::instance()->allEntries();

    QHash<QString, FileOut *> fileHash;
    QHash<QString, QSet<QString>> writtenClasses;

    Indentor INDENT;

    // Seems continue is not supported by our foreach loop, so
    for(AbstractMetaClass *cls : m_classes) {

        FileOut *f = fileHash.value(cls->targetTypeSystem(), nullptr);
        if (f == nullptr && (m_packageGenerationPolicies.value(cls->targetTypeSystem(), 0x0) & GeneratedMetaInfo)) {
            f = new FileOut(cppOutputDirectory() + "/" + subDirectoryForClass(cls, CppDirectory) + "/" + cppFilename());

            QSet<QString> dedupe = QSet<QString>();
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_core.h"), dedupe);
            QString typeSystemName = cls->targetTypeSystem();
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(cls->targetTypeSystem()));
            if(typeSystemEntry){
                if(typeSystemEntry->include().type!=Include::TargetLangImport){
                    writeInclude(f->stream, typeSystemEntry->include(), dedupe);
                }
                for(const Include& incl : typeSystemEntry->extraIncludes()){
                    if(incl.type!=Include::TargetLangImport){
                        writeInclude(f->stream, incl, dedupe);
                    }
                }
            }
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_registry.h"), dedupe);
            f->stream << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position1, INDENT);

            fileHash.insert(cls->targetTypeSystem(), f);

            QString pro_file_name = subDirectoryForClass(cls, CppDirectory) + "/generated.pri";
            priGenerator->addSource(pro_file_name, cppFilename());
        }

        if (f != nullptr) {
            if((!cls->typeEntry()->isIterator() && !cls->isFake() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0)
                    || (cls->isFake() && !cls->enums().isEmpty())){
                if(!writtenClasses[cls->targetTypeSystem()].contains(cls->typeEntry()->qualifiedCppName())){
                    writtenClasses[cls->targetTypeSystem()] << cls->typeEntry()->qualifiedCppName();
                    f->stream << INDENT << "void initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
                }
            }
            for(AbstractMetaFunctional* functional : cls->functionals()){
                f->stream << INDENT << "void initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
            }
        }
    }

    // Primitive types must be added to all packages, in case the other packages are
    // not referenced from the generated code.
    for(QHash<QString, FileOut *>::const_key_value_iterator iter = fileHash.constKeyValueBegin(); iter!=fileHash.constKeyValueEnd(); ++iter) {
        const QString& package = iter->first;
        FileOut *f = iter->second;
        if (f != nullptr) {
            for (const QList<TypeEntry *>& _entries : entries) {
                for(TypeEntry *entry : _entries) {
                    if (shouldGenerate(entry) && entry->isPrimitive()) {
                        CppImplGenerator::writeCustomStructors(f->stream, entry);
                    }
                }
            }
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position2, INDENT);

            // Initialization function: Registers meta types
            f->stream << Qt::endl
                      << INDENT << "extern \"C\" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *, void *)" << Qt::endl
                      << INDENT << "{" << Qt::endl;
            INDENTATION(INDENT)
            f->stream << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << package << "::JNI_OnLoad(JavaVM *, void *)\")" << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Beginning, INDENT);
        }
    }

    {
        INDENTATION(INDENT)
        writtenClasses.clear();
        for(AbstractMetaClass *cls : m_classes) {
            FileOut *f = fileHash.value(cls->targetTypeSystem(), nullptr);

            if (f != nullptr && cls) {
                if((!cls->typeEntry()->isIterator() && !cls->isFake() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0)
                        || (cls->isFake() && !cls->enums().isEmpty())){
                    if(!writtenClasses[cls->targetTypeSystem()].contains(cls->typeEntry()->qualifiedCppName())){
                        writtenClasses[cls->targetTypeSystem()] << cls->typeEntry()->qualifiedCppName();
                        f->stream << INDENT << "initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
                    }
                    for(AbstractMetaFunctional* functional : cls->functionals()){
                        f->stream << INDENT << "initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_") << "();" << Qt::endl;
                    }
                }
            }
        }
    }

    for(QHash<QString, FileOut *>::const_key_value_iterator iter = fileHash.constKeyValueBegin(); iter!=fileHash.constKeyValueEnd(); ++iter) {
        const QString& package = iter->first;
        FileOut *f = iter->second;
        if (f != nullptr) {
            {
                INDENTATION(INDENT)
                TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
                if(typeSystemEntry){
                    generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position3, INDENT);
                    generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position4, INDENT);
                    generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::End, INDENT);
                }
                f->stream << INDENT << "return JNI_VERSION_1_8;" << Qt::endl;
            }
            f->stream << "}" << Qt::endl << Qt::endl;
            if (f->done())
                ++m_num_generated_written;
            ++m_num_generated;

            delete f;
        }
    }
}

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent) {
    if(typeSystemEntry){
        QStringList lines;
        for(const CodeSnip &snip : typeSystemEntry->snips) {
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
            s << Qt::endl;
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
            s << indent << line << Qt::endl;
        }
    }
}

void MetaInfoGenerator::writeLibraryInitializers() {
    // We need to generate a library initializer in Java for all packages
    // that have generated classes in Java, and in C++ for all packages
    // that have generated metainfo.
    for(QHash<QString, quint8>::const_key_value_iterator iter = m_packageGenerationPolicies.constKeyValueBegin(); iter!=m_packageGenerationPolicies.constKeyValueEnd(); ++iter) {

        if (iter->second & GeneratedJavaClasses) {
            const QString& package = iter->first;
            if(package=="io.qt.internal")
                continue;
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
            QString subDir;
            QString typeSystemByPackage;
            if(typeSystemEntry)
                subDir = subDirectoryForPackage(typeSystemEntry->name(), package, JavaDirectory);
            else if(!(typeSystemByPackage = m_typeSystemByPackage[package]).isEmpty())
                subDir = subDirectoryForPackage(typeSystemByPackage, package, JavaDirectory);
            else
                continue;
            FileOut fileOut(javaOutputDirectory() + "/" + subDir + "/QtJambi_LibraryInitializer.java");

            Indentor INDENT;
            QTextStream &s = fileOut.stream;
            s << INDENT << "package " << package << ";" << Qt::endl << Qt::endl;

            QSet<QString> dedupe;
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
                s << Qt::endl;
            }
            s << INDENT << "final class QtJambi_LibraryInitializer {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "static{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "try {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(typeSystemEntry){
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Beginning, INDENT);
                            s << INDENT << "io.qt.QtUtilities.initializePackage(\"io.qt.internal\");" << Qt::endl;
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position1, INDENT);
                            for(const TypeSystemTypeEntry* entry : typeSystemEntry->requiredTypeSystems()){
                                s << INDENT << "io.qt.QtUtilities.initializePackage(\"" << entry->name() << "\");" << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position2, INDENT);
                            for(const QString& lib : typeSystemEntry->requiredQtLibraries()){
                                if(lib.startsWith("Qt")){
                                    s << INDENT << "if(io.qt.QtUtilities.isAvailableQtLibrary(\"" << lib.mid(2) << "\"))" << Qt::endl
                                      << INDENT << "    io.qt.QtUtilities.loadQtLibrary(\"" << lib.mid(2) << "\");" << Qt::endl;
                                }else{
                                    s << INDENT << "if(io.qt.QtUtilities.isAvailableUtilityLibrary(\"" << lib << "\"))" << Qt::endl
                                      << INDENT << "    io.qt.QtUtilities.loadUtilityLibrary(\"" << lib << "\");" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position3, INDENT);
                            if(!typeSystemEntry->qtLibrary().isEmpty()){
                                if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                    s << INDENT << "io.qt.QtUtilities.loadQtLibrary(\"" << typeSystemEntry->qtLibrary().mid(2) << "\");" << Qt::endl;
                                }else{
                                    s << INDENT << "io.qt.QtUtilities.loadUtilityLibrary(\"" << typeSystemEntry->qtLibrary() << "\");" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position4, INDENT);
                            if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                QString libName = typeSystemEntry->qtLibrary();
                                s << INDENT << "io.qt.QtUtilities.loadQtJambiLibrary(\"" << libName.mid(2) << "\");" << Qt::endl;
                            }else{
                                s << INDENT << "io.qt.QtUtilities.loadJambiLibrary(\"" << QString(package).replace(".", "_") << "\");" << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::End, INDENT);
                        }else{
                            s << INDENT << "io.qt.QtUtilities.initializePackage(\"" << typeSystemByPackage << "\");" << Qt::endl;
                        }
                    }
                    s << INDENT << "} catch(RuntimeException | Error t) {" << Qt::endl;
                    s << INDENT << "    throw t;" << Qt::endl;
                    s << INDENT << "} catch(Throwable t) {" << Qt::endl;
                    s << INDENT << "    throw new RuntimeException(t);" << Qt::endl;
                    s << INDENT << "}" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "static void init() { };" << Qt::endl;
                s << INDENT << "private QtJambi_LibraryInitializer() { };" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl << Qt::endl;

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
}
