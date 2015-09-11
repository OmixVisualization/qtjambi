/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "cppgenerator.h"
#include "reporthandler.h"

#include "metajava.h"

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

void CppGenerator::writeTypeInfo(QTextStream &s, const AbstractMetaType *type, Option options) {
    if ((options & OriginalTypeDescription) && !type->originalTypeDescription().isEmpty()) {
        QString originalTypeDescription = type->originalTypeDescription();

        if (options & NormalizeAndFixTypeSignature) {
            originalTypeDescription = QMetaObject::normalizedSignature(originalTypeDescription.toLatin1().constData());
            originalTypeDescription = fixNormalizedSignatureForQt(originalTypeDescription);
        }
        int mtype;
        if (originalTypeDescription.contains("qtjambireal")){ // map generator type to qreal type
            s << originalTypeDescription.replace("qtjambireal", "qreal");
            return;
        }else if (originalTypeDescription.contains("qreal")){ // map generator type to qreal type
            s << originalTypeDescription;
            return;
        }else{
            mtype = QMetaType::type(originalTypeDescription.toLatin1().constData());
        }
#if QT_VERSION < 0x050000
        if(mtype>QMetaType::Void && mtype<=QMetaType::LastCoreExtType){
#else
        if(mtype>QMetaType::UnknownType && mtype<=QMetaType::HighestInternalId){
#endif
            originalTypeDescription = QMetaType::typeName(mtype);
        }
        s << originalTypeDescription;
        return;
    }

    if (type->isArray()) {
        writeTypeInfo(s, type->arrayElementType(), options);
        if (options & ArrayAsPointer) {
            s << "*";
        } else {
            s << "[" << type->arrayElementCount() << "]";
        }
        return;
    }

    const TypeEntry *te = type->typeEntry();

    if (type->isConstant() && !(options & ExcludeConst))
        s << "const ";

    if ((options & EnumAsInts) && (te->isEnum() || te->isFlags())) {
        s << "int";
    } else if (te->isFlags()) {
        s << ((FlagsTypeEntry *) te)->originalName();
    } else {
        s << fixCppTypeName(te->qualifiedCppName());
    }

    if (type->instantiations().size() > 0
            && (!type->isContainer()
            || (
                (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::StringListContainer
            &&  (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::ByteArrayListContainer
            &&  (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::QVector2DArrayContainer
            &&  (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::QVector3DArrayContainer
            &&  (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::QVector4DArrayContainer
               )
            )
        ) {
        s << '<';
        QList<AbstractMetaType *> args = type->instantiations();
        bool nested_template = false;
        for (int i = 0; i < args.size(); ++i) {
            if (i != 0)
                s << ", ";
            nested_template |= args.at(i)->isContainer();
            writeTypeInfo(s, args.at(i));
        }
        if (nested_template)
            s << ' ';
        s << '>';
    }

    for(int i=0; i<type->indirections().size(); i++){
        if(type->indirections()[i]){
            s << "*const ";
        }else{
            s << '*';
        }
    }

    if (type->isReference() && !(options & ExcludeReference))
        s << "&";

    if (!(options & SkipName))
        s << ' ';
}


void CppGenerator::writeFunctionArguments(QTextStream &s,
        const AbstractMetaArgumentList &arguments,
        Option option,
        int numArguments) {
    if (numArguments < 0) numArguments = arguments.size();

    bool needComma = false;
    for (int i = 0; i < numArguments; ++i) {
        if (needComma)
            s << ", ";
        else
            needComma = true;
        AbstractMetaArgument *arg = arguments.at(i);
        writeTypeInfo(s, arg->type(), option);
        if (!(option & SkipName))
            s << " " << arg->indexedName();
        if ((option & IncludeDefaultExpression) && !arg->originalDefaultValueExpression().isEmpty()) {
            s << " = ";

            QString expr = arg->originalDefaultValueExpression();
            if (arg->type()->typeEntry()->isEnum() && expr.indexOf("::") < 0)
                s << ((EnumTypeEntry *)arg->type()->typeEntry())->qualifier() << "::";

            s << expr;
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
        const AbstractMetaFunction *java_function,
        const AbstractMetaClass *implementor,
        const QString &name_prefix,
        Option option,
        const QString &classname_prefix,
        const QStringList &extra_arguments,
        int numArguments) {
// ### remove the implementor
    AbstractMetaType *function_type = java_function->type();

    if (java_function->isStatic() && (option & ShowStatic))
        s << "static ";

    if ((option & SkipReturnType) == 0) {
        if (function_type) {
            writeTypeInfo(s, function_type, option);
            s << " ";
        } else if (!java_function->isConstructor()) {
            s << "void ";
        }
    }

    if (implementor) {
        if (classname_prefix.isEmpty())
            s << shellClassName(implementor) << "::";
        else
            s << classname_prefix << implementor->name() << "::";
    }


    QString function_name;
    if (option & OriginalName)
        function_name = java_function->originalName();
    else
        function_name = java_function->name();

    if (option & UnderscoreSpaces)
        function_name = function_name.replace(' ', '_');

    if (java_function->isConstructor())
        function_name = shellClassName(java_function->ownerClass());

    s << name_prefix << function_name;

    if (java_function->attributes() & AbstractMetaAttributes::SetterFunction)
        s << "_setter";
    else if (java_function->attributes() & AbstractMetaAttributes::GetterFunction)
        s << "_getter";

    s << "(";

    AbstractMetaArgumentList tmpList;
    if ((option & SkipRemovedArguments) == SkipRemovedArguments) {	// this should be a method
        const AbstractMetaArgumentList arguments = java_function->arguments();
        for (int i = 0; i < arguments.size(); ++i) {
            if(java_function->argumentRemoved(i + 1))
                continue;
            tmpList.append(arguments.at(i));
        }
    } else {
        tmpList.append(java_function->arguments());
    }
    writeFunctionArguments(s, tmpList, option, numArguments);

    // The extra arguments...
    bool needComma = tmpList.size() != 0 ? true : false;
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
}
