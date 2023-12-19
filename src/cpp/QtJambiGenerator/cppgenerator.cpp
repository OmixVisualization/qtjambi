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

#include "cppgenerator.h"
#include "reporthandler.h"
#include "metabuilder.h"

#include "typesystem/typedatabase.h"

// ### There's a bug in Qt causing it to fail at normalizing signatures
// on the form FooBar<T> const&, which is the form the C++ parser uses
// for all types, so connections between Java and C++ with const& templates
// will fail. This is a work around which is only needed until that bug is fixed.
// Since Qt works correctly with const FooBar<T> &, we simply change the
// signature to that.
QString CppGenerator::fixNormalizedSignatureForQt(const QString &signature) {
    QString ret = signature;
    if (signature.contains("<") && signature.endsWith("const&")) {
        ret = "const "
              + signature.mid(0, signature.size() - 6)
              + "&";
    }
    return ret;
}

CppGenerator::CppGenerator(PriGenerator *_priGenerator) : AbstractGenerator(), priGenerator(_priGenerator) {}

QString CppGenerator::subDirectoryForPackage(const QString &package) const {
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(database()->findType(package));
    if(typeSystemEntry && !typeSystemEntry->qtLibrary().isEmpty()){
        if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
            QString libName = typeSystemEntry->qtLibrary();
            return "QtJambi" + libName.mid(2);
        }else{
            return typeSystemEntry->qtLibrary();
        }
    }else if(typeSystemEntry && !typeSystemEntry->targetName().isEmpty()){
        return typeSystemEntry->targetName();
    }else
        return QString(package).replace(".", "_");
}
QString CppGenerator::translateType(const MetaType *java_type, Option option) {
    if (!java_type)
        return "void";

    if (java_type->hasNativeId() && (option & UseNativeIds)){
        return "QtJambiNativeID";
    }else if (java_type->isCharString()){
        return "jstring";
    }else if (java_type->isPrimitive()
            || java_type->isQChar()
            || java_type->isQString()
            || java_type->isQStringRef()
            || java_type->isQVariant()
            || java_type->isJObjectWrapper()
            || java_type->isQChar()) {
        return java_type->typeEntry()->jniName();
    } else if (java_type->isArray()){
        if(java_type->typeEntry()->isArray())
            return java_type->typeEntry()->jniName();
        else{
            QString jniName = java_type->typeEntry()->jniName();
            if(jniName=="void"
                    || jniName=="jstring"
                    || jniName=="jthrowable"
                    || jniName=="jclass"
                    || jniName=="jobject"
                    || jniName.endsWith("Array"))
                return "jobjectArray";
            return jniName + "Array";
        }
    } else if (java_type->isInitializerList()){
        Q_ASSERT(java_type->instantiations().size()==1);
        if(java_type->instantiations().first()->isPrimitive()){
            QString jniName = java_type->instantiations().first()->typeEntry()->jniName();
            if(jniName=="void"
                    || jniName=="jstring"
                    || jniName=="jthrowable"
                    || jniName=="jclass"
                    || jniName=="jobject"
                    || jniName.endsWith("Array"))
                return "jobjectArray";
            return jniName + "Array";
        }else{
            return java_type->typeEntry()->jniName();
        }
    } else if (java_type->isIntegerEnum() || java_type->isIntegerFlags()
               || ((option & EnumAsInts)
                   && (java_type->isEnum() || java_type->isFlags())
                   && java_type->indirections().isEmpty()
                   && (java_type->getReferenceType()!=MetaType::Reference || java_type->isConstant()))) {
        uint size = 32;
        if(java_type->isEnum()){
            const EnumTypeEntry* eentry = reinterpret_cast<const EnumTypeEntry*>(java_type->typeEntry());
            if(eentry){
                size = eentry->size();
            }
        }
        switch(size){
        case 8: return "jbyte";
        case 16: return "jshort";
        case 64: return "jlong";
        default: break;
        }
        return "jint";
    } else if (java_type->isQString()
               || java_type->isCharString()
               || java_type->isQStringRef()
               || java_type->isQStringView()
               || java_type->isQAnyStringView()
               || java_type->isQUtf8StringView()
               || java_type->isQLatin1String()
               || java_type->isQLatin1StringView()
               || (java_type->typeEntry()->qualifiedTargetLangName()=="java.lang.String" && java_type->indirections().isEmpty())){
        return "jstring";
    } else {
        return "jobject";
    }
}

void writeTypeInfo(QTextStream &s, const MetaType *type){
    CppGenerator::writeTypeInfo(s, type, CppGenerator::SkipName);
}

