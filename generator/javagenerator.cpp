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

#include "javagenerator.h"
#include "cppgenerator.h"
#include "reporthandler.h"
#include "docparser.h"
#include "jumptable.h"
#include "abstractmetabuilder.h"
#include "docindex/docindexreader.h"
#include "typesystem/handler.h"

#include <QtCore>
#include <QDebug>
#include "typesystem/typedatabase.h"
#include "wrapper.h"			/* for isTargetPlatformArmCpu */
#include "fileout.h"

static Indentor INDENT;

void JavaGenerator::printExtraCode(QStringList& lines, QTextStream &s, bool addFreeLine){

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
    if(!lines.isEmpty() && addFreeLine)
        s << INDENT << Qt::endl;
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
        line = line.replace("@docRoot/", docsUrl);
        s << INDENT << line << Qt::endl;
    }
};

QString findComparableType(const AbstractMetaClass *cls){
    QString comparableType;
    AbstractMetaFunctionList geltFunctions = AbstractMetaFunctionList()
                                                << cls->greaterThanFunctions()
                                                << cls->greaterThanEqFunctions()
                                                << cls->lessThanFunctions()
                                                << cls->lessThanEqFunctions()
                                                << cls->compareFunctions();
    for(AbstractMetaFunction* f : geltFunctions){
        if(f->isPrivate())
            continue;
        AbstractMetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            type = arg->type()->typeEntry()->qualifiedTargetLangName();
            if(arg->type()->typeEntry()->isContainer()){
                if(type=="java.util.List"
                       ||  type=="java.util.LinkedList"
                       ||  type=="java.util.Queue"
                       ||  type=="java.util.Deque"
                       ||  type=="java.util.ArrayList"
                       ||  type=="java.util.Vector"
                       ||  type=="java.util.Set"
                       ||  type=="io.qt.core.QSet"
                       ||  type=="io.qt.core.QList"
                       ||  type=="io.qt.core.QQueue"
                       ||  type=="io.qt.core.QVector"
                       ||  type=="io.qt.core.QStack"
                       ||  type=="io.qt.core.QLinkedList") {
                    type = "java.util.Collection";
                }else if(type=="java.util.Map"
                       ||  type=="java.util.SortedMap"
                       ||  type=="java.util.NavigableMap"
                       ||  type=="java.util.HashMap"
                       ||  type=="java.util.TreeMap"
                       ||  type=="io.qt.core.QMap"
                       ||  type=="io.qt.core.QHash"
                       ||  type=="io.qt.core.QMultiMap"
                       ||  type=="io.qt.core.QMultiHash"){
                    type = "java.util.Map";
                }
            }
            if(arg->type()->instantiations().size()>0){
                type += "<";
                for(int i=0; i<arg->type()->instantiations().size(); i++){
                    if(i==0)
                        type += "?";
                    else
                        type += ",?";
                }
                type += ">";
            }
        }
        type.replace('$', '.');
        if(comparableType.isEmpty()){
            comparableType = type;
        }else if(comparableType!=type){
            comparableType = "Object";
            break;
        }
    }
    if(comparableType.isEmpty()){
        comparableType = cls->typeEntry()->qualifiedTargetLangName();
    }
    return comparableType;
}

JavaGenerator::JavaGenerator()
        : m_doc_parser(nullptr),
        m_docs_enabled(false),
        m_native_jump_table(false),
        target_JDK_version(10)
        {
}

QString JavaGenerator::fileNameForClass(const AbstractMetaClass *java_class) const {
    return QString("%1.java").arg(java_class->name());
}

void JavaGenerator::writeFieldAccessors(QTextStream &s, const AbstractMetaField *field, Option functionOptions) {
    Q_ASSERT(field->isPublic() || field->isProtected());

    const AbstractMetaClass *declaringClass = field->enclosingClass();
    const AbstractMetaFunction *setter = field->setter();
    const AbstractMetaFunction *getter = field->getter();

    FieldModification mod = declaringClass->typeEntry()->fieldModification(field->name());

    // Set function
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
        if (declaringClass->hasFunction(setter)) {
            QString warning =
                QString("class '%1' already has setter '%2' for %3 field '%4'")
                .arg(declaringClass->name()).arg(setter->name(), field->isProtected() ? "protected" : "public", field->name());
            ReportHandler::warning(warning);
        } else {
            writeFunction(s, setter, 0, 0, functionOptions);
        }
    }

    // Get function
    if (mod.isReadable()) {
        if (declaringClass->hasFunction(getter)) {
            QString warning =
                QString("class '%1' already has getter '%2' for %3 field '%4'")
                .arg(declaringClass->name()).arg(getter->name(), field->isProtected() ? "protected" : "public", field->name());
            ReportHandler::warning(warning);
        } else {
            writeFunction(s, getter, 0, 0, functionOptions);
            if(((field->isStatic() && field->type()->isConstant() && field->type()->indirections().isEmpty()) || field->attributes() & AbstractMetaAttributes::ConstExpr)
                    && (
                        field->type()->isPrimitive()
                        || field->type()->isEnum()
                        || field->type()->isTargetLangString()
                        )){
                QString comment;
                QTextStream commentStream(&comment);
                if(!field->brief().isEmpty()){
                    commentStream << "<p>" << QString(field->brief())
                                     .replace("&", "&amp;")
                                     .replace("<", "&lt;")
                                     .replace(">", "&gt;")
                                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                     .replace("@", "&commat;")
                                     .replace("/*", "&sol;*")
                                     .replace("*/", "*&sol;")
                                  << "</p>" << Qt::endl;
                }
                if(!field->href().isEmpty()){
                    QString url = docsUrl+field->href();
                    commentStream << "<p>See <a href=\"" << url << "\">";
                    if(declaringClass){
                        commentStream << declaringClass->qualifiedCppName()
                                         .replace("<JObjectWrapper>", "")
                                         .replace("QtJambi", "Q")
                                         .replace("QVoid", "Q")
                                         .replace("&", "&amp;")
                                         .replace("<", "&lt;")
                                         .replace(">", "&gt;")
                                         .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                         .replace("@", "&commat;")
                                         .replace("/*", "&sol;*")
                                         .replace("*/", "*&sol;")
                                      << "::";
                    }
                    commentStream << QString(field->name())
                                     .replace("&", "&amp;")
                                     .replace("<", "&lt;")
                                     .replace(">", "&gt;")
                                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                     .replace("@", "&commat;")
                                     .replace("/*", "&sol;*")
                                     .replace("*/", "*&sol;")
                                  << "</a></p>" << Qt::endl;
                }
                if(field->getter()->isDeprecated() && !field->getter()->deprecatedComment().isEmpty()){
                    if(!comment.isEmpty())
                        commentStream << Qt::endl;
                    writeDeprecatedComment(commentStream, field->getter());
                }
                if(!comment.trimmed().isEmpty()){
                    s << INDENT << "/**" << Qt::endl;
                    commentStream.seek(0);
                    while(!commentStream.atEnd()){
                        s << INDENT << " * " << commentStream.readLine() << Qt::endl;
                    }
                    s << INDENT << " */" << Qt::endl;
                }
                s << INDENT;
                if(field->isPublic()){
                    s << "public ";
                }else if(field->isProtected()){
                    s << "protected ";
                }
                if(field->isStatic()){
                    s << "static ";
                }
                s << "final " << translateType(field->type(), declaringClass, Option(InitializerListAsArray))
                  << " " << field->getter()->name() << " = " << field->getter()->name() << "();" << Qt::endl << Qt::endl;
            }
        }
    }
}

void JavaGenerator::registerPackage(QString pkgName, bool inclExported){
    auto idx = pkgName.indexOf('<');
    if(idx>0){
        pkgName = pkgName.mid(0, idx);
    }
    if(pkgName==QStringLiteral("io.qt.qml.QQmlListProperty"))
        inclExported = false;
    if(pkgName.startsWith(QStringLiteral("?"))){
        pkgName = pkgName.mid(1).trimmed();
        if(pkgName.startsWith(QStringLiteral("extends "))){
            pkgName = pkgName.mid(8);
        }else if(pkgName.startsWith(QStringLiteral("super "))){
            pkgName = pkgName.mid(6);
        }
        for(const QString& pkg : pkgName.split("&")){
            registerPackage(pkg.trimmed(), inclExported);
        }
        return;
    }
    idx = pkgName.lastIndexOf('.');
    if(idx>0){
        pkgName = pkgName.mid(0, idx);
        m_currentPackages << pkgName;
        if(inclExported)
            m_exportedPackages << pkgName;
    }
}


QString JavaGenerator::translateType(const AbstractMetaType *java_type, const AbstractMetaClass *context, Option option) {
    QString s;

    if (context && java_type && context->typeEntry()->isGenericClass() && java_type->originalTemplateType())
        java_type = java_type->originalTemplateType();

    if (!java_type) {
        s = "void";
    } else if (java_type->isIterator()){
        const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(java_type->typeEntry());
        s = iteratorType->qualifiedTargetLangName();
        bool found = false;
        if(!java_type->iteratorInstantiations().isEmpty()){
            if(java_type->iteratorInstantiations().size()==2){
                s = "io.qt.core.QMapIterator";
            }
            s += "<";
            for(int i=0; i<java_type->iteratorInstantiations().size(); i++){
                if(i>0)
                    s += ", ";
                s += translateType(java_type->iteratorInstantiations().at(i), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
            }
            s += ">";
            found = true;
        }
        if(!found && iteratorType->containerType()){
            AbstractMetaClass * containerClass = m_classes.findClass(iteratorType->containerType()->qualifiedCppName());
            if(containerClass){
                const ContainerTypeEntry* containerType = nullptr;
                if(containerClass->templateBaseClass() && containerClass->templateBaseClass()->typeEntry()->type()==TypeEntry::ContainerType){
                    containerType = static_cast<const ContainerTypeEntry*>(containerClass->templateBaseClass()->typeEntry());
                    if(containerType->type()==ContainerTypeEntry::MapContainer
                        || containerType->type()==ContainerTypeEntry::MultiMapContainer
                        || containerType->type()==ContainerTypeEntry::HashContainer
                            || containerType->type()==ContainerTypeEntry::MultiHashContainer){
                        s = "io.qt.core.QMapIterator";
                    }
                }
                if(!containerClass->templateBaseClassInstantiations().isEmpty() && (option & SkipTemplateParameters)==0){
                    s += "<";
                    for(int i=0; i<containerClass->templateBaseClassInstantiations().size(); i++){
                        if(i>0)
                            s += ", ";
                        s += translateType(containerClass->templateBaseClassInstantiations().at(i), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
                    }
                    s += ">";
                    found = true;
                }
            }
        }
        if(!found){
            AbstractMetaClass * iteratorClass = m_classes.findClass(iteratorType->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName);
            if(iteratorClass){
                if(iteratorClass->typeAliasType()){
                    QScopedPointer<AbstractMetaType> typeAliasType(iteratorClass->typeAliasType()->copy());
                    if(typeAliasType->indirections().size()==1 && typeAliasType->getReferenceType()==AbstractMetaType::NoReference){
                        QList<bool> indirections = typeAliasType->indirections();
                        indirections.takeFirst();
                        typeAliasType->setIndirections(indirections);
                        typeAliasType->setReferenceType(AbstractMetaType::Reference);
                        AbstractMetaBuilder::decideUsagePattern(typeAliasType.data());
                    }
                    if((option & SkipTemplateParameters)==0){
                        s += "<";
                        s += translateType(typeAliasType.data(), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
                        s += ">";
                    }
                }else{
                    for(AbstractMetaFunction* function : iteratorClass->functions()){
                        if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                            if((option & SkipTemplateParameters)==0){
                                s += "<";
                                s += translateType(function->type(), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
                                s += ">";
                            }
                            break;
                        }
                    }
                }
            }
        }
    } else if (java_type->isInitializerList()) {
        AbstractMetaType* instantiation = java_type->instantiations()[0]->copy();
        AbstractMetaBuilder::decideUsagePattern(instantiation);
        s = translateType(instantiation, context, Option((option & ~EnumAsInts & ~UseNativeIds & ~BoxedPrimitive) | NoSuppressExports | InitializerListAsArray));
        if(option & InitializerListAsArray)
            s += "[]";
        else
            s += " ...";
    } else if (java_type->hasNativeId() && (option & UseNativeIds)) {
        s = "long";
    } else if (java_type->isArray()) {
        s = translateType(java_type->arrayElementType(), context, Option((option & ~EnumAsInts & ~UseNativeIds & ~BoxedPrimitive) | NoSuppressExports | InitializerListAsArray)) + "[]";
    } else if (java_type->isEnum()) {
        const EnumTypeEntry * eentry = reinterpret_cast<const EnumTypeEntry *>(java_type->typeEntry());
        uint size = eentry->size();

        if(eentry->forceInteger()){
            switch(size){
            case 8:
                if (option & BoxedPrimitive)
                    s = "java.lang.Byte";
                else
                    s = "byte";
                break;
            case 16:
                if (option & BoxedPrimitive)
                    s = "java.lang.Short";
                else
                    s = "short";
                break;
            case 32:
                if (option & BoxedPrimitive)
                    s = "java.lang.Integer";
                else
                    s = "int";
                break;
            case 64:
                if (option & BoxedPrimitive)
                    s = "java.lang.Long";
                else
                    s = "long";
                break;
            default:
                if (option & BoxedPrimitive)
                    s = "java.lang.Integer";
                else
                    s = "int";
                break;
            }
        }else{
            if (option & EnumAsInts)
                switch(size){
                case 8:
                    s = "byte";
                    break;
                case 16:
                    s = "short";
                    break;
                case 32:
                    s = "int";
                    break;
                case 64:
                    s = "long";
                    break;
                default:
                    s = "int";
                    break;
                }
            else{
                m_currentPackages << static_cast<const EnumTypeEntry *>(java_type->typeEntry())->javaPackage();
                if((option & NoSuppressExports) == 0)
                    m_exportedPackages << static_cast<const EnumTypeEntry *>(java_type->typeEntry())->javaPackage();
                s = java_type->fullName();
            }
        }
    } else if (java_type->isFlags()) {
        if (static_cast<const FlagsTypeEntry *>(java_type->typeEntry())->forceInteger()) {
            if (option & BoxedPrimitive)
                s = "java.lang.Integer";
            else
                s = "int";
        } else {
            if (option & EnumAsInts)
                s = "int";
            else{
                m_currentPackages << static_cast<const FlagsTypeEntry *>(java_type->typeEntry())->javaPackage();
                if((option & NoSuppressExports) == 0)
                    m_exportedPackages << static_cast<const FlagsTypeEntry *>(java_type->typeEntry())->javaPackage();
                s = java_type->fullName();
            }
        }
    } else {
        if (java_type->isPrimitive() && (option & BoxedPrimitive) ) {
            s = static_cast<const PrimitiveTypeEntry *>(java_type->typeEntry())->javaObjectFullName();

        } else if (java_type->isNativePointer()) {
            s = "io.qt.QNativePointer";

        } else if (java_type->isContainer()) {
            const ContainerTypeEntry * container = static_cast<const ContainerTypeEntry *>(java_type->typeEntry());
            if(((option & CollectionAsCollection) != CollectionAsCollection)
                    && ((option & NoQCollectionContainers) != NoQCollectionContainers)
                    && (java_type->getReferenceType()==AbstractMetaType::Reference
                    || !java_type->indirections().isEmpty())
                    && (
                        container->type()==ContainerTypeEntry::ListContainer
                        || container->type()==ContainerTypeEntry::StringListContainer
                        || container->type()==ContainerTypeEntry::ByteArrayListContainer
                        || container->type()==ContainerTypeEntry::QueueContainer
                        || container->type()==ContainerTypeEntry::MapContainer
                        || container->type()==ContainerTypeEntry::SetContainer
                        || container->type()==ContainerTypeEntry::MultiMapContainer
                        || container->type()==ContainerTypeEntry::HashContainer
                        || container->type()==ContainerTypeEntry::MultiHashContainer
                        || container->type()==ContainerTypeEntry::VectorContainer
                        || container->type()==ContainerTypeEntry::StackContainer
                        || container->type()==ContainerTypeEntry::LinkedListContainer
                        )){
                if(container->type()==ContainerTypeEntry::StringListContainer
                        || container->type()==ContainerTypeEntry::ByteArrayListContainer){
                    s = "io.qt.core.QList";
                }else{
                    if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)>=QT_VERSION_CHECK(6,0,0) && container->type()==ContainerTypeEntry::VectorContainer){
                        s = "io.qt.core.QList";
                    }else{
                        s = "io.qt.core."+java_type->typeEntry()->qualifiedCppName();
                    }
                }
                if ((option & SkipTemplateParameters) == 0) {
                    s += '<';
                    const QList<const AbstractMetaType *>& args = java_type->instantiations();
                    for (int i=0; i<args.size(); ++i) {
                        if (i != 0)
                            s += ", ";
                        if((option & CollectionAsCollection) == CollectionAsCollection
                                && !args.at(i)->isPrimitive()
                                && !args.at(i)->isString()
                                && !args.at(i)->isTargetLangChar())
                            s += "? extends ";
                        s += translateType(args.at(i), context, Option(BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
                    }
                    s += '>';
                }
            }else{
                s = java_type->typeEntry()->qualifiedTargetLangName();
                if((option & CollectionAsCollection) == CollectionAsCollection){
                    if(s=="java.util.List"
                            ||  s=="java.util.LinkedList"
                            ||  s=="java.util.Queue"
                            ||  s=="java.util.Deque"
                            ||  s=="java.util.ArrayList"
                            ||  s=="java.util.Vector"
                            ||  s=="java.util.Set"
                            ||  s=="io.qt.core.QSet"
                            ||  s=="io.qt.core.QList"
                            ||  s=="io.qt.core.QStringList"
                            ||  s=="io.qt.core.QQueue"
                            ||  s=="io.qt.core.QVector"
                            ||  s=="io.qt.core.QStack"
                            ||  s=="io.qt.core.QLinkedList"){
                        s = "java.util.Collection";
                    }else if(s=="java.util.Map"
                             ||  s=="java.util.SortedMap"
                             ||  s=="java.util.NavigableMap"
                             ||  s=="java.util.HashMap"
                             ||  s=="java.util.TreeMap"
                             ||  s=="io.qt.core.QMap"
                             ||  s=="io.qt.core.QHash"
                             ||  s=="io.qt.core.QMultiMap"
                             ||  s=="io.qt.core.QMultiHash"){
                        s = "java.util.Map";
                    }
                }else if((option & NoQCollectionContainers) == NoQCollectionContainers){
                    if(s=="io.qt.core.QList"
                            || s=="io.qt.core.QVector"){
                        s = "java.util.List";
                    }else if(s=="io.qt.core.QStringList"){
                        s = "java.util.List";
                    }else if(s=="io.qt.core.QSet"){
                        s = "java.util.Set";
                    }else if(s=="io.qt.core.QQueue"){
                        s = "java.util.Queue";
                    }else if(s=="io.qt.core.QStack"){
                        s = "java.util.Deque";
                    }else if(s=="io.qt.core.QLinkedList"){
                        s = "java.util.LinkedList";
                    }else if(s=="io.qt.core.QMap"
                             ||  s=="io.qt.core.QMultiMap"){
                        s = "java.util.NavigableMap";
                    }else if(s=="io.qt.core.QHash"
                             ||  s=="io.qt.core.QMultiHash"){
                        s = "java.util.Map";
                    }
                }
                if((option & SkipTemplateParameters) == 0 && container->type()==ContainerTypeEntry::QModelRoleDataSpanContainer){
                    s += "<Integer,Object>";
                }else if ((option & SkipTemplateParameters) == 0 && s!="io.qt.core.QStringList") {
                    s += '<';
                    const QList<const AbstractMetaType *>& args = java_type->instantiations();
                    auto argssize = args.size();
                    for (int i=0; i<argssize; ++i) {
                        if (i != 0)
                            s += ", ";
                        bool isMultiMap = (container->type() == ContainerTypeEntry::MultiMapContainer
                                           || container->type() == ContainerTypeEntry::MultiHashContainer)
                                          && ((option & NoQCollectionContainers) == NoQCollectionContainers
                                              || (option & CollectionAsCollection) == CollectionAsCollection)
                                          && i == 1;
                        if (isMultiMap)
                            s += "? extends java.util.List<";
                        if((option & CollectionAsCollection) == CollectionAsCollection
                                && !args.at(i)->isPrimitive()
                                && !args.at(i)->isString()
                                && !args.at(i)->isTargetLangChar())
                            s += "? extends ";
                        s += translateType(args.at(i), context, Option(BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
                        if (isMultiMap)
                            s += ">";
                    }
                    s += '>';
                }
                if(s=="java.util.Optional<java.lang.Integer>"){
                    s = "java.util.OptionalInt";
                }else if(s=="java.util.Optional<java.lang.Long>"){
                    s = "java.util.OptionalLong";
                }else if(s=="java.util.Optional<java.lang.Double>"){
                    s = "java.util.OptionalDouble";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Boolean>"){
                    s = "io.qt.core.QBooleanPropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Byte>"){
                    s = "io.qt.core.QBytePropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Short>"){
                    s = "io.qt.core.QShortPropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Integer>"){
                    s = "io.qt.core.QIntPropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Long>"){
                    s = "io.qt.core.QLongPropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Char>"){
                    s = "io.qt.core.QCharPropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Float>"){
                    s = "io.qt.core.QFloatPropertyBinding";
                }else if(s=="io.qt.core.QPropertyBinding<java.lang.Double>"){
                    s = "io.qt.core.QDoublePropertyBinding";
                }else if(s=="io.qt.core.QBindable<java.lang.Boolean>"){
                    s = "io.qt.core.QBooleanBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Byte>"){
                    s = "io.qt.core.QByteBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Short>"){
                    s = "io.qt.core.QShortBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Integer>"){
                    s = "io.qt.core.QIntBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Long>"){
                    s = "io.qt.core.QLongBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Char>"){
                    s = "io.qt.core.QCharBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Float>"){
                    s = "io.qt.core.QFloatBindable";
                }else if(s=="io.qt.core.QBindable<java.lang.Double>"){
                    s = "io.qt.core.QDoubleBindable";
                }
            }
        } else if (java_type->isPointerContainer() && java_type->instantiations().size()==1) {
            AbstractMetaType* instantiation = java_type->instantiations()[0]->copy();
            instantiation->setIndirections(QList<bool>(instantiation->indirections()) << false);
            AbstractMetaBuilder::decideUsagePattern(instantiation);
            s = translateType(instantiation, context, Option((option & ~EnumAsInts) & ~UseNativeIds));
        } else {
            const TypeEntry *type = java_type->typeEntry();
            if (type->designatedInterface())
                type = type->designatedInterface();
            s = type->qualifiedTargetLangName();
            if(!java_type->instantiations().isEmpty()){
                s += '<';
                const QList<const AbstractMetaType *>& args = java_type->instantiations();
                for (int i=0; i<args.size(); ++i) {
                    if (i != 0)
                        s += ", ";
                    if((option & CollectionAsCollection) == CollectionAsCollection
                            && !args.at(i)->isPrimitive()
                            && !args.at(i)->isString()
                            && !args.at(i)->isTargetLangChar())
                        s += "? extends ";
                    s += translateType(args.at(i), context, Option(BoxedPrimitive | NoQCollectionContainers | InitializerListAsArray));
                }
                s += '>';
            }else if(type->type()==TypeEntry::JMapWrapperType){
                s += "<?,?>";
            }else if(type->type()==TypeEntry::JCollectionWrapperType
                     || type->type()==TypeEntry::JIteratorWrapperType
                     || type->type()==TypeEntry::JEnumWrapperType
                     || type->type()==TypeEntry::JQFlagsWrapperType){
                s += "<?>";
            }else if(type->isComplex()){
                const ComplexTypeEntry *ctype = reinterpret_cast<const ComplexTypeEntry *>(type);
                if(ctype->isGenericClass()){
                    if(java_type->hasInstantiations()){
                        s += '<';
                        const QList<const AbstractMetaType *>& args = java_type->instantiations();
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            s += translateType(args[i], context, option);
                        }
                        s += '>';
                    }else if(context && !context->templateArguments().isEmpty()){
                        s += '<';
                        const QList<TypeEntry *>& args = context->templateArguments();
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            s += args[i]->name();
                        }
                        s += '>';
                    }
                }
            }else if(type->isFunctional()){
                const FunctionalTypeEntry *ftype = reinterpret_cast<const FunctionalTypeEntry *>(type);
                if(ftype->isGenericClass() && context){
                    AbstractMetaFunctional* functional = nullptr;
                    for(AbstractMetaFunctional* f : context->functionals()){
                        if(f->typeEntry()==ftype){
                            functional = f;
                            break;
                        }
                    }
                    if(functional && !functional->javaFunctionalInterfaceParameterTypes().isEmpty()){
                        s += '<';
                        const QList<uint>& args = functional->javaFunctionalInterfaceParameterTypes();
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            //s += args[i]->name();
                            QString modifiedType = functional->typeReplaced(args[i]);
                            if(modifiedType.isEmpty()){
                                if(args[i]==0){
                                    s += translateType(functional->type(), context, Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers));
                                }else{
                                    s += translateType(functional->arguments()[args[i]-1]->type(), context, Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers));
                                }
                            }else{
                                s += modifiedType;
                            }
                        }
                        s += '>';
                    }else if(!context->templateArguments().isEmpty()){
                        s += '<';
                        const QList<TypeEntry *>& args = context->templateArguments();
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            s += args[i]->name();
                        }
                        s += '>';
                    }
                }
            }
        }
    }
    if(java_type && !java_type->isEnum() && !java_type->isFlags())
        registerPackage(s, (option & NoSuppressExports) == 0);
    return s.replace('$', '.');
}

QString JavaGenerator::argumentString(const AbstractMetaFunction *java_function,
                                      const AbstractMetaArgument *java_argument,
                                      Option options) {
    QString modified_type;
    if(java_function->argumentTypeArray(java_argument->argumentIndex() + 1)){
        options = Option(options & ~EnumAsInts);
        QScopedPointer<AbstractMetaType> cpy(java_argument->type()->copy());
        cpy->setConstant(false);
        cpy->setReferenceType(AbstractMetaType::NoReference);
        QList<bool> indirections = cpy->indirections();
        if(!indirections.isEmpty()){
            indirections.removeLast();
            cpy->setIndirections(indirections);
        }
        AbstractMetaBuilder::decideUsagePattern(cpy.get());
        if(java_function->argumentTypeArrayVarArgs(java_argument->argumentIndex() + 1)){
            modified_type = translateType(cpy.get(), java_function->implementingClass(), Option((options & ~UseNativeIds) | NoSuppressExports))+"...";
        }else{
            modified_type = translateType(cpy.get(), java_function->implementingClass(), Option((options & ~UseNativeIds) | NoSuppressExports))+"[]";
        }
    }else if(java_function->argumentTypeBuffer(java_argument->argumentIndex() + 1)){
        options = Option(options & ~EnumAsInts);
        QScopedPointer<AbstractMetaType> cpy(java_argument->type()->copy());
        cpy->setConstant(false);
        cpy->setReferenceType(AbstractMetaType::NoReference);
        QList<bool> indirections = cpy->indirections();
        if(!indirections.isEmpty()){
            indirections.removeLast();
            cpy->setIndirections(indirections);
        }
        AbstractMetaBuilder::decideUsagePattern(cpy.get());
        modified_type = translateType(cpy.get(), java_function->implementingClass(), Option(options & ~UseNativeIds));
        if(modified_type=="int"){
            modified_type = "java.nio.IntBuffer";
        }else if(modified_type=="byte"){
            modified_type = "java.nio.ByteBuffer";
        }else if(modified_type=="char"){
            modified_type = "java.nio.CharBuffer";
        }else if(modified_type=="short"){
            modified_type = "java.nio.ShortBuffer";
        }else if(modified_type=="long"){
            modified_type = "java.nio.LongBuffer";
        }else if(modified_type=="float"){
            modified_type = "java.nio.FloatBuffer";
        }else if(modified_type=="double"){
            modified_type = "java.nio.DoubleBuffer";
        }else{
            modified_type = "java.nio.Buffer";
        }
    }else {
        modified_type = java_function->typeReplaced(java_argument->argumentIndex() + 1);
        if((options & InitializerListAsArray) && modified_type.endsWith("...")){
            modified_type = modified_type.replace("...", "[]");
        }
    }
    QString arg;

    if (modified_type.isEmpty()){
        if(java_argument->argumentIndex()<java_function->arguments().size()-1)
            options = Option(options | InitializerListAsArray);
        arg = translateType(java_argument->type(), java_function->implementingClass(), Option(options));
        if(arg=="io.qt.core.QString")
            arg = "java.lang.CharSequence";
        if((options & SkipTemplateParameters) == 0 && java_argument->type()->typeEntry()->isComplex()){
            const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(java_argument->type()->typeEntry());
            if(ctype->isGenericClass() && java_argument->type()->instantiations().isEmpty()){
                arg = QString("@SuppressWarnings(\"rawtypes\") ") + arg;
            }
        }
    }else{
        registerPackage(modified_type);
        arg = modified_type.replace('$', '.');
    }
    if ((options & SkipTemplateParameters) == SkipTemplateParameters) {
        auto idx = arg.indexOf("<");
        if(idx>0)
            arg = arg.left(idx);
    }

    if ((options & SkipName) == 0) {
        arg += " ";
        arg += java_argument->modifiedArgumentName();
    }

    return arg;
}

void JavaGenerator::writeArgument(QTextStream &s,
                                  const AbstractMetaFunction *java_function,
                                  const AbstractMetaArgument *java_argument,
                                  Option options) {
    s << argumentString(java_function, java_argument, options);
}


void JavaGenerator::writeIntegerEnum(QTextStream &s, const uint size, const AbstractMetaEnum *java_enum) {
    const AbstractMetaEnumValueList &values = java_enum->values();

    if (java_enum->isDeclDeprecated()) {
        s << INDENT << "@Deprecated" << Qt::endl;
    }
    s  << INDENT;
    if(java_enum->isProtected() && (java_enum->enclosingClass() && !java_enum->enclosingClass()->typeEntry()->designatedInterface()) && !java_enum->enclosingClass()->typeEntry()->isInterface()){
        s  << "protected ";
    }else{
        s  << "public ";
    }
    const QMap<QString,QString>& renamedEnumValues = java_enum->typeEntry()->renamedEnumValues();
    if(java_enum->enclosingClass() && !java_enum->enclosingClass()->isFake())
        s << "static ";
    s << "class " << java_enum->name() << "{" << Qt::endl;
    for (int i = 0; i < values.size(); ++i) {
        AbstractMetaEnumValue *value = values.at(i);

        if (java_enum->typeEntry()->isEnumValueRemoveRejected(value->name()))
            continue;

        if (m_doc_parser)
            s << m_doc_parser->documentation(value);

        if(value->deprecated()){
            if(!value->deprecatedComment().isEmpty()){
                s << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * @deprecated " << QString(value->deprecatedComment())
                     .replace("&", "&amp;")
                     .replace("<", "&lt;")
                     .replace(">", "&gt;")
                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                     .replace("@", "&commat;")
                     .replace("/*", "&sol;*")
                     .replace("*/", "*&sol;")
                  << Qt::endl
                  << INDENT << "     */" << Qt::endl;
            }
            s << INDENT << "    @Deprecated" << Qt::endl;
        }

        QString name = renamedEnumValues.value(value->name(), value->name());
        if(value->value().userType()==QMetaType::QString){
            switch(size){
            case 8:
                s << INDENT << "    public static final byte " << name << " = " << value->value().toString();
                break;
            case 16:
                s << INDENT << "    public static final short " << name << " = " << value->value().toString();
                break;
            case 32:
                s << INDENT << "    public static final int " << name << " = " << value->value().toString();
                break;
            case 64:
                s << INDENT << "    public static final long " << name << " = " << value->value().toString();
                break;
            default:
                s << INDENT << "    public static final int " << name << " = " << value->value().toString();
                break;
            }
        }else{
            switch(size){
            case 8:
                s << INDENT << "    public static final byte " << name << " = " << value->value().value<qint8>();
                break;
            case 16:
                s << INDENT << "    public static final short " << name << " = " << value->value().value<qint16>();
                break;
            case 32:
                s << INDENT << "    public static final int " << name << " = " << value->value().value<qint32>();
                break;
            case 64:
                s << INDENT << "    public static final long " << name << " = " << value->value().value<qint64>() << "L";
                break;
            default:
                s << INDENT << "    public static final int " << name << " = " << value->value().value<qint32>();
                break;
            }
        }
        s << ";";
        s << Qt::endl;
    }

    s << INDENT << "} // end of enum " << java_enum->name() << Qt::endl << Qt::endl;
}

