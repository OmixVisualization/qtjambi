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

#include "cppheadergenerator.h"

#include <QtCore/QDir>

#include <qdebug.h>

static Indentor INDENT;

CppHeaderGenerator::CppHeaderGenerator(PriGenerator *pri)
    : CppGenerator(pri){
}

QString CppHeaderGenerator::fileNameForClass(const MetaClass *java_class) const {
    if(java_class->typeEntry()->designatedInterface()){
        return fileNameForClass(java_class->enclosingClass());
    }
    return QString("%1_shell.h").arg(java_class->name().replace("$", "_"));
}

QString CppHeaderGenerator::fileNameForFunctional(const MetaFunctional *java_class) const {
    if(java_class->enclosingClass() && !java_class->enclosingClass()->isFake())
        return QString("%1_%2_shell.h").arg(java_class->enclosingClass()->name().replace("$", "_")).arg(java_class->name().replace("$", "_"));
    else
        return QString("%1_shell.h").arg(java_class->name().replace("$", "_"));
}

void CppHeaderGenerator::writeFieldAccessors(QTextStream &s, const MetaField *java_field) {
    Q_ASSERT(java_field->isProtected());

    const MetaFunction *setter = java_field->setter();
    const MetaFunction *getter = java_field->getter();
    const MetaClass *cls = java_field->enclosingClass();
    FieldModification mod = cls->typeEntry()->fieldModification(java_field->name());

    QStringList pps;
    if(!java_field->enclosingClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(java_field->enclosingClass()->typeEntry()->ppCondition())){
        pps << java_field->enclosingClass()->typeEntry()->ppCondition();
    }
    if(!java_field->type()->typeEntry()->ppCondition().isEmpty() && !pps.contains(java_field->type()->typeEntry()->ppCondition())){
        pps << java_field->type()->typeEntry()->ppCondition();
    }
    for(const MetaType* inst : java_field->type()->instantiations()){
        if(!inst->typeEntry()->ppCondition().isEmpty() && !pps.contains(inst->typeEntry()->ppCondition())){
            pps << inst->typeEntry()->ppCondition();
        }
    }
    if(!pps.isEmpty()){
        for (int i=0; i<pps.size(); ++i) {
            if(pps[i].contains("|")){
                pps[i] = "(" + pps[i] + ")";
            }
        }
    }

    bool hasPPS = false;
    bool isWritable = true;
    if(java_field->type()->isConstant()){
        if(java_field->type()->indirections().isEmpty()){
            isWritable = false;
        }else if(java_field->type()->indirections()[0]){
            isWritable = false;
        }
    }else if(!java_field->type()->indirections().isEmpty()
             && java_field->type()->indirections()[0]){
        isWritable = false;
    }
    if (mod.isWritable() && isWritable && setter->wasProtected()){
        if((hasPPS = !pps.isEmpty()))
            s << Qt::endl << "#if " << pps.join(" && ") << Qt::endl;
        writeFunction(s, setter, Option(EnumAsInts | UseNativeIds | JNIProxyFunction));
    }

    if (mod.isReadable() && getter->wasProtected()) {
        if(!hasPPS && (hasPPS = !pps.isEmpty()))
            s << Qt::endl << "#if " << pps.join(" && ") << Qt::endl;
        writeFunction(s, getter, Option(EnumAsInts | UseNativeIds | JNIProxyFunction));
    }
    if(hasPPS)
        s << "#endif //" << pps.join(" && ") << Qt::endl << Qt::endl;
}

