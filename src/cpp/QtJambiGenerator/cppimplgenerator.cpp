/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "cppimplgenerator.h"
#include "reporthandler.h"
#include "metabuilder.h"
#include "cppheadergenerator.h"
#include "bufferedoutputstream.h"

#include <QDir>
#include <QtDebug>
#include <QVariant>
#include <QBuffer>
#include <QRandomGenerator>

#include "typesystem/typedatabase.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

static Indentor INDENT;

namespace QNativePointer{
    enum class Type{
        /** Java Boolean*/ Boolean = 0,
        /** Java Byte*/ Byte,
        /** Java Char*/ Char,
        /** Java Short*/ Short,
        /** Java Int*/ Int,
        /** Java Long*/ Long,
        /** Java Float*/ Float,
        /** Java Double*/ Double,
        /** Another QNativePointer of any type*/ Pointer,
        /** Java String*/ String
    };
}

using namespace QNativePointer;

static const QMap<QString, QString> default_return_table{
    {QStringLiteral(u"boolean"), QStringLiteral(u"false")},
    {QStringLiteral(u"byte"), QStringLiteral(u"0")},
    {QStringLiteral(u"char"), QStringLiteral(u"0")},
    {QStringLiteral(u"short"), QStringLiteral(u"0")},
    {QStringLiteral(u"int"), QStringLiteral(u"0")},
    {QStringLiteral(u"long"), QStringLiteral(u"0")},
    {QStringLiteral(u"float"), QStringLiteral(u"0.0f")},
    {QStringLiteral(u"double"), QStringLiteral(u"0.0")},
    {QStringLiteral(u"java.lang.Object"), QStringLiteral(u"nullptr")}
};

CppImplGenerator::CppImplGenerator(PriGenerator *pri) : CppGenerator(pri) {
}

bool CppImplGenerator::shouldGenerate(const MetaClass *java_class) const {
    return /*(!java_class->isNamespace() || java_class->functionsInTargetLang().size() > 0)
           &&*/ !java_class->isInterface()
           && !java_class->typeEntry()->isQVariant()
           && !java_class->typeEntry()->isIterator()
           && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
           && !java_class->isFake();
}

QString CppImplGenerator::default_return_statement_qt(const MetaType *java_type, AbstractGenerator::Option options) {
    QString returnStr;
    if((options & AbstractGenerator::NoReturnStatement) == 0)
        returnStr += "return";
    if (!java_type)
        return returnStr;
    if((options & AbstractGenerator::NoReturnStatement) == 0)
        returnStr += " ";
    if (!java_type->indirections().isEmpty())
            return returnStr + "nullptr";

    QString signature = default_return_table.value(java_type->typeEntry()->targetLangName());

    if (!signature.isEmpty())
        return returnStr + signature;

    if(const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(java_type->typeEntry())){
        if(ctype->hasPublicDefaultConstructor()){
            return returnStr + java_type->cppSignature() + "()";
        }else if(ctype->hasNonPrivateConstructors() && ctype->name()!="QItemSelection"){
            CustomFunction customFunction = ctype->customConstructor(ComplexTypeEntry::DefaultConstructor);
            if(customFunction.hasCode()){
                QTextStream s(&returnStr);
                {
                    s << "*reinterpret_cast<";
                    writeTypeInfo(s, java_type, SkipName);
                    s << "*>(QMetaType::";
                    if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                        s << "fromName(\"";
                        writeTypeInfo(s, java_type, SkipName);
                        s << "\")";
                    }else{
                        s << "fromType<";
                        writeTypeInfo(s, java_type, SkipName);
                        s << ">()";
                    }
                    s << ".create())";
                }
                return returnStr;
            }
        }
    }

    Q_ASSERT(!java_type->isPrimitive());
    if (java_type->isJObjectWrapper()){
        if (java_type->typeEntry()->isJMapWrapper())
            return returnStr + QStringLiteral(u"JMapWrapper()");
        else if (java_type->typeEntry()->isJCollectionWrapper())
            return returnStr + QStringLiteral(u"JCollectionWrapper()");
        else if (java_type->typeEntry()->isJIteratorWrapper())
            return returnStr + QStringLiteral(u"JIteratorWrapper()");
        else if (java_type->typeEntry()->isJEnumWrapper())
            return returnStr + QStringLiteral(u"JEnumWrapper()");
        else if (java_type->typeEntry()->isJQFlagsWrapper())
            return returnStr + QStringLiteral(u"JQFlagsWrapper()");
        else return returnStr + QStringLiteral(u"JObjectWrapper()");
    }
    if (java_type->isQVariant())
        return returnStr + QStringLiteral(u"QVariant()");
    if (java_type->isCharString())
        return returnStr + QStringLiteral(u"nullptr");
    if (java_type->isQString())
        return returnStr + QStringLiteral(u"QString()");
    if (java_type->isQStringRef())
        return returnStr + QStringLiteral(u"QStringRef()");
    if (java_type->isQStringView())
        return returnStr + QStringLiteral(u"QStringView()");
    if (java_type->isQAnyStringView())
        return returnStr + QStringLiteral(u"QAnyStringView()");
    if (java_type->isQUtf8StringView())
        return returnStr + QStringLiteral(u"QUtf8StringView()");
    if (java_type->isQChar())
        return returnStr + QStringLiteral(u"QChar()");
    else if (java_type->isEnum())
        return returnStr + java_type->typeEntry()->name() + QStringLiteral(u"(0)");
    else if (java_type->isFlags())
        return returnStr + java_type->typeEntry()->name() + QStringLiteral(u"()");
    else if (java_type->isSmartPointer())
        return returnStr + java_type->cppSignature() + QStringLiteral(u"()");
    else if (java_type->isInitializerList())
        return returnStr + java_type->cppSignature() + QStringLiteral(u"()");
    else if (java_type->isQSpan())
        return returnStr + java_type->cppSignature() + QStringLiteral(u"()");
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::StringListContainer)
        return returnStr + java_type->typeEntry()->name() + QStringLiteral(u"()");
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::ByteArrayListContainer)
        return returnStr + java_type->typeEntry()->name() + QStringLiteral(u"()");
    else if (java_type->isContainer() || java_type->isIterator() || (java_type->typeEntry()->isComplex() && static_cast<const ComplexTypeEntry *>(java_type->typeEntry())->hasPublicDefaultConstructor()))
        return returnStr + java_type->cppSignature() + QStringLiteral(u"()");
    else
        return returnStr + QStringLiteral(u"{}");
}

QString default_return_statement_java(const MetaType *java_type,
                                      AbstractGenerator::Option options = AbstractGenerator::NoOption) {
    QString returnStr = ((options & AbstractGenerator::NoReturnStatement) == 0 ? QStringLiteral(u"return ") : QStringLiteral(u""));
    if (!java_type)
        return QStringLiteral(u"return");
    if (java_type->isArray() || !java_type->indirections().isEmpty())
        return returnStr+QStringLiteral(u"nullptr");

    QString signature = default_return_table.value(java_type->typeEntry()->targetLangName());
    if (!signature.isEmpty())
        return returnStr+signature;

    Q_ASSERT(!java_type->isPrimitive());
    return returnStr+QStringLiteral(u"nullptr");
}

/* Used to decide how which of the Call[Xxx]Method functions to call
 */
QString jniTypeName(const QString &name) {
    static QMap<QString, QString> table;
    if (table.isEmpty()) {
        table[QStringLiteral(u"jboolean")] = QStringLiteral(u"Boolean");
        table[QStringLiteral(u"jbyte")] = QStringLiteral(u"Byte");
        table[QStringLiteral(u"jchar")] = QStringLiteral(u"Char");
        table[QStringLiteral(u"jshort")] = QStringLiteral(u"Short");
        table[QStringLiteral(u"jint")] = QStringLiteral(u"Int");
        table[QStringLiteral(u"jlong")] = QStringLiteral(u"Long");
        table[QStringLiteral(u"jfloat")] = QStringLiteral(u"Float");
        table[QStringLiteral(u"jdouble")] = QStringLiteral(u"Double");
        table[QStringLiteral(u"jobject")] = QStringLiteral(u"Object");
    }

    return table[name];
}

QString CppImplGenerator::jniReturnName(const MetaFunction *java_function) const{
    if(java_function->useArgumentAsBuffer(0)){
        return QStringLiteral(u"jobject");
    }else if(java_function->useArgumentAsArray(0)){
        QScopedPointer<MetaType> cpy(java_function->type()->copy());
        cpy->setConstant(false);
        cpy->setReferenceType(MetaType::NoReference);
        QList<bool> indirections = cpy->indirections();
        if(!indirections.isEmpty()){
            indirections.removeLast();
            cpy->setIndirections(indirections);
        }
        MetaBuilder::decideUsagePattern(cpy.get());
        QString singleType = jni_signature(cpy.get(), JNISignatureFormat::ReturnType);
        if(singleType==QStringLiteral(u"jint") || singleType==QStringLiteral(u"int") || singleType==QStringLiteral(u"I")){
            return QStringLiteral(u"jintArray");
        }else if(singleType==QStringLiteral(u"jbyte") || singleType==QStringLiteral(u"byte") || singleType==QStringLiteral(u"B")){
            return QStringLiteral(u"jbyteArray");
        }else if(singleType==QStringLiteral(u"jboolean") || singleType==QStringLiteral(u"bool") || singleType==QStringLiteral(u"Z")){
            return QStringLiteral(u"jbooleanArray");
        }else if(singleType==QStringLiteral(u"jchar") || singleType==QStringLiteral(u"char") || singleType==QStringLiteral(u"C")){
            return QStringLiteral(u"jcharArray");
        }else if(singleType==QStringLiteral(u"jshort") || singleType==QStringLiteral(u"short") || singleType==QStringLiteral(u"S")){
            return QStringLiteral(u"jshortArray");
        }else if(singleType==QStringLiteral(u"jlong") || singleType==QStringLiteral(u"long") || singleType==QStringLiteral(u"J")){
            return QStringLiteral(u"jlongArray");
        }else if(singleType==QStringLiteral(u"jfloat") || singleType==QStringLiteral(u"float") || singleType==QStringLiteral(u"F")){
            return QStringLiteral(u"jfloatArray");
        }else if(singleType==QStringLiteral(u"jdouble") || singleType==QStringLiteral(u"double") || singleType==QStringLiteral(u"D")){
            return QStringLiteral(u"jdoubleArray");
        }else{
            return QStringLiteral(u"jobjectArray");
        }
        return QStringLiteral(u"jobject");
    }
    QString return_type = translateType(java_function->type(), Option(IsReturnType | EnumAsInts));
    QString new_return_type = java_function->typeReplaced(0);
    if (!new_return_type.isEmpty()) {
        return_type = jniName(annotationFreeTypeName(new_return_type));
    }
    return return_type;
}


QString jniTypeName(const MetaType *java_type) {
    if (!java_type) {
        return QStringLiteral(u"Void");
    } else if (java_type->isQChar()) {
        return QStringLiteral(u"Char");
    } else if (java_type->isPrimitive()) {
        return jniTypeName(java_type->typeEntry()->jniName());
    } else if (java_type->isIntegerEnum() || java_type->isIntegerFlags()) {
        return QStringLiteral(u"Int");
    } else {
        return QStringLiteral(u"Object");
    }
}

QString newXxxArray(const MetaType *java_type) {
    return "New" + jniTypeName(java_type) + "Array";
}

QString setXxxArrayElement(const MetaType *java_type) {
    Q_ASSERT(java_type);
    return "Set" + jniTypeName(java_type) + "ArrayElement";
}

QString getXxxArrayElement(const MetaType *java_type) {
    Q_ASSERT(java_type);
    return "Get" + jniTypeName(java_type) + "ArrayElement";
}

QString getXxxArrayRegion(const MetaType *java_type) {
    Q_ASSERT(java_type);
    return "Get" + jniTypeName(java_type) + "ArrayRegion";
}

QString setXxxArrayRegion(const MetaType *java_type) {
    Q_ASSERT(java_type);
    return "Set" + jniTypeName(java_type) + "ArrayRegion";
}

QString CppImplGenerator::callXxxMethod(const MetaType *java_type) const {
    return "Call" + jniTypeName(java_type) + "Method";
}

QString CppImplGenerator::callXxxMethod(const QString &name) const{
    TypeEntry *entry = database()->findType(name);
    if (entry){
        if(entry->isPrimitive() || entry->isQChar())
            return "Call" + jniTypeName(entry->jniName()) + "Method";
        else return QStringLiteral(u"CallObjectMethod");
    }else{
        if(name==QStringLiteral(u"void"))
            return QStringLiteral(u"CallVoidMethod");
        else if(name==QStringLiteral(u"boolean"))
            return QStringLiteral(u"CallBooleanMethod");
        else if(name==QStringLiteral(u"int"))
            return QStringLiteral(u"CallIntMethod");
        else if(name==QStringLiteral(u"long"))
            return QStringLiteral(u"CallLongMethod");
        else if(name==QStringLiteral(u"short"))
            return QStringLiteral(u"CallShortMethod");
        else if(name==QStringLiteral(u"byte"))
            return QStringLiteral(u"CallByteMethod");
        else if(name==QStringLiteral(u"char"))
            return QStringLiteral(u"CallCharMethod");
        else if(name==QStringLiteral(u"float"))
            return QStringLiteral(u"CallFloatMethod");
        else if(name==QStringLiteral(u"double"))
            return QStringLiteral(u"CallDoubleMethod");
        return QStringLiteral(u"CallObjectMethod");
    }
}

QString jni_function_signature(QString package,
                               QString class_name,
                               const QString &function_name,
                               const QString &return_type,
                               const QString &mangled_arguments = QString(),
                               uint options = CppImplGenerator::StandardJNISignature) {
    QString s;

    if (options & CppImplGenerator::ExternC)
        s += "extern \"C\" ";

    if (options & CppImplGenerator::Deprecated)
        s += "QT_DEPRECATED ";

    if (options & CppImplGenerator::JNIExport)
        s += "JNIEXPORT ";

    if (options & CppImplGenerator::ReturnType) {
        s += return_type;
        s += " ";
    }

    if (options & CppImplGenerator::JNIExport)
        s += "JNICALL ";

    s += "Java_";
    s += package.replace("_", "_1").replace(".", "_");
    s += '_';
    s += class_name.replace("_", "_1").replace("$", "_00024");
    s += '_';
    s += QString(function_name).replace("_", "_1").replace("$", "_00024");
    s += mangled_arguments;

    return s;
}

const MetaClass* lookupClassWithPublicDestructor(const MetaClass *cls) {
    while (cls != nullptr) {
        if (cls->typeEntry()->isDestructorPublic()) {
            return cls;
        } else {
            cls = cls->baseClass();
        }
    }
    return nullptr;
}

bool hasDeleter(const MetaClass *cls) {
    const ComplexTypeEntry *entry = cls->typeEntry();
    if (   !cls->isFake()
        && cls->typeEntry()->isComplex()
        && !cls->typeEntry()->isQObject()
        && !cls->typeEntry()->isNamespace()
        && !cls->isInterface()
        && (entry->codeGeneration() & TypeEntry::GenerateCode)
    ) {
        return lookupClassWithPublicDestructor(cls) != nullptr;
    }
    if(entry->skipMetaTypeRegistration()
            && (entry->codeGeneration() & TypeEntry::GenerateCode)){
        return lookupClassWithPublicDestructor(cls) != nullptr;
    }
    return false;
}

QString CppImplGenerator::fileNameForClass(const MetaClass *java_class) const {
    if(java_class->typeEntry()->designatedInterface()){
        return fileNameForClass(java_class->enclosingClass());
    }
    return QStringLiteral(u"%1_shell.cpp").arg(java_class->name().replace(u'$', u'_'));
}

QString CppImplGenerator::fileNameForFunctional(const MetaFunctional *java_class) const {
    if(java_class->enclosingClass() && !java_class->enclosingClass()->isFake())
        return QStringLiteral(u"%1_%2_shell.cpp").arg(java_class->enclosingClass()->name().replace(u'$', u'_'), java_class->name().replace(u'$', u'_'));
    else
        return QStringLiteral(u"%1_shell.cpp").arg(java_class->name().replace(u'$', u'_'));
}

void CppImplGenerator::write(QTextStream &s, const MetaFunctional *java_functional, int) {
#if 0
    {
        QString fileName("written_classes_cpp.log");
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
    const FunctionalTypeEntry *ftype = reinterpret_cast<const FunctionalTypeEntry *>(java_functional->typeEntry());
    bool hasDeprecation = java_functional->isDeclDeprecated() || (java_functional->type() && (java_functional->type()->typeEntry()->isDeclDeprecated() || java_functional->type()->typeEntry()->isContainer()));
    if(!hasDeprecation){
        for(const MetaArgument* arg : java_functional->arguments()){
            if(arg->type()->typeEntry()->isDeclDeprecated() || arg->type()->typeEntry()->isContainer()){
                hasDeprecation = true;
                break;
            }
        }
    }
    // Includes
    QSet<QString> included;
    if(!(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
        if(java_functional->enclosingClass() && !java_functional->enclosingClass()->isFake()){
            writeInclude(s, Include(Include::LocalPath, QString(java_functional->enclosingClass()->name()).replace(u'$', u'_')+"_"+QString(java_functional->name().replace(u'$', u'_'))+"_shell.h"), included);
        }else{
            writeInclude(s, Include(Include::LocalPath, QString(java_functional->name().replace(u'$', u'_'))+"_shell.h"), included);
        }
        if(hasDeprecation){
            s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
              << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl << Qt::endl;
        }
    }else{
        {
            IncludeList includes = java_functional->typeEntry()->extraIncludes();
            if(java_functional->typeEntry()->designatedInterface()){
                includes << java_functional->typeEntry()->designatedInterface()->extraIncludes();
            }
            std::sort(includes.begin(), includes.end());
            for(const Include& icl : includes){
                if(icl.suppressed)
                    writeInclude(s, icl, included);
            }
        }
        writeInclude(s, Include(Include::IncludePath, "QtCore/QtGlobal"), included);
        if(hasDeprecation){
            writeInclude(s, Include(Include::IncludePath, "QtCore/qcompilerdetection.h"), included);
            s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
              << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl << Qt::endl;
        }
        writeCodeInjections(s, java_functional->typeEntry(), CodeSnip::Position1, TS::NativeCode);
        if(java_functional->enclosingClass()){
            writeInclude(s, java_functional->enclosingClass()->typeEntry()->include(), included);
        }
        writeInclude(s, java_functional->typeEntry()->include(), included);
        writeExtraIncludes(s, java_functional, included, true);
        writeInclude(s, Include(Include::IncludePath, "QtJambi/QtJambiAPI"), included);
        writeExtraIncludes(s, java_functional, included);
    }
    writeInclude(s, Include(Include::IncludePath, "QtJambi/RegistryAPI"), included);
    if(ftype->isFunctionPointer() && java_functional->typeEntry()->getUsing().isEmpty()){
        writeInclude(s, Include(Include::IncludePath, "QtJambi/FunctionPointer"), included);
    }
    writeInclude(s, Include(Include::IncludePath, "QtJambi/Cast"), included);

    if(java_functional->enclosingClass() && !java_functional->enclosingClass()->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << java_functional->enclosingClass()->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    writeCodeInjections(s, java_functional->typeEntry(), CodeSnip::Beginning, TS::NativeCode);

    if(!(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
        s << "// emitting (writeShellConstructor)" << Qt::endl;
        writeShellConstructor(s, java_functional);
        s << "// emitting (writeShellDestructor)" << Qt::endl;
        writeShellDestructor(s, java_functional);

        s << "// construct " << shellClassName(java_functional) << Qt::endl
          << "void __qt_construct_" << QString(java_functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
        s << "(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*, bool, bool, bool)" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"construct " << java_functional->typeEntry()->qualifiedCppName() << "\")" << Qt::endl;
            s << INDENT << "new (__qtjambi_ptr) " << shellClassName(java_functional) << "();" << Qt::endl;
        }
        s << "}" << Qt::endl;

        s << "// destruct " << shellClassName(java_functional) << Qt::endl
          << "void __qt_destruct_" << QString(java_functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
        s << "(void* ptr)" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"destruct " << java_functional->typeEntry()->qualifiedCppName() << "\")" << Qt::endl;
            s << INDENT << "reinterpret_cast<" << shellClassName(java_functional) << "*>(ptr)->~" << shellClassName(java_functional).split("::").last() << "();" << Qt::endl;
        }
        s << "}" << Qt::endl << Qt::endl;

        s << "void " << shellClassName(java_functional) << "::operator delete(void * ptr) noexcept {" << Qt::endl;
        s << "    reinterpret_cast<" << shellClassName(java_functional) << "*>(ptr)->" << shellClassName(java_functional) << "::__shell()->tryDeleteShell(typeid(" << java_functional->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
        s << "}" << Qt::endl << Qt::endl;
        s << "QtJambiShell* " << shellClassName(java_functional) << "::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(" << shellClassName(java_functional) << ") ); }" << Qt::endl << Qt::endl;

        s << shellClassName(java_functional) << "::Functor::Functor(" << shellClassName(java_functional) << "& functional) : FunctorBase(typeid(" << java_functional->typeEntry()->qualifiedCppName() << "), functional) {}" << Qt::endl << Qt::endl
          << shellClassName(java_functional) << "::Functor::Functor(const Functor& functor) : FunctorBase(functor) {}" << Qt::endl << Qt::endl
          << shellClassName(java_functional) << "::Functor::~Functor() {}" << Qt::endl << Qt::endl;

        MetaType* originalReferenceFunctionType = nullptr;
        MetaType *function_type = java_functional->type();

        if(function_type){
            if(function_type->getReferenceType()!=MetaType::NoReference){
                originalReferenceFunctionType = function_type;
                function_type = function_type->copy();
                function_type->setReferenceType(MetaType::NoReference);
                function_type->setIndirections({false});
                MetaBuilder::decideUsagePattern(function_type);
            }
            writeTypeInfo(s, function_type, Option());
        }else{
            s << "void";
        }
        s << " " << shellClassName(java_functional) << "::Functor::operator()(";
        int counter = 0;
        for(const MetaArgument *arg : java_functional->arguments()){
            if(counter>0)
                s << ", ";
            writeTypeInfo(s, arg->type(), Option());
            s << arg->indexedName();
            ++counter;
        }
        s << ")" << Qt::endl
          << "{" << Qt::endl;
        QString new_return_type = java_functional->typeReplaced(0);
        {
            INDENTATION(INDENT)
            if(function_type){
                s << INDENT;
                writeTypeInfo(s, function_type, Option());
                s << "__qt_return_value = " << default_return_statement_qt(function_type, AbstractGenerator::NoReturnStatement) << ";" << Qt::endl;
            }
            s << INDENT << "if (jmethodID method_id = javaMethod()) {" << Qt::endl;
            bool needsScope = false;
            bool isMessageHandler = java_functional->arguments().size()==3 && java_functional->typeEntry()->qualifiedCppName()=="QtMessageHandler";
            {
                INDENTATION(INDENT)
                QString lines;
                {
                    QTextStream s(&lines);
                    INDENTATION(INDENT)
                    if(isMessageHandler){
                        s << INDENT << "JavaException throwable;" << Qt::endl
                          << INDENT << "if(env->ExceptionCheck()){" << Qt::endl
                          << INDENT << "    jthrowable t = env->ExceptionOccurred();" << Qt::endl
                          << INDENT << "    env->ExceptionClear();" << Qt::endl
                          << INDENT << "    throwable = JavaException(env, t);" << Qt::endl
                          << INDENT << "}" << Qt::endl;
                    }
                    s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        {
                            s << INDENT << "if (jobject __java_this = getJavaObjectLocalRef(env)) {" << Qt::endl;
                            QString lines;
                            {
                                QTextStream s(&lines);
                                INDENTATION(INDENT)
                                for(MetaArgument *argument : java_functional->arguments()) {
                                    ArgumentRemove ar = java_functional->argumentRemoved(argument->argumentIndex() + 1);
                                    switch(ar) {
                                    case ArgumentRemove_No:{
                                            if (!(argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                                                    || java_functional->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)
                                            ) {
                                                writeQtToJava(s,
                                                              argument->type(),
                                                              argument->indexedName(),
                                                              "__java_" + argument->indexedName(),
                                                              nullptr,
                                                              java_functional,
                                                              argument->argumentIndex() + 1,
                                                              NoOption,
                                                              "env",
                                                              "env.scope()");
                                            }

                                            if (java_functional->resetObjectAfterUse(argument->argumentIndex() + 1)){
                                                if (argument->type()->isContainer()){
                                                    if(static_cast<const ContainerTypeEntry *>(argument->type()->typeEntry())->type()==ContainerTypeEntry::std_array){
                                                        s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                    }else{
                                                        s << INDENT << "QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                    }
                                                }else if (argument->type()->isArray()){
                                                    s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                }else{
                                                    s << INDENT << "QTJAMBI_INVALIDATE_AFTER_USE(env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                }
                                            }
                                        }
                                        break;
                                    default: break;
                                    }
                                }
                                for(const MetaArgument* argument : java_functional->arguments()) {
                                    writeOwnership(s, nullptr, java_functional, "__java_" + argument->indexedName(), "__qt_" + argument->indexedName(), argument->argumentIndex() + 1, nullptr, TS::ShellCode, "env", "env.scope()");
                                }

                                s << INDENT;
                                QString typeWrap;
                                if (function_type) {
                                    QString _jniName;
                                    if (new_return_type.isEmpty()) {
                                        _jniName = translateType(function_type);
                                    } else {
                                        _jniName = jniName(new_return_type);
                                    }
                                    if(_jniName=="jstring"
                                            || _jniName=="jclass"
                                            || _jniName=="jthrowable"
                                            || _jniName.endsWith("Array"))
                                        typeWrap = _jniName;
                                    s << _jniName << " __java_return_value = ";	// declaration only
                                }else if (!new_return_type.isEmpty()) {
                                    QString _jniName = jniName(new_return_type);
                                    if(_jniName=="jstring"
                                            || _jniName=="jclass"
                                            || _jniName=="jthrowable"
                                            || _jniName.endsWith("Array"))
                                        typeWrap = _jniName;
                                    s << _jniName << " __java_return_value = ";	// declaration only
                                }

                                if(!typeWrap.isEmpty())
                                    s << typeWrap << "(";
                                s << "env->";
                                if (!new_return_type.isEmpty()) {
                                    s << callXxxMethod(new_return_type);
                                }else if (!function_type) {
                                    s << "CallVoidMethod";
                                }else{
                                    s << callXxxMethod(function_type);
                                }

                                s << "(__java_this, method_id";
                                if (java_functional->arguments().size() > 0)
                                    s << ", ";
                                writeFunctionCallArguments(s, java_functional, "__java_", Option(NoCasts | SkipRemovedArguments));
                                s << ")";
                                if(!typeWrap.isEmpty())
                                    s << ")";
                                s << ";" << Qt::endl;
                                s << INDENT << "JavaException::check(env QTJAMBI_STACKTRACEINFO );" << Qt::endl;

                                if (function_type) {
                                    QString scopeName = java_functional->needsReturnScope() ? QStringLiteral(u"__qtjambi_return_scope") : QStringLiteral(u"env.scope()");
                                    writeJavaToQt(s, function_type, "__qt_return_value_tmp", "__java_return_value",
                                                  nullptr, java_functional, 0, GlobalRefJObject, "env",
                                                  scopeName);
                                    // This line below i sonly needed because we can't instruction writeJavaToQt
                                    //  to omit the emitting the function type, so then we could assign
                                    //  __qt_return_value directly.
                                    // This is not very efficient for complex types as it may cause extra copying.
                                    s << INDENT << "__qt_return_value = __qt_return_value_tmp;" << Qt::endl;
                                    writeOwnership(s, nullptr, java_functional, "__java_return_value", "__qt_return_value", 0, nullptr, TS::ShellCode, "env", scopeName);
                                } else if (java_functional->hasConversionRule(TS::ShellCode, 0)) {
                                    writeConversionRule(s, TS::ShellCode, nullptr, java_functional, 0, "<invalid>", "<invalid>", NoOption, "env", "env.scope()");
                                }
                            }
                            if(lines.contains("env.scope()")){
                                needsScope = true;
                            }
                            if(java_functional->needsReturnScope()){
                                if(lines.contains(QStringLiteral(u"__qtjambi_return_scope"))){
                                    s << INDENT << "    QtJambiScope& __qtjambi_return_scope = *shell()->returnScope(env, typeid(" << java_functional->typeEntry()->qualifiedCppName() << "), 0);" << Qt::endl;
                                }
                            }
                            s << lines;
                            s << INDENT << "}" << Qt::endl;
                        }
                        if(function_type){
                            if(java_functional->nullPointersDisabled() || originalReferenceFunctionType){
                                s << INDENT << "if(!__qt_return_value)" << Qt::endl;
                                s << INDENT << "    JavaException::raiseNullPointerException(__jni_env, \"" << java_functional->typeEntry()->qualifiedCppName() << ": Unexpected null pointer returned.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                            }
                        }
                    }
                    s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn) {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(isMessageHandler){
                            s << INDENT << "throwable.addSuppressed(env, exn);" << Qt::endl;
                        }else{
                            s << INDENT << "env.handleException(exn, this, \"" << java_functional->typeEntry()->qualifiedCppName() << "\");" << Qt::endl;
                        }
                    }
                    s << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
                    if(isMessageHandler){
                        s << INDENT << "throwable.raiseInJava(env);" << Qt::endl;
                    }
                }
                if(needsScope){
                    if(isMessageHandler){
                        s << INDENT << "if (JniEnvironmentScope env{shell(), " << QString::number( 100*(java_functional->arguments().size()+2) ) << "}) {" << Qt::endl;
                    }else if(java_functional->isNoExcept()){
                        s << INDENT << "if (JniEnvironmentScopeExceptionInhibitor env{shell(), " << QString::number( 100*(java_functional->arguments().size()+2) ) << "}) {" << Qt::endl;
                    }else{
                        s << INDENT << "if (JniEnvironmentScopeExceptionHandler env{shell(), " << QString::number( 100*(java_functional->arguments().size()+2) ) << "}) {" << Qt::endl;
                    }
                }else{
                    if(isMessageHandler){
                        s << INDENT << "if (JniEnvironment env{" << QString::number( 100*(java_functional->arguments().size()+2) ) << "}) {" << Qt::endl;
                    }else if(java_functional->isNoExcept()){
                        s << INDENT << "if (JniEnvironmentExceptionInhibitor env{" << QString::number( 100*(java_functional->arguments().size()+2) ) << "}) {" << Qt::endl;
                    }else{
                        s << INDENT << "if (JniEnvironmentExceptionHandler env{" << QString::number( 100*(java_functional->arguments().size()+2) ) << "}) {" << Qt::endl;
                    }
                }
                s << lines;
                if(isMessageHandler){
                    s << INDENT << "}else{" << Qt::endl
                      << INDENT << "    switch(" << java_functional->arguments().first()->argumentName() << "){" << Qt::endl
                      << INDENT << "    case QtMsgType::QtCriticalMsg:" << Qt::endl
                      << INDENT << "        printf(\"CRITICAL: %s\\n\", qPrintable(" << java_functional->arguments().last()->argumentName() << "));" << Qt::endl
                      << INDENT << "        break;" << Qt::endl
                      << INDENT << "    case QtMsgType::QtDebugMsg:" << Qt::endl
                      << INDENT << "        printf(\"DEBUG: %s\\n\", qPrintable(" << java_functional->arguments().last()->argumentName() << "));" << Qt::endl
                      << INDENT << "        break;" << Qt::endl
                      << INDENT << "    case QtMsgType::QtInfoMsg:" << Qt::endl
                      << INDENT << "        printf(\"INFO: %s\\n\", qPrintable(" << java_functional->arguments().last()->argumentName() << "));" << Qt::endl
                      << INDENT << "        break;" << Qt::endl
                      << INDENT << "    case QtMsgType::QtWarningMsg:" << Qt::endl
                      << INDENT << "        printf(\"WARNING: %s\\n\", qPrintable(" << java_functional->arguments().last()->argumentName() << "));" << Qt::endl
                      << INDENT << "        break;" << Qt::endl
                      << INDENT << "    case QtMsgType::QtFatalMsg:" << Qt::endl
                      << INDENT << "        printf(\"FATAL: %s\\n\", qPrintable(" << java_functional->arguments().last()->argumentName() << "));" << Qt::endl
                      << INDENT << "        break;" << Qt::endl
                      << INDENT << "    }" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
            if(function_type){
                if(originalReferenceFunctionType){
                    delete function_type;
                    function_type = originalReferenceFunctionType;
                    s << "*";
                    s << INDENT << "if(__qt_return_value)" << Qt::endl
                      << INDENT << "    return *__qt_return_value;" << Qt::endl
                      << INDENT << "else"
                      << INDENT << default_return_statement_qt(function_type, AbstractGenerator::NoOption) << ";" << Qt::endl;
                }else{
                    s << INDENT << "return __qt_return_value;" << Qt::endl;
                }
            }
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;


        s << INDENT << "void " << shellClassName(java_functional) << "::getFunctional(JNIEnv *";
        if(ftype->isFunctionPointer())
            s << "env";
        s << ", void* result)" << Qt::endl
          << INDENT << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            if(ftype->isFunctionPointer()){
                s << INDENT << "jobject __this = " << shellClassName(java_functional) << "::__shell()->getJavaObjectLocalRef(env);" << Qt::endl;
                s << INDENT << "uint hash = QtJambiAPI::getJavaObjectIdentity(env, __this);" << Qt::endl;
                s << INDENT << "*reinterpret_cast<" << java_functional->typeEntry()->qualifiedCppName() << "*>(result) = qtjambi_function_pointer<";
                if(java_functional->typeEntry()->count()!=0){
                    s << QString::number(java_functional->typeEntry()->count());
                }else{
                    s << "14";
                }
                s << ",";
                if(java_functional->type()){
                    writeTypeInfo(s, java_functional->type(), Option(SkipName));
                }else{
                    s << "void";
                }
                s << "(";
                int counter = 0;
                for(const MetaArgument *arg : java_functional->arguments()){
                    if(counter>0)
                        s << ",";
                    writeTypeInfo(s, arg->type(), Option(SkipName));
                    ++counter;
                }
                s << ")>(Functor(*this), hash, &m_functionPointerDeleter, reverseFunctionGetter ? nullptr : &reverseFunctionGetter);" << Qt::endl;
            }else{
                s << INDENT << "*reinterpret_cast<" << java_functional->typeEntry()->qualifiedCppName() << "*>(result) = Functor(*this);" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;


        s << INDENT << "bool " << shellClassName(java_functional) << "::isFunctionPointer()" << Qt::endl
          << INDENT << "{" << Qt::endl;
        if(ftype->isFunctionPointer()){
            s << INDENT << "    return true;" << Qt::endl;
        }else{
            s << INDENT << "    return false;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;

        s << INDENT << "jobject " << shellClassName(java_functional) << "::resolveFunctional(JNIEnv * env, const void* functionPtr, bool* ok)" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "if(functionPtr";
            if(ftype->isFunctionPointer()){
                s << " && reverseFunctionGetter";
            }
            s << "){" << Qt::endl
              << INDENT << "    const " << java_functional->typeEntry()->qualifiedCppName() << "& function = *reinterpret_cast<const " << java_functional->typeEntry()->qualifiedCppName() << "*>(functionPtr);" << Qt::endl
              << INDENT << "    if(const " << shellClassName(java_functional) << "::Functor* functor = ";
            if(ftype->isFunctionPointer()){
                s << "reverseFunctionGetter(function)";
            }else{
                s << "function.target<" << shellClassName(java_functional) << "::Functor>()";
            }
            s << "){" << Qt::endl
              << INDENT << "        if(ok)" << Qt::endl
              << INDENT << "            *ok = true;" << Qt::endl
              << INDENT << "        return functor->getJavaObjectLocalRef(env);" << Qt::endl
              << INDENT << "    }" << Qt::endl
              << INDENT << "    if(ok)" << Qt::endl
              << INDENT << "        *ok = !function;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << INDENT << "return nullptr;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
    }

    QString functionalClassPackage = java_functional->package();
    QString functionalClassName = java_functional->name();
    if(!(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
        functionalClassName += "$Impl";
    }
    if(java_functional->enclosingClass()){
        functionalClassName = java_functional->enclosingClass()->name() + "$" + functionalClassName;
        functionalClassPackage = java_functional->enclosingClass()->package();
    }
    QString return_type = java_functional->type() ? translateType(java_functional->type(), Option(EnumAsInts)) : QLatin1String("void");
    QString functionName = java_functional->typeEntry()->functionName().isEmpty() ? QString("call") : java_functional->typeEntry()->functionName();
    if(java_functional->needsCallThrough())
        functionName = QString("%1_native").arg(functionName);

    QString args = "__";

    JNISignatureFormat format = Underscores;

    if (java_functional->needsCallThrough()){
        if(java_functional->typeEntry()->isNativeIdBased()){
            args += "J";
        }
    }

    if (!java_functional->arguments().isEmpty()) {
        for(const MetaArgument *argument : java_functional->arguments()) {
            if (java_functional->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
                QString jniType;
                QString modified_type = java_functional->typeReplaced(argument->argumentIndex()+1, &jniType);
                if(java_functional->useArgumentAsBuffer(argument->argumentIndex() + 1)){
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
                    if(singleType=="jint" || singleType=="int" || singleType=="I"){
                        args += jni_signature("java.nio.IntBuffer", format);
                    }else if(singleType=="jbyte" || singleType=="byte" || singleType=="B"){
                        args += jni_signature("java.nio.ByteBuffer", format);
                    }else if(singleType=="jchar" || singleType=="char" || singleType=="C"){
                        args += jni_signature("java.nio.CharBuffer", format);
                    }else if(singleType=="jshort" || singleType=="short" || singleType=="S"){
                        args += jni_signature("java.nio.ShortBuffer", format);
                    }else if(singleType=="jlong" || singleType=="long" || singleType=="J"){
                        args += jni_signature("java.nio.LongBuffer", format);
                    }else if(singleType=="jfloat" || singleType=="float" || singleType=="F"){
                        args += jni_signature("java.nio.FloatBuffer", format);
                    }else if(singleType=="jdouble" || singleType=="double" || singleType=="D"){
                        args += jni_signature("java.nio.DoubleBuffer", format);
                    }else{
                        args += jni_signature("java.nio.Buffer", format);
                    }
                }else if(java_functional->useArgumentAsArray(argument->argumentIndex() + 1)){
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
                    args += jni_signature(array.get(), format);
                    int lengthParameter = java_functional->utilArgumentIndex(argument->argumentIndex() + 1);
                    if(lengthParameter>0 && lengthParameter<=java_functional->arguments().size()){
                        const MetaArgument *lengthParam = java_functional->arguments()[lengthParameter - 1];
                        if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                            lengthParam = nullptr;
                            for(const MetaArgument *argument : java_functional->arguments()) {
                                if(argument && argument->argumentIndex()+1==lengthParameter){
                                    lengthParam = argument;
                                }
                            }
                        }
                        if(lengthParam && java_functional->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                            QString typeReplaced = java_functional->typeReplaced(lengthParam->argumentIndex() + 1);
                            if(typeReplaced.isEmpty()){
                                if(isCharSequenceSubstitute(lengthParam->type())){
                                    args += jni_signature("java.lang.CharSequence", format);
                                }else{
                                    args += jni_signature(lengthParam->type(), format);
                                }
                            }else{
                                if(jniType.isEmpty()){
                                    args += jni_signature(annotationFreeTypeName(typeReplaced), format);
                                }else{
                                    args += jni_signature(jniType, format);
                                }
                            }
                        }
                    }
                }else if (!modified_type.isEmpty() || !argument->type()->hasNativeId()) {
                    if (modified_type.isEmpty()){
                        if(java_functional->useArgumentAsSlotContext(argument->argumentIndex() + 1)){
                            args += jni_signature(QStringLiteral(u"io.qt.core.QObject"), format);
                        }else if(isCharSequenceSubstitute(argument->type())){
                            args += jni_signature("java.lang.CharSequence", format);
                        }else{
                            args += jni_signature(argument->type(), format);
                        }
                    }else if(jniType.isEmpty()){
                        args += jni_signature(annotationFreeTypeName(modified_type), format);
                    }else{
                        args += jni_signature(jniType, format);
                    }
                } else {
                    args += "J";
                }
            }
        }
    }

    s << jni_function_signature(
               functionalClassPackage,
               (java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell) ? functionalClassName : functionalClassName+"$ConcreteWrapper",
               functionName,
               return_type,
               args
        );
    s << Qt::endl << "(JNIEnv *__jni_env, jobject";
    if(java_functional->typeEntry()->isNativeIdBased()){
        s << ", QtJambiNativeID __this_nativeId";
    }else{
        s << " __this";
    }
    for(const MetaArgument *arg : java_functional->arguments()){
        if(java_functional->argumentRemoved(arg->argumentIndex() +1))
            continue;
        bool addArrayOffset = false;
        s << ", ";
        if(java_functional->useArgumentAsBuffer(arg->argumentIndex() + 1)){
            s << "jobject";
        }else if(java_functional->useArgumentAsArray(arg->argumentIndex() + 1)){
            QScopedPointer<MetaType> array(arg->type()->copy());
            array->setConstant(false);
            array->setReferenceType(MetaType::NoReference);
            array->setTypeUsagePattern(MetaType::ArrayPattern);
            QScopedPointer<MetaType> cpy(arg->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(MetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            MetaBuilder::decideUsagePattern(cpy.get());
            array->setArrayElementType(cpy.get());
            s << translateType(array.get(), Option(UseNativeIds | EnumAsInts));
            addArrayOffset = java_functional->insertUtilArgument(arg->argumentIndex() + 1);
        }else{
            QString jniType;
            QString modified_type = java_functional->typeReplaced(arg->argumentIndex() + 1, &jniType);
            if(modified_type.isEmpty() && java_functional->useArgumentAsSlotContext(arg->argumentIndex() + 1)){
                modified_type = "io.qt.core.QObject";
            }
            if(!jniType.isEmpty()){
                s << jniType;
            }else if(modified_type.isEmpty()){
                s << translateType(arg->type(), Option(UseNativeIds | EnumAsInts));
            }else{
                QString _jniName = jniName(modified_type);
                s << _jniName;
            }
        }
        s << " " << arg->indexedName();
        if(addArrayOffset){
            s << ", ";
            int lengthParameter = java_functional->utilArgumentIndex(arg->argumentIndex() + 1);
            if(lengthParameter>0 && lengthParameter<=java_functional->arguments().size()){
                const MetaArgument *lengthParam = java_functional->arguments()[lengthParameter - 1];
                if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                    lengthParam = nullptr;
                    for(const MetaArgument *argument : java_functional->arguments()) {
                        if(argument && argument->argumentIndex()+1==lengthParameter){
                            lengthParam = argument;
                        }
                    }
                }
                if(lengthParam && java_functional->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                    QString typeReplaced = java_functional->typeReplaced(lengthParam->argumentIndex() + 1);
                    if(!typeReplaced.isEmpty())
                        s << jniName(annotationFreeTypeName(typeReplaced));
                    else
                        s << translateType(lengthParam->type(), Option(UseNativeIds | EnumAsInts));
                }
            }
            s << " __qt_offset_of_" << arg->indexedName();
        }

    }
    s << ")" << Qt::endl;
    s << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"" << java_functional->name() << "(...)\")" << Qt::endl;
        if(java_functional->isRethrowExceptions() || java_functional->isBlockExceptions() || java_functional->isNoExcept())
            s << INDENT << "QtJambiExceptionRaiser __qt_exceptionRaiser;" << Qt::endl;
        if (java_functional->type())
            s << INDENT << return_type << " __java_return_value{0};" << Qt::endl;
        QString lines;
        {
            QTextStream s(&lines);
            s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << java_functional->typeEntry()->qualifiedCppName() << "* function = ";
                if(java_functional->typeEntry()->isNativeIdBased()){
                    s << "QtJambiAPI::interfaceFromNativeId<" << java_functional->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                }else{
                    s << "QtJambiAPI::convertJavaInterfaceToNative<" << java_functional->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                }
                s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, function);" << Qt::endl;

                for(const MetaArgument *argument : java_functional->arguments()) {
                    if(java_functional->argumentRemoved(argument->argumentIndex() +1))
                        continue;
                    if (!(argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                            || java_functional->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
                        writeJavaToQt(s,
                                      argument->type(),
                                      "__qt_" + argument->indexedName(),
                                      argument->indexedName(),
                                      nullptr,
                                      java_functional,
                                      argument->argumentIndex() + 1,
                                      Option(UseNativeIds | EnumAsInts), "__jni_env", "__qtjambi_scope");
                        writeOwnership(s, nullptr, java_functional, argument->indexedName(), "__qt_" + argument->indexedName(), argument->argumentIndex() + 1, nullptr, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    }
                }

                if (java_functional->type()) {
                    const QString qt_return_value = "__qt_return_value";
                    const QString java_return_value = "__java_return_value";
                    bool noConversion = false;
                    if((java_functional->type()->isPrimitive() || java_functional->type()->isPrimitiveChar())
                            && java_functional->typeReplaced(0).isEmpty()
                            && !java_functional->hasConversionRule(TS::NativeCode, 0)
                             && (java_functional->type()->typeEntry()->qualifiedCppName()=="int"
                                 || java_functional->type()->typeEntry()->qualifiedCppName()=="double"
                                 || java_functional->type()->typeEntry()->qualifiedCppName()=="float"
                                 || java_functional->type()->typeEntry()->qualifiedCppName()=="char"
                                 || java_functional->type()->typeEntry()->qualifiedCppName()=="short"
                                 || java_functional->type()->typeEntry()->qualifiedCppName()=="bool"
                                 || java_functional->type()->typeEntry()->qualifiedCppName()=="long long")){
                        s << java_return_value << " = ";
                        noConversion = true;
                    }else {
                        s << INDENT;
                        writeTypeInfo(s, java_functional->type(), EnumAsInts);
                        s << " " << qt_return_value << " = ";
                    }
                    if(java_functional->type()->isEnum()){
                        uint size = static_cast<const EnumTypeEntry*>(java_functional->type()->typeEntry())->size();
                        s << "qint" << size << "(";
                    }

                    s << "(*function)(";
                    writeFunctionCallArguments(s, java_functional, "__qt_");
                    s << ")";
                    if(java_functional->type()->isEnum()){
                        s << ")";
                    }
                    s << ";" << Qt::endl;
                    if(!noConversion){
                        writeQtToJava(s, java_functional->type(), qt_return_value, java_return_value,
                                      nullptr, nullptr, 0, Option(NoTmpVariable | EnumAsInts),
                                      "__jni_env", "__qtjambi_scope");
                        writeOwnership(s, nullptr, java_functional, java_return_value, qt_return_value, 0, nullptr, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    }
                } else {
                    s << INDENT << "(*function)(";
                    writeFunctionCallArguments(s, java_functional, "__qt_");
                    s << ");" << Qt::endl;
                }
                if(java_functional->isRethrowExceptions() || java_functional->isBlockExceptions() || java_functional->isNoExcept())
                    s << INDENT << "__qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;
            }
            s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn) {" << Qt::endl;
            s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
            s << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
        }
        if(lines.contains("__qtjambi_scope")){
            s << INDENT << "QtJambiScope __qtjambi_scope(";
            if(java_functional->typeEntry()->isNativeIdBased()){
                s << "__this_nativeId";
            }else{
                s << "__jni_env, __this";
            }
            s << ");" << Qt::endl;
        }
        s << lines;
        if(return_type!="void")
            s << INDENT << "return __java_return_value;" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;

    if(!(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
        s << jni_function_signature(
                   functionalClassPackage,
                   functionalClassName,
                   "initialize_native",
                   "void"
            );
        s << Qt::endl << "(JNIEnv *__jni_env, jclass __jni_class, jobject __jni_object)" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"" << shellClassName(java_functional) << "\")" << Qt::endl;
            s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_" << QString(java_functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << ", sizeof(" << shellClassName(java_functional) << "), typeid(" << java_functional->typeEntry()->qualifiedCppName() << "), 0, &deleter_" << QString(java_functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'<', u'_').replace(u'>', u'_') << ");" << Qt::endl;
            }
            s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn) {" << Qt::endl;
            s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
            s << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
        }
        s << "}" << Qt::endl << Qt::endl;

        if(ftype->isFunctionPointer()){
            s << "std::function<const " << shellClassName(java_functional) << "::Functor*(" << java_functional->typeEntry()->qualifiedCppName() << ")> "
              << shellClassName(java_functional) << "::reverseFunctionGetter = std::function<const " << shellClassName(java_functional) << "::Functor*(" << java_functional->typeEntry()->qualifiedCppName() << ")>();" << Qt::endl << Qt::endl;
        }
    }

    writeMetaInfo(s, java_functional);

    writeCodeInjections(s, java_functional->typeEntry(), CodeSnip::End, TS::NativeCode);

    if(java_functional->enclosingClass() && !java_functional->enclosingClass()->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#endif // " << java_functional->enclosingClass()->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    QString pro_file_name = priGenerator->subDirectoryForClass(java_functional, PriGenerator::CppDirectory) + "/generated.pri";
    priGenerator->addSource(pro_file_name, fileNameForFunctional(java_functional));
}

void CppImplGenerator::write(QTextStream &s, const MetaClass *java_class, int) {
    bool shellClass = java_class->generateShellClass();
    bool instantiateShellClass = java_class->instantiateShellClass();
#if 0
    {
        QString fileName("written_classes_cpp.log");
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
    bool hasDeprecation = java_class->isDeclDeprecated();
    if(!hasDeprecation){
        for(const MetaFunction* f : java_class->functions()){
            if(!f->wasPrivate()){
                if(f->isDeclDeprecated()){
                    hasDeprecation = true;
                    break;
                }else{
                    if(f->type() && f->type()->typeEntry()->isDeclDeprecated()){
                        hasDeprecation = true;
                        break;
                    }
                    for(const MetaArgument* arg : f->arguments()){
                        if(arg->type()->typeEntry()->isDeclDeprecated()){
                            hasDeprecation = true;
                            break;
                        }
                    }
                    if(hasDeprecation)
                        break;
                }
            }
        }
        if(!hasDeprecation){
            for(const MetaField* f : java_class->fields()){
                if(f->isDeclDeprecated()){
                    hasDeprecation = true;
                    break;
                }else{
                    if(f->type() && f->type()->typeEntry()->isDeclDeprecated()){
                        hasDeprecation = true;
                        break;
                    }
                }
            }
            if(!hasDeprecation){
                for(const MetaEnum* e : java_class->enums()){
                    if(e->isDeclDeprecated()){
                        hasDeprecation = true;
                        break;
                    }
                }
            }
        }
    }

    QSet<QString> included;
    {
        IncludeList includes = java_class->typeEntry()->extraIncludes();
        if(java_class->typeEntry()->designatedInterface()){
            includes << java_class->typeEntry()->designatedInterface()->extraIncludes();
        }
        std::sort(includes.begin(), includes.end());
        for(const Include& icl : includes){
            if(icl.suppressed)
                writeInclude(s, icl, included);
        }
    }

    if (CppHeaderGenerator::shouldGenerateHeaders(java_class)){
        if(java_class->typeEntry()->designatedInterface() && java_class->enclosingClass()){
            writeInclude(s, Include(Include::LocalPath, QString(java_class->enclosingClass()->name().replace(u'$', u'_'))+"_shell.h"), included);
        }else{
            writeInclude(s, Include(Include::LocalPath, QString(java_class->name().replace(u'$', u'_'))+"_shell.h"), included);
        }
        if(hasDeprecation){
            s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
              << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl << Qt::endl;
        }
    }else{
        writeInclude(s, Include(Include::IncludePath, "QtCore/QtGlobal"), included);
        if(hasDeprecation){
            writeInclude(s, Include(Include::IncludePath, "QtCore/qcompilerdetection.h"), included);
            s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl
              << "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")" << Qt::endl << Qt::endl;
        }
        CppHeaderGenerator::writeInjectedCode(s, java_class, {CodeSnip::Position1});
        writeInclude(s, java_class->typeEntry()->include(), included);
        writeExtraIncludes(s, java_class, included, true);
        writeInclude(s, Include(Include::IncludePath, "QtJambi/QtJambiAPI"), included);
        if(java_class->typeEntry()->isNativeInterface() || (java_class->extractInterface() && java_class->extractInterface()->typeEntry()->isNativeInterface())){
            writeInclude(s, Include(Include::IncludePath, "QtJambi/CoreAPI"), included);
        }
        writeExtraIncludes(s, java_class, included);
        if(java_class->isQWindow()
            || java_class->isQWidget()
            || java_class->isQAction()
            || (java_class->typeEntry()->isThreadAffine() && java_class->typeEntry()->threadAffinity()==QLatin1String("pixmap"))){
            writeInclude(s, Include(Include::IncludePath, QStringLiteral(u"QtJambi/GuiAPI")), included);
        }
        if(java_class->hasPaintMethod())
            writeInclude(s, Include(Include::IncludePath, "QtJambi/AboutToPaint"), included);
//        if(java_class->hasQmlListProperty())
//            writeInclude(s, Include(Include::IncludePath, "QtJambi/QmlAPI"), included);
        writeInclude(s, Include(Include::IncludePath, "QtJambi/RegistryAPI"), included);
    }

    writeInclude(s, Include(Include::IncludePath, "QtJambi/Cast"), included);

    if(!java_class->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    writeClassCodeInjections(s, java_class, CodeSnip::Beginning);

    QList<const MetaFunction *> signalsInTargetLang;

    for(const MetaFunction* signal : java_class->cppSignalFunctions()){
        if (!signal->hasUnresolvedTemplateTypes() && signal->declaringClass() == java_class)
            signalsInTargetLang << signal;
    }

    if(!signalsInTargetLang.isEmpty() && java_class->has_Q_OBJECT()){
        for(int i=0; i<signalsInTargetLang.size(); ++i){
            const MetaFunction* f = signalsInTargetLang.at(i);
            QStringList pps = getFunctionPPConditions(f);
            if(!f->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(f->declaringClass()->typeEntry()->ppCondition())){
                if(pps.isEmpty() || !f->declaringClass()->typeEntry()->ppCondition().contains("|"))
                    pps << f->declaringClass()->typeEntry()->ppCondition();
                else
                    pps << "(" + f->declaringClass()->typeEntry()->ppCondition() + ")";
            }
            pps.removeAll(java_class->typeEntry()->ppCondition());
            QString function_name = f->originalName().replace(' ', '_');
            QString argumentList;
            {
                QTextStream __s(&argumentList);
                writeFunctionArguments(__s, f->arguments(), f, Option(SkipName), int(f->arguments().size()));
            }
            s << Qt::endl
              << "int __signal_method_indexes_" << java_class->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "_" << i << "(){" << Qt::endl;
            if(!pps.isEmpty()){
                s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
            }
            s << "    static int result = ";
            if(java_class->queryFunctionsByOriginalName(f->originalName()).size()>1){
                if(f->isPrivateSignal()){
                    s << f->declaringClass()->typeEntry()->qualifiedCppName()
                      << "::staticMetaObject.indexOfMethod(\"" << function_name << "("
                      << argumentList << ")\");" << Qt::endl;
                }else{
                    s << "QMetaMethod::fromSignal(";
                    if(java_class->queryFunctionsByOriginalName(f->originalName()).size()>1){
                        s << "QOverload<" << argumentList << ">::of(";
                    }
                    s << "&" << f->declaringClass()->typeEntry()->qualifiedCppName() << "::" << function_name;
                    if(java_class->queryFunctionsByOriginalName(f->originalName()).size()>1){
                        s << ")";
                    }
                    s << ").methodIndex();" << Qt::endl;
                }
            }else{
                s << "QMetaMethod::fromSignal(";
                s << "&" << f->declaringClass()->typeEntry()->qualifiedCppName() << "::" << function_name;
                s << ").methodIndex();" << Qt::endl;
            }
            s << "    return result;" << Qt::endl;
            if(!pps.isEmpty()){
                s << "#else" << Qt::endl;
                s << "        return -1;" << Qt::endl;
                s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
            }
            s << "}" << Qt::endl;
            s << Qt::endl;
        }
    }

    writeDeleteAndOwnerFunction(s, java_class);

    bool isInterface = false;
    for(const MetaClass* iface : java_class->interfaces()){
        ImplementorTypeEntry * origin = static_cast<const InterfaceTypeEntry *>(iface->typeEntry())->origin();
        if(origin==java_class->typeEntry()){
            isInterface = true;
            break;
        }
    }

    MetaFunctionList functionsInTargetLang;

    for(MetaFunction *function : java_class->functionsInTargetLang()) {
        if (
            !function->isConstructor()
            && !function->isEmptyFunction()
            && !function->hasUnresolvedTemplateTypes()
            )
            functionsInTargetLang << function;
    }
    for(MetaFunction *function : java_class->queryFunctions(MetaClass::NormalFunctions
                                                                       | MetaClass::AbstractFunctions
                                                                       | MetaClass::NotRemovedFromTargetLang)) {
        if (function->implementingClass() != java_class &&
                !function->hasUnresolvedTemplateTypes() ) {
            functionsInTargetLang << function;
        }
    }

    if (shellClass) {
        if(instantiateShellClass){
            s << "QtJambiShell* " << shellClassName(java_class) << "::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(" << shellClassName(java_class) << ") ); }" << Qt::endl << Qt::endl;
            if(!java_class->implementableFunctions().isEmpty()){
                if(java_class->hasVirtualDestructor()){
                    s << "jmethodID " << oshellClassName(java_class)
                        << "::__shell_javaMethod(int pos) const { return " << shellClassName(java_class) << "::__shell()->javaMethod(typeid(" << java_class->qualifiedCppName() << "), pos); }" << Qt::endl << Qt::endl;
                    if(java_class->typeEntry()->isQAbstractItemModel())
                        s << "ModelData* " << oshellClassName(java_class)
                          << "::__shell_modelData() const {" << Qt::endl
                          << "    QtJambiModelShell* modelShell = dynamic_cast<QtJambiModelShell*>(" << shellClassName(java_class) << "::__shell());" << Qt::endl
                          << "    return modelShell ? modelShell->modelData() : nullptr;" << Qt::endl
                          << "}" << Qt::endl << Qt::endl;
                }else{
                    s << "jmethodID " << shellClassName(java_class)
                        << "::__shell_javaMethod(int pos) const { return " << shellClassName(java_class) << "::__shell()->javaMethod(typeid(" << java_class->qualifiedCppName() << "), pos); }" << Qt::endl << Qt::endl;
                }
            }
        }

        if(!java_class->typeEntry()->isDestructorPrivate() && instantiateShellClass){
            s << "// emitting (writeShellDestructor)" << Qt::endl;
            writeShellDestructor(s, java_class);
            s << "// emitting (writeShellConstructor)" << Qt::endl;
            for(MetaFunction *function : java_class->functions()) {
                if (function->isConstructor() &&
                        !function->isPrivate() &&
                        !function->hasUnresolvedTemplateTypes()){
                    writeShellConstructor(s, isInterface, function, Option(SkipRemovedArguments | PlainShell));
                    /*if(java_class->hasVirtualDestructor()){
                        if(java_class->isQObject()){
                            writeShellConstructor(s, isInterface, function, Option(SkipRemovedArguments | MShell));
                        }
                        writeShellConstructor(s, isInterface, function, Option(SkipRemovedArguments));
                    }*/
                }
            }
        }

        if(instantiateShellClass){
            if(!java_class->isAbstract()){
                s << "void " << shellClassName(java_class) << "::operator delete(void * ptr) noexcept {" << Qt::endl
                  << "    reinterpret_cast<" << shellClassName(java_class) << "*>(ptr)->" << shellClassName(java_class) << "::__shell()->tryDeleteShell(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl
                  << "}" << Qt::endl << Qt::endl;
            }
            if(java_class->hasVirtualDestructor()){
                if(java_class->isQObject() && !java_class->isAbstract()){
                    s << "void " << mshellClassName(java_class) << "::operator delete(void * ptr) noexcept {" << Qt::endl
                      << "    reinterpret_cast<" << mshellClassName(java_class) << "*>(ptr)->" << mshellClassName(java_class) << "::__shell()->tryDeleteShell(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl
                      << "}" << Qt::endl << Qt::endl;
                }
                s << "void " << oshellClassName(java_class) << "::operator delete(void * ptr) noexcept {" << Qt::endl
                  << "    reinterpret_cast<" << oshellClassName(java_class) << "*>(ptr)->" << oshellClassName(java_class) << "::__shell()->tryDeleteShell(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl
                  << "}" << Qt::endl << Qt::endl;
            }

            if (java_class->isQObject() && java_class->qualifiedCppName()!="QDBusInterface") {
                s << "// emitting (writeQObjectFunctions)" << Qt::endl;
                writeQObjectFunctions(s, java_class);
            }
        }

        // Virtual overrides
        s << "// emitting Virtual overrides (virtualFunctions)" << Qt::endl;
        MetaFunctionList virtualFunctions = java_class->virtualFunctions();
        for (int pos = 0; pos < virtualFunctions.size(); ++pos) {
            const MetaFunction *function = virtualFunctions.at(pos);
            if(!function->hasUnresolvedTemplateTypes() && (!function->wasPrivate() || function->isAbstract()))
                writeShellFunction(s, function, java_class, pos);
        }

        // Functions in shell class
        s << "// emitting Functions in shell class (nonVirtualShellFunctions)" << Qt::endl;
        MetaFunctionList shellFunctions = java_class->nonVirtualShellFunctions();
        for (int i = 0; i < shellFunctions.size(); ++i) {
            const MetaFunction *function = shellFunctions.at(i);
            if(!function->hasUnresolvedTemplateTypes())
                writeShellFunction(s, function, java_class, -1);
        }

        // Write public overrides for functions that are protected in the base class
        // so they can be accessed from the native callback
        s << "// emitting Public Override Functions (publicOverrideFunctions)" << Qt::endl;
        for(MetaFunction *function : java_class->publicOverrideFunctions()) {
            if((functionsInTargetLang.contains(function) || signalsInTargetLang.contains(function))
                    && !function->isProxyCall())
                writePublicFunctionOverride(s, function, java_class);
        }

        // Write virtual function overries used to decide on static/virtual calls
        s << "// emitting Virtual Override Functions (virtualOverrideFunctions)" << Qt::endl;
        for(MetaFunction *function : java_class->virtualOverrideFunctions()) {
            if(!function->hasUnresolvedTemplateTypes()
                    && (function->implementingClass()==java_class
                        || function->implementingClass()==java_class->extractInterface()
                        || ( (java_class->isAbstract() || java_class->isInterface()) && function->isAbstract() ))
                    && !(function->isRemovedFrom(java_class, TS::TargetLangCode)
                         || function->isRemovedFrom(function->declaringClass(), TS::TargetLangCode)
                         ))
                writeVirtualFunctionOverride(s, function, java_class);
        }

    }

    if(!java_class->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    s << "// emitting (writeConstructors)" << Qt::endl;
    const MetaFunction * standardConstructor = nullptr;
    if(!java_class->hasUnimplmentablePureVirtualFunction()){
        for(const MetaFunction *function : java_class->queryFunctions(MetaClass::Constructors
                                                                       | MetaClass::Visible
                                                                       | (java_class->isInterface() ? 0 : MetaClass::ClassImplements)
                                                                       | (java_class->isFinal() ? MetaClass::WasPublic : 0))){
            if(!function->isEmptyFunction()
                && !function->hasUnresolvedTemplateTypes()){
                writeFinalConstructor(s, function, java_class);
                if (!function->isModifiedRemoved(TS::NativeCode)
                    && !function->isPrivate()
                    && function->actualMinimumArgumentCount()==0) {
                    standardConstructor = function;
                }
            }
        }
    }

    if(!java_class->isNamespace()
            && !java_class->typeEntry()->isDestructorPrivate()
            && !java_class->hasJustPrivateConstructors()
            && !java_class->hasUnimplmentablePureVirtualFunction()
            && (java_class->generateShellClass() || java_class->typeEntry()->isDestructorPublic())
            ){
        s << "// destruct " << (instantiateShellClass ? shellClassName(java_class) : java_class->fullQualifiedCppName()) << Qt::endl
          << "void __qt_destruct_" << java_class->typeEntry()->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
        s << "(void* ptr)" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"destruct " << java_class->fullQualifiedCppName() << "\")" << Qt::endl;
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
            if(instantiateShellClass){
                s << INDENT << "reinterpret_cast<" << shellClassName(java_class) << "*>(ptr)->~" << shellClassName(java_class).split("::").last() << "();" << Qt::endl;
            }else{
                if(java_class->isTypeAlias() && java_class->templateBaseClass()){
                    s << INDENT << "reinterpret_cast<" << java_class->fullQualifiedCppName() << "*>(ptr)->~" << java_class->templateBaseClass()->qualifiedCppName().split("::").last() << "();" << Qt::endl;
                }else if(java_class->qualifiedCppName().contains("::") && java_class->templateBaseClass()){
                    s << INDENT << "typedef " << java_class->qualifiedCppName() << " DESTRUCTOR;" << Qt::endl
                      << INDENT << "reinterpret_cast<" << java_class->qualifiedCppName() << "*>(ptr)->~DESTRUCTOR();" << Qt::endl;
                }else{
                    s << INDENT << "reinterpret_cast<" << java_class->fullQualifiedCppName() << "*>(ptr)->~" << java_class->fullQualifiedCppName().split("::").last() << "();" << Qt::endl;
                }
            }
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#else" << Qt::endl;
                s << INDENT << "Q_UNUSED(ptr)" << Qt::endl;
                s << Qt::endl << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
        }
        s << "}" << Qt::endl << Qt::endl;
    }

    if(java_class->isQObject() && !java_class->isFinal() && standardConstructor && !java_class->hasUnimplmentablePureVirtualFunction()){
        writeConstructor(s, standardConstructor, {}, java_class, true);

        s << "// emitting  (functionsInTargetLang writePlacementConstructor)" << Qt::endl
          << jni_function_signature(java_class->package(),
                                    java_class->name(),
                                    "initialize_native",
                                    "void", "__"+jni_signature(java_class->typeEntry()->qualifiedTargetLangName(), JNISignatureFormat(NoModification | Underscores))+"Lio_qt_core_QObject_00024QDeclarativeConstructor_2") << Qt::endl
          << "(JNIEnv *__jni_env, jclass __jni_class, jobject __jni_object, jobject placement)" << Qt::endl
          << "{" << Qt::endl;
        QStringList pps = getFunctionPPConditions(standardConstructor);
        if(!java_class->typeEntry()->ppCondition().isEmpty() && !pps.contains(java_class->typeEntry()->ppCondition())){
            if(pps.isEmpty() || !java_class->typeEntry()->ppCondition().contains("|"))
                pps << java_class->typeEntry()->ppCondition();
            else
                pps << "(" + java_class->typeEntry()->ppCondition() + ")";
        }
        if(!pps.isEmpty()){
            s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
        }
        s << "    QTJAMBI_NATIVE_METHOD_CALL(\"" << shellClassName(java_class) << "::" << shellClassName(java_class) << "\")" << Qt::endl
          << "    QTJAMBI_TRY {" << Qt::endl;
        if(standardConstructor->isRethrowExceptions() || standardConstructor->isBlockExceptions() || standardConstructor->isNoExcept())
            s << "        QtJambiExceptionRaiser __qt_exceptionRaiser;" << Qt::endl;
        s << "        jvalue arguments;" << Qt::endl
          << "        arguments.l = placement;" << Qt::endl
          << "        ";
        if(java_class->typeEntry()->isQAbstractItemModel()){
            s << "QtJambiModelShell";
        }else if(java_class->typeEntry()->isQThread()){
            s << "QtJambiThreadShell";
        }else{
            s << "QtJambiShell";
        }
        s << "::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_"
                                                        << java_class->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_')
                                                        << "_declarative, sizeof(" << shellClassName(java_class)
                                                        << "), typeid(" << java_class->qualifiedCppName() << "), "
                                                        << java_class->returnScopeRequired() << ", "
                                                        << java_class->qualifiedCppName() << "::staticMetaObject, "
                                                        << (java_class->generateShellClass() ? "true, " : "false, ")
                                                        << (java_class->hasMetaObjectFunction() ? "true" : "false")
                                                        << ", true, &arguments);" << Qt::endl;
        if(standardConstructor->isRethrowExceptions() || standardConstructor->isBlockExceptions() || standardConstructor->isNoExcept())
            s << "        __qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;
        s << "    } QTJAMBI_CATCH(const JavaException& exn) {" << Qt::endl
          << "        exn.raiseInJava(__jni_env);" << Qt::endl
          << "    } QTJAMBI_TRY_END" << Qt::endl;
        if(!pps.isEmpty()){
            s << "#else" << Qt::endl;
            s << "    Q_UNUSED(__jni_env)" << Qt::endl;
            s << "    Q_UNUSED(__jni_class)" << Qt::endl;
            s << "    Q_UNUSED(__jni_object)" << Qt::endl;
            s << "    Q_UNUSED(placement)" << Qt::endl;
            s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
        }
        s << "}" << Qt::endl << Qt::endl;
    }

    s << "// emitting  (functionsInTargetLang writeFinalFunction)" << Qt::endl;
    for(const MetaFunction *function : functionsInTargetLang) {
        writeFinalFunction(s, function, java_class);
    }

    // Field accessors
    {
        bool hasFields = false;
        for(MetaField *field : java_class->fields()) {
            if (field->wasPublic() || (field->wasProtected() && !java_class->isFinal() && java_class->generateShellClass())){
                if(!hasFields)
                    s << "// emitting Field accessors (writeFieldAccessors)" << Qt::endl;
                hasFields = true;
                writeFieldAccessors(s, field);
            }
        }
    }

    writeCustomStructors(s, java_class->typeEntry());

    s << "// emitting (writeJavaLangObjectOverrideFunctions)" << Qt::endl;
    writeJavaLangObjectOverrideFunctions(s, java_class);

    if (java_class->hasCloneOperator()) {
        s << "// emitting (writeCloneFunction)" << Qt::endl;
        writeCloneFunction(s, java_class);
    }

    QList<const MetaType *> instantiations;
    if(const ContainerTypeEntry* ctype = java_class->findContainerSuperClass(&instantiations)){
        switch(ctype->type()){
        case ContainerTypeEntry::MultiMapContainer:
        case ContainerTypeEntry::MapContainer:
            if(instantiations.size()>1){
                s << Qt::endl
                  << INDENT << jni_function_signature(java_class->package(), java_class->name(), "lessThan", "jboolean")
                  << "(JNIEnv *__jni_env, jobject";
                s << ", " << instantiations.at(0)->typeEntry()->jniName() << " key1";
                s << ", " << instantiations.at(0)->typeEntry()->jniName() << " key2";
                s << INDENT << "){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "bool __java_return_value = false;" << Qt::endl
                      << INDENT << "QTJAMBI_TRY{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(!java_class->typeEntry()->ppCondition().isEmpty()){
                            s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                        }
                        s << INDENT;
                        writeJavaToQt(s, instantiations.at(0), "_qt_key1", "key1", nullptr, nullptr, 0, Option(), "__jni_env");
                        s << INDENT;
                        writeJavaToQt(s, instantiations.at(0), "_qt_key2", "key2", nullptr, nullptr, 0, Option(), "__jni_env");
                        //s << INDENT << "return qMapLessThanKey<";
                        //writeTypeInfo(s, instantiations.at(0), Option());
                        //s << ">(_qt_key1, _qt_key2);" << Qt::endl;
                        s << INDENT << "__java_return_value = _qt_key1 < _qt_key2;" << Qt::endl;
                        if(!java_class->typeEntry()->ppCondition().isEmpty()){
                            s << "#else" << Qt::endl
                              << INDENT << "Q_UNUSED(key1)"
                              << INDENT << "Q_UNUSED(key2)"
                              << "#endif //" << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                        }
                    }
                    s << INDENT << "}QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
                    s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                    s << INDENT << "}QTJAMBI_TRY_END" << Qt::endl
                      << INDENT << "return __java_return_value;" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }
            break;
        default: break;
        }
    }

    for(const MetaEnum *java_enum : java_class->enums()){
        if(!java_enum->isPrivate() && (java_enum->typeEntry()->codeGeneration() & ~TypeEntry::InheritedByTypeSystem)!=TypeEntry::GenerateNothing){
            for(MetaEnumValue *enum_value : java_enum->values()){
                if (java_enum->typeEntry()->isEnumValueRemoveRejected(enum_value->name()))
                    continue;
                if(enum_value->value().userType()==QMetaType::QString){
                    QString jtype;
                    switch(java_enum->typeEntry()->size()){
                    case 8:
                        jtype = "jbyte";
                        break;
                    case 16:
                        jtype = "jshort";
                        break;
                    case 64:
                        jtype = "jlong";
                        break;
                    default:
                        jtype = "jint";
                        break;
                    }
                    QString javaQual = java_enum->typeEntry()->javaQualifier();
                    s << "// emitting  (functionsInTargetLang writePlacementConstructor)" << Qt::endl
                      << jni_function_signature(java_enum->package(),
                                                javaQual.isEmpty() ? java_enum->typeEntry()->targetLangName() : javaQual + '$' + java_enum->typeEntry()->targetLangName(),
                                                "value_" + enum_value->name(),
                                                jtype) << Qt::endl
                      << "(JNIEnv *, jclass) {" << Qt::endl
                      << "    return " << jtype << "(";
                    if(!java_enum->typeEntry()->isScopedEnum()){
                        QStringList qname = java_enum->typeEntry()->qualifiedCppName().split("::");
                        qname.takeLast();
                        s << qname.join("::") << "::";
                    }
                    s << enum_value->cppName() << ");" << Qt::endl
                      << "}" << Qt::endl << Qt::endl;
                }
            }
        }
    }

    writeClassCodeInjections(s, java_class, CodeSnip::End);

    writeMetaInfo(s, java_class, signalsInTargetLang, isInterface);

    QString pro_file_name = priGenerator->subDirectoryForClass(java_class, PriGenerator::CppDirectory) + "/generated.pri";
    priGenerator->addSource(pro_file_name, fileNameForClass(java_class));
    m_nativeConstructorNames.clear();
}

void CppImplGenerator::generateFake(const MetaClass * java_class) {
    for(MetaEnum *_enum : java_class->enums()){
        if(_enum->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp){
            QString fileName = fileNameForClass(java_class);
            BufferedOutputStream s(QFileInfo(resolveOutputDirectory() + "/" + subDirectoryForClass(java_class) + "/" + fileName));
            s << "#include <QtCore/QMetaType>" << Qt::endl
              << "#include <QtCore/QDataStream>" << Qt::endl
              << "#include <QtJambi/RegistryAPI>" << Qt::endl;
            QSet<QString> included;
            included << "<QtCore/QMetaType>"
                     << "<QtCore/QDataStream>"
                     << "<QtJambi/RegistryAPI>";
            for(MetaEnum *cpp_enum : java_class->enums()){
                writeInclude(s, cpp_enum->typeEntry()->include(), included);
            }
            s << Qt::endl;
            writeMetaInfo(s, java_class, QList<const MetaFunction *>{}, false);
            QString pro_file_name = priGenerator->subDirectoryForClass(java_class, PriGenerator::CppDirectory) + "/generated.pri";
            priGenerator->addSource(pro_file_name, fileNameForClass(java_class));
            break;
        }
    }
}

void CppImplGenerator::writeJavaLangObjectOverrideFunctions(QTextStream &s, const MetaClass *cls) {
    if (cls->hasHashFunction() && !cls->isNamespace()) {
        MetaFunctionList hashcode_functions = cls->queryFunctionsByName("hashCode");
        bool found = false;
        for(const MetaFunction *function : hashcode_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            QStringList lines;
            for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                if(codeSnip.language==TS::NativeCode && codeSnip.position==CodeSnip::HashCode){
                    lines << codeSnip.code().split(QStringLiteral(u"\n"));
                }
            }
            s << Qt::endl
              << INDENT;
            if(lines.isEmpty() && !cls->typeEntry()->isNativeIdBased() && !cls->typeEntry()->designatedInterface()){
                s << jni_function_signature(cls->package(), cls->name(), "hashCode", "jint");
            }else{
                s << jni_function_signature(cls->package(), cls->name(), "hashCode_native", "jint");
            }
            s << Qt::endl
              << INDENT << "(JNIEnv * __jni_env, ";
            if(cls->typeEntry()->isNativeIdBased()){
                s << "jclass, QtJambiNativeID __this_nativeId";
            }else if(cls->typeEntry()->designatedInterface()){
                s << "jclass, jobject __this";
            }else{
                s << "jobject __this";
            }
            s << ")" << Qt::endl
            << INDENT << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "jint __java_return_value = 0;" << Qt::endl
                  << INDENT << "QTJAMBI_TRY {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << Qt::endl << "#if " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                    s << INDENT << cls->qualifiedCppName() << " *__qt_this = ";
                    if(cls->typeEntry()->isNativeIdBased()){
                        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::interfaceFromNativeId<" << cls->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        }else{
                            s << "QtJambiAPI::objectFromNativeId<" << cls->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        }
                    }else{
                        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::convertJavaInterfaceToNative<" << cls->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }else if(cls->isQObject()){
                            s << "QtJambiAPI::convertJavaObjectToQObject<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }else{
                            s << "QtJambiAPI::convertJavaObjectToNative<" << cls->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }
                    }
                    s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, __qt_this);" << Qt::endl;
                    s << INDENT << "hash_type hashValue = ";
                    if(!cls->qHashScope().isEmpty())
                        s << cls->qHashScope() << "::";
                    s << "qHash(*__qt_this";
                    if(cls->needsHashWorkaround())
                        s << ", QHashDummyValue()";
                    s << ");" << Qt::endl;
                    if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                        s << INDENT << "__java_return_value = jint(hashValue);" << Qt::endl;
                    }else{
                        s << INDENT << "__java_return_value = jint(quint64(hashValue) ^ quint64(hashValue) >> 32);" << Qt::endl;
                    }
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << "#else" << Qt::endl
                          << INDENT << "Q_UNUSED(";
                        if(cls->typeEntry()->isNativeIdBased()){
                            s << "__this_nativeId";
                        }else{
                            s << "__this";
                        }
                        s << ")" << Qt::endl
                          << "#endif // " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                }
                s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl
                  << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl
                  << INDENT << "} QTJAMBI_TRY_END" << Qt::endl
                  << INDENT << "return __java_return_value;" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }

    // Qt has a standard toString() conversion in QVariant?
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int metaType = QMetaType::type(qPrintable(cls->qualifiedCppName()));
    if (!cls->toStringCapability()
            && metaType!=QMetaType::UnknownType
            && metaType!=QMetaType::QStringList
            && (metaType<=QMetaType::LastCoreType || metaType>QMetaType::LastWidgetsType)
            && QMetaType::sizeOf(metaType)>0
            && QVariant(metaType, nullptr).canConvert(QMetaType::QString)) {
#else
    QMetaType metaType = QMetaType::fromName(qPrintable(cls->qualifiedCppName()));
    if (!cls->toStringCapability() && metaType.isValid() && metaType.id()!=QMetaType::QStringList && QMetaType::canConvert(metaType, QMetaType(QMetaType::QString))) {
#endif
        MetaFunctionList tostring_functions = cls->queryFunctionsByName("toString");
        bool found = false;
        for(const MetaFunction *function : tostring_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            QStringList lines;
            for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                if(codeSnip.language==TS::NativeCode && codeSnip.position==CodeSnip::ToString){
                    lines << codeSnip.code().split(QStringLiteral(u"\n"));
                }
            }
            s << Qt::endl
              << INDENT;
            if(lines.isEmpty() && !cls->typeEntry()->isNativeIdBased() && !cls->typeEntry()->designatedInterface()){
                s << jni_function_signature(cls->package(), cls->name(), "toString", "jstring");
            }else{
                s << jni_function_signature(cls->package(), cls->name(), "toString_native", "jstring");
            }
            s << "(JNIEnv *__jni_env, ";
            if(cls->typeEntry()->isNativeIdBased()){
                s << "jclass, QtJambiNativeID __this_nativeId";
            }else if(cls->typeEntry()->designatedInterface()){
                s << "jclass, jobject __this";
            }else{
                s << "jobject __this";
            }
            s << ")" << Qt::endl
              << INDENT << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "jstring __java_return_value = nullptr;" << Qt::endl
                  << INDENT << "QTJAMBI_TRY {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << Qt::endl << "#if " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                    s << INDENT << cls->qualifiedCppName() << " *__qt_this = ";
                    if(cls->typeEntry()->isNativeIdBased()){
                        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::interfaceFromNativeId<" << cls->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        }else{
                            s << "QtJambiAPI::objectFromNativeId<" << cls->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        }
                    }else{
                        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::convertJavaInterfaceToNative<" << cls->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }else if(cls->isQObject()){
                            s << "QtJambiAPI::convertJavaObjectToQObject<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }else{
                            s << "QtJambiAPI::convertJavaObjectToNative<" << cls->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }
                    }
                    s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, __qt_this);" << Qt::endl;
                    if(cls->qualifiedCppName()=="QString")
                        s << INDENT << "__java_return_value = qtjambi_cast<jstring>(__jni_env, *__qt_this);" << Qt::endl;
                    else
                        s << INDENT << "__java_return_value = qtjambi_cast<jstring>(__jni_env, QVariant(*__qt_this).toString());" << Qt::endl;
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << "#else" << Qt::endl
                          << INDENT << "Q_UNUSED(";
                        if(cls->typeEntry()->isNativeIdBased()){
                            s << "__this_nativeId";
                        }else{
                            s << "__this";
                        }
                        s << ")" << Qt::endl
                          << "#endif // " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                }
                s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
                s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                s << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
                s << INDENT << "return __java_return_value;" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }else{
            writeToStringFunction(s, cls);
        }
    }else{
        writeToStringFunction(s, cls);
    }
}

void CppImplGenerator::writeClassCodeInjections(QTextStream &s, const MetaClass *java_class, CodeSnip::Position position) {
    const ComplexTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    CodeSnipList code_snips = class_type->codeSnips();
    QStringList lines;
    for(const CodeSnip &snip : code_snips) {
        if (snip.position==position && (snip.language == TS::ShellCode || snip.language == TS::NativeCode)) {
            lines << snip.code().replace("%this", "__qt_this").split(QStringLiteral(u"\n"));
        }
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
        s << Qt::endl << "// emitting (writeClassCodeInjections)" << Qt::endl;
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
        if(!line.startsWith(QLatin1Char('#')))
            s << INDENT;
        s << line << Qt::endl;
    }
}

void CppImplGenerator::writeToStringFunction(QTextStream &s, const MetaClass *java_class) {
    const MetaFunction* toStringFun = java_class->toStringCapability();
    bool isQEvent = java_class->isQEvent() && java_class->qualifiedCppName()=="QEvent";

    if (!java_class->hasDefaultToStringFunction() && toStringFun && !isQEvent) {

        auto indirections = toStringFun->arguments().at(1)->type()->indirections().size();
        QString deref = QLatin1String(indirections == 0 ? "*" : "");

        bool hasInjectedJavaCode = false;
        for(const CodeSnip& codeSnip : java_class->typeEntry()->codeSnips()){
            if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::ToString){
                hasInjectedJavaCode = true;
                break;
            }
        }
        s << Qt::endl
          << "#include <QtCore/QDebug>" << Qt::endl
          << INDENT;
        if(!hasInjectedJavaCode && !java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface()){
            s << jni_function_signature(java_class->package(), java_class->name(), "toString", "jstring");
        }else{
            s << jni_function_signature(java_class->package(), java_class->name(), "toString_native", "jstring");
        }
        s << "(JNIEnv *__jni_env, ";
        if(java_class->typeEntry()->designatedInterface()){
            if(java_class->typeEntry()->isNativeIdBased()){
                s << "jclass, QtJambiNativeID __this_nativeId";
            }else{
                s << "jclass, jobject __this";
            }
        }else{
            if(java_class->typeEntry()->isNativeIdBased()){
                s << "jobject, QtJambiNativeID __this_nativeId";
            }else{
                s << "jobject __this";
            }
        }
        s << ")" << Qt::endl
          << INDENT << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "jstring __java_return_value = nullptr;" << Qt::endl
              << INDENT << "QTJAMBI_TRY {" << Qt::endl;
            {
                INDENTATION(INDENT)
                if(!java_class->typeEntry()->ppCondition().isEmpty()){
                    s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                }
                s << INDENT << java_class->qualifiedCppName() << " *__qt_this = ";
                if(java_class->typeEntry()->isNativeIdBased()){
                    if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                        s << "QtJambiAPI::interfaceFromNativeId<" << java_class->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                    }else{
                        s << "QtJambiAPI::objectFromNativeId<" << java_class->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                    }
                }else{
                    if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                        s << "QtJambiAPI::convertJavaInterfaceToNative<" << java_class->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                    }else if(java_class->isQObject()){
                        s << "QtJambiAPI::convertJavaObjectToQObject<" << java_class->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                    }else{
                        s << "QtJambiAPI::convertJavaObjectToNative<" << java_class->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                    }
                }
                s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, __qt_this);" << Qt::endl;

                s << INDENT << "QString res;" << Qt::endl
                  << INDENT << "QDebug(&res).noquote() << " << deref << "__qt_this;" << Qt::endl
                  << INDENT << "if(!res.isEmpty() && res.back().isSpace())" << Qt::endl
                  << INDENT << "    res = res.trimmed();" << Qt::endl
                  << INDENT << "__java_return_value = qtjambi_cast<jstring>(__jni_env, res);" << Qt::endl;
                writeClassCodeInjections(s, java_class, CodeSnip::ToString);
                if(!java_class->typeEntry()->ppCondition().isEmpty()){
                    s << Qt::endl << "#else" << Qt::endl
                      << INDENT << "Q_UNUSED(";
                    if(java_class->typeEntry()->isNativeIdBased()){
                        s << "__this_nativeId";
                    }else{
                        s << "__this";
                    }
                    s << ")" << Qt::endl
                      << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                }
            }
            s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
            s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
            s << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
            s << INDENT << "return __java_return_value;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
    }
}

void CppImplGenerator::writeCloneFunction(QTextStream &s, const MetaClass *java_class) {
    QString name = java_class->name();
    if(java_class->typeEntry() && java_class->typeEntry()->designatedInterface()){
        name = java_class->typeEntry()->designatedInterface()->targetLangName();
    }
    bool hasInjectedJavaCode = false;
    for(const CodeSnip& codeSnip : java_class->typeEntry()->codeSnips()){
        if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::Clone){
            hasInjectedJavaCode = true;
            break;
        }
    }
    QString functionName = "clone";
    if(java_class->typeEntry()->isNativeIdBased() || java_class->typeEntry()->designatedInterface() || hasInjectedJavaCode){
        functionName += "_native";
    }
    s << Qt::endl
      << jni_function_signature(java_class->package(), java_class->name(), functionName, "jobject") << Qt::endl
      << "(JNIEnv *__jni_env, ";
    if(java_class->typeEntry()->designatedInterface()){
        if(java_class->typeEntry()->isNativeIdBased()){
            s << "jclass, QtJambiNativeID __this_nativeId";
        }else{
            s << "jclass, jobject __this";
        }
    }else{
        if(java_class->typeEntry()->isNativeIdBased()){
            s << "jobject, QtJambiNativeID __this_nativeId";
        }else{
            s << "jobject __this";
        }
    }
    s << ")" << Qt::endl
      << INDENT << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "jobject __java_return_value = nullptr;" << Qt::endl
          << INDENT << "QTJAMBI_TRY {" << Qt::endl;
        {
            INDENTATION(INDENT)
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
            s << INDENT << "const " << java_class->typeEntry()->qualifiedCppName() << " *__qt_this = ";
            if(java_class->typeEntry()->isNativeIdBased()){
                if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                    s << "QtJambiAPI::interfaceFromNativeId<" << java_class->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                }else{
                    s << "QtJambiAPI::objectFromNativeId<" << java_class->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                }
            }else{
                if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                    s << "QtJambiAPI::convertJavaInterfaceToNative<" << java_class->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                }else if(java_class->isQObject()){
                    s << "QtJambiAPI::convertJavaObjectToQObject<" << java_class->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                }else{
                    s << "QtJambiAPI::convertJavaObjectToNative<" << java_class->typeEntry()->qualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                }
            }
            s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, __qt_this);" << Qt::endl;
            if(java_class->qualifiedCppName()=="QString"){
                s << INDENT << "__java_return_value = QtJambiAPI::convertQStringToJavaObject(__jni_env, *__qt_this);" << Qt::endl;
            }else if(java_class->qualifiedCppName()=="QChar"){
                s << INDENT << "__java_return_value = QtJambiAPI::convertQCharToJavaObject(__jni_env, *__qt_this);" << Qt::endl;
            }else{
                bool isMetaTypeRegistered = false;
                if(java_class->isPublic() && !java_class->isAbstract() && !java_class->typeEntry()->skipMetaTypeRegistration()){
                    isMetaTypeRegistered = java_class->typeEntry()->isValue()
                                                || ((java_class->typeEntry()->hasPublicCopyConstructor()
                                                       || java_class->typeEntry()->customConstructor(ComplexTypeEntry::CopyConstructor).hasCode())
                                                   && (java_class->typeEntry()->hasPublicDefaultConstructor()
                                                       || java_class->typeEntry()->customConstructor(ComplexTypeEntry::DefaultConstructor).hasCode())
                                                   && (java_class->typeEntry()->isDestructorPublic()
                                                       || java_class->typeEntry()->customDestructor().hasCode())
                                                    && !java_class->typeEntry()->skipMetaTypeRegistration());
                }
                if (!isMetaTypeRegistered) {
    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    QMetaType metaType(QMetaType::type(java_class->typeEntry()->name().toLocal8Bit().constData()));
    #else
                    QMetaType metaType = QMetaType::fromName(java_class->typeEntry()->name().toLocal8Bit().constData());
    #endif
                    if(metaType.isValid() && metaType.id()<QMetaType::HighestInternalId)
                        isMetaTypeRegistered = true;
                }
                if(isMetaTypeRegistered){
                    s << INDENT << "__java_return_value = qtjambi_cast<jobject>(__jni_env, *__qt_this);" << Qt::endl;
                }else{
                    s << INDENT << "__java_return_value = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(__jni_env, new " << java_class->fullQualifiedCppName() << "(*__qt_this));" << Qt::endl;
                }
            }
            writeClassCodeInjections(s, java_class, CodeSnip::Clone);
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#else" << Qt::endl
                  << INDENT << "Q_UNUSED(";
                if(java_class->typeEntry()->isNativeIdBased()){
                    s << "__this_nativeId";
                }else{
                    s << "__this";
                }
                s << ")" << Qt::endl
                  << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
        }
        s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
        s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
        s << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
        s << INDENT << "return __java_return_value;" << Qt::endl;
    }
    s << INDENT << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeQObjectFunctions(QTextStream &s, const MetaClass *java_class) {
    if(!java_class->hasMetaObjectFunction()){
        // QObject::metaObject()
        s << "const QMetaObject *" << mshellClassName(java_class) << "::metaObject() const" << Qt::endl
          << "{" << Qt::endl
          << "  return " << shellClassName(java_class) << "::__shell()->metaObject();" << Qt::endl
          << "}" << Qt::endl << Qt::endl;
    }

    if(!java_class->hasMetaCastFunction()){
        // QObject::qt_metacast()
        s << "void *" << mshellClassName(java_class) << "::qt_metacast(const char *_clname)" << Qt::endl
          << "{" << Qt::endl
          << "    QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL(\"" << mshellClassName(java_class) << "::qt_metacast(const char *_clname)\", this)" << Qt::endl
          << "    bool ok = false;" << Qt::endl
          << "    void *rv = " << shellClassName(java_class) << "::__shell()->qt_metacast(_clname, &ok);" << Qt::endl
          << "    if (!ok)" << Qt::endl
          << "        rv = " << java_class->qualifiedCppName() << "::qt_metacast(_clname);" << Qt::endl
          << "    return rv;" << Qt::endl
          << "}" << Qt::endl << Qt::endl;
    }

    if(!java_class->hasMetaCallFunction()){
        // QObject::qt_metacall()
        s << "int " << mshellClassName(java_class) << "::qt_metacall(QMetaObject::Call _c, int _id, void **_a)" << Qt::endl
          << "{" << Qt::endl;

        s << "    QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL(\"" << mshellClassName(java_class) << "::qt_metacall(QMetaObject::Call _c, int _id, void **_a)\", this)" << Qt::endl;

        if (java_class->hasVirtualSlots()) {
            s << "    typedef void(*VirtualSlot)(" << shellClassName(java_class) << " *,void **);" << Qt::endl
              << "    static QHash<int,VirtualSlot> virtualSlots = []()->QHash<int,VirtualSlot>{" << Qt::endl
              << "            QHash<int,VirtualSlot> virtualSlots;" << Qt::endl
              << "            int idx;" << Qt::endl;
            MetaFunctionList virtualFunctions = java_class->virtualFunctions();
            for (int pos = 0; pos < virtualFunctions.size(); ++pos) {
                const MetaFunction *virtualFunction = virtualFunctions.at(pos);
                if (virtualFunction->isVirtualSlot()) {
                    const MetaArgumentList& arguments = virtualFunction->arguments();
                    MetaType *function_type = virtualFunction->type();
                    s << "            idx = " << java_class->qualifiedCppName() << "::staticMetaObject.indexOfMethod(\"" << virtualFunction->minimalSignature() << "\") - " << java_class->qualifiedCppName() << "::staticMetaObject.methodOffset();" << Qt::endl;
                    s << "            if(idx>=0)" << Qt::endl;
                    s << "                virtualSlots[idx] = [](" << shellClassName(java_class) << "*_this, void **";
                    if(function_type || !arguments.isEmpty())
                        s << "_a";
                    s << "){" << Qt::endl;
                    // Set up variable names so the function call works
                    for (int i = 1; i <= arguments.size(); ++i) {
                        MetaArgument *argument = arguments.at(i - 1);

                        s << "                                ";
                        writeTypeInfo(s, argument->type());
                        s << " __qt_" << argument->indexedName() << " = *reinterpret_cast<";
                        writeTypeInfo(s, argument->type());
                        s << " *>(_a[" << i << "]);" << Qt::endl;
                    }

                    // Function call
                    s << "                                ";
                    if (function_type) {
                        writeTypeInfo(s, virtualFunction->type());
                        s << " _r = ";
                    }

                    writeFunctionCall(s, "_this", virtualFunction);
                    s << ";" << Qt::endl;
                    if (function_type) {
                        s << "                                if (_a[0] != nullptr)" << Qt::endl
                          << "                                    *reinterpret_cast<";
                        writeTypeInfo(s, virtualFunction->type());
                        s << " *>(_a[0]) = _r;" << Qt::endl;
                    }
                    s << "                            };" << Qt::endl;
                }
            }
            s << "            return virtualSlots;" << Qt::endl
              << "        }();" << Qt::endl
              << "    if(VirtualSlot virtualSlot = virtualSlots.value(_id, nullptr)) {" << Qt::endl
              << "        virtualSlot(this, _a);" << Qt::endl
              << "        return -1;" << Qt::endl
              << "    }" << Qt::endl;
        }

        s << "    _id = " << java_class->qualifiedCppName() << "::qt_metacall(_c, _id, _a);" << Qt::endl
          << "    if (_id >= 0)" << Qt::endl
          << "        _id = " << shellClassName(java_class) << "::__shell()->qt_metacall(_c, _id, _a);" << Qt::endl;
        s << "    return _id;" << Qt::endl
          << "}" << Qt::endl << Qt::endl;
    }
}

void CppImplGenerator::writeShellConstructor(QTextStream &s, bool, const MetaFunction *java_function, Option options) {
    if (java_function->isModifiedRemoved(TS::ShellCode))
        return;

    const MetaClass *cls = java_function->ownerClass();

    const MetaArgumentList& arguments = java_function->arguments();

    writeFunctionSignature(s, java_function, cls, /*default-arg*/QString(), java_function->isPublic() ? Option(options) : Option(options | EnumAsInts));

    s << Qt::endl;
    s << "    : ";
    if ((options & PlainShell)){
        s << cls->qualifiedCppName();
    }else if ((options & MShell)){
        s << shellClassName(cls);
    }else{
        if(cls->isQObject()){
            s << mshellClassName(cls);
        }else{
            s << shellClassName(cls);
        }
    }
    s << "(";
    bool needComma = false;
    for (int i = 0; i < arguments.size(); ++i) {
        const MetaArgument* argument = arguments.at(i);
        if ((options & SkipRemovedArguments) == SkipRemovedArguments
                && java_function->argumentRemoved(argument->argumentIndex() + 1)!=ArgumentRemove_No) {
            if(!arguments.at(i)->originalDefaultValueExpression().isEmpty()
                && java_function->conversionRule(TS::NativeCode, argument->argumentIndex() + 1).isEmpty()
                && java_function->conversionRule(TS::TargetLangCode, argument->argumentIndex() + 1).isEmpty()){
                continue;
            }
        }
        if(needComma)
            s << ", ";
        else
            needComma = true;
        if(argument->type()->getReferenceType()==MetaType::RReference){
            s << "std::move(" << argument->indexedName() << ")";
        }else if(!java_function->isPublic() && (argument->type()->typeEntry()->isEnum() || argument->type()->typeEntry()->isFlags()) ){
            s << argument->type()->typeEntry()->qualifiedCppName() << "(" << argument->indexedName() << ")";
        }else{
            s << argument->indexedName();
        }
    }
    s << ")" << Qt::endl;
    s << "{" << Qt::endl;
    if (options & PlainShell){
        INDENTATION(INDENT)

        s << INDENT << "QTJAMBI_IN_CONSTRUCTOR_CALL(\"" << cls->qualifiedCppName() << "::";
        writeFunctionSignature(s, java_function, nullptr, {}, Option(PlainShell | OriginalName));
        s << "\", this)" << Qt::endl;
        s << INDENT << shellClassName(cls) << "::__shell()->constructed(typeid(" << cls->qualifiedCppName() << "));" << Qt::endl;

        writeCodeInjections(s, java_function, cls, CodeSnip::Beginning, TS::ShellCode, "__jni_env", "__qtjambi_scope");
        writeCodeInjections(s, java_function, cls, CodeSnip::End, TS::ShellCode, "__jni_env", "__qtjambi_scope");
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeShellConstructor(QTextStream &s, const MetaFunctional *java_class) {
    QString javaName = java_class->name();
    if(java_class->enclosingClass()){
        javaName = java_class->enclosingClass()->fullName() + "$" + javaName;
    }
    javaName = javaName.replace(".", "/");
    s << "void deleter_" << QString(java_class->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'<', u'_').replace(u'>', u'_') << "(void *ptr,bool) { delete reinterpret_cast<" << shellClassName(java_class) << "*>(ptr); }" << Qt::endl << Qt::endl;
    s << Qt::endl;
    s << shellClassName(java_class) << "::" << shellClassName(java_class)
      << "()" << Qt::endl
      << "    : FunctionalBase()" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_IN_CONSTRUCTOR_CALL(\"" << shellClassName(java_class) << "::" << shellClassName(java_class) << "()\", this)" << Qt::endl;
        s << INDENT << shellClassName(java_class) << "::__shell()->constructed(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeShellDestructor(QTextStream &s, const MetaClass *java_class) {
    auto writeExtraLines = [](const QStringList& lines, QTextStream &s, const MetaClass *java_class, const QString& shellClassName){
        int sp = -1;
        QString spaces;
        //if(!lines.isEmpty())
        //    s << Qt::endl;
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
            if(!line.startsWith(QLatin1Char('#')))
                s << INDENT;
            line = line .replace("%this", "this")
                        .replace("%class", java_class->qualifiedCppName())
                        .replace("%shellclass", shellClassName)
                        .replace("%mshellclass", shellClassName)
                        .replace("%oshellclass", shellClassName);
            s << line << Qt::endl;
        }
    };
    QStringList lines;
    QSet<const ComplexTypeEntry *> classes;
    const MetaClass *_cls = java_class;
    while(_cls){
        classes << (_cls->typeEntry()->designatedInterface() ? _cls->typeEntry()->designatedInterface() : _cls->typeEntry());
        for(const MetaClass *ifc : _cls->interfaces()){
            classes << (ifc->typeEntry()->designatedInterface() ? ifc->typeEntry()->designatedInterface() : ifc->typeEntry());
        }
        _cls = _cls->baseClass();
    }
    for(const ComplexTypeEntry * _cls : qAsConst(classes)){
        for(const CodeSnip& snip : _cls->codeSnips()) {
            if (snip.language == TS::DestructorFunction) {
                lines << snip.code().split(QStringLiteral(u"\n"));
            }
        }
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

    QString shellClassName = CppImplGenerator::shellClassName(java_class);
    s << shellClassName << "::~" << shellClassName << "()" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_IN_DESTRUCTOR_CALL(\""
          << java_class->qualifiedCppName() << "::~"
          << java_class->qualifiedCppName() << "()\", this)" << Qt::endl;
        writeExtraLines(lines, s, java_class, shellClassName);

        s << INDENT << shellClassName << "::__shell()->destructed(typeid(" << java_class->qualifiedCppName() << "));" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
    if(java_class->needsOShellDestructor()){
        shellClassName = CppImplGenerator::oshellClassName(java_class);
        s << shellClassName << "::~" << shellClassName << "()" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            writeExtraLines(lines, s, java_class, shellClassName);
        }
        s << "}" << Qt::endl << Qt::endl;
    }
}

void CppImplGenerator::writeShellDestructor(QTextStream &s, const MetaFunctional *java_class) {
    QString shellClassName = CppImplGenerator::shellClassName(java_class);
    s << shellClassName << "::~" << shellClassName << "()" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_IN_DESTRUCTOR_CALL(\""
          << shellClassName << "::~"
          << shellClassName << "()\", this)" << Qt::endl;
        const FunctionalTypeEntry *ftype = reinterpret_cast<const FunctionalTypeEntry *>(java_class->typeEntry());
        if(ftype->isFunctionPointer()){
            s << INDENT << "if(m_functionPointerDeleter)" << Qt::endl;
            s << INDENT << "    m_functionPointerDeleter();" << Qt::endl;
        }
        QStringList lines;
        for(const CodeSnip& snip : java_class->typeEntry()->codeSnips()) {
            if (snip.language == TS::DestructorFunction) {
                lines << snip.code().replace("%this", "this")
                         .replace("%shellclass", shellClassName)
                         .replace("%mshellclass", shellClassName)
                         .replace("%oshellclass", shellClassName)
                         .split(QStringLiteral(u"\n"));
            }
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
        //if(!lines.isEmpty())
        //    s << Qt::endl;
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
            if(!line.startsWith(QLatin1Char('#')))
                s << INDENT;
            s << line << Qt::endl;
        }
        s << INDENT << shellClassName << "::__shell()->destructed(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeCodeInjections(QTextStream &s,
        const TypeEntry *typeEntry, CodeSnip::Position position,
        TS::Language language) {
    const CodeSnipList* codeSnips = nullptr;
    if(typeEntry->isComplex()){
        codeSnips = &static_cast<const ComplexTypeEntry*>(typeEntry)->codeSnips();
    }else if(typeEntry->isFunctional()){
        codeSnips = &static_cast<const FunctionalTypeEntry*>(typeEntry)->codeSnips();
    }else return;
    for(const CodeSnip& snip : *codeSnips) {
        if (snip.position != position)
            continue ;

        if ((snip.language & language) == false)
            continue ;

        if (position == CodeSnip::End)
            s << Qt::endl;

        QString code = snip.code();
        ArgumentMap map = snip.argumentMap;
        ArgumentMap::const_iterator it = map.constBegin();
        for (;it != map.constEnd();++it) {
            QString debug = QString("argument map specifies invalid argument index %1"
                                    "for class '%2'")
                            .arg(it.key()).arg(typeEntry->name());
            ReportHandler::warning(debug);
        }
        QStringList lines = code.split(QStringLiteral(u"\n"));
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
            if(!line.startsWith(QLatin1Char('#')))
                s << INDENT;
            s << line << Qt::endl;
        }
        if (position == CodeSnip::Beginning)
            s << Qt::endl;
    }
}


void CppImplGenerator::writeCodeInjections(QTextStream &s, const MetaFunction *java_function,
        const MetaClass *implementor, CodeSnip::Position position,
        TS::Language language, const QString& __jni_env, const QString& qtjambi_scope, Option option) {

    FunctionModificationList mods;
    const MetaClass *cls = implementor;
    while (cls!= nullptr) {
        mods += java_function->modifications(cls);

        if (cls == cls->baseClass() || java_function->isStatic())
            break;
        cls = cls->baseClass();
    }
    QString def;
    if(java_function->type()){
        switch(language){
        case TS::ShellCode:
            def = default_return_statement_qt(java_function->type(), AbstractGenerator::NoReturnStatement);
            break;
        case TS::NativeCode:
            def = default_return_statement_java(java_function->type(), AbstractGenerator::NoReturnStatement);
            break;
        default:
            break;
        }
    }

    for(const FunctionModification& mod : mods) {
        if (mod.snips.count() <= 0)
            continue ;

        for(const CodeSnip& snip : mod.snips) {
            if (snip.position != position)
                continue ;

            if ((snip.language & language) == false)
                continue ;

            if (position == CodeSnip::End)
                s << Qt::endl;

            QString code = snip.code();
            if(!def.isEmpty()){
                code = code.replace("$DEFAULT_VALUE_RETURN", def);
            }
            code = code.replace("%shellclass", shellClassName(implementor));
            code = code.replace("%oshellclass", oshellClassName(implementor));
            code = code.replace("%mshellclass", mshellClassName(implementor));
            code = code.replace("%class", implementor->qualifiedCppName());
            code = code.replace("%this", "__qt_this");
            code = code.replace("%env", __jni_env);
            code = code.replace("%scope", (option & OptionalScope) == OptionalScope ? "*"+qtjambi_scope : qtjambi_scope);
            ArgumentMap map = snip.argumentMap;
            ArgumentMap::iterator it = map.begin();
            for (;it != map.end();++it) {
                int pos = it.key() - 1;
                QString meta_name = it.value();

                if(pos == -1){
                    code = code.replace(meta_name, "__qt_return_value");
                }else if (pos >= 0 && pos < java_function->arguments().count()) {
                    code = code.replace(meta_name, java_function->arguments().at(pos)->indexedName());
                } else {
                    QString debug = QString("argument map specifies invalid argument index %1"
                                            "for function '%2'")
                                    .arg(pos + 1).arg(java_function->name());
                    ReportHandler::warning(debug);
                }

            }
            QStringList lines = code.split(QStringLiteral(u"\n"));
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
                if(!line.startsWith(QLatin1Char('#')))
                    s << INDENT;
                s << line << Qt::endl;
            }
            if (position == CodeSnip::Beginning)
                s << Qt::endl;
        }
    }
    if(language==TS::NativeCode
            && position==CodeSnip::End
            && !java_function->isStatic()
            && !java_function->isSignal()
            && !java_function->isConstructor()
            && implementor->typeEntry()->isQObject()){
        bool force = false;
        QStringList conditions;
        for (const MetaArgument* argument : java_function->arguments()) {
            cls = implementor;
            while (cls!= nullptr) {
                const QList<ReferenceCount> rcList = java_function->referenceCounts(cls, argument->argumentIndex()+1);
                for(const ReferenceCount& rc : rcList){
                    if(rc.action!=ReferenceCount::Ignore
                            && rc.action!=ReferenceCount::Remove){
                        if(rc.action == ReferenceCount::AddAll
                                || rc.action == ReferenceCount::ClearAddAll){
                            if(argument
                                    && argument->type()->instantiations().size()==1
                                    && !argument->type()->instantiations()[0]->indirections().isEmpty()
                                    && argument->type()->instantiations()[0]->isObject()){
                                QString condition;
                                {
                                    QTextStream s(&condition);
                                    s << "(QtJambiAPI::isSplitOwnership(";
                                    if(implementor->typeEntry()->isNativeIdBased()){
                                        s << "__this_nativeId";
                                    }else{
                                        s << "__jni_env, __this";
                                    }
                                    s << ")" << Qt::endl;
                                    {
                                        INDENTATION(INDENT)
                                        s << INDENT << "&& [](";
                                        writeTypeInfo(s, argument->type(), Option(ForceConstReference));
                                        s << " container)->bool{" << Qt::endl;
                                        {
                                            INDENTATION(INDENT)
                                            {
                                                INDENTATION(INDENT)
                                                s << INDENT << "for(";
                                                writeTypeInfo(s, argument->type()->instantiations()[0], Option(ForceConstReference));
                                                s << "element : container){" << Qt::endl
                                                  << INDENT << "    if(QtJambiAPI::isJavaOwnership(element))" << Qt::endl
                                                  << INDENT << "        return true;" << Qt::endl
                                                  << INDENT << "}" << Qt::endl
                                                  << INDENT << "return false;" << Qt::endl;
                                            }
                                            s << INDENT << "}(__qt_" << argument->indexedName() << "))";
                                        }
                                    }
                                }
                                conditions << condition;
                            }else{
                                force = true;
                            }
                        }else if(java_function->typeReplaced(argument->argumentIndex()+1).isEmpty()
                                && !java_function->useArgumentAsBuffer(argument->argumentIndex()+1)
                                && !java_function->useArgumentAsArray(argument->argumentIndex()+1)
                                && !argument->type()->isTargetLangEnum()
                                && !argument->type()->isTargetLangFlags()){
                            force = true;
                        }else{
                            if(!rc.condition.isEmpty()){
                                QString condition = rc.condition;
                                condition = condition.replace(QStringLiteral(u"%in"), QStringLiteral(u"__qt_")+argument->indexedName());
                                condition = condition.replace(QStringLiteral(u"%arg"), QStringLiteral(u"__qt_")+argument->indexedName());
                                condition = condition.replace(QStringLiteral(u"%this"), QStringLiteral(u"__qt_this"));
                                condition = condition.replace(QStringLiteral(u"%0"), QStringLiteral(u"__java_return_value"));
                                condition = condition.replace(QStringLiteral(u"%return"), QStringLiteral(u"__java_return_value"));
                                for(const MetaArgument* argument : java_function->arguments()){
                                    condition = condition.replace(QStringLiteral(u"%")+QString::number(argument->argumentIndex()+1), QStringLiteral(u"__qt_")+argument->indexedName());
                                }
                                if(argument->type()->typeEntry()->isNativeIdBased())
                                    conditions << QStringLiteral(u"(QtJambiAPI::isJavaOwnership(%1) && %2)").arg(argument->indexedName(), condition);
                                else
                                    conditions << QStringLiteral(u"(QtJambiAPI::isJavaOwnership(__jni_env, %1) && %2)").arg(argument->indexedName(), condition);
                            }else{
                                if(argument->type()->typeEntry()->isNativeIdBased())
                                    conditions << QStringLiteral(u"QtJambiAPI::isJavaOwnership(%1)").arg(argument->indexedName());
                                else
                                    conditions << QStringLiteral(u"QtJambiAPI::isJavaOwnership(__jni_env, %1)").arg(argument->indexedName());
                            }
                        }
                        cls = nullptr;
                        break;
                    }
                }
                if(!cls)
                    break;
                if (cls == cls->baseClass())
                    break;
                cls = cls->baseClass();
            }
        }
        if(force){
            if(implementor->typeEntry()->isNativeIdBased()){
                s << INDENT << "QtJambiAPI::changeSplitToCppOwnership(__jni_env, __this_nativeId);" << Qt::endl;
            }else{
                s << INDENT << "QtJambiAPI::changeSplitToCppOwnership(__jni_env, __this);" << Qt::endl;
            }
        }else if(!conditions.isEmpty()){
            if(conditions.size()==1 && conditions[0].startsWith(u'(') && conditions[0].endsWith(u')'))
                s << INDENT << "if" << conditions[0] << "{" << Qt::endl;
            else
                s << INDENT << "if(" << conditions.join(QStringLiteral(u" || ")) << "){" << Qt::endl;
            if(implementor->typeEntry()->isNativeIdBased()){
                s << INDENT << "    QtJambiAPI::changeSplitToCppOwnership(__jni_env, __this_nativeId);" << Qt::endl;
            }else{
                s << INDENT << "    QtJambiAPI::changeSplitToCppOwnership(__jni_env, __this);" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }
}

bool CppImplGenerator::hasCodeInjections(const MetaFunction *java_function,
        const MetaClass *implementor, const QSet<CodeSnip::Position>& positions,
        TS::Language language) {

    const MetaClass *cls = implementor;
    while (cls!= nullptr) {
        if(java_function->hasCodeInjections(cls, language, positions))
            return true;
        if(language==TS::NativeCode
                && positions.size()==1
                && positions.contains(CodeSnip::End)
                && !java_function->isSignal()
                && !java_function->isConstructor()
                && !java_function->isStatic()
                && implementor->typeEntry()->isQObject()){
            for (const MetaArgument* argument : java_function->arguments()) {
                const QList<ReferenceCount> rcList = java_function->referenceCounts(cls, argument->argumentIndex()+1);
                for(const ReferenceCount& rc : rcList){
                    if(rc.action!=ReferenceCount::Ignore
                            && rc.action!=ReferenceCount::Remove){
                        return true;
                    }
                }
            }
        }
        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    return false;
}

bool CppImplGenerator::hasCodeInjections(const MetaFunctional *java_function,
        const QSet<CodeSnip::Position>& positions,
        TS::Language language) {
    Q_UNUSED(java_function)
    Q_UNUSED(positions)
    Q_UNUSED(language)
    return false;
}

bool CppImplGenerator::hasOwnership(const MetaFunction *java_function,
                                    const MetaFunctional *java_functional,
                    int var_index,
                    const MetaClass *implementor,
                    TS::Language language)
{
    OwnershipRule ownershipRule = {TS::InvalidOwnership, QString()};
    if(java_functional){
        ownershipRule = java_functional->ownership(language, var_index);
    }else{
        const MetaClass *cls = implementor;
        while (cls!= nullptr && ownershipRule.ownership == TS::InvalidOwnership) {
            ownershipRule = java_function->ownership(cls, language, var_index);
            cls = cls->baseClass();
        }
    }
    return ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership;
}

void CppImplGenerator::writeOwnership(QTextStream &s,
                                      const MetaFunction *java_function,
                                      const MetaFunctional *java_functional,
                                      const QString &var_name,
                                      const QString &qt_var_name,
                                      int var_index,
                                      const MetaClass *implementor,
                                      TS::Language language,
                                      const QString& __jni_env,
                                      const QString& qtjambi_scope) {
    Q_UNUSED(qtjambi_scope)
    OwnershipRule ownershipRule = {TS::InvalidOwnership, QString()};
    if(java_functional){
        ownershipRule = java_functional->ownership(language, var_index);
    }else{
        const MetaClass *cls = implementor;
        while (cls!= nullptr && ownershipRule.ownership == TS::InvalidOwnership) {
            ownershipRule = java_function->ownership(cls, language, var_index);
            cls = cls->baseClass();
        }
    }

    if (ownershipRule.ownership == TS::InvalidOwnership || ownershipRule.ownership == TS::IgnoreOwnership)
        return;

    if (var_index != -1) {
        const MetaType* type{nullptr};
        if(var_index==0){
            type = java_function->type();
        }else{
            for(auto arg : java_function->arguments()){
                if(arg->argumentIndex()==var_index-1){
                   type = arg->type();
                   break;
                }
            }
        }
        if(type){
            switch(ownershipRule.ownership){
            case TS::RegisterDependency:
                if(!ownershipRule.condition.isEmpty()){
                    s << INDENT << "if(" << ownershipRule.condition << ")" << Qt::endl
                      << INDENT << "    QtJambiAPI::registerDependency(" << __jni_env << ", " << var_name << ", __this_nativeId);" << Qt::endl;
                }else{
                    s << INDENT << "QtJambiAPI::registerDependency(" << __jni_env << ", " << var_name << ", __this_nativeId);" << Qt::endl;
                }
                break;
            default:
                if (type->isContainer() && ownershipRule.ownership != TS::Invalidate){
                    if(type->instantiations().size()==1){
                        bool isQObject = (type->instantiations()[0]->typeEntry()->isComplex()
                                && static_cast<const ComplexTypeEntry*>(type->instantiations()[0]->typeEntry())->isQObject());
                        QString indent;
                        if(!ownershipRule.condition.isEmpty()){
                            s << INDENT << "if(" << ownershipRule.condition << ") {" << Qt::endl;
                            indent = "    ";
                        }
                        s << INDENT << indent << "for(auto __element : " << qt_var_name << "){" << Qt::endl
                          << INDENT << indent << "    ";
                        switch(ownershipRule.ownership){
                        case TS::TargetLangOwnership:
                            s << "QtJambiAPI::setJavaOwnership"; break;
                        case TS::CppOwnership:
                            s << "QtJambiAPI::setCppOwnership";
                            if(!isQObject && var_index==0 && language==TS::ShellCode)
                                s << "AndInvalidate";
                            break;
                        default:
                            s << "QtJambiAPI::setDefaultOwnership"; break;
                        }
                        if(isQObject)
                            s << "ForTopLevelObject";
                        s << "(" << __jni_env << ", ";
                        if(isQObject && type->instantiations()[0]->indirections().isEmpty())
                            s << "&";
                        s << "__element);" << Qt::endl;
                        /*if(ownershipRule.ownership==TS::CppOwnership && !isQObject && !type->instantiations()[0]->indirections().isEmpty()){
                            if(var_index>0 && language==TS::NativeCode){
                                s << INDENT << indent << qtjambi_scope << ".addObjectInvalidation(" << __jni_env << ", __element);" << Qt::endl;
                            }
                        }*/
                        s << INDENT << indent << "}" << Qt::endl;
                        if(!ownershipRule.condition.isEmpty())
                            s << INDENT << "}" << Qt::endl;
                    }
                }else if (type->isSmartPointer() && ownershipRule.ownership != TS::Invalidate){
                    if(type->instantiations().size()==1){
                        bool isQObject = (type->instantiations()[0]->typeEntry()->isComplex()
                                && static_cast<const ComplexTypeEntry*>(type->instantiations()[0]->typeEntry())->isQObject());
                        QString indent;
                        if(!ownershipRule.condition.isEmpty()){
                            indent = "    ";
                            s << INDENT << "if(" << ownershipRule.condition << "){" << Qt::endl;
                        }
                        switch(ownershipRule.ownership){
                        case TS::TargetLangOwnership:
                            s << INDENT << indent << "QtJambiAPI::setJavaOwnership"; break;
                        case TS::CppOwnership:
                            s << INDENT << indent << "QtJambiAPI::setCppOwnership";
                            if(!isQObject && var_index==0 && language==TS::ShellCode)
                                s << "AndInvalidate";
                            break;
                        default:
                            s << INDENT << indent << "QtJambiAPI::setDefaultOwnership"; break;
                        }
                        if(isQObject){
                            s << "ForTopLevelObject(" << __jni_env << ", " << qt_var_name;
                            const SmartPointerTypeEntry* pentry = static_cast<const SmartPointerTypeEntry*>(type->typeEntry());
                            switch(pentry->type()){
                            case SmartPointerTypeEntry::QScopedPointer:
                                if(var_index==0 && language==TS::ShellCode)
                                    break;
                                if(var_index>0 && language==TS::NativeCode)
                                    break;
                                Q_FALLTHROUGH();
                            default:
                                s << ".get()";
                                break;
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << "(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                        }
                        if(ownershipRule.ownership==TS::CppOwnership && !isQObject){
                            // (var_index==0 && language==TS::ShellCode) already done
                            if(var_index>0 && language==TS::NativeCode){
                                if(ownershipRule.condition.isEmpty())
                                    s << INDENT << indent << "QTJAMBI_INVALIDATE_AFTER_USE(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                                else{
                                    s << INDENT << indent << qtjambi_scope << ".addObjectInvalidation(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                                }
                            }
                        }
                        if(!ownershipRule.condition.isEmpty())
                            s << INDENT << "}" << Qt::endl;
                    }
                }else{
                    if(type->isQObject() && ownershipRule.ownership == TS::Invalidate)
                        return;
                    QString indent;
                    if(!ownershipRule.condition.isEmpty()){
                        indent = "    ";
                        s << INDENT << "if(" << ownershipRule.condition << ") {" << Qt::endl;
                    }
                    switch(ownershipRule.ownership){
                    case TS::TargetLangOwnership:
                        s << INDENT << indent << "QtJambiAPI::setJavaOwnership"; break;
                    case TS::CppOwnership:
                        s << INDENT << indent << "QtJambiAPI::setCppOwnership";
                        if(!type->isQObject() && var_index==0 && language==TS::ShellCode)
                            s << "AndInvalidate";
                        break;
                    case TS::Invalidate:
                        if(var_index==0){
                            s << INDENT << indent << "InvalidateAfterUse::invalidate";
                        }else{
                            if(ownershipRule.condition.isEmpty())
                                s << INDENT << indent << "QTJAMBI_INVALIDATE_AFTER_USE";
                            else{
                                s << INDENT << indent << qtjambi_scope << ".addObjectInvalidation";
                            }
                        }
                        break;
                    case TS::ForcedInvalidate:
                        if(var_index==0){
                            s << INDENT << indent << "ForcedInvalidateAfterUse::invalidate";
                        }else{
                            if(ownershipRule.condition.isEmpty())
                                s << INDENT << indent << "QTJAMBI_FORCED_INVALIDATE_AFTER_USE";
                            else{
                                s << INDENT << indent << qtjambi_scope << ".addForcedObjectInvalidation";
                            }
                        }
                        break;
                    default:
                        s << INDENT << indent << "QtJambiAPI::setDefaultOwnership"; break;
                    }
                    if(type->isQObject())
                        s << "ForTopLevelObject";
                    s << "(" << __jni_env << ", ";
                    if(type->isQObject()){
                        if(type->indirections().isEmpty())
                            s << "&";
                        s << qt_var_name << ");" << Qt::endl;
                    }else{
                        s << var_name << ");" << Qt::endl;
                    }
                    if(ownershipRule.ownership==TS::CppOwnership && !type->isQObject()){
                        // (var_index==0 && language==TS::ShellCode) already done
                        if(var_index>0 && language==TS::NativeCode){
                            if(ownershipRule.condition.isEmpty())
                                s << INDENT << indent << "QTJAMBI_INVALIDATE_AFTER_USE(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                            else{
                                s << INDENT << indent << qtjambi_scope << ".addObjectInvalidation(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                            }
                        }
                    }
                    if(!ownershipRule.condition.isEmpty())
                        s << INDENT << "}" << Qt::endl;
                }
                break;
            }
        }
    } else {
        if(implementor->typeEntry()->isQObject() && ownershipRule.ownership == TS::Invalidate)
            return;
        QString indent;
        if(!ownershipRule.condition.isEmpty()){
            indent = "    ";
            s << INDENT << "if(" << ownershipRule.condition << ") {" << Qt::endl;
        }
        QString additionalArgument;
        switch(ownershipRule.ownership){
        case TS::TargetLangOwnership:
            s << INDENT << indent << "QtJambiAPI::setJavaOwnership"; break;
        case TS::CppOwnership:
            s << INDENT << indent<< "QtJambiAPI::setCppOwnership"; break;
        case TS::Invalidate:
            s << INDENT << indent<< "InvalidateAfterUse::invalidate";
            break;
        case TS::ForcedInvalidate:
            s << INDENT << indent<< "ForcedInvalidateAfterUse::invalidate";
            break;
        default:
            s << INDENT << indent<< "QtJambiAPI::setDefaultOwnership"; break;
        }
        if(implementor->typeEntry()->isQObject())
            s << "ForTopLevelObject";
        s << "(" << __jni_env << ", ";
        if(implementor->typeEntry()->isQObject()){
            s << qt_var_name;
        }else{
            s << var_name;
        }
        s << additionalArgument << ");" << Qt::endl;
        if(ownershipRule.ownership==TS::CppOwnership && !implementor->typeEntry()->isQObject()){
            if(var_index==0 && language==TS::ShellCode){
                s << INDENT << indent << "InvalidateAfterUse::invalidate(" << __jni_env << ", " << var_name << ");" << Qt::endl;
            }else if(var_index>0 && language==TS::NativeCode){
                if(ownershipRule.condition.isEmpty())
                    s << INDENT << indent << "QTJAMBI_INVALIDATE_AFTER_USE(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                else{
                    s << INDENT << indent << qtjambi_scope << ".addObjectInvalidation(" << __jni_env << ", " << var_name << ");" << Qt::endl;
                }
            }
        }
        if(!ownershipRule.condition.isEmpty())
            s << INDENT << "}" << Qt::endl;
    }

}

void CppImplGenerator::writeShellFunction(QTextStream &s, const MetaFunction *java_function, const MetaClass *implementor, int id) {
    if (java_function->isFinalInCpp() && !java_function->isVirtualSlot())
        return;
    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty()){
        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    }

    writeFunctionSignature(s, java_function, implementor, QString(), Option(OriginalName | FunctionOverride));

    s << Qt::endl
      << "{" << Qt::endl;
    {
        MetaType* originalReferenceFunctionType = nullptr;
        MetaType *function_type = java_function->type();
        const MetaArgumentList& arguments = java_function->arguments();
        INDENTATION(INDENT)
        QString java_function_signature = java_function->originalSignature();
        if(java_function_signature.isEmpty())
            java_function_signature = java_function->minimalSignature();
        bool isRemovedFromTargetLang = java_function->isRemovedFrom(implementor, TS::TargetLangCode)
                                        || java_function->isRemovedFrom(java_function->declaringClass(), TS::TargetLangCode);
        QString returnVariable = "__qt_return_value";

        if (!isRemovedFromTargetLang && (!java_function->isFinalInCpp() || java_function->isVirtualSlot())) {
            s << INDENT << "if(jmethodID method_id = __shell_javaMethod(" << id << ")){" << Qt::endl;
            {
                INDENTATION(INDENT)
                bool useModelCache = false;
                if(java_function->declaringClass()->typeEntry()->isQAbstractItemModel()
                   && (java_function_signature==QStringLiteral(u"rowCount(QModelIndex)const")
                       || java_function_signature==QStringLiteral(u"columnCount(QModelIndex)const"))){
                    useModelCache = true;
                    s << INDENT << "ModelData* modelData = __shell_modelData();" << Qt::endl
                      << INDENT << "if(modelData){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "int __qt_return_value = 0;" << Qt::endl;
                        s << INDENT << "if(modelData->get_" << java_function->name() << "(this, __qt_return_value";
                        for(MetaArgument *argument : arguments) {
                            s << ", " << argument->indexedName();
                        }
                        s << "))" << Qt::endl
                          << INDENT << "    return __qt_return_value;" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                QString lines;
                bool needsScope = false;
                {
                    QTextStream s(&lines);
                    INDENTATION(INDENT)
                    QString new_return_type = java_function->typeReplaced(0);
                    bool has_function_type = ((function_type!= nullptr
                                               || !new_return_type.isEmpty())
                                              && new_return_type != "void");
                    if (function_type && !java_function->isSelfReturningFunction()) {
                        s << INDENT;
                        if(function_type->getReferenceType()!=MetaType::NoReference){
                            originalReferenceFunctionType = function_type;
                            function_type = function_type->copy();
                            function_type->setReferenceType(MetaType::NoReference);
                            function_type->setIndirections({false});
                            MetaBuilder::decideUsagePattern(function_type);
                        }
                        bool done = false;
                        if(function_type->isSmartPointer()){
                            const SmartPointerTypeEntry* pentry = static_cast<const SmartPointerTypeEntry*>(function_type->typeEntry());
                            switch(pentry->type()){
                            case SmartPointerTypeEntry::QScopedPointer:
                                if(function_type->instantiations().size()>0){
                                    writeTypeInfo(s, function_type->instantiations()[0], SkipName);
                                    s << " *__qt_return_value{nullptr};" << Qt::endl;
                                    done = true;
                                    returnVariable.clear();
                                    QTextStream s2(&returnVariable);
                                    writeTypeInfo(s2, function_type, SkipName);
                                    s2 << "(__qt_return_value)";
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        if(!done){
                            writeTypeInfo(s, function_type);
                            if(function_type && (function_type->isPrimitive() || function_type->isEnum() || !function_type->indirections().isEmpty())){
                                s << " __qt_return_value = " << default_return_statement_qt(function_type, AbstractGenerator::NoReturnStatement) << ";" << Qt::endl;
                            }else{
                                bool finished = false;
                                if(const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(function_type->typeEntry())){
                                    if(!ctype->hasPublicDefaultConstructor() && ctype->hasNonPrivateConstructors() && ctype->name()!="QItemSelection"){
                                        CustomFunction customFunction = ctype->customConstructor(ComplexTypeEntry::DefaultConstructor);
                                        if(customFunction.hasCode()){
                                            finished = true;
                                            s << " __qt_return_value = *reinterpret_cast<";
                                            writeTypeInfo(s, function_type, SkipName);
                                            s << "*>(QMetaType::";
                                            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                                                s << "fromName(\"";
                                                writeTypeInfo(s, function_type, SkipName);
                                                s << "\")";
                                            }else{
                                                s << "fromType<";
                                                writeTypeInfo(s, function_type, SkipName);
                                                s << ">()";
                                            }
                                            s << ".create());" << Qt::endl;
                                        }
                                    }
                                }
                                if(!finished)
                                    s << " __qt_return_value;" << Qt::endl;	// declaration only
                            }
                        }
                    }
                    s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "if(jobject __java_this = " << shellClassName(implementor) << "::__shell()->getJavaObjectLocalRef(__jni_env)){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            {
                                QString lines;
                                {
                                    QTextStream s(&lines);
                                    if(java_function->isPaintMethod())
                                        s << INDENT << "AboutToPaint __qj_aboutToPaint(__jni_env, __java_this);" << Qt::endl;
                                    writeCodeInjections(s, java_function, implementor, CodeSnip::Beginning, TS::ShellCode, "__jni_env", "__jni_env.scope()");

                                    for(MetaArgument *argument : arguments) {
                                        if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
                                            if (!(argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                                                    || java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
                                                writeQtToJava(s,
                                                              argument->type(),
                                                              argument->indexedName(),
                                                              "__java_" + argument->indexedName(),
                                                              java_function, nullptr,
                                                              argument->argumentIndex() + 1,
                                                              NoOption,
                                                              "__jni_env", "__jni_env.scope()");
                                            }

                                            if (java_function->resetObjectAfterUse(argument->argumentIndex() + 1)){
                                                if (argument->type()->isContainer()){
                                                    if(static_cast<const ContainerTypeEntry *>(argument->type()->typeEntry())->type()==ContainerTypeEntry::std_array){
                                                        s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                    }else{
                                                        s << INDENT << "QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                    }
                                                }else if (argument->type()->isArray()){
                                                    s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                }else{
                                                    s << INDENT << "QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ");" << Qt::endl;
                                                }
                                            }
                                        }
                                    }

                                    for (int i = 0; i < arguments.size(); ++i)
                                        writeOwnership(s, java_function, nullptr, "__java_" + arguments.at(i)->indexedName(), arguments.at(i)->indexedName(), i + 1, implementor, TS::ShellCode, "__jni_env", "__jni_env.scope()");

                                    s << INDENT;
                                    QString typeWrap;
                                    if (has_function_type && !java_function->isSelfReturningFunction()) {
                                        QString _jniName;
                                        if (new_return_type.isEmpty()) {
                                            _jniName = translateType(function_type);
                                        } else {
                                            _jniName = jniName(new_return_type);
                                        }
                                        if(_jniName=="jstring"
                                                || _jniName=="jclass"
                                                || _jniName=="jthrowable"
                                                || _jniName.endsWith("Array"))
                                            typeWrap = _jniName;
                                        s << _jniName << " " << "__java_return_value = ";	// declaration only
                                    }

                                    if(!typeWrap.isEmpty())
                                        s << typeWrap << "(";
                                    s << "__jni_env->";
                                    if (new_return_type.isEmpty()) {
                                        s << callXxxMethod(java_function->type());
                                    } else if (!has_function_type) {
                                        s << "CallVoidMethod";
                                    } else {
                                        s << callXxxMethod(new_return_type);
                                    }

                                    s << "(__java_this, method_id";
                                    for (int i = 0; i < arguments.size(); ++i) {
                                        if (java_function->argumentRemoved(i + 1)!=ArgumentRemove_No) {
                                            continue;
                                        }
                                        s << ", ";
                                        break;
                                    }
                                    writeFunctionCallArguments(s, java_function, "__java_", Option(NoCasts | SkipRemovedArguments));
                                    s << ")";
                                    if(!typeWrap.isEmpty())
                                        s << ")";
                                    s << ";" << Qt::endl;
                                    // We'd like to not emit this line below, if the one just outside the
                                    //  generated brace section is next.
                                    s << INDENT << "__jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );" << Qt::endl;

                                    if (has_function_type && !java_function->isSelfReturningFunction()) {
                                        QString scopeName = java_function->returnScopeIndex()>=0 ? QStringLiteral(u"__qtjambi_return_scope") : QStringLiteral(u"__jni_env.scope()");
                                        if(java_function->hasConversionRule(TS::ShellCode, 0)){
                                            writeJavaToQt(s, function_type, "__qt_return_value_tmp", "__java_return_value",
                                                          java_function, nullptr, 0, Option(GlobalRefJObject | NoEnumAsInts), "__jni_env",
                                                          scopeName);
                                            if(java_function->type())
                                                s << INDENT << "__qt_return_value = __qt_return_value_tmp;" << Qt::endl;
                                        }else{
                                            writeJavaToQt(s, function_type, "__qt_return_value", "__java_return_value",
                                                          java_function, nullptr, 0, Option(NoTmpVariable | GlobalRefJObject | NoEnumAsInts), "__jni_env",
                                                          scopeName);
                                        }
                                        QString defaultValue = java_function->nullPointerDefaultValue();
                                        if (!defaultValue.isEmpty()){
                                            s << INDENT << "if (__java_return_value == nullptr) {" << Qt::endl;
                                            {
                                                INDENTATION(INDENT)
                                                s << INDENT << "__qt_return_value = " << defaultValue << ";";
                                            }
                                            s << INDENT << "}" << Qt::endl;
                                        }
                                        if (java_function->nullPointersDisabled()) {
                                            s << INDENT << "if (__java_return_value == nullptr) {" << Qt::endl;
                                            {
                                                INDENTATION(INDENT)
                                                s << INDENT << "JavaException::raiseNullPointerException(__jni_env, QStringLiteral(\"Method %1." << java_function->name()
                                                  << " must not return null.\").arg(QtJambiAPI::getObjectClassName(__jni_env, __java_this)) QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                                            }

                                            s << INDENT << "}" << Qt::endl;
                                        }
                                    } else if (java_function->hasConversionRule(TS::ShellCode, 0)) {
                                        writeConversionRule(s, TS::ShellCode, java_function, nullptr, 0, "<invalid>", "<invalid>", NoOption, "__jni_env", "__jni_env.scope()");
                                    }

                                    writeOwnership(s, java_function, nullptr, "this", "__java_this", -1, implementor, TS::ShellCode, "__jni_env", "__jni_env.scope()");
                                    writeOwnership(s, java_function, nullptr, "__java_return_value", "__qt_return_value", 0, implementor, TS::ShellCode, "__jni_env", "__jni_env.scope()");
                                    writeCodeInjections(s, java_function, implementor, CodeSnip::End, TS::ShellCode, "__jni_env", "__jni_env.scope()");
                                    if (function_type && !java_function->isSelfReturningFunction()){
                                        if(java_function->nullPointersDisabled() || originalReferenceFunctionType){
                                            s << INDENT << "if(!__qt_return_value)" << Qt::endl;
                                            s << INDENT << "    JavaException::raiseNullPointerException(__jni_env, \"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function->minimalSignature() << ": Unexpected null pointer returned.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                                        }
                                    }
                                }
                                if(lines.contains(QStringLiteral(u"__jni_env.scope()"))){
                                    needsScope = true;
                                }
                                if(java_function->returnScopeIndex()>=0){
                                    if(lines.contains(QStringLiteral(u"__qtjambi_return_scope"))){
                                        s << INDENT << "QtJambiScope& __qtjambi_return_scope = *" << shellClassName(implementor) << "::__shell()->returnScope(__jni_env, typeid(" << implementor->typeEntry()->qualifiedCppName() << "), "
                                          << java_function->returnScopeIndex() << ");" << Qt::endl;
                                    }
                                }
                                s << lines;
                            }
                        }
                        s << INDENT << "} else {" << Qt::endl;         // !__shell()->getJavaObjectLocalRef(__jni_env)
                        {
                            INDENTATION(INDENT)
                            QSharedPointer<Indentation> needIndent;
                            if(implementor->isQObject()){
                                if(arguments.size()==1
                                        && arguments.at(0)->type()->typeEntry()->qualifiedCppName()=="QEvent"
                                        && java_function->name()=="event"){
                                    s << INDENT << "if(" << arguments.at(0)->indexedName() << "->type()==QEvent::DeferredDelete || " << arguments.at(0)->indexedName() << "->type()==QEvent::ChildRemoved){" << Qt::endl;
                                    {
                                        INDENTATION(INDENT)
                                        s << INDENT << "__qt_return_value = ";
                                        if(java_function->isAbstract()){
                                            s << "QObject::event(" << arguments.at(0)->indexedName() << ");" << Qt::endl;
                                        }else{
                                            writeBaseClassFunctionCall(s, java_function, implementor, NoReturnStatement);
                                        }
                                    }
                                    s << INDENT << "} else {" << Qt::endl;
                                    needIndent.reset(new Indentation(INDENT));
                                }else if(arguments.size()==2
                                         && arguments.at(0)->type()->typeEntry()->qualifiedCppName()=="QObject"
                                         && arguments.at(1)->type()->typeEntry()->qualifiedCppName()=="QEvent"
                                         && java_function->name()=="eventFilter"){
                                     s << INDENT << "if(" << arguments.at(1)->indexedName() << "->type()==QEvent::DeferredDelete || " << arguments.at(1)->indexedName() << "->type()==QEvent::ChildRemoved){" << Qt::endl;
                                     {
                                         INDENTATION(INDENT)
                                         s << INDENT << "__qt_return_value = ";
                                         if(java_function->isAbstract()){
                                             s << "QObject::eventFilter("
                                               << arguments.at(0)->indexedName()
                                               << ", " << arguments.at(1)->indexedName() << ");" << Qt::endl;
                                         }else{
                                             writeBaseClassFunctionCall(s, java_function, implementor, NoReturnStatement);
                                         }
                                     }
                                     s << INDENT << "} else {" << Qt::endl;
                                     needIndent.reset(new Indentation(INDENT));
                                }
                            }
                            // FIXME: Use helper method
                            // Raise exception ?  Maybe this should be QtJambiNoNativeException
                            if(implementor->isQObject()){
                                s << INDENT << shellClassName(implementor) << "::__shell()->warnForMethod(\"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function_signature << "\", this);" << Qt::endl;
                            }else{
                                s << INDENT << shellClassName(implementor) << "::__shell()->warnForMethod(\"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function_signature << "\");" << Qt::endl;
                            }
                            QString defaultValue = java_function->nullPointerDefaultValue();
                            if (!defaultValue.isEmpty()){
                                s << INDENT << "__qt_return_value = " << defaultValue << ";";
                            }
                            if(needIndent){
                                needIndent.reset();
                                s << INDENT << "}" << Qt::endl;
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }
                    s << INDENT << "} QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl
                      << INDENT << "    __jni_env.handleException(exn, this, \"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function_signature << "\");" << Qt::endl
                      << INDENT << "} QTJAMBI_TRY_END" << Qt::endl;
                    if(java_function->isBlockExceptions())
                        s << INDENT << "__jni_env.releaseException();" << Qt::endl;
                    if(java_function->isSelfReturningFunction()){
                        s << INDENT << "return *this;" << Qt::endl;
                    }else if (function_type){
                        if(originalReferenceFunctionType){
                            delete function_type;
                            function_type = originalReferenceFunctionType;
                            s << INDENT << "return *__qt_return_value;" << Qt::endl;
                        }else{
                            if(useModelCache){
                                s << INDENT << "if(modelData)" << Qt::endl
                                  << INDENT << "    modelData->set_" << java_function->name() << "(" << returnVariable;
                                for(MetaArgument *argument : arguments) {
                                    s << ", " << argument->indexedName();
                                }
                                s << ");" << Qt::endl;
                            }
                            s << INDENT << "return " << returnVariable << ";" << Qt::endl;
                        }
                    }
                }
                s << INDENT << "QTJAMBI_JAVA_METHOD_CALL(\"" << implementor->typeEntry()->qualifiedCppName() << "::"
                  << (java_function_signature.isEmpty() ? java_function->minimalSignature() : java_function_signature) << "\", this)" << Qt::endl;

                if(needsScope){
                    lines = lines.replace(shellClassName(implementor)+"::__shell()->getJavaObjectLocalRef(__jni_env)", "__jni_env.getJavaObjectLocalRef()");
                    if(java_function->isNoExcept()){
                        if(java_function->isBlockExceptions()){
                            s << INDENT << "if (JniEnvironmentScopeExceptionInhibitorAndBlocker __jni_env{" << shellClassName(implementor) << "::__shell(), " << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }else{
                            s << INDENT << "if (JniEnvironmentScopeExceptionInhibitor __jni_env{" << shellClassName(implementor) << "::__shell(), " << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }
                    }else{
                        if(java_function->isBlockExceptions()){
                            s << INDENT << "if (JniEnvironmentScopeExceptionHandlerAndBlocker __jni_env{" << shellClassName(implementor) << "::__shell(), " << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }else{
                            s << INDENT << "if (JniEnvironmentScopeExceptionHandler __jni_env{" << shellClassName(implementor) << "::__shell(), " << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }
                    }
                }else{
                    if(java_function->isNoExcept()){
                        if(java_function->isBlockExceptions()){
                            s << INDENT << "if (JniEnvironmentExceptionInhibitorAndBlocker __jni_env{" << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }else{
                            s << INDENT << "if (JniEnvironmentExceptionInhibitor __jni_env{" << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }
                    }else{
                        if(java_function->isBlockExceptions()){
                            s << INDENT << "if (JniEnvironmentExceptionHandlerAndBlocker __jni_env{" << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }else{
                            s << INDENT << "if (JniEnvironmentExceptionHandler __jni_env{" << QString::number( 100*(arguments.size()+2) ) << "}) {" << Qt::endl;
                        }
                    }
                }
                s << lines;
                // no environment
                if(java_function->isAbstract()){
                    if (originalReferenceFunctionType) {
                        s << INDENT << "}else{" << Qt::endl;
                        s << INDENT << "    throw \"Call of java method " << implementor->typeEntry()->qualifiedCppName() << "::" << java_function->minimalSignature() << " without JVM.\";" << Qt::endl;
                    }else if (function_type) {
                        s << INDENT << "}else{" << Qt::endl;
                        s << INDENT << "    " << default_return_statement_qt(function_type, AbstractGenerator::NoOption) << ";" << Qt::endl;
                    }
                }else{
                    s << INDENT << "}else{" << Qt::endl;
                    INDENTATION(INDENT)
                    if(java_function->isBlockExceptions()){
                        s << INDENT << "QtJambiExceptionBlocker __qj_exceptionBlocker;" << Qt::endl;
                        if(function_type){
                            s << INDENT;
                            writeTypeInfo(s, function_type);
                            s << " __qt_return_value = ";
                            writeBaseClassFunctionCall(s, java_function, implementor, NoReturnStatement);
                            s << INDENT << "__qj_exceptionBlocker.release(nullptr);" << Qt::endl
                              << INDENT << "return " << returnVariable << ";" << Qt::endl;
                        }else{
                            writeBaseClassFunctionCall(s, java_function, implementor);
                            s << INDENT << "__qj_exceptionBlocker.release(nullptr);" << Qt::endl;
                        }
                    }else{
                        writeBaseClassFunctionCall(s, java_function, implementor);
                    }
                }
                s << INDENT << "}" << Qt::endl;
            }// no method
            if(java_function->isAbstract()){
                if (originalReferenceFunctionType) {
                    s << INDENT << "}else{" << Qt::endl;
                    s << INDENT << "    throw \"Call of pure virtual method " << implementor->typeEntry()->qualifiedCppName() << "::" << java_function->minimalSignature() << ".\";" << Qt::endl;
                }else if (function_type) {
                    s << INDENT << "}else{" << Qt::endl;
                    s << INDENT << "    " << default_return_statement_qt(function_type, AbstractGenerator::NoOption) << ";" << Qt::endl;
                }
            }else{
                s << INDENT << "}else{" << Qt::endl;
                INDENTATION(INDENT)
                if(java_function->isBlockExceptions()){
                    s << INDENT << "QtJambiExceptionBlocker __qj_exceptionBlocker;" << Qt::endl;
                    if(function_type){
                        s << INDENT;
                        writeTypeInfo(s, function_type);
                        s << " __qt_return_value = ";
                        writeBaseClassFunctionCall(s, java_function, implementor, NoReturnStatement);
                        s << INDENT << "__qj_exceptionBlocker.release(nullptr);" << Qt::endl
                          << INDENT << "return " << returnVariable << ";" << Qt::endl;
                    }else{
                        writeBaseClassFunctionCall(s, java_function, implementor);
                        s << INDENT << "__qj_exceptionBlocker.release(nullptr);" << Qt::endl;
                    }
                }else{
                    writeBaseClassFunctionCall(s, java_function, implementor);
                }
            }
            s << INDENT << "}" << Qt::endl;
        } else {
            if (isRemovedFromTargetLang || java_function->isAbstract()) {
                // Avoid compiler warnings for unused parameters
                const MetaArgumentList& arguments = java_function->arguments();

                for(const MetaArgument *argument : arguments) {
                    if(java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No)
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                }
            }
            writeCodeInjections(s, java_function, implementor, CodeSnip::Beginning, TS::ShellCode, "__jni_env", "");
            writeBaseClassFunctionCall(s, java_function, implementor);
            writeCodeInjections(s, java_function, implementor, CodeSnip::End, TS::ShellCode, "__jni_env", "");
            if(java_function->isBlockExceptions())
                s << INDENT << "__qj_exceptionBlocker.release(__jni_env);" << Qt::endl;
        }
    }
    s << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    s << Qt::endl;
}

// ### kill implementor

void CppImplGenerator::writePublicFunctionOverride(QTextStream &s,
        const MetaFunction *java_function,
        const MetaClass *implementor) {
    Q_ASSERT(java_function->originalAttributes()
             & (MetaAttributes::Protected
                | MetaAttributes::Final));

    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty())
        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    // The write a public override version of this function to be used by native functions
    writeFunctionSignature(s, java_function, implementor, "__qt_",
                           Option(UnderscoreSpaces | EnumAsInts
                                  | UseNativeIds | JNIProxyFunction | SkipRemovedArguments
                                  | (java_function->isFinal() ? NoOption : VirtualCall)
                                  | (java_function->isAbstract() ? SkipName : NoOption)));
    s << INDENT << Qt::endl
      << INDENT << "{" << Qt::endl;
    QString lines;
    {
        QTextStream s(&lines);
        INDENTATION(INDENT)
        if(java_function->isBaseClassDelegateFunction()){
            s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
            s << INDENT << "Q_UNUSED(" << ( java_function->implementingClass()->typeEntry()->isNativeIdBased() ? "__this_nativeId" : "__this" ) << ")" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_function->type(), NoOption);
            s << "__qt_this = this;" << Qt::endl;
            writeQtToJava(s, java_function->type(), "__qt_this", "",
                          java_function, nullptr, 0, Option(DirectReturn | NoTmpVariable),
                          "__jni_env", "");
            s << ";" << Qt::endl;
        }else{
            const MetaClass *cls = implementor ? implementor : java_function->ownerClass();
            const MetaType *function_type = java_function->type();
            QString qt_object_name = java_function->isStatic() ? cls->fullQualifiedCppName() : "this";
            bool hasThisVar = false;
            if(!java_function->isStatic() && !java_function->isConstructor()){
                hasThisVar = java_function->needsCallThrough() || !java_function->implementingClass()->typeEntry()->isNativeIdBased();
            }
            s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
            if(hasThisVar)
                s << INDENT << "Q_UNUSED(" << ( java_function->implementingClass()->typeEntry()->isNativeIdBased() ? "__this_nativeId" : "__this" ) << ")" << Qt::endl;
            writeFinalFunctionSetup(s, java_function, qt_object_name, cls, Option(NoEnumAsInts | JNIProxyFunction));
            writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Beginning, TS::NativeCode, "__jni_env", "__qtjambi_scope");

            // Call the Qt function on the java object

            QString returnTypeReplacement = java_function->typeReplaced(0);
            if ((function_type || !returnTypeReplacement.isEmpty())) {
                const QString qt_return_value = "__qt_return_value";
                const QString java_return_value = "__java_return_value";
                if (function_type) {
                    s << INDENT;
                    writeTypeInfo(s, function_type, NoOption);
                    s << qt_return_value << " = ";
                }

                writeBaseClassFunctionCall(s, java_function, implementor, Option(NoEnumAsInts | NoReturnStatement | JNIProxyFunction));

                bool hasReturn = java_function->argumentReplaced(0).isEmpty() && returnTypeReplacement != "void";
                if(!hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::End}, TS::NativeCode)
                        && !hasOwnership(java_function, nullptr, 0, java_function->implementingClass(), TS::NativeCode)
                        && !java_function->hasConversionRule(TS::NativeCode, 0)){
                    if(hasReturn){
                        writeQtToJava(s, function_type, qt_return_value, "",
                                      java_function, nullptr, 0, Option(EnumAsInts | DirectReturn | NoTmpVariable),
                                      "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                        s << ";" << Qt::endl;
                    }else if(function_type){
                        s << INDENT << "Q_UNUSED(" << qt_return_value << ")" << Qt::endl;
                    }
                }else{
                    if(hasReturn){
                        writeQtToJava(s, function_type, qt_return_value, java_return_value,
                                      java_function, nullptr, 0, EnumAsInts,
                                      "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                    }else if(function_type){
                        s << INDENT << "Q_UNUSED(" << qt_return_value << ")" << Qt::endl;
                    }
                    writeOwnership(s, java_function, nullptr, "__qt_this", "__java_this", -1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    writeOwnership(s, java_function, nullptr, java_return_value, qt_return_value, 0, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    if(hasReturn)
                        s << INDENT << "return " << java_return_value << ";" << Qt::endl;
                }
            } else {
                s << INDENT;
                writeBaseClassFunctionCall(s, java_function, implementor, Option(NoEnumAsInts | NoReturnStatement | JNIProxyFunction));

                writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
            }
        }
    }
    if(lines.contains("__qtjambi_scope")){
        s << INDENT << "    QtJambiScope __qtjambi_scope" << ( java_function->isStatic() ? "" : (java_function->implementingClass()->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
        if(!pps.isEmpty()){
            s << INDENT << "    Q_UNUSED(__qtjambi_scope)" << Qt::endl;
        }
    }
    s << lines;
    s << INDENT << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    s << Qt::endl;
}


void CppImplGenerator::writeVirtualFunctionOverride(QTextStream &s,
        const MetaFunction *java_function,
        const MetaClass *implementor) {
    if (java_function->wasPrivate())
        return ;

    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty())
        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;

    Q_ASSERT(!java_function->isFinalInCpp());

    // The write a public override version of this function to be used by native functions
    writeFunctionSignature(s, java_function, implementor, "__qt_",
                           Option((java_function->isFinal() ? NoOption : VirtualCall) | EnumAsInts | UnderscoreSpaces | JNIProxyFunction | UseNativeIds | SkipRemovedArguments) );
    s << Qt::endl
      << INDENT << "{" << Qt::endl;
    QString lines;
    {
        QTextStream s(&lines);
        INDENTATION(INDENT)
        const MetaClass *cls = implementor ? implementor : java_function->ownerClass();
        const MetaType *function_type = java_function->type();
        QString qt_object_name = java_function->isStatic() ? cls->fullQualifiedCppName() : "this";
        bool hasThisVar = false;
        if(!java_function->isStatic() && !java_function->isConstructor()){
            hasThisVar = java_function->needsCallThrough() || !java_function->implementingClass()->typeEntry()->isNativeIdBased();
        }
        if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
            s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
            s << INDENT << "Q_ASSERT(!!__this_nativeId);" << Qt::endl;
        }
        writeFinalFunctionSetup(s, java_function, qt_object_name, cls, Option(NoEnumAsInts | UseNativeIds | JNIProxyFunction));
        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Beginning, TS::NativeCode, "__jni_env", "__qtjambi_scope");

        // Call the Qt function on the java object
        if(java_function->isBlockExceptions())
            s << INDENT << "QtJambiExceptionBlocker __qj_exceptionBlocker;" << Qt::endl;

        QString typeReplaced = java_function->typeReplaced(0);
        if ((function_type || !typeReplaced.isEmpty()) && !java_function->isSelfReturningFunction()) {
            const QString qt_return_value = "__qt_return_value";
            const QString java_return_value = "__java_return_value";
            if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                s << INDENT << "if (QtJambiAPI::isShell(__this_nativeId)) {" << Qt::endl;
            }else{
                if(java_function->implementingClass()->typeEntry()->designatedInterface())
                    s << INDENT << "if (QtJambiAPI::javaInterfaceHasShell(__jni_env, __this)) {" << Qt::endl;
                else
                    s << INDENT << "if (QtJambiAPI::javaObjectHasShell(__jni_env, __this)) {" << Qt::endl;
            }
            {
                INDENTATION(INDENT)
                if(java_function->isAbstract()){
                    s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"Call of pure virtual method " << implementor->qualifiedCppName() << "::" << java_function->signature() << ".\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                    s << INDENT << "throw \"Call of pure virtual method " << implementor->qualifiedCppName() << "::" << java_function->signature() << ".\";" << Qt::endl;
                }else{
                    s << INDENT;
                    if (function_type) {
                        writeTypeInfo(s, function_type, NoOption);
                        s << qt_return_value << " = ";
                    }
                    writeBaseClassFunctionCall(s, java_function, implementor, Option(NoEnumAsInts | NoReturnStatement | JNIProxyFunction));
                    if(java_function->isBlockExceptions())
                        s << INDENT << "__qj_exceptionBlocker.release(__jni_env);" << Qt::endl;

                    if(!hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::End}, TS::NativeCode)
                            && !hasOwnership(java_function, nullptr, 0, java_function->implementingClass(), TS::NativeCode)
                            && !java_function->hasConversionRule(TS::NativeCode, 0)){
                        writeQtToJava(s, function_type, qt_return_value, "",
                                      java_function, nullptr, 0, Option(EnumAsInts | DirectReturn | NoTmpVariable),
                                      "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                        s << ";" << Qt::endl;
                    }else{
                        writeQtToJava(s, function_type, qt_return_value, java_return_value,
                                      java_function, nullptr, 0, EnumAsInts,
                                      "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                        writeOwnership(s, java_function, nullptr, "__qt_this", "__java_this", -1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                        writeOwnership(s, java_function, nullptr, java_return_value, qt_return_value, 0, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                        s << INDENT << "return " << java_return_value << ";" << Qt::endl;
                    }
                }
            }
            s << INDENT << "} else {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT;
                if (function_type) {
                    writeTypeInfo(s, function_type, NoOption);
                    s << qt_return_value << " = ";
                }
                writeBaseClassFunctionCall(s, java_function, implementor, Option(NoEnumAsInts | NoReturnStatement | VirtualCall | JNIProxyFunction));
                if(java_function->isBlockExceptions())
                    s << INDENT << "__qj_exceptionBlocker.release(__jni_env);" << Qt::endl;

                if(!hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::End}, TS::NativeCode)
                        && !hasOwnership(java_function, nullptr, 0, java_function->implementingClass(), TS::NativeCode)
                        && !java_function->hasConversionRule(TS::NativeCode, 0)){
                    writeQtToJava(s, function_type, qt_return_value, "",
                                  java_function, nullptr, 0, Option(EnumAsInts | DirectReturn | NoTmpVariable),
                                  "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                    s << ";" << Qt::endl;
                }else{
                    writeQtToJava(s, function_type, qt_return_value, java_return_value,
                                  java_function, nullptr, 0, EnumAsInts,
                                  "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                    writeOwnership(s, java_function, nullptr, "__qt_this", "__java_this", -1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    writeOwnership(s, java_function, nullptr, java_return_value, qt_return_value, 0, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    s << INDENT << "return " << java_return_value << ";" << Qt::endl;
                }
            }
            s << INDENT << "}" << Qt::endl;
        } else {
            if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                s << INDENT << "if (QtJambiAPI::isShell(__this_nativeId)) {" << Qt::endl;
            }else{
                if(java_function->implementingClass()->typeEntry()->designatedInterface())
                    s << INDENT << "if (QtJambiAPI::javaInterfaceHasShell(__jni_env, __this)) {" << Qt::endl;
                else
                    s << INDENT << "if (QtJambiAPI::javaObjectHasShell(__jni_env, __this)) {" << Qt::endl;
            }
            {
                INDENTATION(INDENT)
                if(java_function->isAbstract()){
                    s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"Call of pure virtual method " << implementor->qualifiedCppName() << "::" << java_function->signature() << ".\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                    s << INDENT << "throw \"Call of pure virtual method " << implementor->qualifiedCppName() << "::" << java_function->signature() << ".\";" << Qt::endl;
                }else{
                    s << INDENT;
                    writeBaseClassFunctionCall(s, java_function, implementor, Option(NoEnumAsInts | NoReturnStatement | JNIProxyFunction));
                }
            }
            s << INDENT << "} else {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT;
                writeBaseClassFunctionCall(s, java_function, implementor, Option(NoEnumAsInts | NoReturnStatement | VirtualCall | JNIProxyFunction));
            }
            s << INDENT << "}" << Qt::endl;
            if(java_function->isBlockExceptions())
                s << INDENT << "__qj_exceptionBlocker.release(__jni_env);" << Qt::endl;

            writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
            if(java_function->isSelfReturningFunction()){
                s << INDENT << "return nullptr;" << Qt::endl;
            }
        }
    }
    lines = lines.replace("__qt_this", "this");
    if(lines.contains("__qtjambi_scope")){
        s << INDENT << "    QtJambiScope __qtjambi_scope" << ( java_function->isStatic() ? "" : (java_function->implementingClass()->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
        if(!pps.isEmpty()){
            s << INDENT << "    Q_UNUSED(__qtjambi_scope)" << Qt::endl;
        }
    }
    s << lines;
    s << INDENT << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    s << Qt::endl;
}


bool CppImplGenerator::writeBaseClassFunctionCall(QTextStream &s,
        const MetaFunction *java_function,
        const MetaClass *,
        Option options) {
    bool rv = false;    // did we emit a "return"
    if ((options & NoReturnStatement) == 0)
        s << INDENT;
    if (java_function->isAbstract() && !(options & VirtualCall)) {
        s << default_return_statement_qt(java_function->type(), options) << ";" << Qt::endl;
        if ((options & NoReturnStatement) == 0)
            rv = true;
    } else {
        QScopedPointer<Indentation> idt;
        const MetaClass *implementor = java_function->implementingClass();
        if (java_function->isInterfaceFunction())
            implementor = java_function->interfaceClass()->primaryInterfaceImplementor();
        const bool enableInterfaceCast = false;
        if (enableInterfaceCast
                && (options & VirtualCall) == VirtualCall
                && !(options & SuperCall)
                && java_function->isInterfaceFunction()
                && java_function->wasProtected()){
            //create accessor class for protected interface functions
            if((options & NoReturnStatement) && java_function->type()){
                s << "[&]() -> ";
                writeTypeInfo(s, java_function->type(), NoOption);
                s << "{" << Qt::endl;
                idt.reset(new Indentation(INDENT, 2));
            }
            s << INDENT << "struct __qt_ProtectedAccess : public " << implementor->qualifiedCppName() << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT;
                writeFunctionSignature(s, java_function, nullptr, "__public_",
                                       Option(UnderscoreSpaces | EnumAsInts));
                s << Qt::endl
                  << INDENT << "{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    writeBaseClassFunctionCall(s, java_function, implementor, Option(VirtualCall | SuperCall));
                }
                s << INDENT << "}" << Qt::endl;
            }
            s << INDENT << "};" << Qt::endl
              << INDENT;
        }
        if (java_function->type() && (options & NoReturnStatement) == 0) {
            s << "return ";
            rv = true;
            if((options & EnumFromInt)==EnumFromInt && java_function->type()->typeEntry()->isScopedEnum()){
                s << "static_cast<";
                writeTypeInfo(s, java_function->type(), EnumAsInts);
                s << ">(";
            }
        }
        if ((options & VirtualCall) == VirtualCall) {
            if(enableInterfaceCast && java_function->isInterfaceFunction() && !(options & SuperCall)){
                if(java_function->wasProtected()){
                    //create accessor class for protected interface functions
                    if((options & NoReturnStatement) && java_function->type())
                        s << "return ";
                    s << "static_cast<";
                    if(java_function->isConstant())
                        s << "const ";
                    s << "__qt_ProtectedAccess *>(static_cast<";
                    if(java_function->isConstant())
                        s << "const ";
                    s << implementor->qualifiedCppName() << " *>(this))->__public_";
                }else{
                    // this is actually not the shell type, thus, cast back to implementor type
                    s << "static_cast<";
                    if(java_function->isConstant())
                        s << "const ";
                    s << implementor->qualifiedCppName() << "*>(this)->";
                }
            }
        }else{
            s << implementor->qualifiedCppName() << "::";
        }
        s << java_function->originalName();
        if(!java_function->templateParameters().isEmpty()){
            QStringList templateParameters;
            for(MetaTemplateParameter* param : java_function->templateParameters()){
                if(!param->isImplicit()){
                    if(!param->instantiation().isEmpty()){
                        if(param->instantiationType()){
                            QString str;
                            QTextStream _s(&str);
                            writeTypeInfo(_s, param->instantiationType());
                            templateParameters << str;
                        }else
                            templateParameters << param->instantiation();
                    }else if(param->type()){
                        if(param->defaultType().isEmpty()){
                            QString str;
                            QTextStream _s(&str);
                            writeTypeInfo(_s, param->type());
                            templateParameters << str;
                        }
                    }else if(param->defaultType().isEmpty()){
                        templateParameters.clear();
                        break;
                    }
                }
            }
            if(!templateParameters.isEmpty()){
                s << '<' << templateParameters.join(QStringLiteral(u"::")) << '>';
            }
        }
        s << "(";
        writeFunctionCallArguments(s, java_function, (options & JNIProxyFunction) ? "__qt_" : "", Option(options & ~JNIProxyFunction));
        s << ")";
        if(java_function->type()
                && (options & NoReturnStatement) == 0
                && (options & EnumFromInt)==EnumFromInt
                && java_function->type()->typeEntry()->isScopedEnum()){
            s << ")";
        }
        s << ";" << Qt::endl;

        if(enableInterfaceCast
                && (options & VirtualCall) == VirtualCall
                && !(options & SuperCall)
                && (options & NoReturnStatement)
                && java_function->type()
                && java_function->isInterfaceFunction()
                && java_function->wasProtected()){
            idt.reset();
            s << INDENT << "    }();" << Qt::endl;
        }
    }
    return rv;
}


void CppImplGenerator::writeFunctionName(QTextStream &s,
        const MetaFunction *java_function,
        const MetaClass *java_class,
        uint options) const{
    const MetaClass *cls = java_class ? java_class : java_function->ownerClass();
    const MetaArgumentList& arguments = java_function->arguments();

    if(java_function->isDeclDeprecated()){
        options |= CppImplGenerator::Deprecated;
    }

    // Function signature
    QString return_type = jniReturnName(java_function);
    QString function_name;

    bool callThrough = java_function->needsCallThrough();
    if (!callThrough)
        function_name = java_function->name();
    else
        function_name = java_function->marshalledName();

    QString args = "__";

    JNISignatureFormat format;
    if(java_function->isConstructor()){
        format = JNISignatureFormat(Underscores | NoModification);
    }else{
        format = Underscores;
    }

    if(java_function->isConstructor()){
        if(java_function->implementingClass()->typeEntry()->designatedInterface())
            args += jni_signature(java_function->implementingClass()->typeEntry()->designatedInterface()->qualifiedTargetLangName(), format);
        else
            args += jni_signature(java_function->implementingClass()->typeEntry()->qualifiedTargetLangName(), format);
    }else if (callThrough && !java_function->isStatic()){
        if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
            args += "J";
        }else if(java_function->declaringClass()->typeEntry()->isInterface()){
            args += jni_signature(java_function->declaringClass()->typeEntry()->qualifiedTargetLangName(), format);
        }
    }
    QMap<QString,QString> parameterTypesByName;
    for(const Parameter& parameterType : java_function->addedParameterTypes()){
        parameterTypesByName[parameterType.name] = parameterType.extends.split("&").first().trimmed();
    }
    const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
    int argumentCounter = 1;
    while(addedArguments.first.contains(argumentCounter)){
        const ArgumentModification& mod = addedArguments.first[argumentCounter];
        if(parameterTypesByName.contains(mod.modified_type)){
            QString t = parameterTypesByName[mod.modified_type];
            if(t.isEmpty())
                t = "java.lang.Object";
            args += jni_signature(annotationFreeTypeName(t), format);
        }else if(mod.modified_jni_type.isEmpty())
            args += jni_signature(annotationFreeTypeName(mod.modified_type), format);
        else
            args += jni_signature(mod.modified_jni_type, format);
        ++argumentCounter;
    }
    if (!arguments.isEmpty()) {
        for(const MetaArgument *argument : arguments) {
            if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
                QString jniType;
                QString modified_type = java_function->typeReplaced(argument->argumentIndex()+1, &jniType);
                if(java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
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
                    if(singleType=="jint" || singleType=="int" || singleType=="I"){
                        args += jni_signature("java.nio.IntBuffer", format);
                    }else if(singleType=="jbyte" || singleType=="byte" || singleType=="B"){
                        args += jni_signature("java.nio.ByteBuffer", format);
                    }else if(singleType=="jchar" || singleType=="char" || singleType=="C"){
                        args += jni_signature("java.nio.CharBuffer", format);
                    }else if(singleType=="jshort" || singleType=="short" || singleType=="S"){
                        args += jni_signature("java.nio.ShortBuffer", format);
                    }else if(singleType=="jlong" || singleType=="long" || singleType=="J"){
                        args += jni_signature("java.nio.LongBuffer", format);
                    }else if(singleType=="jfloat" || singleType=="float" || singleType=="F"){
                        args += jni_signature("java.nio.FloatBuffer", format);
                    }else if(singleType=="jdouble" || singleType=="double" || singleType=="D"){
                        args += jni_signature("java.nio.DoubleBuffer", format);
                    }else{
                        args += jni_signature("java.nio.Buffer", format);
                    }
                }else if(java_function->useArgumentAsArray(argument->argumentIndex() + 1)){
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
                    args += jni_signature(array.get(), format);
                    int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                            QString typeReplaced = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                            if(typeReplaced.isEmpty()){
                                if(isCharSequenceSubstitute(lengthParam->type())){
                                     args += jni_signature("java.lang.CharSequence", format);
                                }else{
                                     args += jni_signature(lengthParam->type(), format);
                                }
                            }else{
                                if(parameterTypesByName.contains(typeReplaced)){
                                     QString t = parameterTypesByName[typeReplaced];
                                     if(t.isEmpty())
                                         t = "java.lang.Object";
                                     args += jni_signature(annotationFreeTypeName(t), format);
                                }else if(jniType.isEmpty()){
                                     args += jni_signature(annotationFreeTypeName(typeReplaced), format);
                                }else{
                                     args += jni_signature(jniType, format);
                                }
                            }
                        }
                    }
                }else if(java_function->useArgumentAsSlotContext(argument->argumentIndex() + 1)){
                    args += "J";
                }else if (!modified_type.isEmpty() || !argument->type()->hasNativeId() || java_function->isConstructor()) {
                    if (modified_type.isEmpty()){
                        if(isCharSequenceSubstitute(argument->type())){
                            args += jni_signature("java.lang.CharSequence", format);
                        }else{
                            args += jni_signature(argument->type(), format);
                        }
                    }else{
                        if(parameterTypesByName.contains(modified_type)){
                            QString t = parameterTypesByName[modified_type];
                            if(t.isEmpty())
                                t = "java.lang.Object";
                            args += jni_signature(annotationFreeTypeName(t), format);
                        }else if(jniType.isEmpty()){
                            args += jni_signature(annotationFreeTypeName(modified_type), format);
                        }else{
                            args += jni_signature(jniType, format);
                        }
                    }
                } else {
                    args += "J";
                }
            }
            ++argumentCounter;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& mod = addedArguments.first[argumentCounter];
                if(parameterTypesByName.contains(mod.modified_type)){
                    QString t = parameterTypesByName[mod.modified_type];
                    if(t.isEmpty())
                        t = "java.lang.Object";
                    args += jni_signature(annotationFreeTypeName(t), format);
                }else if(mod.modified_jni_type.isEmpty())
                    args += jni_signature(annotationFreeTypeName(mod.modified_type), format);
                else
                    args += jni_signature(mod.modified_jni_type, format);
                ++argumentCounter;
            }
        }
    }
    for(const ArgumentModification& mod : addedArguments.second){
        if(parameterTypesByName.contains(mod.modified_type)){
            QString t = parameterTypesByName[mod.modified_type];
            if(t.isEmpty())
                t = "java.lang.Object";
            args += jni_signature(annotationFreeTypeName(t), format);
        }else if(mod.modified_jni_type.isEmpty())
            args += jni_signature(annotationFreeTypeName(mod.modified_type), format);
        else
            args += jni_signature(mod.modified_jni_type, format);
    }

    if(java_function->isAbstract()){
        s << jni_function_signature(cls->package(), cls->name()+"$ConcreteWrapper", function_name,
                                    return_type, args, options);

    }else{
        s << jni_function_signature(cls->package(), cls->name(), function_name,
                                    return_type, args, options);

    }
}

void CppImplGenerator::writeFinalFunctionArguments(QTextStream &s, const MetaFunction *java_function,
        const QString &java_object_name) const {

    s << "(JNIEnv *__jni_env";
    if (java_function->isConstructor()){
        s << "," << Qt::endl << " jclass __jni_class," << Qt::endl
          << " jobject " << java_object_name;
    }else if(java_function->isStatic()){
        s << "," << Qt::endl << " jclass";
    }else{
        if (java_function->needsCallThrough()){
            s << "," << Qt::endl;
            if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                if(java_function->implementingClass()->typeEntry()->designatedInterface())
                    s << " jclass," << Qt::endl;
                else
                    s << " jobject __this," << Qt::endl;
                s << " QtJambiNativeID __this_nativeId";
            }else{
                if(java_function->declaringClass()->typeEntry()->isInterface() && !java_function->isAbstract())
                    s << " jclass," << Qt::endl;
                s << " jobject __this";
            }
        }else{
            s << "," << Qt::endl << " jobject __this";
        }
    }

    // the function arguments
    const MetaArgumentList& arguments = java_function->arguments();
    Option options = Option(EnumAsInts | UseNativeIds);
    if(java_function->isConstructor())
        options = NoOption;
    else if(!java_function->isFinal())
        options = Option(options | VirtualCall);
    const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
    int argumentCounter = 1;
    while(addedArguments.first.contains(argumentCounter)){
        const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
        s << "," << Qt::endl
          << " " << jniName(annotationFreeTypeName(argumentMod.modified_type)) << " " << argumentMod.modified_name;
        ++argumentCounter;
    }
    for(const MetaArgument *argument : arguments) {
        if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
            s << "," << Qt::endl;
            QString modified_type = java_function->typeReplaced(argument->argumentIndex() + 1);
            bool addOffset = false;
            if(java_function->useArgumentAsSlotContext(argument->argumentIndex() + 1) && modified_type.isEmpty()){
                s << " QtJambiNativeID";
            }else if(java_function->useArgumentAsArray(argument->argumentIndex() + 1)
                && !java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
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
                s << " " << translateType(array.get(), options);
                addOffset = true;
            }else if (modified_type.isEmpty())
                s << " " << translateType(argument->type(), options);
            else
                s << " " << jniName(annotationFreeTypeName(modified_type));
            s << " " << argument->indexedName();
            if(addOffset){
                int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                        s << "," << Qt::endl;
                        QString typeReplaced = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                        if(typeReplaced.isEmpty()){
                            s << " " << translateType(lengthParam->type(), options);
                        }else{
                            s << " " << jniName(annotationFreeTypeName(typeReplaced));
                        }
                        s << " __qt_offset_of_" << argument->indexedName();
                    }
                }
            }
            ++argumentCounter;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                s << "," << Qt::endl
                  << " " << jniName(annotationFreeTypeName(argumentMod.modified_type)) << " " << argumentMod.modified_name;
                ++argumentCounter;
            }
        }
    }
    for(const ArgumentModification& argumentMod : addedArguments.second){
        s << "," << Qt::endl
          << " " << jniName(annotationFreeTypeName(argumentMod.modified_type)) << " " << argumentMod.modified_name;
    }
    s << ")" << Qt::endl;
}


const ComplexTypeEntry* getOwnedType(const MetaClass* classType){
    if(classType){
        if(classType->typeEntry()->isThreadAffine())
            return classType->typeEntry();
        if(classType->typeEntry()->designatedInterface()
                && classType->typeEntry()->designatedInterface()->isThreadAffine())
            return classType->typeEntry()->designatedInterface();
        for(const MetaClass* iface : classType->interfaces()){
            const ComplexTypeEntry* ownedType = getOwnedType(iface);
            if(ownedType)
                return ownedType;
        }
        const ComplexTypeEntry* ownedType = getOwnedType(classType->baseClass());
        if(ownedType)
            return ownedType;
    }
    return nullptr;
}

/*!
    Generates type conversion from Java -> Qt for all the arguments
    that are to be to be passed to the function
*/
void CppImplGenerator::writeFinalFunctionSetup(QTextStream &s, const MetaFunction *java_function,
        const QString &qt_object_name,
        const MetaClass *cls, Option option) {
    // Extract the qt equivalent to the this pointer and name it "qt_object_name"
    bool hasDonePixmapCheck = false;
    bool hasDoneUICheck = false;
    if(java_function->isUIThreadAffine()){
        s << INDENT << "QtJambiAPI::checkMainThread(__jni_env, typeid(" << cls->fullQualifiedCppName() << "));" << Qt::endl;
        hasDoneUICheck = true;
    }else if(java_function->isPixmapThreadAffine()){
        s << INDENT << "QtJambiAPI::checkThreadQPixmap(__jni_env, typeid(" << cls->fullQualifiedCppName() << "));" << Qt::endl;
        hasDonePixmapCheck = true;
    }else if(!java_function->isStatic() && java_function->isThreadAffine()){
        if(cls->isQObject()){
            s << INDENT << "QtJambiAPI::checkThread(__jni_env, " << qt_object_name << ");" << Qt::endl;
        }else{
            const ComplexTypeEntry *entry = cls->typeEntry();
            const QString& ownerCode = entry->threadAffinity();
            if(ownerCode==QStringLiteral(u"main") || ownerCode==QStringLiteral(u"ui")){
                s << INDENT << "QtJambiAPI::checkMainThread(__jni_env, typeid(" << cls->fullQualifiedCppName() << "));" << Qt::endl;
                hasDoneUICheck = true;
            }else if(ownerCode==QLatin1String("pixmap")){
                s << INDENT << "QtJambiAPI::checkThreadQPixmap(__jni_env, typeid(" << cls->fullQualifiedCppName() << "));" << Qt::endl;
                hasDonePixmapCheck = true;
            }else{
                QString ownerCode = entry->threadAffinity();
                if(ownerCode.isEmpty())
                    ownerCode = (entry = java_function->declaringClass()->typeEntry())->threadAffinity();
                if(ownerCode==QStringLiteral(u"main") || ownerCode==QStringLiteral(u"ui")){
                    s << INDENT << "QtJambiAPI::checkMainThread(__jni_env, typeid(" << cls->fullQualifiedCppName() << "));" << Qt::endl;
                    hasDoneUICheck = true;
                }else if(ownerCode==QLatin1String("pixmap")){
                    s << INDENT << "QtJambiAPI::checkThreadQPixmap(__jni_env, typeid(" << cls->fullQualifiedCppName() << "));" << Qt::endl;
                    hasDonePixmapCheck = true;
                }else if(!ownerCode.isEmpty()){
                    if(entry->targetTypeSystem()==cls->typeEntry()->targetTypeSystem()){
                        if(ownerCode.contains("%1")){
                            ownerCode = ownerCode.arg(qt_object_name);
                        }else if(ownerCode.contains("this->")){
                            ownerCode = ownerCode.replace("this->", qt_object_name+"->");
                        }else{
                            ownerCode = qt_object_name+"->" + ownerCode;
                        }
                        s << INDENT << "QtJambiAPI::checkThread(__jni_env, " << ownerCode << ");" << Qt::endl;
                    }else{
                        s << INDENT << "QtJambiAPI::checkThread(__jni_env, typeid(" << entry->qualifiedCppName() << "), " << qt_object_name << ");" << Qt::endl;
                    }
                }
            }
        }
    }
    // Translate each of the function arguments into qt types
    const MetaArgumentList& arguments = java_function->arguments();
    for(const MetaArgument *argument : arguments) {
        switch(java_function->argumentRemoved(argument->argumentIndex()+1)){
            case ArgumentRemove_UseAsUtilArg:{
            s << INDENT;
            writeTypeInfo(s, argument->type(), option);
            s << argument->indexedName() << "{0};" << Qt::endl;
            }
            break;
        default: break;
        }
    }

    QScopedPointer<MetaType> scope;
    for(const MetaArgument *argument : arguments) {
        MetaType* argumentType = argument->type();
        if(argumentType->getReferenceType()==MetaType::RReference){
            argumentType = argumentType->copy();
            if(argumentType->indirections().isEmpty())
                argumentType->setReferenceType(MetaType::Reference);
            else
                argumentType->setReferenceType(MetaType::NoReference);
            MetaBuilder::decideUsagePattern(argumentType);
            scope.reset(argumentType);
        }
        switch(java_function->argumentRemoved(argument->argumentIndex()+1)){
        case ArgumentRemove_No:{
            if (!(argumentType->isPrimitive() || argumentType->isPrimitiveChar())
                    || java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
                Option _option = Option(option | UseNativeIds);
                if(java_function->isConstructor() && (java_function->declaringClass()->typeEntry()->isInterface() || java_function->declaringClass()->typeEntry()->designatedInterface())){
                    _option = Option(option);
                }
                QString variableName = QLatin1String("__qt_%1").arg(argument->indexedName());
                writeJavaToQt(s,
                              argumentType,
                              variableName,
                              argument->indexedName(),
                              java_function,
                              nullptr,
                              argument->argumentIndex() + 1,
                              _option, "__jni_env",
                              "__qtjambi_scope");
                ThreadAffinity argumentThreadAffinity = java_function->argumentThreadAffinity(argument->argumentIndex() + 1);
                if(argumentThreadAffinity!=ThreadAffinity::None){
                    if(argumentType->typeEntry()->isContainer()){
                        const ContainerTypeEntry* containerType = static_cast<const ContainerTypeEntry*>(argumentType->typeEntry());
                        switch(containerType->type()){
                        case ContainerTypeEntry::ListContainer:
                        case ContainerTypeEntry::SetContainer:
                        case ContainerTypeEntry::VectorContainer:
                        case ContainerTypeEntry::LinkedListContainer:
                        {
                            if(argumentType->instantiations().size()==1){
                                s << INDENT << "for(";
                                writeTypeInfo(s, argumentType->instantiations()[0], Option(ForceConstReference));
                                s << "__elm : " << variableName << "){" << Qt::endl;
                                if(argumentThreadAffinity==ThreadAffinity::Pixmap){
                                    s << INDENT << "    QtJambiAPI::checkThreadOnArgumentQPixmap(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->instantiations()[0]->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                }else if(argumentThreadAffinity==ThreadAffinity::UI){
                                    s << INDENT << "    QtJambiAPI::checkMainThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->instantiations()[0]->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                }else if(argumentType->instantiations()[0]->isQObject()){
                                    s << INDENT << "    QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", __elm);" << Qt::endl;
                                }else if(const ComplexTypeEntry* ownedType = getOwnedType(m_classes.findClass(argumentType->instantiations()[0]->typeEntry()->qualifiedCppName()))){
                                    INDENTATION(INDENT)
                                    if(ownedType->targetTypeSystem()==cls->typeEntry()->targetTypeSystem()){
                                        QString ownerCode = ownedType->threadAffinity();
                                        if(argumentType->instantiations()[0]->indirections().isEmpty()){
                                            if(ownerCode.contains("%1->")){
                                                ownerCode = ownerCode.replace("%1->", "__elm.");
                                            }else if(ownerCode.contains("%1")){
                                                ownerCode = ownerCode.arg("&__elm");
                                            }else if(ownerCode.contains("this->")){
                                                ownerCode = ownerCode.replace("this->", "__elm.");
                                            }else{
                                                ownerCode = "__elm." + ownerCode;
                                            }
                                            s << INDENT << "const QObject* " << variableName << "_owner = ";
                                        }else{
                                            if(ownerCode.contains("%1")){
                                                ownerCode = ownerCode.arg("__elm");
                                            }else if(ownerCode.contains("this->")){
                                                ownerCode = ownerCode.replace("this->", "__elm->");
                                            }else{
                                                ownerCode = "__elm->" + ownerCode;
                                            }
                                            s << INDENT << "const QObject* " << variableName << "_owner = !__elm ? nullptr : ";
                                        }
                                        bool first = true;
                                        for(const QString& line : ownerCode.split(QStringLiteral(u"\n"))){
                                            if(first)
                                                first = false;
                                            else
                                                s << Qt::endl;
                                            s << line;
                                        }
                                        s << ";" << Qt::endl;
                                        s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), " << variableName << "_owner);" << Qt::endl;
                                    }else{
                                        if(argumentType->instantiations()[0]->indirections().isEmpty()){
                                            s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), &__elm);" << Qt::endl;
                                        }else{
                                            s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), __elm);" << Qt::endl;
                                        }
                                    }
                                }
                                s << INDENT << "}" << Qt::endl;
                            }
                        }
                            break;
                        default: break;
                        }
                    }else{
                        if(argumentThreadAffinity==ThreadAffinity::Pixmap){
                            if(!hasDonePixmapCheck){
                                s << INDENT << "QtJambiAPI::checkThreadOnArgumentQPixmap(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                hasDonePixmapCheck = true;
                            }
                        }else if(argumentThreadAffinity==ThreadAffinity::UI){
                            if(!hasDoneUICheck){
                                s << INDENT << "QtJambiAPI::checkMainThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                hasDoneUICheck = true;
                            }
                        }else if(argumentType->isQObject()){
                            s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", " << variableName << ");" << Qt::endl;
                        }else if(const ComplexTypeEntry* ownedType = getOwnedType(m_classes.findClass(argumentType->typeEntry()->qualifiedCppName()))){
                            if(ownedType->targetTypeSystem()==cls->typeEntry()->targetTypeSystem()){
                                QString ownerCode = ownedType->threadAffinity();
                                if(argumentType->indirections().isEmpty()){
                                    if(ownerCode.contains("%1->")){
                                        ownerCode = ownerCode.replace("%1->", variableName+".");
                                    }else if(ownerCode.contains("%1")){
                                        ownerCode = ownerCode.arg("&"+variableName);
                                    }else if(ownerCode.contains("this->")){
                                        ownerCode = ownerCode.replace("this->", variableName+".");
                                    }else{
                                        ownerCode = variableName + "." + ownerCode;
                                    }
                                    s << INDENT << "const QObject* " << variableName << "_owner = ";
                                }else{
                                    if(ownerCode.contains("%1")){
                                        ownerCode = ownerCode.arg(variableName);
                                    }else if(ownerCode.contains("this->")){
                                        ownerCode = ownerCode.replace("this->", variableName+"->");
                                    }else{
                                        ownerCode = variableName + "->" + ownerCode;
                                    }
                                    s << INDENT << "const QObject* " << variableName << "_owner = !" << variableName << " ? nullptr : ";
                                }
                                bool first = true;
                                for(const QString& line : ownerCode.split(QStringLiteral(u"\n"))){
                                    if(first)
                                        first = false;
                                    else
                                        s << Qt::endl;
                                    s << line;
                                }
                                s << ";" << Qt::endl;
                                s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), " << variableName << "_owner);" << Qt::endl;
                            }else{
                                if(argumentType->indirections().isEmpty()){
                                    s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), &" << variableName << ");" << Qt::endl;
                                }else{
                                    s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), " << variableName << ");" << Qt::endl;
                                }
                            }
                        }
                    }
                }
                writeOwnership(s, java_function, nullptr, argument->indexedName(), variableName, argument->argumentIndex()+1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
            }
        }
            break;
            case ArgumentRemove_Remove:{
            if(java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)){
                Option _option = Option(option | UseNativeIds);
                if(java_function->isConstructor() && (java_function->declaringClass()->typeEntry()->isInterface() || java_function->declaringClass()->typeEntry()->designatedInterface())){
                    _option = Option(option);
                }
                writeJavaToQt(s,
                              argumentType,
                              "__qt_" + argument->indexedName(),
                              argument->indexedName(),
                              java_function,
                              nullptr,
                              argument->argumentIndex() + 1,
                              _option, "__jni_env",
                              "__qtjambi_scope");
            }
        }
            break;
        default: break;
        }
    }
}

void CppImplGenerator::writeReferenceCount(QTextStream &s, const ReferenceCount &refCount,
                                        const MetaArgument* argument, int argumentIndex, const MetaFunction *java_function, const QString &__jni_env, const QString &thisName) {
    if (refCount.action == ReferenceCount::Ignore)
        return;

    QString argumentName;
    if(argumentIndex==-1){
        argumentName = thisName;
    }else if(argumentIndex==0){
        argumentName = QStringLiteral(u"__qt_return_value");
    }else if(argument){
        argumentName = argument->indexedName();
    }else{
        return;
    }
    bool nullPointersDisabled = argument && argument->isNullPointerDisabled(java_function);

    QScopedPointer<Indentation> indentation;
    QString condition = refCount.condition;
    condition = condition.replace(QStringLiteral(u"%in"), argumentName);
    condition = condition.replace(QStringLiteral(u"%arg"), argumentName);
    condition = condition.replace(QStringLiteral(u"%this"), thisName);
    condition = condition.replace(QStringLiteral(u"%0"), QStringLiteral(u"__qt_return_value"));
    condition = condition.replace(QStringLiteral(u"%return"), QStringLiteral(u"__qt_return_value"));
    for(const MetaArgument* _argument : java_function->arguments()){
        condition = condition.replace(QStringLiteral(u"%")+QString::number(_argument->argumentIndex()+1), _argument->indexedName());
    }
    if (refCount.action != ReferenceCount::Set) {
        if(nullPointersDisabled){
            if (!refCount.condition.isEmpty()){
                s << INDENT << "if (" << condition << ") {" << Qt::endl;
                indentation.reset(new Indentation(INDENT));
            }
        }else{
            s << INDENT << "if (" << argumentName;
            if (!condition.isEmpty())
                s << " && " << condition;
            s << ") {" << Qt::endl;
            indentation.reset(new Indentation(INDENT));
        }
    } else if (!condition.isEmpty()){
        s << INDENT << "if (" << condition << ") {" << Qt::endl;
        indentation.reset(new Indentation(INDENT));
    }

    QString className = java_function->declaringClass()->typeEntry()->qualifiedTargetLangName().replace(u'.', u'/');
    if(java_function->isAbstract()){
        if(java_function->ownerClass()->typeEntry()->lookupName().endsWith(QStringLiteral(u"$Impl$ConcreteWrapper"))){
            QString lookupName = java_function->ownerClass()->typeEntry()->lookupName();
            lookupName.chop(16);
            className = java_function->ownerClass()->typeEntry()->javaPackage().replace(u'.', u'/') + "/" + lookupName;
        }else if(java_function->ownerClass()->typeEntry()->lookupName().endsWith(QStringLiteral(u"$ConcreteWrapper"))){
            QString lookupName = java_function->declaringClass()->typeEntry()->lookupName();
            if(lookupName.endsWith(QStringLiteral(u"$ConcreteWrapper")))
                lookupName.chop(16);
            className = java_function->declaringClass()->typeEntry()->javaPackage().replace(u'.', u'/') + "/" + lookupName;
        }
    }
    switch (refCount.action) {
    case ReferenceCount::Put:
    {
        QString keyArgumentName = java_function->arguments().at(int(refCount.keyArgument) - 1)->indexedName();
        s << INDENT << "Java::QtJambi::ReferenceUtility::putReferenceCount(" << __jni_env << ", " << thisName << ", ";
        if(java_function->ownerClass()->isInterface() || java_function->ownerClass()->extractInterface()){
            s << "JavaAPI::resolveClass(" << __jni_env << ", \"" << className << "\")";
        }else{
            s << "nullptr";
        }
        s << ", " << __jni_env << "->NewStringUTF(\"" << refCount.variableName << "\"), " << ( refCount.threadSafe ? "true" : "false") << ", " << ( java_function->isStatic() ? "true" : "false") << ", " << keyArgumentName << ", " << argumentName << ");" << Qt::endl;
    }
    break;
    case ReferenceCount::ClearAdd:
    case ReferenceCount::Add:
        s << INDENT << "Java::QtJambi::ReferenceUtility::addReferenceCount(" << __jni_env << ", " << thisName << ", ";
        if(java_function->ownerClass()->isInterface() || java_function->ownerClass()->extractInterface()){
            s << "JavaAPI::resolveClass(" << __jni_env << ", \"" << className << "\")";
        }else{
            s << "nullptr";
        }
        s << ", " << __jni_env << "->NewStringUTF(\"" << refCount.variableName << "\"), " << ( refCount.threadSafe ? "true" : "false") << ", " << ( java_function->isStatic() ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
        break;
    case ReferenceCount::ClearAddAll:
    case ReferenceCount::AddAll:
        s << INDENT << "Java::QtJambi::ReferenceUtility::addAllReferenceCount(" << __jni_env << ", " << thisName << ", ";
        if(java_function->ownerClass()->isInterface() || java_function->ownerClass()->extractInterface()){
            s << "JavaAPI::resolveClass(" << __jni_env << ", \"" << className << "\")";
        }else{
            s << "nullptr";
        }
        s << ", " << __jni_env << "->NewStringUTF(\"" << refCount.variableName << "\"), " << ( refCount.threadSafe ? "true" : "false") << ", " << ( java_function->isStatic() ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
        break;
    case ReferenceCount::Set: {
        s << INDENT << "Java::QtJambi::ReferenceUtility::setReferenceCount(" << __jni_env << ", " << thisName << ", ";
        if(java_function->ownerClass()->isInterface() || java_function->ownerClass()->extractInterface()){
            s << "JavaAPI::resolveClass(" << __jni_env << ", \"" << className << "\")";
        }else{
            s << "nullptr";
        }
        s << ", " << __jni_env << "->NewStringUTF(\"" << refCount.variableName << "\"), " << ( refCount.threadSafe ? "true" : "false") << ", " << ( java_function->isStatic() ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
    }
    break;
    default:
        break;
    }
    if(!indentation.isNull()){
        indentation.reset();
        s << INDENT << "}" << Qt::endl;
    }
}

void CppImplGenerator::writeConstructor(QTextStream &s, const MetaFunction *java_function,
                                        const QList<MetaArgument *>& arguments,
                                        const MetaClass *java_class, bool isDeclarative) {
    Q_ASSERT(java_class);

    const MetaClass *cls = java_class ? java_class : java_function->ownerClass();

    QString java_function_signature = java_function->originalSignature().isEmpty() ? java_function->minimalSignature() : java_function->originalSignature();
    s << "// new " << java_function_signature << Qt::endl
      << "void __qt_construct_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
    if(isDeclarative){
        s << "_declarative";
    }else{
        QString jniSignature;
        {
            QTextStream s2(&jniSignature);
            const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
            int argumentCounter = 1;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                s2 << jni_signature(annotationFreeTypeName(argumentMod.modified_type), JNISignatureFormat(NoModification | SlashesAndStuff));
                ++argumentCounter;
            }
            for(const MetaArgument *argument : arguments) {
                if (argument && java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No) {
                    if(java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
                        QScopedPointer<MetaType> cpy(argument->type()->copy());
                        cpy->setConstant(false);
                        cpy->setReferenceType(MetaType::NoReference);
                        QList<bool> indirections = cpy->indirections();
                        if(!indirections.isEmpty()){
                            indirections.removeLast();
                            cpy->setIndirections(indirections);
                        }
                        MetaBuilder::decideUsagePattern(cpy.get());
                        QString modified_type = jni_signature(cpy.get(), JNISignatureFormat(NoModification | SlashesAndStuff));
                        if(modified_type=="I"){
                            s2 << "Ljava/nio/IntBuffer;";
                        }else if(modified_type=="B"){
                            s2 << "Ljava/nio/ByteBuffer;";
                        }else if(modified_type=="C"){
                            s2 << "Ljava/nio/CharBuffer;";
                        }else if(modified_type=="S"){
                            s2 << "Ljava/nio/ShortBuffer;";
                        }else if(modified_type=="J"){
                            s2 << "Ljava/nio/LongBuffer;";
                        }else if(modified_type=="F"){
                            s2 << "Ljava/nio/FloatBuffer;";
                        }else if(modified_type=="D"){
                            s2 << "Ljava/nio/DoubleBuffer;";
                        }else{
                            s2 << "Ljava/nio/Buffer;";
                        }
                    }else if(java_function->useArgumentAsArray(argument->argumentIndex() + 1)){
                        QScopedPointer<MetaType> cpy(argument->type()->copy());
                        cpy->setConstant(false);
                        cpy->setReferenceType(MetaType::NoReference);
                        QList<bool> indirections = cpy->indirections();
                        if(!indirections.isEmpty()){
                            indirections.removeLast();
                            cpy->setIndirections(indirections);
                        }
                        MetaBuilder::decideUsagePattern(cpy.get());
                        s2 << "[" << jni_signature(cpy.get(), JNISignatureFormat(NoModification | SlashesAndStuff));
                        if(java_function->insertUtilArgument(argument->argumentIndex() + 1)){
                            int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                                    QString modified_type = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                                    if (modified_type.isEmpty())
                                        s2 << jni_signature(lengthParam->type(), JNISignatureFormat(NoModification | SlashesAndStuff));
                                    else
                                        s2 << jni_signature(annotationFreeTypeName(modified_type), JNISignatureFormat(NoModification | SlashesAndStuff));
                                }
                            }
                        }
                    }else{
                        QString modified_type = java_function->typeReplaced(argument->argumentIndex() + 1);
                        if(java_function->useArgumentAsSlotContext(argument->argumentIndex() + 1) && modified_type.isEmpty()){
                            modified_type = QStringLiteral(u"io.qt.core.QObject");
                        }
                        if (modified_type.isEmpty())
                            s2 << jni_signature(argument->type(), JNISignatureFormat(NoModification | SlashesAndStuff));
                        else
                            s2 << jni_signature(annotationFreeTypeName(modified_type), JNISignatureFormat(NoModification | SlashesAndStuff));
                    }
                }
                ++argumentCounter;
                while(addedArguments.first.contains(argumentCounter)){
                    const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                    s2 << jni_signature(annotationFreeTypeName(argumentMod.modified_type), JNISignatureFormat(NoModification | SlashesAndStuff));
                    ++argumentCounter;
                }
            }
            for(const ArgumentModification& mod : addedArguments.second){
                s2 << jni_signature(annotationFreeTypeName(mod.modified_type), JNISignatureFormat(NoModification | SlashesAndStuff));
            }
        }
        QString marshalledArguments = MetaFunction::marshalledArguments(arguments, java_function->isConstant());
        s << marshalledArguments;
        QList<ArgumentList>& argumentLists = m_nativeConstructorNames[java_function];
        argumentLists << ArgumentList{arguments, marshalledArguments, jniSignature};
    }
    s << "(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, bool";
    if(cls->isQObject() && !cls->isAbstract() && cls->generateShellClass())
        s << " __qtjambi_has_derivedMetaObject";
    s << ", bool";
    if(!cls->isAbstract() && cls->hasVirtualDestructor() && cls->generateShellClass())
        s << " __qtjambi_has_overrides";
    s << ", bool __qtjambi_is_generic)" << Qt::endl
      << "{" << Qt::endl;
    INDENTATION(INDENT)
    s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"construct " << java_function_signature << "\")" << Qt::endl;

    QStringList pps = getFunctionPPConditions(java_function);
    if(!cls->typeEntry()->ppCondition().isEmpty() && !pps.contains(cls->typeEntry()->ppCondition())){
        if(pps.isEmpty() || !cls->typeEntry()->ppCondition().contains("|"))
            pps << cls->typeEntry()->ppCondition();
        else
            pps << "(" + cls->typeEntry()->ppCondition() + ")";
    }
    if(!pps.isEmpty()){
        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    }

    QString lines;
    {
        QTextStream s(&lines);

        bool has_argument_referenceCounts = false;
        QMap<int,QList<ReferenceCount>> referenceCounts;
        referenceCounts[-1] = java_function->referenceCounts(java_function->implementingClass(), -1);
        for (const MetaArgument* argument : qAsConst(arguments)) {
            if(argument)
                referenceCounts[argument->argumentIndex()+1] = java_function->referenceCounts(java_function->implementingClass(), argument->argumentIndex()+1);
        }
        for(const QList<ReferenceCount>& list : qAsConst(referenceCounts)){
            for(const ReferenceCount& refCount : list) {
                if (refCount.action != ReferenceCount::Ignore) {
                    has_argument_referenceCounts = true;
                    break;
                }
            }
            if(has_argument_referenceCounts)
                break;
        }

        bool hasDonePixmapCheck = false;
        bool hasDoneUICheck = false;
        bool hasDoneConstructorCheck = false;
        Option options = Option(NoOption);
        int counter = 0;
        QScopedPointer<MetaType> scope;
        const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
        for(const MetaArgument *argument : arguments) {
            if(argument){
                switch (java_function->argumentRemoved(argument->argumentIndex()+1)) {
                case ArgumentRemove_UseAsUtilArg:
                    {
                        s << INDENT << "size_t " << argument->indexedName() << " = 0;" << Qt::endl;
                    }
                    break;
                default: break;
                }
            }
        }
        bool hasPos1 = hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::Position1}, TS::NativeCode);
        if(hasPos1){
            s << INDENT << "if(__qtjambi_is_generic){" << Qt::endl;
            {
                INDENTATION(INDENT)
                writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Position1, TS::NativeCode, QStringLiteral(u"__jni_env"), QStringLiteral(u"__qtjambi_scope"));
            }
            s << INDENT << "}" << Qt::endl;
        }
        int argumentCounter = 1;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
            QString jniType = jniName(argumentMod.modified_type);
            s << INDENT << jniType << " " << argumentMod.modified_name << " = ";
            if(jniType=="jobjectArray"
                    || jniType=="jlongArray"
                    || jniType=="jintArray"
                    || jniType=="jshortArray"
                    || jniType=="jbyteArray"
                    || jniType=="jbooleanArray"
                    || jniType=="jcharArray"
                    || jniType=="jdoubleArray"
                    || jniType=="jfloatArray"
                    || jniType=="jarray"
                    || jniType=="jstring"
                    || jniType=="jthrowable"
                    || jniType=="jclass"){
                s << "static_cast<" << jniType << ">(__java_arguments[" << counter++ << "].l);" << Qt::endl;
            }else{
                s << "__java_arguments[" << counter++ << "].";
                if(jniType=="jlong"){
                    s << 'j';
                }else if(jniType=="jint"){
                    s << 'i';
                }else if(jniType=="jshort"){
                    s << 's';
                }else if(jniType=="jbyte"){
                    s << 'b';
                }else if(jniType=="jboolean"){
                    s << 'z';
                }else if(jniType=="jchar"){
                    s << 'c';
                }else if(jniType=="jfloat"){
                    s << 'f';
                }else if(jniType=="jdouble"){
                    s << 'd';
                }else{
                    s << 'l';
                }
                s << ';' << Qt::endl;
            }
            ++argumentCounter;
        }
        for(const MetaArgument *argument : arguments) {
            if(argument){
                MetaType* argumentType = argument->type();
                if(argumentType->getReferenceType()==MetaType::RReference){
                    argumentType = argumentType->copy();
                    if(argumentType->indirections().isEmpty())
                        argumentType->setReferenceType(MetaType::Reference);
                    else
                        argumentType->setReferenceType(MetaType::NoReference);
                    MetaBuilder::decideUsagePattern(argumentType);
                    scope.reset(argumentType);
                }
                switch (java_function->argumentRemoved(argument->argumentIndex()+1)) {
                case ArgumentRemove_No:
                    {
                    QString jniType;
                    QString typeReplaced = java_function->typeReplaced(argument->argumentIndex()+1, &jniType);
                    if(jniType.isEmpty()){
                        if(java_function->useArgumentAsSlotContext(argument->argumentIndex() + 1) && typeReplaced.isEmpty()){
                            typeReplaced = QStringLiteral(u"io.qt.core.QObject");
                        }
                        if(typeReplaced.isEmpty()){
                            if(java_function->useArgumentAsArray(argument->argumentIndex()+1)
                                && !java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
                                QScopedPointer<MetaType> array(argument->type()->copy());
                                array->setConstant(false);
                                array->setReferenceType(MetaType::NoReference);
                                QScopedPointer<MetaType> cpy(argument->type()->copy());
                                cpy->setConstant(false);
                                cpy->setReferenceType(MetaType::NoReference);
                                QList<bool> indirections = cpy->indirections();
                                if(!indirections.isEmpty()){
                                    indirections.removeLast();
                                    cpy->setIndirections(indirections);
                                }
                                MetaBuilder::decideUsagePattern(cpy.get());
                                array->setTypeUsagePattern(MetaType::ArrayPattern);
                                array->setArrayElementType(cpy.get());
                                typeReplaced = jni_signature(array.get(), JNISignatureFormat(ReturnType));
                            }
                        }
                        if(!typeReplaced.isEmpty())
                            jniType = CppGenerator::jniName(annotationFreeTypeName(typeReplaced));
                        else
                            jniType = translateType(argumentType, options);
                    }
                    s << INDENT << jniType << " " << argument->indexedName() << " = ";
                    bool isJObject = false;
                    if(jniType=="jobjectArray"
                            || jniType=="jlongArray"
                            || jniType=="jintArray"
                            || jniType=="jshortArray"
                            || jniType=="jbyteArray"
                            || jniType=="jbooleanArray"
                            || jniType=="jcharArray"
                            || jniType=="jdoubleArray"
                            || jniType=="jfloatArray"
                            || jniType=="jarray"
                            || jniType=="jstring"
                            || jniType=="jthrowable"
                            || jniType=="jclass"){
                        isJObject = true;
                        s << "static_cast<" << jniType << ">(__java_arguments[" << counter++ << "].l);" << Qt::endl;
                    }else{
                        s << "__java_arguments[" << counter++ << "].";
                        if(jniType=="jlong"){
                            s << 'j';
                        }else if(jniType=="jint"){
                            s << 'i';
                        }else if(jniType=="jshort"){
                            s << 's';
                        }else if(jniType=="jbyte"){
                            s << 'b';
                        }else if(jniType=="jboolean"){
                            s << 'z';
                        }else if(jniType=="jchar"){
                            s << 'c';
                        }else if(jniType=="jfloat"){
                            s << 'f';
                        }else if(jniType=="jdouble"){
                            s << 'd';
                        }else{
                            isJObject = true;
                            s << 'l';
                        }
                        s << ';' << Qt::endl;
                    }
                    if(!java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)
                        && java_function->useArgumentAsArray(argument->argumentIndex() + 1)
                        && java_function->insertUtilArgument(argument->argumentIndex() + 1)){
                        int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                                ++argumentCounter;
                                typeReplaced = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                                if(!typeReplaced.isEmpty())
                                    jniType = CppGenerator::jniName(annotationFreeTypeName(typeReplaced));
                                else
                                    jniType = translateType(lengthParam->type(), NoOption);
                                s << INDENT << jniType << " __qt_offset_of_" << argument->indexedName() << " = ";
                                if(jniType=="jobjectArray"
                                    || jniType=="jlongArray"
                                    || jniType=="jintArray"
                                    || jniType=="jshortArray"
                                    || jniType=="jbyteArray"
                                    || jniType=="jbooleanArray"
                                    || jniType=="jcharArray"
                                    || jniType=="jdoubleArray"
                                    || jniType=="jfloatArray"
                                    || jniType=="jarray"
                                    || jniType=="jstring"
                                    || jniType=="jthrowable"
                                    || jniType=="jclass"){
                                    s << "static_cast<" << jniType << ">(__java_arguments[" << counter++ << "].l);" << Qt::endl;
                                }else{
                                    s << "__java_arguments[" << counter++ << "].";
                                    if(jniType=="jlong"){
                                        s << 'j';
                                    }else if(jniType=="jint"){
                                        s << 'i';
                                    }else if(jniType=="jshort"){
                                        s << 's';
                                    }else if(jniType=="jbyte"){
                                        s << 'b';
                                    }else if(jniType=="jboolean"){
                                        s << 'z';
                                    }else if(jniType=="jchar"){
                                        s << 'c';
                                    }else if(jniType=="jfloat"){
                                        s << 'f';
                                    }else if(jniType=="jdouble"){
                                        s << 'd';
                                    }else{
                                        s << 'l';
                                    }
                                    s << ';' << Qt::endl;
                                }
                            }
                        }
                    }
                    if (!(argumentType->isPrimitive() || argumentType->isPrimitiveChar())
                            || java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
                        QString variableName = QString("__qt_%1").arg(argument->indexedName());
                        writeJavaToQt(s,
                                      argumentType,
                                      variableName,
                                      argument->indexedName(),
                                      java_function,
                                      nullptr,
                                      argument->argumentIndex() + 1,
                                      options, "__jni_env",
                                      "__qtjambi_scope");
                        writeOwnership(s, java_function, nullptr, argument->indexedName(), variableName, argument->argumentIndex()+1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                        ThreadAffinity argumentThreadAffinity = java_function->argumentThreadAffinity(argument->argumentIndex() + 1);
                        if(isJObject
                                && argumentType->typeEntry()->isComplex()
                                && (
                                    argumentThreadAffinity!=ThreadAffinity::None
                                    || argument->argumentName().startsWith(QLatin1String("parent"))
                                )){
                            if(argumentType->typeEntry()->isContainer()){
                                const ContainerTypeEntry* containerType = static_cast<const ContainerTypeEntry*>(argumentType->typeEntry());
                                switch(containerType->type()){
                                case ContainerTypeEntry::ListContainer:
                                case ContainerTypeEntry::SetContainer:
                                case ContainerTypeEntry::VectorContainer:
                                case ContainerTypeEntry::LinkedListContainer:
                                {
                                    if(argumentType->instantiations().size()==1){
                                        s << INDENT << "for(";
                                        writeTypeInfo(s, argumentType->instantiations()[0], Option(ForceConstReference));
                                        s << "__elm : " << variableName << "){" << Qt::endl;
                                        if(argumentThreadAffinity==ThreadAffinity::Pixmap){
                                            s << INDENT << "    QtJambiAPI::checkThreadOnArgumentQPixmap(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->instantiations()[0]->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                        }else if(argumentThreadAffinity==ThreadAffinity::UI){
                                            s << INDENT << "    QtJambiAPI::checkMainThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->instantiations()[0]->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                        }else if(argumentType->instantiations()[0]->isQObject()){
                                            s << INDENT << "    QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", __elm);" << Qt::endl;
                                        }else if(const ComplexTypeEntry* ownedType = getOwnedType(m_classes.findClass(argumentType->instantiations()[0]->typeEntry()->qualifiedCppName()))){
                                            INDENTATION(INDENT)
                                            if(ownedType->targetTypeSystem()==cls->typeEntry()->targetTypeSystem()){
                                                QString ownerCode = ownedType->threadAffinity();
                                                if(argumentType->instantiations()[0]->indirections().isEmpty()){
                                                    if(ownerCode.contains("%1->")){
                                                        ownerCode = ownerCode.replace("%1->", "__elm.");
                                                    }else if(ownerCode.contains("%1")){
                                                        ownerCode = ownerCode.arg("&__elm");
                                                    }else if(ownerCode.contains("this->")){
                                                        ownerCode = ownerCode.replace("this->", "__elm.");
                                                    }else{
                                                        ownerCode = "__elm." + ownerCode;
                                                    }
                                                    s << INDENT << "const QObject* " << variableName << "_owner = ";
                                                }else{
                                                    if(ownerCode.contains("%1")){
                                                        ownerCode = ownerCode.arg("__elm");
                                                    }else if(ownerCode.contains("this->")){
                                                        ownerCode = ownerCode.replace("this->", "__elm->");
                                                    }else{
                                                        ownerCode = "__elm->" + ownerCode;
                                                    }
                                                    s << INDENT << "const QObject* " << variableName << "_owner = !__elm ? nullptr : ";
                                                }
                                                bool first = true;
                                                for(const QString& line : ownerCode.split(QStringLiteral(u"\n"))){
                                                    if(first)
                                                        first = false;
                                                    else
                                                        s << Qt::endl;
                                                    s << line;
                                                }
                                                s << ";" << Qt::endl;
                                                s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), " << variableName << "_owner);" << Qt::endl;
                                            }else{
                                                if(argumentType->instantiations()[0]->indirections().isEmpty()){
                                                    s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), &__elm);" << Qt::endl;
                                                }else{
                                                    s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), __elm);" << Qt::endl;
                                                }
                                            }
                                        }
                                        s << INDENT << "}" << Qt::endl;
                                    }
                                }
                                    break;
                                default: break;
                                }
                            }else{
                                if(argumentThreadAffinity==ThreadAffinity::Pixmap){
                                    if(!hasDonePixmapCheck){
                                        s << INDENT << "QtJambiAPI::checkThreadOnArgumentQPixmap(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                        hasDonePixmapCheck = true;
                                    }
                                }else if(argumentThreadAffinity==ThreadAffinity::UI){
                                    if(!hasDoneUICheck){
                                        s << INDENT << "QtJambiAPI::checkMainThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << argumentType->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
                                        hasDoneUICheck = true;
                                    }
                                }else if(argumentType->isQObject()){
                                    if(!hasDoneConstructorCheck){
                                        if(cls->isQWindow() || cls->isQAction()){
                                            s << INDENT << "QtJambiAPI::checkThreadConstructingQWindow(__jni_env, typeid(" << cls->qualifiedCppName() << "), " << variableName << ");" << Qt::endl;
                                        }else if(cls->isQWidget()){
                                            s << INDENT << "QtJambiAPI::checkThreadConstructingQWidget(__jni_env, typeid(" << cls->qualifiedCppName() << "), " << variableName << ");" << Qt::endl;
                                        }else{
                                            s << INDENT << "QtJambiAPI::checkThreadOnParent(__jni_env, " << variableName << ");" << Qt::endl;
                                        }
                                        hasDoneConstructorCheck = true;
                                    }
                                }else{
                                    if(const ComplexTypeEntry* ownedType = getOwnedType(m_classes.findClass(argumentType->typeEntry()->qualifiedCppName()))){
                                        if(ownedType->targetTypeSystem()==cls->typeEntry()->targetTypeSystem()){
                                            QString ownerCode = ownedType->threadAffinity();
                                            if(argumentType->indirections().isEmpty()){
                                                if(ownerCode.contains("%1->")){
                                                    ownerCode = ownerCode.replace("%1->", variableName+".");
                                                }else if(ownerCode.contains("%1")){
                                                    ownerCode = ownerCode.arg("&"+variableName);
                                                }else if(ownerCode.contains("this->")){
                                                    ownerCode = ownerCode.replace("this->", variableName+".");
                                                }else{
                                                    ownerCode = variableName + "." + ownerCode;
                                                }
                                                s << INDENT << "const QObject* " << variableName << "_owner = ";
                                            }else{
                                                if(ownerCode.contains("%1")){
                                                    ownerCode = ownerCode.arg(variableName);
                                                }else if(ownerCode.contains("this->")){
                                                    ownerCode = ownerCode.replace("this->", variableName+"->");
                                                }else{
                                                    ownerCode = variableName + "->" + ownerCode;
                                                }
                                                s << INDENT << "const QObject* " << variableName << "_owner = !" << variableName << " ? nullptr : ";
                                            }
                                            bool first = true;
                                            for(const QString& line : ownerCode.split(QStringLiteral(u"\n"))){
                                                if(first)
                                                    first = false;
                                                else
                                                    s << Qt::endl;
                                                s << line;
                                            }
                                            s << ";" << Qt::endl;
                                            s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), " << variableName << "_owner);" << Qt::endl;
                                        }else{
                                            if(argumentType->indirections().isEmpty()){
                                                s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), &" << variableName << ");" << Qt::endl;
                                            }else{
                                                s << INDENT << "QtJambiAPI::checkThreadOnArgument(__jni_env, \"" << argument->argumentName() << "\", typeid(" << ownedType->qualifiedCppName() << "), " << variableName << ");" << Qt::endl;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //s << INDENT << "Q_UNUSED(__qt_" << argument->indexedName() << ")" << Qt::endl;
                    }else{
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                    }
                    }
                    break;
                default: break;
                }
            }
            ++argumentCounter;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                QString jniType = jniName(argumentMod.modified_type);
                s << INDENT << jniType << " " << argumentMod.modified_name << " = ";
                if(jniType=="jobjectArray"
                        || jniType=="jlongArray"
                        || jniType=="jintArray"
                        || jniType=="jshortArray"
                        || jniType=="jbyteArray"
                        || jniType=="jbooleanArray"
                        || jniType=="jcharArray"
                        || jniType=="jdoubleArray"
                        || jniType=="jfloatArray"
                        || jniType=="jarray"
                        || jniType=="jstring"
                        || jniType=="jthrowable"
                        || jniType=="jclass"){
                    s << "static_cast<" << jniType << ">(__java_arguments[" << counter++ << "].l);" << Qt::endl;
                }else{
                    s << "__java_arguments[" << counter++ << "].";
                    if(jniType=="jlong"){
                        s << 'j';
                    }else if(jniType=="jint"){
                        s << 'i';
                    }else if(jniType=="jshort"){
                        s << 's';
                    }else if(jniType=="jbyte"){
                        s << 'b';
                    }else if(jniType=="jboolean"){
                        s << 'z';
                    }else if(jniType=="jchar"){
                        s << 'c';
                    }else if(jniType=="jfloat"){
                        s << 'f';
                    }else if(jniType=="jdouble"){
                        s << 'd';
                    }else{
                        s << 'l';
                    }
                    s << ';' << Qt::endl;
                }
                ++argumentCounter;
            }
        }
        if(!hasDoneConstructorCheck){
            if(cls->isQObject()){
                if(cls->isQWidget() || cls->isQWindow() || cls->isQAction()){
                    bool hasParent = false;
                    for(const MetaArgument *argument : arguments) {
                        if(argument
                                && argument->argumentName().startsWith(QLatin1String("parent"))
                                && java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No){
                            hasParent = true;
                            break;
                        }
                    }
                    if(!hasParent){
                        if(cls->isQWidget()){
                            s << INDENT << "QtJambiAPI::checkThreadConstructingQWidget(__jni_env, typeid(" << cls->qualifiedCppName() << "), nullptr);" << Qt::endl;
                        }else if(cls->isQWindow() || cls->isQAction()){
                            s << INDENT << "QtJambiAPI::checkThreadConstructingQWindow(__jni_env, typeid(" << cls->qualifiedCppName() << "), nullptr);" << Qt::endl;
                        }
                    }
                }
            }else if(java_function->isUIThreadAffine()){
                s << INDENT << "QtJambiAPI::checkMainThreadConstructing(__jni_env, typeid(" << cls->qualifiedCppName() << "));" << Qt::endl;
            }else if(java_function->isPixmapThreadAffine()){
                s << INDENT << "QtJambiAPI::checkThreadConstructingQPixmap(__jni_env, typeid(" << cls->qualifiedCppName() << "));" << Qt::endl;
            }
        }

        for(int i=0; i<addedArguments.second.size(); ++i){
            QString jniType = addedArguments.second[i].modified_jni_type.isEmpty() ? jniName(addedArguments.second[i].modified_type) : addedArguments.second[i].modified_jni_type;
            s << INDENT << jniType << " " << addedArguments.second[i].modified_name << " = ";
            if(jniType=="jobjectArray"
                    || jniType=="jlongArray"
                    || jniType=="jintArray"
                    || jniType=="jshortArray"
                    || jniType=="jbyteArray"
                    || jniType=="jbooleanArray"
                    || jniType=="jcharArray"
                    || jniType=="jdoubleArray"
                    || jniType=="jfloatArray"
                    || jniType=="jarray"
                    || jniType=="jstring"
                    || jniType=="jthrowable"
                    || jniType=="jclass"){
                s << "static_cast<" << jniType << ">(__java_arguments[" << counter++ << "].l);" << Qt::endl;
            }else{
                s << "__java_arguments[" << counter++ << "].";
                if(jniType=="jlong"){
                    s << 'j';
                }else if(jniType=="jint"){
                    s << 'i';
                }else if(jniType=="jshort"){
                    s << 's';
                }else if(jniType=="jbyte"){
                    s << 'b';
                }else if(jniType=="jboolean"){
                    s << 'z';
                }else if(jniType=="jchar"){
                    s << 'c';
                }else if(jniType=="jfloat"){
                    s << 'f';
                }else if(jniType=="jdouble"){
                    s << 'd';
                }else{
                    s << 'l';
                }
                s << ';' << Qt::endl;
            }
            s << INDENT << "Q_UNUSED(" << addedArguments.second[i].modified_name << ')' << Qt::endl;
        }
        if(counter==0)
            s << INDENT << "Q_UNUSED(__java_arguments)" << Qt::endl;
        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Beginning, TS::NativeCode, "__jni_env", "__qtjambi_scope");
        bool hasPos2 = hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::Position2}, TS::NativeCode);
        if(hasPos2){
            s << INDENT << "if(__qtjambi_is_generic){" << Qt::endl;
            {
                INDENTATION(INDENT)
                writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Position2, TS::NativeCode, QStringLiteral(u"__jni_env"), QStringLiteral(u"__qtjambi_scope"));
            }
            s << INDENT << "}" << Qt::endl;
        }
        bool hasPos5 = hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::Position5}, TS::NativeCode);

        QString argumentCall;
        {
            QTextStream s(&argumentCall);
            if(java_function->attributes() & MetaAttributes::BracketCall){
                s << "{";
            }else{
                s << "(";
            }
            writeFunctionCallArguments(s, java_function, arguments, "__qt_", Option(SkipRemovedArguments));
            if(java_function->attributes() & MetaAttributes::BracketCall){
                s << "}";
            }else{
                s << ")";
            }
        }
        if(java_function->isBlockExceptions())
            s << INDENT << "QtJambiExceptionBlocker __qj_exceptionBlocker;" << Qt::endl;
        if(!java_class->generateShellClass() || !java_class->instantiateShellClass()){
            s << INDENT << cls->fullQualifiedCppName() << " *__qt_this = new(__qtjambi_ptr) ";
            s << cls->fullQualifiedCppName();
            QStringList templateParameters;
            for(MetaTemplateParameter* param : java_function->templateParameters()){
                if(!param->isImplicit()){
                    if(!param->instantiation().isEmpty()){
                        if(param->instantiationType()){
                            QString str;
                            QTextStream _s(&str);
                            writeTypeInfo(_s, param->instantiationType());
                            if(!str.trimmed().isEmpty())
                                templateParameters << str;
                        }else
                            templateParameters << param->instantiation();
                    }else if(param->type()){
                        if(param->defaultType().isEmpty()){
                            QString str;
                            QTextStream _s(&str);
                            writeTypeInfo(_s, param->type());
                            if(!str.trimmed().isEmpty())
                                templateParameters << str;
                        }
                    }else if(param->defaultType().isEmpty()){
                        templateParameters.clear();
                        break;
                    }
                }
            }
            if(!templateParameters.isEmpty()){
                s << '<' << templateParameters.join(QStringLiteral(u", ")) << '>';
            }
        }else if(!java_class->hasVirtualDestructor()){
            s << INDENT << cls->fullQualifiedCppName() << " *__qt_this = new(__qtjambi_ptr) ";
            if(isDeclarative){
                if(java_class->instantiateShellClass()){
                    s << "QtJambiAPI::DeclarativeShellElement<" << shellClassName(cls) << ">";
                }else{
                    s << "QtJambiAPI::DeclarativeElement<" << shellClassName(cls) << ">";
                }
            }else{
                s << shellClassName(cls);
            }
        }else if(cls->isAbstract()){
            s << INDENT << cls->fullQualifiedCppName() << " *__qt_this = new(__qtjambi_ptr) ";
            if(isDeclarative){
                if(java_class->instantiateShellClass()){
                    s << "QtJambiAPI::DeclarativeShellElement<" << oshellClassName(cls) << ">";
                }else{
                    s << "QtJambiAPI::DeclarativeElement<" << oshellClassName(cls) << ">";
                }
            }else{
                s << oshellClassName(cls);
            }
        }else{
            s << INDENT << cls->fullQualifiedCppName() << " *__qt_this;" << Qt::endl
              << INDENT << "if(__qtjambi_has_overrides)" << Qt::endl
              << INDENT << "    __qt_this = new(__qtjambi_ptr) ";
            if(isDeclarative){
                if(java_class->instantiateShellClass()){
                    s << "QtJambiAPI::DeclarativeShellElement<" << oshellClassName(cls) << ">";
                }else{
                    s << "QtJambiAPI::DeclarativeElement<" << oshellClassName(cls) << ">";
                }
            }else{
                s << oshellClassName(cls);
            }
            s << argumentCall << ";" << Qt::endl;
            if(cls->isQObject()){
                s << INDENT << "else if(__qtjambi_has_derivedMetaObject)" << Qt::endl
                  << INDENT << "    __qt_this = new(__qtjambi_ptr) ";
                if(isDeclarative){
                    if(java_class->instantiateShellClass()){
                        s << "QtJambiAPI::DeclarativeShellElement<" << mshellClassName(cls) << ">";
                    }else{
                        s << "QtJambiAPI::DeclarativeElement<" << mshellClassName(cls) << ">";
                    }
                }else{
                    s << mshellClassName(cls);
                }
                s << argumentCall << ";" << Qt::endl;
            }
            s << INDENT << "else" << Qt::endl
              << INDENT << "    __qt_this = new(__qtjambi_ptr) ";
            if(isDeclarative){
                if(java_class->instantiateShellClass()){
                    s << "QtJambiAPI::DeclarativeShellElement<" << shellClassName(cls) << ">";
                }else{
                    s << "QtJambiAPI::DeclarativeElement<" << shellClassName(cls) << ">";
                }
            }else{
                s << shellClassName(cls);
            }
        }
        s << argumentCall << ";" << Qt::endl
          << INDENT << "Q_UNUSED(__qt_this)" << Qt::endl;

        if(has_argument_referenceCounts || hasPos5){
            s << INDENT << "if(__qtjambi_is_generic){" << Qt::endl;
            {
                INDENTATION(INDENT)
                for(const ReferenceCount& refCount : qAsConst(referenceCounts[-1]))
                    writeReferenceCount(s, refCount, nullptr, -1, java_function, QStringLiteral(u"__jni_env"), QStringLiteral(u"__jni_object"));

                // We must ensure we retain a Java hard-reference over the native method call
                // so that the GC will not destroy the C++ object too early.  At this point we
                // have called the native method call so can manage referenceCount issues.
                // First the input arguments
                for (const MetaArgument* argument : qAsConst(arguments)) {
                    if(argument){
                        for(const ReferenceCount& refCount : referenceCounts[argument->argumentIndex()+1])
                            writeReferenceCount(s, refCount, argument, argument->argumentIndex()+1, java_function, QStringLiteral(u"__jni_env"), QStringLiteral(u"__jni_object"));
                    }
                }
                writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Position5, TS::NativeCode, QStringLiteral(u"__jni_env"), QStringLiteral(u"__qtjambi_scope"));
            }
            s << INDENT << "}" << Qt::endl;
        }else if(!hasPos1 && !hasPos1){
            s << INDENT << "Q_UNUSED(__qtjambi_is_generic)" << Qt::endl;
        }
        if(java_function->isBlockExceptions())
            s << INDENT << "__qj_exceptionBlocker.release(__jni_env);" << Qt::endl;
        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, QStringLiteral(u"__jni_env"), QStringLiteral(u"__qtjambi_scope"));
    }

    if(lines.contains("__qtjambi_scope")){
        s << INDENT << "QtJambiScope __qtjambi_scope;" << Qt::endl;
    }
    s << lines;
    if(!lines.contains("__jni_object")){
        s << INDENT << "Q_UNUSED(__jni_object)" << Qt::endl;
    }
    if(!lines.contains("__jni_env")){
        s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
    }
    if(lines.count("__qt_this")==1){
        s << INDENT << "Q_UNUSED(__qt_this)" << Qt::endl;
    }

    if(!pps.isEmpty()){
        s << "#else" << Qt::endl
          << INDENT << "Q_UNUSED(__jni_object)" << Qt::endl
          << INDENT << "Q_UNUSED(__java_arguments)" << Qt::endl
          << INDENT << "Q_UNUSED(__qtjambi_ptr)" << Qt::endl
          << INDENT << "Q_UNUSED(__qtjambi_is_generic)" << Qt::endl;
        if(!cls->isAbstract() && cls->hasVirtualDestructor() && cls->generateShellClass()){
            if(cls->isQObject())
                s << INDENT << "Q_UNUSED(__qtjambi_has_derivedMetaObject)" << Qt::endl;
            s << INDENT << "Q_UNUSED(__qtjambi_has_overrides)" << Qt::endl;
        }
        s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl
          << "#endif // " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeFinalConstructor(QTextStream &s, const MetaFunction *java_function, const MetaClass *java_class) {
    Q_ASSERT(java_class);

    if (!java_function->isConstructor()
            || java_class->hasUnimplmentablePureVirtualFunction()
            || java_function->isModifiedRemoved(TS::NativeCode)
            || (!java_class->generateShellClass() && !(java_function->originalAttributes() & MetaAttributes::Public)))
        return;

    QList<QList<MetaArgument *>> defaultArgumentOverloads;
    if(java_function->arguments().isEmpty()){
        defaultArgumentOverloads << QList<MetaArgument *>();
    }else{
        for(MetaArgument *argument : java_function->arguments()) {
            ArgumentRemove argumentRemoved = java_function->argumentRemoved(argument->argumentIndex()+1);
            bool hasDefault = !argument->originalDefaultValueExpression().isEmpty();
            if(defaultArgumentOverloads.isEmpty()){
                if(argumentRemoved==ArgumentRemove_Remove && hasDefault){
                    defaultArgumentOverloads << QList<MetaArgument *>();
                }else{
                    if(argumentRemoved!=ArgumentRemove_Remove && hasDefault){
                        defaultArgumentOverloads << ( QList<MetaArgument *>() << nullptr);
                    }
                    defaultArgumentOverloads << ( QList<MetaArgument *>() << argument);
                }
            }else{
                if(!hasDefault
                    || argumentRemoved==ArgumentRemove_UseAsUtilArg){
                    for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                        defaultArgumentOverloads[i] << argument;
                    }
                }else if(argumentRemoved!=ArgumentRemove_Remove){
                    QList<QList<MetaArgument *>> copy;
                    for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                        if(defaultArgumentOverloads[i].last()){
                            copy << (QList<MetaArgument *>(defaultArgumentOverloads[i]) << argument);
                        }
                        defaultArgumentOverloads[i] << nullptr;
                    }
                    defaultArgumentOverloads << copy;
                }
            }
        }
    }
    for(int i=0; i<defaultArgumentOverloads.size(); ++i){
        writeConstructor(s, java_function, defaultArgumentOverloads[i], java_class, false);
    }

    if(java_function->isModifiedRemoved(TS::TargetLangCode))
        return;

    const MetaClass *cls = java_class ? java_class : java_function->ownerClass();

    QString java_function_signature = cls->fullQualifiedCppName() + "::" + (java_function->originalSignature().isEmpty() ? java_function->minimalSignature() : java_function->originalSignature());

    s << "// " << java_function_signature << Qt::endl;

    //QString qt_object_name = "__qt_this";
    const QString java_object_name = "__jni_object";
    // function signature...
    uint options = StandardJNISignature;
    writeFunctionName(s, java_function, cls, options);
    s << Qt::endl;
    writeFinalFunctionArguments(s, java_function, java_object_name);
    s << "{" << Qt::endl;
    INDENTATION(INDENT)
    s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"" << java_function_signature << "\")" << Qt::endl;

    s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
    {
        INDENTATION(INDENT)
        // Avoid compiler warnings when the variables are unused
        QStringList pps = getFunctionPPConditions(java_function);
        if(!cls->typeEntry()->ppCondition().isEmpty() && !pps.contains(cls->typeEntry()->ppCondition())){
            if(pps.isEmpty() || !cls->typeEntry()->ppCondition().contains("|"))
                pps << cls->typeEntry()->ppCondition();
            else
                pps << "(" + cls->typeEntry()->ppCondition() + ")";
        }
        if(!pps.isEmpty()){
            s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
        }
        if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept())
            s << INDENT << "QtJambiExceptionRaiser __qt_exceptionRaiser;" << Qt::endl;
        const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
        const QList<ArgumentList>& argumentLists = m_nativeConstructorNames[java_function];
        const ArgumentList& argumentList = argumentLists.last();
        QList<MetaArgument *> arguments;
        for(MetaArgument * argument : argumentList.arguments){
            if(argument && java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No)
                arguments << argument;
        }
        int additionalArguments = 0;
        QString argumentRef;
        if(additionalArguments + arguments.size() + addedArguments.first.size() + addedArguments.second.size() == 0){
            argumentRef = "nullptr";
        }else if(additionalArguments + arguments.size() + addedArguments.first.size() + addedArguments.second.size() == 1){
            argumentRef = "&arguments";
            s << "        jvalue arguments;" << Qt::endl;
            QString jniType;
            QString argName;
            if(!arguments.isEmpty()){
                const MetaArgument * argument = arguments[0];
                QString typeReplaced = java_function->typeReplaced(argument->argumentIndex()+1);
                if(!typeReplaced.isEmpty())
                    jniType = CppGenerator::jniName(annotationFreeTypeName(typeReplaced));
                else
                    jniType = translateType(argument->type(), NoOption);
                argName = argument->indexedName();
            }else if(addedArguments.first.size()==1){
                jniType = addedArguments.first.first().modified_jni_type.isEmpty() ? jniName(addedArguments.first.first().modified_type) : addedArguments.first.first().modified_jni_type;
                argName = addedArguments.first.first().modified_name;
            }else{
                jniType = addedArguments.second.first().modified_jni_type.isEmpty() ? jniName(addedArguments.second.first().modified_type) : addedArguments.second.first().modified_jni_type;
                argName = addedArguments.second.first().modified_name;
            }
            s << "        arguments.";
            if(jniType=="jint")
                s << "i";
            else if(jniType=="jlong")
                s << "j";
            else if(jniType=="jshort")
                s << "s";
            else if(jniType=="jbyte")
                s << "b";
            else if(jniType=="jchar")
                s << "c";
            else if(jniType=="jboolean")
                s << "z";
            else if(jniType=="jfloat")
                s << "f";
            else if(jniType=="jdouble")
                s << "d";
            else
                s << "l";
            s << " = " << argName << ";" << Qt::endl;
        }else{
            argumentRef = "arguments";
            s << "        jvalue arguments[" << (additionalArguments + arguments.size() + addedArguments.first.size() + addedArguments.second.size()) << "];" << Qt::endl;
            int argumentCounter = 1;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                QString jniType = jniName(argumentMod.modified_type);
                s << "        arguments[" << argumentCounter-1 << "].";
                if(jniType=="jint")
                    s << "i";
                else if(jniType=="jlong")
                    s << "j";
                else if(jniType=="jshort")
                    s << "s";
                else if(jniType=="jbyte")
                    s << "b";
                else if(jniType=="jchar")
                    s << "c";
                else if(jniType=="jboolean")
                    s << "z";
                else if(jniType=="jfloat")
                    s << "f";
                else if(jniType=="jdouble")
                    s << "d";
                else
                    s << "l";
                s << " = " << argumentMod.modified_name << ";" << Qt::endl;
                ++argumentCounter;
            }
            for(int i=0; i<arguments.size(); ++i){
                const MetaArgument * argument = arguments[i];
                QString jniType;
                QString typeReplaced = java_function->typeReplaced(argument->argumentIndex()+1);
                if(!typeReplaced.isEmpty())
                    jniType = CppGenerator::jniName(annotationFreeTypeName(typeReplaced));
                else
                    jniType = translateType(argument->type(), NoOption);
                s << "        arguments[" << argumentCounter-1 << "].";
                if(jniType=="jint")
                    s << "i";
                else if(jniType=="jlong")
                    s << "j";
                else if(jniType=="jshort")
                    s << "s";
                else if(jniType=="jbyte")
                    s << "b";
                else if(jniType=="jchar")
                    s << "c";
                else if(jniType=="jboolean")
                    s << "z";
                else if(jniType=="jfloat")
                    s << "f";
                else if(jniType=="jdouble")
                    s << "d";
                else
                    s << "l";
                s << " = " << argument->indexedName() << ";" << Qt::endl;
                if(!java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)
                    && java_function->useArgumentAsArray(argument->argumentIndex() + 1)
                    && java_function->insertUtilArgument(argument->argumentIndex() + 1)){
                    int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                            ++argumentCounter;
                            typeReplaced = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                            if(!typeReplaced.isEmpty())
                                jniType = CppGenerator::jniName(annotationFreeTypeName(typeReplaced));
                            else
                                jniType = translateType(lengthParam->type(), NoOption);
                            s << "        arguments[" << argumentCounter-1 << "].";
                            if(jniType=="jint")
                                s << "i";
                            else if(jniType=="jlong")
                                s << "j";
                            else if(jniType=="jshort")
                                s << "s";
                            else if(jniType=="jbyte")
                                s << "b";
                            else if(jniType=="jchar")
                                s << "c";
                            else if(jniType=="jboolean")
                                s << "z";
                            else if(jniType=="jfloat")
                                s << "f";
                            else if(jniType=="jdouble")
                                s << "d";
                            else
                                s << "l";
                            s << " = __qt_offset_of_" << argument->indexedName() << ";" << Qt::endl;
                        }
                    }
                }
                ++argumentCounter;
                while(addedArguments.first.contains(argumentCounter)){
                    const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                    QString jniType = jniName(argumentMod.modified_type);
                    s << "        arguments[" << argumentCounter-1 << "].";
                    if(jniType=="jint")
                        s << "i";
                    else if(jniType=="jlong")
                        s << "j";
                    else if(jniType=="jshort")
                        s << "s";
                    else if(jniType=="jbyte")
                        s << "b";
                    else if(jniType=="jchar")
                        s << "c";
                    else if(jniType=="jboolean")
                        s << "z";
                    else if(jniType=="jfloat")
                        s << "f";
                    else if(jniType=="jdouble")
                        s << "d";
                    else
                        s << "l";
                    s << " = " << argumentMod.modified_name << ";" << Qt::endl;
                    ++argumentCounter;
                }
            }
            for(int i=0; i<addedArguments.second.size(); ++i){
                QString jniType = jniName(addedArguments.second[i].modified_type);
                s << "        arguments[" << argumentCounter-1 << "].";
                if(jniType=="jint")
                    s << "i";
                else if(jniType=="jlong")
                    s << "j";
                else if(jniType=="jshort")
                    s << "s";
                else if(jniType=="jbyte")
                    s << "b";
                else if(jniType=="jchar")
                    s << "c";
                else if(jniType=="jboolean")
                    s << "z";
                else if(jniType=="jfloat")
                    s << "f";
                else if(jniType=="jdouble")
                    s << "d";
                else
                    s << "l";
                s << " = " << addedArguments.second[i].modified_name << ";" << Qt::endl;
            }
        }
        if(java_class->isQObject()){
            s << "        ";
            if(java_class->typeEntry()->isQAbstractItemModel()){
                s << "QtJambiModelShell";
            }else if(java_class->typeEntry()->isQThread()){
                s << "QtJambiThreadShell";
            }else{
                s << "QtJambiShell";
            }
            s << "::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_"
                                                            << java_class->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_')
                                                            << argumentList.marshalledArguments
                                                            << ", sizeof(" << shellClassName(java_class)
                                                            << "), typeid(" << java_class->qualifiedCppName() << "), "
                                                            << cls->returnScopeRequired() << ", "
                                                            << java_class->qualifiedCppName() << "::staticMetaObject, "
                                                            << (java_class->generateShellClass() ? "true, " : "false, ")
                                                            << (java_class->hasMetaObjectFunction() ? "true, false, " : "false, false, ")
                                                            << argumentRef << ");" << Qt::endl;
        }else{
            QString ownerFunction;
            if (cls->typeEntry()->isThreadAffine()){
                const ComplexTypeEntry *entry = cls->typeEntry();
                const QString& ownerCode = entry->threadAffinity();
                if(ownerCode==QLatin1String("main") || ownerCode==QLatin1String("ui")){
                    ownerFunction = QLatin1String("&QtJambiAPI::mainThreadOwner");
                }else if(ownerCode==QLatin1String("pixmap")){
                    ownerFunction = QLatin1String("&QtJambiAPI::getPixmapOwner");
                }else{
                    ownerFunction = QLatin1String("&owner_function_") + cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                }
            }
            QList<const MetaType *> instantiations;
            if(const ContainerTypeEntry* ctype = java_class->findContainerSuperClass(&instantiations)){
                QString accessName = ctype->qualifiedCppName();
                switch(ctype->type()){
                case ContainerTypeEntry::StackContainer:
                    if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                        accessName = "QVector";
                        break;
                    }
                    Q_FALLTHROUGH();
                case ContainerTypeEntry::QueueContainer:
                    accessName = "QList";
                    break;
                default: break;
                }
                s << "        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_"
                  << java_class->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_')
                  << argumentList.marshalledArguments
                  << ", sizeof(" << shellClassName(java_class) << "), typeid(" << java_class->qualifiedCppName() << "), "
                  << cls->returnScopeRequired() << ", "
                  << (java_class->generateShellClass() ? "true" : "false")
                  << ", QtJambiPrivate::" << accessName << "Access<";
                int i=0;
                for(const MetaType * instantiation : instantiations){
                    if(i>0)
                        s << ", ";
                    writeTypeInfo(s, instantiation, SkipName);
                    ++i;
                }
                s << ">::newInstance()";
                if (hasDeleter(cls)) {
                    s << ", &deleter_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                }else if(java_class->generateShellClass()){
                    s << ", [](void* ptr,bool){ delete reinterpret_cast<" << shellClassName(cls) << "*>(ptr); }";
                }else if(cls->typeEntry()->isDestructorPublic()){
                    s << ", [](void* ptr,bool){ delete reinterpret_cast<" << cls->qualifiedCppName() << "*>(ptr); }";
                }
                if(!ownerFunction.isEmpty()){
                    s << ", " << ownerFunction;
                }
                s << ", " << argumentRef << ");" << Qt::endl;
            }else{
                s << "        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_"
                  << java_class->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_')
                  << argumentList.marshalledArguments
                  << ", sizeof(" << shellClassName(java_class) << "), typeid(" << java_class->qualifiedCppName() << "), " << cls->returnScopeRequired() << ", " << (java_class->generateShellClass() ? "true" : "false");
                if (hasDeleter(cls)) {
                    s << ", &deleter_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                }else if(java_class->generateShellClass()){
                    s << ", [](void* ptr,bool){ delete reinterpret_cast<" << shellClassName(cls) << "*>(ptr); }";
                }else if(cls->typeEntry()->isDestructorPublic()){
                    s << ", [](void* ptr,bool){ delete reinterpret_cast<" << cls->qualifiedCppName() << "*>(ptr); }";
                }
                if(!ownerFunction.isEmpty()){
                    s << ", " << ownerFunction;
                }
                s << ", " << argumentRef << ");" << Qt::endl;
            }
        }
        if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept())
            s << INDENT << "__qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;
        writeOwnership(s, java_function, nullptr, "__jni_object", "__jni_object", -1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
        if(!pps.isEmpty()){
            s << "#else" << Qt::endl;
            s << INDENT << "Q_UNUSED(" << java_object_name << ")" << Qt::endl;
            s << INDENT << "Q_UNUSED(__jni_class)" << Qt::endl;
            for(const MetaArgument *argument : arguments) {
                s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
            }
            s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
            s << "#endif // " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
        }
    }
    s << INDENT << "}QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
    s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
    s << INDENT << "}QTJAMBI_TRY_END" << Qt::endl;
    s << Qt::endl << "}";
    s << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeFinalFunction(QTextStream &s, const MetaFunction *java_function,
        const MetaClass *java_class) {
    Q_ASSERT(java_class);

    if (java_function->wasPrivate())
        return ;

    if (java_function->isModifiedRemoved(TS::NativeCode))
        return;

    if (java_function->isSignal())
        return;

    if (java_function->isPrivateSignal())
        return;

//    if (!java_class->generateShellClass() && (!(java_function->originalAttributes() & AbstractMetaAttributes::Public) || java_function->isAbstract()))
//        return;
    if (!java_class->generateShellClass() && !(java_function->originalAttributes() & MetaAttributes::Public))
        return;

    const MetaClass *cls = java_class ? java_class : java_function->ownerClass();

    QString java_function_signature = cls->fullQualifiedCppName() + "::" + (java_function->originalSignature().isEmpty() ? java_function->minimalSignature() : java_function->originalSignature());

    s << "// " << java_function_signature << Qt::endl;

    const MetaType *function_type = java_function->type();

    QString qt_object_name = java_function->isStatic() ? cls->fullQualifiedCppName() : "__qt_this";
    const QString java_object_name = java_function->isStatic() ? "__jni_class" : "__jni_object";
    // function signature...
    uint options = StandardJNISignature;
    writeFunctionName(s, java_function, cls, options);
    s << Qt::endl;
    writeFinalFunctionArguments(s, java_function, java_object_name);
    s << "{" << Qt::endl;
    INDENTATION(INDENT)
    if(java_function->isStatic())
        s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"" << java_function_signature << "\")" << Qt::endl;

    QStringList pps = getFunctionPPConditions(java_function);
    if(!cls->typeEntry()->ppCondition().isEmpty() && !pps.contains(cls->typeEntry()->ppCondition())){
        if(pps.isEmpty() || !cls->typeEntry()->ppCondition().contains("|"))
            pps << cls->typeEntry()->ppCondition();
        else
            pps << "(" + cls->typeEntry()->ppCondition() + ")";
    }
    const MetaArgumentList& arguments = java_function->arguments();

    QString lines;
    {
        QTextStream s(&lines);
        QString return_type = jniReturnName(java_function);
        if(return_type!="void"){
            s << INDENT << return_type << " __java_return_value{0};" << Qt::endl;
            if(return_type=="jint"){

            }
        }
        s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
        {
            INDENTATION(INDENT)
            // Avoid compiler warnings when the variables are unused
            bool hasThisVar = false;
            if(!java_function->isStatic() && !java_function->isConstructor()){
                hasThisVar = java_function->needsCallThrough() || !java_function->implementingClass()->typeEntry()->isNativeIdBased();
            }
            if(!pps.isEmpty()){
                s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
            }
            if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept())
                s << INDENT << "QtJambiExceptionRaiser __qt_exceptionRaiser;" << Qt::endl;
            if (cls->isFinal() && (!java_function->isAbstract() || !java_function->isFinalInTargetLang()) && !java_function->wasPublic()) {
                QString debug = QString("protected function '%1' in final class '%2'")
                                .arg(java_function->signature(),java_class->name());
                ReportHandler::warning(debug);
                // Avoid compiler warnings for unused parameters

                for(const MetaArgument *argument : arguments) {
                    ArgumentRemove ar = java_function->argumentRemoved(argument->argumentIndex()+1);
                    if(ar==ArgumentRemove_UseAsUtilArg){
                        s << INDENT;
                        writeTypeInfo(s, argument->type(), NoOption);
                        s << argument->indexedName() << "{0};" << Qt::endl;
                    }
                    if(ar!=ArgumentRemove_Remove)
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                }
                s << INDENT << "return;";
            } else {
                bool hasShell = cls->generateShellClass();
                if (!java_function->isStatic() && !java_function->isConstructor()) {
                    if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                        s << INDENT;
                        if(java_function->isConstant())
                            s << "const ";
                        s << cls->fullQualifiedCppName() << " *" << qt_object_name << " = ";
                        if(cls->typeEntry()->isNativeIdBased()){
                            s << "QtJambiAPI::interfaceFromNativeId<" << cls->fullQualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        }else{
                            s << "QtJambiAPI::convertJavaInterfaceToNative<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }
                    }else{
                        s << INDENT;
                        if(java_function->isConstant())
                            s << "const ";
                        s << cls->fullQualifiedCppName() << " *" << qt_object_name << " = ";
                        if(cls->typeEntry()->isNativeIdBased()){
                            s << "QtJambiAPI::objectFromNativeId<" << cls->fullQualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        }else if(cls->isQObject()){
                            s << "QtJambiAPI::convertJavaObjectToQObject<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }else{
                            s << "QtJambiAPI::convertJavaObjectToNative<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                        }
                    }
                    s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, " << qt_object_name << ");" << Qt::endl;
                    s << INDENT << "QTJAMBI_NATIVE_INSTANCE_METHOD_CALL(\"" << java_function_signature << "\", ";
                    if(cls->typeEntry()->isNativeIdBased()){
                        s << "__this_nativeId";
                    }else{
                        s << qt_object_name;
                    }
                    s << ")" << Qt::endl;
                }
                if (java_function->isFinalInCpp() && !java_function->isProxyCall() && !java_function->wasPublic() && hasShell) {
                    const QString& name = cls->typeEntry()->designatedInterface() ? cls->extractInterface()->name() : cls->name();
                    if(java_function->isStatic())
                        qt_object_name = name + "_access";
                    else if(java_function->isConstant())
                        qt_object_name = QString("static_cast<const %1*>(%2)").arg(name + "_access",qt_object_name);
                    else
                        qt_object_name = QString("static_cast<%1*>(%2)").arg(name + "_access",qt_object_name);
                    s << INDENT;
                    if(return_type!="void")
                        s << "__java_return_value = ";
                    writeFunctionCall(s, qt_object_name, java_function, "__qt_", Option(EnumAsInts | NoCasts | JNIProxyFunction | SkipRemovedArguments));
                    s << ";" << Qt::endl;
                    if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept()){
                        s << INDENT << "__qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;
                    }
                } else if (!java_function->isFinalInCpp() && !java_function->isStatic() && hasShell) {
                    const QString& name = cls->typeEntry()->designatedInterface() ? cls->extractInterface()->name() : cls->name();
                    if(java_function->isStatic())
                        qt_object_name = name + "_access";
                    else if(java_function->isConstant())
                        qt_object_name = QString("static_cast<const %1*>(%2)").arg(name + "_access",qt_object_name);
                    else
                        qt_object_name = QString("static_cast<%1*>(%2)").arg(name + "_access",qt_object_name);
                    s << INDENT;
                    if(return_type!="void" && !java_function->isSelfReturningFunction())
                        s << "__java_return_value = ";
                    writeFunctionCall(s, qt_object_name, java_function, "__qt_", Option(EnumAsInts | NoCasts | JNIProxyFunction | SkipRemovedArguments));
                    s << ";" << Qt::endl;
                    if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept()){
                        s << INDENT << "__qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;
                    }
                }else{
                    writeFinalFunctionSetup(s, java_function, qt_object_name, cls);
                    writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Beginning, TS::NativeCode, "__jni_env", "__qtjambi_scope");

                    QStringList extra_param;
                    Option option = EnumAsInts;

                    if (java_function->isFinalOverload()) {
                        // otherwise no prefix
                    } else {
                        option = OriginalName;
                    }

                    // Call the Qt function on the java object
                    s << INDENT;

                    QString returnTypeReplacement = java_function->typeReplaced(0);
                    if ((function_type || !returnTypeReplacement.isEmpty()) && !java_function->isSelfReturningFunction()) {
                        const QString qt_return_value = "__qt_return_value";
                        const QString java_return_value = "__java_return_value";
                        bool paren = false;
                        bool noConversion = false;
                        bool hasReturn = false;
                        if (function_type) {
                            if(!java_function->proxyCall().isEmpty() || function_type->typeEntry()->isUnknown()){
                                if(returnTypeReplacement!="void"){
                                    s << "auto " << qt_return_value << " = ";
                                    hasReturn = true;
                                }
                            }else if(function_type->isEnum() && !static_cast<const EnumTypeEntry*>(function_type->typeEntry())->isPublic()){
                                writeTypeInfo(s, function_type, EnumAsInts);
                                s << qt_return_value << " = ";
                                uint size = static_cast<const EnumTypeEntry*>(function_type->typeEntry())->size();
                                s << "qint" << size << "(";
                                paren = true;
                                hasReturn = true;
                            }else if((function_type->isPrimitive() || function_type->isPrimitiveChar())
                                     && returnTypeReplacement.isEmpty()
                                     && !java_function->hasConversionRule(TS::NativeCode, 0)
                                     && (function_type->typeEntry()->qualifiedCppName()=="int"
                                         || function_type->typeEntry()->qualifiedCppName()=="double"
                                         || function_type->typeEntry()->qualifiedCppName()=="float"
                                         || function_type->typeEntry()->qualifiedCppName()=="char"
                                         || function_type->typeEntry()->qualifiedCppName()=="short"
                                         || function_type->typeEntry()->qualifiedCppName()=="bool"
                                         || function_type->typeEntry()->qualifiedCppName()=="long long")){
                                s << java_return_value << " = ";
                                noConversion = true;
                                hasReturn = true;
                            }else{
                                writeTypeInfo(s, function_type, NoOption);
                                s << qt_return_value << " = ";
                                hasReturn = true;
                            }
                        }

                        writeFunctionCall(s, qt_object_name, java_function, {}, option, extra_param);
                        if(paren)
                            s << ")";
                        s << ";" << Qt::endl;
                        if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept())
                            s << INDENT << "__qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;

                        if(!noConversion){
                            if(!java_function->isSelfReturningFunction() && returnTypeReplacement != "void"){
                                writeQtToJava(s, function_type, qt_return_value, java_return_value,
                                              java_function, nullptr, 0, Option(IsReturnType | EnumAsInts | NoTmpVariable),
                                              "__jni_env", hasThisVar ? "__qtjambi_scope" : "");
                            }else if(hasReturn){
                                s << INDENT << "Q_UNUSED(" << qt_return_value << ")" << Qt::endl;
                            }
                        }
                        if(hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::End}, TS::NativeCode)
                                || hasOwnership(java_function, nullptr, 0, java_function->implementingClass(), TS::NativeCode)
                                || java_function->hasConversionRule(TS::NativeCode, 0)){
                            writeOwnership(s, java_function, nullptr, "__qt_this", "__java_this", -1, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                            writeOwnership(s, java_function, nullptr, java_return_value, qt_return_value, 0, cls, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                            writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                        }
                    } else {
                        writeFunctionCall(s, qt_object_name, java_function, {}, option, extra_param);
                        s << ";" << Qt::endl;
                        if(java_function->isRethrowExceptions() || java_function->isBlockExceptions() || java_function->isNoExcept())
                            s << INDENT << "__qt_exceptionRaiser.raise(__jni_env);" << Qt::endl;

                        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TS::NativeCode, "__jni_env", "__qtjambi_scope");
                    }
                }
            }

            if(!pps.isEmpty()){
                s << "#else" << Qt::endl;
                if (java_function->isConstructor())
                    s << INDENT << "Q_UNUSED(" << java_object_name << ")" << Qt::endl;
                else if(!java_function->isStatic()){
                    if(java_function->implementingClass()->typeEntry()->isNativeIdBased()
                            && !java_function->implementingClass()->typeEntry()->designatedInterface())
                        s << INDENT << "Q_UNUSED(__this)" << Qt::endl;
                    if (hasThisVar){
                        if(cls->typeEntry()->isNativeIdBased()){
                            s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
                        }
                    }
                }
                for(const MetaArgument *argument : arguments) {
                    if(java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No)
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                }
                s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                s << "#endif // " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
            }
            if(java_function->isSelfReturningFunction()){
                if(!java_function->implementingClass()->typeEntry()->designatedInterface())
                    s << INDENT << "__java_return_value = __this;" << Qt::endl;
            }
        }
        s << INDENT << "}QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
        s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
        s << INDENT << "}QTJAMBI_TRY_END" << Qt::endl;
        if(return_type!="void")
            s << INDENT << "return __java_return_value;" << Qt::endl;
    }
    if(lines.contains("__qtjambi_scope")){
        s << INDENT << "QtJambiScope __qtjambi_scope" << ( java_function->isStatic() ? "" : (cls->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
        if(!pps.isEmpty()){
            s << INDENT << "Q_UNUSED(__qtjambi_scope)" << Qt::endl;
        }
    }
    if(!java_function->isStatic()){
        if(java_function->implementingClass()->typeEntry()->isNativeIdBased()
                && !java_function->implementingClass()->typeEntry()->designatedInterface())
            s << INDENT << "Q_UNUSED(__this)" << Qt::endl;
        if(!lines.contains("__this_nativeId") && cls->typeEntry()->isNativeIdBased())
            s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
    }
    s << lines;
    s << Qt::endl << "}";
    s << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeAssignment(QTextStream &s, const QString &destName, const QString &srcName,
                                       const MetaFunction *java_function,
                                       const MetaType *java_type) {
    if (java_type->isArray()) {
        const QMap<QString,QString>& arrayConversions = m_arrayConversions[quintptr(java_function)];
        QList<QList<int>> allEntries;
        for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
            const QPair<int,QString>& pair = java_type->arrayElementCounts()[i];
            if(allEntries.isEmpty()){
                for(int j=0; j<pair.first; ++j){
                    allEntries << QList<int>({j});
                }
            }else{
                QList<QList<int>> nextEntries;
                for(const QList<int>& entries : allEntries){
                    for(int j=0; j<pair.first; ++j){
                        nextEntries << (QList<int>(entries) << j);
                    }
                }
                allEntries = nextEntries;
            }
        }
        for(const QList<int>& entries : allEntries){
            QString braces;
            for(int idx : entries){
                braces += "[" + QString::number(idx) + "]";
            }
            if(arrayConversions.contains(srcName + braces))
                s << INDENT << destName << braces << " = " << arrayConversions[srcName + braces] << ";" << Qt::endl;
        }
    } else if(java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type()==ContainerTypeEntry::std_array){
        const QMap<QString,QString>& arrayConversions = m_arrayConversions[quintptr(java_function)];
        QList<QList<int>> allEntries;
        {
            int arrayElementCounts = java_type->instantiations()[1]->typeEntry()->name().toInt();
            const QPair<int,QString>& pair = {arrayElementCounts, java_type->instantiations()[1]->typeEntry()->name()};
            if(allEntries.isEmpty()){
                for(int j=0; j<pair.first; ++j){
                    allEntries << QList<int>({j});
                }
            }else{
                QList<QList<int>> nextEntries;
                for(const QList<int>& entries : allEntries){
                    for(int j=0; j<pair.first; ++j){
                        nextEntries << (QList<int>(entries) << j);
                    }
                }
                allEntries = nextEntries;
            }
        }
        for(const QList<int>& entries : allEntries){
            QString braces;
            for(int idx : entries){
                braces += "[" + QString::number(idx) + "]";
            }
            if(arrayConversions.contains(srcName + braces))
                s << INDENT << destName << braces << " = " << arrayConversions[srcName + braces] << ";" << Qt::endl;
        }
    } else {
        s << INDENT << destName << " = ";
        if(java_type->isEnum() || java_type->isFlags()){
            s << java_type->typeEntry()->qualifiedCppName() << "(" << srcName << ");" << Qt::endl;
        }else if(java_type->isPrimitive()){
            s << "static_cast<" << java_type->typeEntry()->qualifiedCppName() << ">(" << srcName << ");" << Qt::endl;
        }else if(java_type->isPrimitiveChar()){
            s << java_type->typeEntry()->qualifiedCppName() << "(" << srcName << ");" << Qt::endl;
        }else{
            s << srcName << ";" << Qt::endl;
        }
    }
}

void CppImplGenerator::writeFieldAccessors(QTextStream &s, const MetaField *java_field) {
    Q_ASSERT(java_field && (java_field->isPublic() || java_field->isProtected()));

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

    // Setter
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
    if (mod.isWritable() && isWritable) {
        // Write public override for protected fields
        if (!setter->wasPublic()) {
            writeFunctionSignature(s, setter, setter->ownerClass(), {}, Option(EnumAsInts | UseNativeIds | JNIProxyFunction));
            s << Qt::endl
            << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                Q_ASSERT(setter->arguments().count() > 0);
                QString lines;
                {
                    QTextStream s(&lines);
                    writeFinalFunctionSetup(s, setter, "this", setter->ownerClass(), NoOption);
                    const MetaArgument *argument = setter->arguments().at(0);
                    QString thisRef = java_field->isStatic()
                                      ? QString("%1::").arg(java_field->enclosingClass()->qualifiedCppName())
                                      : QString("this->%1::").arg(java_field->enclosingClass()->qualifiedCppName());
                    QString src;
                    if (argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                        src = argument->indexedName();
                    else
                        src = "__qt_" + argument->indexedName();
                    writeAssignment(s, thisRef + java_field->name(), src, setter, argument->type());
                }
                if(lines.contains("__qtjambi_scope")){
                    s << INDENT << "QtJambiScope __qtjambi_scope" << (setter->isStatic() ? "" : (cls->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
                    if(!setter->isStatic()){
                        if(cls->typeEntry()->isNativeIdBased()){
                            if(!lines.contains("__jni_env"))
                                s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
                        }
                    }
                }else{
                    if (!java_field->isStatic()) {
                        if(cls->typeEntry()->isNativeIdBased()){
                            if(!lines.contains("__this_nativeId"))
                                s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
                        }else{
                            if(!lines.contains("__this"))
                                s << INDENT << "Q_UNUSED(__this)" << Qt::endl;
                        }
                        if(!lines.contains("__jni_env"))
                            s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
                    }
                }
                s << lines;
            }
            s << "}" << Qt::endl << Qt::endl;
        }

        writeFunctionName(s, setter, setter->ownerClass());
        s << Qt::endl;
        writeFinalFunctionArguments(s, setter, "__java_object");
        s  << "{" << Qt::endl;

        {
            INDENTATION(INDENT)
            QString lines;
            {
                QTextStream s(&lines);
                s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!setter->isStatic()){
                        if(setter->implementingClass()->typeEntry()->isNativeIdBased()
                                && !setter->implementingClass()->typeEntry()->designatedInterface())
                            s << INDENT << "Q_UNUSED(__this)" << Qt::endl;
                    }
                    if(!pps.isEmpty())
                        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                    if (!setter->isStatic()) {
                        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                            s << INDENT;
                            if(setter->isConstant())
                                s << "const ";
                            s << cls->fullQualifiedCppName() << " *__qt_this = ";
                            if(cls->typeEntry()->isNativeIdBased()){
                                s << "QtJambiAPI::interfaceFromNativeId<" << cls->fullQualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                            }else{
                                s << "QtJambiAPI::convertJavaInterfaceToNative<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                            }
                        }else{
                            s << INDENT;
                            if(setter->isConstant())
                                s << "const ";
                            s << cls->fullQualifiedCppName() << " *__qt_this = ";
                            if(cls->typeEntry()->isNativeIdBased()){
                                s << "QtJambiAPI::objectFromNativeId<" << cls->fullQualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                            }else if(cls->isQObject()){
                                s << "QtJambiAPI::convertJavaObjectToQObject<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                            }else{
                                s << "QtJambiAPI::convertJavaObjectToNative<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                            }
                        }
                        s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, __qt_this);" << Qt::endl;
                    }
                    if (setter->wasPublic()) {
                        writeFinalFunctionSetup(s, setter, "__qt_this", setter->ownerClass());

                        Q_ASSERT(setter->arguments().count() == 1);
                        const MetaArgument *argument = setter->arguments().at(0);

                        QString dest;
                        if (setter->isStatic())
                            dest = shellClassName(setter->ownerClass()) + "::";
                        else if(setter->wasPublic())
                            dest = "__qt_this->";
                        else
                            dest = QString("static_cast<%1*>(__qt_this)->").arg(shellClassName(cls));

                        QString src;
                        if (argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                            src = argument->indexedName();
                        else
                            src = "__qt_" + argument->indexedName();
                        writeAssignment(s, dest + java_field->name(), src, setter, argument->type());
                    }else{
                        s << INDENT;
                        if (setter->isStatic())
                            s << shellClassName(setter->ownerClass()) + "::";
                        else if(setter->wasPublic())
                            s << "__qt_this->";
                        else{
                            const QString& name = setter->ownerClass()->typeEntry()->designatedInterface() ? setter->ownerClass()->extractInterface()->name() : cls->name();
                           s << QString("static_cast<%1*>(__qt_this)->").arg(name + "_access");
                        }
                        s << setter->name() << marshalledArguments(setter)
                          << "(__jni_env";
                        if (!setter->isStatic()){
                            s << ", __this";
                            if(cls->typeEntry()->isNativeIdBased())
                                s << "_nativeId";
                        }
                        s << ", " << setter->arguments().at(0)->indexedName() << ");" << Qt::endl;
                    }
                    if(!pps.isEmpty()){
                        s << "#else" << Qt::endl;
                        if(!setter->isStatic() && cls->typeEntry()->isNativeIdBased())
                            s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
                        for(const MetaArgument *argument : setter->arguments()) {
                            s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                        }
                        s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                        s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                    }
                }
                s << INDENT << "}QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
                s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                s << INDENT << "}QTJAMBI_TRY_END" << Qt::endl;
            }
            if(lines.contains("__qtjambi_scope")){
                s << INDENT << "QtJambiScope __qtjambi_scope" << (setter->isStatic() ? "" : (cls->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
                if(!pps.isEmpty()){
                    s << INDENT << "Q_UNUSED(__qtjambi_scope)" << Qt::endl;
                }
            }
            s << lines;
        }
        s << "}" << Qt::endl << Qt::endl;
    }

    if (mod.isReadable()) {
        // Getter
        if (!getter->wasPublic()) {
            if(!pps.isEmpty())
                s << Qt::endl << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
            writeFunctionSignature(s, getter, getter->ownerClass(), "", Option(EnumAsInts | UseNativeIds | JNIProxyFunction));
            s << Qt::endl
            << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                QString lines;
                {
                    QTextStream s(&lines);
                    QString thisRef = java_field->isStatic()
                                      ? QString("%1::").arg(java_field->enclosingClass()->qualifiedCppName())
                                      : QString("this->%1::").arg(java_field->enclosingClass()->qualifiedCppName());
                    s << INDENT;
                    writeTypeInfo(s, getter->type(), Option(/*ForceConstReference | */ArrayAsPointer));
                    s << " __qt_value = " << thisRef << java_field->name() << ";" << Qt::endl;
                    writeQtToJava(s, getter->type(), "__qt_value", "", getter, nullptr, 0, Option(EnumAsInts | DirectReturn | NoTmpVariable), "__jni_env", "__qtjambi_scope");
                    s << ";" << Qt::endl;
                }
                if(lines.contains("__qtjambi_scope")){
                    s << INDENT << "QtJambiScope __qtjambi_scope" << (setter->isStatic() ? "" : (cls->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
                    if(!setter->isStatic()){
                        if(cls->typeEntry()->isNativeIdBased()){
                            if(!lines.contains("__jni_env"))
                                s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
                        }
                    }
                }else{
                    if (!java_field->isStatic()) {
                        if(cls->typeEntry()->isNativeIdBased()){
                            if(!lines.contains("__this_nativeId"))
                                s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
                        }else{
                            if(!lines.contains("__this"))
                                s << INDENT << "Q_UNUSED(__this)" << Qt::endl;
                        }
                        if(!lines.contains("__jni_env"))
                            s << INDENT << "Q_UNUSED(__jni_env)" << Qt::endl;
                    }
                }
                s << lines;
            }
            s << "}" << Qt::endl << Qt::endl;
            if(!pps.isEmpty()){
                s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl << Qt::endl;
            }
        }


        writeFunctionName(s, getter, getter->ownerClass());
        s << Qt::endl;
        writeFinalFunctionArguments(s, getter, "__java_object");
        s << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            QString lines;
            {
                QTextStream s(&lines);
                QString return_type = jniReturnName(getter);
                s << INDENT << return_type << " __java_return_value{0};" << Qt::endl;
                s << INDENT << "QTJAMBI_TRY {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!getter->isStatic()){
                        if(getter->implementingClass()->typeEntry()->isNativeIdBased()
                                && !getter->implementingClass()->typeEntry()->designatedInterface())
                            s << INDENT << "Q_UNUSED(__this)" << Qt::endl;
                    }
                    if(!pps.isEmpty())
                        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                    if (!getter->isStatic()) {
                        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                            s << INDENT;
                            if(getter->isConstant())
                                s << "const ";
                            s << cls->fullQualifiedCppName() << " *__qt_this = ";
                            if(cls->typeEntry()->isNativeIdBased()){
                                s << "QtJambiAPI::interfaceFromNativeId<" << cls->fullQualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                            }else{
                                s << "QtJambiAPI::convertJavaInterfaceToNative<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                            }
                        }else{
                            s << INDENT;
                            if(getter->isConstant())
                                s << "const ";
                            s << cls->fullQualifiedCppName() << " *__qt_this = ";
                            if(cls->typeEntry()->isNativeIdBased()){
                                s << "QtJambiAPI::objectFromNativeId<" << cls->fullQualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                            }else if(cls->isQObject()){
                                s << "QtJambiAPI::convertJavaObjectToQObject<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                            }else{
                                s << "QtJambiAPI::convertJavaObjectToNative<" << cls->fullQualifiedCppName() << ">(__jni_env, __this);" << Qt::endl;
                            }
                        }
                        s << INDENT << "QtJambiAPI::checkNullPointer(__jni_env, __qt_this);" << Qt::endl;
                    }
                    if (getter->wasPublic()) {
                        writeFinalFunctionSetup(s, getter, "__qt_this", getter->ownerClass());

                        //const QString java_return_value = "__java_return_value";
                        QString qt_return_value;
                        if (getter->isStatic())
                            qt_return_value = java_field->enclosingClass()->qualifiedCppName() + "::";
                        else
                            qt_return_value = "__qt_this->";
                        qt_return_value += java_field->name();
                        s << INDENT;
                        writeTypeInfo(s, getter->type(), Option(/*ForceConstReference | */ArrayAsPointer));
                        s << " __qt_value = " << qt_return_value << ";" << Qt::endl;
                        writeQtToJava(s, getter->type(), "__qt_value", "__java_return_value", getter, nullptr, 0, Option(EnumAsInts | NoTmpVariable), "__jni_env", "__qtjambi_scope");
                    }else{
                        s << INDENT << "__java_return_value = ";
                        if (getter->isStatic())
                            s << shellClassName(getter->ownerClass()) + "::";
                        else{
                            const QString& name = getter->ownerClass()->typeEntry()->designatedInterface() ? getter->ownerClass()->extractInterface()->name() : cls->name();
                            s << QString("static_cast<%1*>(__qt_this)->").arg(name + "_access");
                        }
                        s << getter->name() << marshalledArguments(getter)
                          << "(__jni_env";
                        if (!setter->isStatic()){
                            s << ", __this";
                            if(cls->typeEntry()->isNativeIdBased())
                                s << "_nativeId";
                        }
                        s << ");" << Qt::endl;
                    }
                    if(!pps.isEmpty()){
                        s << "#else" << Qt::endl;
                        if(!getter->isStatic())
                            s << INDENT << "Q_UNUSED(__this" << (cls->typeEntry()->isNativeIdBased() ? "_nativeId)" : ")") << Qt::endl;
                        s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                        s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                    }
                }
                s << INDENT << "}QTJAMBI_CATCH(const JavaException& exn){" << Qt::endl;
                s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                s << INDENT << "}QTJAMBI_TRY_END" << Qt::endl;
                s << INDENT << "return __java_return_value;" << Qt::endl;
            }
            if(lines.contains("__qtjambi_scope")){
                s << INDENT << "QtJambiScope __qtjambi_scope" << (getter->isStatic() ? "" : (cls->typeEntry()->isNativeIdBased() ? "(__this_nativeId)" : "(__jni_env, __this)" ) ) << ";" << Qt::endl;
                if(!pps.isEmpty()){
                    s << INDENT << "Q_UNUSED(__qtjambi_scope)" << Qt::endl;
                }
            }
            s << lines;
        }
        s << "}" << Qt::endl << Qt::endl;
    }
}

void CppImplGenerator::writeDeleteAndOwnerFunction(QTextStream &s, const MetaClass *cls) {
    if(hasDeleter(cls)){
        const MetaClass *clsWithPublicDestructor = lookupClassWithPublicDestructor(cls);
        {
            if (clsWithPublicDestructor) {
                QStringList purgeLines;
                QSet<const ComplexTypeEntry *> classes;
                const MetaClass *_cls = cls;
                while(_cls){
                    classes << (_cls->typeEntry()->designatedInterface() ? _cls->typeEntry()->designatedInterface() : _cls->typeEntry());
                    for(const MetaClass *ifc : _cls->interfaces()){
                        classes << (ifc->typeEntry()->designatedInterface() ? ifc->typeEntry()->designatedInterface() : ifc->typeEntry());
                    }
                    _cls = _cls->baseClass();
                }
                bool deleterFound = false;
                for(const ComplexTypeEntry * _cls : qAsConst(classes)){
                    for(const CodeSnip& snip : _cls->codeSnips()) {
                        if (snip.language == TS::DeleterFunction) {
                            purgeLines << snip.code().replace("%this", "_ptr").replace("%class", cls->qualifiedCppName()).split(QStringLiteral(u"\n"));
                            deleterFound = true;
                        }
                    }
                }
                if(!deleterFound){
                    for(const ComplexTypeEntry * _cls : qAsConst(classes)){
                        for(const CodeSnip& snip : _cls->codeSnips()) {
                            if (snip.language == TS::DestructorFunction) {
                                purgeLines << snip.code().replace("%this", "_ptr").replace("%class", cls->qualifiedCppName()).split(QStringLiteral(u"\n"));
                            }
                        }
                    }
                }
                while(!purgeLines.isEmpty()){
                    if(purgeLines.last().trimmed().isEmpty()){
                        purgeLines.takeLast();
                    }else{
                        break;
                    }
                }
                while(!purgeLines.isEmpty()){
                    if(purgeLines.first().trimmed().isEmpty()){
                        purgeLines.takeFirst();
                    }else{
                        break;
                    }
                }
                s << Qt::endl << "void deleter_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(void *ptr, bool isShell)" << Qt::endl << "{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "QTJAMBI_NATIVE_METHOD_CALL(\"qtjambi_deleter for " << cls->qualifiedCppName() << "\")" << Qt::endl;
                    s << INDENT << clsWithPublicDestructor->qualifiedCppName() << " *_ptr = reinterpret_cast<" << clsWithPublicDestructor->qualifiedCppName() << " *>(ptr);" << Qt::endl;
                    if(cls->generateShellClass()){
                        if(cls==clsWithPublicDestructor && !clsWithPublicDestructor->hasVirtualDestructor()){
                            s << INDENT << "if(isShell){" << Qt::endl;
                            s << INDENT << "    delete static_cast<" << shellClassName(clsWithPublicDestructor) << "*>(_ptr);" << Qt::endl;
                            s << INDENT << "}else{" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                if(!purgeLines.isEmpty()){
                                    int sp = -1;
                                    QString spaces;
                                    for(QString line : purgeLines) {
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
                                                for(; sp<purgeLines[0].length(); ++sp){
                                                    if(purgeLines[0][sp]!=QLatin1Char('\t')){
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
                                        if(!line.startsWith(QLatin1Char('#')))
                                            s << INDENT;
                                        s << line << Qt::endl;
                                    }
                                }
                                s << INDENT << "QtJambiAPI::registerNonShellDeletion(ptr);" << Qt::endl;
                                s << INDENT << "delete _ptr;" << Qt::endl;
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else{
                            if(!cls->isQObject()){
                                s << INDENT << "if(!isShell){" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    if(!purgeLines.isEmpty()){
                                        int sp = -1;
                                        QString spaces;
                                        for(QString line : purgeLines) {
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
                                                    for(; sp<purgeLines[0].length(); ++sp){
                                                        if(purgeLines[0][sp]!=QLatin1Char('\t')){
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
                                            if(!line.startsWith(QLatin1Char('#')))
                                                s << INDENT;
                                            s << line << Qt::endl;
                                        }
                                    }
                                    s << INDENT << "QtJambiAPI::registerNonShellDeletion(ptr);" << Qt::endl;
                                }
                                s << INDENT << "}" << Qt::endl;
                            }else{
                                s << INDENT << "Q_UNUSED(isShell)" << Qt::endl;
                            }
                            s << INDENT << "delete _ptr;" << Qt::endl;
                        }
                    }else{
                        if(!cls->isQObject()){
                            s << INDENT << "if(!isShell){" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                if(!purgeLines.isEmpty()){
                                    int sp = -1;
                                    QString spaces;
                                    for(QString line : purgeLines) {
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
                                                for(; sp<purgeLines[0].length(); ++sp){
                                                    if(purgeLines[0][sp]!=QLatin1Char('\t')){
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
                                        if(!line.startsWith(QLatin1Char('#')))
                                            s << INDENT;
                                        s << line << Qt::endl;
                                    }
                                }
                                s << INDENT << "QtJambiAPI::registerNonShellDeletion(ptr);" << Qt::endl;
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else{
                            s << INDENT << "Q_UNUSED(isShell)" << Qt::endl;
                        }
                        s << INDENT << "delete _ptr;" << Qt::endl;
                    }
                }
            }
            s   << "}" << Qt::endl;
        }
    }
    if (cls->typeEntry()->isThreadAffine()){
        const ComplexTypeEntry *entry = cls->typeEntry();
        QString ownerCode = entry->threadAffinity();
        if (!ownerCode.isEmpty() && ownerCode!=QLatin1String("main") && ownerCode!=QLatin1String("ui") && ownerCode!=QLatin1String("pixmap")) {
            s << Qt::endl << "const QObject* owner_function_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(const void *ptr)" << Qt::endl
              << "{" << Qt::endl
              << "    const " << cls->qualifiedCppName() << "* __qt_this = reinterpret_cast<const " << cls->qualifiedCppName() << " *>(ptr);" << Qt::endl;
            if(ownerCode.contains("%1")){
                ownerCode = ownerCode.arg("__qt_this");
            }else if(ownerCode.contains("this->")){
                ownerCode = ownerCode.replace("this->", "__qt_this->");
            }else{
                ownerCode = "__qt_this->" + ownerCode;
            }
            s << "    return ";
            bool first = true;
            for(const QString& line : ownerCode.split(QStringLiteral(u"\n"))){
                if(first)
                    first = false;
                else
                    s << Qt::endl;
                s << line;
            }
            s << ";" << Qt::endl;
            s   << "}" << Qt::endl;
        }
    }
    s << Qt::endl;
}

void writeCodeBlock(QTextStream &s, const QString &code) {
    QStringList lines = code.split('\n');
    QString indent = "    ";
    for(const QString& str : lines) {
        if(str.startsWith("#")){
            s << str << Qt::endl;
        }else{
            s << indent << str << Qt::endl;
        }
    }
}

void CppImplGenerator::writeCustomStructors(QTextStream &s, const ComplexTypeEntry *entry) {
    if (!entry->preferredConversion())
        return ;

    CustomFunction customLegacyConstructor = entry->customConstructor(ComplexTypeEntry::LegacyCopyConstructor);
    CustomFunction customCopyConstructor = entry->customConstructor(ComplexTypeEntry::CopyConstructor);
    CustomFunction customMoveConstructor = entry->customConstructor(ComplexTypeEntry::MoveConstructor);
    CustomFunction customDefaultConstructor = entry->customConstructor(ComplexTypeEntry::DefaultConstructor);
    CustomFunction customDestructor = entry->customDestructor();

    if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
        if (!customDestructor.name.isEmpty()
                || !customLegacyConstructor.name.isEmpty()) {
            s << "// emitting (writeCustomStructors)" << Qt::endl;
        }
        if(!customLegacyConstructor.name.isEmpty()){
            QString placement_name = customLegacyConstructor.placement_name.isEmpty() ? QLatin1String("placement") : customLegacyConstructor.placement_name;
            QString param_name = customLegacyConstructor.param_name.isEmpty() ? QLatin1String("copy") : customLegacyConstructor.param_name;
            s << "void* custom_constructor_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "("
              << "void* " << placement_name << ", const void * __in_var)" << Qt::endl
              << "{" << Qt::endl
              << "    Q_UNUSED(" << placement_name << ")" << Qt::endl
              << "    const " << entry->qualifiedCppName() << "* " << param_name << " = static_cast<const " << entry->qualifiedCppName() << "*>(__in_var);" << Qt::endl
              << "    Q_UNUSED(" << param_name << ")" << Qt::endl;
            writeCodeBlock(s, customLegacyConstructor.code());
            s << "}" << Qt::endl;
        }
    }else{
        if(!customDestructor.name.isEmpty()
                || !customCopyConstructor.name.isEmpty()
                || !customDefaultConstructor.name.isEmpty()
                || !customMoveConstructor.name.isEmpty()){
            s << "// emitting (writeCustomStructors)" << Qt::endl;
        }
        if(!customCopyConstructor.name.isEmpty()){
            QString placement_name = customCopyConstructor.placement_name.isEmpty() ? QLatin1String("placement") : customCopyConstructor.placement_name;
            QString param_name = customCopyConstructor.param_name.isEmpty() ? QLatin1String("copy") : customCopyConstructor.param_name;
            s << "void custom_copy_constructor_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(const QtPrivate::QMetaTypeInterface *metaTypeInterface, void* " << placement_name << ", const void * __in_var)" << Qt::endl
              << "{" << Qt::endl
              << "    Q_UNUSED(metaTypeInterface)" << Qt::endl
              << "    Q_UNUSED(" << placement_name << ")" << Qt::endl
              << "    const " << entry->qualifiedCppName() << "* " << param_name << " = static_cast<const " << entry->qualifiedCppName() << "*>(__in_var);" << Qt::endl
              << "    Q_UNUSED(" << param_name << ")" << Qt::endl;
            writeCodeBlock(s, customCopyConstructor.code());
            s << "}" << Qt::endl << Qt::endl;
        }
        if(!customDefaultConstructor.name.isEmpty()){
            QString placement_name = customDefaultConstructor.placement_name.isEmpty() ? QLatin1String("placement") : customDefaultConstructor.placement_name;
            s << "void custom_default_constructor_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(const QtPrivate::QMetaTypeInterface *metaTypeInterface, void* " << placement_name << ")" << Qt::endl
              << "{" << Qt::endl
              << "    Q_UNUSED(metaTypeInterface)" << Qt::endl
              << "    Q_UNUSED(" << placement_name << ")" << Qt::endl;
            writeCodeBlock(s, customDefaultConstructor.code());
            s << "}" << Qt::endl << Qt::endl;
        }
        if(!customMoveConstructor.name.isEmpty()){
            QString placement_name = customMoveConstructor.placement_name.isEmpty() ? QLatin1String("placement") : customMoveConstructor.placement_name;
            QString param_name = customMoveConstructor.param_name.isEmpty() ? QLatin1String("copy") : customMoveConstructor.param_name;
            s << "void custom_move_constructor_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void* " << placement_name << ", void * __in_var)" << Qt::endl
              << "{" << Qt::endl
              << "    Q_UNUSED(metaTypeInterface)" << Qt::endl
              << "    Q_UNUSED(" << placement_name << ")" << Qt::endl
              << "    " << entry->qualifiedCppName() << "* " << param_name << " = static_cast<" << entry->qualifiedCppName() << "*>(__in_var);" << Qt::endl
              << "    Q_UNUSED(" << param_name << ")" << Qt::endl;
            writeCodeBlock(s, customMoveConstructor.code());
            s << "}" << Qt::endl;
        }
    }
    if (!customDestructor.name.isEmpty()) {
        s << "void custom_destructor_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(" << Qt::endl;
        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6, 0, 0)){
            s << "                                                   const QtPrivate::QMetaTypeInterface * metaTypeInterface, " << Qt::endl;
        }
        s << "                                                   void * __in_var)" << Qt::endl
          << "{" << Qt::endl;
        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6, 0, 0)){
            s << "    Q_UNUSED(metaTypeInterface)" << Qt::endl;
        }
        s << "    " << entry->qualifiedCppName() << "* " << customDestructor.param_name << " = static_cast<" << entry->qualifiedCppName() << "*>(__in_var);" << Qt::endl
          << "    Q_UNUSED(" << customDestructor.param_name << ")" << Qt::endl;
        writeCodeBlock(s, customDestructor.code());
        s << "}" << Qt::endl << Qt::endl;
    }
}

bool CppImplGenerator::writeConversionRule(QTextStream &s,
        TS::Language target_language,
        const MetaFunction *java_function,
        const MetaFunctional *java_functional,
        int argument_index,
        const QString &qt_name,
        const QString &java_name,
        Option option,
        const QString& __jni_env,
        const QString& qtjambi_scope) {
    Q_UNUSED(option)
    if (argument_index < 0 || (!java_function && !java_functional))
        return false;

    QString replacedType;
    const MetaType *java_type{nullptr};
    const MetaArgumentList& arguments = java_function ? java_function->arguments() : java_functional->arguments();
    if(argument_index==0){
        if(java_function){
            java_type = java_function->type();
        }else{
            java_type = java_functional->type();
        }
    }else{
        for(MetaArgument* arg : arguments){
            if(arg->argumentIndex()==argument_index-1){
                java_type = arg->type();
                break;
            }
        }
        if(!java_type)
            return false;
    }

    const MetaArgument* argumentTypeArrayLength = nullptr;
    QString argumentTypeArrayExpression;
    bool useArgumentAsArray = false;
    bool useArgumentAsBuffer = false;
    bool useArgumentAsSlotContext = false;
    bool argumentTypeString = false;
    bool argumentTypeArrayDeref = false;
    bool insertUtilArgument = false;
    int argumentTypeArrayMinLength = -1;
    int argumentTypeArrayMaxLength = -1;
    if(java_function){
        useArgumentAsBuffer = java_function->useArgumentAsBuffer(argument_index);
        useArgumentAsArray = !useArgumentAsBuffer && java_function->useArgumentAsArray(argument_index);
        useArgumentAsSlotContext = java_function->useArgumentAsSlotContext(argument_index);
        if(useArgumentAsArray || useArgumentAsBuffer){
            argumentTypeArrayDeref = java_function->useArgumentAsDerefPointer(argument_index);
            int utilArgumentIndex = java_function->utilArgumentIndex(argument_index);
            if(utilArgumentIndex>0){
                for(const MetaArgument *argument : arguments) {
                    if(argument->argumentIndex()+1==utilArgumentIndex){
                        argumentTypeArrayLength = argument;
                    }
                }
            }
            if(argumentTypeArrayLength){
                insertUtilArgument = java_function->insertUtilArgument(argument_index);
            }else{
                argumentTypeArrayMinLength = java_function->arrayOrBufferLengthMinValue(argument_index);
                argumentTypeArrayMaxLength = java_function->arrayOrBufferLengthMaxValue(argument_index);
                if(argumentTypeArrayMinLength<0){
                    argumentTypeArrayExpression = java_function->arrayOrBufferLengthExpression(argument_index);
                }
            }
        }else{
            replacedType = java_function->typeReplaced(argument_index);
        }
    }else{
        useArgumentAsBuffer = java_functional->useArgumentAsBuffer(argument_index);
        useArgumentAsArray = !useArgumentAsBuffer && java_functional->useArgumentAsArray(argument_index);
        useArgumentAsSlotContext = java_functional->useArgumentAsSlotContext(argument_index);
        if(useArgumentAsBuffer || useArgumentAsArray){
            argumentTypeArrayDeref = java_functional->useArgumentAsDerefPointer(argument_index);
            int utilArgumentIndex = java_functional->utilArgumentIndex(argument_index);
            if(utilArgumentIndex>0){
                for(const MetaArgument *argument : arguments) {
                    if(argument->argumentIndex()+1==utilArgumentIndex){
                        argumentTypeArrayLength = argument;
                    }
                }
            }
            if(argumentTypeArrayLength){
                insertUtilArgument = java_functional->insertUtilArgument(argument_index);
            }else{
                argumentTypeArrayMinLength = java_functional->arrayOrBufferLengthMinValue(argument_index);
                argumentTypeArrayMaxLength = java_functional->arrayOrBufferLengthMaxValue(argument_index);
                if(argumentTypeArrayMinLength<0){
                    argumentTypeArrayExpression = java_functional->arrayOrBufferLengthExpression(argument_index);
                }
            }
        }else{
            replacedType = java_functional->typeReplaced(argument_index);
        }
    }
    Q_UNUSED(argumentTypeArrayMaxLength)

    QString conversion_rule;
    if(!argumentTypeArrayLength && argumentTypeArrayMinLength<0){
        if(java_function){
            conversion_rule = java_function->conversionRule(target_language, argument_index);
        }else{
            conversion_rule = java_functional->conversionRule(target_language, argument_index);
        }
    }
    if(java_type
            && conversion_rule.isEmpty()
            && java_type->isConstant()
            && java_type->indirections().size()==1
            && java_type->typeEntry()->targetLangName()=="byte"
            && replacedType=="java.lang.String"){
        argumentTypeString = true;
    }

    if (!conversion_rule.isEmpty() || useArgumentAsArray || useArgumentAsBuffer || argumentTypeString || useArgumentAsSlotContext) {
        QScopedPointer<Indentation> indentationPointer;
        bool needLambda = false;
        if(!useArgumentAsBuffer && !argumentTypeString){
            if((option & DirectReturn) == DirectReturn){
                needLambda = true;
            //}else if((option & NoTmpVariable) == NoTmpVariable){
            //    needLambda = true;
            }
        }

        QString _qt_name = qt_name;
        if(((option & OptionalScope) == OptionalScope) && target_language==TS::NativeCode && argument_index>0){
            _qt_name = qt_name + "_tmp";
        }

        QString out_name = _qt_name;
        QString in_name = java_name;
        QString tmp_out_name = _qt_name;
        QString tmp_in_name = java_name;
        QString lambdaOutType;
        if ((argument_index == 0 && target_language == TS::NativeCode)
                || (argument_index != 0 && target_language == TS::ShellCode)) {
            tmp_out_name = java_name;
            tmp_in_name = _qt_name;
            out_name = java_name;
            in_name = _qt_name;
            if(replacedType.isEmpty())
                lambdaOutType = translateType(java_type, option);
            else
                lambdaOutType = jniName(replacedType);
        } else {
            tmp_out_name = _qt_name;
            tmp_in_name = java_name;
            out_name = _qt_name;
            in_name = java_name;
            if(java_type){
                QTextStream _s(&lambdaOutType);
                writeTypeInfo(_s, java_type, option);
            }
        }
        if(!useArgumentAsBuffer && !argumentTypeString){
            if(out_name.isEmpty()){
               needLambda = true;
            }
        }

        if(needLambda && java_type){
            tmp_out_name = "_lambda_out";
        }

        QString convertCode;
        {
            INDENTATIONRESET(INDENT)
            QTextStream s(&convertCode);
            if(useArgumentAsArray && java_type){
                if(!argumentTypeArrayLength && argumentTypeArrayMinLength<0 && argumentTypeArrayExpression.isEmpty())
                    ReportHandler::warning(QString("ArrayLength expected at argument %1 of %2").arg(QString::number(argument_index), java_function ? (java_function->declaringClass() ? java_function->declaringClass()->qualifiedCppName()+"::" : QString()) + java_function->minimalSignature() : java_functional->name()));
                QScopedPointer<MetaType> _java_type(java_type->copy());
                _java_type->setConstant(false);
                _java_type->setReferenceType(MetaType::NoReference);
                //if(argumentTypeArrayDeref)
                {
                    QList<bool> indirections = _java_type->indirections();
                    if(!indirections.isEmpty()){
                        indirections.takeLast();
                        _java_type->setIndirections(indirections);
                    }
                }
                MetaBuilder::decideUsagePattern(_java_type.get());
                if((argument_index==0 && target_language==TS::ShellCode)
                    || (argument_index>0 && target_language==TS::NativeCode)){
                    if(_java_type->typeEntry()->isPrimitive() || _java_type->isQChar()){
                        QString arrayPointerType = _java_type->typeEntry()->targetLangName();
                        arrayPointerType[0] = arrayPointerType[0].toUpper();
                        if(insertUtilArgument || qtjambi_scope.isEmpty()){
                            s << INDENT << "J";
                            if(java_type->isConstant()){
                                s << "Const";
                            }
                            s << arrayPointerType << "ArrayPointer " << out_name << "_buffer(" << __jni_env << ", " << in_name << ");" << Qt::endl;
                            if(insertUtilArgument){
                                s << INDENT;
                                if(target_language==TS::NativeCode){
                                    writeTypeInfo(s, java_type, option);
                                }
                                s << qt_name << " = " << out_name << "_buffer;" << Qt::endl
                                  << INDENT << "if(" << qt_name << "){" << Qt::endl;
                                if(argumentTypeArrayLength){
                                    s << INDENT << "    if(" << argumentTypeArrayLength->indexedName() << "<0)" << Qt::endl
                                      << INDENT << "        " << argumentTypeArrayLength->indexedName() << " = decltype(" << argumentTypeArrayLength->indexedName() << ")(" << out_name << "_buffer.size());" << Qt::endl;
                                }
                                s << INDENT << "    if(__qt_offset_of_" << in_name << ">0)" << Qt::endl
                                  << INDENT << "        " << qt_name << " += __qt_offset_of_" << in_name << ";" << Qt::endl
                                  << INDENT << "    if(" << out_name << "_buffer.size() - __qt_offset_of_" << in_name << " < ";
                                if(argumentTypeArrayLength){
                                    s << argumentTypeArrayLength->indexedName();
                                }else if(argumentTypeArrayMinLength>0){
                                    s << QString::number(argumentTypeArrayMinLength);
                                }else if(!argumentTypeArrayExpression.isEmpty()){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                    for(const MetaArgument* arg : arguments){
                                        argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                    }
                                    s << argumentTypeArrayExpression;
                                }else{
                                    s << "INT_MAX";
                                }
                                s << ")" << Qt::endl
                                  << INDENT << "        JavaException::raiseIndexOutOfBoundsException(" << __jni_env << ", \"Given offset and length exceed actual array length.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl
                                  << INDENT << "}else{" << Qt::endl
                                  << INDENT << "    " << argumentTypeArrayLength->indexedName() << " = 0;" << Qt::endl
                                  << INDENT << "}" << Qt::endl;
                            }
                        }else{
                            if((option & OptionalScope) == OptionalScope && qt_name!=out_name)
                                s << INDENT << "if(!" << qtjambi_scope << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                            if(argumentTypeArrayLength
                                && (
                                    (java_function && java_function->argumentRemoved(argumentTypeArrayLength->argumentIndex()+1)==ArgumentRemove_Remove)
                                    || (java_functional && java_functional->argumentRemoved(argumentTypeArrayLength->argumentIndex()+1)==ArgumentRemove_Remove)
                                    )){
                                s << INDENT;
                                writeTypeInfo(s, argumentTypeArrayLength->type(), option);
                                s << argumentTypeArrayLength->indexedName() << "{0};" << Qt::endl;
                            }
                            s << INDENT;
                            if(qt_name==out_name){
                                if(target_language==TS::NativeCode)
                                    writeTypeInfo(s, java_type, option);
                                s << out_name;
                            }else{
                                if((option & OptionalScope) == OptionalScope){
                                    s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                                    s << INDENT << "    void** _" << qt_name << " = new void*;" << Qt::endl;
                                    s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                                    s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                                    s << INDENT << "}" << Qt::endl;
                                    s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                                    s << INDENT << "*reinterpret_cast<void**>(" << qt_name << ")";
                                }else
                                    s << qt_name;
                            }
                            s << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", ";
                            if((option & OptionalScope) == OptionalScope)
                                s << "*";
                            s << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength){
                                s << argumentTypeArrayLength->indexedName();
                            }else if(argumentTypeArrayMinLength>0){
                                s << QString::number(argumentTypeArrayMinLength);
                            }else if(!argumentTypeArrayExpression.isEmpty()){
                                argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                for(const MetaArgument* arg : arguments){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                }
                                s << argumentTypeArrayExpression;
                            }else{
                                s << "INT_MAX";
                            }
                            s << ");" << Qt::endl;
                        }
                    }else{
                        if(insertUtilArgument || qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "JConstObjectArrayPointer<";
                            }else{
                                s << INDENT << "JObjectArrayPointer<";
                            }
                            writeTypeInfo(s, _java_type.get(), option);
                            s << "> " << out_name <<  "_buffer(" << __jni_env << ", " << in_name << "," << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                {
                                    INDENTATION(INDENT)
                                    {
                                        INDENTATION(INDENT)
                                        QString text;
                                        {
                                            QTextStream s(&text);
                                            INDENTATION(INDENT)
                                            writeJavaToQt(s, _java_type.get(), "p", "o", nullptr, nullptr, 0, Option((option | NoTmpVariable) & ~UseNativeIds), "env", qtjambi_scope);
                                        }
                                        s << INDENT << "[";
                                        if(!qtjambi_scope.isEmpty() && text.contains(qtjambi_scope))
                                            s << qtjambi_scope;
                                        s << "](";
                                        writeTypeInfo(s, _java_type.get(), option);
                                        s << "& p,JNIEnv * env, jobject o){" << Qt::endl
                                          << text
                                          << INDENT << "}";
                                    }
                                }
                            }
                            if(!java_type->isConstant()){
                                s << "," << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    {
                                        INDENTATION(INDENT)
                                        {
                                            INDENTATION(INDENT)
                                            QString text;
                                            {
                                                QTextStream s(&text);
                                                INDENTATION(INDENT)
                                                writeQtToJava(s, _java_type.get(), "p", "o", nullptr, nullptr, 0, Option(option | DirectReturn), "env", qtjambi_scope);
                                            }
                                            s << INDENT << "[";
                                            if(!qtjambi_scope.isEmpty() && text.contains(qtjambi_scope))
                                                s << qtjambi_scope;
                                            s << "](JNIEnv * env, ";
                                            writeTypeInfo(s, _java_type.get(), Option(option | ExcludeConst));
                                            s << " const& p) -> jobject {" << Qt::endl;
                                            s << INDENT << "}";
                                        }
                                    }
                                }
                            }
                            s << ");" << Qt::endl;
                            if(insertUtilArgument){
                                s << INDENT;
                                if(target_language==TS::NativeCode){
                                    writeTypeInfo(s, java_type, option);
                                }
                                s << qt_name << " = " << out_name << "_buffer;" << Qt::endl
                                  << INDENT << "if(" << qt_name << "){" << Qt::endl;
                                if(argumentTypeArrayLength){
                                    s << INDENT << "    if(" << argumentTypeArrayLength->indexedName() << "<0)" << Qt::endl
                                      << INDENT << "        " << argumentTypeArrayLength->indexedName() << " = decltype(" << argumentTypeArrayLength->indexedName() << ")(" << out_name << "_buffer.size());" << Qt::endl;
                                }
                                s << INDENT << "    if(__qt_offset_of_" << in_name << ">0)" << Qt::endl
                                  << INDENT << "        " << qt_name << " += __qt_offset_of_" << in_name << ";" << Qt::endl
                                  << INDENT << "    if(" << out_name << "_buffer.size() - __qt_offset_of_" << in_name << " < ";
                                if(argumentTypeArrayLength){
                                    s << argumentTypeArrayLength->indexedName();
                                }else if(argumentTypeArrayMinLength>0){
                                    s << QString::number(argumentTypeArrayMinLength);
                                }else if(!argumentTypeArrayExpression.isEmpty()){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                    for(const MetaArgument* arg : arguments){
                                        argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                    }
                                    s << argumentTypeArrayExpression;
                                }else{
                                    s << "INT_MAX";
                                }
                                s << ")" << Qt::endl
                                  << INDENT << "        JavaException::raiseIndexOutOfBoundsException(" << __jni_env << ", \"Given offset and length exceed actual array length.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl
                                  << INDENT << "}else{" << Qt::endl
                                  << INDENT << "    " << argumentTypeArrayLength->indexedName() << " = 0;" << Qt::endl
                                  << INDENT << "}" << Qt::endl;
                            }
                        }else{
                            if((option & OptionalScope) == OptionalScope && qt_name!=out_name)
                                s << INDENT << "if(!" << qtjambi_scope << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                            if(argumentTypeArrayLength
                                && (
                                    (java_function && java_function->argumentRemoved(argumentTypeArrayLength->argumentIndex()+1)==ArgumentRemove_Remove)
                                    || (java_functional && java_functional->argumentRemoved(argumentTypeArrayLength->argumentIndex()+1)==ArgumentRemove_Remove)
                                    )){
                                s << INDENT;
                                writeTypeInfo(s, argumentTypeArrayLength->type(), option);
                                s << argumentTypeArrayLength->indexedName() << "{0};" << Qt::endl;
                            }
                            s << INDENT;
                            if(qt_name==out_name){
                                if(target_language==TS::NativeCode)
                                    writeTypeInfo(s, java_type, option);
                                s << out_name;
                            }else{
                                if((option & OptionalScope) == OptionalScope){
                                    s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                                    s << INDENT << "    void** _" << qt_name << " = new void*;" << Qt::endl;
                                    s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                                    s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                                    s << INDENT << "}" << Qt::endl;
                                    s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                                    s << INDENT << "*reinterpret_cast<void**>(" << qt_name << ")";
                                }else
                                    s << qt_name;
                            }
                            s << " = ";
                            if(java_type->instantiations().isEmpty() && java_type->getReferenceType()==MetaType::Reference && !java_type->isConstant()){
                                s << "*qtjambi_array_cast<";
                                writeTypeInfo(s, java_type, Option(option | ExcludeReference | SkipName));
                                s << "*>(" << __jni_env << ", ";
                            }else{
                                s << "qtjambi_array_cast<";
                                writeTypeInfo(s, java_type, Option(option | SkipName));
                                s << ">(" << __jni_env << ", ";
                            }
                            if((option & OptionalScope) == OptionalScope)
                                    s << "*";
                            s << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength){
                                s << argumentTypeArrayLength->indexedName();
                            }else if(argumentTypeArrayMinLength>0){
                                    s << QString::number(argumentTypeArrayMinLength);
                            }else if(!argumentTypeArrayExpression.isEmpty()){
                                argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                for(const MetaArgument* arg : arguments){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                }
                                s << argumentTypeArrayExpression;
                            }else{
                                s << "INT_MAX";
                            }
                            s << ");" << Qt::endl;
                        }
                    }
                    if(qtjambi_scope.isEmpty() && argumentTypeArrayLength){
                            s << INDENT << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName() << " = " << out_name << ".size();" << Qt::endl;
                    }
                }else{
                    if(_java_type->typeEntry()->isPrimitive() || _java_type->isQChar()){
                        QString pointerConverterType;
                        if(_java_type->typeEntry()->targetLangName()=="int"){
                            pointerConverterType = "Int32";
                        }else if(_java_type->typeEntry()->targetLangName()=="short"){
                            pointerConverterType = "Int16";
                        }else if(_java_type->typeEntry()->targetLangName()=="byte"){
                            if(_java_type->typeEntry()->qualifiedCppName()=="char")
                                pointerConverterType = "Char";
                            else
                                pointerConverterType = "Int8";
                        }else if(_java_type->typeEntry()->targetLangName()=="long"){
                            pointerConverterType = "Int64";
                        }else if(_java_type->typeEntry()->targetLangName()=="double"){
                            pointerConverterType = "Double";
                        }else if(_java_type->typeEntry()->targetLangName()=="float"){
                            pointerConverterType = "Float";
                        }else if(_java_type->typeEntry()->targetLangName()=="boolean"){
                            pointerConverterType = "Bool";
                        }else if(_java_type->typeEntry()->targetLangName()=="char"){
                            pointerConverterType = "WChar";
                        }
                        if(insertUtilArgument || qtjambi_scope.isEmpty()){
                            s << INDENT;
                            if(java_type->isConstant())
                                s << "Const";
                            s << INDENT << pointerConverterType << "PointerArray " << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength){
                                s << (argumentTypeArrayDeref ? "jsize(*" : "jsize(") << argumentTypeArrayLength->indexedName() << ")";
                            }else if(argumentTypeArrayMinLength>0){
                                s << QString::number(argumentTypeArrayMinLength);
                            }else if(!argumentTypeArrayExpression.isEmpty()){
                                argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                for(const MetaArgument* arg : arguments){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                }
                                s << argumentTypeArrayExpression;
                            }else{
                                s << "1024";
                            }
                            s << ");" << Qt::endl;
                            s << INDENT;
                            if((option & NoTmpVariable) != NoTmpVariable)
                                s << _java_type->typeEntry()->jniName() << "Array ";
                            s << tmp_out_name << " = " << out_name << "_array;" << Qt::endl;
                            if(insertUtilArgument){
                                QString typeReplaced = java_function ? java_function->typeReplaced(argumentTypeArrayLength->argumentIndex() + 1)
                                                                     : java_functional->typeReplaced(argumentTypeArrayLength->argumentIndex() + 1);
                                if(!typeReplaced.isEmpty())
                                    s << INDENT << jniName(annotationFreeTypeName(typeReplaced));
                                else
                                    s << INDENT << translateType(argumentTypeArrayLength->type(), NoOption);
                                s << " __qt_offset_of_" << in_name << "{0};" << Qt::endl;
                            }
                        }else{
                            //if((option & OptionalScope) == OptionalScope)
                            //    s << INDENT << "if(!" << qtjambi_scope << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                            s << INDENT;
                            if((option & NoTmpVariable) != NoTmpVariable)
                                s << _java_type->typeEntry()->jniName() << "Array ";
                            s  << tmp_out_name << " = qtjambi_array_cast<" << _java_type->typeEntry()->jniName() << "Array>(" << __jni_env << ", ";
                            if(!_java_type->isConstant()){
                                if((option & OptionalScope) == OptionalScope)
                                    s << "*";
                                s << qtjambi_scope << ", ";
                            }
                            s << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength){
                                s << argumentTypeArrayLength->indexedName();
                            }else if(argumentTypeArrayMinLength>0){
                                s << QString::number(argumentTypeArrayMinLength);
                            }else if(!argumentTypeArrayExpression.isEmpty()){
                                argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                for(const MetaArgument* arg : arguments){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                }
                                s << argumentTypeArrayExpression;
                            }else{
                                s << "1024";
                            }
                            s << ");" << Qt::endl;
                        }
                        lambdaOutType = _java_type->typeEntry()->jniName() + QLatin1String("Array");
                    }else{
                        if(insertUtilArgument || qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstObjectPointerArray<";
                            }else{
                                s << INDENT << "ObjectPointerArray<";
                            }
                            writeTypeInfo(s, _java_type.get(), Option(SkipName | option));
                            s << ">" << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength){
                                s << (argumentTypeArrayDeref ? "jsize(*" : "jsize(") << argumentTypeArrayLength->indexedName() << ")";
                            }else if(argumentTypeArrayMinLength>0){
                                s << QString::number(argumentTypeArrayMinLength);
                            }else if(!argumentTypeArrayExpression.isEmpty()){
                                argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                for(const MetaArgument* arg : arguments){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                }
                                s << argumentTypeArrayExpression;
                            }else{
                                s << "1024";
                            }
                            s << ", " << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                {
                                    INDENTATION(INDENT)
                                    {
                                        INDENTATION(INDENT)
                                        s << INDENT << "[](JNIEnv * env, ";
                                        writeTypeInfo(s, _java_type.get(), Option(ForceConstReference | option));
                                        s << "in) -> jobject {" << Qt::endl;
                                        {
                                            INDENTATION(INDENT)
                                            writeQtToJava(s, _java_type.get(), "in", "out", nullptr, nullptr, 0, Option(option | DirectReturn), "env", "");
                                        }
                                        s << INDENT << "}";
                                    }
                                }
                            }
                            if(!java_type->isConstant()){
                                s << "," << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    {
                                        INDENTATION(INDENT)
                                        {
                                            INDENTATION(INDENT)
                                            s << INDENT << "[](";
                                            writeTypeInfo(s, _java_type.get(), option);
                                            s << "& p,JNIEnv * env, jobject o){" << Qt::endl;
                                            {
                                                INDENTATION(INDENT)
                                                writeJavaToQt(s, _java_type.get(), "p", "o", nullptr, nullptr, 0, Option((option | NoTmpVariable) & ~UseNativeIds), "env", "");
                                            }
                                            s << INDENT << "}";
                                        }
                                    }
                                }
                            }
                            s << ");" << Qt::endl;
                            s << INDENT << "jobjectArray " << tmp_out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            //if((option & OptionalScope) == OptionalScope)
                            //    s << INDENT << "if(!" << qtjambi_scope << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                            s << INDENT;
                            if((option & NoTmpVariable) != NoTmpVariable)
                                s << "jobjectArray ";
                            s  << tmp_out_name << " = qtjambi_array_cast<jobjectArray>(" << __jni_env << ", ";
                            if((option & OptionalScope) == OptionalScope)
                                    s << "*";
                            s << qtjambi_scope << ", ";
                            if(java_type->instantiations().isEmpty() && java_type->getReferenceType()==MetaType::Reference && !java_type->isConstant())
                                s << "&";
                            s << in_name << ", ";
                            if(argumentTypeArrayLength){
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            }else if(argumentTypeArrayMinLength>0){
                                s << QString::number(argumentTypeArrayMinLength);
                            }else if(!argumentTypeArrayExpression.isEmpty()){
                                argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                                for(const MetaArgument* arg : arguments){
                                    argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                                }
                                s << argumentTypeArrayExpression;
                            }else{
                                s << "1024";
                            }
                            s << ");" << Qt::endl;
                        }
                        lambdaOutType = QLatin1String("jobjectArray");
                    }
                }
            }else if(argumentTypeString){
                if((argument_index==0 && target_language==TS::ShellCode)
                    || (argument_index>0 && target_language==TS::NativeCode)){
                    if(qtjambi_scope.isEmpty() || !(option & OptionalScope)){
                        s << INDENT << "J2CStringBuffer " << out_name << "_buffer(" << __jni_env << ", jstring(" << in_name << "));" << Qt::endl
                          << INDENT;
                        if(qt_name==out_name && (target_language==TS::NativeCode)){
                            if(java_type && java_type->isConstant())
                                s << "const ";
                            s << "char* " << out_name;
                        }else
                            s << qt_name;
                        s << " = " << out_name << "_buffer;" << Qt::endl;
                    }else{
                        s << INDENT << "if(!" << qtjambi_scope << ")" << Qt::endl
                          << INDENT << "    return false;" << Qt::endl
                          << INDENT << "PersistentJ2CStringBuffer* " << out_name << "_buffer = new PersistentJ2CStringBuffer(" << __jni_env << ", jstring(" << in_name << "));" << Qt::endl
                          << INDENT << qtjambi_scope << "->addFinally([" << out_name << "_buffer](){ delete " << out_name << "_buffer; });" << Qt::endl
                          << INDENT;
                        if(qt_name==out_name){
                            if(java_type && java_type->isConstant())
                                s << "const ";
                            s << "char* " << out_name << " = *" << out_name << "_buffer;" << Qt::endl;
                        }else{
                            s << INDENT << "if(!" << qt_name << "){" << Qt::endl
                              << INDENT << "    void** _" << qt_name << " = new void*;" << Qt::endl
                              << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl
                              << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl
                              << INDENT << "}" << Qt::endl
                              << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl
                              << INDENT << "*reinterpret_cast<void**>(" << qt_name << ") = " << out_name << "_buffer->data();" << Qt::endl;
                        }
                    }
                }else{
                    if((option & DirectReturn) == DirectReturn){
                        s << INDENT << "return ";
                    }else if((option & NoTmpVariable) == NoTmpVariable){
                        s << INDENT << out_name << " = ";
                    }else{
                        s << INDENT << "jstring " << out_name << " = ";
                    }
                    if(java_type->typeEntry()->name()=="char")
                        s << "qtjambi_cast<jstring>(" << __jni_env << ", " << in_name << ")";
                    else
                        s << "qtjambi_cast<jstring>(" << __jni_env << ", reinterpret_cast<const char*>(" << in_name << "))";
                    if((option & DirectReturn) == 0)
                        s  << ";" << Qt::endl;
                }
            }else if(useArgumentAsBuffer){
                //Q_ASSERT(java_type->typeEntry()->isPrimitive());
                //Q_ASSERT(java_type->indirections().size()==1);
                //Q_ASSERT(java_type->typeEntry()->targetLangName()!="boolean");
                std::unique_ptr<MetaType> typeCopy{java_type->copy()};
                {
                    QList<bool> indirections = java_type->indirections();
                    indirections.removeLast();
//                    if(indirections.isEmpty() && typeCopy->typeEntry()->targetLangName()=="void"){
//                        typeCopy.reset();
//                    }else
                    {
                        typeCopy->setIndirections(indirections);
                        typeCopy->setConstant(false);
                    }
                }
                if((argument_index==0 && target_language==TS::ShellCode)
                    || (argument_index>0 && target_language==TS::NativeCode)){
                    if(qtjambi_scope.isEmpty() || !(option & OptionalScope)){
                        s << INDENT << "JBuffer";
                        if(java_type->isConstant())
                            s << "Const";
                        s << "Data " << out_name << "_buffer(" << __jni_env << ", " << in_name << ");" << Qt::endl
                          << INDENT;
                        if(qt_name==out_name && (target_language==TS::NativeCode)){
                            writeTypeInfo(s, java_type, option);
                            s << out_name;
                        }else
                            s << qt_name;
                        s << " = " << out_name << "_buffer.data<";
                        writeTypeInfo(s, typeCopy.get(), Option(option | SkipName));
                        s << ">();" << Qt::endl;
                        if(argumentTypeArrayLength){
                            if(typeCopy->typeEntry()->isVoid()){
                                s << INDENT << argumentTypeArrayLength->indexedName() << " = " << out_name << "_buffer.size();" << Qt::endl;
                            }else{
                                s << INDENT << argumentTypeArrayLength->indexedName() << " = " << out_name << "_buffer.size<";
                                writeTypeInfo(s, typeCopy.get(), Option(option | SkipName));
                                s << ">();" << Qt::endl;
                            }
                        }
                        if(argument_index==0){
                            s << INDENT << out_name << "_buffer.take();" << Qt::endl;
                        }
                    }else{
                        s << INDENT << "if(!" << qtjambi_scope << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl
                          << INDENT << "JBuffer";
                        if(java_type->isConstant())
                            s << "Const";
                        s << "Data* " << out_name << "_buffer = new JBuffer";
                        if(java_type->isConstant())
                            s << "Const";
                        s << "Data(" << __jni_env << ", " << in_name << ");" << Qt::endl
                          << INDENT << qtjambi_scope << "->addFinally([" << out_name << "_buffer](){ delete " << out_name << "_buffer; });" << Qt::endl
                          << INDENT;
                        if(qt_name==out_name){
                            if(target_language==TS::NativeCode)
                                writeTypeInfo(s, java_type, option);
                            s << out_name << " = " << out_name << "_buffer->data<";
                            writeTypeInfo(s, typeCopy.get(), option);
                            s << ">();" << Qt::endl;
                        }else{
                            s << INDENT << "if(!" << qt_name << "){" << Qt::endl
                              << INDENT << "    void** _" << qt_name << " = new void*;" << Qt::endl
                              << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl
                              << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl
                              << INDENT << "}" << Qt::endl
                              << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl
                              << INDENT << "*reinterpret_cast<void**>(" << qt_name << ") = " << out_name << "_buffer->data<";
                            writeTypeInfo(s, typeCopy.get(), option);
                            s << ">();" << Qt::endl;
                        }
                        if(argumentTypeArrayLength)
                            s << INDENT << "" << argumentTypeArrayLength->indexedName() << " = " << out_name << "_buffer->size();" << Qt::endl;
                    }
                }else{
                    if(out_name.isEmpty()){
                        out_name = java_name;
                    }
                    s << INDENT << "DataJBuffer " << out_name << "_buffer(" << __jni_env << ", " << in_name << ", ";
                    if(argumentTypeArrayLength){
                        s << argumentTypeArrayLength->indexedName();
                    }else if(argumentTypeArrayMinLength>0){
                        s << QString::number(argumentTypeArrayMinLength);
                    }else if(!argumentTypeArrayExpression.isEmpty()){
                        argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%env", __jni_env).replace("%this", "__qt_this").replace("%scope", qtjambi_scope);
                        for(const MetaArgument* arg : arguments){
                            argumentTypeArrayExpression = argumentTypeArrayExpression.replace("%"+QString::number(arg->argumentIndex()+1), arg->indexedName());
                        }
                        s << argumentTypeArrayExpression;
                    }else{
                        s << "INT_MAX";
                    }
                    s << ");" << Qt::endl
                      << INDENT;
                    if((option & DirectReturn) == DirectReturn){
                        s << INDENT << "return ";
                    }else if((option & NoTmpVariable) == NoTmpVariable){
                        s << INDENT << out_name << " = ";
                    }else{
                        s << INDENT << "jobject " << out_name << " = ";
                    }
                    s << out_name << "_buffer." << (argument_index==0 ? "take" : "buffer") << "();" << Qt::endl;
                }
            }else if(useArgumentAsSlotContext){
                s << "QObject* " << out_name << " = QtJambiAPI::objectFromNativeId<QObject>(" << in_name << ");" << Qt::endl;
            }else{
                conversion_rule  = conversion_rule
                                    .replace("%out", tmp_out_name)
                                    .replace("%in", tmp_in_name)
                                    .replace("%env", __jni_env)
                                    .replace("%scope", (option & OptionalScope) == OptionalScope ? "*"+qtjambi_scope : qtjambi_scope);

                if(target_language == TS::ShellCode)
                    conversion_rule  = conversion_rule.replace("%this", "this");
                else
                    conversion_rule  = conversion_rule.replace("%this", "__qt_this");

                if(java_function && java_function->implementingClass()){
                    conversion_rule  = conversion_rule.replace("%class", java_function->implementingClass()->qualifiedCppName());
                    conversion_rule = conversion_rule.replace("%shellclass", shellClassName(java_function->implementingClass()));
                    conversion_rule = conversion_rule.replace("%oshellclass", oshellClassName(java_function->implementingClass()));
                    conversion_rule = conversion_rule.replace("%mshellclass", mshellClassName(java_function->implementingClass()));

                }

                for (int i = 0; i < arguments.size(); ++i) {
                    conversion_rule = conversion_rule.replace("%" + QString::number(i + 1),
                                      arguments.at(i)->indexedName());
                }
                conversion_rule  = conversion_rule.replace("%\"\"", "%");
                QStringList lines = conversion_rule.split(QStringLiteral(u"\n"));
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
                if(!lines.isEmpty() && !needLambda && !java_type)
                    s << Qt::endl;
                for(QString line : lines) {
                    if(sp<0 && line.trimmed().isEmpty()){
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
                    if(!line.startsWith(QLatin1Char('#')))
                        s << INDENT;
                    s << line << Qt::endl;
                }
                if(((option & OptionalScope) == OptionalScope) && target_language==TS::NativeCode && argument_index>0){
                    const ComplexTypeEntry *complexType = dynamic_cast<const ImplementorTypeEntry *>(java_type->typeEntry());
                    if(java_type->indirections().isEmpty()
                            && (java_type->getReferenceType()==MetaType::Reference
                                || (complexType && (complexType->isObject() || complexType->isInterface()) && !complexType->hasPublicDefaultAssignment())
                                )){
                        if(argument_index==0){
                            s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                if(java_type->isConstant()){
                                    s << INDENT << "*reinterpret_cast<";
                                    writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                    s << "*(" << qt_name << ") = const_cast<";
                                    writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                    s << "*>(&" << _qt_name << ");" << Qt::endl;
                                }else{
                                    s << INDENT << "*reinterpret_cast<";
                                    writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                    s << "*(" << qt_name << ") = &" << _qt_name << ";" << Qt::endl;
                                }
                            }
                            s << INDENT << "}else{" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                if(java_type->isConstant()){
                                    s << INDENT << qt_name << " = const_cast<";
                                    writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                    s << "*>(&" << _qt_name << ");" << Qt::endl;
                                }else{
                                    s << INDENT << qt_name << " = &" << _qt_name << ";" << Qt::endl;
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else{
                            if(java_type->isConstant()){
                                s << INDENT << qt_name << " = const_cast<";
                                writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                s << "*>(&" << _qt_name << ");" << Qt::endl;
                            }else{
                                s << INDENT << qt_name << " = &" << _qt_name << ";" << Qt::endl;
                            }
                        }
                    }else{
                        s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                        s << INDENT << "    ";
                        if(java_type->indirections().isEmpty()){
                            writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
                            s << "* _" << qt_name << " = new ";
                            writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
                            s << ";" << Qt::endl;
                        }else{
                            s << "void** _" << qt_name << " = new void*;" << Qt::endl;
                        }
                        s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                        s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                        s << INDENT << "}" << Qt::endl;
                        s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                        s << INDENT << "*reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
                        s << "*>(" << qt_name << ") = " << _qt_name << ";" << Qt::endl;
                    }
                }
            }
        }

        QStringList lines = convertCode.split(QStringLiteral(u"\n"));
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
        if(needLambda && java_type){
            QString expectedBegin = QString("%1 _lambda_out = ").arg(lambdaOutType);
            convertCode = convertCode.trimmed();
            bool isOneExpression = true;
            if(convertCode.startsWith(expectedBegin)
                    && convertCode.endsWith(";")){
                QString code = convertCode.chopped(1).mid(expectedBegin.length());
                while(!code.isEmpty()){
                    auto idx1 = code.indexOf("[](");
                    if(idx1>=0){
                        auto idx2 = code.indexOf("{", idx1);
                        if(idx2>=0){
                            QString prefix = code.mid(0, idx1).trimmed();
                            if(prefix.contains(";")){
                                isOneExpression = false;
                                break;
                            }
                            qWarning("%s", qPrintable(prefix));
                            code = code.mid(idx2+1).trimmed();
                            int openCounter = 0;
                            auto idxReopen = code.indexOf("{");
                            auto idxClose = code.indexOf("}");
                            if(idxClose>=0){
                                if(idxReopen>=0 && idxReopen<idxClose){
                                    while(idxReopen>=0 || idxClose>=0){
                                        if(idxReopen>=0 && idxReopen<idxClose){
                                            ++openCounter;
                                            code = code.mid(idxReopen+1).trimmed();
                                        }else{
                                            --openCounter;
                                            code = code.mid(idxClose+1).trimmed();
                                        }
                                        idxReopen = code.indexOf("{");
                                        idxClose = code.indexOf("}");
                                        if(openCounter==0)
                                            break;
                                    }
                                    if(idxClose>=0){
                                        code = code.mid(idxClose+1).trimmed();
                                    }else{
                                        isOneExpression = false;
                                        break;
                                    }
                                }else{
                                    code = code.mid(idxClose+1).trimmed();
                                }
                            }else{
                                isOneExpression = false;
                                break;
                            }
                        }
                    }else{
                        isOneExpression = !code.contains(";");
                        break;
                    }
                }
            }
            if(convertCode.startsWith(expectedBegin)
                    && convertCode.endsWith(";")
                    && isOneExpression){
                convertCode = convertCode.mid(expectedBegin.length());
                if((option & DirectReturn) == DirectReturn){
                    convertCode.chop(1);
                    s << INDENT << "return ";
                }
                if((option & NoTmpVariable) == NoTmpVariable && !java_name.isEmpty()){
                    convertCode.chop(1);
                    s << INDENT << out_name << " = ";
                }
                s << convertCode;
            }else{
                if((option & DirectReturn) == DirectReturn){
                    s << INDENT << "return ";
                }
                if((option & NoTmpVariable) == NoTmpVariable && !java_name.isEmpty()){
                    s << INDENT << out_name << " = ";
                }
                s << "[&]()";
                if(!lambdaOutType.isEmpty())
                    s << " -> " << lambdaOutType;
                s << " {" << Qt::endl;
                indentationPointer.reset(new Indentation(INDENT));

                for(const QString& line : lines){
                    if(!line.startsWith(QLatin1Char('#')))
                        s << INDENT;
                    s << line << Qt::endl;
                }

                s << INDENT << "return " << tmp_out_name << ";" << Qt::endl;
                indentationPointer.reset();
                s << INDENT << "}()";
            }
            if((option & NoTmpVariable) == NoTmpVariable && !java_name.isEmpty()){
                s << ";" << Qt::endl;
            }
        }else{
            bool first = true;
            for(const QString& line : lines){
                if(!first){
                    s << Qt::endl;
                }
                first = false;
                if(!line.startsWith(QLatin1Char('#')))
                    s << INDENT;
                s << line;
            }
            if((option & DirectReturn) == 0){
                s << Qt::endl;
            }
        }

        return true;
    } else {
        return false;
    }
}

bool CppImplGenerator::writeJavaToQt(QTextStream &s,
                                     const MetaType *java_type,
                                     const QString &qt_name,
                                     const QString &java_name,
                                     const MetaFunction *java_function,
                                     const MetaFunctional *java_functional,
                                     int argument_index,
                                     Option option,
                                     const QString& __jni_env,
                                     const QString& qtjambi_scope) {
    // Conversion to C++: Shell code for return values, native code for arguments
    TS::Language lang = argument_index == 0 ? TS::ShellCode : TS::NativeCode;
    if(writeConversionRule(s, lang, java_function, java_functional, argument_index, qt_name, java_name, option, __jni_env, qtjambi_scope))
        return true;

    if (!java_type) {
        QString functionName;
        if(java_function){
            functionName = java_function->implementingClass()->qualifiedCppName()+"::"+java_function->name();
        }else if(java_functional){
            if(java_functional->enclosingClass())
                functionName = java_functional->enclosingClass()->name();
            functionName += java_functional->name();
        }else{
            functionName = "unknown";
        }
        QString warn = QString("no conversion possible for argument '%1' in function '%2' for "
                               "language '%3'")
                       .arg(argument_index)
                       .arg(functionName)
                       .arg(int(lang));
        ReportHandler::warning(warn);
        return true;
    }

    bool invalidate = false;
    bool parenRequired = false;
    bool semiRequired = (option & NoTmpVariable) != NoTmpVariable || !java_name.isEmpty();

    if (java_type->isJObjectWrapper()) {
        QString wrapperType;
        if (java_type->typeEntry()->isJMapWrapper()) {
            wrapperType = "JMapWrapper";
        } else if (java_type->typeEntry()->isJCollectionWrapper()) {
            wrapperType = "JCollectionWrapper";
        } else if (java_type->typeEntry()->isJIteratorWrapper()) {
            wrapperType = "JIteratorWrapper";
        } else if (java_type->typeEntry()->isJEnumWrapper()) {
            wrapperType = "JEnumWrapper";
        } else if (java_type->typeEntry()->isJQFlagsWrapper()) {
            wrapperType = "JQFlagsWrapper";
        }else{
            wrapperType = "JObjectWrapper";
        }
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    " << wrapperType << "* _" << qt_name << " = new " << wrapperType << "();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<" << wrapperType << "*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, SkipName); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<" << wrapperType << ">(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isQVariant()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QVariant* _" << qt_name << " = new QVariant();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QVariant*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, java_type->indirections().isEmpty() && (java_type->isConstant() || java_type->getReferenceType()!=MetaType::Reference) ? Option(ForceValueType | SkipName) : SkipName);
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<";
        writeTypeInfo(s, java_type, java_type->indirections().isEmpty() && (java_type->isConstant() || java_type->getReferenceType()!=MetaType::Reference) ? Option(ForceValueType | SkipName) : SkipName);
        s << ">(" << __jni_env << ", ";
        if((java_type->isConstant() && !java_type->indirections().isEmpty())
                || (!java_type->isConstant() && java_type->actualIndirections()>0))
            s << qtjambi_scope << ", ";
        s << java_name << ")";
    } else if (java_type->isArray() && java_type->arrayElementCounts().size()>0) {
        MetaType *elementType = java_type->arrayElementType();
        Q_ASSERT(elementType);
        QString argumentName = java_name;
        if(argument_index>0){
            if(java_function){
                for(MetaArgument* arg : java_function->arguments()){
                    if(arg->argumentIndex()==argument_index-1){
                        argumentName = arg->argumentName();
                        break;
                    }
                }
            }else if(java_functional){
                for(MetaArgument* arg : java_functional->arguments()){
                    if(arg->argumentIndex()==argument_index-1){
                        argumentName = arg->argumentName();
                        break;
                    }
                }
            }
        }
        if(java_type->getReferenceType()==MetaType::Reference && java_type->arrayElementCounts().size()==1){
            if(elementType->isNativePointer()){
                s << INDENT << "int " << java_name << "_array_size{0};" << Qt::endl
                  << INDENT;
                writeTypeInfo(s, elementType);
                for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                    s << "*";
                }
                s << " " << java_name << "_array = reinterpret_cast<";
                writeTypeInfo(s, elementType, Option(option | SkipName));
                for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                    s << "*";
                }
                s << ">(QtJambiAPI::convertQNativePointerToNative(" << __jni_env << ", " << java_name << ", &" << java_name << "_array_size)));" << Qt::endl
                  << INDENT << "if(" << java_name << "_array_size > 0 && " << java_name << "_array_size != " << java_type->arrayElementCounts()[0].first << ")" << Qt::endl
                  << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << "': Wrong number of elements in array. Found: %1, expected: " << java_type->arrayElementCounts()[0].first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl;
            }else{
                s << INDENT << "jsize " << java_name << "_array_size{0};" << Qt::endl
                  << INDENT << "if((" << java_name << "_array_size = " << __jni_env << "->GetArrayLength(jarray(" << java_name << "))) != " << java_type->arrayElementCounts()[0].first << ")" << Qt::endl
                  << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << "': Wrong number of elements in array. Found: %1, expected: " << java_type->arrayElementCounts()[0].first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl
                  << INDENT;
                writeTypeInfo(s, elementType);
                for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                    s << "*";
                }
                s << " " << java_name << "_array = qtjambi_array_cast<";
                writeTypeInfo(s, elementType, Option(option | SkipName));
                for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                    s << "*";
                }
                s << ">(" << __jni_env << ", ";
                if((option & OptionalScope) == OptionalScope)
                    s << "*";
                s << qtjambi_scope << ", " << java_name << ", " << java_type->arrayElementCounts()[0].first << ");" << Qt::endl;
            }
            s << INDENT;
            writeTypeInfo(s, elementType);
            s << " (&" << qt_name << ")";
            for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                const QPair<int,QString>& pair = java_type->arrayElementCounts()[i];
                if(pair.second.isEmpty())
                    s << "[" << pair.first << "]";
                else
                    s << "[" << pair.second << "]";
            }
            s << " = *reinterpret_cast<";
            writeTypeInfo(s, elementType, Option(option | SkipName));
            s << "(*)";
            for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                const QPair<int,QString>& pair = java_type->arrayElementCounts()[i];
                if(pair.second.isEmpty())
                    s << "[" << pair.first << "]";
                else
                    s << "[" << pair.second << "]";
            }
            s << ">(" << java_name << "_array);" << Qt::endl;
        }else{
            QMap<QString,QString> arrayElements;
            {
                QList<QPair<QString,QList<int>>> ids{{java_name,{}}};
                for(int i=0, l=java_type->arrayElementCounts().size(); i<l; ++i){
                    const QPair<int,QString>& pair = java_type->arrayElementCounts()[i];
                    QList<QPair<QString,QList<int>>> oldIDs = ids;
                    ids.clear();
                    if(i<l-1){
                        for(int m=0; m<oldIDs.size(); ++m){
                            const QString& id = oldIDs[m].first;
                            QString braces;
                            for(int idx : oldIDs[m].second){
                                braces += "[" + QString::number(idx) + "]";
                            }
                            s << INDENT << "jsize " << java_name << "_array_size{0};" << Qt::endl
                              << INDENT << "if((" << java_name << "_array_size = " << __jni_env << "->GetArrayLength(jarray(" << id << "))) != " << pair.first << ")" << Qt::endl
                              << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << braces << "': Wrong number of elements in array. Found: %1, expected: " << pair.first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl;
                            for(int j=0; j<pair.first; ++j){
                                s << INDENT << "jobjectArray " << id << "_" << QString::number(j) << " = " << __jni_env << "->GetObjectArrayElement(" << id << ", " << QString::number(j) << ");" << Qt::endl;
                                ids << QPair<QString,QList<int>>(id + "_" + QString::number(j), QList<int>() << oldIDs[m].second << j);
                            }
                        }
                    }else{
                        for(int m=0; m<oldIDs.size(); ++m){
                            const QString& id = oldIDs[m].first;
                            QString braces;
                            for(int idx : oldIDs[m].second){
                                braces += "[" + QString::number(idx) + "]";
                            }
                            if(elementType->isNativePointer()){
                                s << INDENT << "int " << java_name << "_array_size{0};" << Qt::endl
                                  << INDENT;
                                writeTypeInfo(s, elementType);
                                for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                                    s << "*";
                                }
                                s << " " << java_name << "_array = reinterpret_cast<";
                                writeTypeInfo(s, elementType, Option(option | SkipName));
                                for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                                    s << "*";
                                }
                                s << ">(QtJambiAPI::convertQNativePointerToNative(" << __jni_env << ", " << java_name << ", " << oldIDs[m].second.size() << ", &" << java_name << "_array_size));" << Qt::endl
                                  << INDENT << "if(" << java_name << "_array_size > 0 && " << java_name << "_array_size != " << pair.first << ")" << Qt::endl
                                  << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << braces << "': Wrong number of elements in array. Found: %1, expected: " << pair.first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl;
                            }else{
                                s << INDENT << "jsize " << java_name << "_array_size{0};" << Qt::endl
                                  << INDENT << "if((" << java_name << "_array_size = " << __jni_env << "->GetArrayLength(jarray(" << java_name << "))) != " << pair.first << ")" << Qt::endl
                                  << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << braces << "': Wrong number of elements in array. Found: %1, expected: " << pair.first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl
                                  << INDENT;
                                writeTypeInfo(s, elementType);
                                s << "* " << id << "_array = qtjambi_array_cast<";
                                writeTypeInfo(s, elementType, Option(option | SkipName));
                                s << "*>(" << __jni_env << ", ";
                                if((option & OptionalScope) == OptionalScope)
                                    s << "*";
                                s << qtjambi_scope << ", " << id << ", " << QString::number(pair.first) << ");" << Qt::endl;
                            }
                            for(int j=0; j<pair.first; ++j){
                                arrayElements[qt_name + braces + QChar(u'[') + QString::number(j) + QChar(u']')] = id + QStringLiteral(u"_array[") + QString::number(j) + QChar(u']');
                            }
                        }
                    }
                }
                struct Appender{
                    static QVariantList append(const QString& variable, int number){
                        QVariantList result;
                        for(int i=0; i<number; ++i){
                            result.append(QVariant::fromValue<QString>(variable+"["+QString::number(i)+"]"));
                        }
                        return result;
                    }

                    static QVariantList append(const QVariantList& variable, int number){
                        QVariantList result;
                        for(int i=0; i<variable.size(); ++i){
                            const QVariant& variant = variable.at(i);
                            QVariantList vlist;
                            if(variant.userType()==QMetaType::QString){
                                vlist = append(variant.toString(), number);
                            }else if(variant.userType()==QMetaType::QVariantList){
                                vlist = append(variant.toList(), number);
                            }
                            if(!vlist.isEmpty())
                                result.append(QVariant::fromValue<QVariantList>(vlist));
                        }
                        return result;
                    }

                    static void stream(QTextStream& s, const QMap<QString,QString> &arrayElements, const QVariantList& variable){
                        s << "{";
                        for(int i=0; i<variable.size(); ++i){
                            if(i>0)
                                s << ", ";
                            const QVariant& variant = variable.at(i);
                            if(variant.userType()==QMetaType::QString){
                                s << arrayElements[variant.toString()];
                            }else if(variant.userType()==QMetaType::QVariantList){
                                stream(s, arrayElements, variant.toList());
                            }
                        }
                        s << "}";
                    }
                };

                QVariantList entries = Appender::append(qt_name, java_type->arrayElementCounts()[0].first);
                for(int i=1, l=java_type->arrayElementCounts().size(); i<l; ++i){
                    const QPair<int,QString>& pair = java_type->arrayElementCounts()[i];
                    QVariantList oldEntries = entries;
                    entries = Appender::append(oldEntries, pair.first);
                }
                if(java_function && (java_function->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction){
                    m_arrayConversions[quintptr(java_function)] = arrayElements;
                }else{
                    QTextStream s(&m_arrayConversions[java_function ? quintptr(java_function) : quintptr(java_functional)][qt_name]);
                    Appender::stream(s, arrayElements, entries);
                }
            }
        }
        semiRequired = false;
    } else if(java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type()==ContainerTypeEntry::std_array){
        const MetaType *elementType = java_type->instantiations()[0];
        Q_ASSERT(elementType);
        QString argumentName = java_name;
        if(argument_index>0){
            if(java_function){
                for(MetaArgument* arg : java_function->arguments()){
                    if(arg->argumentIndex()==argument_index-1){
                        argumentName = arg->argumentName();
                        break;
                    }
                }
            }else if(java_functional){
                for(MetaArgument* arg : java_functional->arguments()){
                    if(arg->argumentIndex()==argument_index-1){
                        argumentName = arg->argumentName();
                        break;
                    }
                }
            }
        }
        QMap<QString,QString> arrayElements;
        {
            QList<QPair<QString,QList<int>>> ids{{java_name,{}}};
            {
                int arrayElementCounts = java_type->instantiations()[1]->typeEntry()->name().toInt();
                const QPair<int,QString>& pair = {arrayElementCounts, java_type->instantiations()[1]->typeEntry()->name()};
                QList<QPair<QString,QList<int>>> oldIDs = ids;
                ids.clear();
                for(int m=0; m<oldIDs.size(); ++m){
                    const QString& id = oldIDs[m].first;
                    QString braces;
                    for(int idx : oldIDs[m].second){
                        braces += "[" + QString::number(idx) + "]";
                    }
                    if(elementType->isNativePointer()){
                        s << INDENT << "int " << java_name << "_array_size{0};" << Qt::endl
                          << INDENT;
                        writeTypeInfo(s, elementType);
                        for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                            s << "*";
                        }
                        s << " " << java_name << "_array = reinterpret_cast<";
                        writeTypeInfo(s, elementType, Option(option | SkipName));
                        for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                            s << "*";
                        }
                        s << ">(QtJambiAPI::convertQNativePointerToNative(" << __jni_env << ", " << java_name << ", " << oldIDs[m].second.size() << ", &" << java_name << "_array_size));" << Qt::endl
                          << INDENT << "if(" << java_name << "_array_size > 0 && " << java_name << "_array_size != " << pair.first << ")" << Qt::endl
                          << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << braces << "': Wrong number of elements in array. Found: %1, expected: " << pair.first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl;
                    }else{
                        s << INDENT << "jsize " << java_name << "_array_size{0};" << Qt::endl
                          << INDENT << "if((" << java_name << "_array_size = " << __jni_env << "->GetArrayLength(jarray(" << java_name << "))) != " << pair.first << ")" << Qt::endl
                          << INDENT << "    JavaException::raiseIllegalArgumentException(" << __jni_env << ", QString(\"Argument '" << argumentName << braces << "': Wrong number of elements in array. Found: %1, expected: " << pair.first << "\").arg(" << java_name << "_array_size) QTJAMBI_STACKTRACEINFO);" << Qt::endl
                          << INDENT;
                        writeTypeInfo(s, elementType);
                        s << "* " << id << "_array = qtjambi_array_cast<";
                        writeTypeInfo(s, elementType, Option(option | SkipName));
                        s << "*>(" << __jni_env << ", ";
                        if((option & OptionalScope) == OptionalScope)
                            s << "*";
                        s << qtjambi_scope << ", " << id << ", " << QString::number(pair.first) << ");" << Qt::endl;
                    }
                    for(int j=0; j<pair.first; ++j){
                        arrayElements[qt_name + braces + QChar(u'[') + QString::number(j) + QChar(u']')] = id + QStringLiteral(u"_array[") + QString::number(j) + QChar(u']');
                    }
                }
            }
            struct Appender{
                static QVariantList append(const QString& variable, int number){
                    QVariantList result;
                    for(int i=0; i<number; ++i){
                        result.append(QVariant::fromValue<QString>(variable+"["+QString::number(i)+"]"));
                    }
                    return result;
                }

                static QVariantList append(const QVariantList& variable, int number){
                    QVariantList result;
                    for(int i=0; i<variable.size(); ++i){
                        const QVariant& variant = variable.at(i);
                        QVariantList vlist;
                        if(variant.userType()==QMetaType::QString){
                            vlist = append(variant.toString(), number);
                        }else if(variant.userType()==QMetaType::QVariantList){
                            vlist = append(variant.toList(), number);
                        }
                        if(!vlist.isEmpty())
                            result.append(QVariant::fromValue<QVariantList>(vlist));
                    }
                    return result;
                }

                static void stream(QTextStream& s, const QMap<QString,QString> &arrayElements, const QVariantList& variable){
                    s << "{";
                    for(int i=0; i<variable.size(); ++i){
                        if(i>0)
                            s << ", ";
                        const QVariant& variant = variable.at(i);
                        if(variant.userType()==QMetaType::QString){
                            s << arrayElements[variant.toString()];
                        }else if(variant.userType()==QMetaType::QVariantList){
                            stream(s, arrayElements, variant.toList());
                        }
                    }
                    s << "}";
                }
            };

            QVariantList entries = Appender::append(qt_name, java_type->arrayElementCounts()[0].first);
            for(int i=1, l=java_type->arrayElementCounts().size(); i<l; ++i){
                const QPair<int,QString>& pair = java_type->arrayElementCounts()[i];
                QVariantList oldEntries = entries;
                entries = Appender::append(oldEntries, pair.first);
            }
            if(java_function && (java_function->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction){
                m_arrayConversions[quintptr(java_function)] = arrayElements;
            }else{
                QTextStream s(&m_arrayConversions[java_function ? quintptr(java_function) : quintptr(java_functional)][qt_name]);
                Appender::stream(s, arrayElements, entries);
            }
        }
        semiRequired = false;
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    } else if(java_type->typeEntry()->isComplex()
              && static_cast<const ComplexTypeEntry*>(java_type->typeEntry())->isQModelIndex()){
        if(java_type->indirections().isEmpty() && ((java_type->isConstant() && java_type->getReferenceType()==MetaType::Reference) || java_type->getReferenceType()==MetaType::NoReference)){
            if((option & OptionalScope) == OptionalScope){
                s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                s << INDENT << "    QModelIndex* _" << qt_name << " = new QModelIndex;" << Qt::endl;
                s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                s << INDENT << "*reinterpret_cast<QModelIndex*>(" << qt_name << ") = ";
            }else if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!qt_name.isEmpty())
                    s << INDENT << qt_name << " = ";
            }else{
                s << INDENT;
                writeTypeInfo(s, java_type, Option(ForceValueType));
                s << qt_name << " = ";
            }
            s << "qtjambi_cast<";
            writeTypeInfo(s, java_type, Option(ForceValueType | SkipName));
            s << ">(" << __jni_env << ", " << java_name << ")";
        }else{
            if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!qt_name.isEmpty())
                    s << INDENT << qt_name << " = ";
            }else{
                s << INDENT;
                writeTypeInfo(s, java_type, NoOption);
                s << qt_name << " = ";
            }
            s << "qtjambi_cast<";
            writeTypeInfo(s, java_type, SkipName);
            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
#endif
    } else if (java_type->isCharString()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    char** _" << qt_name << " = new char*(nullptr);" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<char**>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = ";
        }
        s << "qtjambi_cast<";
        writeTypeInfo(s, java_type, SkipName);
        s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
    } else if (java_type->isQString()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QString* _" << qt_name << " = new QString();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QString*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, java_type->indirections().isEmpty() && (java_type->isConstant() || java_type->getReferenceType()!=MetaType::Reference) ? ForceValueType : NoOption);
            s << qt_name << " = ";
        }
        s << "qtjambi_cast<";
        writeTypeInfo(s, java_type, java_type->indirections().isEmpty() && (java_type->isConstant() || java_type->getReferenceType()!=MetaType::Reference) ? Option(ForceValueType | SkipName) : SkipName);
        s << ">(" << __jni_env << ", ";
        if(java_type->isConstant() && !java_type->indirections().isEmpty())
            s << qtjambi_scope << ", ";
        s << java_name << ")";
    } else if (java_type->isQStringRef()) {
        if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        if(qtjambi_scope.isEmpty()){
            s << INDENT << "QString " << qt_name << "_qstring = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ");" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, ForceValueType); //ForceConstReference
            s << qt_name << " = QStringRef(&" << qt_name << "_qstring);" << Qt::endl;
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QStringRef>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isQLatin1String()) {
        if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = ";
        }
        if(qtjambi_scope.isEmpty()){
            s << INDENT << "QByteArray " << qt_name << "_qByteArray = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ").toLatin1();" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, ForceValueType); //ForceConstReference
            s << qt_name << " = QLatin1String(" << qt_name << "_qByteArray);" << Qt::endl;
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QLatin1String>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isQLatin1StringView()) {
        if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = ";
        }
        if(qtjambi_scope.isEmpty()){
            s << INDENT << "QByteArray " << qt_name << "_qByteArray = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ").toLatin1();" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, ForceValueType); //ForceConstReference
            s << qt_name << " = QLatin1StringView(" << qt_name << "_qByteArray);" << Qt::endl;
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QLatin1StringView>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isQStringView()) {
        if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = ";
        }
        if(qtjambi_scope.isEmpty()){
            s << INDENT << "QString " << qt_name << "_qstring = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ");";
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = QStringView(" << qt_name << "_qstring);";
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QStringView>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isQAnyStringView()) {
        if(qtjambi_scope.isEmpty()){
            if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!qt_name.isEmpty())
                    s << INDENT << qt_name << " = ";
            }else{
                s << INDENT;
                writeTypeInfo(s, java_type, NoOption); //ForceConstReference
                s << qt_name << " = ";
            }
            s << INDENT << "QString " << qt_name << "_qstring = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ");";
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = QAnyStringView(" << qt_name << "_qstring);";
            semiRequired = false;
        }else{
            if((option & OptionalScope) == OptionalScope){
                s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "*reinterpret_cast<QAnyStringView*>(" << qt_name << ") = qtjambi_cast<QAnyStringView>(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                    }
                    s << INDENT << "}else{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << qt_name << " = &qtjambi_cast<QAnyStringView&>(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                s << INDENT << "}else{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "return false;" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
                semiRequired = false;
            }else{
                if((option & DirectReturn) == DirectReturn){
                    s << INDENT << "return ";
                }else if((option & NoTmpVariable) == NoTmpVariable){
                    if(!qt_name.isEmpty())
                        s << INDENT << qt_name << " = ";
                }else{
                    s << INDENT;
                    writeTypeInfo(s, java_type, NoOption); //ForceConstReference
                    s << qt_name << " = ";
                }
                s << "qtjambi_cast<QAnyStringView>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
            }
        }
    } else if (java_type->isQUtf8StringView()) {
        if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = ";
        }
        if(qtjambi_scope.isEmpty()){
            s << INDENT << "QString " << qt_name << "_qstring = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ");";
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = QUtf8StringView(" << qt_name << "_qstring);";
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QUtf8StringView>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isQChar()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QChar* _" << qt_name << " = new QChar;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QChar*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT << "QChar " << qt_name << " = ";
        }
        s << "qtjambi_cast<QChar>(" << __jni_env << ", " << java_name << ")";

    } else if (java_type->isEnum() || java_type->isFlags()) {

        bool written = false;
        if (java_type->isEnum()
                && java_type->indirections().isEmpty()
                && (java_type->getReferenceType()!=MetaType::Reference || java_type->isConstant())) {
            const EnumTypeEntry * enumType = static_cast<const EnumTypeEntry *>(java_type->typeEntry());
            if ((!enumType->isPublic() && (option & NoEnumAsInts)==0) ||
                        (option & EnumAsInts)==EnumAsInts) {
                if((option & OptionalScope) == OptionalScope){
                    s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                    s << INDENT << "    qint" << enumType->size() << "* _" << qt_name << " = new qint" << enumType->size() << ";" << Qt::endl;
                    s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                    s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                    s << INDENT << "}" << Qt::endl;
                    s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                    s << INDENT << "*reinterpret_cast<qint" << enumType->size() << "*>(" << qt_name << ") = ";
                }else if((option & DirectReturn) == DirectReturn){
                    s << INDENT << "return ";
                }else if((option & NoTmpVariable) == NoTmpVariable){
                    if(!qt_name.isEmpty())
                        s << INDENT << qt_name << " = ";
                }else{
                    s << INDENT << "qint" << enumType->size() << " " << qt_name << " = ";
                }
                if(!enumType->isPublic() && (option & NoEnumAsInts)==0){
                    s << "QtJambiAPI::enumValue<qint" << enumType->size() << ">(" << __jni_env << ", " << java_name << ")";
                }else{
                    s << "qtjambi_cast<qint" << enumType->size() << ">(" << __jni_env << ", " << java_name << ")";
                }
                written = true;
            }
        }

        if (!written) {
            QString qualified_name = java_type->typeEntry()->qualifiedCppName();
            if((option & OptionalScope) == OptionalScope){
                s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                s << INDENT << "    " << qualified_name << "* _" << qt_name << " = new " << qualified_name << ";" << Qt::endl;
                s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                s << INDENT << "*reinterpret_cast<" << qualified_name << "*>(" << qt_name << ") = ";
            }else if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!qt_name.isEmpty())
                    s << INDENT << qt_name << " = ";
            }else{
                s << INDENT << qualified_name << " " << qt_name << " = ";
            }
            s << "qtjambi_cast<" << qualified_name << ">(" << __jni_env << ", " << java_name << ")";
        }

    } else if (java_type->isContainer()) {
        const ContainerTypeEntry *type =
            static_cast<const ContainerTypeEntry *>(java_type->typeEntry());
        if(!qtjambi_scope.isEmpty()
                && type->type() != ContainerTypeEntry::QArrayDataContainer
                && type->type() != ContainerTypeEntry::QTypedArrayDataContainer){
            if((option & OptionalScope) == OptionalScope){
                if(java_type->indirections().isEmpty()
                        && java_type->getReferenceType()==MetaType::Reference
                        && ! (type->type() == ContainerTypeEntry::PairContainer
                              //|| type->type() == ContainerTypeEntry::QQmlListPropertyContainer
                              || type->type() == ContainerTypeEntry::QDBusReplyContainer
                              )){
                    s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(type->type() == ContainerTypeEntry::PairContainer
                                || type->type() == ContainerTypeEntry::QDBusReplyContainer){
                            s << INDENT << "if(!" << qt_name << "){" << Qt::endl;
                            s << INDENT << "    ";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << "* _" << qt_name << " = new ";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << ";" << Qt::endl;
                            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                            s << INDENT << "}" << Qt::endl;
                            s << INDENT << "*reinterpret_cast<";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << "*>(" << qt_name << ") = qtjambi_cast<";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                        }else{
                            s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "*reinterpret_cast<";
                                writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                s << "*>(" << qt_name << ") = qtjambi_cast<";
                                writeTypeInfo(s, java_type, SkipName);
                                s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                            }
                            s << INDENT << "}else{" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                s << INDENT << qt_name << " = &qtjambi_cast<";
                                writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                s << "&>(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                            }
                            s << INDENT << "}" << Qt::endl;
                        }
                    }
                    s << INDENT << "}else{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        writeJavaToQtContainer(s, java_type, qt_name, java_name, java_function, java_functional, -1, __jni_env, qtjambi_scope, option);
                    }
                    s << INDENT << "}" << Qt::endl;
                }else{
                    //s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                    {
                        //INDENTATION(INDENT)
                        s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT;
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << "* _" << qt_name << " = new ";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << ";" << Qt::endl;
                            s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                            s << INDENT << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                        s << INDENT << "if(!out)" << Qt::endl;
                        s << INDENT << "    return false;" << Qt::endl;
                        s << INDENT << "*reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(" << qt_name << ") = qtjambi_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << ">(" << __jni_env << ", " << java_name << ");" << Qt::endl;
                    }
                }
                semiRequired = false;
            }else{
                if((option & DirectReturn) == DirectReturn){
                    s << INDENT << "return ";
                }else if((option & NoTmpVariable) == NoTmpVariable){
                    if(!qt_name.isEmpty())
                        s << INDENT << qt_name << " = ";
                }else{
                    s << INDENT;
                    writeTypeInfo(s, java_type, NoOption); //ForceConstReference
                    s << qt_name << " = ";
                }
                s << "qtjambi_cast<";
                if (type->type() == ContainerTypeEntry::PairContainer
                        || type->type() == ContainerTypeEntry::QQmlListPropertyContainer
                        || type->type() == ContainerTypeEntry::QDBusReplyContainer) {
                    writeTypeInfo(s, java_type, ForceValueType);
                }else{
                    writeTypeInfo(s, java_type, NoOption);
                }
                s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
            }
        }else{
            writeJavaToQtContainer(s, java_type, qt_name, java_name, java_function, java_functional, -1, __jni_env, qtjambi_scope, option);
            semiRequired = false;
        }
    } else if (java_type->isInitializerList()) {
        if(!qtjambi_scope.isEmpty()){
            if((option & OptionalScope) == OptionalScope){
                if(java_type->indirections().isEmpty() && java_type->getReferenceType()==MetaType::Reference){
                    s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            if(java_type->isConstant()){
                                s << INDENT << "*reinterpret_cast<";
                                writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                s << "*>(" << qt_name << ") = qtjambi_cast<";
                                writeTypeInfo(s, java_type, SkipName);
                                s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                            }else{
                                s << INDENT << "*reinterpret_cast<";
                                writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                s << "*>(" << qt_name << ") = qtjambi_cast<";
                                writeTypeInfo(s, java_type, SkipName);
                                s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                            }
                        }
                        s << INDENT << "}else{" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            if(java_type->isConstant()){
                                s << INDENT << qt_name << " = const_cast<";
                                writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                                s << "*>(&qtjambi_cast<";
                                writeTypeInfo(s, java_type, SkipName);
                                s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << "));" << Qt::endl;
                            }else{
                                s << INDENT << qt_name << " = &qtjambi_cast<";
                                writeTypeInfo(s, java_type, SkipName);
                                s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }
                    s << INDENT << "}else{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "return false;" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }else{
                    s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "if(!" << qt_name << "){" << Qt::endl;
                        s << INDENT << "    ";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "* _" << qt_name << " = new ";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << ";" << Qt::endl;
                        s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                        s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                        s << INDENT << "}" << Qt::endl;
                        s << INDENT << "*reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
                        s << "*>(" << qt_name << ") = qtjambi_cast<";
                        writeTypeInfo(s, java_type, SkipName);
                        s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                    }
                    s << INDENT << "}else{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "return false;" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                semiRequired = false;
            }else{
                if((option & DirectReturn) == DirectReturn){
                    s << INDENT << "return ";
                }else if((option & NoTmpVariable) == NoTmpVariable){
                    if(!qt_name.isEmpty())
                        s << INDENT << qt_name << " = ";
                }else{
                    s << INDENT;
                    writeTypeInfo(s, java_type, NoOption); //ForceConstReference
                    s << qt_name << " = ";
                }
                s << "qtjambi_cast<";
                writeTypeInfo(s, java_type, SkipName);
                s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
            }
        }else{
            writeJavaToQtInitializerList(s, java_type, qt_name, java_name, java_function, java_functional, -1, __jni_env, qtjambi_scope, option);
            semiRequired = false;
        }
    } else if (java_type->isQSpan()) {
        if ((option & UseNativeIds) == 0 || !java_type->hasNativeId()){
          if((option & OptionalScope) == OptionalScope){
              if(java_type->indirections().isEmpty() && java_type->getReferenceType()==MetaType::Reference){
                  s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                  {
                      INDENTATION(INDENT)
                      s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                      {
                          INDENTATION(INDENT)
                          if(java_type->isConstant()){
                              s << INDENT << "*reinterpret_cast<";
                              writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                              s << "*>(" << qt_name << ") = qtjambi_cast<";
                              writeTypeInfo(s, java_type, SkipName);
                              s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                          }else{
                              s << INDENT << "*reinterpret_cast<";
                              writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                              s << "*>(" << qt_name << ") = qtjambi_cast<";
                              writeTypeInfo(s, java_type, SkipName);
                              s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                          }
                      }
                      s << INDENT << "}else{" << Qt::endl;
                      {
                          INDENTATION(INDENT)
                          if(java_type->isConstant()){
                              s << INDENT << qt_name << " = const_cast<";
                              writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                              s << "*>(&qtjambi_cast<";
                              writeTypeInfo(s, java_type, SkipName);
                              s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << "));" << Qt::endl;
                          }else{
                              s << INDENT << qt_name << " = &qtjambi_cast<";
                              writeTypeInfo(s, java_type, SkipName);
                              s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                          }
                      }
                      s << INDENT << "}" << Qt::endl;
                  }
                  s << INDENT << "}else{" << Qt::endl;
                  {
                      INDENTATION(INDENT)
                      s << INDENT << "return false;" << Qt::endl;
                  }
                  s << INDENT << "}" << Qt::endl;
              }else{
                  s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                  {
                      INDENTATION(INDENT)
                      s << INDENT << "if(!" << qt_name << "){" << Qt::endl;
                      s << INDENT << "    ";
                      writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                      s << "* _" << qt_name << " = new ";
                      writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                      s << ";" << Qt::endl;
                      s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                      s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                      s << INDENT << "}" << Qt::endl;
                      s << INDENT << "*reinterpret_cast<";
                      writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
                      s << "*>(" << qt_name << ") = qtjambi_cast<";
                      writeTypeInfo(s, java_type, SkipName);
                      s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                  }
                  s << INDENT << "}else{" << Qt::endl;
                  {
                      INDENTATION(INDENT)
                      s << INDENT << "return false;" << Qt::endl;
                  }
                  s << INDENT << "}" << Qt::endl;
              }
              semiRequired = false;
          }else{
              if((option & DirectReturn) == DirectReturn){
                  s << INDENT << "return ";
              }else if((option & NoTmpVariable) == NoTmpVariable){
                  if(!qt_name.isEmpty())
                      s << INDENT << qt_name << " = ";
              }else{
                  s << INDENT;
                  writeTypeInfo(s, java_type, NoOption); //ForceConstReference
                  s << qt_name << " = ";
              }
              s << "qtjambi_cast<";

              Q_ASSERT(java_type->instantiations().size()>0);
              const MetaType *elementType = java_type->instantiations().at(0);
              writeTypeInfo(s, java_type, SkipName);
              s << ">(" << __jni_env;
              if(!elementType->isConstant() && argument_index>0)
                  s << ", " << qtjambi_scope;
              s << ", " << java_name << ")";
          }
        } else {
          if((option & DirectReturn) == DirectReturn){
              s << INDENT << "return ";
          }else if((option & NoTmpVariable) == NoTmpVariable){
              if(!qt_name.isEmpty())
                  s << INDENT << qt_name << " = ";
          }else{
              s << INDENT;
              writeTypeInfo(s, java_type, NoOption); //ForceConstReference
              s << qt_name << " = ";
          }
          if(java_type->indirections().size()==0 && (java_type->isConstant() || java_type->getReferenceType()==MetaType::NoReference)){
              if(java_type->getReferenceType()==MetaType::NoReference){
                s << "QtJambiAPI::valueFromNativeId<";
              }else{
                s << "QtJambiAPI::valueReferenceFromNativeId<";
              }
              writeTypeInfo(s, java_type, SkipName);
              s << ">(" << java_name << ")";
          } else {
              if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                  s << "QtJambiAPI::interfaceReferenceFromNativeId";
              }else{
                  s << "QtJambiAPI::objectReferenceFromNativeId";
              }
              s << "<";
              writeTypeInfo(s, java_type, SkipName);
              if(java_type->indirections().size()>1){
                  s << " " << QString(java_type->indirections().size()-1, '*');
              }
              s << ">(" << __jni_env << ", " << java_name << ")";
          }
        }
    } else if (java_type->typeEntry()->isQMetaObjectType()) {
        if((option & OptionalScope) == OptionalScope){
            if(java_type->indirections().size()==0 && java_type->getReferenceType()==MetaType::Reference){
                s << qt_name << " = &qtjambi_cast<QMetaObject&>(" << __jni_env << ", " << java_name << ")";
            }else{
                s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                s << INDENT << "    QMetaObject** _" << qt_name << " = new QMetaObject*;" << Qt::endl;
                s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                s << INDENT << "*reinterpret_cast<QMetaObject**>(" << qt_name << ") = qtjambi_cast<QMetaObject*>(" << __jni_env << ", " << java_name << ")";
            }
        }else{
            if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!qt_name.isEmpty())
                    s << INDENT << qt_name << " = ";
            }else{
                s << INDENT;
                writeTypeInfo(s, java_type, NoOption); //ForceConstReference
                s << qt_name << " = ";
            }
            if(java_type->indirections().size()==0 && java_type->getReferenceType()==MetaType::Reference)
                s << "qtjambi_cast<const QMetaObject&>(" << __jni_env << ", " << java_name << ")";
            else
                s << "qtjambi_cast<const QMetaObject*>(" << __jni_env << ", " << java_name << ")";
        }
    } else if (java_type->typeEntry()->isQMetaObjectConnectionType()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QMetaObject::Connection* _" << qt_name << " = new QMetaObject::Connection;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QMetaObject::Connection*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << qt_name << " = ";
        }
        s << "qtjambi_cast<QMetaObject::Connection>(" << __jni_env << ", " << java_name << ")";
    } else {
        const TypeEntry *type = java_type->typeEntry();
        const ComplexTypeEntry *complexType = dynamic_cast<const ImplementorTypeEntry *>(java_type->typeEntry());
        QString qualified_class_name = type->qualifiedCppName();
        if(!java_type->instantiations().isEmpty()){
            qualified_class_name += "<";
            for(int i=0; i<java_type->instantiations().size(); ++i){
                if(i!=0)
                    qualified_class_name += ",";
                QString tp;
                {
                    QTextStream s(&tp);
                    writeTypeInfo(s, java_type->instantiations()[i], SkipName);
                }
                qualified_class_name += tp;
            }
            qualified_class_name += ">";
        }

        QString convertLine;
        {
            QTextStream s(&convertLine);
            const ImplementorTypeEntry * imptype;
            if (java_type->isFunctional()) {
                //const FunctionalTypeEntry* funEntry = static_cast<const FunctionalTypeEntry*>(java_type->typeEntry());
                //QString registeredName;
                //if(funEntry->getUsing().isEmpty()){
                //    registeredName = funEntry->normalizedSignature();
                //}else{
                //    registeredName = funEntry->name();
                //}
                s << "qtjambi_cast<" << qualified_class_name << ">(" << __jni_env << ", " << java_name << ")";
            }else if (java_type->isIterator()) {
                s << "reinterpret_deref_cast<" << qualified_class_name << " "
                  << QString(java_type->actualIndirections(), '*') << ">(" << __jni_env << ", ";
                s << "QtJambiAPI::convertJavaObjectToNative(" << __jni_env << ", " << java_name << "))";

            } else if (java_type->isPrimitive() || java_type->isPrimitiveChar()) {
                if (option & BoxedPrimitive) {
                    if(java_type->isPrimitiveChar()){
                        s << "QtJambiAPI::fromJavaCharacterObject";
                    }else{
                        const PrimitiveTypeEntry *pentry = static_cast<const PrimitiveTypeEntry *>(type);

                        if (!pentry->preferredConversion())
                            pentry = database()->findTargetLangPrimitiveType(pentry->targetLangName());
                        Q_ASSERT(pentry);

                        if(pentry->targetLangName()=="int"){
                            s << "QtJambiAPI::fromJavaIntegerObject";
                        }else if(pentry->targetLangName()=="short"){
                            s << "QtJambiAPI::fromJavaShortObject";
                        }else if(pentry->targetLangName()=="byte"){
                            s << "QtJambiAPI::fromJavaByteObject";
                        }else if(pentry->targetLangName()=="long"){
                            s << "QtJambiAPI::fromJavaLongObject";
                        }else if(pentry->targetLangName()=="float"){
                            s << "QtJambiAPI::fromJavaFloatObject";
                        }else if(pentry->targetLangName()=="double"){
                            s << "QtJambiAPI::fromJavaDoubleObject";
                        }else if(pentry->targetLangName()=="boolean"){
                            s << "QtJambiAPI::fromJavaBooleanObject";
                        }else if(pentry->targetLangName()=="char"){
                            s << "QtJambiAPI::fromJavaCharacterObject";
                        }
                    }
                    s << "(" << __jni_env << ", " << java_name << ")";

                } else if ((option & GlobalRefJObject) && type->jniName() == QLatin1String("jobject")) {
                    s << __jni_env << "->NewGlobalRef(" << java_name << ")";
                } else if(java_type->isQChar()){
                    s << "QChar(";
                    s << java_name;
                    s << ")";
                } else {
                    s << "static_cast<";
                    writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                    s << ">(";
                    s << java_name;
                    s << ")";
                }

            } else if (java_type->isNativePointer()) {
                if (java_type->getReferenceType()==MetaType::Reference) {
                    s << "reinterpret_deref_cast<"
                      << qualified_class_name;
                    if(java_type->indirections().size()>1){
                        s << " " << QString(java_type->indirections().size()-1, '*');
                    }
                    s << ">(" << __jni_env << ", ";
                }else{
                    s << "reinterpret_cast<";
                    writeTypeInfo(s, java_type, SkipName);
                    s << ">(";
                }
                s << "QtJambiAPI::convertQNativePointerToNative("
                  << __jni_env << ", "
                  << java_name << "))";
            } else if (java_type->isSmartPointer()) {
                bool done = false;
                const SmartPointerTypeEntry* pentry = static_cast<const SmartPointerTypeEntry*>(java_type->typeEntry());
                switch(pentry->type()){
                //case SmartPointerTypeEntry::unique_ptr:
                case SmartPointerTypeEntry::QScopedPointer:
                    if(java_type->instantiations().size()>0){
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type->instantiations()[0]);
                        s << "*>(" << __jni_env << ", " << java_name << ")";
                        done = true;
                    }
                    break;
                default:
                    break;
                }
                if(!done){
                    s << "qtjambi_cast<";
                    writeTypeInfo(s, java_type, SkipName);
                    s << ">(" << __jni_env << ", ";
                    if(!qtjambi_scope.isEmpty()){
                        s << qtjambi_scope << ", ";
                    }
                    s << java_name << ")";
                }
            } else if ((imptype = dynamic_cast<const ImplementorTypeEntry *>(type)) && imptype->designatedInterface() && java_type->actualIndirections()<=1) {
                const InterfaceTypeEntry *ie = imptype->designatedInterface();
                if ((option & UseNativeIds) == 0 || !ie->isNativeIdBased()){
                    s << "qtjambi_cast<";
                    writeTypeInfo(s, java_type, SkipName);
                    s << ">(" << __jni_env << ", " << java_name << ")";
                }else{
                    if(java_type->indirections().size()==0){
                        s << "QtJambiAPI::interfaceReferenceFromNativeId<" << qualified_class_name
                          << ">(" << __jni_env << ", ";
                    }else{
                        s << "QtJambiAPI::interfaceFromNativeId<" << qualified_class_name << ">(";
                    }
                    s << java_name << ")";
                }
            } else if (java_type->isObject() || java_type->isQObject()) {
                if (java_type->isQObject()) {
                    if ((option & UseNativeIds) == 0 || !java_type->typeEntry()->isNativeIdBased()){
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type, SkipName);
                        s << ">(" << __jni_env << ", ";
                        if (java_type->getReferenceType()==MetaType::Reference
                                && !qtjambi_scope.isEmpty()
                                && !java_type->typeEntry()->isNativeIdBased()){
                            if((option & OptionalScope) == OptionalScope)
                                s << "*";
                            s << qtjambi_scope << ", ";
                        }
                        s << java_name << ")";
                    }else{
                        if (java_type->getReferenceType()!=MetaType::NoReference) {
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "QtJambiAPI::interfaceReferenceFromNativeId";
                            }else{
                                s << "QtJambiAPI::objectReferenceFromNativeId";
                            }
                            s << "<" << qualified_class_name << ">(" << __jni_env << ", " << java_name << ")";
                        }else{
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "QtJambiAPI::interfaceFromNativeId";
                            }else{
                                s << "QtJambiAPI::objectFromNativeId";
                            }
                            s << "<" << qualified_class_name << ">(" << java_name << ")";
                        }
                    }
                } else {
                    if ((option & UseNativeIds) == 0 || !java_type->typeEntry()->isNativeIdBased()){
                        if(complexType && complexType->isGLsync()){
                            s << "QtJambiAPI::convertJavaObjectToNative<__GLsync>(" << __jni_env << ", " << java_name << ")";
                        }else{
                            s << "qtjambi_cast<";
                            writeTypeInfo(s, java_type, SkipName);
                            s << ">(" << __jni_env << ", ";
                            if (java_type->getReferenceType()==MetaType::Reference
                                    && !qtjambi_scope.isEmpty()
                                    && !java_type->typeEntry()->isNativeIdBased()){
                                if((option & OptionalScope) == OptionalScope)
                                    s << "*";
                                s << qtjambi_scope << ", ";
                            }
                            s << java_name << ")";
                        }
                    }else{
                        if (java_type->getReferenceType()!=MetaType::NoReference) {
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "QtJambiAPI::interfaceReferenceFromNativeId<" << qualified_class_name << ">(" << __jni_env << ", " << java_name << ")";
                            }else{
                                if(java_type->isConstant() && java_type->typeEntry()->qualifiedCppName()=="QMessageLogContext"){
                                    s << "QtJambiAPI::valueReferenceFromNativeId<" << qualified_class_name << ">(" << java_name << ")";
                                }else{
                                    s << "QtJambiAPI::objectReferenceFromNativeId<" << qualified_class_name << ">(" << __jni_env << ", " << java_name << ")";
                                }
                            }
                        }else{
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "QtJambiAPI::interfaceFromNativeId";
                            }else{
                                s << "QtJambiAPI::objectFromNativeId";
                            }
                            s << "<" << qualified_class_name << ">(" << java_name << ")";
                        }
                    }
                }

            } else {
                // Return values...
                if(!java_type->isConstant() && java_type->getReferenceType()!=MetaType::NoReference){
                    if ((option & UseNativeIds) == 0 || !java_type->typeEntry()->isNativeIdBased()){
                        if(java_type->typeEntry()==TypeDatabase::instance()->qvariantType()){
                            s << "QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(" << __jni_env << ", " << java_name << ")";
                        }else if(java_type->typeEntry()==TypeDatabase::instance()->qstringType()){
                            s << "QtJambiAPI::convertJavaObjectToNativeReference<QString>(" << __jni_env << ", " << java_name << ")";
                        }else if(java_type->typeEntry()==TypeDatabase::instance()->qcharType()){
                            s << "QtJambiAPI::convertJavaObjectToNativeReference<QChar>(" << __jni_env << ", " << java_name << ")";
                        }else{
                            s << "qtjambi_cast<";
                            writeTypeInfo(s, java_type, SkipName);
                            s << ">(" << __jni_env << ", " << java_name << ")";
                        }
                    }else{
                        if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::interfaceReferenceFromNativeId";
                        }else{
                            s << "QtJambiAPI::objectReferenceFromNativeId";
                        }
                        s << "<" << qualified_class_name
                          << ">(" << __jni_env << ", " << java_name << ")";
                    }
                }else if (argument_index == 0) {
                    if ((option & UseNativeIds) == 0 || !java_type->typeEntry()->isNativeIdBased()){
                        //s << "QtJambiAPI::convertJavaObjectToNative(" << __jni_env << ", " << java_name << ")";
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type, SkipName);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    }else{
                        s << "reinterpret_value_cast<"
                          << qualified_class_name;
                        if(java_type->indirections().size()>1){
                            s << " " << QString(java_type->indirections().size()-1, '*');
                        }
                        s << ">(";
                        if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::interfaceFromNativeId";
                        }else{
                            s << "QtJambiAPI::objectFromNativeId";
                        }
                        s << "<" << qualified_class_name
                          << ">(" << __jni_env << ", " << java_name << ")";
                        s << ")";
                    }
                } else {
                    if ((option & UseNativeIds) == 0 || !java_type->typeEntry()->isNativeIdBased()) {
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type, SkipName);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    } else if(java_type->indirections().size()==0 && (java_type->isConstant() || java_type->getReferenceType()==MetaType::NoReference) && hasPublicDefaultConstructor(java_type)){
                        if(java_type->getReferenceType()==MetaType::NoReference){
                          s << "QtJambiAPI::valueFromNativeId<";
                        }else{
                          s << "QtJambiAPI::valueReferenceFromNativeId<";
                        }
                        s << qualified_class_name;
                        if(java_type->indirections().size()>1){
                            s << " " << QString(java_type->indirections().size()-1, '*');
                        }
                        s << ">(" << java_name << ")";
                    } else {
                        if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                            s << "QtJambiAPI::interfaceReferenceFromNativeId";
                        }else{
                            s << "QtJambiAPI::objectReferenceFromNativeId";
                        }
                        s << "<" << qualified_class_name
                          << ">(" << __jni_env << ", " << java_name << ")";
                    }
                }
            }
        }

        if((option & OptionalScope) == OptionalScope){
            Option _option = Option(SkipName | ExcludeReference);
            if (java_type->isPrimitive() || java_type->isPrimitiveChar()) {
                _option = Option(_option | ExcludeConst);
            }
            if(java_type->indirections().isEmpty() &&
                    (java_type->getReferenceType()==MetaType::Reference
                     || (complexType && (complexType->isObject() || complexType->isInterface()) && !complexType->hasPublicDefaultAssignment()))){
                if(java_type->isQObject() || !complexType || (!complexType->hasPublicDefaultAssignment()
                                                              && !(complexType->hasPublicMoveAssignment() && complexType->hasPublicCopyConstructor())
                                                              && !(complexType->isDestructorPublic() && complexType->hasPublicCopyConstructor()))){
                    if(java_type->isConstant()){
                        s << INDENT << qt_name << " = const_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(&" << convertLine << ");" << Qt::endl;
                    }else{
                        s << INDENT << qt_name << " = &" << convertLine << ";" << Qt::endl;
                    }
                }else{
                    s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                    if(complexType->hasPublicDefaultAssignment()){
                        INDENTATION(INDENT)
                        s << INDENT << "*reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(" << qt_name << ") = " << convertLine << ";" << Qt::endl;
                    }else if(complexType->hasPublicMoveAssignment() && complexType->hasPublicCopyConstructor()){
                        INDENTATION(INDENT)
                        s << INDENT << "*reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(" << qt_name << ") = " << java_type->typeEntry()->qualifiedCppName() << "(" << convertLine << ");" << Qt::endl;
                    }else /*if(complexType->isDestructorPublic() && complexType->hasPublicCopyConstructor())*/ {
                        INDENTATION(INDENT)
                        s << INDENT << "reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(" << qt_name << ")->~" << java_type->typeEntry()->qualifiedCppName().split("::").last() << "();" << Qt::endl;
                        s << INDENT << "new (" << qt_name << ") " << java_type->typeEntry()->qualifiedCppName() << "(" << convertLine << ");" << Qt::endl;
                    }
                    s << INDENT << "}else{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(java_type->isConstant()){
                            s << INDENT << qt_name << " = const_cast<";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << "*>(&" << convertLine << ");" << Qt::endl;
                        }else{
                            s << INDENT << qt_name << " = &" << convertLine << ";" << Qt::endl;
                        }
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                semiRequired = false;
            }else{
                if(java_type->indirections().isEmpty()){
                    if((java_type->typeEntry()->isComplex() && dynamic_cast<const ComplexTypeEntry*>(java_type->typeEntry())->hasPublicDefaultConstructor())
                            || java_type->isContainer()
                            || java_type->isPrimitive()
                            || java_type->isPrimitiveChar()
                            || java_type->isEnum()
                            || java_type->isFlags()
                            || java_type->isQString()
                            || java_type->isQVariant()){
                        s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT;
                            writeTypeInfo(s, java_type, _option);
                            s << "* _" << qt_name << " = new ";
                            writeTypeInfo(s, java_type, _option);
                            s << ";" << Qt::endl;
                            s << INDENT << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                            s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else{
                        bool isMetaTypeRegistered = false;
                        if(java_type->typeEntry()->isComplex()){
                            const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(java_type->typeEntry());
                            if(ctype->isInterface()){
                                const ComplexTypeEntry* original = static_cast<const InterfaceTypeEntry*>(ctype)->origin();
                                if(!original->skipMetaTypeRegistration() && !original->hasPureVirtualFunctions()){
                                    isMetaTypeRegistered = original->isValue()
                                                            || ((original->hasPublicCopyConstructor()
                                                                   || original->customConstructor(ComplexTypeEntry::CopyConstructor).hasCode())
                                                               && (original->hasPublicDefaultConstructor()
                                                                   || original->customConstructor(ComplexTypeEntry::DefaultConstructor).hasCode())
                                                               && (original->isDestructorPublic()
                                                                   || original->customDestructor().hasCode())
                                                                && !original->skipMetaTypeRegistration());
                                }
                            }else if(!ctype->skipMetaTypeRegistration() && !ctype->hasPureVirtualFunctions()){
                                isMetaTypeRegistered = ctype->isValue()
                                                            || ((ctype->hasPublicCopyConstructor()
                                                                   || ctype->customConstructor(ComplexTypeEntry::CopyConstructor).hasCode())
                                                               && (ctype->hasPublicDefaultConstructor()
                                                                   || ctype->customConstructor(ComplexTypeEntry::DefaultConstructor).hasCode())
                                                               && (ctype->isDestructorPublic()
                                                                   || ctype->customDestructor().hasCode())
                                                                && !ctype->skipMetaTypeRegistration());
                            }
                        }
                        if(isMetaTypeRegistered){
                            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                                    s << INDENT << "int metaTypeID = QMetaType::type(\"" << qualified_class_name << "\");" << Qt::endl;
                                    s << INDENT << "if(metaTypeID!=QMetaType::UnknownType){" << Qt::endl;
                                    {
                                        INDENTATION(INDENT)
                                        s << INDENT << "void* _" << qt_name << " = QMetaType::create(metaTypeID, nullptr);" << Qt::endl;
                                        s << INDENT << qtjambi_scope << "->addDeletion(metaTypeID, _" << qt_name << ");" << Qt::endl;
                                        s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                                    }
                                    s << INDENT << "}" << Qt::endl;
                                }else{
                                    s << INDENT << "QMetaType metaType = QMetaType::fromType<" << qualified_class_name << ">();" << Qt::endl;
                                    s << INDENT << "if(metaType.iface() && metaType.iface()->defaultCtr){" << Qt::endl;
                                    {
                                        INDENTATION(INDENT)
                                        s << INDENT << "void* _" << qt_name << " = metaType.create();" << Qt::endl;
                                        s << INDENT << qtjambi_scope << "->addDeletion(metaType, _" << qt_name << ");" << Qt::endl;
                                        s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                                    }
                                    s << INDENT << "}" << Qt::endl;
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else{
                            s << INDENT << "Q_UNUSED(" << qtjambi_scope << ")" << Qt::endl;
                        }
                    }
                }else{
                    s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "void** _" << qt_name << " = new void*;" << Qt::endl;
                        s << INDENT << qtjambi_scope << "->addDeletion(_" << qt_name << ");" << Qt::endl;
                        s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return false;" << Qt::endl;
                s << INDENT << "*reinterpret_cast<";
                writeTypeInfo(s, java_type, _option);
                s << "*>(" << qt_name << ") = " << convertLine;
            }
        }else{
            if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!qt_name.isEmpty())
                    s << INDENT << qt_name << " = ";
            }else{
                bool done = false;
                if (java_type->isSmartPointer()) {
                    const SmartPointerTypeEntry* pentry = static_cast<const SmartPointerTypeEntry*>(java_type->typeEntry());
                    switch(pentry->type()){
                    //case SmartPointerTypeEntry::unique_ptr:
                    case SmartPointerTypeEntry::QScopedPointer:
                    if(java_type->instantiations().size()>0){
                        s << INDENT;
                        writeTypeInfo(s, java_type->instantiations()[0], SkipName);
                        s << "* " << qt_name << " = ";
                        done = true;
                        invalidate = lang==TS::ShellCode;
                    }
                        break;
                    default:
                        break;
                    }
                }
                if(!done){
                    s << INDENT;
                    if(java_type->isFunctional()){
                        writeTypeInfo(s, java_type, Option(ExcludeReference | SkipName));
                    }else if(java_type->getReferenceType()==MetaType::RReference && java_type->hasNativeId()){
                        writeTypeInfo(s, java_type, Option(ExcludeReference | SkipName));
                        if(java_type->indirections().isEmpty())
                            s << "&";
                    }else{
                        writeTypeInfo(s, java_type, SkipName); //ForceConstReference
                    }
                    s << ' ' << qt_name << " = ";
                }
            }
            s << convertLine;
        }
    }
    if(parenRequired){
        s << ")";
    }
    if(semiRequired){
        s << ";" << Qt::endl;
    }
    if(invalidate){
        s << INDENT << "QTJAMBI_FORCED_INVALIDATE_AFTER_USE(" << __jni_env << ", " << java_name << ");" << Qt::endl;
    }
    return false;
}

static int nativePointerType(const MetaType *java_type) {
    Q_ASSERT(java_type);
    Q_ASSERT(java_type->isNativePointer());

    if (!java_type->typeEntry()->isPrimitive() && !java_type->typeEntry()->isQChar())
        return int(QNativePointer::Type::Pointer);

    if (java_type->indirections().size() > 1)
        return int(QNativePointer::Type::Pointer);

    static const QMap<QString, int> types{
        {QLatin1String("boolean"), int(QNativePointer::Type::Boolean)},
        {QLatin1String("byte"),    int(QNativePointer::Type::Byte)},
        {QLatin1String("char"),    int(QNativePointer::Type::Char)},
        {QLatin1String("short"),   int(QNativePointer::Type::Short)},
        {QLatin1String("int"),     int(QNativePointer::Type::Int)},
        {QLatin1String("long"),    int(QNativePointer::Type::Long)},
        {QLatin1String("float"),   int(QNativePointer::Type::Float)},
        {QLatin1String("double"),  int(QNativePointer::Type::Double)}
    };

    QString targetLangName = java_type->typeEntry()->targetLangName();
    if (!types.contains(targetLangName))
        return int(QNativePointer::Type::Pointer);

    return types[targetLangName];
}

static const QString nativePointerTypeString(const MetaType *java_type) {
    int type = nativePointerType(java_type);

    static const QHash<int, QString> macrodef{
        {int(QNativePointer::Type::Boolean), QLatin1String("QNativePointer::Type::Boolean")},
        {int(QNativePointer::Type::Byte),    QLatin1String("QNativePointer::Type::Byte")},
        {int(QNativePointer::Type::Char),    QLatin1String("QNativePointer::Type::Char")},
        {int(QNativePointer::Type::Short),   QLatin1String("QNativePointer::Type::Short")},
        {int(QNativePointer::Type::Int),     QLatin1String("QNativePointer::Type::Int")},
        {int(QNativePointer::Type::Long),    QLatin1String("QNativePointer::Type::Long")},
        {int(QNativePointer::Type::Float),   QLatin1String("QNativePointer::Type::Float")},
        {int(QNativePointer::Type::Double),  QLatin1String("QNativePointer::Type::Double")},
        {int(QNativePointer::Type::Pointer), QLatin1String("QNativePointer::Type::Pointer")},
        {int(QNativePointer::Type::String),  QLatin1String("QNativePointer::Type::String")}
    };

    if(macrodef.contains(type))
        return macrodef[type];

    return QString("%1").arg(type);
}

bool CppImplGenerator::writeQtToJava(QTextStream &s,
                                     const MetaType *java_type,
                                     const QString &qt_name,
                                     const QString &java_name,
                                     const MetaFunction *java_function,
                                     const MetaFunctional *java_functional,
                                     int argument_index,
                                     Option option,
                                     const QString& __jni_env,
                                     const QString& qtjambi_scope) {
    // Conversion to Java: Native code for return values, shell code for arguments
    TS::Language lang = argument_index == 0 ? TS::NativeCode : TS::ShellCode;
    if(writeConversionRule(s, lang, java_function, java_functional, argument_index, qt_name, java_name, option, __jni_env, qtjambi_scope))
        return true;

    QScopedPointer<Indentation> indentationPointer;
    bool semiRequired = (option & NoTmpVariable) != NoTmpVariable || !java_name.isEmpty();
    if (!java_type) {
        QString functionName;
        if(java_function){
            functionName = java_function->implementingClass()->qualifiedCppName()+"::"+java_function->name();
        }else if(java_functional){
            if(java_functional->enclosingClass())
                functionName = java_functional->enclosingClass()->name();
            functionName += java_functional->name();
        }else{
            functionName = "unknown";
        }
        QString warn = QString("no conversion possible for argument '%1' in function '%2' for "
                               "language '%3'")
                       .arg(argument_index)
                       .arg(functionName)
                       .arg(int(lang));
        ReportHandler::warning(warn);
        return true;
    }
    if(java_type->isNativePointer()){
        QString functionName;
        if(java_function){
            functionName = java_function->implementingClass()->qualifiedCppName()+"::"+java_function->name();
        }else if(java_functional){
            if(java_functional->enclosingClass())
                functionName = java_functional->enclosingClass()->name();
            functionName += java_functional->name();
        }else{
            functionName = "unknown";
        }
        ReportHandler::debugFull(QString("native pointer pattern for '%1'  %2")
                                 .arg(java_type->cppSignature(), functionName));
    }

    if (java_type->isArray()) { // jniTypeName(java_type)
        MetaType *elementType = java_type->arrayElementType();
        Q_ASSERT(elementType);
        if(elementType->isNativePointer()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << translateType(elementType, option) << " " << java_name << " = ";
            }
            s << "QtJambiAPI::convertNativeToQNativePointer(" << __jni_env << ", ";
            if (elementType->getReferenceType()==MetaType::Reference)
                s << "&";
            int actualIndirections = elementType->actualIndirections() + java_type->arrayElementCounts().size();
            if(actualIndirections==0 && elementType->typeEntry()->isAlias() && static_cast<const TS::AliasTypeEntry*>(elementType->typeEntry())->getHasIndirections()){
                ++actualIndirections;
            }
            s << qt_name << ", " << nativePointerTypeString(elementType) << ", ";
            if(java_type->arrayElementCounts()[0].first==0)
                s << java_type->arrayElementCounts()[0].second;
            else
                s << java_type->arrayElementCounts()[0].first;
            s << ", " << actualIndirections << ")";
        }else if((elementType->isPrimitive() || elementType->isPrimitiveChar()) && java_type->arrayElementCounts().size()==1){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << translateType(java_type, option) << " " << java_name << " = ";
            }
            QString arrayType = translateType(java_type, option);
            if(arrayType.length()>1){
                arrayType[0] = arrayType[0].toUpper();
                arrayType[1] = arrayType[1].toUpper();
            }
            s << "QtJambiAPI::to" << arrayType << "(" << __jni_env << ", reinterpret_cast<"
              << translateType(elementType, option);
            for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                s << "*";
            }
            s << ">(" << qt_name << "), " << java_type->arrayElementCounts()[0].first << ")";
        }else{
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << "jobjectArray " << java_name << " = ";
            }
            QString qualifiedTargetLangName = elementType->fullName();
            if(!elementType->package().isEmpty()){
                qualifiedTargetLangName = qualifiedTargetLangName.mid(elementType->package().length()+1).replace(".", "$");
                qualifiedTargetLangName = elementType->package().replace(".", "/") + "/" + qualifiedTargetLangName;
            }
            if(qualifiedTargetLangName=="void"){
                qualifiedTargetLangName = "io/qt/QNativePointer";
            }
            for(int i=1; i<java_type->arrayElementCounts().size(); ++i){
                if(qualifiedTargetLangName.startsWith("[")){
                    qualifiedTargetLangName = "["+qualifiedTargetLangName;
                }else{
                    if(qualifiedTargetLangName=="int")
                        qualifiedTargetLangName = "[I";
                    else if(qualifiedTargetLangName=="byte")
                        qualifiedTargetLangName = "[B";
                    else if(qualifiedTargetLangName=="short")
                        qualifiedTargetLangName = "[S";
                    else if(qualifiedTargetLangName=="long")
                        qualifiedTargetLangName = "[J";
                    else if(qualifiedTargetLangName=="double")
                        qualifiedTargetLangName = "[D";
                    else if(qualifiedTargetLangName=="float")
                        qualifiedTargetLangName = "[F";
                    else if(qualifiedTargetLangName=="char")
                        qualifiedTargetLangName = "[C";
                    else if(qualifiedTargetLangName=="boolean")
                        qualifiedTargetLangName = "[Z";
                    else
                        qualifiedTargetLangName = "[L"+qualifiedTargetLangName+";";
                }
            }
            const MetaType* newArrayType = elementType;
            std::unique_ptr<TS::ArrayTypeEntry> arrayTypeEntry;
            std::unique_ptr<MetaType> arrayType;
            std::unique_ptr<MetaType> pointerType{elementType->copy()};
            if(java_type->arrayElementCounts().size()>1){
                arrayType.reset(java_type->copy());
                QList<QPair<int,QString>> arrayElementCounts = arrayType->arrayElementCounts();
                arrayElementCounts.takeFirst();
                arrayType->setArrayElementCounts(arrayElementCounts);
                arrayTypeEntry.reset(new ArrayTypeEntry("", elementType->typeEntry(), int(arrayElementCounts.size())));
                arrayType->setTypeEntry(arrayTypeEntry.get());
                MetaBuilder::decideUsagePattern(arrayType.get());
                newArrayType = arrayType.get();
                QList<bool> indirections = pointerType->indirections();
                for(QPair<int,QString>& c : arrayElementCounts){
                    Q_UNUSED(c)
                    indirections << false;
                }
                pointerType->setIndirections(indirections);
                MetaBuilder::decideUsagePattern(pointerType.get());
            }
            s << "QtJambiAPI::toJObjectArray<";
            if(pointerType->indirections().isEmpty())
                writeTypeInfo(s, pointerType.get(), Option(ForceValueType));
            else
                writeTypeInfo(s, pointerType.get(), Option(NoOption));
            s << ">(" << __jni_env
              << ", \"" << qualifiedTargetLangName << "\""
              << ", " << qt_name
              << ", jsize(" << java_type->arrayElementCounts()[0].first << "), [](JNIEnv * " << __jni_env << ", ";
            writeTypeInfo(s, pointerType.get(), Option(ForceConstReference));
            s << " __element) -> jobject {" << Qt::endl;
            {
                INDENTATION(INDENT)
                quint32 option2 = quint32(option);
                option2 |= DirectReturn;
                option2 |= NoTmpVariable;
                writeQtToJava(s, newArrayType, "__element", "", nullptr, nullptr, -1, Option(option2), __jni_env, "");
            }
            s << ";" << Qt::endl << INDENT << "}";
            s << ")";
        }
    } else if(java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type()==ContainerTypeEntry::std_array){
        const MetaType *elementType = java_type->instantiations()[0];
        QString sizeExpression = java_type->instantiations()[1]->typeEntry()->name();
        Q_ASSERT(elementType);
        if(elementType->isNativePointer()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << translateType(elementType, option) << " " << java_name << " = ";
            }
            s << "QtJambiAPI::convertNativeToQNativePointer(" << __jni_env << ", ";
            int actualIndirections = elementType->actualIndirections() + 1;
            if(actualIndirections==0 && elementType->typeEntry()->isAlias() && static_cast<const TS::AliasTypeEntry*>(elementType->typeEntry())->getHasIndirections()){
                ++actualIndirections;
            }
            s << qt_name << ".data(), " << nativePointerTypeString(elementType) << ", " << sizeExpression << ", " << actualIndirections << ")";
        }else if(elementType->isPrimitive() || elementType->isPrimitiveChar()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << translateType(elementType, option) << "Array " << java_name << " = ";
            }
            QString arrayType = translateType(elementType, Option(option | ForceValueType));
            QString _arrayType = arrayType;
            if(arrayType.length()>1){
                arrayType[0] = arrayType[0].toUpper();
                arrayType[1] = arrayType[1].toUpper();
            }
            s << "QtJambiAPI::to" << arrayType << "Array(" << __jni_env << ", reinterpret_cast<const " << _arrayType << "*>(" << qt_name << ".data()), " << sizeExpression << ")";
        }else{
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << "jobjectArray " << java_name << " = ";
            }
            QString qualifiedTargetLangName = elementType->fullName();
            if(!elementType->package().isEmpty()){
                qualifiedTargetLangName = qualifiedTargetLangName.mid(elementType->package().length()+1).replace(".", "$");
                qualifiedTargetLangName = elementType->package().replace(".", "/") + "/" + qualifiedTargetLangName;
            }
            if(qualifiedTargetLangName=="void"){
                qualifiedTargetLangName = "io/qt/QNativePointer";
            }
            for(int i=1; i<java_type->arrayElementCounts().size(); ++i){
                if(qualifiedTargetLangName.startsWith("[")){
                    qualifiedTargetLangName = "["+qualifiedTargetLangName;
                }else{
                    if(qualifiedTargetLangName=="int")
                        qualifiedTargetLangName = "[I";
                    else if(qualifiedTargetLangName=="byte")
                        qualifiedTargetLangName = "[B";
                    else if(qualifiedTargetLangName=="short")
                        qualifiedTargetLangName = "[S";
                    else if(qualifiedTargetLangName=="long")
                        qualifiedTargetLangName = "[J";
                    else if(qualifiedTargetLangName=="double")
                        qualifiedTargetLangName = "[D";
                    else if(qualifiedTargetLangName=="float")
                        qualifiedTargetLangName = "[F";
                    else if(qualifiedTargetLangName=="char")
                        qualifiedTargetLangName = "[C";
                    else if(qualifiedTargetLangName=="boolean")
                        qualifiedTargetLangName = "[Z";
                    else
                        qualifiedTargetLangName = "[L"+qualifiedTargetLangName+";";
                }
            }
            const MetaType* newArrayType = elementType;
            std::unique_ptr<TS::ArrayTypeEntry> arrayTypeEntry;
            std::unique_ptr<MetaType> arrayType;
            std::unique_ptr<MetaType> pointerType{elementType->copy()};
            if(java_type->arrayElementCounts().size()>1){
                arrayType.reset(java_type->copy());
                QList<QPair<int,QString>> arrayElementCounts = arrayType->arrayElementCounts();
                arrayElementCounts.takeFirst();
                arrayType->setArrayElementCounts(arrayElementCounts);
                arrayTypeEntry.reset(new ArrayTypeEntry("", elementType->typeEntry(), int(arrayElementCounts.size())));
                arrayType->setTypeEntry(arrayTypeEntry.get());
                MetaBuilder::decideUsagePattern(arrayType.get());
                newArrayType = arrayType.get();
                QList<bool> indirections = pointerType->indirections();
                for(QPair<int,QString>& c : arrayElementCounts){
                    Q_UNUSED(c)
                    indirections << false;
                }
                pointerType->setIndirections(indirections);
                MetaBuilder::decideUsagePattern(pointerType.get());
            }
            s << "QtJambiAPI::toJObjectArray<";
            if(pointerType->indirections().isEmpty())
                writeTypeInfo(s, pointerType.get(), Option(ForceValueType));
            else
                writeTypeInfo(s, pointerType.get(), Option(NoOption));
            s << ">(" << __jni_env
              << ", \"" << qualifiedTargetLangName << "\""
              << ", " << qt_name
              << ".data(), jsize(" << sizeExpression << "), [](JNIEnv * " << __jni_env << ", ";
            writeTypeInfo(s, pointerType.get(), Option(ForceConstReference));
            s << " __element) -> jobject {" << Qt::endl;
            {
                INDENTATION(INDENT)
                quint32 option2 = quint32(option);
                option2 |= DirectReturn;
                option2 |= NoTmpVariable;
                writeQtToJava(s, newArrayType, "__element", "", nullptr, nullptr, -1, Option(option2), __jni_env, "");
            }
            s << ";" << Qt::endl << INDENT << "}";
            s << ")";
        }
    } else if (java_type->isInitializerList()) {
        Q_ASSERT(java_type->instantiations().size()>0);
        const MetaType *elementType = java_type->instantiations().at(0);
        if(elementType->isPrimitive() || elementType->isPrimitiveChar()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << translateType(java_type, option) << " " << java_name << " = ";
            }
            s << "qtjambi_cast<" << translateType(java_type, option) << ">(" << __jni_env << ", ";
            if(!qtjambi_scope.isEmpty() && !elementType->isConstant()){
                if((option & OptionalScope) == OptionalScope)
                    s << "*";
                s << qtjambi_scope << ", ";
            }
            s << qt_name << ")";
        }else{
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << "jobjectArray " << java_name << " = ";
            }
            s << "qtjambi_cast<jobjectArray>(" << __jni_env << ", ";
            if(!qtjambi_scope.isEmpty()){
                if((option & OptionalScope) == OptionalScope)
                    s << "*";
                s << qtjambi_scope << ", ";
            }
            s << qt_name << ")";
        }
    } else if (java_type->isQSpan()) {
        s << INDENT;
        QString jtype = translateType(java_type, option);
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            s << jtype << " " << java_name << " = ";
        }
        Q_ASSERT(java_type->instantiations().size()>0);
        const MetaType *elementType = java_type->instantiations().at(0);
        if(elementType->isConstant() && argument_index==0){
            s << "QtJambiAPI::convertQSpanToDetachedJavaObject(" << __jni_env << ", " << qt_name << ")";
        }else{
            s << "qtjambi_cast<" << jtype << ">(" << __jni_env << ", ";
            if(argument_index==0 && !elementType->isConstant() && !qtjambi_scope.isEmpty()){
                if((option & OptionalScope) == OptionalScope)
                    s << "*";
                s << qtjambi_scope << ", ";
            }
            s << qt_name << ")";
        }
    } else if (java_type->isPrimitive()) {
        const PrimitiveTypeEntry *type =
            static_cast<const PrimitiveTypeEntry *>(java_type->typeEntry());
        Q_ASSERT(type);
        s << INDENT;
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            if (option & BoxedPrimitive) {
                s << "jobject " << java_name << " = ";
            }else{
                s << type->jniName() <<  " " << java_name << " = ";
            }
        }
        if (option & BoxedPrimitive) {
            if(type->targetLangName()=="int"){
                s << "QtJambiAPI::toJavaIntegerObject";
            }else if(type->targetLangName()=="short"){
                s << "QtJambiAPI::toJavaShortObject";
            }else if(type->targetLangName()=="byte"){
                s << "QtJambiAPI::toJavaByteObject";
            }else if(type->targetLangName()=="long"){
                s << "QtJambiAPI::toJavaLongObject";
            }else if(type->targetLangName()=="float"){
                s << "QtJambiAPI::toJavaFloatObject";
            }else if(type->targetLangName()=="double"){
                s << "QtJambiAPI::toJavaDoubleObject";
            }else if(type->targetLangName()=="boolean"){
                s << "QtJambiAPI::toJavaBooleanObject";
            }else if(type->targetLangName()=="char"){
                s << "QtJambiAPI::toJavaCharacterObject";
            }
            s << "(" << __jni_env << ", " << qt_name << ")";
        } else {
            QString newType;
            if(java_function)
                newType = java_function->typeReplaced(0);
            if(newType.isEmpty()){
                s << type->jniName() << "(" << qt_name << ")";
            }else{
                newType = jniName(newType);
                if(newType=="jboolean"
                        || newType=="jint"
                        || newType=="jdouble"
                        || newType=="jfloat"
                        || newType=="jchar"
                        || newType=="jlong"
                        || newType=="jbyte"
                        || newType=="jshort")
                    s << newType << "(" << qt_name << ")";
            }
        }
    } else if (java_type->isQString()
               || java_type->isCharString()
               || java_type->isQStringRef()
               || java_type->isQStringView()
               || java_type->isQAnyStringView()
               || java_type->isQUtf8StringView()
               || java_type->isQLatin1String()
               || java_type->isQLatin1StringView()) {
        if(java_type->isQString() && !java_type->typeEntry()->isQString()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << "jobject " << java_name << " = ";
            }
            s << "QtJambiAPI::convertQStringToJavaObject(" << __jni_env << ", ";
            if(!java_type->isConstant() && java_type->indirections().isEmpty() && java_type->getReferenceType()==MetaType::Reference)
                s << "&";
            s << qt_name << ")";
        }else{
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << "jstring " << java_name << " = ";
            }
            s << "qtjambi_cast<jstring>(" << __jni_env << ", " << qt_name << ")";
        }
    } else if (java_type->isQChar()) {
        if(java_type->typeEntry()->isQChar()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                if (option & BoxedPrimitive) {
                    s << "jobject " << java_name << " = ";
                }else{
                    s << "jchar " << java_name << " = ";
                }
            }
            if (option & BoxedPrimitive) {
                s << "QtJambiAPI::toJavaCharacterObject("
                    << __jni_env
                    << qt_name << ".unicode())";
            }else{
                s << qt_name << ".unicode()";
            }
        }else{
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << "jobject " << java_name << " = ";
            }
            s << "QtJambiAPI::convertQCharToJavaObject(" << __jni_env << ", " << qt_name << ")";
        }
    } else if (java_type->isIntegerFlags()
               || ((option & EnumAsInts)
                   && java_type->isFlags()
                   && java_type->indirections().isEmpty()
                   && (java_type->getReferenceType()!=MetaType::Reference || java_type->isConstant()))) {
        s << INDENT;
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            if (option & BoxedPrimitive) {
                s << "jobject " << java_name << " = ";
            }else if((option & EnumAsInts)){
                s << "jint " << java_name << " = ";
            }else{
                s << java_type->typeEntry()->jniName() << " " << java_name << " = ";
            }
        }
        if (option & BoxedPrimitive) {
            s << "QtJambiAPI::toJavaIntegerObject(" << __jni_env << ", " << qt_name << ")";
        } else {
            s << qt_name;
        }

    } else if (java_type->isIntegerEnum()
               || ((option & EnumAsInts)
                   && java_type->isEnum()
                   && java_type->indirections().isEmpty()
                   && (java_type->getReferenceType()!=MetaType::Reference || java_type->isConstant()))) {
        s << INDENT;
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            if (option & BoxedPrimitive) {
                s << "jobject " << java_name << " = ";
            }else{
                if((option & EnumAsInts)){
                    switch(reinterpret_cast<const EnumTypeEntry*>(java_type->typeEntry())->size()){
                    case 8:  s << "jbyte " << java_name << " = "; break;
                    case 16: s << "jshort " << java_name << " = "; break;
                    case 64: s << "jlong " << java_name << " = "; break;
                    default: s << "jint " << java_name << " = "; break;
                    }
                }else{
                    s << java_type->typeEntry()->jniName() << " " << java_name << " = ";
                }
            }
        }
        if (option & BoxedPrimitive) {
            s << "qtjambi_from_";
            switch(reinterpret_cast<const EnumTypeEntry*>(java_type->typeEntry())->size()){
            case 8:  s << "byte"; break;
            case 16: s << "short"; break;
            case 64: s << "long"; break;
            default: s << "int"; break;
            }
            s << "(" << __jni_env << ", " << qt_name << ")";
        } else {
            if(java_type->typeEntry()->isScopedEnum()){
                switch(reinterpret_cast<const EnumTypeEntry*>(java_type->typeEntry())->size()){
                case 8:  s << "jbyte"; break;
                case 16: s << "jshort"; break;
                case 64: s << "jlong"; break;
                default: s << "jint"; break;
                }
                s << "(" << qt_name << ")";
            }else{
                s << qt_name;
            }
        }

    } else {
        s << INDENT;
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            s << "jobject " << java_name << " = ";
        }
        if (java_type->isContainer()) {
            const ContainerTypeEntry *type = static_cast<const ContainerTypeEntry *>(java_type->typeEntry());
            if (type->type() == ContainerTypeEntry::ListContainer
                    || type->type() == ContainerTypeEntry::std_vector
                    || type->type() == ContainerTypeEntry::ByteArrayListContainer
                    || type->type() == ContainerTypeEntry::VectorContainer
                    || type->type() == ContainerTypeEntry::QTypedArrayDataContainer
                    || type->type() == ContainerTypeEntry::QArrayDataContainer
                    || type->type() == ContainerTypeEntry::StringListContainer
                    || type->type() == ContainerTypeEntry::LinkedListContainer
                    || type->type() == ContainerTypeEntry::StackContainer
                    || type->type() == ContainerTypeEntry::SetContainer
                    || type->type() == ContainerTypeEntry::QueueContainer
                ) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", ";
                if(!qtjambi_scope.isEmpty()
                        && !java_type->isConstant()
                        && java_type->getReferenceType()==MetaType::Reference){
                    if((option & OptionalScope) == OptionalScope)
                        s << "*";
                    s << qtjambi_scope << ", &";
                }
                s << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::MultiMapContainer
                       || type->type() == ContainerTypeEntry::MultiHashContainer
                       || type->type() == ContainerTypeEntry::MapContainer
                       || type->type() == ContainerTypeEntry::HashContainer) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", ";
                if(!qtjambi_scope.isEmpty()
                        && !java_type->isConstant()
                        && java_type->getReferenceType()==MetaType::Reference){
                    if((option & OptionalScope) == OptionalScope)
                        s << "*";
                    s << qtjambi_scope << ", &";
                }
                s << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::std_atomic
                       || type->type() == ContainerTypeEntry::std_optional
                       || type->type() == ContainerTypeEntry::PairContainer
                       || type->type() == ContainerTypeEntry::QDBusReplyContainer
                       || type->type() == ContainerTypeEntry::QQmlListPropertyContainer
                       || type->type() == ContainerTypeEntry::QPropertyBindingContainer
                       || type->type() == ContainerTypeEntry::QBindableContainer
                       || type->type() == ContainerTypeEntry::QModelRoleDataSpanContainer) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::std_chrono
                       || type->type() == ContainerTypeEntry::std_chrono_template){
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else {
                s << "0";
                ReportHandler::warning(QString("unable to generate container type %1, type=%2")
                                       .arg(java_type->name()).arg(type->type()));
            }
        } else if (java_type->isQVariant()) {
            if(!java_type->typeEntry()->isQVariant()){
                s << "QtJambiAPI::convertQVariantToJavaVariant(" << __jni_env << ", ";
                if(!java_type->isConstant() && java_type->indirections().isEmpty() && java_type->getReferenceType()==MetaType::Reference)
                    s << "&";
                s << qt_name << ")";
            }else{
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            }
        } else if (java_type->isFunctional()) {
            const FunctionalTypeEntry* funEntry = static_cast<const FunctionalTypeEntry*>(java_type->typeEntry());
            QString registeredName;
            if(funEntry->getUsing().isEmpty()){
                registeredName = funEntry->normalizedSignature();
            }else{
                registeredName = funEntry->name();
            }
            s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ", \"" << registeredName << "\")";
        } else if (java_type->isSmartPointer() && java_type->instantiations().size()==1) {
            s << "qtjambi_cast<jobject>(" << __jni_env << ", ";
            /*if(!qtjambi_scope.isEmpty() && argument_index>0){
                if((option & OptionalScope) == OptionalScope)
                    s << "*";
                s << qtjambi_scope << ", ";
            }*/
            s << qt_name << ")";
        }else if (java_type->isJObjectWrapper()
                   || java_type->isTargetLangEnum()
                   || java_type->isTargetLangFlags()
                   || java_type->typeEntry()->isQMetaObjectType()
                   || java_type->typeEntry()->isQMetaObjectConnectionType()) {
            s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
        } else if(java_type->typeEntry()->isComplex() && static_cast<const ComplexTypeEntry*>(java_type->typeEntry())->isGLsync()){
            s << "QtJambiAPI::convertNativeToJavaObjectAsWrapper(" << __jni_env << ", static_cast<const void *>(" << (java_type->getReferenceType()==MetaType::Reference ? "&" : "")
              << qt_name << "), Java::QtGui::GLsync::getClass(" << __jni_env << "))";
        } else if(java_type->typeEntry()->isComplex()
                  && static_cast<const ComplexTypeEntry*>(java_type->typeEntry())->isQModelIndex()
                  && java_type->getReferenceType()==MetaType::Reference
                  && argument_index > 0
                  && !(option & NoTmpVariable)){
            s << "QtJambiAPI::convertModelIndexToEphemeralJavaObject(" << __jni_env << ", ";
            if((option & OptionalScope) == OptionalScope)
                s << "*";
            s << qtjambi_scope;
            s << ", &" << qt_name << ")";
        } else if (java_type->isNativePointer()) {
            s << "QtJambiAPI::convertNativeToQNativePointer(" << __jni_env << ", ";
            if (java_type->getReferenceType()==MetaType::Reference)
                s << "&";
            s << qt_name << ", " << nativePointerTypeString(java_type);
            int actualIndirections = java_type->actualIndirections();
            if(actualIndirections==0 && java_type->typeEntry()->isAlias() && static_cast<const TS::AliasTypeEntry*>(java_type->typeEntry())->getHasIndirections()){
                ++actualIndirections;
            }
            s << ", 1, " << actualIndirections << ")";
        } else if (java_type->isIterator()) {
            const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(java_type->typeEntry());
            bool found = false;
            if(java_type->iteratorInstantiations().size()==2){
                writeQAssociativeIteratorFunction(s, java_type,
                                       java_type->iteratorInstantiations().at(0),
                                       java_type->iteratorInstantiations().at(1),
                                       qt_name, option, "", __jni_env, qtjambi_scope);
                found = true;
            }else if(java_type->iteratorInstantiations().size()==1){
                writeQSequentialIteratorFunction(s, java_type,
                                       java_type->iteratorInstantiations().at(0),
                                       qt_name, option, "", __jni_env, qtjambi_scope);
                found = true;
            }
            if(!found && java_function && java_function->declaringClass()){
                if(MetaClass * iteratorClass = java_function->declaringClass()->findIterator(iteratorType)){
                    if(iteratorClass->templateBaseClassInstantiations().size()==1){
                        QScopedPointer<MetaType> copy(java_type->copy());
                        copy->setConstant(false);
                        copy->setReferenceType(MetaType::NoReference);
                        copy->setIndirections(QList<bool>());
                        if(iteratorClass->enclosingClass())
                            copy->setTypeEntry(iteratorClass->enclosingClass()->typeEntry());
                        if(java_function->declaringClass()->templateBaseClass())
                            copy->setTypeEntry(java_function->declaringClass()->templateBaseClass()->typeEntry());
                        copy->setInstantiations(iteratorClass->templateBaseClassInstantiations());
                        copy->setArrayElementCounts({});
                        copy->setArrayElementType(nullptr);
                        copy->setForceBoxedPrimitives(true);
                        MetaBuilder::decideUsagePattern(copy.data());
                        writeQSequentialIteratorFunction(s, copy.data(), java_function->declaringClass()->templateBaseClassInstantiations().at(0), qt_name, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                        found = true;
                    }else{
                        for(const MetaFunction *function : iteratorClass->functions()){
                            if(function->operatorType()==OperatorType::Times && function->type() && function->arguments().isEmpty() && function->isConstant()){
                                writeQSequentialIteratorFunction(s, java_type, function->type(), qt_name, option, "", __jni_env, qtjambi_scope);
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(!found && iteratorType->containerType()){
                if(MetaClass * containerClass = m_classes.findClass(iteratorType->containerType()->qualifiedCppName())){
                    if(containerClass->templateBaseClass() && containerClass->templateBaseClass()->typeEntry()->isContainer()){
                        const ContainerTypeEntry* containerType = static_cast<const ContainerTypeEntry*>(containerClass->templateBaseClass()->typeEntry());
                        if(containerType->type()==ContainerTypeEntry::MapContainer
                            || containerType->type()==ContainerTypeEntry::MultiMapContainer
                            || containerType->type()==ContainerTypeEntry::HashContainer
                                || containerType->type()==ContainerTypeEntry::MultiHashContainer){
                            if(containerClass->templateBaseClassInstantiations().size()==2){
                                QScopedPointer<MetaType> copy(java_type->copy());
                                copy->setConstant(false);
                                copy->setReferenceType(MetaType::NoReference);
                                copy->setIndirections(QList<bool>());
                                copy->setTypeEntry(containerClass->templateBaseClass()->typeEntry());
                                copy->setInstantiations(containerClass->templateBaseClassInstantiations());
                                copy->setArrayElementCounts({});
                                copy->setArrayElementType(nullptr);
                                copy->setForceBoxedPrimitives(true);
                                MetaBuilder::decideUsagePattern(copy.data());
                                writeQAssociativeIteratorFunction(s, copy.data(),
                                                       containerClass->templateBaseClassInstantiations().at(0),
                                                       containerClass->templateBaseClassInstantiations().at(1),
                                                       qt_name, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                                found = true;
                            }
                        }else{
                            if(containerClass->templateBaseClassInstantiations().size()==1){
                                QScopedPointer<MetaType> copy(java_type->copy());
                                copy->setConstant(false);
                                copy->setReferenceType(MetaType::NoReference);
                                copy->setIndirections(QList<bool>());
                                copy->setTypeEntry(containerClass->templateBaseClass()->typeEntry());
                                copy->setInstantiations(containerClass->templateBaseClassInstantiations());
                                copy->setArrayElementCounts({});
                                copy->setArrayElementType(nullptr);
                                copy->setForceBoxedPrimitives(true);
                                MetaBuilder::decideUsagePattern(copy.data());
                                writeQSequentialIteratorFunction(s, copy.data(), containerClass->templateBaseClassInstantiations().at(0), qt_name, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                                found = true;
                            }
                        }
                    }
                }else{
                    if(java_function
                            && java_function->declaringClass()
                            && java_type->originalTypeDescription()==java_function->declaringClass()->baseClassName()+"::"+iteratorType->iteratorName()){
                        if(java_function->declaringClass()->templateBaseClassInstantiations().size()==1){
                            QScopedPointer<MetaType> copy(java_type->copy());
                            copy->setConstant(false);
                            copy->setReferenceType(MetaType::NoReference);
                            copy->setIndirections(QList<bool>());
                            if(java_function->declaringClass()->templateBaseClass())
                                copy->setTypeEntry(java_function->declaringClass()->templateBaseClass()->typeEntry());
                            copy->setInstantiations(java_function->declaringClass()->templateBaseClassInstantiations());
                            copy->setArrayElementCounts({});
                            copy->setArrayElementType(nullptr);
                            copy->setForceBoxedPrimitives(true);
                            MetaBuilder::decideUsagePattern(copy.data());
                            writeQSequentialIteratorFunction(s, copy.data(), java_function->declaringClass()->templateBaseClassInstantiations().at(0), qt_name, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                            found = true;
                        }
                    }
                }
            }
            if(!found){
                if(MetaClass * iteratorClass = m_classes.findClass(iteratorType->qualifiedCppName(), MetaClassList::QualifiedCppName)){
                    if(iteratorClass->typeAliasType()){
                        QScopedPointer<MetaType> typeAliasType(iteratorClass->typeAliasType()->copy());
                        if(typeAliasType->indirections().size()==1 && typeAliasType->getReferenceType()==MetaType::NoReference){
                            QList<bool> indirections = typeAliasType->indirections();
                            if(!indirections.isEmpty()){
                                indirections.takeFirst();
                                typeAliasType->setIndirections(indirections);
                            }
                            typeAliasType->setReferenceType(MetaType::Reference);
                            MetaBuilder::decideUsagePattern(typeAliasType.data());
                        }
                        writeQSequentialIteratorFunction(s, java_type, typeAliasType.data(), qt_name, option, "", __jni_env, qtjambi_scope);
                        found = true;
                    }else{
                        for(const MetaFunction *function : iteratorClass->functions()){
                            if(function->operatorType()==OperatorType::Times && function->type() && function->arguments().isEmpty() && function->isConstant()){
                                writeQSequentialIteratorFunction(s, java_type, function->type(), qt_name, option, "", __jni_env, qtjambi_scope);
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(!found){
                s << "nullptr /""*unable to create iterator*""/";
            }
        } else if (java_type->indirections().isEmpty() && java_type->getReferenceType()!=MetaType::NoReference && !java_type->isConstant()) {
            s << "qtjambi_cast<jobject>(" << __jni_env << ", &" << qt_name << ")";
        } else {
            bool isMetaTypeRegistered = false;
            if(java_type->typeEntry()->isInterface()){
                const ComplexTypeEntry* original = static_cast<const InterfaceTypeEntry*>(java_type->typeEntry())->origin();
                if(!original->skipMetaTypeRegistration()){
                    isMetaTypeRegistered = original->isValue()
                                                || ((original->hasPublicCopyConstructor()
                                                       || original->customConstructor(ComplexTypeEntry::CopyConstructor).hasCode())
                                                   && (original->hasPublicDefaultConstructor()
                                                       || original->customConstructor(ComplexTypeEntry::DefaultConstructor).hasCode())
                                                   && (original->isDestructorPublic()
                                                       || original->customDestructor().hasCode())
                                                    && !original->skipMetaTypeRegistration());
                }
            }else if(const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(java_type->typeEntry())){
                if(!ctype->skipMetaTypeRegistration()){
                    isMetaTypeRegistered = ctype->isValue()
                                                || ((ctype->hasPublicCopyConstructor()
                                                       || ctype->customConstructor(ComplexTypeEntry::CopyConstructor).hasCode())
                                                   && (ctype->hasPublicDefaultConstructor()
                                                       || ctype->customConstructor(ComplexTypeEntry::DefaultConstructor).hasCode())
                                                   && (ctype->isDestructorPublic()
                                                       || ctype->customDestructor().hasCode())
                                                    && !ctype->skipMetaTypeRegistration());
                }
            }
            if (java_type->indirections().isEmpty() && java_type->getReferenceType()!=MetaType::NoReference && java_type->isConstant() && !isMetaTypeRegistered) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", &" << qt_name << ")";
            }else{
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            }
        }
    }
    if(semiRequired){
        s << ";" << Qt::endl;
    }
    return false;
}

void CppImplGenerator::writeQSequentialIteratorFunction(QTextStream &s,
                              const MetaType *java_type,
                              const MetaType *elementType,
                              const QString& variable,
                              Option option,
                              const QString& iteratorName,
                              const QString& __jni_env,
                              const QString& qtjambi_scope)
{
    s << "qtjambi_cast_sequential_iterator(" << __jni_env << ", ";
    if(qtjambi_scope=="__list_nativeId"){
        s << "QtJambiScope(__list_nativeId)";
    }else{
        s << qtjambi_scope;
    }
    s << ", " << variable << ")";
    Q_UNUSED(java_type)
    Q_UNUSED(elementType)
    Q_UNUSED(iteratorName)
    Q_UNUSED(option)
}

void CppImplGenerator::writeQAssociativeIteratorFunction(QTextStream &s,
                              const MetaType *java_type,
                              const MetaType *keyType,
                              const MetaType *valueType,
                              const QString& variable,
                              Option option,
                              const QString& iteratorName,
                              const QString& __jni_env,
                              const QString& qtjambi_scope)
{
    s << "qtjambi_cast_associative_iterator(" << __jni_env << ", ";
    if(qtjambi_scope=="__list_nativeId"){
        s << "QtJambiScope(__list_nativeId)";
    }else{
        s << qtjambi_scope;
    }
    s << ", " << variable << ")";
    Q_UNUSED(java_type)
    Q_UNUSED(keyType)
    Q_UNUSED(valueType)
    Q_UNUSED(iteratorName)
    Q_UNUSED(option)
}

void CppImplGenerator::writeJavaToQtContainer(QTextStream &s,
        const MetaType *java_type,
        const QString &qt_name,
        const QString &java_name,
        const MetaFunction *java_function,
        const MetaFunctional *java_functional,
        int argument_index,
        const QString& __jni_env,
        const QString& qtjambi_scope,
        Option option) {
    Q_UNUSED(qtjambi_scope)
    // Conversion to C++: Shell code for return value, native code for arguments
    TS::Language lang = argument_index == 0 ? TS::ShellCode : TS::NativeCode;
    //if(writeConversionRule(s, lang, java_function, java_functional, argument_index, qt_name, java_name, option, __jni_env, qtjambi_scope))
    //    return;

    if (!java_type) {
        QString functionName;
        if(java_function){
            functionName = java_function->implementingClass()->name()+"::"+java_function->name();
        }else if(java_functional){
            if(java_functional->enclosingClass())
                functionName = java_functional->enclosingClass()->name();
            functionName += java_functional->name();
        }else{
            functionName = "unknown";
        }
        QString warn = QString("no conversion possible for argument '%1' in function '%2' for "
                               "language '%3'")
                       .arg(argument_index)
                       .arg(functionName)
                       .arg(int(lang));
        ReportHandler::warning(warn);
        return;
    }
    QString shared_pointer = "__qt_scoped_pointer_0x"+QString::number(QRandomGenerator().generate(), 16);

    /*bool needLambda = false;
    if((option & DirectReturn) == DirectReturn){
        needLambda = true;
    }else if((option & NoTmpVariable) == NoTmpVariable){
        if(java_name.isEmpty())
            needLambda = true;
    }*/

    Q_ASSERT(java_type->isContainer());
    const ContainerTypeEntry *type =
        static_cast<const ContainerTypeEntry *>(java_type->typeEntry());

    QScopedPointer<MetaType> scopedPointer;
    const MetaType *container_type = java_type;
    if(java_type->indirections().size()==1){
        scopedPointer.reset(container_type->copy());
        container_type = scopedPointer.data();
        scopedPointer->setIndirections(QList<bool>());
        scopedPointer->setReferenceType(MetaType::Reference);
    }

    if (type->type() == ContainerTypeEntry::ListContainer
            || type->type() == ContainerTypeEntry::ByteArrayListContainer
            || type->type() == ContainerTypeEntry::VectorContainer
            || type->type() == ContainerTypeEntry::StackContainer
            || type->type() == ContainerTypeEntry::StringListContainer
            || type->type() == ContainerTypeEntry::LinkedListContainer
            || type->type() == ContainerTypeEntry::StackContainer
            || type->type() == ContainerTypeEntry::SetContainer
            || type->type() == ContainerTypeEntry::QueueContainer
        ) {
        // the QArray type can have two template arguments
        const MetaType *targ;
        QScopedPointer<MetaType> instantiation;
        if(type->type() == ContainerTypeEntry::ByteArrayListContainer
                 || type->type() == ContainerTypeEntry::StringListContainer){
            Q_ASSERT_X(!java_type->hasInstantiationInCpp() || java_type->instantiations().size() == 0, qPrintable(java_type->cppSignature()), qPrintable(QString("instantiations: %1, [0] = %2").arg(java_type->instantiations().size()).arg(java_type->instantiations().at(0)->typeEntry()->qualifiedCppName())));
            instantiation.reset(java_type->copy());
            targ = instantiation.data();
            instantiation->setReferenceType(MetaType::NoReference);
            instantiation->setConstant(false);
            instantiation->setIndirections(QList<bool>());
            instantiation->setArrayElementCounts({});
            instantiation->setArrayElementType(nullptr);
            instantiation->setForceBoxedPrimitives(false);
            instantiation->setInstantiationInCpp(false);
            instantiation->setInstantiations(QList<const MetaType *>());
            instantiation->setOriginalTemplateType(nullptr);
            switch (type->type()) {
            case ContainerTypeEntry::ByteArrayListContainer:
                {
                    ComplexTypeEntry * qByteArrayTypeEntry = database()->findComplexType("QByteArray");
                    Q_ASSERT(qByteArrayTypeEntry);
                    instantiation->setTypeEntry(qByteArrayTypeEntry);
                    instantiation->setOriginalTypeDescription("QByteArray");
                    instantiation->setTypeUsagePattern(MetaType::ValuePattern);
                }
                break;
            case ContainerTypeEntry::StringListContainer:
                {
                    ComplexTypeEntry * qStringTypeEntry = database()->findComplexType("QString");
                    Q_ASSERT(qStringTypeEntry);
                    instantiation->setTypeEntry(qStringTypeEntry);
                    instantiation->setOriginalTypeDescription("QString");
                    instantiation->setTypeUsagePattern(MetaType::QStringPattern);
                }
                break;
            default:
                break;
            }
        }else{
            Q_ASSERT(java_type->instantiations().size() == 1);
            targ = java_type->instantiations().first();
        }

        bool needScope = (java_type->indirections().size()==1
                          || java_type->getReferenceType()==MetaType::Reference)
                         && ((option & OptionalScope) == 0);
        bool needIntermadiate = needScope && !java_type->isConstant();
        QString internediateName;
        if(needIntermadiate){
            internediateName = QString::number(qHash(java_type), 16);
            s << Qt::endl
              << INDENT << "class IntermediateContainer_" << internediateName << " : public ";
            writeTypeInfo(s, container_type, ForceValueType);
            s << "{" << Qt::endl;
            s << INDENT << "public:" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "IntermediateContainer_" << internediateName << "(JNIEnv *env, jobject object)" << Qt::endl;
                s << INDENT << "      : ";
                writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
                s << "()," << Qt::endl;
                s << INDENT << "        m_env(env), " << Qt::endl;
                s << INDENT << "        m_object(object){" << Qt::endl;
                s << INDENT << "}" << Qt::endl << Qt::endl;
                s << INDENT << "virtual ~IntermediateContainer_" << internediateName << "(){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "QtJambiAPI::clearJavaCollection(m_env, m_object);" << Qt::endl;
                    s << INDENT << "for(";
                    writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
                    s << "::const_iterator i = constBegin(); i!=constEnd(); i++){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        writeQtToJava(s, targ, "*i", "val", nullptr, nullptr, -1, Option(BoxedPrimitive), "m_env", "");
                        s << INDENT << "QtJambiAPI::addToJavaCollection(m_env, m_object, val);" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
                s << INDENT << "JNIEnv *m_env;" << Qt::endl;
                s << INDENT << "jobject m_object;" << Qt::endl;
            }
            s << INDENT << "};" << Qt::endl << Qt::endl;
        }
        QString qt_pointer = qt_name;
        if(needScope){
            s << INDENT << "QScopedPointer<";
            if(needIntermadiate){
                s << "IntermediateContainer_" << internediateName;
            }else {
                writeTypeInfo(s, container_type, ForceValueType);
            }
            if(java_type->indirections().isEmpty()){
                qt_pointer = qt_name + "_pointer";
            }
            s << "> " << shared_pointer << ";" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
            s << "* " << qt_pointer << " = nullptr;" << Qt::endl;
        }else if((option & OptionalScope) == 0){
            s << INDENT;
            writeTypeInfo(s, container_type, ForceValueType);
            s << qt_name << ";" << Qt::endl;
        }

        s << INDENT << "if (" << java_name << ") {" << Qt::endl;
        {
            INDENTATION(INDENT)
            {
                QScopedPointer<Indentation> specialIndentation;
                if(type->type() == ContainerTypeEntry::VectorContainer
                        || type->type() == ContainerTypeEntry::StackContainer
                        || type->type() == ContainerTypeEntry::ListContainer
                        || type->type() == ContainerTypeEntry::LinkedListContainer
                        || type->type() == ContainerTypeEntry::ByteArrayListContainer
                        || type->type() == ContainerTypeEntry::StringListContainer
                        || type->type() == ContainerTypeEntry::SetContainer
                        || type->type() == ContainerTypeEntry::QueueContainer){
                    s << INDENT;
                    writeTypeInfo(s, container_type, ForceValueType);
                    s << "* __tmp_pointer_" << qt_name << "{nullptr};" << Qt::endl
                      << INDENT << "if (ContainerAPI::getAsQ";
                    switch(type->type()){
                    case ContainerTypeEntry::VectorContainer:
                        if(database()->qtVersion() < QVersionNumber(6,0,0)){
                            s << "Vector";
                        }else{
                            s << "List";
                        }
                        break;
                    case ContainerTypeEntry::StackContainer: s << "Stack"; break;
                    case ContainerTypeEntry::LinkedListContainer: s << "LinkedList"; break;
                    case ContainerTypeEntry::ByteArrayListContainer:
                    case ContainerTypeEntry::StringListContainer:
                    case ContainerTypeEntry::ListContainer:s << "List"; break;
                    case ContainerTypeEntry::SetContainer: s << "Set"; break;
                    default: s << "Queue"; break;
                    }
                    s << "<";
                    writeTypeInfo(s, targ, Option(ForceValueType | SkipName));
                    s << ">(" << __jni_env << ", " << java_name << ", __tmp_pointer_" << qt_name << ")) {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT << "*reinterpret_cast<";
                            writeTypeInfo(s, container_type, ForceValueType);
                            s << "*>(" << qt_name << ") = *__tmp_pointer_" << qt_name << ";" << Qt::endl;
                        }
                        s << INDENT << "}else{" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT << qt_name << " = " << "__tmp_pointer_" << qt_name << ";" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                    }
                    s << INDENT << "} else {" << Qt::endl;
                    specialIndentation.reset(new Indentation(INDENT));
                }
                if(needScope){
                    s << INDENT << shared_pointer << ".reset(new ";
                    if(needIntermadiate){
                        s << "IntermediateContainer_" << internediateName << "(" << __jni_env << ", " << java_name << ")";
                    }else{
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "()";
                    }
                    s << ");" << Qt::endl;
                    s << INDENT << qt_pointer << " = " << shared_pointer << ".data();" << Qt::endl;
                }
                s << INDENT << "jobject __qt__iterator = QtJambiAPI::iteratorOfJavaIterable(" << __jni_env << ", "
                  << java_name << ");" << Qt::endl;
                if((option & OptionalScope) == OptionalScope){
                    s << INDENT << "if(!" << qt_name << ") " << Qt::endl
                      << INDENT << "    return false;" << Qt::endl;
                }

                if (type->type() == ContainerTypeEntry::VectorContainer
                        || type->type() == ContainerTypeEntry::StackContainer){
                    s << INDENT;
                    if(needScope){
                        s << qt_pointer << "->";
                    }else if((option & OptionalScope) == OptionalScope){
                        s << "reinterpret_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "*>(" << qt_name << ")->";
                    }else{
                        s << qt_name << ".";
                    }
                    s << "reserve(QtJambiAPI::sizeOfJavaCollection(" << __jni_env << ", " << java_name << "));" << Qt::endl;
                }

                s << INDENT << "while(QtJambiAPI::hasJavaIteratorNext(" << __jni_env << ", __qt__iterator)) {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT <<
                    "jobject __java_element = QtJambiAPI::nextOfJavaIterator(" << __jni_env << ", __qt__iterator);"
                      << Qt::endl;
                    QString element_name = "__qt_element_"+qt_name;
                    writeJavaToQt(s, targ, element_name, "__java_element", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
                    s << INDENT;
                    if(needScope){
                        s  << qt_pointer << "->";
                    }else if((option & OptionalScope) == OptionalScope){
                        s << "reinterpret_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "*>(" << qt_name << ")->";
                    }else{
                        s << qt_name  << ".";
                    }
                    switch(type->type()){
                    case ContainerTypeEntry::SetContainer: s << "insert"; break;
                    default: s << "append"; break;
                    }
                    s  << "(" << element_name << ");" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }
            if(type->type() == ContainerTypeEntry::VectorContainer
                    || type->type() == ContainerTypeEntry::StackContainer
                    || type->type() == ContainerTypeEntry::ListContainer
                    || type->type() == ContainerTypeEntry::LinkedListContainer
                    || type->type() == ContainerTypeEntry::ByteArrayListContainer
                    || type->type() == ContainerTypeEntry::StringListContainer
                    || type->type() == ContainerTypeEntry::SetContainer
                    || type->type() == ContainerTypeEntry::QueueContainer){
                s << INDENT << "}" << Qt::endl;
            }
        }
        if(needScope){
            s << INDENT << "}else{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << shared_pointer << ".reset(new ";
                if(needIntermadiate){
                    s << "IntermediateContainer_" << internediateName << "(" << __jni_env << ", " << java_name << ")";
                }else{
                    writeTypeInfo(s, container_type, ForceValueType);
                    s << "()";
                }
                s << ");" << Qt::endl;
                s << INDENT << qt_pointer << " = " << shared_pointer << ".data();" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
        if(qt_pointer != qt_name){
            s << INDENT;
            if((option & NoTmpVariable) == 0)
                writeTypeInfo(s, container_type, Option());
            s << qt_name << " = *" <<qt_name << "_pointer;" << Qt::endl;
        }
    } else if (type->type() == ContainerTypeEntry::PairContainer) {
        const QList<const MetaType *>& targs = java_type->instantiations();
        Q_ASSERT(targs.size() == 2);

        if((option & NoTmpVariable) == 0 && (option & OptionalScope) == 0){
            s << INDENT;
            writeTypeInfo(s, java_type, ForceValueType);
            s << " " << qt_name << ";" << Qt::endl;
        }
        s << INDENT << "if (" << java_name << ") {" << Qt::endl;
        {
            // separate scope required just in case function takes two QPair's.
            INDENTATION(INDENT)
            if((option & OptionalScope) == OptionalScope){
                s << INDENT << "if(!" << qt_name << ") " << Qt::endl
                  << INDENT << "    return false;" << Qt::endl;
            }
            s << INDENT << "jobject __java_first = QtJambiAPI::getQPairFirst(" << __jni_env << ", " << java_name << ");" << Qt::endl;
            writeJavaToQt(s, targs.at(0), "__qt_first", "__java_first", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);

            s << INDENT << "jobject __java_second = QtJambiAPI::getQPairSecond(" << __jni_env << ", " << java_name << ");" << Qt::endl;
            writeJavaToQt(s, targs.at(1), "__qt_second", "__java_second", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);

            if((option & OptionalScope) == OptionalScope){
                s << INDENT << "reinterpret_cast<";
                writeTypeInfo(s, java_type, ForceValueType);
                s << "*>(" << qt_name << ")->first = __qt_first;" << Qt::endl
                  << INDENT << "reinterpret_cast<";
                writeTypeInfo(s, java_type, ForceValueType);
                s << "*>(" << qt_name << ")->second = __qt_second;" << Qt::endl;
            }else{
                s << INDENT << qt_name << ".first = __qt_first;" << Qt::endl
                  << INDENT << qt_name << ".second = __qt_second;" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
    } else if (type->type() == ContainerTypeEntry::MapContainer
               || type->type() == ContainerTypeEntry::HashContainer
               || type->type() == ContainerTypeEntry::MultiMapContainer
               || type->type() == ContainerTypeEntry::MultiHashContainer) {
        Q_ASSERT(java_type->instantiations().size() == 2);
        const MetaType *targ_key = java_type->instantiations().at(0);
        const MetaType *targ_val = java_type->instantiations().at(1);

        bool needScope = (java_type->indirections().size()==1
                          || java_type->getReferenceType()==MetaType::Reference)
                         && ((option & OptionalScope) == 0);
        bool needIntermadiate = needScope && !java_type->isConstant();
        QString internediateName;
        if(needIntermadiate){
            internediateName = QString::number(qHash(java_type), 16);
            s << Qt::endl << INDENT << "class IntermediateContainer_" << internediateName << " : public ";
            writeTypeInfo(s, container_type, ForceValueType);
            s << "{" << Qt::endl;
            s << INDENT << "public:" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "IntermediateContainer_" << internediateName << "(JNIEnv *env, jobject object)" << Qt::endl;
                s << INDENT << "      : ";
                writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
                s << "()," << Qt::endl;
                s << INDENT << "        m_env(env)," << Qt::endl;
                s << INDENT << "        m_object(object){" << Qt::endl;
                s << INDENT << "}" << Qt::endl << Qt::endl;
                s << INDENT << "virtual ~IntermediateContainer_" << java_name << "(){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "QtJambiAPI::clearJavaMap(m_env, m_object);" << Qt::endl;
                    s << INDENT << "for(";
                    writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
                    s << "::const_iterator i = constBegin(); i!=constEnd(); i++){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        writeQtToJava(s, targ_key, "i.key()", "key", nullptr, nullptr, -1, Option(BoxedPrimitive), "m_env", "");
                        writeQtToJava(s, targ_val, "i.value()", "val", nullptr, nullptr, -1, Option(BoxedPrimitive), "m_env", "");
                        s << INDENT << "QtJambiAPI::putJavaMap(m_env, m_object, key, val);" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
                s << INDENT << "JNIEnv *m_env;" << Qt::endl;
                s << INDENT << "jobject m_object;" << Qt::endl;
            }
            s << INDENT << "};" << Qt::endl << Qt::endl;
        }
        QString qt_pointer = qt_name;
        if(needScope){
            s << INDENT << "QScopedPointer<";
            if(needIntermadiate){
                s << "IntermediateContainer_" << internediateName;
            }else{
                writeTypeInfo(s, container_type, ForceValueType);
            }
            if(java_type->indirections().isEmpty()){
                qt_pointer = qt_name + "_pointer";
            }
            s << "> " << shared_pointer << ";" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, container_type, ForceValueType);
            s << "*" << qt_pointer << " = nullptr;" << Qt::endl;
        }else if((option & OptionalScope) == 0){
            s << INDENT;
            writeTypeInfo(s, container_type, ForceValueType);
            s << qt_name << ";" << Qt::endl;
        }
        s << INDENT << "if (" << java_name << ") {" << Qt::endl;
        {
            INDENTATION(INDENT)

            s << INDENT;
            writeTypeInfo(s, container_type, ForceValueType);
            s << "* __tmp_pointer_" << qt_name << "{nullptr};" << Qt::endl
              << INDENT << "if (ContainerAPI::getAsQ";
            switch(type->type()){
            case ContainerTypeEntry::MapContainer: s << "Map"; break;
            case ContainerTypeEntry::MultiMapContainer: s << "MultiMap"; break;
            case ContainerTypeEntry::MultiHashContainer: s << "MultiHash"; break;
            default: s << "Hash"; break;
            }
            s << "<";
            writeTypeInfo(s, targ_key, Option(ForceValueType | SkipName));
            s << ",";
            writeTypeInfo(s, targ_val, Option(ForceValueType | SkipName));
            s << ">(" << __jni_env << ", " << java_name << ", __tmp_pointer_" << qt_name << ")) {" << Qt::endl;
            if((option & OptionalScope) == OptionalScope){
                INDENTATION(INDENT)
                s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "*reinterpret_cast<";
                    writeTypeInfo(s, container_type, ForceValueType);
                    s << "*>(" << qt_name << ") = *__tmp_pointer_" << qt_name << ";" << Qt::endl;
                }
                s << INDENT << "}else{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << qt_name << " = __tmp_pointer_" << qt_name << ";" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }else{
                INDENTATION(INDENT)
                s << INDENT << qt_name << " = qtjambi_cast<";
                writeTypeInfo(s, container_type, ForceValueType);
                s << ">(" << __jni_env << ", " << java_name << ");" << Qt::endl;
            }

            s << INDENT << "} else {" << Qt::endl;
            {
                INDENTATION(INDENT)
                if(needScope){
                    s << INDENT << shared_pointer << ".reset(new ";
                    if(needIntermadiate){
                        s << "IntermediateContainer_" << internediateName << "(" << __jni_env << ", " << java_name << ")";
                    }else{
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "()";
                    }
                    s << ");" << Qt::endl;
                    s << INDENT << qt_pointer << " = " << shared_pointer << ".data();" << Qt::endl;
                }
                if((option & OptionalScope) == OptionalScope){
                    s << INDENT << "if(!" << qt_name << ") " << Qt::endl
                      << INDENT << "    return false;" << Qt::endl;
                }
                s << INDENT
                  << "jobject __java_entry_set_iterator = QtJambiAPI::entrySetIteratorOfJavaMap(" << __jni_env << ", " << java_name
                  << ");" << Qt::endl;

                s << INDENT << "while(QtJambiAPI::hasJavaIteratorNext(" << __jni_env << ", __java_entry_set_iterator)) {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT
                      << "jobject __java_entry = "
                      << "QtJambiAPI::nextOfJavaIterator(" << __jni_env << ", __java_entry_set_iterator);"
                      << Qt::endl
                      << INDENT << "jobject __java_key = QtJambiAPI::keyOfJavaMapEntry(" << __jni_env << ", __java_entry);" << Qt::endl
                      << INDENT << "jobject __java_val = QtJambiAPI::valueOfJavaMapEntry(" << __jni_env << ", __java_entry);" << Qt::endl;
                    writeJavaToQt(s, targ_key, "__qt_"+qt_name+"_key", "__java_key", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
                    writeJavaToQt(s, targ_val, "__qt_"+qt_name+"_val", "__java_val", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
                    s << INDENT;
                    if(needScope){
                        s << qt_pointer << "->";
                    }else if((option & OptionalScope) == OptionalScope){
                        s << "reinterpret_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "*>(" << qt_name << ")->";
                    }else{
                        s << qt_name << ".";
                    }
                    s << "insert(__qt_" << qt_name << "_key, __qt_" << qt_name << "_val);" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
        if(needScope){
            s << INDENT << "}else{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << shared_pointer << ".reset(new ";
                if(needIntermadiate){
                    s << "IntermediateContainer_" << internediateName << "(" << __jni_env << ", " << java_name << ")";
                }else{
                    writeTypeInfo(s, container_type, ForceValueType);
                    s << "()";
                }
                s << ");" << Qt::endl;
                s << INDENT << qt_pointer << " = " << shared_pointer << ".data();" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
        if(qt_pointer != qt_name){
            s << INDENT;
            if((option & NoTmpVariable) == 0)
                writeTypeInfo(s, container_type, Option());
            s << qt_name << " = *" << qt_pointer << ";" << Qt::endl;
        }

    } else {
        ReportHandler::warning(QString("unable to generate container type %1, %2")
                               .arg(java_type->name()).arg(type->type()));
    }
}

void CppImplGenerator::writeJavaToQtInitializerList(QTextStream &s,
        const MetaType *java_type,
        const QString &qt_name,
        const QString &java_name,
        const MetaFunction *java_function,
        const MetaFunctional *java_functional,
        int argument_index,
        const QString& __jni_env,
        const QString& qtjambi_scope,
        Option option) {
    Q_UNUSED(qtjambi_scope)
    // Conversion to C++: Shell code for return value, native code for arguments
    TS::Language lang = argument_index == 0 ? TS::ShellCode : TS::NativeCode;
    //if(writeConversionRule(s, lang, java_function, java_functional, argument_index, qt_name, java_name, option, __jni_env, qtjambi_scope))
    //    return;

    if (!java_type) {
        QString functionName;
        if(java_function){
            functionName = java_function->implementingClass()->qualifiedCppName()+"::"+java_function->name();
        }else if(java_functional){
            if(java_functional->enclosingClass())
                functionName = java_functional->enclosingClass()->name();
            functionName += java_functional->name();
        }else{
            functionName = "unknown";
        }
        QString warn = QString("no conversion possible for argument '%1' in function '%2' for "
                               "language '%3'")
                       .arg(argument_index)
                       .arg(functionName)
                       .arg(int(lang));
        ReportHandler::warning(warn);
        return;
    }

    Q_ASSERT(java_type->isInitializerList());
    Q_ASSERT(java_type->instantiations().size() == 1);

    const MetaType *targ = java_type->instantiations().first();

    QString vectorName = "tmp_list_" + QString::number(qHash(java_name), 16);
    s << INDENT << "QVector<";
    writeTypeInfo(s, targ, ForceValueType);
    s << "> " << vectorName << ";" << Qt::endl;

    s << INDENT << "if (" << java_name << "!= nullptr) {" << Qt::endl;
    {
        INDENTATION(INDENT)

        s << INDENT << "jsize __qt__size = " << __jni_env << "->GetArrayLength(jarray(" << java_name << "));" << Qt::endl;
        s << INDENT << "JavaException::check(" << __jni_env << " QTJAMBI_STACKTRACEINFO );" << Qt::endl;

        s << INDENT << "if(__qt__size>0){" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << vectorName << ".reserve(__qt__size);" << Qt::endl;

            if(targ->isPrimitive() || targ->isPrimitiveChar()){
                s << INDENT << "jboolean isCopy = false;" << Qt::endl;
                s << INDENT << targ->typeEntry()->jniName() << "* __native_array = " << __jni_env;
                s << "->Get" << jniTypeName(targ->typeEntry()->jniName()) << "ArrayElements(" << targ->typeEntry()->jniName() << "Array(" << java_name << "), &isCopy);" << Qt::endl;
                s << INDENT << "JavaException::check(" << __jni_env << " QTJAMBI_STACKTRACEINFO );" << Qt::endl;
            }
            s << INDENT << "for (int i=0; i<__qt__size; ++i) {" << Qt::endl;
            {
                INDENTATION(INDENT)
                QString element_name = (qt_name == "__qt_element") ? "__qt_element2" : "__qt_element";
                if(targ->isPrimitive()){
                    s << INDENT << vectorName << " << static_cast<" << targ->typeEntry()->qualifiedCppName() << ">(__native_array[i]);" << Qt::endl;
                }else if(targ->isPrimitiveChar()){
                    s << INDENT << vectorName << " << " << targ->typeEntry()->qualifiedCppName() << "(__native_array[i]);" << Qt::endl;
                }else{
                    s << INDENT << "jobject __java_element = " << __jni_env << "->GetObjectArrayElement(jobjectArray(" << java_name << "), i);" << Qt::endl;
                    s << INDENT << "JavaException::check(" << __jni_env << " QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                    writeJavaToQt(s, targ, element_name, "__java_element", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
                    s << INDENT << vectorName << " << " << element_name << ";" << Qt::endl;
                }
            }
            s << INDENT << "}" << Qt::endl;
            if(targ->isPrimitive() || targ->isPrimitiveChar()){
                s << INDENT << __jni_env << "->Release" << jniTypeName(targ->typeEntry()->jniName()) << "ArrayElements(" << targ->typeEntry()->jniName() << "Array(" << java_name << "), __native_array, JNI_ABORT);" << Qt::endl;
            }
        }

        s << INDENT << "}" << Qt::endl;
    }

    s << INDENT << "}" << Qt::endl;

    if((option & NoTmpVariable) == 0){
        s << INDENT;
        writeTypeInfo(s, java_type, ForceValueType);
        s << qt_name << ";" << Qt::endl;
    }
    s << INDENT << "if (!" << vectorName << ".isEmpty()) {" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << qt_name << " = QtJambiAPI::createIterable<std::initializer_list<";
        writeTypeInfo(s, targ, ForceValueType);
        s << ">>(" << vectorName << ".begin(), " << vectorName << ".size());" << Qt::endl;
    }
    s << INDENT << "}" << Qt::endl;
}

void CppImplGenerator::writeFunctionCall(QTextStream &s, const QString &object_name,
        const MetaFunction *java_function,
        QString prefix,
        Option option,
        const QStringList &extra_arguments) {
    if(java_function->isBaseClassDelegateFunction() && java_function->wasPublic()){
        s << object_name;
        return;
    }
    QString proxyCall;
    QString function_name = option & OriginalName ? java_function->originalName() : java_function->name();
    if(option & JNIProxyFunction){
        function_name += marshalledArguments(java_function);
    }else{
        proxyCall = java_function->proxyCall();
    }

    QStringList templateParameters;
    if(!proxyCall.isEmpty()){
        s << proxyCall << "(";
        writeFunctionCallArguments(s, java_function, "__qt_", Option(option | JNIProxyFunction));
        s << ")";
    }else{
        for(MetaTemplateParameter* param : java_function->templateParameters()){
            if(!param->isImplicit()){
                if(!param->instantiation().isEmpty()){
                    if(param->instantiationType()){
                        QString str;
                        QTextStream _s(&str);
                        writeTypeInfo(_s, param->instantiationType());
                        if(!str.trimmed().isEmpty())
                            templateParameters << str;
                    }else
                        templateParameters << param->instantiation();
                }else if(param->type()){
                    if(param->defaultType().isEmpty()){
                        QString str;
                        QTextStream _s(&str);
                        writeTypeInfo(_s, param->type());
                        if(!str.trimmed().isEmpty())
                            templateParameters << str;
                    }
                }else if(param->defaultType().isEmpty()){
                    templateParameters.clear();
                    break;
                }
            }
        }

        //AbstractMetaClassList interfaces = java_function->implementingClass()->interfaces();

        if (prefix.isEmpty()
                && !(option & JNIProxyFunction)
                && !java_function->isInGlobalScope()
                && !java_function->isStatic()
                && !java_function->isAbstract()
                && !java_function->implementingClass()->typeEntry()->isNativeInterface()
                && !java_function->implementingClass()->interfaces().isEmpty()
                && !java_function->implementingClass()->inheritsFrom(java_function->declaringClass())) {
            prefix = java_function->declaringClass()->qualifiedCppName() + "::";
        }

        // Global scope stream operators need the arguments to be reordered (this ref at end)
        // so we special case them in order to simplify this code
        if (!java_function->type()
                && java_function->arguments().size()==1
                && ((java_function->operatorType()==OperatorType::ShiftLeft && java_function->name()==QLatin1String("writeTo"))
                || (java_function->operatorType()==OperatorType::ShiftRight && java_function->name()==QLatin1String("readFrom")))) {
            writeFunctionCallArguments(s, java_function, "__qt_", option);
            if(java_function->operatorType()==OperatorType::ShiftLeft)
                s << " << (*" << object_name << ")";
            else
                s << " >> (*" << object_name << ")";
        } else {
            bool operatorCall = false;
            bool additionalClosingPar = false;
            bool additionalClosingBracket = false;
            if(prefix.isEmpty()
                    && !java_function->isDeclExplicit()
                    && !java_function->isStatic()){
                operatorCall = true;
                switch(java_function->operatorType()){
                case OperatorType::Plus:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        s << "+(*" << object_name << ")";
                    else if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") + ";
                    else operatorCall = true;
                    break;
                case OperatorType::Minus:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        s << "-(*" << object_name << ")";
                    else if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") - ";
                    else operatorCall = true;
                    break;
                case OperatorType::Times:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        s << "*(*" << object_name << ")";
                    else if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") * ";
                    else operatorCall = true;
                    break;
                case OperatorType::Div:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") / ";
                    else operatorCall = true;
                    break;
                case OperatorType::Rem:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") % ";
                    else operatorCall = true;
                    break;
                case OperatorType::And:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        s << "&(*" << object_name << ")";
                    else if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") & ";
                    else operatorCall = true;
                    break;
                case OperatorType::Or:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") | ";
                    else operatorCall = true;
                    break;
                case OperatorType::Xor:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") ^ ";
                    else operatorCall = true;
                    break;
                case OperatorType::Not:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        s << "~(*" << object_name << ")";
                    else if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") ~ ";
                    else operatorCall = true;
                    break;
                case OperatorType::ShiftLeft:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") << ";
                    else operatorCall = true;
                    break;
                case OperatorType::ShiftRight:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") >> ";
                    else operatorCall = true;
                    break;

                // assigments
                case OperatorType::Assign:
                    if(java_function->arguments().size()==1){
                        if(java_function->type()){
                            s << "(";
                            additionalClosingPar = true;
                        }
                        s << "(*" << object_name << ") = ";
                    }else operatorCall = true;
                    break;
                case OperatorType::PlusAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") += ";
                    else operatorCall = true;
                    break;
                case OperatorType::MinusAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") -= ";
                    else operatorCall = true;
                    break;
                case OperatorType::TimesAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") *= ";
                    else operatorCall = true;
                    break;
                case OperatorType::DivAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") /= ";
                    else operatorCall = true;
                    break;
                case OperatorType::RemAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") %= ";
                    else operatorCall = true;
                    break;
                case OperatorType::AndAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") &= ";
                    else operatorCall = true;
                    break;
                case OperatorType::OrAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") |= ";
                    else operatorCall = true;
                    break;
                case OperatorType::XorAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") ^= ";
                    else operatorCall = true;
                    break;
                case OperatorType::ShiftLeftAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") <<= ";
                    else operatorCall = true;
                    break;
                case OperatorType::ShiftRightAssign:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") >>= ";
                    else operatorCall = true;
                    break;

                // Logical
                case OperatorType::ExclusiveAnd:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") && ";
                    else operatorCall = true;
                    break;
                case OperatorType::ExclusiveOr:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") || ";
                    else operatorCall = true;
                    break;
                case OperatorType::Negation:
                    if(java_function->arguments().size()==0)
                        s << "!(*" << object_name << ")";
                    else operatorCall = true;
                    break;

                // incr/decr
                case OperatorType::Inc:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        if(java_function->type() && java_function->type()->getReferenceType()==MetaType::NoReference){
                            s << "*" << object_name << ";" << Qt::endl
                              << INDENT << "++__qt_return_value";
                        }else{
                            s << "++(*" << object_name << ")";
                        }
                    else if((extra_arguments.size() + java_function->arguments().size())==1){
                        s << "(*" << object_name << ")++";
                        return;
                    }
                    else operatorCall = true;
                    break;
                case OperatorType::Dec:
                    if((extra_arguments.size() + java_function->arguments().size())==0)
                        if(java_function->type() && java_function->type()->getReferenceType()==MetaType::NoReference){
                            s << "*" << object_name << ";" << Qt::endl
                              << INDENT << "--__qt_return_value";
                        }else{
                            s << "--(*" << object_name << ")";
                        }
                    else if((extra_arguments.size() + java_function->arguments().size())==1){
                        s << "(*" << object_name << ")--";
                        return;
                    }
                    else operatorCall = true;
                    break;

                // compare
                case OperatorType::Less:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") < ";
                    else operatorCall = true;
                    break;
                case OperatorType::Greater:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") > ";
                    else operatorCall = true;
                    break;
                case OperatorType::LessOrEquals:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") <= ";
                    else operatorCall = true;
                    break;
                case OperatorType::Compare:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") <=> ";
                    else operatorCall = true;
                    break;
                case OperatorType::GreaterOrEquals:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") >= ";
                    else operatorCall = true;
                    break;
                case OperatorType::NotEquals:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") != ";
                    else operatorCall = true;
                    break;
                case OperatorType::Equals:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") == ";
                    else operatorCall = true;
                    break;

                // other
                case OperatorType::Subscript:
                    s << "(*" << object_name << ")[";
                    additionalClosingBracket = true;
                    break;
                case OperatorType::PointerToMemberOfPointer:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ")->*";
                    else operatorCall = true;
                    break;
                case OperatorType::Comma:
                    if(java_function->arguments().size()==1)
                        s << "(*" << object_name << ") , ";
                    else operatorCall = true;
                    break;
                case OperatorType::FunctionCall:
                    s << "(*" << object_name << ")(";
                    additionalClosingPar = true;
                    break;
                case OperatorType::TypeCast:
                    if(java_function->arguments().size()==0)
                        s << "*" << object_name;
                    else operatorCall = true;
                    break;
                default:
                    operatorCall = false;
                    break;
                }
            }
            if(!operatorCall){
                if(java_function->isInGlobalScope()){
                    s << function_name;
                    if(!templateParameters.isEmpty())
                        s << '<' << templateParameters.join(QStringLiteral(u", ")) << '>';
                    s << "(";
                    if(!java_function->isStatic())
                        s << "*" << object_name << ", ";
                }else{
                    s << object_name;
                    if(java_function->isStatic())
                        s << "::" << prefix;
                    else
                        s << "->" << prefix;
                    s << function_name;
                    if(!templateParameters.isEmpty())
                        s << '<' << templateParameters.join(QStringLiteral(u", ")) << '>';
                    if(java_function->arguments().isEmpty()
                            && java_function->declaringClass()->isNamespace()
                            && java_function->declaringClass()->qualifiedCppName().startsWith("QColorConstants")){
                        return;
                    }
                    s << "(";
                }
            }
            QString prefix;
            if(!(option & JNIProxyFunction))
                prefix = "__qt_";
            if(additionalClosingBracket && !java_function->type() && !java_function->arguments().isEmpty()){
                QList<MetaArgument*> result;
                const MetaArgumentList& arguments = java_function->arguments();
                for (int i = 0; i < arguments.size(); ++i) {
                    MetaArgument *argument = arguments.at(i);
                    if ((option & SkipRemovedArguments) == SkipRemovedArguments
                            && java_function->argumentRemoved(argument->argumentIndex() + 1)!=ArgumentRemove_No) {
                        if(!java_function->isConstructor()
                                || ( !argument->originalDefaultValueExpression().isEmpty()
                                     && java_function->conversionRule(TS::NativeCode, argument->argumentIndex() + 1).isEmpty()
                                     && java_function->conversionRule(TS::TargetLangCode, argument->argumentIndex() + 1).isEmpty() )){
                            continue;
                        }
                    }
                    result << argument;
                }
                MetaArgument* assignment = result.takeLast();
                writeFunctionCallArguments(s, java_function, result, prefix, option);
                s << "] = ";
                writeFunctionCallArguments(s, java_function, {assignment}, prefix, option);
            }else{
                writeFunctionCallArguments(s, java_function, prefix, option);
                // The extra arguments...
                bool hasArgs = java_function->arguments().size() != 0;
                for (int i = 0; i < extra_arguments.size(); ++i) {
                    if (i > 0 || hasArgs)
                        s << ", ";
                    s << extra_arguments.at(i);
                }
                if(additionalClosingBracket)
                    s << "]";
            }
            if(!operatorCall || additionalClosingPar){
                s << ")";
            }
        }
    }
}

void CppImplGenerator::writeFunctionCallArguments(QTextStream &s, const MetaFunction *java_function,
                                const QString &prefix, Option options){
    writeFunctionCallArguments(s, java_function,
                                [=]()->QList<MetaArgument*> {
                                    QList<MetaArgument*> result;
                                    const MetaArgumentList& arguments = java_function->arguments();
                                    for (int i = 0; i < arguments.size(); ++i) {
                                        MetaArgument *argument = arguments.at(i);
                                        if ((options & SkipRemovedArguments) == SkipRemovedArguments
                                                && java_function->argumentRemoved(argument->argumentIndex() + 1)!=ArgumentRemove_No) {
                                            if(!java_function->isConstructor()
                                                    || ( !argument->originalDefaultValueExpression().isEmpty()
                                                         && java_function->conversionRule(TS::NativeCode, argument->argumentIndex() + 1).isEmpty()
                                                         && java_function->conversionRule(TS::TargetLangCode, argument->argumentIndex() + 1).isEmpty() )){
                                                continue;
                                            }
                                        }
                                        result << argument;
                                    }
                                    return result;
                                }(),
                                prefix, options);
}

void CppImplGenerator::writeFunctionCallArguments(QTextStream &s,
        const MetaFunction *java_function,
        const QList<MetaArgument*>& arguments,
        const QString &prefix,
        Option options) {
    Q_ASSERT(java_function);
    int written_arguments = 0;
    if(options & JNIProxyFunction){
        s << "__jni_env";
        ++written_arguments;
        if(!java_function->isStatic()){
            if(!java_function->proxyCall().isEmpty())
                s << ", __qt_this";
            else if(java_function->implementingClass()->typeEntry()->isNativeIdBased())
                s << ", __this_nativeId";
            else
                s << ", __this";
            ++written_arguments;
        }
        const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
        for(const ArgumentModification& argumentMod : addedArguments.first.values()){
            s << ", " << argumentMod.modified_name;
            ++written_arguments;
        }
        for(const ArgumentModification& argumentMod : addedArguments.second){
            s << ", " << argumentMod.modified_name;
            ++written_arguments;
        }
    }
    for (int i = 0; i < arguments.size(); ++i) {
        if(const MetaArgument *argument = arguments.at(i)){
            bool hasConversionRule = java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1);
            if ((options & SkipRemovedArguments) == SkipRemovedArguments
                    && java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_Remove
                    && !hasConversionRule
                    && !argument->originalDefaultValueExpression().isEmpty()) {
                continue;
            }
            if(argument->type()->isNativePointer()){
                ReportHandler::debugFull(QString("native pointer pattern for '%1'  %2::%3")
                                         .arg(argument->type()->cppSignature(), java_function->ownerClass()->fullName(),java_function->signature()));
            }

            if (written_arguments++ > 0) {
                s << ", ";
            }
//            if(java_function->attributes() & MetaAttributes::BracketCall){
//                s << "." << argument->argumentName() << "=";
//            }
            bool enum_as_int = (options & EnumAsInts)
                                && (argument->type()->typeEntry()->isEnum()
                                        || argument->type()->typeEntry()->isFlags())
                                && argument->type()->indirections().isEmpty()
                                && argument->type()->getReferenceType()!=MetaType::RReference
                                && (argument->type()->getReferenceType()!=MetaType::Reference || argument->type()->isConstant());
            bool enum_from_int = (options & EnumFromInt)
                                && (argument->type()->typeEntry()->isEnum()
                                        || argument->type()->typeEntry()->isFlags())
                                && argument->type()->indirections().isEmpty()
                                && argument->type()->getReferenceType()!=MetaType::RReference
                                && (argument->type()->getReferenceType()!=MetaType::Reference || argument->type()->isConstant());
            if (argument->type()->isEnum()) {
                enum_as_int |= !static_cast<const EnumTypeEntry *>(argument->type()->typeEntry())->isPublic();
            }
            uint paren = 0;
            if(enum_from_int && argument->type()->indirections().isEmpty()){
                writeTypeInfo(s, argument->type(), Option(SkipName | ForceValueType));
                s << "(";
                paren++;
            }else if ((!(options & NoCasts) && !enum_as_int) || ((options & ForceEnumCast) && argument->type()->isEnum())) {

                // If the type in the signature is specified without template instantiation, but the
                // class is actually a template class, then we have troubles.
                if ( argument->type()->indirections().isEmpty()
                     && (argument->type()->getReferenceType()!=MetaType::Reference || argument->type()->isConstant())
                     && (
                         ((options & NoEnumAsInts)==0 && (argument->type()->isEnum() || argument->type()->isFlags()))
                         || argument->type()->isPrimitive()
                         || argument->type()->isPrimitiveChar()
                      )
                    ) {
                    if(argument->type()->isPrimitive() && !hasConversionRule)
                        s << "static_cast<";
                    writeTypeInfo(s, argument->type(), Option(SkipName | ForceValueType));
                    if(argument->type()->isPrimitive() && !hasConversionRule)
                        s << ">";
                    s << "(";
                    paren++;
                }else if(argument->type()->getReferenceType()==MetaType::RReference){
                    s << "std::move(";
                    paren++;
                }
            }
            if (argument->type()->isSmartPointer() && prefix=="__qt_") {
                const SmartPointerTypeEntry* pentry = static_cast<const SmartPointerTypeEntry*>(argument->type()->typeEntry());
                switch(pentry->type()){
                case SmartPointerTypeEntry::unique_ptr:
                    s << "std::move(";
                    ++paren;
                    break;
                case SmartPointerTypeEntry::QScopedPointer:
                    Q_ASSERT(argument->type()->instantiations().size()>0);
                    s << "QScopedPointer<";
                    writeTypeInfo(s, argument->type()->instantiations()[0], SkipName);
                    s << ">(";
                    ++paren;
                    break;
                default:
                    break;
                }
            }

            if(argument->type()->isArray()
                && argument->type()->arrayElementCounts().size()>0
                && argument->type()->getReferenceType()==MetaType::NoReference){
                QString qtName;
                if (!(argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                    || java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
                    qtName += prefix;
                }
                qtName += argument->indexedName();
                if(m_arrayConversions.contains(quintptr(java_function)) && m_arrayConversions[quintptr(java_function)].contains(qtName)){
                    s << m_arrayConversions[quintptr(java_function)].take(qtName);
                    if(m_arrayConversions[quintptr(java_function)].isEmpty())
                        m_arrayConversions.remove(quintptr(java_function));
                }else{
                    s << qtName;
                }
            }else{
                if (!(argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                        || java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
                    s << prefix;
                }
                s << argument->indexedName();
            }
            for(uint i=0; i<paren; ++i)
                s << ")";
            if(java_function->useArgumentAsArray(argument->argumentIndex() + 1)
                && !java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
                if(java_function->insertUtilArgument(argument->argumentIndex() + 1) && (options & SkipRemovedArguments)){
                    int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                            s << ", __qt_offset_of_" << argument->indexedName();
                        }
                    }
                }
            }
        }
    }
}

void CppImplGenerator::writeFunctionCallArguments(QTextStream &s,
        const MetaFunctional *java_function,
        const QString &prefix,
        Option options) {
    const MetaArgumentList& arguments = java_function->arguments();

    int written_arguments = 0;
    for (int i = 0; i < arguments.size(); ++i) {
        const MetaArgument *argument = arguments.at(i);
        bool hasConversionRule = java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1);

        if ((options & SkipRemovedArguments) == SkipRemovedArguments
                && !hasConversionRule
                && java_function->argumentRemoved(i + 1)!=ArgumentRemove_No) {
            continue;
        }

        /*if (argument->type()->typeEntry()->isObject() && argument->type()->actualIndirections() == 0
                && !java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)) {
            ReportHandler::warning(QString("Object type '%1' passed as value. Resulting code will not compile.  %2")
                                   .arg(argument->type()->cppSignature())
                                   .arg(java_function->fullName()));
        }*/
        if(argument->type()->isNativePointer()){
            ReportHandler::debugFull(QString("native pointer pattern for '%1'  %2")
                                     .arg(argument->type()->cppSignature(), java_function->fullName()));
        }

        if (written_arguments++ > 0) {
            s << ", ";
        }

        bool enum_as_int = (options & EnumAsInts)
                            && (argument->type()->typeEntry()->isEnum()
                                    || argument->type()->typeEntry()->isFlags())
                            && argument->type()->indirections().isEmpty()
                            && (argument->type()->getReferenceType()!=MetaType::Reference || argument->type()->isConstant());
        if (argument->type()->isEnum()) {
            enum_as_int |= !static_cast<const EnumTypeEntry *>(argument->type()->typeEntry())->isPublic();
        }

        int paren = 0;
        if ((!(options & NoCasts) && !enum_as_int) || ((options & ForceEnumCast) && argument->type()->isEnum())) {

            // If the type in the signature is specified without template instantiation, but the
            // class is actually a template class, then we have troubles.
            if ( argument->type()->indirections().isEmpty()
                 && (argument->type()->getReferenceType()!=MetaType::Reference || argument->type()->isConstant())
                 && (argument->type()->isEnum()
                  || argument->type()->isPrimitive()
                  || argument->type()->isPrimitiveChar()
                  || argument->type()->isFlags())
                ) {
                if(argument->type()->isPrimitive() && !hasConversionRule)
                    s << "static_cast<";
                writeTypeInfo(s, argument->type(), Option(SkipName | ForceValueType));
                if(argument->type()->isPrimitive() && !hasConversionRule)
                    s << ">";
                s << "(";
                ++paren;
            }
        }
        if (argument->type()->isSmartPointer() && prefix=="__qt_") {
            const SmartPointerTypeEntry* pentry = static_cast<const SmartPointerTypeEntry*>(argument->type()->typeEntry());
            switch(pentry->type()){
            case SmartPointerTypeEntry::unique_ptr:
                s << "std::move(";
                ++paren;
                break;
            case SmartPointerTypeEntry::QScopedPointer:
                Q_ASSERT(argument->type()->instantiations().size()>0);
                s << "QScopedPointer<";
                writeTypeInfo(s, argument->type()->instantiations()[0], SkipName);
                s << ">(";
                ++paren;
                break;
            default:
                break;
            }
        }
        if(argument->type()->getReferenceType()==MetaType::RReference){
            s << "std::move(";
            ++paren;
        }

        if (!(argument->type()->isPrimitive() || argument->type()->isPrimitiveChar())
                || java_function->hasConversionRule(TS::NativeCode, argument->argumentIndex() + 1)
            ) {
            s << prefix;
        }
        s << argument->indexedName();
        for(int i=0; i<paren; ++i)
            s << ")";
        if(java_function->useArgumentAsArray(argument->argumentIndex() + 1)
            && !java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)){
            if(java_function->insertUtilArgument(argument->argumentIndex() + 1) && (options & SkipRemovedArguments)){
                int lengthParameter = java_function->utilArgumentIndex(argument->argumentIndex() + 1);
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
                        s << ", __qt_offset_of_" << argument->indexedName();
                    }
                }
            }
        }
    }
}

void CppImplGenerator::writeExtraIncludes(QTextStream &s, const MetaClass *java_class, QSet<QString>& dedupe, bool skipQtJambi) {
    IncludeList includes = java_class->typeEntry()->extraIncludes();
    if(java_class->typeEntry()->designatedInterface()){
        includes << java_class->typeEntry()->designatedInterface()->extraIncludes();
    }
    QSet<const MetaClass *> classes;
    const MetaClass *_cls = java_class;
    while(_cls){
        classes << _cls;
        for(const MetaClass *ifc : _cls->interfaces()){
            classes << ifc;
        }
        _cls = _cls->baseClass();
    }
    classes.remove(java_class);
    QList<const MetaClass *> _classes = classes.values();
    std::sort(_classes.begin(), _classes.end(), [](const MetaClass *a, const MetaClass *b) -> bool {
        return a->name() < b->name();
    });
    for(const MetaClass *cls : _classes){
        for(const Include& inc : cls->typeEntry()->extraIncludes()){
            if(inc.type==Include::IncludePath && inc.inherited){
                includes << inc;
            }
        }
        if(cls->typeEntry()->designatedInterface()){
            for(const Include& inc : cls->typeEntry()->designatedInterface()->extraIncludes()){
                if(inc.type==Include::IncludePath && inc.inherited){
                    includes << inc;
                }
            }
        }
    }

    std::sort(includes.begin(), includes.end());
    auto used = dedupe.size();
    for(const Include &i : includes) {
        if (i.type != Include::TargetLangImport) {
            if(skipQtJambi){
                if(!i.name.startsWith("QtJambi")){
                    writeInclude(s, i, dedupe);
                }
            }else{
                writeInclude(s, i, dedupe);
            }
        }
    }

    if (used<dedupe.size())
        s << Qt::endl;
}

void CppImplGenerator::writeExtraIncludes(QTextStream &s, const MetaFunctional *java_class, QSet<QString>& dedupe, bool skipQtJambi) {
    IncludeList includes = java_class->typeEntry()->extraIncludes();
    std::sort(includes.begin(), includes.end());

    auto used = dedupe.size();
    for(const Include &i : includes) {
        if (i.type != Include::TargetLangImport) {
            if(skipQtJambi){
                if(!i.name.startsWith("QtJambi")){
                    writeInclude(s, i, dedupe);
                }
            }else{
                writeInclude(s, i, dedupe);
            }
        }
    }

    if (used<dedupe.size())
        s << Qt::endl;
}

QString computeMangledTypeName(const MetaType *type){
    QString mangledTypeName;
    if(type->typeEntry()->isFunctional()){
        mangledTypeName = type->typeEntry()->name();
    }else{
        mangledTypeName = type->typeEntry()->qualifiedCppName();
    }
    if(mangledTypeName=="qtjamireal")
        mangledTypeName = "double";
    mangledTypeName = mangledTypeName.replace(QStringLiteral(u"::"), QStringLiteral(u"_"))
                                     .replace("<", "_lt")
                                     .replace(">", "_gt")
                                     .replace(",", "_")
                                     .replace(".", "_")
                                     .replace("*", "_ptr")
                                     .replace("&", "_ref")
                                     .replace(" ", "_");
    if(!type->instantiations().isEmpty()){
        mangledTypeName += "_lt";
        for(const MetaType* inst : type->instantiations()){
            mangledTypeName += "_" + computeMangledTypeName(inst);
        }
        mangledTypeName += "_gt";
    }
    for(bool ind : type->indirections()){
        mangledTypeName += ind ? "_cptr" : "_ptr";
    }
    if(type->getReferenceType()==MetaType::Reference){
        if(type->isConstant()){
            mangledTypeName += "_cref";
        }else{
            mangledTypeName += "_ref";
        }
    }else if(type->getReferenceType()==MetaType::RReference){
        if(type->isConstant()){
            mangledTypeName += "_crval";
        }else{
            mangledTypeName += "_rval";
        }
    }else{
        if(type->isConstant()){
            mangledTypeName += "_const";
        }
    }
    return mangledTypeName;
}

void CppImplGenerator::writeTypeConversion(QTextStream &s, const MetaFunction *function, MetaType *type, int index, const QString& metaTypeId, QStringList& converterFunctions, QSet<QString> &forwardDeclarations){
    if(!type){
        s << INDENT << "infos << ParameterInfo{QMetaType::Void};" << Qt::endl;
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    switch(QMetaType::fromName(qPrintable(type->normalizedSignature())).id()){
#else
    switch(QMetaType::type(qPrintable(type->normalizedSignature()))){
#endif
    case QMetaType::UChar:       s << INDENT << "infos << ParameterInfo{QMetaType::UChar};" << Qt::endl; return;
    case QMetaType::Char:        s << INDENT << "infos << ParameterInfo{QMetaType::Char};" << Qt::endl; return;
    case QMetaType::SChar:       s << INDENT << "infos << ParameterInfo{QMetaType::SChar};" << Qt::endl; return;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::Char32:      s << INDENT << "infos << ParameterInfo{QMetaType::Char32};" << Qt::endl; return;
#endif
    case QMetaType::UShort:      s << INDENT << "infos << ParameterInfo{QMetaType::UShort};" << Qt::endl; return;
    case QMetaType::Short:       s << INDENT << "infos << ParameterInfo{QMetaType::Short};" << Qt::endl; return;
    case QMetaType::UInt:        s << INDENT << "infos << ParameterInfo{QMetaType::UInt};" << Qt::endl; return;
    case QMetaType::Int:         s << INDENT << "infos << ParameterInfo{QMetaType::Int};" << Qt::endl; return;
    case QMetaType::ULongLong:   s << INDENT << "infos << ParameterInfo{QMetaType::ULongLong};" << Qt::endl; return;
    case QMetaType::LongLong:    s << INDENT << "infos << ParameterInfo{QMetaType::LongLong};" << Qt::endl; return;
    case QMetaType::ULong:       s << INDENT << "infos << ParameterInfo{QMetaType::ULong};" << Qt::endl; return;
    case QMetaType::Long:        s << INDENT << "infos << ParameterInfo{QMetaType::Long};" << Qt::endl; return;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::Char16:      s << INDENT << "infos << ParameterInfo{QMetaType::Char16};" << Qt::endl; return;
#endif
    case QMetaType::QChar:       s << INDENT << "infos << ParameterInfo{QMetaType::QChar};" << Qt::endl; return;
    case QMetaType::Float:       s << INDENT << "infos << ParameterInfo{QMetaType::Float};" << Qt::endl; return;
    case QMetaType::Double:      s << INDENT << "infos << ParameterInfo{QMetaType::Double};" << Qt::endl; return;
    case QMetaType::Bool:        s << INDENT << "infos << ParameterInfo{QMetaType::Bool};" << Qt::endl; return;
    case QMetaType::QString:     s << INDENT << "infos << ParameterInfo{QMetaType::QString};" << Qt::endl; return;
    case QMetaType::QVariant:    s << INDENT << "infos << ParameterInfo{QMetaType::QVariant};" << Qt::endl; return;
    case QMetaType::QByteArray:  s << INDENT << "infos << ParameterInfo{QMetaType::QByteArray};" << Qt::endl; return;
    default: break;
    }

    const QString mangledTypeName = computeMangledTypeName(type);
    {
        s << INDENT << "infos << ParameterInfo{" << Qt::endl;
        {
            INDENTATION(INDENT)
            {
                INDENTATION(INDENT)
                {
                    INDENTATION(INDENT)
                    s << INDENT << "// ";
                    writeTypeInfo(s, type, Option(SkipName));
                    s << Qt::endl
                      << INDENT << metaTypeId << "," << Qt::endl;
                    s << INDENT << "\"";
                    if(function->useArgumentAsArray(index)){
                        if(function->useArgumentAsBuffer(index)){
                            if(type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()){
                                if(type->typeEntry()->targetLangName()=="int"){
                                    s << "java/nio/IntBuffer";
                                }else if(type->typeEntry()->targetLangName()=="long"){
                                    s << "java/nio/LongBuffer";
                                }else if(type->typeEntry()->targetLangName()=="short"){
                                    s << "java/nio/ShortBuffer";
                                }else if(type->typeEntry()->targetLangName()=="byte"){
                                    s << "java/nio/ByteBuffer";
                                }else if(type->typeEntry()->targetLangName()=="char"){
                                    s << "java/nio/CharBuffer";
                                }else if(type->typeEntry()->targetLangName()=="float"){
                                    s << "java/nio/FloatBuffer";
                                }else if(type->typeEntry()->targetLangName()=="double"){
                                    s << "java/nio/DoubleBuffer";
                                }else{
                                    s << "java/nio/Buffer";
                                }
                            }else{
                                s << "java/nio/Buffer";
                            }
                        }else if(type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()){
                            if(type->typeEntry()->targetLangName()=="int"){
                                s << "[I";
                            }else if(type->typeEntry()->targetLangName()=="long"){
                                s << "[J";
                            }else if(type->typeEntry()->targetLangName()=="short"){
                                s << "[S";
                            }else if(type->typeEntry()->targetLangName()=="byte"){
                                s << "[B";
                            }else if(type->typeEntry()->targetLangName()=="boolean"){
                                s << "[Z";
                            }else if(type->typeEntry()->targetLangName()=="char"){
                                s << "[C";
                            }else if(type->typeEntry()->targetLangName()=="float"){
                                s << "[F";
                            }else if(type->typeEntry()->targetLangName()=="double"){
                                s << "[D";
                            }
                        }else{
                            s << "[L" << type->typeEntry()->qualifiedTargetLangJNIName() << ";";
                        }
                    }else if(type->typeEntry()->isVoid() || type->isNativePointer())
                        s << "io/qt/QNativePointer";
                    else
                        s << type->typeEntry()->qualifiedTargetLangJNIName();
                    s << "\"," << Qt::endl;

                    QStringList pps = getFunctionPPConditions(function);
                    if(!function->declaringClass()->typeEntry()->ppCondition().isEmpty()){
                        pps.append(function->declaringClass()->typeEntry()->ppCondition());
                        pps.removeDuplicates();
                    }
                    QString tsId = priGenerator->subDirectoryForClass(function->declaringClass(), PriGenerator::CppDirectory);
                    bool resetObjectAfterUse = function->resetObjectAfterUse(index);
                    QString _mangledTypeName = mangledTypeName;
                    if(resetObjectAfterUse)
                        _mangledTypeName += QLatin1String("_InvalidateAfterUse");
                    if(nativeToJavaConverterInfos[tsId].contains(_mangledTypeName)){
                        if(!type->typeEntry()->ppCondition().isEmpty()){
                            QString strg;
                            QTextStream stream(&strg);
                            stream << "#if " << type->typeEntry()->ppCondition()
                                   << Qt::endl
                                   << QString("bool nativeToJavaConverter_%1(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool); // declared in %2").arg(_mangledTypeName, nativeToJavaConverterInfos[tsId][_mangledTypeName]->qualifiedCppName())
                                   << Qt::endl << "#endif // " << type->typeEntry()->ppCondition() << Qt::endl;
                            forwardDeclarations.insert(strg);
                        }else{
                            forwardDeclarations.insert(QString("bool nativeToJavaConverter_%1(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool); // declared in %2").arg(_mangledTypeName, nativeToJavaConverterInfos[tsId][_mangledTypeName]->qualifiedCppName()));
                        }
                        s << INDENT << "&nativeToJavaConverter_" << _mangledTypeName << "," << Qt::endl;
                    }else{
                        QString nativeToJavaConverter;
                        bool isCustomized = false;
                        {
                            INDENTATIONRESET(INDENT)
                            QTextStream s(&nativeToJavaConverter);
                            s << INDENT << "Q_UNUSED(env)" << Qt::endl;
                            QString _in;
                            {
                                QTextStream s(&_in);
                                s << "*reinterpret_cast<";
                                writeTypeInfo(s, type, Option(SkipName | ExcludeReference | ExcludeConst));
                                s << " const*>(in)";
                            }
                            QString outArg;
                            QString outBoxing;
                            if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()) && type->indirections().isEmpty()){
                                QString qualifiedTargetLangName = type->typeEntry()->qualifiedTargetLangName();
                                if(qualifiedTargetLangName=="boolean"){
                                    outArg = "out.z";
                                    outBoxing = "out.l = QtJambiAPI::toJavaBooleanObject(env, out.z);";
                                }else if(qualifiedTargetLangName=="byte"){
                                    outArg = "out.b";
                                    outBoxing = "out.l = QtJambiAPI::toJavaByteObject(env, out.b);";
                                }else if(qualifiedTargetLangName=="short"){
                                    outArg = "out.s";
                                    outBoxing = "out.l = QtJambiAPI::toJavaShortObject(env, out.s);";
                                }else if(qualifiedTargetLangName=="int"){
                                    outArg = "out.i";
                                    outBoxing = "out.l = QtJambiAPI::toJavaIntegerObject(env, out.i);";
                                }else if(qualifiedTargetLangName=="long"){
                                    outArg = "out.j";
                                    outBoxing = "out.l = QtJambiAPI::toJavaLongObject(env, out.j);";
                                }else if(qualifiedTargetLangName=="char"){
                                    outArg = "out.c";
                                    outBoxing = "out.l = QtJambiAPI::toJavaCharacterObject(env, out.c);";
                                }else if(qualifiedTargetLangName=="float"){
                                    outArg = "out.f";
                                    outBoxing = "out.l = QtJambiAPI::toJavaFloatObject(env, out.f);";
                                }else if(qualifiedTargetLangName=="double"){
                                    outArg = "out.d";
                                    outBoxing = "out.l = QtJambiAPI::toJavaDoubleObject(env, out.d);";
                                }else{
                                    outArg = "out.l";
                                }
                            }else{
                                outArg = "out.l";
                            }
                            QString scopedConvert1;
                            bool isCustomized1 = false;
                            {
                                QTextStream _s(&scopedConvert1);
                                isCustomized1 = writeQtToJava(_s,
                                                              type,
                                                              _in.trimmed(),
                                                              outArg,
                                                              function, nullptr,
                                                              index,
                                                              Option(NoTmpVariable | OptionalScope),
                                                              "env",
                                                              "scope");
                            }
                            QString scopedConvert2;
                            bool isCustomized2 = false;
                            {
                                QTextStream _s(&scopedConvert2);
                                isCustomized2 = writeQtToJava(_s,
                                                              type,
                                                              _in.trimmed(),
                                                              outArg,
                                                              function, nullptr,
                                                              index,
                                                              Option(NoTmpVariable),
                                                              "env",
                                                              "");
                            }
                            bool checkedScope = false;
                            if (resetObjectAfterUse){
                                s << INDENT << "if(!scope)" << Qt::endl
                                  << INDENT << "    return false;" << Qt::endl;
                                checkedScope = true;
                            }
                            if(scopedConvert1==scopedConvert2){
                                if(!checkedScope){
                                    if((scopedConvert1.contains("*scope") || scopedConvert1.contains("scope->"))
                                            && (!scopedConvert1.contains("if(scope)") || !scopedConvert1.contains("if(!scope)"))){
                                        s << INDENT << "if(!scope)" << Qt::endl
                                          << INDENT << "    return false;" << Qt::endl;
                                        checkedScope = true;
                                    }else if(!resetObjectAfterUse){
                                        s << INDENT << "Q_UNUSED(scope)" << Qt::endl;
                                    }
                                }
                                s << scopedConvert1;
                                isCustomized |= isCustomized1;
                            }else if(!checkedScope && (scopedConvert1.contains("*scope") || scopedConvert1.contains("scope->"))
                                     && (!scopedConvert1.contains("if(scope)") || !scopedConvert1.contains("if(!scope)"))){
                                s << INDENT << "if(!scope)" << Qt::endl
                                  << INDENT << "    return false;" << Qt::endl;
                                checkedScope = true;
                                s << scopedConvert1;
                                isCustomized |= isCustomized1;
                            }else{
                                if(checkedScope){
                                    for(const QString& line : scopedConvert1.split(QStringLiteral(u"\n"))){
                                        if(!line.trimmed().isEmpty()){
                                            s << Qt::endl;
                                            if(!line.startsWith(QLatin1Char('#')))
                                                s << "    ";
                                            s << line;
                                        }
                                    }
                                }else{
                                    s << INDENT << "if(scope){";
                                    for(const QString& line : scopedConvert1.split(QStringLiteral(u"\n"))){
                                        if(!line.trimmed().isEmpty()){
                                            s << Qt::endl;
                                            if(!line.startsWith(QLatin1Char('#')))
                                                s << "    ";
                                            s << line;
                                        }
                                    }
                                    s << Qt::endl << INDENT << "}else{";
                                    for(const QString& line : scopedConvert2.split(QStringLiteral(u"\n"))){
                                        if(!line.trimmed().isEmpty()){
                                            s << Qt::endl;
                                            if(!line.startsWith(QLatin1Char('#')))
                                                s << "    ";
                                            s << line;
                                        }
                                    }
                                    s << Qt::endl << INDENT << "}" << Qt::endl;
                                }
                                isCustomized |= isCustomized1;
                                isCustomized |= isCustomized2;
                            }
                            if(!outBoxing.isEmpty()){
                                s << INDENT << "if(forceBoxedType)" << Qt::endl;
                                s << INDENT << "    " << outBoxing << Qt::endl;
                            }
                            if (resetObjectAfterUse){
                                //isCustomized = true;
                                if (type->isContainer()){
                                    if(static_cast<const ContainerTypeEntry *>(type->typeEntry())->type()==ContainerTypeEntry::std_array){
                                        s << INDENT << "scope->addArrayInvalidation(env, out.l);" << Qt::endl;
                                    }else{
                                        s << INDENT << "scope->addContainerInvalidation(env, out.l);" << Qt::endl;
                                    }
                                }else if (type->isArray()){
                                    s << INDENT << "scope->addArrayInvalidation(env, out.l);" << Qt::endl;
                                }else{
                                    s << INDENT << "scope->addObjectInvalidation(env, out.l);" << Qt::endl;
                                }
                            }
                            s << INDENT << "return true;" << Qt::endl;
                        }
                        if(isCustomized){
                            s << INDENT << "[](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool";
                            if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()) && type->indirections().isEmpty()){
                                s << " forceBoxedType";
                            }
                            s << ")->bool{";
                            for(const QString& line : nativeToJavaConverter.split(QStringLiteral(u"\n"))){
                                if(!line.isEmpty()){
                                    s << Qt::endl;
                                    if(!line.startsWith(QLatin1Char('#')))
                                        s << INDENT << "    ";
                                    s << line;
                                }
                            }
                            s << Qt::endl << INDENT << "}," << Qt::endl;
                        }else{
                            if(!type->typeEntry()->ppCondition().isEmpty()){
                                QString strg;
                                QTextStream stream(&strg);
                                stream << "#if " << type->typeEntry()->ppCondition()
                                       << Qt::endl
                                       << QString("bool nativeToJavaConverter_%1(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);").arg(_mangledTypeName)
                                       << Qt::endl << "#endif // " << type->typeEntry()->ppCondition() << Qt::endl;
                                forwardDeclarations.insert(strg);
                            }else{
                                forwardDeclarations.insert(QString("bool nativeToJavaConverter_%1(JNIEnv*, QtJambiScope*, const void*, jvalue&, bool);").arg(_mangledTypeName));
                            }
                            QString functionCode;
                            {
                                QTextStream s(&functionCode);
                                if(!type->typeEntry()->ppCondition().isEmpty())
                                    s << "#if " << type->typeEntry()->ppCondition() << Qt::endl;
                                s << "bool nativeToJavaConverter_" << _mangledTypeName << "(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool";
                                if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()) && type->indirections().isEmpty()){
                                    s << " forceBoxedType";
                                }
                                s << ") {";
                                for(const QString& line : nativeToJavaConverter.split(QStringLiteral(u"\n"))){
                                    if(!line.isEmpty()){
                                        s << Qt::endl;
                                        if(!line.startsWith(QLatin1Char('#')))
                                            s << "    ";
                                        s << line;
                                    }
                                }
                                s << Qt::endl << "}" << Qt::endl;
                                if(!type->typeEntry()->ppCondition().isEmpty())
                                    s << "#endif // " << type->typeEntry()->ppCondition() << Qt::endl;
                            }
                            converterFunctions << functionCode;
                            s << INDENT << "&nativeToJavaConverter_" << _mangledTypeName << "," << Qt::endl;
                            nativeToJavaConverterInfos[tsId].insert(_mangledTypeName, function->ownerClass());
                        }
                    }

                    if(javaToNativeConverterInfos[tsId].contains(mangledTypeName)){
                        if(type->typeEntry()->ppCondition().isEmpty()){
                            forwardDeclarations.insert(QString("bool javaToNativeConverter_%1(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType); // declared in %2").arg(mangledTypeName, javaToNativeConverterInfos[tsId][mangledTypeName]->qualifiedCppName()));
                        }else{
                            QString strg;
                            QTextStream stream(&strg);
                            stream << "#if " << type->typeEntry()->ppCondition()
                                   << Qt::endl
                                   << QString("bool javaToNativeConverter_%1(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType); // declared in %2").arg(mangledTypeName, javaToNativeConverterInfos[tsId][mangledTypeName]->qualifiedCppName())
                                   << Qt::endl << "#endif // " << type->typeEntry()->ppCondition() << Qt::endl;
                            forwardDeclarations.insert(strg);
                        }
                        s << INDENT << "&javaToNativeConverter_" << mangledTypeName;
                    }else{
                        QString javaToNativeConverter;
                        bool isCustomized = false;
                        {
                            QTextStream s(&javaToNativeConverter);
                            INDENTATIONRESET(INDENT)
                            s << INDENT << "Q_UNUSED(env)" << Qt::endl;
                            QString javaIn = QLatin1String("in.l");
                            if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()) && type->indirections().isEmpty()){
                                QString qualifiedTargetLangName = type->typeEntry()->qualifiedTargetLangName();
                                s << INDENT << translateType(type) << " _in;" << Qt::endl;
                                s << INDENT << "switch(valueType){" << Qt::endl;
                                s << INDENT << "case jValueType::l:" << Qt::endl;
                                if(qualifiedTargetLangName=="int"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaIntegerObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="short"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaShortObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="byte"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaByteObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="long"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaLongObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="float"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaFloatObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="double"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaDoubleObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="boolean"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaBooleanObject(env, in.l);" << Qt::endl;
                                }else if(qualifiedTargetLangName=="char"){
                                    s << INDENT << "    _in = QtJambiAPI::fromJavaCharacterObject(env, in.l);" << Qt::endl;
                                }
                                s << INDENT << "break;" << Qt::endl;
                                s << INDENT << "default:" << Qt::endl;
                                if(qualifiedTargetLangName=="boolean"){
                                    s << INDENT << "    _in = in.z;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="byte"){
                                    s << INDENT << "    _in = in.b;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="short"){
                                    s << INDENT << "    _in = in.s;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="int"){
                                    s << INDENT << "    _in = in.i;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="long"){
                                    s << INDENT << "    _in = in.j;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="char"){
                                    s << INDENT << "    _in = in.c;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="float"){
                                    s << INDENT << "    _in = in.f;" << Qt::endl;
                                }else if(qualifiedTargetLangName=="double"){
                                    s << INDENT << "    _in = in.d;" << Qt::endl;
                                }else{
                                    s << INDENT << "    _in = in.j;" << Qt::endl;
                                }
                                s << INDENT << "break;" << Qt::endl;
                                s << INDENT << "}" << Qt::endl;
                                javaIn = QLatin1String("_in");
                            }
                            QScopedPointer<MetaType> argumentType(type->copy());
                            QScopedPointer<MetaType> ptrToArgumentType(type->copy());
                            {
                                QList<bool> indirections = ptrToArgumentType->indirections();
                                indirections << false;
                                ptrToArgumentType->setIndirections( indirections );
                            }

                            QString scopedConvert;
                            {
                                QTextStream _s(&scopedConvert);
                                if(function->useArgumentAsArray(index)
                                    && !function->useArgumentAsBuffer(index)){
                                    if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar())){
                                        if(type->typeEntry()->targetLangName()=="int"){
                                            javaIn = "jintArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="long"){
                                            javaIn = "jlongArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="short"){
                                            javaIn = "jshortArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="byte"){
                                            javaIn = "jbyteArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="boolean"){
                                            javaIn = "jbooleanArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="char"){
                                            javaIn = "jcharArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="float"){
                                            javaIn = "jfloatArray(in.l)";
                                        }else if(type->typeEntry()->targetLangName()=="double"){
                                            javaIn = "jdoubleArray(in.l)";
                                        }
                                    }else{
                                        javaIn = "jobjectArray(in.l)";
                                    }
                                }
                                isCustomized |= writeJavaToQt(_s,
                                                              type,
                                                              "out",
                                                              javaIn,
                                                              function,
                                                              nullptr,
                                                              index,
                                                              Option(OptionalScope),
                                                              "env",
                                                              "scope");
                            }
                            if(!scopedConvert.contains("scope"))
                                s << INDENT << "Q_UNUSED(scope)" << Qt::endl;
                            s << scopedConvert;
                            s << INDENT << "return true;" << Qt::endl;
                        }
                        if(isCustomized){
                            s << INDENT << "[](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType";
                            if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()) && type->indirections().isEmpty()){
                                s << " valueType";
                            }
                            s << ") -> bool {";
                            {
                                INDENTATION(INDENT)
                                for(const QString& line : javaToNativeConverter.split(QStringLiteral(u"\n"))){
                                    if(!line.isEmpty()){
                                        s << Qt::endl;
                                        if(!line.startsWith(QLatin1Char('#')))
                                            s << INDENT;
                                        s << line;
                                    }
                                }
                            }
                            s << Qt::endl << INDENT << "}";
                        }else{
                            if(type->typeEntry()->ppCondition().isEmpty()){
                                forwardDeclarations.insert(QString("bool javaToNativeConverter_%1(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);").arg(mangledTypeName));
                            }else{
                                QString strg;
                                QTextStream stream(&strg);
                                stream << "#if " << type->typeEntry()->ppCondition()
                                       << Qt::endl
                                       << QString("bool javaToNativeConverter_%1(JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType);").arg(mangledTypeName)
                                       << Qt::endl << "#endif // " << type->typeEntry()->ppCondition() << Qt::endl;
                                forwardDeclarations.insert(strg);
                            }
                            QString functionCode;
                            {
                                QTextStream s(&functionCode);
                                if(!type->typeEntry()->ppCondition().isEmpty())
                                    s << "#if " << type->typeEntry()->ppCondition() << Qt::endl;
                                s << "bool javaToNativeConverter_" << mangledTypeName << "(JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType";
                                if((type->typeEntry()->isPrimitive() || type->typeEntry()->isQChar()) && type->indirections().isEmpty()){
                                    s << " valueType";
                                }
                                s << ") {";
                                for(const QString& line : javaToNativeConverter.split(QStringLiteral(u"\n"))){
                                    if(!line.isEmpty()){
                                        s << Qt::endl;
                                        if(!line.startsWith(QLatin1Char('#')))
                                            s << "    ";
                                        s << line;
                                    }
                                }
                                s << Qt::endl << "}" << Qt::endl;
                                if(!type->typeEntry()->ppCondition().isEmpty())
                                    s << "#endif //" << type->typeEntry()->ppCondition() << Qt::endl;
                            }
                            converterFunctions << functionCode;
                            s << INDENT << "&javaToNativeConverter_" << mangledTypeName;
                            javaToNativeConverterInfos[tsId].insert(mangledTypeName, function->ownerClass());
                        }
                    }
                }
            }
        }
        s << "};" << Qt::endl;
    }
}

void CppImplGenerator::writeArgumentConversion(QTextStream &s, const MetaFunction *function, QStringList& converterFunctions, QSet<QString> &forwardDeclarations){
    MetaArgumentList arguments;
    for (int i = 0; i < function->arguments().size(); ++i) {
        if(function->argumentRemoved(i+1)==ArgumentRemove_No){
            arguments << function->arguments()[i];
        }
    }
    writeTypeConversion(s,
                        function,
                        function->type(),
                        0,
                        "method.returnType()",
                        converterFunctions,
                        forwardDeclarations
                    );
    for(int i=0; i<arguments.size(); ++i){
        MetaArgument* argument = arguments[i];
        writeTypeConversion(s,
                            function,
                            argument->type(),
                            argument->argumentIndex()+1,
                            "method.parameterType(" + QString::number(i) + ")",
                            converterFunctions,
                            forwardDeclarations
                        );
    }
}

void CppImplGenerator::writeMetaInfo(QTextStream &s, const MetaClass *cls,
                                     const QList<const MetaFunction *> &signalsInTargetLang,
                                     bool isInterface){
    Q_UNUSED(isInterface)
    if (!cls)
        return;
    const ComplexTypeEntry *entry = cls->typeEntry();
    if ((entry->codeGeneration() & ~TypeEntry::InheritedByTypeSystem) == TypeEntry::GenerateForSubclass || entry->isIterator())
        return;

    QSet<QString> forwardDeclarations;
    QStringList converterFunctions;
    QString code;
    {
        QTextStream s(&code);
        s << "void initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(){" << Qt::endl;

        {
            INDENTATION(INDENT)
            s << INDENT << "using namespace RegistryAPI;" << Qt::endl;
            if(!cls->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#if " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
            writeCodeInjections(s, cls->typeEntry(), CodeSnip::Beginning, TS::MetaInfo);

            const QString qtName = entry->qualifiedCppName();
            const QString javaTypeName = [entry]()->QString{
                    QString javaPackage = entry->javaPackage();
                    QString javaName =  entry->lookupName();
                    if (!javaPackage.isEmpty()) {
                        javaName.prepend(javaPackage.replace(".", "/") + "/");
                    }
                    if(javaName.endsWith("$ConcreteWrapper")){
                        javaName.chop(16);
                    }
                    if(javaName.endsWith("$Impl")){
                        javaName.chop(5);
                    }
                    return javaName;
            }();
            if(!cls->isNamespace()
                    && !cls->isFake()
                    && cls->typeEntry()!=database()->qvariantType()
                    && cls->typeEntry()!=database()->qcharType()
                    && cls->typeEntry()!=database()->qstringType()){

                QString metaTypeRegistration;
                {
                    QTextStream s(&metaTypeRegistration);
                    CustomFunction customLegacyConstructor = entry->customConstructor(ComplexTypeEntry::LegacyCopyConstructor);
                    CustomFunction customCopyConstructor = entry->customConstructor(ComplexTypeEntry::CopyConstructor);
                    CustomFunction customMoveConstructor = entry->customConstructor(ComplexTypeEntry::MoveConstructor);
                    CustomFunction customDefaultConstructor = entry->customConstructor(ComplexTypeEntry::DefaultConstructor);
                    CustomFunction customDestructor = entry->customDestructor();
                    QString constructorLegacyName;
                    QString constructorCopyName;
                    QString constructorDefaultName;
                    QString constructorMoveName;
                    QString destructorName;
                    if(!customDestructor.name.isEmpty() || !customLegacyConstructor.name.isEmpty()){
                        if(!customLegacyConstructor.name.isEmpty()){
                            constructorLegacyName = "&custom_constructor_" + entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        }else{
                            constructorLegacyName = "QtMetaTypePrivate::QMetaTypeFunctionHelper<" + entry->qualifiedCppName() + ">::Construct";
                        }
                        if(!customDestructor.name.isEmpty()){
                            destructorName = "&custom_destructor_" + entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        }
                    }
                    if(!customCopyConstructor.name.isEmpty()
                            || !customMoveConstructor.name.isEmpty()
                            || !customDefaultConstructor.name.isEmpty()
                            || !customDestructor.name.isEmpty()){
                        if(!customCopyConstructor.name.isEmpty()){
                            constructorCopyName = "&custom_copy_constructor_" + entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        }else{
                            constructorCopyName = "QtJambiPrivate::QMetaTypeInterfaceFunctions<" + entry->qualifiedCppName() + ">::copyCtr";
                        }
                        if(!customMoveConstructor.name.isEmpty()){
                            constructorMoveName = "&custom_move_constructor_" + entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        }else{
                            constructorMoveName = "QtJambiPrivate::QMetaTypeInterfaceFunctions<" + entry->qualifiedCppName() + ">::moveCtr";
                        }
                        if(!customDefaultConstructor.name.isEmpty()){
                            constructorDefaultName = "&custom_default_constructor_" + entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        }else{
                            constructorDefaultName = "QtJambiPrivate::QMetaTypeInterfaceFunctions<" + entry->qualifiedCppName() + ">::defaultCtr";
                        }
                        if(!customDestructor.name.isEmpty()){
                            destructorName = "&custom_destructor_" + entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        }
                    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    QMetaType metaType(QMetaType::type(entry->name().toLocal8Bit().constData()));
#else
                    QMetaType metaType = QMetaType::fromName(entry->name().toLocal8Bit().constData());
#endif
                    if (!metaType.isValid() || metaType.id()>=QMetaType::HighestInternalId){
                        bool hiddenMetaObject = false;

                        bool emittedRegisterType = false;

                        if (!constructorLegacyName.isEmpty() && QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                            s << INDENT << "registerMetaType";
                            if(hiddenMetaObject)
                                s << "NoMetaObject";
                            s << "<" << qtName << ">(\"" << qtName << "\", ";
                            if(!destructorName.isEmpty())
                                s << destructorName << ", ";
                            s << constructorLegacyName << ");" << Qt::endl;
                            emittedRegisterType = true;
                        }
                        if (!constructorCopyName.isEmpty() && QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6, 0, 0)){
                            s << INDENT << "registerMetaType";
                            if(hiddenMetaObject)
                                s << "NoMetaObject";
                            s << "<" << qtName << ">(\"" << qtName << "\", "
                              << constructorDefaultName << ", " << constructorCopyName << ", " << constructorMoveName;
                            if(!destructorName.isEmpty())
                                s << ", " << destructorName;
                            s << ");" << Qt::endl;
                            emittedRegisterType = true;
                        }


                        if(!emittedRegisterType) {
                            // Look for default constructor, required for qRegisterMetaType
                            bool canRegister = false;
                            if(cls->isPublic() && !cls->isAbstract() && !cls->typeEntry()->skipMetaTypeRegistration()){
                                canRegister = cls->typeEntry()->isValue()
                                                || ((cls->typeEntry()->hasPublicCopyConstructor()
                                                       || cls->typeEntry()->customConstructor(ComplexTypeEntry::CopyConstructor).hasCode())
                                                   && (cls->typeEntry()->hasPublicDefaultConstructor()
                                                       || cls->typeEntry()->customConstructor(ComplexTypeEntry::DefaultConstructor).hasCode())
                                                   && (cls->typeEntry()->isDestructorPublic()
                                                       || cls->typeEntry()->customDestructor().hasCode())
                                                    && !cls->typeEntry()->skipMetaTypeRegistration());
                            }
                            if (canRegister) {
                                bool hasDefaultConstructor = entry->hasPublicDefaultConstructor();
                                if (!hasDefaultConstructor && !emittedRegisterType && QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)) {
                                    ReportHandler::warning(QString("Value type '%1' is missing a default constructor. "
                                                                   "If possible, use CustomConstructor{} element to specify default construction.").arg(cls->qualifiedCppName()));
                                }
                            }

                            if(canRegister){
                                if(!emittedRegisterType){
                                    s << INDENT << "registerMetaType";
                                    if(hiddenMetaObject)
                                        s << "NoMetaObject";
                                    s << "<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                                }else{
                                    s << INDENT << "registerMetaType";
                                    if(hiddenMetaObject)
                                        s << "NoMetaObject";
                                    s << "<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                                }
                            }
                        }
                    }else{
                        s << INDENT << "registerMetaType<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                    }
                }

                bool isInterface = false;
                if (!entry->isInterface()){ //  ?? && cls->hasStandardConstructor() ??
                    for(MetaClass *iface : cls->interfaces()){
                        const InterfaceTypeEntry *ientry = static_cast<const InterfaceTypeEntry *>(iface->typeEntry());
                        if(ientry->origin()==cls->typeEntry()){
                            if(ientry->origin()->isContainer() || ientry->origin()->isValue() || !metaTypeRegistration.isEmpty()){
                                s << INDENT << "const std::type_info& typeId = registerInterfaceValueTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl;
                            }else{
                                s << INDENT << "const std::type_info& typeId = registerInterfaceTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl;
                            }
                            if(ientry->isNativeInterface() || cls->typeEntry()->isNativeInterface()){
                                s << INDENT << "registerNativeInterface<" << qtName << ">(\"" << javaTypeName << "\");" << Qt::endl;
                            }
                            isInterface = true;
                            break;
                        }
                    }
                }
                if(!isInterface){
                    if(cls->isQObject()){
                        s << INDENT << "const std::type_info& typeId = registerQObjectTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl;
                        if(cls->typeEntry()->isQMediaControl()){
                            s << INDENT << "registerMediaControlInfo(typeId, qmediacontrol_iid<" << qtName << "*>());" << Qt::endl;
                        }
                    }else if(cls->typeEntry()->isContainer() || cls->typeEntry()->isValue() || !metaTypeRegistration.isEmpty()){
                        s << INDENT << "const std::type_info& typeId = registerValueTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl
                          << INDENT << "Q_UNUSED(typeId)" << Qt::endl;
                    }else{
                        s << INDENT << "const std::type_info& typeId = registerObjectTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl;
                    }
                }
                writeCodeInjections(s, cls->typeEntry(), CodeSnip::Position1, TS::MetaInfo);

                QString javaPackage = entry->javaPackage();

                QString javaName =  entry->lookupName();

                if (!javaPackage.isEmpty()) {
                    javaName.prepend(javaPackage.replace(u'.', u'/') + u'/');
                }

                bool usedTypeID = false;
                if (!cls->isQObject() && cls->typeEntry()->isThreadAffine()){
                    const ComplexTypeEntry *entry = cls->typeEntry();
                    const QString& ownerCode = entry->threadAffinity();
                    if(ownerCode==QStringLiteral(u"main") || ownerCode==QStringLiteral(u"ui")){
                        s << INDENT << "registerOwnerFunction(typeId, &QtJambiAPI::mainThreadOwner);" << Qt::endl;
                    }else if(ownerCode==QLatin1String("pixmap")){
                        s << INDENT << "registerOwnerFunction(typeId, &QtJambiAPI::getPixmapOwner);" << Qt::endl;
                    }else{
                        s << INDENT << "registerOwnerFunction(typeId, &owner_function_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << ");" << Qt::endl;
                    }
                    usedTypeID = true;
                }

                if(!cls->isInterface()){
                    MetaFunctionList virtual_functions = cls->implementableFunctions();
                    if (!virtual_functions.isEmpty() && (entry->codeGeneration() & TypeEntry::GenerateNoShell)==0 && !cls->isFinal()){
                        usedTypeID = true;
                        s << INDENT << "registerFunctionInfos(typeId, {";
                        if (!virtual_functions.isEmpty()) {
                            int digits = 4;
                            if(virtual_functions.size()>99)
                                digits = 3;
                            else if(virtual_functions.size()>9)
                                digits = 2;
                            else
                                digits = 1;
                            INDENTATION(INDENT)
                            for (int i = 0; i < virtual_functions.size(); ++i) {
                                s << Qt::endl << INDENT << "FunctionInfo{/* " << QStringLiteral(u"%1").arg(QString::number(i), digits) << " */ \"" << virtual_functions.at(i)->name() << "\", \""
                                   << jni_signature(virtual_functions.at(i), JNISignatureFormat(NoModification | SlashesAndStuff))
                                   << "\"";
                                if(virtual_functions.at(i)->wasPrivate() && virtual_functions.at(i)->isAbstract()){
                                    s << ", FunctionInfo::Flags(FunctionInfo::Private | FunctionInfo::Abstract)";
                                }else if(virtual_functions.at(i)->isAbstract()){
                                    s << ", FunctionInfo::Abstract";
                                }
                                s << "}";
                                if(i < virtual_functions.size()-1)
                                    s << ",";
                            }
                        }
                        s << "});" << Qt::endl;
                    }
                    if(!cls->typeEntry()->isDestructorPrivate() && !cls->hasJustPrivateConstructors() && !cls->hasUnimplmentablePureVirtualFunction()){
                        bool hasDestructor = false;
                        if(!cls->isNamespace()
                                && !cls->typeEntry()->isDestructorPrivate()
                                && !cls->hasJustPrivateConstructors()
                                && !cls->hasUnimplmentablePureVirtualFunction()
                                && (cls->generateShellClass() || cls->typeEntry()->isDestructorPublic())
                                ){
                            hasDestructor = true;
                        }
                        usedTypeID = true;
                        s << INDENT << "registerConstructorInfos(typeId, " << cls->returnScopeRequired();
                        if(hasDestructor)
                            s << ", &__qt_destruct_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_');
                        else
                            s << ", nullptr";
                        s << ", {";
                        int counter = 0;
                        const MetaFunction *standardConstructor = nullptr;
                        for(const MetaFunction *function : cls->functions()) {
                            if (function->isConstructor()
                                    && !function->isModifiedRemoved(TS::NativeCode)
                                    && !function->isModifiedRemoved(TS::TargetLangCode)
                                    && !function->isPrivate()
                                    && !function->hasUnresolvedTemplateTypes()){
                                if (function->actualMinimumArgumentCount()==0) {
                                    standardConstructor = function;
                                }
                                QStringList pps = getFunctionPPConditions(function);
                                if(!cls->typeEntry()->ppCondition().isEmpty()){
                                    pps.removeAll(cls->typeEntry()->ppCondition());
                                }
                                if(!pps.isEmpty()){
                                    s << Qt::endl << "#if " << pps.join(QStringLiteral(u" && "));
                                }
                                const QList<ArgumentList>& argumentLists = m_nativeConstructorNames[function];
                                for(const ArgumentList& argumentList : argumentLists){
                                    s << Qt::endl << INDENT << "   ";
                                    if(counter++>0){
                                        s << ",";
                                    }else{
                                        s << " ";
                                    }
                                    s << "ConstructorInfo{&__qt_construct_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << argumentList.marshalledArguments << ", ";
                                    if(argumentList.jniSignature.isEmpty())
                                        s << "nullptr}";
                                    else
                                        s << "\"" << argumentList.jniSignature << "\"}";
                                }
                                if(!pps.isEmpty()){
                                    s << Qt::endl << "#endif // " << pps.join(QStringLiteral(u" && "));
                                }
                            }
                        }
                        if(cls->isQObject() && !cls->isFinal() && standardConstructor){
                            QStringList pps = getFunctionPPConditions(standardConstructor);
                            if(!cls->typeEntry()->ppCondition().isEmpty()){
                                pps.removeAll(cls->typeEntry()->ppCondition());
                            }
                            if(!pps.isEmpty()){
                                s << Qt::endl << "#if " << pps.join(QStringLiteral(u" && "));
                            }
                            s << Qt::endl << INDENT << "   ";
                            if(counter>0)
                                s << ",";
                            else
                                s << " ";
                            s << "ConstructorInfo{&__qt_construct_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "_declarative, \"Lio/qt/core/QObject$QDeclarativeConstructor;\"}";
                            if(!pps.isEmpty()){
                                s << Qt::endl << "#endif // " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                            }
                        }
                        s << Qt::endl << INDENT << "});" << Qt::endl;
                    }
                }

                if(entry->isQObject() || cls->has_Q_GADGET()){
                    QString renamedMethods;
                    {
                        QTextStream s(&renamedMethods);
                        for (int i = 0; i < signalsInTargetLang.size(); ++i) {
                            const MetaFunction *signal = signalsInTargetLang.at(i);
                            if(signal->originalName()!=signal->name()){
                                QStringList pps = getFunctionPPConditions(signal);
                                if(!signal->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(signal->declaringClass()->typeEntry()->ppCondition())){
                                    if(pps.isEmpty() || !signal->declaringClass()->typeEntry()->ppCondition().contains(QStringLiteral(u"|")))
                                        pps << signal->declaringClass()->typeEntry()->ppCondition();
                                    else
                                        pps << "(" + signal->declaringClass()->typeEntry()->ppCondition() + ")";
                                }
                                pps.removeAll(cls->typeEntry()->ppCondition());
                                if(!pps.isEmpty())
                                    s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                s << "if(methodIndex==__signal_method_indexes_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "_" << i << "()){ // " << signal->signature() << Qt::endl
                                  << "    return \"" << signal->name() << "\";" << Qt::endl
                                  << "}"<< Qt::endl;
                                if(!pps.isEmpty())
                                    s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                            }
                        }
                        const auto functionsInTargetLang = cls->functionsInTargetLang();
                        for(const MetaFunction *function : functionsInTargetLang) {
                            if((function->isSlot() || function->isInvokable()) && !function->isSignal()){
                                if(function->originalName()!=function->name()){
                                    QStringList pps = getFunctionPPConditions(function);
                                    if(!function->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(function->declaringClass()->typeEntry()->ppCondition())){
                                        if(pps.isEmpty() || !function->declaringClass()->typeEntry()->ppCondition().contains("|"))
                                            pps << function->declaringClass()->typeEntry()->ppCondition();
                                        else
                                            pps << "(" + function->declaringClass()->typeEntry()->ppCondition() + ")";
                                    }
                                    pps.removeAll(cls->typeEntry()->ppCondition());
                                    if(!pps.isEmpty())
                                        s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                    s << "if(methodIndex==" << qtName << "::staticMetaObject.indexOfMethod(\""
                                      << QMetaObject::normalizedSignature(qPrintable(function->signature(true))) << "\")){" << Qt::endl
                                      << "    return \"" << function->name() << "\";" << Qt::endl
                                      << "}" << Qt::endl;
                                    if(!pps.isEmpty())
                                        s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                }
                            }
                        }
                    }
                    const ObjectTypeEntry *oentry = entry->isQObject() ? static_cast<const ObjectTypeEntry *>(entry) : nullptr;
                    bool hasInvokables = false;
                    const auto functionsInTargetLang = cls->functionsInTargetLang();
                    for(const MetaFunction *function : functionsInTargetLang) {
                        if((function->isSlot() || function->isInvokable()) && !function->isSignal()){
                            hasInvokables = true;
                            break;
                        }
                    }
                    if((signalsInTargetLang.size()>0 || hasInvokables) && (cls->has_Q_OBJECT() || cls->has_Q_GADGET())){
                        int digits = 4;
                        if(signalsInTargetLang.size()>99)
                            digits = 3;
                        else if(signalsInTargetLang.size()>9)
                            digits = 2;
                        else
                            digits = 1;
                        usedTypeID = true;
                        s << INDENT << "registerMetaObject(typeId, " << qtName << "::staticMetaObject, " << (oentry && oentry->isValueOwner() ? "true," : "false,") << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            if(signalsInTargetLang.size()==0){
                                s << INDENT << "    {}," << Qt::endl;
                            }else{
                                INDENTATION(INDENT)
                                s << INDENT << "{";
                                int signalCounter = 0;


                                QList<QString> signalNames;
                                QMap<QString,QList<const MetaFunction*>> sortedSignals;
                                for(const MetaFunction *function : signalsInTargetLang){
                                    QString key = function->declaringClass()->typeEntry()->qualifiedCppName() + "::" + function->name();
                                    if(!signalNames.contains(key))
                                        signalNames.append(key);
                                    sortedSignals[key].append(function);
                                }
                                for(const QString& key : signalNames){
                                    const QList<const MetaFunction *>& list = sortedSignals[key];
                                    for (int i = 0; i < list.size(); ++i) {
                                        const MetaFunction *f = list.at(i);
                                        if(f->isRemovedFrom(cls, TS::NativeCode))
                                            continue;
                                        MetaArgumentList arguments;
                                        for (MetaArgument* arg : f->arguments()) {
                                            if(!f->argumentRemoved(arg->argumentIndex()+1)){
                                                arguments << arg;
                                            }
                                        }

                                        QString signalSignature;
                                        if(f->isPrivateSignal()){
                                            signalSignature = "Lio/qt/core/QObject$PrivateSignal" + QString::number(arguments.size()) + ";";
                                        }else{
                                            signalSignature = "Lio/qt/core/QObject$Signal" + QString::number(arguments.size());
                                            int defaultCounter = 0;
                                            for (MetaArgument* arg : qAsConst(arguments)) {
                                                if(!arg->defaultValueExpression().isEmpty()){
                                                    defaultCounter++;
                                                }
                                            }
                                            if(defaultCounter>0){
                                                signalSignature += "Default" + QString::number(defaultCounter);
                                            }

                                            signalSignature += ";";
                                        }

                                        QString cppOriginalSignature;
                                        {
                                            QTextStream qtNameStream(&cppOriginalSignature);
                                            CppGenerator::writeFunctionSignature(qtNameStream, f, nullptr, QString(),
                                                                                 Option(SkipReturnType | SkipName | OriginalName | NormalizeAndFixTypeSignature | OriginalTypeDescription),
                                                                                 QString(), QStringList(), int(f->arguments().size()));
                                        }
                                        if(signalCounter>0)
                                            s << ",";
                                        {
                                            INDENTATION(INDENT)
                                            s << Qt::endl << INDENT << "SignalMetaInfo{/* "
                                               << QString("%1").arg(QString::number(signalCounter), digits) << " */ \"" << f->name() << "\", \""
                                               << signalSignature << "\", " << arguments.size() << ", __signal_method_indexes_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "_" << signalsInTargetLang.indexOf(f) << "}";
                                        }
                                        signalCounter++;
                                    }
                                }
                                s << Qt::endl;
                                s << INDENT << "}," << Qt::endl;
                            }
                        }

                        {
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "[](const QMetaMethod& method, QList<ParameterInfo>& infos) {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << "Q_UNUSED(infos)" << Qt::endl;
                                    s << INDENT << "int methodIndex = method.methodIndex();" << Qt::endl;
                                    for (int i = 0; i < signalsInTargetLang.size(); ++i) {
                                        const MetaFunction *signal = signalsInTargetLang.at(i);
                                        if(signal->isRemovedFrom(cls, TS::NativeCode))
                                            continue;
                                        QStringList pps = getFunctionPPConditions(signal);
                                        if(!signal->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(signal->declaringClass()->typeEntry()->ppCondition())){
                                            if(pps.isEmpty() || !signal->declaringClass()->typeEntry()->ppCondition().contains("|"))
                                                pps << signal->declaringClass()->typeEntry()->ppCondition();
                                            else
                                                pps << "(" + signal->declaringClass()->typeEntry()->ppCondition() + ")";
                                        }
                                        pps.removeAll(cls->typeEntry()->ppCondition());
                                        if(!pps.isEmpty())
                                            s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                        s << INDENT << "if(methodIndex==__signal_method_indexes_" << cls->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "_" << i << "()){ // " << signal->signature() << Qt::endl;
                                        {
                                            INDENTATION(INDENT)
                                            writeArgumentConversion(s, signal, converterFunctions, forwardDeclarations);
                                            s << INDENT << "return true;" << Qt::endl;
                                        }
                                        s << INDENT << "}" << Qt::endl;
                                        if(!pps.isEmpty())
                                            s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                    }
                                    for(const MetaFunction *function : cls->functionsInTargetLang()) {
                                        if((function->isSlot() || function->isInvokable()) && !function->isSignal()){
                                            QStringList pps = getFunctionPPConditions(function);
                                            if(!function->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(function->declaringClass()->typeEntry()->ppCondition())){
                                                if(pps.isEmpty() || !function->declaringClass()->typeEntry()->ppCondition().contains("|"))
                                                    pps << function->declaringClass()->typeEntry()->ppCondition();
                                                else
                                                    pps << "(" + function->declaringClass()->typeEntry()->ppCondition() + ")";
                                            }
                                            pps.removeAll(cls->typeEntry()->ppCondition());
                                            if(!pps.isEmpty())
                                                s << "#if " << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                            s << INDENT << "if(methodIndex==" << qtName << "::staticMetaObject.indexOfMethod(\"";
                                            s << QMetaObject::normalizedSignature(qPrintable(function->signature(true))) << "\")){" << Qt::endl;
                                            {
                                                INDENTATION(INDENT)
                                                writeArgumentConversion(s, function, converterFunctions, forwardDeclarations);
                                                s << INDENT << "return true;" << Qt::endl;
                                            }
                                            s << INDENT << "}" << Qt::endl;
                                            if(!pps.isEmpty())
                                                s << "#endif //" << pps.join(QStringLiteral(u" && ")) << Qt::endl;
                                        }
                                    }
                                    s << INDENT << "return false;" << Qt::endl;
                                }
                                s << INDENT << "}";
                            }
                        }
                        if(!renamedMethods.isEmpty()){
                            s << "," << Qt::endl;
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "[](int methodIndex) -> const char* {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    for(const QString& line : renamedMethods.split(QStringLiteral(u"\n"))){
                                        if(!line.startsWith("#"))
                                            s << INDENT;
                                        s << line << Qt::endl;
                                    }
                                    s << INDENT << "return nullptr;" << Qt::endl;
                                }
                                s << INDENT << "}";
                            }
                        }
                        s << ");" << Qt::endl;
                    }else if(cls->has_Q_OBJECT() || cls->has_Q_GADGET()){
                        usedTypeID = true;
                        s << INDENT << "registerMetaObject(typeId, " << qtName << "::staticMetaObject, " << (oentry && oentry->isValueOwner() ? "true" : "false");
                        if(!renamedMethods.isEmpty()){
                            s << "," << Qt::endl;
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "[](int methodIndex) -> const char* {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    for(const QString& line : renamedMethods.split(QStringLiteral(u"\n"))){
                                        if(!line.startsWith("#"))
                                            s << INDENT;
                                        s << line << Qt::endl;
                                    }
                                    s << INDENT << "return nullptr;" << Qt::endl;
                                }
                                s << INDENT << "}";
                            }
                        }
                        s << ");" << Qt::endl;
                    }else if(entry->isQObject() && cls->hasMetaObjectFunction()){
                        usedTypeID = true;
                        s << INDENT << "registerCustomMetaObject(typeId, " << qtName << "::staticMetaObject);" << Qt::endl;
                    }
                }

                if (hasDeleter(cls)){
                    usedTypeID = true;
                    s << INDENT << "registerDeleter(typeId, &deleter_" << entry->qualifiedCppName().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << ");" << Qt::endl;
                }
                if(!metaTypeRegistration.isEmpty()){
                    s << metaTypeRegistration;
                }
                if(cls->instantiateShellClass()){
                    usedTypeID = true;
                    s << INDENT << "registerSizeOfShell(typeId, sizeof(" << shellClassName(cls) << "));" << Qt::endl;
                }

                QSet<QString> registeredTypes;
                registeredTypes << QStringLiteral(u"QMap<QString, QVariant>");
                registeredTypes << QStringLiteral(u"QHash<QString, QVariant>");
                registeredTypes << QStringLiteral(u"QList<QString>");
                for(const MetaFunction *function : cls->functions()) {
                    if(function->declaringClass()==cls){
                        if(function->isSlot() || function->isInvokable() || function->isSignal()){
                            if(function->type()){
                                if((function->type()->isContainer() || function->type()->isSmartPointer())
                                        && !function->type()->instantiations().isEmpty()
                                        && function->type()->indirections().isEmpty()){
                                    QString qtName;
                                    QTextStream _s(&qtName);
                                    writeTypeInfo(_s, function->type(), Option(ForceValueType | NoSpace));
                                    if(!registeredTypes.contains(qtName)){
                                        registeredTypes.insert(qtName);
                                        s << INDENT << "registerMetaType<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                                    }
                                }
                            }
                            for(MetaArgument* argument : function->arguments()){
                                if(!function->argumentRemoved(argument->argumentIndex()+1)){
                                    if((argument->type()->isContainer() || argument->type()->isSmartPointer())
                                            && !argument->type()->instantiations().isEmpty()
                                            && argument->type()->indirections().isEmpty()){
                                        QString qtName;
                                        QTextStream _s(&qtName);
                                        writeTypeInfo(_s, argument->type(), Option(ForceValueType | NoSpace));
                                        if(!registeredTypes.contains(qtName)){
                                            registeredTypes.insert(qtName);
                                            s << INDENT << "registerMetaType<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                QList<const MetaType *> instantiations;
                if(const ContainerTypeEntry* ctype = cls->findContainerSuperClass(&instantiations)){
                    QString accessName = ctype->qualifiedCppName();
                    switch(ctype->type()){
                    case ContainerTypeEntry::StackContainer:
                        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6, 0, 0)){
                            accessName = QStringLiteral(u"QVector");
                            break;
                        }
                        Q_FALLTHROUGH();
                    case ContainerTypeEntry::QueueContainer:
                        accessName = QStringLiteral(u"QList");
                        break;
                    default: break;
                    }
                    s << INDENT << "registerContainerAccessFactory(typeId, NewContainerAccessFunction(&QtJambiPrivate::" << accessName << "Access<";
                    int i=0;
                    for(const MetaType * instantiation : instantiations){
                        if(i>0)
                            s << ", ";
                        writeTypeInfo(s, instantiation, SkipName);
                        ++i;
                    }
                    s << ">::newInstance));" << Qt::endl;
                }
                for(MetaFunction * function : cls->functions()){
                    if (
                        !function->isEmptyFunction()
                        && !function->hasUnresolvedTemplateTypes()
                        && !function->isRemovedFromAllLanguages(cls)
                    ){
                        QList<const MetaType*> types;
                        if(!function->isConstructor() && function->type())
                            types << function->type();
                        for(MetaArgument* arg : function->arguments()){
                            types << arg->type();
                        }
                        while(!types.isEmpty()){
                            const MetaType* type = types.takeFirst();
                            for(const MetaType* instantiation : type->instantiations()){
                                types << instantiation;
                            }
                            if(type->typeEntry()->isContainer()){
                                const ContainerTypeEntry* ctype = reinterpret_cast<const ContainerTypeEntry*>(type->typeEntry());
                                static const QHash<ContainerTypeEntry::Type, QPair<int,QString>> containerTypes{
                                    {ContainerTypeEntry::ListContainer,{1,QStringLiteral(u"QList")}},
                                    {ContainerTypeEntry::LinkedListContainer,{1,QStringLiteral(u"QLinkedList")}},
                                    {ContainerTypeEntry::SetContainer,{1,QStringLiteral(u"QSet")}},
                                    {ContainerTypeEntry::VectorContainer,{1,QStringLiteral(u"QVector")}},
                                    {ContainerTypeEntry::MapContainer,{2,QStringLiteral(u"QMap")}},
                                    {ContainerTypeEntry::HashContainer,{2,QStringLiteral(u"QHash")}},
                                    {ContainerTypeEntry::MultiMapContainer,{2,QStringLiteral(u"QMultiMap")}},
                                    {ContainerTypeEntry::MultiHashContainer,{2,QStringLiteral(u"QMultiHash")}}
                                };
                                if(containerTypes.contains(ctype->type())){
                                    const QPair<int,QString>& data = containerTypes[ctype->type()];
                                    if(type->instantiations().size()==data.first){
                                        bool onlyPointers = true;
                                        for(const MetaType* instantiation : type->instantiations()){
                                            onlyPointers &= !instantiation->indirections().isEmpty();
                                        }
                                        if(onlyPointers){

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if(cls->isQObject()){
                    if(!cls->has_Q_OBJECT()){
                        MetaClass *baseClass = cls->baseClass();
                        /*while(baseClass && !baseClass->has_Q_OBJECT()){
                            baseClass = baseClass->baseClass();
                        }*/
                        if(baseClass){
                            usedTypeID = true;
                            s << INDENT << "registerPolymorphyHandler(typeid(" << baseClass->qualifiedCppName()
                              << "), typeId, [](void *ptr, qintptr& offset) -> bool {" << Qt::endl
                              << INDENT << "        " << baseClass->qualifiedCppName() << " *object = reinterpret_cast<" << baseClass->qualifiedCppName() << " *>(ptr);" << Qt::endl
                              << INDENT << "        Q_ASSERT(object);" << Qt::endl
                              << INDENT << "        offset = 0;" << Qt::endl
                              << INDENT << "        bool _result = false;" << Qt::endl
                              << INDENT << "        QTJAMBI_TRY_ANY { // cast from baseclass" << Qt::endl
                              << INDENT << "            _result = dynamic_cast<" << cls->qualifiedCppName() << "*>(object);" << Qt::endl
                              << INDENT << "        } QTJAMBI_CATCH_ANY{" << Qt::endl
                              << INDENT << "        } QTJAMBI_TRY_END" << Qt::endl
                              << INDENT << "        return _result;" << Qt::endl
                              << INDENT << "    });" << Qt::endl;
                        }
                    }
                }else if(!entry->polymorphicIdValue().isEmpty()) {
                    MetaClass *baseClass = cls->baseClass();
                    while(baseClass && !baseClass->typeEntry()->isPolymorphicBase()){
                        baseClass = baseClass->baseClass();
                    }
                    if(baseClass && baseClass->typeEntry()->isPolymorphicBase()){
                        MetaClass *polymorphicBaseClass = baseClass;
                        usedTypeID = true;
                        baseClass = cls->baseClass();
                        while(baseClass){
                            s << INDENT << "registerPolymorphyHandler(typeid(" << baseClass->qualifiedCppName()
                              << "), typeId, [](void *ptr, qintptr& offset) -> bool {" << Qt::endl
                              << INDENT << "        " << baseClass->qualifiedCppName() << " *object = reinterpret_cast<" << baseClass->qualifiedCppName() << " *>(ptr);" << Qt::endl
                              << INDENT << "        Q_ASSERT(object);" << Qt::endl
                              << INDENT << "        offset = 0;" << Qt::endl
                              << INDENT << "        bool _result = false;" << Qt::endl
                              << INDENT << "        QTJAMBI_TRY_ANY { // cast from baseclass" << Qt::endl
                              << INDENT << "            _result = " << QString(entry->polymorphicIdValue()).replace("%1", "object") << ";" << Qt::endl
                              << INDENT << "        } QTJAMBI_CATCH_ANY{" << Qt::endl
                              << INDENT << "        } QTJAMBI_TRY_END" << Qt::endl
                              << INDENT << "        return _result;" << Qt::endl
                              << INDENT << "    });" << Qt::endl;
                            if(baseClass==polymorphicBaseClass)
                                break;
                            baseClass = baseClass->baseClass();
                        }
                    }
                }else {
                    MetaClass * baseClass = cls->baseClass();
                    bool hasBaseClassVirtuals = false;
                    while(baseClass){
                        if(baseClass->hasVirtuals()){
                            hasBaseClassVirtuals = true;
                            break;
                        }
                        baseClass = baseClass->baseClass();
                    }
                    if(hasBaseClassVirtuals){
                        s << INDENT << "registerDefaultPolymorphyHandler<" << cls->baseClass()->qualifiedCppName() << ", " << cls->qualifiedCppName() << ">();" << Qt::endl;
                    }else{
                        baseClass = cls->baseClass();
                        while(baseClass){
                            if(baseClass->typeEntry()->isPolymorphicBase()){
                                QString warning = QString("class '%1' inherits from polymorphic class '%2', but has no polymorphic id set")
                                                  .arg(cls->name(), baseClass->name());
                                ReportHandler::warning(warning);
                                break;
                            }
                            baseClass = baseClass->baseClass();
                        }
                    }
                }

                for(MetaClass *interface : cls->interfaces()) {
                    bool hasBaseClassVirtuals = false;
                    {
                        MetaClass * baseClass = interface;
                        while(baseClass){
                            if(baseClass->hasVirtuals()){
                                hasBaseClassVirtuals = true;
                                break;
                            }
                            baseClass = baseClass->baseClass();
                        }
                    }
                    if(interface->primaryInterfaceImplementor()==cls){ // make interface convertible to QObject
                        if(hasBaseClassVirtuals){
                            usedTypeID = true;
                            s << INDENT << "registerPolymorphyHandler(typeid(QObject), typeId, [](void *ptr, qintptr& offset) -> bool {" << Qt::endl
                              << INDENT << "        QObject *object = reinterpret_cast<QObject *>(ptr);" << Qt::endl
                              << INDENT << "        Q_ASSERT(object);" << Qt::endl
                              << INDENT << "        bool _result = false;" << Qt::endl
                              << INDENT << "        QTJAMBI_TRY_ANY {" << Qt::endl
                              << INDENT << "            if(" << cls->qualifiedCppName() << "* _object = dynamic_cast<" << cls->qualifiedCppName() << "*>(object)){" << Qt::endl
                              << INDENT << "                offset = qintptr(object)-qintptr(_object);" << Qt::endl
                              << INDENT << "                _result = true;" << Qt::endl
                              << INDENT << "            }" << Qt::endl
                              << INDENT << "        } QTJAMBI_CATCH_ANY{" << Qt::endl
                              << INDENT << "        } QTJAMBI_TRY_END" << Qt::endl
                              << INDENT << "        return _result;" << Qt::endl
                              << INDENT << "    });" << Qt::endl
                              << INDENT << "registerPolymorphyHandler(typeId, typeid(QObject), [](void *ptr, qintptr& offset) -> bool {" << Qt::endl
                              << INDENT << "        " << cls->qualifiedCppName() << " *object = reinterpret_cast<" << cls->qualifiedCppName() << " *>(ptr);" << Qt::endl
                              << INDENT << "        Q_ASSERT(object);" << Qt::endl
                              << INDENT << "        bool _result = false;" << Qt::endl
                              << INDENT << "        QTJAMBI_TRY_ANY {" << Qt::endl
                              << INDENT << "            if(QObject* _object = dynamic_cast<QObject*>(object)){" << Qt::endl
                              << INDENT << "                offset = qintptr(object)-qintptr(_object);" << Qt::endl
                              << INDENT << "                _result = true;" << Qt::endl
                              << INDENT << "            }" << Qt::endl
                              << INDENT << "        } QTJAMBI_CATCH_ANY{" << Qt::endl
                              << INDENT << "        } QTJAMBI_TRY_END" << Qt::endl
                              << INDENT << "        return _result;" << Qt::endl
                              << INDENT << "    });" << Qt::endl;
                            if(!(cls->typeEntry()->isThreadAffine())){
                                s << INDENT << "registerOwnerFunction(typeId, [](const void *ptr)->const QObject*{" << Qt::endl
                                  << INDENT << "    const " << cls->qualifiedCppName() << " *object = reinterpret_cast<const " << cls->qualifiedCppName() << " *>(ptr);" << Qt::endl
                                  << INDENT << "    const QObject* _result = nullptr;" << Qt::endl
                                  << INDENT << "    QTJAMBI_TRY_ANY {" << Qt::endl
                                  << INDENT << "        _result = dynamic_cast<const QObject*>(object);" << Qt::endl
                                  << INDENT << "    } QTJAMBI_CATCH_ANY{" << Qt::endl
                                  << INDENT << "    } QTJAMBI_TRY_END" << Qt::endl
                                  << INDENT << "    return _result;" << Qt::endl
                                  << INDENT << "});" << Qt::endl;
                            }
                        }
                    }else{
                        QString polymorphicId = entry->interfacePolymorphicIdValues().value(interface->qualifiedCppName());
                        if (polymorphicId.isEmpty()) {
                            if(hasBaseClassVirtuals){
                                usedTypeID = true;
                                s << INDENT << "registerPolymorphyHandler(typeid(" << interface->qualifiedCppName()
                                  << "), typeId, [](void *ptr, qintptr& offset) -> bool {" << Qt::endl
                                  << INDENT << "        " << interface->qualifiedCppName() << " *object = reinterpret_cast<" << interface->qualifiedCppName() << " *>(ptr);" << Qt::endl
                                  << INDENT << "        Q_ASSERT(object);" << Qt::endl
                                  << INDENT << "        bool _result = false;" << Qt::endl
                                  << INDENT << "        QTJAMBI_TRY_ANY {" << Qt::endl
                                  << INDENT << "            if(" << cls->qualifiedCppName() << "* _object = dynamic_cast<" << cls->qualifiedCppName() << "*>(object)){" << Qt::endl
                                  << INDENT << "                offset = qintptr(object)-qintptr(_object);" << Qt::endl
                                  << INDENT << "                _result = true;" << Qt::endl
                                  << INDENT << "            }" << Qt::endl
                                  << INDENT << "        } QTJAMBI_CATCH_ANY{" << Qt::endl
                                  << INDENT << "        } QTJAMBI_TRY_END" << Qt::endl
                                  << INDENT << "        return _result;" << Qt::endl
                                  << INDENT << "    });" << Qt::endl;
                            }else if(!cls->isAbstract()
                                    && (!cls->baseClass() || !cls->baseClass()->inheritsFromInterface(interface))
                                    && cls->name()!=interface->name()+"$Impl"){
                                QString warning = QString("class '%1' inherits from interface '%2', but has no polymorphic id set")
                                                          .arg(cls->qualifiedCppName(), interface->qualifiedCppName());
                                ReportHandler::warning(warning);
                            }
                        }else{
                            if(QString(polymorphicId).trimmed()!="false"){
                                usedTypeID = true;
                                // On first find, open the function
                                s << INDENT << "registerPolymorphyHandler(typeid(" << interface->qualifiedCppName()
                                  << "), typeId, [](void *ptr, qintptr& offset) -> bool {" << Qt::endl
                                  << INDENT << "        " << interface->qualifiedCppName() << " *object = reinterpret_cast<" << interface->qualifiedCppName() << " *>(ptr);" << Qt::endl
                                  << INDENT << "        Q_ASSERT(object);" << Qt::endl
                                  << INDENT << "        bool _result = false;" << Qt::endl
                                  << INDENT << "        QTJAMBI_TRY_ANY {" << Qt::endl
                                  << INDENT << "            if(" << polymorphicId.replace("%1", "object") << "){" << Qt::endl;
                                if(hasBaseClassVirtuals){
                                    s << INDENT << "                offset = qintptr(object)-qintptr(dynamic_cast<" << cls->qualifiedCppName() << "*>(object));" << Qt::endl;
                                }else{
                                    s << INDENT << "                offset = qintptr(object);" << Qt::endl;
                                }
                                s << INDENT << "                _result = true;" << Qt::endl
                                  << INDENT << "            }" << Qt::endl
                                  << INDENT << "        }QTJAMBI_CATCH_ANY{" << Qt::endl
                                  << INDENT << "        } QTJAMBI_TRY_END" << Qt::endl
                                  << INDENT << "        return _result;" << Qt::endl
                                  << INDENT << "    });" << Qt::endl;
                            }
                        }
                    }
                }

                {
                    const MetaClass *parent = cls;
                    while(parent){
                        for(MetaClass *interface : parent->interfaces()) {
                            if(entry->designatedInterface()!=interface->typeEntry()){
                                usedTypeID = true;
                                s << INDENT << "registerInterfaceOffset(typeId, typeid(" << interface->qualifiedCppName() << "), int(reinterpret_cast<quintptr>(static_cast<" << interface->qualifiedCppName() << " *>(reinterpret_cast<" << qtName << " *>(0x10000000)))) - 0x10000000);" << Qt::endl;
                            }
                        }
                        parent = parent->baseClass();
                    }
                }
                if(!usedTypeID)
                    s << INDENT << "Q_UNUSED(typeId)" << Qt::endl;
            }else if(cls->isNamespace() && !cls->isFake()){
                s << INDENT << "registerNamespaceTypeInfo(\"" << qtName << "\", \"" << javaTypeName;
                if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6, 0, 0)
                    && reinterpret_cast<const NamespaceTypeEntry*>(cls->typeEntry())->hasMetaObject())
                    s << "\", &" << qtName << "::staticMetaObject);";
                else s <<"\", nullptr);";
                s << Qt::endl;
            }

            for(MetaEnum *cpp_enum : cls->enums()){
                writeMetaInfo(s, cpp_enum);
            }
            if(entry->designatedInterface()){
                for(MetaClass* interface : cls->interfaces()){
                    if(entry->designatedInterface()==interface->typeEntry()){
                        for(MetaEnum *cpp_enum : interface->enums()){
                            writeMetaInfo(s, cpp_enum);
                        }
                        break;
                    }
                }
            }
            writeCodeInjections(s, cls->typeEntry(), CodeSnip::End, TS::MetaInfo);
            if(!cls->typeEntry()->ppCondition().isEmpty()){
                if(cls->typeEntry()->isNativeInterface()){
                    s << Qt::endl << "#else //" << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl
                      << INDENT << "registerNativeInterface(\"" << javaTypeName << "\", {});" << Qt::endl;
                }
                s << Qt::endl << "#endif //" << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }

            s << "}" << Qt::endl;
        }
    }
    s << Qt::endl;
    if(!forwardDeclarations.isEmpty()){
        QList<QString> _forwardDeclarations = forwardDeclarations.values();
        std::sort(_forwardDeclarations.begin(), _forwardDeclarations.end());
        for(const QString& forwardDeclaration : _forwardDeclarations){
            s << forwardDeclaration << Qt::endl;
        }
        s << Qt::endl;
    }
    if(!converterFunctions.isEmpty()){
        std::sort(converterFunctions.begin(), converterFunctions.end());
        for(const QString& converterFunction : converterFunctions){
            s << converterFunction << Qt::endl;
        }
        s << Qt::endl;
    }
    s << code;
}

void CppImplGenerator::writeMetaInfo(QTextStream &s, const MetaEnum *enm){
    if (!enm)
        return;
    const EnumTypeEntry *entry = enm->typeEntry();
    if ((entry->codeGeneration() & ~TypeEntry::InheritedByTypeSystem) == TypeEntry::GenerateForSubclass)
        return;
    const MetaClass *owner = enm->enclosingClass();
    if(!owner)
        return;

    const QString qtEnumName = entry->qualifiedCppName();
    const QString javaEnumName = [owner,entry]()->QString{
        if(owner){
            if(owner->typeEntry()->isGlobal()){
                return entry->qualifiedTargetLangName().replace(".", "/");
            }else{
                if(owner->typeEntry()->javaPackage().isEmpty()){
                    return owner->typeEntry()->targetLangName() + "$" + entry->targetLangName();
                }else{
                    return owner->typeEntry()->javaPackage().replace(".", "/") + "/" + owner->typeEntry()->targetLangName() + "$" + entry->targetLangName();
                }
            }
        }else{
            return entry->targetLangName();
        }
    }();
    if(!enm->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << enm->typeEntry()->ppCondition() << Qt::endl;
    }
    if(enm->isProtected() || !owner->isPublic()){
        if(owner->typeEntry()->designatedInterface()){
            s << INDENT << owner->extractInterface()->name() << "_access::__registerEnumTypeInfo_" << enm->name().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "();" << Qt::endl;
        }else{
            s << INDENT << owner->name() << "_access::__registerEnumTypeInfo_" << enm->name().replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "();" << Qt::endl;
        }
    }else{
        if(const FlagsTypeEntry * fentry = entry->flags()){
            const QString qtFlagName = fentry->qualifiedCppName();
            const QString javaFlagName = [owner,fentry]()->QString{
                if(owner){
                    if(owner->typeEntry()->isGlobal()){
                        return fentry->qualifiedTargetLangName().replace(".", "/");
                    }else{
                        if(owner->typeEntry()->javaPackage().isEmpty()){
                            return owner->typeEntry()->targetLangName() + "$" + fentry->targetLangName();
                        }else{
                            return owner->typeEntry()->javaPackage().replace(".", "/") + "/" + owner->typeEntry()->targetLangName() + "$" + fentry->targetLangName();
                        }
                    }
                }else{
                    return fentry->targetLangName();
                }
            }();
            QString qtFlagsAliasName = fentry->flagsTemplate();
            s << INDENT << "registerEnumTypeInfo" << (entry->isHiddenMetaObject() ? "NoMetaObject" : "") << "<" << qtEnumName << ">(\"" << qtEnumName << "\", \"" << javaEnumName << "\", \"" << qtFlagName << "\", \"" << qtFlagsAliasName << "\", \"" << javaFlagName << "\");" << Qt::endl;
        }else{
            s << INDENT << "registerEnumTypeInfo" << (entry->isHiddenMetaObject() ? "NoMetaObject" : "") << "<" << qtEnumName << ">(\"" << qtEnumName << "\", \"" << javaEnumName << "\");" << Qt::endl;
        }
    }
    if(!enm->typeEntry()->ppCondition().isEmpty()){
        s << "#endif //" << enm->typeEntry()->ppCondition() << Qt::endl;
    }
}

void CppImplGenerator::writeMetaInfo(QTextStream &s, const MetaFunctional *functional){
    const QString qtName = functional->typeEntry()->qualifiedCppName();
    const QString javaName = [functional]()->QString{
        QString javaName = functional->name();
        if(functional->enclosingClass() && !functional->enclosingClass()->isFake()){
            if(functional->enclosingClass()->typeEntry()->designatedInterface()){
                javaName = functional->enclosingClass()->typeEntry()->designatedInterface()->javaPackage().replace(".", "/")
                        + "/"
                        + functional->enclosingClass()->typeEntry()->designatedInterface()->lookupName() + "$" + javaName;
            }else{
                javaName = functional->enclosingClass()->fullName() + "$" + javaName;
            }
        }else{
            javaName = functional->package().replace(".", "/")+"."+javaName;
        }
        javaName = javaName.replace(".", "/");
        return javaName;
    }();
    s << "void initialize_meta_info_" << QString(functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "(){" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "using namespace RegistryAPI;" << Qt::endl;
        if(!functional->typeEntry()->ppCondition().isEmpty()){
            s << Qt::endl << "#if " << functional->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
        }
        writeCodeInjections(s, functional->typeEntry(), CodeSnip::Beginning, TS::MetaInfo);
        QString registeredName;
        if(functional->typeEntry()->getUsing().isEmpty()){
            registeredName = functional->typeEntry()->normalizedSignature();
        }else{
            registeredName = functional->typeEntry()->name();
        }
        if(functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell){
            s << INDENT << "const char* qt_name = \"" << registeredName << "\";" << Qt::endl
              << INDENT << "const std::type_info& id = typeid(" << qtName << ");" << Qt::endl
              << INDENT << "registerFunctionalTypeInfo(id, QtJambiTypeInfo::of<" << qtName << ">(), qt_name, \"" << javaName << "\");" << Qt::endl
              << INDENT << "registerSizeOfType(id, sizeof(" << qtName << "));" << Qt::endl
              << INDENT << "registerAlignmentOfType(id, Q_ALIGNOF(" << qtName << "));" << Qt::endl
              << INDENT << "registerMetaTypeID(id, qRegisterMetaType<" << qtName << ">(qt_name));" << Qt::endl;
        }else{
            s << INDENT << "registerFunctionalTypeInfo<" << qtName << ", " << shellClassName(functional) << ">(\"" << registeredName << "\", \"" << javaName << "\"," << Qt::endl
              << INDENT << "                          " << (functional->needsReturnScope() ? "true" : "false") << "," << Qt::endl
              << INDENT << "                          &deleter_" << QString(functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "," << Qt::endl
              << INDENT << "                          &__qt_destruct_" << QString(functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << "," << Qt::endl
              << INDENT << "                          {ConstructorInfo{&__qt_construct_" << QString(functional->typeEntry()->name()).replace(QStringLiteral(u"::"), QStringLiteral(u"_")).replace(u'$', u'_').replace(u'>', u'_').replace(u'<', u'_') << ", nullptr}}," << Qt::endl
              << INDENT << "                          {FunctionInfo{\""
                                                                    << (functional->typeEntry()->functionName().isEmpty() ? QString("call") : functional->typeEntry()->functionName())
                                                                    << "\", \"" << jni_signature(functional, JNISignatureFormat(NoModification | SlashesAndStuff)) << "\", FunctionInfo::Abstract}});" << Qt::endl;
        }
        writeCodeInjections(s, functional->typeEntry(), CodeSnip::Position1, TS::MetaInfo);
        writeCodeInjections(s, functional->typeEntry(), CodeSnip::End, TS::MetaInfo);
        if(!functional->typeEntry()->ppCondition().isEmpty()){
            s << Qt::endl << "#endif //" << functional->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
        }
    }
    s << "}" << Qt::endl;
}
