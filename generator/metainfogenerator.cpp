/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
                if(!typeSystemEntry)
                    return pkgDir;
                else if(!typeSystemEntry->module().isEmpty())
                    return typeSystemEntry->module() + "/" + pkgDir;
                else if(!typeSystemEntry->qtLibrary().isEmpty())
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

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, const QString& package, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent);

void MetaInfoGenerator::writeContainerAccess(){
    //FileOut qtjambi_summary_h(QString("%1/QtJambi/qtjambi_containeraccess_all_p.h").arg(cppOutputDirectory()));
    //FileOut qtjambi_summary_cpp(QString("%1/QtJambi/qtjambi_containeraccess_all.cpp").arg(cppOutputDirectory()));
    FileOut qtjambicore_summary_h(QString("%1/QtJambiCore/qtjambi_containeraccess_core.h").arg(cppOutputDirectory()));
    FileOut qtjambicore_summary_cpp(QString("%1/QtJambiCore/qtjambi_containeraccess_core.cpp").arg(cppOutputDirectory()));
    //priGenerator->addHeader("QtJambi/generated.pri", "qtjambi_containeraccess_all_p.h");
    //priGenerator->addSource("QtJambi/generated.pri", "qtjambi_containeraccess_all.cpp");
    priGenerator->addHeader("QtJambiCore/generated.pri", "qtjambi_containeraccess_core.h");
    priGenerator->addSource("QtJambiCore/generated.pri", "qtjambi_containeraccess_core.cpp");
    //qtjambi_summary_h.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
    //qtjambi_summary_cpp.stream << "#include \"qtjambi_containeraccess_all_p.h\"" << Qt::endl << Qt::endl
    //                           << "void qtjambi_register_containeraccess_all(){" << Qt::endl;
    qtjambicore_summary_h.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
    qtjambicore_summary_cpp.stream << "#include \"qtjambi_containeraccess_core.h\"" << Qt::endl << Qt::endl
                               << "void qtjambi_register_containeraccess_core(){" << Qt::endl;

    auto insertUnique = [](QList<QPair<uint,uint>>& sizeAligns, QPair<uint,uint>&& pair){
        if(!sizeAligns.contains(pair))
            sizeAligns.append(pair);
    };

    QList<QPair<uint,uint>> sizeAligns;
    insertUnique(sizeAligns, {24,4});
    insertUnique(sizeAligns, {24,8});
    insertUnique(sizeAligns, {88,8});
    insertUnique(sizeAligns, {16,4});
    insertUnique(sizeAligns, {16,8});
    insertUnique(sizeAligns, {80,8});
    insertUnique(sizeAligns, {12,4});
    insertUnique(sizeAligns, {8,4});
    insertUnique(sizeAligns, {8,8});
    insertUnique(sizeAligns, {72,8});
    insertUnique(sizeAligns, {4,4});
    insertUnique(sizeAligns, {1,1});
    insertUnique(sizeAligns, {2,2});
    insertUnique(sizeAligns, {68,4});
    insertUnique(sizeAligns, {64,8});
    insertUnique(sizeAligns, {48,4});
    insertUnique(sizeAligns, {48,8});
    insertUnique(sizeAligns, {36,4});
    insertUnique(sizeAligns, {32,4});
    insertUnique(sizeAligns, {32,8});
    insertUnique(sizeAligns, {24,8});
    insertUnique(sizeAligns, {1,1});
    insertUnique(sizeAligns, {2,2});
    insertUnique(sizeAligns, {4,4});
    insertUnique(sizeAligns, {8,8});

    {
        QList<QString> containerTypes{"QMap", "QMultiMap"};
        for(const QString& container : containerTypes){
            QString flatTypeName = container.toLower().mid(1);
            for(QPair<uint,uint> key : sizeAligns){
                QString targetDir("QtJambiCore");
                FileOut& h = //bit1<=1 ? qtjambi_summary_h :
                                       qtjambicore_summary_h;
                FileOut& cpp = //bit1<=1 ? qtjambi_summary_cpp :
                                         qtjambicore_summary_cpp;
                QString fileName = QString("qtjambi_containeraccess_%1_%2_%3.cpp").arg(flatTypeName).arg(key.first).arg(key.second);
                FileOut f(QString("%1/%2/%3").arg(cppOutputDirectory(), targetDir, fileName));
                priGenerator->addSource(QString("%1/generated.pri").arg(targetDir), fileName);
                f.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
                f.stream << "#include <QtCore/qcompilerdetection.h>" << Qt::endl
                         << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
                         << "#include <qtjambi/qtjambi_containeraccess_" << flatTypeName << ".h>" << Qt::endl << Qt::endl
                         << "void qtjambi_register_" << flatTypeName << "_access_" << key.first << "_" << key.second << "(){" << Qt::endl;
                for(QPair<uint,uint> mapped : sizeAligns){
                    f.stream << "    BiContainerAccessFactoryHelper<" << container << "," << key.second << ", " << key.first << ", " << mapped.second << ", " << mapped.first << ">::registerContainerAccessFactory();" << Qt::endl;
                }
                f.stream << "}" << Qt::endl;
                h.stream << "void qtjambi_register_" << flatTypeName << "_access_" << key.first << "_" << key.second << "();" << Qt::endl;
                cpp.stream << "    qtjambi_register_" << flatTypeName << "_access_" << key.first << "_" << key.second << "();" << Qt::endl;
            }
        }
    }
    qtjambicore_summary_cpp.stream << "}" << Qt::endl;
    //qtjambi_summary_cpp.stream << "}" << Qt::endl;
}

