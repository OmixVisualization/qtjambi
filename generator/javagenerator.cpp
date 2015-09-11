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

#include "javagenerator.h"
#include "reporthandler.h"
#include "docparser.h"
#include "jumptable.h"
#include "abstractmetabuilder.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtCore/QRegExp>
#include <QDebug>
#include "typesystem/typedatabase.h"
#include "wrapper.h"			/* for isTargetPlatformArmCpu */

static Indentor INDENT;

JavaGenerator::JavaGenerator()
        : m_doc_parser(0),
        m_docs_enabled(false),
        m_native_jump_table(false) {
}

QString JavaGenerator::fileNameForClass(const AbstractMetaClass *java_class) const {
    return QString("%1.java").arg(java_class->name());
}

void JavaGenerator::writeFieldAccessors(QTextStream &s, const AbstractMetaField *field) {
    Q_ASSERT(field->isPublic() || field->isProtected());

    const AbstractMetaClass *declaringClass = field->enclosingClass();

    FieldModification mod = declaringClass->typeEntry()->fieldModification(field->name());

    // Set function
    if (mod.isWritable() && !field->type()->isConstant()) {
        const AbstractMetaFunction *setter = field->setter();
        if (declaringClass->hasFunction(setter)) {
            QString warning =
                QString("class '%1' already has setter '%2' for public field '%3'")
                .arg(declaringClass->name()).arg(setter->name()).arg(field->name());
            ReportHandler::warning(warning);
        } else {
            writeFunction(s, setter);
        }
    }

    // Get function
    const AbstractMetaFunction *getter = field->getter();
    if (mod.isReadable()) {
        if (declaringClass->hasFunction(getter)) {
            QString warning =
                QString("class '%1' already has getter '%2' for public field '%3'")
                .arg(declaringClass->name()).arg(getter->name()).arg(field->name());
            ReportHandler::warning(warning);
        } else {
            writeFunction(s, getter);
        }
    }
}

QString JavaGenerator::translateType(const AbstractMetaType *java_type, const AbstractMetaClass *context, Option option) {
    QString s;

    if (context != 0 && java_type != 0 && context->typeEntry()->isGenericClass() && java_type->originalTemplateType() != 0)
        java_type = java_type->originalTemplateType();

    if (!java_type) {
        s = "void";
    } else if (java_type->isArray()) {
        s = translateType(java_type->arrayElementType(), context) + "[]";
    } else if (java_type->isEnum() || java_type->isFlags()) {
        if ((java_type->isEnum() &&
                ((EnumTypeEntry *) java_type->typeEntry())->forceInteger()) ||
                (java_type->isFlags() &&
                ((FlagsTypeEntry *) java_type->typeEntry())->forceInteger())) {
            if (option & BoxedPrimitive)
                s = "java.lang.Integer";
            else
                s = "int";
        } else {
            if (option & EnumAsInts)
                s = "int";
            else
                s = java_type->fullName().replace("$", ".");
        }
    } else {
        if (java_type->isPrimitive() && (option & BoxedPrimitive)) {
            s = static_cast<const PrimitiveTypeEntry *>(java_type->typeEntry())->javaObjectFullName().replace("$", ".");

        } else if (java_type->isNativePointer()) {
            s = "org.qtjambi.qt.QNativePointer";

        } else if (java_type->isContainer()) {
            s = java_type->typeEntry()->qualifiedTargetLangName().replace("$", ".");
            const ContainerTypeEntry * container = static_cast<const ContainerTypeEntry *>(java_type->typeEntry());
            if ((option & SkipTemplateParameters) == 0) {
                s += '<';
                QList<AbstractMetaType *> args = java_type->instantiations();
                int argssize = args.size();
                if(container->type() == ContainerTypeEntry::QArrayContainer && argssize>1){
                    // the QArray type can have two template arguments.
                    // nevertheless, the java type must have only the first type argument
                    argssize=1;
                }
                for (int i=0; i<argssize; ++i) {
                    if (i != 0)
                        s += ", ";
                    bool isMultiMap = container->type() == ContainerTypeEntry::MultiMapContainer
                                      && i == 1;
                    if (isMultiMap)
                        s += "java.util.List<";
                    if(container->type() == ContainerTypeEntry::QDeclarativeListPropertyContainer){
                        s += translateType(args.at(i), context, BoxedPrimitive).replace("$", ".");
                    }else{
                        s += translateType(args.at(i), context, BoxedPrimitive).replace("$", ".");
                    }
                    if (isMultiMap)
                        s += ">";
                }
                s += '>';
            }

        } else if (java_type->isPointerContainer() && java_type->instantiations().size()==1) {
            AbstractMetaType* instantiation = java_type->instantiations()[0]->copy();
            instantiation->setIndirections(QList<bool>(instantiation->indirections()) << false);
            AbstractMetaBuilder::decideUsagePattern(instantiation);
            s = translateType(instantiation, context, option);
        } else {
            const TypeEntry *type = java_type->typeEntry();
            if (type->designatedInterface())
                type = type->designatedInterface();
            s = type->qualifiedTargetLangName().replace("$", ".");
        }
    }

    return s;
}

QString JavaGenerator::argumentString(const AbstractMetaFunction *java_function,
                                      const AbstractMetaArgument *java_argument,
                                      uint options) {
    QString modified_type = java_function->typeReplaced(java_argument->argumentIndex() + 1);
    QString arg;

    if (modified_type.isEmpty())
        arg = translateType(java_argument->type(), java_function->implementingClass(), (Option) options).replace("$", ".");
    else
        arg = modified_type.replace('$', '.');

    if ((options & SkipName) == 0) {
        arg += " ";
        arg += java_argument->argumentName();
    }

    return arg;
}

void JavaGenerator::writeArgument(QTextStream &s,
                                  const AbstractMetaFunction *java_function,
                                  const AbstractMetaArgument *java_argument,
                                  uint options) {
    s << argumentString(java_function, java_argument, options);
}


void JavaGenerator::writeIntegerEnum(QTextStream &s, const AbstractMetaEnum *java_enum) {
    const AbstractMetaEnumValueList &values = java_enum->values();

    s << "    public static class " << java_enum->name() << "{" << endl;
    for (int i = 0; i < values.size(); ++i) {
        AbstractMetaEnumValue *value = values.at(i);

        if (java_enum->typeEntry()->isEnumValueRejected(value->name()))
            continue;

        if (m_doc_parser)
            s << m_doc_parser->documentation(value);

        s << "        public static final int " << value->name() << " = " << value->value();
        s << ";";
        s << endl;
    }

    s << "    } // end of enum " << java_enum->name() << endl << endl;
}

void JavaGenerator::writeEnum(QTextStream &s, const AbstractMetaEnum *java_enum) {
    if (m_doc_parser) {
        s << m_doc_parser->documentation(java_enum);
    }

    if (java_enum->typeEntry()->forceInteger()) {
        writeIntegerEnum(s, java_enum);
        return;
    }

    // Check if enums in QObjects are declared in the meta object. If not
    if ((java_enum->enclosingClass()->isQObject() || java_enum->enclosingClass()->isQtNamespace())
            && !java_enum->hasQEnumsDeclaration()) {
        s << INDENT << "@QtBlockedEnum" << endl;
    }

    // Generates Java 1.5 type enums
    s  << INDENT << "public enum " << java_enum->name()
    << " implements org.qtjambi.qt.QtEnumerator";
    if (!java_enum->typeEntry()->implements().isEmpty()) {
        s << ", " << java_enum->typeEntry()->implements();
    }
    s << " {" << endl;

    s << INDENT;
    const AbstractMetaEnumValueList &values = java_enum->values();
    EnumTypeEntry *entry = java_enum->typeEntry();

    for (int i = 0; i < values.size(); ++i) {
        AbstractMetaEnumValue *enum_value = values.at(i);

//          only reject enum entries on the resolve side!
//        if (java_enum->typeEntry()->isEnumValueRejected(enum_value->name()))
//            continue;

        if (m_doc_parser)
            s << m_doc_parser->documentation(enum_value);

        s << "        " << enum_value->name() << "(" << enum_value->value() << ")";

        if (i != values.size() - 1 || entry->isExtensible()) {
            s << "," << endl << INDENT;
        }
    }

    if (entry->isExtensible())
        s << "CustomEnum(0)";

    s << ";" << endl << endl;

    s <<  INDENT << java_enum->name() << "(int value) { this.value = value; }" << endl
    << "        public int value() { return value; }" << endl
    << endl << INDENT;

    // Write out the createQFlags() function if its a QFlags enum
    if (entry->flags()) {
        FlagsTypeEntry *flags_entry = entry->flags();
        s << "public static " << flags_entry->targetLangName() << " createQFlags("
          << entry->targetLangName().replace("$",".") << " ... values) {" << endl
        << INDENT << "    return new " << flags_entry->targetLangName() << "(values);" << endl
        << INDENT << "}" << endl;
    }

    // The resolve functions. The public one that returns the right
    // type and an internal one that has a generic signature. Makes it
    // easier to find the right one from JNI.
    s << "        public static " << java_enum->name() << " resolve(int value) {" << endl
    << INDENT << "    return (" << java_enum->name() << ") resolve_internal(value);" << endl
    << INDENT << "}" << endl
    << INDENT << "private static Object resolve_internal(int value) {" << endl
    << INDENT << "    switch (value) {" << endl;

    for (int i = 0; i < values.size(); ++i) {
        AbstractMetaEnumValue *e = values.at(i);

        if (java_enum->typeEntry()->isEnumValueRejected(e->name()))
            continue;

        s << INDENT << "    case " << e->value() << ": return " << e->name() << ";" << endl;
    }

    s << INDENT << "    }" << endl;

    if (entry->isExtensible()) {
        s << INDENT << "    if (enumCache == null)" << endl
        << "        enumCache = new java.util.HashMap<Integer, " << java_enum->name()
        << ">();" << endl
        << INDENT << "        " << java_enum->name() << " e = enumCache.get(value);" << endl
        << INDENT << "        if (e == null) {" << endl
        << INDENT << "            e = (" << java_enum->name() << ") org.qtjambi.qt.GeneratorUtilities.createExtendedEnum("
        << "value, CustomEnum.ordinal(), " << java_enum->name() << ".class, CustomEnum.name());"
        << endl
        << INDENT << "            enumCache.put(value, e);" << endl
        << INDENT << "        }" << endl
        << INDENT << "        return e;" << endl;
    } else {
        s << INDENT << "        throw new org.qtjambi.qt.QNoSuchEnumValueException(value);" << endl;
    }


    s << INDENT << "    }" << endl;

    s << INDENT << "    private final int value;" << endl
    << endl << INDENT;
    if (entry->isExtensible()) {
        s << "    private static java.util.HashMap<Integer, " << java_enum->name()
        << "> enumCache;" << endl << INDENT;
    }
    s << "}" << endl << INDENT;

    // Write out the QFlags if present...
    FlagsTypeEntry *flags_entry = entry->flags();
    if (flags_entry) {
        QString flagsName = flags_entry->targetLangName();
        s << "public static class " << flagsName << " extends org.qtjambi.qt.QFlags<"
        << java_enum->name().replace("$",".") << "> {" << endl << INDENT
        << "    private static final long serialVersionUID = 1L;" << endl
        << "    public " << flagsName << "(" << java_enum->name().replace("$",".") << " ... args)"
        << " { super(args); }" << endl << INDENT
        << "    public " << flagsName << "(int value) { setValue(value); }" << endl << INDENT
        << "}" << endl << endl << INDENT;
    }
}