void JavaGenerator::writeFunctional(QTextStream &s, const AbstractMetaFunctional *java_functional) {
    if(java_functional->isPrivate() || java_functional->typeEntry()->codeGeneration()==TypeEntry::GenerateNothing){
        return;
    }

    QString comment;
    {
        QTextStream commentStream(&comment);

        if (m_doc_parser) {
            commentStream << Qt::endl;
            commentStream << m_doc_parser->documentation(java_functional) << Qt::endl;
        }else{
            if(!java_functional->href().isEmpty()){
                QString url = docsUrl+java_functional->href();
                commentStream << "<p>Java wrapper for Qt function pointer <a href=\"" << url << "\">" << java_functional->typeEntry()->qualifiedCppName()
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "</a></p>" << Qt::endl;
            }else{
                commentStream << "<p>Java wrapper for Qt function pointer " << java_functional->typeEntry()->qualifiedCppName()
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "</p>" << Qt::endl;
            }
        }
    }

    int returnArrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(0);
    QString replacedReturnType = java_functional->typeReplaced(0);
    s << Qt::endl;
    if(!comment.trimmed().isEmpty()){
        s << INDENT << "/**" << Qt::endl;
        QTextStream commentStream(&comment, QIODevice::ReadOnly);
        while(!commentStream.atEnd()){
            s << INDENT << " * " << commentStream.readLine() << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
    }
    if(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell){
        s << INDENT << "public final class " << java_functional->name() << " extends io.qt.QtObject";
        if(!java_functional->javaFunctionalInterface().isEmpty())
            s << " implements ";
    }else{
        s << INDENT << "@FunctionalInterface" << Qt::endl
          << INDENT << "public interface " << java_functional->name() << " extends io.qt.QtObjectInterface";
        if(!java_functional->javaFunctionalInterface().isEmpty())
            s << ", ";
    }
    if(!java_functional->javaFunctionalInterface().isEmpty()){
        s << QString(java_functional->javaFunctionalInterface()).replace("$", ".");
        if(!java_functional->javaFunctionalInterfaceParameterTypes().isEmpty()){
            s << "<";
            int counter = 0;
            for(uint index : java_functional->javaFunctionalInterfaceParameterTypes()){
                if(counter!=0)
                    s << ", ";
                if(index==0){
                    Q_ASSERT(java_functional->type());
                    if(returnArrayLengthIndex>=0){
                        s << translateType(java_functional->type(), nullptr, Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers)) << "[]";
                    }else{
                        if(!replacedReturnType.isEmpty()){
                            registerPackage(replacedReturnType);
                            s << replacedReturnType.replace('$', '.');
                        }else
                            s << translateType(java_functional->type(), nullptr, Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers));
                    }
                }else{
                    Q_ASSERT(index<=uint(java_functional->arguments().size()));
                    AbstractMetaArgument * arg = java_functional->arguments()[index-1];
                    int arrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(arg->argumentIndex() + 1);
                    if(arrayLengthIndex>=0){
                        s << translateType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                    }else{
                        QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex() + 1);
                        if(!replacedArgType.isEmpty()){
                            registerPackage(replacedArgType);
                            s << replacedArgType.replace('$', '.');
                        }else
                            s << translateType(arg->type(), nullptr, Option(CollectionAsCollection));
                    }
                }
                ++counter;
            }
            s << ">";
        }
    }
    s << " {" << Qt::endl << Qt::endl;
    {
        INDENTATION(INDENT)
        if(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell){
            s << INDENT << "static {" << Qt::endl
              << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl;
        }
        if(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell){
            s << INDENT << "private " << java_functional->name() << "(QPrivateConstructor c){" << Qt::endl
              << INDENT << "    super(c);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl;
            writeExtraFunctions(s, java_functional, false);
        }else{
            writeExtraFunctions(s, java_functional, true);
        }
        bool needsCallThrough = java_functional->needsCallThrough();
        if(java_functional->javaFunctionalInterface().isEmpty()
           || !java_functional->javaFunctionalInterfaceParameterTypes().isEmpty()
           || (java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)) // we definitely need method declaration
        {
            if(!java_functional->javaFunctionalInterface().isEmpty())
                s << INDENT << "@Override" << Qt::endl;

            QString functionSignature;
            bool needsSuppressExportsWarning = false;
            {
                m_exportedPackages.clear();
                QTextStream s(&functionSignature);
                s << INDENT << "public ";
                if(!needsCallThrough && (java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
                    s << "native ";
                }
                if(returnArrayLengthIndex>=0){
                    s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | NoQCollectionContainers)) << "[]";
                }else{
                    if(!replacedReturnType.isEmpty()){
                        registerPackage(replacedReturnType);
                        s << replacedReturnType.replace('$', '.');
                    }else
                        s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | NoQCollectionContainers));
                }
                s << " ";
                if(java_functional->typeEntry()->functionName().isEmpty())
                    s << "call";
                else
                    s << java_functional->typeEntry()->functionName();
                s << "(";
                int counter = 0;
                for(AbstractMetaArgument * arg : java_functional->arguments()){
                    if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                        continue;
                    if(counter!=0)
                        s << ", ";
                    int arrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(arg->argumentIndex() + 1);
                    if(arrayLengthIndex>=0){
                        s << translateType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                    }else{
                        QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex() + 1);
                        if(!replacedArgType.isEmpty()){
                            registerPackage(replacedArgType);
                            s << replacedArgType.replace('$', '.');
                        }else
                            s << translateType(arg->type(), nullptr, Option(CollectionAsCollection));
                    }
                    s << " " << arg->modifiedArgumentName();
                    ++counter;
                }

                for(const QString& pkg : m_exportedPackages){
                    if(pkg!=java_functional->package()
                            && !pkg.isEmpty()
                            && m_typeSystemByPackage[pkg]
                            && m_typeSystemByPackage[pkg]->module()!="qtjambi" // because it's transitive
                            && m_typeSystemByPackage[pkg]!=m_typeSystemByPackage[java_functional->package()]
                            && !pkg.startsWith("io.qt.internal")
                            && pkg!="io.qt"){
                        needsSuppressExportsWarning = true;
                        break;
                    }
                }
            }
            if (needsSuppressExportsWarning)
                s << INDENT << "@SuppressWarnings(\"exports\")" << Qt::endl;
            s << functionSignature;
            if(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell){
                if(needsCallThrough){
                    s << "){" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT;
                        bool closeParen = false;
                        if(java_functional->type()){
                            s << "return ";
                            if(replacedReturnType.isEmpty()){
                                if (java_functional->type()->isTargetLangEnum()) {
                                    s << static_cast<const EnumTypeEntry *>(java_functional->type()->typeEntry())->qualifiedTargetLangName() << ".resolve(";
                                    closeParen = true;
                                } else if (java_functional->type()->isTargetLangFlags()) {
                                    registerPackage(java_functional->type()->typeEntry()->qualifiedTargetLangName());
                                    s << "new " << java_functional->type()->typeEntry()->qualifiedTargetLangName().replace('$', '.') << "(";
                                    closeParen = true;
                                }
                            }
                        }
                        if(java_functional->typeEntry()->functionName().isEmpty())
                            s << "call";
                        else
                            s << java_functional->typeEntry()->functionName();
                        s << "_native(";
                        bool needsComma = false;
                        if(java_functional->typeEntry()->isNativeIdBased()){
                            s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                            needsComma = true;
                        }
                        for(AbstractMetaArgument * arg : java_functional->arguments()){
                            if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                                continue;
                            if(needsComma)
                                s << ", ";
                            if(!java_functional->typeReplaced(arg->argumentIndex()+1).isEmpty()){
                                s << arg->modifiedArgumentName();
                            }else if (arg->type()->isTargetLangEnum() || arg->type()->isTargetLangFlags()) {
                                s << arg->modifiedArgumentName() << ".value()";
                            } else if (arg->type()->hasNativeId()) {
                                s << "QtJambi_LibraryUtilities.internal.checkedNativeId(" << arg->modifiedArgumentName() << ")";
                            } else {
                                s << arg->modifiedArgumentName();
                            }
                            needsComma = true;
                        }
                        if(closeParen)
                            s << ")";
                        s << ")";
                        s << ";" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl << Qt::endl
                      << INDENT << "private native ";
                    if(returnArrayLengthIndex>=0){
                        s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | EnumAsInts | NoQCollectionContainers)) << "[]";
                    }else{
                        if(!replacedReturnType.isEmpty())
                            s << replacedReturnType;
                        else
                            s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | EnumAsInts | NoQCollectionContainers));
                    }
                    s << " ";
                    if(java_functional->typeEntry()->functionName().isEmpty())
                        s << "call";
                    else
                        s << java_functional->typeEntry()->functionName();
                    s << "_native(";
                    bool hasArg = false;
                    if(java_functional->typeEntry()->isNativeIdBased()){
                        hasArg = true;
                        s << "long nativeId";
                    }
                    for(AbstractMetaArgument * arg : java_functional->arguments()){
                        if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                            continue;
                        if(hasArg)
                            s << ", ";
                        int arrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(arg->argumentIndex()+1);
                        if(arrayLengthIndex>=0){
                            s << translateType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                        }else{
                            QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex()+1);
                            if(!replacedArgType.isEmpty()){
                                registerPackage(replacedArgType);
                                s << replacedArgType.replace('$', '.');
                            }else
                                s << translateType(arg->type(), nullptr, Option(InitializerListAsArray | CollectionAsCollection | UseNativeIds | EnumAsInts));
                        }
                        s << " " << arg->modifiedArgumentName();
                        hasArg = true;
                    }
                }
            }
            s << ");" << Qt::endl << Qt::endl;
        }

        if(!(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
            s << INDENT << "/**" <<Qt::endl
              << INDENT << " * <p>Implementor class for interface {@link " << java_functional->name().replace("$", ".") << "}</p>" << Qt::endl
              << INDENT << " */" <<Qt::endl
              << INDENT << "public static abstract class Impl extends io.qt.QtObject implements " << java_functional->name() << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "static {" << Qt::endl
                  << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
                writeExtraFunctions(s, java_functional, false);
                s << INDENT << "public Impl(){" << Qt::endl
                  << INDENT << "    super((QPrivateConstructor)null);" << Qt::endl
                  << INDENT << "    initialize_native(this);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "protected Impl(QPrivateConstructor p){" << Qt::endl
                  << INDENT << "    super(p);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "private static native void initialize_native(" << java_functional->name() << " instance);" << Qt::endl << Qt::endl
                  << INDENT << "@io.qt.NativeAccess" << Qt::endl
                  << INDENT << "private final static class ConcreteWrapper extends Impl {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    s << INDENT << "private ConcreteWrapper(QPrivateConstructor c){" << Qt::endl
                      << INDENT << "    super(c);" << Qt::endl
                      << INDENT << "}" << Qt::endl << Qt::endl
                      << INDENT << "@Override" << Qt::endl
                      << INDENT << "public ";
                    if(!needsCallThrough)
                        s << "native ";
                    if(returnArrayLengthIndex>=0){
                        s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | NoQCollectionContainers)) << "[]";
                    }else{
                        if(!replacedReturnType.isEmpty())
                            s << replacedReturnType;
                        else
                            s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | NoQCollectionContainers));
                    }
                    s << " ";
                    if(java_functional->typeEntry()->functionName().isEmpty())
                        s << "call";
                    else
                        s << java_functional->typeEntry()->functionName();
                    s << "(";
                    int counter = 0;
                    for(AbstractMetaArgument * arg : java_functional->arguments()){
                        if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                            continue;
                        if(counter!=0)
                            s << ", ";
                        int arrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(arg->argumentIndex() + 1);
                        if(arrayLengthIndex>=0){
                            s << translateType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                        }else{
                            QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex() + 1);
                            if(!replacedArgType.isEmpty()){
                                registerPackage(replacedArgType);
                                s << replacedArgType.replace('$', '.');
                            }else
                                s << translateType(arg->type(), nullptr, Option(CollectionAsCollection));
                        }
                        s << " " << arg->modifiedArgumentName();
                        ++counter;
                    }
                    if(needsCallThrough){
                        s << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT;
                            bool closeParen = false;
                            if(java_functional->type()){
                                s << "return ";
                                if(replacedReturnType.isEmpty()){
                                    if (java_functional->type()->isTargetLangEnum()) {
                                        s << static_cast<const EnumTypeEntry *>(java_functional->type()->typeEntry())->qualifiedTargetLangName() << ".resolve(";
                                        closeParen = true;
                                    } else if (java_functional->type()->isTargetLangFlags()) {
                                        registerPackage(java_functional->type()->typeEntry()->qualifiedTargetLangName());
                                        s << "new " << java_functional->type()->typeEntry()->qualifiedTargetLangName().replace('$', '.') << "(";
                                        closeParen = true;
                                    }
                                }
                            }
                            if(java_functional->typeEntry()->functionName().isEmpty())
                                s << "call";
                            else
                                s << java_functional->typeEntry()->functionName();
                            s << "_native(";
                            bool needsComma = false;
                            if(java_functional->typeEntry()->isNativeIdBased()){
                                s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                                needsComma = true;
                            }
                            for(AbstractMetaArgument * arg : java_functional->arguments()){
                                if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                                    continue;
                                if(needsComma)
                                    s << ", ";
                                if(!java_functional->typeReplaced(arg->argumentIndex()+1).isEmpty()){
                                    s << arg->modifiedArgumentName();
                                }else if (arg->type()->isTargetLangEnum() || arg->type()->isTargetLangFlags()) {
                                    s << arg->modifiedArgumentName() << ".value()";
                                } else if (arg->type()->hasNativeId()) {
                                    s << "QtJambi_LibraryUtilities.internal.checkedNativeId(" << arg->modifiedArgumentName() << ")";
                                } else {
                                    s << arg->modifiedArgumentName();
                                }
                                needsComma = true;
                            }
                            if(closeParen)
                                s << ")";
                            s << ")";
                            s << ";" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl << Qt::endl
                          << INDENT << "private native ";
                        if(returnArrayLengthIndex>=0){
                            s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | EnumAsInts | NoQCollectionContainers)) << "[]";
                        }else{
                            if(!replacedReturnType.isEmpty())
                                s << replacedReturnType;
                            else
                                s << translateType(java_functional->type(), nullptr, Option(InitializerListAsArray | EnumAsInts | NoQCollectionContainers));
                        }
                        s << " ";
                        if(java_functional->typeEntry()->functionName().isEmpty())
                            s << "call";
                        else
                            s << java_functional->typeEntry()->functionName();
                        s << "_native(";
                        bool hasArg = false;
                        if(java_functional->typeEntry()->isNativeIdBased()){
                            hasArg = true;
                            s << "long nativeId";
                        }
                        for(AbstractMetaArgument * arg : java_functional->arguments()){
                            if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                                continue;
                            if(hasArg)
                                s << ", ";
                            int arrayLengthIndex = java_functional->argumentTypeArrayLengthIndex(arg->argumentIndex()+1);
                            if(arrayLengthIndex>=0){
                                s << translateType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                            }else{
                                QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex()+1);
                                if(!replacedArgType.isEmpty()){
                                    registerPackage(replacedArgType);
                                    s << replacedArgType.replace('$', '.');
                                }else
                                    s << translateType(arg->type(), nullptr, Option(InitializerListAsArray | CollectionAsCollection | UseNativeIds | EnumAsInts));
                            }
                            s << " " << arg->modifiedArgumentName();
                            hasArg = true;
                        }
                    }
                    s << ");" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }
    s << INDENT << "}" << Qt::endl << Qt::endl;
}

void JavaGenerator::writeEnum(QTextStream &s, const AbstractMetaEnum *java_enum) {
    if(java_enum->isPrivate() || java_enum->typeEntry()->codeGeneration()==TypeEntry::GenerateNothing){
        return;
    }

    QString comment;
    QTextStream commentStream(&comment);

    if (m_doc_parser) {
        commentStream << Qt::endl;
        commentStream << m_doc_parser->documentation(java_enum) << Qt::endl;
    }else{
        if(!java_enum->brief().isEmpty()){
            commentStream << "<p>" << QString(java_enum->brief())
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << "</p>" << Qt::endl;
        }
        if(java_enum->href().isEmpty()){
            commentStream << "<p>Java wrapper for Qt enum "
                          << (java_enum->typeEntry()->qualifiedCppName().startsWith("QtJambi") ? java_enum->name().replace("$", "::") : java_enum->typeEntry()->qualifiedCppName() )
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << "</p>" << Qt::endl;
        }else{
            QString url = docsUrl+java_enum->href();
            commentStream << "<p>Java wrapper for Qt enum <a href=\"" << url << "\">"
                          << (java_enum->typeEntry()->qualifiedCppName().startsWith("QtJambi") ? java_enum->name().replace("$", "::") : java_enum->typeEntry()->qualifiedCppName() )
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << "</a></p>" << Qt::endl;
        }
    }
    if (java_enum->isDeclDeprecated()) {
        if(!java_enum->deprecatedComment().isEmpty()){
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            commentStream << "@deprecated " << QString(java_enum->deprecatedComment())
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << Qt::endl;
        }
    }

    if(!java_enum->enclosingClass() || !java_enum->enclosingClass()->isFake()){
        // Write out the QFlags if present...
        FlagsTypeEntry *flags_entry = java_enum->typeEntry()->flags();
        if (flags_entry) {
            commentStream << Qt::endl << "@see " << flags_entry->targetLangName()<< Qt::endl;
        }
    }

    if(!comment.trimmed().isEmpty()){
        s << INDENT << "/**" << Qt::endl;
        commentStream.seek(0);
        while(!commentStream.atEnd()){
            s << INDENT << " * " << commentStream.readLine() << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
    }

    uint size = java_enum->typeEntry()->size();

    if (java_enum->typeEntry()->forceInteger()) {
        writeIntegerEnum(s, size, java_enum);
        return;
    }

    const AbstractMetaEnumValueList &values = java_enum->values();
    EnumTypeEntry *entry = java_enum->typeEntry();
    const QMap<QString,QString>& renamedEnumValues = entry->renamedEnumValues();

    QStringList linesPos1;
    QStringList linesPos2;
    QStringList linesPos3;
    QStringList linesPos4;
    QStringList linesBegin;
    QStringList linesEnd;
    for(const CodeSnip &snip : entry->codeSnips()) {
        if (snip.language == TypeSystem::TargetLangCode) {
            if (snip.position == CodeSnip::Position1) {
                linesPos1 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position2) {
                linesPos2 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position3) {
                linesPos3 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position4) {
                linesPos4 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Beginning) {
                linesBegin << snip.code().split("\n");
            }else if (snip.position == CodeSnip::End){
                linesEnd << snip.code().split("\n");
            }
        }
    }

    // Check if enums in QObjects are declared in the meta object. If not
    if ((java_enum->enclosingClass()->isQObject() || java_enum->enclosingClass()->isQtNamespace())
            && !java_enum->hasQEnumsDeclaration()) {
        s << INDENT << "@io.qt.QtUnlistedEnum" << Qt::endl;
    }
    if (entry->isExtensible()) {
        s << INDENT << "@io.qt.QtExtensibleEnum" << Qt::endl;
    }
    if (java_enum->isDeclDeprecated()) {
        s << INDENT << "@Deprecated" << Qt::endl;
    }
    QStringList rejected;
    for (int i = 0; i < values.size(); ++i) {
        AbstractMetaEnumValue *enum_value = values.at(i);
        if (java_enum->typeEntry()->isEnumValueRejected(enum_value->name()) && !java_enum->typeEntry()->isEnumValueRemoveRejected(enum_value->name())){
            rejected << QString("\"%1\"").arg(renamedEnumValues.value(enum_value->name(), enum_value->name()));
        }
    }
    if(!rejected.isEmpty()){
        s << INDENT << "@io.qt.QtRejectedEntries({" << rejected.join(", ") << "})" << Qt::endl;
    }

    // Generates Java 1.5 type enums
    s  << INDENT;
    if(java_enum->isProtected() && (java_enum->enclosingClass() && !java_enum->enclosingClass()->typeEntry()->designatedInterface()) && !java_enum->enclosingClass()->typeEntry()->isInterface()){
        s  << "protected";
    }else{
        s  << "public";
    }


    QString type;
    QString enumInterface;
    switch(size){
    case 8:
        type = "byte";
        if(java_enum->typeEntry()->flags())
            enumInterface = "QtByteFlagEnumerator";
        else
            enumInterface = "QtByteEnumerator";
        break;
    case 16:
        type = "short";
        if(java_enum->typeEntry()->flags())
            enumInterface = "QtShortFlagEnumerator";
        else
            enumInterface = "QtShortEnumerator";
        break;
    case 32:
        type = "int";
        if(java_enum->typeEntry()->flags())
            enumInterface = "QtFlagEnumerator";
        else
            enumInterface = "QtEnumerator";
        break;
    case 64:
        type = "long";
        if(java_enum->typeEntry()->flags())
            enumInterface = "QtLongFlagEnumerator";
        else
            enumInterface = "QtLongEnumerator";
        break;
    default:
        type = "int";
        if(java_enum->typeEntry()->flags())
            enumInterface = "QtFlagEnumerator";
        else
            enumInterface = "QtEnumerator";
        break;
    }

    s  << " enum " << java_enum->name() << " implements io.qt."+enumInterface;
    if (!java_enum->typeEntry()->implements().isEmpty()) {
        s << ", " << java_enum->typeEntry()->implements();
    }
    s << " {" << Qt::endl;

    {
        INDENTATION(INDENT)

        printExtraCode(linesPos1, s);

        AbstractMetaEnumValueList switchValues;
        bool hasStringValue = false;

        for (int i = 0; i < values.size(); ++i) {
            AbstractMetaEnumValue *enum_value = values.at(i);
            if (java_enum->typeEntry()->isEnumValueRemoveRejected(enum_value->name()))
                continue;

            //          only reject enum entries on the resolve side!
            if (!java_enum->typeEntry()->isEnumValueRejected(enum_value->name())){
                switchValues.append(enum_value);
                if(enum_value->value().userType()==QMetaType::QString){
                    hasStringValue = true;
                }
            }

            if (m_doc_parser)
                s << m_doc_parser->documentation(enum_value);

            if(enum_value->deprecated()){
                if(!enum_value->deprecatedComment().isEmpty()){
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * @deprecated " << QString(enum_value->deprecatedComment())
                         .replace("&", "&amp;")
                         .replace("<", "&lt;")
                         .replace(">", "&gt;")
                         .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                         .replace("@", "&commat;")
                         .replace("/*", "&sol;*")
                         .replace("*/", "*&sol;")
                      << Qt::endl
                      << INDENT << " */" << Qt::endl;
                }
                s << INDENT << "@Deprecated" << Qt::endl;
            }
            s << INDENT << renamedEnumValues.value(enum_value->name(), enum_value->name()) << "(";
            if(enum_value->value().userType()==QMetaType::QString){
                s << enum_value->cppName() << "()";
            }else{
                switch(size){
                case 8:
                    s << "(byte)" << enum_value->value().value<qint8>();
                    break;
                case 16:
                    s << "(short)" << enum_value->value().value<qint16>();
                    break;
                case 32:
                    s << enum_value->value().value<qint32>();
                    break;
                case 64:
                    s << enum_value->value().value<qint64>() << "L";
                    break;
                default:
                    s << enum_value->value().value<qint32>();
                    break;
                }
            }
            s << ")";

            if (i != values.size() - 1) {
                s << "," << Qt::endl;
            }
        }

        printExtraCode(linesPos2, s);

        s << ";" << Qt::endl;

        printExtraCode(linesBegin, s, true);

        s << Qt::endl
          << INDENT << "private " << java_enum->name() << "(" << type << " value) {" << Qt::endl
          << INDENT << "    this.value = value;" << Qt::endl
          << INDENT << "}" << Qt::endl << Qt::endl
          << INDENT << "/**" << Qt::endl
          << INDENT << " * {@inheritDoc}" << Qt::endl
          << INDENT << " */" << Qt::endl
          << INDENT << "public " << type << " value() {" << Qt::endl
          << INDENT << "    return value;" << Qt::endl
          << INDENT << "}" << Qt::endl << Qt::endl;

        // Write out the create flags function if its a QFlags enum
        if (entry->flags()) {
            FlagsTypeEntry *flags_entry = entry->flags();
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * Create a QFlags of the enum entry." << Qt::endl
              << INDENT << " * @return QFlags" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public " << flags_entry->targetLangName() << " asFlags() {" << Qt::endl
              << INDENT << "    return new " << flags_entry->targetLangName() << "(value);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * Combines this entry with other enum entry." << Qt::endl
              << INDENT << " * @param e enum entry" << Qt::endl
              << INDENT << " * @return new flag" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public " << flags_entry->targetLangName() << " combined("
              << entry->targetLangName().replace("$",".") << " e) {" << Qt::endl
              << INDENT << "    return new " << flags_entry->targetLangName() << "(this, e);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * Creates a new {@link " << flags_entry->targetLangName() << "} from the entries."  << Qt::endl
              << INDENT << " * @param values entries"  << Qt::endl
              << INDENT << " * @return new flag"  << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public static " << flags_entry->targetLangName() << " flags("
              << entry->targetLangName().replace("$",".") << " ... values) {" << Qt::endl
              << INDENT << "    return new " << flags_entry->targetLangName() << "(values);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl;
        }



        QString numberType;
        if (entry->isExtensible()) {
            switch(size){
            case 8:
                numberType = "java.lang.Byte";
                break;
            case 16:
                numberType = "java.lang.Short";
                break;
            case 32:
                numberType = "java.lang.Integer";
                break;
            case 64:
                numberType = "java.lang.Long";
                break;
            default:
                numberType = "java.lang.Integer";
                break;
            }
        }

        s << INDENT << "/**" << Qt::endl;
        s << INDENT << " * Returns the corresponding enum entry for the given value." << Qt::endl;
        s << INDENT << " * @param value" << Qt::endl;
        s << INDENT << " * @return enum entry" << Qt::endl;
        if (entry->isExtensible()) {
            s << INDENT << " * @throws io.qt.QNoSuchEnumValueException if value not existent in the enum" << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
        s << INDENT << "public static " << java_enum->name() << " resolve(" << type << " value) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            if(size==64 || hasStringValue){
                for (int i = 0; i < switchValues.size(); ++i) {
                    AbstractMetaEnumValue *e = switchValues.at(i);
                    if(i>0){
                        s << INDENT << "}else ";
                    }else{
                        s << INDENT;
                    }
                    s << "if(value==";
                    if(e->value().userType()==QMetaType::QString){
                        s << e->cppName() << ".value){" << Qt::endl;
                    }else{
                        switch(size){
                        case 8:
                            s << "(byte)" << e->value().value<qint8>();
                            break;
                        case 16:
                            s << "(short)" << e->value().value<qint16>();
                            break;
                        case 32:
                            s << e->value().value<qint32>();
                            break;
                        case 64:
                            s << e->value().value<qint64>() << "L";
                            break;
                        default:
                            s << e->value().value<qint32>();
                            break;
                        }
                        s << "){" << Qt::endl;
                    }
                    s << INDENT << "    return " << renamedEnumValues.value(e->name(), e->name()) << ";" << Qt::endl;
                }
                if(switchValues.size()>0){
                    s << INDENT << "} else {" << Qt::endl << INDENT << "    ";
                }else{
                    s << INDENT << "{" << Qt::endl << INDENT << "    ";
                }
            }else{
                s << INDENT << "switch (value) {" << Qt::endl;

                QSet<qint32> usedCases;
                for (int i = 0; i < switchValues.size(); ++i) {
                    AbstractMetaEnumValue *e = switchValues.at(i);

                    if(!usedCases.contains(e->value().value<qint32>())){
                        usedCases.insert(e->value().value<qint32>());
                        s << INDENT << "case ";
                        switch(size){
                        case 8:
                            s << e->value().value<qint8>();
                            break;
                        case 16:
                            s << e->value().value<qint16>();
                            break;
                        case 32:
                            s << e->value().value<qint32>();
                            break;
                        default:
                            s << e->value().value<qint32>();
                            break;
                        }
                        s << ": return " << renamedEnumValues.value(e->name(), e->name()) << ";" << Qt::endl;
                    }
                }
                s << INDENT << "default: ";
            }
            if (entry->isExtensible()) {
                s << "return QtJambi_LibraryUtilities.internal.resolveEnum(" << java_enum->name() << ".class, value, null);" << Qt::endl;
            } else {
                s << "throw new io.qt.QNoSuchEnumValueException(value);" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
        if (entry->isExtensible()) {
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * Returns the corresponding enum entry for the given value and name." << Qt::endl
              << INDENT << " * @param value" << Qt::endl
              << INDENT << " * @param name" << Qt::endl
              << INDENT << " * @return enum entry" << Qt::endl
              << INDENT << " * @throws io.qt.QNoSuchEnumValueException if value not existent in the enum or name does not match." << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public static " << java_enum->name() << " resolve(" << type << " value, String name) {" << Qt::endl;
            {
                INDENTATION(INDENT)
                s << INDENT << "if(name==null || name.isEmpty())" << Qt::endl
                  << INDENT << "    return resolve(value);" << Qt::endl
                  << INDENT << "else" << Qt::endl
                  << INDENT << "    return QtJambi_LibraryUtilities.internal.resolveEnum(" << java_enum->name() << ".class, value, name);" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }

        bool wendl = false;
        for (int i = 0; i < values.size(); ++i) {
            AbstractMetaEnumValue *enum_value = values.at(i);
            if (java_enum->typeEntry()->isEnumValueRemoveRejected(enum_value->name()))
                continue;
            if(enum_value->value().userType()==QMetaType::QString){
                s << Qt::endl
                  << INDENT << "private static native ";
                switch(size){
                case 8:
                    s << "byte ";
                    break;
                case 16:
                    s << "short ";
                    break;
                case 64:
                    s << "long ";
                    break;
                default:
                    s << "int ";
                    break;
                }
                s << enum_value->cppName() << "();";
                wendl = true;
            }
        }
        if(wendl)
            s << Qt::endl;
        s << Qt::endl << INDENT << "private final " << type << " value;" << Qt::endl;
        printExtraCode(linesEnd, s, true);
    }
    s << INDENT << "}" << Qt::endl
      << INDENT << Qt::endl;

    if(!java_enum->enclosingClass() || !java_enum->enclosingClass()->isFake()){
        // Write out the QFlags if present...
        FlagsTypeEntry *flags_entry = entry->flags();
        if (flags_entry) {
            QString flagsName = flags_entry->targetLangName();
            QCryptographicHash cryptographicHash(QCryptographicHash::Sha512);
            cryptographicHash.addData(flagsName.toLatin1());
            QByteArray result = cryptographicHash.result();
            quint64 serialVersionUID = 0;
            QDataStream stream(result);
            while(!stream.atEnd()){
                quint64 l = 0;
                stream >> l;
                serialVersionUID = serialVersionUID * 31 + l;
            }
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * QFlags type for enum {@link " << java_enum->name().replace("$",".") << "}" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public static final class " << flagsName << " extends io.qt.QFlags<" << java_enum->name().replace("$",".") << "> implements Comparable<" << flagsName << "> {" << Qt::endl
              << INDENT << "    private static final long serialVersionUID = 0x" << QString::number(serialVersionUID, 16) << "L;" << Qt::endl;
            printExtraCode(linesPos1, s, true);
            s << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * Creates a new " << flagsName << " where the flags in <code>args</code> are set." << Qt::endl
              << INDENT << "     * @param args enum entries" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    public " << flagsName << "(" << java_enum->name().replace("$",".") << " ... args){" << Qt::endl
              << INDENT << "        super(args);" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * Creates a new " << flagsName << " with given <code>value</code>." << Qt::endl
              << INDENT << "     * @param value" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    public " << flagsName << "(int value) {" << Qt::endl
              << INDENT << "        super(value);" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * Combines this flags with enum entry." << Qt::endl
              << INDENT << "     * @param e enum entry" << Qt::endl
              << INDENT << "     * @return new " << flagsName << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    @Override" << Qt::endl
              << INDENT << "    public final " << flagsName << " combined(" << java_enum->name().replace("$",".") << " e){" << Qt::endl
              << INDENT << "        return new " << flagsName << "(value() | e.value());" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * Sets the flag <code>e</code>" << Qt::endl
              << INDENT << "     * @param e enum entry" << Qt::endl
              << INDENT << "     * @return this" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    public final " << flagsName << " setFlag(" << java_enum->name().replace("$",".") << " e){" << Qt::endl
              << INDENT << "        super.setFlag(e);" << Qt::endl
              << INDENT << "        return this;" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * Sets or clears the flag <code>flag</code>" << Qt::endl
              << INDENT << "     * @param e enum entry" << Qt::endl
              << INDENT << "     * @param on set (true) or clear (false)" << Qt::endl
              << INDENT << "     * @return this" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    public final " << flagsName << " setFlag(" << java_enum->name().replace("$",".") << " e, boolean on){" << Qt::endl
              << INDENT << "        super.setFlag(e, on);" << Qt::endl
              << INDENT << "        return this;" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * Returns an array of flag objects represented by this " << flagsName << "." << Qt::endl
              << INDENT << "     * @return array of enum entries" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    @Override" << Qt::endl
              << INDENT << "    public final " << java_enum->name().replace("$",".") << "[] flags(){" << Qt::endl
              << INDENT << "        return super.flags(" << java_enum->name().replace("$",".") << ".values());" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * {@inheritDoc}" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    @Override" << Qt::endl
              << INDENT << "    public final " << flagsName << " clone(){" << Qt::endl
              << INDENT << "        return new " << flagsName << "(value());" << Qt::endl
              << INDENT << "    }" << Qt::endl << Qt::endl
              << INDENT << "    /**" << Qt::endl
              << INDENT << "     * {@inheritDoc}" << Qt::endl
              << INDENT << "     */" << Qt::endl
              << INDENT << "    @Override" << Qt::endl
              << INDENT << "    public final int compareTo(" << flagsName << " other){" << Qt::endl
              << INDENT << "        return Integer.compare(value(), other.value());" << Qt::endl
              << INDENT << "    }" << Qt::endl;
            printExtraCode(linesPos4, s, true);
            s << INDENT << "}" << Qt::endl
              << INDENT << Qt::endl;
        }
    }
}

void JavaGenerator::writePrivateNativeFunction(QTextStream &s, const AbstractMetaFunction *java_function) {
    uint exclude_attributes = AbstractMetaAttributes::Public | AbstractMetaAttributes::Protected;
    uint include_attributes = 0;

    if (java_function->isEmptyFunction()){
        exclude_attributes |= AbstractMetaAttributes::Native;
        include_attributes |= AbstractMetaAttributes::Private | AbstractMetaAttributes::Static;
    }else{
        include_attributes |= AbstractMetaAttributes::Private | AbstractMetaAttributes::Native;
    }
    if(java_function->declaringClass()->isInterface() || java_function->isConstructor()){
        include_attributes |= AbstractMetaAttributes::Static;
    }

    writeFunctionAttributes(s, java_function, -1, include_attributes, exclude_attributes,
                            Option(EnumAsInts | InitializerListAsArray
                            | ( (
                                  java_function->isEmptyFunction()
                               || java_function->isNormal()
                               || java_function->isSignal() ) ? NoOption : SkipReturnType)));
    if (java_function->isConstructor()){
        const QList<Parameter> addedParameterTypes = java_function->addedParameterTypes();
        if((java_function->declaringClass()->typeEntry()->isGenericClass()
                && java_function->declaringClass()->templateBaseClass()
                && java_function->declaringClass()->templateBaseClass()->templateArguments().size()>0) || !addedParameterTypes.isEmpty()){
            s << "<";
            bool first = true;
            for(TypeEntry * t : java_function->declaringClass()->templateBaseClass()->templateArguments()){
                if(first){
                    first = false;
                }else{
                    s << ",";
                }
                s << t->name();
            }
            for(const Parameter& p : addedParameterTypes){
                if(first){
                    first = false;
                }else{
                    s << ",";
                }
                s << p.name;
                if(!p.extends.isEmpty()){
                    s << " extends " << p.extends;
                }
            }
            s << "> ";
        }
        s << "void ";
    }
    s << java_function->marshalledName();

    s << "(";

    const AbstractMetaArgumentList& arguments = java_function->arguments();

    bool needsComma = false;
    if(java_function->isConstructor()){
        if(java_function->implementingClass()->typeEntry()->designatedInterface())
            s << java_function->implementingClass()->extractInterface()->simpleName();
        else
            s << java_function->implementingClass()->simpleName();
        if(java_function->declaringClass()->typeEntry()->isGenericClass()
                && java_function->declaringClass()->templateBaseClass()
                && java_function->declaringClass()->templateBaseClass()->templateArguments().size()>0){
            s << "<";
            bool first = true;
            for(TypeEntry * t : java_function->declaringClass()->templateBaseClass()->templateArguments()){
                if(first){
                    first = false;
                }else{
                    s << ",";
                }
                s << t->name();
            }
            s << "> ";
        }
        s << " instance";
        needsComma = true;
    }else if(!java_function->isStatic()){
        if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
            s << "long __this__nativeId";
            needsComma = true;
        }
        else if(java_function->implementingClass()->typeEntry()->designatedInterface()){
            s << java_function->implementingClass()->extractInterface()->simpleName() << " instance";
            needsComma = true;
        }
    }
    const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
    int argumentCounter = 1;
    while(addedArguments.first.contains(argumentCounter)){
        const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
        if(needsComma)
            s << ", ";
        needsComma = true;
        s << QString(argumentMod.modified_type).replace('$', '.') << " " << argumentMod.modified_name;
        ++argumentCounter;
    }
    for (int i = 0; i < arguments.count(); ++i) {
        const AbstractMetaArgument *arg = arguments.at(i);

        if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
            if (needsComma)
                s << ", ";
            needsComma = true;

            if(java_function->isConstructor())
                writeArgument(s, java_function, arg, Option(CollectionAsCollection | InitializerListAsArray));
            else if (!arg->type()->hasNativeId() || !java_function->typeReplaced(arg->argumentIndex()+1).isEmpty())
                writeArgument(s, java_function, arg, Option(EnumAsInts | UseNativeIds | CollectionAsCollection | InitializerListAsArray));
            else
                s << "long " << arg->modifiedArgumentName();
        }
        ++argumentCounter;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
            if(needsComma)
                s << ", ";
            needsComma = true;
            s << QString(argumentMod.modified_type).replace('$', '.') << " " << argumentMod.modified_name;
            ++argumentCounter;
        }
    }
    for(const ArgumentModification& argumentMod : addedArguments.second){
        if(needsComma)
            s << ", ";
        needsComma = true;
        registerPackage(argumentMod.modified_type);
        s << QString(argumentMod.modified_type).replace('$', '.') << " " << argumentMod.modified_name;
    }
    s << ")";

    // Make sure people don't call the private functions
    if (java_function->isEmptyFunction()) {
        s << " throws io.qt.QNoImplementationException {" << Qt::endl
          << INDENT << "    throw new io.qt.QNoImplementationException();" << Qt::endl
          << INDENT << "}" << Qt::endl;
    } else {
        QString throws = java_function->throws();
        if(!throws.isEmpty()){
            s << " throws " << throws << " ";
        }
        s << ";" << Qt::endl
          << INDENT << Qt::endl;
    }
}

void JavaGenerator::writeFunctionCallForOwnership(QTextStream &s, const AbstractMetaFunction *, TypeSystem::Ownership owner, const QString& variable) {
    if (owner == TypeSystem::CppOwnership) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.setCppOwnership(" << variable << ");" << Qt::endl;
    } else if (owner == TypeSystem::TargetLangOwnership) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.setJavaOwnership(" << variable << ");" << Qt::endl;
    } else if (owner == TypeSystem::DefaultOwnership) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.setDefaultOwnership(" << variable << ");" << Qt::endl;
    } else if (owner == TypeSystem::Invalidate) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.invalidateObject(" << variable << ");" << Qt::endl;
    }
}

void JavaGenerator::writeOwnershipForContainer(QTextStream &s, const AbstractMetaFunction *java_function, TypeSystem::Ownership owner, AbstractMetaType *type, const QString &arg_name) {
    Q_ASSERT(type->isContainer());

    if(owner!=TypeSystem::IgnoreOwnership && owner!=TypeSystem::InvalidOwnership){
        s << INDENT << "for (" << type->instantiations().at(0)->fullName() << " i : "
          << arg_name << ")" << Qt::endl
          << "    ";
        writeFunctionCallForOwnership(s, java_function, owner, "i");
    }
}

static FunctionModificationList get_function_modifications_for_class_hierarchy(const AbstractMetaFunction *java_function, bool reverse) {
    FunctionModificationList mods;
    const AbstractMetaClass *cls = java_function->implementingClass();
    while (cls) {
        if(reverse){
            mods = java_function->modifications(cls) + mods;
        }else{
            mods += java_function->modifications(cls);
        }

        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    return mods;
}

bool JavaGenerator::hasCodeInjections(const AbstractMetaFunction *java_function,
                                      const QSet<CodeSnip::Position>& positions) {
    const AbstractMetaClass *cls = java_function->implementingClass();
    while (cls!= nullptr) {
        if(java_function->hasCodeInjections(cls, TypeSystem::TargetLangCode, positions))
            return true;
        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    return false;
}

void JavaGenerator::writeInjectedCode(QTextStream &s, const AbstractMetaFunction *java_function,
                                      CodeSnip::Position position) {
    FunctionModificationList mods = get_function_modifications_for_class_hierarchy(java_function,
                                                                                   position==CodeSnip::Position1
                                                                                   || position==CodeSnip::Position2
                                                                                   || position==CodeSnip::Beginning
                                                                                );

    for(const FunctionModification& mod : mods) {
        if (mod.snips.count() <= 0)
            continue ;

        for(const CodeSnip& snip : mod.snips) {
            if (snip.position != position)
                continue ;

            if (snip.language != TypeSystem::TargetLangCode)
                continue ;

            QString code = snip.code();
            ArgumentMap map = snip.argumentMap;
            ArgumentMap::iterator it = map.begin();
            const AbstractMetaArgumentList& arguments = java_function->arguments();
            for (;it != map.end();++it) {
                int pos = it.key() - 1;
                QString meta_name = it.value();

                if(pos == -1){
                    code = code.replace(meta_name, "__qt_return_value");
                }else if (pos >= 0 && pos < arguments.count()) {
                    code = code.replace(meta_name, arguments.at(pos)->modifiedArgumentName());
                } else {
                    QString debug = QString("argument map specifies invalid argument index %1"
                                            "for function '%2'")
                                    .arg(pos + 1).arg(java_function->name());
                    ReportHandler::warning(debug);
                }

            }
            code = code.replace("%this", "this");
            QStringList lines = code.split("\n");
            printExtraCode(lines, s);
        }
    }
}

void JavaGenerator::writeJavaCallThroughContents(QTextStream &s, const AbstractMetaFunction *java_function, uint attributes) {
//    if((java_function->isAbstract() || !(java_function->originalAttributes() & AbstractMetaAttributes::Public)) && !java_function->implementingClass()->generateShellClass()){
//        s << INDENT << "throw new io.qt.QNoImplementationException();" << Qt::endl;
//    }else
//    if(java_function->isAbstract() && java_function->implementingClass()->hasUnimplmentablePureVirtualFunction()){
//        s << INDENT << "throw new io.qt.QNoImplementationException();" << Qt::endl;
//    }else
    {
        const AbstractMetaArgumentList& arguments = java_function->arguments();

        QString lines;
        QString setArgumentOwnership;
        {
            QTextStream s(&lines);
            QTextStream s2(&setArgumentOwnership);
            writeInjectedCode(s, java_function, CodeSnip::Beginning);

            for (int i = 0; i < arguments.count(); ++i) {
                AbstractMetaArgument *arg = arguments.at(i);
                AbstractMetaType *type = arg->type();

                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    bool nonNull = false;
                    if ( java_function->nullPointersDisabled(java_function->implementingClass(), i + 1)
                            || (arg->type()->indirections().isEmpty() && arg->type()->isObject())
                    ) {
                        s << INDENT << "java.util.Objects.requireNonNull(" << arg->modifiedArgumentName() << ", \"Argument '" << arguments.at(i)->modifiedArgumentName() << "': null not expected.\");" << Qt::endl;
                        nonNull = true;
                    }

                    OwnershipRule ownershipRule = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, i + 1);
                    if (ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership) {
                        if (arg->type()->isContainer()){
                            if(nonNull && ownershipRule.condition.isEmpty()){
                                writeOwnershipForContainer(s2, java_function, ownershipRule.ownership, arg->type(), arg->modifiedArgumentName());
                            }else{
                                s2 << INDENT << "if (";
                                if(!nonNull){
                                    s2 << arg->modifiedArgumentName() << " != null";
                                    if(!ownershipRule.condition.isEmpty()){
                                        s2 << " && ";
                                    }
                                }
                                if(!ownershipRule.condition.isEmpty()){
                                    s2 << ownershipRule.condition;
                                }
                                s2 << ") {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    writeOwnershipForContainer(s2, java_function, ownershipRule.ownership, arg->type(), arg->modifiedArgumentName());
                                }
                                s2 << INDENT << "}" << Qt::endl;
                            }
                        }else{
                            if(ownershipRule.condition.isEmpty()){
                                writeFunctionCallForOwnership(s2, java_function, ownershipRule.ownership, arg->modifiedArgumentName());
                            }else{
                                s2 << INDENT << "if (" << ownershipRule.condition << ") {" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    writeFunctionCallForOwnership(s2, java_function, ownershipRule.ownership, arg->modifiedArgumentName());
                                }
                                s2 << INDENT << "}" << Qt::endl;
                            }
                        }
                    }

                    if (type->isArray()) {
                        s << INDENT << "if (" << arg->modifiedArgumentName() << ".length != " << type->arrayElementCount() << ")" << Qt::endl
                          << INDENT << "    " << "throw new IllegalArgumentException(\"Argument '"
                          << arg->modifiedArgumentName() << "': Wrong number of elements in array. Found: \" + "
                          << arg->modifiedArgumentName() << ".length + \", expected: " << type->arrayElementCount() << "\");"
                          << Qt::endl << Qt::endl;
                    }

                    if (type->isEnum()) {
                        const EnumTypeEntry *et = static_cast<const EnumTypeEntry *>(type->typeEntry());
                        if (et->forceInteger()) {
                            if (!et->lowerBound().isEmpty()) {
                                s << INDENT << "if (" << arg->modifiedArgumentName() << " < " << et->lowerBound() << ")" << Qt::endl
                                  << INDENT << "    throw new IllegalArgumentException(\"Argument " << arg->modifiedArgumentName()
                                  << " is less than lowerbound " << et->lowerBound() << "\");" << Qt::endl;
                            }
                            if (!et->upperBound().isEmpty()) {
                                s << INDENT << "if (" << arg->modifiedArgumentName() << " > " << et->upperBound() << ")" << Qt::endl
                                  << INDENT << "    throw new IllegalArgumentException(\"Argument " << arg->modifiedArgumentName()
                                  << " is greated than upperbound " << et->upperBound() << "\");" << Qt::endl;
                            }
                        }
                    }

                    if(java_function->argumentTypeArray(arg->argumentIndex()+1)){
                        int minArrayLength = java_function->argumentTypeArrayLengthMinValue(arg->argumentIndex()+1);
                        int maxArrayLength = java_function->argumentTypeArrayLengthMaxValue(arg->argumentIndex()+1);
                        if(minArrayLength>0){
                            if(maxArrayLength==minArrayLength){
                                s << INDENT << "if(";
                                if(!nonNull){
                                    s << arg->modifiedArgumentName() << "!=null && ";
                                }
                                s << arg->modifiedArgumentName() << ".length != " << minArrayLength << ")" << Qt::endl;
                                s << INDENT << "    throw new IllegalArgumentException(\"Argument '" << arg->modifiedArgumentName() << "': array of length " << minArrayLength << " expected.\");" << Qt::endl;
                            }else if(maxArrayLength>minArrayLength){
                                s << INDENT << "if(";
                                if(!nonNull){
                                    s << arg->modifiedArgumentName() << "!=null && (";
                                }
                                s << arg->modifiedArgumentName() << ".length < " << minArrayLength << " || " << arg->modifiedArgumentName() << ".length > " << maxArrayLength;
                                if(!nonNull){
                                    s << ")";
                                }
                                s << ")" << Qt::endl
                                  << INDENT << "    throw new IllegalArgumentException(\"Argument '" << arg->modifiedArgumentName() << "': array of length between " << minArrayLength << " and " << maxArrayLength << " expected.\");" << Qt::endl;
                            }else{
                                s << INDENT << "if(";
                                if(!nonNull){
                                    s << arg->modifiedArgumentName() << "!=null && ";
                                }
                                s << arg->modifiedArgumentName() << ".length < " << minArrayLength << ")" << Qt::endl
                                  << INDENT << "    throw new IllegalArgumentException(\"Argument '"
                                            << arg->modifiedArgumentName() << "': Wrong number of elements in array. Found: \" + "
                                            << arg->modifiedArgumentName() << ".length + \", expected: " << minArrayLength << "\");" << Qt::endl;
                            }
                        }
                    }else if(java_function->argumentTypeBuffer(arg->argumentIndex()+1)){
                        int minArrayLength = java_function->argumentTypeArrayLengthMinValue(arg->argumentIndex()+1);
                        int maxArrayLength = java_function->argumentTypeArrayLengthMaxValue(arg->argumentIndex()+1);
                        if(minArrayLength>0){
                            if(maxArrayLength==minArrayLength){
                                s << INDENT << "if(";
                                if(!nonNull){
                                    s << arg->modifiedArgumentName() << "!=null && ";
                                }
                                s << arg->modifiedArgumentName() << ".capacity() != " << minArrayLength << ")" << Qt::endl;
                                s << INDENT << "    throw new IllegalArgumentException(\"Argument '" << arg->modifiedArgumentName() << "': buffer of capacity " << minArrayLength << " expected.\");" << Qt::endl;
                            }else if(maxArrayLength>minArrayLength){
                                s << INDENT << "if(";
                                if(!nonNull){
                                    s << arg->modifiedArgumentName() << "!=null && (";
                                }
                                s << arg->modifiedArgumentName() << ".capacity() < " << minArrayLength << " || " << arg->modifiedArgumentName() << ".capacity() > " << maxArrayLength;
                                if(!nonNull){
                                    s << ")";
                                }
                                s << ")" << Qt::endl
                                  << INDENT << "    throw new IllegalArgumentException(\"Argument '" << arg->modifiedArgumentName() << "': buffer of capacity between " << minArrayLength << " and " << maxArrayLength << " expected.\");" << Qt::endl;
                            }else{
                                s << INDENT << "if(";
                                if(!nonNull){
                                    s << arg->modifiedArgumentName() << "!=null && ";
                                }
                                s << arg->modifiedArgumentName() << ".capacity() < " << minArrayLength << ")" << Qt::endl;
                                s << INDENT << "    throw new IllegalArgumentException(\"Argument '" << arg->modifiedArgumentName() << "': buffer minimum of capacity " << minArrayLength << " expected.\");" << Qt::endl;
                            }
                        }
                    }
                }
            }
        }

        s << lines;
        writeInjectedCode(s, java_function, CodeSnip::Position1);
        writeInjectedCode(s, java_function, CodeSnip::Position2);

        bool has_argument_referenceCounts = false;
        QMap<int,QList<ReferenceCount>> referenceCounts;
        for (int i = -1; i <= arguments.size(); ++i) {
            referenceCounts[i] = java_function->referenceCounts(java_function->implementingClass(), i);
            if (referenceCounts[i].size() > 0) {
                for(const ReferenceCount& refCount : referenceCounts[i]) {
                    // We just want to know this to secure return value into local variable
                    // to hold over ReferenceCount management later on.
                    if (refCount.action != ReferenceCount::Ignore) {
                        // Something active have been specified
                        has_argument_referenceCounts = true;
                        break;
                    }
                }
            }
        }

        // Lookup if there is a reference-count action required on the return value.
        AbstractMetaType *return_type = java_function->type();
        QString new_return_type = java_function->typeReplaced(0);
        registerPackage(new_return_type);
        new_return_type = new_return_type.replace('$', '.');
        bool has_return_type = new_return_type != "void"
        && (!new_return_type.isEmpty() || return_type);
        TypeSystem::Ownership returnValueOwnership = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, 0).ownership;
        TypeSystem::Ownership thisOwnership = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, -1).ownership;

        bool has_code_injections_at_the_end = hasCodeInjections(java_function, {CodeSnip::End, CodeSnip::Position4, CodeSnip::Position3});

        bool needs_return_variable = has_return_type
                                     && ( !setArgumentOwnership.isEmpty()
                                          || (returnValueOwnership != TypeSystem::InvalidOwnership && returnValueOwnership != TypeSystem::IgnoreOwnership)
                                          || (thisOwnership != TypeSystem::InvalidOwnership && thisOwnership != TypeSystem::IgnoreOwnership)
                                          || has_argument_referenceCounts
                                          || has_code_injections_at_the_end);

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

            if (return_type && new_return_type.isEmpty()) {
                if (return_type->isTargetLangEnum() && new_return_type.isEmpty()) {
                    s << static_cast<const EnumTypeEntry *>(return_type->typeEntry())->qualifiedTargetLangName() << ".resolve(";
                } else if (return_type->isTargetLangFlags()) {
                    registerPackage(return_type->typeEntry()->qualifiedTargetLangName());
                    s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace('$', '.') << "(";
                }
            }
        }

        bool needsComma = false;
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
            if (java_function->isConstructor()) {
                s << "this";
                needsComma = true;
            }
        }

        if (!java_function->isConstructor() && !java_function->isStatic()){
            if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                needsComma = true;
            }else if(java_function->implementingClass()->typeEntry()->designatedInterface()){
                s << "this";
                needsComma = true;
            }
        }

        const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
        int argumentCounter = 1;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
            if(needsComma)
                s << ", ";
            needsComma = true;
            s << argumentMod.modified_name;
            ++argumentCounter;
        }
        for (int i = 0; i < arguments.count(); ++i) {
            const AbstractMetaArgument *arg = arguments.at(i);
            const AbstractMetaType *type = arg->type();

            if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                if (needsComma)
                    s << ", ";
                needsComma = true;

                if(java_function->isConstructor()){
                    s << arg->modifiedArgumentName();
                } else if(!java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()){
                    s << arg->modifiedArgumentName();
                }else if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                    s << arg->modifiedArgumentName() << ".value()";
                }else if (type->hasNativeId()) {
                    s << "QtJambi_LibraryUtilities.internal.checkedNativeId(" << arg->modifiedArgumentName() << ")";
                } else {
                    s << arg->modifiedArgumentName();
                }
            }
            ++argumentCounter;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                if(needsComma)
                    s << ", ";
                needsComma = true;
                s << argumentMod.modified_name;
                ++argumentCounter;
            }
        }
        for(const ArgumentModification& argumentMod : addedArguments.second){
            if(needsComma)
                s << ", ";
            needsComma = true;
            s << argumentMod.modified_name;
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
        if (return_type && new_return_type.isEmpty() && (return_type->isTargetLangEnum() || return_type->isTargetLangFlags()))
            s << ")";

        s << ";" << Qt::endl;

        if (thisOwnership != TypeSystem::InvalidOwnership && thisOwnership != TypeSystem::IgnoreOwnership){
            writeFunctionCallForOwnership(s, java_function, thisOwnership, "this");
        }

        s << setArgumentOwnership;

        for(const ReferenceCount& refCount : referenceCounts[-1])
            writeReferenceCount(s, refCount, -1, java_function, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));

        // We must ensure we retain a Java hard-reference over the native method call
        // so that the GC will not destroy the C++ object too early.  At this point we
        // have called the native method call so can manage referenceCount issues.
        // First the input arguments
        for (const AbstractMetaArgument* argument : arguments) {
            for(const ReferenceCount& refCount : referenceCounts[argument->argumentIndex()+1])
                writeReferenceCount(s, refCount, argument->argumentIndex()+1, java_function, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
        }

        // Then the return value
        for(const ReferenceCount& referenceCount : referenceCounts[0]) {
            writeReferenceCount(s, referenceCount, 0, java_function, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
        }

        writeInjectedCode(s, java_function, CodeSnip::Position3);
        writeInjectedCode(s, java_function, CodeSnip::Position4);
        writeInjectedCode(s, java_function, CodeSnip::End);

        if (needs_return_variable) {
            if (returnValueOwnership != TypeSystem::InvalidOwnership && returnValueOwnership != TypeSystem::IgnoreOwnership) {
                if (return_type->isContainer()){
                    s << INDENT << "if (__qt_return_value != null) {" << Qt::endl;
                    writeOwnershipForContainer(s, java_function, returnValueOwnership, return_type, "__qt_return_value");
                    s << INDENT << "}" << Qt::endl;
                }else{
                    writeFunctionCallForOwnership(s, java_function, returnValueOwnership, "__qt_return_value");
                }
            }
            if (!java_function->argumentReplaced(0).isEmpty()) {
                s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
            }else{
                s << INDENT << "return __qt_return_value;" << Qt::endl;
            }
        }else if (!java_function->argumentReplaced(0).isEmpty()) {
            s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
        }
    }
}