void CppGenerator::writeTypeInfo(QTextStream &s, const MetaType *type, Option options) {
    if(type->typeEntry()->isComplex()){
        const ComplexTypeEntry *ctype = reinterpret_cast<const ComplexTypeEntry *>(type->typeEntry());
        if(ctype->isGenericClass() && type->instantiations().isEmpty()){
            s << "auto";
            switch(type->getReferenceType()){
            case MetaType::Reference:
                s << "&";
                break;
            case MetaType::RReference:
                s << "&&";
                break;
            default: break;
            }
            if (!(options & SkipName))
                s << ' ';
            return;
        }
    }
    if ((options & OriginalTypeDescription) && !type->originalTypeDescription().isEmpty()) {
        QString originalTypeDescription = type->originalTypeDescription();

        if (options & NormalizeAndFixTypeSignature) {
            originalTypeDescription = QMetaObject::normalizedSignature(originalTypeDescription.toLatin1().constData());
            originalTypeDescription = fixNormalizedSignatureForQt(originalTypeDescription);
        }
        if (originalTypeDescription.contains("qreal")){ // map generator type to qreal type
            s << originalTypeDescription;
            if (!(options & SkipName))
                s << ' ';
            return;
        }else{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int mtype = QMetaType::type(originalTypeDescription.toLatin1().constData());
            if(mtype>QMetaType::UnknownType && mtype<=QMetaType::HighestInternalId){
                originalTypeDescription = QMetaType::typeName(mtype);
            }
#else
            QMetaType mtype = QMetaType::fromName(originalTypeDescription.toLatin1().constData());
            if(mtype.id()>QMetaType::UnknownType && mtype.id()<=QMetaType::HighestInternalId){
                originalTypeDescription = mtype.name();
            }
#endif
            s << originalTypeDescription;
            if (!(options & SkipName))
                s << ' ';
            return;
        }
    }

    if (type->isArray()) {
        writeTypeInfo(s, type->arrayElementType(), Option(options & ~ForceConstReference));
        if (options & ArrayAsPointer) {
            s << "*";
        } else if (!(options & SkipArray)) {
            s << "[" << type->arrayElementCount() << "]";
        }
        return;
    }

    const TypeEntry *te = type->typeEntry();

    if(type->isConstant() && (options & FunctionOverride) == FunctionOverride){
        s << "const ";
    }else if ( (type->isConstant()
            && !(options & ExcludeConst))
         || ((options & ForceConstReference) == ForceConstReference
               && type->getReferenceType()==MetaType::NoReference
               && type->indirections().isEmpty()
               && !type->isPrimitive()
               && !type->isEnum()
               && !type->isFlags()
             )){
        s << "const ";
    }
    if(type->isVolatile()){
        s << "volatile ";
    }

    if ((options & EnumAsInts) && (te->isEnum() || te->isFlags()) && type->indirections().isEmpty() && (type->getReferenceType()!=MetaType::Reference || type->isConstant())) {
        uint size = 32;
        if(te->isEnum()){
            size = static_cast<const EnumTypeEntry*>(te)->size();
        }
        s << "qint" << size;
    } else if (type->instantiations().size() > 0
               && (!type->isContainer()
               || (
                   (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::StringListContainer
               &&  (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::ByteArrayListContainer
                  )
               )
           ) {
        QString qualifiedCppName = te->qualifiedCppName();
        if(qualifiedCppName.endsWith('>')){
            auto idx = qualifiedCppName.indexOf('<');
            if(idx>0)
                qualifiedCppName = qualifiedCppName.mid(0, idx).trimmed();
        }
        s << qualifiedCppName;
        s << '<';
        const QList<const MetaType *>& args = type->instantiations();
        bool nested_template = false;
        for (int i = 0; i < args.size(); ++i) {
            if (i != 0)
                s << ", ";
            nested_template |= args.at(i)->isContainer();
            writeTypeInfo(s, args.at(i), SkipName);
        }
        if (nested_template)
            s << ' ';
        s << '>';
    } else {
        s << te->qualifiedCppName();
    }

    if(type->typeEntry()->isTemplateArgument() && reinterpret_cast<const TemplateArgumentEntry *>(type->typeEntry())->isVariadic()){
        s << "...";
    }

    for(int i=0; i<type->indirections().size(); i++){
        if(type->indirections()[i]){
            s << "*const ";
        }else{
            s << '*';
        }
    }

    if((options & FunctionOverride) == FunctionOverride){
        switch(type->getReferenceType()){
        case MetaType::Reference:
            s << "&";
            break;
        case MetaType::RReference:
            s << "&&";
            break;
        default: break;
        }
    }else{
        if(!(options & ExcludeReference)){
            switch(type->getReferenceType()){
            case MetaType::Reference:
                s << "&";
                break;
            case MetaType::RReference:
                s << "&&";
                break;
            default:
                if((options & ForceConstReference) == ForceConstReference
                                 && !(options & FunctionOverride)
                                 && type->indirections().isEmpty()
                                 && !type->isPrimitive()
                                 && !type->isEnum()
                                 && !type->isFlags()){
                    s << "&";
                }
                break;
            }
        }
    }

    if (!(options & SkipName))
        s << ' ';
}

QByteArray CppGenerator::jniName(const QString &name) const{
    if(name=="void")
        return "void";
    else if(name=="int")
        return "jint";
    else if(name=="long")
        return "jlong";
    else if(name=="short")
        return "jshort";
    else if(name=="byte")
        return "jbyte";
    else if(name=="double")
        return "jdouble";
    else if(name=="float")
        return "jfloat";
    else if(name=="boolean")
        return "jboolean";
    else if(name=="char")
        return "jchar";
    else if(name=="int[]" || name=="int...")
        return "jintArray";
    else if(name=="long[]" || name=="long...")
        return "jlongArray";
    else if(name=="short[]" || name=="short...")
        return "jshortArray";
    else if(name=="byte[]" || name=="byte...")
        return "jbyteArray";
    else if(name=="double[]" || name=="double...")
        return "jdoubleArray";
    else if(name=="float[]" || name=="float...")
        return "jfloatArray";
    else if(name=="boolean[]" || name=="boolean...")
        return "jbooleanArray";
    else if(name=="char[]" || name=="char...")
        return "jcharArray";
    else if(name.endsWith("[]") || name.endsWith("..."))
        return "jobjectArray";
    else if(name=="java.lang.String" || name=="String")
        return "jstring";
    else if(name.startsWith("java.lang.Class"))
        return "jclass";
    TypeEntry *entry = database()->findType(name);
    if (entry)
        return entry->jniName().toLatin1();
    else
        return "jobject";
}

void CppGenerator::writeFunctionArguments(QTextStream &s,
        const MetaArgumentList &arguments,
        const MetaFunction *java_function,
        Option option,
        int numArguments) const{
    if (numArguments < 0) numArguments = int(arguments.size());

    bool needComma = false;
    for (int i = 0; i < numArguments; ++i) {
        if (needComma)
            s << ", ";
        else
            needComma = true;
        MetaArgument *arg = arguments.at(i);
        if(option & JNIProxyFunction){
            QString typeReplaced = java_function->typeReplaced(arg->argumentIndex() + 1);
            bool addArrayOffset = false;
            if(!typeReplaced.isEmpty())
                s << jniName(annotationFreeTypeName(typeReplaced));
            else{
                if(java_function->useArgumentAsBuffer(arg->argumentIndex() + 1)){
                    s << "jobject";
                }else if(java_function->useArgumentAsArray(arg->argumentIndex() + 1)){
                    if(arg->type()->typeEntry()->isPrimitive()){
                        if(arg->type()->typeEntry()->targetLangName()=="int"){
                            s << "jintArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="long"){
                            s << "jlongArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="short"){
                            s << "jshortArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="byte"){
                            s << "jbyteArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="boolean"){
                            s << "jbooleanArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="char"){
                            s << "jcharArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="float"){
                            s << "jfloatArray";
                        }else if(arg->type()->typeEntry()->targetLangName()=="double"){
                            s << "jdoubleArray";
                        }
                    }else if(arg->type()->typeEntry()->isQChar()){
                        s << "jcharArray";
                    }else{
                        s << "jobjectArray";
                    }
                    addArrayOffset = java_function->insertArrayOffsetArgument(arg->argumentIndex() + 1);
                }else{
                    s << translateType(arg->type(), Option(option & ~JNIProxyFunction));
                }
            }
            if (!(option & SkipName)){
                s << " " << arg->indexedName();
            }
            if(addArrayOffset){
                s << ", ";
                int lengthParameter = java_function->arrayOrBufferLengthIndex(arg->argumentIndex() + 1);
                if(lengthParameter>0 && lengthParameter<=arguments.size()){
                    const MetaArgument *lengthParam = arguments[lengthParameter - 1];
                    if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                        lengthParam = nullptr;
                        for(const MetaArgument *argument : arguments) {
                            if(argument && argument->argumentIndex()+1==lengthParameter){
                                lengthParam = argument;
                            }
                        }
                    }
                    if(lengthParam && java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                        typeReplaced = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                        if(!typeReplaced.isEmpty())
                            s << jniName(annotationFreeTypeName(typeReplaced));
                        else
                            s << translateType(lengthParam->type(), Option(option & ~JNIProxyFunction));
                        if (!(option & SkipName)){
                            s << " __qt_offset_of_" << arg->indexedName();
                        }
                    }
                }
            }
        }else {
            if(arg->type()->isArray() && !(option & SkipName)){
                option = Option(option | SkipArray);
            }
            if(arg->type()->originalTypeDescription().startsWith("const ::")
                    || arg->type()->originalTypeDescription().startsWith("::")){
                writeTypeInfo(s, arg->type(), Option(option | OriginalTypeDescription));
            }else{
                writeTypeInfo(s, arg->type(), option);
            }
            if (!(option & SkipName)){
                s << arg->indexedName();
                if(arg->type()->isArray()){
                    s << "[" << arg->type()->arrayElementCount() << "]";
                }
            }
            if ((option & IncludeDefaultExpression) && !arg->originalDefaultValueExpression().isEmpty()) {
                s << " = ";

                QString expr = arg->originalDefaultValueExpression();
                if(expr!="{}"){
                    if (arg->type()->typeEntry()->isEnum() && expr.indexOf("::") < 0)
                        s << static_cast<const EnumTypeEntry *>(arg->type()->typeEntry())->qualifier() << "::";
                    if (arg->type()->typeEntry()->isFlags() && expr.indexOf("::") < 0)
                        s << static_cast<const FlagsTypeEntry *>(arg->type()->typeEntry())->originator()->qualifier() << "::";
                }
                s << expr;
            }
        }
    }
}

/*!
 * Writes the function \a java_function signature to the textstream \a s.
 *
 * The \a name_prefix can be used to give the function name a prefix,
 * like "__public_" or "__override_" and \a classname_prefix can
 * be used to give the class name a prefix.
 *
 * The \a option flags can be used to tweak various parameters, such as
 * showing static, original vs renamed name, underscores for space etc.
 *
 * The \a extra_arguments list is a list of extra arguments on the
 * form "bool static_call".
 */

void CppGenerator::writeFunctionSignature(QTextStream &s,
        const MetaFunction *java_function,
        const MetaClass *implementor,
        const QString &name_prefix,
        Option option,
        const QString &classname_prefix,
        const QStringList &extra_arguments,
        int numArguments) const{
// ### remove the implementor
    MetaType *function_type = java_function->type();

    if(java_function->hasTemplateTypes()){
        s << "template<";
        for(MetaTemplateParameter * templateParameter : java_function->templateParameters()){
            if(templateParameter->type()){
                writeTypeInfo(s, templateParameter->type());
                s << templateParameter->name();
            }
        }
        s << ">";
    }

    if (java_function->isStatic() && (option & ShowStatic))
        s << "static ";

    if ((option & SkipReturnType) == 0) {
        if((option & JNIProxyFunction)){
            QString typeReplaced = java_function->typeReplaced(0);
            if(!typeReplaced.isEmpty())
                s << jniName(typeReplaced.replace("@Nullable ", "").replace("@NonNull ", "")) << " ";
            else if (function_type) {
                s << translateType(function_type, Option(Option(option & ~JNIProxyFunction) & ~UseNativeIds));
                s << " ";
            } else if (!java_function->isConstructor()) {
                s << "void ";
            }
        }else{
            if (function_type) {
                writeTypeInfo(s, function_type, Option(option | SkipName));
                s << " ";
            } else if (!java_function->isConstructor()) {
                s << "void ";
            }
        }
    }

    if (implementor) {
        if((option & JNIProxyFunction)){
            if(implementor->typeEntry()->designatedInterface()){
                s << implementor->extractInterface()->name() + "_access" << "::";
            }else{
                s << implementor->name() + "_access" << "::";
            }
        }else{
            if (classname_prefix.isEmpty())
                s << shellClassName(implementor) << "::";
            else
                s << classname_prefix << implementor->qualifiedCppName() << "::";
        }
    }

    QString function_name;
    if (option & OriginalName)
        function_name = java_function->originalName();
    else
        function_name = java_function->name();

    /*if (java_function->attributes() & AbstractMetaAttributes::SetterFunction
            || java_function->attributes() & AbstractMetaAttributes::GetterFunction){
        option = Option(option | EnumAsInts);
    }*/

    if (option & UnderscoreSpaces)
        function_name = function_name.replace(' ', '_');

    if (java_function->isConstructor())
        function_name = shellClassName(java_function->ownerClass());
    else if(option & JNIProxyFunction)
        function_name += marshalledArguments(java_function);

    s << name_prefix << function_name;

    s << "(";

    bool needComma = false;
    if(option & JNIProxyFunction){
        s << "JNIEnv *__jni_env";
        if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
            if(!java_function->isStatic())
                s << ", QtJambiNativeID __this_nativeId";
        }else{
            if(!java_function->isStatic())
                s << ", jobject __this";
        }
        const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
        for(const ArgumentModification& argumentMod : addedArguments.first.values()){
            QString jniType = argumentMod.modified_jni_type.isEmpty() ? jniName(argumentMod.modified_type) : argumentMod.modified_jni_type;
            s << ", " << jniType << " " << argumentMod.modified_name;
        }
        for(const ArgumentModification& argumentMod : addedArguments.second){
            QString jniType = argumentMod.modified_jni_type.isEmpty() ? jniName(argumentMod.modified_type) : argumentMod.modified_jni_type;
            s << ", " << jniType << " " << argumentMod.modified_name;
        }
        needComma = true;
    }

    MetaArgumentList tmpList;
    if ((option & SkipRemovedArguments) == SkipRemovedArguments) {	// this should be a method
        const MetaArgumentList& arguments = java_function->arguments();
        for (int i = 0; i < arguments.size(); ++i) {
            MetaArgument* argument = arguments.at(i);
            if(java_function->argumentRemoved(argument->argumentIndex() + 1)!=ArgumentRemove_No){
                if(!java_function->isConstructor()
                        || ( !argument->originalDefaultValueExpression().isEmpty()
                             && java_function->conversionRule(TS::NativeCode, argument->argumentIndex() + 1).isEmpty()
                             && java_function->conversionRule(TS::TargetLangCode, argument->argumentIndex() + 1).isEmpty() )){
                    continue;
                }
            }
            tmpList.append(argument);
        }
    } else {
        tmpList.append(java_function->arguments());
    }
    if(needComma && !tmpList.isEmpty())
        s << ", ";
    writeFunctionArguments(s, tmpList, java_function, option, numArguments);

    // The extra arguments...
    if(!needComma)
        needComma = tmpList.size() != 0 ? true : false;
    for (int i = 0; i < extra_arguments.size(); ++i) {
        if (needComma)
            s << ", ";
        else
            needComma = true;
        s << extra_arguments.at(i);
    }

    s << ")";
    if (java_function->isConstant())
        s << " const";
    if (!java_function->isConstructor() && !java_function->isFinalInCpp() && (option & ShowOverride))
        s << " override";
}