void CppHeaderGenerator::write(QTextStream &s, const MetaFunctional *java_class, int) {
#if 0
    {
        QString fileName("written_classes_h.log");
        QFile file(fileName);
        QFile::OpenMode flags(QFile::WriteOnly);
        if(file.exists()){
            flags.setFlag(QFile::Append);
        }
        if (file.open(flags)) {
            QTextStream s(&file);
            s << java_class->typeEntry()->qualifiedCppName() << Qt::endl;
            file.close();
        }
    }
#endif
    QString include_block = java_class->name().replace("$", "_").toUpper() + "_SHELL_H";
    s << "#ifndef " << include_block << Qt::endl
      << "#define " << include_block << Qt::endl << Qt::endl;

    QSet<QString> included;
    {
        IncludeList includes = java_class->typeEntry()->extraIncludes();
        if(java_class->typeEntry()->designatedInterface()){
            includes << java_class->typeEntry()->designatedInterface()->extraIncludes();
        }
        for(const Include& icl : includes){
            if(icl.suppressed)
                writeInclude(s, icl, included);
        }
    }
    writeInclude(s, Include(Include::IncludePath, "QtCore/QtGlobal"), included);
    bool hasDeprecation = java_class->isDeclDeprecated() || (java_class->type() && (java_class->type()->typeEntry()->isDeclDeprecated() || java_class->type()->typeEntry()->isContainer()));
    if(!hasDeprecation){
        for(const MetaArgument* arg : java_class->arguments()){
            if(arg->type()->typeEntry()->isDeclDeprecated() || arg->type()->typeEntry()->isContainer()){
                hasDeprecation = true;
                break;
            }
        }
    }
    if(hasDeprecation){
        writeInclude(s, Include(Include::IncludePath, "QtCore/qcompilerdetection.h"), included);
        s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
          << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl << Qt::endl;
    }
    writeInjectedCode(s, java_class, {CodeSnip::Position1});
    if(java_class->enclosingClass()){
        writeInclude(s, java_class->enclosingClass()->typeEntry()->include(), included);
    }
    writeInclude(s, java_class->typeEntry()->include(), included);
    writeInclude(s, Include(Include::IncludePath, "QtJambi/QtJambiAPI"), included);
    writeInclude(s, Include(Include::IncludePath, "QtJambi/FunctionalBase"), included);
    if(java_class->isFunctionPointer() && !java_class->typeEntry()->getUsing().isEmpty()){
        writeInclude(s, Include(Include::IncludePath, "QtJambi/FunctionPointer"), included);
    }
    IncludeList list = java_class->typeEntry()->extraIncludes();
    std::sort(list.begin(), list.end());
    for(const Include &inc : list) {
        if (inc.type != Include::TargetLangImport)
            writeInclude(s, inc, included);
    }
    s << Qt::endl;

    if(java_class->enclosingClass() && !java_class->enclosingClass()->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << java_class->enclosingClass()->typeEntry()->ppCondition() << Qt::endl;
    }
    writeForwardDeclareSection(s, java_class);
    writeInjectedCode(s, java_class, {CodeSnip::Position2, CodeSnip::Beginning});

    s << Qt::endl
      << "class " << shellClassName(java_class) << " final : public FunctionalBase" << Qt::endl
      << "{" << Qt::endl;
    writeInjectedCode(s, java_class, {CodeSnip::Beginning});
    s << "public:" << Qt::endl
      << "    " << shellClassName(java_class) << "();" << Qt::endl
      << "    ~" << shellClassName(java_class) << "() override;" << Qt::endl
      << "    void getFunctional(JNIEnv*, void*) override;" << Qt::endl
      << "    static void operator delete(void * ptr) noexcept;" << Qt::endl << Qt::endl
      << "    class Functor final : private FunctorBase{" << Qt::endl
      << "    public:" << Qt::endl
      << "        Functor(const Functor& functor);" << Qt::endl
      << "        ~Functor() override;" << Qt::endl
      << "        ";
    if(java_class->type()){
        writeTypeInfo(s, java_class->type(), Option());
    }else{
        s << "void";
    }
    s << " operator()(";
    int counter = 0;
    for(MetaArgument * arg : java_class->arguments()){
        if(counter>0)
            s << ", ";
        writeTypeInfo(s, arg->type(), Option());
        s << arg->indexedName();
        ++counter;
    }
    s << ");" << Qt::endl
      << "    private:" << Qt::endl
      << "        Functor(" << shellClassName(java_class) << "& functional);" << Qt::endl
      << "        friend class " << shellClassName(java_class) << ";" << Qt::endl
      << "    };" << Qt::endl << Qt::endl
      << "    static jobject resolveFunctional(JNIEnv *, const " << java_class->typeEntry()->qualifiedCppName() << "*);" << Qt::endl << Qt::endl
      << "private:" << Qt::endl
      << "    QtJambiShell* __shell() const override final;" << Qt::endl
      << "    friend class " << shellClassName(java_class) << "::Functor;" << Qt::endl;
    if(java_class->isFunctionPointer()){
        s << "    std::function<void()> m_functionPointerDeleter;" << Qt::endl
          << "    static std::function<const Functor*(" << java_class->typeEntry()->qualifiedCppName() << ")> reverseFunctionGetter;" << Qt::endl;
    }
    writeInjectedCode(s, java_class, {CodeSnip::Position3, CodeSnip::End});

    s  << "};" << Qt::endl << Qt::endl;

    writeInjectedCode(s, java_class, {CodeSnip::Position4});


    if(java_class->enclosingClass() && !java_class->enclosingClass()->typeEntry()->ppCondition().isEmpty()){
        s << "#endif // " << java_class->enclosingClass()->typeEntry()->ppCondition() << Qt::endl;
    }
    s  << "#endif // " << include_block << Qt::endl;

    QString pro_file_name = priGenerator->subDirectoryForClass(java_class, PriGenerator::CppDirectory) + "/generated.pri";

    priGenerator->addHeader(pro_file_name, fileNameForFunctional(java_class));
}