void JavaGenerator::writePrivateNativeFunction(QTextStream &s, const AbstractMetaFunction *java_function) {
    int exclude_attributes = AbstractMetaAttributes::Public | AbstractMetaAttributes::Protected;
    int include_attributes = 0;

    if (java_function->isEmptyFunction())
        exclude_attributes |= AbstractMetaAttributes::Native;
    else
        include_attributes |= AbstractMetaAttributes::Native;

//     if (!java_function->isConstructor())
//         include_attributes |= AbstractMetaAttributes::Static;

    writeFunctionAttributes(s, java_function, include_attributes, exclude_attributes,
                            EnumAsInts
                            | (java_function->isEmptyFunction()
                               || java_function->isNormal()
                               || java_function->isSignal() ? 0 : SkipReturnType));

    if (java_function->isConstructor())
        s << "void ";
    s << java_function->marshalledName();

    s << "(";

    AbstractMetaArgumentList arguments = java_function->arguments();

    if (!java_function->isStatic() && !java_function->isConstructor())
        s << "long __this__nativeId";
    for (int i = 0; i < arguments.count(); ++i) {
        const AbstractMetaArgument *arg = arguments.at(i);

        if (!java_function->argumentRemoved(i + 1)) {
            if (i > 0 || (!java_function->isStatic() && !java_function->isConstructor()))
                s << ", ";

            if (!arg->type()->hasNativeId())
                writeArgument(s, java_function, arg, EnumAsInts);
            else
                s << "long " << arg->argumentName();
        }
    }
    s << ")";

    // Make sure people don't call the private functions
    if (java_function->isEmptyFunction()) {
        s << endl
        << INDENT << "{" << endl
        << INDENT << "    throw new org.qtjambi.qt.QNoImplementationException();" << endl
        << INDENT << "}" << endl << endl;
    } else {
        s << ";" << endl;
    }
}

static QString function_call_for_ownership(TypeSystem::Ownership owner) {
    if (owner == TypeSystem::CppOwnership) {
        return "disableGarbageCollection()";
    } else if (owner == TypeSystem::TargetLangOwnership) {
        return "setJavaOwnership()";
    } else if (owner == TypeSystem::DefaultOwnership) {
        return "reenableGarbageCollection()";

    } else {
        Q_ASSERT(false);
        return "bogus()";
    }
}

void JavaGenerator::writeOwnershipForContainer(QTextStream &s, TypeSystem::Ownership owner,
        AbstractMetaType *type, const QString &arg_name) {
    Q_ASSERT(type->isContainer());

    s << INDENT << "for (" << type->instantiations().at(0)->fullName() << " i : "
    << arg_name << ")" << endl
    << INDENT << "    if (i != null) i." << function_call_for_ownership(owner) << ";" << endl;

}

void JavaGenerator::writeOwnershipForContainer(QTextStream &s, TypeSystem::Ownership owner,
        AbstractMetaArgument *arg) {
    writeOwnershipForContainer(s, owner, arg->type(), arg->argumentName());
}

static FunctionModificationList get_function_modifications_for_class_hierarchy(const AbstractMetaFunction *java_function) {
    FunctionModificationList mods;
    const AbstractMetaClass *cls = java_function->implementingClass();
    while (cls != 0) {
        mods += java_function->modifications(cls);

        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    return mods;
}

void JavaGenerator::writeInjectedCode(QTextStream &s, const AbstractMetaFunction *java_function,
                                      CodeSnip::Position position) {
    FunctionModificationList mods = get_function_modifications_for_class_hierarchy(java_function);
    foreach(FunctionModification mod, mods) {
        if (mod.snips.count() <= 0)
            continue ;

        foreach(CodeSnip snip, mod.snips) {
            if (snip.position != position)
                continue ;

            if (snip.language != TypeSystem::TargetLangCode)
                continue ;

            QString code;
            QTextStream tmpStream(&code);
            snip.formattedCode(tmpStream, INDENT);
            ArgumentMap map = snip.argumentMap;
            ArgumentMap::iterator it = map.begin();
            for (;it != map.end();++it) {
                int pos = it.key() - 1;
                QString meta_name = it.value();

                if (pos >= 0 && pos < java_function->arguments().count()) {
                    code = code.replace(meta_name, java_function->arguments().at(pos)->argumentName());
                } else {
                    QString debug = QString("argument map specifies invalid argument index %1"
                                            "for function '%2'")
                                    .arg(pos + 1).arg(java_function->name());
                    ReportHandler::warning(debug);
                }

            }
            s << code << endl;
        }
    }
}


void JavaGenerator::writeJavaCallThroughContents(QTextStream &s, const AbstractMetaFunction *java_function, uint attributes) {
    writeInjectedCode(s, java_function, CodeSnip::Beginning);

    if (java_function->implementingClass()->isQObject()
            && !java_function->isStatic()
            && !java_function->isConstructor()
            && java_function->name() != QLatin1String("thread")
            && java_function->name() != QLatin1String("disposeLater")) {
        s << INDENT << "org.qtjambi.qt.GeneratorUtilities.threadCheck(this);" << endl;
    }

    AbstractMetaArgumentList arguments = java_function->arguments();

    if (!java_function->isConstructor()) {
        TypeSystem::Ownership owner = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, -1);
        if (owner != TypeSystem::InvalidOwnership)
            s << INDENT << "this." << function_call_for_ownership(owner) << ";" << endl;
    }

    for (int i = 0; i < arguments.count(); ++i) {
        AbstractMetaArgument *arg = arguments.at(i);
        AbstractMetaType *type = arg->type();

        if (!java_function->argumentRemoved(i + 1)) {
            TypeSystem::Ownership owner = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, i + 1);
            if (owner != TypeSystem::InvalidOwnership) {
                s << INDENT << "if (" << arg->argumentName() << " != null) {" << endl;
                {
                    Indentation indent(INDENT);
                    if (arg->type()->isContainer())
                        writeOwnershipForContainer(s, owner, arg);
                    else
                        s << INDENT << arg->argumentName() << "." << function_call_for_ownership(owner) << ";" << endl;
                }
                s << INDENT << "}" << endl;
            }

            if (type->isArray()) {
                s << INDENT << "if (" << arg->argumentName() << ".length != " << type->arrayElementCount() << ")" << endl
                << INDENT << "    " << "throw new IllegalArgumentException(\"Wrong number of elements in array. Found: \" + "
                << arg->argumentName() << ".length + \", expected: " << type->arrayElementCount() << "\");"
                << endl << endl;
            }

            if (type->isEnum()) {
                EnumTypeEntry *et = (EnumTypeEntry *) type->typeEntry();
                if (et->forceInteger()) {
                    if (!et->lowerBound().isEmpty()) {
                        s << INDENT << "if (" << arg->argumentName() << " < " << et->lowerBound() << ")" << endl
                        << INDENT << "    throw new IllegalArgumentException(\"Argument " << arg->argumentName()
                        << " is less than lowerbound " << et->lowerBound() << "\");" << endl;
                    }
                    if (!et->upperBound().isEmpty()) {
                        s << INDENT << "if (" << arg->argumentName() << " > " << et->upperBound() << ")" << endl
                        << INDENT << "    throw new IllegalArgumentException(\"Argument " << arg->argumentName()
                        << " is greated than upperbound " << et->upperBound() << "\");" << endl;
                    }
                }
            }
        }
    }

    if (!java_function->isConstructor() && !java_function->isStatic()) {
        s << INDENT << "if (nativeId() == 0)" << endl
        << INDENT << "    throw new QNoNativeResourcesException(\"Function call on incomplete object of type: \" +getClass().getName());" << endl;
    }
    if((java_function->isAbstract() || !(java_function->originalAttributes() & AbstractMetaAttributes::Public)) && !java_function->implementingClass()->generateShellClass()){
        s << INDENT << "throw new org.qtjambi.qt.QNoImplementationException();" << endl;
    }else if(java_function->isAbstract() && java_function->implementingClass()->hasPrivatePureVirtualFunction()){
        s << INDENT << "throw new org.qtjambi.qt.QNoImplementationException();" << endl;
    }else{

        for (int i = 0; i < arguments.size(); ++i) {
            if (java_function->nullPointersDisabled(java_function->implementingClass(), i + 1)) {
                s << INDENT << "if (" << arguments.at(i)->argumentName() << " == null)" << endl
                << INDENT << "    throw new NullPointerException(\"Argument '" << arguments.at(i)->argumentName() << "': null not expected.\");" << endl;
            }
        }

        bool has_argument_referenceCounts = false;
        QList<ReferenceCount> referenceCounts;
        for (int i = 0; i < arguments.size() + 1; ++i) {
            referenceCounts = java_function->referenceCounts(java_function->implementingClass(),
                              i == 0 ? -1 : i);
            if (referenceCounts.size() > 0) {
                foreach(ReferenceCount refCount, referenceCounts) {
                    // We just want to know this to secure return value into local variable
                    // to hold over ReferenceCount management later on.
                    if (refCount.action != ReferenceCount::Ignore) {
                        // Something active have been specified
                        has_argument_referenceCounts = true;
                        break;
                    }
                }
            }

            // This is too early to manage referenceCount, it causes us to potentially overwrite the
            // last remaining GlobalReference to the object (before we have called the native method)
            // so the GC might destroy/delete it too early.
    //        foreach(ReferenceCount refCount, referenceCounts)
    //            writeReferenceCount(s, refCount, i == 0 ? "this" : arguments.at(i - 1)->argumentName());
        }

        // Lookup if there is a reference-count action required on the return value.
        QList<ReferenceCount> returnReferenceCounts = java_function->referenceCounts(java_function->implementingClass(), 0);
        AbstractMetaType *return_type = java_function->type();
        QString new_return_type = QString(java_function->typeReplaced(0)).replace('$', '.');
        bool has_return_type = new_return_type != "void"
        && (!new_return_type.isEmpty() || return_type != 0);
        TypeSystem::Ownership owner = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, 0);

        bool has_code_injections_at_the_end = false;
        FunctionModificationList mods = get_function_modifications_for_class_hierarchy(java_function);
        foreach(FunctionModification mod, mods) {
            foreach(CodeSnip snip, mod.snips) {
                if (snip.position == CodeSnip::End && snip.language == TypeSystem::TargetLangCode) {
                    has_code_injections_at_the_end = true;
                    break;
                }
            }
        }

        bool needs_return_variable = has_return_type
                                     && (owner != TypeSystem::InvalidOwnership || has_argument_referenceCounts || returnReferenceCounts.size() > 0 || has_code_injections_at_the_end);

        s << INDENT;
        if (has_return_type && java_function->argumentReplaced(0).isEmpty()) {
            if (needs_return_variable) {
                if (new_return_type.isEmpty())
                    s << translateType(return_type, java_function->implementingClass());
                else
                    s << new_return_type;

                s << " __qt_return_value = ";
            } else {
                s << "return ";
            }

            if (return_type && return_type->isTargetLangEnum()) {
                s << ((EnumTypeEntry *) return_type->typeEntry())->qualifiedTargetLangName() << ".resolve(";
            } else if (return_type && return_type->isTargetLangFlags()) {
                s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace("$", ".") << "(";
            }
        }

        bool useJumpTable = java_function->jumpTableId() != -1;
        if (useJumpTable) {
            // The native function returns the correct type, we only have
            // java.lang.Object so we may have to cast...
            QString signature = JumpTablePreprocessor::signature(java_function);

    //         printf("return: %s::%s return=%p, replace-value=%s, replace-type=%s signature: %s\n",
    //                qPrintable(java_function->ownerClass()->name()),
    //                qPrintable(java_function->signature()),
    //                return_type,
    //                qPrintable(java_function->argumentReplaced(0)),
    //                qPrintable(new_return_type),
    //                qPrintable(signature));

            if (has_return_type && signature.at(0) == 'L') {
                if (new_return_type.length() > 0) {
    //                 printf(" ---> replace-type: %s\n", qPrintable(new_return_type));
                    s << "(" << new_return_type << ") ";
                } else if (java_function->argumentReplaced(0).isEmpty()) {
    //                 printf(" ---> replace-value\n");
                    s << "(" << translateType(return_type, java_function->implementingClass()) << ") ";
                }
            }

            s << "JTbl." << JumpTablePreprocessor::signature(java_function) << "("
            << java_function->jumpTableId() << ", ";

            // Constructors and static functions don't have native id, but
            // the functions expect them anyway, hence add '0'. Normal
            // functions get their native ids added just below...
            if (java_function->isConstructor() || java_function->isStatic())
                s << "0, ";

        } else {
            if (attributes & SuperCall) {
                s << "super.";
            }
            s << java_function->marshalledName() << "(";
        }

        if (!java_function->isConstructor() && !java_function->isStatic())
            s << "nativeId()";


        for (int i = 0; i < arguments.count(); ++i) {
            const AbstractMetaArgument *arg = arguments.at(i);
            const AbstractMetaType *type = arg->type();

            if (!java_function->argumentRemoved(i + 1)) {
                if (i > 0 || (!java_function->isStatic() && !java_function->isConstructor()))
                    s << ", ";

                if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                    s << arg->argumentName() << ".value()";
                } else if (!type->hasNativeId()) {
                    s << arg->argumentName();
                } else {
                    bool force_abstract = type->typeEntry()->isComplex() && (((static_cast<const ComplexTypeEntry *>(type->typeEntry()))->typeFlags() & ComplexTypeEntry::ForceAbstract) != 0);
                    if (!force_abstract) {
                        s << arg->argumentName() << " == null ? 0 : ";
                    } // else if (value type is abstract) then we will get a null pointer exception, which is all right

                    s << arg->argumentName() << ".nativeId()";
                }
            }
        }

        if (useJumpTable) {
            if ((!java_function->isConstructor() && !java_function->isStatic()) || arguments.size() > 0)
                s << ", ";

            if (java_function->isStatic())
                s << "null";
            else
                s << "this";
        }

        s << ")";

        // This closed the ".resolve(" or the "new MyType(" fragments
        if (return_type && (return_type->isTargetLangEnum() || return_type->isTargetLangFlags()))
            s << ")";

        s << ";" << endl;

        // We must ensure we retain a Java hard-reference over the native method call
        // so that the GC will not destroy the C++ object too early.  At this point we
        // have called the native method call so can manage referenceCount issues.
        // First the input arguments
        for (int i = 0; i < arguments.size() + 1; ++i) {
            referenceCounts = java_function->referenceCounts(java_function->implementingClass(),
                              i == 0 ? -1 : i);

            foreach(ReferenceCount refCount, referenceCounts)
                writeReferenceCount(s, refCount, i == 0 ? "this" : arguments.at(i - 1)->argumentName(), arguments);
        }

        if (!java_function->argumentReplaced(0).isEmpty()) {
            s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << endl;
            return;
        }

        // Then the return value
        foreach(ReferenceCount referenceCount, returnReferenceCounts) {
            writeReferenceCount(s, referenceCount, "__qt_return_value", arguments);
        }

        writeInjectedCode(s, java_function, CodeSnip::End);

        if (needs_return_variable) {
            if (owner != TypeSystem::InvalidOwnership) {
                s << INDENT << "if (__qt_return_value != null) {" << endl;
                if (return_type->isContainer())
                    writeOwnershipForContainer(s, owner, return_type, "__qt_return_value");
                else
                    s << INDENT << "    __qt_return_value." << function_call_for_ownership(owner) << ";" << endl;
                s << INDENT << "}" << endl;
            }
            s << INDENT << "return __qt_return_value;" << endl;
        }

        if (java_function->isConstructor()) {
            TypeSystem::Ownership owner = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, -1);
            if (owner != TypeSystem::InvalidOwnership && java_function->isConstructor())
                s << INDENT << "this." << function_call_for_ownership(owner) << ";" << endl;
        }
    }
}