void MetaInfoGenerator::writeCppFile() {
    QHash<QString, FileOut *> fileHash;
    QHash<QString, QSet<QString>> writtenClasses;

    Indentor INDENT;

    // Seems continue is not supported by our foreach loop, so
    for(AbstractMetaClass *cls : qAsConst(m_classes)) {

        FileOut *f = fileHash.value(cls->targetTypeSystem(), nullptr);
        if (f == nullptr && (m_packageGenerationPolicies.value(cls->targetTypeSystem(), 0x0) & GeneratedMetaInfo)) {
            f = new FileOut(cppOutputDirectory() + "/" + subDirectoryForClass(cls, CppDirectory) + "/" + cppFilename());

            QSet<QString> dedupe = QSet<QString>();
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_metainfo.h"), dedupe);
            //QString typeSystemName = cls->targetTypeSystem();
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
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_repository.h"), dedupe);
            f->stream << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, {}, TypeSystem::MetaInfo, CodeSnip::Position1, INDENT);
            if(cls->targetTypeSystem()=="io.qt.core" && m_qtVersionMajor>5){
                f->stream << INDENT << "void qtjambi_register_containeraccess_core();" << Qt::endl;
            }

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
                if(functional->enclosingClass()){
                    if(!(!functional->enclosingClass()->isFake()
                         && functional->enclosingClass()->typeEntry()
                         && (functional->enclosingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp))){
                        continue;
                    }
                }
                if(!(functional->typeEntry()->getUsing().isEmpty() || functional->typeEntry()->codeGeneration()==TypeEntry::GenerateCpp))
                    continue;
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
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(package));
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, {}, TypeSystem::MetaInfo, CodeSnip::Position2, INDENT);

            // Initialization function: Registers meta types
            f->stream << Qt::endl
                      << INDENT << "extern \"C\" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *, void *)" << Qt::endl
                      << INDENT << "{" << Qt::endl;
            INDENTATION(INDENT)
            f->stream << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << package << "::JNI_OnLoad(JavaVM *, void *)\")" << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, {}, TypeSystem::MetaInfo, CodeSnip::Beginning, INDENT);
        }
    }

    {
        INDENTATION(INDENT)
        writtenClasses.clear();
        for(AbstractMetaClass *cls : qAsConst(m_classes)) {
            if (FileOut *f = fileHash.value(cls->targetTypeSystem(), nullptr)) {
                if((!cls->typeEntry()->isIterator() && !cls->isFake() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0)
                        || (cls->isFake() && !cls->enums().isEmpty())){
                    if(!writtenClasses[cls->targetTypeSystem()].contains(cls->typeEntry()->qualifiedCppName())){
                        writtenClasses[cls->targetTypeSystem()] << cls->typeEntry()->qualifiedCppName();
                        f->stream << INDENT << "initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
                    }
                    for(AbstractMetaFunctional* functional : cls->functionals()){
                        if(functional->enclosingClass()){
                            if(!(!functional->enclosingClass()->isFake()
                                 && functional->enclosingClass()->typeEntry()
                                 && (functional->enclosingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp))){
                                continue;
                            }
                        }
                        if(!(functional->typeEntry()->getUsing().isEmpty() || functional->typeEntry()->codeGeneration()==TypeEntry::GenerateCpp))
                            continue;
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
                TypeSystemTypeEntry * typeSystemEntry = TypeDatabase::instance()->findTypeSystem(package);
                if(typeSystemEntry){
                    generateInitializer(f->stream, typeSystemEntry, {}, TypeSystem::MetaInfo, CodeSnip::Position3, INDENT);
                    generateInitializer(f->stream, typeSystemEntry, {}, TypeSystem::MetaInfo, CodeSnip::Position4, INDENT);
                    generateInitializer(f->stream, typeSystemEntry, {}, TypeSystem::MetaInfo, CodeSnip::End, INDENT);
                }
                if(package=="io.qt.core" && m_qtVersionMajor>5){
                    f->stream << INDENT << "qtjambi_register_containeraccess_core();" << Qt::endl;
                    writeContainerAccess();
                }
                f->stream << INDENT << "return QTJAMBI_ONLOAD_RETURN;" << Qt::endl;
            }
            f->stream << "}" << Qt::endl << Qt::endl;

            f->stream << INDENT << "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
                      << QString(package).replace("_", "_1").replace(".", "_") << "_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){" << Qt::endl
                      << "    jobject result{0};" << Qt::endl
                      << "    QTJAMBI_TRY{" << Qt::endl
                      << "        result = qtjambi_get_internal_access(env, cls);" << Qt::endl
                      << "    }QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl
                      << "        exn.raiseInJava(env);" << Qt::endl
                      << "    }QTJAMBI_TRY_END" << Qt::endl
                      << "    return result;" << Qt::endl
                      << "}" << Qt::endl << Qt::endl;
            for(const QString& pkg : m_typeSystemByPackage.keys(package)){
                if(pkg!="io.qt.internal"){
                    f->stream << INDENT << "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
                              << QString(pkg).replace("_", "_1").replace(".", "_") << "_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){" << Qt::endl
                              << "    jobject result{0};" << Qt::endl
                              << "    QTJAMBI_TRY{" << Qt::endl
                              << "        result = qtjambi_get_internal_access(env, cls);" << Qt::endl
                              << "    }QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl
                              << "        exn.raiseInJava(env);" << Qt::endl
                              << "    }QTJAMBI_TRY_END" << Qt::endl
                              << "    return result;" << Qt::endl
                              << "}" << Qt::endl << Qt::endl;
                }
            }
        }
    }

    for(QHash<QString, FileOut *>::const_key_value_iterator iter = fileHash.constKeyValueBegin(); iter!=fileHash.constKeyValueEnd(); ++iter) {
        if (FileOut *f = iter->second) {
            if (f->done())
                ++m_num_generated_written;
            ++m_num_generated;
            delete f;
        }
    }
}

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, const QString& package, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent) {
    if(typeSystemEntry){
        QStringList lines;
        for(const CodeSnip &snip : typeSystemEntry->snips[package]) {
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
        for(QString line : qAsConst(lines)) {
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
    QMap<QString,QSet<const TypeSystemTypeEntry*>> allTypeSystems;
    QHash<const TypeSystemTypeEntry*,QList<QString>> exports;
    QHash<const TypeSystemTypeEntry*,QList<QString>> requiredTypeSystems;
    QHash<const TypeSystemTypeEntry*,QList<QString>> nativeLibs;
    TypeDatabase* typeDatabase = TypeDatabase::instance();
    for(auto iter = m_packageGenerationPolicies.constKeyValueBegin(); iter!=m_packageGenerationPolicies.constKeyValueEnd(); ++iter) {

        if (iter->second & GeneratedJavaClasses) {
            const QString& package = iter->first;
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(typeDatabase->findType(package));
            QString subDir;
            QString typeSystemByPackage;
            if(typeSystemEntry)
                subDir = subDirectoryForPackage(typeSystemEntry->name(), package, JavaDirectory);
            else if(!(typeSystemByPackage = m_typeSystemByPackage[package]).isEmpty())
                subDir = subDirectoryForPackage(typeSystemByPackage, package, JavaDirectory);
            else
                continue;
            FileOut fileOut(javaOutputDirectory() + "/" + subDir + "/QtJambi_LibraryUtilities.java");

            Indentor INDENT;
            QTextStream &s = fileOut.stream;
            s << INDENT << "package " << package << ";" << Qt::endl << Qt::endl;

            QSet<QString> dedupe;
            if(typeSystemEntry){
                allTypeSystems[typeSystemEntry->module()].insert(typeSystemEntry);
                //allTypeSystems[typeSystemEntry->module().isEmpty() ? typeSystemEntry->qtLibrary() : typeSystemEntry->module()].insert(typeSystemEntry);
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
            s << INDENT << "final class QtJambi_LibraryUtilities {" << Qt::endl << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "final static int qtMajorVersion = " << m_qtVersionMajor << ";" << Qt::endl << Qt::endl
                  << INDENT << "final static int qtMinorVersion = " << m_qtVersionMinor << ";" << Qt::endl << Qt::endl
                  << INDENT << "final static int qtJambiPatch = " << m_qtjambiVersionPatch << ";" << Qt::endl;
                if(typeSystemEntry){
                    s << Qt::endl << INDENT << "static{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "try {" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            QString typeSystemID = typeSystemEntry->module();
                            if(typeSystemID.isEmpty())
                                typeSystemID = typeSystemEntry->qtLibrary();
                            if(!typeSystemID.isEmpty() && (!exports.contains(typeSystemEntry) || !exports[typeSystemEntry].contains(package)))
                                exports[typeSystemEntry] << package;
                            if(!typeSystemID.isEmpty() && !typeSystemEntry->qtLibrary().isEmpty() && (!nativeLibs.contains(typeSystemEntry) || !nativeLibs[typeSystemEntry].contains(typeSystemEntry->qtLibrary())))
                                nativeLibs[typeSystemEntry] << typeSystemEntry->qtLibrary();
                            generateInitializer(s, typeSystemEntry, {}, TypeSystem::TargetLangCode, CodeSnip::Beginning, INDENT);
                            s << INDENT << "io.qt.QtUtilities.initializePackage(\"io.qt.internal\");" << Qt::endl;
                            generateInitializer(s, typeSystemEntry, {}, TypeSystem::TargetLangCode, CodeSnip::Position1, INDENT);
                            if(typeSystemID!="qtjambi")
                                requiredTypeSystems[typeSystemEntry] << "qtjambi";

                            QSet<QString> alreadyInitializedPackages;

                            struct Analyzer{
                                static void analyzeRequiredQtLibraries(TypeDatabase* typeDatabase, QSet<QString>& alreadyInitializedPackages, QMap<QString,QStringList>& expandedQtLibraries, QList<Dependency>& requiredQtLibraries, const TypeSystemTypeEntry* ts, Dependency::Mode mode = Dependency::Mandatory, const QStringList& platforms = {}){
                                    if(ts && !alreadyInitializedPackages.contains(ts->name())){
                                        alreadyInitializedPackages.insert(ts->name());
                                        for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                            analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, expandedQtLibraries, requiredQtLibraries, entry, mode, platforms);
                                        }
                                        for(const Dependency& dep : ts->requiredQtLibraries()){
                                            if(!platforms.isEmpty() && !dep.platforms.isEmpty()){
                                                if(!QSet<QString>(platforms.begin(), platforms.end()).intersects(QSet<QString>(dep.platforms.begin(), dep.platforms.end()))){
                                                    continue;
                                                }
                                            }
                                            Dependency::Mode newMode;
                                            if(mode==Dependency::ProvideOnly){
                                                newMode = Dependency::ProvideOnly;
                                            }else if(dep.mode==Dependency::Mandatory && mode==Dependency::Mandatory){
                                                newMode = Dependency::Mandatory;
                                            }else{
                                                newMode = dep.mode;
                                            }
                                            QStringList _platforms;
                                            _platforms << platforms << dep.platforms;
                                            if(!_platforms.isEmpty())
                                                _platforms.removeDuplicates();
                                            QSet<QString> _alreadyInitializedPackages = alreadyInitializedPackages;
                                            analyzeRequiredQtLibraries(typeDatabase, _alreadyInitializedPackages, expandedQtLibraries, requiredQtLibraries, dep.entry, newMode, _platforms);
                                        }
                                        if(mode==Dependency::ProvideOnly){
                                            if(!expandedQtLibraries.contains(ts->qtLibrary())){
                                                expandedQtLibraries[ts->qtLibrary()] = platforms;
                                            }else{
                                                QStringList& _platforms = expandedQtLibraries[ts->qtLibrary()];
                                                if(!_platforms.isEmpty()){
                                                    if(platforms.isEmpty()){
                                                        _platforms.clear();
                                                    }else{
                                                        _platforms << platforms;
                                                        _platforms.removeDuplicates();
                                                    }
                                                }else{
                                                    if(platforms.isEmpty())
                                                        _platforms.clear();
                                                    else
                                                        _platforms << platforms;
                                                }
                                            }
                                        }else{
                                            bool found = false;
                                            for(Dependency& dep : requiredQtLibraries){
                                                if(dep.entry==ts->qtLibrary()){
                                                    if(mode==Dependency::Mandatory){
                                                        dep.mode = Dependency::Mandatory;
                                                    }
                                                    found = true;
                                                    break;
                                                }
                                            }
                                            if(!found)
                                                requiredQtLibraries.append(Dependency(QString(ts->qtLibrary()), mode, QStringList(platforms)));
                                        }
                                    }
                                }

                                static void analyzeRequiredQtLibraries(TypeDatabase* typeDatabase, QSet<QString>& alreadyInitializedPackages, QMap<QString,QStringList>& expandedQtLibraries, QList<Dependency>& requiredQtLibraries, const QString& lib, Dependency::Mode mode, const QStringList& platforms = {}){
                                    if(TypeSystemTypeEntry* ts = typeDatabase->typeSystemsByQtLibrary()[lib]){
                                        if(ts && !alreadyInitializedPackages.contains(ts->name())){
                                            alreadyInitializedPackages.insert(ts->name());
                                            for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                                analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, expandedQtLibraries, requiredQtLibraries, entry, mode, platforms);
                                            }
                                            for(const Dependency& dep : ts->requiredQtLibraries()){
                                                if(!platforms.isEmpty() && !dep.platforms.isEmpty()){
                                                    if(!QSet<QString>(platforms.begin(), platforms.end()).intersects(QSet<QString>(dep.platforms.begin(), dep.platforms.end()))){
                                                        continue;
                                                    }
                                                }
                                                Dependency::Mode newMode;
                                                if(mode==Dependency::ProvideOnly){
                                                    newMode = Dependency::ProvideOnly;
                                                }else if(dep.mode==Dependency::Mandatory && mode==Dependency::Mandatory){
                                                    newMode = Dependency::Mandatory;
                                                }else{
                                                    newMode = dep.mode;
                                                }
                                                QStringList _platforms;
                                                _platforms << platforms << dep.platforms;
                                                if(!_platforms.isEmpty())
                                                    _platforms.removeDuplicates();
                                                QSet<QString> _alreadyInitializedPackages = alreadyInitializedPackages;
                                                analyzeRequiredQtLibraries(typeDatabase, _alreadyInitializedPackages, expandedQtLibraries, requiredQtLibraries, dep.entry, newMode, _platforms);
                                            }
                                            if(mode==Dependency::ProvideOnly){
                                                if(!expandedQtLibraries.contains(ts->qtLibrary())){
                                                    expandedQtLibraries[ts->qtLibrary()] = platforms;
                                                }else{
                                                    QStringList& _platforms = expandedQtLibraries[ts->qtLibrary()];
                                                    if(!_platforms.isEmpty()){
                                                        if(platforms.isEmpty()){
                                                            _platforms.clear();
                                                        }else{
                                                            _platforms << platforms;
                                                            _platforms.removeDuplicates();
                                                        }
                                                    }else{
                                                        if(platforms.isEmpty())
                                                            _platforms.clear();
                                                        else
                                                            _platforms << platforms;
                                                    }
                                                }
                                            }else{
                                                bool found = false;
                                                for(Dependency& dep : requiredQtLibraries){
                                                    if(dep.entry==ts->qtLibrary() && dep.platforms==platforms){
                                                        found = true;
                                                        if(mode==Dependency::Mandatory){
                                                            dep.mode = Dependency::Mandatory;
                                                        }
                                                        break;
                                                    }
                                                }
                                                if(!found)
                                                    requiredQtLibraries.append(Dependency(QString(ts->qtLibrary()), mode, QStringList(platforms)));
                                            }
                                        }
                                    }else if(mode==Dependency::ProvideOnly){
                                        if(!expandedQtLibraries.contains(lib)){
                                            expandedQtLibraries[lib] = platforms;
                                        }else{
                                            QStringList& _platforms = expandedQtLibraries[lib];
                                            if(!_platforms.isEmpty()){
                                                if(platforms.isEmpty()){
                                                    _platforms.clear();
                                                }else{
                                                    _platforms << platforms;
                                                    _platforms.removeDuplicates();
                                                }
                                            }else{
                                                if(platforms.isEmpty())
                                                    _platforms.clear();
                                                else
                                                    _platforms << platforms;
                                            }
                                        }
                                    }else{
                                        bool found = false;
                                        for(Dependency& dep : requiredQtLibraries){
                                            if(dep.entry==lib && dep.platforms==platforms){
                                                found = true;
                                                if(mode==Dependency::Mandatory){
                                                    dep.mode = Dependency::Mandatory;
                                                }
                                                break;
                                            }
                                        }
                                        if(!found)
                                            requiredQtLibraries.append(Dependency(QString(lib), Dependency::Mandatory, QStringList(platforms)));
                                    }
                                }
                            };

                            for(const TypeSystemTypeEntry* entry : typeSystemEntry->requiredTypeSystems()){
                                if(!entry || alreadyInitializedPackages.contains(entry->name()))
                                    continue;
                                alreadyInitializedPackages.insert(entry->name());
                                if(!entry->module().isEmpty())
                                    requiredTypeSystems[typeSystemEntry] << entry->module();
                            }
                            alreadyInitializedPackages.clear();
                            requiredTypeSystems[typeSystemEntry].removeAll(typeSystemEntry->module());

                            generateInitializer(s, typeSystemEntry, {}, TypeSystem::TargetLangCode, CodeSnip::Position2, INDENT);
                            QList<Dependency> requiredQtLibraries;
                            QMap<QString,QStringList> expandedQtLibraries;
                            Analyzer::analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, expandedQtLibraries, requiredQtLibraries, typeSystemEntry);
                            expandedQtLibraries.take(typeSystemEntry->qtLibrary());
                            expandedQtLibraries.take("QtCore");
                            expandedQtLibraries.take("QtUiPlugin");
                            expandedQtLibraries.take("QtQmlIntegration");
                            for(const QString& key : expandedQtLibraries.keys()){
                                if(key.startsWith("Qt")){
                                    s << INDENT << "io.qt.QtUtilities.loadQtLibrary(\"" << key.mid(2);
                                }else{
                                    s << INDENT << "io.qt.QtUtilities.loadUtilityLibrary(\"" << key;
                                }
                                s << "\", io.qt.QtUtilities.LibraryRequirementMode.ProvideOnly";
                                for(const QString& pl : expandedQtLibraries[key]){
                                    if(!pl.isEmpty()){
                                        s << ", \"" << pl << "\"";
                                    }
                                }
                                s << ");" << Qt::endl;
                            }

                            QSet<QString> loaded{"QtCore", typeSystemEntry->qtLibrary(), "QtUiPlugin", "QtQmlIntegration"};
                            for(const Dependency& dep : qAsConst(requiredQtLibraries)){
                                if(loaded.contains(dep.entry) || m_staticLibraries.contains(dep.entry))
                                    continue;
                                QString append;
                                switch(dep.mode){
                                case Dependency::Mandatory:
                                    if(dep.entry.startsWith("Qt")){
                                        append = ", io.qt.QtUtilities.LibraryRequirementMode.Mandatory";
                                    }else{
                                        append = ", io.qt.QtUtilities.LibraryRequirementMode.Optional";
                                    }
                                    break;
                                case Dependency::Optional: append = ", io.qt.QtUtilities.LibraryRequirementMode.Optional"; break;
                                case Dependency::ProvideOnly: append = ", io.qt.QtUtilities.LibraryRequirementMode.ProvideOnly"; break;
                                }
                                bool isPlatformDependent = false;
                                for(const QString& pl : dep.platforms){
                                    if(!pl.isEmpty()){
                                        isPlatformDependent = true;
                                        append += ", \"" + pl + "\"";
                                    }
                                }
                                if(!isPlatformDependent && dep.mode==Dependency::Mandatory){
                                    loaded.insert(dep.entry);
                                }
                                if(dep.entry.startsWith("Qt")){
                                    s << INDENT << "io.qt.QtUtilities.loadQtLibrary(\"" << dep.entry.mid(2) << "\"" << append << ");" << Qt::endl;
                                }else{
                                    s << INDENT << "io.qt.QtUtilities.loadUtilityLibrary(\"" << dep.entry << "\"" << append << ");" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, {}, TypeSystem::TargetLangCode, CodeSnip::Position3, INDENT);
                            if(!typeSystemEntry->qtLibrary().isEmpty()
                                    && typeSystemEntry->qtLibrary()!="QtCore"
                                    && !m_staticLibraries.contains(typeSystemEntry->qtLibrary())){
                                if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                    s << INDENT << "io.qt.QtUtilities.loadQtLibrary(\"" << typeSystemEntry->qtLibrary().mid(2) << "\", io.qt.QtUtilities.LibraryRequirementMode.Mandatory);" << Qt::endl;
                                }else{
                                    s << INDENT << "io.qt.QtUtilities.loadUtilityLibrary(\"" << typeSystemEntry->qtLibrary() << "\", io.qt.QtUtilities.LibraryRequirementMode.Mandatory);" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, {}, TypeSystem::TargetLangCode, CodeSnip::Position4, INDENT);
                            if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                QString libName = typeSystemEntry->qtLibrary();
                                s << INDENT << "io.qt.QtUtilities.loadQtJambiLibrary(\"" << libName.mid(2) << "\");" << Qt::endl;
                            }else{
                                s << INDENT << "io.qt.QtUtilities.loadJambiLibrary(\"" << QString(package).replace(".", "_") << "\");" << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, {}, TypeSystem::TargetLangCode, CodeSnip::End, INDENT);
                        }
                        s << INDENT << "} catch(Error t) {" << Qt::endl
                          << INDENT << "    throw t;" << Qt::endl
                          << INDENT << "} catch(Throwable t) {" << Qt::endl
                          << INDENT << "    throw new ExceptionInInitializerError(t);" << Qt::endl
                          << INDENT << "}" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }else{
                    if((typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(typeSystemByPackage)))){
                        QString typeSystemID = typeSystemEntry->module();
                        if(typeSystemID.isEmpty())
                            typeSystemID = typeSystemEntry->qtLibrary();
                        if(!typeSystemID.isEmpty()
                                && (!exports.contains(typeSystemEntry) || !exports[typeSystemEntry].contains(package))
                                && package!="io.qt.internal")
                            exports[typeSystemEntry] << package;
                        if(!typeSystemID.isEmpty() && !typeSystemEntry->qtLibrary().isEmpty() && (!nativeLibs.contains(typeSystemEntry) || !nativeLibs[typeSystemEntry].contains(typeSystemEntry->qtLibrary())))
                            nativeLibs[typeSystemEntry] << typeSystemEntry->qtLibrary();

                        if(package=="io.qt.internal"){
                            s << Qt::endl
                              << INDENT << "private final static java.util.Map<String,java.util.List<NativeLibraryManager.Dependency>> dependencies = new java.util.TreeMap<>();" << Qt::endl << Qt::endl
                              << INDENT << "final static java.util.Map<String,java.util.List<NativeLibraryManager.Dependency>> getDependencies() {" << Qt::endl
                              << INDENT << "    return java.util.Collections.unmodifiableMap(dependencies);" << Qt::endl
                              << INDENT << "}" << Qt::endl << Qt::endl;
                        }

                        generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::Beginning, INDENT);
                        s << Qt::endl << INDENT << "static{" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            if(package=="io.qt.internal"){
                                for(TypeSystemTypeEntry* ts : typeDatabase->typeSystemsByQtLibrary().values()){
                                    if(!m_staticLibraries.contains(ts->qtLibrary())){
                                        QString dependencyString;
                                        QSet<QString> deps{"QtCore"};
                                        {
                                            QTextStream s(&dependencyString);
                                            s << INDENT << "dependencies.put(\"" << ts->qtLibrary() << "\", java.util.Arrays.asList(";
                                            bool isFirst = true;
                                            for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                                if(deps.contains(entry->qtLibrary()))
                                                    continue;
                                                deps.insert(entry->qtLibrary());
                                                s << Qt::endl << INDENT << "                                  ";
                                                if(!isFirst){
                                                    s << ",";
                                                }else{
                                                    s << " ";
                                                    isFirst = false;
                                                }
                                                s << "new NativeLibraryManager.Dependency(\"" << entry->qtLibrary() << "\")";
                                            }
                                            for(const Dependency& dep : ts->requiredQtLibraries()){
                                                if(deps.contains(dep.entry))
                                                    continue;
                                                deps.insert(dep.entry);
                                                s << Qt::endl << INDENT << "                                  ";
                                                if(!isFirst){
                                                    s << ",";
                                                }else{
                                                    s << " ";
                                                    isFirst = false;
                                                }
                                                QString append;
                                                switch(dep.mode){
                                                case Dependency::Mandatory: append = ", io.qt.QtUtilities.LibraryRequirementMode.Mandatory"; break;
                                                case Dependency::Optional: append = ", io.qt.QtUtilities.LibraryRequirementMode.Optional"; break;
                                                case Dependency::ProvideOnly: append = ", io.qt.QtUtilities.LibraryRequirementMode.ProvideOnly"; break;
                                                }
                                                for(const QString& pl : dep.platforms){
                                                    if(!pl.isEmpty())
                                                        append += ", \"" + pl + "\"";
                                                }
                                                QString mode;
                                                s << "new NativeLibraryManager.Dependency(\"" << dep.entry << "\"" << append << ")";
                                            }
                                            s << "));" << Qt::endl;
                                        }
                                        if(deps.size()>1)
                                            s << dependencyString;
                                    }
                                }
                                s << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::Position1, INDENT);
                            s << INDENT << "try {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::Position2, INDENT);
                                if(package!="io.qt.internal")
                                    s << INDENT << "io.qt.QtUtilities.initializePackage(\"" << typeSystemByPackage << "\");" << Qt::endl;
                                generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::Position3, INDENT);
                            }
                            s << INDENT << "} catch(Error t) {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::Position4, INDENT);
                                s << INDENT << "throw t;" << Qt::endl;
                            }
                            s << INDENT << "} catch(Throwable t) {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::Position4, INDENT);
                                s << INDENT << "throw new ExceptionInInitializerError(t);" << Qt::endl;
                            }
                            s << INDENT << "}" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                        generateInitializer(s, typeSystemEntry, package, TypeSystem::TargetLangCode, CodeSnip::End, INDENT);
                    }else if(package!="io.qt.internal"){
                        s << Qt::endl << INDENT << "static{" << Qt::endl;
                        s << INDENT << "    io.qt.QtUtilities.initializePackage(\"" << typeSystemByPackage << "\");" << Qt::endl;
                        s << INDENT << "}" << Qt::endl;
                    }
                }
                if(package!="io.qt.internal"){
                    s << INDENT << "static final io.qt.InternalAccess internal = internalAccess();" << Qt::endl << Qt::endl
                      << INDENT << "private static native io.qt.InternalAccess internalAccess();" << Qt::endl << Qt::endl;
                }
                s << INDENT << "static void initialize() { };" << Qt::endl << Qt::endl
                  << INDENT << "private QtJambi_LibraryUtilities() throws java.lang.InstantiationError { throw new java.lang.InstantiationError(\"Cannot instantiate QtJambi_LibraryUtilities.\"); }" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl << Qt::endl;

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
        const TypeSystemTypeEntry* baseTypeSystem = typeDatabase->typeSystemsByQtLibrary()["QtCore"];
        if(baseTypeSystem && nativeLibs.contains(baseTypeSystem) && !nativeLibs[baseTypeSystem].contains("QtJambi")){
            nativeLibs[baseTypeSystem].insert(0, "QtJambi");
        }

        for(const QString& moduleName : allTypeSystems.keys()){
            if(!moduleName.isEmpty() && !allTypeSystems[moduleName].isEmpty()){
                QStringList dependentModules;
                QStringList bundledLibraries;
                QString description;
                QStringList moduleExcludes;
                FileOut fileOut(javaOutputDirectory() + "/" + moduleName + "/module-info.java");
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(allTypeSystems[moduleName])){
                    for(const QString& forwardDeclaration : typeSystem->forwardDeclarations()){
                        auto idx = forwardDeclaration.lastIndexOf('/');
                        QString pkg;
                        if(idx > 0){
                            pkg = forwardDeclaration.mid(0, idx);
                        }
                        moduleExcludes << forwardDeclaration+".java";
                        moduleExcludes << forwardDeclaration+".class";
                        moduleExcludes << pkg;
                    }
                    if(!typeSystem->description().isEmpty()){
                        fileOut.stream << "/**" << Qt::endl;
                        description = typeSystem->description();
                        for(const QString& line : typeSystem->description().split("\n")){
                            fileOut.stream << " * " << line.trimmed() << Qt::endl;
                        }
                        fileOut.stream << " */" << Qt::endl;
                    }
                }
                Indentor INDENT;
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(allTypeSystems[moduleName])){
                    generateInitializer(fileOut.stream, typeSystem, {}, TypeSystem::ModuleInfo, CodeSnip::Position1, INDENT);
                }
                fileOut.stream << "module " << moduleName << " {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    for(const TypeSystemTypeEntry* typeSystem : qAsConst(allTypeSystems[moduleName])){
                        generateInitializer(fileOut.stream, typeSystem, {}, TypeSystem::ModuleInfo, CodeSnip::Beginning, INDENT);
                    }
                    fileOut.stream << "    requires java.base;" << Qt::endl;
                    for(const TypeSystemTypeEntry* typeSystem : qAsConst(allTypeSystems[moduleName])){
                        QList<QString>& entries = requiredTypeSystems[typeSystem];
                        entries.removeDuplicates();
                        generateInitializer(fileOut.stream, typeSystem, {}, TypeSystem::ModuleInfo, CodeSnip::Position2, INDENT);
                        for(const QString& e : qAsConst(entries)){
                            if(e=="qtjambi")
                                fileOut.stream << "    requires transitive " << e << ";" << Qt::endl;
                            else
                                fileOut.stream << "    requires " << e << ";" << Qt::endl;
                            dependentModules << e;
                        }
                        generateInitializer(fileOut.stream, typeSystem, {}, TypeSystem::ModuleInfo, CodeSnip::Position3, INDENT);
                        generateInitializer(fileOut.stream, typeSystem, {}, TypeSystem::ModuleInfo, CodeSnip::End, INDENT);
                        if(!typeSystem->isNoExports())
                            for(const QString& e : exports[typeSystem])
                                fileOut.stream << "    exports " << e << ";" << Qt::endl;
                    }
                }
                fileOut.stream << "}" << Qt::endl;
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(allTypeSystems[moduleName])){
                    generateInitializer(fileOut.stream, typeSystem, {}, TypeSystem::ModuleInfo, CodeSnip::Position4, INDENT);
                    if(!nativeLibs[typeSystem].isEmpty()){
                        QSet<QString> libraries;
                        for(const QString& e : qAsConst(nativeLibs[typeSystem])){
                            if(e.isEmpty() || libraries.contains(e))
                                continue;
                            libraries << e;
                            if(e.startsWith("QtJambi"))
                                bundledLibraries << e;
                            else if(!e.startsWith("Qt"))
                                bundledLibraries << "QtJambi" + e;
                            else
                                bundledLibraries << "QtJambi" + e.mid(2);
                        }
                    }
                }
                QSettings settings(javaOutputDirectory() + "/" + moduleName + "/build.properties", QSettings::IniFormat);
                settings.clear();
                settings.setValue("qtjambi.module.description", description);
                if(!dependentModules.isEmpty())
                    settings.setValue("qtjambi.required.modules", dependentModules);
                else
                    settings.setValue("qtjambi.required.modules", "");
                if(!bundledLibraries.isEmpty())
                    settings.setValue("qtjambi.bundle.libraries", bundledLibraries);
                else
                    settings.setValue("qtjambi.bundle.libraries", "");
                if(!moduleExcludes.isEmpty())
                    settings.setValue("qtjambi.jar.excludes", moduleExcludes);
                else
                    settings.setValue("qtjambi.jar.excludes", "");
            }
        }
    }
}