void CppHeaderGenerator::write(QTextStream &s, const MetaClass *java_class, int) {
#if 0
    {
        QString fileName("written_classes_h.log");
        QFile file(fileName);
        QFile::OpenMode flags(QFile::WriteOnly);
        if(file.exists()){
            flags.setFlag(QFile::Append);
        }
        if (file.open(flags)) {
            QTextStream s(&file);
            s << java_class->typeEntry()->qualifiedCppName() << Qt::endl;
            file.close();
        }
    }
#endif
    QString include_block = java_class->name().replace("$", "_").toUpper() + "_SHELL_H";

    s << "#ifndef " << include_block << Qt::endl
      << "#define " << include_block << Qt::endl << Qt::endl;

    QSet<QString> included;
    {
        IncludeList includes = java_class->typeEntry()->extraIncludes();
        if(java_class->typeEntry()->designatedInterface()){
            includes << java_class->typeEntry()->designatedInterface()->extraIncludes();
        }
        for(const Include& icl : qAsConst(includes)){
            if(icl.suppressed)
                writeInclude(s, icl, included);
        }
    }
    writeInclude(s, Include(Include::IncludePath, "QtCore/QtGlobal"), included);

    QList<const MetaFunction *> returnScopeRequired;
    for(const MetaFunction *function : java_class->functionsInShellClass()) {
        if((!function->isFinalInCpp() || function->isVirtualSlot()) && !function->wasPrivate() && function->needsReturnScope())
            returnScopeRequired << function;
    }

    if(!returnScopeRequired.isEmpty())
        writeInclude(s, Include(Include::IncludePath, "memory"), included);

    bool hasDeprecation = java_class->isDeclDeprecated();
    if(!hasDeprecation){
        for(const MetaFunction* f : java_class->functions()){
            if(!f->wasPrivate()){
                if(f->isDeclDeprecated()){
                    hasDeprecation = true;
                    break;
                }else{
                    if(f->type() && (f->type()->typeEntry()->isDeclDeprecated() || f->type()->typeEntry()->isContainer())){
                        hasDeprecation = true;
                        break;
                    }
                    for(const MetaArgument* arg : f->arguments()){
                        if(arg->type()->typeEntry()->isDeclDeprecated() || arg->type()->typeEntry()->isContainer()){
                            hasDeprecation = true;
                            break;
                        }
                    }
                    if(hasDeprecation)
                        break;
                }
            }
        }
    }
    if(hasDeprecation){
        writeInclude(s, Include(Include::IncludePath, "QtCore/qcompilerdetection.h"), included);
        s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
          << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl << Qt::endl;
    }
    writeInjectedCode(s, java_class, {CodeSnip::Position1});

    if(java_class->enclosingClass()){
        writeInclude(s, java_class->enclosingClass()->typeEntry()->include(), included);
    }

    writeInclude(s, java_class->typeEntry()->include(), included);

    writeInclude(s, Include(Include::IncludePath, "QtJambi/QtJambiAPI"), included);

    IncludeList list = java_class->typeEntry()->extraIncludes();
    std::sort(list.begin(), list.end());
    for(const Include &inc : list) {
        if (inc.type == Include::TargetLangImport)
            continue;
        writeInclude(s, inc, included);
    }
    if(java_class->hasPaintMethod())
        writeInclude(s, Include(Include::IncludePath, "QtJambi/AboutToPaint"), included);
//    if(java_class->hasQmlListProperty())
//        writeInclude(s, Include(Include::IncludePath, "QtJambi/QmlAPI"), included);


    writeInclude(s, Include(Include::IncludePath, "QtJambi/RegistryAPI"), included);
    s << Qt::endl;

    if(!java_class->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl;
    }

    writeForwardDeclareSection(s, java_class);
    writeInjectedCode(s, java_class, {CodeSnip::Position2, CodeSnip::Beginning});

    if (java_class->generateShellClass()) {
        s << Qt::endl
          << "class " << shellClassName(java_class)
          << " : public " << java_class->qualifiedCppName();
        if(java_class->hasVirtualDestructor())
            s << ", public QtJambiShellInterface";
        s << Qt::endl
          << "{" << Qt::endl;
        writeInjectedCode(s, java_class, {CodeSnip::Beginning});
        s << "public:" << Qt::endl;
        if(!java_class->typeEntry()->isDestructorPrivate() && java_class->instantiateShellClass()){
            for(const MetaFunction *function : java_class->functions()) {
                if (function->isConstructor() && !function->wasPrivate()){
                    QStringList ppConditions;
                    for(const MetaArgument *argument : function->arguments()) {
                        if(function->argumentRemoved(argument->argumentIndex()+1)!=ArgumentRemove_No){
                            if(!argument->type()->typeEntry()->ppCondition().isEmpty() && !ppConditions.contains(argument->type()->typeEntry()->ppCondition())){
                                ppConditions << argument->type()->typeEntry()->ppCondition();
                            }
                            for(const MetaType* inst : argument->type()->instantiations()){
                                if(!inst->typeEntry()->ppCondition().isEmpty() && !ppConditions.contains(inst->typeEntry()->ppCondition())){
                                    ppConditions << inst->typeEntry()->ppCondition();
                                }
                            }
                        }
                    }
                    if(!java_class->typeEntry()->ppCondition().isEmpty()){
                        ppConditions.removeAll(java_class->typeEntry()->ppCondition());
                    }
                    if(!ppConditions.isEmpty()){
                        for (int i=0; i<ppConditions.size(); ++i) {
                            if(ppConditions[i].contains("||")){
                                ppConditions[i] = "(" + ppConditions[i] + ")";
                            }
                        }
                        s << Qt::endl << "#if " << ppConditions.join(" && ") << Qt::endl << Qt::endl;
                    }
                    Option option = Option(SkipRemovedArguments);
                    //if(java_class->isQObject()){
                        option = Option(option | IncludeDefaultExpression);
                    //}
                    if(!function->isPublic())
                        option = Option(option | EnumAsInts);
                    writeFunction(s, function, option);
                    if(!ppConditions.isEmpty()){
                        s << Qt::endl << "#endif //" << ppConditions.join(" && ") << Qt::endl << Qt::endl;
                    }
                }
            }

            s << "    ~" << shellClassName(java_class) << "()";
            if(java_class->hasVirtualDestructor()){
                s << " override";
            }
            s << ";" << Qt::endl;
        }
        s << Qt::endl;

        QList<const MetaFunction *> functionsInTargetLang;
        QList<const MetaFunction *> signalsInTargetLang;

        for(MetaFunction* signal : java_class->cppSignalFunctions()){
            if (signal->declaringClass() == java_class && !signal->hasTemplateArgumentTypes())
                signalsInTargetLang << signal;
        }

        for(MetaFunction *function : java_class->functionsInTargetLang()) {
            if (
                (!function->isConstructor() ||
                 !java_class->hasUnimplmentablePureVirtualFunction()) &&
                 !function->isEmptyFunction() &&
                 !function->hasTemplateArgumentTypes()
                )
                functionsInTargetLang << function;
        }
        for(MetaFunction *function : java_class->queryFunctions(MetaClass::NormalFunctions
                                                                           | MetaClass::AbstractFunctions
                                                                           | MetaClass::NotRemovedFromTargetLang)) {
            if (function->implementingClass() != java_class &&
                    !function->hasTemplateArgumentTypes() ) {
                functionsInTargetLang << function;
            }
        }

        // All functions in original class that should be reimplemented in shell class
        QList<const MetaFunction *> privatePureVirtuals;
        for(const MetaFunction *function : java_class->functionsInShellClass()) {
            if(function->isFinalInCpp() && !function->isVirtualSlot())
                continue;
            if(function->wasPrivate()){
                if(function->isAbstract())
                    privatePureVirtuals << function;
            }else{
                writeFunction(s, function);
            }
        }

        const MetaClass* interfaceClass = nullptr;
        for(const MetaClass* iface : java_class->interfaces()){
            ImplementorTypeEntry * origin = static_cast<const InterfaceTypeEntry *>(iface->typeEntry())->origin();
            if(origin==java_class->typeEntry()){
                interfaceClass = iface;
                break;
            }
        }
        if(java_class->instantiateShellClass()){
            s << "    static void operator delete(void * ptr) noexcept;" << Qt::endl;
            if (java_class->isQObject()) {
                if(!java_class->hasMetaObjectFunction())
                    s << "    const QMetaObject *metaObject() const override final;" << Qt::endl;
                if(!java_class->hasMetaCastFunction())
                    s << "    void *qt_metacast(const char *) override final;" << Qt::endl;
                if(!java_class->hasMetaCallFunction())
                    s << "    int qt_metacall(QMetaObject::Call, int, void **) override final;" << Qt::endl;
            }
        }

        s << "private:" << Qt::endl;
        for(const MetaFunction *function : qAsConst(privatePureVirtuals)) {
            writeFunction(s, function);
        }
        for(const MetaFunction *function : qAsConst(returnScopeRequired)) {
            s << "    ";
            if(function->isConstant())
                s << "mutable ";
            s << "std::unique_ptr<QtJambiScope> __returnScope__" << function->name();
            if(function->isConstant())
                s << "_const";
            s << ";" << Qt::endl;
        }
        if(java_class->instantiateShellClass()){
            s << "    QtJambiShell* __shell() const";
            if(java_class->hasVirtualDestructor()){
                s << " override final";
            }
            s << ";" << Qt::endl;
            s << "    jmethodID __shell_javaMethod(int pos) const;" << Qt::endl;
        }

        //writeVariablesSection(s, java_class, interfaceClass!=nullptr);
        writeInjectedCode(s, java_class, {CodeSnip::Position3, CodeSnip::End});

        s  << "};" << Qt::endl << Qt::endl;

        QList<MetaEnum *> protectedEnums;
        for(MetaEnum *cpp_enum : java_class->enums()){
            if(cpp_enum->isProtected() || !java_class->isPublic()){
                protectedEnums << cpp_enum;
            }
        }
        if(interfaceClass){
            for(MetaEnum *cpp_enum : interfaceClass->enums()){
                if(cpp_enum->isProtected() || !interfaceClass->isPublic()){
                    protectedEnums << cpp_enum;
                }
            }
        }

        bool needsAccess = !protectedEnums.isEmpty() || !java_class->publicOverrideFunctions().isEmpty() || !java_class->virtualOverrideFunctions().isEmpty();
        if(!needsAccess){
            for(const MetaField *field : java_class->fields()) {
                if (field->wasProtected()){
                    FieldModification mod = java_class->typeEntry()->fieldModification(field->name());
                    if(mod.isReadable()){
                        needsAccess = true;
                        break;
                    }
                    bool isWritable = true;
                    if(field->type()->isConstant()){
                        if(field->type()->indirections().isEmpty()){
                            isWritable = false;
                        }else if(field->type()->indirections()[0]){
                            isWritable = false;
                        }
                    }else if(!field->type()->indirections().isEmpty()
                               && field->type()->indirections()[0]){
                        isWritable = false;
                    }
                    if (mod.isWritable() && isWritable) {
                        needsAccess = true;
                        break;
                    }
                }
            }
        }

        if(needsAccess && java_class->generateShellClass()){
            s << "struct " << (java_class->typeEntry()->designatedInterface() ? java_class->extractInterface()->name() : java_class->name()) << "_access"
              << " : public " << java_class->qualifiedCppName() << Qt::endl
              << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                // Public call throughs for protected functions
                for(const MetaFunction *function : java_class->publicOverrideFunctions()) {
                    if((functionsInTargetLang.contains(function) || signalsInTargetLang.contains(function))
                            && !function->isProxyCall())
                        writeFunctionOverride(s, function, "__qt_");
                }

                // Override all virtual functions to get the decision on static/virtual call
                for(const MetaFunction *function : java_class->virtualOverrideFunctions()) {
                    if(!function->hasTemplateArgumentTypes()
                            && !function->isRemovedFrom(java_class, TS::TargetLangCode)
                            && !function->isRemovedFrom(function->declaringClass(), TS::TargetLangCode)
                            && !function->isModifiedRemoved(TS::NativeCode)
                            && !function->wasPrivate()
                            && (function->implementingClass()==java_class
                                || function->implementingClass()==java_class->extractInterface()
                                || ( (java_class->isAbstract() || java_class->isInterface()) && function->isAbstract() ))
                    ){
                        writeFunctionOverride(s, function, "__qt_");
                    }
                }

                // Field accessors
                for(const MetaField *field : java_class->fields()) {
                    if (field->wasProtected())
                        writeFieldAccessors(s, field);
                }

                for(MetaEnum * cpp_enum : protectedEnums){
                    s << INDENT << "static inline const std::type_info& __registerEnumTypeInfo_" << cpp_enum->name().replace("::", "_") << "() {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        const EnumTypeEntry *entry = cpp_enum->typeEntry();
                        const QString qtEnumName = entry->qualifiedCppName();
                        const QString javaEnumName = [java_class,entry]()->QString{
                            if(java_class){
                                if(java_class->typeEntry()->isGlobal()){
                                    return entry->qualifiedTargetLangName().replace(".", "/");
                                }else{
                                    if(java_class->typeEntry()->javaPackage().isEmpty()){
                                        return java_class->typeEntry()->targetLangName() + "$" + entry->targetLangName();
                                    }else{
                                        return java_class->typeEntry()->javaPackage().replace(".", "/") + "/" + java_class->typeEntry()->targetLangName() + "$" + entry->targetLangName();
                                    }
                                }
                            }else{
                                return entry->targetLangName();
                            }
                        }();
                        if(const FlagsTypeEntry * fentry = cpp_enum->typeEntry()->flags()){
                            const QString qtFlagName = fentry->qualifiedCppName();
                            const QString javaFlagName = [java_class,fentry]()->QString{
                                if(java_class){
                                    if(java_class->typeEntry()->isGlobal()){
                                        return fentry->targetLangName().replace(".", "/");
                                    }else{
                                        if(java_class->typeEntry()->javaPackage().isEmpty()){
                                            return java_class->typeEntry()->targetLangName() + "$" + fentry->targetLangName();
                                        }else{
                                            return java_class->typeEntry()->javaPackage().replace(".", "/") + "/" + java_class->typeEntry()->targetLangName() + "$" + fentry->targetLangName();
                                        }
                                    }
                                }else{
                                    return fentry->targetLangName();
                                }
                            }();
                            QString qtFlagsTemplateName = fentry->flagsTemplate();
                            s << INDENT << "return RegistryAPI::registerEnumTypeInfo" << (entry->isHiddenMetaObject() ? "NoMetaObject" : "") << "<" << qtEnumName << ">(\"" << qtEnumName << "\", \"" << javaEnumName << "\", \"" << qtFlagName << "\", \"" << qtFlagsTemplateName << "\", \"" << javaFlagName << "\");" << Qt::endl;
                        }else{
                            s << INDENT << "return RegistryAPI::registerEnumTypeInfo" << (entry->isHiddenMetaObject() ? "NoMetaObject" : "") << "<" << qtEnumName << ">(\"" << qtEnumName << "\", \"" << javaEnumName << "\");" << Qt::endl;
                        }
                    }
                    s << INDENT << "}" << Qt::endl;
                }
            }
            s  << "};" << Qt::endl << Qt::endl;
        }

        writeInjectedCode(s, java_class, {CodeSnip::Position4});
    }

    if(!java_class->typeEntry()->ppCondition().isEmpty()){
        s << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl;
    }

    s  << "#endif // " << include_block << Qt::endl;

    QString pro_file_name = priGenerator->subDirectoryForClass(java_class, PriGenerator::CppDirectory) + "/generated.pri";

    priGenerator->addHeader(pro_file_name, fileNameForClass(java_class));
}