void JavaGenerator::writeSignal(QTextStream &s, const AbstractMetaFunction *java_function) {
    Q_ASSERT(java_function->isSignal());

    if (java_function->isModifiedRemoved(TypeSystem::TargetLangCode))
        return ;

    AbstractMetaArgumentList arguments = java_function->arguments();
    int sz = arguments.count();

    QList<QString> defaultValueArgumentType;
    QList<QString> defaultValueExpressions;

    QString signalTypeName("Signal");
    if (java_function->isPrivate()) {
        signalTypeName = "PrivateSignal";
    }else{
        for (int i = 0; i < sz; ++i) {
            QString defaultValueExpression = arguments.at(i)->defaultValueExpression();
            if(!defaultValueExpression.isEmpty()){
                defaultValueExpressions << defaultValueExpression;

                QString type = java_function->typeReplaced(i + 1);

                if (type.isEmpty())
                    type = translateType(arguments.at(i)->type(), java_function->implementingClass(), BoxedPrimitive).replace("$", ".");
                else
                    type = type.replace("$", ".");

                defaultValueArgumentType << type;
            }
        }
    }

    signalTypeName += QString::number(sz);

    QString signalDefaultArgumentExpressions;
    if(!defaultValueExpressions.isEmpty()){
        int dsz = defaultValueExpressions.size();
        signalTypeName += "Default";
        signalTypeName += QString::number(dsz);
        QTextStream s2(&signalDefaultArgumentExpressions);
        for (int i = 0; i < dsz; ++i) {
            if (i > 0)
                s2 << ", ";
            s2 << endl;
            Indentation indent(INDENT);
            s2 << INDENT << "new DefaultValue<";
            s2 << defaultValueArgumentType.at(i);
            s2 << ">(){";
            s2 << endl;
            {
                Indentation indent2(INDENT);
                s2 << INDENT << "@Override" << endl;
                s2 << INDENT << "public ";
                s2 << defaultValueArgumentType.at(i);
                s2 << " get(){" << endl;
                {
                    Indentation indent3(INDENT);
                    s2 << INDENT << "return (";
                    s2 << defaultValueArgumentType.at(i);
                    s2 << ")";
                    s2 << defaultValueExpressions.at(i) << ";" << endl;
                }
                s2 << INDENT << "}" << endl;
            }
            s2 << INDENT << "}";
        }
        s2 << endl << INDENT;
    }
    if (sz > 0) {
        signalTypeName += "<";
        for (int i = 0; i < sz; ++i) {
            if (i > 0)
                signalTypeName += ", ";

            QString modifiedType = java_function->typeReplaced(i + 1);

            if (modifiedType.isEmpty())
                signalTypeName += translateType(arguments.at(i)->type(), java_function->implementingClass(), BoxedPrimitive).replace("$", ".");
            else
                signalTypeName += modifiedType.replace("$", ".");
        }
        signalTypeName += ">";
    }

    int exclude_attributes = AbstractMetaAttributes::Abstract
                             | AbstractMetaAttributes::Native;
    int include_attributes = AbstractMetaAttributes::Public;

    QString signalName = java_function->name();
    FunctionModificationList mods = java_function->modifications(java_function->implementingClass());
    foreach(FunctionModification mod, mods) {
        if (mod.isAccessModifier()) {
            exclude_attributes |= AbstractMetaAttributes::Public
                                  | AbstractMetaAttributes::Protected
                                  | AbstractMetaAttributes::Private
                                  | AbstractMetaAttributes::Friendly;
            include_attributes &= ~(exclude_attributes);

            if (mod.isPublic())
                include_attributes |= AbstractMetaAttributes::Public;
            else if (mod.isProtected())
                include_attributes |= AbstractMetaAttributes::Protected;
            else if (mod.isPrivate())
                include_attributes |= AbstractMetaAttributes::Private;
            else if (mod.isFriendly())
                include_attributes |= AbstractMetaAttributes::Friendly;

            exclude_attributes &= ~(include_attributes);

        }
    }

    // Insert Javadoc
    if (m_doc_parser) {
        QString signature = functionSignature(java_function,
                                              include_attributes | NoBlockedSlot,
                                              exclude_attributes);
        QString docs = m_doc_parser->documentationForSignal(signature);
        if (docs.isEmpty()) {
            signature.replace(QLatin1String("public"), QLatin1String("protected"));
            docs = m_doc_parser->documentationForSignal(signature);
        }
        s << m_doc_parser->documentationForSignal(signature);
    }

    writeFunctionAttributes(s, java_function, include_attributes, exclude_attributes,
                            SkipReturnType);
    s << signalTypeName;
    s << " " << signalName << " = new " << signalTypeName << "(" << signalDefaultArgumentExpressions << ");" << endl;

    // We don't write out the functions for private signals, because they cannot
    // be emitted, hence they will never be used...
    if (!java_function->isPrivate())
        writeFunction(s, java_function,
                      AbstractMetaAttributes::Private,
                      AbstractMetaAttributes::Visibility & ~AbstractMetaAttributes::Private);
}