QString CppGenerator::marshalledArguments(const MetaFunction *java_function){
    const MetaArgumentList& arguments = java_function->arguments();
    QString args;
    if (!arguments.isEmpty()) {
        for(const MetaArgument *arg : arguments) {
            if(arg->type()->getReferenceType()==MetaType::Reference){
                if(arg->type()->isConstant()){
                    args += "_cref";
                }else{
                    args += "_ref";
                }
            }else if(arg->type()->getReferenceType()==MetaType::RReference){
                if(arg->type()->isConstant()){
                    args += "_crval";
                }else{
                    args += "_rval";
                }
            }else{
                if(arg->type()->isConstant()){
                    args += "_const";
                }
            }
            QString qualifiedCppName;
            if(arg->type()->typeEntry()->isFunctional()){
                qualifiedCppName = arg->type()->typeEntry()->name();
            }else{
                qualifiedCppName = arg->type()->typeEntry()->qualifiedCppName();
            }
            if(qualifiedCppName=="qtjamireal")
                qualifiedCppName = "double";
            args += "_" + qualifiedCppName
                    .replace("::", "_")
                    .replace("<", "_")
                    .replace(">", "_")
                    .replace(",", "_")
                    .replace(".", "_")
                    .replace("*", "_ptr")
                    .replace("&", "_ref")
                    .replace(" ", "_");
            for(bool ind : arg->type()->indirections()){
                args += ind ? "_cptr" : "_ptr";
            }
        }
        if(java_function->isConstant()){
            args += "_constfct";
        }
    }
    return args;
}