/*!
    Writes out declarations of virtual C++ functions so that they
    can be reimplemented from the java side.
*/
void CppHeaderGenerator::writeFunction(QTextStream &s, const MetaFunction *java_function, Option options) {
    if (java_function->isModifiedRemoved(TS::ShellCode))
        return;
    if (java_function->hasTemplateArgumentTypes())
        return;
    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty()){
        s << Qt::endl << "#if " << pps.join(" && ") << Qt::endl;
    }
    s << "    ";
    writeFunctionSignature(s, java_function, nullptr, QString(), Option(OriginalName | ShowStatic | ShowOverride | FunctionOverride | options));
    s << ";" << Qt::endl;
    if(!pps.isEmpty()){
        s << "#endif //" << pps.join(" && ") << Qt::endl << Qt::endl;
    }
}

void CppHeaderGenerator::writeFunctionOverride(QTextStream &s,
        const MetaFunction *java_function, const QString& prefix) {
    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty()){
        s << Qt::endl << "#if " << pps.join(" && ") << Qt::endl;
    }
    s << "    ";
    writeFunctionSignature(s, java_function, nullptr, prefix, Option(EnumAsInts | JNIProxyFunction | ShowStatic | UseNativeIds | UnderscoreSpaces | SkipRemovedArguments));
    s << ";" << Qt::endl;
    if(!pps.isEmpty()){
        s << "#endif //" << pps.join(" && ") << Qt::endl << Qt::endl;
    }
}