void JavaGenerator::retrieveModifications(const AbstractMetaFunction *java_function,
        const AbstractMetaClass *java_class,
        uint *exclude_attributes,
        uint *include_attributes) const {
    FunctionModificationList mods = java_function->modifications(java_class);
//     printf("name: %s has %d mods\n", qPrintable(java_function->signature()), mods.size());
    foreach(FunctionModification mod, mods) {
        if (mod.isAccessModifier()) {
//             printf(" -> access mod to %x\n", mod.modifiers);
            *exclude_attributes |= AbstractMetaAttributes::Public
                                   | AbstractMetaAttributes::Protected
                                   | AbstractMetaAttributes::Private
                                   | AbstractMetaAttributes::Friendly;

            if (mod.isPublic())
                *include_attributes |= AbstractMetaAttributes::Public;
            else if (mod.isProtected())
                *include_attributes |= AbstractMetaAttributes::Protected;
            else if (mod.isPrivate())
                *include_attributes |= AbstractMetaAttributes::Private;
            else if (mod.isFriendly())
                *include_attributes |= AbstractMetaAttributes::Friendly;
        }

        if (mod.isFinal()) {
            *include_attributes |= AbstractMetaAttributes::FinalInTargetLang;
        } else if (mod.isDeclaredFinal()) {
            *include_attributes |= AbstractMetaAttributes::FinalInTargetLang;
            *include_attributes |= AbstractMetaAttributes::FinalInCpp;
            *include_attributes |= AbstractMetaAttributes::DeclaredFinalInCpp;
        } else if (mod.isNonFinal()) {
            *exclude_attributes |= AbstractMetaAttributes::FinalInTargetLang;
        }
    }

    *exclude_attributes &= ~(*include_attributes);
}

QString JavaGenerator::functionSignature(const AbstractMetaFunction *java_function,
        uint included_attributes, uint excluded_attributes,
        Option option,
        int arg_count) {
    AbstractMetaArgumentList arguments = java_function->arguments();
    int argument_count = arg_count < 0 ? arguments.size() : arg_count;

    QString result;
    QTextStream s(&result);
    QString functionName = java_function->name();
    // The actual function
    if (!(java_function->isEmptyFunction() || java_function->isNormal() || java_function->isSignal()))
        option = Option(option | SkipReturnType);
    writeFunctionAttributes(s, java_function, included_attributes, excluded_attributes, option);

    s << functionName << "(";
    writeFunctionArguments(s, java_function, argument_count, option);
    s << ")";

    return result;
}

void JavaGenerator::setupForFunction(const AbstractMetaFunction *java_function,
                                     uint *included_attributes,
                                     uint *excluded_attributes) const {
    *excluded_attributes |= java_function->ownerClass()->isInterface() || java_function->isConstructor()
                            ? AbstractMetaAttributes::Native | AbstractMetaAttributes::Final
                            : 0;
    if (java_function->ownerClass()->isInterface())
        *excluded_attributes |= AbstractMetaAttributes::Abstract;
    if (java_function->needsCallThrough())
        *excluded_attributes |= AbstractMetaAttributes::Native;

    const AbstractMetaClass *java_class = java_function->ownerClass();
    retrieveModifications(java_function, java_class, excluded_attributes, included_attributes);
}

void JavaGenerator::writeReferenceCount(QTextStream &s, const ReferenceCount &refCount,
                                        const QString &argumentName, const AbstractMetaArgumentList& arguments) {
    if (refCount.action == ReferenceCount::Ignore)
        return;

    QString refCountVariableName = refCount.variableName;
    if (!refCount.declareVariable.isEmpty()) {
        if(refCount.action == ReferenceCount::Put){
            s << INDENT << "java.util.Map<Object,Object> __rcTmp = (java.util.Map<Object,Object>)org.qtjambi.qt.GeneratorUtilities.fetchField(this," << endl
            << INDENT << "                                                                 " << refCount.declareVariable << ".class," << endl
            << INDENT << "                                                                 \"" << refCountVariableName << "\");" << endl;
            refCountVariableName = "__rcTmp";
        }else if(refCount.action != ReferenceCount::Set){
            s << INDENT << "java.util.Collection<Object> __rcTmp = (java.util.Collection<Object>)org.qtjambi.qt.GeneratorUtilities.fetchField(this," << endl
            << INDENT << "                                                                 " << refCount.declareVariable << ".class," << endl
            << INDENT << "                                                                 \"" << refCountVariableName << "\");" << endl;
            refCountVariableName = "__rcTmp";
        }
    }

    if (refCount.action != ReferenceCount::Set) {
        s << INDENT << "if (" << argumentName << " != null";

        if (!refCount.conditional.isEmpty())
            s << " && " << refCount.conditional;

        s << ") {" << endl;
    } else {
        if (!refCount.conditional.isEmpty())
            s << INDENT << "if (" << refCount.conditional << ") ";
        s << INDENT << "{" << endl;
    }

    {
        Indentation indent(INDENT);
        switch (refCount.action) {
            case ReferenceCount::Put:
                {
                    QString keyArgumentName = arguments.at(refCount.keyArgument - 1)->argumentName();
                    s << INDENT << refCountVariableName << ".put(" << keyArgumentName << ", " << argumentName << ");" << endl;
                }
                break;
            case ReferenceCount::ClearAdd:
                s << INDENT << refCountVariableName << ".clear();" << endl;
            case ReferenceCount::Add:
                s << INDENT << refCountVariableName << ".add(" << argumentName << ");" << endl;
                break;
            case ReferenceCount::ClearAddAll:
                s << INDENT << refCountVariableName << ".clear();" << endl;
            case ReferenceCount::AddAll:
                s << INDENT << refCountVariableName << ".addAll(" << argumentName << ");" << endl;
                break;
            case ReferenceCount::Remove:
                s << INDENT << "while (" << refCountVariableName << ".remove(" << argumentName << ")) ;" << endl;
                break;
            case ReferenceCount::Set: {
                if (refCount.declareVariable.isEmpty())
                    s << INDENT << refCount.variableName << " = " << argumentName << ";" << endl;
                else
                    s << INDENT << "org.qtjambi.qt.GeneratorUtilities.setField(this, " << refCount.declareVariable << ".class, \"" << refCountVariableName << "\", " << argumentName << ");" << endl;
            }
            default:
                break;
        };
    }
    s << INDENT << "}" << endl;
    if (refCount.action == ReferenceCount::Put) {
        s << INDENT << "else{" << endl;
        {
            Indentation indent(INDENT);
            QString keyArgumentName = arguments.at(refCount.keyArgument - 1)->argumentName();
            s << INDENT << refCountVariableName << ".remove(" << keyArgumentName << ");" << endl;
        }
        s << INDENT << "}" << endl;
    }
}

void JavaGenerator::writeFunction(QTextStream &s, const AbstractMetaFunction *java_function,
                                  uint included_attributes, uint excluded_attributes) {
    s << endl;
    if (java_function->isModifiedRemoved(TypeSystem::TargetLangCode))
        return ;
    if (java_function->hasTemplateArgumentTypes())
        return;
    QString functionName = java_function->name();
    setupForFunction(java_function, &included_attributes, &excluded_attributes);

    if (!java_function->ownerClass()->isInterface()) {
        writeEnumOverload(s, java_function, included_attributes, excluded_attributes);
        if(!java_function->isSignal()){
            writeFunctionOverloads(s, java_function, included_attributes, excluded_attributes);
        }
    }

    static QRegExp regExp("^(insert|set|take|add|remove|install).*");

    if (regExp.exactMatch(java_function->name())) {
        AbstractMetaArgumentList arguments = java_function->arguments();

        const AbstractMetaClass *c = java_function->implementingClass();
        bool hasObjectTypeArgument = false;
        foreach(AbstractMetaArgument *argument, arguments) {
            TypeSystem::Ownership java_ownership = java_function->ownership(c, TypeSystem::TargetLangCode, argument->argumentIndex() + 1);
            TypeSystem::Ownership shell_ownership = java_function->ownership(c, TypeSystem::ShellCode, argument->argumentIndex() + 1);

            if (argument->type()->typeEntry()->isObject()
                    && java_ownership == TypeSystem::InvalidOwnership
                    && shell_ownership == TypeSystem::InvalidOwnership) {
                hasObjectTypeArgument = true;
                break;
            }
        }

        if (hasObjectTypeArgument
                && !java_function->isAbstract()
                && java_function->referenceCounts(java_function->implementingClass()).size() == 0) {
            m_reference_count_candidate_functions.append(java_function);
        }
    }


    if (m_doc_parser) {
        QString signature = functionSignature(java_function,
                                              included_attributes | NoBlockedSlot,
                                              excluded_attributes);
        s << m_doc_parser->documentationForFunction(signature) << endl;
    }

    const QPropertySpec *spec = java_function->propertySpec();
    if (spec && java_function->modifiedName() == java_function->originalName()) {
        if (java_function->isPropertyReader()) {
            s << "    @org.qtjambi.qt.QtPropertyReader(name=\"" << spec->name() << "\")" << endl;
            if (!spec->designable().isEmpty())
                s << "    @org.qtjambi.qt.QtPropertyDesignable(\"" << spec->designable() << "\")" << endl;
            if (!spec->scriptable().isEmpty())
                s << "    @org.qtjambi.qt.QtPropertyScriptable(\"" << spec->scriptable() << "\")" << endl;
            if (!spec->stored().isEmpty())
                s << "    @org.qtjambi.qt.QtPropertyStored(\"" << spec->stored() << "\")" << endl;
//   This seams to be unnecessary in QtJambi
//            if (!spec->revision().isEmpty())
//                s << "    @org.qtjambi.qt.QtPropertyRevision(\"" << spec->revision() << "\")" << endl;
            if (!spec->user().isEmpty())
                s << "    @org.qtjambi.qt.QtPropertyUser(\"" << spec->user() << "\")" << endl;
        } else if (java_function->isPropertyWriter()) {
            s << "    @org.qtjambi.qt.QtPropertyWriter(name=\"" << spec->name() << "\")" << endl;
        } else if (java_function->isPropertyResetter()) {
            s << "    @org.qtjambi.qt.QtPropertyResetter(name=\"" << spec->name() << "\")"
            << endl;
        }
    }

    s << functionSignature(java_function, included_attributes, excluded_attributes);

    if (java_function->isConstructor()) {
        writeConstructorContents(s, java_function);
    } else if (java_function->needsCallThrough()) {
        if (java_function->isAbstract()) {
            s << ";" << endl;
        } else {
            s << INDENT << "{" << endl;
            {
                Indentation indent(INDENT);
                writeJavaCallThroughContents(s, java_function);
            }
            s << INDENT << "}" << endl;
        }

        if((java_function->isAbstract() || !(java_function->originalAttributes() & AbstractMetaAttributes::Public)) && !java_function->implementingClass()->generateShellClass()){
            // do nothing
        }else if(java_function->isAbstract() && java_function->implementingClass()->hasPrivatePureVirtualFunction()){
            // do nothing
        }else if(java_function->jumpTableId() == -1){
            writePrivateNativeFunction(s, java_function);
        }
    } else {
        s << ";" << endl;
    }
}

static void write_equals_parts(QTextStream &s, const AbstractMetaFunctionList &lst, char prefix, bool *first) {
    foreach(AbstractMetaFunction *f, lst) {
        AbstractMetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(1);
        if (type.isEmpty())
            type = arg->type()->typeEntry()->qualifiedTargetLangName();
        type.replace("$", ".");
        s << INDENT << (*first ? "if" : "else if") << " (other instanceof " << type << ")" << endl
        << INDENT << "    return ";
        if (prefix != 0) s << prefix;
        s << f->name() << "((" << type << ") other);" << endl;
        *first = false;
    }
}