void JavaGenerator::writeSignal(QTextStream &s, const AbstractMetaFunction *java_function) {
    Q_ASSERT(java_function->isSignal());

    if (java_function->isModifiedRemoved(TypeSystem::TargetLangCode) || java_function->isPrivate())
        return ;

    AbstractMetaArgumentList arguments;
    for (int i = 0; i < java_function->arguments().size(); ++i) {
        AbstractMetaArgument *arg = java_function->arguments().at(i);
        if(!java_function->argumentRemoved(arg->argumentIndex()+1)){
            arguments << arg;
        }
    }
    auto sz = arguments.size();

    QList<QString> defaultValueArgumentType;
    QList<QString> defaultValueExpressions;

    QString signalTypeName("Signal");
    if (java_function->isPrivateSignal()) {
        signalTypeName = "PrivateSignal";
    }else{
        for (const AbstractMetaArgument* argument : arguments) {
            QString defaultValueExpression = argument->defaultValueExpression();
            if(!defaultValueExpression.isEmpty()){
                defaultValueExpressions << defaultValueExpression;

                QString type = java_function->typeReplaced(argument->argumentIndex() + 1);

                if (type.isEmpty()){
                    type = translateType(argument->type(), java_function->implementingClass(), Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers));
                }else{
                    registerPackage(type);
                    type = type.replace('$', '.');
                }

                defaultValueArgumentType << type;
            }
        }
    }

    signalTypeName += QString::number(sz);

    QString signalDefaultArgumentExpressions;
    if(!defaultValueExpressions.isEmpty()){
        auto dsz = defaultValueExpressions.size();
        signalTypeName += "Default";
        signalTypeName += QString::number(dsz);
        QTextStream s2(&signalDefaultArgumentExpressions);
        for (int i = 0; i < dsz; ++i) {
            if (i > 0)
                s2 << ", ";
            s2 << "()->" << defaultValueExpressions.at(i);
        }
    }
    m_exportedPackages.clear();
    QString constructorCall = signalTypeName;
    if (sz > 0) {
        constructorCall += "<>";
        signalTypeName += "<";
        bool begin = true;
        for (const AbstractMetaArgument* argument : arguments) {
            if (begin){
                begin = false;
            }else{
                signalTypeName += ", ";
            }

            QString modifiedType = java_function->typeReplaced(argument->argumentIndex() + 1);

            QString boxedType;
            if (modifiedType.isEmpty()){
                boxedType = translateType(argument->type(), java_function->implementingClass(), Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers));
            }else{
                registerPackage(modifiedType);
                boxedType = modifiedType.replace('$', '.');
            }
            if(boxedType=="java.lang.Integer"){
                boxedType = "@io.qt.QtPrimitiveType Integer";
            }else if(boxedType=="java.lang.Short"){
                boxedType = "@io.qt.QtPrimitiveType Short";
            }else if(boxedType=="java.lang.Byte"){
                boxedType = "@io.qt.QtPrimitiveType Byte";
            }else if(boxedType=="java.lang.Long"){
                boxedType = "@io.qt.QtPrimitiveType Long";
            }else if(boxedType=="java.lang.Double"){
                boxedType = "@io.qt.QtPrimitiveType Double";
            }else if(boxedType=="java.lang.Float"){
                boxedType = "@io.qt.QtPrimitiveType Float";
            }else if(boxedType=="java.lang.Boolean"){
                boxedType = "@io.qt.QtPrimitiveType Boolean";
            }else if(boxedType=="java.lang.Character"){
                boxedType = "@io.qt.QtPrimitiveType Character";
            }
            signalTypeName += boxedType;
        }
        signalTypeName += ">";
    }

    uint exclude_attributes = AbstractMetaAttributes::Abstract
                             | AbstractMetaAttributes::Native;
    uint include_attributes = AbstractMetaAttributes::Final;

    QString signalName = java_function->name();
    FunctionModificationList mods = java_function->modifications(java_function->implementingClass());
    for(const FunctionModification& mod : mods) {
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

    s << Qt::endl;
    // Insert Javadoc
    QString comment;
    QTextStream commentStream(&comment);
    if (m_doc_parser) {
        QString signature = functionSignature(java_function,
                                              include_attributes | NoBlockedSlot | NoQCollectionContainers,
                                              exclude_attributes, NoSuppressExports);
        QString docs = m_doc_parser->documentationForSignal(signature);
        if (docs.isEmpty()) {
            docs = m_doc_parser->documentationForSignal(signature);
        }
        commentStream << m_doc_parser->documentationForSignal(signature);
    }else{
        if(!java_function->brief().isEmpty()){
            commentStream << "<p>" << QString(java_function->brief())
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << "</p>" << Qt::endl;
        }
        if(!java_function->href().isEmpty()){
            QString url = docsUrl+java_function->href();
            commentStream << "<p>See <a href=\"" << url << "\">";
            if(java_function->declaringClass())
                commentStream << java_function->declaringClass()->qualifiedCppName()
                                 .replace("<JObjectWrapper>", "")
                                 .replace("QtJambi", "Q")
                                 .replace("QVoid", "Q")
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "::";
            commentStream << QString(java_function->originalSignature())
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << "</a></p>" << Qt::endl;
        }
    }
    if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
        if(!comment.isEmpty())
            commentStream << Qt::endl;
        writeDeprecatedComment(commentStream, java_function);
    }
    if(!comment.trimmed().isEmpty()){
        s << INDENT << "/**" << Qt::endl;
        commentStream.seek(0);
        while(!commentStream.atEnd()){
            s << INDENT << " * " << commentStream.readLine() << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
    }

    writeFunctionAttributes(s, java_function, -1, include_attributes, exclude_attributes,
                            Option(SkipReturnType | InitializerListAsArray | NoQCollectionContainers));
    s << signalTypeName << " " << signalName << " = new " << constructorCall << "(" << signalDefaultArgumentExpressions << ");" << Qt::endl;
}