void CppHeaderGenerator::writeForwardDeclareSection(QTextStream &s, const MetaClass *java_class) {
    CodeSnipList code_snips = java_class->typeEntry()->codeSnips();
    for(const CodeSnip &cs : code_snips) {
        if (cs.language == TS::ShellCode) {
            s << cs.code() << Qt::endl;
        }
    }
}

void CppHeaderGenerator::writeForwardDeclareSection(QTextStream &s, const MetaFunctional *java_class) {
    CodeSnipList code_snips = java_class->typeEntry()->codeSnips();
    for(const CodeSnip &cs : code_snips) {
        if (cs.language == TS::ShellCode) {
            s << cs.code() << Qt::endl;
        }
    }
}


void CppHeaderGenerator::writeVariablesSection(QTextStream &, const MetaClass *, bool) {
}

void CppHeaderGenerator::writeInjectedCode(QTextStream &s, const MetaClass *java_class, const QList<CodeSnip::Position>& positions) {
    CodeSnipList code_snips = java_class->typeEntry()->codeSnips();
    for(const CodeSnip &cs : code_snips) {
        if (positions.contains(cs.position) && cs.language == TS::ShellDeclaration) {
            s << cs.code() << Qt::endl;
        }
    }
}

void CppHeaderGenerator::writeInjectedCode(QTextStream &s, const MetaFunctional *java_class, const QList<CodeSnip::Position>& positions) {
    CodeSnipList code_snips = java_class->typeEntry()->codeSnips();
    for(const CodeSnip &cs : code_snips) {
        if (positions.contains(cs.position) && cs.language == TS::ShellDeclaration) {
            s << cs.code() << Qt::endl;
        }
    }
}

bool CppHeaderGenerator::shouldGenerateHeaders(const MetaClass *java_class){
    return (java_class->generateShellClass()
            && shouldGenerateCpp(java_class)
            && !(java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell))
           /*|| (  !java_class->isFake()
               && !java_class->isInterface()
               && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                 && !(java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell))*/;
}

bool CppHeaderGenerator::shouldGenerate(const MetaClass *java_class) const {
    return shouldGenerateHeaders(java_class);
}

bool CppHeaderGenerator::shouldGenerate(const MetaFunctional *functional) const {
    return (!(functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)
                && CppGenerator::shouldGenerate(functional));
}