QString CppGenerator::jni_signature(const MetaFunctional *function, JNISignatureFormat format) {
    QString returned = "(";
    const MetaArgumentList& arguments = function->arguments();
    for(const MetaArgument *argument : arguments) {
        if (function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
            if(function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
                QScopedPointer<MetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(MetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                MetaBuilder::decideUsagePattern(cpy.get());
                QString singleType = jni_signature(cpy.get(), format);
                if(singleType=="jint"){
                    returned += jni_signature("java.nio.IntBuffer", format);
                }else if(singleType=="jbyte"){
                    returned += jni_signature("java.nio.ByteBuffer", format);
                }else if(singleType=="jchar"){
                    returned += jni_signature("java.nio.CharBuffer", format);
                }else if(singleType=="jshort"){
                    returned += jni_signature("java.nio.ShortBuffer", format);
                }else if(singleType=="jlong"){
                    returned += jni_signature("java.nio.LongBuffer", format);
                }else if(singleType=="jfloat"){
                    returned += jni_signature("java.nio.FloatBuffer", format);
                }else if(singleType=="jdouble"){
                    returned += jni_signature("java.nio.DoubleBuffer", format);
                }else{
                    returned += jni_signature("java.nio.Buffer", format);
                }
            }else if(function->useArgumentAsArray(argument->argumentIndex() + 1)){
                QScopedPointer<MetaType> array(argument->type()->copy());
                array->setConstant(false);
                array->setReferenceType(MetaType::NoReference);
                array->setTypeUsagePattern(MetaType::ArrayPattern);
                QScopedPointer<MetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(MetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                MetaBuilder::decideUsagePattern(cpy.get());
                array->setArrayElementType(cpy.get());
                returned += jni_signature(array.get(), format);
                if(function->insertArrayOffsetArgument(argument->argumentIndex() + 1)){
                    int lengthParameter = function->arrayOrBufferLengthIndex(argument->argumentIndex() + 1);
                    if(lengthParameter>0 && lengthParameter<=arguments.size()){
                        const MetaArgument *lengthParam = arguments[lengthParameter - 1];
                        if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                            lengthParam = nullptr;
                            for(const MetaArgument *argument : arguments) {
                                if(argument && argument->argumentIndex()+1==lengthParameter){
                                    lengthParam = argument;
                                }
                            }
                        }
                        if(lengthParam && function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                            QString jniType;
                            QString modified_type = function->typeReplaced(lengthParam->argumentIndex() + 1, &jniType);

                            if (modified_type.isEmpty())
                                returned += jni_signature(lengthParam->type(), format);
                            else if(jniType.isEmpty())
                                returned += jni_signature(modified_type.replace("@Nullable ", "").replace("@NonNull ", ""), format);
                            else
                                returned += jni_signature(jniType, format);
                        }
                    }
                }
            }else{
                QString jniType;
                QString modified_type = function->typeReplaced(argument->argumentIndex() + 1, &jniType);

                if (modified_type.isEmpty())
                    returned += jni_signature(argument->type(), format);
                else if(jniType.isEmpty())
                    returned += jni_signature(modified_type.replace("@Nullable ", "").replace("@NonNull ", ""), format);
                else
                    returned += jni_signature(jniType, format);
            }
        }
    }

    returned += ")";

    QString jniType;
    QString modified_type = function->typeReplaced(0, &jniType);
    if (modified_type.isEmpty())
        returned += jni_signature(function->type(), JNISignatureFormat(format | ReturnType));
    else if(jniType.isEmpty())
        returned += jni_signature(annotationFreeTypeName(modified_type), JNISignatureFormat(format | ReturnType));
    else
        returned += jni_signature(jniType, JNISignatureFormat(format | ReturnType));
    return returned;
}

QString CppGenerator::jni_signature(const MetaFunction *function, JNISignatureFormat format) {
    QString returned = "(";
    if(function->isConstructor()){
        format = JNISignatureFormat(format | NoModification);
    }
    QMap<QString,QString> parameterTypesByName;
    for(const Parameter& parameterType : function->addedParameterTypes()){
        parameterTypesByName[parameterType.name] = parameterType.extends.split("&").first().trimmed();
    }
    const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = function->addedArguments();
    const MetaArgumentList& arguments = function->arguments();
    int argumentCounter = 1;
    while(addedArguments.first.contains(argumentCounter)){
        const ArgumentModification& mod = addedArguments.first[argumentCounter];
        if(parameterTypesByName.contains(mod.modified_type)){
            QString t = parameterTypesByName[mod.modified_type];
            if(t.isEmpty())
                t = "java.lang.Object";
            returned += jni_signature(annotationFreeTypeName(t), format);
        }else if(mod.modified_jni_type.isEmpty())
            returned += jni_signature(annotationFreeTypeName(mod.modified_type), format);
        else
            returned += jni_signature(mod.modified_jni_type, format);
        ++argumentCounter;
    }
    for(const MetaArgument *argument : arguments) {
        if (function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
            if(function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
                QString singleType = argument->type()->fullName();
                if(singleType=="int"){
                    returned += jni_signature("java.nio.IntBuffer", format);
                }else if(singleType=="byte"){
                    returned += jni_signature("java.nio.ByteBuffer", format);
                }else if(singleType=="char"){
                    returned += jni_signature("java.nio.CharBuffer", format);
                }else if(singleType=="short"){
                    returned += jni_signature("java.nio.ShortBuffer", format);
                }else if(singleType=="long"){
                    returned += jni_signature("java.nio.LongBuffer", format);
                }else if(singleType=="float"){
                    returned += jni_signature("java.nio.FloatBuffer", format);
                }else if(singleType=="double"){
                    returned += jni_signature("java.nio.DoubleBuffer", format);
                }else{
                    returned += jni_signature("java.nio.Buffer", format);
                }
            }else if(function->useArgumentAsArray(argument->argumentIndex() + 1)){
                QScopedPointer<MetaType> array(argument->type()->copy());
                array->setConstant(false);
                array->setReferenceType(MetaType::NoReference);
                array->setTypeUsagePattern(MetaType::ArrayPattern);
                QScopedPointer<MetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(MetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                MetaBuilder::decideUsagePattern(cpy.get());
                array->setArrayElementType(cpy.get());
                returned += jni_signature(array.get(), format);
                if(function->insertArrayOffsetArgument(argument->argumentIndex() + 1)){
                    int lengthParameter = function->arrayOrBufferLengthIndex(argument->argumentIndex() + 1);
                    if(lengthParameter>0 && lengthParameter<=arguments.size()){
                        const MetaArgument *lengthParam = arguments[lengthParameter - 1];
                        if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                            lengthParam = nullptr;
                            for(const MetaArgument *argument : arguments) {
                                if(argument && argument->argumentIndex()+1==lengthParameter){
                                    lengthParam = argument;
                                }
                            }
                        }
                        if(lengthParam && function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                            QString jniType;
                            QString modified_type = function->typeReplaced(lengthParam->argumentIndex() + 1, &jniType);

                            if (modified_type.isEmpty())
                                returned += jni_signature(lengthParam->type(), format);
                            else if(jniType.isEmpty())
                                returned += jni_signature(modified_type.replace("@Nullable ", "").replace("@NonNull ", ""), format);
                            else
                                returned += jni_signature(jniType, format);
                        }
                    }
                }
            }else{
                QString jniType;
                QString modified_type = function->typeReplaced(argument->argumentIndex() + 1, &jniType);
                if (modified_type.isEmpty())
                    returned += jni_signature(argument->type(), format);
                else{
                    if(parameterTypesByName.contains(modified_type)){
                        QString t = parameterTypesByName[modified_type];
                        if(t.isEmpty())
                            t = "java.lang.Object";
                        returned += jni_signature(annotationFreeTypeName(t), format);
                    }else if(jniType.isEmpty())
                        returned += jni_signature(annotationFreeTypeName(modified_type), format);
                    else
                        returned += jni_signature(jniType, format);
                }
            }
        }
        ++argumentCounter;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& mod = addedArguments.first[argumentCounter];
            if(parameterTypesByName.contains(mod.modified_type)){
                QString t = parameterTypesByName[mod.modified_type];
                if(t.isEmpty())
                    t = "java.lang.Object";
                returned += jni_signature(annotationFreeTypeName(t), format);
            }else if(mod.modified_jni_type.isEmpty())
                returned += jni_signature(annotationFreeTypeName(mod.modified_type), format);
            else
                returned += jni_signature(mod.modified_jni_type, format);
            ++argumentCounter;
        }
    }

    for(const ArgumentModification &mod : addedArguments.second){
        if(parameterTypesByName.contains(mod.modified_type)){
            QString t = parameterTypesByName[mod.modified_type];
            if(t.isEmpty())
                t = "java.lang.Object";
            returned += jni_signature(annotationFreeTypeName(t), format);
        }else if(mod.modified_jni_type.isEmpty())
            returned += jni_signature(annotationFreeTypeName(mod.modified_type), format);
        else
            returned += jni_signature(mod.modified_jni_type, format);
    }

    returned += ")";

    QString jniType;
    QString modified_type = function->typeReplaced(0, &jniType);
    if (modified_type.isEmpty()){
        if(function->isFinal())
            returned += jni_signature(function->type(), JNISignatureFormat(format | ReturnType));
        else
            returned += jni_signature(function->type(), JNISignatureFormat(format | ReturnType | NoQContainers));
    }else{
        modified_type = modified_type.replace("@Nullable ", "").replace("@NonNull ", "");
        if(parameterTypesByName.contains(modified_type)){
            QString t = parameterTypesByName[modified_type];
            if(t.isEmpty())
                t = "java.lang.Object";
            returned += jni_signature(annotationFreeTypeName(t), JNISignatureFormat(format | ReturnType));
        }else if(jniType.isEmpty())
            returned += jni_signature(annotationFreeTypeName(modified_type), JNISignatureFormat(format | ReturnType));
        else
            returned += jni_signature(jniType, JNISignatureFormat(format | ReturnType));
    }
    return returned;
}