void JavaGenerator::writeMultiSignal(QTextStream &s, const AbstractMetaFunctionList& signalList){
    const QString& signalName = signalList.first()->name();
    QMap<qint64,QSet<QString> > signalTypesByArgs;
    QHash<AbstractMetaFunction*,QString> signalTypes;
    QMap<qint64,QList<QString>> signalParameterClassesList;
    QMap<qint64,int> argumentCountMap;
    s << Qt::endl
      << INDENT << "/**" << Qt::endl
      << INDENT << " * <p>Wrapper class for overloaded signals:</p>" << Qt::endl
      << INDENT << " * <ul>" << Qt::endl;
    for(AbstractMetaFunction* java_function : signalList){
        s << INDENT << " * <li><code>" << java_function->signature()
             .replace("&", "&amp;")
             .replace("<", "&lt;")
             .replace(">", "&gt;")
             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
             .replace("@", "&commat;")
             .replace("/*", "&sol;*")
             .replace("*/", "*&sol;")
          << "</code></li>" << Qt::endl;
    }
    s << INDENT << " * </ul>" << Qt::endl
      << INDENT << " */" << Qt::endl
      << INDENT << "public final class MultiSignal_" << signalName << " extends MultiSignal{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "private MultiSignal_" << signalName << "(){" << Qt::endl;
        {
            INDENTATION(INDENT)
            QString configurations;
            bool hasDefaults = false;
            {
                QTextStream s(&configurations);
                bool first = true;
                QSet<qint64> argCounts;
                for(AbstractMetaFunction* java_function : signalList){
                    const AbstractMetaArgumentList& arguments = java_function->arguments();
                    auto sz = arguments.count();
                    argCounts.insert(sz);
                    argumentCountMap[sz]++;

                    QList<QString> defaultValueArgumentType;
                    QList<QString> defaultValueExpressions;

                    QString constructorCall("Signal");
                    if (java_function->isPrivateSignal()) {
                        constructorCall = "PrivateSignal";
                    }else{
                        for (int i = 0; i < sz; ++i) {
                            auto arg = arguments.at(i);
                            QString defaultValueExpression = arguments.at(i)->defaultValueExpression();
                            if(!defaultValueExpression.isEmpty()){
                                defaultValueExpressions << defaultValueExpression;

                                QString type = java_function->typeReplaced(arg->argumentIndex() + 1);

                                if (type.isEmpty()){
                                    type = translateType(arg->type(), java_function->implementingClass(), Option(BoxedPrimitive | InitializerListAsArray | NoQCollectionContainers));
                                }else{
                                    registerPackage(type);
                                    type = type.replace('$', '.');
                                }

                                defaultValueArgumentType << type;
                            }
                        }
                    }

                    constructorCall += QString::number(sz);

                    QString signalDefaultArgumentExpressions;
                    if((hasDefaults = !defaultValueExpressions.isEmpty())){
                        auto dsz = defaultValueExpressions.size();
                        constructorCall += "Default";
                        constructorCall += QString::number(dsz);
                        QTextStream s2(&signalDefaultArgumentExpressions);
                        for (int i = 0; i < dsz; ++i) {
                            if (i > 0)
                                s2 << ", ";
                            s2 << "()->" << defaultValueExpressions.at(i);
                        }
                    }
                    QString signalParameterClasses;
                    QString signalObjectType = constructorCall;
                    signalTypesByArgs[sz] << signalObjectType;
                    if (sz > 0) {
                        constructorCall += "<>";
                        signalObjectType += "<";
                        for (int i = 0; i < sz; ++i) {
                            auto arg = arguments.at(i);
                            if (i > 0){
                                signalParameterClasses += ", ";
                                signalObjectType += ", ";
                            }

                            QString modifiedType = java_function->typeReplaced(arg->argumentIndex() + 1);

                            QString boxedType;
                            QString unboxedType;
                            if (modifiedType.isEmpty()){
                                boxedType += translateType(arg->type(), java_function->implementingClass(), Option(BoxedPrimitive | InitializerListAsArray));
                                QString type = translateType(arg->type(), java_function->implementingClass(), Option(InitializerListAsArray));
                                auto idx = type.indexOf('<');
                                if(idx>0){
                                    type = type.mid(0, idx);
                                    if(type=="io.qt.core.QList")
                                        type = "java.util.List";
                                    else if(type=="io.qt.core.QMap" || type=="io.qt.core.QHash")
                                        type = "java.util.Map";
                                    else if(type=="io.qt.core.QSet")
                                        type = "java.util.Set";
                                }
                                unboxedType += type;
                            }else{
                                registerPackage(modifiedType);
                                boxedType += modifiedType.replace('$', '.');
                                unboxedType += modifiedType.replace('$', '.');
                            }
                            signalObjectType += boxedType;
                            signalParameterClasses += unboxedType+".class";
                        }
                        signalObjectType += ">";
                        signalParameterClassesList[sz] << signalParameterClasses;
                    }
                    signalTypes[java_function] = signalObjectType;

                    uint exclude_attributes = AbstractMetaAttributes::Abstract
                                             | AbstractMetaAttributes::Native;
                    uint include_attributes = AbstractMetaAttributes::Public;

                    FunctionModificationList mods = java_function->modifications(java_function->implementingClass());
                    for(const FunctionModification& mod : mods) {
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

                    if(!first){
                        s << ", ";
                    }
                    if (sz > 0) {
                        s << "new SignalConfiguration(" << signalParameterClasses << ", new " << constructorCall << "(" << signalDefaultArgumentExpressions << "))";
                    }else{
                        s << "new SignalConfiguration(new " << constructorCall << "(" << signalDefaultArgumentExpressions << "))";
                    }
                    first = false;
                }
            }
            /*if(hasDefaults){
                s << INDENT << "super(" << configurations << ");" << Qt::endl;
            }else{*/
                s << INDENT << "super();" << Qt::endl;
            //}
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
        QSet<qint64> writtenConnects;
        for(QMap<qint64,int>::const_iterator it=argumentCountMap.begin(); it!=argumentCountMap.end(); it++){
            QString parameters;
            QStringList parameterClasses;
            QStringList classes;
            QStringList vars;
            QString annotations;
            {
                QTextStream s2(&annotations);
                for(const QString& signalParameterClasses : signalParameterClassesList[it.key()]){
                    s2 << "@io.qt.QtAllowedTypeSet({" << signalParameterClasses << "})" << Qt::endl << INDENT;
                }
            }
            if(it.key()>0){
                QStringList params;
                for(int j=0; j<it.key(); j++){
                    QString letter = QString(QChar('A'+j));
                    params << letter;
                    classes << QString(QLatin1String("Class<%1> type%2")).arg(letter).arg(j+1);
                    vars << QString(QLatin1String("type%1")).arg(j+1);
                    parameterClasses << QString(QLatin1String("(Class<%1>)parameters[%2]")).arg(letter).arg(j);
                }
                parameters = QString(QLatin1String("<%1>")).arg(params.join(","));
            };

            s << INDENT << "/**" << Qt::endl;
            if(it.key()==0){
                s << INDENT << " * <p>Provides an overloaded parameterless signal.</p>" << Qt::endl
                  << INDENT << " * @return overloaded signal" << Qt::endl;
            }else{
                if(it.key()==1){
                    s << INDENT << " * <p>Provides an overloaded signal by parameter type.</p>" << Qt::endl;
                }else{
                    s << INDENT << " * <p>Provides an overloaded signal by parameter types.</p>" << Qt::endl;
                }
                if(signalParameterClassesList[it.key()].size()==1){
                    s << INDENT << " * <p>The only valid call is <code>" << signalName << ".overload(" << signalParameterClassesList[it.key()].first() << ")</code></p>" << Qt::endl;
                }else{
                    s << INDENT << " * <p>The only valid calls are:</p><ul>" << Qt::endl;
                    for(const QString& signalParameterClasses : signalParameterClassesList[it.key()]){
                        s << INDENT << " * <li><code>" << signalName << ".overload(" << signalParameterClasses << ")</code></li>" << Qt::endl;
                    }
                    s << INDENT << " * </ul>" << Qt::endl;
                }
                s << INDENT << " * <p>{@link io.qt.QNoSuchSignalException} is thrown otherwise.</p>" << Qt::endl << Qt::endl;
                for(int j=0; j<it.key(); j++){
                    s << INDENT << " * @param <" << QChar('A'+j) << "> signal parameter type"<< Qt::endl;
                }
                for(int j=0; j<it.key(); j++){
                    s << INDENT << " * @param type" << QString::number(j+1) << " value of type " << QChar('A'+j) << Qt::endl;
                }
                s << INDENT << " * @return overloaded signal" << Qt::endl
                  << INDENT << " * @throws io.qt.QNoSuchSignalException if signal is not available" << Qt::endl;
            }
            s << INDENT << " */" << Qt::endl;

            if(signalTypesByArgs[it.key()].size()==1){
                s << INDENT << annotations << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << signalTypesByArgs[it.key()].begin().operator *() << parameters << " overload(" << classes.join(", ") << ") throws io.qt.QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return (" << signalTypesByArgs[it.key()].begin().operator *() << parameters << ")super.overload(" << vars.join(", ") << ");" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
            }else{
                s << INDENT << annotations << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.AbstractPrivateSignal" << it.key() << parameters << " overload(" << classes.join(", ") << ") throws io.qt.QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.overload(" << vars.join(", ") << ");" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
            }

            if(it.key()>0){
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * Initializes a connection to the <i>slot</i>." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param slot the slot to be connected" << Qt::endl
                  << INDENT << " * @param connectionType type of connection" << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.Connection connect(io.qt.core.QMetaObject.Slot" << it.key() << parameters << " slot, io.qt.core.Qt.ConnectionType... connectionType) throws io.qt.QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect(slot, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Removes the connection to the given <i>slot</i>." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param slot the slot to be disconnected" << Qt::endl
                  << INDENT << " * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject.Slot" << it.key() << parameters << " slot) {" << Qt::endl
                  << INDENT << "    return super.disconnect(slot);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second" << Qt::endl
                  << INDENT << " * signal to be emitted as well." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal. This will be emitted whenever this signal is emitted." << Qt::endl
                  << INDENT << " * @param connectionType One of the connection types defined in the Qt interface." << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.Connection connect(io.qt.core.QMetaObject.Connectable" << it.key() << parameters << " signal, io.qt.core.Qt.ConnectionType... connectionType) throws io.qt.QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect((io.qt.core.QMetaObject.AbstractSignal)signal, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Disconnects a signal from another signal if the two were previously connected by a call to connect." << Qt::endl
                  << INDENT << " * A call to this function will assure that the emission of the first signal will not cause the emission of the second." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal." << Qt::endl
                  << INDENT << " * @return <code>true</code> if the two signals were successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject.Connectable" << it.key() << parameters << " signal) {" << Qt::endl
                  << INDENT << "    return super.disconnect((io.qt.core.QMetaObject.AbstractSignal)signal);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second" << Qt::endl
                  << INDENT << " * signal to be emitted as well." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal. This will be emitted whenever this signal is emitted." << Qt::endl
                  << INDENT << " * @param connectionType One of the connection types defined in the Qt interface." << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.Connection connect(io.qt.core.QMetaObject.AbstractPublicSignal" << it.key() << parameters << " signal, io.qt.core.Qt.ConnectionType... connectionType) throws io.qt.QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect((io.qt.core.QMetaObject.AbstractSignal)signal, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Disconnects a signal from another signal if the two were previously connected by a call to connect." << Qt::endl
                  << INDENT << " * A call to this function will assure that the emission of the first signal will not cause the emission of the second." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal." << Qt::endl
                  << INDENT << " * @return <code>true</code> if the two signals were successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject.AbstractPublicSignal" << it.key() << parameters << " signal) {" << Qt::endl
                  << INDENT << "    return super.disconnect((io.qt.core.QMetaObject.AbstractSignal)signal);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
                for(qint64 i = it.key()-1; i>0; --i){
                    if(!argumentCountMap.contains(i) && !writtenConnects.contains(i)){
                        writtenConnects.insert(i);
                        QString parameters;
                        QStringList parameterClasses;
                        QStringList classes;
                        QStringList vars;
                        if(i>0){
                            QStringList params;
                            for(int j=0; j<i; j++){
                                QString letter = QString(QChar('A'+j));
                                params << letter;
                                classes << QString(QLatin1String("Class<%1> type%2")).arg(letter).arg(j+1);
                                vars << QString(QLatin1String("type%1")).arg(j+1);
                                parameterClasses << QString(QLatin1String("(Class<%1>)parameters[%2]")).arg(letter).arg(j);
                            }
                            parameters = QString(QLatin1String("<%1>")).arg(params.join(","));
                        }
                        s << INDENT << "/**" << Qt::endl
                          << INDENT << " * Initializes a connection to the <i>slot</i>." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param slot the slot to be connected" << Qt::endl
                          << INDENT << " * @param connectionType type of connection" << Qt::endl
                          << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                          << INDENT << " * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                          << INDENT << " * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.Connection connect(io.qt.core.QMetaObject.Slot" << i << parameters << " slot, io.qt.core.Qt.ConnectionType... connectionType) throws io.qt.QNoSuchSignalException{" << Qt::endl
                          << INDENT << "    return super.connect(slot, connectionType);" << Qt::endl
                          << INDENT << "}" << Qt::endl << Qt::endl
                          << INDENT << "/**" << Qt::endl
                          << INDENT << " * Removes the connection to the given <i>slot</i>." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param slot the slot to be disconnected" << Qt::endl
                          << INDENT << " * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject.Slot" << i << parameters << " slot) {" << Qt::endl
                          << INDENT << "    return super.disconnect(slot);" << Qt::endl
                          << INDENT << "}" << Qt::endl << Qt::endl
                          << INDENT << "/**" << Qt::endl
                          << INDENT << " * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second" << Qt::endl
                          << INDENT << " * signal to be emitted as well." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param signal The second signal. This will be emitted whenever this signal is emitted." << Qt::endl
                          << INDENT << " * @param connectionType One of the connection types defined in the Qt interface." << Qt::endl
                          << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                          << INDENT << " * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.Connection connect(io.qt.core.QMetaObject.AbstractPublicSignal" << i << parameters << " signal, io.qt.core.Qt.ConnectionType... connectionType) throws io.qt.QNoSuchSignalException{" << Qt::endl
                          << INDENT << "    return super.connect((io.qt.core.QMetaObject.AbstractSignal)signal, connectionType);" << Qt::endl
                          << INDENT << "}" << Qt::endl << Qt::endl
                          << INDENT << "/**" << Qt::endl
                          << INDENT << " * Disconnects a signal from another signal if the two were previously connected by a call to connect." << Qt::endl
                          << INDENT << " * A call to this function will assure that the emission of the first signal will not cause the emission of the second." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param signal The second signal." << Qt::endl
                          << INDENT << " * @return <code>true</code> if the two signals were successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject.AbstractPublicSignal" << i << parameters << " signal) {" << Qt::endl
                          << INDENT << "    return super.disconnect((io.qt.core.QMetaObject.AbstractSignal)signal);" << Qt::endl
                          << INDENT << "}" << Qt::endl << Qt::endl;
                    }
                }
            }
        }

        for(AbstractMetaFunction* java_function : signalList){
            QString comment;
            QTextStream commentStream(&comment);
            if(!java_function->brief().isEmpty()){
                commentStream << "<p>" << QString(java_function->brief())
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "</p>" << Qt::endl;
            }
            if(!java_function->href().isEmpty()){
                QString url = docsUrl+java_function->href();
                commentStream << "<p>See <a href=\"" << url << "\">";
                if(java_function->declaringClass())
                    commentStream << java_function->declaringClass()->qualifiedCppName()
                                     .replace("<JObjectWrapper>", "")
                                     .replace("QtJambi", "Q")
                                     .replace("QVoid", "Q")
                                     .replace("&", "&amp;")
                                     .replace("<", "&lt;")
                                     .replace(">", "&gt;")
                                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                     .replace("@", "&commat;")
                                     .replace("/*", "&sol;*")
                                     .replace("*/", "*&sol;")
                                  << "::";
                commentStream << QString(java_function->originalSignature())
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "</a></p>" << Qt::endl;
            }
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
                writeDeprecatedComment(commentStream, java_function);
            }
            if(!java_function->isPrivateSignal() && !comment.trimmed().isEmpty()){
                s << INDENT << "/**" << Qt::endl;
                commentStream.seek(0);
                while(!commentStream.atEnd()){
                    s << INDENT << " * " << commentStream.readLine() << Qt::endl;
                }
                s << INDENT << " */" << Qt::endl;
            }
            QStringList classes;
            bool hasInstantiations = false;
            for(int i = 0; i < java_function->arguments().size(); i++){
                QString type = translateType(java_function->arguments().at(i)->type(), java_function->implementingClass(), Option(InitializerListAsArray));
                auto idx = type.indexOf('<');
                if(idx>0){
                    type = type.mid(0, idx);
                    if(type=="io.qt.core.QList")
                        type = "java.util.List";
                    else if(type=="io.qt.core.QMap" || type=="io.qt.core.QHash")
                        type = "java.util.Map";
                    else if(type=="io.qt.core.QSet")
                        type = "java.util.Set";
                    hasInstantiations = true;
                }
                classes << type+".class";
            }
            m_exportedPackages.clear();
            QString sig = functionSignature(java_function,
                                            java_function->isPrivateSignal() ? AbstractMetaAttributes::Private : AbstractMetaAttributes::Public,
                                            ( java_function->isPrivateSignal() ? AbstractMetaAttributes::Public : 0 ) | AbstractMetaAttributes::Native,
                                            Option(), -1, "emit");// << "public final void emit("
            bool exp = false;
            for(const QString& pkg : m_exportedPackages){
                if(pkg!=java_function->implementingClass()->package()
                        && !pkg.isEmpty()
                        && !pkg.startsWith("io.qt.internal")
                        && pkg!="io.qt"
                        && m_typeSystemByPackage[pkg]
                        && !m_typeSystemByPackage[pkg]->module().isEmpty()
                        && m_typeSystemByPackage[pkg]->module()!="qtjambi" // because it's transitive
                        ){
                    exp = true;
                    break;
                }
            }
            if(exp){
                if(hasInstantiations){
                    if(java_function->isPrivateSignal())
                        s << INDENT << "@SuppressWarnings({\"exports\", \"unchecked\", \"rawtypes\", \"unused\" })" << Qt::endl;
                    else
                        s << INDENT << "@SuppressWarnings({\"exports\", \"unchecked\", \"rawtypes\" })" << Qt::endl;
                }
            }else if(hasInstantiations){
                if(java_function->isPrivateSignal())
                    s << INDENT << "@SuppressWarnings({ \"unchecked\", \"rawtypes\", \"unused\" })" << Qt::endl;
                else
                    s << INDENT << "@SuppressWarnings({ \"unchecked\", \"rawtypes\" })" << Qt::endl;
            }else if(java_function->isPrivateSignal())
                s << INDENT << "@SuppressWarnings(\"unused\")" << Qt::endl;

            s << sig;
            s << " {" << Qt::endl;
            if (java_function->isPrivateSignal()){
                s << INDENT << "    io.qt.core.QObject.emit((";
                if(hasInstantiations){
                    QString signalType = signalTypes.value(java_function);
                    auto idx = signalType.indexOf('<');
                    if(idx>0){
                        signalType = signalType.mid(0, idx);
                    }
                    s << signalType;
                }else{
                    s << signalTypes.value(java_function);
                }
                s << ")overload(";
                s << classes.join(", ") << "), ";
                for(int i = 0; i < java_function->arguments().size(); i++){
                    if(i!=0)
                        s << ", ";
                    s << java_function->arguments().at(i)->modifiedArgumentName();
                }
                s << ");" << Qt::endl;
            }else{
                s << INDENT << "    ((";
                if(hasInstantiations){
                    QString signalType = signalTypes.value(java_function);
                    auto idx = signalType.indexOf('<');
                    if(idx>0){
                        signalType = signalType.mid(0, idx);
                    }
                    s << signalType;
                }else{
                    s << signalTypes.value(java_function);
                }
                s << ")overload(";
                s << classes.join(", ") << ")).emit(";
                for(int i = 0; i < java_function->arguments().size(); i++){
                    if(i!=0)
                        s << ", ";
                    s << java_function->arguments().at(i)->modifiedArgumentName();
                }
                s << ");" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl << Qt::endl;
            writeFunctionOverloads(s, java_function,
                                   java_function->isPrivateSignal() ? AbstractMetaAttributes::Private : AbstractMetaAttributes::Public,
                                   ( java_function->isPrivateSignal() ? AbstractMetaAttributes::Public : 0 ) | AbstractMetaAttributes::Native,
                                   Option(NoOption), "emit");
        }
    }
    s << INDENT << "};" << Qt::endl << Qt::endl;
    s << Qt::endl
      << INDENT << "/**" << Qt::endl
      << INDENT << " * <p>Overloaded signals:</p>" << Qt::endl
      << INDENT << " * <ul>" << Qt::endl;
    for(AbstractMetaFunction* java_function : signalList){
        s << INDENT << " * <li><code>" << java_function->signature() << "</code></li>" << Qt::endl;
    }
    s << INDENT << " * </ul>" << Qt::endl
      << INDENT << " */" << Qt::endl
      << INDENT << "public final MultiSignal_" << signalName << " " << signalName << " = new MultiSignal_" << signalName << "();" << Qt::endl << Qt::endl;
}

void JavaGenerator::retrieveModifications(const AbstractMetaFunction *java_function,
        const AbstractMetaClass *java_class,
        uint *exclude_attributes,
        uint *include_attributes, Option) const {
    FunctionModificationList mods = java_function->modifications(java_class);
//     printf("name: %s has %d mods\n", qPrintable(java_function->signature()), mods.size());
    for(const FunctionModification& mod : mods) {
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
            *include_attributes |= AbstractMetaAttributes::Final;
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
        int arg_count,
        const QString& alternativeFunctionName) {
    const AbstractMetaArgumentList& arguments = java_function->arguments();
    int argument_count = arg_count < 0 ? int(arguments.size()) : arg_count;

    QString result;
    QTextStream s(&result);
    QString functionName = java_function->name();
    if(!alternativeFunctionName.isEmpty())
        functionName = alternativeFunctionName;
    // The actual function
    if (!(java_function->isEmptyFunction() || java_function->isNormal() || java_function->isSignal()))
        option = Option(option | SkipReturnType);
    writeFunctionAttributes(s, java_function, arg_count, included_attributes, excluded_attributes, option);

    s << functionName << "(";
    writeFunctionArguments(s, java_function, argument_count, option);
    s << ")";
    QString throws = java_function->throws();
    if(!throws.isEmpty()){
        s << " throws " << throws;
    }
    return result;
}

void JavaGenerator::setupForFunction(const AbstractMetaFunction *java_function,
                                     uint *included_attributes,
                                     uint *excluded_attributes, Option option) const {
    *excluded_attributes |= java_function->ownerClass()->isInterface() || java_function->isConstructor()
                            ? AbstractMetaAttributes::Native
                            : 0;
    *excluded_attributes |= (java_function->ownerClass()->isInterface() && (option & InFunctionComment)==0) || java_function->isConstructor()
                            ? AbstractMetaAttributes::Final
                            : 0;
    if (java_function->ownerClass()->isInterface() && (option & InFunctionComment)==0)
        *excluded_attributes |= AbstractMetaAttributes::Abstract;
    if (java_function->needsCallThrough())
        *excluded_attributes |= AbstractMetaAttributes::Native;

    const AbstractMetaClass *java_class = java_function->ownerClass();
    retrieveModifications(java_function, java_class, excluded_attributes, included_attributes, option);
}

void JavaGenerator::writeReferenceCount(QTextStream &s, const ReferenceCount &refCount,
                                        int argumentIndex, const AbstractMetaFunction *java_function, const QString &thisName) {
    if (refCount.action == ReferenceCount::Ignore)
        return;

    const AbstractMetaArgumentList& arguments = java_function->arguments();
    QString argumentName;
    if(argumentIndex==-1){
        argumentName = QLatin1String("this");
    }else if(argumentIndex==0){
        argumentName = QLatin1String("__qt_return_value");
    }else if(argumentIndex-1<arguments.size()){
        argumentName = arguments.at(argumentIndex-1)->modifiedArgumentName();
    }else{
        return;
    }
    bool nullPointersDisabled = argumentIndex>0 ? java_function->nullPointersDisabled(java_function->declaringClass(), argumentIndex) : false;

    QScopedPointer<Indentation> indentation;
    QString condition = refCount.condition;
    condition = condition.replace("%in", argumentName);
    condition = condition.replace("%arg", argumentName);
    condition = condition.replace("%this", "this");
    condition = condition.replace("%0", "__qt_return_value");
    condition = condition.replace("%return", "__qt_return_value");
    for(const AbstractMetaArgument* argument : arguments){
        condition = condition.replace(QString("%")+QString::number(argument->argumentIndex()+1), argument->modifiedArgumentName());
    }
    if (refCount.action != ReferenceCount::Set) {
        if(nullPointersDisabled){
            if (!refCount.condition.isEmpty()){
                s << INDENT << "if (" << condition << ") {" << Qt::endl;
                indentation.reset(new Indentation(INDENT));
            }
        }else{
            s << INDENT << "if (" << argumentName << " != null";
            if (!condition.isEmpty())
                s << " && " << condition;
            s << ") {" << Qt::endl;
            indentation.reset(new Indentation(INDENT));
        }
    } else if (!condition.isEmpty()){
        s << INDENT << "if (" << condition << ") {" << Qt::endl;
        indentation.reset(new Indentation(INDENT));
    }
    bool isStatic = java_function->isStatic();

    QString declareVariable = refCount.declareVariable;
    if (!declareVariable.isEmpty()) {
        QList<TypeEntry *> types = TypeDatabase::instance()->findTypes(declareVariable);
        if(types.size()==1){
            declareVariable = types[0]->qualifiedTargetLangName();
        }
    }else if(java_function->isAbstract()){
        if(java_function->ownerClass()->typeEntry()->lookupName().endsWith("$Impl$ConcreteWrapper")){
            QString lookupName = java_function->ownerClass()->typeEntry()->lookupName();
            lookupName.chop(16);
            declareVariable = java_function->ownerClass()->typeEntry()->javaPackage() + "." + lookupName.replace('$', '.');
        }else if(java_function->ownerClass()->typeEntry()->lookupName().endsWith("$ConcreteWrapper")){
            QString lookupName = java_function->declaringClass()->typeEntry()->lookupName();
            if(lookupName.endsWith("$ConcreteWrapper"))
                lookupName.chop(16);
            declareVariable = java_function->declaringClass()->typeEntry()->javaPackage() + "." + lookupName.replace('$', '.');
        }
    }
    switch (refCount.action) {
        case ReferenceCount::Put:
            {
                QString keyArgumentName = arguments.at(int(refCount.keyArgument) - 1)->modifiedArgumentName();
                if (declareVariable.isEmpty()) {
                    s << INDENT << "if(" << refCount.variableName << "==null)" << Qt::endl;
                    if (refCount.threadSafe)
                        s << INDENT << "    " << refCount.variableName << " = java.util.Collections.synchronizedMap(QtJambi_LibraryUtilities.internal.newRCMap());" << Qt::endl;
                    else
                        s << INDENT << "    " << refCount.variableName << " = QtJambi_LibraryUtilities.internal.newRCMap();" << Qt::endl;
                    s << INDENT << refCount.variableName << ".put(" << keyArgumentName << ", " << argumentName << ");" << Qt::endl;
                }else{
                    s << INDENT << "QtJambi_LibraryUtilities.internal.putReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << keyArgumentName << ", " << argumentName << ");" << Qt::endl;
                }
            }
            break;
        case ReferenceCount::ClearAdd:
            if (declareVariable.isEmpty()) {
                s << INDENT << "if(" << refCount.variableName << "!=null)" << Qt::endl;
                s << INDENT << "    " << refCount.variableName << ".clear();" << Qt::endl;
            }else{
                s << INDENT << "QtJambi_LibraryUtilities.internal.clearReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ");" << Qt::endl;
            }
            Q_FALLTHROUGH();
        case ReferenceCount::Add:
            if (declareVariable.isEmpty()) {
                s << INDENT << "if(" << refCount.variableName << "==null)" << Qt::endl;
                if (refCount.threadSafe)
                    s << INDENT << "    " << refCount.variableName << " = java.util.Collections.synchronizedList(new QtJambi_LibraryUtilities.internal.RCList());" << Qt::endl;
                else
                    s << INDENT << "    " << refCount.variableName << " = QtJambi_LibraryUtilities.internal.newRCList();" << Qt::endl;
                s << INDENT << refCount.variableName << ".add(" << argumentName << ");" << Qt::endl;
            }else{
                s << INDENT << "QtJambi_LibraryUtilities.internal.addReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
            }
            break;
        case ReferenceCount::ClearAddAll:
            if (declareVariable.isEmpty()) {
                s << INDENT << "if(" << refCount.variableName << "!=null)" << Qt::endl;
                s << INDENT << "    " << refCount.variableName << ".clear();" << Qt::endl;
            }else{
                s << INDENT << "QtJambi_LibraryUtilities.internal.clearReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ");" << Qt::endl;
            }
            Q_FALLTHROUGH();
        case ReferenceCount::AddAll:
            if (declareVariable.isEmpty()) {
                s << INDENT << "if(" << refCount.variableName << "==null)" << Qt::endl;
                if (refCount.threadSafe)
                    s << INDENT << "    " << refCount.variableName << " = java.util.Collections.synchronizedList(QtJambi_LibraryUtilities.internal.newRCList());" << Qt::endl;
                else
                    s << INDENT << "    " << refCount.variableName << " = QtJambi_LibraryUtilities.internal.newRCList();" << Qt::endl;
                s << INDENT << refCount.variableName << ".addAll(" << argumentName << ");" << Qt::endl;
            }else{
                s << INDENT << "QtJambi_LibraryUtilities.internal.addAllReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
            }
            break;
        case ReferenceCount::Remove:
            if (declareVariable.isEmpty()) {
                s << INDENT << "while (" << refCount.variableName << " != null && " << refCount.variableName << ".remove(" << argumentName << ")) ;" << Qt::endl;
            }else{
                s << INDENT << "QtJambi_LibraryUtilities.internal.removeFromCollectionReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
            }
            break;
        case ReferenceCount::Set: {
            if (declareVariable.isEmpty())
                s << INDENT << refCount.variableName << " = " << argumentName << ";" << Qt::endl;
            else{
                s << INDENT << "QtJambi_LibraryUtilities.internal.setReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
            }
        }
            break;
        default:
            break;
    }
    if(!indentation.isNull()){
        indentation.reset();
        s << INDENT << "}" << Qt::endl;
    }
    if (refCount.action == ReferenceCount::Put) {
        s << INDENT << "else{" << Qt::endl;
        {
            INDENTATION(INDENT)
            QString keyArgumentName = arguments.at(int(refCount.keyArgument) - 1)->modifiedArgumentName();
            if (declareVariable.isEmpty()) {
                s << INDENT << "if(" << refCount.variableName << "!=null)" << Qt::endl;
                s << INDENT << "    " << refCount.variableName << ".remove(" << keyArgumentName << ");" << Qt::endl;
            }else{
                s << INDENT << "removeFromMapReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ", " << keyArgumentName << ");" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
    }
}

void JavaGenerator::writeDeprecatedComment(QTextStream& commentStream, const AbstractMetaFunction *java_function){
    const AbstractMetaFunction *foundFun = nullptr;
    const AbstractMetaClass *declaringClass = nullptr;
    bool hasNull = false;
    QString method;
    if(java_function->deprecatedComment().toLower().startsWith("use ")){
        QString deprecatedComment = java_function->deprecatedComment();
        deprecatedComment = deprecatedComment.mid(4);
        if(deprecatedComment.startsWith(java_function->declaringClass()->qualifiedCppName() + "::")){
            method = deprecatedComment.mid(java_function->declaringClass()->qualifiedCppName().length()+2);
            declaringClass = java_function->declaringClass();
        }else{
            auto idx = deprecatedComment.indexOf("::");
            if(idx>0){
                QString className = deprecatedComment.left(idx);
                method = deprecatedComment.mid(idx+2);
                declaringClass = m_classes.findClass(className);
            }else{
                declaringClass = java_function->declaringClass();
                method = deprecatedComment;
            }
        }
        if(method.endsWith(" instead")){
            method.chop(8);
        }
        if(method.contains("nullptr")){
            hasNull = true;
            method.replace("nullptr", "");
        }
    }
    if(declaringClass){
        QByteArray signature = QMetaObject::normalizedSignature(qPrintable(method));
        for(const AbstractMetaFunction * fun : declaringClass->functions()){
            if(fun->name()==method
                    || fun->signature()==method
                    || fun->signature(true)==method
                    || fun->originalSignature()==method
                    || QMetaObject::normalizedSignature(qPrintable(fun->signature(true)))==signature){
                foundFun = fun;
                break;
            }
        }
        if(!foundFun){
            if(method.endsWith("()")){
                method.chop(2);
                for(const AbstractMetaFunction * fun : declaringClass->functions()){
                    if(fun->name()==method){
                        foundFun = fun;
                        break;
                    }
                }
            }
        }
    }
    if(foundFun && !foundFun->isSignal()){
        commentStream << "@deprecated Use {@link ";
        commentStream << foundFun->declaringClass()->typeEntry()->qualifiedTargetLangName().replace("$", ".");
        commentStream << "#" << foundFun->name() << "(";
        writeFunctionArguments(commentStream, foundFun, int(foundFun->arguments().size()), Option(SkipName | SkipTemplateParameters | NoSuppressExports));
        commentStream << ")}";
        if(hasNull)
            commentStream << " with <code>null</code>";
        commentStream << " instead";
    }else{
        commentStream << "@deprecated " << QString(java_function->deprecatedComment())
                         .replace("&", "&amp;")
                         .replace("<", "&lt;")
                         .replace(">", "&gt;")
                         .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                         .replace("@", "&commat;")
                         .replace("/*", "&sol;*")
                         .replace("*/", "*&sol;");
    }
}

void JavaGenerator::writeFunction(QTextStream &s, const AbstractMetaFunction *java_function,
                                  uint included_attributes, uint excluded_attributes, Option option) {
    if (java_function->isModifiedRemoved(TypeSystem::TargetLangCode))
        return ;
    if (java_function->hasTemplateArgumentTypes())
        return;
    setupForFunction(java_function, &included_attributes, &excluded_attributes, option);

    if (!java_function->ownerClass()->isInterface() || target_JDK_version>=8) {
        writeEnumOverload(s, java_function, included_attributes, excluded_attributes, option);
        if(!java_function->isSignal()){
            writeFunctionOverloads(s, java_function, included_attributes, excluded_attributes, option);
        }
    }

    static QRegularExpression regExp("^(insert|set|take|add|remove|install|uninstall).*");
    if (regExp.match(java_function->name()).hasMatch()) {
        const AbstractMetaArgumentList& arguments = java_function->arguments();

        const AbstractMetaClass *c = java_function->implementingClass();
        bool hasObjectTypeArgument = false;
        for(AbstractMetaArgument *argument : arguments) {
            TypeSystem::Ownership cpp_ownership = java_function->ownership(c, TypeSystem::NativeCode, argument->argumentIndex() + 1).ownership;
            TypeSystem::Ownership java_ownership = java_function->ownership(c, TypeSystem::TargetLangCode, argument->argumentIndex() + 1).ownership;
            TypeSystem::Ownership shell_ownership = java_function->ownership(c, TypeSystem::ShellCode, argument->argumentIndex() + 1).ownership;

            if (argument->type()->typeEntry()->isObject()
                    && argument->type()->indirections().size()==1
                    && argument->type()->getReferenceType()==AbstractMetaType::NoReference
                    && cpp_ownership == TypeSystem::InvalidOwnership
                    && java_ownership == TypeSystem::InvalidOwnership
                    && shell_ownership == TypeSystem::InvalidOwnership) {
                hasObjectTypeArgument = true;
                break;
            }
        }

        if (hasObjectTypeArgument) {
            if(java_function->referenceCounts(java_function->implementingClass()).size() == 0){
                if(java_function->isInterfaceFunction()){
                    if(java_function->ownerClass()->typeEntry()->designatedInterface())
                        m_reference_count_candidate_functions.append(java_function);
                }else{
                    m_reference_count_candidate_functions.append(java_function);
                }
            }
        }
    }

    static QRegularExpression regExp2("^(create|clone|take).*");
    if (java_function->type()
            && !java_function->implementingClass()->typeEntry()->designatedInterface()
            && java_function->type()->typeEntry()->isObject()
            && regExp2.match(java_function->name()).hasMatch()) {
        const AbstractMetaClass *c = java_function->implementingClass();
        TypeSystem::Ownership cpp_ownership = java_function->ownership(c, TypeSystem::NativeCode, 0).ownership;
        TypeSystem::Ownership java_ownership = java_function->ownership(c, TypeSystem::TargetLangCode, 0).ownership;
        TypeSystem::Ownership shell_ownership = java_function->ownership(c, TypeSystem::ShellCode, 0).ownership;

        if ( !java_function->nullPointersDisabled()
                && cpp_ownership == TypeSystem::InvalidOwnership
                    && java_ownership == TypeSystem::InvalidOwnership
                    && shell_ownership == TypeSystem::InvalidOwnership ) {
            m_factory_functions.append(java_function);
        }
    }


    QString comment;
    QTextStream commentStream(&comment);
    if (m_doc_parser) {
        QString signature = functionSignature(java_function,
                                              included_attributes | NoBlockedSlot,
                                              excluded_attributes, NoSuppressExports);
        commentStream << m_doc_parser->documentationForFunction(signature) << Qt::endl;
    }else{
        if(!java_function->brief().isEmpty()){
            commentStream << "<p>" << QString(java_function->brief())
                             .replace("&", "&amp;")
                             .replace("<", "&lt;")
                             .replace(">", "&gt;")
                             .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                             .replace("@", "&commat;")
                             .replace("/*", "&sol;*")
                             .replace("*/", "*&sol;")
                          << "</p>" << Qt::endl;
        }
        if(!java_function->href().isEmpty()){
            QString url = docsUrl+java_function->href();
            commentStream << "<p>See <a href=\"" << url << "\">";
            if(java_function->declaringClass() && java_function->functionType()!=AbstractMetaFunction::GlobalScopeFunction){
                commentStream << java_function->declaringClass()->qualifiedCppName()
                                 .replace("<JObjectWrapper>", "")
                                 .replace("QtJambi", "Q")
                                 .replace("QVoid", "Q")
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "::";
            }
            if(!java_function->originalSignature().isEmpty()){
                commentStream << QString(java_function->originalSignature())
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "</a></p>" << Qt::endl;
            }else if(!java_function->minimalSignature().isEmpty()){
                commentStream << QString(java_function->minimalSignature())
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "</a></p>" << Qt::endl;
            }else{
                commentStream << QString(java_function->name())
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << "(...)</a></p>" << Qt::endl;
            }
        }
    }
    if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
        if(!comment.isEmpty())
            commentStream << Qt::endl;
        writeDeprecatedComment(commentStream, java_function);
    }
    if(!comment.trimmed().isEmpty() && (option & InFunctionComment)==0){
        s << INDENT << "/**" << Qt::endl;
        commentStream.seek(0);
        while(!commentStream.atEnd()){
            s << INDENT << " * " << commentStream.readLine() << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
    }

    const QPropertySpec *spec = java_function->propertySpec();
    if (spec && java_function->modifiedName() == java_function->originalName()) {
        if (java_function->isPropertyReader()) {
            s << INDENT << "@io.qt.QtPropertyReader(name=\"" << spec->name() << "\")" << Qt::endl;
            if (!spec->designable().isEmpty()){
                s << INDENT << "@io.qt.QtPropertyDesignable";
                if(spec->designable()!="true")
                    s << "(\"" << spec->designable() << "\")";
                s << Qt::endl;
            }
            if (!spec->scriptable().isEmpty()){
                s << INDENT << "@io.qt.QtPropertyScriptable";
                if(spec->scriptable()!="true")
                    s << "(\"" << spec->scriptable() << "\")";
                s << Qt::endl;
            }
            if (!spec->stored().isEmpty()){
                s << INDENT << "@io.qt.QtPropertyStored";
                if(spec->stored()!="true")
                    s << "(\"" << spec->stored() << "\")";
                s << Qt::endl;
            }
//   This seams to be unnecessary in QtJambi
//            if (!spec->revision().isEmpty())
//                s << INDENT << "@io.qt.QtPropertyRevision(\"" << spec->revision() << "\")" << Qt::endl;
            if (!spec->user().isEmpty()){
                s << INDENT << "@io.qt.QtPropertyUser";
                if(spec->user()!="true")
                    s << "(\"" << spec->user() << "\")";
                s << Qt::endl;
            }
            if (spec->required())
                s << INDENT << "@io.qt.QtPropertyRequired" << Qt::endl;
            if (spec->constant())
                s << INDENT << "@io.qt.QtPropertyConstant" << Qt::endl;
        } else if (java_function->isPropertyWriter()) {
            s << INDENT << "@io.qt.QtPropertyWriter(name=\"" << spec->name() << "\")" << Qt::endl;
        } else if (java_function->isPropertyResetter()) {
            s << INDENT << "@io.qt.QtPropertyResetter(name=\"" << spec->name() << "\")" << Qt::endl;
        } else if (java_function->isPropertyBindable()) {
            s << INDENT << "@io.qt.QtPropertyBindable(name=\"" << spec->name() << "\")" << Qt::endl;
        }
    }

    QString interfaceDefaultImpl;
    {
        Option option = NoOption;
        if(java_function->ownerClass()->isInterface()
                && !java_function->isProtected()
                && (option & InFunctionComment)==0){
            if(!java_function->isAbstract()){
                option = DefaultFunction;
            }else{
                for(const FunctionModification& mod : java_function->modifications(java_function->ownerClass())) {
                    if (mod.snips.count() > 0){
                        for(const CodeSnip& snip : mod.snips) {
                            if (snip.language == TypeSystem::Interface){
                                QString code = snip.code();
                                ArgumentMap map = snip.argumentMap;
                                ArgumentMap::iterator it = map.begin();
                                const AbstractMetaArgumentList& arguments = java_function->arguments();
                                for (;it != map.end();++it) {
                                    int pos = it.key() - 1;
                                    QString meta_name = it.value();
                                    if(pos == -1){
                                        code = code.replace(meta_name, "__qt_return_value");
                                    }else if (pos >= 0 && pos < arguments.count()) {
                                        code = code.replace(meta_name, arguments.at(pos)->modifiedArgumentName());
                                    } else {
                                        QString debug = QString("argument map specifies invalid argument index %1"
                                                                "for function '%2'")
                                                        .arg(pos + 1).arg(java_function->name());
                                        ReportHandler::warning(debug);
                                    }

                                }
                                code = code.replace("%this", "this");
                                QStringList lines = code.split("\n");
                                QTextStream s(&interfaceDefaultImpl);
                                {
                                    INDENTATION(INDENT)
                                    printExtraCode(lines, s);
                                }
                            }
                        }
                    }
                }
                if(!interfaceDefaultImpl.isEmpty()){
                    option = DefaultFunction;
                }
            }
        }
        uint attr = (java_function->attributes() & (~excluded_attributes)) | included_attributes;
        if(java_function->name().startsWith("operator_")){
            if (!(attr & AbstractMetaAttributes::Static && java_function->ownerClass()->typeEntry()->designatedInterface())
                    && !(attr & AbstractMetaAttributes::Private))
                ReportHandler::warning(QString("Non-private operator function %1 in %2").arg(java_function->originalSignature()).arg(java_function->ownerClass()->qualifiedCppName()));
        }
        s << functionSignature(java_function, included_attributes, excluded_attributes, option);
    }

    if(!interfaceDefaultImpl.isEmpty()){
        s << "{" << Qt::endl
          << interfaceDefaultImpl
          << INDENT << "}" << Qt::endl
          << INDENT << Qt::endl;
    }else if(java_function->ownerClass()->isInterface() && !java_function->isAbstract()){
        s << "{" << Qt::endl;
        const AbstractMetaArgumentList& arguments = java_function->arguments();
        if(!java_function->isStatic() && java_function->isPublic()){
            INDENTATION(INDENT)
            bool has_argument_referenceCounts = false;
            QMap<int,QList<ReferenceCount>> referenceCounts;
            for (int i = -1; i <= arguments.size(); ++i) {
                referenceCounts[i] = java_function->referenceCounts(java_function->implementingClass(), i);
                if (referenceCounts[i].size() > 0) {
                    for(const ReferenceCount& refCount : referenceCounts[i]) {
                        // We just want to know this to secure return value into local variable
                        // to hold over ReferenceCount management later on.
                        if (refCount.action != ReferenceCount::Ignore) {
                            // Something active have been specified
                            has_argument_referenceCounts = true;
                            break;
                        }
                    }
                }
            }

            writeInjectedCode(s, java_function, CodeSnip::Beginning);
            writeInjectedCode(s, java_function, CodeSnip::Position1);
            writeInjectedCode(s, java_function, CodeSnip::Position2);

            // Lookup if there is a reference-count action required on the return value.
            AbstractMetaType *return_type = java_function->type();
            QString new_return_type = QString(java_function->typeReplaced(0)).replace('$', '.');
            bool has_return_type = new_return_type != "void"
            && (!new_return_type.isEmpty() || return_type);
            OwnershipRule ownershipRule = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, 0);

            bool has_code_injections_at_the_end = hasCodeInjections(java_function, {CodeSnip::End, CodeSnip::Position4, CodeSnip::Position3});

            bool needs_return_variable = has_return_type
                                         && ( ( ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership)
                                              || has_argument_referenceCounts || referenceCounts[0].size() > 0 || has_code_injections_at_the_end);

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

                if (return_type && new_return_type.isEmpty()) {
                    if (return_type->isTargetLangEnum()) {
                        s << static_cast<const EnumTypeEntry *>(return_type->typeEntry())->qualifiedTargetLangName() << ".resolve(";
                    } else if (return_type->isTargetLangFlags()) {
                        s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace('$', '.') << "(";
                    }
                }
            }

            s << "Impl." << java_function->marshalledName() << "(";
            bool needsComma = false;
            if(!java_function->isStatic()){
                if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                    s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                }else
                    s << "this";
                needsComma = true;
            }

            for (int i = 0; i < arguments.count(); ++i) {
                const AbstractMetaArgument *arg = arguments.at(i);
                const AbstractMetaType *type = arg->type();

                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    if(needsComma)
                        s << ", ";

                    if(!java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()){
                        s << arg->modifiedArgumentName();
                    }else if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                        s << arg->modifiedArgumentName() << ".value()";
                    } else if (type->hasNativeId()) {
                        s << "QtJambi_LibraryUtilities.internal.checkedNativeId(" << arg->modifiedArgumentName() << ")";
                    } else {
                        s << arg->modifiedArgumentName();
                    }
                    needsComma = true;
                }
            }
            s << ")";

            // This closed the ".resolve(" or the "new MyType(" fragments
            if (return_type && new_return_type.isEmpty() && (return_type->isTargetLangEnum() || return_type->isTargetLangFlags()))
                s << ")";

            s << ";" << Qt::endl;

            for(ReferenceCount refCount : referenceCounts[-1]){
                refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace('$', '.');
                writeReferenceCount(s, refCount, -1, java_function, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
            }

            // We must ensure we retain a Java hard-reference over the native method call
            // so that the GC will not destroy the C++ object too early.  At this point we
            // have called the native method call so can manage referenceCount issues.
            // First the input arguments
            for (const AbstractMetaArgument* argument : arguments) {
                for(ReferenceCount refCount : referenceCounts[argument->argumentIndex()+1]){
                    refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace('$', '.');
                    writeReferenceCount(s, refCount, argument->argumentIndex()+1, java_function, QLatin1String("this"));
                }
            }

            if (!java_function->argumentReplaced(0).isEmpty()) {
                s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
            }else{
                // Then the return value
                for(ReferenceCount referenceCount : referenceCounts[0]) {
                    referenceCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace('$', '.');
                    writeReferenceCount(s, referenceCount, 0, java_function, QLatin1String("this"));
                }

                writeInjectedCode(s, java_function, CodeSnip::Position3);
                writeInjectedCode(s, java_function, CodeSnip::Position4);
                writeInjectedCode(s, java_function, CodeSnip::End);

                if (needs_return_variable) {
                    if (ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership) {
                        if (return_type->isContainer()){
                            s << INDENT << "if (__qt_return_value != null";
                            if(!ownershipRule.condition.isEmpty()){
                                s << " && " << ownershipRule.condition;
                            }
                            s << ") {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, "__qt_return_value");
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else if(!ownershipRule.condition.isEmpty()){
                            s << INDENT << "if (" << ownershipRule.condition << ") {" << Qt::endl;
                            {
                                INDENTATION(INDENT)
                                writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, "__qt_return_value");
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else{
                            writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, "__qt_return_value");
                        }
                    }
                    s << INDENT << "return __qt_return_value;" << Qt::endl;
                }
            }
        }else{
            INDENTATION(INDENT)
            s << INDENT;
            if(java_function->type())
                s << "return ";
            if(java_function->isStatic() || java_function->isPublic()){
                s << "Impl";
            }else{
                s << java_function->declaringClass()->simpleName();
                s << ".MemberAccess.of";
                if(!java_function->isProtected())
                    s << "Instance";
                s << "(this)";
            }
            bool hasArg = false;
            if(java_function->isPublic() && !java_function->isStatic()){
                s << "." << java_function->marshalledName() << "(";
                s << "this";
                hasArg = true;
            }else {
                s << "." << java_function->name() << "(";
            }
            for (int i = 0; i < arguments.size(); ++i) {
                AbstractMetaArgument *arg = arguments.at(i);
                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    if(hasArg)
                        s << ", ";
                    s << arguments.at(i)->modifiedArgumentName();
                    hasArg = true;
                }
            }
            s << ");" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl
          << INDENT << Qt::endl;
    }else if (java_function->isConstructor()) {
        writeConstructorContents(s, java_function);
    } else if (java_function->needsCallThrough()) {
        if (java_function->isAbstract()) {
            s << ";" << Qt::endl
              << INDENT << Qt::endl;
        } else {
            if (java_function->isEmptyFunction()) {
                s << " throws io.qt.QNoImplementationException ";
            }
            s << "{" << Qt::endl;
            {
                INDENTATION(INDENT)
                writeJavaCallThroughContents(s, java_function);
            }
            s << INDENT << "}" << Qt::endl
              << INDENT << Qt::endl;
        }

        if((java_function->isAbstract() || !(java_function->originalAttributes() & AbstractMetaAttributes::Public)) && !java_function->implementingClass()->generateShellClass()){
            // do nothing
        }else if(java_function->isAbstract() && java_function->implementingClass()->hasUnimplmentablePureVirtualFunction()){
            // do nothing
        }else if(java_function->jumpTableId() == -1){
            writePrivateNativeFunction(s, java_function);
        }
    } else {
        s << ";" << Qt::endl
          << INDENT << Qt::endl;
    }
}

void JavaGenerator::write_equals_parts(QTextStream &s, const AbstractMetaFunctionList &lst, char prefix, bool& first, bool& suppressUnchecked, bool& suppressRaw) {
    for(AbstractMetaFunction *f : lst) {
        AbstractMetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            if(arg->type()->typeEntry()->designatedInterface())
                type = arg->type()->typeEntry()->designatedInterface()->qualifiedTargetLangName();
            else
                type = arg->type()->typeEntry()->qualifiedTargetLangName();
            registerPackage(type);
            type = type.replace('$', '.');
            if(arg->type()->typeEntry()->isContainer()){
                if(type=="java.util.List"
                        ||  type=="java.util.LinkedList"
                        ||  type=="java.util.Queue"
                        ||  type=="java.util.Deque"
                        ||  type=="java.util.ArrayList"
                        ||  type=="java.util.Vector"
                        ||  type=="java.util.Set"
                        ||  type=="io.qt.core.QSet"
                        ||  type=="io.qt.core.QList"
                        ||  type=="io.qt.core.QQueue"
                        ||  type=="io.qt.core.QVector"
                        ||  type=="io.qt.core.QStack"
                        ||  type=="io.qt.core.QLinkedList"){
                    type = "java.util.Collection";
                }else if(type=="java.util.Map"
                         ||  type=="java.util.SortedMap"
                         ||  type=="java.util.NavigableMap"
                         ||  type=="java.util.HashMap"
                         ||  type=="java.util.TreeMap"
                         ||  type=="io.qt.core.QMap"
                         ||  type=="io.qt.core.QHash"
                         ||  type=="io.qt.core.QMultiMap"
                         ||  type=="io.qt.core.QMultiHash"){
                    type = "java.util.Map";
                }
            }
            const QList<const AbstractMetaType *>& instantiations = arg->type()->instantiations();
            if(instantiations.size()>0){
                suppressRaw = true;
                suppressUnchecked = true;
            }
        }
        auto idx = type.indexOf("<");
        if(idx>0){
            suppressRaw = true;
            type = type.left(idx);
        }else{
            if(arg->type()->typeEntry()->isComplex()){
                const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(arg->type()->typeEntry());
                if(ctype->isGenericClass() && arg->type()->instantiations().isEmpty()){
                    suppressRaw = true;
                }
            }
        }
        QString boxedType = type;
        if(boxedType=="io.qt.core.QString")
            boxedType = type = "java.lang.CharSequence";
        else if(boxedType=="int")
            boxedType = "java.lang.Integer";
        else if(boxedType=="byte")
            boxedType = "java.lang.Byte";
        else if(boxedType=="short")
            boxedType = "java.lang.Short";
        else if(boxedType=="long")
            boxedType = "java.lang.Long";
        else if(boxedType=="double")
            boxedType = "java.lang.Double";
        else if(boxedType=="float")
            boxedType = "java.lang.Float";
        else if(boxedType=="boolean")
            boxedType = "java.lang.Boolean";
        else if(boxedType=="char")
            boxedType = "java.lang.Character";

        if(type=="null"){
            s << INDENT << (first ? "if" : "else if") << " (other==" << type;
            s << ") {" << Qt::endl;
            s << INDENT << "    return ";
            if (prefix != 0) s << prefix;
            s << f->name() << "();" << Qt::endl;
        }else{
            s << INDENT << (first ? "if" : "else if") << " (other instanceof " << boxedType;
            if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size()){
                s << " && checkContainerType((" << type << ") other";
                for(const AbstractMetaType * instantiation : arg->type()->instantiations()){
                    s << ", " << instantiation->typeEntry()->qualifiedTargetLangName() << ".class";
                }
                s << ")";
            }
            s << ") {" << Qt::endl;
            s << INDENT << "    return ";
            if (prefix != 0) s << prefix;
            s << f->name() << "((" << type << ") other);" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl;
        first = false;
    }
}

static void write_compareto_parts(QTextStream &s, const AbstractMetaFunctionList &lst, int value, bool *first, bool& suppressUnchecked, bool& suppressRaw) {
    bool suppressNull = false;
    for(AbstractMetaFunction *f : lst) {
        AbstractMetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            type = arg->type()->typeEntry()->qualifiedTargetLangName();
            if(arg->type()->typeEntry()->isContainer()){
                if(type=="java.util.List"
                       ||  type=="java.util.LinkedList"
                       ||  type=="java.util.Queue"
                       ||  type=="java.util.Deque"
                       ||  type=="java.util.ArrayList"
                       ||  type=="java.util.Vector"
                       ||  type=="java.util.Set"
                       ||  type=="io.qt.core.QSet"
                       ||  type=="io.qt.core.QList"
                       ||  type=="io.qt.core.QQueue"
                       ||  type=="io.qt.core.QVector"
                       ||  type=="io.qt.core.QStack"
                       ||  type=="io.qt.core.QLinkedList") {
                    type = "java.util.Collection";
                }else if(type=="java.util.Map"
                       ||  type=="java.util.SortedMap"
                       ||  type=="java.util.NavigableMap"
                       ||  type=="java.util.HashMap"
                       ||  type=="java.util.TreeMap"
                       ||  type=="io.qt.core.QMap"
                       ||  type=="io.qt.core.QHash"
                       ||  type=="io.qt.core.QMultiMap"
                       ||  type=="io.qt.core.QMultiHash"){
                    type = "java.util.Map";
                }
            }
            const QList<const AbstractMetaType *>& instantiations = arg->type()->instantiations();
            if(instantiations.size()>0){
                suppressRaw = true;
                suppressUnchecked = true;
            }
        }
        type = type.replace('$', '.');
        auto idx = type.indexOf("<");
        if(idx>0){
            suppressRaw = true;
            type = type.left(idx);
        }else{
            if(arg->type()->typeEntry()->isComplex()){
                const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(arg->type()->typeEntry());
                if(ctype->isGenericClass() && arg->type()->instantiations().isEmpty()){
                    suppressRaw = true;
                }
            }
        }
        QString boxedType = type;
        if(boxedType=="io.qt.core.QString")
            boxedType = type = "java.lang.CharSequence";
        else if(boxedType=="int")
            boxedType = "java.lang.Integer";
        else if(boxedType=="byte")
            boxedType = "java.lang.Byte";
        else if(boxedType=="short")
            boxedType = "java.lang.Short";
        else if(boxedType=="long")
            boxedType = "java.lang.Long";
        else if(boxedType=="double")
            boxedType = "java.lang.Double";
        else if(boxedType=="float")
            boxedType = "java.lang.Float";
        else if(boxedType=="boolean")
            boxedType = "java.lang.Boolean";
        else if(boxedType=="char")
            boxedType = "java.lang.Character";
        if(value==0){
            s << INDENT << (*first ? "if" : "else if") << " (other instanceof " << boxedType;
            if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size()){
                s << " && checkContainerType((" << type << ") other";
                for(const AbstractMetaType * instantiation : arg->type()->instantiations()){
                    s << ", " << instantiation->typeEntry()->qualifiedTargetLangName() << ".class";
                }
                s << ")";
            }
            s << ") {" << Qt::endl;
            s << INDENT << "    return " << f->name() << "((" << type << ") other);" << Qt::endl
              << INDENT << "}" << Qt::endl;
        }else{
            if(type=="null"){
                suppressNull = true;
                s << INDENT << (first ? "if" : "else if") << " (other==" << type;
                s << ") {" << Qt::endl;
                s << INDENT << "    if (" << f->name() << "()) return " << value << ";" << Qt::endl
                  << INDENT << "    else return " << -value << ";" << Qt::endl
                  << INDENT << "}" << Qt::endl;
            }else{
                s << INDENT << (*first ? "if" : "else if") << " (other instanceof " << boxedType;
                if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size()){
                    s << " && checkContainerType((" << type << ") other";
                    for(const AbstractMetaType * instantiation : arg->type()->instantiations()){
                        s << ", " << instantiation->typeEntry()->qualifiedTargetLangName() << ".class";
                    }
                    s << ")";
                }
                s << ") {" << Qt::endl;
                s << INDENT << "    if (" << f->name() << "((" << type << ") other)) return " << value << ";" << Qt::endl
                  << INDENT << "    else return " << -value << ";" << Qt::endl
                  << INDENT << "}" << Qt::endl;
            }
        }
        *first = false;
    }
    if(!suppressNull){
        s << INDENT << (*first ? "if" : "else if") << "(other==null)" << Qt::endl;
        if(lst.isEmpty()){
            s << INDENT << "    throw new NullPointerException();" << Qt::endl;
        }else{
            AbstractMetaFunction *f = lst.at(0);
            s << INDENT << "    if (" << f->name() << "((" << f->declaringClass()->typeEntry()->qualifiedTargetLangName() << ") other)) return " << value << ";" << Qt::endl
              << INDENT << "    else return " << -value << ";" << Qt::endl;
        }
    }
    s << INDENT << "else throw new ClassCastException();" << Qt::endl;
}