static void write_compareto_parts(QTextStream &s, const AbstractMetaFunctionList &lst, int value, bool *first) {
    foreach(AbstractMetaFunction *f, lst) {
        AbstractMetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(1);
        if (type.isEmpty())
            type = arg->type()->typeEntry()->qualifiedTargetLangName();
        type.replace("$", ".");
        s << INDENT << (*first ? "if" : "else if") << " (other instanceof " << type << ") {" << endl
        << INDENT << "    if (" << f->name() << "((" << type << ") other)) return " << value << ";" << endl
        << INDENT << "    else return " << -value << ";" << endl
        << INDENT << "}" << endl;
        *first = false;
    }
    s << INDENT << "throw new ClassCastException();" << endl;
}

bool JavaGenerator::isComparable(const AbstractMetaClass *cls) const {
    AbstractMetaFunctionList eq_functions = cls->equalsFunctions();
    AbstractMetaFunctionList neq_functions = cls->notEqualsFunctions();

    // Write the comparable functions
    AbstractMetaFunctionList ge_functions = cls->greaterThanFunctions();
    AbstractMetaFunctionList geq_functions = cls->greaterThanEqFunctions();
    AbstractMetaFunctionList le_functions = cls->lessThanFunctions();
    AbstractMetaFunctionList leq_functions = cls->lessThanEqFunctions();

    bool hasEquals = eq_functions.size() || neq_functions.size();
    bool isComparable = hasEquals
                        ? ge_functions.size() || geq_functions.size() || le_functions.size() || leq_functions.size()
                        : geq_functions.size() == 1 && leq_functions.size() == 1;

    return isComparable;
}


void JavaGenerator::writeJavaLangObjectOverrideFunctions(QTextStream &s,
        const AbstractMetaClass *cls) {
    AbstractMetaFunctionList eq_functions = cls->equalsFunctions();
    AbstractMetaFunctionList neq_functions = cls->notEqualsFunctions();

    if (eq_functions.size() || neq_functions.size()) {
        s << endl
        << INDENT << "@SuppressWarnings(\"unchecked\")" << endl
        << INDENT << "@Override" << endl
        << INDENT << "public boolean equals(Object other) {" << endl;
        bool first = true;
        write_equals_parts(s, eq_functions, (char) 0, &first);
        write_equals_parts(s, neq_functions, '!', &first);
        s << INDENT << "    return false;" << endl
        << INDENT << "}" << endl << endl;
    }

    // Write the comparable functions
    AbstractMetaFunctionList ge_functions = cls->greaterThanFunctions();
    AbstractMetaFunctionList geq_functions = cls->greaterThanEqFunctions();
    AbstractMetaFunctionList le_functions = cls->lessThanFunctions();
    AbstractMetaFunctionList leq_functions = cls->lessThanEqFunctions();

    bool hasEquals = eq_functions.size() || neq_functions.size();
    bool comparable = isComparable(cls);
    if (comparable) {
        s << INDENT << "public int compareTo(Object other) {" << endl;
        {
            Indentation indent(INDENT);
            if (hasEquals) {
                s << INDENT << "if (equals(other)) return 0;" << endl;
                bool first = false;
                if (le_functions.size()) {
                    write_compareto_parts(s, le_functions, -1, &first);
                } else if (ge_functions.size()) {
                    write_compareto_parts(s, ge_functions, 1, &first);
                } else if (leq_functions.size()) {
                    write_compareto_parts(s, leq_functions, -1, &first);
                } else if (geq_functions.size()) {
                    write_compareto_parts(s, geq_functions, 1, &first);
                }

            } else if (le_functions.size() == 1) {
                QString className = cls->typeEntry()->qualifiedTargetLangName();
                s << INDENT << "if (operator_less((" << className << ") other)) return -1;" << endl
                << INDENT << "else if (((" << className << ") other).operator_less(this)) return 1;" << endl
                << INDENT << "else return 0;" << endl;

            } else if (geq_functions.size() == 1 && leq_functions.size()) {
                QString className = cls->typeEntry()->qualifiedTargetLangName();
                s << INDENT << "boolean less = operator_less_or_equal((" << className << ") other);" << endl
                << INDENT << "boolean greater = operator_greater_or_equal((" << className << ") other);" << endl
                << INDENT << "if (less && greater) return 0;" << endl
                << INDENT << "else if (less) return -1;" << endl
                << INDENT << "else return 1;" << endl;
            }
        }

        s << INDENT << "}" << endl;
    }


    if (cls->hasHashFunction() || eq_functions.size() > 0 || neq_functions.size() > 0) {
        AbstractMetaFunctionList hashcode_functions = cls->queryFunctionsByName("hashCode");
        bool found = false;
        foreach(const AbstractMetaFunction *function, hashcode_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            if (cls->hasHashFunction()) {
                s << endl
                << INDENT << "@Override" << endl
                << INDENT << "public int hashCode() {" << endl
                << INDENT << "    if (nativeId() == 0)" << endl
                << INDENT << "        throw new QNoNativeResourcesException(\"Function call on incomplete object of type: \" +getClass().getName());" << endl
                << INDENT << "    return __qt_hashCode(nativeId());" << endl
                << INDENT << "}" << endl
                << INDENT << "native int __qt_hashCode(long __this_nativeId);" << endl;
            } else { // We have equals() but no qHash(), we return 0 from hashCode() to respect
                // contract of java.lang.Object
                s << endl
                << INDENT << "@Override" << endl
                << INDENT << "public int hashCode() { return 0; }" << endl;
            }
        }
    }

    // Qt has a standard toString() conversion in QVariant?
    QVariant::Type type = QVariant::nameToType(cls->qualifiedCppName().toLatin1());
    if (type<QVariant::LastCoreType && QVariant(type).canConvert(QVariant::String) &&  !cls->hasToStringCapability()) {
        AbstractMetaFunctionList tostring_functions = cls->queryFunctionsByName("toString");
        bool found = false;
        foreach(const AbstractMetaFunction *function, tostring_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            s << endl
            << INDENT << "@Override" << endl
            << INDENT << "public String toString() {" << endl
            << INDENT << "    if (nativeId() == 0)" << endl
            << INDENT << "        throw new QNoNativeResourcesException(\"Function call on incomplete object of type: \" +getClass().getName());" << endl
            << INDENT << "    return __qt_toString(nativeId());" << endl
            << INDENT << "}" << endl
            << INDENT << "native String __qt_toString(long __this_nativeId);" << endl;
        }
    }
}

void JavaGenerator::writeEnumOverload(QTextStream &s, const AbstractMetaFunction *java_function,
                                      uint include_attributes, uint exclude_attributes) {
    AbstractMetaArgumentList arguments = java_function->arguments();

    if ((java_function->implementingClass() != java_function->declaringClass())
            || ((!java_function->isNormal() && !java_function->isConstructor()) || java_function->isEmptyFunction() || java_function->isAbstract())) {
        return ;
    }


    int option = 0;
    if (java_function->isConstructor())
        option = Option(option | SkipReturnType);
    else
        include_attributes |= AbstractMetaAttributes::FinalInTargetLang;

    int generate_enum_overload = -1;
    for (int i = 0; i < arguments.size(); ++i)
        generate_enum_overload = arguments.at(i)->type()->isTargetLangFlags() ? i : -1;

    if (generate_enum_overload >= 0) {
        if (m_doc_parser) {
            // steal documentation from main function
            QString signature = functionSignature(java_function,
                                                  include_attributes | NoBlockedSlot,
                                                  exclude_attributes);
            s << m_doc_parser->documentationForFunction(signature) << endl;
        }

        s << endl;

        writeFunctionAttributes(s, java_function, include_attributes, exclude_attributes, option);
        s << java_function->name() << "(";
        if (generate_enum_overload > 0) {
            writeFunctionArguments(s, java_function, generate_enum_overload);
            s << ", ";
        }

        // Write the ellipsis convenience argument
        AbstractMetaArgument *affected_arg = arguments.at(generate_enum_overload);
        EnumTypeEntry *originator = ((FlagsTypeEntry *)affected_arg->type()->typeEntry())->originator();

        s << originator->javaPackage().replace("$",".")
          << "." << originator->javaQualifier().replace("$",".")
          << "." << originator->targetLangName().replace("$",".")
        << " ... " << affected_arg->argumentName() << ") {" << endl;

        s << "        ";
        QString new_return_type = java_function->typeReplaced(0);
        if (new_return_type != "void" && (!new_return_type.isEmpty() || java_function->type() != 0))
            s << "return ";

        if (java_function->isConstructor()) {
            s << "this";
        } else {
            if (java_function->isStatic())
                s << java_function->implementingClass()->fullName() << ".";
            else
                s << "this.";
            s << java_function->name();
        }

        s << "(";
        for (int i = 0; i < generate_enum_overload; ++i) {
            s << arguments.at(i)->argumentName() << ", ";
        }
        s << "new " << affected_arg->type()->fullName().replace("$", ".") << "(" << affected_arg->argumentName() << "));" << endl
        << "    }" << endl;
    }
}

void JavaGenerator::writeInstantiatedType(QTextStream &s, const AbstractMetaType *abstractMetaType, bool forceBoxed) const {
    Q_ASSERT(abstractMetaType != 0);

    const TypeEntry *type = abstractMetaType->typeEntry();
    /* avoid output like java.util.List<int>*/
    if(forceBoxed){
        if(type->qualifiedTargetLangName()=="int"){
            s << "java.lang.Integer";
        }else if(type->qualifiedTargetLangName()=="boolean"){
            s << "java.lang.Boolean";
        }else if(type->qualifiedTargetLangName()=="short"){
            s << "java.lang.Short";
        }else if(type->qualifiedTargetLangName()=="char"){
            s << "java.lang.Character";
        }else if(type->qualifiedTargetLangName()=="byte"){
            s << "java.lang.Byte";
        }else if(type->qualifiedTargetLangName()=="long"){
            s << "java.lang.Long";
        }else if(type->qualifiedTargetLangName()=="double"){
            s << "java.lang.Double";
        }else if(type->qualifiedTargetLangName()=="float"){
            s << "java.lang.Float";
        }else{
            s << type->qualifiedTargetLangName().replace("$", ".");
        }
    }else{
        s << type->qualifiedTargetLangName().replace("$", ".");
    }

    if (abstractMetaType->hasInstantiations()) {
        s << "<";
        QList<AbstractMetaType *> instantiations = abstractMetaType->instantiations();
        for (int i = 0; i < instantiations.size(); ++i) {
            if (i > 0)
                s << ", ";

            writeInstantiatedType(s, instantiations.at(i), true);
        }
        s << ">";
    }
}

