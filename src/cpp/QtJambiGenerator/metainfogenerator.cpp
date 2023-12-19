/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "metainfogenerator.h"
#include "reporthandler.h"
#include "cppimplgenerator.h"
#include "bufferedoutputstream.h"

#include <QDir>
#include <QMetaType>
#include "typesystem/typedatabase.h"

MetaInfoGenerator::MetaInfoGenerator(PriGenerator *pri):
        AbstractGenerator(),
        priGenerator(pri), m_database(pri->database())

{
    setFilenameStub("metainfo");
}

void MetaInfoGenerator::generate() {
    buildSkipList();
    writeCppFile();
}

bool MetaInfoGenerator::shouldGenerate(const TypeEntry *entry) const {
    return entry != nullptr && !entry->isIterator() && !entry->isNamespace() && !entry->isEnum() && (entry->codeGeneration() & TypeEntry::GenerateCpp);
}

bool MetaInfoGenerator::shouldGenerate(const MetaClass *) const {
    return true; //(!cls->isInterface() && cls->typeEntry()->isValue() && !cls->typeEntry()->isIterator()
            //&& !cls->isAbstract() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp));
}

QString MetaInfoGenerator::fileNameForClass(const MetaClass *) const {
    return filenameStub() + ".cpp";
}

void MetaInfoGenerator::write(QTextStream &, const MetaClass *, int) {
    // not used
}

/**
 * Builds a skip list of classes that shouldn't be built.
 */
