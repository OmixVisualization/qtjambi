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
#include "cppimplgenerator.h"
#include "reporthandler.h"
#include "abstractmetabuilder.h"
#include "fileout.h"

#include <QDir>
#include <QtDebug>
#include <QVariant>
#include <QBuffer>
#include <QRandomGenerator>

#define VOID_POINTER_ORDINAL 8
#include "typesystem/typedatabase.h"

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

QString jni_signature(const AbstractMetaFunctional *function, JNISignatureFormat format) {
    QString returned = "(";
    const AbstractMetaArgumentList& arguments = function->arguments();
    for(const AbstractMetaArgument *argument : arguments) {
        if (function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
            if(function->argumentTypeArray(argument->argumentIndex() + 1)){
                QScopedPointer<AbstractMetaType> array(argument->type()->copy());
                array->setConstant(false);
                array->setReferenceType(AbstractMetaType::NoReference);
                array->setTypeUsagePattern(AbstractMetaType::ArrayPattern);
                QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(AbstractMetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                AbstractMetaBuilder::decideUsagePattern(cpy.get());
                array->setArrayElementType(cpy.get());
                returned += jni_signature(array.get(), format);
            }else if(function->argumentTypeBuffer(argument->argumentIndex() + 1)){
                QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(AbstractMetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                AbstractMetaBuilder::decideUsagePattern(cpy.get());
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
            }else{
                QString jniType;
                QString modified_type = function->typeReplaced(argument->argumentIndex() + 1, &jniType);

                if (modified_type.isEmpty())
                    returned += jni_signature(argument->type(), format);
                else if(jniType.isEmpty())
                    returned += jni_signature(modified_type, format);
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
        returned += jni_signature(modified_type, JNISignatureFormat(format | ReturnType));
    else
        returned += jni_signature(jniType, JNISignatureFormat(format | ReturnType));
    return returned;
}

QString jni_signature(const AbstractMetaFunction *function, JNISignatureFormat format) {
    QString returned = "(";
    const AbstractMetaArgumentList& arguments = function->arguments();
    for(const AbstractMetaArgument *argument : arguments) {
        if (function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
            if(function->argumentTypeArray(argument->argumentIndex() + 1)){
                QScopedPointer<AbstractMetaType> array(argument->type()->copy());
                array->setConstant(false);
                array->setReferenceType(AbstractMetaType::NoReference);
                array->setTypeUsagePattern(AbstractMetaType::ArrayPattern);
                QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(AbstractMetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                AbstractMetaBuilder::decideUsagePattern(cpy.get());
                array->setArrayElementType(cpy.get());
                returned += jni_signature(array.get(), format);
            }else if(function->argumentTypeBuffer(argument->argumentIndex() + 1)){
                QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(AbstractMetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                AbstractMetaBuilder::decideUsagePattern(cpy.get());
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
            }else{
                QString jniType;
                QString modified_type = function->typeReplaced(argument->argumentIndex() + 1, &jniType);
                if (modified_type.isEmpty())
                    returned += jni_signature(argument->type(), format);
                else if(jniType.isEmpty())
                    returned += jni_signature(modified_type, format);
                else
                    returned += jni_signature(jniType, format);
            }
        }
    }

    for(const ArgumentModification *mod : function->addedArguments()){
        returned += jni_signature(mod->modified_type, format);
    }

    returned += ")";

    QString jniType;
    QString modified_type = function->typeReplaced(0, &jniType);
    if (modified_type.isEmpty())
        returned += jni_signature(function->type(), JNISignatureFormat(format | ReturnType));
    else if(jniType.isEmpty())
        returned += jni_signature(modified_type, JNISignatureFormat(format | ReturnType));
    else
        returned += jni_signature(jniType, JNISignatureFormat(format | ReturnType));

    return returned;
}

QString jni_signature(const QString &_full_name, JNISignatureFormat format) {
    QString signature;
    QString full_name = _full_name;

    if (full_name.endsWith("...")) {
        full_name.chop(3);
        signature = "[";
    }
    while(full_name.endsWith("[]")) {
        full_name.chop(2);
        signature = "[";
    }

    int start = 0, end = -1;
    while ((start = full_name.indexOf("<")) >= 0 && (end = full_name.indexOf(">")) >= 0) {
        full_name.remove(start, end - start + 1);
    }

    static QHash<QString, QString> table;
    if (table.isEmpty()) {
        table["boolean"] = "Z";
        table["byte"] = "B";
        table["char"] = "C";
        table["short"] = "S";
        table["int"] = "I";
        table["long"] = "J";
        table["float"] = "F";
        table["double"] = "D";
    }

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

QString jni_signature(const AbstractMetaType *java_type, JNISignatureFormat format) {
    if (!java_type)
        return "V";

    if(( (format & ReturnType) != ReturnType )
            && java_type->typeEntry()->isContainer()
            && (java_type->typeEntry()->qualifiedTargetLangName()=="java.util.List"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.LinkedList"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.ArrayList"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Vector"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Queue"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Deque"
            ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Set")){
        return jni_signature("java.util.Collection", format);
    }else if(( (format & ReturnType) != ReturnType )
             && java_type->typeEntry()->isContainer()
             && (java_type->typeEntry()->qualifiedTargetLangName()=="java.util.Map"
             ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.SortedMap"
             ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.HashMap"
             ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.TreeMap"
             ||  java_type->typeEntry()->qualifiedTargetLangName()=="java.util.NavigableMap")){
         return jni_signature("java.util.Map", format);
     }else if (java_type->isArray()) {
        if ((format & Underscores) == Underscores)
            return "_3" + jni_signature(java_type->arrayElementType(), format);
        else
            return "[" + jni_signature(java_type->arrayElementType(), format);
    } else if (java_type->isInitializerList() && java_type->instantiations().size()==1) {
        const AbstractMetaType* instantiation = java_type->instantiations()[0];
        if (instantiation->isTargetLangEnum() || instantiation->isTargetLangFlags()) {
            QString name = instantiation->name();
            if (instantiation->isTargetLangEnum()) {
                const EnumTypeEntry *et = static_cast<const EnumTypeEntry *>(instantiation->typeEntry());
                name = et->javaQualifier() + "$" + et->targetLangName();

            } else if (instantiation->isTargetLangFlags()) {
                const FlagsTypeEntry *ft = static_cast<const FlagsTypeEntry *>(instantiation->typeEntry());
                name = ft->originator()->javaQualifier() + "$" + ft->targetLangName();
            }
            return jni_signature((instantiation->package().isEmpty() ? QString() : instantiation->package() + ".") + name + "[]", format);
        }
        if ((format & Underscores) == Underscores)
            return "_3" + jni_signature(instantiation, format);
        else
            return "[" + jni_signature(instantiation, format);
    } else if (java_type->isPointerContainer() && java_type->instantiations().size()==1) {
        AbstractMetaType* instantiation = java_type->instantiations()[0]->copy();
        instantiation->setIndirections(QList<bool>(instantiation->indirections()) << false);
        AbstractMetaBuilder::decideUsagePattern(instantiation);
        return jni_signature(instantiation, format);
    } else if (java_type->isNativePointer()) {
        if ((format & Underscores) == Underscores)
            return "Lio_qt_QNativePointer_2";
        else
            return "Lio/qt/QNativePointer;";
    } else if (java_type->isIntegerFlags()
               || ((format & (NoModification | Underscores)) == Underscores && java_type->isFlags())) {
        return "I";
    } else if (java_type->isIntegerEnum()
               || ((format & (NoModification | Underscores)) == Underscores && java_type->isEnum())) {
        uint size = reinterpret_cast<const EnumTypeEntry *>(java_type->typeEntry())->size();
        switch(size){
        case 8: return "B";
        case 16: return "S";
        case 64: return "J";
        default: return "I";
        }
    } else if (java_type->isThread()) {
        if ((format & Underscores) == Underscores)
            return "Ljava_lang_Thread_2";
        else
            return "Ljava/lang/Thread;";
    }

    QString name = java_type->name();
    if (java_type->isFunctional()) {
        const FunctionalTypeEntry *func = static_cast<const FunctionalTypeEntry *>(java_type->typeEntry());
        if(!func->qualifier().isEmpty())
            name = func->qualifier()+"$"+name;
    } else if (java_type->isObject() || java_type->isValue()) {
        if (const InterfaceTypeEntry *ie
                = static_cast<const ImplementorTypeEntry *>(java_type->typeEntry())->designatedInterface())
            name = ie->targetLangName();
    } else if (java_type->isTargetLangEnum()) {
        const EnumTypeEntry *et = static_cast<const EnumTypeEntry *>(java_type->typeEntry());
        name = et->javaQualifier() + "$" + et->targetLangName();

    } else if (java_type->isTargetLangFlags()) {
        const FlagsTypeEntry *ft = static_cast<const FlagsTypeEntry *>(java_type->typeEntry());
        name = ft->originator()->javaQualifier() + "$" + ft->targetLangName();
    }

    return jni_signature((java_type->package().isEmpty() ? QString() : java_type->package() + ".") + name, format);
}

static const QHash<QString, QString> default_return_table{
    {"boolean", "false"},
    {"byte", "0"},
    {"char", "0"},
    {"short", "0"},
    {"int", "0"},
    {"long", "0"},
    {"float", "0.0f"},
    {"double", "0.0"},
    {"java.lang.Object", "nullptr"}
};

QString default_return_statement_qt(const AbstractMetaType *java_type,
                                    Generator::Option options = Generator::NoOption) {
    QString returnStr;
    if((options & Generator::NoReturnStatement) == 0)
        returnStr += "return";
    if (!java_type)
        return returnStr;
    if((options & Generator::NoReturnStatement) == 0)
        returnStr += " ";
    if (!java_type->indirections().isEmpty())
            return returnStr + "nullptr";

    QString signature = default_return_table.value(java_type->typeEntry()->targetLangName());

    if (!signature.isEmpty())
        return returnStr + signature;

    Q_ASSERT(!java_type->isPrimitive());
    if (java_type->isJObjectWrapper())
        return returnStr + "JObjectWrapper()";
    if (java_type->isJMapWrapper())
        return returnStr + "JMapWrapper()";
    if (java_type->isJCollectionWrapper())
        return returnStr + "JCollectionWrapper()";
    if (java_type->isJIteratorWrapper())
        return returnStr + "JIteratorWrapper()";
    if (java_type->isJEnumWrapper())
        return returnStr + "JEnumWrapper()";
    if (java_type->isJQFlagsWrapper())
        return returnStr + "JQFlagsWrapper()";
    if (java_type->isVariant())
        return returnStr + "QVariant()";
    if (java_type->isTargetLangString())
        return returnStr + "QString()";
    if (java_type->isTargetLangStringRef())
        return returnStr + "QStringRef()";
    if (java_type->isTargetLangStringView())
        return returnStr + "QStringView()";
    if (java_type->isTargetLangChar())
        return returnStr + "QChar()";
    else if (java_type->isEnum())
        return returnStr + java_type->typeEntry()->name() + "(0)";
    else if (java_type->isFlags())
        return returnStr + java_type->typeEntry()->name() + "()";
    else if (java_type->isPointerContainer())
        return returnStr + java_type->cppSignature() + "()";
    else if (java_type->isInitializerList())
        return returnStr + java_type->cppSignature() + "()";
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::StringListContainer)
        return returnStr + java_type->typeEntry()->name() + "()";
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::ByteArrayListContainer)
        return returnStr + java_type->typeEntry()->name() + "()";
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::QVector2DArrayContainer)
        return returnStr + java_type->typeEntry()->name() + "()";
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::QVector3DArrayContainer)
        return returnStr + java_type->typeEntry()->name() + "()";
    else if (java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type() == ContainerTypeEntry::QVector4DArrayContainer)
        return returnStr + java_type->typeEntry()->name() + "()";
    else if (java_type->isValue() || java_type->isContainer() || java_type->isIterator())
        return returnStr + java_type->cppSignature() + "()";
    else
        return returnStr + "0";
}

QString default_return_statement_java(const AbstractMetaType *java_type,
                                      Generator::Option options = Generator::NoOption) {
    QString returnStr = ((options & Generator::NoReturnStatement) == 0 ? "return " : "");
    if (!java_type)
        return "return";
    if (java_type->isArray() || !java_type->indirections().isEmpty())
        return returnStr+"nullptr";

    QString signature = default_return_table.value(java_type->typeEntry()->targetLangName());
    if (!signature.isEmpty())
        return returnStr+signature;

    Q_ASSERT(!java_type->isPrimitive());
    return returnStr+"nullptr";
}

/* Used to decide how which of the Call[Xxx]Method functions to call
 */
QByteArray jniTypeName(const QString &name) {
    static QHash<QString, const char *> table;
    if (table.isEmpty()) {
        table["jboolean"] = "Boolean";
        table["jbyte"] = "Byte";
        table["jchar"] = "Char";
        table["jshort"] = "Short";
        table["jint"] = "Int";
        table["jlong"] = "Long";
        table["jfloat"] = "Float";
        table["jdouble"] = "Double";
        table["jobject"] = "Object";
    }

    return table[name];
}

QByteArray jniName(const QString &name) {
    if(name=="java.lang.String" || name=="String")
        return "jstring";
    else if(name=="void")
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
    else if(name=="int[]")
        return "jintArray";
    else if(name=="long[]")
        return "jlongArray";
    else if(name=="short[]")
        return "jshortArray";
    else if(name=="byte[]")
        return "jbyteArray";
    else if(name=="double[]")
        return "jdoubleArray";
    else if(name=="float[]")
        return "jfloatArray";
    else if(name=="boolean[]")
        return "jbooleanArray";
    else if(name=="char[]")
        return "jcharArray";
    else if(name.endsWith("[]"))
        return "jobjectArray";
    TypeEntry *entry = TypeDatabase::instance()->findType(name);
    if (entry)
        return entry->jniName().toLatin1();
    else if(name=="boolean")
        return "jboolean";
    else if(name=="void")
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
    else if(name=="char")
        return "jchar";
    else
        return "jobject";
}

QString CppImplGenerator::jniReturnName(const AbstractMetaFunction *java_function) {
    QString return_type = translateType(java_function->type(), EnumAsInts);
    QString new_return_type = java_function->typeReplaced(0);
    if (!new_return_type.isEmpty()) {
        return_type = jniName(new_return_type);
    }
    return return_type;
}


QByteArray jniTypeName(const AbstractMetaType *java_type) {
    if (!java_type) {
        return "Void";
    } else if (java_type->isTargetLangChar()) {
        return "Char";
    } else if (java_type->isPrimitive()) {
        return jniTypeName(java_type->typeEntry()->jniName());
    } else if (java_type->isIntegerEnum() || java_type->isIntegerFlags()) {
        return "Int";
    } else {
        return "Object";
    }
}

QByteArray newXxxArray(const AbstractMetaType *java_type) {
    return "New" + jniTypeName(java_type) + "Array";
}

QByteArray setXxxArrayElement(const AbstractMetaType *java_type) {
    Q_ASSERT(java_type);
    return "Set" + jniTypeName(java_type) + "ArrayElement";
}

QByteArray getXxxArrayElement(const AbstractMetaType *java_type) {
    Q_ASSERT(java_type);
    return "Get" + jniTypeName(java_type) + "ArrayElement";
}

QByteArray getXxxArrayRegion(const AbstractMetaType *java_type) {
    Q_ASSERT(java_type);
    return "Get" + jniTypeName(java_type) + "ArrayRegion";
}

QByteArray setXxxArrayRegion(const AbstractMetaType *java_type) {
    Q_ASSERT(java_type);
    return "Set" + jniTypeName(java_type) + "ArrayRegion";
}

QByteArray callXxxMethod(const AbstractMetaType *java_type) {
    return "Call" + jniTypeName(java_type) + "Method";
}

QByteArray callXxxMethod(const QString &name) {
    TypeEntry *entry = TypeDatabase::instance()->findType(name);
    if (entry){
        if(entry->isPrimitive())
            return "Call" + jniTypeName(entry->jniName()) + "Method";
        else return "CallObjectMethod";
    }else{
        if(name=="void")
            return "CallVoidMethod";
        else if(name=="boolean")
            return "CallBooleanMethod";
        else if(name=="int")
            return "CallIntMethod";
        else if(name=="long")
            return "CallLongMethod";
        else if(name=="short")
            return "CallShortMethod";
        else if(name=="byte")
            return "CallByteMethod";
        else if(name=="char")
            return "CallCharMethod";
        else if(name=="float")
            return "CallFloatMethod";
        else if(name=="double")
            return "CallDoubleMethod";
        return "CallObjectMethod";
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

    if (options & CppImplGenerator::JNIExport)
        s += "Q_DECL_EXPORT ";

    if (options & CppImplGenerator::Deprecated)
        s += "QT_DEPRECATED ";

    if (options & CppImplGenerator::ReturnType) {
        s += return_type;
        s += " ";
    }

    if (options & CppImplGenerator::JNIExport)
        s += "JNICALL QTJAMBI_FUNCTION_PREFIX(";

    s += "Java_";
    s += package.replace("_", "_1").replace(".", "_");
    s += '_';
    s += class_name.replace("_", "_1").replace("$", "_00024");
    s += '_';
    s += QString(function_name).replace("_", "_1").replace("$", "_00024");
    s += mangled_arguments;

    if (options & CppImplGenerator::JNIExport)
        s += ")";

    return s;
}

const AbstractMetaClass* lookupClassWithPublicDestructor(const AbstractMetaClass *cls) {
    while (cls != nullptr) {
        if (cls->hasPublicDestructor()) {
            return cls;
        } else {
            cls = cls->baseClass();
        }
    }
    return nullptr;
}

bool hasDeleter(const AbstractMetaClass *cls) {
    const ComplexTypeEntry *entry = cls->typeEntry();
    if (   !cls->isFake()
        && cls->typeEntry()->isObject()
        && !cls->typeEntry()->isQObject()
        && !cls->isInterface()
        && ((entry->codeGeneration() & TypeEntry::GenerateCode) != 0)
    ) {
        return lookupClassWithPublicDestructor(cls) != nullptr;
    }
    return false;
}

QString CppImplGenerator::fileNameForClass(const AbstractMetaClass *java_class) const {
    if(java_class->typeEntry()->designatedInterface()){
        return fileNameForClass(java_class->enclosingClass());
    }
    return QString("%1_shell.cpp").arg(java_class->name().replace("$", "_"));
}

QString CppImplGenerator::fileNameForFunctional(const AbstractMetaFunctional *java_class) const {
    if(java_class->enclosingClass() && !java_class->enclosingClass()->isFake())
        return QString("%1_%2_shell.cpp").arg(java_class->enclosingClass()->name().replace("$", "_")).arg(java_class->name().replace("$", "_"));
    else
        return QString("%1_shell.cpp").arg(java_class->name().replace("$", "_"));
}

void CppImplGenerator::writeSignalFunction(QTextStream &s, const AbstractMetaFunction *signal, const AbstractMetaClass *cls, int pos) {
    if (signal->hasRReferences() || !cls->has_Q_OBJECT())
        return ;
    s << "#ifdef QT_QTJAMBI_PORT" << Qt::endl;
    QStringList pps = getFunctionPPConditions(signal);
    if(!signal->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(signal->declaringClass()->typeEntry()->ppCondition())){
        if(pps.isEmpty() || !signal->declaringClass()->typeEntry()->ppCondition().contains("|"))
            pps << signal->declaringClass()->typeEntry()->ppCondition();
        else
            pps << "(" + signal->declaringClass()->typeEntry()->ppCondition() + ")";
    }
    if(!pps.isEmpty())
        s << "#if " << pps.join(" && ") << Qt::endl;
    writeFunctionSignature(s, signal, nullptr, "__qt_signal_"+cls->typeEntry()->qualifiedCppName().replace("::", "_")+"_", Option(OriginalName), "", QStringList()<<"const WeakNativeID& __qt_weakNativeID");
    s << Qt::endl << "{" << Qt::endl;
    {
        AbstractMetaArgumentList arguments;
        for (int i = 0; i < signal->arguments().size(); ++i) {
            if(!signal->argumentRemoved(i+1)){
                arguments << signal->arguments()[i];
            }
        }
        INDENTATION(INDENT)

        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"shell\", \"";
        writeFunctionSignature(s, signal, cls, "",
                           Option(OriginalName));
        s << "\")" << Qt::endl
          << INDENT << "if(JNIEnv *__jni_env = qtjambi_current_environment()){" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 100)" << Qt::endl;
            QString lines;
            {
                QTextStream s(&lines);
                if (arguments.size() == 1)
                    s << INDENT << "jvalue arguments;" << Qt::endl;
                else if (arguments.size() > 0)
                    s << INDENT << "jvalue arguments[" << arguments.size() << "];" << Qt::endl;

                writeCodeInjections(s, signal, cls, CodeSnip::Beginning, TypeSystem::Signal, "__jni_env", "__qtjambi_scope");

                bool hasCodeInj = hasCodeInjections(signal, cls, {CodeSnip::Beginning, CodeSnip::End}, TypeSystem::Signal)
                        || signal->hasConversionRule(TypeSystem::ShellCode, -1);

                for (int i = 0; i < arguments.size(); ++i) {
                    const AbstractMetaArgument *argument = arguments.at(i);
                    if(hasCodeInj){
                        writeQtToJava(s,
                                      argument->type(),
                                      argument->indexedName(),
                                      QString("__tmp_arguments%1").arg(QString::number(i)),
                                      signal, nullptr,
                                      argument->argumentIndex() + 1,
                                      Option(BoxedPrimitive),
                                      "__jni_env",
                                      "__qtjambi_scope");
                        s << INDENT;
                        if(arguments.size() == 1)
                            s << "arguments.l";
                        else
                            s << "arguments[" << QString::number(i) << "].l";
                        s << " = __tmp_arguments" << QString::number(i) << ";" << Qt::endl;
                    }else{
                        writeQtToJava(s,
                                      argument->type(),
                                      argument->indexedName(),
                                      arguments.size() == 1 ? QString("arguments.l") : QString("arguments[%1].l").arg(QString::number(i)),
                                      signal, nullptr,
                                      argument->argumentIndex() + 1,
                                      Option(NoTmpVariable | BoxedPrimitive),
                                      "__jni_env",
                                      "__qtjambi_scope");
                    }
                    if (signal->resetObjectAfterUse(argument->argumentIndex() + 1)){
                        if (argument->type()->isContainer()){
                            s << INDENT << "QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE";
                        }else if (argument->type()->isArray()){
                            s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE";
                        }else{
                            s << INDENT << "QTJAMBI_INVALIDATE_AFTER_USE";
                        }
                        s << "(__jni_env, " << (arguments.size() == 1 ? QString("arguments.l") : QString("arguments[%1].l").arg(QString::number(i))) << ")" << Qt::endl;
                    }
                }
                s << INDENT << "qtjambi_emit_signal(__jni_env, __qt_weakNativeID, &__signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << pos << ", ";
                if(arguments.size()==0)
                    s << "nullptr";
                else if(arguments.size()==1)
                    s << "&arguments";
                else
                    s << "arguments";
                s << ");" << Qt::endl;

                writeCodeInjections(s, signal, cls, CodeSnip::End, TypeSystem::Signal, "__jni_env", "__qtjambi_scope");
            }
            if(lines.contains("__qtjambi_scope")){
                s << INDENT << "QtJambiScope __qtjambi_scope(__qt_sharedlink);" << Qt::endl;
            }
            s << lines;
        }
        s << INDENT << "}" << Qt::endl;

        if (signal->type()!= nullptr)
            s << INDENT << default_return_statement_qt(signal->type()) << ";" << Qt::endl;
    }
    s << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(" && ") << Qt::endl;

    if (signal->implementingClass() == signal->ownerClass() &&
            !signal->hasTemplateArgumentTypes())
        writeFinalFunction(s, signal, cls);
    s << "#endif //QT_QTJAMBI_PORT" << Qt::endl << Qt::endl;
}

bool CppImplGenerator::hasCustomDestructor(const AbstractMetaClass *java_class) const {
    return !java_class->isQObject() && !java_class->typeEntry()->isValue();
}

void CppImplGenerator::write(QTextStream &s, const AbstractMetaFunctional *java_class, int) {
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
    // Includes
    QSet<QString> dedupe = QSet<QString>();
    writeExtraIncludes(s, java_class, dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QMetaType"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QDataStream"), dedupe);
    if(java_class->enclosingClass() && !java_class->enclosingClass()->isFake()){
        writeInclude(s, Include(Include::LocalPath, QString(java_class->enclosingClass()->name()).replace("$", "_")+"_"+QString(java_class->name().replace("$", "_"))+"_shell.h"), dedupe);
    }else{
        writeInclude(s, Include(Include::LocalPath, QString(java_class->name().replace("$", "_"))+"_shell.h"), dedupe);
    }

    writeInclude(s, java_class->typeEntry()->include(), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_core.h"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_cast.h"), dedupe);
    if(java_class->isFunctionPointer()){
        writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_functionpointer.h"), dedupe);
    }

    if(java_class->enclosingClass() && !java_class->enclosingClass()->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << java_class->enclosingClass()->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    s << "// emitting (writeShellConstructor)" << Qt::endl;
    writeShellConstructor(s, java_class);
    s << "// emitting (writeShellDestructor)" << Qt::endl;
    writeShellDestructor(s, java_class);

    s << "// construct " << shellClassName(java_class) << Qt::endl
      << "void __qt_construct_" << QString(java_class->typeEntry()->name()).replace("::", "_") << "_0";
    s << "(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"new " << java_class->typeEntry()->qualifiedCppName() << "\")" << Qt::endl;
        s << INDENT << "new (__qtjambi_ptr) " << shellClassName(java_class) << "();" << Qt::endl;
    }
    s << "}" << Qt::endl;

    s << "// destruct " << shellClassName(java_class) << Qt::endl
      << "void __qt_destruct_" << QString(java_class->typeEntry()->name()).replace("::", "_");
    s << "(void* ptr)" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"destruct " << java_class->typeEntry()->qualifiedCppName() << "\")" << Qt::endl;
        s << INDENT << "reinterpret_cast<" << shellClassName(java_class) << "*>(ptr)->~" << shellClassName(java_class).split("::").last() << "();" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;

    s << "void " << shellClassName(java_class) << "::operator delete(void * ptr) noexcept {" << Qt::endl;
    s << "    reinterpret_cast<" << shellClassName(java_class) << "*>(ptr)->" << shellClassName(java_class) << "::__shell()->deleteShell();" << Qt::endl;
    s << "}" << Qt::endl << Qt::endl;
    s << "QtJambiShell* " << shellClassName(java_class) << "::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(" << shellClassName(java_class) << ") ); }" << Qt::endl << Qt::endl;

    s << shellClassName(java_class) << "::Functor::Functor(" << shellClassName(java_class) << "& functional) : FunctorBase(typeid(" << java_class->typeEntry()->qualifiedCppName() << "), functional) {}" << Qt::endl << Qt::endl
      << shellClassName(java_class) << "::Functor::Functor(const Functor& functor) : FunctorBase(functor) {}" << Qt::endl << Qt::endl
      << shellClassName(java_class) << "::Functor::~Functor() {}" << Qt::endl << Qt::endl;

    if(java_class->type()){
        writeTypeInfo(s, java_class->type(), Option());
    }else{
        s << "void";
    }
    s << " " << shellClassName(java_class) << "::Functor::operator()(";
    int counter = 0;
    for(const AbstractMetaArgument *arg : java_class->arguments()){
        if(counter>0)
            s << ", ";
        writeTypeInfo(s, arg->type(), Option());
        s << arg->indexedName();
        ++counter;
    }
    s << ")" << Qt::endl
      << "{" << Qt::endl;
    QString new_return_type = java_class->typeReplaced(0);
    {
        INDENTATION(INDENT)
        s << INDENT << "if (jmethodID method_id = javaMethod()) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "if(JNIEnv * env = qtjambi_current_environment()){" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "QTJAMBI_JNI_LOCAL_FRAME(env, " << QString::number( 100*(java_class->arguments().size()+2) ) << ")" << Qt::endl;
                {
                    s << INDENT << "if (jobject __java_this = getJavaObjectLocalRef(env)) {" << Qt::endl;
                    QString lines;
                    {
                        QTextStream s(&lines);
                        INDENTATION(INDENT)
                        if(java_class->type()){
                            s << INDENT;
                            writeTypeInfo(s, java_class->type(), Option());
                            s << "__qt_return_value =" << default_return_statement_qt(java_class->type(), Generator::NoReturnStatement) << ";" << Qt::endl;
                        }
                        for(AbstractMetaArgument *argument : java_class->arguments()) {
                            ArgumentRemove ar = java_class->argumentRemoved(argument->argumentIndex() + 1);
                            switch(ar) {
                            case ArgumentRemove_No:{
                                    if (!argument->type()->isPrimitive()
                                            || java_class->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)
                                    ) {
                                        writeQtToJava(s,
                                                      argument->type(),
                                                      argument->indexedName(),
                                                      "__java_" + argument->indexedName(),
                                                      nullptr,
                                                      java_class,
                                                      argument->argumentIndex() + 1,
                                                      NoOption,
                                                      "env",
                                                      "__qtjambi_scope");
                                    }

                                    if (java_class->resetObjectAfterUse(argument->argumentIndex() + 1)){
                                        if (argument->type()->isContainer()){
                                            s << INDENT << "QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(env, __java_" << argument->indexedName() << ")" << Qt::endl;
                                        }else if (argument->type()->isArray()){
                                            s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(env, __java_" << argument->indexedName() << ")" << Qt::endl;
                                        }else{
                                            s << INDENT << "QTJAMBI_INVALIDATE_AFTER_USE(env, __java_" << argument->indexedName() << ")" << Qt::endl;
                                        }
                                    }
                                }
                                break;
                            default: break;
                            }
                        }
                        int counter = 0;
                        for(const AbstractMetaArgument* argument : java_class->arguments()) {
                            writeOwnership(s, nullptr, "__java_" + argument->indexedName(), "__qt_" + argument->indexedName(), counter + 1, nullptr, TypeSystem::ShellCode, "env", "__qtjambi_scope");
                            ++counter;
                        }

                        s << INDENT;
                        QString typeWrap;
                        if (java_class->type()) {
                            if (new_return_type.isEmpty()) {
                                s << translateType(java_class->type());
                            } else {
                                QString _jniName = jniName(new_return_type);
                                if(_jniName=="jstring"
                                        || _jniName=="jclass"
                                        || _jniName=="jthrowable"
                                        || _jniName.endsWith("Array"))
                                    typeWrap = _jniName;
                                s << _jniName;
                            }
                            s << " __java_return_value = ";	// declaration only
                        }else if (!new_return_type.isEmpty()) {
                            QString _jniName = jniName(new_return_type);
                            if(_jniName=="jstring"
                                    || _jniName=="jclass"
                                    || _jniName=="jthrowable"
                                    || _jniName.endsWith("Array"))
                                typeWrap = _jniName;
                            s << _jniName;
                            s << " __java_return_value = ";	// declaration only
                        }

                        if(!typeWrap.isEmpty())
                            s << typeWrap << "(";
                        s << "env->";
                        if (!new_return_type.isEmpty()) {
                            s << callXxxMethod(new_return_type);
                        }else if (!java_class->type()) {
                            s << "CallVoidMethod";
                        }else{
                            s << callXxxMethod(java_class->type());
                        }

                        s << "(__java_this, method_id";
                        if (java_class->arguments().size() > 0)
                            s << ", ";
                        writeFunctionCallArguments(s, java_class, "__java_", Option(NoCasts | SkipRemovedArguments));
                        s << ")";
                        if(!typeWrap.isEmpty())
                            s << ")";
                        s << ";" << Qt::endl;
                        s << INDENT << "qtjambi_throw_java_exception(env)" << Qt::endl;

                        if (java_class->type()) {
                            writeJavaToQt(s, java_class->type(), "__qt_return_value_tmp", "__java_return_value",
                                          nullptr, java_class, 0, GlobalRefJObject, "env",
                                          "__qtjambi_scope");
                            // This line below i sonly needed because we can't instruction writeJavaToQt
                            //  to omit the emitting the function type, so then we could assign
                            //  __qt_return_value directly.
                            // This is not very efficient for complex types as it may cause extra copying.
                            if(java_class->type())
                                s << INDENT << "__qt_return_value = __qt_return_value_tmp;" << Qt::endl;
                        } else if (java_class->hasConversionRule(TypeSystem::ShellCode, 0)) {
                            writeConversionRule(s, TypeSystem::ShellCode, nullptr, java_class, 0, "<invalid>", "<invalid>", NoOption, "env", "__qtjambi_scope");
                        }

                        if(java_class->type()){
                            s << INDENT << "return __qt_return_value;" << Qt::endl;
                        }
                    }
                    if(lines.contains("__qtjambi_scope")){
                        s << INDENT << "    QtJambiScope __qtjambi_scope(shell());" << Qt::endl;
                    }
                    s << lines;
                    s << INDENT << "}" << Qt::endl;
                }
            }
            s << INDENT << "}" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl;
        if(java_class->type()){
            s << INDENT << default_return_statement_qt(java_class->type()) << ";" << Qt::endl;
        }
    }
    s << INDENT << "}" << Qt::endl << Qt::endl;


    s << "void " << shellClassName(java_class) << "::getFunctional(void* result)" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        if(java_class->isFunctionPointer()){
            s << INDENT << "if(JNIEnv *env = qtjambi_current_environment()){" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "QTJAMBI_JNI_LOCAL_FRAME(env, 300)" << Qt::endl;
                s << INDENT << "jobject __this = __shell()->getJavaObjectLocalRef(env);" << Qt::endl;
                s << INDENT << "uint hash = qtjambi_java_object_identity(env, __this);" << Qt::endl;
                s << INDENT << "*reinterpret_cast<" << java_class->typeEntry()->qualifiedCppName() << "*>(result) = qtjambi_function_pointer<";
                if(java_class->typeEntry()->count()!=0){
                    s << QString::number(java_class->typeEntry()->count());
                }else{
                    s << "4";
                }
                s << ",";
                if(java_class->type()){
                    writeTypeInfo(s, java_class->type(), Option(SkipName));
                }else{
                    s << "void";
                }
                s << "(";
                int counter = 0;
                for(const AbstractMetaArgument *arg : java_class->arguments()){
                    if(counter>0)
                        s << ",";
                    writeTypeInfo(s, arg->type(), Option(SkipName));
                    ++counter;
                }
                s << ")>(Functor(*this), hash, &m_functionPointerDeleter, reverseFunctionGetter ? nullptr : &reverseFunctionGetter);" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }else{
            s << INDENT << "*reinterpret_cast<" << java_class->typeEntry()->qualifiedCppName() << "*>(result) = Functor(*this);" << Qt::endl;
        }
    }
    s << "}" << Qt::endl << Qt::endl;

    s << "jobject " << shellClassName(java_class) << "::resolveFunctional(JNIEnv * env, const " << java_class->typeEntry()->qualifiedCppName() << "* function)" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "if(function";
        if(java_class->isFunctionPointer()){
            s << " && reverseFunctionGetter";
        }
        s << "){" << Qt::endl
          << INDENT << "    if(const " << shellClassName(java_class) << "::Functor* functor = ";
        if(java_class->isFunctionPointer()){
            s << "reverseFunctionGetter(*function)";
        }else{
            s << "function->target<" << shellClassName(java_class) << "::Functor>()";
        }
        s << "){" << Qt::endl
          << INDENT << "        return functor->getJavaObjectLocalRef(env);" << Qt::endl
          << INDENT << "    }" << Qt::endl
          << INDENT << "}" << Qt::endl
          << INDENT << "return nullptr;" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;


    QString functionalClassPackage = java_class->package();
    QString functionalClassName = java_class->name()+"$Impl";
    if(java_class->enclosingClass()){
        functionalClassName = java_class->enclosingClass()->name() + "$" + functionalClassName;
        functionalClassPackage = java_class->enclosingClass()->package();
    }
    QString return_type = java_class->type() ? translateType(java_class->type(), Option(EnumAsInts)) : QLatin1String("void");
    s << jni_function_signature(
               functionalClassPackage,
               functionalClassName+"$ConcreteWrapper",
               "__qt_call",
               return_type
        );
    s << Qt::endl << "(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId";
    for(const AbstractMetaArgument *arg : java_class->arguments()){
        if(java_class->argumentRemoved(arg->argumentIndex() +1))
            continue;
        s << ", ";
        if(java_class->argumentTypeArray(arg->argumentIndex() + 1)){
            QScopedPointer<AbstractMetaType> array(arg->type()->copy());
            array->setConstant(false);
            array->setReferenceType(AbstractMetaType::NoReference);
            array->setTypeUsagePattern(AbstractMetaType::ArrayPattern);
            QScopedPointer<AbstractMetaType> cpy(arg->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(AbstractMetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            AbstractMetaBuilder::decideUsagePattern(cpy.get());
            array->setArrayElementType(cpy.get());
            s << translateType(array.get(), Option(UseNativeIds | EnumAsInts));
        }else if(java_class->argumentTypeBuffer(arg->argumentIndex() + 1)){
            s << "jobject";
        }else{
            QString jniType;
            QString modified_type = java_class->typeReplaced(arg->argumentIndex() + 1, &jniType);

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
    }
    s << ")" << Qt::endl;
    s << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << java_class->name() << "(...)\")" << Qt::endl;
        s << INDENT << "try{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QtJambiScope __qtjambi_scope(__this_nativeId);" << Qt::endl
              << INDENT << "Q_UNUSED(__qtjambi_scope)" << Qt::endl
              << INDENT << java_class->typeEntry()->qualifiedCppName() << "* function = ";
            if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                s << "qtjambi_interface_from_nativeId<" << java_class->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                s << INDENT << "qtjambi_check_resource(__jni_env, function);" << Qt::endl;
            }else{
                s << "qtjambi_object_from_nativeId<" << java_class->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                s << INDENT << "Q_ASSERT(function);" << Qt::endl;
            }

            bool hasCodeInj = hasCodeInjections(java_class, {CodeSnip::Beginning, CodeSnip::End}, TypeSystem::Signal)
                    || java_class->hasConversionRule(TypeSystem::ShellCode, -1);

            for(const AbstractMetaArgument *argument : java_class->arguments()) {
                if(java_class->argumentRemoved(argument->argumentIndex() +1))
                    continue;
                if (!argument->type()->isPrimitive()
                        || java_class->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
                    writeJavaToQt(s,
                                  argument->type(),
                                  "__qt_" + argument->indexedName(),
                                  argument->indexedName(),
                                  nullptr,
                                  java_class,
                                  argument->argumentIndex() + 1,
                                  Option(UseNativeIds | EnumAsInts), "__jni_env", "__qtjambi_scope");
                }
            }

            if (java_class->type()) {
                const QString qt_return_value = "__qt_return_value";
                const QString java_return_value = "__java_return_value";
                s << INDENT;
                writeTypeInfo(s, java_class->type(), EnumAsInts);
                s << " " << qt_return_value
                << " = ";
                if(java_class->type()->isEnum()){
                    uint size = static_cast<const EnumTypeEntry*>(java_class->type()->typeEntry())->size();
                    s << "qint" << size << "(";
                }

                s << "(*function)(";
                writeFunctionCallArguments(s, java_class, "__qt_");
                s << ")";
                if(java_class->type()->isEnum()){
                    s << ")";
                }
                s << ";" << Qt::endl;

                if(hasCodeInj){
                    writeQtToJava(s, java_class->type(), qt_return_value, java_return_value,
                                  nullptr, nullptr, 0, EnumAsInts, "__jni_env", "__qtjambi_scope");
                    s << INDENT << "return " << java_return_value << ";" << Qt::endl;
                }else{
                    writeQtToJava(s, java_class->type(), qt_return_value, "",
                                  nullptr, nullptr, 0, Option(DirectReturn | NoTmpVariable | EnumAsInts),
                                  "__jni_env", "__qtjambi_scope");
                    s << ";" << Qt::endl;
                }
            } else {
                s << INDENT << "(*function)(";
                writeFunctionCallArguments(s, java_class, "__qt_");
                s << ");" << Qt::endl;
            }
        }
        s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
        s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
        if(return_type=="jobject"
                || return_type=="jstring"
                || return_type=="jthrowable"
                || return_type=="jclass"
                || return_type=="jarray"
                || return_type.endsWith("Array")) {
            s << INDENT << "    return nullptr;" << Qt::endl;
        }else if(return_type=="jboolean") {
            s << INDENT << "    return false;" << Qt::endl;
        }else if(return_type!="void") {
            s << INDENT << "    return " << return_type << "(0);" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;

    s << jni_function_signature(
               functionalClassPackage,
               functionalClassName,
               "__qt_"+java_class->name()+"_new",
               "void"
        );
    s << Qt::endl << "(JNIEnv *__jni_env, jclass __jni_class, jobject __jni_object)" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << shellClassName(java_class) << "\")" << Qt::endl;
        s << INDENT << "try{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_" << QString(java_class->typeEntry()->name()).replace("::", "_") << "_0, sizeof(" << shellClassName(java_class) << "), typeid(" << java_class->typeEntry()->qualifiedCppName() << "), true, true, nullptr, nullptr, nullptr);" << Qt::endl;
        }
        s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
        s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
        s << INDENT << "}" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;

    if(java_class->isFunctionPointer()){
        s << "std::function<const " << shellClassName(java_class) << "::Functor*(" << java_class->typeEntry()->qualifiedCppName() << ")> "
          << shellClassName(java_class) << "::reverseFunctionGetter = std::function<const " << shellClassName(java_class) << "::Functor*(" << java_class->typeEntry()->qualifiedCppName() << ")>();" << Qt::endl << Qt::endl;
    }

    writeMetaInfo(s, java_class);

    if(java_class->enclosingClass() && !java_class->enclosingClass()->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#endif // " << java_class->enclosingClass()->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    QString pro_file_name = MetaInfoGenerator::subDirectoryForClass(java_class, MetaInfoGenerator::CppDirectory) + "/generated.pri";
    priGenerator->addSource(pro_file_name, fileNameForFunctional(java_class));
}

void CppImplGenerator::write(QTextStream &s, const AbstractMetaClass *java_class, int) {
    bool shellClass = java_class->generateShellClass();
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

    QSet<QString> dedupe = QSet<QString>();
    // Includes

    writeInclude(s, Include(Include::IncludePath, "QtCore/QMetaType"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "QtCore/QDataStream"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_core.h"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_cast.h"), dedupe);
    writeInclude(s, Include(Include::IncludePath, "qtjambi/qtjambi_templates.h"), dedupe);

    writeExtraIncludes(s, java_class, dedupe);
    bool shellInclude = (java_class->generateShellClass()
                         || (java_class->isQObject()
                             &&
                             java_class->queryFunctions(AbstractMetaClass::Constructors
                                                        | AbstractMetaClass::Visible
                                                        | AbstractMetaClass::NotRemovedFromShell).size() > 0
                             )
                         || java_class->cppSignalFunctions().size() > 0);

    // need to include QPainter for all widgets...
    {
        const AbstractMetaClass *qwidget = java_class;
        while (qwidget && qwidget->name() != "QWidget") {
            qwidget = qwidget->baseClass();
        }
        if (qwidget)
            writeInclude(s, Include(Include::IncludePath, "QtGui/QPainter"), dedupe);
    }

    if (shellInclude){
        if(java_class->typeEntry()->designatedInterface() && java_class->enclosingClass()){
            writeInclude(s, Include(Include::LocalPath, QString(java_class->enclosingClass()->name().replace("$", "_"))+"_shell.h"), dedupe);
        }else{
            writeInclude(s, Include(Include::LocalPath, QString(java_class->name().replace("$", "_"))+"_shell.h"), dedupe);
        }
    }
    writeInclude(s, java_class->typeEntry()->include(), dedupe);

    if(!java_class->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    for(const AbstractMetaFunction* f : java_class->functions()){
        if(!f->wasPrivate() && f->isDeclDeprecated()){
            s << Qt::endl << "QT_WARNING_DISABLE_DEPRECATED" << Qt::endl << Qt::endl;
            break;
        }
    }

    QList<const AbstractMetaFunction *> signalsInTargetLang;

    for(const AbstractMetaFunction* signal : java_class->cppSignalFunctions()){
        if (!signal->hasTemplateArgumentTypes() && signal->declaringClass() == java_class)
            signalsInTargetLang << signal;
    }

    if(!signalsInTargetLang.isEmpty() && java_class->has_Q_OBJECT()){
        for(int i=0; i<signalsInTargetLang.size(); ++i){
            const AbstractMetaFunction* f = signalsInTargetLang.at(i);
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
                writeFunctionArguments(__s, f->arguments(), Option(SkipName), f->arguments().size());
            }
            s << Qt::endl
              << "int __signal_method_indexes_" << java_class->qualifiedCppName().replace("::", "_") << "_" << i << "(){" << Qt::endl;
            if(!pps.isEmpty()){
                s << "#if " << pps.join(" && ") << Qt::endl;
            }
            s << "    static int result = QMetaMethod::fromSignal(";
            if(java_class->queryFunctionsByOriginalName(f->originalName()).size()>1){
                s << "QOverload<" << argumentList << ">::of(";
            }
            s << "&" << f->declaringClass()->typeEntry()->qualifiedCppName() << "::" << function_name;
            if(java_class->queryFunctionsByOriginalName(f->originalName()).size()>1){
                s << ")";
            }
            s << ").methodIndex();" << Qt::endl
              << "    return result;" << Qt::endl;
            if(!pps.isEmpty()){
                s << "#else" << Qt::endl;
                s << "        return -1;" << Qt::endl;
                s << "#endif //" << pps.join(" && ") << Qt::endl;
            }
            s << "}" << Qt::endl;
            s << Qt::endl;
        }
    }

    writeDeleteAndOwnerFunction(s, java_class);

    bool isInterface = false;
    for(const AbstractMetaClass* iface : java_class->interfaces()){
        ImplementorTypeEntry * origin = static_cast<const InterfaceTypeEntry *>(iface->typeEntry())->origin();
        if(origin==java_class->typeEntry()){
            isInterface = true;
            break;
        }
    }

    QMultiMap<int,AbstractMetaFunction *> availabeConstructors;
    QList<const AbstractMetaFunction *> functionsInTargetLang;

    for(const AbstractMetaFunction *function : java_class->functionsInTargetLang()) {
        if (
            (!function->isConstructor() ||
             !java_class->hasUnimplmentablePureVirtualFunction()) &&
             !function->isEmptyFunction() &&
             !function->hasTemplateArgumentTypes()
            )
            functionsInTargetLang << function;
    }
    for(const AbstractMetaFunction *function : java_class->queryFunctions(AbstractMetaClass::NormalFunctions
                                                                       | AbstractMetaClass::AbstractFunctions
                                                                       | AbstractMetaClass::NotRemovedFromTargetLang)) {
        if (function->implementingClass() != java_class &&
                !function->hasTemplateArgumentTypes() ) {
            functionsInTargetLang << function;
        }
    }

    if (shellClass) {
        s << "QtJambiShell* " << shellClassName(java_class) << "::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(" << shellClassName(java_class) << ") ); }" << Qt::endl << Qt::endl;
        s << "jmethodID " << shellClassName(java_class) << "::__shell_javaMethod(int pos) const { return __shell()->javaMethod(typeid(" << java_class->qualifiedCppName() << "), pos); }" << Qt::endl << Qt::endl;

        if(!java_class->hasPrivateDestructor()){
            s << "// emitting (writeShellConstructor)" << Qt::endl;
            for(AbstractMetaFunction *function : java_class->functions()) {
                if (function->isConstructor() &&
                        !function->isPrivate() &&
                        !function->hasTemplateArgumentTypes()){
                    writeShellConstructor(s, isInterface, function, SkipRemovedArguments);
                    for(int i=0; i<function->arguments().size(); i++){
                        if(!function->arguments().at(i)->originalDefaultValueExpression().isEmpty()){
                            availabeConstructors.insert(i, function);
                        }
                    }
                    availabeConstructors.insert(function->arguments().size(), function);
                }
            }

            s << "// emitting (writeShellDestructor)" << Qt::endl;
            writeShellDestructor(s, isInterface, java_class);
        }

        s << "void " << shellClassName(java_class) << "::operator delete(void * ptr) noexcept {" << Qt::endl;
        s << "    reinterpret_cast<" << shellClassName(java_class) << "*>(ptr)->" << shellClassName(java_class) << "::__shell()->deleteShell();" << Qt::endl;
        s << "}" << Qt::endl << Qt::endl;

        if (java_class->isQObject() && java_class->qualifiedCppName()!="QDBusInterface") {
            s << "// emitting (writeQObjectFunctions)" << Qt::endl;
            writeQObjectFunctions(s, java_class);
        }

        // Virtual overrides
        s << "// emitting Virtual overrides (virtualFunctions)" << Qt::endl;
        AbstractMetaFunctionList virtualFunctions = java_class->virtualFunctions();
        for (int pos = 0; pos < virtualFunctions.size(); ++pos) {
            const AbstractMetaFunction *function = virtualFunctions.at(pos);
            if(!function->hasTemplateArgumentTypes() && (!function->wasPrivate() || function->isAbstract()))
                writeShellFunction(s, function, java_class, pos);
        }

        // Functions in shell class
        s << "// emitting Functions in shell class (nonVirtualShellFunctions)" << Qt::endl;
        AbstractMetaFunctionList shellFunctions = java_class->nonVirtualShellFunctions();
        for (int i = 0; i < shellFunctions.size(); ++i) {
            const AbstractMetaFunction *function = shellFunctions.at(i);
            if(!function->hasTemplateArgumentTypes())
                writeShellFunction(s, function, java_class, -1);
        }

        // Write public overrides for functions that are protected in the base class
        // so they can be accessed from the native callback
        s << "// emitting Public Override Functions (publicOverrideFunctions)" << Qt::endl;
        for(const AbstractMetaFunction *function : java_class->publicOverrideFunctions()) {
            if(functionsInTargetLang.contains(function) || signalsInTargetLang.contains(function))
                writePublicFunctionOverride(s, function, java_class);
        }

        // Write virtual function overries used to decide on static/virtual calls
        s << "// emitting Virtual Override Functions (virtualOverrideFunctions)" << Qt::endl;
        for(const AbstractMetaFunction *function : java_class->virtualOverrideFunctions()) {
            if(!function->hasTemplateArgumentTypes()
                    && !(function->isRemovedFrom(java_class, TypeSystem::TargetLangCode)
                         || function->isRemovedFrom(function->declaringClass(), TypeSystem::TargetLangCode)
                         ))
                writeVirtualFunctionOverride(s, function, java_class);
        }

    }

    if(!java_class->typeEntry()->ppCondition().isEmpty()){
        s << Qt::endl << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
    }

    s << "// emitting (writeExtraFunctions)" << Qt::endl;
    writeExtraFunctions(s, java_class);

    s << "// emitting (writeToStringFunction)" << Qt::endl;
    writeToStringFunction(s, java_class);

    if (java_class->hasCloneOperator()) {
        s << "// emitting (writeCloneFunction)" << Qt::endl;
        writeCloneFunction(s, java_class);
    }

    // Signals
    s << "// emitting (writeSignalFunction)" << Qt::endl;
    QList<QString> signalNames;
    QMap<QString,QList<const AbstractMetaFunction*>> sortedSignals;
    for(const AbstractMetaFunction *function : signalsInTargetLang){
        QString key = function->declaringClass()->typeEntry()->qualifiedCppName() + "::" + function->name();
        if(!signalNames.contains(key))
            signalNames.append(key);
        sortedSignals[key].append(function);
    }
    for(const QString& key : signalNames){
        QList<const AbstractMetaFunction*>& list = sortedSignals[key];
        for (int i = 0; i < list.size(); ++i){
            writeSignalFunction(s, list.at(i), java_class, signalsInTargetLang.indexOf(list.at(i)));
        }
    }

    s << "// emitting (writeConstructors)" << Qt::endl;
    int counter = 0;
    QHash<const AbstractMetaFunction *,uint> constructorIndexes;
    const AbstractMetaFunction * standardConstructor = nullptr;
    for(const AbstractMetaFunction *function : java_class->functions()) {
        if (function->isConstructor()
                && !function->hasRReferences()
                && !function->isModifiedRemoved(TypeSystem::NativeCode)
                && !function->isPrivate()
                && !function->hasTemplateArgumentTypes()){
            if (function->actualMinimumArgumentCount()==0) {
                standardConstructor = function;
            }
            QList<QList<const AbstractMetaArgument *>> defaultArgumentOverloads;
            if(function->arguments().isEmpty()){
                defaultArgumentOverloads << QList<const AbstractMetaArgument *>();
            }else{
                for(const AbstractMetaArgument *argument : function->arguments()) {
                    if(defaultArgumentOverloads.isEmpty()){
                        if(function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No
                                && !argument->originalDefaultValueExpression().isEmpty()){
                            defaultArgumentOverloads << ( QList<const AbstractMetaArgument *>() << nullptr);
                        }
                        defaultArgumentOverloads << ( QList<const AbstractMetaArgument *>() << argument);
                    }else{
                        if(function->argumentRemoved(argument->argumentIndex()+1)!=ArgumentRemove_No){
                            if(argument->originalDefaultValueExpression().isEmpty()){
                                for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                    defaultArgumentOverloads[i] << argument;
                                }
                            }
                        }else if(argument->originalDefaultValueExpression().isEmpty()){
                            for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                defaultArgumentOverloads[i] << argument;
                            }
                        }else{
                            QList<QList<const AbstractMetaArgument *>> copy;
                            for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                if(defaultArgumentOverloads[i].last()){
                                    copy << (QList<const AbstractMetaArgument *>(defaultArgumentOverloads[i]) << argument);
                                }
                                defaultArgumentOverloads[i] << nullptr;
                            }
                            defaultArgumentOverloads << copy;
                        }
                    }
                }
            }
            if(!java_class->hasUnimplmentablePureVirtualFunction()){
                for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                    writeConstructor(s, function, defaultArgumentOverloads[i], java_class, counter++);
                }
            }
            constructorIndexes[function] = uint(counter - 1);
        }
    }

    if(!java_class->isNamespace() && !java_class->hasPrivateDestructor() && !java_class->hasJustPrivateConstructors() && !java_class->hasUnimplmentablePureVirtualFunction()){
        s << "// destruct " << shellClassName(java_class) << Qt::endl
          << "void __qt_destruct_" << java_class->typeEntry()->qualifiedCppName().replace("::", "_");
        s << "(void* ptr)" << Qt::endl
          << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"destruct " << java_class->typeEntry()->qualifiedCppName() << "\")" << Qt::endl;
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
            s << INDENT << "reinterpret_cast<" << shellClassName(java_class) << "*>(ptr)->~" << shellClassName(java_class).split("::").last() << "();" << Qt::endl;
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#else" << Qt::endl;
                s << INDENT << "Q_UNUSED(ptr)" << Qt::endl;
                s << Qt::endl << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
        }
        s << "}" << Qt::endl << Qt::endl;
    }

    if(java_class->isQObject() && !java_class->isFinal() && standardConstructor && !java_class->hasUnimplmentablePureVirtualFunction()){
        writeConstructor(s, standardConstructor, QList<const AbstractMetaArgument *>(), java_class, -1);

        s << "// emitting  (functionsInTargetLang writePlacementConstructor)" << Qt::endl
          << jni_function_signature(java_class->package(),
                                    java_class->name(),
                                    "__qt_"+java_class->qualifiedCppName().replace("::", "_")+"_declarative_new",
                                    "void") << Qt::endl
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
            s << "#if " << pps.join(" && ") << Qt::endl;
        }
        s << "    QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << shellClassName(java_class) << "::" << shellClassName(java_class) << "\")" << Qt::endl
          << "    try{" << Qt::endl
          << "        jvalue arguments;" << Qt::endl
          << "        arguments.l = placement;" << Qt::endl
          << "        qtjambi_initialize_native_qobject(__jni_env, __jni_class, __jni_object, &__qt_construct_" << java_class->qualifiedCppName().replace("::", "_") << "_declarative, sizeof(" << shellClassName(java_class) << "), typeid(" << java_class->qualifiedCppName() << "), " << java_class->qualifiedCppName() << "::staticMetaObject, " << (java_class->generateShellClass() ? "true" : "false") << ", true, &arguments);" << Qt::endl
          << "    }catch(const JavaException& exn){" << Qt::endl
          << "        exn.raiseInJava(__jni_env);" << Qt::endl
          << "    }" << Qt::endl;
        if(!pps.isEmpty()){
            s << "#else" << Qt::endl;
            s << "    Q_UNUSED(__jni_env)" << Qt::endl;
            s << "    Q_UNUSED(__jni_class)" << Qt::endl;
            s << "    Q_UNUSED(__jni_object)" << Qt::endl;
            s << "    Q_UNUSED(placement)" << Qt::endl;
            s << "#endif //" << pps.join(" && ") << Qt::endl;
        }
        s << "}" << Qt::endl << Qt::endl;
    }

    // Native callbacks (all java functions require native callbacks)
    s << "// emitting  (functionsInTargetLang writeFinalFunction)" << Qt::endl;
    for(const AbstractMetaFunction *function : functionsInTargetLang) {
        if (
            (!java_class->hasUnimplmentablePureVirtualFunction() || !function->isConstructor())
                && !function->isEmptyFunction()
                && !function->hasTemplateArgumentTypes()
        ){
            if(function->isConstructor()){
                writeFinalConstructor(s, function, java_class, constructorIndexes);
            } else {
                writeFinalFunction(s, function, java_class);
            }
        }
    }

    // Field accessors
    s << "// emitting Field accessors (writeFieldAccessors)" << Qt::endl;
    for(AbstractMetaField *field : java_class->fields()) {
        if (field->wasPublic() || (field->wasProtected() && !java_class->isFinal() && java_class->generateShellClass()))
            writeFieldAccessors(s, field);
    }

    s << "// emitting (writeCustomStructors)" << Qt::endl;
    writeCustomStructors(s, java_class->typeEntry());

    s << "// emitting (writeJavaLangObjectOverrideFunctions)" << Qt::endl;
    writeJavaLangObjectOverrideFunctions(s, java_class);

    if(java_class->templateBaseClass()){
        if((java_class->templateBaseClass()->typeEntry()->qualifiedCppName()=="QMap"
            || java_class->templateBaseClass()->typeEntry()->qualifiedCppName()=="QMultiMap")
                && java_class->templateBaseClassInstantiations().size()>1){
            s << Qt::endl
              << INDENT << jni_function_signature(java_class->package(), java_class->name(), "lessThan", "jboolean")
              << "(JNIEnv *__jni_env, jobject";
            s << ", " << java_class->templateBaseClassInstantiations().at(0)->typeEntry()->jniName() << " key1";
            s << ", " << java_class->templateBaseClassInstantiations().at(0)->typeEntry()->jniName() << " key2";
            s << INDENT << "){" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "try{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!java_class->typeEntry()->ppCondition().isEmpty()){
                        s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                    s << INDENT;
                    writeJavaToQt(s, java_class->templateBaseClassInstantiations().at(0), "_qt_key1", "key1", nullptr, nullptr, 0, Option(), "__jni_env");
                    s << INDENT;
                    writeJavaToQt(s, java_class->templateBaseClassInstantiations().at(0), "_qt_key2", "key2", nullptr, nullptr, 0, Option(), "__jni_env");
                    s << INDENT << "return qMapLessThanKey<";
                    writeTypeInfo(s, java_class->templateBaseClassInstantiations().at(0), Option());
                    s << ">(_qt_key1, _qt_key2);" << Qt::endl;
                    if(!java_class->typeEntry()->ppCondition().isEmpty()){
                        s << "#else" << Qt::endl
                          << INDENT << "Q_UNUSED(key1)"
                          << INDENT << "Q_UNUSED(key2)"
                          << INDENT << "return false;" << Qt::endl
                          << "#endif //" << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                }
                s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
                s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                s << INDENT << "    return false;" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }

    writeMetaInfo(s, java_class, availabeConstructors, signalsInTargetLang, signalNames, sortedSignals, isInterface);

    QString pro_file_name = MetaInfoGenerator::subDirectoryForClass(java_class, MetaInfoGenerator::CppDirectory) + "/generated.pri";
    priGenerator->addSource(pro_file_name, fileNameForClass(java_class));
}

