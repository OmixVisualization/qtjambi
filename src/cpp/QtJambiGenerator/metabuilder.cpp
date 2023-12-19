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

#include "metabuilder.h"
#include "reporthandler.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#endif
#include <QTextStream>
#include <QVariant>

static const char* GLOBAL_PACKAGE = "package_global";

inline QString strip_template_args(const QString &name) {
    auto pos = name.indexOf('<');
    return pos < 0 ? name : name.left(pos);
}

inline void hide_functions(const MetaFunctionList &l) {
    for(MetaFunction *f : l) {
        if(!f->declaringClass()->isInterface()
                && !f->isAbstract()
                && f->name().startsWith("operator_")){
            FunctionModification mod;
            mod.signature = f->minimalSignature();
            if(f->implementingClass()->isInterface())
                mod.modifiers = FunctionModification::Friendly;
            else
                mod.modifiers = FunctionModification::Private;
            const_cast<ComplexTypeEntry *>(static_cast<const ComplexTypeEntry *>(f->implementingClass()->typeEntry()))->addFunctionModification(mod);
        }
    }
}

inline void remove_function(MetaFunction *f) {
    FunctionModification mod;
    mod.removal = TS::All;
    mod.signature = f->minimalSignature();
    const_cast<ComplexTypeEntry *>(static_cast<const ComplexTypeEntry *>(f->implementingClass()->typeEntry()))->addFunctionModification(mod);
}

template<typename Functor>
void applyOnType(ComplexTypeEntry* type, Functor&& functor){
    functor(type);
    for(const ComplexTypeEntry* ins : type->instantiations()){
        functor(const_cast<ComplexTypeEntry*>(ins));
    }
    if(type->designatedInterface()){
        functor(type->designatedInterface());
        for(const ComplexTypeEntry* ins : type->designatedInterface()->instantiations()){
            functor(const_cast<ComplexTypeEntry*>(ins));
        }
    }else if(type->isInterface()){
        InterfaceTypeEntry* itype = dynamic_cast<InterfaceTypeEntry*>(type);
        functor(itype->origin());
        for(const ComplexTypeEntry* ins : itype->origin()->instantiations()){
            functor(const_cast<ComplexTypeEntry*>(ins));
        }
    }
}

const QString &MetaBuilder::generateTypeSystemQML() const
{
    return m_generateTypeSystemQML;
}

void MetaBuilder::setGenerateTypeSystemQML(const QString &newGenerateTypeSystemQML)
{
    m_generateTypeSystemQML = newGenerateTypeSystemQML;
}

const QStringList &MetaBuilder::getIncludePathsList() const
{
    return m_includePathsList;
}

void MetaBuilder::setIncludePathsList(const QStringList &newIncludePathsList)
{
    m_includePathsList = newIncludePathsList;
}

MetaBuilder::MetaBuilder(TS::TypeDatabase* database)
        : m_current_class(nullptr),
          m_current_function(nullptr),
          m_features(nullptr),
          m_qtVersionMajor(QT_VERSION_MAJOR),
          m_qtVersionMinor(QT_VERSION_MINOR),
          m_qtVersionPatch(QT_VERSION_PATCH),
          m_qtjambiVersionPatch(0),
          m_database(database)
{
}

MetaBuilder::~MetaBuilder() {
}

/**
 * This function is here, simply to print out warnings about
 * modifications not having the proper signature...
 */
void MetaBuilder::checkFunctionModifications() {
    SingleTypeEntryHash entryHash = m_database->entries();
    const QList<TypeEntry *> entries = entryHash.values();
    for(TypeEntry *entry : entries) {
        if (!entry || entry->codeGeneration() == TypeEntry::GenerateNothing)
            continue;
        if (entry->isTypeSystem()){
            /*
            TS::TypeSystemTypeEntry *tentry = static_cast<TS::TypeSystemTypeEntry *>(entry);
            const FunctionModificationList& functionModifications = tentry->functionModifications();
            MetaClass *clazz = nullptr;
            for(const FunctionModification& modification : functionModifications) {
                const QString& signature = modification.signature;

                QString name = signature.trimmed();
                if(!name.isEmpty()){
                    name = name.mid(0, signature.indexOf("("));

                    if (!clazz){
                        clazz = m_meta_classes.findClass(centry->qualifiedCppName(), MetaClassList::QualifiedCppName);
                        if (!clazz)
                            break;
                    }

                    bool found = false;
                    QStringList possibleSignatures;
                    for(MetaFunction *function : clazz->functions()) {
                        if (function->minimalSignature() == signature && function->implementingClass() == clazz) {
                            found = true;
                            break;
                        }

                        if (function->originalName() == name)
                            possibleSignatures.append(function->minimalSignature() + " in " + function->implementingClass()->name());
                    }

                    if(!found && clazz->typeEntry()->isInterface()){
                        clazz = clazz->extractInterfaceImpl();
                        for(MetaFunction *function : clazz->functions()) {
                            if (function->minimalSignature() == signature && function->implementingClass() == clazz) {
                                found = true;
                                break;
                            }
                            if (function->originalName() == name)
                                possibleSignatures.append(function->minimalSignature() + " in " + function->implementingClass()->name());
                        }
                    }

                    if (!found && clazz->typeEntry()->codeGeneration()!=TypeEntry::GenerateNothing && modification.template_instantiations.isEmpty()) {
                        QString warning
                        = QString("signature '%1' for function modification in '%2' not found. Possible candidates: %3")
                          .arg(signature, clazz->qualifiedCppName(), possibleSignatures.join(",\n    "));

                        ReportHandler::warning(warning);
                    }
                }
            }*/
        }else if (entry->isComplex()){
            ComplexTypeEntry *centry = static_cast<ComplexTypeEntry *>(entry);
            const FunctionModificationList& functionModifications = centry->functionModifications();
            MetaClass *clazz = nullptr;
            for(const FunctionModification& modification : functionModifications) {
                const QString& signature = modification.signature;

                QString name = signature.trimmed();
                if(!name.isEmpty()){
                    name = name.mid(0, signature.indexOf("("));

                    if (!clazz){
                        clazz = m_meta_classes.findClass(centry->qualifiedCppName(), MetaClassList::QualifiedCppName);
                        if (!clazz)
                            break;
                    }

                    bool found = false;
                    QStringList possibleSignatures;
                    for(MetaFunction *function : clazz->functions()) {
                        if (function->minimalSignature() == signature && function->implementingClass() == clazz) {
                            found = true;
                            break;
                        }

                        if (function->originalName() == name)
                            possibleSignatures.append(function->minimalSignature() + " in " + function->implementingClass()->name());
                    }
                    if(!found && clazz->typeEntry()->codeGeneration()!=TypeEntry::GenerateAll){
                        for(MetaFunction *function : clazz->invalidFunctions()) {
                            if (function->originalSignature() == signature) {
                                found = true;
                                break;
                            }
                        }
                    }

                    if(!found && clazz->typeEntry()->isInterface()){
                        clazz = clazz->extractInterfaceImpl();
                        for(MetaFunction *function : clazz->functions()) {
                            if (function->minimalSignature() == signature && function->implementingClass() == clazz) {
                                found = true;
                                break;
                            }
                            if (function->originalName() == name)
                                possibleSignatures.append(function->minimalSignature() + " in " + function->implementingClass()->name());
                        }
                        if(!found && clazz->typeEntry()->codeGeneration()!=TypeEntry::GenerateAll){
                            for(MetaFunction *function : clazz->invalidFunctions()) {
                                if (function->originalSignature() == signature) {
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }

                    if (!found && clazz->typeEntry()->codeGeneration()!=TypeEntry::GenerateNothing && modification.template_instantiations.isEmpty()) {
                        QString warning
                        = QString("signature '%1' for function modification in '%2' not found. Possible candidates: %3")
                          .arg(signature, clazz->qualifiedCppName(), possibleSignatures.join(",\n    "));

                        ReportHandler::warning(warning);
                    }
                }
            }
        }
    }
}

MetaClass *MetaBuilder::argumentToClass(const MetaType* type) {
    MetaClass *returned = nullptr;
    if (type && type->typeEntry() && type->typeEntry()->isComplex()) {
        const TypeEntry *entry = type->typeEntry();
        if(entry->designatedInterface())
            entry = entry->designatedInterface();
        returned = m_meta_classes.findClass(entry->qualifiedCppName());
        if(!returned)
            returned = m_meta_classes.findClass(entry->name());
        if(!returned)
            returned = m_templates.findClass(entry->qualifiedCppName());
        if(!returned)
            returned = m_templates.findClass(entry->name());
    }
    return returned;
}

void MetaBuilder::addInclude(TS::ComplexTypeEntry * entry, const MetaType* includedType){
    if(includedType->typeEntry()->isComplex()){
        const ComplexTypeEntry *centry = static_cast<const ComplexTypeEntry *>(includedType->typeEntry());
        if (entry && centry->include().isValid())
            entry->addExtraInclude(centry->include());
    }else if(includedType->isEnum()){
        const EnumTypeEntry *eentry = static_cast<const EnumTypeEntry *>(includedType->typeEntry());
        if (entry && eentry->include().isValid())
            entry->addExtraInclude(eentry->include());
    }
    for(const MetaType* ins : includedType->instantiations()){
        addInclude(entry, ins);
    }
}

void MetaBuilder::addInclude(TS::TypeEntry * entry, QString fileName, bool extra){
    QMap<QString,QString> requiredFeatures;
    for(const QString& feature : _M_requiredFeatures[fileName]){
        if(m_features && m_features->contains(feature)){
            QFileInfo _info(m_features->value(feature));
            requiredFeatures[feature] = _info.fileName();
        }else{
            requiredFeatures[feature] = "";
        }
    }
    bool ckeckAvailability = !entry->ppCondition().isEmpty();
    QFileInfo file(QDir::fromNativeSeparators(fileName));
    if(file.exists()){
        fileName = file.canonicalFilePath();
#ifdef Q_OS_MAC
        QDir dir = file.dir();
        if(QFileInfo(dir.absolutePath()).fileName()==QStringLiteral("Headers") && dir.cdUp()){
            if(!QFileInfo(dir.absolutePath()).fileName().endsWith(".framework")){
                if(dir.cdUp() && QFileInfo(dir.absolutePath()).fileName()==QStringLiteral("Versions") && dir.cdUp()){
                }
            }
            QString moduleName = QFileInfo(dir.absolutePath()).fileName();
            if(moduleName.endsWith(".framework")){
                moduleName = moduleName.mid(0, moduleName.length()-10);
                QString path = QString("%1/%2").arg(moduleName, file.fileName());
                if(entry->isComplex()){
                    if(extra)
                        dynamic_cast<TS::ComplexTypeEntry*>(entry)->addExtraInclude(Include(Include::IncludePath, path, requiredFeatures));
                    else
                        dynamic_cast<TS::ComplexTypeEntry*>(entry)->setInclude(Include(Include::IncludePath, path, requiredFeatures, false, false, ckeckAvailability));
                }else if(entry->isEnum()){
                    if(!extra)
                        dynamic_cast<TS::EnumTypeEntry*>(entry)->setInclude(Include(Include::IncludePath, path, requiredFeatures, false, false, ckeckAvailability));
                }else if(entry->isFunctional()){
                    if(extra)
                        dynamic_cast<TS::FunctionalTypeEntry*>(entry)->addExtraInclude(Include(Include::IncludePath, path, requiredFeatures));
                    else
                        dynamic_cast<TS::FunctionalTypeEntry*>(entry)->setInclude(Include(Include::IncludePath, path, requiredFeatures, false, false, ckeckAvailability));
                }
                fileName.clear();
            }
        }
#endif
    }
    if(!fileName.isEmpty()){
        bool found = false;
        for(auto iter = m_includePathsList.rbegin(); iter!=m_includePathsList.rend(); ++iter){
            QFileInfo f(*iter);
            if(f.isDir()){
                QString path = f.canonicalFilePath();
                if(!path.endsWith("/"))
                    path += "/";
                if(fileName.startsWith(path)){
                    path = fileName.mid(path.size());
                    if(entry->isComplex()){
                        if(extra)
                            dynamic_cast<TS::ComplexTypeEntry*>(entry)->addExtraInclude(Include(Include::IncludePath, path, requiredFeatures));
                        else
                            dynamic_cast<TS::ComplexTypeEntry*>(entry)->setInclude(Include(Include::IncludePath, path, requiredFeatures, false, false, ckeckAvailability));
                    }else if(entry->isEnum()){
                        if(!extra)
                            dynamic_cast<TS::EnumTypeEntry*>(entry)->setInclude(Include(Include::IncludePath, path, requiredFeatures, false, false, ckeckAvailability));
                    }else if(entry->isFunctional()){
                        if(extra)
                            dynamic_cast<TS::FunctionalTypeEntry*>(entry)->addExtraInclude(Include(Include::IncludePath, path, requiredFeatures));
                        else
                            dynamic_cast<TS::FunctionalTypeEntry*>(entry)->setInclude(Include(Include::IncludePath, path, requiredFeatures, false, false, ckeckAvailability));
                    }
                    found = true;
                    break;
                }
            }
        }
        if(!found){
            ReportHandler::warning(QString("Unable to decide include for '%1'.").arg(fileName));
        }
    }
}

void MetaBuilder::sortLists() {
    std::sort(m_meta_classes.begin(), m_meta_classes.end(), [](MetaClass *a, MetaClass *b) -> bool {
                  return a->qualifiedCppName() < b->qualifiedCppName();
              });
    for(MetaClass *cls : qAsConst(m_meta_classes)) {
        cls->sortFunctions();
        cls->sortEnums();
        cls->sortFields();
        cls->sortFunctionals();
    }
}

void writeContent(QTextStream &stream,
                  QString indent,
                  const QList<ClassModelItem>& classes,
                  const QList<EnumModelItem>& enums,
                  const QList<TypeAliasModelItem>& typeAliases,
                  const QList<VariableModelItem>& variables,
                  const QList<FunctionDefinitionModelItem>& functionDefinitions,
                  const QList<FunctionModelItem>& functions,
                  const QList<NamespaceModelItem>& namespaces){
    {
        stream << Qt::endl;
        stream << indent << "Classes:" << Qt::endl;
        for(ClassModelItem cls : classes){
            stream << indent << "class " << cls->qualifiedName().join("::") << "{" << Qt::endl;
            writeContent(stream, indent+"    ", cls->classes(), cls->enums(), cls->typeAliases(), cls->variables(), cls->functionDefinitions(), cls->functions(), {});
            stream << indent << "}" << Qt::endl;
        }
    }
    {
        QMap<QString, EnumModelItem> _map;
        for(const EnumModelItem& item : enums){
            _map.insert(item->name(), item);
        }
        if(!_map.isEmpty()){
            stream << Qt::endl;
            stream << indent << "Enums:" << Qt::endl;
            for(QMap<QString, EnumModelItem>::const_iterator i = _map.cbegin(); i!=_map.cend(); i++){
                stream << indent << i.key() << Qt::endl;
            }
        }
    }
    {
        QMap<QString, TypeAliasModelItem> _map;
        for(const TypeAliasModelItem& item : typeAliases){
            _map.insert(item->name(), item);
        }
        if(!_map.isEmpty()){
            stream << Qt::endl;
            stream << indent << "Type Aliases:" << Qt::endl;
            for(QMap<QString, TypeAliasModelItem>::const_iterator i = _map.cbegin(); i!=_map.cend(); i++){
                stream << indent << i.key() << Qt::endl;
            }
        }
    }
    {
        QMap<QString, VariableModelItem> _map;
        for(const VariableModelItem& item : variables){
            _map.insert(item->name(), item);
        }
        if(!_map.isEmpty()){
            stream << Qt::endl;
            stream << indent << "Variables:" << Qt::endl;
            for(QMap<QString, VariableModelItem>::const_iterator i = _map.cbegin(); i!=_map.cend(); i++){
                stream << indent << i.key() << Qt::endl;
            }
        }
    }
    {
        QMap<QString, FunctionDefinitionModelItem> _map;
        for(const FunctionDefinitionModelItem& item : functionDefinitions){
            _map.insert(item->name(), item);
        }
        if(!_map.isEmpty()){
            stream << Qt::endl;
            stream << indent << "Function Definitions:" << Qt::endl;
            for(QMap<QString, FunctionDefinitionModelItem>::const_iterator i = _map.cbegin(); i!=_map.cend(); i++){
                stream << indent << i.key() << Qt::endl;
            }
        }
    }
    {
        QMap<QString, FunctionModelItem> _map;
        for(const FunctionModelItem& item : functions){
            _map.insert(item->name(), item);
        }
        if(!_map.isEmpty()){
            stream << Qt::endl;
            stream << indent << "Functions:" << Qt::endl;
            for(QMap<QString, FunctionModelItem>::const_iterator i = _map.cbegin(); i!=_map.cend(); i++){
                stream << indent << i.key() << Qt::endl;
            }
        }
    }
    {
        QMap<QString, NamespaceModelItem> _map;
        for(const NamespaceModelItem& item : namespaces){
            _map.insert(item->name(), item);
        }
        if(!_map.isEmpty()){
            stream << Qt::endl;
            stream << indent << "Namespaces:" << Qt::endl;
            for(QMap<QString, NamespaceModelItem>::const_iterator i = _map.cbegin(); i!=_map.cend(); i++){
                stream << indent << "namespace " << i.key() << "{" << Qt::endl;
                writeContent(stream, indent+"    ", i.value()->classes(), i.value()->enums(), i.value()->typeAliases(), i.value()->variables(), i.value()->functionDefinitions(), i.value()->functions(), i.value()->namespaces());
                stream << indent << "}" << Qt::endl;
            }
        }
    }
}

bool MetaBuilder::build(FileModelItem&& dom) {
    m_dom = std::move(dom);
    ReportHandler::setContext("MetaJavaBuilder");
    pushScope(model_dynamic_cast<ScopeModelItem>(m_dom));

#if 0
    {
        QFile file2("found-classes.log");
        if (file2.open(QFile::WriteOnly)){
            QTextStream stream(&file2);
            stream.setCodec(QTextCodec::codecForName("UTF-8"));
            writeContent(stream, "", m_dom->classes(), m_dom->enums(), m_dom->typeAliases(), m_dom->variables(), m_dom->functionDefinitions(), m_dom->functions(), m_dom->namespaces());
            file2.close();
        }
    }
#endif
    QList<PendingClass> pendingClasses;
    QList<PendingFunctional> pendingFunctionals;

    // Start the generation...
    for(const ClassModelItem& cls : m_dom->classes()){
        MetaClass* meta_class = traverseClass(cls, pendingClasses, pendingFunctionals);
        if(meta_class){
            addClass(meta_class);
        }
    }

    for(const NamespaceModelItem& ns : m_dom->namespaces()){
        MetaClass *meta_class = traverseNamespace(ns, pendingClasses, pendingFunctionals);
        if (meta_class){
            addClass(meta_class);
//            m_meta_classes << meta_class;
//            if(!meta_class->typeEntry()->isString()){
//                if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(meta_class->targetTypeSystem())){
//                    m_typeSystemByPackage[meta_class->package()] = ts;
//                }
//            }
        }
    }

    QSet<QString> metaEnums;
    for(const FunctionModelItem& function_item : m_dom->functions()){
        if(function_item->name()==QStringLiteral("qt_getEnumMetaObject")
                && function_item->arguments().size()==1){
            metaEnums << function_item->arguments()[0]->type().toString();
        }
    }

    // Some trickery to support global-namespace enums...
    m_current_class = nullptr;
    for(const EnumModelItem& e : m_dom->enums()){
        MetaEnum *meta_enum = traverseEnum(e, nullptr, metaEnums, m_dom->flagsMap());

        if (meta_enum) {
            MetaClass *global = m_globals[meta_enum->typeEntry()->targetTypeSystem()];
            if (!global) {
                ComplexTypeEntry *gte = new GlobalTypeEntry(GLOBAL_PACKAGE);
                gte->setTargetTypeSystem(meta_enum->typeEntry()->targetTypeSystem());
                gte->setTargetLangPackage(meta_enum->typeEntry()->targetTypeSystem());
                gte->setCodeGeneration(TypeEntry::GenerateNothing);
                global = new MetaClass();
                global->setTypeEntry(gte);
                *global += MetaAttributes::Final;
                *global += MetaAttributes::Public;
                *global += MetaAttributes::Fake;

                m_meta_classes << global;
                m_globals[meta_enum->typeEntry()->targetTypeSystem()] = global;
                if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(global->targetTypeSystem())){
                    m_typeSystemByPackage[global->package()] = ts;
                }
            }

            global->addEnum(meta_enum);
            meta_enum->setEnclosingClass(global);
//            meta_enum->typeEntry()->setQualifier(globalName);
        }
    }

    // Go through all typedefs to see if we have defined any
    // specific typedefs to be used as classes.
    for(const TypeAliasModelItem& tai : m_dom->typeAliases()){
        if(tai->type().qualifiedName()==QStringList{"std","function"}
                || tai->type().qualifiedName()==QStringList{"qxp","function_ref"}
                || tai->type().isFunctionPointer()){
            pendingFunctionals.append(PendingFunctional{tai, nullptr, m_scopes});
        }else{
            MetaClass *cls = traverseTypeAlias(tai);
            if(cls){
                addClass(cls);
            }
        }
    }

    for(const QPair<MetaClass*,QString>& pair : qAsConst(m_pendingScopedClasses)){
        if(MetaClass *cls = m_meta_classes.findClass(pair.second, MetaClassList::QualifiedCppName)){
            if(cls!=pair.first->enclosingClass()){
                cls->addEnclosedClass(pair.first);
                pair.first->setEnclosingClass(cls);
            }
        }
    }

    for(MetaEnum *enm : qAsConst(m_scopeChangedEnums)){
        if(MetaClass *cls = m_meta_classes.findClass(enm->typeEntry()->javaScope(), MetaClassList::QualifiedCppName)){
            if(cls!=enm->enclosingClass()){
                cls->addEnum(enm);
                enm->setEnclosingClass(cls);
            }
        }
    }

    QList<PendingHiddenBaseType> pendingHiddenBaseTypes;
    QList<MetaClass *> pendingConstructorUsages;
    for(MetaClass *cls : qAsConst(m_meta_classes)) {
        if (!cls->isInterface() && !cls->isNamespace()) {
            setupInheritance(cls, pendingHiddenBaseTypes, pendingConstructorUsages);
        }else if(cls->isInterface()){
            setupInheritance(cls->extractInterfaceImpl(), pendingHiddenBaseTypes, pendingConstructorUsages);
        }
    }

    pendingClasses.append(PendingClass{model_dynamic_cast<ScopeModelItem>(m_dom), nullptr, m_scopes});

    {
        QList<ScopeModelItem> previousScopes = m_scopes;
        MetaClass* last_class = m_current_class;
        for(const PendingFunctional& pf : qAsConst(pendingFunctionals)){
            m_scopes = pf.scopes;
            m_namespace_prefix = currentScope()->qualifiedName().join("::");
            m_current_class = pf.metaClass;
            MetaFunctional *fnl = traverseFunctional(pf.scope);
            if(fnl){
                if(pf.metaClass)
                    pf.metaClass->addFunctional(fnl);
                else
                    addFunctional(fnl);
            }
        }
        for(const PendingClass& pc : qAsConst(pendingClasses)){
            m_scopes = pc.scopes;
            m_namespace_prefix = currentScope()->qualifiedName().join("::");
            m_current_class = pc.metaClass;
            traverseFunctions(pc.scope);
            traverseFields(pc.scope);
        }
        m_current_class = last_class;
        pushScope(model_dynamic_cast<ScopeModelItem>(m_dom));
        m_scopes = previousScopes;
        m_namespace_prefix = currentScope()->qualifiedName().join("::");
    }

    for(const PendingHiddenBaseType& p : qAsConst(pendingHiddenBaseTypes)) {
        MetaClass *old_current_class = m_current_class;
        m_current_class = p.subclass;
        inheritHiddenBaseType(p.subclass, p.hidden_base_class, p.info, pendingConstructorUsages);
        m_current_class = old_current_class;
    }

    for(MetaClass *cls : qAsConst(pendingConstructorUsages)) {
        if(cls->baseClass() && (cls->usingProtectedBaseConstructors() || cls->usingPublicBaseConstructors())){
            for(const MetaFunction* fun : cls->baseClass()->functions()){
                if(fun->isConstructor()){
                    MetaFunction* constructor = fun->copy();
                    constructor->setImplementingClass(cls);
                    constructor->setDeclaringClass(cls);
                    constructor->setName(cls->qualifiedCppName().split("::").last());
                    constructor->setOriginalName(constructor->name());
                    if(cls->usingPublicBaseConstructors()){
                        constructor->setVisibility(MetaAttributes::Public);
                        applyOnType(cls->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicDefaultConstructor();});
                    }else{
                        constructor->setVisibility(MetaAttributes::Protected);
                        applyOnType(cls->typeEntry(), [](ComplexTypeEntry* c){c->setHasProtectedDefaultConstructor();});
                    }
                    cls->addFunction(constructor);
                }
            }
        }
    }

    fixMissingIterator();

    for(MetaClass *cls : qAsConst(m_templates)) {
        setupConstructorAvailability(cls);
    }
    for(MetaClass *cls : qAsConst(m_meta_classes)) {
        setupConstructorAvailability(cls);
        fixFunctions(cls);
    }

    const QList<TypeEntry *> entries = m_database->entries().values();
    for(const TypeEntry *entry : entries) {
        if (entry->isPrimitive())
            continue;

        if ((entry->isValue() || entry->isObject())
                && !entry->isString()
                && !entry->isQChar()
                && !entry->isContainer()
                && !entry->isPointerContainer()
                && !entry->isInitializerList()
                && !entry->isQMetaObjectType()
                && !entry->isQMetaObjectConnectionType()
                && !entry->isQVariant()
                && !m_meta_classes.findClass(entry->qualifiedCppName(), MetaClassList::QualifiedCppName)) {
            ReportHandler::warning(QString("type '%1' is specified in typesystem, "
                                           "but not defined. This could potentially "
                                           "lead to compilation errors.")
                                   .arg(entry->qualifiedCppName()));
        }

        if (entry->isEnum()) {
            QString name;
            MetaClass *cls;
            if(static_cast<const EnumTypeEntry *>(entry)->javaQualifier().isEmpty()){
                name = entry->targetTypeSystem() + "." + GLOBAL_PACKAGE;
                cls = m_globals[entry->targetTypeSystem()];
            }else{
                QString pkg = entry->javaPackage();
                name = (pkg.isEmpty() ? QString() : pkg + ".") + static_cast<const EnumTypeEntry *>(entry)->javaQualifier();
                cls = m_meta_classes.findClass(name);
            }

            if(!cls && static_cast<const EnumTypeEntry *>(entry)->javaQualifier().isEmpty()){
                ComplexTypeEntry *gte = new GlobalTypeEntry(GLOBAL_PACKAGE);
                gte->setTargetLangPackage(entry->targetTypeSystem());
                gte->setTargetTypeSystem(entry->targetTypeSystem());
                gte->setCodeGeneration(TypeEntry::GenerateNothing);
                cls = new MetaClass();
                cls->setTypeEntry(gte);
                *cls += MetaAttributes::Final;
                *cls += MetaAttributes::Public;
                *cls += MetaAttributes::Fake;
                m_meta_classes << cls;
                m_globals[entry->targetTypeSystem()] = cls;
                if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(cls->targetTypeSystem())){
                    m_typeSystemByPackage[cls->package()] = ts;
                }
            }

            if(entry->codeGeneration()==TypeEntry::GenerateAll){
                if (!cls) {
                    ReportHandler::warning(QString("namespace '%1' for enum '%2' is not declared")
                                           .arg(name, entry->targetLangName()));
                } else {
                    MetaEnum *e = cls->findEnum(entry->targetLangName());
                    if (!e)
                        ReportHandler::warning(QString("enum '%1' is specified in typesystem, "
                                                       "but not declared")
                                               .arg(entry->qualifiedCppName()));
                }
            }
        }

        if (entry->isFunctional()) {
            QString name;
            MetaClass *cls;
            if(static_cast<const FunctionalTypeEntry *>(entry)->javaQualifier().isEmpty()){
                name = entry->targetTypeSystem() + "." + GLOBAL_PACKAGE;
                cls = m_globals[entry->targetTypeSystem()];
            }else{
                QString pkg = entry->javaPackage();
                name = (pkg.isEmpty() ? QString() : pkg + ".") + static_cast<const FunctionalTypeEntry *>(entry)->javaQualifier();
                cls = m_meta_classes.findClass(name);
            }

            if(!cls && static_cast<const FunctionalTypeEntry *>(entry)->javaQualifier().isEmpty()){
                ComplexTypeEntry *gte = new GlobalTypeEntry(GLOBAL_PACKAGE);
                gte->setTargetLangPackage(entry->targetTypeSystem());
                gte->setTargetTypeSystem(entry->targetTypeSystem());
                gte->setCodeGeneration(TypeEntry::GenerateNothing);
                cls = new MetaClass();
                cls->setTypeEntry(gte);
                *cls += MetaAttributes::Final;
                *cls += MetaAttributes::Public;
                *cls += MetaAttributes::Fake;
                m_meta_classes << cls;
                m_globals[entry->targetTypeSystem()] = cls;
                if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(cls->targetTypeSystem())){
                    m_typeSystemByPackage[cls->package()] = ts;
                }
            }

            if(entry->codeGeneration()==TypeEntry::GenerateAll){
                if (!cls) {
                    ReportHandler::warning(QStringLiteral("namespace '%1' for functional '%2' is not declared")
                                           .arg(name, entry->targetLangName()));
                } else {
                    MetaFunctional *e = cls->findFunctional(entry->targetLangName());
                    if (!e)
                        e = cls->findFunctional(entry->name());
                    if (!e)
                        e = cls->findFunctional(entry->qualifiedCppName());
                    if (!e){
                        e = findFunctional(cls, static_cast<const FunctionalTypeEntry *>(entry));
                    }
                    if (!e)
                        ReportHandler::warning(QStringLiteral("function '%1' is specified in typesystem, "
                                                       "but not declared")
                                               .arg(entry->qualifiedCppName()));
                }
            }
        }
    }

    figureOutEnumValues();
    figureOutDefaultEnumArguments();
    checkFunctionModifications();

    for(MetaClass *cls : qAsConst(m_templates)) {
        setupEquals(cls);
        setupComparable(cls);
        setupBeginEnd(cls);
    }
    for(MetaClass *cls : qAsConst(m_meta_classes)) {
        setupEquals(cls);
        setupComparable(cls);
        setupBeginEnd(cls);
    }
    dumpLog();
    sortLists();
    for(const QList<TypeEntry *>& entries : m_database->allEntries().values()){
        for(const TypeEntry* te : entries){
            if(te->isComplex()){
                const ComplexTypeEntry *centry = static_cast<const ComplexTypeEntry *>(te);
                if(!centry->polymorphicIdValue().isEmpty()){
                    if(MetaClass *cls = classes().findClass(centry->qualifiedCppName())){
                        while (cls!= nullptr && !cls->typeEntry()->isPolymorphicBase())
                            cls = cls->baseClass();
                        if (!cls) {
                            ReportHandler::warning(QString("class '%1' has polymorphic id but does not inherit a polymorphic class")
                                                   .arg(centry->qualifiedCppName()));
                        }
                    }
                }
            }
        }
    }
    return true;
}

void MetaBuilder::applyDocs(const DocModel* docModel){
    if(docModel){
        QMap<QString, QSharedPointer<MetaType>> analyzedTypes;
        MetaFunctionalList meta_functionals = m_meta_functionals;
        const DocNamespace* globalNamespace = docModel->getNamespace("");
        for(MetaClass *meta_class : qAsConst(m_meta_classes)) {
            if(meta_class->isNamespace() || meta_class->isFake()){
                const DocNamespace* ns = meta_class->isFake() ? globalNamespace : docModel->getNamespace(meta_class->qualifiedCppName());
                if(!ns && !meta_class->isFake()){
                    QStringList qualifiedCppName = meta_class->qualifiedCppName().split("::");
                    if(!qualifiedCppName.isEmpty()){
                        if(qualifiedCppName.last().startsWith("QtJambi")){
                            qualifiedCppName.last().replace("QtJambi", "Q");
                            ns = docModel->getNamespace(qualifiedCppName.join("::"));
                            if(!ns && qualifiedCppName.last().endsWith("Util")){
                                qualifiedCppName.last().replace("Util", "");
                                ns = docModel->getNamespace(qualifiedCppName.join("::"));
                            }
                        }
                    }
                }
                if(ns){
                    meta_class->setHref(ns->href());
                    meta_class->setBrief(ns->brief());
                }
                for(MetaFunction * meta_function : meta_class->functions()){
                    for(int i=0; i<meta_function->arguments().size(); ++i){
                        MetaArgument *argument = meta_function->arguments()[i];
                        analyzedTypes[argument->type()->cppSignature()] = QSharedPointer<MetaType>(argument->type()->copy());
                    }
                }
                for(MetaFunction * meta_function : meta_class->functions()){
                    QList<const DocFunction*> functions;
                    if(meta_function->functionType()==MetaFunction::GlobalScopeFunction){
                        if(meta_function->functionTemplate()){
                            if(ns){
                                functions = ns->getFunctions(meta_function->functionTemplate()->originalName());
                            }
                            if(functions.isEmpty() && globalNamespace){
                                functions = globalNamespace->getFunctions(meta_function->functionTemplate()->originalName());
                            }
                        }else{
                            if(ns){
                                functions = ns->getFunctions(meta_function->originalName());
                            }
                            if(functions.isEmpty() && globalNamespace){
                                functions = globalNamespace->getFunctions(meta_function->originalName());
                            }
                        }
                    }else if(!ns){
                        continue;
                    }else{
                        if(meta_function->functionTemplate()){
                            functions = ns->getFunctions(meta_function->functionTemplate()->originalName());
                        }else{
                            functions = ns->getFunctions(meta_function->originalName());
                        }
                    }
                    if(functions.size()==1){
                        meta_function->setHref(functions[0]->href());
                        meta_function->setBrief(functions[0]->brief());
                    }else{
                        for(const DocFunction* function : qAsConst(functions)){
                            if(meta_function->isConstant()==function->isConst()
                                    && (meta_function->isStatic()==function->isStatic() || meta_function->functionType()==MetaFunction::GlobalScopeFunction)
                                    && meta_function->arguments().size()==function->parameters().size()){
                                bool ok = true;
                                for(int i=0; i<meta_function->arguments().size(); ++i){
                                    MetaArgument *argument = meta_function->arguments()[i];
                                    QString arg = function->parameters()[i];
                                    if(arg.endsWith("...")){
                                        arg.chop(3);
                                        if(!meta_function->isVariadics()){
                                            ok = false;
                                            break;
                                        }
                                    }
                                    if(arg.endsWith(" &")){
                                        arg.chop(2);
                                        arg += "&";
                                    }
                                    if(argument->type()->cppSignature()!=arg){
                                        QSharedPointer<MetaType> type = analyzedTypes[arg];
                                        if(!type){
                                            TypeInfo typeInfo = analyzeTypeInfo(meta_class, arg);
                                            type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                            if(!ok || !type){
                                                QString qualifiedName = typeInfo.qualifiedName().join("::");
                                                if(qualifiedName=="ushort"){
                                                    typeInfo.setQualifiedName({"unsigned short"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="uint"){
                                                    typeInfo.setQualifiedName({"unsigned int"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="uchar"){
                                                    typeInfo.setQualifiedName({"unsigned char"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="ulonglong"){
                                                    typeInfo.setQualifiedName({"unsigned long long"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }
                                            }
                                            analyzedTypes[arg] = type;
                                        }
                                        if(ok && type){
                                            if(type->isConstant()!=argument->type()->isConstant()
                                                    || type->getReferenceType()!=argument->type()->getReferenceType()
                                                    || type->indirections()!=argument->type()->indirections()){
                                                ok = false;
                                                break;
                                            }else{
                                                if(type->typeEntry()->isTemplateArgument()
                                                        && argument->type()->typeEntry()->isTemplateArgument()){
                                                    if(type->typeEntry()->qualifiedCppName()!=argument->type()->typeEntry()->qualifiedCppName()){
                                                        ok = false;
                                                        break;
                                                    }
                                                }else if(type->typeEntry()!=argument->type()->typeEntry()
                                                        && !(type->typeEntry()->isQString() && argument->type()->typeEntry()==m_database->qstringType())
                                                        && !(type->typeEntry()->isQVariant() && argument->type()->typeEntry()==m_database->qvariantType())
                                                        && !(type->typeEntry()->isQChar() && argument->type()->typeEntry()==m_database->qcharType())){
                                                    ok = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                if(ok){
                                    meta_function->setHref(function->href());
                                    meta_function->setBrief(function->brief());
                                    break;
                                }else if(meta_function->functionTemplate()){
                                    ok = true;
                                    for(int i=0; i<meta_function->functionTemplate()->arguments().size(); ++i){
                                        MetaArgument *argument = meta_function->functionTemplate()->arguments()[i];
                                        QString arg = function->parameters()[i];
                                        if(arg.endsWith("...")){
                                            arg.chop(3);
                                            if(!meta_function->functionTemplate()->isVariadics()){
                                                ok = false;
                                                break;
                                            }
                                        }
                                        if(arg.endsWith(" &")){
                                            arg.chop(2);
                                            arg += "&";
                                        }
                                        QSharedPointer<MetaType> type = analyzedTypes[arg];
                                        if(!type){
                                            TypeInfo typeInfo = analyzeTypeInfo(meta_class, arg);
                                            type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                            if(!ok || !type){
                                                QString qualifiedName = typeInfo.qualifiedName().join("::");
                                                if(qualifiedName=="ushort"){
                                                    typeInfo.setQualifiedName({"unsigned short"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="uint"){
                                                    typeInfo.setQualifiedName({"unsigned int"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="uchar"){
                                                    typeInfo.setQualifiedName({"unsigned char"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="ulonglong"){
                                                    typeInfo.setQualifiedName({"unsigned long long"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }
                                            }
                                            analyzedTypes[arg] = type;
                                        }
                                        if(ok && type){
                                            if(type->isConstant()!=argument->type()->isConstant()
                                                    || type->getReferenceType()!=argument->type()->getReferenceType()
                                                    || type->indirections()!=argument->type()->indirections()){
                                                ok = false;
                                                break;
                                            }else{
                                                if(type->typeEntry()!=argument->type()->typeEntry()
                                                        && !(type->typeEntry()->isQString() && argument->type()->typeEntry()==m_database->qstringType())
                                                        && !(type->typeEntry()->isQVariant() && argument->type()->typeEntry()==m_database->qvariantType())
                                                        && !(type->typeEntry()->isQChar() && argument->type()->typeEntry()==m_database->qcharType())){
                                                    ok = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if(ok){
                                        meta_function->setHref(function->href());
                                        meta_function->setBrief(function->brief());
                                        break;
                                    }
                                }
                            }
                        }
                        if(meta_function->href().isEmpty()){
                            for(const DocFunction* function : qAsConst(functions)){
                                if(function->minimalSignature()==meta_function->minimalSignatureNoTemplate()
                                    || (meta_function->functionTemplate() && function->minimalSignature()==meta_function->functionTemplate()->minimalSignatureNoTemplate())){
                                    meta_function->setHref(function->href());
                                    meta_function->setBrief(function->brief());
                                    break;
                                }
                            }
                        }
                    }
                }
            }else{
                const DocClass* cls = docModel->getClass(meta_class->qualifiedCppName());
                if(!cls && meta_class->templateBaseClass()){
                    cls = docModel->getClass(meta_class->templateBaseClass()->qualifiedCppName());
                }
                if(!cls){
                    QStringList qualifiedCppName = meta_class->qualifiedCppName().split("::");
                    if(!qualifiedCppName.isEmpty()){
                        if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                            qualifiedCppName.last().replace("<JObjectWrapper>", "");
                            cls = docModel->getClass(qualifiedCppName.join("::"));
                        }else if(qualifiedCppName.last().endsWith("<void>")){
                            qualifiedCppName.last().replace("<void>", "");
                            cls = docModel->getClass(qualifiedCppName.join("::"));
                        }else if(qualifiedCppName.last().startsWith("QtJambi")){
                            qualifiedCppName.last().replace("QtJambi", "Q");
                            cls = docModel->getClass(qualifiedCppName.join("::"));
                        }else if(qualifiedCppName.last().startsWith("QVoid")){
                            qualifiedCppName.last().replace("QVoid", "Q");
                            cls = docModel->getClass(qualifiedCppName.join("::"));
                        }
                        if(!cls && qualifiedCppName.size()>1){
                            qualifiedCppName.takeFirst();
                            cls = docModel->getClass(qualifiedCppName.join("::"));
                        }
                    }
                }
                if(cls){
                    meta_class->setHref(cls->href());
                    meta_class->setBrief(cls->brief());
                    for(MetaFunction * meta_function : meta_class->functions()){
                        for(int i=0; i<meta_function->arguments().size(); ++i){
                            MetaArgument *argument = meta_function->arguments()[i];
                            analyzedTypes[argument->type()->cppSignature()] = QSharedPointer<MetaType>(argument->type()->copy());
                        }
                    }
                    for(MetaFunction * meta_function : meta_class->functions()){
                        const DocClass* _cls = cls;
                        if(meta_function->declaringClass()!=meta_class){
                            const DocClass* __cls = docModel->getClass(meta_function->declaringClass()->qualifiedCppName());
                            if(!__cls && meta_function->declaringClass()->templateBaseClass()){
                                __cls = docModel->getClass(meta_function->declaringClass()->templateBaseClass()->qualifiedCppName());
                            }
                            if(!__cls){
                                QStringList qualifiedCppName = meta_function->declaringClass()->qualifiedCppName().split("::");
                                if(!qualifiedCppName.isEmpty()){
                                    if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                                        qualifiedCppName.last().replace("<JObjectWrapper>", "");
                                        cls = docModel->getClass(qualifiedCppName.join("::"));
                                    }else if(qualifiedCppName.last().endsWith("<void>")){
                                        qualifiedCppName.last().replace("<void>", "");
                                        cls = docModel->getClass(qualifiedCppName.join("::"));
                                    }else if(qualifiedCppName.last().startsWith("QtJambi")){
                                        qualifiedCppName.last().replace("QtJambi", "Q");
                                        __cls = docModel->getClass(qualifiedCppName.join("::"));
                                    }else if(qualifiedCppName.last().startsWith("QVoid")){
                                        qualifiedCppName.last().replace("QVoid", "Q");
                                        __cls = docModel->getClass(qualifiedCppName.join("::"));
                                    }
                                    if(!__cls && qualifiedCppName.size()>1){
                                        qualifiedCppName.takeFirst();
                                        __cls = docModel->getClass(qualifiedCppName.join("::"));
                                    }
                                }
                            }
                            if(__cls){
                                _cls = __cls;
                            }
                        }
                        QList<const DocFunction*> functions;
                        functions = _cls->getFunctions(meta_function->originalName());
                        if(functions.isEmpty() && meta_function->functionType()==MetaFunction::GlobalScopeFunction && globalNamespace){
                            functions = globalNamespace->getFunctions(meta_function->originalName());
                        }
                        if(functions.size()==1){
                            meta_function->setHref(functions[0]->href());
                            meta_function->setBrief(functions[0]->brief());
                        }else for(const DocFunction* function : qAsConst(functions)){
                            if(meta_function->isConstant()==function->isConst()
                                    && meta_function->isStatic()==function->isStatic()
                                    && meta_function->arguments().size()==function->parameters().size()){
                                bool ok = true;
                                if(functions.size()>1){
                                    for(int i=0; i<meta_function->arguments().size(); ++i){
                                        MetaArgument *argument = meta_function->arguments()[i];
                                        QString arg = function->parameters()[i];
                                        if(arg.endsWith("...")){
                                            arg.chop(3);
                                            if(!meta_function->isVariadics()){
                                                ok = false;
                                                break;
                                            }
                                        }
                                        if(arg.endsWith(" &")){
                                            arg.chop(2);
                                            arg += "&";
                                        }
                                        QSharedPointer<MetaType> type = analyzedTypes[arg];
                                        if(!type){
                                            TypeInfo typeInfo = analyzeTypeInfo(meta_class, arg);
                                            type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                            if(!ok || !type){
                                                QString qualifiedName = typeInfo.qualifiedName().join("::");
                                                if(qualifiedName=="ushort"){
                                                    typeInfo.setQualifiedName({"unsigned short"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="uint"){
                                                    typeInfo.setQualifiedName({"unsigned int"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="uchar"){
                                                    typeInfo.setQualifiedName({"unsigned char"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }else if(qualifiedName=="ulonglong"){
                                                    typeInfo.setQualifiedName({"unsigned long long"});
                                                    type = QSharedPointer<MetaType>(translateType(typeInfo, &ok, ""));
                                                }
                                            }
                                            analyzedTypes[arg] = type;
                                        }
                                        if(ok && type){
                                            if(type->isConstant()!=argument->type()->isConstant()
                                                    || type->getReferenceType()!=argument->type()->getReferenceType()
                                                    || type->indirections()!=argument->type()->indirections()){
                                                ok = false;
                                                break;
                                            }else{
                                                if(type->typeEntry()!=argument->type()->typeEntry()
                                                        && !(type->typeEntry()->isQString() && argument->type()->typeEntry()==m_database->qstringType())
                                                        && !(type->typeEntry()->isQVariant() && argument->type()->typeEntry()==m_database->qvariantType())
                                                        && !(type->typeEntry()->isQChar() && argument->type()->typeEntry()==m_database->qcharType())){
                                                    ok = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                if(ok){
                                    meta_function->setHref(function->href());
                                    meta_function->setBrief(function->brief());
                                    break;
                                }
                            }
                        }
                        if(meta_function->href().isEmpty() && meta_function->brief().isEmpty() && meta_function->propertySpec()){
                                if(const DocProperty* prop = _cls->getProperty(meta_function->propertySpec()->name())){
                                    meta_function->setHref(prop->href());
                                    meta_function->setBrief(prop->brief());
                                }
                        }
                        if(meta_function->href().isEmpty()){
                            for(const DocFunction* function : qAsConst(functions)){
                                if(function->minimalSignature()==meta_function->minimalSignatureNoTemplate()
                                        || (meta_function->functionTemplate() && function->minimalSignature()==meta_function->functionTemplate()->minimalSignatureNoTemplate())){
                                    meta_function->setHref(function->href());
                                    meta_function->setBrief(function->brief());
                                    break;
                                }
                            }
                        }
                    }

                    for(MetaField * meta_field : meta_class->fields()){
                        if(const DocVariable* variable = cls->getVariable(meta_field->name())){
                            meta_field->setHref(variable->href());
                            meta_field->setBrief(variable->brief());
                        }
                    }
                }

                for(MetaEnum* meta_enum : meta_class->enums()){
                    const DocEnum* docEnum = docModel->getEnum(meta_enum->typeEntry()->qualifiedCppName());
                    if(!docEnum && meta_class->templateBaseClass()){
                        docEnum = docModel->getEnum(meta_class->templateBaseClass()->qualifiedCppName()+"::"+meta_enum->typeEntry()->name());
                    }
                    if(!docEnum){
                        QStringList qualifiedCppName = meta_enum->typeEntry()->qualifiedCppName().split("::");
                        if(!qualifiedCppName.isEmpty()){
                            if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                                qualifiedCppName.last().replace("<JObjectWrapper>", "");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::"));
                            }else if(qualifiedCppName.last().endsWith("<void>")){
                                qualifiedCppName.last().replace("<void>", "");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::"));
                            }else if(qualifiedCppName.last().startsWith("QtJambi")){
                                qualifiedCppName.last().replace("QtJambi", "Q");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::"));
                            }else if(qualifiedCppName.last().startsWith("QVoid")){
                                qualifiedCppName.last().replace("QVoid", "Q");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::"));
                            }
                            if(!docEnum && qualifiedCppName.size()>1){
                                qualifiedCppName.takeFirst();
                                docEnum = docModel->getEnum(qualifiedCppName.join("::"));
                            }
                        }
                    }
                    if(!docEnum){
                        docEnum = docModel->getEnum(meta_class->qualifiedCppName()+"::"+meta_enum->typeEntry()->name());
                        if(!docEnum){
                            QStringList qualifiedCppName = meta_class->qualifiedCppName().split("::");
                            if(!qualifiedCppName.isEmpty()){
                                if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                                    qualifiedCppName.last().replace("<JObjectWrapper>", "");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }else if(qualifiedCppName.last().endsWith("<void>")){
                                    qualifiedCppName.last().replace("<void>", "");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }else if(qualifiedCppName.last().startsWith("QtJambi")){
                                    qualifiedCppName.last().replace("QtJambi", "Q");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }else if(qualifiedCppName.last().startsWith("QVoid")){
                                    qualifiedCppName.last().replace("QVoid", "Q");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }
                                if(!docEnum && qualifiedCppName.size()>1){
                                    qualifiedCppName.takeFirst();
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }
                            }
                        }
                    }
                    if(!docEnum){
                        docEnum = docModel->getEnum(meta_enum->typeEntry()->qualifier()+"::"+meta_enum->typeEntry()->name());
                        if(!docEnum){
                            QStringList qualifiedCppName = meta_enum->typeEntry()->qualifier().split("::");
                            if(!qualifiedCppName.isEmpty()){
                                if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                                    qualifiedCppName.last().replace("<JObjectWrapper>", "");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }else if(qualifiedCppName.last().endsWith("<void>")){
                                    qualifiedCppName.last().replace("<void>", "");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }else if(qualifiedCppName.last().startsWith("QtJambi")){
                                    qualifiedCppName.last().replace("QtJambi", "Q");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }else if(qualifiedCppName.last().startsWith("QVoid")){
                                    qualifiedCppName.last().replace("QVoid", "Q");
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }
                                if(!docEnum && qualifiedCppName.size()>1){
                                    qualifiedCppName.takeFirst();
                                    docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                                }
                            }
                        }
                    }
                    if(docEnum){
                        meta_enum->setHref(docEnum->href());
                        meta_enum->setBrief(docEnum->brief());
                    }
                }

                meta_functionals << meta_class->functionals();
            }
        }
        for(MetaFunctional *meta_class : qAsConst(meta_functionals)) {
            const DocClass* cls = docModel->getClass(meta_class->typeEntry()->qualifiedCppName());
            if(!cls){
                QStringList qualifiedCppName = meta_class->typeEntry()->qualifiedCppName().split("::");
                if(!qualifiedCppName.isEmpty()){
                    if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                        qualifiedCppName.last().replace("<JObjectWrapper>", "");
                        cls = docModel->getClass(qualifiedCppName.join("::"));
                    }else if(qualifiedCppName.last().endsWith("<void>")){
                        qualifiedCppName.last().replace("<void>", "");
                        cls = docModel->getClass(qualifiedCppName.join("::"));
                    }else if(qualifiedCppName.last().startsWith("QtJambi")){
                        qualifiedCppName.last().replace("QtJambi", "Q");
                        cls = docModel->getClass(qualifiedCppName.join("::"));
                    }else if(qualifiedCppName.last().startsWith("QVoid")){
                        qualifiedCppName.last().replace("QVoid", "Q");
                        cls = docModel->getClass(qualifiedCppName.join("::"));
                    }
                    if(!cls && qualifiedCppName.size()>1){
                        qualifiedCppName.takeFirst();
                        cls = docModel->getClass(qualifiedCppName.join("::"));
                    }
                }
                if(!cls && meta_class->enclosingClass()){
                    QString functionalName = qualifiedCppName.takeLast();
                    const DocClass* embeddingClass = docModel->getClass(meta_class->enclosingClass()->typeEntry()->qualifiedCppName());
                    if(!embeddingClass
                            && !( qualifiedCppName = meta_class->enclosingClass()->typeEntry()->qualifiedCppName().split("::") ).isEmpty()
                            && qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                        qualifiedCppName.last().replace("<JObjectWrapper>", "");
                        embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                        if(!embeddingClass && qualifiedCppName.size()>1){
                            qualifiedCppName.takeFirst();
                            embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                        }
                    }else if(!embeddingClass
                             && !( qualifiedCppName = meta_class->enclosingClass()->typeEntry()->qualifiedCppName().split("::") ).isEmpty()
                             && qualifiedCppName.last().endsWith("<void>")){
                        qualifiedCppName.last().replace("<void>", "");
                        embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                        if(!embeddingClass && qualifiedCppName.size()>1){
                            qualifiedCppName.takeFirst();
                            embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                        }
                    }else if(!embeddingClass
                            && !( qualifiedCppName = meta_class->enclosingClass()->typeEntry()->qualifiedCppName().split("::") ).isEmpty()
                            && qualifiedCppName.last().startsWith("QtJambi")){
                        qualifiedCppName.last().replace("QtJambi", "Q");
                        embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                        if(!embeddingClass && qualifiedCppName.size()>1){
                            qualifiedCppName.takeFirst();
                            embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                        }
                    }else if(!embeddingClass
                             && !( qualifiedCppName = meta_class->enclosingClass()->typeEntry()->qualifiedCppName().split("::") ).isEmpty()
                             && qualifiedCppName.last().startsWith("QVoid")){
                         qualifiedCppName.last().replace("QVoid", "Q");
                         embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                         if(!embeddingClass && qualifiedCppName.size()>1){
                             qualifiedCppName.takeFirst();
                             embeddingClass = docModel->getClass(qualifiedCppName.join("::"));
                         }
                     }
                    if(embeddingClass){
                        if(const DocTypeDef* def = embeddingClass->getTypeDef(functionalName)){
                            meta_class->setHref(def->href());
                            meta_class->setBrief(def->brief());
                        }
                    }
                }
            }
            if(cls){
                meta_class->setHref(cls->href());
                meta_class->setBrief(cls->brief());
            }
        }
        for(MetaEnum* meta_enum : qAsConst(m_enums)){
            if(meta_enum->href().isEmpty() && meta_enum->brief().isEmpty()){
                const DocEnum* docEnum = docModel->getEnum(meta_enum->typeEntry()->qualifiedCppName());
                if(!docEnum){
                    docEnum = docModel->getEnum(meta_enum->typeEntry()->qualifier()+"::"+meta_enum->typeEntry()->name());
                    if(!docEnum){
                        QStringList qualifiedCppName = meta_enum->typeEntry()->qualifier().split("::");
                        if(!qualifiedCppName.isEmpty()){
                            if(qualifiedCppName.last().endsWith("<JObjectWrapper>")){
                                qualifiedCppName.last().replace("<JObjectWrapper>", "");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                            }else if(qualifiedCppName.last().endsWith("<void>")){
                                qualifiedCppName.last().replace("<void>", "");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                            }else if(qualifiedCppName.last().startsWith("QtJambi")){
                                qualifiedCppName.last().replace("QtJambi", "Q");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                            }else if(qualifiedCppName.last().startsWith("QVoid")){
                                qualifiedCppName.last().replace("QVoid", "Q");
                                docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                            }
                            if(!docEnum && qualifiedCppName.size()>1){
                                qualifiedCppName.takeFirst();
                                docEnum = docModel->getEnum(qualifiedCppName.join("::")+"::"+meta_enum->typeEntry()->name());
                            }
                        }
                    }
                }
                if(docEnum){
                    meta_enum->setHref(docEnum->href());
                    meta_enum->setBrief(docEnum->brief());
                }
            }
        }
    }
}

void analyzeFunctional(MetaFunctional* meta_functional, const QList<MetaArgument*>& actualArguments){
    const QString& oldFunctionName = meta_functional->typeEntry()->functionName();
    QString modifiedReturnType = meta_functional->typeReplaced(0);
    MetaType * type = meta_functional->type();
    if(type || (!modifiedReturnType.isEmpty() && modifiedReturnType!="void")){
        if((type && (type->isPrimitive() || type->isQChar()) && modifiedReturnType.isEmpty())
                || modifiedReturnType=="byte"
                || modifiedReturnType=="int"
                || modifiedReturnType=="long"
                || modifiedReturnType=="short"
                || modifiedReturnType=="float"
                || modifiedReturnType=="double"
                || modifiedReturnType=="char"
                || modifiedReturnType=="boolean"){
            if(modifiedReturnType.isEmpty() && type){
                modifiedReturnType = type->typeEntry()->qualifiedTargetLangName();
            }
            if(modifiedReturnType=="boolean"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsBoolean")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsBoolean");
                    meta_functional->setJavaFunctionalInterface("java.util.function.BooleanSupplier");
                    break;
                case 1:{
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="test")
                        return;
                    meta_functional->typeEntry()->setFunctionName("test");
                    QString modifiedArgType = meta_functional->typeReplaced(actualArguments[0]->argumentIndex()+1);
                    if(modifiedArgType=="double"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.DoublePredicate");
                    }else if(modifiedArgType=="int"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.IntPredicate");
                    }else if(modifiedArgType=="long"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.LongPredicate");
                    }else if(modifiedArgType.isEmpty()){
                        if(actualArguments[0]->type()->isPrimitive() || actualArguments[0]->type()->isQChar()){
                            if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                                meta_functional->setJavaFunctionalInterface("java.util.function.DoublePredicate");
                            }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                                meta_functional->setJavaFunctionalInterface("java.util.function.IntPredicate");
                            }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                                meta_functional->setJavaFunctionalInterface("java.util.function.LongPredicate");
                            }
                        }else{
                            meta_functional->setJavaFunctionalInterface("java.util.function.Predicate");
                            meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                        }
                    }else if(modifiedArgType!="char"
                             && modifiedArgType!="boolean"
                             && modifiedArgType!="byte"
                             && modifiedArgType!="short"
                             && modifiedArgType!="float"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.Predicate");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                    }
                    break;
                }
                case 2:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="test")
                        return;
                    meta_functional->typeEntry()->setFunctionName("test");
                    if(!actualArguments[0]->type()->isPrimitive() && !actualArguments[1]->type()->isPrimitive()){
                        meta_functional->setJavaFunctionalInterface("java.util.function.BiPredicate");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),
                                                                                   uint(actualArguments[1]->argumentIndex()+1)});
                    }
                    break;
                default:break;
                }
            }else if(modifiedReturnType=="char"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsChar")
                        return;
                    meta_functional->setJavaFunctionalInterface("QtUtilities$CharSupplier");
                    meta_functional->typeEntry()->setFunctionName("getAsChar");
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsChar")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsChar");
                    break;
                }
            }else if(modifiedReturnType=="byte"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsByte")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsByte");
                    meta_functional->setJavaFunctionalInterface("QtUtilities$ByteSupplier");
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsByte")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsByte");
                    break;
                }
            }else if(modifiedReturnType=="short"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsShort")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsShort");
                    meta_functional->setJavaFunctionalInterface("QtUtilities$ShortSupplier");
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsShort")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsShort");
                    break;
                }
            }else if(modifiedReturnType=="int"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsInt")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsInt");
                    meta_functional->setJavaFunctionalInterface("java.util.function.IntSupplier");
                    break;
                case 1:{
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsInt")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsInt");
                    QString modifiedArgType = meta_functional->typeReplaced(actualArguments[0]->argumentIndex()+1);
                    if(modifiedArgType=="double"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.DoubleToIntFunction");
                    }else if(modifiedArgType=="int"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.IntUnaryOperator");
                    }else if(modifiedArgType=="long"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.LongToIntFunction");
                    }else if(modifiedArgType.isEmpty() && actualArguments[0]->type()->isPrimitive()){
                        if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.IntUnaryOperator");
                        }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.LongToIntFunction");
                        }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.DoubleToIntFunction");
                        }
                    }else{
                        meta_functional->setJavaFunctionalInterface("java.util.function.ToIntFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                    }
                    break;
                }
                case 2:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsInt")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsInt");
                    if(!actualArguments[0]->type()->isPrimitive() && !actualArguments[1]->type()->isPrimitive()){
                        meta_functional->setJavaFunctionalInterface("java.util.function.ToIntBiFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),
                                                                                   uint(actualArguments[1]->argumentIndex()+1)});
                    }else if(actualArguments[0]->type()->isPrimitive() && actualArguments[1]->type()->isPrimitive()
                             && actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"
                             && actualArguments[1]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.IntBinaryOperator");
                    }
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsInt")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsInt");
                    break;
                }
            }else if(modifiedReturnType=="float"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsFloat")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsFloat");
                    meta_functional->setJavaFunctionalInterface("QtUtilities$FloatSupplier");
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsFloat")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsFloat");
                    break;
                }
            }else if(modifiedReturnType=="double"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsDouble")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsDouble");
                    meta_functional->setJavaFunctionalInterface("java.util.function.DoubleSupplier");
                    break;
                case 1:{
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsDouble")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsDouble");
                    QString modifiedArgType = meta_functional->typeReplaced(actualArguments[0]->argumentIndex()+1);
                    if(modifiedArgType=="double"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.DoubleUnaryOperator");
                    }else if(modifiedArgType=="int"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.IntToDoubleFunction");
                    }else if(modifiedArgType=="long"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.LongToDoubleFunction");
                    }else if(modifiedArgType.isEmpty() && actualArguments[0]->type()->isPrimitive()){
                        if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.IntToDoubleFunction");
                        }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.LongToDoubleFunction");
                        }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.DoubleUnaryOperator");
                        }
                    }else{
                        meta_functional->setJavaFunctionalInterface("java.util.function.ToDoubleFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                    }
                    break;
                }
                case 2:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsDouble")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsDouble");
                    if(!actualArguments[0]->type()->isPrimitive() && !actualArguments[1]->type()->isPrimitive()){
                        meta_functional->setJavaFunctionalInterface("java.util.function.ToDoubleBiFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),
                                                                                   uint(actualArguments[1]->argumentIndex()+1)});
                    }else if(actualArguments[0]->type()->isPrimitive() && actualArguments[1]->type()->isPrimitive()
                             && actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"
                             && actualArguments[1]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.DoubleBinaryOperator");
                    }
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsDouble")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsDouble");
                    break;
                }
            }else if(modifiedReturnType=="long"){
                switch(actualArguments.size()){
                case 0:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="getAsLong")
                        return;
                    meta_functional->typeEntry()->setFunctionName("getAsLong");
                    meta_functional->setJavaFunctionalInterface("java.util.function.LongSupplier");
                    break;
                case 1:{
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsLong")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsLong");
                    QString modifiedArgType = meta_functional->typeReplaced(actualArguments[0]->argumentIndex()+1);
                    if(modifiedArgType=="double"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.DoubleToLongFunction");
                    }else if(modifiedArgType=="int"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.IntToLongFunction");
                    }else if(modifiedArgType=="long"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.LongUnaryOperator");
                    }else if(modifiedArgType.isEmpty() && actualArguments[0]->type()->isPrimitive()){
                        if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.IntToLongFunction");
                        }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.LongUnaryOperator");
                        }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                            meta_functional->setJavaFunctionalInterface("java.util.function.DoubleToLongFunction");
                        }
                    }else{
                        meta_functional->setJavaFunctionalInterface("java.util.function.ToLongFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                    }
                    break;
                }
                case 2:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsLong")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsLong");
                    if(!actualArguments[0]->type()->isPrimitive() && !actualArguments[1]->type()->isPrimitive()){
                        meta_functional->setJavaFunctionalInterface("java.util.function.ToLongBiFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),
                                                                                   uint(actualArguments[1]->argumentIndex()+1)});
                    }else if(actualArguments[0]->type()->isPrimitive() && actualArguments[1]->type()->isPrimitive()
                             && actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"
                             && actualArguments[1]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.LongBinaryOperator");
                    }
                    break;
                default:
                    if(!oldFunctionName.isEmpty() && oldFunctionName!="applyAsLong")
                        return;
                    meta_functional->typeEntry()->setFunctionName("applyAsLong");
                    break;
                }
            } // other primitive types unsupported
        }else{
            // non-primitive types
            switch(actualArguments.size()){
            case 0:
                if(!oldFunctionName.isEmpty() && oldFunctionName!="get")
                    return;
                meta_functional->typeEntry()->setFunctionName("get");
                meta_functional->setJavaFunctionalInterface("java.util.function.Supplier");
                meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                break;
            case 1:{
                if(!oldFunctionName.isEmpty() && oldFunctionName!="apply")
                    return;
                meta_functional->typeEntry()->setFunctionName("apply");
                QString modifiedArgType = meta_functional->typeReplaced(actualArguments[0]->argumentIndex()+1);
                if(modifiedArgType=="double"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.DoubleFunction");
                    meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                }else if(modifiedArgType=="int"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.IntFunction");
                    meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                }else if(modifiedArgType=="long"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.LongFunction");
                    meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                }else if(modifiedArgType.isEmpty() && actualArguments[0]->type()->isPrimitive()){
                    if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.IntFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                    }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.LongFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                    }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                        meta_functional->setJavaFunctionalInterface("java.util.function.DoubleFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                    }
                }else if(type && type->typeEntry()==actualArguments[0]->type()->typeEntry()
                         && type->typeUsagePattern()==actualArguments[0]->type()->typeUsagePattern()){
                     meta_functional->setJavaFunctionalInterface("java.util.function.UnaryOperator");
                     meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                 }else{
                     meta_functional->setJavaFunctionalInterface("java.util.function.Function");
                     meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),0});
                 }
                break;
            }
            case 2:
                if(!oldFunctionName.isEmpty() && oldFunctionName!="apply")
                    return;
                meta_functional->typeEntry()->setFunctionName("apply");
                if(!actualArguments[0]->type()->isPrimitive() && !actualArguments[1]->type()->isPrimitive()){
                    if(type && type->typeEntry()==actualArguments[0]->type()->typeEntry()
                            && type->typeUsagePattern()==actualArguments[0]->type()->typeUsagePattern()
                            && type->typeEntry()==actualArguments[1]->type()->typeEntry()
                            && type->typeUsagePattern()==actualArguments[1]->type()->typeUsagePattern()){
                        meta_functional->setJavaFunctionalInterface("java.util.function.BinaryOperator");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({0});
                    }else{
                        meta_functional->setJavaFunctionalInterface("java.util.function.BiFunction");
                        meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),
                                                                                   uint(actualArguments[1]->argumentIndex()+1),0});
                    }
                }
                break;
            default:
                if(!oldFunctionName.isEmpty() && oldFunctionName!="apply")
                    return;
                meta_functional->typeEntry()->setFunctionName("apply");
                break;
            }
        }
    }else{
        // consumer
        switch(actualArguments.size()){
        case 0:
            if(!oldFunctionName.isEmpty() && oldFunctionName!="run")
                return;
            meta_functional->typeEntry()->setFunctionName("run");
            meta_functional->setJavaFunctionalInterface("java.lang.Runnable");
            break;
        case 1:{
            if(!oldFunctionName.isEmpty() && oldFunctionName!="accept")
                return;
            meta_functional->typeEntry()->setFunctionName("accept");
            QString modifiedArgType = meta_functional->typeReplaced(actualArguments[0]->argumentIndex()+1);
            if(modifiedArgType=="double"){
                meta_functional->setJavaFunctionalInterface("java.util.function.DoubleConsumer");
            }else if(modifiedArgType=="int"){
                meta_functional->setJavaFunctionalInterface("java.util.function.IntConsumer");
            }else if(modifiedArgType=="long"){
                meta_functional->setJavaFunctionalInterface("java.util.function.LongConsumer");
            }else if(modifiedArgType.isEmpty() && actualArguments[0]->type()->isPrimitive()){
                if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.DoubleConsumer");
                }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.IntConsumer");
                }else if(actualArguments[0]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.LongConsumer");
                }
            }else{
                meta_functional->setJavaFunctionalInterface("java.util.function.Consumer");
                meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
            }
            break;
        }
        case 2:{
            if(!oldFunctionName.isEmpty() && oldFunctionName!="accept")
                return;
            meta_functional->typeEntry()->setFunctionName("accept");
            QString modifiedArgType2 = meta_functional->typeReplaced(actualArguments[1]->argumentIndex()+1);
            if(modifiedArgType2=="double"){
                meta_functional->setJavaFunctionalInterface("java.util.function.ObjDoubleConsumer");
                meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
            }else if(modifiedArgType2=="int"){
                meta_functional->setJavaFunctionalInterface("java.util.function.ObjIntConsumer");
                meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
            }else if(modifiedArgType2=="long"){
                meta_functional->setJavaFunctionalInterface("java.util.function.ObjLongConsumer");
                meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
            }else if(modifiedArgType2.isEmpty() && actualArguments[1]->type()->isPrimitive()){
                if(actualArguments[1]->type()->typeEntry()->qualifiedTargetLangName()=="double"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.ObjDoubleConsumer");
                    meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                }else if(actualArguments[1]->type()->typeEntry()->qualifiedTargetLangName()=="int"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.ObjIntConsumer");
                    meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                }else if(actualArguments[1]->type()->typeEntry()->qualifiedTargetLangName()=="long"){
                    meta_functional->setJavaFunctionalInterface("java.util.function.ObjLongConsumer");
                    meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1)});
                }
            }else{
                meta_functional->setJavaFunctionalInterface("java.util.function.BiConsumer");
                meta_functional->setJavaFunctionalInterfaceParameterTypes({uint(actualArguments[0]->argumentIndex()+1),
                                                                           uint(actualArguments[1]->argumentIndex()+1)});
            }
            break;
        }
        default:
            if(!oldFunctionName.isEmpty() && oldFunctionName!="accept")
                return;
            meta_functional->typeEntry()->setFunctionName("accept");
            break;
        }
    }
    if(meta_functional->typeEntry()->codeGeneration()==TypeEntry::GenerateNothing && !meta_functional->javaFunctionalInterface().isEmpty()){
        auto idx = meta_functional->javaFunctionalInterface().lastIndexOf('.');
        if(idx>0){
            meta_functional->typeEntry()->setGenericClass(!meta_functional->javaFunctionalInterfaceParameterTypes().isEmpty());
            meta_functional->typeEntry()->setQualifier({});
            meta_functional->typeEntry()->setTargetLangName(meta_functional->javaFunctionalInterface().mid(idx+1));
            meta_functional->typeEntry()->setTargetLangPackage(meta_functional->javaFunctionalInterface().mid(0, idx));
        }
    }
}

MetaFunctional * MetaBuilder::findFunctional(MetaClass *cls, const FunctionalTypeEntry * fentry)
{
    MetaFunctional *e = nullptr;
    const QString& _using = fentry->getUsing();
    if(!_using.isEmpty()){
        FunctionalTypeEntry *ftype = const_cast<FunctionalTypeEntry *>(fentry);
        if(ftype){
            TypeInfo usingType = analyzeTypeInfo(cls, _using);
            bool is_qxp = false;
            if(usingType.qualifiedName()==QStringList{"std", "function"}
                    || (is_qxp = usingType.qualifiedName()==QStringList{"qxp", "function_ref"})
                    || usingType.isFunctionPointer()){
                QString normalizedSignature = usingType.isFunctionPointer() ? "" : (is_qxp ? "qxp::function_ref<" : "std::function<");
                QString newUsing = usingType.isFunctionPointer() ? "FunctionPointerPrivate::fn<" : (is_qxp ? "qxp::function_ref<" : "std::function<");
                std::unique_ptr<MetaFunctional> meta_functional;
                meta_functional.reset(new MetaFunctional());
                meta_functional->setTypeEntry(ftype);
                meta_functional->setBaseTypeName(fentry->targetLangName());
                bool ok = false;
                meta_functional->setFunctionPointer(usingType.isFunctionPointer());
                MetaType * type = translateType(usingType.functionalReturnType(), &ok, QString("traverseFunctional %1").arg(fentry->name()));
                if(ok){
                    newUsing += type ? type->minimalSignature() : "void";
                    normalizedSignature += type ? type->normalizedSignature() : "void";
                    if(type && type->typeEntry()->isComplex()){
                        ftype->addExtraInclude(dynamic_cast<const ComplexTypeEntry*>(type->typeEntry())->include());
                    }
                    if(usingType.isFunctionPointer()){
                        newUsing += ",";
                        normalizedSignature += "(*)(";
                    }else{
                        newUsing += "(";
                        normalizedSignature += "(";
                    }
                    meta_functional->setType(type);
                    int counter = 0;
                    QList<MetaArgument*> actualArguments;
                    for(const TypeInfo& arg : usingType.functionalArgumentTypes()){
                        MetaType * atype = translateType(arg, &ok, QString("traverseFunctional %1").arg(fentry->name()));
                        if(ok){
                            if(atype && atype->typeEntry()->isComplex()){
                                ftype->addExtraInclude(dynamic_cast<const ComplexTypeEntry*>(atype->typeEntry())->include());
                            }
                            MetaArgument* argument = new MetaArgument();
                            argument->setArgumentIndex(counter);
                            argument->setType(atype);
                            if(counter<usingType.functionalArgumentNames().size())
                                argument->setName(usingType.functionalArgumentNames()[counter]);
                            for(const ArgumentModification& mod : ftype->argumentModification()){
                                if(mod.index==counter+1 && !mod.modified_name.isEmpty()){
                                    argument->setName(mod.modified_name);
                                }
                            }
                            meta_functional->addArgument(argument);
                            if(counter>0){
                                newUsing += ",";
                                normalizedSignature += ",";
                            }
                            newUsing += atype->minimalSignature();
                            normalizedSignature += atype->normalizedSignature();
                            if(!meta_functional->argumentRemoved(counter + 1))
                                actualArguments << argument;
                            ++counter;
                        }
                    }
                    analyzeFunctional(meta_functional.get(), actualArguments);
                    normalizedSignature += ")";
                    if(!usingType.isFunctionPointer()){
                        newUsing += ")>";
                        normalizedSignature += ">";
                    }else{
                        newUsing += ">::type";
                    }
                    e = meta_functional.release();
                    if(cls){
                        cls->addFunctional(e);
                        if(!usingType.isFunctionPointer()){
                            cls->addFunctionalByUsing(_using, e);
                            cls->addFunctionalByUsing(QMetaObject::normalizedSignature(qPrintable(_using)), e);
                            cls->addFunctionalByUsing(QMetaObject::normalizedSignature(qPrintable(newUsing)), e);
                        }
                        cls->addFunctionalByUsing(newUsing, e);
                        cls->addFunctionalByUsing(normalizedSignature, e);
                    }else{
                        addFunctional(e);
                    }
                    ftype->setUsing(newUsing);
                    ftype->setNormalizedSignature(normalizedSignature);
                }
            }
        }
    }
    return e;
}

TypeInfo MetaBuilder::convertInfo(MetaClass *cls, const TypeParser::Info& ti){
    TypeInfo info;
    info.setConstant(ti.is_constant);
    info.setVolatile(ti.is_volatile);
    info.setReferenceType(TypeInfo::ReferenceType(ti.reference_type));
    info.setArrayElements(ti.arrays);
    info.setIndirections(ti.indirections);
    info.setFunctionPointer(ti.is_functionPointer);
    if(!ti.qualified_name.isEmpty()){
        QString typeName = ti.qualified_name.join("::");
        if(!cls){
            info.setQualifiedName(ti.qualified_name);
        }else if(cls->findEnum(typeName)){
            info.setQualifiedName(cls->qualifiedCppName().split("::") << ti.qualified_name);
        }else if(m_meta_classes.findClass(cls->qualifiedCppName()+"::"+typeName)){
            info.setQualifiedName(cls->qualifiedCppName().split("::") << ti.qualified_name);
        }else{
            info.setQualifiedName(ti.qualified_name);
        }
    }
    if(!ti.functionalReturnType.isEmpty()){
        info.setFunctionalReturnType(convertInfo(cls, ti.functionalReturnType.first()));
        QList<TypeInfo> functionalArgumentTypes;
        for(const TypeParser::Info& ins : ti.functionalArgumentTypes){
            functionalArgumentTypes << convertInfo(cls, ins);
        }
        info.setFunctionalArgumentTypes(functionalArgumentTypes);
    }
    for(const TypeParser::Info& ins : ti.arguments){
        info.addArgument(convertInfo(cls, ins));
    }
    return info;
}

TypeInfo MetaBuilder::analyzeTypeInfo(MetaClass *cls, QString strg){
    return convertInfo(cls, TypeParser::parse(strg));
}

void MetaBuilder::addFunctional(MetaFunctional *cls) {
    if (!cls)
        return;
    cls->setOriginalAttributes(cls->attributes());
    m_meta_functionals << cls;
    MetaClass *_cls = m_globals[cls->typeEntry()->targetTypeSystem()];
    if(!_cls){
        ComplexTypeEntry *gte = new GlobalTypeEntry(GLOBAL_PACKAGE);
        gte->setTargetLangPackage(cls->typeEntry()->targetTypeSystem());
        gte->setTargetTypeSystem(cls->typeEntry()->targetTypeSystem());
        gte->setCodeGeneration(TypeEntry::GenerateNothing);
        _cls = new MetaClass();
        _cls->setTypeEntry(gte);
        *_cls += MetaAttributes::Final;
        *_cls += MetaAttributes::Public;
        *_cls += MetaAttributes::Fake;
        m_meta_classes << _cls;
        m_globals[cls->typeEntry()->targetTypeSystem()] = _cls;
        if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(_cls->targetTypeSystem())){
            m_typeSystemByPackage[_cls->package()] = ts;
        }
    }
    _cls->addFunctional(cls);
}


void MetaBuilder::addClass(MetaClass *cls) {
    if (!cls)
        return;

    cls->setOriginalAttributes(cls->attributes());
    if (cls->typeEntry()->isTemplate()
            || (cls->typeEntry()->isContainer()
                && static_cast<const ContainerTypeEntry *>(cls->typeEntry())->type()!=ContainerTypeEntry::StringListContainer)) {
        m_templates << cls;
        ReportHandler::debugSparse(" -> template "+cls->qualifiedCppName());
    } else {
        if(cls->typeEntry()->isIterator()){
            const IteratorTypeEntry* entry = static_cast<const IteratorTypeEntry*>(cls->typeEntry());
            if(entry->containerType() && entry->containerType()->isContainer()){
                m_template_iterators << cls;
                ReportHandler::debugSparse(" -> iterator "+cls->qualifiedCppName());
                return;
            }
        }
        {
            QString fullQualifiedName = cls->typeEntry()->isGlobal() ? cls->typeEntry()->targetLangName() : cls->fullQualifiedCppName();
            if(m_classNames.contains(fullQualifiedName)){
                //ReportHandler::warning("Duplicate class: "+fullQualifiedName);
            }
            m_classNames.insert(fullQualifiedName, {});
        }
        m_meta_classes << cls;
        if(m_current_class){
            if(!cls->qualifiedCppName().startsWith(m_current_class->qualifiedCppName())){
                QStringList qualifiedCppName = cls->qualifiedCppName().split("::");
                if(qualifiedCppName.size()>1){
                    qualifiedCppName.removeLast();
                    m_pendingScopedClasses.append({cls, qualifiedCppName.join("::")});
                }
            }
        }else if(cls->qualifiedCppName().contains("::")){
            QStringList qualifiedCppName = cls->qualifiedCppName().split("::");
            if(qualifiedCppName.size()>1){
                qualifiedCppName.removeLast();
                m_pendingScopedClasses.append({cls, qualifiedCppName.join("::")});
            }
        }
        if(!cls->typeEntry()->isString())
            if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(cls->targetTypeSystem())){
                m_typeSystemByPackage[cls->package()] = ts;
            }
        if(cls->isInterface()){
            MetaClass *interfaceImpl = cls->extractInterfaceImpl();
            m_meta_classes << interfaceImpl;
            ReportHandler::debugSparse(QString(" -> interface implementation class '%1'").arg(interfaceImpl->name()));
        }
    }
}


MetaClass *MetaBuilder::traverseNamespace(NamespaceModelItem namespace_item, QList<PendingClass>& pendingClasses, QList<PendingFunctional>& pendingFunctionals) {
    QString namespace_name = (!m_namespace_prefix.isEmpty() ? m_namespace_prefix + "::" : QString()) + namespace_item->name();

    NamespaceTypeEntry *type = m_database->findNamespaceType(namespace_name);

    if (m_database->isClassRejected(namespace_name)) {
        m_rejected_classes.insert({namespace_name, namespace_item->fileName()}, GenerationDisabled);
        return nullptr;
    }

    if (!type) {
        ReportHandler::warning(QString("namespace '%1' does not have a type entry")
                               .arg(namespace_name));
        return nullptr;
    }

    MetaClass *meta_class = m_meta_classes.findClass(type->qualifiedCppName(), MetaClassList::QualifiedCppName);
    if(meta_class && meta_class->typeEntry()!=type){
        meta_class = nullptr;
    }

    bool isNewType = false;
    if(!meta_class){
        meta_class = new MetaClass();
        meta_class->setTypeEntry(type);
        *meta_class += MetaAttributes::Public;
        isNewType = true;
    }

    m_current_class = meta_class;

    ReportHandler::debugSparse(QString("namespace '%1.%2'")
                               .arg(meta_class->package(), namespace_item->name()));

    QSet<QString> metaEnums;
    for(const FunctionModelItem& function_item : namespace_item->functions()){
        if(function_item->name()==QStringLiteral("qt_getEnumMetaObject")
                && function_item->arguments().size()==1){
            metaEnums << function_item->arguments()[0]->type().toString();
        }
    }
    traverseEnums(model_dynamic_cast<ScopeModelItem>(namespace_item), meta_class, metaEnums, namespace_item->flagsMap());
//     traverseClasses(model_dynamic_cast<ScopeModelItem>(namespace_item));

    pushScope(model_dynamic_cast<ScopeModelItem>(namespace_item));
    m_namespace_prefix = currentScope()->qualifiedName().join("::");

    for(const ClassModelItem& cls : namespace_item->classes()){
        MetaClass *mjc = traverseClass(cls, pendingClasses, pendingFunctionals);
        // the classes inside of a namespace are realized as static member classes
        // of the namespace representing java interface.
        if (mjc) {
            bool isEnclosedClass = mjc->typeEntry()->targetLangName().startsWith(meta_class->typeEntry()->targetLangName()+"$");
            if(isEnclosedClass){
                meta_class->addEnclosedClass(mjc);
            }
            addClass(mjc);
//            m_meta_classes << mjc;
//            if(!mjc->typeEntry()->isString())
//                if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(mjc->targetTypeSystem())){
//                    m_typeSystemByPackage[mjc->package()] = ts;
//                }
//            if(mjc->isInterface()){
//                MetaClass *interfaceImpl = mjc->extractInterfaceImpl();
//                m_meta_classes << interfaceImpl;
//                ReportHandler::debugSparse(QString(" -> interface implementation class '%1'").arg(interfaceImpl->name()));
//            }
        }
    }

    // Go through all typedefs to see if we have defined any
    // specific typedefs to be used as classes.
    for(const TypeAliasModelItem& tai : namespace_item->typeAliases()){
        if(tai->type().qualifiedName()==QStringList{"std","function"}
                || tai->type().qualifiedName()==QStringList{"qxp","function_ref"}
                || tai->type().isFunctionPointer()){
            pendingFunctionals.append(PendingFunctional{tai, meta_class, m_scopes});
        }else{
            MetaClass *cls = traverseTypeAlias(tai);
            // the classes inside of a namespace are realized as static member classes
            // of the namespace representing java interface.
            if (cls) {
                bool isEnclosedClass = cls->typeEntry()->targetLangName().startsWith(meta_class->typeEntry()->targetLangName()+"$");
                if(isEnclosedClass){
                    meta_class->addEnclosedClass(cls);
                }
                m_meta_classes << cls;
                if(!cls->typeEntry()->isString())
                    if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(cls->targetTypeSystem())){
                        m_typeSystemByPackage[cls->package()] = ts;
                    }
                if(cls->isInterface()){
                    MetaClass *interfaceImpl = cls->extractInterfaceImpl();
                    m_meta_classes << interfaceImpl;
                    ReportHandler::debugSparse(QString(" -> interface implementation class '%1'").arg(interfaceImpl->name()));
                }
            }
        }
    }


    // Traverse namespaces recursively
    for(const NamespaceModelItem& n : namespace_item->namespaces()){
        MetaClass *mjc = traverseNamespace(n, pendingClasses, pendingFunctionals);
        // the namespace inside of a namespace are realized as static member interfaces
        // of the namespace representing java interface.
        if (mjc) {
            bool isEnclosedClass = mjc->typeEntry()->targetLangName().startsWith(meta_class->typeEntry()->targetLangName()+"$");
            if(isEnclosedClass){
                meta_class->addEnclosedClass(mjc);
            }
            m_meta_classes << mjc;
            if(!mjc->typeEntry()->isString())
                if(TypeSystemTypeEntry* ts = m_database->findTypeSystem(mjc->targetTypeSystem())){
                    m_typeSystemByPackage[mjc->package()] = ts;
                }
        }
    }

    m_current_class = nullptr;


    popScope();
    m_namespace_prefix = currentScope()->qualifiedName().join("::");

    if (!type->include().isValid()) {
        addInclude(type, namespace_item->fileName());
    }
    if(!namespace_item->requiredFeatures().isEmpty()){
        QStringList ppConditions;
        if(!type->ppCondition().isEmpty()){
            ppConditions << type->ppCondition();
        }
        for(const QString& feature : namespace_item->requiredFeatures()){
            ppConditions << QString("QT_CONFIG(%1)").arg(feature);
        }
        ppConditions.removeDuplicates();
        type->setPPCondition(ppConditions.join(" && "));
    }
    pendingClasses.append(PendingClass{model_dynamic_cast<ScopeModelItem>(namespace_item), meta_class, m_scopes});

    return isNewType ? meta_class : nullptr;
}

QVariant Operator::calculate(QVariant x) {
    QVariant result;
    switch (type) {
        case Plus:
        if(value.isValid()){
            switch(size){
            case 8:
                result.setValue<qint8>(x.value<qint8>() + value.value<qint8>());
                break;
            case 16:
                result.setValue<qint16>(x.value<qint16>() + value.value<qint16>());
                break;
            case 32:
                result.setValue<qint32>(x.value<qint32>() + value.value<qint32>());
                break;
            case 64:
                result.setValue<qint64>(x.value<qint64>() + value.value<qint64>());
                break;
            default:
                result.setValue<qint32>(x.value<qint32>() + value.value<qint32>());
                break;
            }
        }else{
            result = x;
        }
        break;
        case Minus:
        if(value.isValid()){
            switch(size){
            case 8:
                result.setValue<qint8>(x.value<qint8>() - value.value<qint8>());
                break;
            case 16:
                result.setValue<qint16>(x.value<qint16>() - value.value<qint16>());
                break;
            case 32:
                result.setValue<qint32>(x.value<qint32>() - value.value<qint32>());
                break;
            case 64:
                result.setValue<qint64>(x.value<qint64>() - value.value<qint64>());
                break;
            default:
                result.setValue<qint32>(x.value<qint32>() - value.value<qint32>());
                break;
            }
        }else{
            switch(size){
            case 8:
                result.setValue<qint8>(-x.value<qint8>());
                break;
            case 16:
                result.setValue<qint16>(-x.value<qint16>());
                break;
            case 32:
                result.setValue<qint32>(-x.value<qint32>());
                break;
            case 64:
                result.setValue<qint64>(-x.value<qint64>());
                break;
            default:
                result.setValue<qint32>(-x.value<qint32>());
                break;
            }
        }
        break;
        case ShiftLeft:
        switch(size){
        case 8:
            result.setValue<qint8>(qint8(x.value<qint8>() << value.value<qint8>()));
            break;
        case 16:
            result.setValue<qint16>(qint16(x.value<qint16>() << value.value<qint16>()));
            break;
        case 32:
            result.setValue<qint32>(x.value<qint32>() << value.value<qint32>());
            break;
        case 64:
            result.setValue<qint64>(x.value<qint64>() << value.value<qint64>());
            break;
        default:
            result.setValue<qint32>(x.value<qint32>() << value.value<qint32>());
            break;
        }
        break;
        case Not:
        switch(size){
        case 8:
            result.setValue<qint8>(~x.value<qint8>());
            break;
        case 16:
            result.setValue<qint16>(~x.value<qint16>());
            break;
        case 32:
            result.setValue<qint32>(~x.value<qint32>());
            break;
        case 64:
            result.setValue<qint64>(~x.value<qint64>());
            break;
        default:
            result.setValue<qint32>(~x.value<qint32>());
            break;
        }
        break;
        case None: result = x;
        break;
    }
    return result;
}

QString Operator::toString(QString x) {
    QString result;
    switch (type) {
        case Plus:
        if(value.userType()==QMetaType::QString){
            result = "(" + x + "+" + value.toString() + ")";
        }else if(value.isValid()){
            switch(size){
            case 8:
                result = "(" + x + "+" + QString::number(value.value<qint8>()) + ")";
                break;
            case 16:
                result = "(" + x + "+" + QString::number(value.value<qint16>()) + ")";
                break;
            case 32:
                result = "(" + x + "+" + QString::number(value.value<qint32>()) + ")";
                break;
            case 64:
                result = "(" + x + "+" + QString::number(value.value<qint64>()) + ")";
                break;
            default:
                result = "(" + x + "+" + QString::number(value.value<qint32>()) + ")";
                break;
            }
        }else{
            result = x;
        }
        break;
        case Minus:
        if(value.userType()==QMetaType::QString){
            result = "(" + x + "-" + value.toString() + ")";
        }else if(value.isValid()){
            switch(size){
            case 8:
                result = "(" + x + "-" + QString::number(value.value<qint8>()) + ")";
                break;
            case 16:
                result = "(" + x + "-" + QString::number(value.value<qint16>()) + ")";
                break;
            case 32:
                result = "(" + x + "-" + QString::number(value.value<qint32>()) + ")";
                break;
            case 64:
                result = "(" + x + "-" + QString::number(value.value<qint64>()) + ")";
                break;
            default:
                result = "(" + x + "-" + QString::number(value.value<qint32>()) + ")";
                break;
            }
        }else{
            result = "-" + x;
        }
        break;
        case ShiftLeft:
        if(value.userType()==QMetaType::QString){
            result = "(" + x + " << " + value.toString() + ")";
        }else{
            switch(size){
            case 8:
                result = "(" + x + " << " + QString::number(value.value<qint8>()) + ")";
                break;
            case 16:
                result = "(" + x + " << " + QString::number(value.value<qint16>()) + ")";
                break;
            case 32:
                result = "(" + x + " << " + QString::number(value.value<qint32>()) + ")";
                break;
            case 64:
                result = "(" + x + " << " + QString::number(value.value<qint64>()) + ")";
                break;
            default:
                result = "(" + x + " << " + QString::number(value.value<qint32>()) + ")";
                break;
            }
        }
        break;
        case Not:
            result = "~" + x;
        break;
        case None: result = x;
        break;
    }
    return result;
}


Operator MetaBuilder::findOperator(const uint size, QString *s,
                                           MetaClass *global,
                                           MetaEnum *meta_enum,
                                           MetaFunction *meta_function,
                                           QSet<QString> *warnings) {
    const char *names[] = {
        "+",
        "-",
        "<<",
        "~"
    };

    for (int i = 0; i < Operator::None; ++i) {
        QString name = QLatin1String(names[i]);
        QString str = *s;
        auto splitPoint = str.indexOf(name);
        if (splitPoint > 0) {
            QString right = str.mid(splitPoint + name.length());
            Operator op(size);
            op.value = figureOutEnumValue(size,
                                right,
                                QVariant::fromValue<QString>(right),
                                global, meta_enum, meta_function, warnings);
            if (op.value.isValid()) {
                op.type = Operator::Type(i);
                *s = str.left(splitPoint).trimmed();
                return op;
            }
        }else if (splitPoint == 0 && name!="<<") {
            QString number = str.mid(splitPoint + name.length()).trimmed();
            bool ok = false;
            if(name!="-"){
                // don't apply unary minus to numbers. They are parsed as negative numbers.
                if(number.endsWith("L")){
                    number = number.left(number.length()-1);
                }
                if(number.size()>1 && number.startsWith("0")){
                    number.toLongLong(&ok, 0);
                }else{
                    number.toLongLong(&ok);
                }
            }
            if(!ok){
                Operator op(size);
                op.type = Operator::Type(i);
                *s = str.mid(splitPoint + name.length()).trimmed();
                return op;
            }else{
                break;
            }
        }
    }
    return Operator(size);
}

QVariant MetaBuilder::figureOutEnumValue(
        const uint size,
        const QString &origStringValue,
        QVariant oldValuevalue,
        MetaClass *global,
        MetaEnum *meta_enum,
        MetaFunction *meta_function,
        QSet<QString> *warnings) {
    if (origStringValue.isEmpty())
        return oldValuevalue;

    QString stringValue(origStringValue.trimmed());
    bool negate = false;
    bool _not = false;
    if(stringValue.startsWith("~")){
        stringValue = stringValue.mid(1);
        _not = true;
    }else if(stringValue.startsWith("-")){
        stringValue = stringValue.mid(1);
        negate = true;
    }
    while(stringValue.startsWith("(") && stringValue.endsWith(")")){
        stringValue = stringValue.mid(1, stringValue.length()-2);
    }

    // This block deals with "static_cast<FooBar::Type>" prefix on cpp defaulted values
    const QString keyword_static_cast("static_cast");
    if (stringValue.startsWith(keyword_static_cast)) {
        stringValue = stringValue.remove(0, keyword_static_cast.length()).trimmed();
        if (stringValue.length() > 0 && stringValue.at(0) == QChar('<')) {
            auto end_pos = stringValue.indexOf(QChar('>'));
            if (end_pos >= 0)	// remove the whole "<FooBar::Type>"
                stringValue = stringValue.remove(0, end_pos).trimmed();
        }
    }
    // This block deals with "FooBar::Type(.....)" around the part we really want "....."
    // or with "(type)....."
    {
        auto beg_pos = stringValue.indexOf(QChar('('));
        if (beg_pos >= 0) {
            QString tmp_stringValue = stringValue;
            tmp_stringValue = tmp_stringValue.remove(0, beg_pos+1).trimmed();	// remove "FooBar::Type("
            if (beg_pos == 0) { // was "(type)....."
                auto nextClose_pos = tmp_stringValue.indexOf(QChar(')'));
                if(nextClose_pos>0){
                    tmp_stringValue = tmp_stringValue.remove(0, nextClose_pos+1).trimmed();	// remove "type)"
                }
            }

            auto nextClose_pos = tmp_stringValue.indexOf(QChar(')'));
            auto nextOpen_pos = tmp_stringValue.indexOf(QChar('('));
            if(nextOpen_pos<0 || nextOpen_pos<nextClose_pos){
                auto end_pos = tmp_stringValue.lastIndexOf(QChar(')'));
                if (end_pos >= 0)
                    tmp_stringValue = tmp_stringValue.remove(end_pos, 1).trimmed();	// remove ")"
                stringValue = tmp_stringValue;
                if(stringValue.trimmed().isEmpty()){
                    return 0;
                }
            }
        }
    }

    QStringList stringValues = stringValue.split("|");

    QVariantList disjointReturnValues;
    QStringList disjointReturnValueStrings;
    bool hasValidNumberVariants = true;

    for (int i = 0; i < stringValues.size(); ++i) {
        QString s = stringValues.at(i).trimmed();

        while(s.startsWith("(") && s.endsWith(")")){
            s = s.mid(1, s.length()-2);
        }

        // This block deals with "static_cast<FooBar::Type>" prefix on cpp defaulted values
        if (s.startsWith(keyword_static_cast)) {
            s = s.remove(0, keyword_static_cast.length()).trimmed();
            if (s.length() > 0 && s.at(0) == QChar('<')) {
                auto end_pos = s.indexOf(QChar('>'));
                if (end_pos >= 0)	// remove the whole "<FooBar::Type>"
                    s = s.remove(0, end_pos).trimmed();
            }
        }
        // This block deals with "FooBar::Type(.....)" around the part we really want "....."
        // or with "(type)....."
        {
            auto beg_pos = s.indexOf(QChar('('));
            if (beg_pos >= 0) {
                QString tmp_string = s;
                tmp_string = tmp_string.remove(0, beg_pos+1).trimmed();	// remove "FooBar::Type("
                if (beg_pos == 0) { // was "(type)....."
                    auto nextClose_pos = tmp_string.indexOf(QChar(')'));
                    if(nextClose_pos>0){
                        tmp_string = tmp_string.remove(0, nextClose_pos+1).trimmed();	// remove "type)"
                    }
                }

                auto nextClose_pos = tmp_string.indexOf(QChar(')'));
                auto nextOpen_pos = tmp_string.indexOf(QChar('('));
                if(nextOpen_pos<0 || nextOpen_pos<nextClose_pos){
                    auto end_pos = tmp_string.lastIndexOf(QChar(')'));
                    if (end_pos >= 0)
                        tmp_string = tmp_string.remove(end_pos, 1).trimmed();	// remove ")"
                    s = tmp_string;
                }
            }
        }

        {
            auto beg_pos = s.indexOf(QChar('('));
            if (beg_pos >= 0) {
                QString tmp_stringValue = s;
                tmp_stringValue = tmp_stringValue.remove(0, beg_pos+1).trimmed();	// remove "("

                auto nectClose_pos = tmp_stringValue.indexOf(QChar(')'));
                auto nextOpen_pos = tmp_stringValue.indexOf(QChar('('));
                if(nextOpen_pos<0 || nextOpen_pos<nectClose_pos){
                    auto end_pos = tmp_stringValue.lastIndexOf(QChar(')'));
                    if (end_pos >= 0)
                        tmp_stringValue = tmp_stringValue.remove(end_pos, 1).trimmed();	// remove ")"
                    s = tmp_stringValue;
                }
            }
        }

        bool ok = false;
        QVariant v;

        Operator op = findOperator(size, &s, global, meta_enum, meta_function, warnings);

        if(s.length()==3 && s.startsWith("'") && s.endsWith("'")){
            QChar chr = s[1];
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(quint8(chr.toLatin1())));
                break;
            case 16:
                v.setValue<qint16>(qint16(quint16(chr.toLatin1())));
                break;
            case 32:
                v.setValue<qint32>(qint32(quint32(chr.toLatin1())));
                break;
            case 64:
                v.setValue<qint64>(qint64(quint64(chr.toLatin1())));
                break;
            default:
                v.setValue<qint32>(qint32(quint32(chr.toLatin1())));
                break;
            }
            ok = true;
        }else if(s.length()>3 && s.startsWith("'\\u") && s.endsWith("'")){
            QString unicode = "0x"+s.mid(3, s.length()-2);
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(quint8(unicode.toULongLong(&ok, 16))));
                break;
            case 16:
                v.setValue<qint16>(qint16(quint16(unicode.toULongLong(&ok, 16))));
                break;
            case 32:
                v.setValue<qint32>(qint32(quint32(unicode.toULongLong(&ok, 16))));
                break;
            case 64:
                v.setValue<qint64>(qint64(quint64(unicode.toULongLong(&ok, 16))));
                break;
            default:
                v.setValue<qint32>(qint32(quint32(unicode.toULongLong(&ok, 16))));
                break;
            }
        }else if(s.length()==4 && s.startsWith("'\\") && s.endsWith("'")){
            QChar chr = s[2];
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(quint8(chr.toLatin1())));
                break;
            case 16:
                v.setValue<qint16>(qint16(quint16(chr.toLatin1())));
                break;
            case 32:
                v.setValue<qint32>(qint32(quint32(chr.toLatin1())));
                break;
            case 64:
                v.setValue<qint64>(qint64(quint64(chr.toLatin1())));
                break;
            default:
                v.setValue<qint32>(qint32(quint32(chr.toLatin1())));
                break;
            }
            ok = true;
        }else if (s.length() > 0 && s.at(0) == QLatin1Char('0')
                  && s.at(s.length()-1).toUpper() != QLatin1Char('U')
                  && s.at(s.length()-1).toUpper() != QLatin1Char('L')){
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(quint8(s.toUShort(&ok, 0))));
                break;
            case 16:
                v.setValue<qint16>(qint16(s.toUShort(&ok, 0)));
                break;
            case 32:
                v.setValue<qint32>(qint32(s.toUInt(&ok, 0)));
                break;
            case 64:
                v.setValue<qint64>(qint64(s.toULongLong(&ok, 0)));
                break;
            default:
                v.setValue<qint32>(qint32(s.toUInt(&ok, 0)));
                break;
            }
        }else if (s.length() > 0 && s.at(s.length()-1).toUpper() == QLatin1Char('U')){
            QString unsignedNumber = s.left(s.length()-1);
            if(unsignedNumber.startsWith("0x")){
                switch(size){
                case 8:
                    v.setValue<qint8>(qint8(unsignedNumber.toULongLong(&ok, 16)));
                    break;
                case 16:
                    v.setValue<qint16>(qint16(unsignedNumber.toULongLong(&ok, 16)));
                    break;
                case 32:
                    v.setValue<qint32>(qint32(unsignedNumber.toULongLong(&ok, 16)));
                    break;
                case 64:
                    v.setValue<qint64>(qint64(unsignedNumber.toULongLong(&ok, 16)));
                    break;
                default:
                    v.setValue<qint32>(qint32(unsignedNumber.toULongLong(&ok, 16)));
                    break;
                }
            }else{
                switch(size){
                case 8:
                    v.setValue<qint8>(qint8(unsignedNumber.toULongLong(&ok)));
                    break;
                case 16:
                    v.setValue<qint16>(qint16(unsignedNumber.toULongLong(&ok)));
                    break;
                case 32:
                    v.setValue<qint32>(qint32(unsignedNumber.toULongLong(&ok)));
                    break;
                case 64:
                    v.setValue<qint64>(qint64(unsignedNumber.toULongLong(&ok)));
                    break;
                default:
                    v.setValue<qint32>(qint32(unsignedNumber.toULongLong(&ok)));
                    break;
                }
            }
        }else if (s.length() > 1
                  && s.at(s.length()-2).toUpper() == QLatin1Char('u')
                  && s.at(s.length()-1).toUpper() == QLatin1Char('L')){
            QString longNumber = s.left(s.length()-2);
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(longNumber.toULongLong(&ok)));
                break;
            case 16:
                v.setValue<qint16>(qint16(longNumber.toULongLong(&ok)));
                break;
            case 32:
                v.setValue<qint32>(qint32(longNumber.toULongLong(&ok)));
                break;
            case 64:
                v.setValue<qint64>(qint64(longNumber.toULongLong(&ok)));
                break;
            default:
                v.setValue<qint32>(qint32(longNumber.toULongLong(&ok)));
                break;
            }
        }else if (s.length() > 0 && s.at(s.length()-1).toUpper() == QLatin1Char('L')){
            QString longNumber = s.left(s.length()-1);
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(longNumber.toLongLong(&ok)));
                break;
            case 16:
                v.setValue<qint16>(qint16(longNumber.toLongLong(&ok)));
                break;
            case 32:
                v.setValue<qint32>(qint32(longNumber.toLongLong(&ok)));
                break;
            case 64:
                v.setValue<qint64>(qint64(longNumber.toLongLong(&ok)));
                break;
            default:
                v.setValue<qint32>(qint32(longNumber.toLongLong(&ok)));
                break;
            }
        } else {
            switch(size){
            case 8:
                v.setValue<qint8>(qint8(s.toShort(&ok)));
                break;
            case 16:
                v.setValue<qint16>(qint16(s.toShort(&ok)));
                break;
            case 32:
                v.setValue<qint32>(qint32(s.toInt(&ok)));
                break;
            case 64:
                v.setValue<qint64>(qint64(s.toLongLong(&ok)));
                break;
            default:
                v.setValue<qint32>(qint32(s.toInt(&ok)));
                break;
            }
        }

        MetaEnumValue *ev = nullptr;
        if (!ok) {
            v = QVariant();
            if (m_enum_values.contains(s)) {
                ev = m_enum_values[s];
                // if target value has not yet been computed do it
                if(!ev->value().isValid()){
                    MetaClass * ownerType = nullptr;
                    QString ownerTypeName;
                    bool found = false;
                    if (s.contains("::")) {
                        ownerTypeName = s.left(s.lastIndexOf("::"));
                        ownerType = classes().findClass(ownerTypeName);
                        if(!ownerType){
                            if(ownerTypeName.contains("::")){
                                QString _ownerTypeName = ownerTypeName.left(ownerTypeName.lastIndexOf("::"));
                                ownerType = classes().findClass(_ownerTypeName);
                            }else if(global){
                                if(MetaEnum* target_meta_enum = global->findEnum(ownerTypeName)){
                                    QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), target_meta_enum->values().indexOf(ev), global, target_meta_enum, nullptr, warnings);
                                    if(variant.isValid()){
                                        ev->setValue(variant);
                                        v = variant;
                                        found = true;
                                    }
                                }
                            }else{
                                for(MetaClass *cls : qAsConst(m_meta_classes)){
                                    if(cls->typeEntry()->isGlobal()){
                                        if(MetaEnum* target_meta_enum = cls->findEnum(ownerTypeName)){
                                            QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), target_meta_enum->values().indexOf(ev), cls, target_meta_enum, nullptr, warnings);
                                            if(variant.isValid()){
                                                ev->setValue(variant);
                                                v = variant;
                                                found = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        ownerType = global;
                    }
                    if(!found){
                        if(ownerType){
                            if(MetaEnum* target_meta_enum = ownerType->findEnumForValue(ev->cppName())){
                                QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), target_meta_enum->values().indexOf(ev), global, target_meta_enum, nullptr, warnings);
                                if(variant.isValid()){
                                    ev->setValue(variant);
                                    v = variant;
                                }
                            }
                        }else if(ownerTypeName.isEmpty()){
                            if(warnings){
                                warnings->insert("Not able to find enum constant '"+s+"'");
                            }else{
                                ReportHandler::warning("Not able to find enum constant '"+s+"'");
                            }
                        }else{
                            if(warnings){
                                warnings->insert("Enum constant '"+s+"' belongs to unknown type "+ownerTypeName);
                            }else{
                                ReportHandler::warning("Enum constant '"+s+"' belongs to unknown type "+ownerTypeName);
                            }
                        }
                    }
                }else{
                    v = ev->value();
                }
            } else {
                MetaEnum* target_meta_enum = nullptr;
                QString enumEntry = s;
                if(meta_enum && meta_enum->typeEntry()->isScopedEnum() && enumEntry.startsWith(meta_enum->typeEntry()->targetLangName()+"::")){
                    enumEntry = enumEntry.right(enumEntry.length() - meta_enum->typeEntry()->targetLangName().length() - 2);
                }else if(meta_enum && meta_enum->typeEntry()->isScopedEnum() && enumEntry.startsWith(meta_enum->typeEntry()->qualifiedCppName()+"::")){
                    enumEntry = enumEntry.right(enumEntry.length() - meta_enum->typeEntry()->qualifiedCppName().length() - 2);
                }
                if (meta_enum && (ev = meta_enum->values().find(enumEntry))) {
                    // if target value has not yet been computed do it
                    if(!ev->value().isValid()){
                        QVariant variant = figureOutEnumValue(meta_enum->typeEntry()->size(), ev->stringValue(), QVariant(), global, meta_enum, nullptr, warnings);
                        if(variant.isValid()){
                            ev->setValue(variant);
                            v = variant;
                        }
                    }else{
                        v = ev->value();
                    }
                } else if (meta_enum && (ev = meta_enum->enclosingClass()->findEnumValue(s, meta_enum))) {
                    // if target value has not yet been computed do it
                    if(!ev->value().isValid()){
                        target_meta_enum = meta_enum->enclosingClass()->findEnumForValue(ev->cppName());
                        if(target_meta_enum){
                            QVariant variant = figureOutEnumValue(meta_enum->typeEntry()->size(), ev->stringValue(), QVariant(), global, target_meta_enum, nullptr, warnings);
                            if(variant.isValid()){
                                ev->setValue(variant);
                                v = variant;
                            }
                        }
                    }else{
                        v = ev->value();
                    }
                } else if (global
                           && !s.contains("::")
                           && (target_meta_enum = global->findEnumForValue(s))
                           && (ev = global->findEnumValue(s, target_meta_enum))) {
                    // if target value has not yet been computed do it
                    if(!ev->value().isValid()){
                        QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), QVariant(), global, target_meta_enum, nullptr, warnings);
                        if(variant.isValid()){
                            ev->setValue(variant);
                            v = variant;
                        }
                    }else{
                        v = ev->value();
                    }
                } else if (global
                           && s!=enumEntry
                           && !enumEntry.contains("::")
                           && (target_meta_enum = global->findEnumForValue(enumEntry))
                           && (ev = global->findEnumValue(enumEntry, target_meta_enum))) {
                    // if target value has not yet been computed do it
                    if(!ev->value().isValid()){
                        QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), QVariant(), global, target_meta_enum, nullptr, warnings);
                        if(variant.isValid()){
                            ev->setValue(variant);
                            v = variant;
                        }
                    }else{
                        v = ev->value();
                    }
                } else {
                    MetaClass * ownerType = nullptr;
                    QString ownerTypeName;
                    QString enumValueName;
                    if (s.contains("::")) {
                        ownerTypeName = s.left(s.lastIndexOf("::"));
                        enumValueName = s.right(s.length()-s.lastIndexOf("::")-2);
                        ownerType = classes().findClass(ownerTypeName);
                        if(!ownerType && ownerTypeName.contains("::")){
                            QString _ownerTypeName = ownerTypeName.left(ownerTypeName.lastIndexOf("::"));
                            enumValueName = ownerTypeName.right(ownerTypeName.length()-ownerTypeName.lastIndexOf("::")-2) + "::" + enumValueName;
                            ownerType = classes().findClass(_ownerTypeName);
                        }
                    }else{
                        ownerType = global;
                    }
                    if(ownerType && (target_meta_enum = ownerType->findEnumForValue(s))
                            && (ev = ownerType->findEnumValue(s, target_meta_enum))){
                        if(!ev->value().isValid()){
                            QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), QVariant(), global, target_meta_enum, nullptr, warnings);
                            if(variant.isValid()){
                                ev->setValue(variant);
                                v = variant;
                            }
                        }else{
                            v = ev->value();
                        }
                    }
                    if(ownerType && (target_meta_enum = ownerType->findEnumForValue(enumValueName))
                            && (ev = ownerType->findEnumValue(enumValueName, target_meta_enum))){
                        if(!ev->value().isValid()){
                            QVariant variant = figureOutEnumValue(target_meta_enum->typeEntry()->size(), ev->stringValue(), QVariant(), global, target_meta_enum, nullptr, warnings);
                            if(variant.isValid()){
                                ev->setValue(variant);
                                v = variant;
                            }
                        }else{
                            v = ev->value();
                        }
                    }
                }
            }
        }

        if(v.userType()==QMetaType::QString){
            disjointReturnValueStrings << op.toString(v.toString());
            disjointReturnValues << QVariant();
            hasValidNumberVariants = false;
        }else if (v.isValid()){
            QString number;
            switch(size){
            case 8:
                number = "((byte)"+QString::number(v.value<qint8>())+")";
                break;
            case 16:
                number = "((short)"+QString::number(v.value<qint16>())+")";
                break;
            case 32:
                number = QString::number(v.value<qint32>());
                break;
            case 64:
                number = QString::number(v.value<qint64>())+"L";
                break;
            default:
                number = QString::number(v.value<qint32>());
                break;
            }
            disjointReturnValueStrings << op.toString(number);
            QVariant result = op.calculate(v);
            if(!result.isValid() || result.userType()==QMetaType::QString){
                hasValidNumberVariants = false;
                disjointReturnValues << QVariant();
            }else{
                disjointReturnValues << result;
            }
        }else{
            if(ev){
                if(ev->getEnum()->typeEntry()->forceInteger()){
                    disjointReturnValueStrings << op.toString(ev->getEnum()->typeEntry()->qualifiedTargetLangName()+"."+ev->name());
                }else{
                    disjointReturnValueStrings << op.toString(ev->getEnum()->typeEntry()->qualifiedTargetLangName()+"."+ev->name()+".value()");
                }
            }else{
                disjointReturnValueStrings << op.toString(s);
            }
            disjointReturnValues << QVariant();
            hasValidNumberVariants = false;
        }
    }

    QVariant returnValue;
    if(hasValidNumberVariants){
        for(int i=0; i<disjointReturnValues.size(); i++){
            if(i==0){
                returnValue = disjointReturnValues.at(i);
            }else{
                switch(size){
                case 8:
                    returnValue.setValue<qint8>(returnValue.value<qint8>() | disjointReturnValues.at(i).value<qint8>());
                    break;
                case 16:
                    returnValue.setValue<qint16>(returnValue.value<qint16>() | disjointReturnValues.at(i).value<qint16>());
                    break;
                case 32:
                    returnValue.setValue<qint32>(returnValue.value<qint32>() | disjointReturnValues.at(i).value<qint32>());
                    break;
                case 64:
                    returnValue.setValue<qint64>(returnValue.value<qint64>() | disjointReturnValues.at(i).value<qint64>());
                    break;
                default:
                    returnValue.setValue<qint32>(returnValue.value<qint32>() | disjointReturnValues.at(i).value<qint32>());
                    break;
                }
            }
        }
    }else{
        QString stringResult;
        for(int i=0; i<disjointReturnValueStrings.size(); i++){
            if(i==0){
                stringResult = disjointReturnValueStrings.at(i);
            }else{
                stringResult += "|" + disjointReturnValueStrings.at(i);
            }
        }
        returnValue.setValue(stringResult);
    }

    if(negate){
        switch(size){
        case 8:
            returnValue.setValue<qint8>(-returnValue.value<qint8>());
            break;
        case 16:
            returnValue.setValue<qint16>(-returnValue.value<qint16>());
            break;
        case 32:
            returnValue.setValue<qint32>(-returnValue.value<qint32>());
            break;
        case 64:
            returnValue.setValue<qint64>(-returnValue.value<qint64>());
            break;
        default:
            returnValue.setValue<qint32>(-returnValue.value<qint32>());
            break;
        }
    }else if(_not){
        switch(size){
        case 8:
            returnValue.setValue<qint8>(~returnValue.value<qint8>());
            break;
        case 16:
            returnValue.setValue<qint16>(~returnValue.value<qint16>());
            break;
        case 32:
            returnValue.setValue<qint32>(~returnValue.value<qint32>());
            break;
        case 64:
            returnValue.setValue<qint64>(~returnValue.value<qint64>());
            break;
        default:
            returnValue.setValue<qint32>(~returnValue.value<qint32>());
            break;
        }
    }

    return returnValue;
}

void MetaBuilder::figureOutEnumValuesForClass(MetaClass *meta_class,
        QSet<MetaClass *> *classes, QSet<MetaClass *> *repeatClasses, QSet<QString> *warnings) {
    MetaClass *base = meta_class->baseClass();

    if (base && !classes->contains(base))
        figureOutEnumValuesForClass(base, classes, repeatClasses, warnings);

    if (classes->contains(meta_class))
        return;

    bool hasStringEnumValues = false;

    MetaEnumList enums = meta_class->enums();
    for(MetaEnum *e : enums) {
        if (!e)
            ReportHandler::warning("bad enum in class " + meta_class->name());
        QVariant value;
        switch(e->typeEntry()->size()){
        case 8:
            value = QVariant::fromValue<qint8>(0); break;
        case 16:
            value = QVariant::fromValue<qint16>(0); break;
        case 32:
            value = QVariant::fromValue<qint32>(0); break;
        case 64:
            value = QVariant::fromValue<qint64>(0); break;
        default:
            value = QVariant::fromValue<qint32>(0); break;
        }
        for(MetaEnumValue *v : e->values()) {
            // value could have been computed previously
            if(!v->value().isValid() || v->value().userType()==QMetaType::QString){
                QVariant _value = figureOutEnumValue(e->typeEntry()->size(), v->stringValue(), value, nullptr, e, nullptr, warnings);
                if(_value.isValid()){
                    v->setValue(_value);
                    value = _value;
                }
            }else{
                value = v->value();
            }
            if(value.userType()==QMetaType::QString){
                value.setValue(value.toString()+QLatin1String("+1"));
                hasStringEnumValues = true;
            }else{
                switch(e->typeEntry()->size()){
                case 8:
                    value.setValue<qint8>(value.value<qint8>()+1); break;
                case 16:
                    value.setValue<qint16>(value.value<qint16>()+1); break;
                case 32:
                    value.setValue<qint32>(value.value<qint32>()+1); break;
                case 64:
                    value.setValue<qint64>(value.value<qint64>()+1); break;
                default:
                    value.setValue<qint32>(value.value<qint32>()+1); break;
                }
            }
        }

        // Check for duplicate values...
        EnumTypeEntry *ete = e->typeEntry();
        if (!ete->forceInteger()) {
            QHash<qint64, MetaEnumValue *> entries;
            for(MetaEnumValue *v : e->values()) {

                bool vRejected = ete->isEnumValueRejected(v->name());

                if(v->value().isValid() && v->value().userType()!=QMetaType::QString){
                    MetaEnumValue *current = entries.value(v->value().value<qint64>());
                    if (current) {
                        bool currentRejected = ete->isEnumValueRejected(current->name());
                        if (!currentRejected && !vRejected) {
                            ReportHandler::warning(
                                QString("duplicate enum values: %1::%2, %3 and %4 are %5, already rejected: (%6)")
                                .arg(meta_class->name(), e->name(), v->name(),
                                     entries[v->value().value<qint64>()]->name())
                                .arg(v->value().value<qint64>())
                                .arg(ete->enumValueRejections().join(", ")));
                            continue;
                        }
                    }

                    if (!vRejected)
                        entries[v->value().value<qint64>()] = v;
                }
            }

            // Entries now contain all the original entries, no
            // rejected ones... Use this to generate the enumValueRedirection table.
            for(MetaEnumValue *reject : e->values()) {
                if (!ete->isEnumValueRejected(reject->name()))
                    continue;

                if(reject->value().isValid() && reject->value().userType()!=QMetaType::QString){
                    MetaEnumValue *used = entries.value(reject->value().value<qint64>());
                    if (!used) {
                        ReportHandler::warning(
                            QString::fromLatin1("Rejected enum has no alternative...: %1::%2\n")
                            .arg(meta_class->name(), reject->name()));
                        continue;
                    }
                    ete->addEnumValueRedirection(reject->name(), used->name());
                }
            }

        }
    }


    if(!hasStringEnumValues) // otherwise compute enum values again
        *classes += meta_class;
    else
        *repeatClasses += meta_class;
}


void MetaBuilder::figureOutEnumValues() {
    // Keep a set of classes that we already traversed. We use this to
    // enforce that we traverse base classes prior to subclasses.
    QSet<MetaClass *> classes;
    QSet<MetaClass *> repeatClasses;
    QSet<QString> warnings;
    for(MetaClass *c : qAsConst(m_meta_classes)) {
        figureOutEnumValuesForClass(c, &classes, &repeatClasses, &warnings);
    }
    if(!repeatClasses.isEmpty()){
        int i=0;
        QSet<MetaClass *> _repeatClasses;
        while(i<3){
            warnings.clear();
            for(MetaClass *c : qAsConst(repeatClasses)) {
                figureOutEnumValuesForClass(c, &classes, &_repeatClasses, &warnings);
            }
            if(_repeatClasses.isEmpty() || _repeatClasses==repeatClasses){
                break;
            }
            if(_repeatClasses.size()==repeatClasses.size()){
                i++;
            }
            repeatClasses.swap(_repeatClasses);
            _repeatClasses.clear();
        }
        for(const QString& w : qAsConst(warnings)){
            ReportHandler::warning(w);
        }

        /*for(AbstractMetaClass *c : repeatClasses) {
            for(const AbstractMetaEnum* enm : c->enums()){
                for(const AbstractMetaEnumValue* value : enm->values()) {
                    if(value->value().userType()==QMetaType::QString){
                        QString warn = QString("possible compilation error in enum value %1").arg(value->value().toString());
                        ReportHandler::warning(warn);
                    }
                }
            }
        }*/
    }
}

void MetaBuilder::figureOutDefaultEnumArguments() {
    for(MetaClass *meta_class : qAsConst(m_meta_classes)) {
        MetaClass *global = m_globals[meta_class->typeEntry()->targetTypeSystem()];

        for(MetaFunction *meta_function : meta_class->functions()) {
            for(MetaArgument *arg : meta_function->arguments()) {

                QString expr = arg->defaultValueExpression();
                if (expr.isEmpty())
                    continue;

                if (!meta_function->replacedDefaultExpression(meta_function->implementingClass(),
                        arg->argumentIndex() + 1).isEmpty()) {
                    continue;
                }

                QString new_expr = expr;
                if (arg->type()->isEnum()) {
                    QStringList lst = expr.split(QLatin1String("::"));
                    if (lst.size() == 1
                            || (arg->type()->typeEntry()->isScopedEnum() && lst.size() == 2)) {
                        QVector<MetaClass *> classes(1, meta_class);
                        if(!meta_class->typeEntry()->isInterface()
                                && meta_class->typeEntry()->designatedInterface()
                                && meta_class->extractInterface()){
                            classes << meta_class->extractInterface();
                        }
                        MetaEnum *e = nullptr;
                        MetaEnumValue *ev = nullptr;
                        while (!classes.isEmpty() && !e) {
                            if (classes.front()) {
                                classes << classes.front()->baseClass();

                                MetaClassList interfaces = classes.front()->interfaces();
                                for(MetaClass *interface : interfaces){
                                    MetaClass * primaryInterfaceImplementor = interface->primaryInterfaceImplementor();
                                    if(!classes.contains(primaryInterfaceImplementor)){
                                        classes << primaryInterfaceImplementor;
                                    }else{
                                        qDebug("The interface of %s is %s.", qPrintable(classes.front()->name()), qPrintable(primaryInterfaceImplementor->name()));
                                    }
                                }

                                ev = classes.front()->findEnumValue(expr, nullptr);
                                if(ev && !ev->getEnum())
                                    ev = nullptr;
                                if(!ev && !expr.startsWith(classes.front()->qualifiedCppName()+"::")){
                                    ev = classes.front()->findEnumValue(classes.front()->qualifiedCppName()+"::"+expr, nullptr);
                                    if(ev && !ev->getEnum())
                                        ev = nullptr;
                                }
                                if(!ev){
                                    e = classes.front()->findEnumForValue(expr);
                                    if(!e && !expr.startsWith(classes.front()->qualifiedCppName()+"::")){
                                        e = classes.front()->findEnumForValue(classes.front()->qualifiedCppName()+"::"+expr);
                                    }
                                }
                                if(e || ev)
                                    break;
                            }

                            classes.pop_front();
                        }

                        if (!e && !ev && global) {
                            ev = global->findEnumValue(expr, nullptr);
                            if(!ev){
                                e = global->findEnumForValue(expr);
                            }
                        }

                        if (ev) {
                            new_expr = QString("%1.%2")
                                       .arg(ev->getEnum()->typeEntry()->qualifiedTargetLangName(), ev->name());
                        } else if (e) {
                            new_expr = QString("%1.%2")
                                       .arg(e->typeEntry()->qualifiedTargetLangName(), lst.last());
                        } else {
                            new_expr.clear();
                            if(lst.size()>1){
                                if(MetaClass * clazz = m_meta_classes.findClass(lst.mid(0, lst.size()-1).join("::"))){
                                    if(lst.last().endsWith("()")){
                                        if(MetaFunction * f = clazz->findFunction(lst.last().mid(0, lst.last().length()-2))){
                                            new_expr = QString("%1.%2()")
                                                           .arg(clazz->typeEntry()->qualifiedTargetLangName(), f->name());
                                        }
                                    }else if(MetaField * f = clazz->findField(lst.last())){
                                        new_expr = QString("%1.%2")
                                                       .arg(clazz->typeEntry()->qualifiedTargetLangName(), f->name());
                                    }
                                }
                            }else{
                                if(lst.last().endsWith("()")){
                                    if(MetaFunction * f = meta_class->findFunction(lst.last().mid(0, lst.last().length()-2))){
                                        new_expr = QString("%1.%2()")
                                                       .arg(meta_class->typeEntry()->qualifiedTargetLangName(), f->name());
                                    }
                                }else if(MetaField * f = meta_class->findField(lst.last())){
                                    new_expr = QString("%1.%2")
                                                   .arg(meta_class->typeEntry()->qualifiedTargetLangName(), f->name());
                                }
                            }
                            if(new_expr.isEmpty()){
                                new_expr = expr;
                                ReportHandler::warning("Cannot find enum constant for value '" + expr + "' in '" + meta_class->name() + "' or any of its super classes");
                            }
                        }
                    } else {
                        MetaEnum *e = nullptr;
                        QString enumValueName;
                        QString className;
                        if(arg->type()->typeEntry()->isScopedEnum() && lst.length()>1){
                            enumValueName = lst.at(lst.length()-2) + "::" + lst.at(lst.length()-1);
                            for(int i=0; i<lst.size()-2; i++){
                                if(!className.isEmpty()){
                                    className += "::";
                                }
                                className += lst.at(i);
                            }
                        }else{
                            enumValueName = lst.at(lst.length()-1);
                            for(int i=0; i<lst.size()-1; i++){
                                if(!className.isEmpty()){
                                    className += "::";
                                }
                                className += lst.at(i);
                            }
                        }
                        MetaClass *cl = m_meta_classes.findClass(className);
                        if (!cl) {
                            if(arg->type()->typeEntry()->isScopedEnum()){
                                cl = global;
                            }else{
                                if(lst.size()==2 && global){
                                    enumValueName = lst.join("::");
                                    cl = global;
                                } else if(lst.size()>2){
                                    className = "";
                                    enumValueName = lst.at(lst.length()-2)+"::"+lst.at(lst.length()-1);
                                    for(int i=0; i<lst.size()-2; i++){
                                        if(!className.isEmpty()){
                                            className += "::";
                                        }
                                        className += lst.at(i);
                                    }
                                    cl = m_meta_classes.findClass(className);
                                }
                            }
                        }

                        if (!cl) {
                            ReportHandler::warning("missing required class for enums: " + className);
                            continue;
                        }
                        e = cl->findEnumForValue(enumValueName);
                        if(e){
                            new_expr = QString("%1.%2")
                                   .arg(e->typeEntry()->qualifiedTargetLangName(), lst.last());
                        }else if(cl->isFake()){
                            new_expr = QString("%1.%2")
                                   .arg(arg->type()->name(), lst.last());
                        }else{
                            new_expr = QString("%1.%2.%3")
                                   .arg(cl->typeEntry()->qualifiedTargetLangName(),
                                        arg->type()->name(),
                                        lst.last());
                        }
                    }

                } else if (arg->type()->isFlags()) {
                    const FlagsTypeEntry *flagsEntry = static_cast<const FlagsTypeEntry *>(arg->type()->typeEntry());
                    const EnumTypeEntry *enumEntry = flagsEntry->originator();
                    MetaEnum *meta_enum = m_meta_classes.findEnum(enumEntry);
                    if (!meta_enum) {
                        ReportHandler::warning("unknown required enum " + enumEntry->qualifiedCppName());
                        continue;
                    }
                    MetaEnumValue * v = meta_enum->values().find(expr);
                    if(v){
                        if(v->value().isValid()){
                            if(v->value().userType()==QMetaType::QString){
                                new_expr = v->value().toString();
                                ReportHandler::warning(QString("possible compilation error in enum value %1 when parsing default value of '%2' in class '%3'")
                                                       .arg(new_expr,
                                                            meta_function->name(),
                                                            meta_function->implementingClass()->name()));
                            }else{
                                new_expr = QString::number(v->value().value<qint32>());
                            }
                        }else if(expr.trimmed()=="{}"){
                            new_expr = "  ";
                        }else{
                            QVariant value = figureOutEnumValue(32, expr, meta_enum->values().indexOf(v), global, meta_enum, meta_function);
                            if(value.isValid()){
                                v->setValue(value);
                                if(value.userType()==QMetaType::QString){
                                    new_expr = value.toString();
                                    ReportHandler::warning(QString("possible compilation error in enum value %1 when parsing default value of '%2' in class '%3'")
                                                           .arg(new_expr, meta_function->name(), meta_function->implementingClass()->name()));
                                }else{
                                    new_expr = QString::number(value.value<qint32>());
                                }
                            }
                        }
                    }else{
                        QVariant value = figureOutEnumValue(32, expr, QVariant::fromValue<QString>(expr), global, meta_enum, meta_function);
                        if(value.isValid()){
                            if(value.userType()==QMetaType::QString){
                                new_expr = value.toString();
                            }else{
                                new_expr = QString::number(value.value<qint32>());
                            }
                        }
                    }
                } else if (arg->type()->isPrimitive()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    QMetaType baseTypeId(QMetaType::type(qPrintable(arg->type()->typeEntry()->qualifiedCppName())));
#else
                    QMetaType baseTypeId = QMetaType::fromName(qPrintable(arg->type()->typeEntry()->qualifiedCppName()));
#endif
                    uint size = 32;
                    if(baseTypeId.isValid()){
                        size = uint(baseTypeId.sizeOf())*8;
                        if(size!=8 && size!=16 && size!=32 && size!=64){
                            size = 32;
                        }
                    }
                    MetaEnumValue *value = nullptr;
                    if (expr.contains("::"))
                        value = m_meta_classes.findEnumValue(expr);
                    if (!value)
                        value = meta_class->findEnumValue(expr, nullptr);
                    if (!value && global)
                        value = global->findEnumValue(expr, nullptr);

                    QVariant vvalue;
                    if (value) {
                        vvalue = value->value();
                    } else if (expr.contains(QLatin1Char('+'))) {
                        vvalue = figureOutEnumValue(size, expr, 0, global, nullptr);
                    }
                    if(vvalue.isValid()){
                        if(vvalue.userType()==QMetaType::QString){
                            new_expr = vvalue.toString();
                        }else{
                            switch(size){
                            case 8:
                                new_expr = QString::number(vvalue.value<qint8>());
                                break;
                            case 16:
                                new_expr = QString::number(vvalue.value<qint16>());
                                break;
                            case 32:
                                new_expr = QString::number(vvalue.value<qint32>());
                                break;
                            case 64:
                                new_expr = QString::number(vvalue.value<qint64>())+"L";
                                break;
                            default:
                                new_expr = QString::number(vvalue.value<qint32>());
                                break;
                            }
                        }
                    }
                }

                arg->setDefaultValueExpression(new_expr);
            }
        }
    }
}

MetaEnum *MetaBuilder::traverseEnum(EnumModelItem enum_item, MetaClass *enclosing, const QSet<QString> &metaEnums, const QMap<QString,QString>& flagByEnums) {
    // Skipping private enums.
    QString qualified_name = enum_item->qualifiedName().join("::");
    if (enum_item->accessPolicy() == CodeModel::Private) {
        m_rejected_enums.insert({qualified_name, enum_item->fileName()}, IsPrivate);
        return nullptr;
    }

    TypeEntry *type_entry = m_database->findType(qualified_name);
    QString enum_name = enum_item->name();

    QString class_name;
    if (m_current_class)
        class_name = m_current_class->typeEntry()->qualifiedCppName();

    if (m_database->isEnumRejected(class_name, enum_name)) {
        m_rejected_enums.insert({qualified_name, enum_item->fileName()}, GenerationDisabled);
        return nullptr;
    }

    if(!type_entry && enclosing && !enclosing->isFake()){
        QStringList names = enum_item->qualifiedName();
        std::unique_ptr<EnumTypeEntry> eentry;
        if (names.size() == 1) {
            eentry.reset(new EnumTypeEntry(QString(), qualified_name));
        } else {
            eentry.reset(new EnumTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"), names.last()));
        }
        eentry->setCodeGeneration(enclosing->typeEntry()->codeGeneration());
        eentry->setTargetLangPackage(enclosing->typeEntry()->javaPackage());
        eentry->setTargetTypeSystem(enclosing->typeEntry()->targetTypeSystem());
        ReportHandler::debugTypes("Adding to TypeDatabase(1): " + eentry->name());
        m_database->addType(type_entry = eentry.release());
    }

    if (!type_entry || !type_entry->isEnum()) {
        //QString context = m_current_class ? m_current_class->name() : QLatin1String("");
        ReportHandler::warning(QString("enum '%1' does not have a type entry or is not an enum")
                               .arg(qualified_name));
        if(qualified_name.startsWith("QtPrivate::") || enum_item->fileName().startsWith("_p.h"))
            m_rejected_enums.insert({qualified_name, enum_item->fileName()}, IsPrivate);
        else
            m_rejected_enums.insert({qualified_name, enum_item->fileName()}, NotInTypeSystem);
        return nullptr;
    }

    MetaEnum *meta_enum = new MetaEnum();
    EnumTypeEntry * enumType = static_cast<EnumTypeEntry *>(type_entry);

    QString enumName = qualified_name;
    if(!flagByEnums.contains(enumName) && enum_item->qualifiedName().size()>1){
        enumName = enum_item->qualifiedName().last();
    }
    if(flagByEnums.contains(enumName) && !enumType->forceNoFlags()){
        QString flagsName = flagByEnums[enumName];
        QString simpleFlagsName = flagsName;
        if(enum_item->qualifiedName().size()>1){
            QStringList scope = enum_item->qualifiedName();
            scope.takeLast();
            if(!flagsName.startsWith(scope.join("::"))){
                scope << flagsName;
                flagsName = scope.join("::");
            }else{
                simpleFlagsName = flagsName.split("::").last();
            }
        }
        std::unique_ptr<FlagsTypeEntry> ftype(new FlagsTypeEntry(flagsName));
        ftype->setOriginator(enumType);
        ftype->setFlagsTemplate("QFlags<" + enumType->qualifiedCppName() + ">");
        ftype->setCodeGeneration(enumType->codeGeneration());
        ftype->setFlagsName(simpleFlagsName);
        enumType->setFlags(ftype.get());
        m_database->addFlagsType(ftype.get());
        //qDebug()<<"Adding ftype"<<ftype->name();
        ReportHandler::debugTypes("Adding to TypeDatabase(1): " + ftype->name());
        m_database->addType(ftype.release());
    }

    meta_enum->setTypeEntry(enumType);

    if (metaEnums.contains(qualified_name)
            || metaEnums.contains(enum_name)) {
        meta_enum->setMetaEnum(true);
    }else if(enumType->flags()){
        if (metaEnums.contains(enumType->flags()->qualifiedCppName())
                || metaEnums.contains(enumType->flags()->name())
                || metaEnums.contains(enumType->flags()->name().split("::").last())
                || metaEnums.contains(enumType->flags()->flagsTemplate())) {
            meta_enum->setMetaEnum(true);
        }
    }
    switch (enum_item->accessPolicy()) {
        case CodeModel::Public: *meta_enum += MetaAttributes::Public; break;
        case CodeModel::Protected:
        *meta_enum += MetaAttributes::Protected;
        meta_enum->typeEntry()->setPublic(false);
        break;
        case CodeModel::Private:
        meta_enum->typeEntry()->setPublic(false);
        break;
    }
    if(enum_item->isAnonymous()){
        *meta_enum += MetaAttributes::Annonymous;
    }
    if(enum_item->isDeclDeprecated()){
        *meta_enum += MetaAttributes::Deprecated;
        enumType->setDeclDeprecated(true);
        meta_enum->setDeprecatedComment(enum_item->getDeclDeprecatedComment());
    }
    if(enum_item->isScopedEnum()){
        meta_enum->typeEntry()->setEnumClass(true);
    }
    meta_enum->setBaseTypeName(enum_item->baseType().qualifiedName().join("::"));
    uint size = 32;
    if(!meta_enum->baseTypeName().isEmpty()){
        QString baseType = meta_enum->baseTypeName();
        TypeDatabase *types = m_database;
        TypeEntry *base_class_entry = types->findType(qPrintable(baseType));
        if(base_class_entry)
            baseType = base_class_entry->qualifiedCppName();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QMetaType baseTypeId(QMetaType::type(qPrintable(baseType)));
#else
        QMetaType baseTypeId = QMetaType::fromName(qPrintable(baseType));
#endif
        if(baseTypeId.isValid()){
            size = uint(baseTypeId.sizeOf()*8);
            if(size!=8 && size!=16 && size!=32 && size!=64){
                size = 32;
            }
        }
    }
    meta_enum->typeEntry()->setSize(size);

    ReportHandler::debugMedium(QString(" - traversing enum %1").arg(meta_enum->fullName()));

    for(const EnumeratorModelItem& value : enum_item->enumerators()) {

        MetaEnumValue *meta_enum_value = new MetaEnumValue();
        meta_enum_value->setName(value->name());
        meta_enum_value->setEnum(meta_enum);
        // Deciding the enum value...

        meta_enum_value->setStringValue(value->value());
        meta_enum_value->setDeprecated(value->deprecated());
        meta_enum_value->setDeprecatedComment(value->deprecatedComment());
        meta_enum->addEnumValue(meta_enum_value);

        ReportHandler::debugFull("   - " + meta_enum_value->name() + " = "
                                 + QString::number(meta_enum_value->value().value<qint64>()));

        // Add into global register...
        if (enclosing)
            m_enum_values[enclosing->name() + "::" + meta_enum_value->cppName()] = meta_enum_value;
        else
            m_enum_values[meta_enum_value->cppName()] = meta_enum_value;
    }

    m_enums << meta_enum;

    if (!meta_enum->typeEntry()->include().isValid()) {
        addInclude(meta_enum->typeEntry(), enum_item->fileName());
    }
    if(!enum_item->requiredFeatures().isEmpty()){
        QStringList ppConditions;
        if(!meta_enum->typeEntry()->ppCondition().isEmpty())
            ppConditions << meta_enum->typeEntry()->ppCondition();
        if (m_current_class && !m_current_class->typeEntry()->ppCondition().isEmpty())
            ppConditions << m_current_class->typeEntry()->ppCondition();
        for(const QString& feature : enum_item->requiredFeatures()){
            ppConditions << QString("QT_CONFIG(%1)").arg(feature);
        }
        ppConditions.removeDuplicates();
        static_cast<EnumTypeEntry *>(type_entry)->setPPCondition(ppConditions.join(" && "));
    }else if (m_current_class && !m_current_class->typeEntry()->ppCondition().isEmpty()){
        QStringList ppConditions;
        ppConditions << m_current_class->typeEntry()->ppCondition();
        if(!meta_enum->typeEntry()->ppCondition().isEmpty())
            ppConditions << meta_enum->typeEntry()->ppCondition();
        ppConditions.removeDuplicates();
        static_cast<EnumTypeEntry *>(type_entry)->setPPCondition(ppConditions.join(" && "));
    }
    return meta_enum;
}

MetaFunctional *MetaBuilder::traverseFunctional(TypeAliasModelItem item){
    if (m_database->isClassRejected(item->qualifiedName().join("::"))) {
        m_rejected_functionals.insert({item->qualifiedName().join("::"), item->fileName()}, GenerationDisabled);
        return nullptr;
    }
    std::unique_ptr<MetaFunctional> meta_functional;
    FunctionalTypeEntry *ftype = m_database->findFunctionalType(item->qualifiedName().join("::"));
    if(ftype){
        meta_functional.reset(new MetaFunctional());
        meta_functional->setTypeEntry(ftype);
        meta_functional->setBaseTypeName(item->name());
        meta_functional->setFunctionPointer(item->type().isFunctionPointer());
        QList<MetaArgument*> actualArguments;
        bool ok = false;
        MetaType * type = translateType(item->type().functionalReturnType(), &ok, QString("traverseFunctional %1").arg(item->type().qualifiedName().join("::")));
        if(ok){
            meta_functional->setType(type);
            int counter = 0;
            for(const TypeInfo& arg : item->type().functionalArgumentTypes()){
                MetaType * atype = translateType(arg, &ok, QString("traverseFunctional %1").arg(item->type().qualifiedName().join("::")));
                if(ok){
                    MetaArgument* argument = new MetaArgument();
                    argument->setArgumentIndex(counter);
                    if(counter<item->type().functionalArgumentNames().size())
                        argument->setName(item->type().functionalArgumentNames()[counter]);
                    argument->setType(atype);
                    for(const ArgumentModification& mod : ftype->argumentModification()){
                        if(mod.index==counter+1 && !mod.modified_name.isEmpty()){
                            argument->setName(mod.modified_name);
                        }
                    }
                    meta_functional->addArgument(argument);
                    if(!meta_functional->argumentRemoved(counter + 1))
                        actualArguments << argument;
                    ++counter;
                }else {
                    m_rejected_functionals.insert({item->qualifiedName().join("::"), item->fileName()}, UnmatchedArgumentType);
                    return nullptr;
                }
            }
        }else {
            m_rejected_functionals.insert({item->qualifiedName().join("::"), item->fileName()}, UnmatchedReturnType);
            return nullptr;
        }
        analyzeFunctional(meta_functional.get(), actualArguments);
    }else{
        if(item->accessPolicy()==CodeModel::Private || item->qualifiedName().join("::").startsWith("QtPrivate::") || item->fileName().startsWith("_p.h"))
            m_rejected_functionals.insert({item->qualifiedName().join("::"), item->fileName()}, IsPrivate);
        else
            m_rejected_functionals.insert({item->qualifiedName().join("::"), item->fileName()}, NotInTypeSystem);
    }
    return meta_functional.release();
}

MetaClass *MetaBuilder::traverseTypeAlias(TypeAliasModelItem typeAlias) {
    QString class_name = strip_template_args(typeAlias->name());
    QString full_class_name = class_name;
    // we have an inner class
    if (m_current_class) {
        full_class_name = strip_template_args(m_current_class->typeEntry()->qualifiedCppName())
                          + "::" + full_class_name;
    }

    // If we haven't specified anything for the typedef, then we don't care
    ComplexTypeEntry *type = m_database->findComplexType(full_class_name);
    if (!type)
        return nullptr;

    MetaClass *meta_class = new MetaClass();
    meta_class->setTypeAlias(true);
    meta_class->setTypeEntry(type);
    TypeInfo typeInfo = typeAlias->type();
    typeInfo.setConstant(false);
    typeInfo.setReferenceType(TypeInfo::NoReference);
    typeInfo.setIndirections({});
    TypeEntry* typeAliasEntry = m_database->findType(typeInfo.toString());
    if(typeAliasEntry){
        MetaType* typeAliasType = new MetaType();
        typeAliasType->setTypeEntry(typeAliasEntry);
        typeAliasType->setIndirections(typeAlias->type().indirections());
        typeAliasType->setConstant(typeAlias->type().isConstant());
        typeAliasType->setReferenceType(MetaType::ReferenceType(typeAlias->type().getReferenceType()));
        Q_ASSERT(typeAliasType->typeEntry());
        meta_class->setTypeAliasType(typeAliasType);
    }else{
        bool ok = false;
        MetaType* typeAliasType = translateType(typeInfo, &ok, "traverseTypeAlias " + class_name);
        if(typeAliasType && typeAliasType->typeEntry()){
            typeAliasType->setIndirections(typeAlias->type().indirections());
            typeAliasType->setConstant(typeAlias->type().isConstant());
            typeAliasType->setReferenceType(MetaType::ReferenceType(typeAlias->type().getReferenceType()));
            Q_ASSERT(typeAliasType->typeEntry());
            meta_class->setTypeAliasType(typeAliasType);
        }
        meta_class->setBaseClassTypeInfo({{typeAlias->type(), true}});
    }
    *meta_class += MetaAttributes::Public;

    // Set the default include file name
    if (!type->include().isValid()) {
        addInclude(type, typeAlias->fileName());
    }
    if(!typeAlias->requiredFeatures().isEmpty()){
        QStringList ppifs;
        if(!type->ppCondition().isEmpty()){
            ppifs << type->ppCondition();
        }
        for(const QString& feature : typeAlias->requiredFeatures()){
            ppifs << QString("QT_CONFIG(%1)").arg(feature);
        }
        ppifs.removeDuplicates();
        type->setPPCondition(ppifs.join(" && "));
    }
    return meta_class;
}

MetaClass *MetaBuilder::traverseClass(ClassModelItem class_item, QList<PendingClass>& pendingClasses, QList<PendingFunctional>& pendingFunctionals) {
    QString class_name = class_item->name();
    QString full_class_name = class_name;
    // we have an inner class
    if (m_current_class) {
        full_class_name = strip_template_args(m_current_class->typeEntry()->qualifiedCppName())
                          + "::" + full_class_name;
    }

    ComplexTypeEntry *type = m_database->findComplexType(full_class_name);
    if(type){
        if(type->isQString() && m_database->qstringType()){
            type = m_database->qstringType();
        }else if(type->isQVariant() && m_database->qvariantType()){
            type = m_database->qvariantType();
        }else if(type->isQChar() && m_database->qcharType()){
            type = m_database->qcharType();
        }
    }
    if(type && type->designatedInterface())
        type = type->designatedInterface();
    RejectReason reason = NoReason;

    if (m_database->isClassRejected(full_class_name)) {
        reason = GenerationDisabled;
    } else if (!type) {
        TypeEntry *te = m_database->findType(full_class_name);
        if (te && !te->isComplex())
            reason = RedefinedToNotClass;
        else if(class_item->accessPolicy()==CodeModel::Private || full_class_name.startsWith("QtPrivate::") || class_item->fileName().endsWith("_p.h"))
            reason = IsPrivate;
        else
            reason = NotInTypeSystem;
    } else if (type->codeGeneration() == TypeEntry::GenerateNothing) {
        reason = GenerationDisabled;
    }

    if (reason != NoReason) {
        if(class_item->isTemplate() && !class_item->templateInstantiations().isEmpty()){
            QStringList targs;
            for(const TypeInfo& ti : class_item->templateInstantiations()){
                targs << ti.toString();
            }
            full_class_name += "<" + targs.join(",") + ">";
        }
        m_rejected_classes.insert({full_class_name, class_item->fileName()}, reason);
        return nullptr;
    }

    if (type->isObject()) {
        if(full_class_name==QLatin1String("QObject")){
            type->setQObject(true);
        }else if(full_class_name==QLatin1String("QEvent")){
            type->setQEvent(true);
        }else if(full_class_name==QLatin1String("QByteArrayView")){
            type->setQByteArrayView(true);
        }else if(full_class_name==QLatin1String("QWidget")){
            type->setQWidget(true);
        }else if(full_class_name==QLatin1String("QWindow")){
            type->setQWindow(true);
        }else if(full_class_name==QLatin1String("QCoreApplication")){
            type->setQCoreApplication(true);
        }else if(full_class_name==QLatin1String("QAction")){
            type->setQAction(true);
        }else if(full_class_name==QLatin1String("QMediaControl")){
            type->setQMediaControl(true);
        }
    }

    MetaClass *meta_class = new MetaClass();
    meta_class->setTypeEntry(type);
    meta_class->setUsingProtectedBaseConstructors(class_item->usingBaseConstructors()==CodeModel::Protected);
    meta_class->setUsingPublicBaseConstructors(class_item->usingBaseConstructors()==CodeModel::Public);
    QList<QPair<TypeInfo,bool>> baseClasses = class_item->baseClasses();
    if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6, 0, 0)){
        for(int i=0; i<baseClasses.size(); ++i){
            QString baseClassName = baseClasses[i].first.toString();
            if(baseClassName=="QList<QString>"){
                baseClasses[i].first.setQualifiedName({QStringLiteral("QStringList")});
                baseClasses[i].first.setArguments({});
            }else if(baseClassName=="QByteArrayList"){
                baseClasses[i].first.setQualifiedName({QStringLiteral("QList")});
                TypeInfo arg;
                arg.setQualifiedName({QStringLiteral("QByteArray")});
                baseClasses[i].first.setArguments({arg});
            }
        }
    }
    meta_class->setBaseClassTypeInfo(baseClasses);
    meta_class->setHas_Q_GADGET(class_item->has_Q_GADGET());
    meta_class->setHas_Q_OBJECT(class_item->has_Q_OBJECT());
    *meta_class += MetaAttributes::Public;
    if(class_item->isDeclFinal()){
        *meta_class += MetaAttributes::Final;
        *meta_class += MetaAttributes::DeclaredFinalInCpp;
    }
    if(class_item->isDeclDeprecated()){
        *meta_class += MetaAttributes::Deprecated;
        type->setDeclDeprecated(true);
        meta_class->setDeprecatedComment(class_item->declDeprecatedComment());
    }

    MetaClass *old_current_class = m_current_class;
    m_current_class = meta_class;

    if (type->isContainer()) {
        ReportHandler::debugSparse(QString("container: '%1'").arg(full_class_name));
    } else {
        ReportHandler::debugSparse(QString("class: '%1'").arg(meta_class->fullName()));
    }

    const TemplateParameterList& template_parameters = class_item->templateParameters();
    QList<TypeEntry *> template_args;
    //template_args.clear();
    for (int i = 0; i < template_parameters.size(); ++i) {
        const TemplateParameterModelItem &param = template_parameters.at(i);
        TemplateArgumentEntry *param_type = new TemplateArgumentEntry(param->name());
        param_type->setOrdinal(i);
        param_type->setVariadic(param->isVaradic());
        template_args.append(param_type);
    }
    meta_class->setTemplateArguments(template_args);

    // Inner classes
    {
        for(ClassModelItem ci : class_item->classes()) {
            MetaClass *cl = traverseClass(ci, pendingClasses, pendingFunctionals);
            if (cl) {
                bool isEnclosedClass = cl->typeEntry()->targetLangName().startsWith(meta_class->typeEntry()->targetLangName()+"$");
                if(isEnclosedClass)
                    meta_class->addEnclosedClass(cl);
                addClass(cl);

            }
        }
    }

    // Go through all typedefs to see if we have defined any
    // specific typedefs to be used as classes.
    for(const TypeAliasModelItem& tai : class_item->typeAliases()){
        if(tai->type().qualifiedName()==QStringList{"std","function"}
                || tai->type().qualifiedName()==QStringList{"qxp","function_ref"}
                || tai->type().isFunctionPointer()){
            pendingFunctionals.append(PendingFunctional{tai, meta_class, m_scopes});
        }else{
            MetaClass *cls = traverseTypeAlias(tai);
            if (cls) {
                meta_class->addEnclosedClass(cls);
                addClass(cls);
            }
        }
    }

    parseQ_Property(meta_class, class_item->propertyDeclarations());

    QSet<QString> metaEnums;
    for(const FunctionModelItem& function_item : class_item->functions()){
        if(function_item->name()==QStringLiteral("qt_getEnumMetaObject")
                && function_item->arguments().size()==1){
            metaEnums << function_item->arguments()[0]->type().toString();
        }
    }
    pendingClasses.append(PendingClass{model_dynamic_cast<ScopeModelItem>(class_item), meta_class, m_scopes});
    traverseEnums(model_dynamic_cast<ScopeModelItem>(class_item), meta_class, metaEnums, class_item->flagsMap());

    m_current_class = old_current_class;

    // Set the default include file name
    if (!type->include().isValid()) {
        addInclude(type, class_item->fileName());
    }

    if(!class_item->requiredFeatures().isEmpty()){
        QStringList ppifs;
        if(!type->ppCondition().isEmpty()){
            ppifs << type->ppCondition();
        }
        for(const QString& feature : class_item->requiredFeatures()){
            ppifs << QString("QT_CONFIG(%1)").arg(feature);
        }
        ppifs.removeDuplicates();
        type->setPPCondition(ppifs.join(" && "));
    }

    // not possible to inherit from a union type, so declare final
    if(class_item.data()->classType()==CodeModel::Union){
        *meta_class += MetaAttributes::Final;
    }

    if(meta_class->typeEntry()->isTemplate()){
        const QMap<QStringList,const ComplexTypeEntry*>& instantiations = meta_class->typeEntry()->instantiations();
        for(const QStringList& args : instantiations.keys()){
            if(ComplexTypeEntry* instantiation = const_cast<ComplexTypeEntry*>(instantiations[args])){
                if (meta_class->typeEntry()->isQEvent()) {
                    instantiation->setQEvent(true);
                }else if(meta_class->typeEntry()->isQObject()){
                    instantiation->setQObject(true);
                    if(meta_class->typeEntry()->isQWidget()){
                        instantiation->setQWidget(true);
                    }else if(meta_class->typeEntry()->isQWindow()){
                        instantiation->setQWindow(true);
                    }else if(meta_class->typeEntry()->isQAction()){
                        instantiation->setQAction(true);
                    }else if(meta_class->typeEntry()->isQCoreApplication()){
                        instantiation->setQCoreApplication(true);
                    }else if(meta_class->typeEntry()->isQMediaControl()){
                        instantiation->setQMediaControl(true);
                    }
                }

                // Set the default include file name
                if (!instantiation->include().isValid()) {
                    addInclude(instantiation, class_item->fileName());
                }
                if(!class_item->requiredFeatures().isEmpty()){
                    QStringList ppifs;
                    if(!instantiation->ppCondition().isEmpty()){
                        ppifs << type->ppCondition();
                    }
                    for(const QString& feature : class_item->requiredFeatures()){
                        ppifs << QString("QT_CONFIG(%1)").arg(feature);
                    }
                    ppifs.removeDuplicates();
                    instantiation->setPPCondition(ppifs.join(" && "));
                }

                MetaClass *instantiation_meta_class = new MetaClass();
                instantiation_meta_class->setTypeEntry(instantiation);
                instantiation_meta_class->setBaseClassTypeInfo(meta_class->baseClassTypeInfo());
                instantiation_meta_class->setTemplateBaseClass(meta_class);
                instantiation_meta_class->setAttributes(meta_class->attributes());
                instantiation_meta_class->setOriginalAttributes(meta_class->originalAttributes());
                if(m_current_class)
                    m_current_class->addEnclosedClass(instantiation_meta_class);
                addClass(instantiation_meta_class);
            }
        }
    }
    return meta_class;
}

MetaField *MetaBuilder::traverseField(VariableModelItem field) {
    const QString& field_name = field->name();
    QString class_name;
    if(m_current_class)
        class_name = m_current_class->typeEntry()->qualifiedCppName();

    //qDebug()<<"\n"<<field_name<<class_name;

    // Ignore friend decl.
    if (field->isFriend())
        return nullptr;

    if (field->accessPolicy() == CodeModel::Private)
        return nullptr;
    if(field->isTemplate() && !field->templateInstantiations().isEmpty())
        return nullptr;

    if (m_database->isFieldRejected(class_name, field_name)) {
        if(field_name==QLatin1String("static_assert"))
            return nullptr;
        if(m_current_class)
            m_rejected_fields.insert({class_name + "::" + field_name, field->fileName()}, GenerationDisabled);
        else
            m_rejected_fields.insert({field_name, field->fileName()}, GenerationDisabled);
        return nullptr;
    }

    MetaField *meta_field = new MetaField();
    meta_field->setName(field_name);
    meta_field->setEnclosingClass(m_current_class);

    bool ok;
    const TypeInfo& field_type = field->type();
    //qDebug()<<"\n\n\n"<<"Class in question:"<<cls->name()<<"\n\n\n";
    MetaType *meta_type = translateType(field_type, &ok, "traverseField " + class_name);

    if (!meta_type || !ok) {
        if(m_current_class)
            ReportHandler::warning(QString("skipping field '%1::%2' with unmatched type '%3'")
                            .arg(m_current_class->name(), field_name, field_type.toString()));
        else
            ReportHandler::warning(QString("skipping field '%1' with unmatched type '%2' (%3)")
                            .arg(field_name, field_type.toString(), field->fileName()));
        delete meta_field;
        return nullptr;
    }

    meta_field->setType(meta_type);

    uint attr = 0;
    if(field->isConstExpr())
        attr |= MetaAttributes::ConstExpr;
    if (field->isStatic() || !m_current_class || m_current_class->isNamespace())
        attr |= MetaAttributes::Static;

    CodeModel::AccessPolicy policy = field->accessPolicy();
    if (policy == CodeModel::Public)
        attr |= MetaAttributes::Public;
    else if (policy == CodeModel::Protected)
        attr |= MetaAttributes::Protected;
    else
        attr |= MetaAttributes::Private;
    meta_field->setAttributes(attr);

    return meta_field;
}

void MetaBuilder::traverseFields(ScopeModelItem scope_item) {
    for(const VariableModelItem& field : scope_item->variables()) {
        MetaField *meta_field = traverseField(field);

        if (meta_field) {
            if(meta_field->type()->isPointerContainer()){
                if(field->type().toString().startsWith("QScopedPointer<")
                        || field->type().toString().startsWith("QScopedArrayPointer<")
                        || field->type().toString().startsWith("std::unique_ptr<")){
                    if(!m_current_class->typeEntry()->hasPrivateCopyConstructor()
                        && !m_current_class->typeEntry()->hasProtectedCopyConstructor()
                        && !m_current_class->typeEntry()->hasPublicCopyConstructor()){
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateCopyConstructor();});
                    }
                    if(!m_current_class->typeEntry()->hasPrivateMoveConstructor()
                        && !m_current_class->typeEntry()->hasProtectedMoveConstructor()
                        && !m_current_class->typeEntry()->hasPublicMoveConstructor()){
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateMoveConstructor();});
                    }
                }
            }
            meta_field->setOriginalAttributes(meta_field->attributes());
            if(m_current_class){
                if(m_current_class->typeEntry()->designatedInterface()){
                    m_current_class->extractInterface()->addField(meta_field);
                    if(meta_field->isPublic()){
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasFields();});
                    }else{
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasNonPublicFields();});
                    }
                }else{
                    m_current_class->addField(meta_field);
                    if(meta_field->isPublic()){
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasFields();});
                    }else{
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasNonPublicFields();});
                    }
                }
            }else{
                m_rejected_fields.insert({meta_field->name(), field->fileName()}, IsGlobal);
            }
        }else{
            if(m_current_class){
                applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasFields();});
                if (field->accessPolicy() != CodeModel::Public)
                    applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasNonPublicFields();});
                if(field->type().toString().startsWith("QScopedPointer<")
                        || field->type().toString().startsWith("QScopedArrayPointer<")
                        || field->type().toString().startsWith("std::unique_ptr<")){
                    if(!m_current_class->typeEntry()->hasPrivateCopyConstructor()
                        && !m_current_class->typeEntry()->hasProtectedCopyConstructor()
                        && !m_current_class->typeEntry()->hasPublicCopyConstructor()){
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateCopyConstructor();});
                    }
                    if(!m_current_class->typeEntry()->hasPrivateMoveConstructor()
                        && !m_current_class->typeEntry()->hasProtectedMoveConstructor()
                        && !m_current_class->typeEntry()->hasPublicMoveConstructor()){
                        applyOnType(m_current_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateMoveConstructor();});
                    }
                }
            }
        }
    }
}

void MetaBuilder::traverseFunctions(ScopeModelItem scope_item) {
    // if there is a pure virtual function which is private don't generate shell class
    QSet<QString> unimplementablePureVirtualFunctions;

    QList<MetaFunction *> rValueFunctions;
    QMap<QString,MetaFunction *> subscriptionFunctions;
    QList<FunctionModelItem> functions;
    QList<FunctionModelItem> templateInstantiations;
    for(const FunctionModelItem& function_item : scope_item->functions()) {
        if(function_item->isTemplate()){
            if(function_item->templateParameters().isEmpty()
                    || !function_item->templateInstantiations().isEmpty()){
                templateInstantiations << function_item;
            }else{
                functions << function_item;
            }
        }else{
            functions << function_item;
        }
    }
    for(const FunctionModelItem& function_item : qAsConst(functions << templateInstantiations)) {
        if(!m_current_class || m_current_class->isNamespace()){
            if(function_item->name().startsWith(QStringLiteral("qRegisterNormalizedMetaType"))){
                continue;
            }
        }
        const ArgumentList& arguments = function_item->arguments();
        QString function_name = function_item->name();
        QString _function_name = function_name;
        QStringList tparams;
        if(function_item->isTemplate()){
            for(const TypeInfo& ti : function_item->templateInstantiations()){
                tparams << ti.toString();
            }
            for(TemplateParameterModelItem t : function_item->templateParameters()){
                if(t->isVaradic())
                    tparams << t->name()+"...";
                else tparams << t->name();
            }
            _function_name += "<" + tparams.join(",") + ">";
        }
        _function_name += "(";
        int counter = 0;
        for(ArgumentModelItem arg : arguments){
            if(arg->type().getReferenceType()==TypeInfo::NoReference
                && arg->type().indirections().isEmpty()
                && arguments.size()==1
                && arg->type().toString()=="void"){
                continue;
            }
            if(counter!=0){
                _function_name += ",";
            }
            _function_name += arg->type().toString();
            ++counter;
        }
        _function_name += ") ";
        if(function_item->isConstant())
            _function_name += "const ";
        if(function_item->referenceType()==TypeInfo::Reference)
            _function_name += "& ";
        _function_name += "-> " + function_item->type().toString();
        switch(function_item->referenceType()){
        case TypeInfo::RReference:
            _function_name += "&";
            break;
        default: break;
        }
        MetaFunction *meta_function = traverseFunction(function_item, _function_name, tparams);

        if (meta_function) {
            if(m_current_class){
                if(function_item->isInline() && !meta_function->isConstructor() && !meta_function->originalSignature().isEmpty()){
                    for(MetaFunction *f : m_current_class->queryFunctionsByOriginalName(meta_function->originalName())) {
                        if(f->originalSignature()==meta_function->originalSignature()
                                || f->minimalSignature()==meta_function->minimalSignature()){
                            delete meta_function;
                            meta_function = nullptr;
                            break;
                        }
                    }
                    if(!meta_function)
                        continue;
                }else if((function_item->hasBody() || function_item->isDeclDefault()) && !function_item->isInline()){
                    for(MetaFunction *f : m_current_class->queryFunctionsByOriginalName(meta_function->originalName())) {
                        if(f->originalSignature()==meta_function->originalSignature()
                            || f->minimalSignature()==meta_function->minimalSignature()){
                            delete meta_function;
                            meta_function = nullptr;
                            break;
                        }
                    }
                    if(!meta_function)
                        continue;
                }
            }else{
                meta_function->setFunctionType(MetaFunction::GlobalScopeFunction);
            }
            bool isRemoved = false;
            bool hasInstantiations = false;
            MetaClass* targetClass = nullptr;
            bool targetClassGlobalSet = false;
            FunctionModificationList functionModificationList;
            meta_function->setOriginalAttributes(meta_function->attributes());
            if(!m_current_class || meta_function->isInGlobalScope()){
                const QList<TypeSystemTypeEntry*> typeSystems = m_database->typeSystems();
                for(const TypeSystemTypeEntry* ts : typeSystems){
                    if(!ts->functionModifications().isEmpty()){
                        FunctionModificationList list = meta_function->modifications(ts);
                        for(const FunctionModification& mod : qAsConst(list)){
                            if(mod.removal & TS::TargetLangCode){
                                isRemoved = true;
                            }
                            if(function_item->isTemplate()){
                                hasInstantiations |= !mod.template_instantiations.isEmpty();
                            }
                            if(!mod.targetType.isEmpty()){
                                targetClassGlobalSet = true;
                                targetClass = m_meta_classes.findClass(ts->name() + "." + mod.targetType);
                                if(!targetClass){
                                    ComplexTypeEntry *gte = m_database->findComplexType(mod.targetType);
                                    if(!gte){
                                        gte = new NamespaceTypeEntry(mod.targetType, true, false);
                                        gte->setTargetTypeSystem(ts->name());
                                        gte->setTargetLangPackage(ts->name());
                                        gte->setCodeGeneration(TypeEntry::GenerateAll);
                                    }
                                    targetClass = new MetaClass();
                                    targetClass->setTypeEntry(gte);
                                    *targetClass += MetaAttributes::Final;
                                    *targetClass += MetaAttributes::Public;
                                    m_meta_classes << targetClass;
                                }
                                functionModificationList << list;
                                break;
                            }
                        }
                        if(targetClass)
                            break;
                    }
                }
                *meta_function += MetaAttributes::Static;
                *meta_function += MetaAttributes::Public;
            }else if(m_current_class->isNamespace()){
                FunctionModificationList list = meta_function->modifications(m_current_class);
                for(const FunctionModification& mod : qAsConst(list)){
                    if(mod.removal & TS::TargetLangCode){
                        isRemoved = true;
                    }
                    if(function_item->isTemplate()){
                        hasInstantiations |= !mod.template_instantiations.isEmpty();
                    }
                    if(!mod.targetType.isEmpty()){
                        targetClassGlobalSet = true;
                        targetClass = m_meta_classes.findClass(m_current_class->typeEntry()->targetTypeSystem() + "." + mod.targetType);
                        if(!targetClass){
                            ComplexTypeEntry *gte = m_database->findComplexType(mod.targetType);
                            if(!gte){
                                gte = new NamespaceTypeEntry(mod.targetType, true, false);
                                gte->setTargetTypeSystem(m_current_class->typeEntry()->targetTypeSystem());
                                gte->setTargetLangPackage(m_current_class->typeEntry()->targetTypeSystem());
                                gte->setCodeGeneration(TypeEntry::GenerateAll);
                            }
                            targetClass = new MetaClass();
                            targetClass->setTypeEntry(gte);
                            *targetClass += MetaAttributes::Final;
                            *targetClass += MetaAttributes::Public;
                            m_meta_classes << targetClass;
                        }
                        functionModificationList << list;
                        break;
                    }
                }
                if(targetClass)
                    break;
                *meta_function += MetaAttributes::Static;
                *meta_function += MetaAttributes::Public;
            }else{
                for(const FunctionModification& mod : meta_function->modifications(m_current_class)){
                    if (mod.isPrivateSignal()) {
                        meta_function->setFunctionType(MetaFunction::PrivateSignalFunction);
                    }
                    if(mod.removal & TS::TargetLangCode){
                        isRemoved = true;
                    }
                    if(function_item->isTemplate()){
                        hasInstantiations |= !mod.template_instantiations.isEmpty();
                    }
                }
            }

            if(!targetClass && !meta_function->isInGlobalScope())
                targetClass = m_current_class;

            if (!meta_function->isInvalid() && targetClass) {
                if (QPropertySpec *read = targetClass->propertySpecForRead(meta_function->name())) {
                    //if (meta_function->type() && read->type() == meta_function->type()->typeEntry()) {
                        *meta_function += MetaAttributes::PropertyReader;
                        meta_function->setPropertySpec(read);
                    //}
                } else if (QPropertySpec *write = targetClass->propertySpecForWrite(meta_function->name())) {
                    //if (meta_function->arguments().at(0)->type() &&
                    //        write->type() == meta_function->arguments().at(0)->type()->typeEntry()) {
                        *meta_function += MetaAttributes::PropertyWriter;
                        meta_function->setPropertySpec(write);
                    //}
                } else if (QPropertySpec *reset = targetClass->propertySpecForReset(meta_function->name())) {
                    *meta_function += MetaAttributes::PropertyResetter;
                    meta_function->setPropertySpec(reset);
                } else if (QPropertySpec *reset = targetClass->propertySpecForBindable(meta_function->name())) {
                    *meta_function += MetaAttributes::PropertyBindable;
                    meta_function->setPropertySpec(reset);
                } else if (QPropertySpec *reset = targetClass->propertySpecForNotify(meta_function->name())) {
                    *meta_function += MetaAttributes::PropertyNotify;
                    meta_function->setPropertySpec(reset);
                }
            }

            if(targetClass){
                if(targetClassGlobalSet){
                    if(targetClass->typeEntry()==m_database->qvariantType()){
                        if(meta_function->type() && meta_function->type()->typeEntry()->isQVariant())
                            meta_function->type()->setTypeEntry(m_database->qvariantType());
                        for(MetaArgument* arg : meta_function->arguments()){
                            if(arg->type()->typeEntry()->isQVariant())
                                arg->type()->setTypeEntry(m_database->qvariantType());
                        }
                    }else if(targetClass->typeEntry()==m_database->qstringType()){
                        if(meta_function->type() && meta_function->type()->typeEntry()->isQString())
                            meta_function->type()->setTypeEntry(m_database->qstringType());
                        for(MetaArgument* arg : meta_function->arguments()){
                            if(arg->type()->typeEntry()->isQString())
                                arg->type()->setTypeEntry(m_database->qstringType());
                        }
                    }else if(targetClass->typeEntry()==m_database->qcharType()){
                        if(meta_function->type() && meta_function->type()->typeEntry()->isQChar())
                            meta_function->type()->setTypeEntry(m_database->qcharType());
                        for(MetaArgument* arg : meta_function->arguments()){
                            if(arg->type()->typeEntry()->isQChar())
                                arg->type()->setTypeEntry(m_database->qcharType());
                        }
                    }
                }
                if(meta_function->isDestructor()){
                    if(function_item->isDeleted() || (function_item->accessPolicy() & CodeModel::Private)){
                        applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setDestructorPrivate();});
                    }else if(function_item->accessPolicy() & CodeModel::Protected){
                        applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setDestructorProtected();});
                    }
                    if(function_item->isVirtual())
                        applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setDestructorVirtual();});
                    delete meta_function;
                    meta_function = nullptr;
                    continue;
                }else if(meta_function->isConstructor()){
                    bool isDefaultConstructor = false;
                    bool isCopyConstructor = false;
                    bool isMoveConstructor = false;
                    if(!meta_function->isInvalid()){
                        const MetaArgumentList& arguments = meta_function->arguments();
                        if(arguments.isEmpty()){
                            isDefaultConstructor = true;
                        }else if(arguments.size()==1){
                            MetaArgument* arg = arguments[0];
                            isDefaultConstructor = !arg->originalDefaultValueExpression().isEmpty();
                            if(!isDefaultConstructor
                                    && (arg->type()->typeEntry()==targetClass->typeEntry()
                                        || arg->type()->typeEntry()==targetClass->typeEntry()->designatedInterface()
                                        || arg->type()->typeEntry()->designatedInterface()==targetClass->typeEntry())
                                    && arg->type()->indirections().isEmpty()){
                                if(arg->type()->isConstant() && arg->type()->getReferenceType()==MetaType::Reference){
                                    isCopyConstructor = true;
                                }else if(!arg->type()->isConstant() && arg->type()->getReferenceType()==MetaType::RReference){
                                    isMoveConstructor = true;
                                }
                            }
                        }else{
                            isDefaultConstructor = true;
                            for(MetaArgument* arg : arguments){
                                if(arg->originalDefaultValueExpression().isEmpty()){
                                    isDefaultConstructor = false;
                                    break;
                                }
                            }
                        }
                    }
                    if((function_item->accessPolicy() & CodeModel::Private) || function_item->isDeleted()){
                        if(isDefaultConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateDefaultConstructor();});
                        }else if(isCopyConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateCopyConstructor();});
                        }else if(isMoveConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateMoveConstructor();});
                        }else{
                            if(!targetClass->typeEntry()->hasPublicDefaultConstructor()
                                    && !targetClass->typeEntry()->hasProtectedDefaultConstructor()){
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateDefaultConstructor();});
                            }
                        }
                    }else if((function_item->accessPolicy() & CodeModel::Protected)){
                        if(isDefaultConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasProtectedDefaultConstructor();});
                        }else if(isCopyConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasProtectedCopyConstructor();});
                        }else if(isMoveConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasProtectedMoveConstructor();});
                        }else{
                            if(!targetClass->typeEntry()->hasPublicDefaultConstructor()
                                    && !targetClass->typeEntry()->hasProtectedDefaultConstructor()){
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateDefaultConstructor();});
                            }
                        }
                    }else{
                        if(isDefaultConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicDefaultConstructor();});
                        }else if(isCopyConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicCopyConstructor();});
                        }else if(isMoveConstructor){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicMoveConstructor();});
                        }else{
                            if(!targetClass->typeEntry()->hasPublicDefaultConstructor()
                                    && !targetClass->typeEntry()->hasProtectedDefaultConstructor()){
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateDefaultConstructor();});
                            }
                        }
                    }
                    if(meta_function->isInvalid() || (function_item->accessPolicy() & CodeModel::Private) || function_item->isDeleted()){
                        if(!targetClass->typeEntry()->hasPrivateConstructors()){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateConstructors();});
                        }
                    }else{
                        if(!targetClass->typeEntry()->hasNonPrivateConstructors()){
                            applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasNonPrivateConstructors();});
                        }
                    }
                }else if((/*meta_function->isPrivate() ||*/ meta_function->isInvalid()) && meta_function->isAbstract()){
                    ReportHandler::warning("Unimplementable pure virtual function: "+targetClass->qualifiedCppName()+"::"+_function_name);
                    unimplementablePureVirtualFunctions << meta_function->signature();
                }else if(!meta_function->isInvalid()){
                    if(meta_function->operatorType()==OperatorType::Assign){
                        bool isDefaultAssignment = false;
                        bool isMoveAssignment = false;
                        const MetaArgumentList& arguments = meta_function->arguments();
                        if(arguments.size()==1){
                            MetaArgument* arg = arguments[0];
                            if(arg->type()->typeEntry()==targetClass->typeEntry() && arg->type()->indirections().isEmpty()){
                                if(!arg->type()->isConstant() && arg->type()->getReferenceType()==MetaType::RReference){
                                    isMoveAssignment = true;
                                }else if(arg->type()->isConstant() && arg->type()->getReferenceType()==MetaType::Reference){
                                    isDefaultAssignment = true;
                                }
                            }
                        }
                        if(meta_function->isPrivate() || function_item->isDeleted()){
                            if(isDefaultAssignment)
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateDefaultAssignment();});
                            else if(isMoveAssignment)
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPrivateMoveAssignment();});
                        }else if(meta_function->isProtected()){
                            if(isDefaultAssignment)
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasProtectedDefaultAssignment();});
                            else if(isMoveAssignment)
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasProtectedMoveAssignment();});
                        }else{
                            if(isDefaultAssignment)
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicDefaultAssignment();});
                            else if(isMoveAssignment)
                                applyOnType(targetClass->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicMoveAssignment();});
                        }
                    }else if((meta_function->operatorType()==OperatorType::Equals
                                 || meta_function->operatorType()==OperatorType::NotEquals)
                            && meta_function->arguments().size()==1){
                        MetaArgument* arg = meta_function->arguments()[0];
                        if(arg->type()->typeEntry()==targetClass->typeEntry() && arg->type()->indirections().isEmpty()
                                && arg->type()->isConstant() && arg->type()->getReferenceType()==MetaType::Reference
                                && !meta_function->isPrivate() && !function_item->isDeleted()){
                            targetClass->typeEntry()->setHasEquals();
                        }
                    }else if(function_item->isDeleted()){
                        targetClass->addDeletedFunction(meta_function);
                    }
                    if(!meta_function->isFinalInCpp()){
                        targetClass->typeEntry()->setHasVirtualFunctions();
                        if(meta_function->isAbstract()){
                            targetClass->typeEntry()->setHasPureVirtualFunctions(!meta_function->isPrivate());
                        }
                    }else{
                        targetClass->typeEntry()->setHasFinalFunctions();
                    }
                }
            }

            if (!meta_function->isDestructor()
                    && !meta_function->isInvalid()
                    && (!meta_function->isConstructor() || !meta_function->isPrivate())) {

                if (targetClass && targetClass->typeEntry()->designatedInterface() && !meta_function->isPublic()
                        && !meta_function->isPrivate()) {
                    QString warn = QString("non-public function '%1' in interface '%2'")
                                   .arg(meta_function->name(), targetClass->name());
                    ReportHandler::warning(warn);

//                  don't make public! just skip protected methods in the interface
                    //meta_function->setVisibility(AbstractMetaClass::Public);
                }

                meta_function->setDeclaringClass(targetClass);
                meta_function->setImplementingClass(targetClass);

                /*if (meta_function->isSignal() && targetClass->hasSignal(meta_function)) { // overloaded signals are possible now
                    QString warn = QString("signal '%1' in class '%2' is overloaded.")
                                   .arg(meta_function->name()).arg(targetClass->name());
                    ReportHandler::warning(warn);
                }*/

                if (meta_function->isSignal() && targetClass && !targetClass->isQObject()) {
                    QString warn = QString("signal '%1' in non-QObject class '%2'")
                                   .arg(meta_function->name(), targetClass->name());
                    ReportHandler::warning(warn);
                }
                if(meta_function->hasRReferences()
                        && !meta_function->isPrivate()
                        && !meta_function->isRemovedFrom(targetClass, TS::TargetLangCode)){
                    rValueFunctions << meta_function;
                }
                if(targetClass && targetClass->typeEntry()->isNamespace()){
                    addInclude(targetClass->typeEntry(), function_item->fileName(), true);
                }
                // convert operator[] to get and set method...
                if(meta_function->operatorType()==OperatorType::Subscript
                        && meta_function->name()=="operator_subscript"
                        && meta_function->type()){
                    subscriptionFunctions[meta_function->originalSignature()] = meta_function;
                }

                bool skip = function_item->isDeleted();
                QString oldMinimalSignature;
                if(!skip && (!targetClass
                             || (targetClassGlobalSet && meta_function->operatorType()!=OperatorType::None)
                             || (targetClass==m_current_class && m_current_class->isNamespace()))){
                    if(meta_function->operatorType()!=OperatorType::None){
                        const MetaArgumentList& arguments = meta_function->arguments();
                        switch(meta_function->operatorType()){
                        case OperatorType::ExclusiveAnd:
                        case OperatorType::ExclusiveOr:
                        case OperatorType::Negation:
                        case OperatorType::Less:
                        case OperatorType::Greater:
                        case OperatorType::LessOrEquals:
                        case OperatorType::Compare:
                        case OperatorType::GreaterOrEquals:
                        case OperatorType::NotEquals:
                        case OperatorType::Equals:
                        case OperatorType::Plus:
                        case OperatorType::Minus:
                        case OperatorType::Times:
                        case OperatorType::Div:
                        case OperatorType::Rem:
                            if(arguments.size()>=1 && arguments[0]->type()->getReferenceType()!=MetaType::RReference){
                                if (MetaClass *cls = argumentToClass(arguments[0]->type())) {
                                    if(m_current_class
                                            && !targetClassGlobalSet
                                            && !m_current_class->isNamespace()
                                            && function_item->isFriend()
                                            && cls!=m_current_class){
                                        delete meta_function;
                                        meta_function = nullptr;
                                        continue;
                                    }
                                    meta_function->setFunctionType(MetaFunction::GlobalScopeFunction);
                                    oldMinimalSignature = meta_function->minimalSignature();
                                    // Strip away last argument, since that is the containing object
                                    MetaArgumentList arguments = meta_function->arguments();
                                    arguments.removeFirst();
                                    if(cls->typeEntry()==m_database->qvariantType()){
                                        if(meta_function->type() && meta_function->type()->typeEntry()->isQVariant())
                                            meta_function->type()->setTypeEntry(m_database->qvariantType());
                                        for(MetaArgument* arg : arguments){
                                            if(arg->type()->typeEntry()->isQVariant())
                                                arg->type()->setTypeEntry(m_database->qvariantType());
                                        }
                                    }else if(cls->typeEntry()==m_database->qstringType()){
                                        if(meta_function->type() && meta_function->type()->typeEntry()->isQString())
                                            meta_function->type()->setTypeEntry(m_database->qstringType());
                                        for(MetaArgument* arg : arguments){
                                            if(arg->type()->typeEntry()->isQString())
                                                arg->type()->setTypeEntry(m_database->qstringType());
                                        }
                                    }else if(cls->typeEntry()==m_database->qcharType()){
                                        if(meta_function->type() && meta_function->type()->typeEntry()->isQChar())
                                            meta_function->type()->setTypeEntry(m_database->qcharType());
                                        for(MetaArgument* arg : arguments){
                                            if(arg->type()->typeEntry()->isQChar())
                                                arg->type()->setTypeEntry(m_database->qcharType());
                                        }
                                    }
                                    meta_function->setArguments(arguments);
                                    meta_function->setOriginalAttributes(meta_function->attributes());

                                    *meta_function += MetaAttributes::Final;
                                    *meta_function += MetaAttributes::Public;
                                    *meta_function -= MetaAttributes::Static;
                                    targetClass = cls;

                                    const MetaFunctionList otherFunctions = cls->queryFunctionsByName(meta_function->name());
                                    bool skip = false;
                                    for(MetaFunction *other : otherFunctions){
                                        if(!other->isRemovedFrom(cls, TS::TargetLangCode)
                                                && other->arguments().size()==arguments.size()){
                                            skip = true;
                                            for(auto l = other->arguments().size(), i = decltype(l)(0); i<l; ++i){
                                                if(other->arguments()[i]->type()->typeEntry()!=arguments[i]->type()->typeEntry()
                                                        || other->arguments()[i]->type()->indirections().size()!=arguments[i]->type()->indirections().size()){
                                                    skip = false;
                                                    break;
                                                }
                                            }
                                            if(skip){
                                                targetClass = nullptr;
                                                break;
                                            }
                                        }
                                    }
                                    if(!skip && (meta_function->operatorType()==OperatorType::Equals
                                                 || meta_function->operatorType()==OperatorType::NotEquals)
                                            && arguments.size()==1){
                                        MetaArgument* arg = arguments[0];
                                        if(arg->type()->typeEntry()==cls->typeEntry() && arg->type()->indirections().isEmpty()
                                                && arg->type()->isConstant() && arg->type()->getReferenceType()==MetaType::Reference){
                                            cls->typeEntry()->setHasEquals();
                                        }
                                    }
                                }
                            }else if(!targetClassGlobalSet){
                                targetClass = nullptr;
                            }
                            break;
                        case OperatorType::ShiftRight:
                        case OperatorType::ShiftLeft:
                            if(arguments.size()==2){
                                if(((arguments[0]->type()->getReferenceType()==MetaType::Reference
                                     && !arguments[0]->type()->isConstant())
                                    || (arguments[0]->type()->getReferenceType()==MetaType::NoReference
                                        && !arguments[0]->type()->isConstant() && arguments[0]->type()->indirections().isEmpty()))
                                        && meta_function->type()
                                        && meta_function->type()->isConstant()==arguments[0]->type()->isConstant()
                                        && meta_function->type()->getReferenceType()==arguments[0]->type()->getReferenceType()
                                        && meta_function->type()->typeEntry()==arguments[0]->type()->typeEntry()){
                                    if(meta_function->operatorType()==OperatorType::ShiftLeft && arguments[0]->type()->typeEntry()->qualifiedCppName()=="QDebug"){
                                        if (MetaClass *cls = argumentToClass(arguments[1]->type())) {
                                            if (arguments[1]->type()->actualIndirections()<=1) {
                                                cls->setToStringCapability(meta_function);
                                                addInclude(cls->typeEntry(), function_item->fileName(), true);
                                            }
                                        }
                                        skip = true;
                                        break;
                                    }else if(arguments[0]->type()->getReferenceType()==MetaType::Reference
                                             && (arguments[0]->type()->typeEntry()->qualifiedCppName()=="QDataStream"
                                                || arguments[0]->type()->typeEntry()->qualifiedCppName()=="QTextStream")){
                                        targetClass = nullptr;
                                        if (MetaClass *cls = argumentToClass(arguments[1]->type())) {
                                            meta_function->setFunctionType(MetaFunction::GlobalScopeFunction);
                                            oldMinimalSignature = meta_function->minimalSignature();
                                            meta_function->setName(meta_function->operatorType()==OperatorType::ShiftRight ? "readFrom" : "writeTo");
                                            // Strip away last argument, since that is the containing object
                                            MetaArgumentList arguments = meta_function->arguments();
                                            arguments.removeLast();
                                            meta_function->setArguments(arguments);
                                            meta_function->setOriginalAttributes(meta_function->attributes());

                                            *meta_function += MetaAttributes::Final;
                                            *meta_function += MetaAttributes::Public;
                                            *meta_function -= MetaAttributes::Static;
                                            meta_function->setType(nullptr);

                                            FunctionModification mod;
                                            mod.signature = meta_function->minimalSignature();
                                            mod.modifiers |= Modification::NonFinal;
                                            cls->typeEntry()->addFunctionModification(mod);
                                            targetClass = cls;
                                        }
                                        break;
                                    }else{
                                        if(arguments[0]->type()->getReferenceType()==MetaType::Reference && arguments[0]->type()->typeEntry()->isComplex()){
                                            bool isRenamed = false;
//                                            bool isRemoved = false;
//                                            bool isReturnChanged = false;
                                            for(const FunctionModification& mod : dynamic_cast<const TS::ComplexTypeEntry*>(arguments[0]->type()->typeEntry())->functionModifications(meta_function->minimalSignature())){
                                                if (mod.isRenameModifier())
                                                    isRenamed = true;
//                                                if(mod.removal & TS::TargetLangCode)
//                                                    isRemoved = true;
//                                                for(const ArgumentModification& argumentModification : mod.argument_mods){
//                                                    if(argumentModification.type==ArgumentModification::Default){
//                                                        if(argumentModification.index==0)
//                                                            isReturnChanged = true;
//                                                    }
//                                                }
                                            }
                                            if(!isRenamed){
                                                if(meta_function->operatorType()==OperatorType::ShiftLeft){
                                                    meta_function->setName("append");
                                                }else{
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Q_FALLTHROUGH();
                        default:
                            if(arguments.isEmpty() || (
                                        arguments.size()>=1 && arguments[0]->type()->getReferenceType()!=MetaType::RReference
                                        )){
                                if (MetaClass *cls = argumentToClass(meta_function->type())) {
                                    if(m_current_class
                                            && !targetClassGlobalSet
                                            && !m_current_class->isNamespace()
                                            && function_item->isFriend()
                                            && cls!=m_current_class){
                                        delete meta_function;
                                        meta_function = nullptr;
                                        continue;
                                    }
                                    if(cls->typeEntry()==m_database->qvariantType()){
                                        if(meta_function->type() && meta_function->type()->typeEntry()->isQVariant())
                                            meta_function->type()->setTypeEntry(m_database->qvariantType());
                                        for(MetaArgument* arg : arguments){
                                            if(arg->type()->typeEntry()->isQVariant())
                                                arg->type()->setTypeEntry(m_database->qvariantType());
                                        }
                                    }else if(cls->typeEntry()==m_database->qstringType()){
                                        if(meta_function->type() && meta_function->type()->typeEntry()->isQString())
                                            meta_function->type()->setTypeEntry(m_database->qstringType());
                                        for(MetaArgument* arg : arguments){
                                            if(arg->type()->typeEntry()->isQString())
                                                arg->type()->setTypeEntry(m_database->qstringType());
                                        }
                                    }else if(cls->typeEntry()==m_database->qcharType()){
                                        if(meta_function->type() && meta_function->type()->typeEntry()->isQChar())
                                            meta_function->type()->setTypeEntry(m_database->qcharType());
                                        for(MetaArgument* arg : arguments){
                                            if(arg->type()->typeEntry()->isQChar())
                                                arg->type()->setTypeEntry(m_database->qcharType());
                                        }
                                    }
                                    if(arguments.isEmpty() || (
                                                arguments.size()>=1 && arguments[0]->type()->typeEntry()==cls->typeEntry()
                                                )){
                                        meta_function->setFunctionType(MetaFunction::GlobalScopeFunction);
                                        oldMinimalSignature = meta_function->minimalSignature();
                                        // Strip away last argument, since that is the containing object
                                        MetaArgumentList arguments = meta_function->arguments();
                                        arguments.removeFirst();
                                        meta_function->setArguments(arguments);
                                        meta_function->setOriginalAttributes(meta_function->attributes());

                                        *meta_function += MetaAttributes::Final;
                                        *meta_function += MetaAttributes::Public;
                                        *meta_function -= MetaAttributes::Static;
                                        targetClass = cls;

                                        MetaFunctionList otherFunctions = cls->queryFunctionsByName(meta_function->name());
                                        if(cls->isInterface()){
                                            if(MetaClass *ifaceImpl = cls->extractInterfaceImpl())
                                                otherFunctions << ifaceImpl->queryFunctionsByName(meta_function->name());
                                        }
                                        bool skip = false;
                                        for(MetaFunction *other : qAsConst(otherFunctions)){
                                            if(!other->isRemovedFrom(cls, TS::TargetLangCode)
                                                    && other->arguments().size()==arguments.size()){
                                                skip = true;
                                                for(auto l = other->arguments().size(), i = decltype(l)(0); i<l; ++i){
                                                    if(other->arguments()[i]->type()->typeEntry()!=arguments[i]->type()->typeEntry()
                                                            || other->arguments()[i]->type()->indirections().size()!=arguments[i]->type()->indirections().size()){
                                                        skip = false;
                                                        break;
                                                    }
                                                }
                                                if(skip){
                                                    targetClass = nullptr;
                                                    break;
                                                }
                                            }
                                        }
                                    }else if(!targetClassGlobalSet){
                                        targetClass = nullptr;
                                    }
                                }
                            }
                            break;
                        }
                    }else if(meta_function->originalName()==QStringLiteral("qHash")){
                        const MetaArgumentList& arguments = meta_function->arguments();
                        bool isWorkaround = false;
                        bool hasDefaultArgs = arguments.size() > 0;
                        for(int i=1; i<arguments.size(); i++){
                            if(arguments.at(i)->type()->typeEntry()->qualifiedCppName()==QStringLiteral("QHashDummyValue")){
                                isWorkaround = true;
                                hasDefaultArgs = true;
                            }else{
                                hasDefaultArgs &= !arguments.at(i)->defaultValueExpression().isEmpty();
                            }
                        }
                        if (arguments.size() == 1 || hasDefaultArgs || isWorkaround){
                            if (MetaClass *cls = argumentToClass(arguments.at(0)->type())) {
                                addInclude(cls->typeEntry(), function_item->fileName(), true);
                                cls->typeEntry()->setHasHash();
                                cls->setNeedsHashWorkaround(isWorkaround);
                                cls->setQHashScope(m_current_class && !meta_function->isInGlobalScope() ? m_current_class->qualifiedCppName() : QString{});
                                skip = true;
                            }
                        }
                        if(!skip && !meta_function->templateParameters().isEmpty())
                            skip = true;
                    }else if(meta_function->originalName()==QStringLiteral("swap")){
                        const MetaArgumentList& arguments = meta_function->arguments();
                        if(arguments.size()==2
                                && arguments[0]->type()->typeEntry()==arguments[1]->type()->typeEntry()
                                && !arguments[0]->type()->isConstant()
                                && !arguments[1]->type()->isConstant()
                                && arguments[0]->type()->getReferenceType()==arguments[1]->type()->getReferenceType()
                                && arguments[0]->type()->instantiations()==arguments[1]->type()->instantiations()
                                && arguments[0]->type()->indirections().isEmpty()
                                && arguments[1]->type()->indirections().isEmpty()){
                            if (MetaClass *cls = argumentToClass(arguments.at(0)->type())) {
                                MetaFunctionList swapFunctions = cls->queryFunctionsByName("swap");
                                const TypeEntry* implEntry = nullptr;
                                if(cls->isInterface()){
                                    if(MetaClass *ifaceImpl = cls->extractInterfaceImpl()){
                                        swapFunctions << ifaceImpl->queryFunctionsByName("swap");
                                        implEntry = ifaceImpl->typeEntry();
                                    }
                                }
                                bool skip = false;
                                for(MetaFunction *other : qAsConst(swapFunctions)){
                                    if(!other->isRemovedFrom(cls, TS::TargetLangCode)
                                            && other->arguments().size()==1
                                            && (other->arguments()[0]->type()->typeEntry()==cls->typeEntry()
                                                || other->arguments()[0]->type()->typeEntry()==implEntry)){
                                        skip = true;
                                        break;
                                    }
                                }
                                if(!skip){
                                    meta_function->setFunctionType(MetaFunction::GlobalScopeFunction);
                                    *meta_function += MetaAttributes::Final;
                                    *meta_function += MetaAttributes::Public;
                                    *meta_function -= MetaAttributes::Static;
                                    oldMinimalSignature = meta_function->minimalSignature();
                                    MetaArgumentList arguments = meta_function->arguments();
                                    arguments.removeFirst();
                                    meta_function->setArguments(arguments);
                                    targetClass = cls;
                                }else{
                                    targetClass = nullptr;
                                }
                            }else if(!meta_function->templateParameters().isEmpty()){
                                skip = true;
                            }
                        }
                    }
                }
                if(!skip){
                    if(targetClass){
                        if(targetClass->typeEntry()->designatedInterface()){
                            targetClass = targetClass->extractInterface();
                        }

                        if((targetClass->typeEntry()->codeGeneration() & TypeEntry::GenerateCode)
                                && !function_item->isDeleted()
                                && function_item->accessPolicy() != CodeModel::Private
                                && function_item->isTemplate()
                                && !hasInstantiations
                                && !isRemoved
                                && meta_function->operatorType()==OperatorType::None){
                            bool skipWarning = false;
                            MetaFunctionList functions = targetClass->queryFunctionsByOriginalName(function_item->name());
                            MetaFunctionList foundTemplates;
                            for(auto l=functions.size(), i = decltype(l)(0); i<l; ++i){
                                if(functions[i]->arguments().size() && meta_function->arguments().size()){
                                    if(functions[i]->isTemplate()){
                                        foundTemplates << functions[i];
                                    }
                                }
                            }
                            if(function_item->templateInstantiations().isEmpty()){
                                if(function_item->templateParameters().isEmpty()){
                                    if(foundTemplates.size()==1){
                                        if(foundTemplates[0]->isStatic()){
                                            *meta_function += MetaAttributes::Static;
                                        }
                                    }
                                    skipWarning = true;
                                }else if(function_item->templateParameters().size()==1){
                                    if(function_item->templateParameters()[0]->name().isEmpty()
                                            && function_item->templateParameters()[0]->defaultValue()==QStringLiteral("void")){
                                        // this is Q_WEAK_OVERLOAD
                                        skipWarning = true;
                                    }
                                }else{
                                    skipWarning = true;
                                    for(auto l=function_item->templateParameters().size(), i = decltype(l)(0); i<l; ++i){
                                        if(function_item->templateParameters()[0]->defaultValue().isEmpty()){
                                            skipWarning = false;
                                            break;
                                        }
                                    }
                                }
                            }else{
                                if(foundTemplates.size()==1){
                                    if(foundTemplates[0]->isStatic()){
                                        *meta_function += MetaAttributes::Static;
                                    }
                                }
                            }
                            if(!skipWarning){
//                                ReportHandler::warning(QString("Untreated template method %1::%2").arg(targetClass->qualifiedCppName(), meta_function->minimalSignature()));
                            }
                        }

                        addInclude(targetClass->typeEntry(), function_item->fileName(), true);
                        meta_function->setDeclaringClass(targetClass);
                        meta_function->setImplementingClass(targetClass);
                        if(targetClass->isNamespace())
                            *meta_function += MetaAttributes::Static;
                        if(meta_function->isTemplate() && targetClass->functionBySignature().contains(meta_function->minimalSignature())){
                            delete meta_function;
                            continue;
                        }
                        targetClass->addFunction(meta_function);

                        for(FunctionModification mod : qAsConst(functionModificationList)){
                            if(!mod.argument_mods.isEmpty()
                                    || !mod.template_instantiations.isEmpty()
                                    || !mod.association.isEmpty()
                                    || !mod.ppCondition.isEmpty()
                                    || mod.isCodeInjection()
                                    || !mod.throws.isEmpty()){
                                mod.signature = meta_function->minimalSignature();
                                if(targetClass->typeEntry()->designatedInterface())
                                    targetClass->typeEntry()->designatedInterface()->addFunctionModification(mod);
                                else
                                    targetClass->typeEntry()->addFunctionModification(mod);
                            }
                        }
                        if(!oldMinimalSignature.isEmpty() && oldMinimalSignature!=meta_function->minimalSignature()){
                            for(const FunctionModification& mod : targetClass->typeEntry()->functionModifications()){
                                if (mod.signature == oldMinimalSignature) {
                                    const_cast<QString&>(mod.signature) = meta_function->minimalSignature();
                                }
                            }
                        }
                    }else{
                        if(function_item->functionType() == CodeModel::Signal
                                || function_item->functionType() == CodeModel::PrivateSignal){
                            m_rejected_signals.insert({_function_name, function_item->fileName()}, IsGlobal);
                        }else if(!function_item->templateParameters().isEmpty()){
                            m_rejected_template_functions.insert({_function_name, function_item->fileName()}, IsGlobal);
                        }else{
                            m_rejected_functions.insert({_function_name, function_item->fileName()}, IsGlobal);
                        }
                    }
                }
            }else if(!meta_function->isDestructor()
                     && !meta_function->isConstructor()
                     && meta_function->isInvalid()
                     && !function_item->isDeleted()){
                if(targetClass){
                    if(targetClass->typeEntry()->designatedInterface() && !meta_function->isConstructor()){
                        targetClass = targetClass->extractInterface();
                    }else if(meta_function->isConstructor() && targetClass->typeEntry()->isInterface()){
                        targetClass = targetClass->extractInterfaceImpl();
                    }
                    meta_function->setDeclaringClass(targetClass);
                    meta_function->setImplementingClass(targetClass);
                    targetClass->addInvalidFunction(meta_function);
                }else{
                    if(function_item->functionType() == CodeModel::Signal
                            || function_item->functionType() == CodeModel::PrivateSignal){
                        m_rejected_signals.insert({_function_name, function_item->fileName()}, IsGlobal);
                    }else if(!function_item->templateParameters().isEmpty()){
                        m_rejected_template_functions.insert({_function_name, function_item->fileName()}, IsGlobal);
                    }else{
                        m_rejected_functions.insert({_function_name, function_item->fileName()}, IsGlobal);
                    }
                }
            }
        }
    }
    if(m_current_class){
        // remove duplicates by rvalue
        for(MetaFunction *meta_function : qAsConst(rValueFunctions)){
            for(MetaFunction * other : m_current_class->functions()){
                if(meta_function!=other
                        && other->name()==meta_function->name()
                        && other->arguments().size()==meta_function->arguments().size()){
                    bool isSame = true;
                    for(int i=0; i<meta_function->arguments().size(); ++i){
                        if(other->arguments()[i]->type()->typeEntry()!=meta_function->arguments()[i]->type()->typeEntry()){
                            isSame = false;
                            break;
                        }
                    }
                    if(isSame){
                        FunctionModification mod;
                        mod.removal = TS::All;
                        mod.signature = meta_function->minimalSignature();
                        m_current_class->typeEntry()->addFunctionModification(mod);
                    }
                }
            }
        }
        QStringList treatedSubscriptions;
        QStringList subscriptions = subscriptionFunctions.keys();
        for(const QString& sig : qAsConst(subscriptions)){
            if(MetaFunction *subscription = subscriptionFunctions.take(sig)){
                treatedSubscriptions << sig;
                MetaFunction * getter{nullptr};
                MetaFunction * setter{nullptr};
                bool addSetter = false;
                bool addGetter = false;
                if(subscription->isConstant()){
                    QString sig2 = sig.mid(0, sig.length()-5).trimmed();
                    getter = subscription;
                    if(!treatedSubscriptions.contains(sig2)){
                        setter = subscriptionFunctions.take(sig2);
                        if(setter)
                            treatedSubscriptions << sig2;
                        if(!setter && (!getter->type()->isConstant() && getter->type()->getReferenceType()==MetaType::Reference)){
                            //ReportHandler::warning("There is no function '"+m_current_class->qualifiedCppName()+"::"+sig2+"' in ['"+subscriptionFunctions.keys().join("', '")+"']. (Treated: ['"+treatedSubscriptions.join("', '")+"']) Making copy of "+m_current_class->qualifiedCppName()+"::"+sig);
                            setter = subscription->copy();
                            setter->setOriginalSignature(subscription->originalSignature());
                            setter->setOriginalName(subscription->originalName());
                            addSetter = true;
                        }
                    }
                }else{
                    QString sig2 = sig + "const";
                    setter = subscription;
                    if(!treatedSubscriptions.contains(sig2)){
                        getter = subscriptionFunctions.take(sig2);
                        if(getter)
                            treatedSubscriptions << sig2;
                        if(!getter){
                            //ReportHandler::warning("There is no function '"+m_current_class->qualifiedCppName()+"::"+sig2+"' in ['"+subscriptionFunctions.keys().join("', '")+"']. (Treated: ['"+treatedSubscriptions.join("', '")+"']) Making copy of "+m_current_class->qualifiedCppName()+"::"+sig);
                            getter = subscription->copy();
                            getter->setOriginalSignature(subscription->originalSignature());
                            getter->setOriginalName(subscription->originalName());
                            addGetter = true;
                        }
                    }
                }
                if(getter){
                    getter->setName("get");
                    if(getter->type()->indirections().isEmpty()){
                        if(getter->type()->typeEntry()->isPrimitive()){
                            getter->type()->setReferenceType(MetaType::NoReference);
                        }else{
                            getter->type()->setConstant(true);
                            if(getter->type()->typeEntry()==m_database->qstringType()){
                                getter->type()->setTypeEntry(m_database->findType("QString"));
                            }else if(getter->type()->typeEntry()==m_database->qvariantType()){
                                getter->type()->setTypeEntry(m_database->findType("QVariant"));
                            }else if(getter->type()->typeEntry()==m_database->qcharType()){
                                getter->type()->setTypeEntry(m_database->findType("QChar"));
                            }
                        }
                    }
                    decideUsagePattern(getter->type());
                    if(addGetter){
                        if(m_current_class->typeEntry()->designatedInterface())
                            m_current_class->extractInterface()->addFunction(getter);
                        else
                            m_current_class->addFunction(getter);
                    }
                }
                if(setter){
                    setter->setName("set");
                    MetaType* type;
                    if(getter && !addGetter){
                        type = getter->type()->copy();
                    }else{
                        type = setter->type();
                        if(type->indirections().isEmpty()){
                            if(type->typeEntry()->isPrimitive()){
                                type->setReferenceType(MetaType::NoReference);
                            }else{
                                type->setConstant(true);
                                if(type->typeEntry()==m_database->qstringType()){
                                    type->setTypeEntry(m_database->findType("QString"));
                                }else if(type->typeEntry()==m_database->qvariantType()){
                                    type->setTypeEntry(m_database->findType("QVariant"));
                                }else if(type->typeEntry()==m_database->qcharType()){
                                    type->setTypeEntry(m_database->findType("QChar"));
                                }
                            }
                        }else{
                            type->setReferenceType(MetaType::NoReference);
                        }
                    }
                    decideUsagePattern(type);
                    QString oldMinimalSignature = setter->minimalSignature();
                    setter->setType(nullptr);
                    MetaArgument* arg = new MetaArgument();
                    arg->setArgumentIndex(setter->arguments().size());
                    arg->setType(type);
                    bool hasValue = false;
                    for(const MetaArgument* other : setter->arguments()){
                        if(other->argumentName()=="value"){
                            hasValue = true;
                            break;
                        }
                    }
                    if(!hasValue)
                        arg->setName("value");
                    setter->addArgument(arg);
                    if(oldMinimalSignature!=setter->minimalSignature()){
                        for(const FunctionModification& mod : m_current_class->typeEntry()->functionModifications()){
                            if (mod.signature == oldMinimalSignature) {
                                const_cast<QString&>(mod.signature) = setter->minimalSignature();
                            }
                        }
                    }
                    if(addSetter){
                        if(m_current_class->typeEntry()->designatedInterface())
                            m_current_class->extractInterface()->addFunction(setter);
                        else
                            m_current_class->addFunction(setter);
                    }
                }
            }
        }
        if(m_current_class->typeEntry()->hasJustPrivateConstructors() || m_current_class->typeEntry()->isDestructorPrivate()){
            *m_current_class += MetaAttributes::Final;
        }
        if(m_current_class->typeEntry()->isDestructorVirtual())
            m_current_class->setHasVirtuals(true);
        m_current_class->setHasJustPrivateConstructors(m_current_class->typeEntry()->hasJustPrivateConstructors());
        m_current_class->setUnimplmentablePureVirtualFunctions(unimplementablePureVirtualFunctions);
        if(m_current_class->isInterface()){
            MetaClass *ifaceImpl = m_current_class->extractInterfaceImpl();
            if(m_current_class->typeEntry()->hasJustPrivateConstructors() || m_current_class->typeEntry()->isDestructorPrivate()){
                *ifaceImpl += MetaAttributes::Final;
            }
            if(m_current_class->typeEntry()->isDestructorVirtual())
                ifaceImpl->setHasVirtuals(true);
            ifaceImpl->setHasJustPrivateConstructors(m_current_class->typeEntry()->hasJustPrivateConstructors());
            ifaceImpl->setUnimplmentablePureVirtualFunctions(unimplementablePureVirtualFunctions);
        }else if(m_current_class->typeEntry()->designatedInterface()){
            MetaClass *iface = m_current_class->extractInterface();
            if(m_current_class->typeEntry()->hasJustPrivateConstructors() || m_current_class->typeEntry()->isDestructorPrivate()){
                *iface += MetaAttributes::Final;
            }
            if(m_current_class->typeEntry()->isDestructorVirtual())
                iface->setHasVirtuals(true);
            iface->setHasJustPrivateConstructors(m_current_class->typeEntry()->hasJustPrivateConstructors());
            iface->setUnimplmentablePureVirtualFunctions(unimplementablePureVirtualFunctions);
        }
    }
}

MetaType* MetaBuilder::exchangeTemplateTypes(const MetaType* type, bool isReturn, const QMap<QString,MetaType*>& templateTypes){
    if(type->typeEntry()->isTemplateArgument()){
        if(MetaType* ttype = templateTypes[type->typeEntry()->name()]){
            MetaType*rtype = type->copy();
            rtype->setConstant(ttype->isConstant() || type->isConstant());
            rtype->setIndirections(QList<bool>() << ttype->indirections() << type->indirections());
            //rtype->setReferenceType(ttype->getReferenceType());
            if(rtype->getReferenceType()==MetaType::NoReference && type->getReferenceType()!=MetaType::NoReference && !rtype->indirections().isEmpty())
                rtype->setReferenceType(type->getReferenceType());
            rtype->setTypeEntry(ttype->typeEntry());
            decideUsagePattern(rtype);
            if(!rtype->indirections().isEmpty() && isReturn){
                rtype->setTypeUsagePattern(MetaType::ObjectPattern);
            }
            QList<const MetaType *> instantiations;
            for(auto instantiation : rtype->instantiations()){
                instantiations << exchangeTemplateTypes(instantiation, false, templateTypes);
            }
            rtype->setInstantiations(instantiations);
            return rtype;
        }
    }else{
        QList<const MetaType *> instantiations;
        for(auto instantiation : type->instantiations()){
            instantiations << exchangeTemplateTypes(instantiation, false, templateTypes);
        }
        if(type->instantiations()!=instantiations){
            MetaType*rtype = type->copy();
            rtype->setInstantiations(instantiations);
            return rtype;
        }
    }
    return const_cast<MetaType*>(type);
}

bool MetaBuilder::setupFunctionTemplateInstantiations(MetaClass *meta_class){
    MetaFunctionList functions;
    for(MetaFunction *func : meta_class->functions()) {
        MetaTemplateParameterList templateParameters;
        for(MetaTemplateParameter* tparam : func->templateParameters()){
            if(tparam->type()->typeEntry()->isTemplateArgument()){
                templateParameters << tparam;
            }
        }
        if(!templateParameters.isEmpty() || (func->type() && func->type()->typeUsagePattern()==MetaType::AutoPattern)){
            QList<TemplateInstantiation> templateInstantiations = func->templateInstantiations(meta_class);
            if(!templateInstantiations.isEmpty()){
                for(const TemplateInstantiation& template_instantiation : qAsConst(templateInstantiations)){
                    // duplicate
                    std::unique_ptr<MetaFunction> func2;
                    func2.reset(func->copy());
                    // rename function
                    if(!template_instantiation.renamedTo().isEmpty()){
                        func2->setOriginalName(func2->name());
                        func2->setName(template_instantiation.renamedTo());
                    }

                    // find types of the template instantiations
                    MetaTemplateParameterList untreatedTemplateParameters = func2->templateParameters();
                    QMap<QString,MetaType*> templateTypes;
                    for(int k=0; k<template_instantiation.arguments.size() && k<func2->templateParameters().size(); k++){
                        MetaTemplateParameter* tparam = func2->templateParameters()[k];
                        if(tparam->type()->typeUsagePattern()==MetaType::TemplateArgumentPattern){
                            if(template_instantiation.arguments[k].name.isEmpty()){
                                TypeInfo info = analyzeTypeInfo(meta_class, template_instantiation.arguments[k].type);
                                MetaClass * tmp_current_class = m_current_class;
                                m_current_class = meta_class;
                                bool ok = false;
                                MetaType* ttype = translateType(info, &ok, QString("traverseTemplateInstantiation <%1>").arg(template_instantiation.arguments[k].type), true, true, false);
                                m_current_class = tmp_current_class;
                                if(ok && ttype){
                                    decideUsagePattern(ttype);
                                    templateTypes[tparam->name()] = ttype;
                                    tparam->setName("");
                                    tparam->setType(ttype);
                                    tparam->setImplicit(template_instantiation.arguments[k].implicit);
                                    tparam->setInstantiation(template_instantiation.arguments[k].type);
                                    tparam->setDefaultType({});
                                    untreatedTemplateParameters.removeOne(tparam);
                                }
                            }else if(template_instantiation.arguments[k].name==tparam->name()){
                                untreatedTemplateParameters.removeOne(tparam);
                            }else if(!tparam->defaultType().isEmpty()){
                                untreatedTemplateParameters.removeOne(tparam);
                            }
                        }else{
                            untreatedTemplateParameters.removeOne(tparam);
                        }
                    }

                    // exchange template types
                    if(func2->type()){
                        if(func2->type()->typeUsagePattern()==MetaType::AutoPattern){
                            if(template_instantiation.arguments.size() > func2->templateParameters().size()){
                                QString arg = template_instantiation.arguments[func2->templateParameters().size()].type;
                                TypeInfo info = analyzeTypeInfo(meta_class, arg);
                                MetaClass * tmp_current_class = m_current_class;
                                m_current_class = meta_class;
                                bool ok = false;
                                MetaType* ttype = translateType(info, &ok, QString("traverseTemplateInstantiation <%1>").arg(arg), true, true, false);
                                m_current_class = tmp_current_class;
                                if(ok && ttype){
                                    decideUsagePattern(ttype);
                                    func2->setType(ttype);
                                }
                            }
                        }else{
                            MetaType* rtype = exchangeTemplateTypes(func2->type(), true, templateTypes);
                            if(rtype!=func2->type())
                                func2->setType(rtype);
                        }
                    }

                    for(MetaArgument* arg : func2->arguments()){
                        MetaType* rtype = exchangeTemplateTypes(arg->type(), false, templateTypes);
                        if(rtype!=arg->type())
                            arg->setType(rtype);
                    }

                    if(!untreatedTemplateParameters.isEmpty()){
                        QStringList templ;
                        for(MetaTemplateParameter* tparam : untreatedTemplateParameters){
                            templ << tparam->name();
                        }
                        ReportHandler::warning(QString("template method %1::%2 has uninstantiated parameters <%3>").arg(func->implementingClass()->qualifiedCppName(), func->minimalSignature(), templ.join(", ")));
                    }else if(func2->type() && func2->type()->typeUsagePattern()==MetaType::AutoPattern){
                        ReportHandler::warning(QString("method %1::%2 returns auto").arg(func->implementingClass()->qualifiedCppName(), func->minimalSignature()));
                    }else{
                        func2->setFunctionTemplate(func);
                        func2->setDeclaringClass(meta_class);
                        func2->setOwnerClass(meta_class);
                        func2->setImplementingClass(meta_class);
                        //if(!meta_class->hasFunction(func2.get()))
                        {
                            if(template_instantiation.isCodeInjection()
                                    || !template_instantiation.ppCondition.isEmpty()
                                    || !template_instantiation.throws.isEmpty()
                                    || !template_instantiation.association.isEmpty()
                                    || !template_instantiation.snips.isEmpty()
                                    || !template_instantiation.targetType.isEmpty()
                                    || !template_instantiation.argument_mods.isEmpty()){
                                FunctionModification mod;
                                AbstractFunctionModification& amod = mod;
                                amod = template_instantiation;
                                mod.template_instantiations.clear();
                                mod.signature = func2->minimalSignature();
                                mod.removal = TS::NoLanguage;
                                meta_class->typeEntry()->addFunctionModification(mod);
                            }
                            functions << func2.release();
                        }
                    }
                }
            }else if(!func->hasTemplateTypes() && !(func->type() && func->type()->typeUsagePattern()==MetaType::AutoPattern)){
                std::unique_ptr<MetaFunction> func2;
                func2.reset(func->copy());
                func2->setFunctionTemplate(func);
                func2->setDeclaringClass(meta_class);
                func2->setOwnerClass(meta_class);
                func2->setImplementingClass(meta_class);
                QMap<QString,MetaType*> templateTypes;
                for(int k=0; k<func2->templateParameters().size(); k++){
                    MetaTemplateParameter* tparam = func2->templateParameters()[k];
                    if(tparam->type()->typeUsagePattern()==MetaType::TemplateArgumentPattern){
                        TypeInfo info = analyzeTypeInfo(meta_class, tparam->defaultType());
                        bool ok = false;
                        MetaClass * tmp_current_class = m_current_class;
                        m_current_class = meta_class;
                        MetaType* ttype = translateType(info, &ok, QString("traverseTemplateInstantiation <%1>").arg(tparam->defaultType()), true, true, false);
                        m_current_class = tmp_current_class;
                        tparam->setDefaultType({});
                        if(ok && ttype){
                            templateTypes[tparam->name()] = ttype;
                            tparam->setName("");
                            tparam->setType(ttype);
                            tparam->setImplicit(true);
                            tparam->setInstantiation(tparam->defaultType());
                        }
                    }
                }
                if(func2->type()){
                    MetaType* rtype = exchangeTemplateTypes(func2->type(), true, templateTypes);
                    if(rtype!=func2->type())
                        func2->setType(rtype);
                }
                for(MetaArgument* arg : func2->arguments()){
                    MetaType* rtype = exchangeTemplateTypes(arg->type(), false, templateTypes);
                    if(rtype!=arg->type())
                        arg->setType(rtype);
                }
                functions << func2.release();
            }else if(func->hasTemplateArgumentTypes()){
                if(func->isRemovedFromAllLanguages(meta_class))
                    functions << func;
                else if(!func->isPrivate()){
                    if(!func->implementingClass() || (func->implementingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCode)){
                        if(func->functionType()==MetaFunction::GlobalScopeFunction)
                            ReportHandler::warning(QString("Missing instantiations for template method %1").arg(func->minimalSignature()));
                        else
                            ReportHandler::warning(QString("Missing instantiations for template method %1::%2").arg(func->implementingClass()->qualifiedCppName(), func->minimalSignature()));
                    }
                }
            }else{
                if(func->isRemovedFromAllLanguages(meta_class))
                    functions << func;
            }
        }else{
            functions << func;
        }
    }
    meta_class->setFunctions(functions);
    return true;
}


void MetaBuilder::fixFunctions(MetaClass * cls) {
    if (m_functions_fixed.contains(cls))
        return;
    else
        m_functions_fixed.insert(cls);

    setupFunctionTemplateInstantiations(cls);
    if(!cls->isInterface()){
        for(MetaClass* interface : cls->interfaces()){
            if(cls->typeEntry()->designatedInterface() && cls->extractInterface()==interface){
                for(MetaFunction *function : interface->functions()) {
                    MetaFunction *cpy = function->copy();
                    cpy->setDeclaringClass(cls);
                    cpy->setImplementingClass(cls);
                    if(cpy->isConstructor()){
                        cpy->setName(InterfaceTypeEntry::implName({}).mid(2));
                    }
                    cls->addFunction(cpy);
                }
                for(const MetaField *field : interface->fields()) {
                    if (!field->wasPrivate()) {
                        MetaField *new_field = field->copy();
                        new_field->setEnclosingClass(cls);
                        cls->addField(new_field);
                        cls->typeEntry()->setHasNonPublicFields();
                    }
                }
            }else{
                for(MetaFunction *function : interface->functions()) {
                    if (!function->isConstructor() && !cls->hasFunction(function)) {
                        MetaFunction *cpy = function->copy();

                        // Setup that this function is an interface class.
                        cpy->setInterfaceClass(interface);
                        *cpy += MetaAttributes::InterfaceFunction;

                        // Copy the modifications in interface into the implementing classes.
                        FunctionModificationList mods = function->modifications(interface);
                        for(const FunctionModification &mod : mods) {
                            cls->typeEntry()->addFunctionModification(mod);
                        }

                        // It should be mostly safe to assume that when we implement an interface
                        // we don't "pass on" pure virtual functions to our sublcasses...
            //                 *cpy -= AbstractMetaAttributes::Abstract;

                        cpy->setImplementingClass(cls);
                        cls->addFunction(cpy);
                    }
                }
                for(const MetaField *field : interface->fields()) {
                    if (field->isPublic()) {
                        MetaField *new_field = field->copy();
                        new_field->setEnclosingClass(cls);
                        cls->addField(new_field);
                    }
                }
            }
        }
    }

    MetaClass *interfaceImplClass{nullptr};
    MetaClass *super_class = cls->baseClass();
    MetaFunctionList funcs = cls->functions();
    MetaFunctionList deletedFuncs = cls->deletedFunctions();

//     printf("fix functions for %s\n", qPrintable(name()));

    if (super_class)
        fixFunctions(super_class);
    int iface_idx = 0;

    while (super_class || iface_idx < cls->interfaces().size()) {
//         printf(" - base: %s\n", qPrintable(super_class->name()));

        // Since we always traverse the complete hierarchy we are only
        // interrested in what each super class implements, not what
        // we may have propagated from their base classes again.
        MetaFunctionList super_funcs;
        if (super_class) {
            bool pullDown = super_class->isPullProtectedMethodsDown();
            // Super classes can never be final
            if (super_class->isFinalInTargetLang()) {
                ReportHandler::warning("Final class '" + super_class->name() + "' set to non-final, as it is extended by other classes");
                *super_class -= MetaAttributes::FinalInTargetLang;
                *super_class += MetaAttributes::PullProtectedMethodsDown;
                pullDown = true;
            }
            if(pullDown){
                // when a class cannot be subclassed from external but has subclasses inside Qt,
                // the protected functions of the class must be pulled down to the subclasses.
                super_funcs = super_class->queryFunctions(MetaClass::Protected | MetaClass::NormalFunctions);
                for (int sfi = 0; sfi < super_funcs.size(); ++sfi) {
                    MetaFunction *sf = super_funcs.at(sfi);
                    if(!sf->isAbstract() && !sf->isConstructor() && !sf->isEmptyFunction() && !sf->isInvalid()){
                        MetaFunction *sfcopy = sf->copy();
                        sfcopy->setDeclaringClass(cls);
                        cls->addFunction(sfcopy);
                    }
                }
            }
            super_funcs = super_class->queryFunctions(MetaClass::ClassImplements);
        } else {
            interfaceImplClass = cls->interfaces().at(iface_idx)->extractInterfaceImpl();
            super_funcs = cls->interfaces().at(iface_idx)->queryFunctions(MetaClass::NormalFunctions);
        }

        QHash<MetaFunction *, FunctionModificationList> funcs_to_add;
        for (int sfi = 0; sfi < super_funcs.size(); ++sfi) {
            MetaFunction *sf = super_funcs.at(sfi);

            if (sf->isRemovedFromAllLanguages(sf->implementingClass()))
                continue;

            // we generally don't care about private functions, but we have to get the ones that are
            // virtual in case they override abstract functions.
            bool add = (sf->isNormal() || sf->isSignal() || sf->isEmptyFunction());
            for (int fi = 0; fi < funcs.size(); ++fi) {
                MetaFunction *f = funcs.at(fi);
                if (f->isRemovedFromAllLanguages(f->implementingClass()))
                    continue;

                const uint cmp = f->compareTo(sf);

                if (cmp & MetaFunction::EqualModifiedName) {
//                     printf("   - %s::%s similar to %s::%s %x vs %x\n",
//                            qPrintable(sf->implementingClass()->typeEntry()->qualifiedCppName()),
//                            qPrintable(sf->name()),
//                            qPrintable(f->implementingClass()->typeEntry()->qualifiedCppName()),
//                            qPrintable(f->name()),
//                            sf->attributes(),
//                            f->attributes());

                    add = sf->isSignal();
                    if (cmp & MetaFunction::EqualArguments) {

//                         if (!(cmp & AbstractMetaFunction::EqualReturnType)) {
//                             ReportHandler::warning(QString("%1::%2 and %3::%4 differ in retur type")
//                                                    .arg(sf->implementingClass()->name())
//                                                    .arg(sf->name())
//                                                    .arg(f->implementingClass()->name())
//                                                    .arg(f->name()));
//                         }

                        // Same function, propegate virtual...
                        if (!(cmp & MetaFunction::EqualAttributes)) {
                            if (!f->isEmptyFunction()) {
                                if (!sf->isFinalInCpp() && f->isFinalInCpp() && !f->isDeclaredFinalInCpp()) {
                                    *f -= MetaAttributes::FinalInCpp;
                                    //                                 printf("   --- inherit virtual\n");
                                }
                                if (!sf->isFinalInTargetLang() && f->isFinalInTargetLang() && !f->isDeclaredFinalInCpp()) {
                                    *f -= MetaAttributes::FinalInTargetLang;
                                    if(!sf->implementingClass()->isInterface() || sf->wasPublic()){
                                        if(!sf->wasPrivate())
                                            *f += MetaAttributes::Override;
                                        else if(sf->isDeprecated() && !f->isDeprecated()){
                                            *f += MetaAttributes::Deprecated;
                                            f->setDeprecatedComment(sf->deprecatedComment());
                                        }
                                    }
                                    //                                 printf("   --- inherit virtual\n");
                                }
                                if (!f->isFinalInTargetLang() && f->isPrivate() && !sf->isPrivate()) {
                                    f->setFunctionType(MetaFunction::EmptyFunction);
                                    f->setVisibility(MetaAttributes::Protected);
                                    *f += MetaAttributes::FinalInTargetLang;
                                    ReportHandler::warning(QString("private virtual function '%1' in '%2'")
                                                           .arg(f->signature(), f->implementingClass()->name()));
                                }
                            }
                        }

                        if (f->visibility() != sf->visibility()) {
                            QString warn = QString("visibility of function '%1' modified in class '%2'")
                                           .arg(f->name(), cls->name());
                            ReportHandler::warning(warn);

                            // If new visibility is private, we can't
                            // do anything. If it isn't, then we
                            // prefer the parent class's visibility
                            // setting for the function.
                            if (!f->isPrivate() && !sf->isPrivate())
                                f->setVisibility(sf->visibility());

                            // Private overrides of abstract functions have to go into the class or
                            // the subclasses will not compile as non-abstract classes.
                            // But they don't need to be implemented, since they can never be called.
                            if (f->isPrivate() && sf->isAbstract()) {
                                f->setFunctionType(MetaFunction::EmptyFunction);
                                f->setVisibility(sf->visibility());
                                *f += MetaAttributes::FinalInTargetLang;
                                *f += MetaAttributes::FinalInCpp;
                            }
                        }

                        // Set the class which first declares this function, afawk
                        f->setDeclaringClass(sf->declaringClass());

                        if (sf->isFinalInTargetLang() && !sf->isPrivate() && !f->isPrivate() && !sf->isStatic() && !f->isStatic() && !f->isSignal()) {
                            // Shadowed funcion, need to make base class
                            // function non-virtual
                            if (f->implementingClass() != sf->implementingClass() && f->implementingClass()->inheritsFrom(sf->implementingClass())) {

                                // Check whether the superclass method has been redefined to non-final

                                bool hasNonFinalModifier = false;
                                bool isBaseImplPrivate = false;
                                FunctionModificationList mods = sf->modifications(sf->implementingClass());
                                for(const FunctionModification& mod : mods) {
                                    if (mod.isNonFinal()) {
                                        hasNonFinalModifier = true;
                                        break;
                                    } else if (mod.isPrivate()) {
                                        isBaseImplPrivate = true;
                                        break;
                                    }
                                }

                                QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments;
                                if (!hasNonFinalModifier
                                        && !isBaseImplPrivate
                                        && ( addedArguments = f->addedArguments() ).first.isEmpty()
                                        && addedArguments.second.isEmpty()
                                        && (f->implementingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)
                                        && (sf->implementingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)) {
                                    ReportHandler::warning(QStringLiteral("Shadowing: %1::%2 and %3::%4; Java code will not compile")
                                                           .arg(sf->implementingClass()->qualifiedCppName(),
                                                                sf->signature(),
                                                                f->implementingClass()->qualifiedCppName(),
                                                                f->signature()));
                                }
                            }
                        }

                    }

                    if (!f->isPrivate() && cmp & MetaFunction::EqualDefaultValueOverload) {
                        const MetaArgumentList& arguments = f->arguments().size() < sf->arguments().size() ? sf->arguments() : f->arguments();

                        for (int i = 0; i < arguments.size(); ++i)
                            arguments[i]->setDefaultValueExpression(QString());
                    }


                    // Otherwise we have function shadowing and we can
                    // skip the thing...
                } else if (cmp & MetaFunction::EqualName && !sf->isSignal()) {

                    // In the case of function shadowing where the function name has been altered to
                    // avoid conflict, we don't copy in the original.
                    add = false;
                }

                // this feature is necessary because
                // Q_DECL_FINAL (meaning the shell must not override the method)
                // is not yet recognized by generator.
                // Example: Qt3D::QColorMask::copy(const Qt3D::QNode *) Q_DECL_FINAL

                FunctionModificationList mods = f->modifications(cls);
                for(const FunctionModification& mod : mods) {
                    if (mod.isDeclaredFinal()) {
                        *f += MetaAttributes::FinalInTargetLang;
                        *f += MetaAttributes::FinalInCpp;
//                        ReportHandler::warning(QString("final overriding method %1 in class %2").arg(f->signature()).arg(f->implementingClass()->qualifiedCppName()));
                        break;
                    }
                }
            }
            if(!sf->isFinalInCpp()){
                for (int fi = 0; fi < deletedFuncs.size(); ++fi) {
                    MetaFunction *f = deletedFuncs.at(fi);
                    if (f->isRemovedFromAllLanguages(f->implementingClass()))
                        continue;
                    //ReportHandler::warning(QString("deleted method %1 in class %2").arg(f->signature()).arg(f->implementingClass()->qualifiedCppName()));
                    const uint cmp = f->compareTo(sf);
                    if ((cmp & MetaFunction::EqualModifiedName)
                        && (cmp & MetaFunction::EqualArguments)) {
                        f = f->copy();
                        f->setFunctionType(MetaFunction::EmptyFunction);
                        f->setVisibility(sf->visibility());
                        *f += MetaAttributes::FinalInTargetLang;
                        *f += MetaAttributes::FinalInCpp;
                        *f += MetaAttributes::Override;
                        funcs_to_add[f] = interfaceImplClass ? sf->modifications(interfaceImplClass) : FunctionModificationList{};
                    }
                }
            }

            if (add)
                funcs_to_add[sf] = interfaceImplClass ? sf->modifications(interfaceImplClass) : FunctionModificationList{};
        }

        for(QHash<MetaFunction *, FunctionModificationList>::const_iterator i = funcs_to_add.cbegin(); i!=funcs_to_add.cend(); i++){
            funcs << i.key()->copy();
            if(cls->isInterface())
                cls->extractInterfaceImpl()->typeEntry()->addFunctionModifications(i.value());
            else
                cls->typeEntry()->addFunctionModifications(i.value());
        }

        if (super_class)
            super_class = super_class->baseClass();
        else
            iface_idx++;
    }

    for(MetaFunction *func : funcs) {
        FunctionModificationList mods = func->modifications(cls);
        for(const FunctionModification &mod : mods) {
            if (mod.isRenameModifier()) {
                func->setName(mod.renamedTo());
            }
        }

        // Make sure class is abstract if one of the functions is
        if (func->isAbstract()) {
            (*cls) += MetaAttributes::Abstract;
            (*cls) -= MetaAttributes::Final;
        }
    }

    /*if (cls->typeEntry()->hasJustPrivateConstructors()) {
        (*cls) += MetaAttributes::Abstract;
        (*cls) -= MetaAttributes::Final;
    }*/

    for(MetaFunction *f1 : funcs) {
        for(MetaFunction *f2 : funcs) {
            if (f1 != f2) {
                uint cmp = f1->compareTo(f2);
                if ((cmp & MetaFunction::EqualName)
                        && !f1->isFinalInCpp()
                        && f2->isFinalInCpp()) {
                    *f2 += MetaAttributes::FinalOverload;
                }
            }
        }
    }

    cls->setFunctions(funcs);
    for(MetaFunction *func : cls->functions()) {
        // Make sure that we include files for all classes that are in use
        if (!func->isRemovedFrom(cls, TS::ShellCode)){
            if(func->type()){
                addInclude(cls->typeEntry(), func->type());
            }
            for(MetaArgument* arg : func->arguments()){
                addInclude(cls->typeEntry(), arg->type());
            }
        }
    }

    if (!cls->typeEntry()) {
        ReportHandler::warning(QString("class '%1' does not have an entry in the type system").arg(cls->name()));
    }

    if (cls->isAbstract() && !cls->isInterface()) {
        cls->typeEntry()->setLookupName(cls->typeEntry()->targetLangName() + "$ConcreteWrapper");
    }
    cls->fixUnimplmentablePureVirtualFunctions();
    if(!cls->typeEntry()->extendType().isEmpty()){
        if(MetaClass *extendedClass = m_meta_classes.findClass(cls->typeEntry()->extendType())){
            for(MetaFunction* function : cls->functions()){
                function = function->copy();
                if(function->isConstructor()){
                    function->setName(extendedClass->simpleName());
                }
                function->setImplementingClass(extendedClass);
                function->setDeclaringClass(extendedClass);
                function->setOwnerClass(extendedClass);
                extendedClass->addFunction(function);
                //add_extra_includes_for_function(extendedClass, function);
            }
            for(const Include& incl : cls->typeEntry()->extraIncludes()){
                extendedClass->typeEntry()->addExtraInclude(incl);
            }
        }else{
            ReportHandler::warning(QString("Unable to extend class '%1'.").arg(cls->typeEntry()->extendType()));
        }
    }
}

void MetaBuilder::setupInheritance(MetaClass *meta_class, QList<PendingHiddenBaseType>& pendingHiddenBaseTypes, QList<MetaClass *>& pendingConstructorUsages) {
    Q_ASSERT(!meta_class->isInterface());

    if (m_setup_inheritance_done.contains(meta_class))
        return;
    m_setup_inheritance_done.insert(meta_class);

    if(meta_class->templateBaseClass() && meta_class->qualifiedCppName().endsWith(">")){
        setupInheritance(const_cast<MetaClass *>(meta_class->templateBaseClass()), pendingHiddenBaseTypes, pendingConstructorUsages);
        TypeInfo info = analyzeTypeInfo(meta_class, meta_class->qualifiedCppName());
        pendingHiddenBaseTypes.append(PendingHiddenBaseType{meta_class, meta_class->templateBaseClass(), info});
        return;
    }

    QList<TypeInfo> publicBaseClasses;
    QList<TypeInfo> protectedBaseClasses;

    QStringList scope = meta_class->typeEntry()->qualifiedCppName().split("::");
    scope.removeLast();

    if(!scope.isEmpty()){
        for (QPair<TypeInfo,bool> pair : meta_class->baseClassTypeInfo()) {
            for (auto l = scope.size(); l >= 0; --l) {
                QStringList prefix = l > 0 ? QStringList(scope.mid(0, l)) : QStringList();
                TypeInfo baseClass = pair.first;
                baseClass.setQualifiedName(prefix << baseClass.qualifiedName());
                if(m_database->findType(baseClass.toString())){
                    pair.first = baseClass;
                    break;
                }
            }
            if(pair.second){
                publicBaseClasses << pair.first;
            }else{
                protectedBaseClasses << pair.first;
            }
        }
    }else{
        for(const QPair<TypeInfo,bool>& pair : meta_class->baseClassTypeInfo()){
            if(pair.second){
                publicBaseClasses << pair.first;
            }else{
                protectedBaseClasses << pair.first;
            }
        }
    }

    if(publicBaseClasses.isEmpty() && meta_class->typeAliasType()){
        const MetaType* typeAliasType = meta_class->typeAliasType();
        if(typeAliasType->typeEntry() && typeAliasType->typeEntry()->isAlias()){
            if(MetaClass *cls = m_meta_classes.findClass(typeAliasType->typeEntry()->qualifiedCppName())){
                setupInheritance(cls, pendingHiddenBaseTypes, pendingConstructorUsages);
                if(cls->typeAliasType()){
                    MetaType* newTypeAliasType = cls->typeAliasType()->copy();
                    if(newTypeAliasType->indirections().isEmpty()
                            && newTypeAliasType->getReferenceType()==MetaType::NoReference
                            && !newTypeAliasType->typeEntry()->isPrimitive()){
                        newTypeAliasType->setReferenceType(typeAliasType->getReferenceType());
                    }
                    newTypeAliasType->setIndirections( QList<bool>() << newTypeAliasType->indirections() << typeAliasType->indirections());
                    if((!newTypeAliasType->typeEntry()->isPrimitive() || !newTypeAliasType->indirections().isEmpty()) && typeAliasType->isConstant())
                        newTypeAliasType->setConstant(true);
                    decideUsagePattern(newTypeAliasType);
                    delete typeAliasType;
                    Q_ASSERT(newTypeAliasType->typeEntry());
                    meta_class->setTypeAliasType(newTypeAliasType);
                }
            }
        }
    }

    // we only support our own containers and ONLY if there is only one baseclass
    if (publicBaseClasses.size() == 1 && publicBaseClasses.first().toString().count('<') == 1) {
        QStringList base = publicBaseClasses.first().qualifiedName();
        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6, 0, 0) && base==QStringList{"QVector"}){
            base = QStringList{"QList"};
        }
        //for (auto i = scope.size(); i >= 0; --i)
        {
            QString base_name = base.join("::");

            MetaClass *templ = nullptr;
            for(MetaClass *c : m_templates) {
                if (c->typeEntry()->name() == base_name) {
                    templ = c;
                    break;
                }
            }

            if (!templ)
                templ = m_meta_classes.findClass(base_name);

            if (templ) {
                setupInheritance(templ, pendingHiddenBaseTypes, pendingConstructorUsages);
                pendingHiddenBaseTypes.append(PendingHiddenBaseType{meta_class, templ, publicBaseClasses.first()});
                return;
            }
        }

        ReportHandler::warning(QString("template baseclass '%1' of '%2' is not known")
                               .arg(publicBaseClasses.first().toString(), meta_class->name()));
        return;
    }

    int primary = -1;
    int primaries = 0;
    for (int i = 0; i < publicBaseClasses.size(); ++i) {
        QString base_class_name = publicBaseClasses[i].toString();
        if (m_database->isClassRejected(strip_template_args(base_class_name)))
            continue;

        QString base_name = publicBaseClasses[i].qualifiedName().join("::");
        TypeEntry *base_class_entry = m_database->findType(base_name);
        if (!base_class_entry) {
            ReportHandler::warning(QString("class '%1' inherits from unknown base class '%2'")
                                   .arg(meta_class->name(), base_class_name));
        }

        // true for primary base class
        else if (!base_class_entry->designatedInterface() && !base_class_entry->isInterface()) {
            if (primaries > 0) {
                if(meta_class->typeEntry()->delegatedBaseClasses().contains(base_class_name)){
                    QString delegate = meta_class->typeEntry()->delegatedBaseClasses()[base_class_name];
                    if(delegate.isEmpty()){
                        if(base_name.startsWith("Q")){
                            delegate = QString("to%1").arg(base_name.mid(1));
                        }else{
                            delegate = QString("to%1").arg(base_name);
                        }
                    }
                    bool ok = false;
                    MetaType* type = translateType(publicBaseClasses[i], &ok, QString("setupInheritance of %1").arg(meta_class->name()));
                    if(!ok || !type){
                        ReportHandler::warning(QString("class '%1' inherits from unknown base class '%2'")
                                               .arg(meta_class->name(), publicBaseClasses[i].toString()));
                    }else{
                        type->setIndirections({false});
                        decideUsagePattern(type);
                    }
                    MetaFunction * delegateFunction = new MetaFunction();
                    delegateFunction->setType(type);
                    delegateFunction->setName(delegate);
                    delegateFunction->setOriginalName(delegate);
                    delegateFunction->setFunctionType(MetaFunction::BaseClassDelegateFunction);
                    delegateFunction->setOriginalAttributes(MetaAttributes::Final | MetaAttributes::Protected);
                    *delegateFunction += MetaAttributes::Final;
                    *delegateFunction += MetaAttributes::Public;
                    *delegateFunction += MetaAttributes::Native;
                    delegateFunction->setDeclaringClass(meta_class);
                    delegateFunction->setImplementingClass(meta_class);
                    meta_class->addFunction(delegateFunction);
                }else{
                    ReportHandler::warning(QString("class '%1' has multiple primary public base classes"
                                                   " '%2' and '%3'")
                                           .arg(meta_class->name(),
                                                publicBaseClasses.at(primary).toString(),
                                                base_class_name));
                }
            }else{
                primaries++;
                primary = i;
            }
        }
    }
    for (int i = 0; i < protectedBaseClasses.size(); ++i) {
        QString base_class_name = protectedBaseClasses[i].toString();
        if (m_database->isClassRejected(strip_template_args(base_class_name)))
            continue;

        QString base_name = protectedBaseClasses[i].qualifiedName().join("::");
        TypeEntry *base_class_entry = m_database->findType(base_name);
        if (!base_class_entry) {
            ReportHandler::warning(QString("class '%1' inherits from unknown base class '%2'")
                                   .arg(meta_class->name(), base_class_name));
        }else {
            QString delegate;
            if(base_class_name.startsWith("Q")){
                delegate = QString("to%1").arg(base_class_name.mid(1));
            }else{
                delegate = QString("to%1").arg(base_class_name);
            }
            if(!meta_class->typeEntry()->delegatedBaseClasses()[base_class_name].isEmpty()){
                delegate = meta_class->typeEntry()->delegatedBaseClasses()[base_class_name];
            }
            bool ok = false;
            MetaType* type = translateType(protectedBaseClasses[i], &ok, QString("Inheritance %1 of %2").arg(base_class_name, meta_class->name()));
            if(ok){
                type->setIndirections({false});
                decideUsagePattern(type);
                MetaFunction * delegateFunction = new MetaFunction();
                delegateFunction->setType(type);
                delegateFunction->setName(delegate);
                delegateFunction->setOriginalName(delegate);
                delegateFunction->setFunctionType(MetaFunction::BaseClassDelegateFunction);
                delegateFunction->setOriginalAttributes(MetaAttributes::Final | MetaAttributes::Protected);
                *delegateFunction += MetaAttributes::Final;
                *delegateFunction += MetaAttributes::Protected;
                *delegateFunction += MetaAttributes::Native;
                delegateFunction->setDeclaringClass(meta_class);
                delegateFunction->setImplementingClass(meta_class);
                meta_class->addFunction(delegateFunction);
            }
        }
    }

    if (primary >= 0) {
        MetaClass *base_class = m_meta_classes.findClass(publicBaseClasses.at(primary).toString());
        if (!base_class) {
            if(!meta_class->isTypeAlias()){
                ReportHandler::warning(QString("unknown baseclass for '%1': '%2'")
                                       .arg(meta_class->name(), publicBaseClasses.at(primary).toString()));
            }
            return;
        }
        if(base_class->isInterface()){
            setupInheritance(const_cast<MetaClass *>(base_class)->extractInterfaceImpl(), pendingHiddenBaseTypes, pendingConstructorUsages);
            if(!meta_class->interfaces().contains(base_class))
                meta_class->addInterface(base_class);
        }else{
            setupInheritance(base_class, pendingHiddenBaseTypes, pendingConstructorUsages);
            meta_class->setBaseClass(base_class);
            pendingConstructorUsages.append(base_class);
            if(meta_class->typeEntry()->designatedInterface()
                    && const_cast<MetaClass *>(meta_class)->extractInterface()
                    && base_class->typeEntry()->designatedInterface()
                    && const_cast<MetaClass *>(base_class)->extractInterface()){
                if(!meta_class->extractInterface()->interfaces().contains(base_class->extractInterface()))
                    meta_class->extractInterface()->addInterface(base_class->extractInterface());
            }
        }

        if (meta_class->typeEntry()->designatedInterface() && base_class && !base_class->isInterface()) {
            ReportHandler::warning(QString("Interface type '%2' extending object type '%1'. This is not supported and the generated Java code will not compile.")
                                   .arg(base_class->name(), meta_class->typeEntry()->designatedInterface()->name()));
        }

    }

    for (int i = 0; i < publicBaseClasses.size(); ++i) {
        if (m_database->isClassRejected(publicBaseClasses.at(i).toString()))
            continue;

        if (i != primary) {
            if(MetaClass *base_class = m_meta_classes.findClass(publicBaseClasses.at(i).toString(), MetaClassList::QualifiedCppName)){
                if(base_class->isInterface()){
                    setupInheritance(const_cast<MetaClass *>(base_class)->extractInterfaceImpl(), pendingHiddenBaseTypes, pendingConstructorUsages);
                    if(!meta_class->interfaces().contains(base_class)){
                        meta_class->addInterface(base_class);
                    }
                    QString impl_name = InterfaceTypeEntry::implName(base_class->fullName()).replace("::", "$");
                    MetaClass *ifaceImpl = m_meta_classes.findClass(impl_name, MetaClassList::FullName);
                    if (!ifaceImpl) {
                        ReportHandler::warning(QString("unknown implementation for interface '%1': '%2'")
                                               .arg(meta_class->name(), impl_name));
                        continue;
                    }
                    for(MetaClass *iface : ifaceImpl->interfaces()){
                        if(iface->isInterface()){
                            if(!meta_class->interfaces().contains(iface))
                                meta_class->addInterface(iface);
                        }else{
                            ReportHandler::warning(QString("inherited interface '%1' for class '%2' is not an interface type: '%3'")
                                                   .arg(base_class->fullName(),
                                                        meta_class->name(),
                                                        iface->fullName()));
                        }
                    }
                }else{
                    setupInheritance(base_class, pendingHiddenBaseTypes, pendingConstructorUsages);
                    if(meta_class->typeEntry()->designatedInterface()
                            && const_cast<MetaClass *>(meta_class)->extractInterface()
                            && base_class->typeEntry()->designatedInterface()
                            && const_cast<MetaClass *>(base_class)->extractInterface()){
                        if(!meta_class->extractInterface()->interfaces().contains(base_class->extractInterface())){
                            meta_class->extractInterface()->addInterface(base_class->extractInterface());
                        }
                    }
                }
            }
        }
    }
}

MetaClass * MetaBuilder::instantiateIterator(IteratorTypeEntry *iteratorTypeEntry, MetaClass *targetClass, const QList<const MetaType *>& template_types, const QHash<const TypeEntry *,const MetaType *>& template_types_by_name){
    Q_UNUSED(template_types_by_name)
    MetaClass * instantiationClass = nullptr;
    MetaClass *iteratorClass = nullptr;
    for(MetaClass *c : m_template_iterators) {
        if (c->typeEntry() == iteratorTypeEntry) {
            iteratorClass = c;
            break;
        }
    }
    if(iteratorClass){
        {
            instantiationClass = m_meta_classes.findClass(targetClass->qualifiedCppName()+"::"+iteratorTypeEntry->name());
            if(!instantiationClass){
                instantiationClass = m_templates.findClass(targetClass->qualifiedCppName()+"::"+iteratorTypeEntry->name());
            }
        }
        if(!instantiationClass){
            for(const Include& inc : iteratorTypeEntry->extraIncludes()){
                targetClass->typeEntry()->addExtraInclude(inc);
            }
            IteratorTypeEntry * newIteratorTypeEntry = iteratorTypeEntry->clone(targetClass->typeEntry(), targetClass->baseClassTypeInfo().at(0).first.toString());
            m_database->addType(newIteratorTypeEntry);
            instantiationClass = new MetaClass();
            instantiationClass->setTypeEntry(newIteratorTypeEntry);
            instantiationClass->setAttributes(iteratorClass->attributes());
            instantiationClass->setOriginalAttributes(iteratorClass->originalAttributes());
            instantiationClass->setHasEqualsOperator(iteratorClass->hasEqualsOperator());
            instantiationClass->setHasJustPrivateConstructors(iteratorClass->hasJustPrivateConstructors());
            instantiationClass->setUnimplmentablePureVirtualFunctions(iteratorClass->unimplmentablePureVirtualFunctions());
            instantiationClass->setToStringCapability(iteratorClass->toStringCapability());
            instantiationClass->setHasVirtuals(iteratorClass->hasVirtuals());
            instantiationClass->setHasVirtualSlots(iteratorClass->hasVirtualSlots());
            instantiationClass->setHas_Q_GADGET(iteratorClass->has_Q_GADGET());
            instantiationClass->setHas_Q_OBJECT(iteratorClass->has_Q_OBJECT());
            instantiationClass->setTemplateBaseClass(iteratorClass);
            instantiationClass->setTemplateBaseClassInstantiations(template_types);
            instantiationClass->setEnclosingClass(targetClass);
            targetClass->addEnclosedClass(instantiationClass);
            addClass(instantiationClass);
        }
    }
    return instantiationClass;
}

void MetaBuilder::traverseEnums(ScopeModelItem scope_item, MetaClass *meta_class, const QSet<QString> &metaEnums, const QMap<QString,QString>& flagByEnums) {
    EnumList enums = scope_item->enums();
    for(const EnumModelItem& enum_item : enums) {
        MetaEnum *meta_enum = traverseEnum(enum_item, meta_class, metaEnums, flagByEnums);
        if (meta_enum) {
            meta_enum->setOriginalAttributes(meta_enum->attributes());
            if(!meta_enum->typeEntry()->javaScope().isEmpty()){
                m_scopeChangedEnums << meta_enum;
            }
            meta_class->addEnum(meta_enum);
            meta_enum->setEnclosingClass(meta_class);
        }
    }
}

MetaFunction *MetaBuilder::traverseFunction(FunctionModelItem function_item, const QString& _function_name, const QStringList& tparams) {
    if(function_item->referenceType()==TypeInfo::RReference)
        return nullptr;
    if(function_item->isTemplate() && !function_item->templateInstantiations().isEmpty()){
        //ReportHandler::warning(
        //printf("Excluding template function '%s' from %s\n", qPrintable(m_current_class ? QString("%1::%2").arg(m_current_class->name(), _function_name) : _function_name), qPrintable(function_item->fileName()));
        return nullptr;
    }
    const ArgumentList& arguments = function_item->arguments();
    QString function_name = function_item->name();

    QString class_name;
    if(m_current_class)
        class_name = m_current_class->typeEntry()->qualifiedCppName();

    QString signature = _function_name.split("->")[0].trimmed();
    QString _originalSignature = QString::fromUtf8(QMetaObject::normalizedSignature(qPrintable(signature)));
    bool remove = m_database->isFunctionRejected(class_name, function_name);
    if (!remove && !m_current_class) {
        const QList<TypeSystemTypeEntry*> typeSystems = m_database->typeSystems();
        for(const TypeSystemTypeEntry* ts : typeSystems){
            FunctionModificationList list = ts->functionModifications(signature);
            if(_originalSignature!=signature)
                list << ts->functionModifications(_originalSignature);
            for(const FunctionModification& mod : qAsConst(list)){
                if(mod.isRemoveModifier()){
                    remove = true;
                    break;
                }
            }
            if(remove)
                break;
        }
    }

    if (remove) {
        if(function_name==QLatin1String("static_assert"))
            return nullptr;
        if(m_current_class){
            if(function_item->accessPolicy() != CodeModel::Private && m_current_class->typeEntry()->codeGeneration()==TypeEntry::GenerateAll){
                if(function_item->functionType() == CodeModel::Signal
                        || function_item->functionType() == CodeModel::PrivateSignal){
                    m_rejected_signals.insert({class_name + "::" + _function_name, function_item->fileName()}, GenerationDisabled);
                }else if(!function_item->templateParameters().isEmpty()){
                    m_rejected_template_functions.insert({class_name + "::" + _function_name, function_item->fileName()}, GenerationDisabled);
                }else{
                    m_rejected_functions.insert({class_name + "::" + _function_name, function_item->fileName()}, GenerationDisabled);
                }
            }
        }else{
            if(!function_item->templateParameters().isEmpty()){
                m_rejected_template_functions.insert({_function_name, function_item->fileName()}, GenerationDisabled);
            }else{
                m_rejected_functions.insert({_function_name, function_item->fileName()}, GenerationDisabled);
            }
        }
        return nullptr;
    }

    Q_ASSERT(function_item->functionType() == CodeModel::Normal
             || function_item->functionType() == CodeModel::Signal
             || function_item->functionType() == CodeModel::PrivateSignal
             || function_item->functionType() == CodeModel::Slot);

    if (function_item->isFriend() && !function_item->hasBody()/* && !isOperator*/)
        return nullptr;

    std::unique_ptr<MetaFunction> meta_function(new MetaFunction());
    meta_function->setConstant(function_item->isConstant());
    meta_function->setTemplate(function_item->isTemplate());
    meta_function->setFunctionReferenceType(MetaType::ReferenceType(function_item->referenceType()));

    ReportHandler::debugMedium(QString(" - %2()").arg(function_name));
    meta_function->setName(function_name);
    meta_function->setOriginalName(function_item->name());
    meta_function->setVariadics(function_item->isVariadics());

    if (function_item->isAbstract())
        *meta_function += MetaAttributes::Abstract;
    *meta_function += MetaAttributes::Native;

    if (!function_item->isVirtual())
        *meta_function += MetaAttributes::Final;

    if(function_item->isDeclFinal()){
        *meta_function += MetaAttributes::Final;
        *meta_function += MetaAttributes::DeclaredFinalInCpp;
    }

    if(function_item->isDeprecated()){
        *meta_function += MetaAttributes::Deprecated;
        meta_function->setDeprecatedComment(function_item->getDeprecatedComment());
    }

    if (function_item->isInvokable())
        *meta_function += MetaAttributes::Invokable;

    if (function_item->isStatic()) {
        *meta_function += MetaAttributes::Static;
        *meta_function += MetaAttributes::Final;
    }

    // Access rights
    if (function_item->accessPolicy() == CodeModel::Public)
        *meta_function += MetaAttributes::Public;
    else if (function_item->accessPolicy() == CodeModel::Private)
        *meta_function += MetaAttributes::Private;
    else
        *meta_function += MetaAttributes::Protected;
    QMap<QString,MetaTemplateParameter*> metaTemplateParameters;
    int templateParameterCounter=0;
    for(const TemplateParameterModelItem& p : function_item->templateParameters()){
        if(!p->ownerClass()){
            TemplateArgumentEntry* tae = new TemplateArgumentEntry(p->name());
            tae->setVariadic(p->isVaradic());
            tae->setOrdinal(templateParameterCounter);
            MetaType *type = new MetaType();
            type->setTypeUsagePattern(MetaType::TemplateArgumentPattern);
            type->setTypeEntry(tae);
            templateParameterCounter++;
            MetaTemplateParameter* tp = new MetaTemplateParameter();
            tp->setName(p->name());
            tp->setType(type);
            tp->setDefaultType(p->defaultValue());
            metaTemplateParameters[p->name()] = tp;
            meta_function->addTemplateParameter(tp);
        }
    }
    m_current_function = meta_function.get();
    auto onfinish = qScopeGuard([=]{this->m_current_function = nullptr;});

    QString stripped_class_name = class_name;
    auto cc_pos = stripped_class_name.lastIndexOf("::");
    if (cc_pos > 0)
        stripped_class_name = stripped_class_name.mid(cc_pos + 2);

    TypeInfo function_type = function_item->type();
    if (m_current_class && !m_current_class->isNamespace() && function_name.startsWith('~')) {
        meta_function->setFunctionType(MetaFunction::DestructorFunction);
        meta_function->setInvalid(true);
    } else if (m_current_class && !m_current_class->isNamespace() && strip_template_args(function_name) == stripped_class_name) {
        meta_function->setFunctionType(MetaFunction::ConstructorFunction);
        meta_function->setDeclExplicit(function_item->isExplicit());
        meta_function->setName(m_current_class->simpleName());
    } else {
        bool ok = false;
        MetaType *type = nullptr;

        if(metaTemplateParameters.contains(function_type.qualifiedName().join("::"))){
            type = metaTemplateParameters[function_type.qualifiedName().join("::")]->type()->copy();
            type->setIndirections(function_type.indirections());
            type->setConstant(function_type.isConstant());
            type->setReferenceType(MetaType::ReferenceType(function_type.getReferenceType()));
            ok = true;
        }

        if(!ok){
            type = translateType(function_type, &ok, QString("traverseFunction %1.%2").arg(class_name, function_name));
            if(type && m_current_class){
                if(type->typeEntry()->isQVariant() && m_current_class->typeEntry()==m_database->qvariantType()){
                    type->setTypeEntry(m_database->qvariantType());
                }else if(type->typeEntry()->isQString() && m_current_class->typeEntry()==m_database->qstringType()){
                    type->setTypeEntry(m_database->qstringType());
                }else if(type->typeEntry()->isQChar() && m_current_class->typeEntry()==m_database->qcharType()){
                    type->setTypeEntry(m_database->qcharType());
                }
            }
        }

        if(!ok){
            FunctionModificationList list;
            if(m_current_class){
                list = m_current_class->typeEntry()->functionModifications(signature);
                if(_originalSignature!=signature)
                    list << m_current_class->typeEntry()->functionModifications(_originalSignature);
            }else{
                const QList<TypeSystemTypeEntry*> typeSystems = m_database->typeSystems();
                for(const TypeSystemTypeEntry* ts : typeSystems){
                    if(!ts->functionModifications().isEmpty()){
                        list << ts->functionModifications(signature);
                        if(_originalSignature!=signature)
                            list << ts->functionModifications(_originalSignature);
                    }
                }
            }
            for(const FunctionModification& mod : qAsConst(list)){
                for(const ArgumentModification& amod : mod.argument_mods){
                    if(amod.index==0 && !amod.modified_type.isEmpty()){
                        TypeInfo _type = function_type;
                        _type.setVolatile(false);
                        _type.setConstant(false);
                        _type.setReferenceType(TypeInfo::NoReference);
                        UnknownTypeEntry* tae = new UnknownTypeEntry(_type.toString());
                        type = new MetaType();
                        if(function_type.isConstant())
                            type->setConstant(true);
                        if(function_type.isVolatile())
                            type->setVolatile(true);
                        type->setReferenceType(MetaType::ReferenceType(function_type.getReferenceType()));
                        type->setTypeUsagePattern(MetaType::InvalidPattern);
                        type->setOriginalTypeDescription(function_type.toString());
                        type->setTypeEntry(tae);
                        ok = true;
                        break;
                    }
                }
                if(ok)
                    break;
            }
        }

        if (!ok) {
            if(function_item->accessPolicy() != CodeModel::Private
                    && m_current_class && m_current_class->typeEntry()->codeGeneration()==TypeEntry::GenerateAll
                    && !m_database->isClassRejected(function_item->type().qualifiedName().join("::"))){
                if(function_item->functionType() == CodeModel::Signal
                        || function_item->functionType() == CodeModel::PrivateSignal){
                    ReportHandler::warning(QString("skipping signal '%1::%2', unmatched return type '%3'")
                                           .arg(class_name, function_item->name(), function_item->type().toString()));
                    m_rejected_signals[{class_name + "::" + _function_name, function_item->fileName()}] = UnmatchedReturnType;
                }else if(function_item->isTemplate()){
                    ReportHandler::warning(QString("skipping function '%1::%2<%3>', unmatched return type '%4'")
                                           .arg(class_name, function_item->name(), tparams.join(", "), function_item->type().toString()));
                    m_rejected_template_functions.insert({class_name + "::" + _function_name, function_item->fileName()}, UnmatchedReturnType);
                }else{
                    ReportHandler::warning(QString("skipping function '%1::%2', unmatched return type '%3'")
                                           .arg(class_name, function_item->name(), function_item->type().toString()));
                    m_rejected_functions[{class_name + "::" + _function_name, function_item->fileName()}] = UnmatchedReturnType;
                }
            }
            meta_function->setInvalid(true);
            return meta_function.release();
        }
        meta_function->setType(type);

        if (function_item->functionType() == CodeModel::Signal)
            meta_function->setFunctionType(MetaFunction::SignalFunction);
        else if (function_item->functionType() == CodeModel::PrivateSignal)
            meta_function->setFunctionType(MetaFunction::PrivateSignalFunction);
        else if (function_item->functionType() == CodeModel::Slot)
            meta_function->setFunctionType(MetaFunction::SlotFunction);
    }

    QHash<MetaArgument *,ArgumentModelItem> argumentModelItems;
    MetaArgumentList meta_arguments;

    QString originalSignature = function_item->name();
    originalSignature += "(";

    FunctionModification mod;
    if(meta_function->functionType()!=MetaFunction::DestructorFunction
            && function_item->accessPolicy() != CodeModel::Private
            && ((m_current_class && m_database->isPixmapType(m_current_class->typeEntry()))
                || (meta_function->type() && m_database->isPixmapType(meta_function->type()->typeEntry())))){
        mod.modifiers = Modification::PixmapThreadAffine;
    }

    int first_default_argument = 0;
    for (int i = 0; i < arguments.size(); ++i) {
        ArgumentModelItem arg = arguments.at(i);
        bool ok = false;
        MetaType *argumentType = nullptr;
        if(arg->type().getReferenceType()==TypeInfo::NoReference
                && arg->type().indirections().isEmpty()
                && arguments.size()==1
                && arg->type().toString()=="void"){
            //originalSignature += "void";
            continue;
        }
        if(i>0)
            originalSignature += ",";
        originalSignature += arg->type().toString();

        if(metaTemplateParameters.contains(arg->type().qualifiedName().join("::"))){
            argumentType = metaTemplateParameters[arg->type().qualifiedName().join("::")]->type()->copy();
            argumentType->setIndirections(arg->type().indirections());
            argumentType->setConstant(arg->type().isConstant());
            argumentType->setReferenceType(MetaType::ReferenceType(arg->type().getReferenceType()));
            ok = true;
        }

        if(!ok){
            argumentType = translateType(arg->type(), &ok, QString("traverseFunction %1.%2 arg#%3").arg(class_name, function_name).arg(i));
            if(argumentType && m_current_class){
                if(argumentType->typeEntry()->isQVariant() && m_current_class->typeEntry()==m_database->qvariantType()){
                    argumentType->setTypeEntry(m_database->qvariantType());
                }else if(argumentType->typeEntry()->isQString() && m_current_class->typeEntry()==m_database->qstringType()){
                    argumentType->setTypeEntry(m_database->qstringType());
                }else if(argumentType->typeEntry()->isQChar() && m_current_class->typeEntry()==m_database->qcharType()){
                    argumentType->setTypeEntry(m_database->qcharType());
                }
            }
            if (argumentType && ok && argumentType->typeEntry()->isEnum() && !argumentType->typeEntry()->qualifiedCppName().contains("::")) {
                bool _ok = false;
                TypeInfo typeInfo = arg->type();
                if(m_current_class)
                    typeInfo.setQualifiedName(QStringList() << m_current_class->typeEntry()->qualifiedCppName().split("::") << typeInfo.qualifiedName());
                else
                    typeInfo.setQualifiedName(QStringList() << typeInfo.qualifiedName());
                MetaType *_meta_type = translateType(typeInfo, &_ok, QString("traverseFunction %1.%2 arg#%3").arg(class_name, function_name).arg(i));
                if (_meta_type && _ok){
                    argumentType = _meta_type;
                }
            }
        }
        if(!ok){
            FunctionModificationList list;
            if(m_current_class){
                list = m_current_class->typeEntry()->functionModifications(signature);
                if(_originalSignature!=signature)
                    list << m_current_class->typeEntry()->functionModifications(_originalSignature);
            }else{
                const QList<TypeSystemTypeEntry*> typeSystems = m_database->typeSystems();
                for(const TypeSystemTypeEntry* ts : typeSystems){
                    if(!ts->functionModifications().isEmpty()){
                        list << ts->functionModifications(signature);
                        if(_originalSignature!=signature)
                            list << ts->functionModifications(_originalSignature);
                    }
                }
            }
            for(const FunctionModification& mod : qAsConst(list)){
                for(const ArgumentModification& amod : mod.argument_mods){
                    if(amod.index==i+1 && (!amod.modified_type.isEmpty() || amod.removed)){
                        TypeInfo _type = arg->type();
                        _type.setVolatile(false);
                        _type.setConstant(false);
                        _type.setReferenceType(TypeInfo::NoReference);
                        UnknownTypeEntry* tae = new UnknownTypeEntry(_type.toString());
                        argumentType = new MetaType();
                        if(arg->type().isConstant())
                            argumentType->setConstant(true);
                        if(arg->type().isVolatile())
                            argumentType->setVolatile(true);
                        argumentType->setReferenceType(MetaType::ReferenceType(arg->type().getReferenceType()));
                        argumentType->setOriginalTypeDescription(arg->type().toString());
                        argumentType->setTypeUsagePattern(MetaType::InvalidPattern);
                        argumentType->setTypeEntry(tae);
                        ok = true;
                        break;
                    }
                }
                if(ok)
                    break;
            }
        }
        if (!argumentType || !ok) {
            if(arg->type().isVolatile())
                argumentType = translateType(arg->type(), &ok, QString("traverseFunction %1.%2 arg#%3").arg(class_name, function_name).arg(i));
            if(function_item->accessPolicy() != CodeModel::Private
                    && m_current_class
                    && m_current_class->typeEntry()->codeGeneration()==TypeEntry::GenerateAll
                    && !m_database->isClassRejected(arg->type().qualifiedName().join("::"))
                    && arg->type().qualifiedName().join("::")!=m_current_class->typeEntry()->qualifiedCppName()+"Private"){
                if(function_item->functionType() == CodeModel::Signal
                        || function_item->functionType() == CodeModel::PrivateSignal){
                    ReportHandler::warning(QString("skipping signal '%1::%2', "
                                                   "unmatched parameter type '%3'")
                                           .arg(class_name, function_item->name(), arg->type().toString()));
                    m_rejected_signals[{class_name + "::" + _function_name, function_item->fileName()}] = UnmatchedArgumentType;
                }else if(function_item->isTemplate()){
                    ReportHandler::warning(QString("skipping function '%1::%2<%3>', "
                                                   "unmatched parameter type '%4'")
                                           .arg(class_name, function_item->name(), tparams.join(", "), arg->type().toString()));
                    m_rejected_template_functions.insert({class_name + "::" + _function_name, function_item->fileName()}, UnmatchedArgumentType);
                }else{
                    ReportHandler::warning(QString("skipping function '%1::%2', "
                                                   "unmatched parameter type '%3'")
                                           .arg(class_name, function_item->name(), arg->type().toString()));
                    m_rejected_functions[{class_name + "::" + _function_name, function_item->fileName()}] = UnmatchedArgumentType;
                }
            }
            meta_function->setInvalid(true);
            return meta_function.release();
        }

//        if (function_item->isFriend() && function_item->hasBody() && i==0){
//            if(!(m_current_class && argumentType->typeEntry()==m_current_class->typeEntry()
//                    && argumentType->indirections().isEmpty())){
//                return nullptr;
//            }
//        }

        ArgumentModification argumentModification(int(meta_arguments.size()+1));

        /*if(m_current_class && m_current_class->typeEntry()->isComplex() &&
                arguments.size()==1 &&
                argumentType->getReferenceType()==MetaType::Reference &&
                !argumentType->isConstant() &&
                argumentType->typeEntry()==m_current_class->typeEntry() &&
                meta_function->name()=="swap"){
            argumentType->setTypeUsagePattern(MetaType::ValuePattern);
            argumentModification.no_null_pointers = true;
        }*/
        if(m_database->isPixmapType(argumentType->typeEntry())){
            argumentModification.thread_affine = ThreadAffinity::Pixmap;
        }
        if(argumentModification.no_null_pointers || argumentModification.thread_affine!=ThreadAffinity::None){
            mod.argument_mods << argumentModification;
        }
        MetaArgument *meta_argument = new MetaArgument();
        meta_argument->setType(argumentType);
        meta_argument->setName(arg->name());
        meta_argument->setArgumentIndex(int(meta_arguments.size()));
        meta_arguments << meta_argument;
        argumentModelItems[meta_argument] = arg;
    }
    meta_function->setArguments(meta_arguments);
    originalSignature += ")";
    if(function_item->isConstant()){
        originalSignature += "const";
    }
    if(!originalSignature.contains("std::function<") && !originalSignature.contains("qxp::function_ref<") && !originalSignature.contains("(*)"))
        meta_function->setOriginalSignature(QString::fromLatin1(QMetaObject::normalizedSignature(qPrintable(originalSignature))));
    else
        meta_function->setOriginalSignature(originalSignature);
    if (function_item->isFriend() && function_item->hasBody()){
        meta_function->setFunctionType(MetaFunction::GlobalScopeFunction);
        meta_function->setOriginalAttributes(meta_function->attributes());
    }

    bool isRemoved = false;
    bool isRenamed = false;
    bool isReturnChanged = false;
    if(m_current_class){
        for(const FunctionModification& mod : m_current_class->typeEntry()->functionModifications(meta_function->minimalSignature())){
            if(mod.removal & TS::TargetLangCode){
                isRemoved = true;
            }
            if (mod.isRenameModifier())
                isRenamed = true;
            for(const ArgumentModification& argumentModification : mod.argument_mods){
                if(argumentModification.type==ArgumentModification::Default){
                    if(argumentModification.index==0){
                        isReturnChanged = true;
                    }
                    if(!argumentModification.modified_name.isEmpty() && argumentModification.index>0 && argumentModification.index<=meta_arguments.size()){
                        meta_arguments[argumentModification.index-1]->setModifiedName(argumentModification.modified_name);
                    }
                }
            }
        }
    }

    if (!isRemoved && function_item->operatorType()!=OperatorType::None) {
        meta_function->setOperatorType(function_item->operatorType());
        auto rejectFunction = [&](){
            if(function_item->functionType() == CodeModel::Signal
                    || function_item->functionType() == CodeModel::PrivateSignal){
                m_rejected_signals.insert({class_name + "::" + _function_name, function_item->fileName()}, GenerationDisabled);
            }else if(!function_item->templateParameters().isEmpty()){
                m_rejected_template_functions.insert({class_name + "::" + _function_name, function_item->fileName()}, GenerationDisabled);
            }else{
                m_rejected_functions.insert({class_name + "::" + _function_name, function_item->fileName()}, GenerationDisabled);
            }
        };
        switch(function_item->operatorType()){
        case OperatorType::Assign:
            if(!meta_function->isPrivate()
                && meta_arguments.size()==1
                && meta_arguments[0]->type()->getReferenceType()==MetaType::RReference){
                mod.removal = TS::All;
            }else{
                if(!isRenamed)
                    meta_function->setName("assign");
                if(!isReturnChanged)
                    meta_function->setType(nullptr);
            }
            break;
        case OperatorType::TypeCast:
            if(function_item->name().startsWith("operator typename")){
                rejectFunction();
                return nullptr;
            }else{
                if(meta_function->type()){
                    meta_function->setDeclExplicit(function_item->isExplicit());
                    if(!isRenamed){
                        QString targetLangName = meta_function->type()->typeEntry()->targetLangName();
                        auto idx = targetLangName.lastIndexOf('.');
                        if(idx>=0){
                            targetLangName = targetLangName.mid(idx+1);
                        }
                        idx = targetLangName.lastIndexOf('$');
                        if(idx>=0){
                            targetLangName = targetLangName.mid(idx+1);
                        }
                        if(targetLangName.startsWith("Q")){
                            targetLangName = targetLangName.mid(1);
                        }
                        if(targetLangName.size()>0){
                            targetLangName[0] = targetLangName[0].toUpper();
                            meta_function->setName("to" + targetLangName);
                        }else{
                            meta_function->setName("cast");
                        }
                    }
                }else{
                    if(m_current_class)
                        ReportHandler::warning(QString("Operator type unknown: %1::%2").arg(m_current_class->qualifiedCppName(), meta_function->originalSignature()));
                    else
                        ReportHandler::warning(QString("Operator type unknown: %1").arg(meta_function->originalSignature()));
                    rejectFunction();
                    return nullptr;
                }
            }
            break;
        case OperatorType::PlusAssign:
        case OperatorType::MinusAssign:
        case OperatorType::TimesAssign:
        case OperatorType::DivAssign:
        case OperatorType::RemAssign:
        case OperatorType::AndAssign:
        case OperatorType::OrAssign:
        case OperatorType::XorAssign:
            if(!isRenamed){
                switch(meta_function->operatorType()){
                case OperatorType::PlusAssign:
                    meta_function->setName("plusAssign"); break;
                case OperatorType::MinusAssign:
                    meta_function->setName("minusAssign"); break;
                case OperatorType::TimesAssign:
                    meta_function->setName("timesAssign"); break;
                case OperatorType::DivAssign:
                    meta_function->setName("divAssign"); break;
                case OperatorType::RemAssign:
                    meta_function->setName("remAssign"); break;
                case OperatorType::AndAssign:
                    meta_function->setName("andAssign"); break;
                case OperatorType::OrAssign:
                    meta_function->setName("orAssign"); break;
                case OperatorType::XorAssign:
                    meta_function->setName("xorAssign"); break;
                default: break;
                }
            }
            if(!isRenamed && !isReturnChanged)
                meta_function->setType(nullptr);
            break;
        case OperatorType::ShiftLeft:
            if(!isRenamed)
                meta_function->setName("operator_shift_left");
            break;
        case OperatorType::ShiftRight:
            if(!isRenamed)
                meta_function->setName("operator_shift_right");
            break;
        case OperatorType::ShiftLeftAssign:
            if(!isRenamed)
                meta_function->setName("operator_shift_left_assign");
            break;
        case OperatorType::ShiftRightAssign:
            if(!isRenamed)
                meta_function->setName("operator_shift_right_assign");
            break;
        case OperatorType::Plus:
            if(!isRenamed){
                if(m_current_class==nullptr || m_current_class->isNamespace() || meta_function->isInGlobalScope()){
                    if(meta_arguments.size()==1){
                        meta_function->setName("unaryPlus");
                    }else{
                        meta_function->setName("plus");
                    }
                }else{
                    if(meta_arguments.isEmpty()){
                        meta_function->setName("unaryPlus");
                    }else{
                        meta_function->setName("plus");
                    }
                }
            }
            break;
        case OperatorType::Minus:
            if(!isRenamed){
                if(m_current_class==nullptr || m_current_class->isNamespace() || meta_function->isInGlobalScope()){
                    if(meta_arguments.size()==1){
                        meta_function->setName("unaryMinus");
                    }else{
                        meta_function->setName("minus");
                    }
                }else{
                    if(meta_arguments.isEmpty()){
                        meta_function->setName("unaryMinus");
                    }else{
                        meta_function->setName("minus");
                    }
                }
            }
            break;
        case OperatorType::Times:
            if(!isRenamed){
                if(m_current_class==nullptr || m_current_class->isNamespace() || meta_function->isInGlobalScope()){
                    if(meta_arguments.size()==1){
                        meta_function->setName("operator_indirection");
                    }else{
                        meta_function->setName("times");
                    }
                }else{
                    if(meta_arguments.isEmpty()){
                        meta_function->setName("operator_indirection");
                    }else{
                        meta_function->setName("times");
                    }
                }
            }
            break;
        case OperatorType::Div:
            if(!isRenamed)
                meta_function->setName("div");
            break;
        case OperatorType::Rem:
            if(!isRenamed)
                meta_function->setName("rem");
            break;
        case OperatorType::And:
            if(!isRenamed){
                if(meta_arguments.isEmpty()){
                    meta_function->setName("operator_address_of");
                }else{
                    meta_function->setName("operator_bitwise_and");
                }
            }
            break;
        case OperatorType::Or:
            if(!isRenamed)
                meta_function->setName("operator_bitwise_or");
            break;
        case OperatorType::Xor:
            if(!isRenamed)
                meta_function->setName("operator_bitwise_xor");
            break;
        case OperatorType::Not:
            if(!isRenamed)
                meta_function->setName("operator_bitwise_not");
            break;
        case OperatorType::ExclusiveAnd:
            if(!isRenamed)
                meta_function->setName("operator_logical_and");
            break;
        case OperatorType::ExclusiveOr:
            if(!isRenamed)
                meta_function->setName("operator_logical_or");
            break;
        case OperatorType::Negation:
            if(!isRenamed){
                if(meta_function->type() && meta_function->type()->typeEntry()->qualifiedTargetLangName()=="boolean"){
                    meta_function->setName("not");
                }else{
                    meta_function->setName("operator_logical_not");
                }
            }
            break;
        case OperatorType::Inc:
            if(!isRenamed){
                if(meta_arguments.isEmpty()){
                    meta_function->setName("inc");
                    if(!isReturnChanged && meta_function->type())
                        meta_function->type()->setReferenceType(MetaType::NoReference);
                }else{
                    meta_function->setName("operator_postfix_increment");
                }
            }
            break;
        case OperatorType::Dec:
            if(!isRenamed){
                if(meta_arguments.isEmpty()){
                    meta_function->setName("dec");
                    if(!isReturnChanged && meta_function->type())
                        meta_function->type()->setReferenceType(MetaType::NoReference);
                }else{
                    meta_function->setName("operator_postfix_decrement");
                }
            }
            break;
        case OperatorType::Less:
            if(!isRenamed)
                meta_function->setName("operator_less");
            break;
        case OperatorType::Greater:
            if(!isRenamed)
                meta_function->setName("operator_greater");
            break;
        case OperatorType::LessOrEquals:
            if(!isRenamed)
                meta_function->setName("operator_less_or_equal");
            break;
        case OperatorType::Compare:
            if(!isRenamed)
                meta_function->setName("compareTo");
            break;
        case OperatorType::GreaterOrEquals:
            if(!isRenamed)
                meta_function->setName("operator_greater_or_equal");
            break;
        case OperatorType::NotEquals:
            if(!isRenamed)
                meta_function->setName("operator_not_equal");
            break;
        case OperatorType::Equals:
            if(!isRenamed)
                meta_function->setName("equals");
            break;
        case OperatorType::Subscript:
            if(!isRenamed){
                meta_function->setName("operator_subscript");
            }
            break;
        case OperatorType::MemberOfPointer:
            if(!isRenamed)
                meta_function->setName("operator_member_of_pointer");
            break;
        case OperatorType::PointerToMemberOfPointer:
            if(!isRenamed)
                meta_function->setName("operator_pointer_to_member_of_pointer");
            break;
        case OperatorType::Comma:
            if(!isRenamed)
                meta_function->setName("operator_comma");
            break;
        case OperatorType::FunctionCall:
            if(!isRenamed)
                meta_function->setName("invoke");
            break;
        case OperatorType::New:
            if(!isRenamed)
                meta_function->setName("operator_new");
            break;
        case OperatorType::Delete:
            if(!isRenamed)
                meta_function->setName("operator_delete");
            break;
        default:
            break;
        }
    }else if(meta_function->functionType()==MetaFunction::ConstructorFunction
             && !meta_function->isPrivate()
             && meta_arguments.size()==1
             && m_current_class && !m_current_class->isNamespace()
             && meta_arguments[0]->type()->getReferenceType()==MetaType::RReference
             && meta_arguments[0]->type()->typeEntry()==m_current_class->typeEntry()){
        mod.removal = TS::All;
    }

    if(!mod.argument_mods.isEmpty() || mod.modifiers!=0 || mod.removal!=0){
        mod.signature = meta_function->minimalSignature();
        if(m_current_class && !function_item->isDeleted())
            m_current_class->typeEntry()->addFunctionModification(mod);
    }

    // Find the correct default values
    for (int i = 0; i < meta_arguments.size(); ++i) {
        MetaArgument *meta_arg = meta_arguments[i];
        ArgumentModelItem arg = argumentModelItems[meta_arg];
        if(arg->type().toString()=="void"
                && arg->type().getReferenceType()==TypeInfo::NoReference
                && arg->type().indirections().isEmpty()
                && meta_arguments.size()==1){
            continue;
        }

        if (arg->defaultValue()) {
            QString expr = arg->defaultValueExpression();
            if (!expr.isEmpty())
                meta_arg->setOriginalDefaultValueExpression(expr);
            expr = translateDefaultValue(arg->defaultValueExpression(), meta_arg->type(), meta_function.get(), m_current_class, i);
            if (expr.isEmpty()) {
                first_default_argument = i;
            } else {
                meta_arg->setDefaultValueExpression(expr);
            }
        /*}else{
            QString inserted_default_expression = meta_function->replacedDefaultExpression(m_current_class, meta_arg->argumentIndex() + 1);
            if(!inserted_default_expression.isEmpty()){
                meta_arg->setDefaultValueExpression(inserted_default_expression);
            }*/
        }
    }

    // If we where not able to translate the default argument make it
    // reset all default arguments before this one too.
    for (int i = 0; i < first_default_argument; ++i)
        meta_arguments[i]->setDefaultValueExpression(QString());
    if (ReportHandler::debugLevel() == ReportHandler::FullDebug) {
        for(MetaArgument *arg : meta_arguments)
            ReportHandler::debugFull("   - " + arg->toString());
    }
    return meta_function.release();
}

void removeDuplicates(QList<TypeInfo>& contexts){
    QSet<QString> names;
    for (int i = 0; i < contexts.size(); ++i) {
        QString name = contexts[i].toString();
        if(names.contains(name)){
            contexts.takeAt(i);
            --i;
        }else{
            names.insert(name);
        }
    }
}

// uncomment the qDebug()s in order to inspect internals of the function
MetaType *MetaBuilder::translateType(TypeInfo typei,
        bool *ok,
        const QString &contextString,
        bool resolveType,
        bool resolveScope,
        bool prependScope) {
    QSet<QString> visitedNames;
    Q_ASSERT(ok);
    *ok = true;

    if(prependScope && typei.qualifiedName().size()>1 && typei.qualifiedName()[0].isEmpty()){
        prependScope = false;
    }
    if(prependScope && !m_database->isUniqueClassName(typei.qualifiedName().join("::"))){
        // the generator must distinguish between QTransform (QtCore) and QTransform (Qt3DCore)
        // the generator must distinguish between QCamera (QtMultimedia) and QCamera (Qt3DCore)
        QStringList scopes;
        if(m_current_class){
            scopes = m_current_class->qualifiedCppName().split("::");
        }else{
            scopes = currentScope()->qualifiedName();
        }
        while(!scopes.isEmpty()){
            TypeInfo tmp_type_info = typei;
            QStringList qualifiedName = tmp_type_info.qualifiedName();
            if(!qualifiedName.join("::").startsWith(scopes[0])){
                for(auto i=scopes.size()-1; i>=0; i--){
                    qualifiedName.insert(0, scopes[i]);
                }
                tmp_type_info.setQualifiedName(qualifiedName);

                if(!visitedNames.contains(tmp_type_info.qualifiedName().join("::"))){
                    visitedNames.insert(tmp_type_info.qualifiedName().join("::"));
                    bool _ok;
                    MetaType * result = translateType(tmp_type_info, &_ok, contextString, resolveType, resolveScope, false);
                    if(result && _ok){
                        return result;
                    }
                }
            }
            scopes.removeLast();
        }
    }
    //qDebug()<<"Start of translateType()"<<type_info.toString();

    // 1. Test the type info without resolving typedefs in case this is present in the
    //    type system

    //TypeParser::Info typeInfo = TypeParser::parse(typei.toString());

    if (typei.functionalReturnType()) {
        TypeInfo type_pure = typei;
        type_pure.setIndirections({});
        type_pure.setConstant(false);
        type_pure.setReferenceType(TypeInfo::NoReference);
        type_pure.setVariadic(false);
        type_pure.setVolatile(false);
        QString qualifiedName = type_pure.toString();
        if(m_current_class){
            MetaFunctional* functional = m_current_class->functionalByUsing()[qualifiedName];
            if(!functional){
                if(FunctionalTypeEntry * entry = m_database->findFunctionalTypeByUsing(m_current_class->qualifiedCppName(), qualifiedName)){
                    functional = findFunctional(m_current_class, entry);
                }
            }
            if(functional){
                MetaType *meta_type = new MetaType();
                meta_type->setTypeEntry(functional->typeEntry());
                meta_type->setIndirections(typei.indirections());
                meta_type->setReferenceType(MetaType::ReferenceType(typei.getReferenceType()));
                meta_type->setConstant(typei.isConstant());
                meta_type->setOriginalTypeDescription(typei.toString());
                decideUsagePattern(meta_type);
                return meta_type;
            }
        }else{
            MetaFunctional* functional = nullptr;//m_current_class->functionalByUsing()[qualifiedName];
            if(!functional){
                if(FunctionalTypeEntry * entry = m_database->findFunctionalTypeByUsing({}, qualifiedName)){
                    MetaClass* global = m_globals[entry->targetTypeSystem()];
                    functional = findFunctional(global, entry);
                }
            }
            if(functional){
                MetaType *meta_type = new MetaType();
                meta_type->setTypeEntry(functional->typeEntry());
                meta_type->setIndirections(typei.indirections());
                meta_type->setReferenceType(MetaType::ReferenceType(typei.getReferenceType()));
                meta_type->setConstant(typei.isConstant());
                meta_type->setOriginalTypeDescription(typei.toString());
                decideUsagePattern(meta_type);
                return meta_type;
            }
        }
        if(typei.isFunctionPointer()){
            *ok = false;
            return nullptr;
        }
    }

    //qDebug()<<"TypeInfo is after 1. method:"<<typeInfo.toString();

    // 2. Handle pointers specified as arrays with unspecified size

    bool array_of_unspecified_size = false;
    if (typei.arrayElements().size() > 0) {
        array_of_unspecified_size = true;
        for (int i = 0; i < typei.arrayElements().size(); ++i)
            array_of_unspecified_size = array_of_unspecified_size && typei.arrayElements().at(i).isEmpty();

        if (!array_of_unspecified_size) {
            TypeInfo newInfo;
            //newInfo.setArguments(typei.arguments());
            newInfo.setIndirections(typei.indirections());
            newInfo.setConstant(typei.isConstant());
            if(typei.isFunctionPointer())
                newInfo.setFunctionPointer(true);
            newInfo.setQualifiedName(typei.qualifiedName());
            newInfo.setReferenceType(typei.getReferenceType());
            newInfo.setVolatile(typei.isVolatile());

            MetaType *elementType = translateType(newInfo, ok, contextString, true, resolveScope, prependScope);
            if (!elementType || !ok) {
                if(prependScope)
                    ReportHandler::warning("Something has happened when trying to resolve type " + newInfo.toString());
                return nullptr;
            }

            for (auto i = typei.arrayElements().size() - 1; i >= 0; --i) {
                QString s = typei.arrayElements().at(i);
                bool ok;

                int elems = s.toInt(&ok);
                if (!ok) {
                    if(prependScope)
                        ReportHandler::warning("Something has happened when trying to read array initialization: " + s);
                    return nullptr;
                }

                MetaType *arrayType = new MetaType();
                arrayType->setArrayElementCount(elems);
                arrayType->setArrayElementType(elementType);
                arrayType->setTypeEntry(new ArrayTypeEntry(elementType->typeEntry(), int(elementType->indirections().size())));
                decideUsagePattern(arrayType);

                elementType = arrayType;
            }

            //qDebug()<<"Returning element type:"<<elementType->fullName();
            return elementType;
        }  else {
            for(int i=0; i<typei.arrayElements().size(); i++){
                typei.setIndirections(QList<bool>() << typei.indirections() << false);
            }
        }
    }

    QStringList qualifier_list = typei.qualifiedName();
    if(qualifier_list.startsWith({}))
        qualifier_list.takeFirst();
    if (qualifier_list.isEmpty()) {
        if(prependScope)
            ReportHandler::warning(QString("horribly broken type '%1'").arg(typei.toString()));
        *ok = false;
        return nullptr;
    }

    QString qualified_name = qualifier_list.join("::");
    QString name = qualifier_list.takeLast();

    // 3. Special case 'void' type
    if (name == "void" && typei.indirections().size() == 0) {
        //if(prependScope) qDebug()<<"Returning from name == \"void\" && typeInfo.indirections == 0";
        return nullptr;
    }

    // 4. Special case QFlags (include instantiation in name)
    if (qualified_name == "QFlags")
        qualified_name = typei.toString();

    // 5. Try to find the type
    const TypeEntry *type = m_database->findType(qualified_name);

    const TypeEntry* backup_type = nullptr;

    if(type && type->isComplex()
            && reinterpret_cast<const ComplexTypeEntry*>(type)->isTemplate()
            && !typei.arguments().isEmpty()){
        const ComplexTypeEntry* ctype = reinterpret_cast<const ComplexTypeEntry*>(type);
        QStringList templateArgs;
        for(const TypeInfo &i : typei.arguments()) {
            templateArgs << i.toString();
        }
        if(const ComplexTypeEntry* instantiation = ctype->instantiations()[templateArgs]){
            if(!instantiation->isGenericClass())
                typei.setArguments({});
            type = instantiation;
        }
    }
    if(type && (type->isObject() || type->isValue())
            && (!m_current_class || m_current_class->typeEntry()!=type)
            && (type->codeGeneration()==TypeEntry::GenerateNothing
                || type->codeGeneration()==TypeEntry::GenerateForSubclass)
            && typei.arguments().size()==1 && typei.arguments()[0].qualifiedName().join("::")=="void"
            && qualified_name.startsWith("Q") && !qualified_name.startsWith("Qt")){
        if(const TypeEntry *_type = m_database->findType(qualified_name = QLatin1String("QVoid")+qualified_name.mid(1))){
            backup_type = type;
            type = _type;
        }
    }

    //if (type)
        //qDebug()<<"After 5. name: " << type->name();

        // 6. No? Try looking it up as a flags type
        if (!type)
            type = m_database->findFlagsType(qualified_name);

    // 7. No? Try looking it up as a container type
    if (!type)
        type = m_database->findContainerType(name);
    if (!type)
        type = m_database->findFunctionalType(name);

    // 8. No? Try looking it up as a pointer container type
    if (!type)
        type = m_database->findPointerContainerType(name);

    // 8.1. No? Try looking it up as a initializer list type
    if (!type)
        type = m_database->findInitializerListType(name);

    // 8.2. No? Check if the current class is a template and this type is one
    //    of the parameters.
    if (!type && m_current_function) {
        for(MetaTemplateParameter* t : m_current_function->templateParameters()){
            if (t->type()->typeEntry()->name() == qualified_name){
                type = t->type()->typeEntry();
            }
        }
    }
    if (!type && m_current_class) {
        QList<TypeEntry *> template_args = m_current_class->templateArguments();
        for(TypeEntry *te : template_args) {
            if (te->name() == qualified_name){
                type = te;
                break;
            }
        }
    }

    bool isClassRejected = m_database->isClassRejected(qualified_name);
    if (!type){
        ClassModelItem containing_class = m_dom->findClass(qualifier_list.join("::"));

        // 9. Try finding the type by prefixing it with
        //     all baseclasses of the containing class
        if (!type && !isClassRejected && containing_class && resolveScope) {
            QList<TypeInfo> contexts;
            QSet<QString> visitedContexts;
            for(const QPair<TypeInfo,bool>& p : containing_class->baseClasses())
                contexts.append(p.first);
            removeDuplicates(contexts);

            // qDebug() << "9."<< contexts;

            TypeInfo info = typei;
            while (!contexts.isEmpty() && !type) {
                //type = m_database->findType(contexts.at(0) + "::" + qualified_name);

                QString base = contexts.at(0).qualifiedName().join("::");
                if(!visitedContexts.contains(base)){
                    visitedContexts.insert(base);
                    bool ok;
                    if(!name.startsWith(base)){
                        info.setQualifiedName(QStringList() << contexts.at(0).qualifiedName() << name.split("::"));
                        if(!visitedNames.contains(info.qualifiedName().join("::"))){
                            visitedNames.insert(info.qualifiedName().join("::"));
                            //   qDebug()<< "whiling in 9. type," << info.toString();
                            MetaType *t = translateType(info, &ok, contextString, true, false, prependScope);
                            if (t && ok) {
                                //     qDebug()<<"Returning ok from t != 0 && ok"<<t->fullName();
                                return t;
                            }
                        }
                    }

                    //10. Try if the type is in a base class
                    //e.g. MimeType is QWebPluginFactory::MimeType and called by QWebPluginFactory::Plugin
                    QStringList parts = contexts.at(0).qualifiedName();
                    while (parts.size() > 1) {
                        parts.removeLast();
                        if(!name.startsWith(parts.join("::"))){
                            info.setQualifiedName(QStringList() << parts << name.split("::"));
                            if(!visitedNames.contains(info.qualifiedName().join("::"))){
                                visitedNames.insert(info.qualifiedName().join("::"));
                                MetaType *t = translateType(info, &ok, contextString, true, false, prependScope);
                                if (t && ok) {
                                    //       qDebug()<<"Returning ok from 11. method"<<t->fullName();
                                    return t;
                                }
                            }
                        }
                    }

                    ClassModelItem item = m_dom->findClass(contexts.at(0).toString());
                    if (item) {
                        for(const QPair<TypeInfo,bool>& p : item->baseClasses())
                            contexts.append(p.first);
                    }else{
                        MetaClass* cls = m_meta_classes.findClass(contexts.at(0).toString());
                        if(cls){
                            for(const QPair<TypeInfo,bool>& p : cls->baseClassTypeInfo()){
                                if(p.second){
                                    contexts.insert(1,p.first);
                                    break;
                                }
                            }
                        }
                    }
                    removeDuplicates(contexts);
                }
                contexts.removeFirst();
            }
        }
    }

    // 12. Try finding the type by prefixing it with the current
    //    context and all baseclasses of the current context
    if (!type && !isClassRejected && m_current_class && resolveScope) {
        QStringList contexts;
        QSet<QString> visitedContexts;
        contexts.append(m_current_class->qualifiedCppName());
        QList<TypeInfo> superClasses;
        for(const QPair<TypeInfo,bool>& p : m_current_class->baseClassTypeInfo()){
            if(p.second){
                superClasses << p.first;
                contexts.append(p.first.qualifiedName().join("::"));
            }
        }
        while(!superClasses.isEmpty()){
            QList<TypeInfo> _superClasses;
            for(const TypeInfo& scl : qAsConst(superClasses)){
                if(MetaClass* superClass = m_meta_classes.findClass(scl.toString())){
                    for(const QPair<TypeInfo,bool>& p : superClass->baseClassTypeInfo()){
                        if(p.second){
                            _superClasses << p.first;
                            contexts.append(p.first.qualifiedName().join("::"));
                        }
                    }
                }
            }
            superClasses = _superClasses;
        }
        contexts.append(currentScope()->qualifiedName().join("::"));
        contexts.removeDuplicates();

        // qDebug() << "9."<< contexts;

        MetaClass *current_class = m_current_class;
        TypeInfo info = typei;
        bool subclasses_done = false;
        while (!contexts.isEmpty() && !type) {
            //type = m_database->findType(contexts.at(0) + "::" + qualified_name);

            bool ok;
            QString base = contexts.at(0);
            if(!visitedContexts.contains(base)){
                visitedContexts.insert(base);
                info.setQualifiedName(base.split("::") << typei.qualifiedName());
                //   qDebug()<< "whiling in 9. type," << info.toString();
                if(!visitedNames.contains(info.qualifiedName().join("::"))){
                    visitedNames.insert(info.qualifiedName().join("::"));
                    MetaType *t = translateType(info, &ok, contextString, true, false, prependScope);
                    if (t && ok) {
                        //     qDebug()<<"Returning ok from t != 0 && ok"<<t->fullName();
                        return t;
                    }
                }

                //13. Try if the type is in a base class
                //e.g. MimeType is QWebPluginFactory::MimeType and called by QWebPluginFactory::Plugin
                QStringList parts = base.split("::");
                while (parts.size() > 1) {
                    parts.removeLast();
                    if(!qualified_name.startsWith(parts.join("::"))){
                        info.setQualifiedName(QStringList() << parts << typei.qualifiedName());
                        if(!visitedNames.contains(info.qualifiedName().join("::"))){
                            visitedNames.insert(info.qualifiedName().join("::"));
                            MetaType *t = translateType(info, &ok, contextString, true, false, prependScope);
                            if (t && ok) {
                                //       qDebug()<<"Returning ok from 11. method"<<t->fullName();
                                return t;
                            }
                        }
                    }
                }

                ClassModelItem item = m_dom->findClass(contexts.at(0));
                if (item) {
                    for(const QPair<TypeInfo,bool>& p : item->baseClasses())
                        contexts += p.first.qualifiedName().join("::");
                }else if(current_class){
                    for(const QPair<TypeInfo,bool>& p : m_current_class->baseClassTypeInfo()){
                        if(p.second){
                            contexts.insert(1, p.first.qualifiedName().join("::"));
                            current_class = m_meta_classes.findClass(p.first.toString());
                            break;
                        }
                    }
                }
                contexts.removeDuplicates();
            }
            contexts.removeFirst();

            // 14. Last resort: special cased prefix of Qt namespace since the meta object implicitly inherits this, so
            //     enum types from there may be addressed without any scope resolution in properties.
            if (contexts.size() == 0 && !subclasses_done) {
                contexts << "Qt";
                subclasses_done = true;
            }
        }
    }

    if(!type && resolveType){
        TypeInfo type_info = typei;
        auto i = m_scopes.size() - 1;
        while (i >= 0) {
            typei = TypeInfo::resolveType(type_info, m_scopes.at(i--)->toItem());
            if (typei.qualifiedName().join("::") != type_info.qualifiedName().join("::"))
                break;
        }
        if (typei.qualifiedName().join("::") != type_info.qualifiedName().join("::")){
            bool ok;
            MetaType *t = translateType(typei, &ok, contextString, false, resolveScope, prependScope);
            if (t && ok) {
                return t;
            }
        }
    }

    // if error happened in type resolving
    if (!type) {
        *ok = false;
        //   qDebug()<<"Every type checking methods failed, typei:"<<typei.toString()<<", type_info:"<<type_info.toString();
        return nullptr;
    }

    //qDebug()<<"A type found:"<<type->name();

    // Used to for diagnostics later
    m_used_types << type;

    // These are only implicit and should not appear in code
    if(type->isInterface()){
        type = static_cast<const InterfaceTypeEntry*>(type)->origin();
    }
    Q_ASSERT(!type->isInterface());

    MetaType *meta_type = new MetaType();
    meta_type->setTypeEntry(type);
    meta_type->setIndirections(typei.indirections());
    meta_type->setReferenceType(MetaType::ReferenceType(typei.getReferenceType()));
    meta_type->setConstant(typei.isConstant());
    meta_type->setVolatile(typei.isVolatile());
    meta_type->setOriginalTypeDescription(typei.toString());
    decideUsagePattern(meta_type);

    if (meta_type->typeEntry()->isInitializerList()) {
        Q_ASSERT(typei.arguments().size() == 1);
        const TypeInfo& info = typei.arguments()[0];
        MetaType *targ_type = translateType(info, ok, contextString);
        if (!(*ok)) {
            delete meta_type;
            return nullptr;
        }
        meta_type->addInstantiation(targ_type);

    }else if (meta_type->typeEntry()->isPointerContainer()) {
        PointerContainerTypeEntry::Type pointer_container_type = static_cast<const PointerContainerTypeEntry *>(type)->type();
        if (    pointer_container_type == PointerContainerTypeEntry::QPointer
             || pointer_container_type == PointerContainerTypeEntry::QSharedPointer
             || pointer_container_type == PointerContainerTypeEntry::QWeakPointer
             || pointer_container_type == PointerContainerTypeEntry::QScopedPointer
             || pointer_container_type == PointerContainerTypeEntry::unique_ptr
             || pointer_container_type == PointerContainerTypeEntry::shared_ptr
             || pointer_container_type == PointerContainerTypeEntry::weak_ptr ) {
            Q_ASSERT(typei.arguments().size() >= 1);
            TypeInfo info = typei.arguments()[0];
            QList<bool> ic = info.indirections();
            if(!ic.isEmpty())ic.removeAt(0);
            info.setIndirections(ic);

            //    qDebug()<< "Foreaching in container thingy," << info.toString();
            MetaType *targ_type = translateType(info, ok, contextString);
            if (!(*ok)) {
                delete meta_type;
                return nullptr;
            }
            meta_type->addInstantiation(targ_type);
        }
    }else if (meta_type->typeEntry()->isContainer()) {
        //   qDebug()<<"The type is a container, descending...";
        ContainerTypeEntry::Type container_type = static_cast<const ContainerTypeEntry *>(type)->type();

        if (container_type == ContainerTypeEntry::StringListContainer) {
            TypeInfo info;
            info.setQualifiedName(QStringList() << "QString");
            MetaType *targ_type = translateType(info, ok, contextString);

            Q_ASSERT(*ok);
            Q_ASSERT(targ_type);

            meta_type->addInstantiation(targ_type);
            meta_type->setInstantiationInCpp(false);

        }else if (container_type == ContainerTypeEntry::ByteArrayListContainer) {
                TypeInfo info;
                info.setQualifiedName(QStringList() << "QByteArray");
                MetaType *targ_type = translateType(info, ok, contextString);

                Q_ASSERT(*ok);
                Q_ASSERT(targ_type);

                meta_type->addInstantiation(targ_type);
                meta_type->setInstantiationInCpp(false);

        } else {
            for(TypeInfo info : typei.arguments()) {
                if (container_type == ContainerTypeEntry::QQmlListPropertyContainer) {
                    QList<bool> ic = info.indirections();
                    ic << false;
                    info.setIndirections(ic);
                }

                //    qDebug()<< "Foreaching in container thingy," << info.toString();
                MetaType *targ_type = translateType(info, ok, contextString);
                if (!(*ok)) {
                    delete meta_type;
                    return nullptr;
                }
                if(!targ_type && info.toString()=="void"){
                    const TypeEntry *type = m_database->findType("void");
                    if(type){
                        targ_type = new MetaType();
                        targ_type->setTypeUsagePattern(MetaType::PrimitivePattern);
                        targ_type->setTypeEntry(type);
                    }
                }
                if (container_type == ContainerTypeEntry::QQmlListPropertyContainer) {
                    QList<bool> ic = targ_type->indirections();
                    if(!ic.isEmpty())ic.removeAt(0);
                    targ_type->setIndirections(ic);
                }
                meta_type->addInstantiation(targ_type);
            }
        }

    }else if (meta_type->typeEntry()->isComplex() && reinterpret_cast<const ComplexTypeEntry *>(type)->isGenericClass()) {
        for(TypeInfo info : typei.arguments()) {
            //    qDebug()<< "Foreaching in container thingy," << info.toString();
            bool _ok = false;
            MetaType *targ_type = translateType(info, &_ok, contextString);
            if (!_ok) {
                if(m_current_class && m_current_class->qualifiedCppName().startsWith("QVoid")){
                    if(const TypeEntry *_type = m_database->findType(qualified_name = QLatin1String("QVoid")+qualified_name.mid(1))){
                        meta_type->setTypeEntry(_type);
                        meta_type->setInstantiations({});
                        break;
                    }
                }
                if(backup_type){
                    meta_type->setTypeEntry(backup_type);
                    meta_type->setInstantiations({});
                    break;
                }else{
                    delete meta_type;
                    return nullptr;
                }
            }
            if(!targ_type && info.toString()=="void"){
                const TypeEntry *type = m_database->findType("void");
                if(type){
                    targ_type = new MetaType();
                    targ_type->setTypeUsagePattern(MetaType::PrimitivePattern);
                    targ_type->setTypeEntry(type);
                }
            }
            if(targ_type)
                meta_type->addInstantiation(targ_type);
        }
    }else if (meta_type->typeEntry()->isIterator()) {
        const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(meta_type->typeEntry());
        bool found = false;
        if(MetaClass * iteratorClass = classes().findClass(iteratorType->qualifiedCppName(), MetaClassList::QualifiedCppName)){
            if(iteratorClass->typeAliasType()){
                Q_ASSERT(iteratorClass->typeAliasType()->typeEntry());
                if(iteratorClass->typeAliasType()->typeEntry()->isAlias()){
                    m_missing_iterators << MissingIterator(iteratorType, meta_type, m_current_class);
                    found = true;
                }else{
                    MetaType* typeAliasType = iteratorClass->typeAliasType()->copy();
                    QList<bool> indirections = typeAliasType->indirections();
                    if(!indirections.isEmpty()){
                        indirections.takeFirst();
                        typeAliasType->setIndirections(indirections);
                    }
                    typeAliasType->setReferenceType(MetaType::Reference);
                    decideUsagePattern(typeAliasType);
                    meta_type->addIteratorInstantiation(typeAliasType);
                    found = true;
                }
            }
        }
        if(!found && m_current_class){
            if(MetaClass * iteratorClass = m_current_class->findIterator(iteratorType)){
                meta_type->setIteratorInstantiations(iteratorClass->templateBaseClassInstantiations());
                found = true;
            }
        }
        if(!found){
            m_missing_iterators << MissingIterator(iteratorType, meta_type, m_current_class);
        }
    }
    return meta_type;
}

void MetaBuilder::fixMissingIterator(){
    QList<MissingIterator> missing_iterators;
    for(const MissingIterator& missingIterator : m_missing_iterators){
        bool found = false;
        if(MetaClass * iteratorClass = classes().findClass(missingIterator.iteratorType->qualifiedCppName(), MetaClassList::QualifiedCppName)){
            if(iteratorClass->typeAliasType()){
                MetaType* typeAliasType = nullptr;
                if(iteratorClass->typeAliasType()->typeEntry()->isAlias()){
                    if(MetaClass *cls = m_meta_classes.findClass(iteratorClass->typeEntry()->qualifiedCppName())){
                        if(cls->typeAliasType()){
                            typeAliasType = cls->typeAliasType()->copy();
                            if(typeAliasType->indirections().isEmpty() && typeAliasType->getReferenceType()==MetaType::NoReference){
                                typeAliasType->setReferenceType(iteratorClass->typeAliasType()->getReferenceType());
                            }
                            typeAliasType->setIndirections( QList<bool>() << typeAliasType->indirections() << typeAliasType->indirections());
                            if(iteratorClass->typeAliasType()->isConstant())
                                typeAliasType->setConstant(true);
                        }
                    }
                }
                if(!typeAliasType){
                    typeAliasType = iteratorClass->typeAliasType()->copy();
                    QList<bool> indirections = typeAliasType->indirections();
                    if(!indirections.isEmpty()){
                        indirections.takeFirst();
                        typeAliasType->setIndirections(indirections);
                    }
                    typeAliasType->setReferenceType(MetaType::Reference);
                }
                MetaBuilder::decideUsagePattern(typeAliasType);
                missingIterator.meta_type->addIteratorInstantiation(typeAliasType);
                found = true;
            }
        }
        if(!found && missingIterator.current_class){
            if(MetaClass * iteratorClass = missingIterator.current_class->findIterator(missingIterator.iteratorType)){
                missingIterator.meta_type->setIteratorInstantiations(iteratorClass->templateBaseClassInstantiations());
                found = true;
            }
            if(missingIterator.current_class->templateBaseClass()){
                if(missingIterator.iteratorType->containerType()==missingIterator.current_class->templateBaseClass()->typeEntry()){
                    missingIterator.meta_type->setIteratorInstantiations(missingIterator.current_class->templateBaseClassInstantiations());
                    QStringList baseClassNames;
                    for(const QPair<TypeInfo,bool>& p : missingIterator.current_class->baseClassTypeInfo())
                        baseClassNames << p.first.toString();
                    IteratorTypeEntry* newIteratorType = missingIterator.iteratorType->clone(missingIterator.iteratorType->containerType(), baseClassNames.join("::"));
                    missingIterator.meta_type->setTypeEntry(newIteratorType);
                    found = true;
                }
            }
        }
        if(!found)
            missing_iterators << missingIterator;
    }
    m_missing_iterators = missing_iterators;
}

void MetaBuilder::decideUsagePattern(MetaType *meta_type) {
    const TypeEntry *type = meta_type->typeEntry();
    if (type->isPrimitive()) {
        if(meta_type->actualIndirections() == 0
                || (meta_type->isConstant() && meta_type->getReferenceType()==MetaType::Reference && meta_type->indirections().size() == 0)
                || (meta_type->getReferenceType()==MetaType::RReference && meta_type->indirections().size() == 0)){
            if(type->qualifiedCppName()=="std::nullptr_t")
                meta_type->setTypeUsagePattern(MetaType::NullptrPattern);
            else
                meta_type->setTypeUsagePattern(MetaType::PrimitivePattern);
        }else if(meta_type->actualIndirections() == 1
                 && type->qualifiedCppName().startsWith("_j")){
            meta_type->setTypeUsagePattern(MetaType::PrimitivePattern);
        }else if(meta_type->isConstant()
                 && meta_type->indirections().size() == 1
                 && meta_type->typeEntry()->qualifiedCppName()=="char"){
            meta_type->setTypeUsagePattern(MetaType::CharStringPattern);
        }else{
            meta_type->setTypeUsagePattern(MetaType::NativePointerPattern);
        }
    } else if (type->isAuto()) {
        meta_type->setTypeUsagePattern(MetaType::AutoPattern);
    } else if (type->isVoid()) {
        meta_type->setTypeUsagePattern(MetaType::NativePointerPattern);

    } else if (type->isFunctional()) {
        meta_type->setTypeUsagePattern(MetaType::FunctionalPattern);

    } else if (type->isQString()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QStringPattern);
    } else if (type->isQString()
               && (meta_type->indirections().size() == 1
                   || (meta_type->getReferenceType()==MetaType::Reference && !meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::RReference
                )) {
        if(!meta_type->isConstant())
            meta_type->setTypeEntry(TypeDatabase::instance()->qstringType());
        meta_type->setTypeUsagePattern(MetaType::QStringPattern);
    } else if (type->isQLatin1String()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QLatin1StringPattern);

    } else if (type->isQLatin1StringView()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QLatin1StringViewPattern);

    } else if (type->isQStringView()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QStringViewPattern);

    } else if (type->isQUtf8StringView()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QUtf8StringViewPattern);

    } else if (type->isQAnyStringView()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QAnyStringViewPattern);

    } else if (type->isQStringRef()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QStringRefPattern);

    } else if (type->isQChar()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QCharPattern);

    } else if ((
                   type->isJObjectWrapper()
                   || type->isJMapWrapper()
                   || type->isJCollectionWrapper()
                   || type->isJEnumWrapper()
                   || type->isJIteratorWrapper()
                   || type->isJQFlagsWrapper()
                )
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::JObjectWrapperPattern);
    } else if (type->isQVariant()
               && meta_type->indirections().size() == 0
               && ((meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::QVariantPattern);

    } else if (type->isQVariant()
               && (meta_type->indirections().size() == 1
                   || (meta_type->getReferenceType()==MetaType::Reference && !meta_type->isConstant())
                   || meta_type->getReferenceType()==MetaType::RReference
                )) {
        if(!meta_type->isConstant())
            meta_type->setTypeEntry(TypeDatabase::instance()->qvariantType());
        meta_type->setTypeUsagePattern(MetaType::QVariantPattern);

    } else if (type->isEnum() && (meta_type->actualIndirections() == 0
                    || (meta_type->isConstant() && meta_type->getReferenceType()==MetaType::Reference && meta_type->indirections().size() == 0))) {
        meta_type->setTypeUsagePattern(MetaType::EnumPattern);

    } else if (type->isObject()
               && meta_type->indirections().size() == 0
               && (meta_type->getReferenceType()==MetaType::Reference || meta_type->getReferenceType()==MetaType::RReference)) {
        meta_type->setTypeUsagePattern(MetaType::ObjectPattern);

    } else if (type->isObject()
               && meta_type->indirections().size() == 1) {
        meta_type->setTypeUsagePattern(MetaType::ObjectPattern);

        // const-references to pointers can be passed as pointers
        if (meta_type->getReferenceType()==MetaType::Reference && meta_type->isConstant()) {
            meta_type->setReferenceType(MetaType::NoReference);
            meta_type->setConstant(false);
        }

    } else if (type->isContainer() && meta_type->indirections().size() <= 1) {
        meta_type->setTypeUsagePattern(MetaType::ContainerPattern);

    } else if (type->isPointerContainer() && meta_type->indirections().size() == 0) {
        meta_type->setTypeUsagePattern(MetaType::PointerContainerPattern);

    } else if (type->isInitializerList() && meta_type->indirections().size() == 0) {
        meta_type->setTypeUsagePattern(MetaType::InitializerListPattern);

    } else if (type->isUnknown()) {
        meta_type->setTypeUsagePattern(MetaType::InvalidPattern);

    } else if (type->isTemplateArgument()) {
        meta_type->setTypeUsagePattern(MetaType::TemplateArgumentPattern);

    } else if (type->isFlags() && (meta_type->actualIndirections() == 0
                    || (meta_type->isConstant() && meta_type->getReferenceType()==MetaType::Reference && meta_type->indirections().size() == 0))) {
        meta_type->setTypeUsagePattern(MetaType::FlagsPattern);

    } else if (type->isArray()) {
        meta_type->setTypeUsagePattern(MetaType::ArrayPattern);

    } else if (type->isIterator()) {
        meta_type->setTypeUsagePattern(MetaType::IteratorPattern);

    } else if (type->isQMetaObjectType()) {
        meta_type->setTypeUsagePattern(MetaType::ObjectPattern);

    } else if (type->isQMetaObjectConnectionType()) {
        meta_type->setTypeUsagePattern(MetaType::ValuePattern);
    } else if (type->isValue()
               && meta_type->indirections().size() == 0
               && (meta_type->isConstant() == (meta_type->getReferenceType()==MetaType::Reference)
                   || meta_type->getReferenceType()==MetaType::NoReference)) {
        meta_type->setTypeUsagePattern(MetaType::ValuePattern);

    } else if (type->isValue()
               && meta_type->indirections().size() == 1) {
        meta_type->setTypeUsagePattern(MetaType::ObjectPattern);

    } else if (type->isValue()
               && meta_type->indirections().size() == 0
               && !meta_type->isConstant()
               && meta_type->getReferenceType()!=MetaType::NoReference) {
        meta_type->setTypeUsagePattern(MetaType::ObjectPattern);

    } else if (type->isObject() && meta_type->actualIndirections() == 0) {
        meta_type->setTypeUsagePattern(MetaType::ValuePattern);

    } else if (type->type()==TypeEntry::InstantiatedTemplateArgumentType) {
        meta_type->setTypeUsagePattern(MetaType::JObjectWrapperPattern);

    } else {
        meta_type->setTypeUsagePattern(MetaType::NativePointerPattern);
    }
}

QString MetaBuilder::translateDefaultValue(const QString& defaultValueExpression, MetaType *type,
        MetaFunction *fnc, MetaClass *implementing_class,
        int argument_index) {
    //QString function_name = fnc->name();
    QString class_name;
    QString replaced_expression;
    if(implementing_class){
        class_name = implementing_class->name();
        replaced_expression = fnc->replacedDefaultExpression(implementing_class, argument_index + 1);
        if (fnc->removedDefaultExpression(implementing_class, argument_index + 1)){
            return "";
        }
    }
    if (!replaced_expression.isEmpty())
        return replaced_expression;

    QString expr = defaultValueExpression;
    if(type->isTemplateArgument()){
        return expr;
    }
    if (type->isPrimitive() || type->isQChar()) {
        if (type->name() == "boolean") {
            if (expr == "false" || expr == "true") {
                return expr;
            } else if (expr == "boolean()") {
                return "false";
            } else {
                bool ok = false;
                int number = expr.toInt(&ok);
                if (ok && number)
                    return "true";
                else
                    return "false";
            }
        } else if (expr == "QChar()") {
            return "'\\0'";
        } else if (expr == "ULONG_MAX") {
            return "Long.MAX_VALUE";
        } else if (expr.startsWith("u'") && expr.endsWith("'")) {
            return expr.mid(1);
        } else if (expr.startsWith("QChar(u'") && expr.endsWith("')")) {
            return expr.chopped(1).mid(7);
        } else if (expr.startsWith("QChar('") && expr.endsWith("')")) {
            return expr.chopped(1).mid(6);
        } else if (expr.startsWith("QLatin1Char('") && expr.endsWith("')")) {
            return expr.chopped(1).mid(12);
        } else if (expr == "QVariant::Invalid") {
            return "0";
        } else {
            if(expr == type->name()+"()" || expr == "{}"){
                return type->isQChar() ? "'\\0'" : "0";
            }
            // This can be an enum or flag so I need to delay the
            // translation untill all namespaces are completly
            // processed. This is done in figureOutEnumValues()
            return expr;
        }
    }
    if (type && (type->isFlags() || type->isEnum())) {
        if(expr == "{}")
            return "0";
        // Same as with enum explanation above...
        return expr;

    }
    if (type && type->isFunctional() && (expr == "{}"
                                         || (expr.startsWith("std::function<") && expr.endsWith(">()"))
                                         || (expr.startsWith("qxp::function_ref<") && expr.endsWith(">()")))) {
        return "null";
    }
    if (type
            && type->typeEntry()->isUnknown()
            && (type->typeEntry()->qualifiedCppName().startsWith("std::function<")
                || type->typeEntry()->qualifiedCppName().startsWith("qxp::function_ref<")
                || type->typeEntry()->qualifiedCppName().contains("(*)")
                )
            && (expr == "{}" || expr == "0" || expr == "nullptr" || (expr.startsWith("std::function<") && expr.endsWith(">()")))) {
        return "null";
    }
    // constructor or functioncall can be a bit tricky...
    if (expr == "QVariant()") {
        return "null";
    }
    if (expr == "QModelIndex()") {
        return "new io.qt.core.QModelIndex()";
    }
    if (expr == "QStringList()"
               || ((expr.startsWith("QVector<") || expr.startsWith("QList<") || expr.startsWith("QStack<") || expr.startsWith("QQueue<")) && expr.endsWith(">()"))
              ) {
        return "java.util.Collections.emptyList()";
    }
    if (expr == "QVariantHash()"
               || ((expr.startsWith("QHash<") || expr.startsWith("QMultiHash<")) && expr.endsWith(">()"))) {
        return "java.util.Collections.emptyMap()";
    }
    if (expr == "QVariantMap()"
               || ((expr.startsWith("QMap<") || expr.startsWith("QMultiMap<")) && expr.endsWith(">()"))) {
        return "java.util.Collections.emptyNavigableMap()";
    }
    if (expr.startsWith("QSet<") && expr.endsWith(">()")) {
        return "java.util.Collections.emptySet()";
    }
    if (type && type->typeEntry() && type->typeEntry()->isInitializerList()) {
        if(expr == "{}")
            return "";
    }
    if (type && type->typeEntry() && type->typeEntry()->isContainer()) {
        const ContainerTypeEntry* cte = reinterpret_cast<const ContainerTypeEntry*>(type->typeEntry());
        switch(cte->type()){
        case ContainerTypeEntry::StringListContainer:
        case ContainerTypeEntry::ByteArrayListContainer:
        case ContainerTypeEntry::ListContainer:
        case ContainerTypeEntry::std_vector:
        case ContainerTypeEntry::LinkedListContainer:
        case ContainerTypeEntry::VectorContainer:
        case ContainerTypeEntry::QArrayDataContainer:
        case ContainerTypeEntry::QTypedArrayDataContainer:
            if(expr == "{}")
                return "java.util.Collections.emptyList()";
            break;
        case ContainerTypeEntry::StackContainer:
            if(expr == "{}")
                return "new java.util.ArrayDeque<>()";
            break;
        case ContainerTypeEntry::QueueContainer:
            if(expr == "{}")
                return "new java.util.ArrayDeque<>()";
            break;
        case ContainerTypeEntry::SetContainer:
            if(expr == "{}")
                return "java.util.Collections.emptySet()";
            break;
        case ContainerTypeEntry::MultiMapContainer:
        case ContainerTypeEntry::MapContainer:
            if(expr == "{}")
                return "java.util.Collections.emptyNavigableMap()";
            break;
        case ContainerTypeEntry::QDBusReplyContainer:
            if(expr == "{}")
                return "new io.qt.dbus.QDBusReply()";
            break;
        case ContainerTypeEntry::HashContainer:
            if(expr == "{}")
                return "java.util.Collections.emptyMap()";
            break;
        case ContainerTypeEntry::MultiHashContainer:
            if(expr == "{}")
                return "java.util.Collections.emptyMap()";
            break;
        case ContainerTypeEntry::PairContainer:
            if(expr == "{}")
                return "new io.qt.core.QPair<>()";
            break;
        case ContainerTypeEntry::std_optional:
            if(expr == "{}")
                return "java.util.Optional.empty()";
            break;
        case ContainerTypeEntry::std_chrono:
        case ContainerTypeEntry::std_chrono_template:
            if(expr == "std::chrono::milliseconds::zero()"
                || expr == "std::chrono::seconds::zero()"
                || expr == "std::chrono::nanoseconds::zero()"){
                return "java.time.Duration.ZERO";
            }
            if((expr.startsWith("std::chrono::seconds{") && expr.endsWith("}"))
                || (expr.startsWith("std::chrono::seconds(") && expr.endsWith(")"))){
                return QStringLiteral("java.time.Duration.ofSeconds(") + expr.mid(21).chopped(1) + ")";
            }
            if((expr.startsWith("std::chrono::milliseconds{") && expr.endsWith("}"))
                || (expr.startsWith("std::chrono::milliseconds(") && expr.endsWith(")"))){
                return QStringLiteral("java.time.Duration.ofMillis(") + expr.mid(26).chopped(1) + ")";
            }
            break;
        default:
            break;
        }
    }
    if (expr.endsWith(")") && expr.contains("::")) {
        TypeEntry *typeEntry = m_database->findType(expr.left(expr.indexOf("::")));
        if (typeEntry)
            return typeEntry->qualifiedTargetLangName().replace("$", ".") + "." + expr.right(expr.length() - expr.indexOf("::") - 2);
        expr = defaultValueExpression;
    }
    if (expr.endsWith(")") && type->typeEntry()->isComplex()
               && !(type->typeEntry()->isContainer()
                    || type->typeEntry()->isQChar()
                    || type->isQString()
                    || type->isCharString()
                    || type->isQStringView()
                    || type->isQAnyStringView()
                    || type->isQUtf8StringView())) {
        auto pos = expr.indexOf("(");

        TypeEntry *typeEntry = m_database->findType(expr.left(pos));
        if (typeEntry)
            return "new " + typeEntry->qualifiedTargetLangName().replace("$", ".") + expr.right(expr.length() - pos);
        else
            return expr;
    }
    if (expr == "0" || expr == "nullptr" || expr == "NULL") {
        return "null";
    }
    if (type->isQString()
               || type->isCharString()
               || type->isQStringView()
               || type->isQAnyStringView()
               || type->isQUtf8StringView()) {
        if (expr == "{}"
                || expr == "QString()"
                || expr == "QStringRef()"
                || expr == "QStringView()"
                || expr == "QAnyStringView()"
                || expr == "QUtf8StringView()") {
            return "(String)null";
        }
        if(expr.startsWith("QString(\"") && expr.endsWith("\")"))
            return expr.mid(8, expr.length()-9);
        if(expr.startsWith("QLatin1String(\"") && expr.endsWith("\")"))
            return expr.mid(14, expr.length()-15);
        else if ((expr.startsWith("u'") || expr.startsWith("U'") || expr.startsWith("L'")) && expr.endsWith("'"))
            return expr.mid(1);
        else if (expr.startsWith("QLatin1Char('") && expr.endsWith("')"))
            return expr.chopped(1).mid(12);
        else if (expr.startsWith("u8'") && expr.endsWith("'"))
            return expr.mid(2);
        else if(expr.startsWith("u8\"") && expr.endsWith("\""))
            return expr.mid(2);
        else if((expr.startsWith("u\"") || expr.startsWith("U\"") || expr.startsWith("L\"")) && expr.endsWith("\""))
            return expr.mid(1);
        else if(expr.startsWith("R\"(") && expr.endsWith(")\"")){
            expr = expr.mid(3, expr.length()-5);
            expr.replace("\\", "\\\\");
            expr.replace("\"", "\\\"");
            expr.replace("\t", "\\t");
            expr.replace("\n", "\\n");
            expr.replace("\r", "\\r");
            expr.replace("\b", "\\b");
            expr.replace("\f", "\\f");
            expr = "\"" + expr + "\"";
        }
        return expr;
    }
    if ((type->typeEntry()->isComplex() && !type->typeEntry()->isContainer() && !type->typeEntry()->isQChar()) || expr.contains("::")) { // like Qt::black passed to a QColor
        if(type->isObject() && expr == type->name()+"()")
            return "null";
        if (expr == "{}")
            return "new " + type->typeEntry()->qualifiedTargetLangName().replace("$", ".") + "()";
        if (expr.startsWith("QLatin1Char('") && expr.endsWith("')"))
            return expr.chopped(1).mid(12);
        if ((expr.startsWith("u'") || expr.startsWith("U'") || expr.startsWith("L'")) && expr.endsWith("'"))
            return expr.mid(1);
        if (defaultValueExpression == QLatin1String("std::nullopt"))
            return "java.util.Optional.empty()";
        if(MetaEnumValue * enumValue = m_meta_classes.findEnumValue(expr)){
            return "new " + type->typeEntry()->qualifiedTargetLangName().replace("$", ".") +
                   "(" + enumValue->getEnum()->typeEntry()->qualifiedTargetLangName().replace('$', '.') + "." + enumValue->name() + ")";
        }
        TypeEntry *typeEntry = m_database->findType(expr.left(expr.indexOf("::")));
        if (typeEntry) {
            expr = expr.right(expr.length() - expr.indexOf("::") - 2);
            return "new " + type->typeEntry()->qualifiedTargetLangName().replace("$", ".") +
                   "(" + typeEntry->qualifiedTargetLangName().replace("$", ".") + "." + expr + ")";
        }
        expr = defaultValueExpression;
    }
    if (expr.startsWith("QLatin1Char('") && expr.endsWith("')"))
        return expr.chopped(1).mid(12);
    if ((expr.startsWith("u'") || expr.startsWith("U'") || expr.startsWith("L'")) && expr.endsWith("'"))
        return expr.mid(1);
    if (expr == "QString()" || expr == "QStringRef()" || expr == "QStringView()")
        return "null";
    if (expr == "QChar()")
        return "'\\0'";
    if (defaultValueExpression == QLatin1String("std::nullopt"))
        return "java.util.Optional.empty()";

    QString warn = QString("Unsupported default value '%1' of argument %2 in function '%3%4'")
                   .arg(defaultValueExpression, QString::number(argument_index+1), class_name.isEmpty() ? class_name : QString("%1::").arg(class_name), fnc->minimalSignature());
    ReportHandler::warning(warn);

    return QString();
}


bool MetaBuilder::isClass(const QString &qualified_name, const QString& className) {
    if (qualified_name == className)
        return true;

    ClassModelItem class_item = m_dom->findClass(qualified_name);

    if (!class_item) {
        QStringList names = qualified_name.split(QLatin1String("::"));
        NamespaceModelItem ns = model_dynamic_cast<NamespaceModelItem>(m_dom);
        for (int i = 0; i < names.size() - 1 && ns; ++i)
            ns = ns->findNamespace(names.at(i));
        if (ns && names.size() >= 2)
            class_item = ns->findClass(names.at(names.size() - 1));
    }

    bool result = class_item && class_item->extendsClass(className);

    if (class_item && !result) {
        for(const QPair<TypeInfo,bool>& p : class_item->baseClasses()){
            if(p.second){
                result = isClass(p.first.qualifiedName().join("::"), className);
                if (result)
                    break;
            }
        }
    }

    return result;
}


bool MetaBuilder::isEnum(const QStringList &qualified_name) {
    CodeModelItem item = m_dom->model()->findItem(qualified_name, m_dom->toItem());
    return item && item->kind() == _EnumModelItem::__node_kind;
}

MetaType *MetaBuilder::inheritTemplateType(const QList<const MetaType *> &template_types,
        const MetaType *meta_type, bool *ok) {
    if (ok)
        *ok = true;
    if (!meta_type || (!meta_type->typeEntry()->isTemplateArgument() && !meta_type->hasInstantiations()))
        return meta_type ? meta_type->copy() : nullptr;

    MetaType *returned = meta_type->copy();
    returned->setOriginalTemplateType(meta_type->copy());

    if (returned->typeEntry()->isTemplateArgument()) {
        const TemplateArgumentEntry *tae = static_cast<const TemplateArgumentEntry *>(returned->typeEntry());

        // If the template is intantiated with void we special case this as rejecting the functions that use this
        // parameter from the instantiation.
        if (template_types.size() <= tae->ordinal() || template_types.at(tae->ordinal())->typeEntry()->name() == "void") {
            if (ok)
                *ok = false;
            return nullptr;
        }

        MetaType *t = returned->copy();
        const MetaType * template_type = template_types.at(tae->ordinal());
        t->setTypeEntry(template_type->typeEntry());
        t->setForceBoxedPrimitives(template_type->forceBoxedPrimitives());
        if(template_type->indirections().size() + t->indirections().size()>0){
            t->setIndirections(QList<bool>() << false);
        }else{
            t->setIndirections(QList<bool>());
        }
        decideUsagePattern(t);

        delete returned;
        if(template_type->typeEntry()->isTemplateArgument()){
            return t;
        }
        returned = inheritTemplateType(template_types, t, ok);
        if (ok && !(*ok))
            return nullptr;
    }

    if (returned->hasInstantiations()) {
        QList<const MetaType *> instantiations = returned->instantiations();
        for (int i = 0; i < instantiations.count(); ++i) {
            instantiations[i] = inheritTemplateType(template_types, instantiations.at(i), ok);
            if (ok && !(*ok))
                return nullptr;
        }
        returned->setInstantiations(instantiations);
    }

    return returned;
}

void MetaBuilder::inheritHiddenBaseType(MetaClass *subclass, const MetaClass *hidden_base_class,
        const TypeInfo &info, QList<MetaClass *>& pendingConstructorUsages) {
    QList<const MetaType *> template_types;
    QHash<const TypeEntry *,const MetaType *> template_types_by_name;
    for (int i = 0; i < info.arguments().size(); ++i) {
        const TypeInfo &ti = info.arguments()[i];
        bool ok = false;
        MetaType *temporary_type = translateType(ti, &ok);
        if(ok){
            template_types << temporary_type;
        }else{
            TypeEntry *t = m_database->findType(ti.toString());
            if (t) {
                temporary_type = new MetaType();
                temporary_type->setTypeEntry(t);
                temporary_type->setConstant(ti.isConstant());
                temporary_type->setReferenceType(MetaType::ReferenceType(ti.getReferenceType()));
                temporary_type->setIndirections(ti.indirections());
                if( t->isPrimitive() && temporary_type->indirections().isEmpty() && (
                            hidden_base_class->typeEntry()->isContainer()
                            || hidden_base_class->typeEntry()->isIterator() ) ){
                    temporary_type->setForceBoxedPrimitives(true);
                }
                decideUsagePattern(temporary_type);
                if(subclass->typeEntry()->isGenericClass() && i < hidden_base_class->templateArguments().size()){
                    temporary_type->setTypeEntry(new InstantiatedTemplateArgumentEntry(i, hidden_base_class->templateArguments().at(i), t));
                }
                template_types << temporary_type;
            }else if(m_templates.contains(subclass) && template_types.size()<subclass->templateArguments().size()){
                temporary_type = new MetaType();
                temporary_type->setTypeEntry(subclass->templateArguments().at(template_types.size()));
                temporary_type->setConstant(ti.isConstant());
                temporary_type->setReferenceType(MetaType::ReferenceType(ti.getReferenceType()));
                temporary_type->setIndirections(ti.indirections());
                decideUsagePattern(temporary_type);
                template_types << temporary_type;
            }
        }
    }

    for (auto l = qMin(template_types.size(), qMin(hidden_base_class->templateArguments().size(), info.arguments().size())), i = decltype(l)(0); i < l; ++i) {
        template_types_by_name.insert(hidden_base_class->templateArguments().at(i), template_types.at(i));
    }

    {
        if(!info.arguments().isEmpty()){
            subclass->setTemplateBaseClass(hidden_base_class);
            subclass->setTemplateBaseClassInstantiations(template_types);

            subclass->setInterfaces(hidden_base_class->interfaces());
            subclass->setBaseClass(hidden_base_class->baseClass());
            pendingConstructorUsages.append(subclass);
        }

        QString implements = hidden_base_class->typeEntry()->implements();
        if(!implements.isEmpty()){
            for (QHash<const TypeEntry *,const MetaType *>::const_iterator ki = template_types_by_name.constBegin();
                 ki != template_types_by_name.constEnd(); ki++) {
                const TypeEntry * t_entry = ki.value()->typeEntry();
                if(t_entry->isTemplateArgument())
                    implements = implements.replace("%"+ki.key()->name(),"%"+t_entry->qualifiedCppName());
                else
                    implements = implements.replace("%"+ki.key()->name(),t_entry->qualifiedTargetLangName());
            }
            if(subclass->typeEntry()->implements().isEmpty())
                subclass->typeEntry()->setImplements(implements);
            else
                subclass->typeEntry()->setImplements(subclass->typeEntry()->implements()+", "+implements);
        }
    }

    if(hidden_base_class->typeEntry()->isContainer()){
        const ContainerTypeEntry* ctype = static_cast<const ContainerTypeEntry*>(hidden_base_class->typeEntry());
        switch(ctype->type()){
        case ContainerTypeEntry::ListContainer:
        case ContainerTypeEntry::LinkedListContainer:
        case ContainerTypeEntry::VectorContainer:
        case ContainerTypeEntry::StackContainer:
        case ContainerTypeEntry::QueueContainer:
        case ContainerTypeEntry::SetContainer:
        case ContainerTypeEntry::MapContainer:
        case ContainerTypeEntry::HashContainer:
        case ContainerTypeEntry::MultiMapContainer:
        case ContainerTypeEntry::MultiHashContainer:
            return;
        default: break;
        }
    }

    IteratorTypeEntry * iteratorTypeEntry = m_database->findIteratorType(hidden_base_class->typeEntry());
    MetaClass * newIteratorClass = nullptr;
    if(iteratorTypeEntry){
        newIteratorClass = instantiateIterator(iteratorTypeEntry, subclass, template_types, template_types_by_name);
    }

    const MetaFunctionList& funcs = subclass->functions();
    for(const MetaFunction *function : hidden_base_class->functions()) {
        if (function->isModifiedRemoved(TS::All) || function->isStatic())
            continue;

        MetaFunction *f = function->copy();
        f->setArguments(MetaArgumentList());

        bool ok = true;
        MetaType *ftype = function->type();
        if(ftype){
            bool exchanged = false;
            if(ftype->typeEntry()->isComplex()
                    && reinterpret_cast<const ComplexTypeEntry*>(ftype->typeEntry())->isTemplate()
                    && !info.arguments().isEmpty()){
                const ComplexTypeEntry* ctype = reinterpret_cast<const ComplexTypeEntry*>(ftype->typeEntry());
                QStringList templateArgs;
                for(const TypeInfo &i : info.arguments()) {
                    templateArgs << i.toString();
                }
                if(const ComplexTypeEntry* instantiation = ctype->instantiations()[templateArgs]){
                    MetaType *_ftype = ftype->copy();
                    if(!instantiation->isGenericClass()){
                        _ftype->setInstantiations({});
                        _ftype->setTypeEntry(instantiation);
                    }else{
                        _ftype->setInstantiations(template_types);
                    }
                    decideUsagePattern(_ftype);
                    f->setType(_ftype);
                    exchanged = true;
                }
            }
            if(!exchanged){
                if(ftype->typeEntry()->qualifiedCppName()==hidden_base_class->typeEntry()->qualifiedCppName()
                        && ftype->instantiations().size()==0){
                    ftype = ftype->copy();
                    ftype->setInstantiations(template_types);
                    f->setType(inheritTemplateType(template_types, ftype, &ok));
                    delete ftype;
                }else if(iteratorTypeEntry
                         && newIteratorClass
                         && ftype->typeEntry()==iteratorTypeEntry){
                     ftype = ftype->copy();
                     ftype->setTypeEntry(newIteratorClass->typeEntry());
                     f->setType(inheritTemplateType(template_types, ftype, &ok));
                     delete ftype;
                }else{
                    f->setType(inheritTemplateType(template_types, ftype, &ok));
                }
            }
        }
        if (!ok) {
            delete f;
            continue;
        }

        for(MetaArgument *argument : function->arguments()) {
            MetaArgument *arg = argument->copy();
            MetaType *atype = argument->type();
            if(atype){
                bool exchanged = false;
                if(atype->typeEntry()->isComplex()
                        && reinterpret_cast<const ComplexTypeEntry*>(atype->typeEntry())->isTemplate()
                        && !info.arguments().isEmpty()){
                    const ComplexTypeEntry* ctype = reinterpret_cast<const ComplexTypeEntry*>(atype->typeEntry());
                    QStringList templateArgs;
                    for(const TypeInfo &i : info.arguments()) {
                        templateArgs << i.toString();
                    }
                    if(const ComplexTypeEntry* instantiation = ctype->instantiations()[templateArgs]){
                        MetaType *_atype = atype->copy();
                        if(!instantiation->isGenericClass()){
                            _atype->setInstantiations({});
                            _atype->setTypeEntry(instantiation);
                        }else{
                            _atype->setInstantiations(template_types);
                        }
                        decideUsagePattern(_atype);
                        arg->setType(_atype);
                        exchanged = true;
                    }
                }
                if(!exchanged){
                    if(atype->typeEntry()->qualifiedCppName()==hidden_base_class->typeEntry()->qualifiedCppName()
                            && atype->instantiations().size()==0){
                        atype = atype->copy();
                        atype->setInstantiations(template_types);
                        arg->setType(inheritTemplateType(template_types, atype, &ok));
                        delete atype;
                    }else if(iteratorTypeEntry
                             && newIteratorClass
                             && atype->typeEntry()==iteratorTypeEntry){
                        atype = atype->copy();
                        atype->setTypeEntry(newIteratorClass->typeEntry());
                        arg->setType(inheritTemplateType(template_types, atype, &ok));
                        delete atype;
                    }else{
                        arg->setType(inheritTemplateType(template_types, atype, &ok));
                    }
                }

                if (!ok)
                    break;

                if(!arg->defaultValueExpression().isEmpty() && argument->type()->typeEntry()->isTemplateArgument()){
                    QString exp = arg->defaultValueExpression();
                    arg->setOriginalDefaultValueExpression(exp);
                    exp = exp.replace(argument->type()->typeEntry()->qualifiedCppName(), arg->type()->typeEntry()->qualifiedCppName());
                    exp = translateDefaultValue(exp, arg->type(), f, subclass, int(f->arguments().size()));
                    arg->setDefaultValueExpression(exp);
                }
                f->addArgument(arg);
            }
        }

        if (!ok) {
            delete f;
            continue ;
        }

        // There is no base class in java to inherit from here, so the
        // template instantiation is the class that implements the function..
        f->setImplementingClass(subclass);

        // We also set it as the declaring class, since the superclass is
        // supposed to disappear. This allows us to make certain function modifications
        // on the inherited functions.
        f->setDeclaringClass(subclass);

        f->setOriginalSignature(function->originalSignature());
        if (f->isConstructor()) {
            f->setName(subclass->simpleName());
        //} else if (f->isConstructor()) {
        //    delete f;
        //    continue;
        }

        // if the instantiation has a function named the same as an existing
        // function we have shadowing so we need to skip it.
        bool found = false;
        for (int i = 0; i < funcs.size(); ++i) {
            MetaFunction* f2 = funcs.at(i);
            if (f2->name() == f->name() && f2->arguments().size()==f->arguments().size()) {
                found = true;
                for(int j=0; j<f->arguments().size(); j++){
                    MetaArgument* arg = f->arguments().at(j);
                    MetaArgument* arg2 = f2->arguments().at(j);
                    if(
                            arg->type()->isConstant()!=arg2->type()->isConstant()
                            || arg->type()->indirections()!=arg2->type()->indirections()
                            || arg->type()->typeUsagePattern()!=arg2->type()->typeUsagePattern()
                            || arg->type()->typeEntry()->qualifiedCppName()!=arg2->type()->typeEntry()->qualifiedCppName()
                            ){
                        found = false;
                        break;
                    }
                }
                if (found) {
                    continue;
                }
            }
        }
        if (found) {
            delete f;
            continue;
        }

        ComplexTypeEntry *te = subclass->typeEntry();
        FunctionModificationList mods = function->modifications(hidden_base_class);
        for (int i = 0; i < mods.size(); ++i) {
            FunctionModification mod = mods.at(i);
            mod.signature = f->minimalSignature();

            // If we ever need it... Below is the code to do
            // substitution of the template instantation type inside
            // injected code..
            if (mod.modifiers & Modification::CodeInjection) {
                for (QHash<const TypeEntry *,const MetaType *>::const_iterator ki = template_types_by_name.begin();
                     ki != template_types_by_name.end(); ki++) {
                    CodeSnip &snip = mod.snips.last();
                    QString code = snip.code();
                    const TypeEntry * t_entry = ki.value()->typeEntry();
                    if(t_entry->isTemplateArgument())
                        code = code.replace("%"+ki.key()->name(),"%"+t_entry->qualifiedCppName());
                    else {
                        if(snip.language==TS::TargetLangCode){
                            if(t_entry->isPrimitive()){
                                const PrimitiveTypeEntry* ptype = reinterpret_cast<const PrimitiveTypeEntry*>(t_entry);
                                code = code.replace("%"+ki.key()->name(),ptype->javaObjectFullName());
                            }else{
                                code = code.replace("%"+ki.key()->name(),t_entry->qualifiedTargetLangName());
                            }
                        }else{
                            code = code.replace("%"+ki.key()->name(),t_entry->qualifiedCppName());
                        }
                    }
                    snip.codeList.clear();
                    snip.addCode(code);
                }
            }
            te->addFunctionModification(mod);
        }
        subclass->addFunction(f);
    }

    const MetaFieldList& fields = subclass->fields();
    for(const MetaField *field : hidden_base_class->fields()) {
        if (field->isStatic())
            continue;

        MetaField *f = field->copy();

        bool ok = true;
        MetaType *ftype = field->type();
        if(ftype){
            bool exchanged = false;
            if(ftype->typeEntry()->isComplex()
                    && reinterpret_cast<const ComplexTypeEntry*>(ftype->typeEntry())->isTemplate()
                    && !info.arguments().isEmpty()){
                const ComplexTypeEntry* ctype = reinterpret_cast<const ComplexTypeEntry*>(ftype->typeEntry());
                QStringList templateArgs;
                for(const TypeInfo &i : info.arguments()) {
                    templateArgs << i.toString();
                }
                if(const ComplexTypeEntry* instantiation = ctype->instantiations()[templateArgs]){
                    MetaType *_ftype = ftype->copy();
                    if(!instantiation->isGenericClass()){
                        _ftype->setInstantiations({});
                        _ftype->setTypeEntry(instantiation);
                    }else{
                        _ftype->setInstantiations(template_types);
                    }
                    decideUsagePattern(_ftype);
                    f->setType(_ftype);
                    exchanged = true;
                }
            }
            if(!exchanged){
                if(ftype->typeEntry()->qualifiedCppName()==hidden_base_class->typeEntry()->qualifiedCppName()
                        && ftype->instantiations().size()==0){
                    ftype = ftype->copy();
                    ftype->setInstantiations(template_types);
                    f->setType(inheritTemplateType(template_types, ftype, &ok));
                    delete ftype;
                }else if(iteratorTypeEntry
                         && newIteratorClass
                         && ftype->typeEntry()==iteratorTypeEntry){
                     ftype = ftype->copy();
                     ftype->setTypeEntry(newIteratorClass->typeEntry());
                     f->setType(inheritTemplateType(template_types, ftype, &ok));
                     delete ftype;
                }else{
                    f->setType(inheritTemplateType(template_types, ftype, &ok));
                }
            }
        }
        if (!ok) {
            delete f;
            continue;
        }

        if (!ok) {
            delete f;
            continue ;
        }

        // if the instantiation has a function named the same as an existing
        // function we have shadowing so we need to skip it.
        bool found = false;
        for (int i = 0; i < fields.size(); ++i) {
            MetaField* f2 = fields.at(i);
            if (f2->name() == f->name()) {
                found = true;
                if (found) {
                    continue;
                }
            }
        }
        if (found) {
            delete f;
            continue;
        }
        f->setEnclosingClass(subclass);
        subclass->addField(f);
        if(!f->isPublic())
            subclass->typeEntry()->setHasNonPublicFields();
    }

    if(hidden_base_class->typeEntry()->qualifiedCppName()=="QStack"
            && template_types.size()>0
            && template_types.at(0)->isPrimitive()){
        FunctionModification mod2;
        mod2.signature = QMetaObject::normalizedSignature("pop()");
        mod2.modifiers = Modification::Rename | Modification::Private;
        mod2.renamedToName = "_pop";
        subclass->typeEntry()->addFunctionModification(mod2);
    }else if((hidden_base_class->typeEntry()->qualifiedCppName()=="QMap"
        || hidden_base_class->typeEntry()->qualifiedCppName()=="QMultiMap"
        || hidden_base_class->typeEntry()->qualifiedCppName()=="QHash"
        || hidden_base_class->typeEntry()->qualifiedCppName()=="QMultiHash")
            && template_types.size()>1){
        FunctionModification mod2;
        if(template_types.at(0)->isPrimitive()
                && (hidden_base_class->typeEntry()->qualifiedCppName()=="QMap"
                    || hidden_base_class->typeEntry()->qualifiedCppName()=="QMultiMap")){
            mod2.signature = QMetaObject::normalizedSignature("lastKey() const");
            mod2.modifiers = Modification::Rename | Modification::Private;
            mod2.renamedToName = "_lastKey";
            subclass->typeEntry()->addFunctionModification(mod2);
            mod2.signature = QMetaObject::normalizedSignature("firstKey() const");
            mod2.modifiers = Modification::Rename | Modification::Private;
            mod2.renamedToName = "_firstKey";
            subclass->typeEntry()->addFunctionModification(mod2);
        }
    }
    subclass->setHasJustPrivateConstructors(hidden_base_class->hasJustPrivateConstructors());
}

void MetaBuilder::parseQ_Property(MetaClass *meta_class, const QStringList &declarations) {
    for (int i = 0; i < declarations.size(); ++i) {
        QString p = declarations.at(i);

        /*
        Pass 1: normalize all whitespace.
        Remove leading/trailing, convert contiguous whitespace sequences to a single space character.
        */
        {
            QString newP = QString();
            auto len = p.length();
            int state = 0;
            int j;
            for (j = 0; j < len; j++) {
                QChar c = p.at(j);
                if (state == 0) { // skip leading spaces
                    if(!c.isSpace()) {
                        newP += c;
                        state++;
                    }
                } else if(state == 1) { // last token was not whitespace
                    if(c.isSpace()) { // normalize to an actual space, from maybe \r\n\v\t etc.
                        newP += ' ';
                        state++;
                    } else {
                        newP += c;
                    }
                } else { // last token was whitespace
                    if(!c.isSpace()) {
                        newP += c;
                        state--;
                    }
                }
            }

            auto newplen = newP.length();

            // remove that last space we added
            if(state >= 2 && newplen > 1 && newP.at(newplen - 1) == QChar(' ')) {
                newP = newP.left(newplen - 1);
            }

            if(!newP.isNull()) {
                if (newP != p)
                    p = newP;
            }
        }

        /*
        Pass 2: Correct the first word to always be a type (this means removing
        all whitespace from the type declaration) by looking ahead at the data.

        Convert "Q_PROPERTY(QGraphicsObject * parent READ parentObject WRITE setParentItem NOTIFY parentChanged DESIGNABLE false)"
        into "Q_PROPERTY(QGraphicsObject* parent READ parentObject WRITE setParentItem NOTIFY parentChanged DESIGNABLE false)"

        moc.exe doesn't allow any comma in the property spec string, we do just in case
        that changes someday since it can be part of a type with multiple template info.
        */
        {
            QString newFirstWord = QString();
            auto len = p.length();

            /*
            0 = start (expect isalnum() || :)
            1 = last-char-was-space (expect <*&)
            2+ = inside-open-angle (expect isalnum() || : || <>*&, || ">")
            */
            int state = 0;

            int j;
            for (j = 0; j < len; j++) {
                QChar c = p.at(j);
                if (state == 0) {
                    if (c.isLetterOrNumber() || c == QChar(':') || c == QChar('*') || c == QChar('&')) {
                        newFirstWord += c;
                    } else if(c == QChar('<')) {
                        newFirstWord += c;
                        state = 2;
                    } else if (c.isSpace()) {
                        state = 1;
                    }
                } else if (state == 1) {
                    if (c == QChar('<')) {
                        newFirstWord += c;
                        state = 2;
                    } else if (c == QChar('*') || c == QChar('&')) {
                        newFirstWord += c;
                    } else if (c.isSpace()) {
                        /*
                        Use-case from state==0: strictly speaking 2 space chars in a row are not allowed
                        Use-case from state==2: we want to eat the space between 1st and 2nd word
                        */
                    } else { // if (c.isLetterOrNumber())
                        // this is to be the 2nd word, so we are done
                        break;
                    }
                } else if (state >= 2) {
                    if (c.isLetterOrNumber() || c == QChar(':') || c == QChar('*') || c == QChar('&') || c == QChar(',')) {
                        newFirstWord += c;
                    } else if (c == QChar('<')) {
                        newFirstWord += c;
                        state++;
                    } else if (c == QChar('>')) {
                        newFirstWord += c;
                        state--;
                    } else if (c.isSpace()) {
                        // nop - strictly speaking space is only allowed around non-isalnum() chars
                    } else {
                        qWarning() << "Q_PROPERTY() parse error p=" << p;
                        newFirstWord = QString(); // abort
                        break;
                    }
                }
            }
            // cut length i from left(), stitch newFirstWord
            if(!newFirstWord.isNull()) {
                QString newP = newFirstWord + ' ' + p.mid(j);
                if (newP != p)
                    p = newP;
            }
        }

        /*
        Pass 3: Split by space character then normalize parentesis by joining
        expression parts into single elements.

        "FooClass*" "foo" "READ" "(expr" "!=" "0)" "WRITE" "setFoo"

        becomes:

        "FooClass*" "foo" "READ" "(expr != 0)" "WRITE" "setFoo"

        This is known to allow correct parsing of some Q_PROPERTY in QToolBar.
        */
        QStringList l = p.split(QLatin1String(" "), Qt::SkipEmptyParts);
        {
            QStringList newL = QStringList();
            auto l_size = l.size();
            int nest = 0;
            QString newItem = QString(); // recombined item
            for (int j = 0; j < l_size; j++) {
                const QString item = l.at(j); // original item
                auto item_length = item.length();
                for (int k = 0; k < item_length; k++) {
                    const QChar ch = item.at(k);
                    if (ch == QChar('('))
                        nest++;
                    else if (ch == QChar(')'))
                        nest--;
                }
                if (newItem.isNull()) {
                    newItem = item;
                } else {
                    // only add spaces between parts we join
                    newItem += ' ';
                    newItem += item;
                }
                if(nest == 0) {
                    newL.append(newItem);
                    newItem = QString(); // set to null
                }
            }
            if (!newItem.isNull())
                newL.append(newItem);

            l = newL;
        }

        QString typeName = l.takeAt(0);
        if(typeName=="const")
            typeName += " " + l.takeAt(0);
        QPropertySpec *spec = new QPropertySpec();
        QString name = l.takeAt(0);
        if(name.startsWith("*")){
            typeName += "*";
            name = name.mid(1);
        }
        spec->setName(name);
        spec->setIndex(i);

        for (int pos = 0; pos < l.size(); ++pos) {
            // I have seen DESIGNABLE and SCRIPTABLE examples that do not have
            //  a true/false after but another keyword.
            const QString& aspect = l.at(pos);
            if (aspect == QLatin1String("READ")){
                if(pos < l.size()-1)
                    spec->setRead(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("WRITE")){
                if(pos < l.size()-1)
                    spec->setWrite(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("BINDABLE")){
                if(pos < l.size()-1)
                    spec->setBindable(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("DESIGNABLE")){
                if(pos < l.size()-1)
                    spec->setDesignable(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("SCRIPTABLE")){
                if(pos < l.size()-1)
                    spec->setScriptable(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("RESET")){
                if(pos < l.size()-1)
                    spec->setReset(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("NOTIFY")){
                if(pos < l.size()-1)
                    spec->setNotify(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("USER")){
                if(pos < l.size()-1)
                    spec->setUser(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("STORED")){
                if(pos < l.size()-1)
                    spec->setStored(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect == QLatin1String("REVISION")){
                if(pos < l.size()-1)
                    spec->setRevision(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else if (aspect.startsWith(QLatin1String("REVISION(")) && aspect.endsWith(QLatin1String(")"))){
                QString revision = aspect;
                revision.chop(1);
                revision = revision.mid(9);
                spec->setRevision(revision);
            }
            else if (aspect == QLatin1String("CONSTANT")){
                spec->setConstant(true);
            }
            else if (aspect == QLatin1String("REQUIRED")){
                spec->setRequired(true);
            }
            else if (aspect == QLatin1String("FINAL")){
                spec->setFinal(true);
            }
            else if (aspect == QLatin1String("MEMBER")){
                if(pos < l.size()-1)
                    spec->setMember(l.at(++pos));
                else
                    ReportHandler::warning(QString("Q_PROPERTY(%1): aspect %2 requires argument").arg(declarations.at(i), aspect));
            }
            else
                ReportHandler::warning(QString("Q_PROPERTY(%1): unknown aspect %2").arg(declarations.at(i), aspect));
        }

        meta_class->addPropertySpec(spec);
    }
}

static MetaFunctionList filter_functions(const MetaFunctionList &lst, QSet<QString> *signatures) {
    MetaFunctionList functions;
    for(MetaFunction *f : lst) {
        if(f->wasPrivate())
            continue;
        QString signature = f->minimalSignature();
        auto start = signature.indexOf(QLatin1Char('(')) + 1;
        auto end = signature.lastIndexOf(QLatin1Char(')'));
        signature = signature.mid(start, end - start);
        if (signatures->contains(signature)) {
            remove_function(f);
            continue;
        }
        (*signatures) << signature;
        functions << f;
    }
    return functions;
}

void MetaBuilder::setupBeginEnd(MetaClass *cls) {
    MetaFunctionList begins;
    MetaFunctionList ends;

    QString name_begin = QLatin1String("begin");
    QString name_end = QLatin1String("end");
    QString name_constBegin = QLatin1String("constBegin");
    QString name_constEnd = QLatin1String("constEnd");

    MetaFunctionList functions = cls->queryFunctions(MetaClass::ClassImplements
                                         | MetaClass::NotRemovedFromTargetLang);
    for(MetaFunction *f : functions) {
        if(f->type() && f->type()->typeEntry()->isIterator() && f->arguments().size()==0){
            if (f->name() == name_begin || f->name() == name_constBegin)
                begins << f;
            else if (f->name() == name_end || f->name() == name_constEnd)
                ends << f;
        }
    }

    if (begins.size() || ends.size()) {
        for(MetaFunction *f : begins) {
            FunctionModification mod;
            mod.signature = f->minimalSignature();
            mod.modifiers = FunctionModification::Protected;
            const_cast<ComplexTypeEntry *>(static_cast<const ComplexTypeEntry *>(f->implementingClass()->typeEntry()))->addFunctionModification(mod);
        }
        for(MetaFunction *f : ends) {
            FunctionModification mod;
            mod.signature = f->minimalSignature();
            mod.modifiers = FunctionModification::Protected;
            const_cast<ComplexTypeEntry *>(static_cast<const ComplexTypeEntry *>(f->implementingClass()->typeEntry()))->addFunctionModification(mod);
        }
        cls->setBeginFunctions(begins);
        cls->setEndFunctions(ends);
    }
}

void MetaBuilder::setupEquals(MetaClass *cls) {
    MetaFunctionList equals;
    MetaFunctionList nequals;
    MetaFunctionList functions = cls->queryFunctions(
                                        MetaClass::ClassImplements
                                        | MetaClass::Visible
                                        | MetaClass::NotRemovedFromTargetLang);
    for(MetaFunction *f : functions) {
        switch(f->operatorType()){
        case OperatorType::Equals:
            if(f->arguments().size()==1)
                equals << f;
            break;
        case OperatorType::NotEquals:
            if(f->arguments().size()==1)
                nequals << f;
            break;
        default: break;
        }
    }

    if (equals.size() || nequals.size()) {
        if (!cls->hasHashFunction()
                && cls->typeEntry()->hasEquals()
                && cls->typeEntry()->codeGeneration()==TypeEntry::GenerateAll
                && (cls->typeEntry()->isValue()
                    || (cls->typeEntry()->hasPublicDefaultConstructor() && cls->typeEntry()->hasPublicCopyConstructor() && cls->typeEntry()->isDestructorPublic()))) {
            ReportHandler::warning(QString::fromLatin1("Class '%1' has equals operators but no qHash() function. Hashcode of objects will consistently be 0.")
                                   .arg(cls->qualifiedCppName()));
        }

        hide_functions(equals);
        hide_functions(nequals);

        // We only need == if we have both == and !=, and one == for
        // each signature type, like QDateTime::==(QDate) and (QTime)
        // if such a thing exists...
        QSet<QString> func_signatures;
        cls->setEqualsFunctions(filter_functions(equals, &func_signatures));
        cls->setNotEqualsFunctions(filter_functions(nequals, &func_signatures));
    }
}

void MetaBuilder::setupComparable(MetaClass *cls) {
    MetaFunctionList greater;
    MetaFunctionList greaterEquals;
    MetaFunctionList less;
    MetaFunctionList lessEquals;
    MetaFunctionList compare;

    const MetaFunctionList functions = cls->queryFunctions(MetaClass::ClassImplements
                                         | MetaClass::NotRemovedFromTargetLang);
    for(MetaFunction *f : functions) {
        switch(f->operatorType()){
        case OperatorType::Greater:
            if(f->arguments().size()==1)
                greater << f;
            break;
        case OperatorType::GreaterOrEquals:
            if(f->arguments().size()==1)
                greaterEquals << f;
            break;
        case OperatorType::Less:
            if(f->arguments().size()==1)
                less << f;
            break;
        case OperatorType::LessOrEquals:
            if(f->arguments().size()==1)
                lessEquals << f;
            break;
        case OperatorType::Compare:
            if(f->arguments().size()==1)
                compare << f;
            break;
        default: break;
        }
    }

    bool hasEquals = cls->equalsFunctions().size() || cls->notEqualsFunctions().size();

    // Conditions for comparable is:
    //     >, ==, <             - The basic case
    //     >, ==                - Less than becomes else case
    //     <, ==                - Greater than becomes else case
    //     >=, <=               - if (<= && >=) -> equal
    bool mightBeComparable = compare.size() || greater.size() || greaterEquals.size() || less.size() || lessEquals.size()
                             || greaterEquals.size() == 1 || lessEquals.size() == 1;

    if (mightBeComparable) {
        QSet<QString> signatures;

        // We only hide the original functions if we are able to make a compareTo() method
        bool wasComparable = false;

        // The three upper cases, prefer the <, == approach
        if(compare.size()){
            cls->setCompareFunctions(filter_functions(compare, &signatures));
            wasComparable = true;
        }else if (hasEquals && (greater.size() || less.size())) {
            cls->setLessThanFunctions(filter_functions(less, &signatures));
            cls->setGreaterThanFunctions(filter_functions(greater, &signatures));
            filter_functions(greaterEquals, &signatures);
            filter_functions(lessEquals, &signatures);
            wasComparable = true;
        } else if (hasEquals && (greaterEquals.size() || lessEquals.size())) {
            cls->setLessThanEqFunctions(filter_functions(lessEquals, &signatures));
            cls->setGreaterThanEqFunctions(filter_functions(greaterEquals, &signatures));
            filter_functions(less, &signatures);
            filter_functions(greater, &signatures);
            wasComparable = true;
        } else if (greaterEquals.size() == 1 || lessEquals.size() == 1) {
            cls->setGreaterThanEqFunctions(filter_functions(greaterEquals, &signatures));
            cls->setLessThanEqFunctions(filter_functions(lessEquals, &signatures));
            filter_functions(less, &signatures);
            filter_functions(greater, &signatures);
            wasComparable = true;
        } else if (greater.size() == 1 || less.size() == 1) {
            cls->setGreaterThanFunctions(filter_functions(greater, &signatures));
            cls->setLessThanFunctions(filter_functions(less, &signatures));
            filter_functions(greaterEquals, &signatures);
            filter_functions(lessEquals, &signatures);
            wasComparable = true;
        }

        if (wasComparable) {
            hide_functions(compare);
            hide_functions(greater);
            hide_functions(greaterEquals);
            hide_functions(less);
            hide_functions(lessEquals);
        }
    }

}

void MetaBuilder::setupConstructorAvailability(MetaClass *meta_class){
    if(meta_class && meta_class->isInterface())
        meta_class = meta_class->extractInterfaceImpl();
    if(meta_class && !meta_class->isNamespace()){
        bool hasSuperClasses = false;
        bool inheritingPublicDefaultConstructor = true;
        bool inheritingPublicCopyConstructor = true;
        bool inheritingPublicMoveConstructor = true;
        bool inheritingPublicDefaultAssignment = true;
        bool inheritingPublicMoveAssignment = true;
        if(meta_class->baseClass()){
            hasSuperClasses = true;
            setupConstructorAvailability(meta_class->baseClass());
            inheritingPublicDefaultConstructor &= meta_class->baseClass()->typeEntry()->hasPublicDefaultConstructor();
            inheritingPublicCopyConstructor &= meta_class->baseClass()->typeEntry()->hasPublicCopyConstructor();
            inheritingPublicMoveConstructor &= meta_class->baseClass()->typeEntry()->hasPublicMoveConstructor();
            inheritingPublicDefaultAssignment &= meta_class->baseClass()->typeEntry()->hasPublicDefaultAssignment();
            inheritingPublicMoveAssignment &= meta_class->baseClass()->typeEntry()->hasPublicMoveAssignment();
        }
        for(MetaClass * ifc : meta_class->interfaces()){
            if(ifc->extractInterfaceImpl()!=meta_class){
                hasSuperClasses = true;
                setupConstructorAvailability(ifc);
                inheritingPublicDefaultConstructor &= ifc->typeEntry()->hasPublicDefaultConstructor();
                inheritingPublicCopyConstructor &= ifc->typeEntry()->hasPublicCopyConstructor();
                inheritingPublicMoveConstructor &= ifc->typeEntry()->hasPublicMoveConstructor();
                inheritingPublicDefaultAssignment &= ifc->typeEntry()->hasPublicDefaultAssignment();
                inheritingPublicMoveAssignment &= ifc->typeEntry()->hasPublicMoveAssignment();
            }
        }
        if(inheritingPublicDefaultConstructor
                && !meta_class->typeEntry()->hasPublicDefaultConstructor()
                && !meta_class->typeEntry()->hasPrivateDefaultConstructor()
                && !meta_class->typeEntry()->hasProtectedDefaultConstructor()
                && meta_class->typeEntry()->isDestructorPublic()){
            meta_class->addDefaultConstructor();
            applyOnType(meta_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicDefaultConstructor();});
        }
        if(inheritingPublicCopyConstructor
                && !meta_class->typeEntry()->hasPublicCopyConstructor()
                && !meta_class->typeEntry()->hasPrivateCopyConstructor()
                && !meta_class->typeEntry()->hasProtectedCopyConstructor()
                && meta_class->typeEntry()->isDestructorPublic()
                && (hasSuperClasses || meta_class->typeEntry()->hasFields())){
            applyOnType(meta_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicCopyConstructor();});
            if(inheritingPublicMoveConstructor
                    && !meta_class->typeEntry()->hasPublicMoveConstructor()
                    && !meta_class->typeEntry()->hasPrivateMoveConstructor()
                    && !meta_class->typeEntry()->hasProtectedMoveConstructor()){
                applyOnType(meta_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicMoveConstructor();});
            }
        }
        if(inheritingPublicDefaultAssignment
                && !meta_class->typeEntry()->hasPublicDefaultAssignment()
                && !meta_class->typeEntry()->hasProtectedDefaultAssignment()
                && !meta_class->typeEntry()->hasPrivateDefaultAssignment()
                && (hasSuperClasses || meta_class->typeEntry()->hasFields())){
            applyOnType(meta_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicDefaultAssignment();});
            if(inheritingPublicMoveAssignment
                    && !meta_class->typeEntry()->hasPublicMoveAssignment()
                    && !meta_class->typeEntry()->hasProtectedMoveAssignment()
                    && !meta_class->typeEntry()->hasPrivateMoveAssignment()){
                applyOnType(meta_class->typeEntry(), [](ComplexTypeEntry* c){c->setHasPublicMoveAssignment();});
            }
        }

        if(meta_class->typeEntry()->hasPublicCopyConstructor()
            && meta_class->typeEntry()->hasPublicDefaultConstructor()
            && meta_class->typeEntry()->hasPublicDefaultAssignment()){
            for(MetaFunction * constructor : meta_class->queryFunctions(MetaClass::Constructors)){
                const MetaArgumentList& arguments = constructor->arguments();
                if(constructor->isConstructor()
                    && !constructor->isCopyConstructor()
                    && constructor->wasPublic()
                    && arguments.size()>=1
                    && !constructor->isTemplate()
                    && !constructor->hasTemplateArgumentTypes()
                    && !constructor->isModifiedRemoved(TS::TargetLangCode)
                ){
                    decltype(arguments.size()) i = 0;
                    bool is1Arg = false;
                    for(decltype(arguments.size()) l = arguments.size(); i < l; ++i){
                        if(!constructor->argumentRemoved(arguments[0]->argumentIndex()+1)
                            && arguments[0]->type()
                            && arguments[0]->type()->indirections().isEmpty()
                            && (arguments[0]->type()->isConstant() || arguments[0]->type()->getReferenceType()!=MetaType::Reference)
                            && constructor->typeReplaced(arguments[0]->argumentIndex() + 1).isEmpty()
                            && !arguments[0]->type()->typeEntry()->isInitializerList()){
                            is1Arg = true;
                            ++i;
                            break;
                        }
                    }
                    for(decltype(arguments.size()) l = arguments.size(); i < l; ++i){
                        if(arguments[i]->defaultValueExpression().isEmpty()){
                            is1Arg = false;
                            break;
                        }
                    }
                    if(is1Arg){
                        if(!constructor->isDeclExplicit()){
                            if(!constructor->isForcedExplicit())
                                meta_class->typeEntry()->addDeclImplicitCast(constructor);
                        }else if(constructor->isForcedImplicit()){
                            meta_class->typeEntry()->addDeclImplicitCast(constructor);
                        }
                    }
                }
            }
        }
    }
}

static void write_reject_log_file(QFile &f, const QMap<QPair<QString,QString>, MetaBuilder::RejectReason> &rejects) {
    if(rejects.isEmpty()){
        f.remove();
        return;
    }else if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ReportHandler::warning(QString("failed to write log file: '%1'")
                               .arg(f.fileName()));
        return;
    }

    QTextStream s(&f);

    decltype(QString().size()) maxSize = 0;
    for (QMap<QPair<QString,QString>, MetaBuilder::RejectReason>::const_iterator it = rejects.constBegin();
            it != rejects.constEnd(); ++it) {
        maxSize = qMax(maxSize, it.key().first.size());
    }

    for (int reason = 0; reason < MetaBuilder::NoReason; ++reason) {
        QString headline;
        switch (reason) {
            case MetaBuilder::NotInTypeSystem:
                headline = "Not in type system";
                break;
            case MetaBuilder::IsPrivate:
                headline = "Is private";
                break;
        case MetaBuilder::IsGlobal:
            headline = "Is in global scope";
            break;
            case MetaBuilder::GenerationDisabled:
                headline = "Generation disabled by type system";
                break;
            case MetaBuilder::RedefinedToNotClass:
                headline = "Type redefined to not be a class";
                break;

            case MetaBuilder::UnmatchedReturnType:
                headline = "Unmatched return type";
                break;

            case MetaBuilder::UnmatchedArgumentType:
                headline = "Unmatched argument type";
                break;

            default:
                headline = "unknown reason";
                break;
        }

        for (QMap<QPair<QString,QString>, MetaBuilder::RejectReason>::const_iterator it = rejects.constBegin();
                it != rejects.constEnd(); ++it) {
            if (it.value() != reason)
                continue;
            if(!headline.isEmpty()){
                s << QString(72, '*') << Qt::endl << headline << Qt::endl;
                headline.clear();
            }
            QString space;
            space.fill(QLatin1Char(' '), maxSize+5-it.key().first.size());
            s << " - " << it.key().first << space << it.key().second << Qt::endl;
        }
        if(headline.isEmpty())
            s << QString(72, '*') << Qt::endl << Qt::endl;
    }

}


void MetaBuilder::dumpLog() {
    {
        QString fileName("mjb_rejected_classes.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_classes);
    }

    {
        QString fileName("mjb_rejected_function_pointers.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_functionals);
    }

    {
        QString fileName("mjb_rejected_enums.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_enums);
    }

    {
        QString fileName("mjb_rejected_functions.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_functions);
    }

    {
        QString fileName("mjb_rejected_template_functions.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_template_functions);
    }

    {
        QString fileName("mjb_rejected_signals.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_signals);
    }

    {
        QString fileName("mjb_rejected_fields.log");
        QFile file(fileName);
        if (!outputDirectory().isNull())
            file.setFileName(QDir(outputDirectory()).absoluteFilePath(fileName));
        write_reject_log_file(file, m_rejected_fields);
    }
}

MetaClassList MetaBuilder::classesTopologicalSorted() const {
    MetaClassList res;

    MetaClassList classes = m_meta_classes;

    QSet<MetaClass*> noDependency;
    QHash<MetaClass*, QSet<MetaClass* >* > hash;
    for(MetaClass *cls : classes) {
        QSet<MetaClass* > *depends = new QSet<MetaClass* >();

        if (cls->baseClass())
            depends->insert(cls->baseClass());

        for(MetaClass *interface : cls->interfaces()) {
            depends->insert(interface);
        }

        if (depends->empty()) {
            noDependency.insert(cls);
        } else {
            hash.insert(cls, depends);
        }
    }

    while (!noDependency.empty()) {
        for(MetaClass *cls : noDependency.values()) {
            if (!cls->isInterface())
                res.append(cls);
            noDependency.remove(cls);
            QHashIterator<MetaClass*, QSet<MetaClass* >* > i(hash);
            while (i.hasNext()) {
                i.next();
                i.value()->remove(cls);
                if (i.value()->empty()) {
                    MetaClass *key = i.key();
                    noDependency.insert(key);
                    hash.remove(key);
                    delete(i.value());
                }
            }
        }
    }

    if (!noDependency.empty() || !hash.empty()) {
        qWarning("dependency graph was cyclic.");
    }

    return res;
}