const AbstractMetaType * JavaGenerator::getIterableType(const AbstractMetaClass *cls) const{
    const AbstractMetaFunctionList& begin_functions = cls->beginFunctions();
    const AbstractMetaFunctionList& end_functions = cls->endFunctions();
    if(begin_functions.isEmpty() || end_functions.isEmpty()){
        return nullptr;
    }
    if(begin_functions.first()->type()->typeEntry()->qualifiedCppName()==end_functions.first()->type()->typeEntry()->qualifiedCppName()){
        if (begin_functions.first()->type()->isIterator()) {
            if(!begin_functions.first()->type()->iteratorInstantiations().isEmpty()){
                return begin_functions.first()->type()->iteratorInstantiations().first();
            }
            const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(begin_functions.first()->type()->typeEntry());
            if(AbstractMetaClass * iteratorClass = cls->findIterator(iteratorType)){
                if(iteratorClass->templateBaseClassInstantiations().size()==1){
                    return iteratorClass->templateBaseClassInstantiations().at(0);
                }
                for(AbstractMetaFunction* function : iteratorClass->functions()){
                    if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                        return function->type();
                    }
                }
            }else if(AbstractMetaClass * iteratorClass = m_classes.findClass(iteratorType->qualifiedCppName(), AbstractMetaClassList::QualifiedCppName)){
                if(iteratorClass->templateBaseClassInstantiations().size()==1){
                    return iteratorClass->templateBaseClassInstantiations().at(0);
                }
                for(AbstractMetaFunction* function : iteratorClass->functions()){
                    if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                        return function->type();
                    }
                }
            }
        }
    }
    return nullptr;
}

bool JavaGenerator::isComparable(const AbstractMetaClass *cls) const {
    const AbstractMetaFunctionList& eq_functions = cls->equalsFunctions();
    const AbstractMetaFunctionList& neq_functions = cls->notEqualsFunctions();

    // Write the comparable functions
    const AbstractMetaFunctionList& gt_functions = cls->greaterThanFunctions();
    const AbstractMetaFunctionList& geq_functions = cls->greaterThanEqFunctions();
    const AbstractMetaFunctionList& lt_functions = cls->lessThanFunctions();
    const AbstractMetaFunctionList& leq_functions = cls->lessThanEqFunctions();
    const AbstractMetaFunctionList& compare_functions = cls->compareFunctions();

    bool hasEquals = eq_functions.size() || neq_functions.size();
    bool isComparable = compare_functions.size() || (hasEquals
                                                        ? gt_functions.size() || geq_functions.size() || lt_functions.size() || leq_functions.size()
                                                        : gt_functions.size() == 1 || lt_functions.size() == 1);

    return isComparable;
}

void JavaGenerator::writeJavaLangObjectOverrideFunctions(QTextStream &s,
        const AbstractMetaClass *cls) {
    const AbstractMetaFunctionList& eq_functions = cls->equalsFunctions();
    const AbstractMetaFunctionList& neq_functions = cls->notEqualsFunctions();

    if (eq_functions.size() || neq_functions.size()) {
        bool suppressRaw = false;
        bool suppressUnchecked = false;
        QString eq;
        {
            INDENTATION(INDENT)
            QTextStream _s(&eq);
            bool first = true;
            write_equals_parts(_s, eq_functions,  char(0), first, suppressUnchecked, suppressRaw);
            write_equals_parts(_s, neq_functions, '!', first, suppressUnchecked, suppressRaw);
        }
        s << Qt::endl;
        s << INDENT << "@Override" << Qt::endl;
        s << INDENT << "@io.qt.QtUninvokable" << Qt::endl;
        if(suppressRaw && suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"rawtypes\", \"unchecked\"})" << Qt::endl;
        else if(suppressRaw)
            s << INDENT << "@SuppressWarnings({\"rawtypes\"})" << Qt::endl;
        else if(suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"unchecked\"})" << Qt::endl;
        s << INDENT << "public boolean equals(Object other) {" << Qt::endl;
        {
            INDENTATION(INDENT)
            QStringList lines;
            for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                if(codeSnip.language==TypeSystem::TargetLangCode && codeSnip.position==CodeSnip::Equals){
                    lines << codeSnip.code().split("\n");
                }
            }
            if(!lines.isEmpty())
                printExtraCode(lines, s);
            s << eq << Qt::endl;
            s << INDENT << "return false;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
    }else if(cls->typeEntry()->isValue()){
        AbstractMetaClass* baseClass = cls->baseClass();
        while(baseClass){
            if(baseClass->typeEntry()->isValue() || baseClass->typeEntry()->isContainer()){
                break;
            }
            baseClass = baseClass->baseClass();
        }
        if(!baseClass){
            if (cls->templateBaseClass() && cls->templateBaseClass()->typeEntry()->isContainer()){

            }else if (cls->hasHashFunction()) {
                s << Qt::endl;
                s << INDENT << "@Override" << Qt::endl;
                s << INDENT << "@io.qt.QtUninvokable" << Qt::endl;
                s << INDENT << "public boolean equals(Object other) {" << Qt::endl;
                {
                    INDENTATION(INDENT)
                    QStringList lines;
                    for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                        if(codeSnip.language==TypeSystem::TargetLangCode && codeSnip.position==CodeSnip::Equals){
                            lines << codeSnip.code().split("\n");
                        }
                    }
                    if(!lines.isEmpty())
                        printExtraCode(lines, s);
                    QString nonGenericType = cls->typeEntry()->qualifiedTargetLangName();
                    registerPackage(nonGenericType);
                    nonGenericType = nonGenericType.replace('$', '.');
                    auto idx = nonGenericType.indexOf("<");
                    if(idx>0)
                        nonGenericType = nonGenericType.left(idx);
                    if(nonGenericType=="int")
                        nonGenericType = "java.lang.Integer";
                    else if(nonGenericType=="byte")
                        nonGenericType = "java.lang.Byte";
                    else if(nonGenericType=="short")
                        nonGenericType = "java.lang.Short";
                    else if(nonGenericType=="long")
                        nonGenericType = "java.lang.Long";
                    else if(nonGenericType=="double")
                        nonGenericType = "java.lang.Double";
                    else if(nonGenericType=="float")
                        nonGenericType = "java.lang.Float";
                    else if(nonGenericType=="boolean")
                        nonGenericType = "java.lang.Boolean";
                    else if(nonGenericType=="char")
                        nonGenericType = "java.lang.Character";
                    s << INDENT << "if(other instanceof " << nonGenericType << ")" << Qt::endl
                      << INDENT << "    return hashCode()==other.hashCode();" << Qt::endl
                      << INDENT << "else return super.equals(other);";
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
            }else{
                ReportHandler::warning(QString("No ==/!= operator found for value type %1.").arg(cls->qualifiedCppName()));
            }
        }
    }

    // Write the comparable functions
    const AbstractMetaFunctionList& gt_functions = cls->greaterThanFunctions();
    const AbstractMetaFunctionList& geq_functions = cls->greaterThanEqFunctions();
    const AbstractMetaFunctionList& lt_functions = cls->lessThanFunctions();
    const AbstractMetaFunctionList& leq_functions = cls->lessThanEqFunctions();
    const AbstractMetaFunctionList& compare_functions = cls->compareFunctions();

    bool hasEquals = eq_functions.size() || neq_functions.size();
    bool comparable = compare_functions.size() || (hasEquals
                                                        ? gt_functions.size() || geq_functions.size() || lt_functions.size() || leq_functions.size()
                                                        : gt_functions.size() == 1 || lt_functions.size() == 1);
    if (comparable) {
        bool suppressRaw = false;
        bool suppressUnchecked = false;
        QString comparableType = findComparableType(cls);
        QString comparableFunctionCode;
        {
            QTextStream s(&comparableFunctionCode);
            s << INDENT << "@io.qt.QtUninvokable" << Qt::endl;
            s << INDENT << "public int compareTo(" << comparableType << " other) {" << Qt::endl;
            {
                INDENTATION(INDENT)
                QStringList lines;
                for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                    if(codeSnip.language==TypeSystem::TargetLangCode && codeSnip.position==CodeSnip::Compare){
                        lines << codeSnip.code().split("\n");
                    }
                }
                if(!lines.isEmpty())
                    printExtraCode(lines, s);
                if(compare_functions.size()){
                    bool first = false;
                    write_compareto_parts(s, compare_functions, 0, &first, suppressUnchecked, suppressRaw);
                }else if (hasEquals) {
                    s << INDENT << "if (equals(other)) return 0;" << Qt::endl;
                    if (lt_functions.size() == 1
                            && gt_functions.size() == 0
                            && leq_functions.size() == 0
                            && geq_functions.size() == 0) {
                        s << INDENT << "else if (" << lt_functions[0]->name() << "(other)) return -1;" << Qt::endl
                          << INDENT << "else return 1;" << Qt::endl;
                    }else if (gt_functions.size() == 1
                              && lt_functions.size() == 0
                              && leq_functions.size() == 0
                              && geq_functions.size() == 0) {
                          s << INDENT << "else if (" << gt_functions[0]->name() << "(other)) return -1;" << Qt::endl
                            << INDENT << "else return 1;" << Qt::endl;
                    }else{
                        bool first = false;
                        if (lt_functions.size()) {
                            write_compareto_parts(s, lt_functions, -1, &first, suppressUnchecked, suppressRaw);
                        } else if (gt_functions.size()) {
                            write_compareto_parts(s, gt_functions, 1, &first, suppressUnchecked, suppressRaw);
                        } else if (leq_functions.size()) {
                            write_compareto_parts(s, leq_functions, -1, &first, suppressUnchecked, suppressRaw);
                        } else if (geq_functions.size()) {
                            write_compareto_parts(s, geq_functions, 1, &first, suppressUnchecked, suppressRaw);
                        }
                    }
                } else if (lt_functions.size() == 1) {
                    QString className = cls->typeEntry()->qualifiedTargetLangName();
                    if(cls->typeEntry()->isGenericClass()){
                        if(cls->templateBaseClass()){
                            QList<TypeEntry *> templateArguments = cls->templateBaseClass()->templateArguments();
                            if(templateArguments.size()>0){
                                className += "<";
                                for (int i = 0; i < templateArguments.size(); ++i) {
                                    if (i > 0)
                                        className += ",";
                                    className += "?";
                                }
                                className += ">";
                            }
                        }else{
                            className += "<T>";
                        }
                    }
                    s << INDENT << "if (" << lt_functions[0]->name() << "((" << className << ") other)) return -1;" << Qt::endl
                      << INDENT << "else if (((" << className << ") other)." << lt_functions[0]->name() << "(this)) return 1;" << Qt::endl
                      << INDENT << "else return 0;" << Qt::endl;

                } else if (gt_functions.size() == 1) {
                    QString className = cls->typeEntry()->qualifiedTargetLangName();
                    if(cls->typeEntry()->isGenericClass()){
                        if(cls->templateBaseClass()){
                            QList<TypeEntry *> templateArguments = cls->templateBaseClass()->templateArguments();
                            if(templateArguments.size()>0){
                                className += "<";
                                for (int i = 0; i < templateArguments.size(); ++i) {
                                    if (i > 0)
                                        className += ",";
                                    className += "?";
                                }
                                className += ">";
                            }
                        }else{
                            className += "<T>";
                        }
                    }
                    s << INDENT << "if (" << gt_functions[0]->name() << "((" << className << ") other)) return 1;" << Qt::endl
                      << INDENT << "else if (((" << className << ") other)." << gt_functions[0]->name() << "(this)) return -1;" << Qt::endl
                      << INDENT << "else return 0;" << Qt::endl;

                } else if (geq_functions.size() == 1 && leq_functions.size()) {
                    QString className = cls->typeEntry()->qualifiedTargetLangName();
                    if(cls->typeEntry()->isContainer()){
                        if(className=="java.util.List"
                               ||  className=="java.util.LinkedList"
                               ||  className=="java.util.Queue"
                               ||  className=="java.util.Deque"
                               ||  className=="java.util.ArrayList"
                               ||  className=="java.util.Vector"
                               ||  className=="java.util.Set"
                               ||  className=="io.qt.core.QSet"
                               ||  className=="io.qt.core.QList"
                               ||  className=="io.qt.core.QQueue"
                               ||  className=="io.qt.core.QVector"
                               ||  className=="io.qt.core.QStack"
                               ||  className=="io.qt.core.QLinkedList") {
                            className = "java.util.Collection";
                        }else if(className=="java.util.Map"
                               ||  className=="java.util.SortedMap"
                               ||  className=="java.util.NavigableMap"
                               ||  className=="java.util.HashMap"
                               ||  className=="java.util.TreeMap"
                               ||  className=="io.qt.core.QMap"
                               ||  className=="io.qt.core.QHash"
                               ||  className=="io.qt.core.QMultiMap"
                               ||  className=="io.qt.core.QMultiHash"){
                            className = "java.util.Map";
                        }
                    }
                    if(cls->typeEntry()->isGenericClass()){
                        if(cls->templateBaseClass()){
                            QList<TypeEntry *> templateArguments = cls->templateBaseClass()->templateArguments();
                            if(templateArguments.size()>0){
                                className += "<";
                                for (int i = 0; i < templateArguments.size(); ++i) {
                                    if (i > 0)
                                        className += ",";
                                    className += "?";
                                }
                                className += ">";
                            }
                        }
                    }
                    s << INDENT << "boolean less = operator_less_or_equal((" << className << ") other);" << Qt::endl
                      << INDENT << "boolean greater = operator_greater_or_equal((" << className << ") other);" << Qt::endl
                      << INDENT << "if (less && greater) return 0;" << Qt::endl
                      << INDENT << "else if (less) return -1;" << Qt::endl
                      << INDENT << "else return 1;" << Qt::endl;
                }else{
                    bool first = true;
                    if (lt_functions.size()) {
                        write_compareto_parts(s, lt_functions, -1, &first, suppressUnchecked, suppressRaw);
                    } else if (gt_functions.size()) {
                        write_compareto_parts(s, gt_functions, 1, &first, suppressUnchecked, suppressRaw);
                    } else if (leq_functions.size()) {
                        write_compareto_parts(s, leq_functions, -1, &first, suppressUnchecked, suppressRaw);
                    } else if (geq_functions.size()) {
                        write_compareto_parts(s, geq_functions, 1, &first, suppressUnchecked, suppressRaw);
                    }
                }
            }

            s << INDENT << "}" << Qt::endl;
        }
        if(suppressRaw && suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"rawtypes\", \"unchecked\"})" << Qt::endl;
        else if(suppressRaw)
            s << INDENT << "@SuppressWarnings({\"rawtypes\"})" << Qt::endl;
        else if(suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"unchecked\"})" << Qt::endl;
        s << comparableFunctionCode << Qt::endl;
    }

    if (!cls->isNamespace() && (cls->hasHashFunction() || ( cls->typeEntry()->isValue() && (eq_functions.size() > 0 || neq_functions.size() > 0) ))) {
        AbstractMetaFunctionList hashcode_functions = cls->queryFunctionsByName("hashCode");
        bool found = false;
        for(const AbstractMetaFunction* function : hashcode_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            QStringList lines;
            for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                if(codeSnip.language==TypeSystem::TargetLangCode && codeSnip.position==CodeSnip::HashCode){
                    lines << codeSnip.code().split("\n");
                }
            }
            if(lines.isEmpty() && cls->hasHashFunction() && !cls->typeEntry()->isNativeIdBased() && !cls->typeEntry()->designatedInterface()){
                s << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public native int hashCode();" << Qt::endl;
            }else{
                s << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public int hashCode() {" << Qt::endl;
                if(!lines.isEmpty()){
                    INDENTATION(INDENT)
                    printExtraCode(lines, s);
                }
                if (cls->hasHashFunction()) {
                    QString name = cls->name();
                    if(cls->typeEntry() && cls->typeEntry()->designatedInterface()){
                        name = cls->typeEntry()->designatedInterface()->targetLangName();
                    }
                    s << INDENT << "    return hashCode_native(";
                    if(cls->typeEntry()->isNativeIdBased()){
                        s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                    }else if(cls->typeEntry()->designatedInterface()){
                        s << "this";
                    }
                    s << ");" << Qt::endl
                      << INDENT << "}" << Qt::endl;
                    if(cls->typeEntry()->isNativeIdBased()){
                        s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                          << INDENT << "private native static int hashCode_native(long __this_nativeId);" << Qt::endl;
                    }else if(cls->typeEntry()->designatedInterface()){
                        s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                          << INDENT << "private native static int hashCode_native(" << cls->simpleName() << " __this);" << Qt::endl;
                    }else{
                        s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                          << INDENT << "private native int hashCode_native();" << Qt::endl;
                    }
                } else { // We have equals() but no qHash(), we return 0 from hashCode() to respect
                    // contract of java.lang.Object
                    if(lines.isEmpty() || !lines.last().contains("return ")){
                        s << INDENT << "    return 0;" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
            }
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
        AbstractMetaFunctionList tostring_functions = cls->queryFunctionsByName("toString");
        bool found = false;
        for(const AbstractMetaFunction* function : tostring_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        QStringList lines;
        for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
            if(codeSnip.language==TypeSystem::TargetLangCode && codeSnip.position==CodeSnip::ToString){
                lines << codeSnip.code().split("\n");
            }
        }
        if (!found) {
            if(lines.isEmpty() && !cls->typeEntry()->isNativeIdBased() && !cls->typeEntry()->designatedInterface()){
                s << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public native String toString();" << Qt::endl;
            }else{
                s << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public String toString() {" << Qt::endl;
                if(!lines.isEmpty()){
                    INDENTATION(INDENT)
                    printExtraCode(lines, s);
                }
                s << INDENT << "    return toString_native(";
                if(cls->typeEntry()->isNativeIdBased()){
                    s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                }else if(cls->typeEntry()->designatedInterface()){
                    s << "this";
                }
                s << ");" << Qt::endl
                  << INDENT << "}" << Qt::endl;
                if(cls->typeEntry()->isNativeIdBased()){
                    s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "private native static String toString_native(long __this_nativeId);" << Qt::endl;
                }else if(cls->typeEntry()->designatedInterface()){
                    s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "private native static String toString_native(" << cls->simpleName() << " __this);" << Qt::endl;
                }else{
                    s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "private native String toString_native();" << Qt::endl;
                }
            }
        }else if(!lines.isEmpty()){
            s << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "public String toString() {" << Qt::endl;
            {
                INDENTATION(INDENT)
                printExtraCode(lines, s);
                s << INDENT << "return super.toString();" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }
    }
}

void JavaGenerator::writeEnumOverload(QTextStream &s, const AbstractMetaFunction *java_function,
                                      uint include_attributes, uint exclude_attributes, Option _option) {
    const AbstractMetaArgumentList& arguments = java_function->arguments();

    const AbstractMetaClass *decl_class = java_function->declaringClass();
    if ((java_function->implementingClass() != decl_class
         && java_function->implementingClass()->baseClass()
         && (decl_class->isInterface()
             ? java_function->implementingClass()->baseClass()->inheritsFromInterface(decl_class)
             : java_function->implementingClass()->baseClass()->inheritsFrom(decl_class)))
            ||
             ((!java_function->isNormal() && !java_function->isConstructor()) || java_function->isEmptyFunction() || java_function->isAbstract())) {
        return ;
    }


    Option option = Option(_option);
    if (java_function->isConstructor())
        option = Option(option | SkipReturnType);
    else
        include_attributes |= AbstractMetaAttributes::FinalInTargetLang;
    exclude_attributes |= AbstractMetaAttributes::Native;
    exclude_attributes |= AbstractMetaAttributes::Abstract;
    if(java_function->ownerClass()->isInterface() && target_JDK_version>=8){
        option = Option(option | DefaultFunction);
    }

    int generate_enum_overload = -1;
    for (int i = 0; i < arguments.size(); ++i)
        generate_enum_overload = arguments.at(i)->type()->isTargetLangFlags() ? i : -1;

    if (generate_enum_overload >= 0) {
        const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
        QString comment;
        QTextStream commentStream(&comment);
        if (m_doc_parser) {
            // steal documentation from main function
            QString signature = functionSignature(java_function,
                                                  include_attributes | NoBlockedSlot,
                                                  exclude_attributes);
            commentStream << m_doc_parser->documentationForFunction(signature) << Qt::endl;
        }else{
            if(java_function->isConstructor()){
                commentStream << "<p>Overloaded constructor for {@link #";
            }else{
                commentStream << "<p>Overloaded function for {@link #";
            }
            commentStream << java_function->name();
            commentStream << "(";
            writeFunctionArguments(commentStream, java_function, int(arguments.size()), Option(option | NoSuppressExports | SkipName | SkipTemplateParameters));
            commentStream << ")}.</p>" << Qt::endl;
        }
        if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            writeDeprecatedComment(commentStream, java_function);
        }
        if(!comment.trimmed().isEmpty() && (_option & InFunctionComment)==0){
            s << INDENT << "/**" << Qt::endl;
            commentStream.seek(0);
            while(!commentStream.atEnd()){
                s << INDENT << " * " << commentStream.readLine() << Qt::endl;
            }
            s << INDENT << " */" << Qt::endl;
        }

        writeFunctionAttributes(s, java_function, generate_enum_overload >= 0 ? generate_enum_overload : -1, include_attributes, exclude_attributes, option);
        s << java_function->name() << "(";
        if (generate_enum_overload > 0 || addedArguments.first.size() + addedArguments.second.size()>0) {
            writeFunctionArguments(s, java_function, generate_enum_overload);
            s << ", ";
        }

        // Write the ellipsis convenience argument
        AbstractMetaArgument *affected_arg = arguments.at(generate_enum_overload);
        const EnumTypeEntry *originator = static_cast<const FlagsTypeEntry *>(affected_arg->type()->typeEntry())->originator();

        s << originator->javaPackage().replace("$",".");
        if(!originator->javaQualifier().isEmpty())
            s << "." << originator->javaQualifier().replace("$",".");
        s << "." << originator->targetLangName().replace("$",".")
          << " ... " << affected_arg->modifiedArgumentName() << ")";
        QString throws = java_function->throws();
        if(!throws.isEmpty()){
            s << " throws " << throws << " ";
        }
        s << "{" << Qt::endl;

        s << INDENT << "    ";
        QString new_return_type = java_function->typeReplaced(0);
        if (new_return_type != "void" && (!new_return_type.isEmpty() || java_function->type()))
            s << "return ";

        if (java_function->isConstructor()) {
            s << "this";
        } else {
            /*if (java_function->isStatic())
                s << java_function->implementingClass()->fullName().replace("$", ".") << ".";
            else
                s << "this.";*/
            s << java_function->name();
        }

        s << "(";
        int argumentCounter = 1;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& mod = addedArguments.first[argumentCounter];
            s << mod.modified_name << ", ";
            ++argumentCounter;
        }
        for (int i = 0; i < generate_enum_overload; ++i) {
            AbstractMetaArgument *arg = arguments.at(i);
            if(java_function->argumentRemoved(arg->argumentIndex()+1)==ArgumentRemove_No)
                s << arg->modifiedArgumentName() << ", ";
            ++argumentCounter;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& mod = addedArguments.first[argumentCounter];
                s << mod.modified_name << ", ";
                ++argumentCounter;
            }
        }
        for(const ArgumentModification& mod : addedArguments.second){
            s << mod.modified_name << ", ";
        }
        registerPackage(affected_arg->type()->fullName());
        s << "new " << affected_arg->type()->fullName().replace('$', '.') << "(" << affected_arg->modifiedArgumentName() << "));" << Qt::endl
          << INDENT << "}" << Qt::endl
          << INDENT << Qt::endl;
    }
}

void JavaGenerator::writeInstantiatedType(QTextStream &s, const AbstractMetaType *abstractMetaType, bool forceBoxed) const {
    Q_ASSERT(abstractMetaType);

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
        }else if(type->designatedInterface()){
            s << type->designatedInterface()->qualifiedTargetLangName().replace('$', '.');
        }else{
            s << type->qualifiedTargetLangName().replace('$', '.');
        }
    }else if(type->designatedInterface()){
        s << type->designatedInterface()->qualifiedTargetLangName().replace('$', '.');
    }else if(type->isContainer()){
        const ContainerTypeEntry* ctype = static_cast<const ContainerTypeEntry*>(type);
        switch(ctype->type()){
        case ContainerTypeEntry::StringListContainer:
        case ContainerTypeEntry::ByteArrayListContainer:
        case ContainerTypeEntry::ListContainer:
        case ContainerTypeEntry::LinkedListContainer:
        case ContainerTypeEntry::VectorContainer:
        case ContainerTypeEntry::StackContainer:
        case ContainerTypeEntry::QueueContainer:
        case ContainerTypeEntry::SetContainer:
        case ContainerTypeEntry::std_vector: s << "java.util.Collection"; break;
        case ContainerTypeEntry::MultiMapContainer:
        case ContainerTypeEntry::HashContainer:
        case ContainerTypeEntry::MultiHashContainer:
        case ContainerTypeEntry::MapContainer: s << "java.util.Map"; break;
        default:
            s << type->qualifiedTargetLangName().replace('$', '.'); break;
        }
    }else{
        s << type->qualifiedTargetLangName().replace('$', '.');
    }

    if (abstractMetaType->hasInstantiations()) {
        s << "<";
        const QList<const AbstractMetaType *>& instantiations = abstractMetaType->instantiations();
        for (int i = 0; i < instantiations.size(); ++i) {
            if (i > 0)
                s << ", ";

            writeInstantiatedType(s, instantiations.at(i), true);
        }
        s << ">";
    }
}

void JavaGenerator::writeFunctionOverloads(QTextStream &s, const AbstractMetaFunction *java_function,
        uint include_attributes, uint exclude_attributes, Option _option, const QString& alternativeFunctionName) {
    const AbstractMetaArgumentList& arguments = java_function->arguments();

    // We only create the overloads for the class that actually declares the function
    // unless this is an interface, in which case we create the overloads for all
    // classes that directly implement the interface.
    const AbstractMetaClass *decl_class = java_function->declaringClass();

    if (java_function->implementingClass() != decl_class
         && java_function->implementingClass()->baseClass()
         && (decl_class->isInterface()
            ? java_function->implementingClass()->baseClass()->inheritsFromInterface(decl_class)
            : java_function->implementingClass()->baseClass()->inheritsFrom(decl_class)))
        return;

    // Figure out how many functions we need to write out,
    // One extra for each default argument.
    QList<int> argumentCounts;
    uint excluded_attributes = AbstractMetaAttributes::Abstract
                               | AbstractMetaAttributes::Native
                               | exclude_attributes;
    uint included_attributes = (java_function->isConstructor() ? 0 : AbstractMetaAttributes::Final) | include_attributes;

    for (int i = int(arguments.size()-1); i >= 0; --i) {
        const AbstractMetaArgument *arg = arguments.at(i);
        if(java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No){
            if (arg->defaultValueExpression().isEmpty()){
                break;
            }
            argumentCounts << i;
        }
    }
    for (int i = 0; i < argumentCounts.size(); ++i) {
        QString comment;
        QTextStream commentStream(&comment);

        int used_arguments = argumentCounts[i];

        uint option = uint(_option);
        if(!java_function->isEmptyFunction()
                && !java_function->isNormal()
                && !java_function->isSignal()){
            option = option | SkipReturnType;
        }
        if(java_function->ownerClass()->isInterface() && target_JDK_version>=8 && (_option & InFunctionComment)==0){
            option = option | DefaultFunction;
        }
        QString signature = functionSignature(java_function, included_attributes,
                                              excluded_attributes,
                                              Option(option),
                                              used_arguments,
                                              alternativeFunctionName);
        {
            if(java_function->isConstructor()){
                commentStream << "<p>Overloaded constructor for {@link #";
            }else{
                commentStream << "<p>Overloaded function for {@link #";
            }
            if(alternativeFunctionName.isEmpty()){
                commentStream << java_function->name();
            }else{
                commentStream << alternativeFunctionName;
            }
            commentStream << "(";
            writeFunctionArguments(commentStream, java_function, int(arguments.size()), Option(SkipName | NoSuppressExports | SkipTemplateParameters));
            commentStream << ")}";
            bool useList = arguments.size()-used_arguments>1;
            if(useList)
                commentStream << "</p>" << Qt::endl << "<p>with: </p><ul>" << Qt::endl;
            else
                commentStream << Qt::endl << " with ";
            for (int j = used_arguments; j < arguments.size(); ++j) {
                auto arg = arguments.at(j);
                if(useList)
                    commentStream << "<li>";
                commentStream << "<code>" << arg->argumentName() << " = ";
                QString defaultExpr = arg->defaultValueExpression();
                auto pos = defaultExpr.indexOf(".");
                if (pos > 0) {
                    QString someName = defaultExpr.left(pos);
                    ComplexTypeEntry *ctype =
                        TypeDatabase::instance()->findComplexType(someName);
                    QString replacement;
                    if (ctype && ctype->isVariant())
                        replacement = "io.qt.QVariant.";
                    else if (ctype)
                        if(ctype->javaPackage().isEmpty())
                            replacement = ctype->targetLangName() + ".";
                        else
                            replacement = ctype->javaPackage() + "." + ctype->targetLangName() + ".";
                    else
                        replacement = someName + ".";
                    defaultExpr = defaultExpr.replace(someName + ".", replacement);
                }
                if (java_function->typeReplaced(arg->argumentIndex() + 1).isEmpty() && arg->type()->isFlags()) {
                    defaultExpr = "new " + arg->type()->fullName().replace('$', '.') + "(" + defaultExpr + ")";
                }
                commentStream << defaultExpr
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;");
                commentStream << "</code>";
                if(useList)
                    commentStream << "</li>" << Qt::endl;
            }
            if(useList)
                commentStream << "</ul>" << Qt::endl;
            else
                commentStream << ".</p>" << Qt::endl;
        }

        if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            writeDeprecatedComment(commentStream, java_function);
        }

        if(!comment.trimmed().isEmpty() && (_option & InFunctionComment)==0 && !java_function->isPrivateSignal()){
            s << INDENT << "/**" << Qt::endl;
            commentStream.seek(0);
            while(!commentStream.atEnd()){
                s << INDENT << " * " << commentStream.readLine() << Qt::endl;
            }
            s << INDENT << " */" << Qt::endl;
        }
        s << signature << " {" << Qt::endl;
        {
            INDENTATION(INDENT)
            QString new_return_type = java_function->typeReplaced(0);
            s << INDENT;
            if (new_return_type != "void" && (!new_return_type.isEmpty() || java_function->type()))
                s << "return ";
            if (java_function->isConstructor())
                s << "this";
            else{
                if(alternativeFunctionName.isEmpty())
                    s << java_function->name();
                else
                    s << alternativeFunctionName;
            }
            s << "(";

            int written_arguments = 0;
            const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
            int argumentCounter = 1;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                if(written_arguments > 0)
                    s << ", ";
                s << argumentMod.modified_name;
                ++argumentCounter;
                ++written_arguments;
            }
            for (int j = 0; j < arguments.size(); ++j) {
                const AbstractMetaArgument* arg = arguments.at(j);
                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    if (written_arguments > 0)
                        s << ", ";

                    if (j < used_arguments) {
                        s << arg->modifiedArgumentName();
                    } else {
                        QString defaultExpr = arg->defaultValueExpression();
                        AbstractMetaType *arg_type = nullptr;
                        QString modified_type = java_function->typeReplaced(arg->argumentIndex() + 1);
                        if (modified_type.isEmpty()) {
                            arg_type = arg->type();
                            if (arg_type->isNativePointer()) {
                                if(defaultExpr=="null"
                                        && !java_function->argumentTypeArray(arg->argumentIndex() + 1)
                                        && !java_function->argumentTypeBuffer(arg->argumentIndex() + 1))
                                    s << "(io.qt.QNativePointer)";
                            } else {
                                const AbstractMetaType *abstractMetaType = arg->type();
                                const TypeEntry *type = abstractMetaType->typeEntry();
                                if (type->designatedInterface())
                                    type = type->designatedInterface();
                                if (!type->isEnum() && !type->isFlags()) {
                                    if(defaultExpr=="null" || type->isPrimitive()){
                                        s << "(";
                                        writeInstantiatedType(s, abstractMetaType, false);
                                        s << ")";
                                    }
                                }
                            }
                        } else {
                            if(defaultExpr=="null")
                                s << "(" << modified_type.replace('$', '.') << ")";
                        }

                        auto pos = defaultExpr.indexOf(".");
                        if (pos > 0) {
                            QString someName = defaultExpr.left(pos);
                            ComplexTypeEntry *ctype =
                                TypeDatabase::instance()->findComplexType(someName);
                            QString replacement;
                            if (ctype && ctype->isVariant())
                                replacement = "io.qt.QVariant.";
                            else if (ctype)
                                if(ctype->javaPackage().isEmpty())
                                    replacement = ctype->targetLangName() + ".";
                                else
                                    replacement = ctype->javaPackage() + "." + ctype->targetLangName() + ".";
                            else
                                replacement = someName + ".";
                            defaultExpr = defaultExpr.replace(someName + ".", replacement);
                        }

                        if (arg_type && arg_type->isFlags()) {
                            s << "new " << arg_type->fullName().replace('$', '.') << "(" << defaultExpr << ")";
                        } else {
                            s << defaultExpr;
                        }
                    }
                    ++written_arguments;
                }
                ++argumentCounter;
                while(addedArguments.first.contains(argumentCounter)){
                    const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
                    if(written_arguments > 0)
                        s << ", ";
                    s << argumentMod.modified_name;
                    ++argumentCounter;
                    ++written_arguments;
                }
            }
            for(const ArgumentModification& argumentMod : addedArguments.second){
                if(written_arguments > 0)
                    s << ", ";
                ++written_arguments;
                s << argumentMod.modified_name;
            }
            s << ");" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl
          << INDENT << Qt::endl;
    }
}

QString JavaGenerator::subDirectoryForClass(const AbstractMetaClass *java_class) const{
    QString pkgDir = java_class->typeEntry()->isGlobal() ? QString() : subDirectoryForPackage(java_class->package());
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(java_class->typeEntry()->targetTypeSystem()));
    if(!typeSystemEntry)
        typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(java_class->package()));
    if(!typeSystemEntry)
        return pkgDir;
    if(!typeSystemEntry->module().isEmpty()){
        return typeSystemEntry->module() + "/" + pkgDir;
    }else if(!typeSystemEntry->qtLibrary().isEmpty()){
        if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
            QString libName = typeSystemEntry->qtLibrary();
            return "QtJambi" + libName.mid(2) + "/" + pkgDir;
        }else
            return typeSystemEntry->qtLibrary() + "/" + pkgDir;
    }else{
        return pkgDir;
    }
}