void CppImplGenerator::generateFake(const AbstractMetaClass * java_class) {
    for(AbstractMetaEnum *_enum : java_class->enums()){
        if(_enum->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp){
            QString fileName = fileNameForClass(java_class);
            FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForClass(java_class) + "/" + fileName);
            QTextStream &s = fileOut.stream;
            s << "#include <QtCore/QMetaType>" << Qt::endl
              << "#include <QtCore/QDataStream>" << Qt::endl
              << "#include <qtjambi/qtjambi_registry.h>" << Qt::endl
              << "#include <qtjambi/qtjambi_templates.h>" << Qt::endl;
            QSet<QString> included;
            included << "<QtCore/QMetaType>"
                     << "<QtCore/QDataStream>"
                     << "<qtjambi/qtjambi_registry.h>"
                     << "<qtjambi/qtjambi_templates.h>";
            for(AbstractMetaEnum *cpp_enum : java_class->enums()){
                writeInclude(s, cpp_enum->typeEntry()->include(), included);
            }
            s << Qt::endl;
            writeMetaInfo(s, java_class, QMultiMap<int,AbstractMetaFunction *>(), QList<const AbstractMetaFunction *>(), QList<QString>(), QMap<QString,QList<const AbstractMetaFunction*>>(), false);
            QString pro_file_name = MetaInfoGenerator::subDirectoryForClass(java_class, MetaInfoGenerator::CppDirectory) + "/generated.pri";
            priGenerator->addSource(pro_file_name, fileNameForClass(java_class));
            break;
        }
    }
}

void CppImplGenerator::writeJavaLangObjectOverrideFunctions(QTextStream &s, const AbstractMetaClass *cls) {
    if (cls->hasHashFunction() && !cls->isNamespace()) {
        AbstractMetaFunctionList hashcode_functions = cls->queryFunctionsByName("hashCode");
        bool found = false;
        for(const AbstractMetaFunction *function : hashcode_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            s << Qt::endl
            << INDENT << jni_function_signature(cls->package(), cls->name(), "__qt_"+cls->name()+"_hashCode", "jint") << Qt::endl
            << INDENT << "(JNIEnv * __jni_env, jobject, QtJambiNativeID __this_nativeId)" << Qt::endl
            << INDENT << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "try{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << Qt::endl << "#if " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                    s << INDENT << cls->qualifiedCppName() << " *__qt_this = ";
                    if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                        s << "qtjambi_interface_from_nativeId<" << cls->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        s << INDENT << "qtjambi_check_resource(__jni_env, __qt_this);" << Qt::endl;
                    }else{
                        s << "qtjambi_object_from_nativeId<" << cls->typeEntry()->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        s << INDENT << "Q_ASSERT(__qt_this);" << Qt::endl;
                    }
                    s << INDENT << "return jint(qHash(*__qt_this));" << Qt::endl;
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << "#else" << Qt::endl
                          << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl
                          << INDENT<< "return 0;" << Qt::endl
                          << "#endif // " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                }
                s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
                s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                s << INDENT << "    return 0;" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }

    // Qt has a standard toString() conversion in QVariant?
    QVariant::Type type = QVariant::nameToType(cls->qualifiedCppName().toLatin1());
    if (type<QVariant::LastCoreType && QVariant(type).canConvert(QVariant::String) && !cls->toStringCapability()) {
        AbstractMetaFunctionList tostring_functions = cls->queryFunctionsByName("toString");
        bool found = false;
        for(const AbstractMetaFunction *function : tostring_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            QString name = cls->name();
            if(cls->typeEntry()->designatedInterface()){
                name = cls->typeEntry()->designatedInterface()->targetLangName();
            }
            s << Qt::endl
            << INDENT << jni_function_signature(cls->package(), cls->name(), "__qt_"+name+"_toString", "jstring")
            << "(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)" << Qt::endl
            << INDENT << "{" << Qt::endl;

            {
                INDENTATION(INDENT)
                s << INDENT << "try{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << Qt::endl << "#if " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                    s << INDENT << cls->qualifiedCppName() << " *__qt_this = ";
                    if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
                        s << "qtjambi_interface_from_nativeId<" << cls->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        s << INDENT << "qtjambi_check_resource(__jni_env, __qt_this);" << Qt::endl;
                    }else{
                        s << "qtjambi_object_from_nativeId<" << cls->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                        s << INDENT << "Q_ASSERT(__qt_this);" << Qt::endl;
                    }
                    s << INDENT << "return qtjambi_from_qstring(__jni_env, QVariant(*__qt_this).toString());" << Qt::endl;
                    if(!cls->typeEntry()->ppCondition().isEmpty()){
                        s << "#else" << Qt::endl
                          << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl
                          << INDENT<< "return nullptr;" << Qt::endl
                          << "#endif // " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                    }
                }
                s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
                s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
                s << INDENT << "    return nullptr;" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }
}

void CppImplGenerator::writeExtraFunctions(QTextStream &s, const AbstractMetaClass *java_class) {
    const ComplexTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    CodeSnipList code_snips = class_type->codeSnips();
    QStringList lines;
    for(const CodeSnip &snip : code_snips) {
        if (snip.language == TypeSystem::ShellCode || snip.language == TypeSystem::NativeCode) {
            lines << snip.code().replace("%this", "__qt_this").split("\n");
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
        s << INDENT << line << Qt::endl;
    }
}

void CppImplGenerator::writeToStringFunction(QTextStream &s, const AbstractMetaClass *java_class) {
    FunctionModelItem fun = java_class->toStringCapability();
    bool core = java_class->package() == QLatin1String("io.qt.core");
    bool qevent = false;

    const AbstractMetaClass *cls = java_class;
    while (cls) {
        if (cls->name() == "QEvent") {
            qevent = true;
            fun = cls->toStringCapability();
            break;
        }
        cls = cls->baseClass();
    }

    if (!java_class->hasDefaultToStringFunction() && fun && !(qevent && core)) {

        int indirections = fun->arguments().at(1)->type().indirections().size();
        QString deref = QLatin1String(indirections == 0 ? "*" : "");

        QString name = java_class->name();
        if(java_class->typeEntry() && java_class->typeEntry()->designatedInterface()){
            name = java_class->typeEntry()->designatedInterface()->targetLangName();
        }
        s << Qt::endl;
        s << "#include <QtCore/QDebug>" << Qt::endl;
        s << jni_function_signature(java_class->package(), java_class->name(), "__qt_"+name+"_toString", "jstring")
          << "(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)" << Qt::endl
          << INDENT << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "try{" << Qt::endl;
            {
                INDENTATION(INDENT)
                if(!java_class->typeEntry()->ppCondition().isEmpty()){
                    s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                }
                s << INDENT << java_class->qualifiedCppName() << " *__qt_this = ";
                if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                    s << "qtjambi_interface_from_nativeId<" << java_class->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                    s << INDENT << "qtjambi_check_resource(__jni_env, __qt_this);" << Qt::endl;
                }else{
                    s << "qtjambi_object_from_nativeId<" << java_class->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                    s << INDENT << "Q_ASSERT(__qt_this);" << Qt::endl;
                }

                s << INDENT << "QString res;" << Qt::endl
                  << INDENT << "QDebug d(&res);" << Qt::endl
                  << INDENT << "d << " << deref << "__qt_this;" << Qt::endl;
                s << INDENT << "return qtjambi_from_qstring(__jni_env, res);" << Qt::endl;
                if(!java_class->typeEntry()->ppCondition().isEmpty()){
                    s << Qt::endl << "#else" << Qt::endl
                      << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl
                      << INDENT << "return nullptr;" << Qt::endl
                      << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
                }
            }
            s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
            s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
            s << INDENT << "    return nullptr;" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
    }
}