void JavaGenerator::writeFunctionOverloads(QTextStream &s, const AbstractMetaFunction *java_function,
        uint include_attributes, uint exclude_attributes) {
    AbstractMetaArgumentList arguments = java_function->arguments();
    int argument_count = arguments.size();

    // We only create the overloads for the class that actually declares the function
    // unless this is an interface, in which case we create the overloads for all
    // classes that directly implement the interface.
    const AbstractMetaClass *decl_class = java_function->declaringClass();
    if (decl_class->isInterface()) {
        AbstractMetaClassList interfaces = java_function->implementingClass()->interfaces();
        foreach(AbstractMetaClass *iface, interfaces) {
            if (iface == decl_class) {
                decl_class = java_function->implementingClass();
                break;
            }
        }
    }
    if (decl_class != java_function->implementingClass())
        return;

    // Figure out how many functions we need to write out,
    // One extra for each default argument.
    int overload_count = 0;
    uint excluded_attributes = AbstractMetaAttributes::Abstract
                               | AbstractMetaAttributes::Native
                               | exclude_attributes;
    uint included_attributes = (java_function->isConstructor() ? 0 : AbstractMetaAttributes::Final) | include_attributes;

    for (int i = 0; i < argument_count; ++i) {
        if (!arguments.at(i)->defaultValueExpression().isEmpty() && !java_function->argumentRemoved(i + 1))
            ++overload_count;
    }
    Q_ASSERT(overload_count <= argument_count);
    for (int i = 0; i < overload_count; ++i) {
        int used_arguments = argument_count - i - 1;

        QString signature = functionSignature(java_function, included_attributes,
                                              excluded_attributes,
                                              java_function->isEmptyFunction()
                                              || java_function->isNormal()
                                              || java_function->isSignal() ? NoOption
                                              : SkipReturnType,
                                              used_arguments);

        s << endl;
        if (m_doc_parser) {
            s << m_doc_parser->documentationForFunction(signature) << endl;
        }

        s << signature << " {\n        ";
        QString new_return_type = java_function->typeReplaced(0);
        if (new_return_type != "void" && (!new_return_type.isEmpty() || java_function->type()))
            s << "return ";
        if (java_function->isConstructor())
            s << "this";
        else
            s << java_function->name();
        s << "(";

        int written_arguments = 0;
        for (int j = 0; j < argument_count; ++j) {
            if (!java_function->argumentRemoved(j + 1)) {
                if (written_arguments++ > 0)
                    s << ", ";

                if (j < used_arguments) {
                    s << arguments.at(j)->argumentName();
                } else {
                    AbstractMetaType *arg_type = 0;
                    QString modified_type = java_function->typeReplaced(j + 1);
                    if (modified_type.isEmpty()) {
                        arg_type = arguments.at(j)->type();
                        if (arg_type->isNativePointer()) {
                            s << "(org.qtjambi.qt.QNativePointer)";
                        } else {
                            const AbstractMetaType *abstractMetaType = arguments.at(j)->type();
                            const TypeEntry *type = abstractMetaType->typeEntry();
                            if (type->designatedInterface())
                                type = type->designatedInterface();
                            if (!type->isEnum() && !type->isFlags()) {
                                s << "(";
                                writeInstantiatedType(s, abstractMetaType, false);
                                s << ")";
                            }
                        }
                    } else {
                        s << "(" << modified_type.replace('$', '.') << ")";
                    }

                    QString defaultExpr = arguments.at(j)->defaultValueExpression();
                    int pos = defaultExpr.indexOf(".");
                    if (pos > 0) {
                        QString someName = defaultExpr.left(pos);
                        ComplexTypeEntry *ctype =
                            TypeDatabase::instance()->findComplexType(someName);
                        QString replacement;
                        if (ctype != 0 && ctype->isVariant())
                            replacement = "org.qtjambi.qt.QVariant.";
                        else if (ctype != 0)
                            replacement = ctype->javaPackage() + "." + ctype->targetLangName() + ".";
                        else
                            replacement = someName + ".";
                        defaultExpr = defaultExpr.replace(someName + ".", replacement);
                    }

                    // Check global command flag for targetting ARM then enable this kludge
                    /* this was nonsense. ARM API has full doubles that are cast into floats internally.
                    if(Wrapper::isTargetPlatformArmCpu) {
                        if(fileNameForClass(decl_class).compare(QLatin1String("QAbstractTextDocumentLayout.java")) == 0
                                                         && java_function->name().compare(QLatin1String("update")) == 0) {
                            defaultExpr = arm_platform_kludge_defaultValue(defaultExpr);
                        }
                    }*/

                    if (arg_type != 0 && arg_type->isFlags()) {
                        s << "new " << arg_type->fullName().replace("$", ".") << "(" << defaultExpr << ")";
                    } else {
                        s << defaultExpr;
                    }
                }
            }
        }
        s << ");\n    }" << endl;
    }
}