QString JavaGenerator::subDirectoryForFunctional(const AbstractMetaFunctional * java_class) const{
    QString pkgDir = subDirectoryForPackage(java_class->package());
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(java_class->typeEntry()->targetTypeSystem()));
    if(!typeSystemEntry)
        typeSystemEntry = static_cast<TypeSystemTypeEntry *>(TypeDatabase::instance()->findType(java_class->package()));
    if(typeSystemEntry && !typeSystemEntry->qtLibrary().isEmpty()){
        if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
            QString libName = typeSystemEntry->qtLibrary();
            return "QtJambi" + libName.mid(2) + "/" + pkgDir;
        }else
            return typeSystemEntry->qtLibrary() + "/" + pkgDir;
    }else
        return pkgDir;
}

void JavaGenerator::write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level) {
    if(java_class->enclosingClass() && nesting_level==0){ // don't write nested classes into own file
        return;
    }
    if(java_class->typeEntry()->designatedInterface() && java_class->typeEntry()->designatedInterface()->noImpl()){
        return;
    }

    if(java_class->typeEntry()->isIterator())
        return;

    {
        AbstractMetaFunctionList inconsistentFunctions = java_class->cppInconsistentFunctions();
        if(!inconsistentFunctions.isEmpty()){
            for(AbstractMetaFunction* f : inconsistentFunctions)
                m_inconsistent_functions << f;
            ReportHandler::warning("Unable to generate class "+java_class->fullName()+" due to inconsistent functions.");
            return;
        }
    }

    ReportHandler::debugSparse("Generating class: " + java_class->fullName());
    bool fakeClass = java_class->isFake();

    if(nesting_level==0){
       if (m_docs_enabled) {
           m_doc_parser = new DocParser(m_doc_directory + "/" + java_class->name().toLower() + ".jdoc");
       }
        s << INDENT << "package " << java_class->package() << ";" << Qt::endl << Qt::endl;
        m_currentPackages.clear();

        for(const Include &inc : java_class->typeEntry()->extraIncludes()) {
            if (inc.type == Include::TargetLangImport) {
                s << inc.toString() << Qt::endl;
            }
        }
    }

    QString comment;
    QString lines;
    {
        QTextStream s(&lines);
        QTextStream commentStream(&comment);
        if (m_doc_parser) {
           commentStream << m_doc_parser->documentation(java_class) << Qt::endl;
        }else{
            if(java_class->typeEntry()->designatedInterface()){
                commentStream << "<p>Implementor class for interface {@link " << QString(java_class->typeEntry()->designatedInterface()->qualifiedTargetLangName()).replace("$", ".") << "}</p>" << Qt::endl;
            }else{
                if(!java_class->brief().isEmpty()){
                    commentStream << "<p>" << QString(java_class->brief())
                                     .replace("&", "&amp;")
                                     .replace("<", "&lt;")
                                     .replace(">", "&gt;")
                                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                     .replace("@", "&commat;")
                                     .replace("/*", "&sol;*")
                                     .replace("*/", "*&sol;")
                                  << "</p>" << Qt::endl;
                }
                if(java_class->href().isEmpty()){
                    commentStream << "<p>Java wrapper for Qt ";
                    if(java_class->typeEntry()->isNamespace()){
                        if(reinterpret_cast<const NamespaceTypeEntry*>(java_class->typeEntry())->isHeader())
                            commentStream << "namespace ";
                        else
                            commentStream << "header file ";
                    }else{
                        commentStream << "class ";
                    }
                    commentStream << (java_class->templateBaseClass() ? java_class->templateBaseClass()->qualifiedCppName().replace("<JObjectWrapper>", "<T>") : java_class->qualifiedCppName() )
                                     .replace("&", "&amp;")
                                     .replace("<", "&lt;")
                                     .replace(">", "&gt;")
                                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                     .replace("@", "&commat;")
                                     .replace("/*", "&sol;*")
                                     .replace("*/", "*&sol;") << "</p>" << Qt::endl;
                }else{
                    QString url = docsUrl+java_class->href();
                    commentStream << "<p>Java wrapper for Qt's "
                                  << (java_class->typeEntry()->isNamespace() ? "namespace" : "class")
                                  << " <a href=\"" << url << "\">"
                                  << (java_class->templateBaseClass() ? java_class->templateBaseClass()->qualifiedCppName().replace("<JObjectWrapper>", "<T>") : java_class->qualifiedCppName() )
                                     .replace("&", "&amp;")
                                     .replace("<", "&lt;")
                                     .replace(">", "&gt;")
                                     .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                     .replace("@", "&commat;")
                                     .replace("/*", "&sol;*")
                                     .replace("*/", "*&sol;")
                                  << "</a></p>" << Qt::endl;
                }
            }
        }

        if (java_class->isDeprecated()) {
            if(!java_class->deprecatedComment().isEmpty()){
                if(!comment.isEmpty())
                    commentStream << Qt::endl;
                commentStream << "@deprecated " << QString(java_class->deprecatedComment())
                                 .replace("&", "&amp;")
                                 .replace("<", "&lt;")
                                 .replace(">", "&gt;")
                                 .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                 .replace("@", "&commat;")
                                 .replace("/*", "&sol;*")
                                 .replace("*/", "*&sol;")
                              << Qt::endl;
            }
            s << INDENT << "@Deprecated" << Qt::endl;
        }

        s << INDENT;

        bool force_abstract = (java_class->typeEntry()->typeFlags() & ComplexTypeEntry::ForceAbstract) != 0;
        bool isInterface = false;
        if (java_class->isInterface()) {
            s << "public interface ";
            isInterface = true;
        } else {
            bool force_friendly = (java_class->typeEntry()->typeFlags() & ComplexTypeEntry::ForceFriendly) != 0;
            if (java_class->isPublic() && !force_friendly)
                s << "public ";
            // else friendly

            if(nesting_level>0){
                s << "static ";
            }

            bool isFinal = false;
            bool isAbstract = false;
            if (!java_class->typeEntry()->targetType().isEmpty()) {
                isInterface = java_class->typeEntry()->targetType().contains("interface");
                isFinal = java_class->typeEntry()->targetType().contains("final");
                isAbstract = java_class->typeEntry()->targetType().contains("abstract");
            }

            if ((java_class->isFinal() || java_class->isNamespace())
                    && !java_class->hasSubClasses()
                    && !isInterface
                    && !isFinal
                    && !isAbstract
                    && !force_abstract
                    && !java_class->isAbstract()
            ){
                s << "final ";
            }else if ( ( (java_class->isAbstract() && !java_class->isNamespace()) || force_abstract ) && !isInterface && !isAbstract && !isFinal){
                s << "abstract ";
            }

            if (!java_class->typeEntry()->targetType().isEmpty()) {
                s << java_class->typeEntry()->targetType() << " ";
            } else {
                s << "class ";
            }

        }

        const ComplexTypeEntry *type = java_class->typeEntry();

        s << java_class->simpleName();

        if (type->isGenericClass()) {
            s << "<";
            if(java_class->templateBaseClass()){
                QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
                for (int i = 0; i < templateArguments.size(); ++i) {
                    TypeEntry *templateArgument = templateArguments.at(i);
                    if (i > 0)
                        s << ", ";
                    s << QString(templateArgument->name()).replace('$', '.');
                }
            }else{
                s << "T";
            }
            s << ">";
        }

        bool isContainer = false;
        bool isTemplate = false;
        if(java_class->templateBaseClass() && java_class->templateBaseClass()->typeEntry()->isContainer()){
            const ContainerTypeEntry* ctype = static_cast<const ContainerTypeEntry*>(java_class->templateBaseClass()->typeEntry());
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
            {
                m_currentPackages << ctype->javaPackage();
                s << " extends " << ctype->javaPackage() << "." << ctype->targetLangName() << "<";
                int k=0;
                for(const AbstractMetaType * instantiation : java_class->templateBaseClassInstantiations()){
                    if(k>0){
                        s << ", ";
                    }
                    s << translateType(instantiation, java_class, BoxedPrimitive);
                    k++;
                }
                s << ">";
                isTemplate = true;
                break;
            }
            default: break;
            }
            isContainer = true;
        }
        if(!isTemplate){
            if (!java_class->isNamespace() && !java_class->isInterface()) {
                if (!java_class->baseClassName().isEmpty()) {
                    m_currentPackages << java_class->baseClass()->package();
                    if(java_class->baseClass()->fullName()==java_class->fullName()){
                        QString sc = QString(type->defaultSuperclass()).replace("$",".");
                        if (!sc.isEmpty())
                            s << " extends " << sc;
                        else
                            s << " extends io.qt.QtObject";
                    }else{
                        s << " extends " << java_class->baseClass()->fullName().replace("$",".");
                    }
                } else {
                    QString sc = QString(type->defaultSuperclass()).replace("$",".");
                    if (!sc.isEmpty())
                        s << " extends " << sc;
                    else
                        s << " extends io.qt.QtObject";
                }
            } else if (java_class->isInterface()) {
                s << " extends io.qt.QtObjectInterface";
            }
        }

        // implementing interfaces...
        bool implements = java_class->isInterface();
        AbstractMetaClassList interfaces = java_class->interfaces();

        if (!interfaces.isEmpty()) {
            if (java_class->isInterface())
                s << ", ";
            else {
                s << Qt::endl << INDENT << "    implements ";
                implements = true;
            }
            for (int i = 0; i < interfaces.size(); ++i) {
                AbstractMetaClass *iface = interfaces.at(i);
                m_currentPackages << iface->package();
                if (i != 0)
                    s << "," << Qt::endl << INDENT << "            ";
                s << iface->package() << "." << iface->name().replace('$', '.');
            }
        }

        if (isComparable(java_class)) {
            if (!implements) {
                implements = true;
                s << Qt::endl << INDENT << "    implements ";
            } else {
                s << "," << Qt::endl << INDENT << "            ";
            }
            QString ct = findComparableType(java_class);
            registerPackage(ct);
            s << "java.lang.Comparable<" << ct << ">";
        }

        const AbstractMetaType * iterableType = nullptr;
        if(!isContainer)
            iterableType = getIterableType(java_class);
        if (iterableType) {
            if (!implements) {
                implements = true;
                s << Qt::endl << INDENT << "    implements ";
            } else {
                s << "," << Qt::endl << INDENT << "            ";
            }
            s << "java.lang.Iterable<" << translateType(iterableType, java_class, BoxedPrimitive)<< ">";
        }

        if (java_class->hasCloneOperator()) {
            if (!implements) {
                implements = true;
                s << Qt::endl << INDENT << "    implements ";
            } else {
                s << "," << Qt::endl << INDENT << "            ";
            }
            s << "java.lang.Cloneable";
        }

        if (!java_class->typeEntry()->implements().isEmpty()) {
            if (!implements) {
                implements = true;
                s << Qt::endl << INDENT << "    implements ";
            } else {
                s << "," << Qt::endl << INDENT << "            ";
            }
            for(QString cls : java_class->typeEntry()->implements().split(",")){
                registerPackage(cls);
            }
            s << java_class->typeEntry()->implements();
        }

        s << Qt::endl << INDENT << "{" << Qt::endl;

        {
            INDENTATION(INDENT)

            if (!java_class->isInterface()
                && !java_class->typeEntry()->targetType().contains("interface")) {
                if(!java_class->baseClass()
                    || java_class->package() != java_class->baseClass()->package()
                    || java_class->baseClass()->fullName()==java_class->fullName()){
                    s << INDENT << "static {" << Qt::endl;
                        s << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl; //" << java_class->package() << ".
                    s << INDENT << "}" << Qt::endl
                      << INDENT << Qt::endl;
                }
                if (java_class->typeEntry()->expensePolicy().isValid()) {
                    s << INDENT << "private static long __qt_expenseCounter = 0;" << Qt::endl;
                }
            }

            // Define variables for reference count mechanism
            if (!java_class->isInterface() && !java_class->isNamespace()) {
                QHash<QString, int> variables;
                //bool isWrapperClass = java_class->typeEntry()->lookupName().endsWith("$ConcreteWrapper");
                for(AbstractMetaFunction *function : java_class->functions()) {
                    QList<ReferenceCount> referenceCounts = function->referenceCounts(java_class);
                    for(const ReferenceCount& refCount : referenceCounts) {
                        variables[refCount.variableName] |=
                            uint(refCount.action)
                            | ( /*(isWrapperClass && function->isAbstract()) ? ReferenceCount::Friendly :*/ refCount.access )
                            | (refCount.threadSafe ? ReferenceCount::ThreadSafe : 0)
                            | (function->isStatic() ? ReferenceCount::Static : 0)
                            | (refCount.declareVariable.isEmpty() ? ReferenceCount::DeclareVariable : 0);
                    }
                }
                for(AbstractMetaField *field : java_class->fields()) {
                    QList<ReferenceCount> referenceCounts = field->setter()->referenceCounts(java_class);
                    for(const ReferenceCount& refCount : referenceCounts) {
                        variables[refCount.variableName] |=
                            uint(refCount.action)
                            | ( /*(isWrapperClass && function->isAbstract()) ? ReferenceCount::Friendly :*/ refCount.access )
                            | (refCount.threadSafe ? ReferenceCount::ThreadSafe : 0)
                            | (field->isStatic() ? ReferenceCount::Static : 0)
                            | (refCount.declareVariable.isEmpty() ? ReferenceCount::DeclareVariable : 0);
                    }
                }

                for(const QString& variableName : variables.keys()) {
                    int attributes = variables[variableName];
                    int actions = attributes & ReferenceCount::ActionsMask;
                    bool threadSafe = attributes & ReferenceCount::ThreadSafe;
                    bool isStatic = attributes & ReferenceCount::Static;
                    bool declareVariable = attributes & ReferenceCount::DeclareVariable;
                    int access = attributes & ReferenceCount::AccessMask;

                    if (actions == ReferenceCount::Ignore || !declareVariable)
                        continue;

                    if (((actions & ReferenceCount::Add) == 0) != ((actions & ReferenceCount::Remove) == 0)
                            && !(actions & ReferenceCount::ClearAdd)&& !(actions & ReferenceCount::ClearAddAll)) {
                        QString warn = QString("Either add or remove specified for reference count variable '%1' in '%2' but not both.")
                                       .arg(variableName).arg(java_class->qualifiedCppName());
                        ReportHandler::warning(warn);
                    }

                    s << INDENT << "@io.qt.QtPropertyMember(enabled=false)" << Qt::endl
                      << INDENT;
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
                        s << "java.util.Map<Object,Object> " << variableName;
                        s << ";" << Qt::endl;
                    }else if (actions != ReferenceCount::Set && actions != ReferenceCount::Ignore) {
                        s << "java.util.Collection<Object> " << variableName;
                        s << ";" << Qt::endl;
                    } else if (actions != ReferenceCount::Ignore) {

                        if (threadSafe)
                            s << "synchronized ";
                        s << "Object " << variableName << " = null;" << Qt::endl;
                    }
                }
                if(!variables.isEmpty())
                    s << INDENT << Qt::endl;
            }

            if(java_class->isNamespace()){
                s << INDENT << "private " << java_class->simpleName() << "() throws java.lang.InstantiationError { throw new java.lang.InstantiationError(\"Cannot instantiate namespace " << java_class->simpleName() << ".\"); }" << Qt::endl
                  << INDENT << Qt::endl;
            }

            if(java_class->isInterface() || !java_class->typeEntry()->designatedInterface()){
                if (java_class->has_Q_GADGET() || java_class->has_Q_OBJECT()) {
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * This variable stores the meta-object for the class." << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "public static final io.qt.core.QMetaObject staticMetaObject = io.qt.core.QMetaObject.forType("
                      << java_class->name().replace('$', '.') << ".class);" << Qt::endl
                      << INDENT << Qt::endl;
                }
            }

            if (!java_class->isInterface() && (java_class->isAbstract() || force_abstract)) {
                s << INDENT << "@io.qt.NativeAccess" << Qt::endl
                  << INDENT << "private static final class ConcreteWrapper extends " << java_class->name().replace('$', '.') << " {" << Qt::endl;

                {
                    INDENTATION(INDENT)
                    s << INDENT << Qt::endl
                      << INDENT << "@io.qt.NativeAccess" << Qt::endl
                      << INDENT << "private ConcreteWrapper(QPrivateConstructor p) { super(p); }" << Qt::endl;

                    AbstractMetaFunctionList functions = java_class->queryFunctions(AbstractMetaClass::NormalFunctions | AbstractMetaClass::AbstractFunctions | AbstractMetaClass::NonEmptyFunctions | AbstractMetaClass::NotRemovedFromTargetLang);
                    for(const AbstractMetaFunction *java_function : functions) {
                        if(!java_function->isPrivate()){
                            uint exclude_attributes = AbstractMetaAttributes::Abstract;
                            if(java_function->needsCallThrough()){
                                exclude_attributes |= AbstractMetaAttributes::Native;
                            }
                            uint include_attributes = 0;
                            retrieveModifications(java_function, java_class, &exclude_attributes, &include_attributes);

                            s << INDENT << Qt::endl
                              << INDENT << "@Override" << Qt::endl;
                            writeFunctionAttributes(s, java_function, -1, include_attributes, exclude_attributes,
                                                    (java_function->isNormal() || java_function->isSignal()) ? Option(NoSuppressExports) : Option(SkipReturnType | NoSuppressExports));

                            s << java_function->name() << "(";
                            writeFunctionArguments(s, java_function, int(java_function->arguments().count()));
                            s << ")";
                            QString throws = java_function->throws();
                            if(!throws.isEmpty()){
                                s << " throws " << throws << " ";
                            }
                            if(java_function->needsCallThrough()){
                                s << "{" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    writeJavaCallThroughContents(s, java_function);
                                }
                                s << INDENT << "}" << Qt::endl;
                                writePrivateNativeFunction(s, java_function);
                            }else{
                                s << ";" << Qt::endl << Qt::endl;
                            }
                        }
                    }
                }
                s << INDENT << "}" << Qt::endl
                  << INDENT << Qt::endl;
            }

            // Enums
            int writtenEnums = 0;
            for(AbstractMetaEnum *java_enum : java_class->enums()){
                if(java_enum->enclosingClass()==java_class){
                    writeEnum(s, java_enum);
                    ++writtenEnums;
                }
            }

            // functionals
            for(AbstractMetaFunctional *java_f : java_class->functionals())
                writeFunctional(s, java_f);

            if ((writtenEnums>0 || !java_class->functionals().isEmpty()) && !java_class->enclosedClasses().isEmpty())
                s << Qt::endl;
            // write enclosed types as static embedded classes
            for(AbstractMetaClass *enclosed_java_class : java_class->enclosedClasses()){
                if ((enclosed_java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)){
                    write(s, enclosed_java_class, nesting_level+1);
                }
            }
            if (!java_class->enclosedClasses().isEmpty() && !java_class->functions().isEmpty())
                s << Qt::endl;

            // Signals
            QList<QString> signalNames;
            QMap<QString,AbstractMetaFunctionList> sortedSignals;
            for(AbstractMetaFunction* function : java_class->queryFunctions(AbstractMetaClass::Signals
                                                                                | AbstractMetaClass::ClassImplements
                                                                                | AbstractMetaClass::NotRemovedFromTargetLang)){
                QString key = function->declaringClass()->typeEntry()->qualifiedCppName() + "::" + function->name();
                if(!signalNames.contains(key))
                    signalNames.append(key);
                sortedSignals[key].append(function);
            }

            for(const QString& key : signalNames){
                const AbstractMetaFunctionList& list = sortedSignals[key];
                if(list.size()==1)
                    writeSignal(s, list.first());
                else if(list.size()>1)
                    writeMultiSignal(s, list);
            }
            s << INDENT << Qt::endl;

            // Class has subclasses but also only private constructors
            if (!java_class->isInterface() && java_class->hasUnimplmentablePureVirtualFunction()) {
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * This constructor is a place holder intended to prevent" << Qt::endl
                  << INDENT << " * users from subclassing the class. Certain classes can" << Qt::endl
                  << INDENT << " * unfortunately only be subclasses internally. The constructor" << Qt::endl
                  << INDENT << " * will indiscriminately throw an exception if called. If the" << Qt::endl
                  << INDENT << " * exception is ignored, any use of the constructed object will" << Qt::endl
                  << INDENT << " * cause an exception to occur." << Qt::endl << Qt::endl
                  << INDENT << " * @throws io.qt.QClassCannotBeSubclassedException" << Qt::endl
                  << INDENT << " **/" << Qt::endl
                  << INDENT << "@SuppressWarnings(\"unused\")" << Qt::endl
                  << INDENT << "private " << java_class->simpleName() << "() throws io.qt.QClassCannotBeSubclassedException {" << Qt::endl
                  << INDENT << "    super((QPrivateConstructor)null);" << Qt::endl
                  << INDENT << "    throw new io.qt.QClassCannotBeSubclassedException(" << java_class->simpleName() << ".class);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
            }

            // Functions
            bool alreadyHasCloneMethod = false;
            bool generateShellClass = java_class->generateShellClass();
            AbstractMetaFunctionList privatePureVirtualFunctions;

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
                            || !function->isPublic())
                        continue;
                }

                if (function->name() == "clone" && function->arguments().isEmpty())
                    alreadyHasCloneMethod = true;

                if(java_class->isInterface() && java_class->hasJustPrivateConstructors() && function->isFinalInTargetLang())
                    continue;
                if(java_class->isInterface() && !function->isPublic())
                    continue;
                if(java_class->isInterface() && function->isConstructor())
                    continue;
                if(java_class->isInterface() && function->declaringClass()!=java_class && !function->isAbstract())
                    continue;
                if(java_class->hasPrivateDestructor() && function->isConstructor())
                    continue;
                if(java_class->hasUnimplmentablePureVirtualFunction() && function->isConstructor())
                    continue;
                if(!function->isPublic() && !generateShellClass)
                    continue;
                if(function->isPrivate()){
                    if(!function->isFinal() && function->isAbstract()){
                        privatePureVirtualFunctions << function;
                    }
                    continue;
                }
                writeFunction(s, function);
            }

            // Just the private functions for abstract functions implemented in superclasses
            /*
            if (!java_class->isInterface() &&
                    java_class->isAbstract() &&
                    !java_class->hasUnimplmentablePureVirtualFunction()) {
                java_funcs = java_class->queryFunctions(AbstractMetaClass::NormalFunctions |
                                                        AbstractMetaClass::AbstractFunctions |
                                                        AbstractMetaClass::NotRemovedFromTargetLang);
                for(AbstractMetaFunction *java_function : java_funcs) {
                    if (java_function->implementingClass() != java_class) {
                        writePrivateNativeFunction(s, java_function);
                    }
                }
            }*/

            if (iterableType) {
                s << INDENT << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public java.util.Iterator<" << translateType(iterableType, java_class, BoxedPrimitive) << "> iterator() {" << Qt::endl
                  << INDENT << "    return begin().iterator();" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << INDENT << Qt::endl;
            }

            // Field accessors
            AbstractMetaFieldList fields = java_class->fields();
            AbstractMetaFieldList nonPublicFields;
            for(AbstractMetaField *field : fields) {
                if (field->wasPublic() || (!java_class->isInterface() && field->wasProtected() && !java_class->isFinal())) {
                    writeFieldAccessors(s, field);
                }else{
                    FieldModification mod = java_class->typeEntry()->fieldModification(field->name());
                    // Set function
                    if (mod.isWritable() || mod.isReadable())
                        nonPublicFields << field;
                }
            }

            // Add dummy constructor for use when constructing subclasses
            if (!isInterface && !java_class->isNamespace() && !fakeClass) {
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * Constructor for internal use only." << Qt::endl
                  << INDENT << " * @param p expected to be <code>null</code>." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@io.qt.NativeAccess" << Qt::endl
                  << INDENT << "protected "<< java_class->simpleName() << "(QPrivateConstructor p) { super(p); } " << Qt::endl
                  << INDENT << Qt::endl;
                if(java_class->isQObject() && java_class->hasStandardConstructor() && !java_class->hasUnimplmentablePureVirtualFunction()){
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * Constructor for internal use only." << Qt::endl
                      << INDENT << " * It is not allowed to call the declarative constructor from inside Java." << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "@io.qt.NativeAccess" << Qt::endl
                      << INDENT << "protected " << java_class->simpleName() << "(QDeclarativeConstructor constructor) {" << Qt::endl;
                    {
                        INDENTATION(INDENT)
                        s << INDENT << "super((QPrivateConstructor)null);" << Qt::endl;
                        s << INDENT << "initialize_native(this, constructor);" << Qt::endl;
                    }
                    s << INDENT << "} " << Qt::endl
                      << INDENT << Qt::endl
                      << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "private static native void initialize_native(" << java_class->simpleName() << " instance, QDeclarativeConstructor constructor);" << Qt::endl
                      << INDENT << Qt::endl;
                }
            }

            if (!java_class->isInterface()) {
                writeJavaLangObjectOverrideFunctions(s, java_class);
            }
            writeExtraFunctions(s, java_class);
            if (!java_class->isInterface()) {
                writeToStringFunction(s, java_class);
            }

            if (java_class->hasCloneOperator() && !alreadyHasCloneMethod) {
                writeCloneFunction(s, java_class);
            }

            if (java_class->isInterface()) {
                const InterfaceTypeEntry* itype = static_cast<const InterfaceTypeEntry*>(java_class->typeEntry());
                AbstractMetaFunctionList nonPublicFunctions;
                AbstractMetaFunctionList restrictedFunctions;
                for(AbstractMetaFunction *java_function : java_funcs) {
                    if(java_function->declaringClass()==java_class){
                        if(!java_function->isStatic()
                                && !java_function->isConstructor()
                                && !java_function->isAbstract()
                                && !java_function->isPrivate()
                                && !java_function->isPublic()
                                && !java_function->isRemovedFrom(java_class, TypeSystem::TargetLangCode)){
                            restrictedFunctions << java_function;
                        }
                        if(!java_function->isPublic() && !java_function->isConstructor())
                            nonPublicFunctions << java_function;
                    }
                }

                {
                    if(!nonPublicFields.isEmpty() || !nonPublicFunctions.isEmpty()){
                        if((nonPublicFunctions.size() + nonPublicFields.size()) == 1){
                            commentStream << "<p>Following function is protected in the Qt interface, all implementations of this interface may implement this function:</p>";
                        }else{
                            commentStream << "<p>Following functions are protected in the Qt interface, all implementations of this interface may implement these function:</p>";
                        }
                        INDENTATIONRESET(INDENT)
                        QString comment2;
                        QTextStream commentStream2(&comment2);
                        for (AbstractMetaFunction *function : nonPublicFunctions) {
                            writeFunction(commentStream2, function, 0, 0, Option(InFunctionComment | NoSuppressExports));
                        }
                        for(const AbstractMetaField *field : nonPublicFields) {
                            writeFieldAccessors(commentStream2, field, Option(InFunctionComment | NoSuppressExports));
                        }
                        commentStream2.seek(0);
                        commentStream << Qt::endl
                                      << "<br>" << Qt::endl
                                      << "<code>" << Qt::endl;
                        while(!commentStream2.atEnd()){
                            QString line = commentStream2.readLine()
                                    .replace("&", "&amp;")
                                    .replace("<", "&lt;")
                                    .replace(">", "&gt;")
                                    .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                                    .replace("@", "&commat;")
                                    .replace("/*", "&sol;*")
                                    .replace("*/", "*&sol;");
                            int count = 0;
                            while(line.startsWith(" ")){
                                line = line.mid(1);
                                ++count;
                            }
                            for(int i=0; i<count; ++i){
                                line = "&nbsp;"+line;
                            }
                            commentStream << line << "<br>" << Qt::endl;
                        }
                        commentStream << "</code>" << Qt::endl;
                    }
                    if(!restrictedFunctions.isEmpty()){
                        QString targetLangName = java_class->typeEntry()->targetLangName().replace('$', '.');
                        s << INDENT << "public static class MemberAccess extends io.qt.MemberAccess<" << targetLangName << "> {" << Qt::endl;
                        {
                            INDENTATION(INDENT)
                            s << INDENT << "private MemberAccess(" << targetLangName << " instance){" << Qt::endl
                              << INDENT << "    super(instance);" << Qt::endl
                              << INDENT << "}" << Qt::endl
                              << INDENT << Qt::endl;
                            for(AbstractMetaFunction *java_function : restrictedFunctions) {
                                if(java_function->isPublic()){
                                    *java_function -= AbstractMetaAttributes::Public;
                                    *java_function += AbstractMetaAttributes::Private;
                                }else{
                                    *java_function += AbstractMetaAttributes::Public;
                                }
                                *java_function -= AbstractMetaAttributes::Native;
                                *java_function -= AbstractMetaAttributes::Abstract;
                                s << functionSignature(java_function, 0, 0, Option(NoOption))
                                  << "{" << Qt::endl;
                                {
                                    INDENTATION(INDENT)
                                    s << INDENT << targetLangName << " instance = instance();" << Qt::endl;

                                    const AbstractMetaArgumentList& arguments = java_function->arguments();

                                    for (int i = 0; i < arguments.size(); ++i) {
                                        if (java_function->nullPointersDisabled(java_function->implementingClass(), i + 1)) {
                                            s << INDENT << "if (" << arguments.at(i)->modifiedArgumentName() << " == null)" << Qt::endl
                                              << INDENT << "    throw new NullPointerException(\"Argument '" << arguments.at(i)->modifiedArgumentName() << "': null not expected.\");" << Qt::endl;
                                        }
                                    }

                                    bool has_argument_referenceCounts = false;
                                    QMap<int,QList<ReferenceCount>> referenceCounts;
                                    for (int i = -1; i <= arguments.size(); ++i) {
                                        referenceCounts[i] = java_function->referenceCounts(java_function->implementingClass(), i);
                                        if (referenceCounts[i].size() > 0) {
                                            for(const ReferenceCount& refCount : referenceCounts[i]) {
                                                // We just want to know this to secure return value into local variable
                                                // to hold over ReferenceCount management later on.
                                                if (refCount.action != ReferenceCount::Ignore) {
                                                    // Something active have been specified
                                                    has_argument_referenceCounts = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                    {
                                        QString injectedCode;
                                        QTextStream _s(&injectedCode);
                                        writeInjectedCode(_s, java_function, CodeSnip::Beginning);
                                        writeInjectedCode(_s, java_function, CodeSnip::Position1);
                                        writeInjectedCode(_s, java_function, CodeSnip::Position2);
                                        s << injectedCode.replace("this", "instance");
                                    }

                                    // Lookup if there is a reference-count action required on the return value.
                                    AbstractMetaType *return_type = java_function->type();
                                    QString new_return_type = QString(java_function->typeReplaced(0)).replace('$', '.');
                                    bool has_return_type = new_return_type != "void"
                                    && (!new_return_type.isEmpty() || return_type);
                                    OwnershipRule ownershipRule = java_function->ownership(java_function->implementingClass(), TypeSystem::TargetLangCode, 0);

                                    bool has_code_injections_at_the_end = hasCodeInjections(java_function, {CodeSnip::End, CodeSnip::Position4, CodeSnip::Position3});

                                    bool needs_return_variable = has_return_type
                                                                 && ( ( ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership)
                                                                      || has_argument_referenceCounts || referenceCounts[0].size() > 0 || has_code_injections_at_the_end);

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

                                        if (return_type && new_return_type.isEmpty()) {
                                            if (return_type->isTargetLangEnum()) {
                                                s << static_cast<const EnumTypeEntry *>(return_type->typeEntry())->qualifiedTargetLangName() << ".resolve(";
                                            } else if (return_type->isTargetLangFlags()) {
                                                s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace('$', '.') << "(";
                                            }
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
                                        s << itype->origin()->targetLangName().replace('$', '.')
                                          << "." << java_function->marshalledName() << "(";
                                    }
                                    if(java_class->typeEntry()->isNativeIdBased()){
                                        s << "QtJambi_LibraryUtilities.internal.nativeId(instance)";
                                    }else{
                                        s << "instance";
                                    }

                                    bool needsComma = false;
                                    for (int i = 0; i < arguments.count(); ++i) {
                                        const AbstractMetaArgument *arg = arguments.at(i);
                                        const AbstractMetaType *type = arg->type();

                                        if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                                            if (needsComma || (!java_function->isStatic() && !java_function->isConstructor()))
                                                s << ", ";
                                            needsComma = true;

                                            if(!java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()){
                                                s << arg->modifiedArgumentName();
                                            }else if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                                                s << arg->modifiedArgumentName() << ".value()";
                                            } else if (type->hasNativeId()) {
                                                s << "QtJambi_LibraryUtilities.internal.checkedNativeId(" << arg->modifiedArgumentName() << ")";
                                            } else {
                                                s << arg->modifiedArgumentName();
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
                                    if (return_type && new_return_type.isEmpty() && (return_type->isTargetLangEnum() || return_type->isTargetLangFlags()))
                                        s << ")";

                                    s << ";" << Qt::endl;

                                    for(ReferenceCount refCount : referenceCounts[-1]){
                                        refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace('$', '.');
                                        writeReferenceCount(s, refCount, -1, java_function, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
                                    }

                                    // We must ensure we retain a Java hard-reference over the native method call
                                    // so that the GC will not destroy the C++ object too early.  At this point we
                                    // have called the native method call so can manage referenceCount issues.
                                    // First the input arguments
                                    for (const AbstractMetaArgument* argument : arguments) {
                                        for(ReferenceCount refCount : referenceCounts[argument->argumentIndex()+1]){
                                            refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace('$', '.');
                                            writeReferenceCount(s, refCount, argument->argumentIndex()+1, java_function, QLatin1String("instance"));
                                        }
                                    }

                                    if (!java_function->argumentReplaced(0).isEmpty()) {
                                        s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
                                    }else{
                                        // Then the return value
                                        for(ReferenceCount referenceCount : referenceCounts[0]) {
                                            referenceCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace('$', '.');
                                            writeReferenceCount(s, referenceCount, 0, java_function, QLatin1String("instance"));
                                        }

                                        {
                                            QString injectedCode;
                                            QTextStream _s(&injectedCode);
                                            writeInjectedCode(_s, java_function, CodeSnip::Position3);
                                            writeInjectedCode(_s, java_function, CodeSnip::Position4);
                                            writeInjectedCode(_s, java_function, CodeSnip::End);
                                            s << injectedCode.replace("this", "instance");
                                        }

                                        if (needs_return_variable) {
                                            if (ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership) {
                                                if (return_type->isContainer()){
                                                    s << INDENT << "if (__qt_return_value != null";
                                                    if(!ownershipRule.condition.isEmpty()){
                                                        s << " && " << ownershipRule.condition;
                                                    }
                                                    s << ") {" << Qt::endl;
                                                    {
                                                        INDENTATION(INDENT)
                                                        writeOwnershipForContainer(s, java_function, ownershipRule.ownership, return_type, "__qt_return_value");
                                                    }
                                                    s << INDENT << "}" << Qt::endl;
                                                }else if(!ownershipRule.condition.isEmpty()){
                                                    s << INDENT << "if (" << ownershipRule.condition << ") {" << Qt::endl;
                                                    {
                                                        INDENTATION(INDENT)
                                                        writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, "__qt_return_value");
                                                    }
                                                    s << INDENT << "}" << Qt::endl;
                                                }else{
                                                    writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, "__qt_return_value");
                                                }
                                            }
                                            s << INDENT << "return __qt_return_value;" << Qt::endl;
                                        }
                                    }
                                }
                                s << INDENT << "}" << Qt::endl
                                  << INDENT << Qt::endl;
                            }
                            s << INDENT << "public static MemberAccess of(" << targetLangName << " instance){" << Qt::endl
                              << INDENT << "    Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();" << Qt::endl
                              << INDENT << "    if(!" << targetLangName << ".class.isAssignableFrom(callerClass) || !callerClass.isInstance(instance))" << Qt::endl
                              << INDENT << "        throw new RuntimeException(\"Access to restricted functions of class " << targetLangName << " is only granted from inside a user-implemented subclass.\");" << Qt::endl
                              << INDENT << "    return QtJambi_LibraryUtilities.internal.findMemberAccess(instance, " << targetLangName << ".class, MemberAccess.class);" << Qt::endl // MemberAccess accessWrapper = \\, MemberAccess::new
                              << INDENT << "}" << Qt::endl;
                        }
                        s << INDENT << "}" << Qt::endl;
                    }
                }
            }else if(!privatePureVirtualFunctions.isEmpty()){
                QStringList privatePureVirtualFunctionSignatures;
                for(AbstractMetaFunction *function : privatePureVirtualFunctions){
                    QString sig = functionSignature(function, 0, AbstractMetaAttributes::Native | AbstractMetaAttributes::Abstract, Option(InFunctionComment | NoSuppressExports));
                    if(!privatePureVirtualFunctionSignatures.contains(sig))
                        privatePureVirtualFunctionSignatures << sig;
                }
                if(privatePureVirtualFunctionSignatures.size()==1)
                    commentStream << "<p>The following private function is pure virtual in Qt and thus has to " << Qt::endl
                      << "be implemented in derived Java classes by using the &commat;QtPrivateOverride annotation:</p>" << Qt::endl;
                else
                    commentStream << "<p>The following private functions are pure virtual in Qt and thus have to " << Qt::endl
                      << "be implemented in derived Java classes by using the &commat;QtPrivateOverride annotation:</p>" << Qt::endl;
                INDENTATIONRESET(INDENT)
                QString comment2;
                QTextStream commentStream2(&comment2);
                for(const QString& signature : privatePureVirtualFunctionSignatures){
                    commentStream2 << Qt::endl
                                   << "@io.qt.QtPrivateOverride" << Qt::endl
                                   << signature << " {...}" << Qt::endl;
                }
                commentStream2.seek(0);
                commentStream << "<code>" << Qt::endl;
                while(!commentStream2.atEnd()){
                    QString line = commentStream2.readLine()
                            .replace("&", "&amp;")
                            .replace("<", "&lt;")
                            .replace(">", "&gt;")
                            .replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
                            .replace("@", "&commat;")
                            .replace("/*", "&sol;*")
                            .replace("*/", "*&sol;");
                    int count = 0;
                    while(line.startsWith(" ")){
                        line = line.mid(1);
                        ++count;
                    }
                    for(int i=0; i<count; ++i){
                        line = "&nbsp;"+line;
                    }
                    commentStream << line << "<br>" << Qt::endl;
                }
                commentStream << "</code>" << Qt::endl;
            }
        }

        s << INDENT << "}" << Qt::endl;
    }

    if(nesting_level==0){
        /*if(m_current_class_needs_internal_import){
            s << INDENT << "import static io.qt.internal.QtJambiInternal.*;" << Qt::endl
              << INDENT << Qt::endl;
        }else */
        if(!java_class->typeEntry()->extraIncludes().isEmpty()){
            s << INDENT << Qt::endl;
        }
    }
    if(!comment.trimmed().isEmpty()){
        s << INDENT << "/**" << Qt::endl;
        QTextStream commentStream(&comment, QIODevice::ReadOnly);
        while(!commentStream.atEnd()){
            s << INDENT << " * " << commentStream.readLine() << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
    }
    if(nesting_level==0){
        m_packagesByTypeSystem[TypeDatabase::instance()->findTypeSystem(java_class->targetTypeSystem())].append(m_currentPackages);
    }

    s << lines;

    if (m_docs_enabled) {
        delete m_doc_parser;
        m_doc_parser = nullptr;
    }
}

void JavaGenerator::writeIteratorFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    //const IteratorTypeEntry* iteratorTypeEntry = reinterpret_cast<const IteratorTypeEntry*>(java_class->templateBaseClass()->typeEntry());
    if(java_class->templateBaseClassInstantiations().size()==1
            && java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
        QString type = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        s << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "protected " << type << " value(){" << Qt::endl
          << INDENT << "    return _value();" << Qt::endl
          << INDENT << "}" << Qt::endl;
    }else if(java_class->templateBaseClassInstantiations().size()==2){
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
             QString type = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
             s << Qt::endl
               << INDENT << "@Override" << Qt::endl
               << INDENT << "@io.qt.QtUninvokable" << Qt::endl
               << INDENT << "protected " << type << " key(){" << Qt::endl
               << INDENT << "    return _key();" << Qt::endl
               << INDENT << "}" << Qt::endl;
        }
        if(java_class->templateBaseClassInstantiations().at(1)->typeEntry()->isPrimitive()){
             QString type = translateType(java_class->templateBaseClassInstantiations().at(1), java_class, BoxedPrimitive);
             s << Qt::endl
               << INDENT << "@Override" << Qt::endl
               << INDENT << "@io.qt.QtUninvokable" << Qt::endl
               << INDENT << "protected " << type << " value(){" << Qt::endl
               << INDENT << "    return _value();" << Qt::endl
               << INDENT << "}" << Qt::endl;
        }
    }
}

void JavaGenerator::writeCollectionFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>0){
        QString boxedType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
            QString primitiveType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean contains(Object e){" << Qt::endl
              << INDENT << "    return e instanceof " << boxedType << " ? contains((" << primitiveType << ")e) : false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }else{
            if(boxedType!="java.lang.Object"){
                s << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean contains(Object e){" << Qt::endl
                  << INDENT << "    return e instanceof " << boxedType << " ? contains((" << boxedType << ")e) : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }
        }
    }
}

void JavaGenerator::writeLinkedListFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>0){
        QString boxedType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
            QString primitiveType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s
                << INDENT << "@Override" << Qt::endl
                << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                << INDENT << "public boolean add(" << boxedType << " e){" << Qt::endl
                << INDENT << "    if(e!=null){" << Qt::endl
                << INDENT << "        append((" << primitiveType << ")e);" << Qt::endl
                << INDENT << "        return true;" << Qt::endl
                << INDENT << "    }else" << Qt::endl
                << INDENT << "        return false;" << Qt::endl
                << INDENT << "}" << Qt::endl
                << Qt::endl
                << INDENT << "@Override" << Qt::endl
                << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                << INDENT << "public boolean remove(Object e){" << Qt::endl
                << INDENT << "    return e instanceof " << boxedType << " ? removeOne((" << primitiveType << ")e) : false;" << Qt::endl
                << INDENT << "}" << Qt::endl
                << Qt::endl;
        }else{
            s
                << INDENT << "@Override" << Qt::endl
                << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                << INDENT << "public boolean add(" << boxedType << " e){" << Qt::endl
                << INDENT << "    if(e!=null){" << Qt::endl
                << INDENT << "        append(e);" << Qt::endl
                << INDENT << "        return true;" << Qt::endl
                << INDENT << "    }else" << Qt::endl
                << INDENT << "        return false;" << Qt::endl
                << INDENT << "}" << Qt::endl
                << Qt::endl
                << INDENT << "@Override" << Qt::endl
                << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                << INDENT << "public boolean remove(Object e){" << Qt::endl
                << INDENT << "    return e instanceof " << boxedType << " ? removeOne((" << boxedType << ")e) : false;" << Qt::endl
                << INDENT << "}" << Qt::endl
                << Qt::endl;
        }
        writeCollectionFunctions(s, java_class);
    }
}