QString CppGenerator::jni_signature(const QString &_full_name, JNISignatureFormat format) {
    QString signature;
    QString full_name = _full_name;

    if (full_name.endsWith("...")) {
        full_name.chop(3);
        full_name = full_name.trimmed();
        signature = "[";
    }
    while(full_name.endsWith("[]")) {
        full_name.chop(2);
        full_name = full_name.trimmed();
        signature = "[";
    }

    decltype(QString().length()) start = 0, end = -1;
    while ((start = full_name.indexOf("<")) >= 0 && (end = full_name.indexOf(">")) >= 0) {
        full_name.remove(start, end - start + 1);
    }

    static QMap<QString, QString> table{        {"boolean", "Z"},
                                                {"byte", "B"},
                                                {"char", "C"},
                                                {"short", "S"},
                                                {"int", "I"},
                                                {"long", "J"},
                                                {"float", "F"},
                                                {"double", "D"}};

    if ((format & Underscores) == Underscores)
        signature.replace("[", "_3");

    if (table.contains(full_name)) {
        signature += table[full_name];
    } else if ((format & Underscores) == Underscores) {
        signature.replace("[", "_3");
        signature += "L";
        signature += QString(full_name).replace("_", "_1").replace('.', '_').replace("$", "_00024");
        signature += "_2";
    } else {
        signature += "L";
        signature += QString(full_name).replace('.', '/');
        signature += ";";
    }

    return signature;
}