void CppImplGenerator::writeCloneFunction(QTextStream &s, const AbstractMetaClass *java_class) {
    QString name = java_class->name();
    if(java_class->typeEntry() && java_class->typeEntry()->designatedInterface()){
        name = java_class->typeEntry()->designatedInterface()->targetLangName();
    }
    s << Qt::endl
      << jni_function_signature(java_class->package(), java_class->name(), "__qt_"+name+"_clone", "jobject") << Qt::endl
      << "(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)" << Qt::endl
      << INDENT << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "try{" << Qt::endl;
        {
            INDENTATION(INDENT)
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#if " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
            s << INDENT << "const " << java_class->qualifiedCppName() << " *__qt_this = ";
            if(java_class->typeEntry()->isInterface() || java_class->typeEntry()->designatedInterface() || java_class->typeEntry()->isFunctional()){
                s << "qtjambi_interface_from_nativeId<" << java_class->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                s << INDENT << "qtjambi_check_resource(__jni_env, __qt_this);" << Qt::endl;
            }else{
                s << "qtjambi_object_from_nativeId<" << java_class->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
                s << INDENT << "Q_ASSERT(__qt_this);" << Qt::endl;
            }
            s << INDENT << "return qtjambi_cast<jobject>(__jni_env, *__qt_this);" << Qt::endl;
            if(!java_class->typeEntry()->ppCondition().isEmpty()){
                s << Qt::endl << "#else" << Qt::endl
                  << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl
                  << INDENT << "return nullptr;" << Qt::endl
                  << "#endif // " << java_class->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
            }
        }
        s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
        s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
        s << INDENT << "    return nullptr;" << Qt::endl;
        s << INDENT << "}" << Qt::endl;
    }
    s << INDENT << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeQObjectFunctions(QTextStream &s, const AbstractMetaClass *java_class) {
    // QObject::metaObject()
    s << "const QMetaObject *" << shellClassName(java_class) << "::metaObject() const" << Qt::endl
      << "{" << Qt::endl
      << "  return __shell()->metaObject();" << Qt::endl
      << "}" << Qt::endl << Qt::endl;

    // QObject::qt_metacast()
    s << "void *" << shellClassName(java_class) << "::qt_metacast(const char *_clname)" << Qt::endl
      << "{" << Qt::endl
      << "    bool ok = false;" << Qt::endl
      << "    void *rv = __shell()->qt_metacast(_clname, &ok);" << Qt::endl
      << "    QTJAMBI_DEBUG_METHOD_PRINT_WHERE(\"shell\", \"" << shellClassName(java_class) << "::qt_metacast(const char *_clname)\", __shell())" << Qt::endl
      << "    if (!ok)" << Qt::endl
      << "        rv = " << java_class->qualifiedCppName() << "::qt_metacast(_clname);" << Qt::endl
      << "    return rv;" << Qt::endl
      << "}" << Qt::endl << Qt::endl;

    // QObject::qt_metacall()
    s << "int " << shellClassName(java_class) << "::qt_metacall(QMetaObject::Call _c, int _id, void **_a)" << Qt::endl
      << "{" << Qt::endl;

    s << "    QTJAMBI_DEBUG_METHOD_PRINT_WHERE(\"shell\", \"" << shellClassName(java_class) << "::qt_metacall(QMetaObject::Call _c, int _id, void **_a)\", __shell())" << Qt::endl;

    if (java_class->hasVirtualSlots()) {
        s << "    typedef void(*VirtualSlot)(" << shellClassName(java_class) << " *,void **);" << Qt::endl
          << "    static QHash<int,VirtualSlot> virtualSlots = []()->QHash<int,VirtualSlot>{" << Qt::endl
          << "            QHash<int,VirtualSlot> virtualSlots;" << Qt::endl
          << "            int idx;" << Qt::endl;
        AbstractMetaFunctionList virtualFunctions = java_class->virtualFunctions();
        for (int pos = 0; pos < virtualFunctions.size(); ++pos) {
            const AbstractMetaFunction *virtualFunction = virtualFunctions.at(pos);
            if (virtualFunction->hasRReferences())
                continue;
            if (virtualFunction->isVirtualSlot()) {
                const AbstractMetaArgumentList& arguments = virtualFunction->arguments();
                AbstractMetaType *function_type = virtualFunction->type();
                s << "            idx = " << java_class->qualifiedCppName() << "::staticMetaObject.indexOfMethod(\"" << virtualFunction->minimalSignature() << "\") - " << java_class->qualifiedCppName() << "::staticMetaObject.methodOffset();" << Qt::endl;
                s << "            if(idx>=0)" << Qt::endl;
                s << "                virtualSlots[idx] = [](" << shellClassName(java_class) << "*_this, void **";
                if(function_type || !arguments.isEmpty())
                    s << "_a";
                s << "){" << Qt::endl;
                // Set up variable names so the function call works
                for (int i = 1; i <= arguments.size(); ++i) {
                    AbstractMetaArgument *argument = arguments.at(i - 1);

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
      << "        _id = __shell()->qt_metacall(_c, _id, _a);" << Qt::endl;
    s << "    return _id;" << Qt::endl
      << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeShellConstructor(QTextStream &s, bool, const AbstractMetaFunction *java_function, int options) {
    if (java_function->hasRReferences())
        return ;
    if (java_function->isModifiedRemoved(TypeSystem::ShellCode))
        return;

    const AbstractMetaClass *cls = java_function->ownerClass();

    const AbstractMetaArgumentList& arguments = java_function->arguments();

    writeFunctionSignature(s, java_function, cls, /*default-arg*/QString(), java_function->isPublic() ? Option(options) : Option(options | EnumAsInts));

    s << Qt::endl;
    s << "    : " << cls->qualifiedCppName() << "(";
    bool needComma = false;
    for (int i = 0; i < arguments.size(); ++i) {
        if ((options & SkipRemovedArguments) == SkipRemovedArguments
                && java_function->argumentRemoved(i + 1)!=ArgumentRemove_No) {
            if(!arguments.at(i)->originalDefaultValueExpression().isEmpty()
                && java_function->conversionRule(TypeSystem::NativeCode, i + 1).isEmpty()
                && java_function->conversionRule(TypeSystem::TargetLangCode, i + 1).isEmpty()){
                continue;
            }
        }
        if(needComma)
            s << ", ";
        else
            needComma = true;
        const AbstractMetaArgument* argument = arguments.at(i);
        if(!java_function->isPublic() && (argument->type()->typeEntry()->isEnum() || argument->type()->typeEntry()->isFlags()) ){
            s << argument->type()->typeEntry()->qualifiedCppName() << "(" << argument->indexedName() << ")";
        }else{
            s << argument->indexedName();
        }
    }
    s << ")" << Qt::endl;
    s << "{" << Qt::endl;
    {
        INDENTATION(INDENT)

        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT_WHERE(\"shell\", \"";
        writeFunctionSignature(s, java_function, cls);
        s << "\", __shell())" << Qt::endl;
        s << INDENT << "__shell()->constructed(typeid(" << cls->qualifiedCppName() << "));" << Qt::endl;

        writeCodeInjections(s, java_function, cls, CodeSnip::Beginning, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");
        writeCodeInjections(s, java_function, cls, CodeSnip::End, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeShellConstructor(QTextStream &s, const AbstractMetaFunctional *java_class) {
    QString javaName = java_class->name();
    if(java_class->enclosingClass()){
        javaName = java_class->enclosingClass()->fullName() + "$" + javaName;
    }
    javaName = javaName.replace(".", "/");
    s << "void deleter_" << QString(java_class->typeEntry()->name()).replace("::", "_") << "(void *ptr) { delete reinterpret_cast<" << shellClassName(java_class) << "*>(ptr); }" << Qt::endl << Qt::endl;
    s << Qt::endl;
    s << shellClassName(java_class) << "::" << shellClassName(java_class)
      << "()" << Qt::endl
      << "    : FunctionalBase()" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"shell\", \"" << shellClassName(java_class) << "::" << shellClassName(java_class) << "()\")" << Qt::endl;
        s << INDENT << "__shell()->constructed(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeShellDestructor(QTextStream &s, bool, const AbstractMetaClass *java_class) {
    s << shellClassName(java_class) << "::~" << shellClassName(java_class) << "()" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT_WHERE(\"shell\", \""
          << shellClassName(java_class) << "::~"
          << shellClassName(java_class) << "()\", __shell())" << Qt::endl;
        QStringList lines;
        QSet<const ComplexTypeEntry *> classes;
        const AbstractMetaClass *_cls = java_class;
        while(_cls){
            classes << (_cls->typeEntry()->designatedInterface() ? _cls->typeEntry()->designatedInterface() : _cls->typeEntry());
            for(const AbstractMetaClass *ifc : _cls->interfaces()){
                classes << (ifc->typeEntry()->designatedInterface() ? ifc->typeEntry()->designatedInterface() : ifc->typeEntry());
            }
            _cls = _cls->baseClass();
        }
        for(const ComplexTypeEntry * _cls : classes){
            for(const CodeSnip& snip : _cls->codeSnips()) {
                if (snip.language == TypeSystem::DestructorFunction) {
                    lines << snip.code().replace("%this", "this").split("\n");
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
            s << INDENT << line << Qt::endl;
        }
        s << INDENT << "__shell()->destructed(typeid(" << java_class->qualifiedCppName() << "));" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeShellDestructor(QTextStream &s, const AbstractMetaFunctional *java_class) {
    s << shellClassName(java_class) << "::~" << shellClassName(java_class) << "()" << Qt::endl
      << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT_WHERE(\"shell\", \""
          << shellClassName(java_class) << "::~"
          << shellClassName(java_class) << "()\", __shell())" << Qt::endl;
        if(java_class->isFunctionPointer()){
            s << INDENT << "if(m_functionPointerDeleter)" << Qt::endl;
            s << INDENT << "    m_functionPointerDeleter();" << Qt::endl;
        }
        s << INDENT << "__shell()->destructed(typeid(" << java_class->typeEntry()->qualifiedCppName() << "));" << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeCodeInjections(QTextStream &s,
        const ComplexTypeEntry *typeEntry, CodeSnip::Position position,
        TypeSystem::Language language) {
    for(const CodeSnip& snip : typeEntry->codeSnips()) {
        if (snip.position != position)
            continue ;

        if ((snip.language & language) == false)
            continue ;

        if (position == CodeSnip::End)
            s << Qt::endl;

        QString code = snip.code();
        ArgumentMap map = snip.argumentMap;
        ArgumentMap::iterator it = map.begin();
        for (;it != map.end();++it) {
            QString meta_name = it.value();
            QString debug = QString("argument map specifies invalid argument index %1"
                                    "for class '%2'")
                            .arg(it.key()).arg(typeEntry->name());
            ReportHandler::warning(debug);
        }
        QStringList lines = code.split("\n");
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
            s << INDENT << line << Qt::endl;
        }
        if (position == CodeSnip::Beginning)
            s << Qt::endl;
    }
}


void CppImplGenerator::writeCodeInjections(QTextStream &s, const AbstractMetaFunction *java_function,
        const AbstractMetaClass *implementor, CodeSnip::Position position,
        TypeSystem::Language language, const QString& __jni_env, const QString& qtjambi_scope, Option option) {

    FunctionModificationList mods;
    const AbstractMetaClass *cls = implementor;
    while (cls!= nullptr) {
        mods += java_function->modifications(cls);

        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    QString def;
    if(java_function->type()){
        switch(language){
        case TypeSystem::ShellCode:
            def = default_return_statement_qt(java_function->type(), Generator::NoReturnStatement);
            break;
        case TypeSystem::NativeCode:
            def = default_return_statement_java(java_function->type(), Generator::NoReturnStatement);
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
            QStringList lines = code.split("\n");
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
                s << INDENT << line << Qt::endl;
            }
            if (position == CodeSnip::Beginning)
                s << Qt::endl;
        }
    }
}

bool CppImplGenerator::hasCodeInjections(const AbstractMetaFunction *java_function,
        const AbstractMetaClass *implementor, const QSet<CodeSnip::Position>& positions,
        TypeSystem::Language language) {

    const AbstractMetaClass *cls = implementor;
    while (cls!= nullptr) {
        if(java_function->hasCodeInjections(cls, language, positions))
            return true;
        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    return false;
}

bool CppImplGenerator::hasCodeInjections(const AbstractMetaFunctional *java_function,
        const QSet<CodeSnip::Position>& positions,
        TypeSystem::Language language) {
    Q_UNUSED(java_function)
    Q_UNUSED(positions)
    Q_UNUSED(language)
    return false;
}

static QString function_call_for_ownership(TypeSystem::Ownership owner, const QString &var_name) {
    if (owner == TypeSystem::CppOwnership) {
        return "setCppOwnership(__jni_env, " + var_name + ")";
    } else if (owner == TypeSystem::TargetLangOwnership) {
        return "setJavaOwnership(__jni_env, " + var_name + ")";
    } else if (owner == TypeSystem::DefaultOwnership) {
        return "setDefaultOwnership(__jni_env, " + var_name + ")";
    } else {
        Q_ASSERT(false);
        return "bogus()";
    }
}

bool CppImplGenerator::hasOwnership(const AbstractMetaFunction *java_function,
                    int var_index,
                    const AbstractMetaClass *implementor,
                    TypeSystem::Language language)
{
    OwnershipRule ownershipRule = {TypeSystem::InvalidOwnership, QString()};
    const AbstractMetaClass *cls = implementor;
    while (cls!= nullptr && ownershipRule.ownership == TypeSystem::InvalidOwnership) {
        ownershipRule = java_function->ownership(cls, language, var_index);
        cls = cls->baseClass();
    }
    return ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership;
}

void CppImplGenerator::writeOwnership(QTextStream &s,
                                      const AbstractMetaFunction *java_function,
                                      const QString &var_name,
                                      const QString &qt_var_name,
                                      int var_index,
                                      const AbstractMetaClass *implementor,
                                      TypeSystem::Language language,
                                      const QString& __jni_env,
                                      const QString& qtjambi_scope) {
    Q_UNUSED(qtjambi_scope)
    OwnershipRule ownershipRule = {TypeSystem::InvalidOwnership, QString()};
    const AbstractMetaClass *cls = implementor;
    while (cls!= nullptr && ownershipRule.ownership == TypeSystem::InvalidOwnership) {
        ownershipRule = java_function->ownership(cls, language, var_index);
        cls = cls->baseClass();
    }

    if (ownershipRule.ownership == TypeSystem::InvalidOwnership || ownershipRule.ownership == TypeSystem::IgnoreOwnership)
        return;

    if (var_index != -1) {
        const AbstractMetaType* type;
        if(var_index==0){
            type = java_function->type();
        }else{
            type = java_function->arguments().at(var_index-1)->type();
        }
        if(type && type->isQObject()){
            if (ownershipRule.ownership == TypeSystem::Invalidate)
                return;
            s << INDENT;
            if(!ownershipRule.condition.isEmpty())
                s << "if(" << ownershipRule.condition << ")" << Qt::endl
                  << INDENT << "    ";
            switch(ownershipRule.ownership){
            case TypeSystem::TargetLangOwnership:
                s << "qtjambi_set_java_ownership_for_toplevel_object"; break;
            case TypeSystem::CppOwnership:
                s << "qtjambi_set_cpp_ownership_for_toplevel_object"; break;
            default:
                s << "qtjambi_set_default_ownership_for_toplevel_object"; break;
            }
            s << "(" << __jni_env << ", " << qt_var_name << ");" << Qt::endl;
        }else{
            s << INDENT;
            if(!ownershipRule.condition.isEmpty())
                s << "if(" << ownershipRule.condition << ")" << Qt::endl
                  << INDENT << "    ";
            switch(ownershipRule.ownership){
            case TypeSystem::TargetLangOwnership:
                s << "qtjambi_set_java_ownership"; break;
            case TypeSystem::CppOwnership:
                s << "qtjambi_set_cpp_ownership"; break;
            case TypeSystem::Invalidate:
                s << "qtjambi_invalidate_object"; break;
            default:
                s << "qtjambi_set_default_ownership"; break;
            }
            s << "(" << __jni_env << ", " << var_name << ");" << Qt::endl;
        }
    } else {
        if(language==TypeSystem::ShellCode){
            QScopedPointer<Indentation> indentation;
            if(java_function->ownerClass()->isQObject()){
                if(ownershipRule.condition.isEmpty())
                    s << INDENT << "if (!parent()) {" << Qt::endl;
                else
                    s << INDENT << "if (" << ownershipRule.condition << " && !parent()) {" << Qt::endl;
                indentation.reset(new Indentation(INDENT));
            }else if(!ownershipRule.condition.isEmpty()){
                s << INDENT << "if (" << ownershipRule.condition << ") {" << Qt::endl;
                indentation.reset(new Indentation(INDENT));
            }
            s << INDENT << "jobject __obj_tmp = __shell()->getJavaObjectLocalRef(" << __jni_env << ");" << Qt::endl;
            s << INDENT << "__link->" << function_call_for_ownership(ownershipRule.ownership, "__obj_tmp") << ";" << Qt::endl;
            if(!indentation.isNull()){
                s << INDENT << "}" << Qt::endl;
                indentation.reset();
            }
        }else{
            QScopedPointer<Indentation> indentation;
            if(java_function->ownerClass()->isQObject()){
                if(ownershipRule.condition.isEmpty())
                    s << INDENT << "if (!parent()) {" << Qt::endl;
                else
                    s << INDENT << "if (" << ownershipRule.condition << " && !parent()) {" << Qt::endl;
                indentation.reset(new Indentation(INDENT));
            }else if(!ownershipRule.condition.isEmpty()){
                s << INDENT << "if (" << ownershipRule.condition << ") {" << Qt::endl;
                indentation.reset(new Indentation(INDENT));
            }
            s << INDENT << "jobject __obj_tmp = qtjambi_java_object_reference(__this_nativeId, " << __jni_env << ");" << Qt::endl;
            s << INDENT << "__link->" << function_call_for_ownership(ownershipRule.ownership, "__obj_tmp") << ";" << Qt::endl;
            if(!indentation.isNull()){
                s << INDENT << "}" << Qt::endl;
                indentation.reset();
            }
        }
    }

}

void CppImplGenerator::writeShellFunction(QTextStream &s, const AbstractMetaFunction *java_function, const AbstractMetaClass *implementor, int id) {
    if (java_function->hasRReferences())
        return;
    if (java_function->isFinalInCpp() && !java_function->isVirtualSlot())
        return;
    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty()){
        s << "#if " << pps.join(" && ") << Qt::endl;
    }

    writeFunctionSignature(s, java_function, implementor, QString(), OriginalName);

    s << Qt::endl
      << "{" << Qt::endl;
    {
        AbstractMetaType *function_type = java_function->type();
        const AbstractMetaArgumentList& arguments = java_function->arguments();
        INDENTATION(INDENT)
        QString java_function_signature = java_function->signature();
        if(java_function->isStatic()){
            s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"shell\", \"" << implementor->typeEntry()->qualifiedCppName() << "::"
              << java_function_signature << "\")" << Qt::endl;
        }else{
            s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT_WHERE(\"shell\", \"" << implementor->typeEntry()->qualifiedCppName() << "::"
              << java_function_signature << "\", __shell())" << Qt::endl;
        }

        bool needEnv = true;
        QScopedPointer<Indentation> _indent;
        if(java_function->isNoExcept()){
            needEnv = false;
            s << INDENT << "JNIEnv *__jni_env(nullptr);" << Qt::endl;
            s << INDENT << "try{" << Qt::endl;
            _indent.reset(new Indentation(INDENT));
        }

        bool isRemovedFromTargetLang = java_function->isRemovedFrom(implementor, TypeSystem::TargetLangCode)
                                        || java_function->isRemovedFrom(java_function->declaringClass(), TypeSystem::TargetLangCode);

        if (!isRemovedFromTargetLang && (!java_function->isFinalInCpp() || java_function->isVirtualSlot())) {
            if(needEnv)
                s << INDENT << "JNIEnv *__jni_env(nullptr);" << Qt::endl;
            s << INDENT << "jmethodID method_id = __shell_javaMethod(" << id << ");" << Qt::endl;
            s << INDENT << "if(method_id && (__jni_env = qtjambi_current_environment())) {" << Qt::endl;
            {
                INDENTATION(INDENT)
                QString new_return_type = java_function->typeReplaced(0);
                bool has_function_type = ((function_type!= nullptr
                                           || !new_return_type.isEmpty())
                                          && new_return_type != "void");
                if (function_type) {
                    s << INDENT;
                    writeTypeInfo(s, function_type);
                    if(function_type && (function_type->isPrimitive() || function_type->isEnum() || !function_type->indirections().isEmpty())){
                        s << " __qt_return_value = " << default_return_statement_qt(function_type, Generator::NoReturnStatement) << ";" << Qt::endl;
                    }else{
                        s << " __qt_return_value;" << Qt::endl;	// declaration only
                    }
                }

                s << INDENT << "QTJAMBI_JNI_LOCAL_FRAME(__jni_env, " << QString::number(100*(arguments.size()+2)) << ")" << Qt::endl
                  << INDENT << "if(jobject __java_this = __shell()->getJavaObjectLocalRef(__jni_env)){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    {
                        QString lines;
                        {
                            QTextStream s(&lines);
                            writeCodeInjections(s, java_function, implementor, CodeSnip::Beginning, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");

                            for(AbstractMetaArgument *argument : arguments) {
                                if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
                                    if (!argument->type()->isPrimitive()
                                            || java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
                                        writeQtToJava(s,
                                                      argument->type(),
                                                      argument->indexedName(),
                                                      "__java_" + argument->indexedName(),
                                                      java_function, nullptr,
                                                      argument->argumentIndex() + 1,
                                                      NoOption,
                                                      "__jni_env", "__qtjambi_scope");
                                    }

                                    if (java_function->resetObjectAfterUse(argument->argumentIndex() + 1)){
                                        if (argument->type()->isContainer()){
                                            s << INDENT << "QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ")" << Qt::endl;
                                        }else if (argument->type()->isArray()){
                                            s << INDENT << "QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ")" << Qt::endl;
                                        }else{
                                            s << INDENT << "QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_" << argument->indexedName() << ")" << Qt::endl;
                                        }
                                    }
                                }
                            }

                            for (int i = 0; i < arguments.size(); ++i)
                                writeOwnership(s, java_function, "__java_" + arguments.at(i)->indexedName(), "__qt_" + arguments.at(i)->indexedName(), i + 1, implementor, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");

                            s << INDENT;
                            QString typeWrap;
                            if (has_function_type) {
                                if (new_return_type.isEmpty()) {
                                    s << translateType(function_type);
                                } else {
                                    QString _jniName = jniName(new_return_type);
                                    if(_jniName=="jstring"
                                            || _jniName=="jclass"
                                            || _jniName=="jthrowable"
                                            || _jniName.endsWith("Array"))
                                        typeWrap = _jniName;
                                    s << _jniName;
                                }
                                s << " " << "__java_return_value = ";	// declaration only
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
                            s << INDENT << "qtjambi_throw_java_exception(__jni_env)" << Qt::endl;

                            if (has_function_type) {
                                if(java_function->hasConversionRule(TypeSystem::ShellCode, 0)){
                                    writeJavaToQt(s, function_type, "__qt_return_value_tmp", "__java_return_value",
                                                  java_function, nullptr, 0, Option(GlobalRefJObject), "__jni_env",
                                                  "__qtjambi_scope");
                                    if(java_function->type())
                                        s << INDENT << "__qt_return_value = __qt_return_value_tmp;" << Qt::endl;
                                }else{
                                    writeJavaToQt(s, function_type, "__qt_return_value", "__java_return_value",
                                                  java_function, nullptr, 0, Option(NoTmpVariable | GlobalRefJObject), "__jni_env",
                                                  "__qtjambi_scope");
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
                                        s << INDENT << "JavaException::raiseNullPointerException(__jni_env, qPrintable(QString(\"Method %1." << java_function->name()
                                          << " must not return null.\").arg(qtjambi_object_class_name(__jni_env, __java_this))) QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                                    }

                                    s << INDENT << "}" << Qt::endl;
                                }
                            } else if (java_function->hasConversionRule(TypeSystem::ShellCode, 0)) {
                                writeConversionRule(s, TypeSystem::ShellCode, java_function, nullptr, 0, "<invalid>", "<invalid>", NoOption, "__jni_env", "__qtjambi_scope");
                            }

                            writeOwnership(s, java_function, "this", "__java_this", -1, implementor, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");
                            writeOwnership(s, java_function, "__java_return_value", "__qt_return_value", 0, implementor, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");
                            writeCodeInjections(s, java_function, implementor, CodeSnip::End, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");
                        }
                        if(lines.contains("__qtjambi_scope")){
                            s << INDENT << "QtJambiScope __qtjambi_scope(__shell());" << Qt::endl;
                        }
                        s << lines;
                    }
                }
                s << INDENT << "} else {" << Qt::endl;         // !__shell()->getJavaObjectLocalRef(__jni_env)
                {
                    INDENTATION(INDENT)
                    if(implementor->isQObject()){
                        if(arguments.size()==1
                                && arguments.at(0)->type()->typeEntry()->qualifiedCppName()=="QEvent"
                                && java_function->name()=="event"){
                            s << INDENT << "if(" << arguments.at(0)->indexedName() << "->type()==QEvent::DeferredDelete || " << arguments.at(0)->indexedName() << "->type()==QEvent::ChildRemoved){" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "QTJAMBI_DEBUG_TRACE(\"(shell) -> super()\")" << Qt::endl;
                                if(java_function->isAbstract()){
                                    s << INDENT << "return QObject::event(" << arguments.at(0)->indexedName() << ");" << Qt::endl;
                                }else{
                                    writeBaseClassFunctionCall(s, java_function, implementor);
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else if(arguments.size()==2
                                 && arguments.at(0)->type()->typeEntry()->qualifiedCppName()=="QObject"
                                 && arguments.at(1)->type()->typeEntry()->qualifiedCppName()=="QEvent"
                                 && java_function->name()=="eventFilter"){
                             s << INDENT << "if(" << arguments.at(1)->indexedName() << "->type()==QEvent::DeferredDelete || " << arguments.at(1)->indexedName() << "->type()==QEvent::ChildRemoved){" << Qt::endl;
                             {
                                 INDENTATION(INDENT)
                                 s << INDENT << "QTJAMBI_DEBUG_TRACE(\"(shell) -> super()\")" << Qt::endl;
                                 if(java_function->isAbstract()){
                                     s << INDENT << "return QObject::eventFilter("
                                       << arguments.at(0)->indexedName()
                                       << ", " << arguments.at(1)->indexedName() << ");" << Qt::endl;
                                 }else{
                                     writeBaseClassFunctionCall(s, java_function, implementor);
                                 }
                             }
                             s << INDENT << "}" << Qt::endl;
                        }
                    }
                    // FIXME: Use helper method
                    // Raise exception ?  Maybe this should be QtJambiNoNativeException
                    if(implementor->isQObject()){
                        s << INDENT << "__shell()->warnForMethod(\"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function_signature << "\", this);" << Qt::endl;
                    }else{
                        s << INDENT << "__shell()->warnForMethod(\"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function_signature << "\");" << Qt::endl;
                    }
                    QString defaultValue = java_function->nullPointerDefaultValue();
                    if (!defaultValue.isEmpty()){
                        s << INDENT << "__qt_return_value = " << defaultValue << ";";
                    }
                }
                s << INDENT << "}" << Qt::endl;
                if (function_type){
                    if(java_function->nullPointersDisabled()){
                        s << INDENT << "Q_ASSERT_X(__qt_return_value!=nullptr, \"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function->minimalSignature() << "\", \"Unexpected null pointer returned.\");" << Qt::endl;
                    }
                    s << INDENT << "return __qt_return_value;" << Qt::endl;
                }
            }
            if(java_function->isAbstract()){
                if (function_type) {
                    s << INDENT << "} else {" << Qt::endl;
                    INDENTATION(INDENT)
                    s << INDENT << default_return_statement_qt(function_type, Generator::NoOption) << ";" << Qt::endl;
                }else{
                }
            }else{
                s << INDENT << "} else {" << Qt::endl;
                INDENTATION(INDENT)
                s << INDENT << "QTJAMBI_DEBUG_TRACE(\"(shell) -> super()\")" << Qt::endl;
                writeBaseClassFunctionCall(s, java_function, implementor);
            }
            s << INDENT << "}" << Qt::endl;
        } else {
            writeCodeInjections(s, java_function, implementor, CodeSnip::Beginning, TypeSystem::ShellCode, "__jni_env", "");
            if (isRemovedFromTargetLang || java_function->isAbstract()) {
                // Avoid compiler warnings for unused parameters
                const AbstractMetaArgumentList& arguments = java_function->arguments();

                for(const AbstractMetaArgument *argument : arguments) {
                    if(java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No)
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                }
            }
            writeBaseClassFunctionCall(s, java_function, implementor);
            writeCodeInjections(s, java_function, implementor, CodeSnip::End, TypeSystem::ShellCode, "__jni_env", "");
        }

        if(_indent.get()){
            _indent.reset();
            s << INDENT << "}catch(const JavaException& exn){" << Qt::endl
              << INDENT << "    if(!__jni_env)" << Qt::endl
              << INDENT << "        __jni_env = qtjambi_current_environment();" << Qt::endl
              << INDENT << "    if(__jni_env) {" << Qt::endl
              << INDENT << "        exn.report(__jni_env);" << Qt::endl
              << INDENT << "    }else{" << Qt::endl
              << INDENT << "        qWarning(\"Exception occurred in " << implementor->typeEntry()->qualifiedCppName() << "::" << java_function_signature << ": %s\", exn.what());" << Qt::endl
              << INDENT << "    }" << Qt::endl;
            if(function_type){
                INDENTATION(INDENT)
                if(java_function->nullPointersDisabled()){
                    s << INDENT << "Q_ASSERT_X(false, \"" << implementor->typeEntry()->qualifiedCppName() << "::" << java_function->minimalSignature() << "\", \"Unexpected null pointer returned.\");" << Qt::endl;
                }
                s << INDENT << default_return_statement_qt(function_type, Generator::NoOption) << ";" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }
    s << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(" && ") << Qt::endl;
    s << Qt::endl;
}

// ### kill implementor

void CppImplGenerator::writePublicFunctionOverride(QTextStream &s,
        const AbstractMetaFunction *java_function,
        const AbstractMetaClass *implementor) {
    if (java_function->hasRReferences())
        return;

    Q_ASSERT(java_function->originalAttributes()
             & (AbstractMetaAttributes::Protected
                | AbstractMetaAttributes::Final));

    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty())
        s << "#if " << pps.join(" && ") << Qt::endl;
    // The write a public override version of this function to be used by native functions
    writeFunctionSignature(s, java_function, implementor, "__public_",
                           Option(UnderscoreSpaces | EnumAsInts
                                  | (java_function->isAbstract() ? SkipName : NoOption)));
    s << Qt::endl
    << "{" << Qt::endl;
    INDENTATION(INDENT)
    writeBaseClassFunctionCall(s, java_function, implementor);
    s << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(" && ") << Qt::endl;
    s << Qt::endl;
}


void CppImplGenerator::writeVirtualFunctionOverride(QTextStream &s,
        const AbstractMetaFunction *java_function,
        const AbstractMetaClass *implementor) {
    if (java_function->hasRReferences() || java_function->wasPrivate())
        return ;

    QStringList pps = getFunctionPPConditions(java_function);
    if(!pps.isEmpty())
        s << "#if " << pps.join(" && ") << Qt::endl;

    Q_ASSERT(!java_function->isFinalInCpp());

    Option options = Option(EnumAsInts | UnderscoreSpaces);

    // The write a public override version of this function to be used by native functions
    writeFunctionSignature(s, java_function, implementor, "__override_",
                           options,
                           QString(), // the class prefix
                           QStringList() << ( java_function->isAbstract() ? "JNIEnv *__jni_env, QtJambiNativeID __this_nativeId" : "QtJambiNativeID __this_nativeId") );
    s << Qt::endl
      << "{" << Qt::endl;
    INDENTATION(INDENT)
    s << INDENT << "Q_ASSERT(!!__this_nativeId);" << Qt::endl;
    s << INDENT << "if (qtjambi_is_shell(__this_nativeId)) {" << Qt::endl;
    {
        INDENTATION(INDENT)
        if(java_function->isAbstract()){
            s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"Call of pure virtual method " << implementor->qualifiedCppName() << "::" << java_function->signature() << ".\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
        }
        writeBaseClassFunctionCall(s, java_function, implementor, Option(EnumFromInt));
    }
    s << INDENT << "} else {" << Qt::endl;
    {
        INDENTATION(INDENT)
        writeBaseClassFunctionCall(s, java_function, implementor, Option(EnumFromInt | VirtualCall));
    }

    s << INDENT << "}" << Qt::endl
      << "}" << Qt::endl;
    if(!pps.isEmpty())
        s << "#endif //" << pps.join(" && ") << Qt::endl;
    s << Qt::endl;
}


bool CppImplGenerator::writeBaseClassFunctionCall(QTextStream &s,
        const AbstractMetaFunction *java_function,
        const AbstractMetaClass *,
        Option options) {
    bool rv = false;    // did we emit a "return"
    if ((options & NoReturnStatement) == 0)
        s << INDENT;
    if (java_function->isAbstract() && !(options & VirtualCall)) {
        s << default_return_statement_qt(java_function->type(), options) << ";" << Qt::endl;
        if ((options & NoReturnStatement) == 0)
            rv = true;
    } else {
        const AbstractMetaClass *implementor = java_function->implementingClass();
        if (java_function->isInterfaceFunction())
            implementor = java_function->interfaceClass()->primaryInterfaceImplementor();
        if ((options & VirtualCall) == VirtualCall
                && !(options & SuperCall)
                && java_function->isInterfaceFunction()
                && java_function->wasProtected()){
            //create accessor class for protected interface functions
            s << "struct " << "__qt_ProtectedAccess : public " << implementor->qualifiedCppName() << "{" << Qt::endl;
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
        }
        if ((options & VirtualCall) == VirtualCall) {
            if(java_function->isInterfaceFunction() && !(options & SuperCall)){
                if(java_function->wasProtected()){
                    //create accessor class for protected interface functions
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
            s << "<";
            for(int i=0; i<java_function->templateParameters().size(); i++){
                AbstractMetaTemplateParameter* param = java_function->templateParameters()[i];
                if(param->type()){
                    writeTypeInfo(s, param->type());
                }else{
                    s << param->instantiation();
                }
                if(i<java_function->templateParameters().size()-1){
                    s << ", ";
                }
            }
            s << " >";
        }
        s << "(";
        writeFunctionCallArguments(s, java_function, QString(), Option(options | ForceEnumCast));
        s << ");" << Qt::endl;
    }
    return rv;
}


void CppImplGenerator::writeFunctionName(QTextStream &s,
        const AbstractMetaFunction *java_function,
        const AbstractMetaClass *java_class,
        uint options) {
    const AbstractMetaClass *cls = java_class ? java_class : java_function->ownerClass();
    const AbstractMetaArgumentList& arguments = java_function->arguments();

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

    if (callThrough && !java_function->isStatic() && !java_function->isConstructor())
        args += "J";

    if(java_function->isConstructor())
        args += jni_signature("java.lang.Object", Underscores);

    if (!arguments.isEmpty()) {
        for(const AbstractMetaArgument *argument : arguments) {
            if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
                QString modified_type = java_function->typeReplaced(argument->argumentIndex()+1);
                if(java_function->argumentTypeArray(argument->argumentIndex() + 1)){
                    QScopedPointer<AbstractMetaType> array(argument->type()->copy());
                    array->setConstant(false);
                    array->setReferenceType(AbstractMetaType::NoReference);
                    array->setTypeUsagePattern(AbstractMetaType::ArrayPattern);
                    QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                    cpy->setConstant(false);
                    cpy->setReferenceType(AbstractMetaType::NoReference);
                    QList<bool> indirections = cpy->indirections();
                    if(!indirections.isEmpty()){
                        indirections.removeLast();
                        cpy->setIndirections(indirections);
                    }
                    AbstractMetaBuilder::decideUsagePattern(cpy.get());
                    array->setArrayElementType(cpy.get());
                    if(java_function->isConstructor()){
                        args += jni_signature(array.get(), JNISignatureFormat(Underscores | NoModification));
                    }else{
                        args += jni_signature(array.get(), Underscores);
                    }
                }else if(java_function->argumentTypeBuffer(argument->argumentIndex() + 1)){
                    QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                    cpy->setConstant(false);
                    cpy->setReferenceType(AbstractMetaType::NoReference);
                    QList<bool> indirections = cpy->indirections();
                    if(!indirections.isEmpty()){
                        indirections.removeLast();
                        cpy->setIndirections(indirections);
                    }
                    AbstractMetaBuilder::decideUsagePattern(cpy.get());
                    JNISignatureFormat format;
                    if(java_function->isConstructor()){
                        format = JNISignatureFormat(Underscores | NoModification);
                    }else{
                        format = Underscores;
                    }
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
                }else if (!modified_type.isEmpty() || !argument->type()->hasNativeId() || java_function->isConstructor()) {
                    if (modified_type.isEmpty()){
                        if(java_function->isConstructor()){
                            args += jni_signature(argument->type(), JNISignatureFormat(Underscores | NoModification));
                        }else{
                            args += jni_signature(argument->type(), Underscores);
                        }
                    }else{
                        args += jni_signature(modified_type, Underscores);
                    }
                } else {
                    args += "J";
                }
            }
        }
    }
    for(const ArgumentModification *mod : java_function->addedArguments()){
        args += jni_signature(mod->modified_type, Underscores);
    }

    s << jni_function_signature(cls->package(), cls->name(), function_name,
                                return_type, args, options);

}

void CppImplGenerator::writeFinalFunctionArguments(QTextStream &s, const AbstractMetaFunction *java_function,
        const QString &java_object_name) {
    bool callThrough = java_function->needsCallThrough();

    s << "(JNIEnv *__jni_env," << Qt::endl;
    if (java_function->isConstructor()){
        s << " jclass __jni_class," << Qt::endl
          << " jobject " << java_object_name;
    }else if(java_function->isStatic()){
        s << " jclass";
    }else{
        s << " jobject";
    }

    bool hasNativeId = (callThrough && !java_function->isStatic() && !java_function->isConstructor());
    if (hasNativeId)
        s << "," << Qt::endl << " QtJambiNativeID __this_nativeId";

    // the function arguments
    const AbstractMetaArgumentList& arguments = java_function->arguments();
    Option options = Option(EnumAsInts | UseNativeIds);
    if(java_function->isConstructor())
        options = NoOption;
    for(const AbstractMetaArgument *argument : arguments) {
        if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No) {
            s << "," << Qt::endl;
            QString modified_type = java_function->typeReplaced(argument->argumentIndex() + 1);
            if(java_function->argumentTypeArray(argument->argumentIndex() + 1)){
                QScopedPointer<AbstractMetaType> array(argument->type()->copy());
                array->setConstant(false);
                array->setReferenceType(AbstractMetaType::NoReference);
                array->setTypeUsagePattern(AbstractMetaType::ArrayPattern);
                QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                cpy->setConstant(false);
                cpy->setReferenceType(AbstractMetaType::NoReference);
                QList<bool> indirections = cpy->indirections();
                if(!indirections.isEmpty()){
                    indirections.removeLast();
                    cpy->setIndirections(indirections);
                }
                AbstractMetaBuilder::decideUsagePattern(cpy.get());
                array->setArrayElementType(cpy.get());
                s << " " << translateType(array.get(), options);
            }else if (modified_type.isEmpty())
                s << " " << translateType(argument->type(), options);
            else if(modified_type=="java.lang.String" || modified_type=="String")
                s << " jstring";
            else if(modified_type=="int")
                s << " jint";
            else if(modified_type=="long")
                s << " jlong";
            else if(modified_type=="short")
                s << " jshort";
            else if(modified_type=="byte")
                s << " jbyte";
            else if(modified_type=="double")
                s << " jdouble";
            else if(modified_type=="float")
                s << " jfloat";
            else if(modified_type=="boolean")
                s << " jboolean";
            else if(modified_type=="char")
                s << " jchar";
            else if(modified_type=="int[]")
                s << " jintArray";
            else if(modified_type=="long[]")
                s << " jlongArray";
            else if(modified_type=="short[]")
                s << " jshortArray";
            else if(modified_type=="byte[]")
                s << " jbyteArray";
            else if(modified_type=="double[]")
                s << " jdoubleArray";
            else if(modified_type=="float[]")
                s << " jfloatArray";
            else if(modified_type=="boolean[]")
                s << " jbooleanArray";
            else if(modified_type=="char[]")
                s << " jcharArray";
            else if(modified_type.endsWith("[]"))
                s << " jobjectArray";
            else
                s << " jobject";
            s << " " << argument->indexedName();
        }
    }
    QList<const ArgumentModification*> addedArguments = java_function->addedArguments();
    for(const ArgumentModification* argumentMod : addedArguments){
        s << ",";
        if(argumentMod->modified_type=="java.lang.String" || argumentMod->modified_type=="String")
            s << " jstring";
        else if(argumentMod->modified_type=="int")
            s << " jint";
        else if(argumentMod->modified_type=="long")
            s << " jlong";
        else if(argumentMod->modified_type=="short")
            s << " jshort";
        else if(argumentMod->modified_type=="byte")
            s << " jbyte";
        else if(argumentMod->modified_type=="double")
            s << " jdouble";
        else if(argumentMod->modified_type=="float")
            s << " jfloat";
        else if(argumentMod->modified_type=="boolean")
            s << " jboolean";
        else if(argumentMod->modified_type=="char")
            s << " jchar";
        else if(argumentMod->modified_type=="int[]")
            s << " jintArray";
        else if(argumentMod->modified_type=="long[]")
            s << " jlongArray";
        else if(argumentMod->modified_type=="short[]")
            s << " jshortArray";
        else if(argumentMod->modified_type=="byte[]")
            s << " jbyteArray";
        else if(argumentMod->modified_type=="double[]")
            s << " jdoubleArray";
        else if(argumentMod->modified_type=="float[]")
            s << " jfloatArray";
        else if(argumentMod->modified_type=="boolean[]")
            s << " jbooleanArray";
        else if(argumentMod->modified_type=="char[]")
            s << " jcharArray";
        else if(argumentMod->modified_type.endsWith("[]"))
            s << " jobjectArray";
        else
            s << " jobject";
        s << " " << argumentMod->modified_name;
    }
    s << ")" << Qt::endl;
}


/*!
    Generates type conversion from Java -> Qt for all the arguments
    that are to be to be passed to the function
*/
void CppImplGenerator::writeFinalFunctionSetup(QTextStream &s, const AbstractMetaFunction *java_function,
        const QString &qt_object_name,
        const AbstractMetaClass *cls) {
    // Extract the qt equivalent to the this pointer and name it "qt_object_name"
    if (!java_function->isStatic() && !java_function->isConstructor()) {
        if(cls->typeEntry()->isInterface() || cls->typeEntry()->designatedInterface() || cls->typeEntry()->isFunctional()){
            s << INDENT;
            if(java_function->isConstant())
                s << "const ";
            s << cls->qualifiedCppName() << " *" << qt_object_name << " = qtjambi_interface_from_nativeId<" << cls->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
            s << INDENT << "qtjambi_check_resource(__jni_env, " << qt_object_name << ");" << Qt::endl;
        }else{
            s << INDENT;
            if(java_function->isConstant())
                s << "const ";
            s << cls->qualifiedCppName() << " *" << qt_object_name << " = qtjambi_object_from_nativeId<" << cls->qualifiedCppName() << ">(__this_nativeId);" << Qt::endl;
            s << INDENT << "Q_ASSERT(" << qt_object_name << ");" << Qt::endl;
        }
    }
    // Translate each of the function arguments into qt types
    const AbstractMetaArgumentList& arguments = java_function->arguments();
    for(const AbstractMetaArgument *argument : arguments) {
        switch(java_function->argumentRemoved(argument->argumentIndex()+1)){
        case ArgumentRemove_UseAsLength:{
            s << INDENT << "jsize " << argument->indexedName() << " = 0;" << Qt::endl;
        }
            break;
        default: break;
        }
    }

    for(const AbstractMetaArgument *argument : arguments) {
        switch(java_function->argumentRemoved(argument->argumentIndex()+1)){
        case ArgumentRemove_No:{
            if (!argument->type()->isPrimitive()
                    || java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
                Option option = Option(UseNativeIds | EnumAsInts);
                if(java_function->isConstructor() && (java_function->declaringClass()->typeEntry()->isInterface() || java_function->declaringClass()->typeEntry()->designatedInterface())){
                    option = Option(EnumAsInts);
                }
                writeJavaToQt(s,
                              argument->type(),
                              "__qt_" + argument->indexedName(),
                              argument->indexedName(),
                              java_function,
                              nullptr,
                              argument->argumentIndex() + 1,
                              option, "__jni_env",
                              "__qtjambi_scope");
                writeOwnership(s, java_function, argument->indexedName(), "__qt_" + argument->indexedName(), argument->argumentIndex()+1, cls, TypeSystem::ShellCode, "__jni_env", "__qtjambi_scope");
            }
        }
            break;
            case ArgumentRemove_Remove:{
            if(java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)){
                Option option = Option(UseNativeIds | EnumAsInts);
                if(java_function->isConstructor() && (java_function->declaringClass()->typeEntry()->isInterface() || java_function->declaringClass()->typeEntry()->designatedInterface())){
                    option = Option(EnumAsInts);
                }
                writeJavaToQt(s,
                              argument->type(),
                              "__qt_" + argument->indexedName(),
                              argument->indexedName(),
                              java_function,
                              nullptr,
                              argument->argumentIndex() + 1,
                              option, "__jni_env",
                              "__qtjambi_scope");
            }
        }
            break;
        default: break;
        }
    }
}

void CppImplGenerator::writeConstructor(QTextStream &s, const AbstractMetaFunction *java_function,
                                        const QList<const AbstractMetaArgument *>& arguments,
                                        const AbstractMetaClass *java_class, int constructorIndex) {
    Q_ASSERT(java_class);

    const AbstractMetaClass *cls = java_class ? java_class : java_function->ownerClass();

    QString java_function_signature = java_function->signature();
    s << "// new " << java_function_signature << Qt::endl
      << "void __qt_construct_" << cls->qualifiedCppName().replace("::", "_") << "_";
    if(constructorIndex>=0)
        s << constructorIndex;
    else
        s << "declarative";
    s << "(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)" << Qt::endl
      << "{" << Qt::endl;
    INDENTATION(INDENT)
    s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"new " << java_function_signature << "\")" << Qt::endl;

    QStringList pps = getFunctionPPConditions(java_function);
    if(!cls->typeEntry()->ppCondition().isEmpty() && !pps.contains(cls->typeEntry()->ppCondition())){
        if(pps.isEmpty() || !cls->typeEntry()->ppCondition().contains("|"))
            pps << cls->typeEntry()->ppCondition();
        else
            pps << "(" + cls->typeEntry()->ppCondition() + ")";
    }
    if(!pps.isEmpty()){
        s << "#if " << pps.join(" && ") << Qt::endl;
    }

    QString lines;
    {
        QTextStream s(&lines);

        Option options = Option(NoOption);
        int counter = 0;
        for(const AbstractMetaArgument *argument : arguments) {
            if(argument){
                switch (java_function->argumentRemoved(argument->argumentIndex()+1)) {
                case ArgumentRemove_UseAsLength:
                    {
                        s << INDENT << "size_t " << argument->indexedName() << " = 0;" << Qt::endl;
                    }
                    break;
                case ArgumentRemove_No:
                    {
                    QString jniType = translateType(argument->type(), options);
                    s << INDENT << jniType << " " << argument->indexedName() << " = ";
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
                    if (!argument->type()->isPrimitive()
                            || java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
                        writeJavaToQt(s,
                                      argument->type(),
                                      "__qt_" + argument->indexedName(),
                                      argument->indexedName(),
                                      java_function,
                                      nullptr,
                                      argument->argumentIndex() + 1,
                                      options, "__jni_env",
                                      "__qtjambi_scope");
                        //s << INDENT << "Q_UNUSED(__qt_" << argument->indexedName() << ")" << Qt::endl;
                    }else{
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                    }
                    }
                    break;
                default: break;
                }
            }
        }
        QList<const ArgumentModification*> addedArguments = java_function->addedArguments();
        for(int i=0; i<addedArguments.size(); ++i){
            QString jniType = jniName(addedArguments.at(i)->modified_type);
            s << INDENT << jniType << " " << addedArguments.at(i)->modified_name << " = ";
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
        if(counter==0)
            s << INDENT << "Q_UNUSED(__java_arguments)" << Qt::endl;
        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Beginning, TypeSystem::NativeCode, "__jni_env", "__qtjambi_scope");

        s << INDENT << cls->qualifiedCppName() << " *__qt_this = ";
        s << "new(__qtjambi_ptr) ";
        if(constructorIndex>=0){
            s << shellClassName(cls);
        }else{
            if(java_class->generateShellClass()){
                s << "QtJambiQmlShellElement<" << shellClassName(cls) << ">";
            }else{
                s << "QtJambiQmlElement<" << shellClassName(cls) << ">";
            }
        }
        s << "(";
        writeFunctionCallArguments(s, java_function, arguments, "__qt_", Option(SkipRemovedArguments));
        s << ");" << Qt::endl;
        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TypeSystem::NativeCode, "__jni_env", "__qtjambi_scope");
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
          << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl
          << "#endif // " << pps.join(" && ") << Qt::endl;
    }
    s << "}" << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeFinalConstructor(QTextStream &s, const AbstractMetaFunction *java_function,
        const AbstractMetaClass *java_class, const QHash<const AbstractMetaFunction *,uint>& constructorIndexes) {
    Q_ASSERT(java_class);

    if (java_function->hasRReferences()
            || !java_function->isConstructor()
            || java_class->hasUnimplmentablePureVirtualFunction()
            || java_function->isModifiedRemoved(TypeSystem::NativeCode)
            || (!java_class->generateShellClass() && !(java_function->originalAttributes() & AbstractMetaAttributes::Public)))
        return;

    const AbstractMetaClass *cls = java_class ? java_class : java_function->ownerClass();

    QString java_function_signature = cls->qualifiedCppName() + "::" + java_function->signature();

    s << "// " << java_function_signature << Qt::endl;

    QString qt_object_name = "__qt_this";
    const QString java_object_name = "__jni_object";
    // function signature...
    uint options = m_native_jump_table ? ReturnType | ExternC : StandardJNISignature;
    writeFunctionName(s, java_function, cls, options);
    s << Qt::endl;
    writeFinalFunctionArguments(s, java_function, java_object_name);
    s << "{" << Qt::endl;
    INDENTATION(INDENT)
    s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << java_function_signature << "\")" << Qt::endl;

    s << INDENT << "try{" << Qt::endl;
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
            s << "#if " << pps.join(" && ") << Qt::endl;
        }
        QList<const ArgumentModification*> addedArguments = java_function->addedArguments();
        QList<const AbstractMetaArgument *> arguments;
        for(const AbstractMetaArgument *argument : java_function->arguments()) {
            if(java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No)
                arguments << argument;
        }
        QString argumentRef;
        if(arguments.size() + addedArguments.size() == 0){
            argumentRef = "nullptr";
        }else if(arguments.size() + addedArguments.size() == 1){
            argumentRef = "&arguments";
            s << "        jvalue arguments;" << Qt::endl;
            QString jniType;
            if(!arguments.isEmpty()){
                jniType = translateType(arguments.at(0)->type(), NoOption);
            }else{
                jniType = jniName(addedArguments.at(0)->modified_type);
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
            if(!arguments.isEmpty()){
                s << " = " << arguments.at(0)->indexedName() << ";" << Qt::endl;
            }else{
                s << " = " << addedArguments.at(0)->modified_name << ";" << Qt::endl;
            }
        }else{
            argumentRef = "arguments";
            s << "        jvalue arguments[" << (arguments.size() + addedArguments.size()) << "];" << Qt::endl;
            for(int i=0; i<arguments.size(); ++i){
                QString jniType = translateType(arguments.at(i)->type(), NoOption);
                s << "        arguments[" << i << "].";
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
                s << " = " << arguments.at(i)->indexedName() << ";" << Qt::endl;
            }
            for(int i=0; i<addedArguments.size(); ++i){
                QString jniType = jniName(addedArguments.at(i)->modified_type);
                s << "        arguments[" << i << "].";
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
                s << " = " << addedArguments.at(i)->modified_name << ";" << Qt::endl;
            }
        }
        if(java_class->isQObject()){
            s << "        qtjambi_initialize_native_qobject(__jni_env, __jni_class, __jni_object, &__qt_construct_" << java_class->qualifiedCppName().replace("::", "_") << "_" << constructorIndexes.value(java_function, 1000000000) << ", sizeof(" << shellClassName(java_class) << "), typeid(" << java_class->qualifiedCppName() << "), " << java_class->qualifiedCppName() << "::staticMetaObject, " << (java_class->generateShellClass() ? "true" : "false") << ", false, " << argumentRef << ");" << Qt::endl;
        }else{
            s << "        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_construct_" << java_class->qualifiedCppName().replace("::", "_") << "_" << constructorIndexes.value(java_function, 1000000000) << ", sizeof(" << shellClassName(java_class) << "), typeid(" << java_class->qualifiedCppName() << "), " << (java_class->generateShellClass() ? "true" : "false") << ", false, ";
            if (hasDeleter(cls)) {
                s << "&deleter_" << cls->qualifiedCppName().replace("::", "_");
            }else{
                s << "nullptr";
            }
            s << ", ";
            if (cls->typeEntry()->typeFlags() & ComplexTypeEntry::DeleteInMainThread){
                s << "&qtjambi_main_thread_owner";
            }else if (cls->typeEntry()->typeFlags() & ComplexTypeEntry::DeleteInOwnerThread){
                s << "&owner_function_"<< cls->qualifiedCppName().replace("::", "_");
            }else{
                s << "nullptr";
            }
            s << ", " << argumentRef << ");" << Qt::endl;
        }
        if(!pps.isEmpty()){
            s << "#else" << Qt::endl;
            s << INDENT << "Q_UNUSED(" << java_object_name << ")" << Qt::endl;
            s << INDENT << "Q_UNUSED(__jni_class)" << Qt::endl;
            for(const AbstractMetaArgument *argument : arguments) {
                s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
            }
            s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
            s << "#endif // " << pps.join(" && ") << Qt::endl;
        }
    }
    s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
    s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
    s << INDENT << "}" << Qt::endl;
    s << Qt::endl << "}";
    s << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeFinalFunction(QTextStream &s, const AbstractMetaFunction *java_function,
        const AbstractMetaClass *java_class) {
    Q_ASSERT(java_class);

    if (java_function->hasRReferences())
        return ;
    if (java_function->wasPrivate())
        return ;
    if (java_class->hasUnimplmentablePureVirtualFunction() && java_function->isConstructor())
        return;

    if (java_class->hasUnimplmentablePureVirtualFunction() && java_function->isAbstract())
        return;

    if (java_function->isModifiedRemoved(TypeSystem::NativeCode))
        return;

    if (java_function->isSignal() && java_function->isPrivate())
        return;

    if (java_function->isPrivateSignal())
        return;

    if (!java_class->generateShellClass() && (!(java_function->originalAttributes() & AbstractMetaAttributes::Public) || java_function->isAbstract()))
        return;

    const AbstractMetaClass *cls = java_class ? java_class : java_function->ownerClass();

    QString java_function_signature = cls->qualifiedCppName() + "::" + java_function->signature();

    s << "// " << java_function_signature << Qt::endl;

    const AbstractMetaType *function_type = java_function->type();

    QString qt_object_name = java_function->isStatic() ? cls->qualifiedCppName() : "__qt_this";
    const QString java_object_name = java_function->isStatic() ? "__jni_class" : "__jni_object";
    // function signature...
    bool callThrough = java_function->needsCallThrough();
    uint options = m_native_jump_table ? ReturnType | ExternC : StandardJNISignature;
    writeFunctionName(s, java_function, cls, options);
    s << Qt::endl;
    writeFinalFunctionArguments(s, java_function, java_object_name);
    s << "{" << Qt::endl;
    INDENTATION(INDENT)
    s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"" << java_function_signature << "\")" << Qt::endl;

    s << INDENT << "try{" << Qt::endl;
    {
        INDENTATION(INDENT)
        // Avoid compiler warnings when the variables are unused
        bool hasNativeId = (callThrough && !java_function->isStatic() && !java_function->isConstructor());
        QStringList pps = getFunctionPPConditions(java_function);
        if(!cls->typeEntry()->ppCondition().isEmpty() && !pps.contains(cls->typeEntry()->ppCondition())){
            if(pps.isEmpty() || !cls->typeEntry()->ppCondition().contains("|"))
                pps << cls->typeEntry()->ppCondition();
            else
                pps << "(" + cls->typeEntry()->ppCondition() + ")";
        }
        if(!pps.isEmpty()){
            s << "#if " << pps.join(" && ") << Qt::endl;
        }

        QString lines;
        {
            QTextStream s(&lines);
            if (cls->isFinal() && (!java_function->isAbstract() || !java_function->isFinalInTargetLang()) && !java_function->wasPublic()) {
                QString debug = QString("protected function '%1' in final class '%2'")
                                .arg(java_function->signature()).arg(java_class->name());
                ReportHandler::warning(debug);
                // Avoid compiler warnings for unused parameters
                const AbstractMetaArgumentList& arguments = java_function->arguments();

                for(const AbstractMetaArgument *argument : arguments) {
                    ArgumentRemove ar = java_function->argumentRemoved(argument->argumentIndex()+1);
                    if(ar==ArgumentRemove_UseAsLength){
                        s << INDENT << "jsize " << argument->indexedName() << " = 0;" << Qt::endl;
                    }
                    if(ar!=ArgumentRemove_Remove)
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                }
                s << INDENT << default_return_statement_java(java_function->type()) << ";";

            } else {
                writeFinalFunctionSetup(s, java_function, qt_object_name, cls);

                writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::Beginning, TypeSystem::NativeCode, "__jni_env", "__qtjambi_scope");

                QString function_prefix = "";
                QStringList extra_param;
                Option option = NoOption;

                bool hasShell = cls->generateShellClass();
                if (java_function->isFinalOverload()) {
                    // otherwise no prefix
                } else if (java_function->isFinalInCpp() && !java_function->wasPublic() && hasShell) {
                    function_prefix = "__public_";
                    if(java_function->isStatic())
                        qt_object_name = shellClassName(cls);
                    else if(java_function->isConstant())
                        qt_object_name = QString("static_cast<const %1*>(%2)").arg(shellClassName(cls)).arg(qt_object_name);
                    else
                        qt_object_name = QString("static_cast<%1*>(%2)").arg(shellClassName(cls)).arg(qt_object_name);
                    option = Option(option | EnumAsInts);
                } else if (!java_function->isFinalInCpp() && !java_function->isStatic() && hasShell) {
                    function_prefix = "__override_";
                    if(java_function->isStatic())
                        qt_object_name = shellClassName(cls);
                    else if(java_function->isConstant())
                        qt_object_name = QString("static_cast<const %1*>(%2)").arg(shellClassName(cls)).arg(qt_object_name);
                    else
                        qt_object_name = QString("static_cast<%1*>(%2)").arg(shellClassName(cls)).arg(qt_object_name);
                    if(java_function->isAbstract())
                        extra_param.append("__jni_env");
                    extra_param.append("__this_nativeId");
                    option = Option(option | EnumAsInts);
                } else {
                    if(!java_function->declaringTemplate().isEmpty()){
                        if(java_function->isStatic())
                            qt_object_name = java_function->declaringTemplate();
                        else if(java_function->isConstant())
                            qt_object_name = QString("static_cast<const %1*>(%2)").arg(java_function->declaringTemplate()).arg(qt_object_name);
                        else
                            qt_object_name = QString("static_cast<%1*>(%2)").arg(java_function->declaringTemplate()).arg(qt_object_name);
                    }
                    option = OriginalName;
                }

                // Call the Qt function on the java object
                s << INDENT;

                if (function_type || !java_function->typeReplaced(0).isEmpty()) {
                    const QString qt_return_value = "__qt_return_value";
                    const QString java_return_value = "__java_return_value";
                    bool paren = false;
                    if (function_type) {
                        writeTypeInfo(s, function_type, Option(EnumAsInts));
                        s << qt_return_value << " = ";
                        if(function_type->isEnum()){
                            uint size = static_cast<const EnumTypeEntry*>(function_type->typeEntry())->size();
                            s << "qint" << size << "(";
                            paren = true;
                        }
                    }

                    writeFunctionCall(s, qt_object_name, java_function, function_prefix, option, extra_param);
                    if(paren)
                        s << ")";
                    s << ";" << Qt::endl;

                    bool hasReturn = java_function->argumentReplaced(0).isEmpty() && java_function->typeReplaced(0) != "void";
                    if(!hasCodeInjections(java_function, java_function->implementingClass(), {CodeSnip::End}, TypeSystem::NativeCode)
                            && !hasOwnership(java_function, 0, java_function->implementingClass(), TypeSystem::NativeCode)
                            && !java_function->hasConversionRule(TypeSystem::NativeCode, 0)){
                        if(hasReturn){
                            writeQtToJava(s, function_type, qt_return_value, "",
                                          java_function, nullptr, 0, Option(DirectReturn | NoTmpVariable | EnumAsInts),
                                          "__jni_env", hasNativeId ? "__qtjambi_scope" : "");
                            s << ";" << Qt::endl;
                        }else if(function_type){
                            s << "Q_UNUSED(" << qt_return_value << ")" << Qt::endl;
                        }
                    }else{
                        if(hasReturn){
                            writeQtToJava(s, function_type, qt_return_value, java_return_value,
                                          java_function, nullptr, 0, EnumAsInts,
                                          "__jni_env", hasNativeId ? "__qtjambi_scope" : "");
                        }else if(function_type){
                            s << INDENT << "Q_UNUSED(" << qt_return_value << ")" << Qt::endl;
                        }
                        writeOwnership(s, java_function, java_return_value, qt_return_value, 0, cls, TypeSystem::NativeCode, "__jni_env", "__qtjambi_scope");
                        writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TypeSystem::NativeCode, "__jni_env", "__qtjambi_scope");
                        if(hasReturn)
                            s << INDENT << "return " << java_return_value << ";" << Qt::endl;
                    }
                } else {
                    writeFunctionCall(s, qt_object_name, java_function, function_prefix, option, extra_param);
                    s << ";" << Qt::endl;

                    writeCodeInjections(s, java_function, java_function->implementingClass(), CodeSnip::End, TypeSystem::NativeCode, "__jni_env", "__qtjambi_scope");
                }
            }
        }
        if(lines.contains("__qtjambi_scope")){
            s << INDENT << "QtJambiScope __qtjambi_scope" << ( java_function->isStatic() ? "" : "(__this_nativeId)") << ";" << Qt::endl;
        }
        s << lines;

        if(!pps.isEmpty()){
            s << "#else" << Qt::endl;
            if (java_function->isConstructor())
                s << INDENT << "Q_UNUSED(" << java_object_name << ")" << Qt::endl;
            if (hasNativeId)
                s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
            for(const AbstractMetaArgument *argument : java_function->arguments()) {
                if(java_function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No)
                    s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
            }
            s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
            s << "#endif // " << pps.join(" && ") << Qt::endl;
        }
    }
    s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
    s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
    s << INDENT << "}" << Qt::endl;
    QString return_type = jniReturnName(java_function);
    if(return_type=="jobject"
            || return_type=="jstring"
            || return_type=="jthrowable"
            || return_type=="jclass"
            || return_type=="jarray"
            || return_type.endsWith("Array")) {
        s << INDENT << "return nullptr;" << Qt::endl;
    }else if(return_type=="jboolean") {
        s << INDENT << "return false;" << Qt::endl;
    }else if(return_type!="void") {
        s << INDENT << "return " << return_type << "(0);" << Qt::endl;
    }

    s << Qt::endl << "}";
    s << Qt::endl << Qt::endl;
}

void CppImplGenerator::writeAssignment(QTextStream &s, const QString &destName, const QString &srcName,
                                       const AbstractMetaType *java_type) {
    if (java_type->isArray()) {
        for (int i = 0; i < java_type->arrayElementCount(); ++i) {
            writeAssignment(s, destName + "[" + QString::number(i) + "]",
                            srcName + "[" + QString::number(i) + "]", java_type->arrayElementType());
        }
    } else {
        s << INDENT << destName << " = ";
        if(java_type->isEnum() || java_type->isFlags()){
            s << java_type->typeEntry()->qualifiedCppName() << "(" << srcName << ");" << Qt::endl;
        }else{
            s << srcName << ";" << Qt::endl;
        }
    }
}

void CppImplGenerator::writeFieldAccessors(QTextStream &s, const AbstractMetaField *java_field) {
    Q_ASSERT(java_field);
    Q_ASSERT(java_field->isPublic() || java_field->isProtected());

    const AbstractMetaFunction *setter = java_field->setter();
    const AbstractMetaFunction *getter = java_field->getter();

    const AbstractMetaClass *cls = java_field->enclosingClass();
    FieldModification mod = cls->typeEntry()->fieldModification(java_field->name());

    QStringList pps;
    if(!java_field->enclosingClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(java_field->enclosingClass()->typeEntry()->ppCondition())){
        pps << java_field->enclosingClass()->typeEntry()->ppCondition();
    }
    if(!java_field->type()->typeEntry()->ppCondition().isEmpty() && !pps.contains(java_field->type()->typeEntry()->ppCondition())){
        pps << java_field->type()->typeEntry()->ppCondition();
    }
    for(const AbstractMetaType* inst : java_field->type()->instantiations()){
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
    if (mod.isWritable() && !java_field->type()->isConstant()) {
        // Write public override for protected fields
        if (setter->wasProtected()) {
            writeFunctionSignature(s, setter, setter->ownerClass());
            s << Qt::endl
            << "{" << Qt::endl;
            {
                INDENTATION(INDENT)

                Q_ASSERT(setter->arguments().count() > 0);
                const AbstractMetaArgument *argument = setter->arguments().at(0);

                QString thisRef = java_field->isStatic()
                                  ? setter->ownerClass()->qualifiedCppName() + QString("::")
                                  : QString("this->");
                writeAssignment(s, thisRef + java_field->name(), argument->indexedName(), argument->type());
            }
            s << "}" << Qt::endl << Qt::endl;
        }

        writeFunctionName(s, setter, setter->ownerClass());
        s << Qt::endl;
        writeFinalFunctionArguments(s, setter, "__java_object");
        s  << "{" << Qt::endl;

        {
            INDENTATION(INDENT)
            s << INDENT << "try{" << Qt::endl;
            {
                INDENTATION(INDENT)
                if(!pps.isEmpty())
                    s << "#if " << pps.join(" && ") << Qt::endl;
                s << INDENT << "QtJambiScope __qtjambi_scope" << (setter->isStatic() ? "" : "(__this_nativeId)") << ";" << Qt::endl
                  << INDENT << "Q_UNUSED(__qtjambi_scope)" << Qt::endl;
                writeFinalFunctionSetup(s, setter, "__qt_this", setter->ownerClass());

                Q_ASSERT(setter->arguments().count() == 1);
                const AbstractMetaArgument *argument = setter->arguments().at(0);

                QString dest;
                if (setter->isStatic())
                    dest = shellClassName(setter->ownerClass()) + "::";
                else if(setter->wasPublic())
                    dest = "__qt_this->";
                else
                    dest = QString("static_cast<%1*>(__qt_this)->").arg(shellClassName(cls));

                QString src;
                if (!argument->type()->isPrimitive())
                    src = "__qt_" + argument->indexedName();
                else
                    src = argument->indexedName();

                if (setter->wasPublic())
                    writeAssignment(s, dest + java_field->name(), src, argument->type());
                else
                    s << INDENT << dest << setter->name() << "_setter(" << src << ");" << Qt::endl;
                if(!pps.isEmpty()){
                    s << "#else" << Qt::endl;
                    if(!setter->isStatic())
                        s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
                    for(const AbstractMetaArgument *argument : setter->arguments()) {
                        s << INDENT << "Q_UNUSED(" << argument->indexedName() << ")" << Qt::endl;
                    }
                    s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                    s << "#endif //" << pps.join(" && ") << Qt::endl;
                }
            }
            s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
            s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
        }
        s << "}" << Qt::endl << Qt::endl;
    }

    if (mod.isReadable()) {
        // Getter
        if (getter->wasProtected()) {
            if(!pps.isEmpty())
                s << Qt::endl << "#if " << pps.join(" && ") << Qt::endl;
            writeFunctionSignature(s, getter, getter->ownerClass());
            s << Qt::endl
            << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "return " << java_field->name() << ";" << Qt::endl;
            }
            s << "}" << Qt::endl << Qt::endl;
            if(!pps.isEmpty()){
                s << "#endif //" << pps.join(" && ") << Qt::endl << Qt::endl;
            }
        }


        writeFunctionName(s, getter, getter->ownerClass());
        s << Qt::endl;
        writeFinalFunctionArguments(s, getter, "__java_object");
        s << "{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "try{" << Qt::endl;
            {
                INDENTATION(INDENT)
                if(!pps.isEmpty())
                    s << "#if " << pps.join(" && ") << Qt::endl;
                s << INDENT << "QtJambiScope __qtjambi_scope" << (getter->isStatic() ? "" : "(__this_nativeId)") << ";" << Qt::endl
                  << INDENT << "Q_UNUSED(__qtjambi_scope)" << Qt::endl;
                writeFinalFunctionSetup(s, getter, "__qt_this", getter->ownerClass());

                //const QString java_return_value = "__java_return_value";
                QString qt_return_value;
                if (setter->isStatic())
                    qt_return_value = shellClassName(setter->ownerClass()) + "::";
                else if(setter->wasPublic())
                    qt_return_value = "__qt_this->";
                else
                    qt_return_value = QString("static_cast<%1*>(__qt_this)->").arg(shellClassName(cls));

                if (getter->wasPublic())
                    qt_return_value += java_field->name();
                else
                    qt_return_value += getter->name() + "_getter()";
                //s << qt_return_value << ";" << Qt::endl;
                s << INDENT;
                writeTypeInfo(s, getter->type(), Option(/*ForceConstReference | */ArrayAsPointer));
                s << " __qt_value = " << qt_return_value << ";" << Qt::endl;
                writeQtToJava(s, getter->type(), "__qt_value", "", getter, nullptr, 0, Option(DirectReturn | NoTmpVariable | EnumAsInts), "__jni_env", "__qtjambi_scope");
                s << ";" << Qt::endl;
                if(!pps.isEmpty()){
                    s << "#else" << Qt::endl;
                    if(!getter->isStatic())
                        s << INDENT << "Q_UNUSED(__this_nativeId)" << Qt::endl;
                    s << INDENT << "JavaException::raiseQNoImplementationException(__jni_env, \"The method has no implementation on this platform.\" QTJAMBI_STACKTRACEINFO );" << Qt::endl;
                    s << "#endif //" << pps.join(" && ") << Qt::endl;
                }
            }
            s << INDENT << "}catch(const JavaException& exn){" << Qt::endl;
            s << INDENT << "    exn.raiseInJava(__jni_env);" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            QString return_type = jniReturnName(getter);
            if(return_type=="jobject"
                    || return_type=="jstring"
                    || return_type=="jthrowable"
                    || return_type=="jclass"
                    || return_type=="jarray"
                    || return_type.endsWith("Array")) {
                s << INDENT << "return nullptr;" << Qt::endl;
            }else if(return_type=="jboolean") {
                s << INDENT << "return false;" << Qt::endl;
            }else if(return_type!="void") {
                s << INDENT << "return " << return_type << "(0);" << Qt::endl;
            }
        }
        s << "}" << Qt::endl << Qt::endl;
    }
}

void CppImplGenerator::writeDeleteAndOwnerFunction(QTextStream &s, const AbstractMetaClass *cls) {
    if(hasDeleter(cls)){
        const AbstractMetaClass *clsWithPublicDestructor = lookupClassWithPublicDestructor(cls);
        {
            if (clsWithPublicDestructor!= nullptr) {
                s << Qt::endl << "void deleter_" << cls->qualifiedCppName().replace("::", "_") << "(void *ptr)" << Qt::endl
                  << "{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "QTJAMBI_DEBUG_METHOD_PRINT(\"native\", \"qtjambi_deleter for " << cls->qualifiedCppName() << "\")" << Qt::endl;
                    s << INDENT << clsWithPublicDestructor->qualifiedCppName() << " *_ptr = reinterpret_cast<" << clsWithPublicDestructor->qualifiedCppName() << " *>(ptr);" << Qt::endl;
                    QStringList lines;
                    QSet<const ComplexTypeEntry *> classes;
                    const AbstractMetaClass *_cls = cls;
                    while(_cls){
                        classes << (_cls->typeEntry()->designatedInterface() ? _cls->typeEntry()->designatedInterface() : _cls->typeEntry());
                        for(const AbstractMetaClass *ifc : _cls->interfaces()){
                            classes << (ifc->typeEntry()->designatedInterface() ? ifc->typeEntry()->designatedInterface() : ifc->typeEntry());
                        }
                        _cls = _cls->baseClass();
                    }
                    for(const ComplexTypeEntry * _cls : classes){
                        for(const CodeSnip& snip : _cls->codeSnips()) {
                            if (snip.language == TypeSystem::DestructorFunction) {
                                lines << snip.code().replace("%this", "_ptr").split("\n");
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

                    if(cls->generateShellClass()){
                        if((cls==clsWithPublicDestructor && !clsWithPublicDestructor->hasVirtualDestructor()) || !cls->isQObject()){
                            if(cls==clsWithPublicDestructor && !clsWithPublicDestructor->hasVirtualDestructor())
                                s << INDENT << "bool isShell = false;" << Qt::endl;
                            s << INDENT << "qtjambi_register_pointer_deletion(ptr";
                            if(!lines.isEmpty()){
                                s << ", [](void* ptr){" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    {
                                        INDENTATION(INDENT)
                                        s << INDENT << clsWithPublicDestructor->qualifiedCppName() << " *_ptr = reinterpret_cast<" << clsWithPublicDestructor->qualifiedCppName() << " *>(ptr);" << Qt::endl;
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
                                            s << INDENT << line << Qt::endl;
                                        }
                                    }
                                }
                                s << INDENT << "}";
                            }
                            if(cls==clsWithPublicDestructor && !clsWithPublicDestructor->hasVirtualDestructor())
                                s << ", &isShell";
                            s << ");" << Qt::endl;
                        }
                        if(cls==clsWithPublicDestructor && !clsWithPublicDestructor->hasVirtualDestructor()){
                            s << INDENT << "if(isShell)" << Qt::endl;
                            s << INDENT << "    delete static_cast<" << shellClassName(clsWithPublicDestructor) << "*>(_ptr);" << Qt::endl;
                            s << INDENT << "else delete _ptr;" << Qt::endl;
                        }else{
                            s << INDENT << "delete _ptr;" << Qt::endl;
                        }
                    }else{
                        s << INDENT << "delete _ptr;" << Qt::endl;
                    }
                }
            }
            s   << "}" << Qt::endl;
        }
    }
    if (cls->typeEntry()->typeFlags() & ComplexTypeEntry::DeleteInOwnerThread){
        const ComplexTypeEntry *entry = cls->typeEntry();
        if (!entry->owner().isEmpty()) {
            s << Qt::endl << "const QObject* owner_function_" << cls->qualifiedCppName().replace("::", "_") << "(void *ptr)" << Qt::endl
              << "{" << Qt::endl
              << "    " << cls->qualifiedCppName() << "* __qt_this = reinterpret_cast<" << cls->qualifiedCppName() << " *>(ptr);" << Qt::endl;
            QString ownerCode = entry->owner();
            if(ownerCode.contains("%1")){
                ownerCode = ownerCode.arg("__qt_this");
            }else if(ownerCode.contains("this->")){
                ownerCode = ownerCode.replace("this->", "__qt_this->");
            }else{
                ownerCode = "__qt_this->" + ownerCode;
            }
            s << "    return ";
            bool first = true;
            for(const QString& line : ownerCode.split("\n")){
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
    QString indent;
    for(const QString& str : lines) {
        s << "    " << indent << str.trimmed() << Qt::endl;
        if (!str.trimmed().endsWith(";") && !str.trimmed().isEmpty())
            indent = "    ";
        else
            indent = "";
    }
}

void CppImplGenerator::writeCustomStructors(QTextStream &s, const TypeEntry *entry) {
    if (!entry->preferredConversion())
        return ;

    CustomFunction customConstructor = entry->customConstructor();
    CustomFunction customDestructor = entry->customDestructor();

    if(!customConstructor.name.isEmpty()){
        s << "void* custom_constructor_" << entry->qualifiedCppName().replace("::", "_") << "("
        << "void* " << customConstructor.placement_name << ", const void * __in_var"
        << ")" << Qt::endl
        << "{" << Qt::endl
        << "    Q_UNUSED(" << customConstructor.placement_name << ")" << Qt::endl
        << "    const " << entry->qualifiedCppName() << "* " << customConstructor.param_name << " = static_cast<const " << entry->qualifiedCppName() << "*>(__in_var);" << Qt::endl
        << "    Q_UNUSED(" << customConstructor.param_name << ")" << Qt::endl;
        writeCodeBlock(s, customConstructor.code());
        s << "}" << Qt::endl << Qt::endl;
    }

    if (!customDestructor.name.isEmpty()) {
        s << "void custom_destructor_" << entry->qualifiedCppName().replace("::", "_") << "("
        << "void * __in_var"
        << ")" << Qt::endl
        << "{" << Qt::endl
        << "    " << entry->qualifiedCppName() << "* " << customDestructor.param_name << " = static_cast<" << entry->qualifiedCppName() << "*>(__in_var);" << Qt::endl
        << "    Q_UNUSED(" << customDestructor.param_name << ")" << Qt::endl;
        writeCodeBlock(s, customDestructor.code());
        s << "}" << Qt::endl << Qt::endl;
    }
}

QString CppImplGenerator::fromObject(const AbstractMetaType *java_type,
                                     const QString &var_name, bool invalidateAfterUse,
                                     const QString& __jni_env) {
    bool makeCopyOfValueTypes = java_type->isValue() || java_type->indirections().isEmpty();
    const TypeEntry *entry = java_type->typeEntry();
    QString returned;
    QString package = entry->javaPackage();
    const ComplexTypeEntry *centry = entry->isComplex()
                                     ? static_cast<const ComplexTypeEntry *>(entry)
                                     : nullptr;

    if (!centry || centry->polymorphicIdValue().isEmpty()) {
        if(centry && (centry->isInterface() || centry->designatedInterface())){
            returned = "qtjambi_cast<jobject>(" + __jni_env + ", " + var_name + ")";
        }else{
            if(entry->qualifiedCppName()=="__GLsync"){
                returned = "qtjambi_from_object(" + __jni_env + ", static_cast<const void *>(" + (java_type->getReferenceType()==AbstractMetaType::Reference ? "&" : "") + var_name + "), \"" + QString(package).replace(".", "/") + "/"
                        + entry->lookupName() + "\", " + ( makeCopyOfValueTypes ? "true" : "false"  ) + ", " + ( invalidateAfterUse ? "true" : "false" ) + ")";
            }else{
                returned = "qtjambi_cast<jobject>(" + __jni_env + ", " + var_name + ")";
            }
        }
    } else {
        AbstractMetaClass *cls = classes().findClass(centry->qualifiedCppName());
        if (!cls) {
            qWarning("CppImplGenerator::fromObject(): class '%s' could not be resolved...",
                   qPrintable(centry->qualifiedCppName()));
        }

        while (cls!= nullptr && !cls->typeEntry()->isPolymorphicBase())
            cls = cls->baseClass();

        if (!cls) {//, " + entry->qualifiedCppName() + "
            ReportHandler::warning(QString("class '%1' has polymorphic id but does not inherit a polymorphic class")
                                   .arg(centry->qualifiedCppName()));
        }
        returned = "qtjambi_cast<jobject>(" + __jni_env + ", " + var_name + ")";
    }

    return returned;
}

bool CppImplGenerator::writeConversionRule(QTextStream &s,
        TypeSystem::Language target_language,
        const AbstractMetaFunction *java_function,
        const AbstractMetaFunctional *java_functional,
        int argument_index,
        const QString &qt_name,
        const QString &java_name,
        Option option,
        const QString& __jni_env,
        const QString& qtjambi_scope) {
    Q_UNUSED(option)
    if (argument_index < 0)
        return false;

    if(java_function){
        if(argument_index>java_function->arguments().size())
            return false;
    }else if(java_functional){
        if(argument_index>java_functional->arguments().size())
            return false;
    }else{
        return false;
    }

    QString replacedType;
    const AbstractMetaType *java_type;
    const AbstractMetaArgumentList& arguments = java_function ? java_function->arguments() : java_functional->arguments();
    if(java_function){
        if(argument_index==0){
            java_type = java_function->type();
        }else{
            java_type = arguments.at(argument_index-1)->type();
        }
    }else{
        if(argument_index==0){
            java_type = java_functional->type();
        }else{
            java_type = arguments.at(argument_index-1)->type();
        }
    }

    const AbstractMetaArgument* argumentTypeArrayLength = nullptr;
    bool argumentTypeArray = false;
    bool argumentTypeBuffer = false;
    bool argumentTypeString = false;
    bool argumentTypeArrayDeref = false;
    int argumentTypeArrayMinLength = -1;
    int argumentTypeArrayMaxLength = -1;
    if(java_function){
        int argumentTypeArrayLengthIndex = java_function->argumentTypeArrayLengthIndex(argument_index);
        argumentTypeArrayDeref = java_function->argumentTypeArrayDeref(argument_index);
        argumentTypeArray = java_function->argumentTypeArray(argument_index);
        argumentTypeBuffer = java_function->argumentTypeBuffer(argument_index);
        argumentTypeArrayLength = java_function->arguments().value(argumentTypeArrayLengthIndex-1, nullptr);
        argumentTypeArrayMinLength = java_function->argumentTypeArrayLengthMinValue(argument_index);
        argumentTypeArrayMaxLength = java_function->argumentTypeArrayLengthMaxValue(argument_index);
        if(!argumentTypeArray && !argumentTypeBuffer){
            replacedType = java_function->typeReplaced(argument_index);
        }
    }else{
        int argumentTypeArrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(argument_index);
        argumentTypeArrayDeref = java_functional->argumentTypeArrayDeref(argument_index);
        argumentTypeArray = java_functional->argumentTypeArray(argument_index);
        argumentTypeBuffer = java_functional->argumentTypeBuffer(argument_index);
        argumentTypeArrayLength = java_functional->arguments().value(argumentTypeArrayLengthIndex-1, nullptr);
        argumentTypeArrayMinLength = java_functional->argumentTypeArrayLengthMinValue(argument_index);
        argumentTypeArrayMaxLength = java_functional->argumentTypeArrayLengthMaxValue(argument_index);
        if(!argumentTypeArray && !argumentTypeBuffer){
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

    if (!conversion_rule.isEmpty() || argumentTypeArray || argumentTypeBuffer || argumentTypeString) {
        QScopedPointer<Indentation> indentationPointer;
        bool needLambda = false;
        if(!argumentTypeBuffer && !argumentTypeString){
            if((option & DirectReturn) == DirectReturn){
                needLambda = true;
            }else if((option & NoTmpVariable) == NoTmpVariable){
                needLambda = true;
            }
        }

        QString _qt_name = qt_name;
        if((option & OptionalScope) == OptionalScope){
            _qt_name = qt_name + "_tmp";
        }

        QString out_name = _qt_name;
        QString in_name = java_name;
        QString tmp_out_name = _qt_name;
        QString tmp_in_name = java_name;
        QString lambdaOutType;
        if ((argument_index == 0 && target_language == TypeSystem::NativeCode)
                || (argument_index != 0 && target_language == TypeSystem::ShellCode)) {
            tmp_out_name = java_name;
            tmp_in_name = _qt_name;
            out_name = java_name;
            in_name = _qt_name;
            if(replacedType.isEmpty())
                lambdaOutType = " -> "+translateType(java_type, option);
            else
                lambdaOutType = " -> "+jniName(replacedType);
        } else {
            tmp_out_name = _qt_name;
            tmp_in_name = java_name;
            out_name = _qt_name;
            in_name = java_name;
            if(java_type){
                QTextStream _s(&lambdaOutType);
                writeTypeInfo(_s, java_type, option);
                lambdaOutType = " -> "+lambdaOutType;
            }
        }
        if(!argumentTypeBuffer && !argumentTypeString){
            if(out_name.isEmpty()){
               needLambda = true;
            }
        }

        if(needLambda && java_type){
            tmp_out_name = "_lambda_out";
            if((option & DirectReturn) == DirectReturn){
                s << INDENT << "return ";
            }
            if((option & NoTmpVariable) == NoTmpVariable && !java_name.isEmpty()){
                s << INDENT << out_name << " = ";
            }
            s << "[&]()" << lambdaOutType << " {" << Qt::endl;
            indentationPointer.reset(new Indentation(INDENT));
        }

        if(argumentTypeArray){
            if(!argumentTypeArrayLength && argumentTypeArrayMinLength<=0)
                ReportHandler::warning("ArrayLength expected");
            QScopedPointer<AbstractMetaType> _java_type(java_type->copy());
            _java_type->setConstant(false);
            _java_type->setReferenceType(AbstractMetaType::NoReference);
            //if(argumentTypeArrayDeref)
            {
                QList<bool> indirections = _java_type->indirections();
                if(!indirections.isEmpty()){
                    indirections.takeLast();
                    _java_type->setIndirections(indirections);
                }
            }
            AbstractMetaBuilder::decideUsagePattern(_java_type.get());
            if(target_language==TypeSystem::NativeCode){
                if(_java_type->typeEntry()->isPrimitive()){
                    if(_java_type->typeEntry()->targetLangName()=="int"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JIntArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="long"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JLongArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="short"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JShortArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="byte"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JByteArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="char"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JCharArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="float"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JFloatArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="double"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JDoubleArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="boolean"){
                        if(qtjambi_scope.isEmpty()){
                            s << INDENT << "JBooleanArrayPointer " << out_name << "(" << __jni_env << ", " << in_name;
                            if(java_type->isConstant()){
                                s << ", false";
                            }
                            s << ");" << Qt::endl;
                        }else{
                            s << INDENT;
                            writeTypeInfo(s, java_type, option);
                            s << out_name << " = qtjambi_array_cast<";
                            writeTypeInfo(s, java_type, Option(option | SkipName));
                            s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }
                }else{
                    if(qtjambi_scope.isEmpty()){
                        s << INDENT << "JObjectArrayPointer<";
                        writeTypeInfo(s, _java_type.get(), option);
                        s << "> " << out_name << "(" << __jni_env << ", " << in_name << "," << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << "[__qtjambi_scope](";
                                    writeTypeInfo(s, _java_type.get(), option);
                                    s << "& p,JNIEnv * env, jobject o){" << Qt::endl;
                                    {
                                        INDENTATION(INDENT)
                                        writeJavaToQt(s, _java_type.get(), "p", "o", nullptr, nullptr, 0, Option((option | NoTmpVariable) & ~UseNativeIds), "env", "__qtjambi_scope");
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
                                        s << INDENT << "[__qtjambi_scope](JNIEnv * env, ";
                                        writeTypeInfo(s, _java_type.get(), Option(option | ExcludeConst));
                                        s << " const& p) -> jobject {" << Qt::endl;
                                        {
                                            INDENTATION(INDENT)
                                            writeQtToJava(s, _java_type.get(), "p", "o", nullptr, nullptr, 0, Option(option | DirectReturn), "env", "__qtjambi_scope");
                                        }
                                        s << INDENT << "}";
                                    }
                                }
                            }
                        }
                        s << ");" << Qt::endl;
                    }else{
                        s << INDENT;
                        writeTypeInfo(s, java_type, option);
                        s << out_name << " = qtjambi_array_cast<";
                        writeTypeInfo(s, java_type, Option(option | SkipName));
                        s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                        if(argumentTypeArrayLength)
                            s << argumentTypeArrayLength->indexedName();
                        else
                            s << argumentTypeArrayMinLength;
                        s << ");" << Qt::endl;
                    }
                }
                if(qtjambi_scope.isEmpty() && argumentTypeArrayLength){
                        s << INDENT << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName() << " = " << out_name << ".size();" << Qt::endl;
                }
            }else if(target_language==TypeSystem::ShellCode){
                if(_java_type->typeEntry()->isPrimitive()){
                    if(_java_type->typeEntry()->targetLangName()=="int"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstInt32PointerArray";
                            }else{
                                s << INDENT << "Int32PointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jintArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jintArray " << out_name << " = qtjambi_array_cast<jintArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="long"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstInt64PointerArray";
                            }else{
                                s << INDENT << "Int64PointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jlongArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jlongArray " << out_name << " = qtjambi_array_cast<jlongArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="short"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstInt16PointerArray";
                            }else{
                                s << INDENT << "Int16PointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jshortArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jshortArray " << out_name << " = qtjambi_array_cast<jshortArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="byte"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstInt8PointerArray";
                            }else{
                                s << INDENT << "Int8PointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jbyteArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jbyteArray " << out_name << " = qtjambi_array_cast<jbyteArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="char"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstWCharPointerArray";
                            }else{
                                s << INDENT << "WCharPointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jcharArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jcharArray " << out_name << " = qtjambi_array_cast<jcharArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="float"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstFloatPointerArray";
                            }else{
                                s << INDENT << "FloatPointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jfloatArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jfloatArray " << out_name << " = qtjambi_array_cast<jfloatArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="double"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstDoublePointerArray";
                            }else{
                                s << INDENT << "DoublePointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jdoubleArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jdoubleArray " << out_name << " = qtjambi_array_cast<jdoubleArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }else if(_java_type->typeEntry()->targetLangName()=="boolean"){
                        if(qtjambi_scope.isEmpty()){
                            if(java_type->isConstant()){
                                s << INDENT << "ConstBoolPointerArray";
                            }else{
                                s << INDENT << "BoolPointerArray";
                            }
                            s << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                            if(argumentTypeArrayLength)
                                s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                            s << INDENT << "jbooleanArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                        }else{
                            s << INDENT << "jbooleanArray " << out_name << " = qtjambi_array_cast<jbooleanArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", " << (argumentTypeArrayDeref ? "*" : "");
                            if(argumentTypeArrayLength)
                                s << argumentTypeArrayLength->indexedName();
                            else
                                s << argumentTypeArrayMinLength;
                            s << ");" << Qt::endl;
                        }
                    }
                }else{
                    if(qtjambi_scope.isEmpty()){
                        if(java_type->isConstant()){
                            s << INDENT << "ConstObjectPointerArray<";
                        }else{
                            s << INDENT << "ObjectPointerArray<";
                        }
                        writeTypeInfo(s, _java_type.get(), Option(SkipName | option));
                        s << ">" << out_name << "_array(" << __jni_env << ", " << in_name << ", ";
                        if(argumentTypeArrayLength)
                            s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                        else
                            s << argumentTypeArrayMinLength;
                        s << ", " << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << "[__qtjambi_scope](JNIEnv * env, ";
                                    writeTypeInfo(s, _java_type.get(), Option(ForceConstReference | option));
                                    s << "in) -> jobject {" << Qt::endl;
                                    {
                                        INDENTATION(INDENT)
                                        writeQtToJava(s, _java_type.get(), "in", "out", nullptr, nullptr, 0, Option(option | DirectReturn), "env", "__qtjambi_scope");
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
                                        s << INDENT << "[__qtjambi_scope](";
                                        writeTypeInfo(s, _java_type.get(), option);
                                        s << "& p,JNIEnv * env, jobject o){" << Qt::endl;
                                        {
                                            INDENTATION(INDENT)
                                            writeJavaToQt(s, _java_type.get(), "p", "o", nullptr, nullptr, 0, Option((option | NoTmpVariable) & ~UseNativeIds), "env", "__qtjambi_scope");
                                        }
                                        s << INDENT << "}";
                                    }
                                }
                            }
                        }
                        s << ");" << Qt::endl;
                        s << INDENT << "jobjectArray " << out_name << " = " << out_name << "_array;" << Qt::endl;
                    }else{
                        s << INDENT << "jobjectArray " << out_name << " = qtjambi_array_cast<jobjectArray>(" << __jni_env << ", " << qtjambi_scope << ", " << in_name << ", ";
                        if(argumentTypeArrayLength)
                            s << (argumentTypeArrayDeref ? "*" : "") << argumentTypeArrayLength->indexedName();
                        else
                            s << argumentTypeArrayMinLength;
                        s << ");" << Qt::endl;
                    }
                }
            }
        }else if(argumentTypeString){
            if(target_language==TypeSystem::NativeCode){
                if(argument_index==0){
                    if((option & DirectReturn) == DirectReturn){
                        s << INDENT << "return ";
                    }else if((option & NoTmpVariable) == NoTmpVariable){
                        s << INDENT << out_name << " = ";
                    }else{
                        s << INDENT << "jstring " << out_name << " = ";
                    }
                    s << "qtjambi_from_qstring("  << __jni_env << ", QLatin1String(reinterpret_cast<const char*>(" << in_name << ")))";
                }else{
                    s << INDENT << "J2CStringBuffer " << out_name << "_buffer(" << __jni_env << ", jstring(" << in_name << "));" << Qt::endl
                      << INDENT;
                    if(java_type && java_type->isConstant())
                        s << "const ";
                    s << "char* " << out_name << " = " << out_name << "_buffer;" << Qt::endl;
                }
            }else if(target_language==TypeSystem::ShellCode){
                if(argument_index==0){
                    s << INDENT << "J2CStringBuffer " << out_name << "_buffer(" << __jni_env << ", jstring(" << in_name << "));" << Qt::endl
                      << INDENT;
                    if(java_type && java_type->isConstant())
                        s << "const ";
                    s << "char* " << out_name << " = " << out_name << "_buffer;" << Qt::endl;
                }else{
                    s << INDENT << "jstring " << out_name << " = qtjambi_from_qstring("  << __jni_env << ", QLatin1String(reinterpret_cast<const char*>(" << in_name << ")));" << Qt::endl;
                }
            }
        }else if(argumentTypeBuffer){
            //Q_ASSERT(java_type->typeEntry()->isPrimitive());
            //Q_ASSERT(java_type->indirections().size()==1);
            //Q_ASSERT(java_type->typeEntry()->targetLangName()!="boolean");
            if(target_language==TypeSystem::NativeCode){
                if(argument_index==0){
                    if((option & DirectReturn) == DirectReturn){
                        s << INDENT << "return ";
                    }else if((option & NoTmpVariable) == NoTmpVariable){
                        s << INDENT << out_name << " = ";
                    }else{
                        s << INDENT << "jobject " << out_name << " = ";
                    }
                    if(argumentTypeArrayLength){
                        s << __jni_env << "->NewDirectByteBuffer(" << in_name << ", " << argumentTypeArrayLength->indexedName() << ")";
                    }else if(argumentTypeArrayMinLength>0){
                        s << __jni_env << "->NewDirectByteBuffer(" << in_name << ", " << argumentTypeArrayMinLength << ")";
                    }else{
                        s << __jni_env << "->NewDirectByteBuffer(" << in_name << ", 1024)";
                    }
                }else{
                    s << INDENT << "JBuffer";
                    if(java_type->isConstant())
                        s << "Const";
                    s << "Data " << out_name << "_buffer(" << __jni_env << ", " << in_name << ");" << Qt::endl;
                    s << INDENT;
                    writeTypeInfo(s, java_type, option);
                    s << out_name << " = " << out_name << "_buffer;" << Qt::endl;
                }
            }else if(target_language==TypeSystem::ShellCode){
                s << INDENT << "// TODO convert pointer to Buffer" << Qt::endl;
            }
        }else{
            conversion_rule  = conversion_rule.replace("%out", tmp_out_name)
                               .replace("%in", tmp_in_name)
                                .replace("%env", __jni_env)
                                .replace("%scope", (option & OptionalScope) == OptionalScope ? "*"+qtjambi_scope : qtjambi_scope);

            for (int i = 0; i < arguments.size(); ++i) {
                conversion_rule = conversion_rule.replace("%" + QString::number(i + 1),
                                  arguments.at(i)->indexedName());
            }
            QStringList lines = conversion_rule.split("\n");
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
                s << INDENT << line << Qt::endl;
            }
            if((option & OptionalScope) == OptionalScope){
                if(java_type->indirections().isEmpty()
                        && (java_type->getReferenceType()==AbstractMetaType::Reference
                            || ((java_type->typeEntry()->isObject() || java_type->typeEntry()->isInterface()) && !this->hasPublicAssignmentOperator(java_type))
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
                    s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                    s << INDENT << "}" << Qt::endl;
                    s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
                    s << INDENT << "*reinterpret_cast<";
                    writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
                    s << "*>(" << qt_name << ") = " << _qt_name << ";" << Qt::endl;
                }
            }
        }

        if(needLambda && java_type){
            s << INDENT << "return " << tmp_out_name << ";" << Qt::endl;
            indentationPointer.reset();
            s << INDENT << "}()";
            if((option & NoTmpVariable) == NoTmpVariable && !java_name.isEmpty()){
                s << ";" << Qt::endl;
            }
        }

        return true;
    } else {
        return false;
    }
}

void CppImplGenerator::writeJavaToQt(QTextStream &s,
                                     const AbstractMetaType *java_type,
                                     const QString &qt_name,
                                     const QString &java_name,
                                     const AbstractMetaFunction *java_function,
                                     const AbstractMetaFunctional *java_functional,
                                     int argument_index,
                                     Option option,
                                     const QString& __jni_env,
                                     const QString& qtjambi_scope) {
    // Conversion to C++: Shell code for return values, native code for arguments
    TypeSystem::Language lang = argument_index == 0 ? TypeSystem::ShellCode : TypeSystem::NativeCode;
    if(writeConversionRule(s, lang, java_function, java_functional, argument_index, qt_name, java_name, option, __jni_env, qtjambi_scope))
        return;

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

    bool invalidate = false;
    bool parenRequired = false;
    bool semiRequired = (option & NoTmpVariable) != NoTmpVariable || !java_name.isEmpty();
    /*bool needLambda = false;
    if((option & DirectReturn) == DirectReturn){
        needLambda = true;
    }else if((option & NoTmpVariable) == NoTmpVariable){
        if(java_name.isEmpty())
            needLambda = true;
    }*/

    if (java_type->isJObjectWrapper()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    JObjectWrapper* _" << qt_name << " = new JObjectWrapper();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<JObjectWrapper*>(" << qt_name << ") = ";
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
        s << "qtjambi_cast<JObjectWrapper>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isJMapWrapper()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    JMapWrapper* _" << qt_name << " = new JMapWrapper();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<JMapWrapper*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<JMapWrapper>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isJCollectionWrapper()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    JCollectionWrapper* _" << qt_name << " = new JCollectionWrapper();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<JCollectionWrapper*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<JCollectionWrapper>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isJIteratorWrapper()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    JIteratorWrapper* _" << qt_name << " = new JIteratorWrapper();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<JIteratorWrapper*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT << "const JIteratorWrapper& " << qt_name << " = ";
        }
        s << "qtjambi_cast<JIteratorWrapper>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isJEnumWrapper()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    JEnumWrapper* _" << qt_name << " = new JEnumWrapper();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<JEnumWrapper*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<JEnumWrapper>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isJQFlagsWrapper()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    JQFlagsWrapper* _" << qt_name << " = new JQFlagsWrapper();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<JQFlagsWrapper*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT << "const JQFlagsWrapper& " << qt_name << " = ";
        }
        s << "qtjambi_cast<JQFlagsWrapper>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isVariant()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QVariant* _" << qt_name << " = new QVariant();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QVariant*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<QVariant>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isArray()) {
        AbstractMetaType *elementType = java_type->arrayElementType();
        if(elementType->isPrimitive()){
            // ### Don't assert on wrong array lengths
            s << INDENT << "Q_ASSERT(" << __jni_env << "->GetArrayLength(jarray(" << java_name << ")) == " << java_type->arrayElementCount() << ");" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, elementType);
            s << " " << qt_name << "[" << java_type->arrayElementCount() << "];" << Qt::endl;

            s << INDENT << __jni_env << "->" << getXxxArrayRegion(elementType)
              << "( " << translateType(java_type, option) << "(" << java_name << "), 0, " << java_type->arrayElementCount()
              << ", reinterpret_cast<" << translateType(elementType, option) << " *>(" << qt_name << "));" << Qt::endl;
        }else{
            s << INDENT << "Q_ASSERT(" << __jni_env << "->GetArrayLength(jarray(" << java_name << ")) == " << java_type->arrayElementCount() << ");" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, elementType);
            s << qt_name << "[" << java_type->arrayElementCount() << "];" << Qt::endl;

            for (int i = 0; i < java_type->arrayElementCount(); ++i) {
                writeJavaToQt(s, elementType, qt_name + "[" + QString::number(i) + "]",
                               __jni_env + "->GetObjectArrayElement(" + java_name + ", " + QString::number(i) + ")", nullptr, nullptr, -1, Option(option | NoTmpVariable), __jni_env);
            }
        }
        semiRequired = false;
    } else if (java_type->isTargetLangString()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QString* _" << qt_name << " = new QString();" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QString*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<";
        writeTypeInfo(s, java_type, ForceValueType);
        s << ">(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->isTargetLangStringRef()) {
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
    } else if (java_type->isTargetLangLatin1String()) {
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
            s << INDENT << "QByteArray " << qt_name << "_qByteArray = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ").toLatin1();" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, ForceValueType); //ForceConstReference
            s << qt_name << " = QLatin1String(" << qt_name << "_qByteArray);" << Qt::endl;
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QLatin1String>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isTargetLangStringView()) {
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
            s << INDENT << "QString " << qt_name << "_qstring = qtjambi_cast<QString>(" << __jni_env << ", " << java_name << ");";
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = QStringView(&" << qt_name << "_qstring);";
            semiRequired = false;
        }else{
            s << "qtjambi_cast<QStringView>(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
        }
    } else if (java_type->isTargetLangChar()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QChar* _" << qt_name << " = new QChar;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
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
        AbstractMetaEnum *java_enum = nullptr;
        if (java_type->isEnum()) {
            const EnumTypeEntry * enumType = static_cast<const EnumTypeEntry *>(java_type->typeEntry());
            java_enum = m_classes.findEnum(enumType);
            if ((java_enum && !java_enum->isPublic()) || (option & EnumAsInts)==EnumAsInts) {
                if((option & OptionalScope) == OptionalScope){
                    s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                    s << INDENT << "    qint" << enumType->size() << "* _" << qt_name << " = new qint" << enumType->size() << ";" << Qt::endl;
                    s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                    s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                    s << INDENT << "}" << Qt::endl;
                    s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
                    s << INDENT << "*reinterpret_cast<qint" << enumType->size() << "*>(" << qt_name << ") = ";
                }else if((option & DirectReturn) == DirectReturn){
                    s << INDENT << "return ";
                }else if((option & NoTmpVariable) == NoTmpVariable){
                    if(!qt_name.isEmpty())
                        s << INDENT << qt_name << " = ";
                }else{
                    s << INDENT << "qint" << enumType->size() << " " << qt_name << " = ";
                }
                s << "qtjambi_cast<qint" << enumType->size() << ">(" << __jni_env << ", " << java_name << ")";
                written = true;
            }
        }

        if (!written) {
            QString qualified_name = java_type->typeEntry()->qualifiedCppName();
            if((option & OptionalScope) == OptionalScope){
                s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                s << INDENT << "    " << qualified_name << "* _" << qt_name << " = new " << qualified_name << ";" << Qt::endl;
                s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                && type->type() != ContainerTypeEntry::InitializerListContainer
                && type->type() != ContainerTypeEntry::QArrayDataContainer
                && type->type() != ContainerTypeEntry::QTypedArrayDataContainer){
            if((option & OptionalScope) == OptionalScope){
                if(java_type->indirections().isEmpty()
                        && java_type->getReferenceType()==AbstractMetaType::Reference
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
                            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                            s << INDENT << "}" << Qt::endl;
                            s << INDENT << "*reinterpret_cast<";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << "*>(" << qt_name << ") = qtjambi_cast<";
                            writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                            s << ">(" << __jni_env << ", *" << qtjambi_scope << ", " << java_name << ");" << Qt::endl;
                        }else if(argument_index==0){
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
                        }else{
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
                            s << INDENT << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                        s << INDENT << "if(!out)" << Qt::endl;
                        s << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                    s << ' ' << qt_name << " = ";
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
                if(java_type->indirections().isEmpty() && java_type->getReferenceType()==AbstractMetaType::Reference){
                    s << INDENT << "if(" << qtjambi_scope << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if(argument_index==0){
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
                        }else{
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
                    }
                    s << INDENT << "}else{" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                        s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
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
                        s << INDENT << "return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                    writeTypeInfo(s, java_type, SkipName); //ForceConstReference
                    s << ' ' << qt_name << " = ";
                }
                s << "qtjambi_cast<";
                writeTypeInfo(s, java_type, SkipName);
                s << ">(" << __jni_env << ", " << qtjambi_scope << ", " << java_name << ")";
            }
        }else{
            writeJavaToQtInitializerList(s, java_type, qt_name, java_name, java_function, java_functional, -1, __jni_env, qtjambi_scope, option);
            semiRequired = false;
        }

    } else if (java_type->isThread()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    void** _" << qt_name << " = new void*;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<";
            writeTypeInfo(s, java_type, Option(SkipName | ExcludeReference));
            s << "*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_to_thread(" << __jni_env << ", " << java_name << ")";

    } else if (java_type->typeEntry()->isQModelIndexType()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QModelIndex* _" << qt_name << " = new QModelIndex;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QModelIndex*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<QModelIndex>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->typeEntry()->isQMetaObjectType()) {
        if((option & OptionalScope) == OptionalScope){
            if(java_type->indirections().size()==0 && java_type->getReferenceType()==AbstractMetaType::Reference){
                s << qt_name << " = &qtjambi_cast<QMetaObject&>(" << __jni_env << ", " << java_name << ")";
            }else{
                s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                s << INDENT << "    QMetaObject** _" << qt_name << " = new QMetaObject*;" << Qt::endl;
                s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
                s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                writeTypeInfo(s, java_type, SkipName); //ForceConstReference
                s << ' ' << qt_name << " = ";
            }
            if(java_type->indirections().size()==0 && java_type->getReferenceType()==AbstractMetaType::Reference)
                s << "qtjambi_cast<const QMetaObject&>(" << __jni_env << ", " << java_name << ")";
            else
                s << "qtjambi_cast<const QMetaObject*>(" << __jni_env << ", " << java_name << ")";
        }
    } else if (java_type->typeEntry()->isQMetaPropertyType()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QMetaProperty* _" << qt_name << " = new QMetaProperty;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QMetaProperty*>(" << qt_name << ") = ";
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
        s << "qtjambi_cast<QMetaProperty>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->typeEntry()->isQMetaObjectConnectionType()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QMetaObject::Connection* _" << qt_name << " = new QMetaObject::Connection;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QMetaObject::Connection*>(" << qt_name << ") = ";
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
        s << "qtjambi_cast<QMetaObject::Connection>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->typeEntry()->isQMetaMethodType()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QMetaMethod* _" << qt_name << " = new QMetaMethod;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QMetaMethod*>(" << qt_name << ") = ";
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
        s << "qtjambi_cast<QMetaMethod>(" << __jni_env << ", " << java_name << ")";
    } else if (java_type->typeEntry()->isQMetaEnumType()) {
        if((option & OptionalScope) == OptionalScope){
            s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
            s << INDENT << "    QMetaEnum* _" << qt_name << " = new QMetaEnum;" << Qt::endl;
            s << INDENT << "    " << qt_name << " = _" << qt_name << ";" << Qt::endl;
            s << INDENT << "    " << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
            s << INDENT << "}" << Qt::endl;
            s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            s << INDENT << "*reinterpret_cast<QMetaEnum*>(" << qt_name << ") = ";
        }else if((option & DirectReturn) == DirectReturn){
            s << INDENT << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!qt_name.isEmpty())
                s << INDENT << qt_name << " = ";
        }else{
            s << INDENT;
            writeTypeInfo(s, java_type, NoOption); //ForceConstReference
            s << ' ' << qt_name << " = ";
        }
        s << "qtjambi_cast<QMetaEnum>(" << __jni_env << ", " << java_name << ")";
    } else {
        const TypeEntry *type = java_type->typeEntry();
        QString qualified_class_name = fixCppTypeName(type->qualifiedCppName());

        QString convertLine;
        {
            QTextStream s(&convertLine);
            if (java_type->isFunctional()) {
                const FunctionalTypeEntry* funEntry = static_cast<const FunctionalTypeEntry*>(java_type->typeEntry());
                QString funName = funEntry->targetLangName();
                if(!funEntry->qualifier().isEmpty()){
                    funName = funEntry->qualifier() + "$" + funName;
                }
                s << "qtjambi_cast<" << qualified_class_name << ">(" << __jni_env << ", " << java_name << ")";
            }else if (java_type->isIterator()) {
                s << "reinterpret_deref_cast<" << qualified_class_name << " "
                  << QString(java_type->actualIndirections(), '*') << ">(" << __jni_env << ", ";
                s << "qtjambi_to_object(" << __jni_env << ", " << java_name << "))";

            } else if (java_type->isPrimitive()) {
                if (option & BoxedPrimitive) {
                    const PrimitiveTypeEntry *pentry = static_cast<const PrimitiveTypeEntry *>(type);

                    if (!pentry->preferredConversion())
                        pentry = TypeDatabase::instance()->findTargetLangPrimitiveType(pentry->targetLangName());
                    Q_ASSERT(pentry);

                    s << "qtjambi_to_" << pentry->targetLangName() << "(" << __jni_env << ", " << java_name << ")";

                } else if ((option & GlobalRefJObject) && type->jniName() == QLatin1String("jobject")) {
                    s << __jni_env << "->NewGlobalRef(" << java_name << ")";
                } else {
                    writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                    s << "(";
                    s << java_name;
                    s << ")";
                }

            } else if ((java_type->isQObject() || java_type->isObject() || java_type->isValue())
                       && static_cast<const ImplementorTypeEntry *>(type)->designatedInterface()) {
                const InterfaceTypeEntry *ie =
                    static_cast<const ImplementorTypeEntry *>(type)->designatedInterface();
                if ((option & UseNativeIds) == 0 || !ie->isNativeIdBased()){
                    s << "qtjambi_cast<";
                    writeTypeInfo(s, java_type);
                    s << ">(" << __jni_env << ", " << java_name << ")";
                }else{
                    if(java_type->indirections().size()==0){
                        s << "qtjambi_interface_from_nativeId_deref<" << java_type->typeEntry()->qualifiedCppName()
                          << ">(" << __jni_env << ", ";
                    }else{
                        s << "qtjambi_interface_from_nativeId<" << java_type->typeEntry()->qualifiedCppName() << ">(";
                    }
                    s << java_name << ")";
                }
            } else if (java_type->isPointerContainer()) {
                bool done = false;
                const PointerContainerTypeEntry* pentry = static_cast<const PointerContainerTypeEntry*>(java_type->typeEntry());
                switch(pentry->type()){
                case PointerContainerTypeEntry::unique_ptr:
                case PointerContainerTypeEntry::QScopedPointer:
                if(java_type->instantiations().size()>0){
                    s << "qtjambi_cast<";
                    writeTypeInfo(s, java_type->instantiations()[0]);
                    s << "*>(" << __jni_env << ", ";
                    if(!qtjambi_scope.isEmpty()){
                        s << qtjambi_scope << ", ";
                    }
                    s << java_name << ")";
                    done = true;
                }
                    break;
                default:
                    break;
                }
                if(!done){
                    s << "qtjambi_cast<";
                    writeTypeInfo(s, java_type);
                    s << ">(" << __jni_env << ", ";
                    if(!qtjambi_scope.isEmpty()){
                        s << qtjambi_scope << ", ";
                    }
                    s << java_name << ")";
                }
            } else if (java_type->isObject() || java_type->isQObject() || java_type->isNativePointer()) {
                if (java_type->isNativePointer()) {
                    if (java_type->getReferenceType()==AbstractMetaType::Reference) {
                        s << "reinterpret_deref_cast<"
                          << qualified_class_name;
                        if(java_type->indirections().size()>1){
                            s << " " << QString(java_type->indirections().size()-1, '*');
                        }
                        s << ">(" << __jni_env << ", ";
                    }else{
                        s << "reinterpret_cast<";
                        writeTypeInfo(s, java_type);
                        s << ">(";
                    }
                    s << "qtjambi_to_cpointer("
                      << __jni_env << ", "
                      << java_name << ", "
                      << java_type->actualIndirections() << "))";
                } else if (java_type->isQObject()) {
                    if ((option & UseNativeIds) == 0){
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    }else{
                        if (java_type->getReferenceType()==AbstractMetaType::Reference) {
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "qtjambi_interface_from_nativeId_deref";
                            }else{
                                s << "qtjambi_object_from_nativeId_deref";
                            }
                            s << "<" << java_type->typeEntry()->qualifiedCppName() << ">(" << __jni_env << ", " << java_name << ")";
                        }else{
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "qtjambi_interface_from_nativeId";
                            }else{
                                s << "qtjambi_object_from_nativeId";
                            }
                            s << "<" << java_type->typeEntry()->qualifiedCppName() << ">(" << java_name << ")";
                        }
                    }
                } else {
                    if ((option & UseNativeIds) == 0){
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    }else{
                        if (java_type->getReferenceType()==AbstractMetaType::Reference) {
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "qtjambi_interface_from_nativeId_deref";
                            }else{
                                s << "qtjambi_object_from_nativeId_deref";
                            }
                            s << "<" << java_type->typeEntry()->qualifiedCppName()
                              << ">(" << __jni_env << ", " << java_name << ")";
                        }else{
                            if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                                s << "qtjambi_interface_from_nativeId";
                            }else{
                                s << "qtjambi_object_from_nativeId";
                            }
                            s << "<" << java_type->typeEntry()->qualifiedCppName() << ">(" << java_name << ")";
                        }
                    }
                }

            } else {
                // Return values...
                if(!java_type->isConstant() && java_type->getReferenceType()==AbstractMetaType::Reference){
                    if ((option & UseNativeIds) == 0){
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    }else{
                        if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                            s << "qtjambi_interface_from_nativeId_deref";
                        }else{
                            s << "qtjambi_object_from_nativeId_deref";
                        }
                        s << "<" << java_type->typeEntry()->qualifiedCppName()
                          << ">(" << __jni_env << ", " << java_name << ")";
                    }
                }else if (argument_index == 0) {
                    if ((option & UseNativeIds) == 0){
                        //s << "qtjambi_to_object(" << __jni_env << ", " << java_name << ")";
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    }else{
                        s << "reinterpret_value_cast<"
                          << qualified_class_name;
                        if(java_type->indirections().size()>1){
                            s << " " << QString(java_type->indirections().size()-1, '*');
                        }
                        s << ">(";
                        if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                            s << "qtjambi_interface_from_nativeId";
                        }else{
                            s << "qtjambi_object_from_nativeId";
                        }
                        s << "<" << java_type->typeEntry()->qualifiedCppName()
                          << ">(" << __jni_env << ", " << java_name << ")";
                        s << ")";
                    }
                } else {
                    if ((option & UseNativeIds) == 0) {
                        s << "qtjambi_cast<";
                        writeTypeInfo(s, java_type);
                        s << ">(" << __jni_env << ", " << java_name << ")";
                    } else if(java_type->indirections().size()==0 && (java_type->isConstant() || java_type->getReferenceType()==AbstractMetaType::NoReference) && hasPublicDefaultConstructor(java_type)){
                        s << "qtjambi_value_from_nativeId<" << qualified_class_name;
                        if(java_type->indirections().size()>1){
                            s << " " << QString(java_type->indirections().size()-1, '*');
                        }
                        s << ">(" << java_name << ")";
                    } else {
                        if(java_type->typeEntry()->isInterface() || java_type->typeEntry()->designatedInterface() || java_type->typeEntry()->isFunctional()){
                            s << "qtjambi_interface_from_nativeId_deref";
                        }else{
                            s << "qtjambi_object_from_nativeId_deref";
                        }
                        s << "<" << java_type->typeEntry()->qualifiedCppName()
                          << ">(" << __jni_env << ", " << java_name << ")";
                    }
                }
            }
        }

        if((option & OptionalScope) == OptionalScope){
            Option _option = Option(SkipName | ExcludeReference);
            if (java_type->isPrimitive()) {
                _option = Option(_option | ExcludeConst);
            }
            if(java_type->indirections().isEmpty() &&
                    (java_type->getReferenceType()==AbstractMetaType::Reference
                     || ((java_type->typeEntry()->isObject() || java_type->typeEntry()->isInterface()) && !this->hasPublicAssignmentOperator(java_type)))){
                if(argument_index==0){
                    s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "*reinterpret_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(" << qt_name << ") = " << convertLine << ";" << Qt::endl;
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
                }else{
                    if(java_type->isConstant()){
                        s << INDENT << qt_name << " = const_cast<";
                        writeTypeInfo(s, java_type, Option(SkipName | ForceValueType));
                        s << "*>(&" << convertLine << ");" << Qt::endl;
                    }else{
                        s << INDENT << qt_name << " = &" << convertLine << ";" << Qt::endl;
                    }
                }
                semiRequired = false;
            }else{
                s << INDENT << "if(" << qtjambi_scope << " && !" << qt_name << "){" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    if(java_type->indirections().isEmpty()){
                        if(this->hasPublicDefaultConstructor(java_type)){
                            s << INDENT;
                            writeTypeInfo(s, java_type, _option);
                            s << "* _" << qt_name << " = new ";
                            writeTypeInfo(s, java_type, _option);
                            s << ";" << Qt::endl;
                            s << INDENT << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                            s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                        }else{
                            s << INDENT << "int metaTypeID = QMetaType::type(\"" << java_type->typeEntry()->qualifiedCppName() << "\");" << Qt::endl;
                            s << INDENT << "if(metaTypeID!=QMetaType::UnknownType){" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "void* _" << qt_name << " = QMetaType::create(metaTypeID, nullptr);" << Qt::endl;
                                s << INDENT << qtjambi_scope << "->addFinalAction([metaTypeID, _" << qt_name << "](){QMetaType::destroy(metaTypeID, _" << qt_name << ");});" << Qt::endl;
                                s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                            }
                            s << INDENT << "}" << Qt::endl;
                        }
                    }else{
                        s << INDENT << "void** _" << qt_name << " = new void*;" << Qt::endl;
                        s << INDENT << qtjambi_scope << "->addFinalAction([_" << qt_name << "](){delete _" << qt_name << ";});" << Qt::endl;
                        s << INDENT << qt_name << " = _" << qt_name << ";" << Qt::endl;
                    }
                }
                s << INDENT << "}" << Qt::endl;
                s << INDENT << "if(!" << qt_name << ")" << Qt::endl << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                if (java_type->isPointerContainer()) {
                    const PointerContainerTypeEntry* pentry = static_cast<const PointerContainerTypeEntry*>(java_type->typeEntry());
                    switch(pentry->type()){
                    case PointerContainerTypeEntry::unique_ptr:
                    case PointerContainerTypeEntry::QScopedPointer:
                    if(java_type->instantiations().size()>0){
                        s << INDENT;
                        writeTypeInfo(s, java_type->instantiations()[0], SkipName);
                        s << "* " << qt_name << " = ";
                        done = true;
                        invalidate = true;
                    }
                        break;
                    default:
                        break;
                    }
                }
                if(!done){
                    s << INDENT;
                    writeTypeInfo(s, java_type, SkipName); //ForceConstReference
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
        s << INDENT << "QTJAMBI_INVALIDATE_AFTER_USE_UNCHECKED(" << __jni_env << ", " << java_name << ")" << Qt::endl;
    }
}

static int nativePointerType(const AbstractMetaType *java_type) {
    Q_ASSERT(java_type);
    Q_ASSERT(java_type->isNativePointer());

    if (!java_type->typeEntry()->isPrimitive())
        return int(QNativePointer::Type::Pointer);

    if (java_type->indirections().size() > 1)
        return int(QNativePointer::Type::Pointer);

    static const QHash<QString, int> types{
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

static const QString nativePointerTypeString(const AbstractMetaType *java_type) {
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

void CppImplGenerator::writeQtToJava(QTextStream &s,
                                     const AbstractMetaType *java_type,
                                     const QString &qt_name,
                                     const QString &java_name,
                                     const AbstractMetaFunction *java_function,
                                     const AbstractMetaFunctional *java_functional,
                                     int argument_index,
                                     Option option,
                                     const QString& __jni_env,
                                     const QString& qtjambi_scope) {
    // Conversion to Java: Native code for return values, shell code for arguments
    TypeSystem::Language lang = argument_index == 0 ? TypeSystem::NativeCode : TypeSystem::ShellCode;
    if(writeConversionRule(s, lang, java_function, java_functional, argument_index, qt_name, java_name, option, __jni_env, qtjambi_scope))
        return;

    QScopedPointer<Indentation> indentationPointer;
    bool semiRequired = (option & NoTmpVariable) != NoTmpVariable || !java_name.isEmpty();
    /*bool needLambda = false;
    if((option & DirectReturn) == DirectReturn){
        needLambda = true;
    }else if((option & NoTmpVariable) == NoTmpVariable){
        if(java_name.isEmpty())
            needLambda = true;
    }*/
    bool resetObjectAfterUse = (
                (java_function && java_function->resetObjectAfterUse(argument_index))
                || (java_functional && java_functional->resetObjectAfterUse(argument_index))
            );

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
    if (java_type->typeEntry()->isObject() && java_type->actualIndirections() == 0) {
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
        ReportHandler::warning(QString("Object type '%1' passed as value. Resulting code will not compile.  %2")
                               .arg(java_type->cppSignature())
                               .arg(functionName));
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
                                 .arg(java_type->cppSignature())
                                 .arg(functionName));
    }

    if (java_type->isArray()) { // jniTypeName(java_type)
        AbstractMetaType *elementType = java_type->arrayElementType();
        Q_ASSERT(elementType);
        if(elementType->isPrimitive()){
            s << INDENT;
            if((option & DirectReturn) == DirectReturn){
                s << "return ";
            }else if((option & NoTmpVariable) == NoTmpVariable){
                if(!java_name.isEmpty())
                    s << java_name << " = ";
            }else{
                s << translateType(java_type, option) << " " << java_name << " = ";
            }
            s << "qtjambi_to_" << translateType(java_type, option) << "(" << __jni_env << ", reinterpret_cast<"
              << translateType(elementType, option) << " *>("
              << qt_name << "), " << java_type->arrayElementCount() << ")";
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
            QString qualifiedTargetLangName = elementType->fullName().mid(elementType->package().length()+1).replace(".", "$");
            s << "qtjambi_to_jobjectArray<";
            writeTypeInfo(s, elementType, Option(ForceValueType));
            s << ">(" << __jni_env
              << ", \"" << elementType->package().replace(".", "/") << "/" << qualifiedTargetLangName << "\""
              << ", " << qt_name
              << ", jsize(" << java_type->arrayElementCount() << "), [](JNIEnv * " << __jni_env << ", ";
            writeTypeInfo(s, elementType, Option(ForceConstReference));
            s << " __element) -> jobject {" << Qt::endl;
            {
                INDENTATION(INDENT)
                quint32 option2 = quint32(option);
                option2 |= DirectReturn;
                option2 |= NoTmpVariable;
                writeQtToJava(s, elementType, "__element", "", nullptr, nullptr, -1, Option(option2), __jni_env, "");
            }
            s << ";" << Qt::endl << INDENT << "}";
            s << ")";
        }
    } else if (java_type->isInitializerList()) {
        Q_ASSERT(java_type->instantiations().size()>0);
        const AbstractMetaType *elementType = java_type->instantiations().at(0);
        if(elementType->isPrimitive()){
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
            if(!qtjambi_scope.isEmpty()){
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
            s << "qtjambi_from_" << type->targetLangName() << "(" << __jni_env << ", " << qt_name << ")";
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
    } else if (java_type->isTargetLangString()
               || java_type->isTargetLangStringRef()
               || java_type->isTargetLangStringView()) {
        s << INDENT;
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            s << "jstring " << java_name << " = ";
        }
        if (java_type->isTargetLangStringRef()) {
            s << "qtjambi_cast<jstring>(" << __jni_env << ", " << qt_name << ")";
        } else if (java_type->isTargetLangStringView()) {
            s << "qtjambi_cast<jstring>(" << __jni_env << ", " << qt_name << ")";
        } else {
            s << "qtjambi_cast<jstring>(" << __jni_env << ", " << qt_name << ")";
        }
    } else if (java_type->isTargetLangChar()) {
        s << INDENT;
        if((option & DirectReturn) == DirectReturn){
            s << "return ";
        }else if((option & NoTmpVariable) == NoTmpVariable){
            if(!java_name.isEmpty())
                s << java_name << " = ";
        }else{
            s << "jchar " << java_name << " = ";
        }
        s << qt_name << ".unicode()";
    } else if (java_type->isIntegerFlags()
               || ((option & EnumAsInts) && java_type->isFlags())) {
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
                s << "jint " << java_name << " = ";
            }
        }
        if (option & BoxedPrimitive) {
            s << "qtjambi_from_int(" << __jni_env << ", " << qt_name << ")";
        } else {
            s << qt_name;
        }

    } else if (java_type->isIntegerEnum()
               || ((option & EnumAsInts) && java_type->isEnum())) {
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
                s << java_type->typeEntry()->jniName() << " " << java_name << " = ";
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
            s << qt_name;
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
            QScopedPointer<AbstractMetaType> scopedPointer;
            const AbstractMetaType *container_type = java_type;
            if(java_type->indirections().size()==1){
                scopedPointer.reset(container_type->copy());
                container_type = scopedPointer.data();
                scopedPointer->setIndirections(QList<bool>());
                scopedPointer->setReferenceType(AbstractMetaType::Reference);
            }
            const ContainerTypeEntry *type = static_cast<const ContainerTypeEntry *>(container_type->typeEntry());
            if (type->type() == ContainerTypeEntry::ListContainer
                    || type->type() == ContainerTypeEntry::ByteArrayListContainer
                    || type->type() == ContainerTypeEntry::VectorContainer
                    || type->type() == ContainerTypeEntry::QTypedArrayDataContainer
                    || type->type() == ContainerTypeEntry::QArrayDataContainer
                    || type->type() == ContainerTypeEntry::StringListContainer
                    || type->type() == ContainerTypeEntry::LinkedListContainer
                    || type->type() == ContainerTypeEntry::StackContainer
                    || type->type() == ContainerTypeEntry::SetContainer
                    || type->type() == ContainerTypeEntry::QueueContainer
                    || type->type() == ContainerTypeEntry::QArrayContainer
                    || type->type() == ContainerTypeEntry::QVector2DArrayContainer
                    || type->type() == ContainerTypeEntry::QVector3DArrayContainer
                    || type->type() == ContainerTypeEntry::QVector4DArrayContainer
                ) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", ";
                if(!qtjambi_scope.isEmpty()
                        && container_type->getReferenceType()==AbstractMetaType::Reference){
                    s << qtjambi_scope << ", ";
                }
                s << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::MultiMapContainer
                       || type->type() == ContainerTypeEntry::MultiHashContainer
                       || type->type() == ContainerTypeEntry::MapContainer
                       || type->type() == ContainerTypeEntry::HashContainer) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", ";
                if(!qtjambi_scope.isEmpty()
                        && container_type->getReferenceType()==AbstractMetaType::Reference){
                    s << qtjambi_scope << ", ";
                }
                s << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::PairContainer) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::QDBusReplyContainer) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::QQmlListPropertyContainer) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (type->type() == ContainerTypeEntry::QDeclarativeListPropertyContainer) {
                const QList<const AbstractMetaType *>& args = container_type->instantiations();
                Q_ASSERT(args.size() == 1);
                const AbstractMetaType *arg = container_type->instantiations().at(0);
                s << "qtjambi_cast<jobject>(" << __jni_env << ", QtjambiDeclarativeListProperty::createFrom<";
                writeTypeInfo(s, arg, NoOption);
                s << ">(";
                if(java_type->indirections().size()==1){
                    s << "*";
                }
                s << qt_name << "))";
            } else {
                s << "0";
                ReportHandler::warning(QString("unable to generate container type %1, type=%2")
                                       .arg(java_type->name()).arg(type->type()));
            }
        }else if (java_type->isJObjectWrapper()
                   || java_type->isJMapWrapper()
                   || java_type->isJCollectionWrapper()
                   || java_type->isJEnumWrapper()
                   || java_type->isJIteratorWrapper()
                   || java_type->isJQFlagsWrapper()) {
            s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
        } else if (java_type->isVariant()) {
            s << "qtjambi_cast<jobject>(" << __jni_env << ", "<< qt_name << ")";
        } else if (java_type->isTargetLangEnum()) {
            s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
        } else if (java_type->isTargetLangFlags()) {
            Q_ASSERT((option & EnumAsInts) == 0);
            s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
        } else if (java_type->isThread()) {
            s << "qtjambi_from_thread(" << __jni_env << ", " << qt_name << ")";
        } else if (java_type->isFunctional()) {
            const FunctionalTypeEntry* funEntry = static_cast<const FunctionalTypeEntry*>(java_type->typeEntry());
            QString funName = funEntry->targetLangName();
            if(!funEntry->qualifier().isEmpty()){
                funName = funEntry->qualifier() + "$" + funName;
            }
            s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ", \"" << funEntry->qualifiedCppName() << "\")";
        } else if (java_type->isPointerContainer() && java_type->instantiations().size()==1) {
            //const PointerContainerTypeEntry* pcEntry = static_cast<const PointerContainerTypeEntry*>(java_type->typeEntry());
            s << "qtjambi_cast<jobject>(" << __jni_env << ", ";
            if(!qtjambi_scope.isEmpty()){
                s << qtjambi_scope << ", ";
            }
            s << qt_name << ")";
        } else {
            if (java_type->isQObject()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->isNativePointer()) {
                s << "qtjambi_from_cpointer(" << __jni_env << ", ";
                if (java_type->getReferenceType()==AbstractMetaType::Reference)
                    s << "&";
                s << qt_name << ", " << nativePointerTypeString(java_type) << ", "
                << java_type->actualIndirections() << ")";
            } else if (java_type->typeEntry()->isQModelIndexType()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->typeEntry()->isQMetaObjectType()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->typeEntry()->isQMetaObjectConnectionType()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->typeEntry()->isQMetaPropertyType()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->typeEntry()->isQMetaMethodType()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->typeEntry()->isQMetaEnumType()) {
                s << "qtjambi_cast<jobject>(" << __jni_env << ", " << qt_name << ")";
            } else if (java_type->isValue()) {
                s << fromObject(java_type, qt_name, false, __jni_env);
            } else if (java_type->isIterator()) {
                const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(java_type->typeEntry());
                bool found = false;
                if(java_type->iteratorInstantiations().size()==2){
                    QString newIterator;
                    QTextStream s2(&newIterator);
                    s2 << "new ";
                    writeTypeInfo(s2, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                    s2 << "(" << qt_name << ")";
                    writeQMapIteratorFunction(s, java_type,
                                           java_type->iteratorInstantiations().at(0),
                                           java_type->iteratorInstantiations().at(1),
                                           newIterator, option, "", __jni_env, qtjambi_scope);
                    found = true;
                }else if(java_type->iteratorInstantiations().size()==1){
                    QString newIterator;
                    QTextStream s2(&newIterator);
                    s2 << "new ";
                    writeTypeInfo(s2, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                    s2 << "(" << qt_name << ")";
                    writeQIteratorFunction(s, java_type,
                                           java_type->iteratorInstantiations().at(0),
                                           newIterator, option, "", __jni_env, qtjambi_scope);
                    found = true;
                }
                if(!found && java_function && java_function->declaringClass()){
                    if(AbstractMetaClass * iteratorClass = java_function->declaringClass()->findIterator(iteratorType)){
                        if(iteratorClass->templateBaseClassInstantiations().size()==1){
                            QScopedPointer<AbstractMetaType> copy(java_type->copy());
                            copy->setConstant(false);
                            copy->setReferenceType(AbstractMetaType::NoReference);
                            copy->setIndirections(QList<bool>());
                            if(iteratorClass->enclosingClass())
                                copy->setTypeEntry(iteratorClass->enclosingClass()->typeEntry());
                            if(java_function->declaringClass()->templateBaseClass())
                                copy->setTypeEntry(java_function->declaringClass()->templateBaseClass()->typeEntry());
                            copy->setInstantiations(iteratorClass->templateBaseClassInstantiations());
                            copy->setArrayElementCount(0);
                            copy->setArrayElementType(nullptr);
                            copy->setForceBoxedPrimitives(true);
                            AbstractMetaBuilder::decideUsagePattern(copy.data());
                            QString newIterator;
                            QTextStream s2(&newIterator);
                            s2 << "new ";
                            writeTypeInfo(s2, copy.data(), Option(ExcludeReference | ExcludeConst | SkipName));
                            s2 << "::" << iteratorType->iteratorName() << "(" << qt_name << ")";
                            writeQIteratorFunction(s, copy.data(), java_function->declaringClass()->templateBaseClassInstantiations().at(0), newIterator, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                            found = true;
                        }else{
                            for(const AbstractMetaFunction *function : iteratorClass->functions()){
                                if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                                    QString newIterator;
                                    QTextStream s2(&newIterator);
                                    s2 << "new ";
                                    writeTypeInfo(s2, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                                    s2 << "(" << qt_name << ")";
                                    writeQIteratorFunction(s, java_type, function->type(), newIterator, option, "", __jni_env, qtjambi_scope);
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(!found && iteratorType->containerType()){
                    if(AbstractMetaClass * containerClass = m_classes.findClass(iteratorType->containerType()->qualifiedCppName())){
                        if(containerClass->templateBaseClass() && containerClass->templateBaseClass()->typeEntry()->isContainer()){
                            const ContainerTypeEntry* containerType = static_cast<const ContainerTypeEntry*>(containerClass->templateBaseClass()->typeEntry());
                            if(containerType->type()==ContainerTypeEntry::MapContainer
                                || containerType->type()==ContainerTypeEntry::MultiMapContainer
                                || containerType->type()==ContainerTypeEntry::HashContainer
                                    || containerType->type()==ContainerTypeEntry::MultiHashContainer){
                                if(containerClass->templateBaseClassInstantiations().size()==2){
                                    QScopedPointer<AbstractMetaType> copy(java_type->copy());
                                    copy->setConstant(false);
                                    copy->setReferenceType(AbstractMetaType::NoReference);
                                    copy->setIndirections(QList<bool>());
                                    copy->setTypeEntry(containerClass->templateBaseClass()->typeEntry());
                                    copy->setInstantiations(containerClass->templateBaseClassInstantiations());
                                    copy->setArrayElementCount(0);
                                    copy->setArrayElementType(nullptr);
                                    copy->setForceBoxedPrimitives(true);
                                    AbstractMetaBuilder::decideUsagePattern(copy.data());
                                    QString newIterator;
                                    QTextStream s2(&newIterator);
                                    s2 << "new ";
                                    writeTypeInfo(s2, copy.data(), Option(ExcludeReference | ExcludeConst | SkipName));
                                    s2 << "::" << iteratorType->iteratorName() << "(" << qt_name << ")";
                                    writeQMapIteratorFunction(s, copy.data(),
                                                           containerClass->templateBaseClassInstantiations().at(0),
                                                           containerClass->templateBaseClassInstantiations().at(1),
                                                           newIterator, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                                    found = true;
                                }
                            }else{
                                if(containerClass->templateBaseClassInstantiations().size()==1){
                                    QScopedPointer<AbstractMetaType> copy(java_type->copy());
                                    copy->setConstant(false);
                                    copy->setReferenceType(AbstractMetaType::NoReference);
                                    copy->setIndirections(QList<bool>());
                                    copy->setTypeEntry(containerClass->templateBaseClass()->typeEntry());
                                    copy->setInstantiations(containerClass->templateBaseClassInstantiations());
                                    copy->setArrayElementCount(0);
                                    copy->setArrayElementType(nullptr);
                                    copy->setForceBoxedPrimitives(true);
                                    AbstractMetaBuilder::decideUsagePattern(copy.data());
                                    QString newIterator;
                                    QTextStream s2(&newIterator);
                                    s2 << "new ";
                                    writeTypeInfo(s2, copy.data(), Option(ExcludeReference | ExcludeConst | SkipName));
                                    s2 << "::" << iteratorType->iteratorName() << "(" << qt_name << ")";
                                    writeQIteratorFunction(s, copy.data(), containerClass->templateBaseClassInstantiations().at(0), newIterator, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                                    found = true;
                                }
                            }
                        }
                    }else{
                        if(java_function
                                && java_function->declaringClass()
                                && java_type->originalTypeDescription()==java_function->declaringClass()->baseClassNames().join("::")+"::"+iteratorType->iteratorName()){
                            if(java_function->declaringClass()->templateBaseClassInstantiations().size()==1){
                                QScopedPointer<AbstractMetaType> copy(java_type->copy());
                                copy->setConstant(false);
                                copy->setReferenceType(AbstractMetaType::NoReference);
                                copy->setIndirections(QList<bool>());
                                if(java_function->declaringClass()->templateBaseClass())
                                    copy->setTypeEntry(java_function->declaringClass()->templateBaseClass()->typeEntry());
                                copy->setInstantiations(java_function->declaringClass()->templateBaseClassInstantiations());
                                copy->setArrayElementCount(0);
                                copy->setArrayElementType(nullptr);
                                copy->setForceBoxedPrimitives(true);
                                AbstractMetaBuilder::decideUsagePattern(copy.data());
                                QString newIterator;
                                QTextStream s2(&newIterator);
                                s2 << "new ";
                                writeTypeInfo(s2, copy.data(), Option(ExcludeReference | ExcludeConst | SkipName));
                                s2 << "::" << iteratorType->iteratorName() << "(" << qt_name << ")";
                                writeQIteratorFunction(s, copy.data(), java_function->declaringClass()->templateBaseClassInstantiations().at(0), newIterator, option, iteratorType->iteratorName(), __jni_env, qtjambi_scope);
                                found = true;
                            }
                        }
                    }
                }
                if(!found){
                    if(AbstractMetaClass * iteratorClass = m_classes.findClass(iteratorType->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName)){
                        if(iteratorClass->typeAliasType()){
                            QScopedPointer<AbstractMetaType> typeAliasType(iteratorClass->typeAliasType()->copy());
                            if(typeAliasType->indirections().size()==1 && typeAliasType->getReferenceType()==AbstractMetaType::NoReference){
                                QList<bool> indirections = typeAliasType->indirections();
                                if(!indirections.isEmpty()){
                                    indirections.takeFirst();
                                    typeAliasType->setIndirections(indirections);
                                }
                                typeAliasType->setReferenceType(AbstractMetaType::Reference);
                                AbstractMetaBuilder::decideUsagePattern(typeAliasType.data());
                            }
                            QString newIterator;
                            QTextStream s2(&newIterator);
                            s2 << "new ";
                            writeTypeInfo(s2, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                            s2 << "(" << qt_name << ")";
                            writeQIteratorFunction(s, java_type, typeAliasType.data(), newIterator, option, "", __jni_env, qtjambi_scope);
                            found = true;
                        }else{
                            for(const AbstractMetaFunction *function : iteratorClass->functions()){
                                if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                                    QString newIterator;
                                    QTextStream s2(&newIterator);
                                    s2 << "new ";
                                    writeTypeInfo(s2, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                                    s2 << "(" << qt_name << ")";
                                    writeQIteratorFunction(s, java_type, function->type(), newIterator, option, "", __jni_env, qtjambi_scope);
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(!found){
                    s << "nullptr /*unable to create iterator*/";
                }
            } else {
                s << fromObject(java_type, qt_name, resetObjectAfterUse, __jni_env);
            }
        }
    }
    if(semiRequired){
        s << ";" << Qt::endl;
    }
}

void CppImplGenerator::writeQIteratorFunction(QTextStream &s,
                              const AbstractMetaType *java_type,
                              const AbstractMetaType *elementType,
                              const QString& variable,
                              Option option,
                              const QString& iteratorName,
                              const QString& __jni_env,
                              const QString& qtjambi_scope)
{
    if(qtjambi_scope=="__list_nativeId"){
        s << "qtjambi_to_QIterator(" << __jni_env << ", __list_nativeId, " << Qt::endl;
    }else{
        s << "qtjambi_to_QIterator(" << __jni_env << ", " << qtjambi_scope << ".relatedNativeID(), " << Qt::endl;
    }
    {
        INDENTATION(INDENT)
        s << INDENT << variable << "," << Qt::endl;
        s << INDENT << "[/*QIteratorValueFunction*/](JNIEnv *" << __jni_env << ", void* ptr) -> jobject {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "Q_UNUSED(" << __jni_env << ")" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, elementType, ForceConstReference);
            s << "_qt_result = *(*iterator);" << Qt::endl;
            writeQtToJava(s, elementType, "_qt_result", "", nullptr, nullptr, 0, Option((option & ~EnumAsInts) | DirectReturn | NoTmpVariable | BoxedPrimitive), __jni_env, qtjambi_scope);
            s << ";" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*QIteratorIncrementFunction*/](void* ptr) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT << "++(*iterator);" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*QIteratorDecrementFunction*/](void* ptr) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT << "--(*iterator);" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        bool skip = true;
        if(java_type->typeEntry()->type()==TypeEntry::ContainerType){
            const ContainerTypeEntry* container = static_cast<const ContainerTypeEntry*>(java_type->typeEntry());
            if(container->type()==ContainerTypeEntry::ListContainer
                    || container->type()==ContainerTypeEntry::StringListContainer
                    || container->type()==ContainerTypeEntry::ByteArrayListContainer
                    || container->type()==ContainerTypeEntry::VectorContainer
                    || container->type()==ContainerTypeEntry::QueueContainer
                    || container->type()==ContainerTypeEntry::StackContainer){
                skip = false;
            }else if(const AbstractMetaClass* containerClass = classes().findClass(container->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName)){
                if(const AbstractMetaClass* iteratorClass = containerClass->enclosedClasses().findClass(iteratorName, AbstractMetaClassList::SimpleName)){
                    if(iteratorClass->typeAliasType()){
                        if(!iteratorClass->typeAliasType()->indirections().isEmpty()
                                || !iteratorClass->queryFunctionsByOriginalName("operator<").isEmpty()){
                            skip = false;
                        }
                    }
                }
            }
        }else if(java_type->typeEntry()->type()==TypeEntry::IteratorType){
            const IteratorTypeEntry* iteratorTypeEntry = static_cast<const IteratorTypeEntry*>(java_type->typeEntry());
            if(iteratorTypeEntry->containerType()){
                if(const AbstractMetaClass* containerClass = classes().findClass(iteratorTypeEntry->containerType()->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName)){
                    if(const AbstractMetaClass* iteratorClass = containerClass ? containerClass->findIterator(iteratorTypeEntry) : nullptr){
                        if(iteratorClass->typeAliasType() && !iteratorClass->typeAliasType()->indirections().isEmpty()){
                            skip = false;
                        }
                    }
                }
            }
        }
        if(skip){
            s << INDENT << "nullptr /*QIteratorLessThanFunction missing operator < */," << Qt::endl;
        }else{
            s << INDENT << "[/*QIteratorLessThanFunction*/](void* ptr, void* ptr2) -> jboolean {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT;
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
                s << INDENT;
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator2 = static_cast<";
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr2);" << Qt::endl;
                s << INDENT << "return (*iterator)<(*iterator2);" << Qt::endl;
            }
            s << INDENT << "}," << Qt::endl;
        }
        s << INDENT << "[/*QIteratorEqualsFunction*/](void* ptr, void* ptr2) -> jboolean {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator2 = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr2);" << Qt::endl;
            s << INDENT << "return (*iterator)==(*iterator2);" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*PtrDeleterFunction*/](void* ptr) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT << "delete iterator;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl;
    }
    s << INDENT << ")";
}

void CppImplGenerator::writeQMapIteratorFunction(QTextStream &s,
                              const AbstractMetaType *java_type,
                              const AbstractMetaType *keyType,
                              const AbstractMetaType *valueType,
                              const QString& variable,
                              Option option,
                              const QString& iteratorName,
                              const QString& __jni_env,
                              const QString& qtjambi_scope)
{
    if(qtjambi_scope=="__list_nativeId"){
        s << "qtjambi_to_QMapIterator(" << __jni_env << ", __list_nativeId, " << Qt::endl;
    }else{
        s << "qtjambi_to_QMapIterator(" << __jni_env << ", " << qtjambi_scope << ".relatedNativeID(), " << Qt::endl;
    }
    {
        INDENTATION(INDENT)
        s << INDENT << variable << "," << Qt::endl;
        s << INDENT << "[/*QMapIteratorKeyFunction*/](JNIEnv *" << __jni_env << ", void* ptr) -> jobject {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "Q_UNUSED(" << __jni_env << ")" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, keyType, ForceConstReference);
            s << "_qt_result = iterator->key();" << Qt::endl;
            writeQtToJava(s, keyType, "_qt_result", "", nullptr, nullptr, 0, Option((option & ~EnumAsInts) | DirectReturn | NoTmpVariable | BoxedPrimitive), __jni_env, qtjambi_scope);
            s << ";" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*QMapIteratorValueFunction*/](JNIEnv *" << __jni_env << ", void* ptr) -> jobject {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << "Q_UNUSED(" << __jni_env << ")" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, valueType, ForceConstReference);
            s << "_qt_result = iterator->value();" << Qt::endl;
            writeQtToJava(s, valueType, "_qt_result", "", nullptr, nullptr, 0, Option((option & ~EnumAsInts) | DirectReturn | NoTmpVariable | BoxedPrimitive), __jni_env, qtjambi_scope);
            s << ";" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*QIteratorIncrementFunction*/](void* ptr) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT << "++(*iterator);" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*QIteratorDecrementFunction*/](void* ptr) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT << "--(*iterator);" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        bool skip = true;
        if(java_type->typeEntry()->type()==TypeEntry::ContainerType){
            const ContainerTypeEntry* container = static_cast<const ContainerTypeEntry*>(java_type->typeEntry());
            if(container->type()==ContainerTypeEntry::ListContainer
                    || container->type()==ContainerTypeEntry::StringListContainer
                    || container->type()==ContainerTypeEntry::ByteArrayListContainer
                    || container->type()==ContainerTypeEntry::VectorContainer
                    || container->type()==ContainerTypeEntry::QueueContainer
                    || container->type()==ContainerTypeEntry::StackContainer){
                skip = false;
            }else if(const AbstractMetaClass* containerClass = classes().findClass(container->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName)){
                if(const AbstractMetaClass* iteratorClass = containerClass->enclosedClasses().findClass(iteratorName, AbstractMetaClassList::SimpleName)){
                    if(iteratorClass->typeAliasType()){
                        if(!iteratorClass->typeAliasType()->indirections().isEmpty()
                                || !iteratorClass->queryFunctionsByOriginalName("operator<").isEmpty()){
                            skip = false;
                        }
                    }
                }
            }
        }else if(java_type->typeEntry()->type()==TypeEntry::IteratorType){
            const IteratorTypeEntry* iteratorTypeEntry = static_cast<const IteratorTypeEntry*>(java_type->typeEntry());
            if(const AbstractMetaClass* containerClass = classes().findClass(iteratorTypeEntry->containerType()->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName)){
                if(const AbstractMetaClass* iteratorClass = containerClass->findIterator(iteratorTypeEntry)){
                    if(iteratorClass->typeAliasType() && !iteratorClass->typeAliasType()->indirections().isEmpty()){
                        skip = false;
                    }
                }
            }
        }
        if(skip){
            s << INDENT << "nullptr /*QIteratorLessThanFunction missing operator < */," << Qt::endl;
        }else{
            s << INDENT << "[/*QIteratorLessThanFunction*/](void* ptr, void* ptr2) -> jboolean {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT;
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
                s << INDENT;
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator2 = static_cast<";
                writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
                s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr2);" << Qt::endl;
                s << INDENT << "return (*iterator)<(*iterator2);" << Qt::endl;
            }
            s << INDENT << "}," << Qt::endl;
        }
        s << INDENT << "[/*QIteratorEqualsFunction*/](void* ptr, void* ptr2) -> jboolean {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator2 = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr2);" << Qt::endl;
            s << INDENT << "return (*iterator)==(*iterator2);" << Qt::endl;
        }
        s << INDENT << "}," << Qt::endl;
        s << INDENT << "[/*PtrDeleterFunction*/](void* ptr) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "* iterator = static_cast<";
            writeTypeInfo(s, java_type, Option(ExcludeReference | ExcludeConst | SkipName));
            s << (iteratorName.isEmpty() ? "" : "::" + iteratorName) << "*>(ptr);" << Qt::endl;
            s << INDENT << "delete iterator;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl;
    }
    s << INDENT << ")";
}

void CppImplGenerator::writeJavaToQtContainer(QTextStream &s,
        const AbstractMetaType *java_type,
        const QString &qt_name,
        const QString &java_name,
        const AbstractMetaFunction *java_function,
        const AbstractMetaFunctional *java_functional,
        int argument_index,
        const QString& __jni_env,
        const QString& qtjambi_scope,
        Option option) {
    Q_UNUSED(qtjambi_scope)
    // Conversion to C++: Shell code for return value, native code for arguments
    TypeSystem::Language lang = argument_index == 0 ? TypeSystem::ShellCode : TypeSystem::NativeCode;
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

    QScopedPointer<AbstractMetaType> scopedPointer;
    const AbstractMetaType *container_type = java_type;
    if(java_type->indirections().size()==1){
        scopedPointer.reset(container_type->copy());
        container_type = scopedPointer.data();
        scopedPointer->setIndirections(QList<bool>());
        scopedPointer->setReferenceType(AbstractMetaType::Reference);
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
            || type->type() == ContainerTypeEntry::QArrayContainer
            || type->type() == ContainerTypeEntry::QVector2DArrayContainer
            || type->type() == ContainerTypeEntry::QVector3DArrayContainer
            || type->type() == ContainerTypeEntry::QVector4DArrayContainer
        ) {
        // the QArray type can have two template arguments
        const AbstractMetaType *targ;
        QScopedPointer<AbstractMetaType> instantiation;
        if(type->type() == ContainerTypeEntry::QArrayContainer){
            Q_ASSERT(java_type->instantiations().size() == 1 || java_type->instantiations().size() == 2);
            targ = java_type->instantiations().first();
        }else if(type->type() == ContainerTypeEntry::ByteArrayListContainer
                 || type->type() == ContainerTypeEntry::StringListContainer){
            Q_ASSERT_X(!java_type->hasInstantiationInCpp() || java_type->instantiations().size() == 0, qPrintable(java_type->cppSignature()), qPrintable(QString("instantiations: %1, [0] = %2").arg(java_type->instantiations().size()).arg(java_type->instantiations().at(0)->typeEntry()->qualifiedCppName())));
            instantiation.reset(java_type->copy());
            targ = instantiation.data();
            instantiation->setReferenceType(AbstractMetaType::NoReference);
            instantiation->setConstant(false);
            instantiation->setIndirections(QList<bool>());
            instantiation->setArrayElementCount(0);
            instantiation->setArrayElementType(nullptr);
            instantiation->setForceBoxedPrimitives(false);
            instantiation->setInstantiationInCpp(false);
            instantiation->setInstantiations(QList<const AbstractMetaType *>());
            instantiation->setOriginalTemplateType(nullptr);
            switch (type->type()) {
            case ContainerTypeEntry::ByteArrayListContainer:
                {
                    ComplexTypeEntry * qByteArrayTypeEntry = TypeDatabase::instance()->findComplexType("QByteArray");
                    Q_ASSERT(qByteArrayTypeEntry);
                    instantiation->setTypeEntry(qByteArrayTypeEntry);
                    instantiation->setOriginalTypeDescription("QByteArray");
                    instantiation->setTypeUsagePattern(AbstractMetaType::ValuePattern);
                }
                break;
            case ContainerTypeEntry::StringListContainer:
                {
                    ComplexTypeEntry * qStringTypeEntry = TypeDatabase::instance()->findComplexType("QString");
                    Q_ASSERT(qStringTypeEntry);
                    instantiation->setTypeEntry(qStringTypeEntry);
                    instantiation->setOriginalTypeDescription("QString");
                    instantiation->setTypeUsagePattern(AbstractMetaType::StringPattern);
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
                          || java_type->getReferenceType()==AbstractMetaType::Reference)
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
                    s << INDENT << "qtjambi_collection_clear(m_env, m_object);" << Qt::endl;
                    s << INDENT << "for(";
                    writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
                    s << "::const_iterator i = constBegin(); i!=constEnd(); i++){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        writeQtToJava(s, targ, "*i", "val", nullptr, nullptr, -1, Option(BoxedPrimitive), "m_env", "");
                        s << INDENT << "qtjambi_collection_add(m_env, m_object, val);" << Qt::endl;
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

        s << INDENT << "if (" << java_name << " != nullptr) {" << Qt::endl;
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
                    s << INDENT << "if (qtjambi_is_Q";
                    switch(type->type()){
                    case ContainerTypeEntry::VectorContainer: s << "Vector"; break;
                    case ContainerTypeEntry::StackContainer: s << "Stack"; break;
                    case ContainerTypeEntry::LinkedListContainer: s << "LinkedList"; break;
                    case ContainerTypeEntry::ByteArrayListContainer:
                    case ContainerTypeEntry::StringListContainer:
                    case ContainerTypeEntry::ListContainer:s << "List"; break;
                    case ContainerTypeEntry::SetContainer: s << "Set"; break;
                    default: s << "Queue"; break;
                    }
                    s << "(" << __jni_env << ", " << java_name << ", resolveClass(" << __jni_env << ", ";
                    if(targ->isPrimitive()){
                        s << "\""
                          << reinterpret_cast<const PrimitiveTypeEntry*>(targ->typeEntry())->javaObjectPackage().replace(".", "/") << "/"
                          << reinterpret_cast<const PrimitiveTypeEntry*>(targ->typeEntry())->javaObjectName()
                          << "\"";
                    }else{
                        s << "\"" << targ->package().replace(".", "/") << "/" << targ->name() << "\"";
                    }
                    s << "))) {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        if((option & OptionalScope) == OptionalScope){
                            if(argument_index==0){
                                s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << "*reinterpret_cast<";
                                    writeTypeInfo(s, container_type, ForceValueType);
                                    s << "*>(" << qt_name << ") = reinterpret_value_cast<";
                                    writeTypeInfo(s, container_type, ForceValueType);
                                    s << ">(qtjambi_to_object(" << __jni_env << ", " << java_name << "));" << Qt::endl;
                                }
                                s << INDENT << "}else{" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << qt_name << " = const_cast<";
                                    writeTypeInfo(s, container_type, ForceValueType);
                                    s << "*>(&reinterpret_value_cast<";
                                    writeTypeInfo(s, container_type, ForceValueType);
                                    s << ">(qtjambi_to_object(" << __jni_env << ", " << java_name << ")));" << Qt::endl;
                                }
                                s << INDENT << "}" << Qt::endl;
                            }else{
                                s << INDENT << qt_name << " = const_cast<";
                                writeTypeInfo(s, container_type, ForceValueType);
                                s << "*>(&reinterpret_value_cast<";
                                writeTypeInfo(s, container_type, ForceValueType);
                                s << ">(qtjambi_to_object(" << __jni_env << ", " << java_name << ")));" << Qt::endl;
                            }
                        }else{
                            if(needScope){
                                s << INDENT << qt_pointer << " = reinterpret_cast<";
                                writeTypeInfo(s, container_type, ForceValueType);
                                s << "*>";
                            }else{
                                s << INDENT << qt_name << " = ";
                                s << "reinterpret_value_cast<";
                                writeTypeInfo(s, container_type, ForceValueType);
                                s << ">";
                            }
                            s << "(qtjambi_to_object(" << __jni_env << ", " << java_name << "));" << Qt::endl;
                        }
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
                s << INDENT << "jobject __qt__iterator = qtjambi_collection_iterator(" << __jni_env << ", "
                  << java_name << ");" << Qt::endl;
                if((option & OptionalScope) == OptionalScope){
                    s << INDENT << "if(!" << qt_name << ") " << Qt::endl
                      << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
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
                    s << "reserve(qtjambi_collection_size(" << __jni_env << ", " << java_name << "));" << Qt::endl;
                }

                s << INDENT << "while(qtjambi_iterator_has_next(" << __jni_env << ", __qt__iterator)) {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT <<
                    "jobject __java_element = qtjambi_iterator_next(" << __jni_env << ", __qt__iterator);"
                      << Qt::endl;
                    QString element_name = (qt_name == "__qt_element") ? "__qt_element2" : "__qt_element";
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
        const QList<const AbstractMetaType *>& targs = java_type->instantiations();
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
                  << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
            }
            s << INDENT << "jobject __java_first = qtjambi_pair_get(" << __jni_env << ", "
            << java_name << ", 0);" << Qt::endl;
            writeJavaToQt(s, targs.at(0), "__qt_first", "__java_first", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);

            s << INDENT << "jobject __java_second = qtjambi_pair_get(" << __jni_env << ", "
            << java_name << ", 1);" << Qt::endl;
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
        const AbstractMetaType *targ_key = java_type->instantiations().at(0);
        const AbstractMetaType *targ_val = java_type->instantiations().at(1);

        bool needScope = (java_type->indirections().size()==1
                          || java_type->getReferenceType()==AbstractMetaType::Reference)
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
                    s << INDENT << "qtjambi_map_clear(m_env, m_object);" << Qt::endl;
                    s << INDENT << "for(";
                    writeTypeInfo(s, container_type, Option(ForceValueType | SkipName));
                    s << "::const_iterator i = constBegin(); i!=constEnd(); i++){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        writeQtToJava(s, targ_key, "i.key()", "key", nullptr, nullptr, -1, Option(BoxedPrimitive), "m_env", "");
                        writeQtToJava(s, targ_val, "i.value()", "val", nullptr, nullptr, -1, Option(BoxedPrimitive), "m_env", "");
                        s << INDENT << "qtjambi_map_put(m_env, m_object, key, val);" << Qt::endl;
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

            s << INDENT << "if (qtjambi_is_Q";
            switch(type->type()){
            case ContainerTypeEntry::MapContainer: s << "Map"; break;
            case ContainerTypeEntry::MultiMapContainer: s << "MultiMap"; break;
            case ContainerTypeEntry::MultiHashContainer: s << "MultiHash"; break;
            default: s << "Hash"; break;
            }
            s << "(" << __jni_env << ", " << java_name << ", resolveClass(" << __jni_env << ", ";
            if(targ_key->isPrimitive()){
                s << "\""
                  << reinterpret_cast<const PrimitiveTypeEntry*>(targ_key->typeEntry())->javaObjectPackage().replace(".", "/") << "/"
                  << reinterpret_cast<const PrimitiveTypeEntry*>(targ_key->typeEntry())->javaObjectName()
                  << "\"";
            }else{
                s << "\"" << targ_key->package().replace(".", "/") << "/" << targ_key->name() << "\"";
            }
            s << "), resolveClass(" << __jni_env << ", ";
            if(targ_val->isPrimitive()){
                s << "\""
                  << reinterpret_cast<const PrimitiveTypeEntry*>(targ_val->typeEntry())->javaObjectPackage().replace(".", "/") << "/"
                  << reinterpret_cast<const PrimitiveTypeEntry*>(targ_val->typeEntry())->javaObjectName()
                  << "\"";
            }else{
                s << "\"" << targ_val->package().replace(".", "/") << "/" << targ_val->name() << "\"";
            }
            s << "))) {" << Qt::endl;
            {
                INDENTATION(INDENT)
                if((option & OptionalScope) == OptionalScope){
                    if(argument_index==0){
                        s << INDENT << "if(" << qt_name << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT << "*reinterpret_cast<";
                            writeTypeInfo(s, container_type, ForceValueType);
                            s << "*>(" << qt_name << ") = reinterpret_value_cast<";
                            writeTypeInfo(s, container_type, ForceValueType);
                            s << ">(qtjambi_to_object(" << __jni_env << ", " << java_name << "));" << Qt::endl;
                        }
                        s << INDENT << "}else{" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT << qt_name << " = const_cast<";
                            writeTypeInfo(s, container_type, ForceValueType);
                            s << "*>(&reinterpret_value_cast<";
                            writeTypeInfo(s, container_type, ForceValueType);
                            s << ">(qtjambi_to_object(" << __jni_env << ", " << java_name << ")));" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else{
                        s << INDENT << qt_name << " = const_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "*>(&reinterpret_value_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << ">(qtjambi_to_object(" << __jni_env << ", " << java_name << ")));" << Qt::endl;
                    }
                }else{
                    if(needScope){
                        s << INDENT << qt_pointer << " = reinterpret_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << "*>";
                    }else{
                        s << INDENT << qt_name << " = reinterpret_value_cast<";
                        writeTypeInfo(s, container_type, ForceValueType);
                        s << ">";
                    }
                    s << "(qtjambi_to_object(" << __jni_env << ", " << java_name << "));" << Qt::endl;
                }
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
                      << INDENT << "    return ConvertResponse::NotSuccessful;" << Qt::endl;
                }
                s << INDENT
                  << "jobject __java_entry_set_iterator = qtjambi_map_entryset_iterator(" << __jni_env << ", " << java_name
                  << ");" << Qt::endl;

                s << INDENT << "while(qtjambi_iterator_has_next(" << __jni_env << ", __java_entry_set_iterator)) {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT
                      << "jobject __java_entry = "
                      << "qtjambi_iterator_next(" << __jni_env << ", __java_entry_set_iterator);"
                      << Qt::endl
                      << INDENT << "jobject __java_key = qtjambi_map$entry_key(" << __jni_env << ", __java_entry);" << Qt::endl
                      << INDENT << "jobject __java_val = qtjambi_map$entry_value(" << __jni_env << ", __java_entry);" << Qt::endl;
                    writeJavaToQt(s, targ_key, "__qt_key", "__java_key", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
                    writeJavaToQt(s, targ_val, "__qt_val", "__java_val", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
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
                    s << "insert(__qt_key, __qt_val);" << Qt::endl;
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
        const AbstractMetaType *java_type,
        const QString &qt_name,
        const QString &java_name,
        const AbstractMetaFunction *java_function,
        const AbstractMetaFunctional *java_functional,
        int argument_index,
        const QString& __jni_env,
        const QString& qtjambi_scope,
        Option option) {
    Q_UNUSED(qtjambi_scope)
    // Conversion to C++: Shell code for return value, native code for arguments
    TypeSystem::Language lang = argument_index == 0 ? TypeSystem::ShellCode : TypeSystem::NativeCode;
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

    /*bool needLambda = false;
    if((option & DirectReturn) == DirectReturn){
        needLambda = true;
    }else if((option & NoTmpVariable) == NoTmpVariable){
        if(java_name.isEmpty())
            needLambda = true;
    }*/

    Q_ASSERT(java_type->isInitializerList());
    Q_ASSERT(java_type->instantiations().size() == 1);

    const AbstractMetaType *targ = java_type->instantiations().first();

    QString vectorName = "tmp_list_" + QString::number(qHash(java_name), 16);
    s << INDENT << "QVector<";
    writeTypeInfo(s, targ, ForceValueType);
    s << "> " << vectorName << ";" << Qt::endl;

    s << INDENT << "if (" << java_name << "!= nullptr) {" << Qt::endl;
    {
        INDENTATION(INDENT)

        s << INDENT << "jsize __qt__size = " << __jni_env << "->GetArrayLength(jarray(" << java_name << "));" << Qt::endl;
        s << INDENT << "qtjambi_throw_java_exception(" << __jni_env << ")" << Qt::endl;

        s << INDENT << "if(__qt__size>0){" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT << vectorName << ".reserve(__qt__size);" << Qt::endl;

            if(targ->isPrimitive()){
                s << INDENT << "jboolean isCopy = false;" << Qt::endl;
                s << INDENT << targ->typeEntry()->jniName() << "* __native_array = " << __jni_env;
                s << "->Get" << jniTypeName(targ->typeEntry()->jniName()) << "ArrayElements(" << targ->typeEntry()->jniName() << "Array(" << java_name << "), &isCopy);" << Qt::endl;
                s << INDENT << "qtjambi_throw_java_exception(" << __jni_env << ")" << Qt::endl;
            }
            s << INDENT << "for (int i=0; i<__qt__size; ++i) {" << Qt::endl;
            {
                INDENTATION(INDENT)
                QString element_name = (qt_name == "__qt_element") ? "__qt_element2" : "__qt_element";
                if(targ->isPrimitive()){
                    s << INDENT << vectorName << " << static_cast<" << targ->typeEntry()->qualifiedCppName() << ">(__native_array[i]);" << Qt::endl;
                }else{
                    s << INDENT << "jobject __java_element = " << __jni_env << "->GetObjectArrayElement(jobjectArray(" << java_name << "), i);" << Qt::endl;
                    s << INDENT << "qtjambi_throw_java_exception(" << __jni_env << ")" << Qt::endl;
                    writeJavaToQt(s, targ, element_name, "__java_element", nullptr, nullptr, -1, BoxedPrimitive, __jni_env);
                    s << INDENT << vectorName << " << " << element_name << ";" << Qt::endl;
                }
            }
            s << INDENT << "}" << Qt::endl;
            if(targ->isPrimitive()){
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
        s << "#ifdef Q_OS_WIN32" << Qt::endl;
        s << INDENT << qt_name << " = std::initializer_list<";
        writeTypeInfo(s, targ, ForceValueType);
        s << ">(" << vectorName << ".begin(), " << vectorName << ".end());" << Qt::endl;
        s << "#else" << Qt::endl;
        s << INDENT << "struct{" << Qt::endl;
        {
            INDENTATION(INDENT)
            s << INDENT;
            writeTypeInfo(s, targ, ForceValueType);
            s << "* _M_array;" << Qt::endl;
            s << INDENT << "size_t _M_len;" << Qt::endl;
        }
        s << INDENT << "} " << java_name << "__initializer_tmp;" << Qt::endl;
        s << INDENT << java_name << "__initializer_tmp._M_array = " << vectorName << ".begin();" << Qt::endl;
        s << INDENT << java_name << "__initializer_tmp._M_len = (size_t)" << vectorName << ".size();" << Qt::endl;
        s << INDENT << qt_name << " = *((std::initializer_list<";
        writeTypeInfo(s, targ, ForceValueType);
        s << ">*) &" << java_name << "__initializer_tmp);" << Qt::endl;
        s << "#endif" << Qt::endl;
    }
    s << INDENT << "}" << Qt::endl;
}


void CppImplGenerator::writeFunctionCall(QTextStream &s, const QString &object_name,
        const AbstractMetaFunction *java_function,
        QString prefix,
        Option option,
        const QStringList &extra_arguments) {
    QString function_name = option & OriginalName ? java_function->originalName() : java_function->name();

    if(!java_function->templateParameters().isEmpty()){
        function_name += "<";
        for(int i=0; i<java_function->templateParameters().size(); i++){
            AbstractMetaTemplateParameter* param = java_function->templateParameters()[i];
            if(param->type()){
                QTextStream s2(&function_name);
                writeTypeInfo(s2, param->type());
            }else{
                function_name += param->instantiation();
            }
            if(i<java_function->templateParameters().size()-1){
                function_name += ", ";
            }
        }
        function_name += " >";
    }

    //AbstractMetaClassList interfaces = java_function->implementingClass()->interfaces();

    if (prefix.isEmpty()
            && !java_function->implementingClass()->interfaces().isEmpty()
            && !java_function->implementingClass()->inheritsFrom(java_function->declaringClass())) {
        prefix = java_function->declaringClass()->qualifiedCppName() + "::";
    }

    // Global scope stream operators need the arguments to be reordered (this ref at end)
    // so we special case them in order to simplify this code
    bool stream_operator = java_function->originalName() == "operator<<"
                            || java_function->originalName() == "operator>>";
    if (java_function->isInGlobalScope() && stream_operator) {
        if (!java_function->type())
            s << "if (" << object_name << " != nullptr) ";
        else
            s << "(" << object_name << " != nullptr) ? ";
        if(!prefix.isEmpty())
            s << "::" << prefix;
        s << function_name << "(";
        if (!stream_operator)
            s << "*" << object_name << ", ";
        writeFunctionCallArguments(s, java_function, "__qt_", option);
        if (stream_operator)
            s << ", *" << object_name;
        s << ")";
        if (java_function->type()!= nullptr)
            s << " : " << default_return_statement_qt(java_function->type(), Generator::Option(option | Generator::NoReturnStatement));
    } else {
        bool noPar = false;
        bool additionalClosingPar = false;
        if(prefix.isEmpty() && !java_function->isStatic() && (extra_arguments.size() + java_function->arguments().size())<=1){
            if(function_name=="operator++"){
                if((extra_arguments.size() + java_function->arguments().size())==0)
                    s << "++(*" << object_name << ")";
                else if((extra_arguments.size() + java_function->arguments().size())==1){
                    s << "(*" << object_name << ")++";
                    return;
                }
                noPar = true;
            }else if(function_name=="operator--"){
                if((extra_arguments.size() + java_function->arguments().size())==0)
                    s << "--(*" << object_name << ")";
                else if((extra_arguments.size() + java_function->arguments().size())==1){
                    s << "(*" << object_name << ")--";
                    return;
                }
                noPar = true;
            }else if(function_name=="operator*"){
                if((extra_arguments.size() + java_function->arguments().size())==0)
                    s << "*(*" << object_name << ")";
                else
                    s << "(*" << object_name << ") * ";
                noPar = true;
            }else if(function_name=="operator+"){
                if((extra_arguments.size() + java_function->arguments().size())==0)
                    s << "+(*" << object_name << ")";
                else
                    s << "(*" << object_name << ") + ";
                noPar = true;
            }else if(function_name=="operator-"){
                if((extra_arguments.size() + java_function->arguments().size())==0)
                    s << "-(*" << object_name << ")";
                else
                    s << "(*" << object_name << ") - ";
                noPar = true;
            }else if(function_name=="operator/"){
                s << "(*" << object_name << ") / ";
                noPar = true;
            }else if(function_name=="operator<"){
                s << "(*" << object_name << ") < ";
                noPar = true;
            }else if(function_name=="operator>"){
                s << "(*" << object_name << ") > ";
                noPar = true;
            }else if(function_name=="operator<="){
                s << "(*" << object_name << ") <= ";
                noPar = true;
            }else if(function_name=="operator>="){
                s << "(*" << object_name << ") >= ";
                noPar = true;
            }else if(function_name=="operator%"){
                s << "(*" << object_name << ") % ";
                noPar = true;
            }else if(function_name=="operator^"){
                s << "(*" << object_name << ") ^ ";
                noPar = true;
            }else if(function_name=="operator~"){
                if((extra_arguments.size() + java_function->arguments().size())==0)
                    s << "~(*" << object_name << ")";
                else s << "(*" << object_name << ") ~ ";
                noPar = true;
            }else if(function_name=="operator!"){
                    s << "!(*" << object_name << ")";
                noPar = true;
            }else if(function_name=="operator&"){
                s << "(*" << object_name << ") & ";
                noPar = true;
            }else if(function_name=="operator|"){
                s << "(*" << object_name << ") | ";
                noPar = true;
            }else if(function_name=="operator&&"){
                s << "(*" << object_name << ") && ";
                noPar = true;
            }else if(function_name=="operator||"){
                s << "(*" << object_name << ") || ";
                noPar = true;
            }else if(function_name=="operator/="){
                s << "(*" << object_name << ") /= ";
                noPar = true;
            }else if(function_name=="operator-="){
                s << "(*" << object_name << ") -= ";
                noPar = true;
            }else if(function_name=="operator+="){
                s << "(*" << object_name << ") += ";
                noPar = true;
            }else if(function_name=="operator*="){
                s << "(*" << object_name << ") *= ";
                noPar = true;
            }else if(function_name=="operator~="){
                s << "(*" << object_name << ") ~= ";
                noPar = true;
            }else if(function_name=="operator^="){
                s << "(*" << object_name << ") ^= ";
                noPar = true;
            }else if(function_name=="operator|="){
                s << "(*" << object_name << ") |= ";
                noPar = true;
            }else if(function_name=="operator%="){
                s << "(*" << object_name << ") %= ";
                noPar = true;
            }else if(function_name=="operator&="){
                s << "(*" << object_name << ") &= ";
                noPar = true;
            }else if(function_name=="operator=="){
                s << "(*" << object_name << ") == ";
                noPar = true;
            }else if(function_name=="operator="){
                s << "((*" << object_name << ") = ";
                noPar = true;
                additionalClosingPar = true;
            }
        }
        if(!noPar){
            s << object_name;
            if(java_function->isStatic())
                s << "::" << prefix;
            else
                s << "->" << prefix;
            s << function_name << "(";
        }
        if(!java_function->arguments().isEmpty())
            writeFunctionCallArguments(s, java_function, "__qt_", option);

        // The extra arguments...
        bool hasArgs = java_function->arguments().size() != 0;
        for (int i = 0; i < extra_arguments.size(); ++i) {
            if (i > 0 || hasArgs)
                s << ", ";
            s << extra_arguments.at(i);
        }
        if(!noPar || additionalClosingPar){
            s << ")";
        }
    }
}

void CppImplGenerator::writeFunctionCallArguments(QTextStream &s, const AbstractMetaFunction *java_function,
                                const QString &prefix, Option options){
    writeFunctionCallArguments(s, java_function,
                                [=]()->QList<const AbstractMetaArgument*> {
                                    QList<const AbstractMetaArgument*> result;
                                    const AbstractMetaArgumentList& arguments = java_function->arguments();
                                    for (int i = 0; i < arguments.size(); ++i) {
                                        const AbstractMetaArgument *argument = arguments.at(i);
                                        if ((options & SkipRemovedArguments) == SkipRemovedArguments
                                                && java_function->argumentRemoved(argument->argumentIndex() + 1)!=ArgumentRemove_No) {
                                            if(!java_function->isConstructor()
                                                    || ( !argument->originalDefaultValueExpression().isEmpty()
                                                         && java_function->conversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1).isEmpty()
                                                         && java_function->conversionRule(TypeSystem::TargetLangCode, argument->argumentIndex() + 1).isEmpty() )){
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
        const AbstractMetaFunction *java_function,
        const QList<const AbstractMetaArgument*>& arguments,
        const QString &prefix,
        Option options) {
    Q_ASSERT(java_function);
    int written_arguments = 0;
    for (int i = 0; i < arguments.size(); ++i) {
        if(const AbstractMetaArgument *argument = arguments.at(i)){
            if ((options & SkipRemovedArguments) == SkipRemovedArguments
                    && java_function->argumentRemoved(argument->argumentIndex() + 1)!=ArgumentRemove_No
                    && !java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)
                    && !argument->originalDefaultValueExpression().isEmpty()) {
                continue;
            }
            if (argument->type()->typeEntry()->isObject() && argument->type()->actualIndirections() == 0
                    && !java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
                ReportHandler::warning(QString("Object type '%1' passed as value. Resulting code will not compile.  %2::%3")
                                       .arg(argument->type()->cppSignature())
                                       .arg(java_function->ownerClass()->fullName())
                                       .arg(java_function->signature()));
            }
            if(argument->type()->isNativePointer()){
                ReportHandler::debugFull(QString("native pointer pattern for '%1'  %2::%3")
                                         .arg(argument->type()->cppSignature())
                                         .arg(java_function->ownerClass()->fullName())
                                         .arg(java_function->signature()));
            }

            if (written_arguments++ > 0) {
                s << ", ";
            }

            bool enum_as_int = (options & EnumAsInts) && (argument->type()->typeEntry()->isEnum()
                               || argument->type()->typeEntry()->isFlags());
            bool enum_from_int = (options & EnumFromInt) && (argument->type()->typeEntry()->isEnum()
                               || argument->type()->typeEntry()->isFlags());
            if (argument->type()->isEnum()) {
                AbstractMetaEnum *java_enum =
                    m_classes.findEnum(static_cast<const EnumTypeEntry *>(argument->type()->typeEntry()));
                if (!java_enum) {
                    ReportHandler::warning(QString("enum not found: '%1'")
                                           .arg(argument->type()->typeEntry()->qualifiedCppName()));
                } else {
                    enum_as_int |= !java_enum->isPublic();
                }
            }

            bool paren = false;
            if(enum_from_int && argument->type()->indirections().isEmpty()){
                writeTypeInfo(s, argument->type(), Option(SkipName | ForceValueType));
                s << "(";
                paren = true;
            }else if ((!(options & NoCasts) && !enum_as_int) || ((options & ForceEnumCast) && argument->type()->isEnum())) {

                // If the type in the signature is specified without template instantiation, but the
                // class is actually a template class, then we have troubles.
                if ( argument->type()->indirections().isEmpty()
                     && (argument->type()->getReferenceType()!=AbstractMetaType::Reference || argument->type()->isConstant())
                     && (argument->type()->isEnum()
                      || argument->type()->isPrimitive()
                      || argument->type()->isFlags())
                    ) {
                    if(argument->type()->isPrimitive())
                        s << "static_cast<";
                    writeTypeInfo(s, argument->type(), Option(SkipName | ForceValueType));
                    if(argument->type()->isPrimitive())
                        s << ">";
                    s << "(";
                    paren = true;
                }
            }

            if (!argument->type()->isPrimitive()
                    || java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
                s << prefix;
            }
            s << argument->indexedName();
            if(paren)
                s << ")";
        }
    }
}

void CppImplGenerator::writeFunctionCallArguments(QTextStream &s,
        const AbstractMetaFunctional *java_function,
        const QString &prefix,
        Option options) {
    const AbstractMetaArgumentList& arguments = java_function->arguments();

    int written_arguments = 0;
    for (int i = 0; i < arguments.size(); ++i) {
        const AbstractMetaArgument *argument = arguments.at(i);

        if ((options & SkipRemovedArguments) == SkipRemovedArguments
                && java_function->argumentRemoved(i + 1)!=ArgumentRemove_No) {
            continue;
        }

        if (argument->type()->typeEntry()->isObject() && argument->type()->actualIndirections() == 0
                && !java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)) {
            ReportHandler::warning(QString("Object type '%1' passed as value. Resulting code will not compile.  %2")
                                   .arg(argument->type()->cppSignature())
                                   .arg(java_function->fullName()));
        }
        if(argument->type()->isNativePointer()){
            ReportHandler::debugFull(QString("native pointer pattern for '%1'  %2")
                                     .arg(argument->type()->cppSignature())
                                     .arg(java_function->fullName()));
        }

        if (written_arguments++ > 0) {
            s << ", ";
        }

        bool enum_as_int = (options & EnumAsInts) && (argument->type()->typeEntry()->isEnum()
                           || argument->type()->typeEntry()->isFlags());
        if (argument->type()->isEnum()) {
            AbstractMetaEnum *java_enum =
                m_classes.findEnum(static_cast<const EnumTypeEntry *>(argument->type()->typeEntry()));
            if (!java_enum) {
                ReportHandler::warning(QString("enum not found: '%1'")
                                       .arg(argument->type()->typeEntry()->qualifiedCppName()));
            } else {
                enum_as_int |= !java_enum->isPublic();
            }
        }

        bool paren = false;
        if ((!(options & NoCasts) && !enum_as_int) || ((options & ForceEnumCast) && argument->type()->isEnum())) {

            // If the type in the signature is specified without template instantiation, but the
            // class is actually a template class, then we have troubles.
            if ( argument->type()->indirections().isEmpty()
                 && (argument->type()->getReferenceType()!=AbstractMetaType::Reference || argument->type()->isConstant())
                 && (argument->type()->isEnum()
                  || argument->type()->isPrimitive()
                  || argument->type()->isFlags())
                ) {
                if(argument->type()->isPrimitive())
                    s << "static_cast<";
                writeTypeInfo(s, argument->type(), Option(SkipName | ForceValueType));
                if(argument->type()->isPrimitive())
                    s << ">";
                s << "(";
                paren = true;
            }
        }

        if (argument->type()->isPointerContainer() && prefix=="__qt_") {
            const PointerContainerTypeEntry* pentry = static_cast<const PointerContainerTypeEntry*>(argument->type()->typeEntry());
            switch(pentry->type()){
            case PointerContainerTypeEntry::unique_ptr:
                Q_ASSERT(argument->type()->instantiations().size()>0);
                s << "std::unique_ptr<";
                writeTypeInfo(s, argument->type()->instantiations()[0], SkipName);
                s << ">(";
                paren = true;
                break;
            case PointerContainerTypeEntry::QScopedPointer:
                Q_ASSERT(argument->type()->instantiations().size()>0);
                s << "QScopedPointer<";
                writeTypeInfo(s, argument->type()->instantiations()[0], SkipName);
                s << ">(";
                paren = true;
                break;
            default:
                break;
            }
        }

        if (!argument->type()->isPrimitive()
                || java_function->hasConversionRule(TypeSystem::NativeCode, argument->argumentIndex() + 1)
            ) {
            s << prefix;
        }
        s << argument->indexedName();
        if(paren)
            s << ")";
    }
}


QString CppImplGenerator::translateType(const AbstractMetaType *java_type, Option option) {
    if (!java_type)
        return "void";

    if (java_type->hasNativeId() && (option & UseNativeIds)){
        return "QtJambiNativeID";
    }else if (java_type->isPrimitive()
            || java_type->isTargetLangString()
            || java_type->isTargetLangStringRef()
            || java_type->isVariant()
            || java_type->isJObjectWrapper()
            || java_type->isJMapWrapper()
            || java_type->isJCollectionWrapper()
            || java_type->isJEnumWrapper()
            || java_type->isJIteratorWrapper()
            || java_type->isJQFlagsWrapper()
            || java_type->isTargetLangChar()) {
        return java_type->typeEntry()->jniName();
    } else if (java_type->isArray()){
        if(java_type->typeEntry()->isArray())
            return java_type->typeEntry()->jniName();
        else{
            QString jniName = java_type->typeEntry()->jniName();
            if(jniName=="void")
                jniName = "jobject";
            return jniName + "Array";
        }
    } else if (java_type->isInitializerList()){
        Q_ASSERT(java_type->instantiations().size()==1);
        if(java_type->instantiations().first()->isPrimitive()){
            return java_type->instantiations().first()->typeEntry()->jniName()+"Array";
        }else{
            return java_type->typeEntry()->jniName();
        }
    } else if (java_type->isIntegerEnum() || java_type->isIntegerFlags()
               || ((option & EnumAsInts) && (java_type->isEnum() || java_type->isFlags()))) {
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
    } else if (java_type->isTargetLangString()
               || java_type->isTargetLangStringRef()
               || java_type->isTargetLangStringView()
               || java_type->isTargetLangLatin1String()
               || (java_type->typeEntry()->qualifiedTargetLangName()=="java.lang.String" && java_type->indirections().isEmpty())){
        return "jstring";
    } else {
        return "jobject";
    }
}

void CppImplGenerator::writeExtraIncludes(QTextStream &s, const AbstractMetaClass *java_class, QSet<QString>& dedupe) {
    IncludeList includes = java_class->typeEntry()->extraIncludes();
    if(java_class->typeEntry()->designatedInterface()){
        includes << java_class->typeEntry()->designatedInterface()->extraIncludes();
    }
    QSet<const AbstractMetaClass *> classes;
    const AbstractMetaClass *_cls = java_class;
    while(_cls){
        classes << _cls;
        for(const AbstractMetaClass *ifc : _cls->interfaces()){
            classes << ifc;
        }
        _cls = _cls->baseClass();
    }
    classes.remove(java_class);
    for(const AbstractMetaClass *cls : classes){
        for(const Include& inc : cls->typeEntry()->extraIncludes()){
            if(inc.inherited){
                includes << inc;
            }
        }
        if(cls->typeEntry()->designatedInterface()){
            for(const Include& inc : cls->typeEntry()->designatedInterface()->extraIncludes()){
                if(inc.inherited){
                    includes << inc;
                }
            }
        }
    }
    std::sort(includes.begin(), includes.end());

    int used = dedupe.size();
    for(const Include &i : includes) {
        if (i.type != Include::TargetLangImport) {
            writeInclude(s, i, dedupe);
        }
    }

    if (used<dedupe.size())
        s << Qt::endl;
}

void CppImplGenerator::writeExtraIncludes(QTextStream &s, const AbstractMetaFunctional *java_class, QSet<QString>& dedupe) {
    IncludeList includes = java_class->typeEntry()->extraIncludes();
    std::sort(includes.begin(), includes.end());

    int used = dedupe.size();
    for(const Include &i : includes) {
        if (i.type != Include::TargetLangImport) {
            writeInclude(s, i, dedupe);
        }
    }

    if (used<dedupe.size())
        s << Qt::endl;
}

void CppImplGenerator::writeTypeConversion(QTextStream &s, const AbstractMetaFunction *function, AbstractMetaType *type, int index, const QString& metaTypeId, const QString& parameterType){
    s << INDENT << "infos << ParameterTypeInfo(" << Qt::endl;
    {
        INDENTATION(INDENT)
        {
            INDENTATION(INDENT)
            {
                INDENTATION(INDENT)
                s << INDENT << "// ";
                writeTypeInfo(s, type, Option(SkipName));
                s << Qt::endl
                  << INDENT << "env," << Qt::endl
                  << INDENT << metaTypeId << "," << Qt::endl;
                s << INDENT << parameterType << "," << Qt::endl;
                s << INDENT << "\"";
                s << type->typeEntry()->qualifiedTargetLangJNIName() << "\"," << Qt::endl;
                s << INDENT << "[](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* out, bool";
                if(type->typeEntry()->isPrimitive()){
                    s << " forceBoxedType";
                }
                s << ")->bool{" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "Q_UNUSED(env)" << Qt::endl;
                    s << INDENT << "Q_ASSERT(in);" << Qt::endl;
                    s << INDENT;
                    writeTypeInfo(s, type, Option(SkipName | ExcludeReference));
                    if(type->indirections().isEmpty())
                        s << "&";
                    s << "_in = *reinterpret_cast<";
                    writeTypeInfo(s, type, Option(SkipName | ExcludeReference));
                    s << " *>(const_cast<void*>(in));" << Qt::endl;
                    QString outArg;
                    QString outBoxing;
                    if(type->typeEntry()->isPrimitive()){
                        QString qualifiedTargetLangName = type->typeEntry()->qualifiedTargetLangName();
                        if(qualifiedTargetLangName=="boolean"){
                            outArg = "out->z";
                            outBoxing = "out->l = qtjambi_from_boolean(env, out->z);";
                        }else if(qualifiedTargetLangName=="byte"){
                            outArg = "out->b";
                            outBoxing = "out->l = qtjambi_from_byte(env, out->b);";
                        }else if(qualifiedTargetLangName=="short"){
                            outArg = "out->s";
                            outBoxing = "out->l = qtjambi_from_short(env, out->s);";
                        }else if(qualifiedTargetLangName=="int"){
                            outArg = "out->i";
                            outBoxing = "out->l = qtjambi_from_int(env, out->i);";
                        }else if(qualifiedTargetLangName=="long"){
                            outArg = "out->j";
                            outBoxing = "out->l = qtjambi_from_long(env, out->j);";
                        }else if(qualifiedTargetLangName=="char"){
                            outArg = "out->c";
                            outBoxing = "out->l = qtjambi_from_char(env, out->c);";
                        }else if(qualifiedTargetLangName=="float"){
                            outArg = "out->f";
                            outBoxing = "out->l = qtjambi_from_float(env, out->f);";
                        }else if(qualifiedTargetLangName=="double"){
                            outArg = "out->d";
                            outBoxing = "out->l = qtjambi_from_double(env, out->d);";
                        }else{
                            outArg = "out->l";
                        }
                    }else{
                        outArg = "out->l";
                    }
                    QString scopedConvert1;
                    {
                        QTextStream _s(&scopedConvert1);
                        writeQtToJava(_s,
                                  type,
                                  QString("_in"),
                                  outArg,
                                  function, nullptr,
                                  index,
                                  Option(NoTmpVariable),
                                  "env",
                                  "(*scope)");
                    }
                    QString scopedConvert2;
                    {
                        QTextStream _s(&scopedConvert2);
                        writeQtToJava(_s,
                                  type,
                                  QString("_in"),
                                  outArg,
                                  function, nullptr,
                                  index,
                                  Option(NoTmpVariable),
                                  "env",
                                  "");
                    }
                    bool assertedScope = false;
                    if(scopedConvert1==scopedConvert2){
                        if(scopedConvert1.contains("(*scope)")){
                            s << INDENT << "Q_ASSERT(scope);" << Qt::endl;
                            assertedScope = true;
                        }else if(!function->resetObjectAfterUse(index)){
                            s << INDENT << "Q_UNUSED(scope)" << Qt::endl;
                        }
                        s << scopedConvert1;
                    }else if(scopedConvert1.contains("(*scope)")){
                        s << INDENT << "Q_ASSERT(scope);" << Qt::endl;
                        s << scopedConvert1;
                    }else{
                        s << INDENT << "if(scope){";
                        for(QStringRef line : scopedConvert1.splitRef("\n")){
                            if(!line.trimmed().isEmpty())
                                s << Qt::endl << "    " << line;
                        }
                        s << Qt::endl << INDENT << "}else{";
                        for(QStringRef line : scopedConvert2.splitRef("\n")){
                            if(!line.trimmed().isEmpty())
                                s << Qt::endl << "    " << line;
                        }
                        s << Qt::endl << INDENT << "}" << Qt::endl;
                    }
                    if(!outBoxing.isEmpty()){
                        s << INDENT << "if(forceBoxedType)" << Qt::endl;
                        s << INDENT << "    " << outBoxing << Qt::endl;
                    }
                    if (function->resetObjectAfterUse(index)){
                        if(!assertedScope)
                            s << INDENT << "Q_ASSERT(scope);" << Qt::endl;
                        if (type->isContainer()){
                            s << INDENT << "InvalidateContainerAfterUse* __invalidate_out_after_use = new InvalidateContainerAfterUse(env, out->l);" << Qt::endl;
                        }else if (type->isArray()){
                            s << INDENT << "InvalidateArrayAfterUse* __invalidate_out_after_use = new InvalidateArrayAfterUse(env, out->l);" << Qt::endl;
                        }else{
                            s << INDENT << "InvalidateAfterUse* __invalidate_out_after_use = new InvalidateAfterUse(env, out->l);" << Qt::endl;
                        }
                        s << INDENT << "scope->addFinalAction([__invalidate_out_after_use](){ delete __invalidate_out_after_use; });" << Qt::endl;
                    }
                    s << INDENT << "return true;" << Qt::endl;
                }
                s << INDENT << "}," << Qt::endl;
                s << INDENT << "[](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType";
                if(type->typeEntry()->isPrimitive()){
                    s << " valueType";
                }
                s << ") -> ConvertResponse {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "Q_UNUSED(env)" << Qt::endl;
                    QString javaIn = QLatin1String("in.l");
                    if(type->typeEntry()->isPrimitive()){
                        QString qualifiedTargetLangName = type->typeEntry()->qualifiedTargetLangName();
                        s << INDENT << translateType(type) << " _in;" << Qt::endl;
                        s << INDENT << "switch(valueType){" << Qt::endl;
                        s << INDENT << "case jValueType::l:" << Qt::endl;
                        s << INDENT << "    _in = qtjambi_to_" << qualifiedTargetLangName<< "(env, in.l);" << Qt::endl;
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
                    QScopedPointer<AbstractMetaType> argumentType(type->copy());
                    QScopedPointer<AbstractMetaType> ptrToArgumentType(type->copy());
                    {
                        QList<bool> indirections = ptrToArgumentType->indirections();
                        indirections << false;
                        ptrToArgumentType->setIndirections( indirections );
                    }

                    QString scopedConvert;
                    {
                        QTextStream _s(&scopedConvert);
                        writeJavaToQt(_s,
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
                    if(scopedConvert.contains("scope"))
                        s << INDENT << "Q_ASSERT(scope);" << Qt::endl;
                    else
                        s << INDENT << "Q_UNUSED(scope)" << Qt::endl;
                    s << scopedConvert;
                    s << INDENT << "return ConvertResponse::Successful;" << Qt::endl;
                }
                s << INDENT << "}";
            }
        }
    }
    s << ");" << Qt::endl;
}

void CppImplGenerator::writeArgumentConversion(QTextStream &s, const AbstractMetaFunction *function){
    AbstractMetaArgumentList arguments;
    for (int i = 0; i < function->arguments().size(); ++i) {
        if(!function->argumentRemoved(i+1)){
            arguments << function->arguments()[i];
        }
    }
    if(!arguments.isEmpty())
        s << INDENT << "QList<QByteArray> parameterTypes = method.parameterTypes();" << Qt::endl;
    if(function->type()){
        writeTypeConversion(s,
                            function,
                            function->type(),
                            0,
                            "method.returnType()",
                            "method.typeName()"
                        );
    }else{
        s << INDENT << "infos << ParameterTypeInfo::voidTypeInfo(env);" << Qt::endl;
    }
    for(int i=0; i<arguments.size(); ++i){
        AbstractMetaArgument* argument = arguments[i];
        writeTypeConversion(s,
                            function,
                            argument->type(),
                            argument->argumentIndex()+1,
                            "method.parameterType(" + QString::number(i) + ")",
                            "parameterTypes[" + QString::number(i) + "]"
                        );
    }
}

void CppImplGenerator::writeMetaInfo(QTextStream &s, const AbstractMetaClass *cls,
                                     const QMultiMap<int,AbstractMetaFunction *>&,
                                     const QList<const AbstractMetaFunction *> &signalsInTargetLang,
                                     const QList<QString>& signalNames,
                                     const QMap<QString,QList<const AbstractMetaFunction*>>& sortedSignals,
                                     bool isInterface){
    Q_UNUSED(isInterface)
    if (!cls)
        return;
    const ComplexTypeEntry *entry = cls->typeEntry();
    if (entry->codeGeneration() == TypeEntry::GenerateForSubclass || entry->isIterator())
        return;

    s << "void initialize_meta_info_" << cls->typeEntry()->qualifiedCppName().replace("::", "_").replace("<", "_").replace(">", "_") << "(){" << Qt::endl;

    {
        INDENTATION(INDENT)

        if(!cls->typeEntry()->ppCondition().isEmpty()){
            s << Qt::endl << "#if " << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
        }
        writeCodeInjections(s, cls->typeEntry(), CodeSnip::Beginning, TypeSystem::MetaInfo);

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
        if(cls->isNamespace()){
        }else if(!cls->isFake()){
            bool isInterface = false;
            if (!entry->isInterface()){ //  ?? && cls->hasStandardConstructor() ??
                for(AbstractMetaClass *iface : cls->interfaces()){
                    const InterfaceTypeEntry *ientry = static_cast<const InterfaceTypeEntry *>(iface->typeEntry());
                    if(ientry->origin()==cls->typeEntry()){
                        QString interfaceName = ientry->qualifiedTargetLangName().replace(".", "/");
                        if(cls->typeEntry()->isValue()){
                            s << INDENT << "const std::type_info& typeId = registerInterfaceValueTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\", qobject_interface_iid<" << qtName << "*>());" << Qt::endl;
                        }else{
                            s << INDENT << "const std::type_info& typeId = registerInterfaceTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\", qobject_interface_iid<" << qtName << "*>());" << Qt::endl;
                        }
                        isInterface = true;
                        break;
                    }
                }
            }
            if(!isInterface){
                if(cls->isQObject()){
                    s << INDENT << "const std::type_info& typeId = registerQObjectTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl;
                    static const AbstractMetaClass *mediaControlClass = m_classes.findClass("io.qt.multimedia.QMediaControl", AbstractMetaClassList::FullName);
                    if(mediaControlClass && cls->inheritsFrom(mediaControlClass)){
                        s << INDENT << "registerMediaControlInfo(typeId, qmediacontrol_iid<" << qtName << "*>());" << Qt::endl;
                    }
                }else if(cls->typeEntry()->isValue()){
                    s << INDENT << "const std::type_info& typeId = registerValueTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl
                      << INDENT << "Q_UNUSED(typeId)" << Qt::endl;
                }else{
                    s << INDENT << "const std::type_info& typeId = registerObjectTypeInfo<" << qtName << ">(\"" << qtName << "\", \"" << javaTypeName << "\");" << Qt::endl;
                }
            }
            writeCodeInjections(s, cls->typeEntry(), CodeSnip::Position1, TypeSystem::MetaInfo);

            QString constructorName = entry->customConstructor().name.isEmpty() ? QString() : "custom_constructor_" + entry->qualifiedCppName().replace("::", "_");
            QString destructorName = entry->customDestructor().name.isEmpty() ? QString() : "custom_destructor_" + entry->qualifiedCppName().replace("::", "_");

            if (!constructorName.isEmpty() && destructorName.isEmpty()){
                destructorName = "QtMetaTypePrivate::QMetaTypeFunctionHelper<" + entry->qualifiedCppName() + ">::Destruct";
            }else if (!destructorName.isEmpty() && constructorName.isEmpty()){
                constructorName = "QtMetaTypePrivate::QMetaTypeFunctionHelper<" + entry->qualifiedCppName() + ">::Construct";
            }


            QString javaPackage = entry->javaPackage();

            QString javaName =  entry->lookupName();

            if (!javaPackage.isEmpty()) {
                javaName.prepend(javaPackage.replace(".", "/") + "/");
            }

            bool usedTypeID = false;
            if (!cls->isQObject()) {
                if (entry->typeFlags() & ComplexTypeEntry::DeleteInMainThread){
                    s << INDENT << "registerOwnerFunction(typeId, &qtjambi_main_thread_owner);" << Qt::endl;
                    usedTypeID = true;
                }
                if (entry->typeFlags() & ComplexTypeEntry::DeleteInOwnerThread){
                    s << INDENT << "registerOwnerFunction(typeId, &owner_function_" << entry->qualifiedCppName().replace("::", "_") << ");" << Qt::endl;
                    usedTypeID = true;
                }
            }

            s << "// BEGIN: function table" << Qt::endl;

            if(!cls->isInterface()){
                AbstractMetaFunctionList virtual_functions = cls->implementableFunctions();
                if (!virtual_functions.isEmpty()){
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
                            s << Qt::endl << INDENT << "FunctionInfo(/* " << QString("%1").arg(QString::number(i), digits) << " */ \"" << virtual_functions.at(i)->name() << "\", \""
                               << jni_signature(virtual_functions.at(i), SlashesAndStuff)
                               << "\"";
                            if(virtual_functions.at(i)->wasPrivate() && virtual_functions.at(i)->isAbstract()){
                                s << ", FunctionInfo::Flags(FunctionInfo::Private | FunctionInfo::Abstract)";
                            }else if(virtual_functions.at(i)->isAbstract()){
                                s << ", FunctionInfo::Abstract";
                            }
                            s << ")";
                            if(i < virtual_functions.size()-1)
                                s << ",";
                        }
                    }
                    s << "});" << Qt::endl;
                }
                if(!cls->hasPrivateDestructor() && !cls->hasJustPrivateConstructors() && !cls->hasUnimplmentablePureVirtualFunction()){
                    usedTypeID = true;
                    s << INDENT << "registerConstructorInfos(typeId, &__qt_destruct_" << cls->qualifiedCppName().replace("::", "_") << ", {";
                    int counter = 0;
                    const AbstractMetaFunction *standardConstructor = nullptr;
                    for(const AbstractMetaFunction *function : cls->functions()) {
                        if (function->isConstructor()
                                && !function->hasRReferences()
                                && !function->isModifiedRemoved(TypeSystem::NativeCode)
                                && !function->isPrivate()
                                && !function->hasTemplateArgumentTypes()){
                            if (function->actualMinimumArgumentCount()==0) {
                                standardConstructor = function;
                            }
                            QStringList pps = getFunctionPPConditions(function);
                            if(!cls->typeEntry()->ppCondition().isEmpty()){
                                pps.removeAll(cls->typeEntry()->ppCondition());
                            }
                            if(!pps.isEmpty()){
                                s << Qt::endl << "#if " << pps.join(" && ");
                            }
                            QList<QList<const AbstractMetaArgument *>> defaultArgumentOverloads;
                            if(function->arguments().isEmpty()){
                                defaultArgumentOverloads << QList<const AbstractMetaArgument *>();
                            }else{
                                for(const AbstractMetaArgument *argument : function->arguments()) {
                                    if(defaultArgumentOverloads.isEmpty()){
                                        if(function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No
                                                && !argument->originalDefaultValueExpression().isEmpty()){
                                            defaultArgumentOverloads << ( QList<const AbstractMetaArgument *>() << nullptr);
                                        }
                                        defaultArgumentOverloads << ( QList<const AbstractMetaArgument *>() << argument);
                                    }else{
                                        if(function->argumentRemoved(argument->argumentIndex()+1)!=ArgumentRemove_No){
                                            if(argument->originalDefaultValueExpression().isEmpty()){
                                                for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                                    defaultArgumentOverloads[i] << argument;
                                                }
                                            }
                                        }else if(argument->originalDefaultValueExpression().isEmpty()){
                                            for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                                defaultArgumentOverloads[i] << argument;
                                            }
                                        }else{
                                            QList<QList<const AbstractMetaArgument *>> copy;
                                            for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                                if(defaultArgumentOverloads[i].last()){
                                                    copy << (QList<const AbstractMetaArgument *>(defaultArgumentOverloads[i]) << argument);
                                                }
                                                defaultArgumentOverloads[i] << nullptr;
                                            }
                                            defaultArgumentOverloads << copy;
                                        }
                                    }
                                }
                            }
                            for(int i=0; i<defaultArgumentOverloads.size(); ++i){
                                s << Qt::endl << INDENT << "   ";
                                if(counter>0){
                                    s << ",";
                                }else{
                                    s << " ";
                                }
                                s << "ConstructorInfo(&__qt_construct_" << cls->qualifiedCppName().replace("::", "_") << "_" << counter++ << ", ";
                                QString args;
                                QTextStream s2(&args);
                                for(const AbstractMetaArgument *argument : defaultArgumentOverloads[i]) {
                                    if (argument && function->argumentRemoved(argument->argumentIndex()+1)==ArgumentRemove_No) {
                                        if(function->argumentTypeArray(argument->argumentIndex() + 1)){
                                            QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                                            cpy->setConstant(false);
                                            cpy->setReferenceType(AbstractMetaType::NoReference);
                                            QList<bool> indirections = cpy->indirections();
                                            if(!indirections.isEmpty()){
                                                indirections.removeLast();
                                                cpy->setIndirections(indirections);
                                            }
                                            AbstractMetaBuilder::decideUsagePattern(cpy.get());
                                            s2 << "[" << jni_signature(cpy.get(), SlashesAndStuff);
                                        }else if(function->argumentTypeBuffer(argument->argumentIndex() + 1)){
                                            QScopedPointer<AbstractMetaType> cpy(argument->type()->copy());
                                            cpy->setConstant(false);
                                            cpy->setReferenceType(AbstractMetaType::NoReference);
                                            QList<bool> indirections = cpy->indirections();
                                            if(!indirections.isEmpty()){
                                                indirections.removeLast();
                                                cpy->setIndirections(indirections);
                                            }
                                            AbstractMetaBuilder::decideUsagePattern(cpy.get());
                                            QString modified_type = jni_signature(cpy.get(), SlashesAndStuff);
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
                                        }else{
                                            QString modified_type = function->typeReplaced(argument->argumentIndex() + 1);
                                            if (modified_type.isEmpty())
                                                s2 << jni_signature(argument->type(), SlashesAndStuff);
                                            else
                                                s2 << jni_signature(modified_type, SlashesAndStuff);
                                        }
                                    }
                                }
                                for(const ArgumentModification* mod : function->addedArguments()){
                                    s2 << jni_signature(mod->modified_type, SlashesAndStuff);
                                }
                                if(args.isEmpty())
                                    s << "nullptr)";
                                else
                                    s << "\"" << args << "\")";
                            }
                            if(!pps.isEmpty()){
                                s << Qt::endl << "#endif // " << pps.join(" && ");
                            }
                        }
                    }
                    if(cls->isQObject() && !cls->isFinal() && standardConstructor){
                        QStringList pps = getFunctionPPConditions(standardConstructor);
                        if(!cls->typeEntry()->ppCondition().isEmpty()){
                            pps.removeAll(cls->typeEntry()->ppCondition());
                        }
                        if(!pps.isEmpty()){
                            s << Qt::endl << "#if " << pps.join(" && ");
                        }
                        s << Qt::endl << INDENT << "   ";
                        if(counter>0)
                            s << ",";
                        else
                            s << " ";
                        s << "ConstructorInfo(&__qt_construct_" << cls->qualifiedCppName().replace("::", "_") << "_declarative, \"Lio/qt/core/QObject$QDeclarativeConstructor;\")";
                        if(!pps.isEmpty()){
                            s << Qt::endl << "#endif // " << pps.join(" && ") << Qt::endl;
                        }
                    }
                    s << Qt::endl << INDENT << "});" << Qt::endl;
                }
            }

            s << "// END: function table" << Qt::endl << Qt::endl;

            if(entry->isQObject() || cls->has_Q_GADGET()){
                QString renamedMethods;
                {
                    QTextStream s(&renamedMethods);
                    for (int i = 0; i < signalsInTargetLang.size(); ++i) {
                        const AbstractMetaFunction *signal = signalsInTargetLang.at(i);
                        if(signal->originalName()!=signal->name()){
                            QStringList pps = getFunctionPPConditions(signal);
                            if(!signal->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(signal->declaringClass()->typeEntry()->ppCondition())){
                                if(pps.isEmpty() || !signal->declaringClass()->typeEntry()->ppCondition().contains("|"))
                                    pps << signal->declaringClass()->typeEntry()->ppCondition();
                                else
                                    pps << "(" + signal->declaringClass()->typeEntry()->ppCondition() + ")";
                            }
                            pps.removeAll(cls->typeEntry()->ppCondition());
                            if(!pps.isEmpty())
                                s << "#if " << pps.join(" && ") << Qt::endl;
                            s << "if(methodIndex==__signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << i << "()){ // " << signal->signature() << Qt::endl
                              << "    return \"" << signal->name() << "\";" << Qt::endl
                              << "}"<< Qt::endl;
                            if(!pps.isEmpty())
                                s << "#endif //" << pps.join(" && ") << Qt::endl;
                        }
                    }
                    for(const AbstractMetaFunction *function : cls->functionsInTargetLang()) {
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
                                    s << "#if " << pps.join(" && ") << Qt::endl;
                                s << "if(methodIndex==" << qtName << "::staticMetaObject.indexOfMethod(\""
                                  << QMetaObject::normalizedSignature(qPrintable(function->signature(true))) << "\")){" << Qt::endl
                                  << "    return \"" << function->name() << "\";" << Qt::endl
                                  << "}" << Qt::endl;
                                if(!pps.isEmpty())
                                    s << "#endif //" << pps.join(" && ") << Qt::endl;
                            }
                        }
                    }
                }
                const ObjectTypeEntry *oentry = entry->isQObject() ? static_cast<const ObjectTypeEntry *>(entry) : nullptr;
                if (cls->isQObject()){
                    if(signalsInTargetLang.size()>0 && cls->has_Q_OBJECT()){
                        int digits = 4;
                        if(signalsInTargetLang.size()>99)
                            digits = 3;
                        else if(signalsInTargetLang.size()>9)
                            digits = 2;
                        else
                            digits = 1;
                        usedTypeID = true;
                        s << INDENT << "registerMetaObject(typeId, " << qtName << "::staticMetaObject, " << (oentry && oentry->isValueOwner() ? "true," : "false,") << Qt::endl
                          << "#ifdef QT_QTJAMBI_PORT" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "[](WeakNativeID __qt_weakNativeID, QObject* __qt_object, QHash<int,QMetaObject::Connection>* connections) {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << "Q_ASSERT(__qt_object);" << Qt::endl
                                      << INDENT << cls->typeEntry()->qualifiedCppName() << "* object = static_cast<" << cls->typeEntry()->qualifiedCppName() << "*>(__qt_object);" << Qt::endl;
                                    for (int i = 0; i < signalsInTargetLang.size(); ++i) {
                                        const AbstractMetaFunction *signal = signalsInTargetLang.at(i);
                                        QStringList pps = getFunctionPPConditions(signal);
                                        if(!signal->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(signal->declaringClass()->typeEntry()->ppCondition())){
                                            if(pps.isEmpty() || !signal->declaringClass()->typeEntry()->ppCondition().contains("|"))
                                                pps << signal->declaringClass()->typeEntry()->ppCondition();
                                            else
                                                pps << "(" + signal->declaringClass()->typeEntry()->ppCondition() + ")";
                                        }
                                        pps.removeAll(cls->typeEntry()->ppCondition());
                                        if(!pps.isEmpty())
                                            s << "#if " << pps.join(" && ") << Qt::endl;

                                        QString function_name = signal->originalName().replace(' ', '_');
                                        QString argumentList;
                                        {
                                            QTextStream _s(&argumentList);
                                            writeFunctionArguments(_s, signal->arguments(), Option(SkipName), signal->arguments().size());
                                        }
                                        s << INDENT << "if(!connections->value(__signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << i << "(), QMetaObject::Connection())){" << Qt::endl;

                                        if(signal->declaringClass()->typeEntry()->qualifiedCppName()=="QSignalMapper"
                                                && function_name=="mapped"
                                                && argumentList=="QWidget*"){
                                            INDENTATION(INDENT)
                                            // this cannot be solved in XML up to now
                                            s << INDENT << "connections->insert(__signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << i << "(), connectSignalMapper(object, __qt_weakNativeID, QOverload<"
                                              << argumentList << ", const WeakNativeID&>::of(&__qt_signal_"
                                              << cls->typeEntry()->qualifiedCppName().replace("::", "_")
                                              << "_" << function_name << ")));" << Qt::endl;
                                        }else{
                                            INDENTATION(INDENT)
                                            s << INDENT << "connections->insert(__signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << i << "(), QObject::connect(object, ";
                                            if(cls->queryFunctionsByOriginalName(signal->originalName()).size()>1){
                                                s << "QOverload<" << argumentList << ">::of(&"
                                                  << signal->declaringClass()->typeEntry()->qualifiedCppName()
                                                  << "::" << function_name << ")";
                                            }else{
                                                s << "&" << signal->declaringClass()->typeEntry()->qualifiedCppName()
                                                  << "::" << function_name;
                                            }
                                            s << ", [__qt_weakNativeID](";
                                            writeFunctionArguments(s, signal->arguments(), Option(), signal->arguments().size());
                                            s << "){__qt_signal_" << cls->typeEntry()->qualifiedCppName().replace("::", "_")
                                              << "_" << function_name << "(";
                                            for(AbstractMetaArgument* argument : signal->arguments()){
                                                s << argument->indexedName() << ", ";
                                            }
                                            s << "__qt_weakNativeID);}));" << Qt::endl;
                                        }
                                        s << INDENT << "}" << Qt::endl;
                                        if(!pps.isEmpty())
                                            s << "#endif //" << pps.join(" && ") << Qt::endl;
                                    }
                                }
                                s << INDENT << "}," << Qt::endl;
                            }
                        }
                        s << "#endif // QT_QTJAMBI_PORT" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            {
                                INDENTATION(INDENT)
                                s << INDENT << "{";
                                int signalCounter = 0;
                                for(const QString& key : signalNames){
                                    const QList<const AbstractMetaFunction *>& list = sortedSignals[key];
                                    for (int i = 0; i < list.size(); ++i) {
                                        const AbstractMetaFunction *f = list.at(i);
                                        AbstractMetaArgumentList arguments;
                                        for (AbstractMetaArgument* arg : f->arguments()) {
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
                                            for (AbstractMetaArgument* arg : arguments) {
                                                if(!arg->defaultValueExpression().isEmpty()){
                                                    defaultCounter++;
                                                }
                                            }
                                            if(defaultCounter>0){
                                                signalSignature += "Default" + QString::number(defaultCounter);
                                            }

                                            signalSignature += ";";
                                        }

                                        QString methodSignature;
                                        if(f->isPrivateSignal()){
                                            methodSignature = "nullptr /* private signal */";
                                        }else{
                                            methodSignature = "\"" + jni_signature(f, SlashesAndStuff) + "\"";
                                        }
                                        QString cppOriginalSignature;
                                        {
                                            QTextStream qtNameStream(&cppOriginalSignature);
                                            CppGenerator::writeFunctionSignature(qtNameStream, f, nullptr, QString(),
                                                                                 Option(SkipReturnType | SkipName | OriginalName | NormalizeAndFixTypeSignature | OriginalTypeDescription),
                                                                                 QString(), QStringList(), f->arguments().size());
                                        }
                                        if(signalCounter>0)
                                            s << ",";
                                        {
                                            INDENTATION(INDENT)
                                            s << Qt::endl << INDENT << "SignalMetaInfo(/* "
                                               << QString("%1").arg(QString::number(signalCounter), digits) << " */ \""
                                               << f->declaringClass()->typeEntry()->javaPackage().replace(".", "/")
                                               << "/" << f->declaringClass()->typeEntry()->targetLangName() << "\", \"" << f->name() << "\", \""
                                               << signalSignature << "\", " << methodSignature << ", " << arguments.size() << ", __signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << signalsInTargetLang.indexOf(f) << ")";
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
                                s << INDENT << "[](JNIEnv * env, const QMetaMethod& method, QVector<ParameterTypeInfo>& infos) {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << "Q_UNUSED(env)" << Qt::endl;
                                    s << INDENT << "Q_UNUSED(infos)" << Qt::endl;
                                    s << INDENT << "int methodIndex = method.methodIndex();" << Qt::endl;
                                    for (int i = 0; i < signalsInTargetLang.size(); ++i) {
                                        const AbstractMetaFunction *signal = signalsInTargetLang.at(i);
                                        QStringList pps = getFunctionPPConditions(signal);
                                        if(!signal->declaringClass()->typeEntry()->ppCondition().isEmpty() && !pps.contains(signal->declaringClass()->typeEntry()->ppCondition())){
                                            if(pps.isEmpty() || !signal->declaringClass()->typeEntry()->ppCondition().contains("|"))
                                                pps << signal->declaringClass()->typeEntry()->ppCondition();
                                            else
                                                pps << "(" + signal->declaringClass()->typeEntry()->ppCondition() + ")";
                                        }
                                        pps.removeAll(cls->typeEntry()->ppCondition());
                                        if(!pps.isEmpty())
                                            s << "#if " << pps.join(" && ") << Qt::endl;
                                        s << INDENT << "if(methodIndex==__signal_method_indexes_" << cls->qualifiedCppName().replace("::", "_") << "_" << i << "()){ // " << signal->signature() << Qt::endl;
                                        {
                                            INDENTATION(INDENT)
                                            writeArgumentConversion(s, signal);
                                            s << INDENT << "return true;" << Qt::endl;
                                        }
                                        s << INDENT << "}" << Qt::endl;
                                        if(!pps.isEmpty())
                                            s << "#endif //" << pps.join(" && ") << Qt::endl;
                                    }
                                    for(const AbstractMetaFunction *function : cls->functionsInTargetLang()) {
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
                                                s << "#if " << pps.join(" && ") << Qt::endl;
                                            s << INDENT << "if(methodIndex==" << qtName << "::staticMetaObject.indexOfMethod(\"";
                                            s << QMetaObject::normalizedSignature(qPrintable(function->signature(true))) << "\")){" << Qt::endl;
                                            {
                                                INDENTATION(INDENT)
                                                writeArgumentConversion(s, function);
                                                s << INDENT << "return true;" << Qt::endl;
                                            }
                                            s << INDENT << "}" << Qt::endl;
                                            if(!pps.isEmpty())
                                                s << "#endif //" << pps.join(" && ") << Qt::endl;
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
                                    for(QString line : renamedMethods.split("\n")){
                                        if(line.startsWith("#")){
                                            s << line << Qt::endl;
                                        }else{
                                            s << INDENT << line << Qt::endl;
                                        }
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
                                    for(QString line : renamedMethods.split("\n")){
                                        if(line.startsWith("#")){
                                            s << line << Qt::endl;
                                        }else{
                                            s << INDENT << line << Qt::endl;
                                        }
                                    }
                                    s << INDENT << "return nullptr;" << Qt::endl;
                                }
                                s << INDENT << "}";
                            }
                        }
                        s << ");" << Qt::endl;
                    }
                    s << "// END: signal table" << Qt::endl << Qt::endl;
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
                                for(QString line : renamedMethods.split("\n")){
                                    if(line.startsWith("#")){
                                        s << line << Qt::endl;
                                    }else{
                                        s << INDENT << line << Qt::endl;
                                    }
                                }
                                s << INDENT << "return nullptr;" << Qt::endl;
                            }
                            s << INDENT << "}";
                        }
                    }
                    s << ");" << Qt::endl;
                }
            }

            s << "// BEGIN: type registration" << Qt::endl;
            if (entry->isObject() && !entry->isQObject() && !entry->isInterface()) {
                if (lookupClassWithPublicDestructor(cls)){
                    usedTypeID = true;
                    s << INDENT << "registerDeleter(typeId, &deleter_" << entry->qualifiedCppName().replace("::", "_") << ");" << Qt::endl;
                }
            }

            bool emittedRegisterType = false;

            int metaType = QMetaType::type(entry->name().toLocal8Bit().constData());
        #if QT_VERSION < 0x050000
            if (metaType == QMetaType::Void){
        #else
            if (metaType == QMetaType::UnknownType && entry->name() != "QStringRef"){
        #endif
                // This variable exists so that the emitted code is the same with QTJAMBI_DEBUG_TOOLS
                // is not defined during compilation.

                bool hiddenMetaObject = false;

                if (!constructorName.isEmpty() && !destructorName.isEmpty()) {
                    usedTypeID = true;
                    s << INDENT << "registerMetaType";
                    if(hiddenMetaObject)
                        s << "NoMetaObject";
                    s << "<" << qtName << ">(\"" << qtName << "\", &" << destructorName << ", &" << constructorName << ");" << Qt::endl;
                    emittedRegisterType = true;
                }


                if(!emittedRegisterType) {
                    // Look for default constructor, required for qRegisterMetaType
                    bool canRegister = entry->isValue() || (entry->isInterface() && static_cast<const InterfaceTypeEntry*>(entry)->origin()->isValue());
                    if (canRegister) {
                        if(!cls->isPublic() || cls->isAbstract()){
                            canRegister = false;
                        }
                        AbstractMetaFunctionList functions = cls->queryFunctions(AbstractMetaClass::WasPublic | AbstractMetaClass::Constructors);

                        bool hasDefaultConstructor = false;
                        for(const AbstractMetaFunction *function : functions) {
                            // Default constructor has to be present
                            if (function->wasPublic() && function->actualMinimumArgumentCount() == 0){
                                hasDefaultConstructor = true;
                                break;
                            }
                        }

                        if (!hasDefaultConstructor) {
                            ReportHandler::warning(QString("Value type '%1' is missing a default constructor. "
                                                           "The resulting C++ code will not compile. If necessary, use <custom-constructor> and "
                                                           "<custom-destructor> tags to provide the constructors.").arg(cls->fullName()));
                        }
                    }

                    if(canRegister){
                        usedTypeID = true;
                        s << INDENT << "registerMetaType";
                        if(hiddenMetaObject)
                            s << "NoMetaObject";
                        s << "<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                    }
                    emittedRegisterType = true;
                }
            }else if(metaType<QMetaType::HighestInternalId){
                usedTypeID = true;
                s << INDENT << "registerMetaTypeID(typeId, " << metaType << ");" << Qt::endl;
                /*
                if(cls->hasHashFunction()){
                    usedTypeID = true;
                    s << INDENT << "registerHashFunction(typeId, [](const void* ptr)->uint{ return !ptr ? 0 : qHash(*reinterpret_cast<const " << cls->qualifiedCppName() << "*>(ptr)); });" << Qt::endl;
                }
                for(const AbstractMetaFunction* less : cls->lessThanFunctions()){
                    if(less->arguments().size()==1){
                        AbstractMetaArgument* arg = less->arguments().at(0);
                        if(arg->type()->indirections().isEmpty() && arg->type()->typeEntry()->qualifiedCppName()==cls->typeEntry()->qualifiedCppName()){
                            usedTypeID = true;
                            s << INDENT << "registerLessFunction(typeId, [](const void* ptr1, const void* ptr2)->bool{ return !ptr1 || !ptr2 ? false : *reinterpret_cast<const " << cls->qualifiedCppName() << "*>(ptr1) < *reinterpret_cast<const " << cls->qualifiedCppName() << "*>(ptr2); });" << Qt::endl;
                        }
                    }
                }
                */
            }else{
                usedTypeID = true;
                s << INDENT << "registerMetaTypeID(typeId, qMetaTypeId<" << qtName << ">());" << Qt::endl;
                /*
                if(cls->hasHashFunction()){
                    usedTypeID = true;
                    s << INDENT << "registerHashFunction<" << cls->qualifiedCppName() << ">();" << Qt::endl;
                }
                for(const AbstractMetaFunction* less : cls->lessThanFunctions()){
                    if(less->arguments().size()==1){
                        AbstractMetaArgument* arg = less->arguments().at(0);
                        if(arg->type()->indirections().isEmpty() && arg->type()->typeEntry()->qualifiedCppName()==cls->typeEntry()->qualifiedCppName()){
                            usedTypeID = true;
                            s << INDENT << "registerLessFunction<" << cls->qualifiedCppName() << ">();" << Qt::endl;
                            break;
                        }
                    }
                }*/
            }
            if(cls->generateShellClass()){
                usedTypeID = true;
                s << INDENT << "registerSizeOfShell(typeId, sizeof(" << shellClassName(cls) << "));" << Qt::endl;
            }

            QSet<QString> registeredTypes;
            registeredTypes << QLatin1String("QMap<QString, QVariant>");
            registeredTypes << QLatin1String("QHash<QString, QVariant>");
            registeredTypes << QLatin1String("QList<QString>");
            for(const AbstractMetaFunction *function : cls->functions()) {
                if(function->declaringClass()==cls){
                    if(function->isSlot() || function->isInvokable() || function->isSignal()){
                        if(function->type()){
                            if((function->type()->isContainer() || function->type()->isPointerContainer())
                                    && !function->type()->instantiations().isEmpty()
                                    && function->type()->indirections().isEmpty()){
                                QString qtName;
                                QTextStream _s(&qtName);
                                writeTypeInfo(_s, function->type(), Option(SkipName | ForceValueType));
                                if(!registeredTypes.contains(qtName)){
                                    registeredTypes.insert(qtName);
                                    s << INDENT << "registerMetaType" << "<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                                }
                            }
                        }
                        for(AbstractMetaArgument* argument : function->arguments()){
                            if(!function->argumentRemoved(argument->argumentIndex()+1)){
                                if((argument->type()->isContainer() || argument->type()->isPointerContainer())
                                        && !argument->type()->instantiations().isEmpty()
                                        && argument->type()->indirections().isEmpty()){
                                    QString qtName;
                                    QTextStream _s(&qtName);
                                    writeTypeInfo(_s, argument->type(), Option(SkipName | ForceValueType));
                                    if(!registeredTypes.contains(qtName)){
                                        registeredTypes.insert(qtName);
                                        s << INDENT << "registerMetaType" << "<" << qtName << ">(\"" << qtName << "\");" << Qt::endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            s << "// END: type registration" << Qt::endl << Qt::endl;

            s << "// BEGIN: polymorphic ids" << Qt::endl;
            if(!entry->polymorphicIdValue().isEmpty()) {
                AbstractMetaClass *baseClass = cls->baseClass();
                while(baseClass && !baseClass->typeEntry()->isPolymorphicBase()){
                    baseClass = baseClass->baseClass();
                }
                if(baseClass){
                    usedTypeID = true;
                    s << INDENT << "registerPolymorphyHandler(typeid(" << baseClass->qualifiedCppName()
                      << "), [](void *ptr) -> bool {" << Qt::endl
                      << INDENT << "        " << baseClass->qualifiedCppName() << " *object = reinterpret_cast<" << baseClass->qualifiedCppName() << " *>(ptr);" << Qt::endl
                      << INDENT << "        Q_ASSERT(object);" << Qt::endl
                      << INDENT << "        return " << QString(entry->polymorphicIdValue()).replace("%1", "object") << ";" << Qt::endl
                      << INDENT << "    }, \"" << cls->package().replace(".", "/") << "/" << cls->name() << "\", typeId, " << (entry->isQObject() ? "true" : "false") << ");" << Qt::endl;
                }
            }else{
                AbstractMetaClass * baseClass = cls->baseClass();
                while(baseClass){
                    if(baseClass->typeEntry()->isPolymorphicBase()){
                        QString warning = QString("class '%1' inherits from polymorphic class '%2', but has no polymorphic id set")
                                          .arg(cls->name())
                                          .arg(baseClass->name());
                        ReportHandler::warning(warning);
                        break;
                    }
                    baseClass = baseClass->baseClass();
                }
            }

            for(AbstractMetaClass *interface : cls->interfaces()) {
                QString polymorphicId = entry->interfacePolymorphicIdValues().value(interface->qualifiedCppName());
                if (polymorphicId.isEmpty()) {
                    if(!cls->isAbstract()
                            && (!cls->baseClass() || !cls->baseClass()->inheritsFromInterface(interface))
                            && cls->name()!=interface->name()+"$Impl"){
                        QString warning = QString("class '%1' inherits from interface '%2', but has no polymorphic id set")
                                                  .arg(cls->name())
                                                  .arg(interface->name());
                        ReportHandler::warning(warning);
                    }
                }else{
                    if(QString(polymorphicId).trimmed()!="false"){
                        usedTypeID = true;
                        // On first find, open the function
                        s << INDENT << "registerPolymorphyHandler(typeid(" << interface->qualifiedCppName()
                          << "), [](void *ptr) -> bool {" << Qt::endl
                          << INDENT << "        " << interface->qualifiedCppName() << " *object = reinterpret_cast<" << interface->qualifiedCppName() << " *>(ptr);" << Qt::endl
                          << INDENT << "        Q_ASSERT(object);" << Qt::endl
                          << INDENT << "        return " << polymorphicId.replace("%1", "object") << ";" << Qt::endl
                          << INDENT << "    }, \"" << cls->package().replace(".", "/") << "/" << cls->name() << "\", typeId, " << (entry->isQObject() ? "true" : "false") << ");" << Qt::endl;
                    }
                }
            }
            s << "// END: polymorphic ids" << Qt::endl << Qt::endl;

            {
                const AbstractMetaClass *parent = cls;
                while(parent){
                    for(AbstractMetaClass *interface : parent->interfaces()) {
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
        }

        s << Qt::endl << "// BEGIN: enums and flags" << Qt::endl;
        for(AbstractMetaEnum *cpp_enum : cls->enums()){
            writeMetaInfo(s, cls, cpp_enum, cls->isPublic());
        }
        if(entry->designatedInterface()){
            for(AbstractMetaClass* interface : cls->interfaces()){
                if(entry->designatedInterface()==interface->typeEntry()){
                    for(AbstractMetaEnum *cpp_enum : interface->enums()){
                        writeMetaInfo(s, interface, cpp_enum, interface->isPublic());
                    }
                    break;
                }
            }
        }
        s << "// END: enums and flags" << Qt::endl;
        writeCodeInjections(s, cls->typeEntry(), CodeSnip::End, TypeSystem::MetaInfo);
        if(!cls->typeEntry()->ppCondition().isEmpty()){
            s << Qt::endl << "#endif //" << cls->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
        }

        s << "}" << Qt::endl;
    }
}

void CppImplGenerator::writeMetaInfo(QTextStream &s, const AbstractMetaClass *owner, const AbstractMetaEnum *enm, bool ownerIsPublic){
    if (!enm)
        return;
    const EnumTypeEntry *entry = enm->typeEntry();
    if (entry->codeGeneration() == TypeEntry::GenerateForSubclass)
        return;

    const QString qtEnumName = entry->qualifiedCppName();
    const QString javaEnumName = [owner,entry]()->QString{
        if(owner){
            if(owner->typeEntry()->targetLangName()==TypeDatabase::globalNamespaceClassName()){
                if(owner->typeEntry()->javaPackage().isEmpty()){
                    return entry->targetLangName();
                }else{
                    return owner->typeEntry()->javaPackage().replace(".", "/") + "/" + entry->targetLangName();
                }
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
    if(enm->isProtected() || !ownerIsPublic){
        s << INDENT << shellClassName(owner) << "::__registerEnumTypeInfo_" << enm->name().replace("::", "_") << "();" << Qt::endl;
    }else{
        if(const FlagsTypeEntry * fentry = entry->flags()){
            const QString qtFlagName = fentry->qualifiedCppName();
            const QString javaFlagName = [owner,fentry]()->QString{
                if(owner){
                    if(owner->typeEntry()->targetLangName()==TypeDatabase::globalNamespaceClassName()){
                        if(owner->typeEntry()->javaPackage().isEmpty()){
                            return fentry->targetLangName();
                        }else{
                            return owner->typeEntry()->javaPackage().replace(".", "/") + "/" + fentry->targetLangName();
                        }
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
            QString qtFlagsAliasName = fentry->originalName();
            s << INDENT << "registerEnumTypeInfo" << (entry->isHiddenMetaObject() ? "NoMetaObject" : "") << "<" << qtEnumName << ">(\"" << qtEnumName << "\", \"" << javaEnumName << "\", \"" << qtFlagName << "\", \"" << qtFlagsAliasName << "\", \"" << javaFlagName << "\");" << Qt::endl;
        }else{
            s << INDENT << "registerEnumTypeInfo" << (entry->isHiddenMetaObject() ? "NoMetaObject" : "") << "<" << qtEnumName << ">(\"" << qtEnumName << "\", \"" << javaEnumName << "\");" << Qt::endl;
        }
    }
    if(!enm->typeEntry()->ppCondition().isEmpty()){
        s << "#endif //" << enm->typeEntry()->ppCondition() << Qt::endl;
    }
}

void CppImplGenerator::writeMetaInfo(QTextStream &s, const AbstractMetaFunctional *functional){
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
    s << "void initialize_meta_info_" << QString(functional->typeEntry()->name()).replace("::", "_").replace("<", "_").replace(">", "_") << "(){" << Qt::endl;
    {
        INDENTATION(INDENT)
        if(!functional->typeEntry()->ppCondition().isEmpty()){
            s << Qt::endl << "#if " << functional->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
        }
        s << INDENT << "registerFunctionalTypeInfo<" << qtName << ", " << shellClassName(functional) << ">(\"" << functional->typeEntry()->normalizedSignature() << "\", \"" << javaName << "\"," << Qt::endl;
        s << INDENT << "                          &deleter_" << QString(functional->typeEntry()->name()).replace("::", "_") << "," << Qt::endl;
        s << INDENT << "                          &__qt_destruct_" << QString(functional->typeEntry()->name()).replace("::", "_") << "," << Qt::endl;
        s << INDENT << "                          {ConstructorInfo(&__qt_construct_" << QString(functional->typeEntry()->name()).replace("::", "_") << "_0, nullptr)}," << Qt::endl;
        s << INDENT << "                          {FunctionInfo(\"call\", \"" << jni_signature(functional, SlashesAndStuff) << "\")});" << Qt::endl;
        if(!functional->typeEntry()->ppCondition().isEmpty()){
            s << Qt::endl << "#endif //" << functional->typeEntry()->ppCondition() << Qt::endl << Qt::endl;
        }
    }
    s << "}" << Qt::endl;
}