void JavaGenerator::writeAbstractListFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>0){
        QString boxedType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
            QString primitiveType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s
                << INDENT << "@Override" << Qt::endl
                << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                << INDENT << "public boolean add(" << boxedType << " e){" << Qt::endl
                << INDENT << "    if(e!=null){" << Qt::endl
                << INDENT << "        append((" << primitiveType << ")e);" << Qt::endl
                << INDENT << "        return true;" << Qt::endl
                << INDENT << "    }else" << Qt::endl
                << INDENT << "        return false;" << Qt::endl
                << INDENT << "}" << Qt::endl
                << Qt::endl
                << INDENT << "@Override" << Qt::endl
                << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                << INDENT << "public boolean remove(Object e){" << Qt::endl
                << INDENT << "    return e instanceof " << boxedType << " ? removeOne((" << primitiveType << ")e) : false;" << Qt::endl
                << INDENT << "}" << Qt::endl
                << Qt::endl
                << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public void add(int index, " << boxedType << " e){" << Qt::endl
              << INDENT << "    if(e!=null)" << Qt::endl
              << INDENT << "        insert(index, (" << primitiveType << ")e);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedType << " set(int index, " << boxedType << " e){" << Qt::endl
              << INDENT << "    if(e!=null){" << Qt::endl
              << INDENT << "        " << boxedType << " el = get(index);" << Qt::endl
              << INDENT << "        replace(index, (" << primitiveType << ")e);" << Qt::endl
              << INDENT << "        return el;" << Qt::endl
              << INDENT << "    } else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public int indexOf(Object e){" << Qt::endl
              << INDENT << "    return e instanceof " << boxedType << " ? ";
            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
                s << "(int)longIndexOf";
            }else{
                s << "indexOf";
            }
            s << "((" << primitiveType << ")e) : -1;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public int lastIndexOf(Object e){" << Qt::endl
              << INDENT << "    return e instanceof " << boxedType << " ? ";
            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
                s << "(int)lastLongIndexOf";
            }else{
                s << "lastIndexOf";
            }
            s << "((" << primitiveType << ")e) : -1;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << Qt::endl;
        }else{
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean add(" << boxedType << " e){" << Qt::endl
              << INDENT << "    append(e);" << Qt::endl
              << INDENT << "    return true;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public void add(int index, " << boxedType << " e){" << Qt::endl
              << INDENT << "        insert(index, e);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedType << " set(int index, " << boxedType << " e){" << Qt::endl
              << INDENT << "    if(e!=null){" << Qt::endl
              << INDENT << "        " << boxedType << " el = get(index);" << Qt::endl
              << INDENT << "        replace(index, e);" << Qt::endl
              << INDENT << "        return el;" << Qt::endl
              << INDENT << "    } else return null;" << Qt::endl
              << INDENT << "}"
              << Qt::endl;
            if(boxedType!="java.lang.Object"){
                s << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean remove(Object e){" << Qt::endl
                  << INDENT << "    return e instanceof " << boxedType << " ? removeOne((" << boxedType << ")e) : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public int indexOf(Object e){" << Qt::endl
                  << INDENT << "    return e instanceof " << boxedType << " ? ";
                if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
                    s << "(int)longIndexOf";
                }else{
                    s << "indexOf";
                }
                s << "((" << boxedType << ")e) : -1;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public int lastIndexOf(Object e){" << Qt::endl
                  << INDENT << "    return e instanceof " << boxedType << " ? ";
                if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
                    s << "(int)lastLongIndexOf";
                }else{
                    s << "lastIndexOf";
                }
                s << "((" << boxedType << ")e) : -1;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }else if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
                s << INDENT << "public int indexOf(Object e){" << Qt::endl
                  << INDENT << "    return (int)longIndexOf(e);" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public int lastIndexOf(Object e){" << Qt::endl
                  << INDENT << "    return (int)lastLongIndexOf(e);" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }
        }
        s << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "public " << boxedType << " get(int index) {" << Qt::endl
          << INDENT << "    return at(index);" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final int size(){" << Qt::endl
              << INDENT << "    long size = longSize();" << Qt::endl
              << INDENT << "    if(size>Integer.MAX_VALUE)" << Qt::endl
              << INDENT << "        throw new RuntimeException(\"Size exceeds maximum int value: \"+size+\"Use longSize() instead.\");" << Qt::endl
              << INDENT << "    return (int)size;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }
        s << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "protected final void removeAtIndex(int cursor){" << Qt::endl
          << INDENT << "    removeAt(cursor);" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        writeCollectionFunctions(s, java_class);
    }
}

void JavaGenerator::writeQueueFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>0){
        QString boxedType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
            QString primitiveType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public void enqueue(" << boxedType << " e){" << Qt::endl
              << INDENT << "    if(e!=null)" << Qt::endl
              << INDENT << "        enqueue((" << primitiveType << ")e);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }else{
        }
        s << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "public " << boxedType << " peek() {" << Qt::endl
          << INDENT << "    return head();" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "public " << boxedType << " poll() {" << Qt::endl
          << INDENT << "    return dequeue();" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        writeListFunctions(s, java_class);
    }
}

void JavaGenerator::writeListFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    writeAbstractListFunctions(s, java_class);
}

void JavaGenerator::writeSetFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>0){
        QString boxedType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
            QString primitiveType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean add(" << boxedType << " e){" << Qt::endl
              << INDENT << "    if(e!=null){" << Qt::endl
              << INDENT << "        insert((" << primitiveType << ")e);" << Qt::endl
              << INDENT << "        return true;" << Qt::endl
              << INDENT << "    }else" << Qt::endl
              << INDENT << "        return false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean remove(Object e){" << Qt::endl
              << INDENT << "    return e instanceof " << boxedType << " ? remove((" << primitiveType << ")e) : false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }else{
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean add(" << boxedType << " e){" << Qt::endl
              << INDENT << "    insert(e);" << Qt::endl
              << INDENT << "    return true;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
            if(boxedType!="java.lang.Object"){
                s << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean remove(Object e){" << Qt::endl
                  << INDENT << "    return e instanceof " << boxedType << " ? remove((" << boxedType << ")e) : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }
        }
        writeCollectionFunctions(s, java_class);
    }
}

void JavaGenerator::writeVectorFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    writeAbstractListFunctions(s, java_class);
}

void JavaGenerator::writeStackFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>0){
        QString boxedType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->typeEntry()->isPrimitive()){
            QString primitiveType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedType << " pop(){" << Qt::endl
              << INDENT << "    return _pop();" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public void push(" << boxedType << " e){" << Qt::endl
              << INDENT << "    if(e!=null)" << Qt::endl
              << INDENT << "        push((" << primitiveType << ")e);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }else{
        }
        s << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "public " << boxedType << " peekLast(){" << Qt::endl
          << INDENT << "    return last();" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "public " << boxedType << " peekFirst(){" << Qt::endl
          << INDENT << "    return first();" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        writeVectorFunctions(s, java_class);
    }
}

void JavaGenerator::writeMapFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>1){
        QString boxedKeyType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        QString boxedValueType = translateType(java_class->templateBaseClassInstantiations().at(1), java_class, BoxedPrimitive);
        s << INDENT << "@Override" << Qt::endl
          << INDENT << "public java.util.Comparator<" << boxedKeyType << "> comparator(){" << Qt::endl
          << INDENT << "    return (o1,o2)->lessThan(o1, o2) ? -1 : (lessThan(o2, o1) ? 1 : 0);" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        if(java_class->templateBaseClassInstantiations().at(0)->isPrimitive()){
            QString primitiveKeyType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << INDENT << "private static native boolean lessThan(" << primitiveKeyType << " key1, " << primitiveKeyType << " key2);" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final " << boxedKeyType << " firstKey(){" << Qt::endl
              << INDENT << "    return _firstKey();" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final " << boxedKeyType << " lastKey(){" << Qt::endl
              << INDENT << "    return _lastKey();" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "protected final io.qt.core.QMapIterator<" << boxedKeyType << ", " << boxedValueType << "> find(" << boxedKeyType << " key){" << Qt::endl
              << INDENT << "    return find((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
            << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "protected final io.qt.core.QMapIterator<" << boxedKeyType << ", " << boxedValueType << "> lowerBound(" << boxedKeyType << " key){" << Qt::endl
              << INDENT << "    return lowerBound((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "protected final io.qt.core.QMapIterator<" << boxedKeyType << ", " << boxedValueType << "> upperBound(" << boxedKeyType << " key){" << Qt::endl
              << INDENT << "    return upperBound((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }else{
            s << INDENT << "private static native boolean lessThan(" << boxedKeyType << " key1, " << boxedKeyType << " key2);" << Qt::endl
              << Qt::endl;
        }
        writeAbstractMapFunctions(s, java_class);
    }
}

void JavaGenerator::writeHashFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    writeAbstractMapFunctions(s, java_class);
}

void JavaGenerator::writeMultiHashFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    writeAbstractMultiMapFunctions(s, java_class);
}

