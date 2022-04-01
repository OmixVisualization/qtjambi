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

static void generateInitializer(QTextStream &s, const TypeSystemTypeEntry * typeSystemEntry, TypeSystem::Language language, CodeSnip::Position pos, Indentor indent);

void MetaInfoGenerator::writeContainerAccess(){
    FileOut qtjambi_summary_h(QString("%1/QtJambi/qtjambi_containeraccess_all_p.h").arg(cppOutputDirectory()));
    FileOut qtjambi_summary_cpp(QString("%1/QtJambi/qtjambi_containeraccess_all.cpp").arg(cppOutputDirectory()));
    FileOut qtjambicore_summary_h(QString("%1/QtJambiCore/qtjambi_containeraccess_core.h").arg(cppOutputDirectory()));
    FileOut qtjambicore_summary_cpp(QString("%1/QtJambiCore/qtjambi_containeraccess_core.cpp").arg(cppOutputDirectory()));
    priGenerator->addHeader("QtJambi/generated.pri", "qtjambi_containeraccess_all_p.h");
    priGenerator->addSource("QtJambi/generated.pri", "qtjambi_containeraccess_all.cpp");
    priGenerator->addHeader("QtJambiCore/generated.pri", "qtjambi_containeraccess_core.h");
    priGenerator->addSource("QtJambiCore/generated.pri", "qtjambi_containeraccess_core.cpp");
    qtjambi_summary_h.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
    qtjambi_summary_cpp.stream << "#include \"qtjambi_containeraccess_all_p.h\"" << Qt::endl << Qt::endl
                               << "void qtjambi_register_containeraccess_all(){" << Qt::endl;
    qtjambicore_summary_h.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
    qtjambicore_summary_cpp.stream << "#include \"qtjambi_containeraccess_core.h\"" << Qt::endl << Qt::endl
                               << "void qtjambi_register_containeraccess_core(){" << Qt::endl;
    QList<uint> alignments{0,1,2,4,8,16};
    {
        QList<QPair<bool,QString>> containerTypes{{false,"QList"}, {false,"QSet"}, {true,"QVector"}, {true,"QLinkedList"}};
        QList<uint> listBits{0,1,2,4,6,8,10,12,14,16,24,32,48,56,64,72,96,128};
        for(const QPair<bool,QString>& pair : containerTypes){
            if(pair.first){
                qtjambi_summary_h.stream << "#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
                qtjambi_summary_cpp.stream << "#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
            }
            for(uint bit : listBits){
                QString flatTypeName = pair.second.toLower().mid(1);
                QString fileName = QString("qtjambi_containeraccess_%1_%2.cpp").arg(flatTypeName).arg(bit);
                FileOut f(QString("%1/QtJambi/%2").arg(cppOutputDirectory(), fileName));
                priGenerator->addSource("QtJambi/generated.pri", fileName);
                f.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
                if(pair.first){
                    f.stream << "#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
                }
                f.stream << "#include <QtCore/qcompilerdetection.h>" << Qt::endl
                         << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
                         << "#include <qtjambi/qtjambi_containeraccess_" << flatTypeName << "_p.h>" << Qt::endl << Qt::endl
                         << "void qtjambi_register_" << flatTypeName << "_access_" << bit << "(){" << Qt::endl;
                for(uint algn : alignments){
                    f.stream << "    ContainerAccessFactoryHelper<" << pair.second << ", " << algn << ", " << bit << ", false>::registerContainerAccessFactory();" << Qt::endl;
                }
                if(pair.second=="QList"){
                    f.stream << "#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
                    for(uint algn : alignments){
                        if(algn<=bit)
                            f.stream << "    ContainerAccessFactoryHelper<" << pair.second << ", " << algn << ", " << bit << ", true>::registerContainerAccessFactory();" << Qt::endl;
                    }
                    f.stream << "#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
                }
                f.stream << "}" << Qt::endl;
                qtjambi_summary_h.stream << "void qtjambi_register_" << flatTypeName << "_access_" << bit << "();" << Qt::endl;
                qtjambi_summary_cpp.stream << "    qtjambi_register_" << flatTypeName << "_access_" << bit << "();" << Qt::endl;
                if(pair.first){
                    f.stream << "#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
                }
            }
            if(pair.first){
                qtjambi_summary_h.stream << "#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
                qtjambi_summary_cpp.stream << "#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)" << Qt::endl;
            }
        }
    }
    {
        QList<uint> pairBits{0,1,2,4,6,8,16,24,32,64};
        for(uint bit1 : pairBits){
            for(uint bit2 : pairBits){
                QString fileName = QString("qtjambi_containeraccess_pair_%1_%2.cpp").arg(bit1).arg(bit2);
                FileOut f(QString("%1/QtJambi/%2").arg(cppOutputDirectory(), fileName));
                priGenerator->addSource("QtJambi/generated.pri", fileName);
                f.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
                f.stream << "#include <QtCore/qcompilerdetection.h>" << Qt::endl
                         << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
                         << "#include <qtjambi/qtjambi_containeraccess_pair_p.h>" << Qt::endl << Qt::endl
                         << "void qtjambi_register_pair_access_" << bit1 << "_" << bit2 << "(){" << Qt::endl;
                for(uint algn1 : alignments){
                    if(algn1<=bit1)
                        for(uint algn2 : alignments){
                            if(algn2<=bit2)
                                f.stream << "    BiContainerAccessFactoryHelper<QPair," << algn1 << ", " << bit1 << ", " << algn2 << ", " << bit2 << ">::registerContainerAccessFactory();" << Qt::endl;
                        }
                }
                f.stream << "}" << Qt::endl;
                qtjambi_summary_h.stream << "void qtjambi_register_pair_access_" << bit1 << "_" << bit2 << "();" << Qt::endl;
                qtjambi_summary_cpp.stream << "    qtjambi_register_pair_access_" << bit1 << "_" << bit2 << "();" << Qt::endl;
            }
        }
    }
    {
        QList<QString> containerTypes{"QMap", "QHash", "QMultiMap", "QMultiHash"};
        QList<uint> mapBits1{0,1,2,4,8,16,24,32,64};
        QList<uint> mapBits2{0,1,2,4,8,16,24,32,64};
        for(const QString& container : containerTypes){
            QString flatTypeName = container.toLower().mid(1);
            for(uint bit1 : mapBits1){
                QString targetDir(bit1<=1 ? "QtJambi" : "QtJambiCore");
                FileOut& h = bit1<=1 ? qtjambi_summary_h : qtjambicore_summary_h;
                FileOut& cpp = bit1<=1 ? qtjambi_summary_cpp : qtjambicore_summary_cpp;
                for(uint bit2 : mapBits2){
                    QString fileName = QString("qtjambi_containeraccess_%1_%2_%3.cpp").arg(flatTypeName).arg(bit1).arg(bit2);
                    FileOut f(QString("%1/%2/%3").arg(cppOutputDirectory(), targetDir, fileName));
                    priGenerator->addSource(QString("%1/generated.pri").arg(targetDir), fileName);
                    f.stream << "#include <QtCore/QtGlobal>" << Qt::endl;
                    f.stream << "#include <QtCore/qcompilerdetection.h>" << Qt::endl
                             << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
                             << "#include <qtjambi/qtjambi_containeraccess_" << flatTypeName << ".h>" << Qt::endl << Qt::endl
                             << "void qtjambi_register_" << flatTypeName << "_access_" << bit1 << "_" << bit2 << "(){" << Qt::endl;
                    for(uint algn1 : alignments){
                        if(algn1<=bit1)
                            for(uint algn2 : alignments){
                                if(algn2<=bit2)
                                    f.stream << "    BiContainerAccessFactoryHelper<" << container << "," << algn1 << ", " << bit1 << ", " << algn2 << ", " << bit2 << ">::registerContainerAccessFactory();" << Qt::endl;
                            }
                    }
                    f.stream << "}" << Qt::endl;
                    h.stream << "void qtjambi_register_" << flatTypeName << "_access_" << bit1 << "_" << bit2 << "();" << Qt::endl;
                    cpp.stream << "    qtjambi_register_" << flatTypeName << "_access_" << bit1 << "_" << bit2 << "();" << Qt::endl;
                }
            }
        }
    }
    qtjambicore_summary_cpp.stream << "}" << Qt::endl;
    qtjambi_summary_cpp.stream << "}" << Qt::endl;
}

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
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_internal.h"), dedupe);
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
            writeInclude(f->stream, Include(Include::IncludePath, "qtjambi/qtjambi_repository.h"), dedupe);
            f->stream << Qt::endl;
            if(typeSystemEntry)
                generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position1, INDENT);
            if(cls->targetTypeSystem()=="io.qt.core"){
                f->stream << "void qtjambi_register_containeraccess_core();" << Qt::endl;
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
                    generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position3, INDENT);
                    generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::Position4, INDENT);
                    generateInitializer(f->stream, typeSystemEntry, TypeSystem::MetaInfo, CodeSnip::End, INDENT);
                }
                if(package=="io.qt.core"){
                    f->stream << INDENT << "qtjambi_register_containeraccess_core();" << Qt::endl;
                    writeContainerAccess();
                }
                f->stream << INDENT << "return JNI_VERSION_1_8;" << Qt::endl;
            }
            f->stream << "}" << Qt::endl << Qt::endl;

            f->stream << INDENT << "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
                      << QString(package).replace("_", "_1").replace(".", "_") << "_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){" << Qt::endl
                      << "    try{" << Qt::endl
                      << "        return qtjambi_get_internal_access(env, cls);" << Qt::endl
                      << "    }catch(const JavaException& exn){" << Qt::endl
                      << "        exn.raiseInJava(env);" << Qt::endl
                      << "    }" << Qt::endl
                      << "    return nullptr;" << Qt::endl
                      << "}" << Qt::endl << Qt::endl;
            for(const QString& pkg : m_typeSystemByPackage.keys(package)){
                if(pkg=="io.qt.internal")
                    continue;
                f->stream << INDENT << "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
                          << QString(pkg).replace("_", "_1").replace(".", "_") << "_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){" << Qt::endl
                          << "    try{" << Qt::endl
                          << "        return qtjambi_get_internal_access(env, cls);" << Qt::endl
                          << "    }catch(const JavaException& exn){" << Qt::endl
                          << "        exn.raiseInJava(env);" << Qt::endl
                          << "    }" << Qt::endl
                          << "    return nullptr;" << Qt::endl
                          << "}" << Qt::endl << Qt::endl;
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
    QMap<QString,QSet<const TypeSystemTypeEntry*>> allTypeSystems;
    QHash<const TypeSystemTypeEntry*,QList<QString>> exports;
    QHash<const TypeSystemTypeEntry*,QList<QString>> requiredTypeSystems;
    QHash<const TypeSystemTypeEntry*,QList<QString>> nativeLibs;
    TypeDatabase* typeDatabase = TypeDatabase::instance();
    for(auto iter = m_packageGenerationPolicies.constKeyValueBegin(); iter!=m_packageGenerationPolicies.constKeyValueEnd(); ++iter) {

        if (iter->second & GeneratedJavaClasses) {
            const QString& package = iter->first;
            if(package=="io.qt.internal")
                continue;
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
            s << INDENT << "final class QtJambi_LibraryUtilities {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "static{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "try {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(typeSystemEntry){
                            QString typeSystemID = typeSystemEntry->module();
                            if(typeSystemID.isEmpty())
                                typeSystemID = typeSystemEntry->qtLibrary();
                            if(!typeSystemID.isEmpty() && (!exports.contains(typeSystemEntry) || !exports[typeSystemEntry].contains(package)))
                                exports[typeSystemEntry] << package;
                            if(!typeSystemID.isEmpty() && !typeSystemEntry->qtLibrary().isEmpty() && (!nativeLibs.contains(typeSystemEntry) || !nativeLibs[typeSystemEntry].contains(typeSystemEntry->qtLibrary())))
                                nativeLibs[typeSystemEntry] << typeSystemEntry->qtLibrary();
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Beginning, INDENT);
                            s << INDENT << "io.qt.QtUtilities.initializePackage(\"io.qt.internal\");" << Qt::endl;
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position1, INDENT);
                            if(typeSystemID!="qtjambi")
                                requiredTypeSystems[typeSystemEntry] << "qtjambi";

                            QSet<QString> alreadyInitializedPackages;

                            struct Analyzer{
                                static void analyzeRequiredQtLibraries(TypeDatabase* typeDatabase, QSet<QString>& alreadyInitializedPackages, QList<QPair<QString,bool>>& requiredQtLibraries, const TypeSystemTypeEntry* ts, bool optional){
                                    if(ts && !alreadyInitializedPackages.contains(ts->name())){
                                        for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                            analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, requiredQtLibraries, entry, optional);
                                        }
                                        for(const QPair<QString,bool>& pair : ts->requiredQtLibraries()){
                                            analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, requiredQtLibraries, pair.first, pair.second || optional);
                                        }
                                        if(!requiredQtLibraries.contains({ts->qtLibrary(), true})
                                                && !requiredQtLibraries.contains({ts->qtLibrary(), false}))
                                            requiredQtLibraries.append({ts->qtLibrary(), optional});
                                    }
                                }

                                static void analyzeRequiredQtLibraries(TypeDatabase* typeDatabase, QSet<QString>& alreadyInitializedPackages, QList<QPair<QString,bool>>& requiredQtLibraries, const QString& lib, bool optional){
                                    if(TypeSystemTypeEntry* ts = typeDatabase->typeSystemsByQtLibrary()[lib]){
                                        if(ts && !alreadyInitializedPackages.contains(ts->name())){
                                            for(const TypeSystemTypeEntry* entry : ts->requiredTypeSystems()){
                                                analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, requiredQtLibraries, entry, optional);
                                            }
                                            for(const QPair<QString,bool>& pair : ts->requiredQtLibraries()){
                                                analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, requiredQtLibraries, pair.first, pair.second || optional);
                                            }
                                            if(!requiredQtLibraries.contains({ts->qtLibrary(), true})
                                                    && !requiredQtLibraries.contains({ts->qtLibrary(), false}))
                                                requiredQtLibraries.append({ts->qtLibrary(), optional});
                                        }
                                    }else{
                                        requiredQtLibraries.append({lib, true});
                                    }
                                }
                            };

                            for(const TypeSystemTypeEntry* entry : typeSystemEntry->requiredTypeSystems()){
                                if(!entry || alreadyInitializedPackages.contains(entry->name()))
                                    continue;
                                alreadyInitializedPackages.insert(entry->name());
                                requiredTypeSystems[typeSystemEntry] << entry->module();
                            }
                            alreadyInitializedPackages.clear();
                            requiredTypeSystems[typeSystemEntry].removeAll(typeSystemEntry->module());

                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position2, INDENT);
                            QList<QPair<QString,bool>> requiredQtLibraries;
                            for(const QPair<QString,bool>& pair : typeSystemEntry->requiredQtLibraries()){
                                Analyzer::analyzeRequiredQtLibraries(typeDatabase, alreadyInitializedPackages, requiredQtLibraries, pair.first, pair.second);
                            }
                            requiredQtLibraries.removeAll({"QtCore", true});
                            requiredQtLibraries.removeAll({"QtCore", false});// is always loaded by package io.qt.internal
                            requiredQtLibraries.removeAll({typeSystemEntry->qtLibrary(), true});
                            requiredQtLibraries.removeAll({typeSystemEntry->qtLibrary(), false});
                            requiredQtLibraries.removeAll({"QtUiPlugin", true});
                            requiredQtLibraries.removeAll({"QtUiPlugin", false});// does not exist as library
                            requiredQtLibraries.removeAll({"QtQmlIntegration", true});
                            requiredQtLibraries.removeAll({"QtQmlIntegration", false});// does not exist as library
                            QSet<QString> loaded;
                            for(const QPair<QString,bool>& pair : requiredQtLibraries){
                                if(loaded.contains(pair.first) || m_staticLibraries.contains(pair.first))
                                    continue;
                                loaded.insert(pair.first);
                                if(pair.first.startsWith("Qt")){
                                    s << INDENT;
                                    if(pair.second)
                                        s << "if(io.qt.QtUtilities.isAvailableQtLibrary(\"" << pair.first.mid(2) << "\"))" << Qt::endl << INDENT << "    ";
                                    s << "io.qt.QtUtilities.loadQtLibrary(\"" << pair.first.mid(2) << "\");" << Qt::endl;
                                }else{
                                    s << INDENT << "if(io.qt.QtUtilities.isAvailableUtilityLibrary(\"" << pair.first << "\"))" << Qt::endl
                                      << INDENT << "    io.qt.QtUtilities.loadUtilityLibrary(\"" << pair.first << "\");" << Qt::endl;
                                }
                            }
                            generateInitializer(s, typeSystemEntry, TypeSystem::TargetLangCode, CodeSnip::Position3, INDENT);
                            if(!typeSystemEntry->qtLibrary().isEmpty() && !m_staticLibraries.contains(typeSystemEntry->qtLibrary())){
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
                            if((typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(typeSystemByPackage)))){
                                QString typeSystemID = typeSystemEntry->module();
                                if(typeSystemID.isEmpty())
                                    typeSystemID = typeSystemEntry->qtLibrary();
                                if(!typeSystemID.isEmpty() && (!exports.contains(typeSystemEntry) || !exports[typeSystemEntry].contains(package)))
                                    exports[typeSystemEntry] << package;
                                if(!typeSystemID.isEmpty() && !typeSystemEntry->qtLibrary().isEmpty() && (!nativeLibs.contains(typeSystemEntry) || !nativeLibs[typeSystemEntry].contains(typeSystemEntry->qtLibrary())))
                                    nativeLibs[typeSystemEntry] << typeSystemEntry->qtLibrary();
                            }
                            s << INDENT << "io.qt.QtUtilities.initializePackage(\"" << typeSystemByPackage << "\");" << Qt::endl;
                        }
                    }
                    s << INDENT << "} catch(Error t) {" << Qt::endl;
                    s << INDENT << "    throw t;" << Qt::endl;
                    s << INDENT << "} catch(Throwable t) {" << Qt::endl;
                    s << INDENT << "    throw new ExceptionInInitializerError(t);" << Qt::endl;
                    s << INDENT << "}" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
                s << INDENT << "static final io.qt.InternalAccess internal = internalAccess();" << Qt::endl << Qt::endl;
                s << INDENT << "private static native io.qt.InternalAccess internalAccess();" << Qt::endl << Qt::endl;
                s << INDENT << "static void initialize() { };" << Qt::endl << Qt::endl;
                s << INDENT << "private QtJambi_LibraryUtilities() throws java.lang.InstantiationError { throw new java.lang.InstantiationError(\"Cannot instantiate QtJambi_LibraryUtilities.\"); }" << Qt::endl;
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
                for(const TypeSystemTypeEntry* typeSystem : allTypeSystems[moduleName]){
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
                for(const TypeSystemTypeEntry* typeSystem : allTypeSystems[moduleName]){
                    generateInitializer(fileOut.stream, typeSystem, TypeSystem::ModuleInfo, CodeSnip::Position1, INDENT);
                }
                fileOut.stream << "module " << moduleName << " {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    for(const TypeSystemTypeEntry* typeSystem : allTypeSystems[moduleName]){
                        generateInitializer(fileOut.stream, typeSystem, TypeSystem::ModuleInfo, CodeSnip::Beginning, INDENT);
                    }
                    fileOut.stream << "    requires java.base;" << Qt::endl;
                    for(const TypeSystemTypeEntry* typeSystem : allTypeSystems[moduleName]){
                        QList<QString>& entries = requiredTypeSystems[typeSystem];
                        entries.removeDuplicates();
                        generateInitializer(fileOut.stream, typeSystem, TypeSystem::ModuleInfo, CodeSnip::Position2, INDENT);
                        for(const QString& e : entries){
                            if(e=="qtjambi")
                                fileOut.stream << "    requires transitive " << e << ";" << Qt::endl;
                            else
                                fileOut.stream << "    requires " << e << ";" << Qt::endl;
                            dependentModules << e;
                        }
                        generateInitializer(fileOut.stream, typeSystem, TypeSystem::ModuleInfo, CodeSnip::Position3, INDENT);
                        generateInitializer(fileOut.stream, typeSystem, TypeSystem::ModuleInfo, CodeSnip::End, INDENT);
                        if(!typeSystem->isNoExports())
                            for(const QString& e : exports[typeSystem])
                                fileOut.stream << "    exports " << e << ";" << Qt::endl;
                    }
                }
                fileOut.stream << "}" << Qt::endl;
                for(const TypeSystemTypeEntry* typeSystem : allTypeSystems[moduleName]){
                    generateInitializer(fileOut.stream, typeSystem, TypeSystem::ModuleInfo, CodeSnip::Position4, INDENT);
                    if(!nativeLibs[typeSystem].isEmpty()){
                        QSet<QString> libraries;
                        for(const QString& e : nativeLibs[typeSystem]){
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