void JavaGenerator::write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level) {
    if(java_class->enclosingClass() && nesting_level==0){ // don't write nested classes into own file
        return;
    }

    ReportHandler::debugSparse("Generating class: " + java_class->fullName());
    bool fakeClass = java_class->attributes() & AbstractMetaAttributes::Fake;

    if(nesting_level==0){
       if (m_docs_enabled) {
           m_doc_parser = new DocParser(m_doc_directory + "/" + java_class->name().toLower() + ".jdoc");
       }
        s << INDENT << "package " << java_class->package() << ";" << endl << endl;

        s << INDENT << "import org.qtjambi.qt.*;" << endl << endl;

        QList<Include> includes = java_class->typeEntry()->extraIncludes();
        foreach(const Include &inc, includes) {
            if (inc.type == Include::TargetLangImport) {
                s << inc.toString() << endl;
            }
        }
        s << endl;

    }
   if (m_doc_parser) {
       s << INDENT << m_doc_parser->documentation(java_class) << endl << endl;
   }

    s << INDENT << "@QtJambiGeneratedClass" << endl;

    if ((java_class->typeEntry()->typeFlags() & ComplexTypeEntry::Deprecated) != 0) {
        s << INDENT << "@Deprecated" << endl;
    }

    s << INDENT;

    if (java_class->isInterface()) {
        s << "public interface ";
    } else {
        if (java_class->isPublic())
            s << "public ";
        // else friendly

        if(nesting_level>0){
            s << "static ";
        }

        bool force_abstract = (java_class->typeEntry()->typeFlags() & ComplexTypeEntry::ForceAbstract) != 0;
        if (java_class->isFinal() && !force_abstract)
            s << "final ";
        if ((java_class->isAbstract() && !java_class->isNamespace()) || force_abstract)
            s << "abstract ";

        if (!java_class->typeEntry()->targetType().isEmpty()) {
            s << java_class->typeEntry()->targetType() << " ";
        } else if (java_class->isNamespace() && java_class->functionsInTargetLang().size() == 0) {
            s << "interface ";
        } else if (java_class->isNamespace()) {
            s << "class ";
        } else {
            s << "class ";
        }

    }

    const ComplexTypeEntry *type = java_class->typeEntry();

    s << java_class->simpleName();

    if (type->isGenericClass()) {
        s << "<";
        QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
        for (int i = 0; i < templateArguments.size(); ++i) {
            TypeEntry *templateArgument = templateArguments.at(i);
            if (i > 0)
                s << ", ";
            s << templateArgument->name().replace("$", ".");
        }
        s << ">";
    }

    if (!java_class->isNamespace() && !java_class->isInterface()) {
        if (!java_class->baseClassName().isEmpty()) {
            s << " extends " << java_class->baseClass()->fullName().replace("$",".");
        } else {
            QString sc = type->defaultSuperclass().replace("$",".");

            if (!sc.isEmpty())
                s << " extends " << sc;
        }
    } else if (java_class->isInterface()) {
        s << " extends QtJambiInterface";
    }

    // implementing interfaces...
    bool implements = false;
    AbstractMetaClassList interfaces = java_class->interfaces();

    if (!interfaces.isEmpty()) {
        if (java_class->isInterface())
            s << ", ";
        else {
            s << endl << INDENT << "    implements ";
            implements = true;
        }
        for (int i = 0; i < interfaces.size(); ++i) {
            AbstractMetaClass *iface = interfaces.at(i);
            if (i != 0)
                s << "," << endl << INDENT << "            ";
            s << iface->package() << "." << iface->name().replace("$", ".");
        }
    }

    if (isComparable(java_class)) {
        if (!implements) {
            implements = true;
            s << endl << INDENT << "    implements ";
        } else {
            s << "," << endl << INDENT << "            ";
        }
        s << "java.lang.Comparable<Object>";
    }

    if (java_class->hasCloneOperator()) {
        if (!implements) {
            implements = true;
            s << endl << INDENT << "    implements ";
        } else {
            s << "," << endl << INDENT << "            ";
        }
        s << "java.lang.Cloneable";
    }

    if (!java_class->typeEntry()->implements().isEmpty()) {
        if (!implements) {
            implements = true;
            s << endl << INDENT << "    implements ";
        } else {
            s << "," << endl << INDENT << "            ";
        }
        s << java_class->typeEntry()->implements();
    }

    s << endl << INDENT << "{" << endl << INDENT;

    {
        Indentation indent(INDENT);

        // Define variables for reference count mechanism
        if (!java_class->isInterface() && !java_class->isNamespace()) {
            QHash<QString, int> variables;
            foreach(AbstractMetaFunction *function, java_class->functions()) {
                QList<ReferenceCount> referenceCounts = function->referenceCounts(java_class);
                foreach(ReferenceCount refCount, referenceCounts) {
                    variables[refCount.variableName] |=
                        refCount.action
                        | refCount.access
                        | (refCount.threadSafe ? ReferenceCount::ThreadSafe : 0)
                        | (function->isStatic() ? ReferenceCount::Static : 0)
                        | (refCount.declareVariable.isEmpty() ? ReferenceCount::DeclareVariable : 0);
                }
            }

            foreach(QString variableName, variables.keys()) {
                int actions = variables.value(variableName) & ReferenceCount::ActionsMask;
                bool threadSafe = variables.value(variableName) & ReferenceCount::ThreadSafe;
                bool isStatic = variables.value(variableName) & ReferenceCount::Static;
                bool declareVariable = variables.value(variableName) & ReferenceCount::DeclareVariable;
                int access = variables.value(variableName) & ReferenceCount::AccessMask;

                if (actions == ReferenceCount::Ignore || !declareVariable)
                    continue;

                if (((actions & ReferenceCount::Add) == 0) != ((actions & ReferenceCount::Remove) == 0)
                        && !(actions & ReferenceCount::ClearAdd)&& !(actions & ReferenceCount::ClearAddAll)) {
                    QString warn = QString("either add or remove specified for reference count variable '%1' in '%2' but not both")
                                   .arg(variableName).arg(java_class->fullName());
                    ReportHandler::warning(warn);
                }
                s << endl;

                if (TypeDatabase::instance()->includeEclipseWarnings())
                    s << INDENT << "@SuppressWarnings(\"unused\")" << endl;

                s << INDENT;
                switch (access) {
                    case ReferenceCount::Private:
                        s << "private "; break;
                    case ReferenceCount::Protected:
                        s << "protected "; break;
                    case ReferenceCount::Public:
                        s << "public "; break;
                    default:
                        ; // friendly
                }

                if (isStatic)
                    s << "static ";

                if (actions == ReferenceCount::Put){
                    s << "java.util.Map<Object,Object> " << variableName << " = ";

                    if (threadSafe)
                        s << "java.util.Collections.synchronizedMap(";
                    s << "new java.util.HashMap<Object,Object>()";
                    if (threadSafe)
                        s << ")";
                    s << ";" << endl;
                }else if (actions != ReferenceCount::Set && actions != ReferenceCount::Ignore) {
                    s << "java.util.Collection<Object> " << variableName << " = ";

                    if (threadSafe)
                        s << "java.util.Collections.synchronizedCollection(";
                    s << "new java.util.ArrayList<Object>()";
                    if (threadSafe)
                        s << ")";
                    s << ";" << endl;
                } else if (actions != ReferenceCount::Ignore) {

                    if (threadSafe)
                        s << "synchronized ";
                    s << "Object " << variableName << " = null;" << endl;
                }
            }
        }

        if (!java_class->isInterface() && (!java_class->isNamespace() || java_class->functionsInTargetLang().size() > 0)
                && (java_class->baseClass() == 0 || java_class->package() != java_class->baseClass()->package())) {
            s << endl
            << INDENT << "static {" << endl;

            if (java_class->isNamespace()) {
                s << INDENT << "    org.qtjambi.qt.QtJambi_LibraryInitializer.init();" << endl;
            }

            s << INDENT << "    " << java_class->package() << ".QtJambi_LibraryInitializer.init();" << endl
            << INDENT << "}" << endl;
        }

        if (!java_class->isInterface() && java_class->isAbstract()) {
            s << endl;

            if (TypeDatabase::instance()->includeEclipseWarnings())
                s << INDENT << "@SuppressWarnings(\"unused\")" << endl;

            s << INDENT << "private static class ConcreteWrapper extends " << java_class->fullName().replace("$", ".") << " {" << endl;

            {
                Indentation indent(INDENT);
                s << INDENT << "protected ConcreteWrapper(QPrivateConstructor p) { super(p); }" << endl;

                uint exclude_attributes = AbstractMetaAttributes::Native | AbstractMetaAttributes::Abstract;
                uint include_attributes = 0;
                AbstractMetaFunctionList functions = java_class->queryFunctions(AbstractMetaClass::NormalFunctions | AbstractMetaClass::AbstractFunctions | AbstractMetaClass::NonEmptyFunctions | AbstractMetaClass::NotRemovedFromTargetLang);
                foreach(const AbstractMetaFunction *java_function, functions) {
                    if(!java_function->isPrivate()){
                        retrieveModifications(java_function, java_class, &exclude_attributes, &include_attributes);

                        s << endl
                        << INDENT << "@Override" << endl;
                        writeFunctionAttributes(s, java_function, include_attributes, exclude_attributes,
                                                java_function->isNormal() || java_function->isSignal() ? 0 : SkipReturnType);

                        s << java_function->name() << "(";
                        writeFunctionArguments(s, java_function, java_function->arguments().count());
                        s << ") {" << endl;
                        {
                            Indentation indent(INDENT);
                            writeJavaCallThroughContents(s, java_function, SuperCall);
                        }
                        s << INDENT << "}" << endl;
                    }
                }
            }
            s  << INDENT << "}" << endl << endl;
        }

        // Enums
        foreach(AbstractMetaEnum *java_enum, java_class->enums())
            writeEnum(s, java_enum);
        if (!java_class->enums().isEmpty() && !java_class->enclosedClasses().isEmpty())
            s << endl;
        // write enclosed types as static embedded classes
        foreach(AbstractMetaClass *enclosed_java_class, java_class->enclosedClasses()){
            write(s, enclosed_java_class, nesting_level+1);
        }
        if (!java_class->enclosedClasses().isEmpty() && !java_class->functions().isEmpty())
            s << endl;

        // Signals
        AbstractMetaFunctionList signal_funcs = java_class->queryFunctions(AbstractMetaClass::Signals
                                                | AbstractMetaClass::ClassImplements
                                                | AbstractMetaClass::NotRemovedFromTargetLang);
        for (int i = 0; i < signal_funcs.size(); ++i)
            writeSignal(s, signal_funcs.at(i));

        // Class has subclasses but also only private constructors
        if (java_class->hasPrivatePureVirtualFunction()) {
            s << endl << INDENT << "/**" << endl
            << INDENT << " * This constructor is a place holder intended to prevent" << endl
            << INDENT << " * users from subclassing the class. Certain classes can" << endl
            << INDENT << " * unfortunately only be subclasses internally. The constructor" << endl
            << INDENT << " * will indiscriminately throw an exception if called. If the" << endl
            << INDENT << " * exception is ignored, any use of the constructed object will" << endl
            << INDENT << " * cause an exception to occur." << endl << endl
            << INDENT << " * @throws QClassCannotBeSubclassedException" << endl
            << INDENT << " **/" << endl
            << INDENT << "private " << java_class->simpleName() << "() throws QClassCannotBeSubclassedException {" << endl
            << INDENT << "    super((QPrivateConstructor)null);" << endl
            << INDENT << "    throw new QClassCannotBeSubclassedException(" << java_class->simpleName() << ".class);" << endl
            << INDENT << "}" << endl << endl;
        }

        // Functions
        bool alreadyHasCloneMethod = false;
        AbstractMetaFunctionList java_funcs = java_class->functionsInTargetLang();
        for (int i = 0; i < java_funcs.size(); ++i) {
            AbstractMetaFunction *function = java_funcs.at(i);

            // If a method in an interface class is modified to be private or protected, this should
            // not be present in the interface at all, only in the implementation.
            if (java_class->isInterface()) {
                uint includedAttributes = 0;
                uint excludedAttributes = 0;
                retrieveModifications(function, java_class, &excludedAttributes, &includedAttributes);
                if (includedAttributes & AbstractMetaAttributes::Private
                        || includedAttributes & AbstractMetaAttributes::Protected
                        || !function->isPublic()
                        || function->isStatic())
                    continue;
            }

            if (function->name() == "clone" && function->arguments().isEmpty())
                alreadyHasCloneMethod = true;

            if(java_class->hasPrivatePureVirtualFunction() && function->isConstructor())
                continue;

            if(java_class->generateShellClass() || function->isPublic())
                writeFunction(s, function);
        }

        // Just the private functions for abstract functions implemented in superclasses
        if (!java_class->isInterface() &&
                java_class->isAbstract() &&
                !java_class->hasPrivatePureVirtualFunction()) {
            java_funcs = java_class->queryFunctions(AbstractMetaClass::NormalFunctions |
                                                    AbstractMetaClass::AbstractFunctions |
                                                    AbstractMetaClass::NotRemovedFromTargetLang);
            foreach(AbstractMetaFunction *java_function, java_funcs) {
                if (java_function->implementingClass() != java_class) {
                    s << endl;
                    writePrivateNativeFunction(s, java_function);
                }
            }
        }

        // Field accessors
        AbstractMetaFieldList fields = java_class->fields();
        foreach(const AbstractMetaField *field, fields) {
            if (field->wasPublic() || (field->wasProtected() && !java_class->isFinal())) {
                writeFieldAccessors(s, field);
            }
        }

        // the static fromNativePointer function...
        if (!java_class->isNamespace() && !java_class->isInterface() && !fakeClass) {
            s << endl
            << INDENT << "public static native " << java_class->simpleName() << " fromNativePointer("
            << "QNativePointer nativePointer);" << endl;
        }

        if (java_class->isQObject()) {
            s << endl;
            if (TypeDatabase::instance()->includeEclipseWarnings())
                s << INDENT << "@SuppressWarnings(\"unused\")" << endl;

            s << INDENT << "private static native long originalMetaObject();" << endl;
        }

        // The __qt_signalInitialization() function
        if (signal_funcs.size() > 0) {
            s << endl
            << INDENT << "@Override" << endl
            << INDENT << "@QtBlockedSlot protected boolean __qt_signalInitialization(String name) {" << endl
            << INDENT << "    return (__qt_signalInitialization(nativeId(), name)" << endl
            << INDENT << "            || super.__qt_signalInitialization(name));" << endl
            << INDENT << "}" << endl
            << endl
            << INDENT << "@QtBlockedSlot" << endl
            << INDENT << "private native boolean __qt_signalInitialization(long ptr, String name);" << endl;
        }

        // Add dummy constructor for use when constructing subclasses
        if (!java_class->isNamespace() && !java_class->isInterface() && !fakeClass) {
            s << endl
            << INDENT << "protected "
            << java_class->simpleName()
            << "(QPrivateConstructor p) { super(p); } "
            << endl;
        }

        // Add a function that converts an array of the value type to a QNativePointer
        if (java_class->typeEntry()->isValue() && !fakeClass) {
            s << endl
            << INDENT << "public static native QNativePointer nativePointerArray(" << java_class->simpleName()
            << " array[]);" << endl;
        }

        // write the cast to this function....
        if (java_class->isInterface()) {
            s << endl
            << "    public long __qt_cast_to_"
            << static_cast<const InterfaceTypeEntry *>(type)->origin()->targetLangName()
            << "(long ptr);" << endl;
        } else {
            foreach(AbstractMetaClass *cls, interfaces) {
                s << endl
                << "    @QtBlockedSlot public native long __qt_cast_to_"
                << static_cast<const InterfaceTypeEntry *>(cls->typeEntry())->origin()->targetLangName()
                << "(long ptr);" << endl;
            }
        }

        writeJavaLangObjectOverrideFunctions(s, java_class);
        writeExtraFunctions(s, java_class);
        writeToStringFunction(s, java_class);

        if (java_class->hasCloneOperator() && !alreadyHasCloneMethod) {
            writeCloneFunction(s, java_class);
        }
    }

    s << INDENT << "}" << endl;

    if (m_docs_enabled) {
        delete m_doc_parser;
        m_doc_parser = 0;
    }
}