QString CppGenerator::jni_signature(const MetaType *java_type, JNISignatureFormat format) {
    if (!java_type)
        return "V";

    if(( (format & NoQContainers) == NoQContainers ) && java_type->typeEntry()->isContainer()){
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QSet")
            return jni_signature("java.util.Set", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QQueue")
            return jni_signature("java.util.Queue", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QStack")
            return jni_signature("java.util.Deque", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QLinkedList")
            return jni_signature("java.util.LinkedList", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QMap"
                || java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QMultiMap")
            return jni_signature("java.util.NavigableMap", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QHash"
                || java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QMultiHash")
            return jni_signature("java.util.Map", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QList"
                || java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QVector"
                || java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QStringList")
            return jni_signature("java.util.List", format);
//        if(java_type->typeEntry()->qualifiedTargetLangName()=="java.time.Duration")
//            return jni_signature("java.time.temporal.TemporalAmount", format);
//        if(java_type->typeEntry()->qualifiedTargetLangName()=="java.time.Instant")
//            return jni_signature("java.time.temporal.Temporal", format);
    }
    if(( (format & ReturnType) != ReturnType )
            && java_type->typeEntry()->isContainer()){
        if(java_type->typeEntry()->qualifiedTargetLangName()=="java.util.List"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.LinkedList"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.ArrayList"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Vector"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Queue"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Deque"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Set"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QSet"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QList"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QStringList"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QQueue"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QVector"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QStack"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QLinkedList")
            return jni_signature("java.util.Collection", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Map"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.SortedMap"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.HashMap"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.TreeMap"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.NavigableMap"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QMap"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QHash"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QMultiMap"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="io.qt.core.QMultiHash")
            return jni_signature("java.util.Map", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="java.time.Duration")
            return jni_signature("java.time.temporal.TemporalAmount", format);
        if(java_type->typeEntry()->qualifiedTargetLangName()=="java.time.Instant")
            return jni_signature("java.time.temporal.Temporal", format);
    }
    if (java_type->isArray()) {
        if ((format & Underscores) == Underscores)
            return "_3" + jni_signature(java_type->arrayElementType(), format);
        else
            return "[" + jni_signature(java_type->arrayElementType(), format);
    }
    if (java_type->isInitializerList() && java_type->instantiations().size()==1) {
        const MetaType* instantiation = java_type->instantiations()[0];
        if (instantiation->isTargetLangEnum() || instantiation->isTargetLangFlags()) {
            QString name = instantiation->name();
            if (instantiation->isTargetLangEnum()) {
                const EnumTypeEntry *et = static_cast<const EnumTypeEntry *>(instantiation->typeEntry());
                if(!et->javaQualifier().isEmpty())
                    name = et->javaQualifier() + "$" + et->targetLangName();
                else
                    name = et->targetLangName();

            } else if (instantiation->isTargetLangFlags()) {
                const FlagsTypeEntry *ft = static_cast<const FlagsTypeEntry *>(instantiation->typeEntry());
                if(!ft->originator()->javaQualifier().isEmpty())
                    name = ft->originator()->javaQualifier() + "$" + ft->targetLangName();
                else
                    name = ft->targetLangName();
            }
            return jni_signature((instantiation->package().isEmpty() ? QString() : instantiation->package() + ".") + name + "[]", format);
        }
        if ((format & Underscores) == Underscores)
            return "_3" + jni_signature(instantiation, format);
        else
            return "[" + jni_signature(instantiation, format);
    }
    if (java_type->isPointerContainer() && java_type->instantiations().size()==1) {
        MetaType* instantiation = java_type->instantiations()[0]->copy();
        instantiation->setIndirections(QList<bool>(instantiation->indirections()) << false);
        MetaBuilder::decideUsagePattern(instantiation);
        return jni_signature(instantiation, format);
    }
    if(java_type->typeEntry()->type()==TypeEntry::InstantiatedTemplateArgumentType){
        return jni_signature(reinterpret_cast<const InstantiatedTemplateArgumentEntry*>(java_type->typeEntry())->javaInstantiationBaseType(), format);
    }
    if (java_type->isCharString()) {
        if ((format & Underscores) == Underscores)
            return "Ljava_lang_String_2";
        else
            return "Ljava/lang/String;";
    }
    if (java_type->isNativePointer()) {
        if ((format & Underscores) == Underscores)
            return "Lio_qt_QNativePointer_2";
        else
            return "Lio/qt/QNativePointer;";
    }
    if (java_type->isFlags() && !(format & NoModification)) {
        return "I";
    }
    if (java_type->isEnum() && (!(format & NoModification) || reinterpret_cast<const EnumTypeEntry *>(java_type->typeEntry())->forceInteger())) {
        uint size = reinterpret_cast<const EnumTypeEntry *>(java_type->typeEntry())->size();
        switch(size){
        case 8: return "B";
        case 16: return "S";
        case 64: return "J";
        default: return "I";
        }
    }

    QString name = java_type->name();
    if (java_type->isFunctional()) {
        const FunctionalTypeEntry *func = static_cast<const FunctionalTypeEntry *>(java_type->typeEntry());
        if(!func->javaQualifier().isEmpty())
            name = func->javaQualifier()+"$"+name;
    } else if (const ImplementorTypeEntry * imptype = dynamic_cast<const ImplementorTypeEntry *>(java_type->typeEntry())) {
        if (const InterfaceTypeEntry *ie = imptype->designatedInterface())
            name = ie->targetLangName();
    } else if (java_type->isTargetLangEnum()) {
        const EnumTypeEntry *et = static_cast<const EnumTypeEntry *>(java_type->typeEntry());
        if(!et->javaQualifier().isEmpty())
            name = et->javaQualifier() + "$" + et->targetLangName();
        else
            name = et->targetLangName();

    } else if (java_type->isTargetLangFlags()) {
        const FlagsTypeEntry *ft = static_cast<const FlagsTypeEntry *>(java_type->typeEntry());
        if(!ft->originator()->javaQualifier().isEmpty())
            name = ft->originator()->javaQualifier() + "$" + ft->targetLangName();
        else
            name = ft->targetLangName();
    }

    return jni_signature((java_type->package().isEmpty() ? QString() : java_type->package() + ".") + name, format);
}

void getTypePPConditions(QStringList& ppConditions, const MetaType* type){
    if(type && type->typeEntry()->qualifiedCppName()=="QThread"){
        if(!type->typeEntry()->ppCondition().isEmpty() && !ppConditions.contains(type->typeEntry()->ppCondition())){
            ppConditions << type->typeEntry()->ppCondition();
        }
        for(const MetaType* inst : type->instantiations()){
            getTypePPConditions(ppConditions, inst);
        }
    }
}

QStringList CppGenerator::getFunctionPPConditions(const MetaFunction *java_function){
    QStringList pps;
    for(const FunctionModification& mod : java_function->modifications(java_function->declaringClass())){
        if(!mod.ppCondition.isEmpty() && !pps.contains(mod.ppCondition)){
            pps << mod.ppCondition;
        }
    }
    getTypePPConditions(pps, java_function->type());
    for(const MetaArgument *argument : java_function->arguments()) {
        getTypePPConditions(pps, argument->type());
    }
    if(java_function->ownerClass() && !java_function->ownerClass()->typeEntry()->ppCondition().isEmpty()){
        pps.removeAll(java_function->ownerClass()->typeEntry()->ppCondition());
    }
    if(!pps.isEmpty()){
        for (int i=0; i<pps.size(); ++i) {
            if(pps[i].contains("|")){
                pps[i] = "(" + pps[i] + ")";
            }
        }
    }
    return pps;
}

QString CppGenerator::resolveOutputDirectory() const { return cppOutputDirectory(); }

QString CppGenerator::cppOutputDirectory() const {
    if (!m_cpp_out_dir.isNull())
        return m_cpp_out_dir;
    return outputDirectory() + QLatin1String("/cpp");
}
void CppGenerator::setCppOutputDirectory(const QString &cppOutDir) { m_cpp_out_dir = cppOutDir; }

QString CppGenerator::subDirectoryForFunctional(const MetaFunctional * cls) const
{ return subDirectoryForPackage(cls->targetTypeSystem()); }

QString CppGenerator::subDirectoryForClass(const MetaClass *cls) const {
    return subDirectoryForPackage(cls->targetTypeSystem());
}

bool CppGenerator::shouldGenerateCpp(const MetaClass *java_class){
    return (!java_class->isNamespace() || java_class->functionsInTargetLang().size() > 0)
           && !java_class->isInterface()
           && !java_class->typeEntry()->isQVariant()
           && !java_class->typeEntry()->isIterator()
           && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
           && !java_class->isFake();
}

bool CppGenerator::shouldGenerate(const MetaClass *java_class) const {
    return shouldGenerateCpp(java_class);
}

bool CppGenerator::shouldGenerate(const MetaFunctional *functional) const {
    if(functional->enclosingClass()){
        if(!(!functional->enclosingClass()->isFake()
             && functional->enclosingClass()->typeEntry()
             && (functional->enclosingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp))){
            return false;
        }
        return functional->typeEntry()->getUsing().isEmpty() && (functional->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp);
    }
    return (functional->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp);
}

QString CppGenerator::shellClassName(const MetaClass *java_class) {
    if(java_class->typeEntry()->designatedInterface() && java_class->enclosingClass()){
        return java_class->generateShellClass()
               ? java_class->enclosingClass()->name() + "_shell"
               : java_class->enclosingClass()->qualifiedCppName();
    }
    return java_class->generateShellClass()
           ? java_class->name() + "_shell"
           : java_class->qualifiedCppName();
}

QString CppGenerator::shellClassName(const MetaFunctional *java_class) {
    QString _shellClassName;
    if(java_class->enclosingClass() && !java_class->enclosingClass()->isFake()){
        _shellClassName = shellClassName(java_class->enclosingClass());
        if(_shellClassName.endsWith("_shell")){
            _shellClassName = _shellClassName.chopped(6);
        }
        _shellClassName += "$";
    }
    return _shellClassName + java_class->name() + "_shell";
    //return java_class->enclosingClass()
    //        ? shellClassName(java_class->enclosingClass()) + "$" + java_class->name()
    //                                    : java_class->name() + "_shell";
}