void MetaInfoGenerator::buildSkipList() {
    for(MetaClass *cls : m_classes) {
        if(cls->isFake()){
            for(MetaEnum* _cls : cls->enums()){
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
            for(MetaFunctional* _cls : cls->functionals()){
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
            for(MetaClass *_cls : cls->enclosedClasses()) {
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

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, const QString& package, TS::Language language, CodeSnip::Position pos, Indentor indent);

void MetaInfoGenerator::writeContainerAccess(){
    //FileOut qtjambi_summary_h(QString("%1/QtJambi/containeraccess_all_p.h").arg(cppOutputDirectory()));
    //FileOut qtjambi_summary_cpp(QString("%1/QtJambi/containeraccess_all.cpp").arg(cppOutputDirectory()));
    BufferedOutputStream qtjambicore_summary_h(QFileInfo(QString("%1/QtJambiCore/containeraccess_core.h").arg(cppOutputDirectory())));
    BufferedOutputStream qtjambicore_summary_cpp(QFileInfo(QString("%1/QtJambiCore/containeraccess_core.cpp").arg(cppOutputDirectory())));
    //priGenerator->addHeader("QtJambi/generated.pri", "containeraccess_all_p.h");
    //priGenerator->addSource("QtJambi/generated.pri", "containeraccess_all.cpp");
    priGenerator->addHeader("QtJambiCore/generated.pri", "containeraccess_core.h");
    priGenerator->addSource("QtJambiCore/generated.pri", "containeraccess_core.cpp");
    //qtjambi_summary_h << "#include <QtCore/QtGlobal>" << Qt::endl;
    //qtjambi_summary_cpp << "#include \"containeraccess_all_p.h\"" << Qt::endl << Qt::endl
    //                           << "void register_containeraccess_all(){" << Qt::endl;
    qtjambicore_summary_h << "#include <QtCore/QtGlobal>" << Qt::endl;
    qtjambicore_summary_cpp << "#include \"containeraccess_core.h\"" << Qt::endl << Qt::endl
                               << "void register_containeraccess_core(){" << Qt::endl;

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
                BufferedOutputStream& h = //bit1<=1 ? qtjambi_summary_h :
                                       qtjambicore_summary_h;
                BufferedOutputStream& cpp = //bit1<=1 ? qtjambi_summary_cpp :
                                         qtjambicore_summary_cpp;
                QString fileName = QString("containeraccess_%1_%2_%3.cpp").arg(flatTypeName).arg(key.first).arg(key.second);
                BufferedOutputStream f(QFileInfo(QString("%1/%2/%3").arg(cppOutputDirectory(), targetDir, fileName)));
                priGenerator->addSource(QString("%1/generated.pri").arg(targetDir), fileName);
                f << "#include <QtCore/QtGlobal>" << Qt::endl;
                f << "#include <QtCore/qcompilerdetection.h>" << Qt::endl
                         << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
                         << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl
                         << "#include <QtJambi/containeraccess_" << flatTypeName << ".h>" << Qt::endl << Qt::endl
                         << "void register_" << flatTypeName << "_access_" << key.first << "_" << key.second << "(){" << Qt::endl;
                for(QPair<uint,uint> mapped : sizeAligns){
                    f << "    ContainerAccessAPI::AssociativeContainerAccessFactoryHelper<" << container << "," << key.second << ", " << key.first << ", " << mapped.second << ", " << mapped.first << ">::registerContainerAccessFactory();" << Qt::endl;
                }
                f << "}" << Qt::endl;
                h << "void register_" << flatTypeName << "_access_" << key.first << "_" << key.second << "();" << Qt::endl;
                cpp << "    register_" << flatTypeName << "_access_" << key.first << "_" << key.second << "();" << Qt::endl;
            }
        }
    }
    qtjambicore_summary_cpp << "}" << Qt::endl;
    //qtjambi_summary_cpp << "}" << Qt::endl;
}

void MetaInfoGenerator::setIncludeDirectories(const QStringList &newIncludeDirectories)
{
    m_includeDirectories = newIncludeDirectories;
}

void MetaInfoGenerator::writeCppFile() {
    QMap<QString, BufferedOutputStream *> buffers;
    QMap<QString, QSet<QString>> writtenClasses;

    Indentor INDENT;

    // Seems continue is not supported by our foreach loop, so
    for(MetaClass *cls : qAsConst(m_classes)) {

        BufferedOutputStream *buffer = buffers.value(cls->targetTypeSystem(), nullptr);
        if (buffer == nullptr && (m_packageGenerationPolicies.value(cls->targetTypeSystem(), 0x0) & GeneratedMetaInfo)) {
            buffer = new BufferedOutputStream(QFileInfo(cppOutputDirectory() + "/" + priGenerator->subDirectoryForClass(cls, PriGenerator::CppDirectory) + "/" + cppFilename()));
            QTextStream& stream  = *buffer;
            buffers.insert(cls->targetTypeSystem(), buffer);

            QSet<QString> includes;
            writeInclude(stream, Include(Include::IncludePath, "QtJambi/MetaInfo"), includes);
            //QString typeSystemName = cls->targetTypeSystem();
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(cls->targetTypeSystem()));
            if(typeSystemEntry){
                if(typeSystemEntry->include().type!=Include::TargetLangImport){
                    writeInclude(stream, typeSystemEntry->include(), includes);
                }
                for(const Include& incl : typeSystemEntry->extraIncludes()){
                    if(incl.type!=Include::TargetLangImport){
                        writeInclude(stream, incl, includes);
                    }
                }
            }
            stream << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(stream, typeSystemEntry, {}, TS::MetaInfo, CodeSnip::Position1, INDENT);
            if(cls->targetTypeSystem()=="io.qt.core" && m_qtVersionMajor>5){
                stream << INDENT << "void register_containeraccess_core();" << Qt::endl;
            }

            QString pro_file_name = priGenerator->subDirectoryForClass(cls, PriGenerator::CppDirectory) + "/generated.pri";
            priGenerator->addSource(pro_file_name, cppFilename());
        }

        if (buffer != nullptr) {
            QTextStream& stream  = *buffer;
            if((!cls->typeEntry()->isIterator() && !cls->isFake() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0)
                    || (cls->isFake() && !cls->enums().isEmpty())){
                if(!writtenClasses[cls->targetTypeSystem()].contains(cls->typeEntry()->qualifiedCppName())){
                    writtenClasses[cls->targetTypeSystem()] << cls->typeEntry()->qualifiedCppName();
                    stream << INDENT << "void initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
                }
            }
            for(MetaFunctional* functional : cls->functionals()){
                if(functional->enclosingClass()){
                    if(!(!functional->enclosingClass()->isFake()
                         && functional->enclosingClass()->typeEntry()
                         && (functional->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                         && (functional->enclosingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp))){
                        continue;
                    }
                }
                if(!(functional->typeEntry()->getUsing().isEmpty() || functional->typeEntry()->codeGeneration()==TypeEntry::GenerateCpp))
                    continue;
                stream << INDENT << "void initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
            }
        }
    }

    // Primitive types must be added to all packages, in case the other packages are
    // not referenced from the generated code.
    for(QMap<QString, BufferedOutputStream *>::const_key_value_iterator iter = buffers.constKeyValueBegin(); iter!=buffers.constKeyValueEnd(); ++iter) {
        const QString& package = iter->first;
        BufferedOutputStream *f = iter->second;
        if (f != nullptr) {
            QTextStream& stream  = *f;
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(package));
            if(typeSystemEntry)
                generateInitializer(stream, typeSystemEntry, {}, TS::MetaInfo, CodeSnip::Position2, INDENT);

            // Initialization function: Registers meta types
            stream << Qt::endl
                      << INDENT << "extern \"C\" Q_DECL_EXPORT jint JNICALL JNI_ONLOAD(JavaVM *, void *)" << Qt::endl
                      << INDENT << "{" << Qt::endl;
            INDENTATION(INDENT)
            stream << INDENT << "QTJAMBI_LIBRARY_INITIALIZATION_METHOD_CALL(\"" << package << "\")" << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(stream, typeSystemEntry, {}, TS::MetaInfo, CodeSnip::Beginning, INDENT);
        }
    }

    {
        INDENTATION(INDENT)
        writtenClasses.clear();
        for(MetaClass *cls : qAsConst(m_classes)) {
            if (BufferedOutputStream *f = buffers.value(cls->targetTypeSystem(), nullptr)) {
                QTextStream& stream  = *f;
                if((!cls->typeEntry()->isIterator() && !cls->isFake() && (cls->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp) != 0)
                        || (cls->isFake() && !cls->enums().isEmpty())){
                    if(!writtenClasses[cls->targetTypeSystem()].contains(cls->typeEntry()->qualifiedCppName())){
                        writtenClasses[cls->targetTypeSystem()] << cls->typeEntry()->qualifiedCppName();
                        stream << INDENT << "initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "();" << Qt::endl;
                    }
                    for(MetaFunctional* functional : cls->functionals()){
                        if(functional->enclosingClass()){
                            if(!(!functional->enclosingClass()->isFake()
                                 && functional->enclosingClass()->typeEntry()
                                 && (functional->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                                 && (functional->enclosingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp))){
                                continue;
                            }
                        }
                        if(!(functional->typeEntry()->getUsing().isEmpty() || functional->typeEntry()->codeGeneration()==TypeEntry::GenerateCpp))
                            continue;
                        stream << INDENT << "initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_") << "();" << Qt::endl;
                    }
                }
            }
        }
    }

    for(QMap<QString, BufferedOutputStream *>::const_key_value_iterator iter = buffers.constKeyValueBegin(); iter!=buffers.constKeyValueEnd(); ++iter) {
        const QString& package = iter->first;
        BufferedOutputStream *f = iter->second;
        if (f != nullptr) {
            QTextStream& stream  = *f;
            {
                INDENTATION(INDENT)
                TypeSystemTypeEntry * typeSystemEntry = m_database->findTypeSystem(package);
                if(typeSystemEntry){
                    generateInitializer(stream, typeSystemEntry, {}, TS::MetaInfo, CodeSnip::Position3, INDENT);
                    generateInitializer(stream, typeSystemEntry, {}, TS::MetaInfo, CodeSnip::Position4, INDENT);
                    generateInitializer(stream, typeSystemEntry, {}, TS::MetaInfo, CodeSnip::End, INDENT);
                }
                if(package=="io.qt.core" && m_qtVersionMajor>5){
                    stream << INDENT << "register_containeraccess_core();" << Qt::endl;
                    writeContainerAccess();
                }
                stream << INDENT << "return QTJAMBI_ONLOAD_RETURN;" << Qt::endl;
            }
            stream << "}" << Qt::endl << Qt::endl;

            stream << INDENT << "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
                      << QString(package).replace("_", "_1").replace(".", "_") << "_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){" << Qt::endl
                      << "    jobject result{0};" << Qt::endl
                      << "    QTJAMBI_TRY{" << Qt::endl
                      << "        result = MetaInfoAPI::internalAccess(env, cls);" << Qt::endl
                      << "    }QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl
                      << "        exn.raiseInJava(env);" << Qt::endl
                      << "    }QTJAMBI_TRY_END" << Qt::endl
                      << "    return result;" << Qt::endl
                      << "}" << Qt::endl << Qt::endl;
            for(const QString& pkg : m_typeSystemByPackage.keys(package)){
                if(pkg!="io.qt.internal"){
                    stream << INDENT << "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
                              << QString(pkg).replace("_", "_1").replace(".", "_") << "_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){" << Qt::endl
                              << "    jobject result{0};" << Qt::endl
                              << "    QTJAMBI_TRY{" << Qt::endl
                              << "        result = MetaInfoAPI::internalAccess(env, cls);" << Qt::endl
                              << "    }QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl
                              << "        exn.raiseInJava(env);" << Qt::endl
                              << "    }QTJAMBI_TRY_END" << Qt::endl
                              << "    return result;" << Qt::endl
                              << "}" << Qt::endl << Qt::endl;
                }
            }
        }
    }

    for(QMap<QString, BufferedOutputStream *>::const_key_value_iterator iter = buffers.constKeyValueBegin(); iter!=buffers.constKeyValueEnd(); ++iter) {
        if (BufferedOutputStream *f = iter->second) {
            if (f->finish())
                ++m_num_generated_written;
            ++m_num_generated;
            delete f;
        }
    }
}

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, const QString& package, TS::Language language, CodeSnip::Position pos, Indentor indent) {
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
    for(auto iter = m_packageGenerationPolicies.constKeyValueBegin(); iter!=m_packageGenerationPolicies.constKeyValueEnd(); ++iter) {

        if (iter->second & GeneratedJavaClasses) {
            const QString& package = iter->first;
            TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(package));
            QString subDir;
            QString typeSystemByPackage;
            if(typeSystemEntry)
                subDir = priGenerator->subDirectoryForPackage(typeSystemEntry->name(), package, PriGenerator::JavaDirectory);
            else if(!(typeSystemByPackage = m_typeSystemByPackage[package]).isEmpty())
                subDir = priGenerator->subDirectoryForPackage(typeSystemByPackage, package, PriGenerator::JavaDirectory);
            else
                continue;
            BufferedOutputStream s(QFileInfo(javaOutputDirectory() + "/" + subDir + "/QtJambi_LibraryUtilities.java"));

            Indentor INDENT;
            s << INDENT << "package " << package << ";" << Qt::endl << Qt::endl;

            QSet<QString> imports;
            imports.insert("io.qt.*");
            s << "import io.qt.*;" << Qt::endl;
            imports.insert("static io.qt.QtUtilities.*");
            s << "import static io.qt.QtUtilities.*;" << Qt::endl;
            if(package=="io.qt.internal"){
                imports.insert("java.util.*");
                s << "import java.util.*;" << Qt::endl;
                imports.insert("static java.util.Arrays.*");
                s << "import static java.util.Arrays.*;" << Qt::endl;
                imports.insert("static java.util.Collections.*");
                s << "import static java.util.Collections.*;" << Qt::endl;
            }

            if(typeSystemEntry){
                allTypeSystems[typeSystemEntry->module()].insert(typeSystemEntry);
                //allTypeSystems[typeSystemEntry->module().isEmpty() ? typeSystemEntry->qtLibrary() : typeSystemEntry->module()].insert(typeSystemEntry);
                if(typeSystemEntry->include().type==Include::TargetLangImport){
                    writeInclude(s, typeSystemEntry->include(), imports);
                }
                for(const Include& incl : typeSystemEntry->extraIncludes()){
                    if(incl.type==Include::TargetLangImport){
                        writeInclude(s, incl, imports);
                    }
                }
            }
            if(!imports.isEmpty()){
                s << Qt::endl;
            }
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * @hidden" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "final class QtJambi_LibraryUtilities {" << Qt::endl << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "final static int qtMajorVersion = " << m_qtVersionMajor << ";" << Qt::endl << Qt::endl
                  << INDENT << "final static int qtMinorVersion = " << m_qtVersionMinor << ";" << Qt::endl << Qt::endl
                  << INDENT << "final static int qtJambiPatch = " << m_qtjambiVersionPatch << ";" << Qt::endl;
                if(typeSystemEntry){
                    s << Qt::endl << INDENT << "final static InternalAccess internal;" << Qt::endl
                      << Qt::endl << INDENT << "static{" << Qt::endl;
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
                            generateInitializer(s, typeSystemEntry, {}, TS::TargetLangCode, CodeSnip::Beginning, INDENT);
                            if(package=="io.qt.core"){
                                s << INDENT << "initializePackage(\"io.qt.internal\");" << Qt::endl;
                            }else{
                                s << INDENT << "initializePackage(\"io.qt.core\");" << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, {}, TS::TargetLangCode, CodeSnip::Position1, INDENT);
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
                                                    if(!dep.platforms.isEmpty()){
                                                        if(platforms.isEmpty())
                                                            dep.platforms.clear();
                                                        else{
                                                            QStringList _platforms;
                                                            _platforms << platforms << dep.platforms;
                                                            if(!_platforms.isEmpty())
                                                                _platforms.removeDuplicates();
                                                            dep.platforms = _platforms;
                                                        }
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

                            generateInitializer(s, typeSystemEntry, {}, TS::TargetLangCode, CodeSnip::Position2, INDENT);
                            QList<Dependency> requiredQtLibraries;
                            QMap<QString,QStringList> expandedQtLibraries;
                            Analyzer::analyzeRequiredQtLibraries(m_database, alreadyInitializedPackages, expandedQtLibraries, requiredQtLibraries, typeSystemEntry);
                            expandedQtLibraries.take(typeSystemEntry->qtLibrary());
                            expandedQtLibraries.take("QtCore");
                            expandedQtLibraries.take("QtUiPlugin");
                            expandedQtLibraries.take("QtQmlIntegration");
                            for(const QString& key : expandedQtLibraries.keys()){
                                if(key.startsWith("Qt")){
                                    s << INDENT << "loadQtLibrary(\"" << key.mid(2);
                                }else{
                                    s << INDENT << "loadUtilityLibrary(\"" << key;
                                }
                                s << "\", LibraryRequirementMode.ProvideOnly";
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
                                        append = ", LibraryRequirementMode.Mandatory";
                                    }else{
                                        append = ", LibraryRequirementMode.Optional";
                                    }
                                    break;
                                case Dependency::Optional: append = ", LibraryRequirementMode.Optional"; break;
                                case Dependency::ProvideOnly: append = ", LibraryRequirementMode.ProvideOnly"; break;
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
                                    s << INDENT << "loadQtLibrary(\"" << dep.entry.mid(2) << "\"" << append << ");" << Qt::endl;
                                }else{
                                    s << INDENT << "loadUtilityLibrary(\"" << dep.entry << "\"" << append << ");" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, {}, TS::TargetLangCode, CodeSnip::Position3, INDENT);
                            if(!typeSystemEntry->qtLibrary().isEmpty()
                                    && typeSystemEntry->qtLibrary()!="QtCore"
                                    && !m_staticLibraries.contains(typeSystemEntry->qtLibrary())){
                                if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                    s << INDENT << "loadQtLibrary(\"" << typeSystemEntry->qtLibrary().mid(2) << "\", LibraryRequirementMode.Mandatory);" << Qt::endl;
                                }else{
                                    s << INDENT << "loadUtilityLibrary(\"" << typeSystemEntry->qtLibrary() << "\", LibraryRequirementMode.Mandatory);" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, {}, TS::TargetLangCode, CodeSnip::Position4, INDENT);
                            if(typeSystemEntry->qtLibrary().startsWith("Qt")){
                                QString libName = typeSystemEntry->qtLibrary();
                                s << INDENT << "loadQtJambiLibrary(\"" << libName.mid(2) << "\");" << Qt::endl;
                            }else if(!typeSystemEntry->targetName().isEmpty()){
                                if(typeSystemEntry->targetName().startsWith("QtJambi"))
                                    s << INDENT << "loadQtJambiLibrary(\"" << typeSystemEntry->targetName().mid(7) << "\");" << Qt::endl;
                                else
                                    s << INDENT << "loadJambiLibrary(\"" << typeSystemEntry->targetName() << "\");" << Qt::endl;
                            }else{
                                s << INDENT << "loadJambiLibrary(\"" << QString(package).replace(".", "_") << "\");" << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position5, INDENT);
                        }
                        s << INDENT << "} catch(Error t) {" << Qt::endl
                          << INDENT << "    throw t;" << Qt::endl
                          << INDENT << "} catch(Throwable t) {" << Qt::endl
                          << INDENT << "    throw new ExceptionInInitializerError(t);" << Qt::endl
                          << INDENT << "}" << Qt::endl;
                        if(package!="io.qt.internal")
                            s << INDENT << "internal = internalAccess();" << Qt::endl;
                        generateInitializer(s, typeSystemEntry, {}, TS::TargetLangCode, CodeSnip::End, INDENT);
                    }
                    s << INDENT << "}" << Qt::endl;
                }else{
                    if((typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(typeSystemByPackage)))){
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
                              << INDENT << "final static Map<String,List<Dependency>> dependencies;" << Qt::endl << Qt::endl;
                        }else{
                            s << INDENT << "final static InternalAccess internal;" << Qt::endl << Qt::endl;
                        }

                        generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Beginning, INDENT);
                        s << Qt::endl << INDENT << "static{" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            if(package=="io.qt.internal"){
                                s << INDENT << "Map<String,List<Dependency>> _dependencies = new TreeMap<>();" << Qt::endl;
                                const QList<TypeSystemTypeEntry*> typeSystems = m_database->typeSystems();
                                for(TypeSystemTypeEntry* ts : typeSystems){
                                    if(ts->codeGeneration()!=TypeEntry::GenerateNothing && !m_staticLibraries.contains(ts->qtLibrary())){
                                        QString dependencyString;
                                        QSet<QString> deps{"QtCore"};
                                        {
                                            QTextStream s(&dependencyString);
                                            s << INDENT << "_dependencies.put(\"" << ts->qtLibrary() << "\", unmodifiableList(asList(";
                                            bool isFirst = true;
                                            for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                                if(entry->qtLibrary().isEmpty() || deps.contains(entry->qtLibrary()))
                                                    continue;
                                                deps.insert(entry->qtLibrary());
                                                s << Qt::endl << INDENT << "                                  ";
                                                if(!isFirst){
                                                    s << ",";
                                                }else{
                                                    s << " ";
                                                    isFirst = false;
                                                }
                                                s << "new Dependency(\"" << entry->qtLibrary() << "\")";
                                            }
                                            for(const Dependency& dep : ts->requiredQtLibraries()){
                                                if(dep.entry.isEmpty() || deps.contains(dep.entry))
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
                                                case Dependency::Mandatory: append = ", LibraryRequirementMode.Mandatory"; break;
                                                case Dependency::Optional: append = ", LibraryRequirementMode.Optional"; break;
                                                case Dependency::ProvideOnly: append = ", LibraryRequirementMode.ProvideOnly"; break;
                                                }
                                                for(const QString& pl : dep.platforms){
                                                    if(!pl.isEmpty())
                                                        append += ", \"" + pl + "\"";
                                                }
                                                s << "new Dependency(\"" << dep.entry << "\"" << append << ")";
                                            }
                                            s << ")));" << Qt::endl;
                                        }
                                        if(deps.size()>1)
                                            s << dependencyString;
                                    }
                                }
                                s << Qt::endl << INDENT << "dependencies = unmodifiableMap(_dependencies);" << Qt::endl;
                            }
                            generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position1, INDENT);
                            s << INDENT << "try {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position2, INDENT);
                                if(package!="io.qt.internal")
                                    s << INDENT << "initializePackage(\"" << typeSystemByPackage << "\");" << Qt::endl;
                                generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position3, INDENT);
                            }
                            s << INDENT << "} catch(Error t) {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position4, INDENT);
                                s << INDENT << "throw t;" << Qt::endl;
                            }
                            s << INDENT << "} catch(Throwable t) {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position4, INDENT);
                                s << INDENT << "throw new ExceptionInInitializerError(t);" << Qt::endl;
                            }
                            s << INDENT << "}" << Qt::endl;
                        }
                        if(package!="io.qt.internal")
                            s << INDENT << "    internal = internalAccess();" << Qt::endl;
                        generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::Position5, INDENT);
                        s << INDENT << "}" << Qt::endl;
                        generateInitializer(s, typeSystemEntry, package, TS::TargetLangCode, CodeSnip::End, INDENT);
                    }else if(package!="io.qt.internal"){
                        s << INDENT << "static final InternalAccess internal;" << Qt::endl << Qt::endl
                          << INDENT << "static{" << Qt::endl
                          << INDENT << "    initializePackage(\"" << typeSystemByPackage << "\");" << Qt::endl
                          << INDENT << "    internal = internalAccess();" << Qt::endl
                          << INDENT << "}" << Qt::endl;
                    }
                }
                if(package!="io.qt.internal"){
                    s << INDENT << "private static native InternalAccess internalAccess();" << Qt::endl << Qt::endl;
                }
                s << INDENT << "static void initialize() { };" << Qt::endl << Qt::endl
                  << INDENT << "private QtJambi_LibraryUtilities() throws InstantiationError { throw new InstantiationError(\"Cannot instantiate QtJambi_LibraryUtilities.\"); }" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl << Qt::endl;

            if (s.finish())
                ++m_num_generated_written;
            ++m_num_generated;
        }
        const TypeSystemTypeEntry* baseTypeSystem = m_database->typeSystemsByQtLibrary()["QtCore"];
        if(baseTypeSystem && nativeLibs.contains(baseTypeSystem) && !nativeLibs[baseTypeSystem].contains("QtJambi")){
            nativeLibs[baseTypeSystem].insert(0, "QtJambi");
        }

        for(const QString& moduleName : allTypeSystems.keys()){
            if(!moduleName.isEmpty() && !allTypeSystems[moduleName].isEmpty()){
                QStringList dependentModules;
                QStringList bundledLibraries;
                QString description;
                QStringList moduleExcludes;
                BufferedOutputStream stream(QFileInfo(javaOutputDirectory() + "/" + moduleName + "/module-info.java"));
                QList<const TypeSystemTypeEntry*> sortedTypeSystems;
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(allTypeSystems[moduleName]))
                    sortedTypeSystems << typeSystem;
                std::sort(sortedTypeSystems.begin(), sortedTypeSystems.end(), [](const TypeSystemTypeEntry*a, const TypeSystemTypeEntry*b)->bool{return a->name()<b->name();});
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(sortedTypeSystems)){
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
                        stream << "/**" << Qt::endl;
                        description = typeSystem->description();
                        for(const QString& line : typeSystem->description().split("\n")){
                            stream << " * " << line.trimmed() << Qt::endl;
                        }
                        stream << " */" << Qt::endl;
                    }
                }
                Indentor INDENT;
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(sortedTypeSystems)){
                    generateInitializer(stream, typeSystem, {}, TS::ModuleInfo, CodeSnip::Position1, INDENT);
                }
                stream << "module " << moduleName << " {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    for(const TypeSystemTypeEntry* typeSystem : qAsConst(sortedTypeSystems)){
                        generateInitializer(stream, typeSystem, {}, TS::ModuleInfo, CodeSnip::Beginning, INDENT);
                    }
                    stream << "    requires java.base;" << Qt::endl;
                    QStringList myExports;
                    for(const TypeSystemTypeEntry* typeSystem : qAsConst(sortedTypeSystems)){
                        QList<QString>& entries = requiredTypeSystems[typeSystem];
                        entries.removeDuplicates();
                        generateInitializer(stream, typeSystem, {}, TS::ModuleInfo, CodeSnip::Position2, INDENT);
                        for(const QString& e : qAsConst(entries)){
                            if(e=="qtjambi")
                                stream << "    requires transitive " << e << ";" << Qt::endl;
                            else
                                stream << "    requires " << e << ";" << Qt::endl;
                            dependentModules << e;
                        }
                        generateInitializer(stream, typeSystem, {}, TS::ModuleInfo, CodeSnip::Position3, INDENT);
                        generateInitializer(stream, typeSystem, {}, TS::ModuleInfo, CodeSnip::End, INDENT);
                        if(!typeSystem->isNoExports())
                            myExports << exports[typeSystem];
                    }
                    std::sort(myExports.begin(), myExports.end());
                    for(const QString& e : qAsConst(myExports))
                        stream << "    exports " << e << ";" << Qt::endl;
                }
                stream << "}" << Qt::endl;
                for(const TypeSystemTypeEntry* typeSystem : qAsConst(sortedTypeSystems)){
                    generateInitializer(stream, typeSystem, {}, TS::ModuleInfo, CodeSnip::Position4, INDENT);
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
                    if(!typeSystem->targetName().isEmpty()){
                        bundledLibraries << typeSystem->targetName();
                        bundledLibraries.removeDuplicates();
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

                const QList<TypeSystemTypeEntry*> typeSystems = m_database->typeSystems();
                for(TypeSystemTypeEntry* ts : typeSystems){
                    if(ts
                            && ts->codeGeneration()!=TypeEntry::GenerateForSubclass
                            && ts->codeGeneration()!=TypeEntry::GenerateNothing){
                        QString qtJambiLibrary;
                        if(ts->qtLibrary().isEmpty()){
                            qtJambiLibrary = ts->targetName();
                        }else{
                            qtJambiLibrary = ts->qtLibrary();
                            if(qtJambiLibrary.startsWith("Qt") && !qtJambiLibrary.startsWith("QtJambi")){
                                qtJambiLibrary = "QtJambi" + qtJambiLibrary.mid(2);
                            }
                        }
                        if(!qtJambiLibrary.isEmpty()){
                            {
                                BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/" + qtJambiLibrary + "/" + qtJambiLibrary + "Depends"));
                                stream << "/* This file was generated by QtJambiGenerator. */" << Qt::endl
                                       << "#ifdef __cplusplus" << Qt::endl;
                                QSet<QString> deps;
                                for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                    if(entry->qtLibrary().isEmpty() || deps.contains(entry->qtLibrary()))
                                        continue;
                                    deps.insert(entry->qtLibrary());
                                    stream << "#include <" << entry->qtLibrary() << "/" << entry->qtLibrary() << ">" << Qt::endl;
                                }
                                for(const QString& lib : qAsConst(nativeLibs[ts])){
                                    if(lib.isEmpty() || deps.contains(lib))
                                        continue;
                                    deps.insert(lib);
                                    stream << "#include <" << lib << "/" << lib << ">" << Qt::endl;
                                }
                                stream << "#include <QtJambi/QtJambi>" << Qt::endl
                                       << "#endif" << Qt::endl;
                            }
                            {
                                BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/" + qtJambiLibrary + "/" + qtJambiLibrary + "Version"));
                                stream << "#include \"version.h\"" << Qt::endl;
                            }
                            {
                                BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/" + qtJambiLibrary + "/version.h"));
                                uint v = ((0x0FF & m_qtVersionMajor) << 16) | ((0x0FF & m_qtVersionMinor) << 8) | (0x0FF & m_qtjambiVersionPatch);
                                QString version = QString::number(v, 16);
                                while(version.length()<6)
                                    version.prepend('0');
                                stream << "/* This file was generated by QtJambiGenerator. */" << Qt::endl
                                       << "#ifndef " << qtJambiLibrary.toUpper() << "_VERSION_H" << Qt::endl
                                       << "#define " << qtJambiLibrary.toUpper() << "_VERSION_H" << Qt::endl
                                       << Qt::endl
                                       << "#define " << qtJambiLibrary.toUpper() << "_VERSION_STR \"" << QVersionNumber(m_qtVersionMajor, m_qtVersionMinor, m_qtjambiVersionPatch).toString() << "\"" << Qt::endl
                                       << Qt::endl
                                       << "#define " << qtJambiLibrary.toUpper() << "_VERSION 0x" << version << Qt::endl
                                       << Qt::endl
                                       << "#endif // " << qtJambiLibrary.toUpper() << "_VERSION_H" << Qt::endl;
                            }
                            {
                                BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/" + qtJambiLibrary + "/" + qtJambiLibrary));
                                stream << "/* This file was generated by QtJambiGenerator. */" << Qt::endl
                                       << "#ifndef " << qtJambiLibrary.toUpper() << "_MODULE_H" << Qt::endl
                                       << "#define " << qtJambiLibrary.toUpper() << "_MODULE_H" << Qt::endl
                                       << Qt::endl
                                       << "#include \"version.h\"" << Qt::endl;
                                for(const QString& directory : m_includeDirectories){
                                    QDir dir(directory);
                                    if(dir.exists(qtJambiLibrary) && dir.cd(qtJambiLibrary)){
                                        for(const QString& entry : dir.entryList(QDir::Files)){
                                            if(entry.endsWith(".h") && !entry.endsWith("_p.h") && !entry.endsWith("_shell.h")){
                                                stream << "#include \"" << entry << "\"" << Qt::endl;
                                            }
                                        }
                                    }
                                }
                                stream << "#include <" << qtJambiLibrary << "/" << qtJambiLibrary << "Depends>" << Qt::endl
                                       << Qt::endl
                                       << "#endif // " << qtJambiLibrary.toUpper() << "_MODULE_H" << Qt::endl;
                            }
                        }
                    }
                }

                if(baseTypeSystem && baseTypeSystem->codeGeneration()!=TypeEntry::GenerateNothing){
                    {
                        BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/QtJambi/QtJambiDepends"));
                        stream << "/* This file was generated by QtJambiGenerator. */" << Qt::endl
                               << "#ifdef __cplusplus" << Qt::endl
                               << "#include <QtCore/QtCore>" << Qt::endl
                               << "#endif" << Qt::endl;
                    }
                    {
                        BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/QtJambi/QtJambiVersion"));
                        stream << "#include \"version.h\"" << Qt::endl;
                    }
                    {
                        BufferedOutputStream stream(QFileInfo(cppOutputDirectory() + "/QtJambi/version.h"));
                        uint v = ((0x0FF & m_qtVersionMajor) << 16) | ((0x0FF & m_qtVersionMinor) << 8) | (0x0FF & m_qtjambiVersionPatch);
                        QString version = QString::number(v, 16);
                        while(version.length()<6)
                            version.prepend('0');
                        stream << "/* This file was generated by QtJambiGenerator. */" << Qt::endl
                               << "#ifndef QTJAMBI_VERSION_H" << Qt::endl
                               << "#define QTJAMBI_VERSION_H" << Qt::endl
                               << Qt::endl
                               << "#define QTJAMBI_VERSION_STR \"" << QVersionNumber(m_qtVersionMajor, m_qtVersionMinor, m_qtjambiVersionPatch).toString() << "\"" << Qt::endl
                               << Qt::endl
                               << "#define QTJAMBI_VERSION 0x" << version << "\n" << Qt::endl
                               << Qt::endl
                               << "#endif // QTJAMBI_VERSION_H" << Qt::endl;
                    }
                }
            }
        }
    }
}