void JavaGenerator::generate() {
    Generator::generate();

    { //log native pointer api
        const AbstractMetaClass *last_class = 0;
        QString fileName("mjb_nativepointer_api.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            AbstractMetaFunctionList nativepointer_functions;
            for (int i = 0; i < m_nativepointer_functions.size(); ++i) {
                AbstractMetaFunction *f =
                    const_cast<AbstractMetaFunction *>(m_nativepointer_functions[i]);
                if (f->ownerClass() == f->declaringClass() || f->isFinal())
                    nativepointer_functions.append(f);
            }

            s << "Number of public or protected functions with QNativePointer API: " <<
            nativepointer_functions.size() << endl;
            foreach(const AbstractMetaFunction *f, nativepointer_functions) {
                if (last_class != f->ownerClass()) {
                    last_class = f->ownerClass();
                    s << endl << endl << "Class " << last_class->name().replace("$", ".") << ":" << endl;
                    s << "---------------------------------------------------------------------------------"
                    << endl;
                }

                s << f->minimalSignature() << endl;
            }

            m_nativepointer_functions.clear();
        }
    }

    { // log object type usage of classes
        const AbstractMetaClass *last_class = 0;
        QString fileName("mjb_nativepointer_api.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            AbstractMetaFunctionList resettable_object_functions;
            for (int i = 0; i < m_resettable_object_functions.size(); ++i) {
                AbstractMetaFunction *f =
                    const_cast<AbstractMetaFunction *>(m_resettable_object_functions[i]);
                if (f->ownerClass() == f->declaringClass() || f->isFinal())
                    resettable_object_functions.append(f);
            }

            s << "Number of public or protected functions that return a " <<
            "non-QObject object type, or that are virtual and take " <<
            "a non-QObject object type argument: " <<
            resettable_object_functions.size() << endl;
            foreach(const AbstractMetaFunction *f, resettable_object_functions) {
                if (last_class != f->ownerClass()) {
                    last_class = f->ownerClass();
                    s << endl << endl << "Class " << last_class->name().replace("$", ".") << ":" << endl;
                    s << "---------------------------------------------------------------------------------"
                    << endl;
                }

                s << f->minimalSignature() << endl;
            }

            m_resettable_object_functions.clear();
        }
    }

    { // log possible reference counting candidates
        QString fileName("mjb_reference_count_candidates.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            s << "The following functions have a signature pattern which may imply that" << endl
            << "they need to apply reference counting to their arguments ("
            << m_reference_count_candidate_functions.size() << " functions) : " << endl;

            foreach(const AbstractMetaFunction *f, m_reference_count_candidate_functions) {
                s << f->implementingClass()->fullName() << " : " << f->minimalSignature() << endl;
            }
        }
        file.close();
    }
}

void JavaGenerator::writeFunctionAttributes(QTextStream &s, const AbstractMetaFunction *java_function,
        uint included_attributes, uint excluded_attributes,
        uint options) {
    uint attr = (java_function->attributes() & (~excluded_attributes)) | included_attributes;

    if ((attr & AbstractMetaAttributes::Public) || (attr & AbstractMetaAttributes::Protected)) {

        // Does the function use native pointer API?
        bool nativePointer = java_function->type() && java_function->type()->isNativePointer()
                             && java_function->typeReplaced(0).isEmpty();

        // Does the function need to be considered for resetting the Java objects after use?
        bool resettableObject = false;

        if (!nativePointer
                && java_function->type()
                && java_function->type()->hasInstantiations()
                && java_function->typeReplaced(0).isEmpty()) {

            QList<AbstractMetaType *> instantiations = java_function->type()->instantiations();

            foreach(const AbstractMetaType *type, instantiations) {
                if (type && type->isNativePointer()) {
                    nativePointer = true;
                    break;
                }
            }

        }

        AbstractMetaArgumentList arguments = java_function->arguments();
        if (!nativePointer || (!resettableObject && !java_function->isFinal())) {
            foreach(const AbstractMetaArgument *argument, arguments) {
                if (!java_function->argumentRemoved(argument->argumentIndex() + 1)
                        && java_function->typeReplaced(argument->argumentIndex() + 1).isEmpty()) {

                    if (argument->type()->isNativePointer()) {

                        nativePointer = true;
                        if (resettableObject) break ;

                    } else if (!java_function->isFinalInTargetLang()
                               && argument->type()->isObject()
                               && !argument->type()->isQObject()
                               && !java_function->resetObjectAfterUse(argument->argumentIndex() + 1)
                               && java_function->ownership(java_function->declaringClass(),
                                        TypeSystem::ShellCode, argument->argumentIndex() + 1) ==
                                            TypeSystem::InvalidOwnership) {

                        resettableObject = true;
                        if (nativePointer) break ;

                    } else if (argument->type()->hasInstantiations()) {

                        QList<AbstractMetaType *> instantiations = argument->type()->instantiations();
                        foreach(AbstractMetaType *type, instantiations) {
                            if (type && type->isNativePointer()) {
                                nativePointer = true;
                                if (resettableObject) break;
                            } else if (!java_function->isFinal()
                                       && type
                                       && type->isObject()
                                       && !type->isQObject()
                                       && !java_function->resetObjectAfterUse(argument->argumentIndex() + 1)) {
                                resettableObject = true;
                                if (nativePointer) break ;
                            }
                        }

                        if (nativePointer && resettableObject)
                            break;

                    }
                }
            }
        }

        if (nativePointer && !m_nativepointer_functions.contains(java_function))
            m_nativepointer_functions.append(java_function);
        if (resettableObject && !m_resettable_object_functions.contains(java_function))
            m_resettable_object_functions.append(java_function);
    }

    if ((options & SkipAttributes) == 0) {
        if (java_function->isEmptyFunction()
                || java_function->isDeprecated()) s << INDENT << "@Deprecated" << endl;

        bool needsSuppressUnusedWarning = TypeDatabase::instance()->includeEclipseWarnings()
                                          && java_function->isSignal()
                                          && (((excluded_attributes & AbstractMetaAttributes::Private) == 0)
                                              && (java_function->isPrivate()
                                                  || ((included_attributes & AbstractMetaAttributes::Private) != 0)));

        if (needsSuppressUnusedWarning && java_function->needsSuppressUncheckedWarning()) {
            s << INDENT << "@SuppressWarnings({\"unchecked\", \"unused\"})" << endl;
        } else if (java_function->needsSuppressUncheckedWarning()) {
            s << INDENT << "@SuppressWarnings(\"unchecked\")" << endl;
        } else if (needsSuppressUnusedWarning) {
            s << INDENT << "@SuppressWarnings(\"unused\")" << endl;
        }

        const QPropertySpec *spec = java_function->propertySpec();
        if (spec && java_function->isPropertyNotify()) {
            s << "    @org.qtjambi.qt.QtPropertyNotify(name=\"" << spec->name() << "\")" << endl;
        }

        if (!(attr & NoBlockedSlot)
                && !java_function->isAllowedAsSlot()
                && !java_function->isConstructor()
                && !java_function->isSlot()
                && !java_function->isSignal()
                && !java_function->isStatic()
                && !(included_attributes & AbstractMetaAttributes::Static))
            s << INDENT << "@QtBlockedSlot" << endl;

        s << INDENT;
        if (attr & AbstractMetaAttributes::Public) s << "public ";
        else if (attr & AbstractMetaAttributes::Protected) s << "protected ";
        else if (attr & AbstractMetaAttributes::Private) s << "private ";

        bool isStatic = (attr & AbstractMetaAttributes::Static);

        if (attr & AbstractMetaAttributes::Native) s << "native ";
        else if (!isStatic && (attr & AbstractMetaAttributes::FinalInTargetLang)) s << "final ";
        else if (!isStatic && (attr & AbstractMetaAttributes::Abstract)) s << "abstract ";

        if (isStatic) s << "static ";
    }

    if ((options & SkipReturnType) == 0) {
        QString modified_type = java_function->typeReplaced(0);
        if (modified_type.isEmpty())
            s << translateType(java_function->type(), java_function->implementingClass(), (Option) options);
        else
            s << modified_type.replace('$', '.');
        s << " ";
    }
}

void JavaGenerator::writeConstructorContents(QTextStream &s, const AbstractMetaFunction *java_function) {
    // Write constructor
    s << "{" << endl;
    {
        Indentation indent(INDENT);
        s << INDENT << "super((QPrivateConstructor)null);" << endl;

        writeJavaCallThroughContents(s, java_function);

        // Write out expense checks if present...
        const AbstractMetaClass *java_class = java_function->implementingClass();
        const ComplexTypeEntry *te = java_class->typeEntry();
        if (te->expensePolicy().isValid()) {
            s << endl;
            const ExpensePolicy &ep = te->expensePolicy();
            s << INDENT << "org.qtjambi.qt.GeneratorUtilities.countExpense(" << java_class->fullName()
            << ".class, " << ep.cost << ", " << ep.limit << ");" << endl;
        }

        foreach(CodeSnip snip, te->codeSnips()) {
            if (snip.language == TypeSystem::Constructors) {
                snip.formattedCode(s, INDENT);
            }
        }
    }
    s << INDENT << "}" << endl << endl;

    // Write native constructor
    if (java_function->jumpTableId() == -1)
        writePrivateNativeFunction(s, java_function);
}

void JavaGenerator::writeFunctionArguments(QTextStream &s, const AbstractMetaFunction *java_function,
        int argument_count, uint options) {
    AbstractMetaArgumentList arguments = java_function->arguments();

    if (argument_count == -1)
        argument_count = arguments.size();

    for (int i = 0; i < argument_count; ++i) {
        if (!java_function->argumentRemoved(i + 1)) {
            if (i != 0)
                s << ", ";
            writeArgument(s, java_function, arguments.at(i), options);
        }
    }
}


void JavaGenerator::writeExtraFunctions(QTextStream &s, const AbstractMetaClass *java_class) {
    const ComplexTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    CodeSnipList code_snips = class_type->codeSnips();
    foreach(const CodeSnip &snip, code_snips) {
        if ((!java_class->isInterface() && snip.language == TypeSystem::TargetLangCode)
                || (java_class->isInterface() && snip.language == TypeSystem::Interface)) {
            s << endl;
            snip.formattedCode(s, INDENT);
        }
    }
}


void JavaGenerator::writeToStringFunction(QTextStream &s, const AbstractMetaClass *java_class) {
    bool generate = java_class->hasToStringCapability() && !java_class->hasDefaultToStringFunction();
    bool core = java_class->package() == QLatin1String("org.qtjambi.qt.core");
    bool qevent = false;

    const AbstractMetaClass *cls = java_class;
    while (cls) {
        if (cls->name() == "QEvent") {
            qevent = true;
            break;
        }
        cls = cls->baseClass();
    }

    if (generate || qevent) {

        if (qevent && core) {
            s << endl
              << INDENT << "    @Override" << endl
              << INDENT << "    public String toString() {" << endl
              << INDENT << "        return getClass().getSimpleName() + \"(type=\" + type().name() + \")\";" << endl
              << INDENT << "    }" << endl;
        } else {
            s << endl
              << INDENT << "    @Override" << endl
              << INDENT << "    public String toString() {" << endl
              << INDENT << "        if (nativeId() == 0)" << endl
              << INDENT << "            throw new QNoNativeResourcesException(\"Function call on incomplete object of type: \" +getClass().getName());" << endl
              << INDENT << "        return __qt_toString(nativeId());" << endl
              << INDENT << "    }" << endl
              << INDENT << "    native String __qt_toString(long __this_nativeId);" << endl;
        }
    }
}

void JavaGenerator::writeCloneFunction(QTextStream &s, const AbstractMetaClass *java_class) {
    s << INDENT << endl
      << INDENT << "    @Override" << endl
      << INDENT << "    public " << java_class->simpleName() << " clone() {" << endl
      << INDENT << "        if (nativeId() == 0)" << endl
      << INDENT << "            throw new QNoNativeResourcesException(\"Function call on incomplete object of type: \" +getClass().getName());" << endl
      << INDENT << "        return __qt_clone(nativeId());" << endl
      << INDENT << "    }" << endl
      << INDENT << "    native " << java_class->simpleName() << " __qt_clone(long __this_nativeId);" << endl;
}