void JavaGenerator::writeMultiMapFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>1){
        QString boxedKeyType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        s << INDENT << "@Override" << Qt::endl
          << INDENT << "public java.util.Comparator<" << boxedKeyType << "> comparator(){" << Qt::endl
          << INDENT << "    return (o1,o2)->lessThan(o1, o2) ? -1 : (lessThan(o2, o1) ? 1 : 0);" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        if(java_class->templateBaseClassInstantiations().at(0)->isPrimitive()){
            QString primitiveKeyType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            QString boxedValueType = translateType(java_class->templateBaseClassInstantiations().at(1), java_class, BoxedPrimitive);
            s << INDENT << "private static native boolean lessThan(" << primitiveKeyType << " key1, " << primitiveKeyType << " key2);" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final " << boxedKeyType << " firstKey(){" << Qt::endl
              << INDENT << "    return _firstKey();" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final " << boxedKeyType << " lastKey(){" << Qt::endl
              << INDENT << "    return _lastKey();" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "protected final io.qt.core.QMapIterator<" << boxedKeyType << ", " << boxedValueType << "> find(" << boxedKeyType << " key){" << Qt::endl
              << INDENT << "    return find((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
            << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "protected final io.qt.core.QMapIterator<" << boxedKeyType << ", " << boxedValueType << "> lowerBound(" << boxedKeyType << " key){" << Qt::endl
              << INDENT << "    return lowerBound((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "protected final io.qt.core.QMapIterator<" << boxedKeyType << ", " << boxedValueType << "> upperBound(" << boxedKeyType << " key){" << Qt::endl
              << INDENT << "    return upperBound((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }else{
            s << INDENT << "private static native boolean lessThan(" << boxedKeyType << " key1, " << boxedKeyType << " key2);" << Qt::endl
              << Qt::endl;
        }
        writeAbstractMultiMapFunctions(s, java_class);
    }
}

void JavaGenerator::writeAbstractMapFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>1){
        QString boxedKeyType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        QString boxedValueType = translateType(java_class->templateBaseClassInstantiations().at(1), java_class, BoxedPrimitive);
        if(java_class->templateBaseClassInstantiations().at(0)->isPrimitive()){
            QString primitiveKeyType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean containsKey(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " ? contains((" << primitiveKeyType << ")key) : false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedValueType << " get(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " ? value((" << primitiveKeyType << ")key) : null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedValueType << " put(" << boxedKeyType << " key, " << boxedValueType << " value){" << Qt::endl
              << INDENT << "    if(key!=null){" << Qt::endl
              << INDENT << "        " << boxedValueType << " old = value((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "        insert(key, value);" << Qt::endl
              << INDENT << "        return old;" << Qt::endl
              << INDENT << "    }else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedValueType << " remove(Object key){" << Qt::endl
              << INDENT << "    if(key instanceof " << boxedKeyType << ")" << Qt::endl
              << INDENT << "        return take((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "    else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
            if(java_class->templateBaseClassInstantiations().at(1)->isPrimitive()){
                QString primitiveValueType = java_class->templateBaseClassInstantiations().at(1)->typeEntry()->targetLangName();
                s << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                  << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << primitiveValueType << ")value).isEmpty() : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }else{
                s << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                  << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << boxedValueType << ")value).isEmpty() : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }
        }else{
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean containsKey(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " || key==null ? contains((" << boxedKeyType << ")key) : false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedValueType << " get(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " ? value((" << boxedKeyType << ")key) : null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedValueType << " put(" << boxedKeyType << " key, " << boxedValueType << " value){" << Qt::endl
              << INDENT << "    " << boxedValueType << " old = value(key);" << Qt::endl
              << INDENT << "    insert(key, value);" << Qt::endl
              << INDENT << "    return old;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public " << boxedValueType << " remove(Object key){" << Qt::endl
              << INDENT << "    if(key instanceof " << boxedKeyType << " || key==null)" << Qt::endl
              << INDENT << "        return take((" << boxedKeyType << ")key);" << Qt::endl
              << INDENT << "    else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
            if(java_class->templateBaseClassInstantiations().at(1)->isPrimitive()){
                QString primitiveValueType = java_class->templateBaseClassInstantiations().at(1)->typeEntry()->targetLangName();
                s << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                  << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << primitiveValueType << ")value).isEmpty() : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }else{
                s << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                  << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << boxedValueType << ")value).isEmpty() : false;" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << Qt::endl;
            }
        }
        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final int size(){" << Qt::endl
              << INDENT << "    long size = longSize();" << Qt::endl
              << INDENT << "    if(size>Integer.MAX_VALUE)" << Qt::endl
              << INDENT << "        throw new RuntimeException(\"Size exceeds maximum int value: \"+size+\"Use longSize() instead.\");" << Qt::endl
              << INDENT << "    return (int)size;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }
    }
}

void JavaGenerator::writeAbstractMultiMapFunctions(QTextStream &s, const AbstractMetaClass *java_class){
    if(java_class->templateBaseClassInstantiations().size()>1){
        QString boxedKeyType = translateType(java_class->templateBaseClassInstantiations().at(0), java_class, BoxedPrimitive);
        QString boxedValueType = translateType(java_class->templateBaseClassInstantiations().at(1), java_class, BoxedPrimitive);
        s << INDENT << "@Override" << Qt::endl
          << INDENT << "@io.qt.QtUninvokable" << Qt::endl
          << INDENT << "public java.util.Collection<java.util.List<" << boxedValueType << ">> values() {" << Qt::endl
          << INDENT << "    java.util.List<java.util.List<" << boxedValueType << ">> result = new java.util.ArrayList<>();" << Qt::endl
          << INDENT << "    for(" << boxedKeyType << " key : keys()) {" << Qt::endl
          << INDENT << "        result.add(values(key));" << Qt::endl
          << INDENT << "    }" << Qt::endl
          << INDENT << "    return result;" << Qt::endl
          << INDENT << "}" << Qt::endl
          << Qt::endl;
        if(java_class->templateBaseClassInstantiations().at(0)->isPrimitive()){
            QString primitiveKeyType = java_class->templateBaseClassInstantiations().at(0)->typeEntry()->targetLangName();
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean containsKey(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " ? contains((" << primitiveKeyType << ")key) : false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public java.util.List<" << boxedValueType << "> get(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " ? values((" << primitiveKeyType << ")key) : null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public java.util.List<" << boxedValueType << "> put(" << boxedKeyType << " key, java.util.List<" << boxedValueType << "> values){" << Qt::endl
              << INDENT << "    if(key!=null){" << Qt::endl
              << INDENT << "        java.util.List<" << boxedValueType << "> old = values((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "        for(" << boxedValueType << " value : values)" << Qt::endl
              << INDENT << "            insert(key, value);" << Qt::endl
              << INDENT << "        return old;" << Qt::endl
              << INDENT << "    }else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public java.util.List<" << boxedValueType << "> remove(Object key){" << Qt::endl
              << INDENT << "    if(key instanceof " << boxedKeyType << "){" << Qt::endl
              << INDENT << "        java.util.List<" << boxedValueType << "> old = values((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "        take((" << primitiveKeyType << ")key);" << Qt::endl
              << INDENT << "        return old;" << Qt::endl
              << INDENT << "    }else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6,0,0)){
                if(java_class->templateBaseClassInstantiations().at(1)->isPrimitive()){
                    QString primitiveValueType = java_class->templateBaseClassInstantiations().at(1)->typeEntry()->targetLangName();
                    s << INDENT << "@Override" << Qt::endl
                      << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                      << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << primitiveValueType << ")value).isEmpty() : false;" << Qt::endl
                      << INDENT << "}" << Qt::endl
                      << Qt::endl;
                }else{
                    s << INDENT << "@Override" << Qt::endl
                      << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                      << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << boxedValueType << ")value).isEmpty() : false;" << Qt::endl
                      << INDENT << "}" << Qt::endl
                      << Qt::endl;
                }
            }
        }else{
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean containsKey(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " || key==null ? contains((" << boxedKeyType << ")key) : false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public java.util.List<" << boxedValueType << "> get(Object key){" << Qt::endl
              << INDENT << "    return key instanceof " << boxedKeyType << " ? values((" << boxedKeyType << ")key) : null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public java.util.List<" << boxedValueType << "> put(" << boxedKeyType << " key, java.util.List<" << boxedValueType << "> values){" << Qt::endl
              << INDENT << "    java.util.List<" << boxedValueType << "> old = values(key);" << Qt::endl
              << INDENT << "    for(" << boxedValueType << " value : values)" << Qt::endl
              << INDENT << "        insert(key, value);" << Qt::endl
              << INDENT << "    return old;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public java.util.List<" << boxedValueType << "> remove(Object key){" << Qt::endl
              << INDENT << "    if(key instanceof " << boxedKeyType << " || key==null){" << Qt::endl
              << INDENT << "        java.util.List<" << boxedValueType << "> old = values((" << boxedKeyType << ")key);" << Qt::endl
              << INDENT << "        take((" << boxedKeyType << ")key);" << Qt::endl
              << INDENT << "        return old;" << Qt::endl
              << INDENT << "    }else return null;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) < QT_VERSION_CHECK(6,0,0)){
                if(java_class->templateBaseClassInstantiations().at(1)->isPrimitive()){
                    QString primitiveValueType = java_class->templateBaseClassInstantiations().at(1)->typeEntry()->targetLangName();
                    s << INDENT << "@Override" << Qt::endl
                      << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                      << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << primitiveValueType << ")value).isEmpty() : false;" << Qt::endl
                      << INDENT << "}" << Qt::endl
                      << Qt::endl;
                }else{
                    s << INDENT << "@Override" << Qt::endl
                      << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                      << INDENT << "public boolean containsValue(Object value){" << Qt::endl
                      << INDENT << "    return value instanceof " << boxedValueType << " ? keys((" << boxedValueType << ")value).isEmpty() : false;" << Qt::endl
                      << INDENT << "}" << Qt::endl
                      << Qt::endl;
                }
            }
        }
        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch) >= QT_VERSION_CHECK(6,0,0)){
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public boolean containsValue(Object value){" << Qt::endl
              << INDENT << "    if(value instanceof " << boxedValueType << "){" << Qt::endl
              << INDENT << "        for(" << boxedKeyType << " key : keys()) {" << Qt::endl
              << INDENT << "            if(values(key).contains((" << boxedValueType << ")value))" << Qt::endl
              << INDENT << "                return true;" << Qt::endl
              << INDENT << "        }" << Qt::endl
              << INDENT << "    }" << Qt::endl
              << INDENT << "    return false;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
            s << INDENT << "@Override" << Qt::endl
              << INDENT << "@io.qt.QtUninvokable" << Qt::endl
              << INDENT << "public final int size(){" << Qt::endl
              << INDENT << "    long size = longSize();" << Qt::endl
              << INDENT << "    if(size>Integer.MAX_VALUE)" << Qt::endl
              << INDENT << "        throw new RuntimeException(\"Size exceeds maximum int value: \"+size+\"Use longSize() instead.\");" << Qt::endl
              << INDENT << "    return (int)size;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << Qt::endl;
        }
    }
}

void JavaGenerator::generate() {
    Generator::generate();

    for(TypeSystemTypeEntry * typeSystemEntry : m_packagesByTypeSystem.keys()){
        if(typeSystemEntry){
            m_packagesByTypeSystem[typeSystemEntry].removeDuplicates();
            for(const QString& pkg : m_packagesByTypeSystem[typeSystemEntry]){
                bool isSuppressed = false;
                for(QString forwardDeclaration : typeSystemEntry->forwardDeclarations()){
                    auto idx = forwardDeclaration.lastIndexOf('/');
                    forwardDeclaration = forwardDeclaration.mid(0, idx).replace('/', '.');
                    if(pkg==forwardDeclaration){
                        isSuppressed = true;
                        break;
                    }
                }
                if(!isSuppressed){
                    if(TypeSystemTypeEntry * relatedTypeSystem = m_typeSystemByPackage[pkg]){
                        if(typeSystemEntry->module()!=relatedTypeSystem->module()){
                            if(relatedTypeSystem->requiredTypeSystems().contains(typeSystemEntry))
                                TypesystemException::raise(QString("Cyclic dependency detected between modules %1 and %2.").arg(typeSystemEntry->module(), relatedTypeSystem->module()));
                            typeSystemEntry->addRequiredTypeSystem(relatedTypeSystem);
                        }
                    }
                }
            }
            if(!typeSystemEntry->module().isEmpty()){
                for(QString forwardDeclaration : typeSystemEntry->forwardDeclarations()){
                    FileOut fileOut(javaOutputDirectory() + "/" + typeSystemEntry->module() + "/" + forwardDeclaration + ".java");
                    auto idx = forwardDeclaration.lastIndexOf('/');
                    QString pkg;
                    if(idx > 0){
                        pkg = forwardDeclaration.mid(0, idx);
                        forwardDeclaration = forwardDeclaration.mid(idx + 1);
                    }
                    fileOut.stream << "package " << pkg.replace('/', '.') << ";" << Qt::endl
                                   << "public class " << forwardDeclaration << "{}" << Qt::endl;
                    //QString pkg =
                }
            }
        }
    }

    { //log native pointer api
        QString fileName("mjb_nativepointer_api.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if(m_nativepointer_functions.isEmpty()){
            file.remove();
        }else if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            s << "Number of public or protected functions with QNativePointer API: "
              << m_nativepointer_functions.size() << Qt::endl;
            for(const AbstractMetaFunction *f : m_nativepointer_functions) {
                s << f->implementingClass()->qualifiedCppName() << " :: " << f->minimalSignature();
                if(f->type()){
                    s << " -> ";
                    CppGenerator::writeTypeInfo(s, f->type(), NoOption);
                }
                s << Qt::endl;
            }

            m_nativepointer_functions.clear();
        }
    }

    { // log object type usage of classes
        QString fileName("mjb_resettable_object_functions.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if(m_resettable_object_functions.isEmpty()){
            file.remove();
        }else if (file.open(QFile::WriteOnly)) {
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
            resettable_object_functions.size() << Qt::endl;
            for(const AbstractMetaFunction *f : resettable_object_functions) {
                s << f->implementingClass()->qualifiedCppName() << " :: " << f->minimalSignature();
                if(f->type()){
                    s << " -> ";
                    CppGenerator::writeTypeInfo(s, f->type(), NoOption);
                }
                s << Qt::endl;
            }

            m_resettable_object_functions.clear();
        }
    }

    { // log possible reference counting candidates
        QString fileName("mjb_reference_count_candidates.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if(m_reference_count_candidate_functions.isEmpty()){
            file.remove();
        }else if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            s << "The following functions have a signature pattern which may imply that" << Qt::endl
            << "they need to apply reference counting to their arguments ("
            << m_reference_count_candidate_functions.size() << " functions) : " << Qt::endl;

            for(const AbstractMetaFunction *f : m_reference_count_candidate_functions) {
                s << f->implementingClass()->qualifiedCppName() << " :: " << f->minimalSignature();
                if(f->type()){
                    s << " -> ";
                    CppGenerator::writeTypeInfo(s, f->type(), NoOption);
                }
                s << Qt::endl;
            }
        }
        file.close();
    }

    { // log possible factory candidates
        QString fileName("mjb_factory_functions.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if(m_factory_functions.isEmpty()){
            file.remove();
        }else if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            s << "The following functions have a signature pattern which may imply that" << Qt::endl
            << "they need to apply ownership to their return value and/or need to disable null pointers ("
            << m_factory_functions.size() << " functions) : " << Qt::endl;

            for(const AbstractMetaFunction *f : m_factory_functions) {
                if(!f->isFinalInCpp()){
                    s << "virtual ";
                }
                s << f->implementingClass()->qualifiedCppName() << " :: " << f->minimalSignature();
                if(f->type()){
                    s << " -> ";
                    CppGenerator::writeTypeInfo(s, f->type(), NoOption);
                }
                if(!f->isFinal() && !f->nullPointersDisabled()){

                }
                s << Qt::endl;
            }
        }
        file.close();
    }

    { // log possible factory candidates
        QString fileName("mjb_inconsistent_functions.log");
        QFile file(fileName);
        if (!logOutputDirectory().isNull())
            file.setFileName(QDir(logOutputDirectory()).absoluteFilePath(fileName));
        if(m_inconsistent_functions.isEmpty()){
            file.remove();
        }else if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            s << "The following functions are inconsistent (virtual but declared final in java) ("
            << m_inconsistent_functions.size() << " functions) : " << Qt::endl;

            for(const AbstractMetaFunction *f : m_inconsistent_functions) {
                if(!f->isFinalInCpp()){
                    s << "virtual ";
                }
                s << f->implementingClass()->qualifiedCppName() << " :: " << f->minimalSignature();
                if(f->type()){
                    s << " -> ";
                    CppGenerator::writeTypeInfo(s, f->type(), NoOption);
                }
                s << Qt::endl;
            }
        }
        file.close();
    }
}

void JavaGenerator::writeFunctionAttributes(QTextStream &s, const AbstractMetaFunction *java_function, int arg_count,
        uint included_attributes, uint excluded_attributes,
        Option options) {
    uint attr = (java_function->attributes() & (~excluded_attributes)) | included_attributes;

    m_exportedPackages.clear();
    QString returnType;
    if ((options & SkipReturnType) == 0) {
        QString modified_type;
        if(java_function->type() && java_function->argumentTypeArray(0)){
            QScopedPointer<AbstractMetaType> cpy(java_function->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(AbstractMetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            AbstractMetaBuilder::decideUsagePattern(cpy.get());
            modified_type = translateType(cpy.get(), java_function->implementingClass(), Option(options & ~UseNativeIds)).replace('$', '.')+"[]";
        }else if(java_function->type() && java_function->argumentTypeBuffer(0)){
            QScopedPointer<AbstractMetaType> cpy(java_function->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(AbstractMetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            AbstractMetaBuilder::decideUsagePattern(cpy.get());
            modified_type = translateType(cpy.get(), java_function->implementingClass(), Option(options & ~UseNativeIds)).replace('$', '.');
            if(modified_type=="int"){
                modified_type = "java.nio.IntBuffer";
            }else if(modified_type=="byte"){
                modified_type = "java.nio.ByteBuffer";
            }else if(modified_type=="char"){
                modified_type = "java.nio.CharBuffer";
            }else if(modified_type=="short"){
                modified_type = "java.nio.ShortBuffer";
            }else if(modified_type=="long"){
                modified_type = "java.nio.LongBuffer";
            }else if(modified_type=="float"){
                modified_type = "java.nio.FloatBuffer";
            }else if(modified_type=="double"){
                modified_type = "java.nio.DoubleBuffer";
            }else{
                modified_type = "java.nio.Buffer";
            }
        }else{
            modified_type = java_function->typeReplaced(0);
            if(modified_type.isEmpty() && java_function->argumentReplaced(0)=="this"){
                Q_ASSERT(java_function->ownerClass());
                modified_type = java_function->ownerClass()->typeEntry()->targetLangName();
            }
        }
        if (modified_type.isEmpty()){
            if(!java_function->isFinal()){
                 options = Option(options | NoQCollectionContainers);
            }
            returnType = translateType(java_function->type(), java_function->implementingClass(), Option(options | InitializerListAsArray));
        }else{
            registerPackage(modified_type);
            returnType = modified_type.replace('$', '.');
        }
    }

    if ((attr & AbstractMetaAttributes::Public) || (attr & AbstractMetaAttributes::Protected)) {

        // Does the function use native pointer API?
        bool nativePointer = false;
        if(java_function->type() && java_function->type()->isNativePointer()){
            if(java_function->typeReplaced(0).isEmpty()
                     && java_function->argumentReplaced(0)!="this"
                     && !java_function->argumentTypeArray(0)
                     && !java_function->argumentTypeBuffer(0)
                     && java_function->type()->typeEntry()->qualifiedCppName()!="QMetaObject"){
                nativePointer = true;
            }
        }

        // Does the function need to be considered for resetting the Java objects after use?
        bool resettableObject = false;

        if (!nativePointer && java_function->type() && !java_function->type()->isPointerContainer()) {
            for(const AbstractMetaType *type : java_function->type()->instantiations()) {
                if (type && type->isNativePointer()) {
                    nativePointer = true;
                    break;
                }
            }
        }

        const AbstractMetaArgumentList& arguments = java_function->arguments();
        if (!nativePointer || (!resettableObject && !java_function->isFinal())) {
            for(const AbstractMetaArgument *argument : arguments) {
                if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No
                        && java_function->typeReplaced(argument->argumentIndex() + 1).isEmpty()
                        && !java_function->argumentTypeArray(argument->argumentIndex() + 1)
                        && !java_function->argumentTypeBuffer(argument->argumentIndex() + 1)) {

                    if (argument->type()->isNativePointer()) {
                        nativePointer = true;
                        if (resettableObject) break ;

                    } else if (!java_function->isFinalInTargetLang()
                               && argument->type()->isObject()
                               && !argument->type()->isQObject()
                               && !java_function->resetObjectAfterUse(argument->argumentIndex() + 1)
                               && java_function->ownership(java_function->declaringClass(),
                                        TypeSystem::ShellCode, argument->argumentIndex() + 1).ownership ==
                                            TypeSystem::InvalidOwnership) {
                        resettableObject = true;
                        if (nativePointer) break ;

                    } else if (argument->type()->hasInstantiations()) {

                        for(const AbstractMetaType *type : argument->type()->instantiations()) {
                            if (type && type->isNativePointer()) {
                                if(!argument->type()->isPointerContainer()){
                                    nativePointer = true;
                                    if (resettableObject) break;
                                }
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

        if (nativePointer
                && !m_nativepointer_functions.contains(java_function)
                && !java_function->ownerClass()->isFake()
                && (java_function->ownerClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)!=0
        ){
            if (java_function->ownerClass() == java_function->declaringClass() || java_function->isFinal())
                m_nativepointer_functions.append(java_function);
        }
        if (resettableObject
                && !java_function->ownerClass()->isFake()
                && (java_function->ownerClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)!=0
                && !m_resettable_object_functions.contains(java_function))
            m_resettable_object_functions.append(java_function);
    }

    if ((options & SkipAttributes) == 0) {
        if (java_function->isEmptyFunction()
                || java_function->isDeprecated()) s << INDENT << "@Deprecated" << Qt::endl;

        bool needsSuppressUnusedWarning = TypeDatabase::instance()->includeEclipseWarnings()
                                          && java_function->isSignal()
                                          && ((((excluded_attributes & AbstractMetaAttributes::Private) == 0)
                                              && (java_function->isPrivate()
                                                  || ((included_attributes & AbstractMetaAttributes::Private) != 0))) || java_function->isPrivateSignal());

        bool needsSuppressExportsWarning = false;

        if(java_function->isPublic()
                && (included_attributes & AbstractMetaAttributes::Private)==0
                && (options & NoSuppressExports)!=NoSuppressExports){
            QString strg;
            QTextStream s(&strg);
            writeFunctionArguments(s, java_function, arg_count, options);
            for(const QString& pkg : m_exportedPackages){
                if(pkg!=java_function->implementingClass()->package()
                        && !pkg.isEmpty()
                        && m_typeSystemByPackage[pkg]
                        && m_typeSystemByPackage[pkg]->module()!="qtjambi" // because it's transitive
                        && m_typeSystemByPackage[pkg]!=m_typeSystemByPackage[java_function->implementingClass()->package()]
                        && !pkg.startsWith("io.qt.internal")
                        && pkg!="io.qt"){
                    needsSuppressExportsWarning = true;
                    break;
                }
            }
        }

        if (needsSuppressUnusedWarning || needsSuppressExportsWarning || java_function->needsSuppressUncheckedWarning()) {
            s << INDENT << "@SuppressWarnings({";
            if (java_function->needsSuppressUncheckedWarning()) {
                s << "\"unchecked\"";
            } else if (needsSuppressUnusedWarning) {
                s << "\"unused\"";
            } else if (needsSuppressExportsWarning) {
                s << "\"exports\"";
            }
            s << "})" << Qt::endl;
        }

        const QPropertySpec *spec = java_function->propertySpec();
        if (spec && java_function->isPropertyNotify()) {
            s << "    @io.qt.QtPropertyNotify(name=\"" << spec->name() << "\")" << Qt::endl;
        }
        if (!(options & InitializerListAsArray) && java_function->arguments().size()>0) {
            QString typeReplaced = java_function->typeReplaced(java_function->arguments().size());
            if(java_function->arguments().last()->type()->isInitializerList()
               && java_function->arguments().last()->type()->instantiations().size()>0
               && java_function->arguments().last()->type()->instantiations().first()->instantiations().size()>0
               && typeReplaced.isEmpty()){
                s << INDENT << "@SafeVarargs" << Qt::endl;
            }else if(!typeReplaced.isEmpty()
                     && typeReplaced!="java.lang.Object..."
                     && typeReplaced.endsWith("...")){
                s << INDENT << "@SafeVarargs" << Qt::endl;
            }
        }

        if(!java_function->isConstructor()
                && (!java_function->isInGlobalScope() || (java_function->name()!="readFrom"
                                                          && java_function->name()!="writeTo") )
                && !java_function->isSignal()
                && !java_function->isStatic()
                && java_function->isFinalInCpp() && (excluded_attributes & AbstractMetaAttributes::FinalInTargetLang)==AbstractMetaAttributes::FinalInTargetLang){
            s << INDENT << "@io.qt.QtDeclaredFinal" << Qt::endl;
        }

        if (!(attr & NoBlockedSlot)
                && !java_function->isAllowedAsSlot()
                && !java_function->isInvokable()
                && !java_function->isConstructor()
                && !java_function->isSlot()
                && !java_function->isSignal()
                && !java_function->isStatic()
                && !(included_attributes & AbstractMetaAttributes::Static))
            s << INDENT << "@io.qt.QtUninvokable" << Qt::endl;
        s << INDENT;
        if (attr & AbstractMetaAttributes::Static && java_function->ownerClass()->typeEntry()->designatedInterface()) s << "private ";
        else if (attr & AbstractMetaAttributes::Public) s << "public ";
        else if (attr & AbstractMetaAttributes::Protected) s << "protected ";
        else if (attr & AbstractMetaAttributes::Private) s << "private ";

        bool isStatic = (attr & AbstractMetaAttributes::Static);
        bool isDefault = !isStatic && (options & DefaultFunction);

        if (!isDefault && attr & AbstractMetaAttributes::Native) s << "native ";
        else if (!isDefault && !isStatic && (attr & AbstractMetaAttributes::FinalInTargetLang)) s << "final ";
        else if (!isDefault && !isStatic && (attr & AbstractMetaAttributes::Abstract)) s << "abstract ";

        if (isStatic) s << "static ";

        if (isDefault) s << "default ";

        QList<QString> templateArguments;
        if(isStatic){
            if(java_function->type() && java_function->type()->typeEntry()->isTemplateArgument()
                    && !templateArguments.contains(java_function->type()->typeEntry()->qualifiedCppName())){
                templateArguments << java_function->type()->typeEntry()->qualifiedCppName();
            }
            for(const AbstractMetaArgument* arg : java_function->arguments()){
                if(arg->type()->typeEntry()->isTemplateArgument()
                        && !templateArguments.contains(arg->type()->typeEntry()->qualifiedCppName())){
                    templateArguments << arg->type()->typeEntry()->qualifiedCppName();
                }
            }
        }
        if(!java_function->isConstructor()){
            const QList<Parameter> addedParameterTypes = java_function->addedParameterTypes();
            for(const Parameter& p : addedParameterTypes){
                if(p.extends.isEmpty())
                    templateArguments << p.name;
                else
                    templateArguments << p.name + " extends " + p.extends;
            }
        }
        if(!templateArguments.isEmpty())
            s << "<" << templateArguments.join(",") << "> ";
    }

    if ((options & SkipReturnType) == 0) {
        s << returnType << " ";
    }
}

void JavaGenerator::writeConstructorContents(QTextStream &s, const AbstractMetaFunction *java_function) {
    // Write constructor
    s << "{" << Qt::endl;
    {
        INDENTATION(INDENT)
        s << INDENT << "super((QPrivateConstructor)null);" << Qt::endl;

        writeJavaCallThroughContents(s, java_function);

        // Write out expense checks if present...
        const AbstractMetaClass *java_class = java_function->implementingClass();
        const ComplexTypeEntry *te = java_class->typeEntry();
        if (te->expensePolicy().isValid()) {
            s << Qt::endl;
            const ExpensePolicy &ep = te->expensePolicy();
            s << INDENT << "boolean _dogc;" << Qt::endl
              << INDENT << "synchronized(" << java_class->fullName() << ".class){" << Qt::endl
              << INDENT << "    if(_dogc = (__qt_expenseCounter += " << (ep.cost.isEmpty() ? ep.cost : "1") << ") > " << ep.limit << ")" << Qt::endl
              << INDENT << "        __qt_expenseCounter = 0;" << Qt::endl
              << INDENT << "}" << Qt::endl
              << INDENT << "if (_dogc)" << Qt::endl
              << INDENT << "    System.gc();" << Qt::endl;
        }

        QStringList lines;
        for(const CodeSnip& snip : te->codeSnips()) {
            if (snip.language == TypeSystem::Constructors) {
                lines << snip.code().split("\n");
            }
        }
        printExtraCode(lines, s);
    }
    s << INDENT << "}" << Qt::endl
      << INDENT << Qt::endl;

    // Write native constructor
    if (java_function->jumpTableId() == -1)
        writePrivateNativeFunction(s, java_function);
}

void JavaGenerator::writeFunctionArguments(QTextStream &s, const AbstractMetaFunction *java_function,
        int argument_count, Option options) {
    const AbstractMetaArgumentList& arguments = java_function->arguments();

    if (argument_count == -1)
        argument_count = int(arguments.size());

    bool commaRequired = false;
    const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
    int argumentCounter = 1;
    while(addedArguments.first.contains(argumentCounter)){
        const ArgumentModification& mod = addedArguments.first[argumentCounter];
        if(commaRequired)
            s << ", ";
        commaRequired = true;
        s << QString(mod.modified_type).replace('$', '.') << " " << mod.modified_name;
        ++argumentCounter;
    }
    for (int i = 0; i < argument_count; ++i) {
        const AbstractMetaArgument *arg = arguments.at(i);
        if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
            if (commaRequired)
                s << ", ";
            writeArgument(s, java_function, arg, Option(options | CollectionAsCollection));
            commaRequired = true;
        }
        ++argumentCounter;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& mod = addedArguments.first[argumentCounter];
            if(commaRequired)
                s << ", ";
            commaRequired = true;
            s << QString(mod.modified_type).replace('$', '.') << " " << mod.modified_name;
            ++argumentCounter;
        }
    }
    for(const ArgumentModification& argumentMod : addedArguments.second){
        if(commaRequired)
            s << ", ";
        commaRequired = true;
        s << QString(argumentMod.modified_type).replace('$', '.') << " " << argumentMod.modified_name;
    }
}


void JavaGenerator::writeExtraFunctions(QTextStream &s, const AbstractMetaClass *java_class) {
    const ComplexTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    QStringList lines;
    for(const CodeSnip &snip : class_type->codeSnips()) {
        if (snip.position!=CodeSnip::Equals
            && snip.position!=CodeSnip::Compare
            && snip.position!=CodeSnip::HashCode
            && snip.position!=CodeSnip::ToString
            && ((!java_class->isInterface() && snip.language == TypeSystem::TargetLangCode)
                            || (java_class->isInterface() && snip.language == TypeSystem::Interface))) {
            lines << snip.code().split("\n");
        }
    }
    if(!lines.isEmpty())
        printExtraCode(lines, s);
}

void JavaGenerator::writeExtraFunctions(QTextStream &s, const AbstractMetaFunctional *java_class, bool inInterface) {
    const FunctionalTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    QStringList lines;
    for(const CodeSnip &snip : class_type->codeSnips()) {
        if (snip.position!=CodeSnip::Equals
            && snip.position!=CodeSnip::Compare
            && snip.position!=CodeSnip::HashCode
            && snip.position!=CodeSnip::ToString
            && ((!inInterface && snip.language == TypeSystem::TargetLangCode)
                            || (inInterface && snip.language == TypeSystem::Interface))) {
            lines << snip.code().split("\n");
        }
    }
    if(!lines.isEmpty()){
        printExtraCode(lines, s);
        s << Qt::endl;
    }
}

void JavaGenerator::writeToStringFunction(QTextStream &s, const AbstractMetaClass *java_class) {
    bool generate = java_class->toStringCapability() && !java_class->hasDefaultToStringFunction();
    bool core = java_class->package() == QLatin1String("io.qt.core");
    bool qevent = false;

    {
        const AbstractMetaClass *cls = java_class;
        while (cls) {
            if (cls->name() == "QEvent") {
                qevent = true;
                break;
            }
            cls = cls->baseClass();
        }
    }

    if (generate || qevent) {
        QStringList lines;
        for(const CodeSnip& codeSnip : java_class->typeEntry()->codeSnips()){
            if(codeSnip.language==TypeSystem::TargetLangCode && codeSnip.position==CodeSnip::ToString){
                lines << codeSnip.code().split("\n");
            }
        }
        if (qevent && core) {
            s << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "public String toString() {" << Qt::endl;
            if(!lines.isEmpty()){
                printExtraCode(lines, s);
            }
            s << INDENT << "    return getClass().getSimpleName() + \"(type=\" + type().name() + \")\";" << Qt::endl
              << INDENT << "}" << Qt::endl;
        } else {
            if(lines.isEmpty() && !java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface()){
                s << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public native String toString();" << Qt::endl;
            }else{
                QString name = java_class->name();
                if(java_class->typeEntry() && java_class->typeEntry()->designatedInterface()){
                    name = java_class->typeEntry()->designatedInterface()->targetLangName();
                }
                s << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "public String toString() {" << Qt::endl;
                if(!lines.isEmpty()){
                    printExtraCode(lines, s);
                }
                s << INDENT << "    return toString_native(";
                if(java_class->typeEntry()->isNativeIdBased()){
                    s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                }else if(java_class->typeEntry()->designatedInterface()){
                    s << "this";
                }
                s << ");" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << INDENT << "@io.qt.QtUninvokable" << Qt::endl
                  << INDENT << "private ";
                if(java_class->typeEntry()->isNativeIdBased() || java_class->typeEntry()->designatedInterface())
                    s << "static ";
                s << "native String toString_native(";
                if(java_class->typeEntry()->isNativeIdBased()){
                    s << "long __this_nativeId";
                }else if(java_class->typeEntry()->designatedInterface()){
                    s << java_class->simpleName() << " _this";
                }
                s << ");" << Qt::endl;
            }
        }
    }
}

void JavaGenerator::writeCloneFunction(QTextStream &s, const AbstractMetaClass *java_class) {
    s << INDENT << Qt::endl
      << INDENT << "@Override" << Qt::endl
      << INDENT << "public ";
    if(!java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface()){
        s << "native ";
    }
    s << java_class->simpleName();
    if(java_class->typeEntry()->isGenericClass()){
        if(java_class->templateBaseClass()){
            QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
            if(templateArguments.size()>0){
                s << "<";
                for (int i = 0; i < templateArguments.size(); ++i) {
                    if (i > 0)
                        s << ",";
                    s << QString(templateArguments.at(i)->name()).replace('$', '.');
                }
                s << ">";
            }
        }else{
            s << "<T>";
        }
    }
    if(!java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface()){
        s << " clone();" << Qt::endl;
    }else{
        QString name = java_class->name();
        if(java_class->typeEntry() && java_class->typeEntry()->designatedInterface()){
            name = java_class->typeEntry()->designatedInterface()->targetLangName();
        }
        s << " clone() {" << Qt::endl
          << INDENT << "    return clone_native(";
        if(java_class->typeEntry()->isNativeIdBased()){
            s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
        }else{
            s << "this";
        }
        s << ");" << Qt::endl
          << INDENT << "}" << Qt::endl
          << INDENT << "private native " << java_class->simpleName();
        if(java_class->typeEntry()->isGenericClass()){
            if(java_class->templateBaseClass()){
                QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
                if(templateArguments.size()>0){
                    s << "<";
                    for (int i = 0; i < templateArguments.size(); ++i) {
                        if (i > 0)
                            s << ",";
                        s << QString(templateArguments.at(i)->name()).replace('$', '.');
                    }
                    s << ">";
                }
            }else{
                s << "<T>";
            }
        }
        s << " clone_native(";
        if(java_class->typeEntry()->isNativeIdBased()){
            s << "long __this_nativeId";
        }else{
            s << java_class->simpleName() << " __this";
        }
        s << ");" << Qt::endl;
    }
}

void JavaGenerator::generateFake(const AbstractMetaClass *fake_class) {
    for(AbstractMetaFunctional* functional : fake_class->functionals()){
        if(functional->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
            QString fileName = QString("%1.java").arg(functional->name());
            ReportHandler::debugSparse(QString("generating: %1").arg(fileName));

            FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForClass(fake_class) + subDirectoryForPackage(functional->package()) + "/" + fileName);
            write(fileOut.stream, functional);

            if (fileOut.done())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
    for(const AbstractMetaEnum * enm : fake_class->enums()){
        if(enm->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
            {
                QString fileName = QString("%1.java").arg(enm->name());
                ReportHandler::debugSparse(QString("generating: %1").arg(fileName));
                FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForClass(fake_class) + subDirectoryForPackage(enm->package()) + "/" + fileName);
                write(fileOut.stream, enm);
                if (fileOut.done())
                    ++m_num_generated_written;
                ++m_num_generated;
            }

            // Write out the QFlags if present...
            FlagsTypeEntry *flags_entry = enm->typeEntry()->flags();
            if (flags_entry && !enm->typeEntry()->forceInteger()) {

                QStringList linesPos1;
                QStringList linesPos2;
                QStringList linesPos3;
                QStringList linesPos4;
                QStringList linesBegin;
                QStringList linesEnd;
                for(const CodeSnip &snip : enm->typeEntry()->codeSnips()) {
                    if (snip.language == TypeSystem::TargetLangCode) {
                        if (snip.position == CodeSnip::Position1) {
                            linesPos1 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position2) {
                            linesPos2 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position3) {
                            linesPos3 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position4) {
                            linesPos4 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Beginning) {
                            linesBegin << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::End){
                            linesEnd << snip.code().split("\n");
                        }
                    }
                }

                QString flagsName = flags_entry->targetLangName();
                QCryptographicHash cryptographicHash(QCryptographicHash::Sha512);
                cryptographicHash.addData(flagsName.toLatin1());
                QByteArray result = cryptographicHash.result();
                quint64 serialVersionUID = 0;
                QDataStream stream(result);
                while(!stream.atEnd()){
                    quint64 l = 0;
                    stream >> l;
                    serialVersionUID = serialVersionUID * 31 + l;
                }
                QString fileName = QString("%1.java").arg(flagsName);
                ReportHandler::debugSparse(QString("generating: %1").arg(fileName));
                FileOut fileOut(resolveOutputDirectory() + "/" + subDirectoryForClass(fake_class) + subDirectoryForPackage(enm->package()) + "/" + fileName);
                QTextStream& s = fileOut.stream;
                s << INDENT << "package " << enm->package() << ";" << Qt::endl
                  << Qt::endl
                  << INDENT << "public final class " << flagsName << " extends io.qt.QFlags<" << enm->name().replace("$",".") << "> {" << Qt::endl
                  << INDENT << "    private static final long serialVersionUID = 0x" << QString::number(serialVersionUID, 16) << "L;" << Qt::endl;
                printExtraCode(linesPos1, s, true);
                s << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public " << flagsName << "(" << enm->name().replace("$",".") << " ... args){" << Qt::endl
                  << INDENT << "        super(args);" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public " << flagsName << "(int value) {" << Qt::endl
                  << INDENT << "        super(value);" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final " << flagsName << " combined(" << enm->name().replace("$",".") << " e){" << Qt::endl
                  << INDENT << "        return new " << flagsName << "(value() | e.value());" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final " << enm->name().replace("$",".") << "[] flags(){" << Qt::endl
                  << INDENT << "        return super.flags(" << enm->name().replace("$",".") << ".values());" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final " << flagsName << " clone(){" << Qt::endl
                  << INDENT << "        return new " << flagsName << "(value());" << Qt::endl
                  << INDENT << "    }" << Qt::endl;
                printExtraCode(linesPos4, s, true);
                s << INDENT << "}" << Qt::endl << Qt::endl;
                if (fileOut.done())
                    ++m_num_generated_written;
                ++m_num_generated;
            }
        }
    }
}

void JavaGenerator::write(QTextStream &s, const AbstractMetaFunctional *global_fun, int){
    ReportHandler::debugSparse("Generating functional: " + global_fun->fullName());

   if (m_docs_enabled) {
       m_doc_parser = new DocParser(m_doc_directory + "/" + global_fun->name().toLower() + ".jdoc");
   }
    s << INDENT << "package " << global_fun->package() << ";" << Qt::endl << Qt::endl;

    const QList<Include>& includes = global_fun->typeEntry()->extraIncludes();
    for(const Include &inc : includes) {
        if (inc.type == Include::TargetLangImport) {
            s << inc.toString() << Qt::endl;
        }
    }

    QString lines;
    {
        QTextStream s(&lines);
        writeFunctional(s, global_fun);
    }

    if(!includes.isEmpty()){
        s << Qt::endl;
    }
    s << lines;

    if (m_docs_enabled) {
        delete m_doc_parser;
        m_doc_parser = nullptr;
    }
}

void JavaGenerator::write(QTextStream &s, const AbstractMetaEnum *global_enum) {
    if(global_enum->typeEntry()->codeGeneration()==TypeEntry::GenerateNothing)
        return;
    ReportHandler::debugSparse("Generating enum: " + global_enum->fullName());

   if (m_docs_enabled) {
       m_doc_parser = new DocParser(m_doc_directory + "/" + global_enum->name().toLower() + ".jdoc");
   }
    s << INDENT << "package " << global_enum->package() << ";" << Qt::endl << Qt::endl;

    QString lines;
    {
        QTextStream s(&lines);
        writeEnum(s, global_enum);
    }

    /*if(m_current_class_needs_internal_import){
        s << INDENT << "import static io.qt.internal.QtJambiInternal.*;" << Qt::endl << Qt::endl;
    }*/
    s << lines;

    if (m_docs_enabled) {
        delete m_doc_parser;
        m_doc_parser = nullptr;
    }
}
