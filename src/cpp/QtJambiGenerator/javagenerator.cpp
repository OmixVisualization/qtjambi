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

#include "javagenerator.h"
#include "cppgenerator.h"
#include "reporthandler.h"
#include "metabuilder.h"
#include "typesystem/qmltypesystemreader.h"

#include <QtCore>
#include <QDebug>
#include "typesystem/typedatabase.h"
#include "bufferedoutputstream.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

static Indentor INDENT;

QString encodeHtml(QString comment){
    return comment  .replace(QStringLiteral(u"&"), QStringLiteral(u"&amp;"))
                    .replace(QStringLiteral(u"<"), QStringLiteral(u"&lt;"))
                    .replace(QStringLiteral(u">"), QStringLiteral(u"&gt;"))
                    .replace(QStringLiteral(u"\t"), QStringLiteral(u"&nbsp;&nbsp;&nbsp;&nbsp;"))
                    .replace(QStringLiteral(u"@"), QStringLiteral(u"&commat;"))
                    .replace(QStringLiteral(u"/*"), QStringLiteral(u"&sol;*"))
                    .replace(QStringLiteral(u"*/"), QStringLiteral(u"*&sol;"));
}

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

QString findComparableType(const MetaClass *cls){
    QString comparableType;
    const MetaFunctionList geltFunctions = MetaFunctionList()
                                                << cls->greaterThanFunctions()
                                                << cls->greaterThanEqFunctions()
                                                << cls->lessThanFunctions()
                                                << cls->lessThanEqFunctions()
                                                << cls->compareFunctions();
    for(MetaFunction* f : geltFunctions){
        if(f->isPrivate())
            continue;
        MetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            if(arg->type()->typeEntry()->isComplex() && !static_cast<const ComplexTypeEntry *>(arg->type()->typeEntry())->implicitCasts().isEmpty()){
                type = QStringLiteral(u"Object");
            }else{
                type = arg->type()->typeEntry()->qualifiedTargetLangName();
                if(arg->type()->typeEntry()->isContainer()){
                    if(type==QStringLiteral(u"java.util.List")
                           ||  type==QStringLiteral(u"java.util.LinkedList")
                           ||  type==QStringLiteral(u"java.util.Queue")
                           ||  type==QStringLiteral(u"java.util.Deque")
                           ||  type==QStringLiteral(u"java.util.ArrayList")
                           ||  type==QStringLiteral(u"java.util.Vector")
                           ||  type==QStringLiteral(u"java.util.Set")
                           ||  type==QStringLiteral(u"io.qt.core.QSet")
                           ||  type==QStringLiteral(u"io.qt.core.QList")
                           ||  type==QStringLiteral(u"io.qt.core.QQueue")
                           ||  type==QStringLiteral(u"io.qt.core.QVector")
                           ||  type==QStringLiteral(u"io.qt.core.QStack")
                           ||  type==QStringLiteral(u"io.qt.core.QLinkedList")) {
                        type = QStringLiteral(u"java.util.Collection");
                    }else if(type==QStringLiteral(u"java.util.Map")
                           ||  type==QStringLiteral(u"java.util.SortedMap")
                           ||  type==QStringLiteral(u"java.util.NavigableMap")
                           ||  type==QStringLiteral(u"java.util.HashMap")
                           ||  type==QStringLiteral(u"java.util.TreeMap")
                           ||  type==QStringLiteral(u"io.qt.core.QMap")
                           ||  type==QStringLiteral(u"io.qt.core.QHash")
                           ||  type==QStringLiteral(u"io.qt.core.QMultiMap")
                           ||  type==QStringLiteral(u"io.qt.core.QMultiHash")){
                        type = QStringLiteral(u"java.util.Map");
                    }
                }
                if(arg->type()->instantiations().size()>0){
                    type += u"<";
                    for(int i=0; i<arg->type()->instantiations().size(); i++){
                        if(i==0)
                            type += u"?";
                        else
                            type += u",?";
                    }
                    type += u">";
                }
            }
        }
        type.replace(u'$', u'.');
        if(comparableType.isEmpty()){
            comparableType = type;
        }else if(comparableType!=type){
            comparableType = QStringLiteral(u"Object");
            break;
        }
    }
    if(comparableType.isEmpty()){
        comparableType = cls->typeEntry()->qualifiedTargetLangName();
    }
    auto idx = comparableType.indexOf(u'<');
    QString package;
    QString typeName = comparableType;
    if(idx>0){
        typeName = comparableType.mid(0, idx);
    }
    idx = typeName.lastIndexOf(u'.');
    QString ann = QStringLiteral(u"@NonNull ");
    if(idx>0){
        typeName = comparableType.mid(idx+1);
        package = comparableType.mid(0, idx+1);
        comparableType = package + ann + typeName;
    }else{
        comparableType = ann + comparableType;
    }
    return comparableType;
}

JavaGenerator::JavaGenerator(TS::TypeDatabase* database, bool nullness, bool noKotlinDelegates) :
    m_database(database),
    m_noKotlinDelegates(noKotlinDelegates),
    m_nullness(nullness) {
}

QString JavaGenerator::fileNameForClass(const MetaClass *java_class) const {
    return QStringLiteral(u"%1.java").arg(java_class->name());
}

void JavaGenerator::writeFieldAccessors(QTextStream &s, const MetaField *field, Option functionOptions, QList<const MetaFunction *> *overloadedFunctions) {
    Q_ASSERT(field->isPublic() || field->isProtected());

    const MetaClass *declaringClass = field->enclosingClass();
    const MetaFunction *setter = field->setter();
    const MetaFunction *getter = field->getter();

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
                QStringLiteral(u"class '%1' already has setter '%2' for %3 field '%4'")
                                  .arg(declaringClass->name()).arg(setter->name(), field->isProtected() ? QStringLiteral(u"protected") : QStringLiteral(u"public"), field->name());
            ReportHandler::warning(warning);
        } else {
            writeFunction(s, setter, 0, 0, functionOptions);
            if(overloadedFunctions)
                overloadedFunctions->append(setter);
        }
    }

    // Get function
    if (mod.isReadable()) {
        if (declaringClass->hasFunction(getter)) {
            QString warning =
                QStringLiteral(u"class '%1' already has getter '%2' for %3 field '%4'")
                                  .arg(declaringClass->name()).arg(getter->name(), field->isProtected() ? QStringLiteral(u"protected") : QStringLiteral(u"public"), field->name());
            ReportHandler::warning(warning);
        } else {
            writeFunction(s, getter, 0, 0, functionOptions);
            if(overloadedFunctions)
                overloadedFunctions->append(getter);
            if(((field->isStatic() && field->type()->isConstant() && field->type()->indirections().isEmpty()) || field->attributes() & MetaAttributes::ConstExpr)
                    && (
                        field->type()->isPrimitive()
                        || field->type()->isPrimitiveChar()
                        || field->type()->isEnum()
                        || field->type()->isQString()
                        || field->type()->isCharString()
                        || field->type()->isQStringRef()
                        || field->type()->isQStringView()
                        || field->type()->isQAnyStringView()
                        || field->type()->isQUtf8StringView()
                        )){
                QString comment;
                QTextStream commentStream(&comment);
                if(!field->brief().isEmpty()){
                    commentStream << "<p>" << encodeHtml(field->brief()) << "</p>" << Qt::endl;
                }
                commentStream << "<p>See <code>";
                if(!field->href().isEmpty())
                    commentStream << "<a href=\"" << docsUrl << field->href() << "\">";
                if(declaringClass){
                    commentStream << encodeHtml(declaringClass->qualifiedCppName()
                                         .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                         .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                         .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                                  << "::<wbr/>";
                }
                commentStream << encodeHtml(field->name());
                if(!field->href().isEmpty())
                    commentStream << "</a>";
                commentStream << "</code></p>" << Qt::endl;
                if(!field->since().isEmpty()){
                    commentStream << "@since This field was introduced in Qt " << field->since() << "." << Qt::endl;
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
                s << "final " << qualifiedJavaType(field->type(), declaringClass, Option(VarArgsAsArray))
                  << " " << field->getter()->name() << " = " << field->getter()->name() << "();" << Qt::endl << Qt::endl;
            }
        }
    }
}

void JavaGenerator::registerPackage(QString pkgName, bool inclExported){
    auto idx = pkgName.indexOf(u'<');
    if(idx>0){
        pkgName = pkgName.mid(0, idx);
    }
    if(pkgName==QStringLiteral(u"io.qt.qml.QQmlListProperty"))
        inclExported = false;
    if(pkgName.startsWith(QStringLiteral(u"?"))){
        pkgName = pkgName.mid(1).trimmed();
        if(pkgName.startsWith(QStringLiteral(u"extends "))){
            pkgName = pkgName.mid(8);
        }else if(pkgName.startsWith(QStringLiteral(u"super "))){
            pkgName = pkgName.mid(6);
        }
        const QStringList pkgs = pkgName.split(QStringLiteral(u"&"));
        for(const QString& pkg : pkgs){
            registerPackage(pkg.trimmed(), inclExported);
        }
        return;
    }
    idx = pkgName.lastIndexOf(u'.');
    if(idx>0){
        pkgName = pkgName.mid(0, idx);
        m_currentPackages << pkgName;
        if(inclExported)
            m_exportedPackages.insert(pkgName);
    }
}


QString JavaGenerator::qualifiedJavaType(const MetaType *java_type, const MetaClass *context, Option option) {
    QString s;

    if (context && java_type && context->typeEntry()->isGenericClass() && java_type->originalTemplateType())
        java_type = java_type->originalTemplateType();

    if (!java_type) {
        s = QStringLiteral(u"void");
    }else if(java_type->typeUsagePattern()==MetaType::VoidPattern){
        if(option & BoxedPrimitive){
            s = QStringLiteral(u"Void");
        }else{
            s = QStringLiteral(u"void");
        }
    } else if (java_type->isIterator()){
        const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(java_type->typeEntry());
        s = iteratorType->qualifiedTargetLangName();
        bool found = false;
        if(!java_type->iteratorInstantiations().isEmpty()){
            if(java_type->iteratorInstantiations().size()==2){
                s = QStringLiteral(u"io.qt.core.QAssociativeConstIterator");
            }
            if ((option & StrictNonNull)
                    || !(option & NoNullness)) {
                auto idx = s.indexOf(u'<');
                QString package;
                QString typeName = s;
                if(idx>0){
                    typeName = s.mid(0, idx);
                }
                idx = typeName.lastIndexOf(u'.');
                QString nullness;
                if(option & StrictNonNull){
                    nullness = QStringLiteral(u"@StrictNonNull ");
                }else{
                    nullness = QStringLiteral(u"@NonNull ");
                }
                if(idx>0){
                    typeName = s.mid(idx+1);
                    package = s.mid(0, idx+1);
                    s = package + nullness + typeName;
                }else{
                    s = nullness + s;
                }
            }
            s += "<";
            for(int i=0; i<java_type->iteratorInstantiations().size(); i++){
                if(i>0)
                    s += ", ";
                s += qualifiedJavaType(java_type->iteratorInstantiations().at(i), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
            }
            s += ">";
            found = true;
        }
        if(!found && iteratorType->containerType()){
            MetaClass * containerClass = m_classes.findClass(iteratorType->containerType()->qualifiedCppName());
            if(containerClass){
                const ContainerTypeEntry* containerType = nullptr;
                if(containerClass->templateBaseClass() && containerClass->templateBaseClass()->typeEntry()->type()==TypeEntry::ContainerType){
                    containerType = static_cast<const ContainerTypeEntry*>(containerClass->templateBaseClass()->typeEntry());
                    if(containerType->type()==ContainerTypeEntry::MapContainer
                        || containerType->type()==ContainerTypeEntry::MultiMapContainer
                        || containerType->type()==ContainerTypeEntry::HashContainer
                            || containerType->type()==ContainerTypeEntry::MultiHashContainer){
                        s = QStringLiteral(u"io.qt.core.QAssociativeConstIterator");
                    }
                }
                if(!containerClass->templateBaseClassInstantiations().isEmpty() && (option & SkipTemplateParameters)==0){
                    if ((option & StrictNonNull)
                            || !(option & NoNullness)) {
                        auto idx = s.indexOf(u'<');
                        QString package;
                        QString typeName = s;
                        if(idx>0){
                            typeName = s.mid(0, idx);
                        }
                        idx = typeName.lastIndexOf(u'.');
                        QString nullness;
                        if(option & StrictNonNull){
                            nullness = QStringLiteral(u"@StrictNonNull ");
                        }else{
                            nullness = QStringLiteral(u"@NonNull ");
                        }
                        if(idx>0){
                            typeName = s.mid(idx+1);
                            package = s.mid(0, idx+1);
                            s = package + nullness + typeName;
                        }else{
                            s = nullness + s;
                        }
                    }
                    s += "<";
                    for(int i=0; i<containerClass->templateBaseClassInstantiations().size(); i++){
                        if(i>0)
                            s += ", ";
                        s += qualifiedJavaType(containerClass->templateBaseClassInstantiations().at(i), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
                    }
                    s += ">";
                    found = true;
                }
            }
        }
        if(!found){
            MetaClass * iteratorClass = m_classes.findClass(iteratorType->qualifiedCppName(), MetaClassList::QualifiedCppName);
            if(iteratorClass){
                if ((option & StrictNonNull)
                        || !(option & NoNullness)) {
                    auto idx = s.indexOf(u'<');
                    QString package;
                    QString typeName = s;
                    if(idx>0){
                        typeName = s.mid(0, idx);
                    }
                    idx = typeName.lastIndexOf(u'.');
                    QString nullness;
                    if(option & StrictNonNull){
                        nullness = QStringLiteral(u"@StrictNonNull ");
                    }else{
                        nullness = QStringLiteral(u"@NonNull ");
                    }
                    if(idx>0){
                        typeName = s.mid(idx+1);
                        package = s.mid(0, idx+1);
                        s = package + nullness + typeName;
                    }else{
                        s = nullness + s;
                    }
                }
                if(iteratorClass->typeAliasType()){
                    QScopedPointer<MetaType> typeAliasType(iteratorClass->typeAliasType()->copy());
                    if(typeAliasType->indirections().size()==1 && typeAliasType->getReferenceType()==MetaType::NoReference){
                        QList<bool> indirections = typeAliasType->indirections();
                        indirections.takeFirst();
                        typeAliasType->setIndirections(indirections);
                        typeAliasType->setReferenceType(MetaType::Reference);
                        MetaBuilder::decideUsagePattern(typeAliasType.data());
                    }
                    if((option & SkipTemplateParameters)==0){
                        s += "<";
                        s += qualifiedJavaType(typeAliasType.data(), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
                        s += ">";
                    }
                }else{
                    for(MetaFunction* function : iteratorClass->functions()){
                        if(function->originalName()==QStringLiteral(u"operator*") && function->type() && function->arguments().isEmpty() && function->isConstant()){
                            if((option & SkipTemplateParameters)==0){
                                s += "<";
                                s += qualifiedJavaType(function->type(), context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
                                s += ">";
                            }
                            break;
                        }
                    }
                }
            }
        }
    } else if (java_type->isQSpan() && java_type->instantiations().size()>=1) {
        if (java_type->hasNativeId() && (option & UseNativeIds)) {
            s = QStringLiteral(u"long");
        }else{
            const MetaType* instantiation = java_type->instantiations()[0];
            QString nonnull;
            if(!(option & NoNullness)){
                nonnull = QStringLiteral(u"@NonNull ");
            }
            QString argument = qualifiedJavaType(instantiation, context, Option((option & ~EnumAsInts & ~UseNativeIds) | BoxedPrimitive | NoSuppressExports | VarArgsAsArray));
            if(instantiation->isConstant()){
                s = QStringLiteral(u"io.qt.core.%1QConstSpan<%2>").arg(nonnull, argument);
            }else{
                s = QStringLiteral(u"io.qt.core.%1QSpan<%2>").arg(nonnull, argument);
            }
        }
    } else if (java_type->isInitializerList() && java_type->instantiations().size()==1) {
        const MetaType* instantiation = java_type->instantiations()[0];
        s = qualifiedJavaType(instantiation, context, Option((option & ~EnumAsInts & ~UseNativeIds & ~BoxedPrimitive) | NoSuppressExports | VarArgsAsArray));
        if (option & StrictNonNull) {
            s += QStringLiteral(u"@StrictNonNull ");
        }else if (!(option & NoNullness)) {
            if(!java_type->typeEntry()->isPrimitive()
                    && !java_type->typeEntry()->isQVariant()
                    && !java_type->typeEntry()->isQChar()
                    && !java_type->typeEntry()->isFunctional()
                    && java_type->indirections().isEmpty()){
                s += QStringLiteral(u"@NonNull ");
            }else if(!java_type->indirections().isEmpty()
                     || java_type->typeEntry()->isFunctional()){
                s += QStringLiteral(u"@Nullable ");
            }
        }
        if(option & VarArgsAsArray)
            s += QStringLiteral(u"[]");
        else
            s += QStringLiteral(u" ...");
    } else if (java_type->isCharString()) {
        if (option & NoNullness) {
            s = QStringLiteral(u"java.lang.String");
        }else{
            s = QStringLiteral(u"java.lang.@Nullable String");
        }
    } else if (java_type->hasNativeId() && (option & UseNativeIds)) {
        s = QStringLiteral(u"long");
    } else if (java_type->isArray()) {
        s = qualifiedJavaType(java_type->arrayElementType(), context, Option((option & ~EnumAsInts & ~UseNativeIds & ~BoxedPrimitive) | NoSuppressExports | VarArgsAsArray));
        if(!s.endsWith("QNativePointer")){
            for(int i=0; i<java_type->arrayElementCounts().size(); ++i){
                if (!(option & NoNullness))
                    s += QStringLiteral(u" @StrictNonNull[]");
                else
                    s += QStringLiteral(u"[]");
            }
        }else{
            s = s.replace("@NonNull", "@StrictNonNull").replace("@Nullable", "@StrictNonNull");
        }
    } else if(java_type->isContainer() && static_cast<const ContainerTypeEntry *>(java_type->typeEntry())->type()==ContainerTypeEntry::std_array){
        const MetaType *elementType = java_type->instantiations()[0];
        s = qualifiedJavaType(elementType, context, Option((option & ~EnumAsInts & ~UseNativeIds & ~BoxedPrimitive) | NoSuppressExports | VarArgsAsArray));
        if(!s.endsWith("QNativePointer")){
            if (!(option & NoNullness))
                s += QStringLiteral(u" @StrictNonNull[]");
            else
                s += QStringLiteral(u"[]");
        }else{
            s = s.replace("@NonNull", "@StrictNonNull").replace("@Nullable", "@StrictNonNull");
        }
    } else if (java_type->isEnum()) {
        const EnumTypeEntry * eentry = reinterpret_cast<const EnumTypeEntry *>(java_type->typeEntry());
        uint size = eentry->size();

        if(eentry->forceInteger()){
            switch(size){
            case 8:
                if (option & BoxedPrimitive)
                    s = QStringLiteral(u"Byte");
                else
                    s = QStringLiteral(u"byte");
                break;
            case 16:
                if (option & BoxedPrimitive)
                    s = QStringLiteral(u"Short");
                else
                    s = QStringLiteral(u"short");
                break;
            case 32:
                if (option & BoxedPrimitive)
                    s = QStringLiteral(u"Integer");
                else
                    s = "int";
                break;
            case 64:
                if (option & BoxedPrimitive)
                    s = QStringLiteral(u"Long");
                else
                    s = QStringLiteral(u"long");
                break;
            default:
                if (option & BoxedPrimitive)
                    s = QStringLiteral(u"Integer");
                else
                    s = QStringLiteral(u"int");
                break;
            }
            if (((option & StrictNonNull)
                    || !(option & NoNullness))
                    && (option & BoxedPrimitive)) {
                if(option & StrictNonNull){
                    s = QStringLiteral(u"java.lang.@QtPrimitiveType@StrictNonNull ") + s;
                }else if(java_type->indirections().isEmpty()){
                    s = QStringLiteral(u"java.lang.@QtPrimitiveType@NonNull ") + s;
                }else{
                    s = QStringLiteral(u"java.lang.@QtPrimitiveType@Nullable ") + s;
                }
            }
        }else{
            if (option & EnumAsInts)
                switch(size){
                case 8:
                    s = QStringLiteral(u"byte");
                    break;
                case 16:
                    s = QStringLiteral(u"short");
                    break;
                case 32:
                    s = QStringLiteral(u"int");
                    break;
                case 64:
                    s = QStringLiteral(u"long");
                    break;
                default:
                    s = QStringLiteral(u"int");
                    break;
                }
            else{
                m_currentPackages << static_cast<const EnumTypeEntry *>(java_type->typeEntry())->javaPackage();
                if((option & NoSuppressExports) == 0)
                    m_exportedPackages.insert(static_cast<const EnumTypeEntry *>(java_type->typeEntry())->javaPackage());
                s = java_type->fullName().replace(u'$', u'.');

                if ((option & StrictNonNull)
                        || !(option & NoNullness)) {
                    auto idx = s.indexOf(u'<');
                    QString package;
                    QString typeName = s;
                    if(idx>0){
                        typeName = s.mid(0, idx);
                    }
                    idx = typeName.lastIndexOf(u'.');
                    QString nullness;
                    if(option & StrictNonNull){
                        nullness = QStringLiteral(u"@StrictNonNull ");
                    }else if(java_type->indirections().isEmpty()){
                        nullness = QStringLiteral(u"@NonNull ");
                    }else{
                        nullness = QStringLiteral(u"@Nullable ");
                    }
                    if(idx>0){
                        typeName = s.mid(idx+1);
                        package = s.mid(0, idx+1);
                        s = package + nullness + typeName;
                    }else{
                        s = nullness + s;
                    }
                }
            }
        }
    } else if (java_type->isFlags()) {
        if (static_cast<const FlagsTypeEntry *>(java_type->typeEntry())->forceInteger()) {
            if (option & BoxedPrimitive){
                if ((option & StrictNonNull)
                        || !(option & NoNullness)) {
                    if(option & StrictNonNull){
                        s = QStringLiteral(u"java.lang.@QtPrimitiveType@StrictNonNull Integer");
                    }else if(java_type->indirections().isEmpty()){
                        s = QStringLiteral(u"java.lang.@QtPrimitiveType@NonNull Integer");
                    }else{
                        s = QStringLiteral(u"java.lang.@QtPrimitiveType@Nullable Integer");
                    }
                }else{
                    s = QStringLiteral(u"java.lang.@QtPrimitiveType@Nullable Integer");
                }
            }else
                s = QStringLiteral(u"int");
        } else {
            if (option & EnumAsInts)
                s = QStringLiteral(u"int");
            else{
                m_currentPackages << static_cast<const FlagsTypeEntry *>(java_type->typeEntry())->javaPackage();
                if((option & NoSuppressExports) == 0)
                    m_exportedPackages.insert(static_cast<const FlagsTypeEntry *>(java_type->typeEntry())->javaPackage());
                s = java_type->fullName().replace(u'$', u'.');
                if ((option & StrictNonNull)
                        || !(option & NoNullness)) {
                    auto idx = s.indexOf(u'<');
                    QString package;
                    QString typeName = s;
                    if(idx>0){
                        typeName = s.mid(0, idx);
                    }
                    idx = typeName.lastIndexOf(u'.');
                    QString nullness;
                    if(option & StrictNonNull){
                        nullness = QStringLiteral(u"@StrictNonNull ");
                    }else if(java_type->indirections().isEmpty()){
                        nullness = QStringLiteral(u"@NonNull ");
                    }else{
                        nullness = QStringLiteral(u"@Nullable ");
                    }
                    if(idx>0){
                        typeName = s.mid(idx+1);
                        package = s.mid(0, idx+1);
                        s = package + nullness + typeName;
                    }else{
                        s = nullness + s;
                    }
                }
            }
        }
    } else {
        if ((java_type->isPrimitive() || java_type->isPrimitiveChar()) && (option & BoxedPrimitive) ) {
            s = static_cast<const PrimitiveTypeEntry *>(java_type->typeEntry())->javaObjectFullName();
            auto idx = s.lastIndexOf(u'.');
            QString ann;
            if (!(option & StrictNonNull)
                    && (option & NoNullness)) {
                ann = QStringLiteral(u"@QtPrimitiveType ");
            }
            if(idx>0){
                QString typeName = s.mid(idx+1);
                QString package = s.mid(0, idx+1);
                s = package + ann + typeName;
            }else{
                s = ann + s;
            }
        } else if (java_type->isNativePointer()) {
            s = QStringLiteral(u"QNativePointer");
        } else if (java_type->isContainer()) {
            const ContainerTypeEntry * container = static_cast<const ContainerTypeEntry *>(java_type->typeEntry());
            if(container->type()==ContainerTypeEntry::std_array){
                const QList<const MetaType *>& args = java_type->instantiations();
                Q_ASSERT(args.size()>0);
                s += qualifiedJavaType(args[0], context, Option(NoQCollectionContainers | VarArgsAsArray));
                if (!(option & NoNullness))
                    s += QStringLiteral(u" @NonNull[]");
                else
                    s += QStringLiteral(u"[]");
            }else{
                if(container->type()==ContainerTypeEntry::std_chrono){
                    option = Option(option | SkipTemplateParameters);
                }
                if(((option & CollectionAsCollection) != CollectionAsCollection)
                        && ((option & NoQCollectionContainers) != NoQCollectionContainers)
                        && (java_type->getReferenceType()==MetaType::Reference
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
                        s = QStringLiteral(u"io.qt.core.QList");
                    }else{
                        if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)>=QT_VERSION_CHECK(6,0,0) && container->type()==ContainerTypeEntry::VectorContainer){
                            s = QStringLiteral(u"io.qt.core.QList");
                        }else{
                            s = QStringLiteral(u"io.qt.core.")+java_type->typeEntry()->qualifiedCppName();
                        }
                    }
                    if ((option & SkipTemplateParameters) == 0) {
                        s += u'<';
                        const QList<const MetaType *>& args = java_type->instantiations();
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            if((option & IsReturnType) != IsReturnType
                                    && !args.at(i)->isPrimitive()
                                    && !args.at(i)->isPrimitiveChar()
                                    && !args.at(i)->isJavaString())
                                s += "? extends ";
                            s += qualifiedJavaType(args.at(i), context, Option((option & (NoNullness | IsReturnType)) | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
                        }
                        s += '>';
                    }
                }else{
                    s = java_type->typeEntry()->qualifiedTargetLangName();
                    if((option & CollectionAsCollection) == CollectionAsCollection){
                        if(s==QStringLiteral(u"java.util.List")
                                ||  s==QStringLiteral(u"java.util.LinkedList")
                                ||  s==QStringLiteral(u"java.util.Queue")
                                ||  s==QStringLiteral(u"java.util.Deque")
                                ||  s==QStringLiteral(u"java.util.ArrayList")
                                ||  s==QStringLiteral(u"java.util.Vector")
                                ||  s==QStringLiteral(u"java.util.Set")
                                ||  s==QStringLiteral(u"io.qt.core.QSet")
                                ||  s==QStringLiteral(u"io.qt.core.QList")
                                ||  s==QStringLiteral(u"io.qt.core.QStringList")
                                ||  s==QStringLiteral(u"io.qt.core.QQueue")
                                ||  s==QStringLiteral(u"io.qt.core.QVector")
                                ||  s==QStringLiteral(u"io.qt.core.QStack")
                                ||  s==QStringLiteral(u"io.qt.core.QLinkedList")){
                            s = QStringLiteral(u"java.util.Collection");
                        }else if(s==QStringLiteral(u"java.util.Map")
                                 ||  s==QStringLiteral(u"java.util.SortedMap")
                                 ||  s==QStringLiteral(u"java.util.NavigableMap")
                                 ||  s==QStringLiteral(u"java.util.HashMap")
                                 ||  s==QStringLiteral(u"java.util.TreeMap")
                                 ||  s==QStringLiteral(u"io.qt.core.QMap")
                                 ||  s==QStringLiteral(u"io.qt.core.QHash")
                                 ||  s==QStringLiteral(u"io.qt.core.QMultiMap")
                                 ||  s==QStringLiteral(u"io.qt.core.QMultiHash")){
                            s = QStringLiteral(u"java.util.Map");
                        }else if(s==QStringLiteral(u"java.time.Duration")){
                            s = QStringLiteral(u"java.time.temporal.TemporalAmount");
                        }
                    }else if((option & NoQCollectionContainers) == NoQCollectionContainers && java_type->indirections().isEmpty()){
                        if(s==QStringLiteral(u"io.qt.core.QList")
                                || s==QStringLiteral(u"io.qt.core.QVector")){
                            s = "java.util.List";
                        }else if(s==QStringLiteral(u"io.qt.core.QStringList")){
                            s = "java.util.List";
                        }else if(s==QStringLiteral(u"io.qt.core.QSet")){
                            s = "java.util.Set";
                        }else if(s==QStringLiteral(u"io.qt.core.QQueue")){
                            s = "java.util.Queue";
                        }else if(s==QStringLiteral(u"io.qt.core.QStack")){
                            s = "java.util.Deque";
                        }else if(s==QStringLiteral(u"io.qt.core.QLinkedList")){
                            s = "java.util.Deque";
                        }else if(s==QStringLiteral(u"io.qt.core.QMap")
                                 ||  s==QStringLiteral(u"io.qt.core.QMultiMap")){
                            s = "java.util.NavigableMap";
                        }else if(s==QStringLiteral(u"io.qt.core.QHash")
                                 ||  s==QStringLiteral(u"io.qt.core.QMultiHash")){
                            s = QStringLiteral(u"java.util.Map");
                        }else if(s==QStringLiteral(u"java.time.Duration")){
                            s = QStringLiteral(u"java.time.temporal.TemporalAmount");
                        }
                    }

                    QString plainType;
                    if((option & SkipTemplateParameters) == 0 && container->type()==ContainerTypeEntry::QModelRoleDataSpanContainer){
                        s += QStringLiteral(u"<Integer,Object>");
                    }else if ((option & SkipTemplateParameters) == 0 && s!=QStringLiteral(u"io.qt.core.QStringList")) {
                        plainType = s;
                        s += u'<';
                        const QList<const MetaType *>& args = java_type->instantiations();
                        auto argssize = args.size();
                        for (int i=0; i<argssize; ++i) {
                            if (i != 0)
                                s += QStringLiteral(u", ");
                            bool isMultiMap = (container->type() == ContainerTypeEntry::MultiMapContainer
                                               || container->type() == ContainerTypeEntry::MultiHashContainer)
                                              && ((option & NoQCollectionContainers) == NoQCollectionContainers
                                                  || (option & CollectionAsCollection) == CollectionAsCollection)
                                              && i == 1;
                            if (isMultiMap)
                                s += "? extends java.util.List<";
                            if((option & IsReturnType) != IsReturnType
                                    && !args.at(i)->isPrimitive()
                                    && !args.at(i)->isJavaString()
                                    && !args.at(i)->isPrimitiveChar())
                                s += "? extends ";
                            s += qualifiedJavaType(args.at(i), context, Option((option & (NoNullness | IsReturnType)) | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
                            if (isMultiMap)
                                s += ">";
                        }
                        s += '>';
                    }
                    if(s.startsWith(QStringLiteral(u"java.util.Optional<java.lang.")) && s.endsWith(QStringLiteral(u"Integer>"))){
                        s = QStringLiteral(u"java.util.OptionalInt");
                    }else if(s.startsWith(QStringLiteral(u"java.util.Optional<java.lang.")) && s.endsWith(QStringLiteral(u"Long>"))){
                        s = QStringLiteral(u"java.util.OptionalLong");
                    }else if(s.startsWith(QStringLiteral(u"java.util.Optional<java.lang.")) && s.endsWith(QStringLiteral(u"Double>"))){
                        s = QStringLiteral(u"java.util.OptionalDouble");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Boolean>"))){
                        s = QStringLiteral(u"io.qt.core.QBooleanPropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Byte>"))){
                        s = QStringLiteral(u"io.qt.core.QBytePropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Short>"))){
                        s = QStringLiteral(u"io.qt.core.QShortPropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Integer>"))){
                        s = QStringLiteral(u"io.qt.core.QIntPropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Long>"))){
                        s = QStringLiteral(u"io.qt.core.QLongPropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Char>"))){
                        s = QStringLiteral(u"io.qt.core.QCharPropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Float>"))){
                        s = QStringLiteral(u"io.qt.core.QFloatPropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QPropertyBinding<java.lang.")) && s.endsWith(QStringLiteral(u"Double>"))){
                        s = QStringLiteral(u"io.qt.core.QDoublePropertyBinding");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Boolean>"))){
                        s = QStringLiteral(u"io.qt.core.QBooleanBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Byte>"))){
                        s = QStringLiteral(u"io.qt.core.QByteBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Short>"))){
                        s = QStringLiteral(u"io.qt.core.QShortBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Integer>"))){
                        s = QStringLiteral(u"io.qt.core.QIntBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Long>"))){
                        s = QStringLiteral(u"io.qt.core.QLongBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Char>"))){
                        s = QStringLiteral(u"io.qt.core.QCharBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Float>"))){
                        s = QStringLiteral(u"io.qt.core.QFloatBindable");
                    }else if(s.startsWith(QStringLiteral(u"io.qt.core.QBindable<java.lang.")) && s.endsWith(QStringLiteral(u"Double>"))){
                        s = QStringLiteral(u"io.qt.core.QDoubleBindable");
                    }
                }
            }
        } else if (java_type->isSmartPointer() && java_type->instantiations().size()==1) {
            MetaType* instantiation = java_type->instantiations()[0]->copy();
            instantiation->setIndirections(QList<bool>(instantiation->indirections()) << false);
            MetaBuilder::decideUsagePattern(instantiation);
            if(!instantiation->typeEntry()->isObject() && instantiation->typeUsagePattern()==MetaType::NativePointerPattern)
                instantiation->setTypeUsagePattern(MetaType::ObjectPattern);
            return qualifiedJavaType(instantiation, context, Option((option & ~EnumAsInts) & ~UseNativeIds));
        } else {
            const TypeEntry *type = java_type->typeEntry();
            if (type->designatedInterface())
                type = type->designatedInterface();
            s = type->qualifiedTargetLangName();
            if(!java_type->instantiations().isEmpty()){
                s += '<';
                const QList<const MetaType *>& args = java_type->instantiations();
                Option loopOption = Option(option & (NoNullness | IsReturnType));
                for (int i=0; i<args.size(); ++i) {
                    if (i != 0)
                        s += ", ";
                    if((option & IsReturnType) != IsReturnType
                            && !args.at(i)->isPrimitive()
                            && !args.at(i)->isJavaString()
                            && !args.at(i)->isPrimitiveChar())
                        s += "? extends ";
                    s += qualifiedJavaType(args.at(i), context, Option(loopOption | BoxedPrimitive | NoQCollectionContainers | VarArgsAsArray));
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
                        const QList<const MetaType *>& args = java_type->instantiations();
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            s += qualifiedJavaType(args[i], context, option);
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
                    MetaFunctional* functional = nullptr;
                    for(MetaFunctional* f : context->functionals()){
                        if(f->typeEntry()==ftype){
                            functional = f;
                            break;
                        }
                    }
                    if(functional && !functional->javaFunctionalInterfaceParameterTypes().isEmpty()){
                        s += '<';
                        const QList<uint>& args = functional->javaFunctionalInterfaceParameterTypes();
                        Option loopOption = Option(option & NoNullness);
                        for (int i=0; i<args.size(); ++i) {
                            if (i != 0)
                                s += ", ";
                            //s += args[i]->name();
                            QString modifiedType = functional->typeReplaced(args[i]);
                            if(modifiedType.isEmpty()){
                                if(args[i]==0){
                                    s += qualifiedJavaType(functional->type(), context, Option(loopOption | BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers));
                                }else{
                                    s += qualifiedJavaType(functional->arguments()[args[i]-1]->type(), context, Option(loopOption | BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers));
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
        if(java_type && !java_type->isEnum() && !java_type->isFlags())
            registerPackage(s, (option & NoSuppressExports) == 0);
        s = s.replace(u'$', u'.');
        if ((option & StrictNonNull)
                || (!(option & NoNullness)
                    && (((!java_type->typeEntry()->isPrimitive() || (option & BoxedPrimitive))
                         && !java_type->typeEntry()->isQVariant()
                         && !java_type->typeEntry()->isQChar()
                         && java_type->indirections().isEmpty())
                        || !java_type->indirections().isEmpty()))) {
            auto idx = s.indexOf(u'<');
            QString package;
            QString typeName = s;
            if(idx>0){
                typeName = s.mid(0, idx);
            }
            idx = typeName.lastIndexOf(u'.');
            QString ann;
            if((option & BoxedPrimitive) && java_type->typeEntry()->isPrimitive()){
                if(java_type->typeEntry()->isVoid()){
                    ann = QStringLiteral(u"@QtPrimitiveType ");
                }else if(option & StrictNonNull){
                    ann = QStringLiteral(u"@QtPrimitiveType@StrictNonNull ");
                }else if(java_type->indirections().isEmpty()){
                    ann = QStringLiteral(u"@QtPrimitiveType@NonNull ");
                }else{
                    ann = QStringLiteral(u"@QtPrimitiveType@Nullable ");
                }
            }else{
                if(option & StrictNonNull){
                    ann = QStringLiteral(u"@StrictNonNull ");
                }else if(java_type->indirections().isEmpty() && !java_type->typeEntry()->isFunctional()){
                    ann = QStringLiteral(u"@NonNull ");
                }else{
                    ann = QStringLiteral(u"@Nullable ");
                }
            }
            if(idx>0){
                typeName = s.mid(idx+1);
                package = s.mid(0, idx+1);
                s = package + ann + typeName;
            }else{
                s = ann + s;
            }
        }
    }
    return s;
}

void JavaGenerator::writeFunctionArgument(QTextStream &s,
                                  const MetaFunction *java_function,
                                  const MetaArgument *java_argument,
                                  bool &commaRequired,
                                  const QMap<int,Replacement>* alternativeTypes,
                                  Option options) {
    bool addArrayOffset = false;
    bool addArrayLength = false;
    Replacement alternativeType;
    if(alternativeTypes)
        alternativeType = alternativeTypes->value(java_argument->argumentIndex());
    QString modified_type;
    bool addNullness = false;
    if(alternativeType){
        if(alternativeType.type==Replacement::RemoveArgument || alternativeType.type==Replacement::ContextFromLambda)
            return;
        if(alternativeType.type==Replacement::ArrayWithOffsetAsBuffer || alternativeType.type==Replacement::ArrayAsBuffer){
            addNullness = true;
            options = Option(options & ~EnumAsInts);
            QScopedPointer<MetaType> cpy(java_argument->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(MetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            MetaBuilder::decideUsagePattern(cpy.get());
            addArrayOffset = alternativeType.type==Replacement::ArrayWithOffsetAsBuffer;
            modified_type = qualifiedJavaType(cpy.get(), java_function->implementingClass(), Option((options & ~UseNativeIds) | NoSuppressExports));
            if(addArrayOffset){
                addArrayLength = true;
                modified_type += "[]";
            }else{
                if(!(options & VarArgsAsArray) && java_function->useArgumentAsVarArgs(java_argument->argumentIndex() + 1)){
                    modified_type += "...";
                }else{
                    modified_type += "[]";
                }
            }
        }else{
            modified_type = alternativeType.typeName;
        }
    }else if(java_function->useArgumentAsBuffer(java_argument->argumentIndex() + 1)){
        addNullness = true;
        options = Option(options & ~EnumAsInts);
        QScopedPointer<MetaType> cpy(java_argument->type()->copy());
        cpy->setConstant(false);
        cpy->setReferenceType(MetaType::NoReference);
        QList<bool> indirections = cpy->indirections();
        if(!indirections.isEmpty()){
            indirections.removeLast();
            cpy->setIndirections(indirections);
        }
        MetaBuilder::decideUsagePattern(cpy.get());
        modified_type = qualifiedJavaType(cpy.get(), java_function->implementingClass(), Option(options & ~UseNativeIds));
        if(modified_type==QStringLiteral(u"int")){
            modified_type = QStringLiteral(u"java.nio.IntBuffer");
        }else if(modified_type==QStringLiteral(u"byte")){
            modified_type = QStringLiteral(u"java.nio.ByteBuffer");
        }else if(modified_type==QStringLiteral(u"char")){
            modified_type = QStringLiteral(u"java.nio.CharBuffer");
        }else if(modified_type==QStringLiteral(u"short")){
            modified_type = QStringLiteral(u"java.nio.ShortBuffer");
        }else if(modified_type==QStringLiteral(u"long")){
            modified_type = QStringLiteral(u"java.nio.LongBuffer");
        }else if(modified_type==QStringLiteral(u"float")){
            modified_type = QStringLiteral(u"java.nio.FloatBuffer");
        }else if(modified_type==QStringLiteral(u"double")){
            modified_type = QStringLiteral(u"java.nio.DoubleBuffer");
        }else{
            modified_type = QStringLiteral(u"java.nio.Buffer");
        }
    }else if(java_function->useArgumentAsArray(java_argument->argumentIndex() + 1)){
        addNullness = true;
        addArrayOffset = java_function->insertUtilArgument(java_argument->argumentIndex() + 1);
        options = Option(options & ~EnumAsInts);
        QScopedPointer<MetaType> cpy(java_argument->type()->copy());
        cpy->setConstant(false);
        cpy->setReferenceType(MetaType::NoReference);
        QList<bool> indirections = cpy->indirections();
        if(!indirections.isEmpty()){
            indirections.removeLast();
            cpy->setIndirections(indirections);
        }
        MetaBuilder::decideUsagePattern(cpy.get());
        modified_type = qualifiedJavaType(cpy.get(), java_function->implementingClass(), Option((options & ~UseNativeIds) | NoSuppressExports));
        if(!addArrayOffset && !(options & VarArgsAsArray) && java_function->useArgumentAsVarArgs(java_argument->argumentIndex() + 1)){
            modified_type += "...";
        }else{
            modified_type += "[]";
        }
    }else {
        addNullness = true;
        modified_type = java_function->typeReplaced(java_argument->argumentIndex() + 1);
        if(modified_type.isEmpty()){
            if(java_function->useArgumentAsSlotContext(java_argument->argumentIndex() + 1)){
                if (options & NoNullness) {
                    modified_type = QStringLiteral(u"io.qt.core.QObject");
                }else{
                    modified_type = QStringLiteral(u"io.qt.core.@Nullable QObject");
                }
            }
        }
        if((options & SkipTemplateParameters) && modified_type.contains("<")){
            auto idx = modified_type.indexOf(u'<');
            QString arrayType;
            if(modified_type.endsWith(QStringLiteral(u"..."))){
                if((options & VarArgsAsArray)){
                    arrayType = QStringLiteral(u"[]");
                }else{
                    arrayType = QStringLiteral(u"...");
                }
            }else if(modified_type.endsWith(QStringLiteral(u"[]"))){
                arrayType = QStringLiteral(u"[]");
            }
            modified_type = modified_type.mid(0, idx)+arrayType;
        }else if((options & VarArgsAsArray) && modified_type.endsWith(QStringLiteral(u"..."))){
            modified_type = modified_type.replace(QStringLiteral(u"..."), QStringLiteral(u"[]"));
        }
    }
    QString arg;

    bool suppressWarning = false;
    if (modified_type.isEmpty()){
        if(!(options & NoNullness) && java_argument->isNullPointerDisabled(java_function)){
            options = Option(options | StrictNonNull);
        }
        if(isCharSequenceSubstitute(java_argument->type())){
            if(options & NoNullness){
                arg = QStringLiteral(u"java.lang.CharSequence");
            }else{
                if(options & StrictNonNull){
                    arg = QStringLiteral(u"java.lang.@StrictNonNull CharSequence");
                }else if(java_argument->type()->indirections().isEmpty()){
                    arg = QStringLiteral(u"java.lang.@NonNull CharSequence");
                }else{
                    arg = QStringLiteral(u"java.lang.@Nullable CharSequence");
                }
            }
        }else{
            arg = qualifiedJavaType(java_argument->type(), java_function->implementingClass(), Option(options));
        }

        if((options & SkipTemplateParameters) == 0 && java_argument->type()->typeEntry()->isComplex()){
            const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(java_argument->type()->typeEntry());
            if(ctype->isGenericClass() && java_argument->type()->instantiations().isEmpty()){
                suppressWarning = true;
            }
        }
    }else{
        registerPackage(modified_type);
        modified_type = modified_type.replace(u'$', u'.');

        if (!(options & NoNullness)) {
            if(addNullness){
                if(java_function->nullPointersDisabled(java_function->declaringClass(), java_argument->argumentIndex() + 1)){
                    if(modified_type.endsWith(QStringLiteral(u"[]"))){
                        modified_type = modified_type.mid(0, modified_type.length()-2) + " @StrictNonNull[]";
                    }else if(modified_type.endsWith(QStringLiteral(u"..."))){
                        modified_type = modified_type.mid(0, modified_type.length()-3) + " @StrictNonNull...";
                    }else{
                        auto idx = modified_type.indexOf(u'<');
                        QString package;
                        QString typeName = modified_type;
                        if(idx>0){
                            typeName = modified_type.mid(0, idx);
                        }
                        idx = typeName.lastIndexOf(u'.');
                        if(idx>0){
                            typeName = modified_type.mid(idx+1);
                            package = modified_type.mid(0, idx+1);
                            modified_type = package + QStringLiteral(u"@StrictNonNull ") + typeName;
                        }else{
                            modified_type = QStringLiteral(u"@StrictNonNull ") + modified_type;
                        }
                    }
                }else if(modified_type.startsWith(QStringLiteral(u"java.nio.")) && !modified_type.contains(QStringLiteral(u"@"))){
                    auto idx = modified_type.indexOf(u'<');
                    QString package;
                    QString typeName = modified_type;
                    if(idx>0){
                        typeName = modified_type.mid(0, idx);
                    }
                    idx = typeName.lastIndexOf(u'.');
                    if(idx>0){
                        typeName = modified_type.mid(idx+1);
                        package = modified_type.mid(0, idx+1);
                        modified_type = package + QStringLiteral(u"@Nullable ") + typeName;
                    }else{
                        modified_type = QStringLiteral(u"@Nullable ") + modified_type;
                    }
                }else if(modified_type==QStringLiteral(u"java.lang.String") || modified_type==QStringLiteral(u"String")){
                    if(java_argument->type()
                            && java_argument->type()->typeEntry()->qualifiedCppName()==QStringLiteral(u"char")
                            && java_argument->type()->indirections().size()==1
                            && !java_function->hasConversionRule(TS::Language::NativeCode, java_argument->argumentIndex() + 1)){
                        auto idx = modified_type.indexOf(u'<');
                        QString package;
                        QString typeName = modified_type;
                        if(idx>0){
                            typeName = modified_type.mid(0, idx);
                        }
                        idx = typeName.lastIndexOf(u'.');
                        QString ann = QStringLiteral(u"@Nullable ");
                        if(idx>0){
                            typeName = modified_type.mid(idx+1);
                            package = modified_type.mid(0, idx+1);
                            modified_type = package + ann + typeName;
                        }else{
                            modified_type = ann + modified_type;
                        }
                    }
                }
            }
        }else if(modified_type.contains(u'@')){
            modified_type = annotationFreeTypeName(modified_type);
        }
        arg = modified_type;
    }
    if ((options & SkipTemplateParameters) == SkipTemplateParameters) {
        auto idx = arg.indexOf(u'<');
        if(idx>0)
            arg = arg.left(idx);
    }

    if(suppressWarning)
        arg = QStringLiteral(u"@SuppressWarnings(\"rawtypes\") ") + arg;
    if ((options & SkipName) == 0) {
        arg += ' ';
        arg += java_argument->modifiedArgumentName();
    }

    if (commaRequired)
        s << ", ";
    commaRequired = true;
    s << arg;

    if(addArrayOffset){
        int lengthParameter = java_function->utilArgumentIndex(java_argument->argumentIndex() + 1);
        if(lengthParameter>0 && lengthParameter<=java_function->arguments().size()){
            const MetaArgument *lengthParam = java_function->arguments()[lengthParameter - 1];
            if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                lengthParam = nullptr;
                for(const MetaArgument *argument : java_function->arguments()) {
                    if(argument && argument->argumentIndex()+1==lengthParameter){
                        lengthParam = argument;
                    }
                }
            }
            if(alternativeTypes){
                if(Replacement lengthType = alternativeTypes->value(lengthParam->argumentIndex())){
                    if(lengthType.type==Replacement::RemoveArgument || lengthType.type==Replacement::ContextFromLambda)
                        lengthParam = nullptr;
                }
            }
            if(lengthParam && (addArrayLength || java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No)){
                s << ", ";
                QString argumentType = java_function->typeReplaced(lengthParam->argumentIndex() + 1);
                if(!argumentType.isEmpty()){
                    argumentType = argumentType.replace(u'$', u'.');
                    if (!(options & NoNullness)) {
                        if(java_function->nullPointersDisabled(java_function->declaringClass(), java_argument->argumentIndex() + 1)){
                            if(argumentType.endsWith(QStringLiteral(u"[]"))){
                                argumentType = argumentType.mid(0, argumentType.length()-2) + " @StrictNonNull[]";
                            }else if(argumentType.endsWith(QStringLiteral(u"..."))){
                                argumentType = argumentType.mid(0, argumentType.length()-3) + " @StrictNonNull...";
                            }else{
                                auto idx = argumentType.indexOf(u'<');
                                QString package;
                                QString typeName = argumentType;
                                if(idx>0){
                                    typeName = argumentType.mid(0, idx);
                                }
                                idx = typeName.lastIndexOf(u'.');
                                if(idx>0){
                                    typeName = argumentType.mid(idx+1);
                                    package = argumentType.mid(0, idx+1);
                                    argumentType = package + QStringLiteral(u"@StrictNonNull ") + typeName;
                                }else{
                                    argumentType = QStringLiteral(u"@StrictNonNull ") + argumentType;
                                }
                            }
                        }else if(argumentType==QStringLiteral(u"java.lang.String") || argumentType==QStringLiteral(u"String")){
                            if(java_argument->type()
                                && java_argument->type()->typeEntry()->qualifiedCppName()==QStringLiteral(u"char")
                                && java_argument->type()->indirections().size()==1
                                && !java_function->hasConversionRule(TS::Language::NativeCode, java_argument->argumentIndex() + 1)){
                                auto idx = argumentType.indexOf(u'<');
                                QString package;
                                QString typeName = argumentType;
                                if(idx>0){
                                    typeName = argumentType.mid(0, idx);
                                }
                                idx = typeName.lastIndexOf(u'.');
                                QString ann = QStringLiteral(u"@Nullable ");
                                if(idx>0){
                                    typeName = argumentType.mid(idx+1);
                                    package = argumentType.mid(0, idx+1);
                                    argumentType = package + ann + typeName;
                                }else{
                                    argumentType = ann + argumentType;
                                }
                            }
                        }
                    }else if(argumentType.contains(u'@')){
                        argumentType = annotationFreeTypeName(argumentType);
                    }
                }else{
                    if(!(options & NoNullness) && lengthParam->isNullPointerDisabled(java_function)){
                        options = Option(options | StrictNonNull);
                    }
                    if(isCharSequenceSubstitute(lengthParam->type())){
                        if(options & NoNullness){
                            argumentType = QStringLiteral(u"java.lang.CharSequence");
                        }else{
                            if(options & StrictNonNull){
                                argumentType = QStringLiteral(u"java.lang.@StrictNonNull CharSequence");
                            }else if(lengthParam->type()->indirections().isEmpty()){
                                argumentType = QStringLiteral(u"java.lang.@NonNull CharSequence");
                            }else{
                                argumentType = QStringLiteral(u"java.lang.@Nullable CharSequence");
                            }
                        }
                    }else{
                        argumentType = qualifiedJavaType(lengthParam->type(), java_function->implementingClass(), Option(options));
                    }
                }
                if(addArrayLength){
                    argumentType = "int";
                }
                s << argumentType;
                if (!(options & SkipName)){
                    s << " offsetOf";
                    QString modifiedArgumentName = java_argument->modifiedArgumentName();
                    modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                    s << modifiedArgumentName;
                }
                if(addArrayLength){
                    s << ", " << argumentType;
                    if (!(options & SkipName)){
//                        s << " lengthOf";
//                        QString modifiedArgumentName = java_argument->modifiedArgumentName();
//                        modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                        s << " " << lengthParam->modifiedArgumentName();
                    }
                }
            }
        }
    }
}


void JavaGenerator::writeIntegerEnum(QTextStream &s, const uint size, const MetaEnum *java_enum) {
    const MetaEnumValueList &values = java_enum->values();

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
    s << "class " << java_enum->name() << "{" << Qt::endl
      << INDENT << "static {" << Qt::endl
      << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl
      << INDENT << "}" << Qt::endl << Qt::endl;
    for (int i = 0; i < values.size(); ++i) {
        MetaEnumValue *value = values.at(i);

        if (java_enum->typeEntry()->isEnumValueRemoveRejected(value->name()))
            continue;

        if(value->deprecated()){
            if(!value->deprecatedComment().isEmpty()){
                s << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * @deprecated " << encodeHtml(value->deprecatedComment()) << Qt::endl
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

void JavaGenerator::writeFunctional(QTextStream &s, const MetaFunctional *java_functional) {
    if(java_functional->isPrivate() || (java_functional->typeEntry()->codeGeneration() & ~TypeEntry::InheritedByTypeSystem)==TypeEntry::GenerateNothing){
        return;
    }

    QString comment;
    {
        QTextStream commentStream(&comment);
        bool hasIntro = false;
        {
            Indentor indentor;
            for(const CodeSnip& snip : java_functional->typeEntry()->codeSnips()){
                if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::CommentIntro){
                    snip.formattedCode(commentStream, indentor);
                    hasIntro = true;
                }
            }
        }
        if(!hasIntro || !java_functional->href().isEmpty()){
            commentStream << "<p>Java wrapper for Qt callable <code>";
            if(!java_functional->href().isEmpty())
                commentStream << "<a href=\"" << docsUrl << java_functional->href() << "\">";
            commentStream << encodeHtml(java_functional->typeEntry()->qualifiedCppName());
            if(!java_functional->href().isEmpty())
                commentStream << "</a>";
            commentStream << "</code></p>" << Qt::endl;
        }
        if(!java_functional->since().isEmpty()){
            commentStream << "@since This type was introduced in Qt " << java_functional->since() << "." << Qt::endl;
        }
        {
            Indentor indentor;
            for(const CodeSnip& snip : java_functional->typeEntry()->codeSnips()){
                if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::Comment){
                    snip.formattedCode(commentStream, indentor);
                }
            }
        }
    }

    int returnArrayLengthIndex = java_functional->utilArgumentIndex(0);
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
        s << INDENT << "public final class " << java_functional->name() << " extends QtObject";
        if(!java_functional->javaFunctionalInterface().isEmpty())
            s << " implements ";
    }else{
        s << INDENT << "@FunctionalInterface" << Qt::endl
          << INDENT << "public interface " << java_functional->name() << " extends QtObjectInterface";
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
                        s << qualifiedJavaType(java_functional->type(), nullptr, Option(BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers)) << "[]";
                    }else{
                        if(!replacedReturnType.isEmpty()){
                            registerPackage(replacedReturnType);
                            s << replacedReturnType.replace(u'$', u'.');
                        }else
                            s << qualifiedJavaType(java_functional->type(), nullptr, Option(BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers));
                    }
                }else{
                    Q_ASSERT(index<=uint(java_functional->arguments().size()));
                    MetaArgument * arg = java_functional->arguments()[index-1];
                    int arrayLengthIndex = java_functional->utilArgumentIndex(arg->argumentIndex() + 1);
                    if(arrayLengthIndex>=0){
                        s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                    }else{
                        QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex() + 1);
                        if(!replacedArgType.isEmpty()){
                            registerPackage(replacedArgType);
                            s << replacedArgType.replace(u'$', u'.');
                        }else
                            s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection));
                    }
                }
                ++counter;
            }
            s << ">";
        }
    }
    s << " {" << Qt::endl << Qt::endl;
    {
        INDENTATION(INDENT);
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
                    s << qualifiedJavaType(java_functional->type(), nullptr, Option(VarArgsAsArray | NoQCollectionContainers)) << "[]";
                }else{
                    if(!replacedReturnType.isEmpty()){
                        registerPackage(replacedReturnType);
                        s << replacedReturnType.replace(u'$', u'.');
                    }else
                        s << qualifiedJavaType(java_functional->type(), nullptr, Option(VarArgsAsArray | NoQCollectionContainers));
                }
                s << " ";
                if(java_functional->typeEntry()->functionName().isEmpty())
                    s << "call";
                else
                    s << java_functional->typeEntry()->functionName();
                s << "(";
                int counter = 0;
                for(MetaArgument * arg : java_functional->arguments()){
                    if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                        continue;
                    if(counter!=0)
                        s << ", ";
                    int arrayLengthIndex = java_functional->utilArgumentIndex(arg->argumentIndex() + 1);
                    if(arrayLengthIndex>=0){
                        s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                    }else{
                        QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex() + 1);
                        if(!replacedArgType.isEmpty()){
                            registerPackage(replacedArgType);
                            s << replacedArgType.replace(u'$', u'.');
                        }else
                            s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection));
                    }
                    s << " " << arg->modifiedArgumentName();
                    ++counter;
                }

                for(const QString& pkg : qAsConst(m_exportedPackages)){
                    if(pkg!=java_functional->package()
                        && !pkg.isEmpty()
                        && m_typeSystemByPackage[pkg]
                        && m_typeSystemByPackage[pkg]->module()!=QStringLiteral(u"qtjambi") // because it's transitive
                        && m_typeSystemByPackage[pkg]!=m_typeSystemByPackage[java_functional->package()]
                        && !pkg.startsWith(QStringLiteral(u"io.qt.internal"))
                        && pkg!=QStringLiteral(u"io.qt")){
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
                        INDENTATION(INDENT);
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
                                    s << "new " << java_functional->type()->typeEntry()->qualifiedTargetLangName().replace(u'$', u'.') << "(";
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
                        for(MetaArgument * arg : java_functional->arguments()){
                            if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                                continue;
                            if(needsComma)
                                s << ", ";
                            if(!java_functional->typeReplaced(arg->argumentIndex()+1).isEmpty()
                                    || java_functional->useArgumentAsArray(arg->argumentIndex()+1)
                                || java_functional->useArgumentAsBuffer(arg->argumentIndex()+1)){
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
                        s << qualifiedJavaType(java_functional->type(), nullptr, Option(NoNullness | VarArgsAsArray | EnumAsInts | NoQCollectionContainers)) << "[]";
                    }else{
                        if(!replacedReturnType.isEmpty())
                            s << replacedReturnType;
                        else
                            s << qualifiedJavaType(java_functional->type(), nullptr, Option(NoNullness | VarArgsAsArray | EnumAsInts | NoQCollectionContainers));
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
                    for(MetaArgument * arg : java_functional->arguments()){
                        if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                            continue;
                        if(hasArg)
                            s << ", ";
                        int arrayLengthIndex = java_functional->utilArgumentIndex(arg->argumentIndex()+1);
                        if(arrayLengthIndex>=0){
                            s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                        }else{
                            QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex()+1);
                            if(!replacedArgType.isEmpty()){
                                registerPackage(replacedArgType);
                                s << replacedArgType.replace(u'$', u'.');
                            }else if(java_functional->useArgumentAsArray(arg->argumentIndex()+1)){
                                s << qualifiedJavaType(arg->type(), nullptr, Option(NoNullness | VarArgsAsArray | CollectionAsCollection | EnumAsInts)) << "[]";
                            }else{
                                s << qualifiedJavaType(arg->type(), nullptr, Option(NoNullness | VarArgsAsArray | CollectionAsCollection | UseNativeIds | EnumAsInts));
                            }
                        }
                        s << " " << arg->modifiedArgumentName();
                        hasArg = true;
                    }
                }
            }
            s << ");" << Qt::endl << Qt::endl;
        }

        if(!(java_functional->typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)){
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * @hidden" << Qt::endl
              << INDENT << " * <p>Implementor class for interface {@link " << java_functional->name().replace("$", ".") << "}</p>" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public static abstract class Impl extends QtObject implements " << java_functional->name() << "{" << Qt::endl;
            {
                INDENTATION(INDENT);
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
                  << INDENT << "@NativeAccess" << Qt::endl
                  << INDENT << "private final static class ConcreteWrapper extends Impl {" << Qt::endl;
                {
                    INDENTATION(INDENT);
                    s << INDENT << "private ConcreteWrapper(QPrivateConstructor c){" << Qt::endl
                      << INDENT << "    super(c);" << Qt::endl
                      << INDENT << "}" << Qt::endl << Qt::endl
                      << INDENT << "@Override" << Qt::endl
                      << INDENT << "public ";
                    if(!needsCallThrough)
                        s << "native ";
                    if(returnArrayLengthIndex>=0){
                        s << qualifiedJavaType(java_functional->type(), nullptr, Option(VarArgsAsArray | NoQCollectionContainers)) << "[]";
                    }else{
                        if(!replacedReturnType.isEmpty())
                            s << replacedReturnType;
                        else
                            s << qualifiedJavaType(java_functional->type(), nullptr, Option(VarArgsAsArray | NoQCollectionContainers));
                    }
                    s << " ";
                    if(java_functional->typeEntry()->functionName().isEmpty())
                        s << "call";
                    else
                        s << java_functional->typeEntry()->functionName();
                    s << "(";
                    int counter = 0;
                    for(MetaArgument * arg : java_functional->arguments()){
                        if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                            continue;
                        if(counter!=0)
                            s << ", ";
                        int arrayLengthIndex = java_functional->utilArgumentIndex(arg->argumentIndex() + 1);
                        if(arrayLengthIndex>=0){
                            s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection)) << "[]";
                        }else{
                            QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex() + 1);
                            if(!replacedArgType.isEmpty()){
                                registerPackage(replacedArgType);
                                s << replacedArgType.replace(u'$', u'.');
                            }else
                                s << qualifiedJavaType(arg->type(), nullptr, Option(CollectionAsCollection));
                        }
                        s << " " << arg->modifiedArgumentName();
                        ++counter;
                    }
                    if(needsCallThrough){
                        s << "){" << Qt::endl;
                        {
                            INDENTATION(INDENT);
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
                                        s << "new " << java_functional->type()->typeEntry()->qualifiedTargetLangName().replace(u'$', u'.') << "(";
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
                            for(MetaArgument * arg : java_functional->arguments()){
                                if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                                    continue;
                                if(needsComma)
                                    s << ", ";
                                if(!java_functional->typeReplaced(arg->argumentIndex()+1).isEmpty()
                                        || java_functional->useArgumentAsArray(arg->argumentIndex()+1)
                                    || java_functional->useArgumentAsBuffer(arg->argumentIndex()+1)){
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
                            s << qualifiedJavaType(java_functional->type(), nullptr, Option(NoNullness | VarArgsAsArray | EnumAsInts | NoQCollectionContainers)) << "[]";
                        }else{
                            if(!replacedReturnType.isEmpty())
                                s << replacedReturnType;
                            else
                                s << qualifiedJavaType(java_functional->type(), nullptr, Option(NoNullness | VarArgsAsArray | EnumAsInts | NoQCollectionContainers));
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
                        for(MetaArgument * arg : java_functional->arguments()){
                            if(java_functional->argumentRemoved(arg->argumentIndex() + 1))
                                continue;
                            if(hasArg)
                                s << ", ";
                            int arrayLengthIndex = java_functional->utilArgumentIndex(arg->argumentIndex()+1);
                            if(arrayLengthIndex>=0){
                                s << qualifiedJavaType(arg->type(), nullptr, Option(NoNullness | CollectionAsCollection)) << "[]";
                            }else{
                                QString replacedArgType = java_functional->typeReplaced(arg->argumentIndex()+1);
                                if(!replacedArgType.isEmpty()){
                                    registerPackage(replacedArgType);
                                    s << replacedArgType.replace(u'$', u'.');
                                }else if(java_functional->useArgumentAsArray(arg->argumentIndex()+1)){
                                    s << qualifiedJavaType(arg->type(), nullptr, Option(NoNullness | VarArgsAsArray | CollectionAsCollection | EnumAsInts)) << "[]";
                                }else{
                                    s << qualifiedJavaType(arg->type(), nullptr, Option(NoNullness | VarArgsAsArray | CollectionAsCollection | UseNativeIds | EnumAsInts));
                                }
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

void JavaGenerator::writeEnum(QTextStream &s, const MetaEnum *java_enum) {
    if(java_enum->isPrivate() || (java_enum->typeEntry()->codeGeneration() & ~TypeEntry::InheritedByTypeSystem)==TypeEntry::GenerateNothing){
        return;
    }

    QString comment;
    QTextStream commentStream(&comment);
    bool hasIntro = false;
    {
        Indentor indentor;
        for(const CodeSnip& snip : java_enum->typeEntry()->codeSnips()){
            if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::CommentIntro){
                snip.formattedCode(commentStream, indentor);
                hasIntro = true;
            }
        }
    }
    if(!hasIntro || !java_enum->href().isEmpty() || !java_enum->brief().isEmpty()){
        if(!java_enum->brief().isEmpty()){
            commentStream << "<p>" << encodeHtml(java_enum->brief()) << "</p>" << Qt::endl;
        }
        commentStream << "<p>Java wrapper for Qt enum <code>";
        if(!java_enum->href().isEmpty())
            commentStream << "<a href=\"" << docsUrl << java_enum->href() << "\">";
        commentStream << encodeHtml(java_enum->typeEntry()->qualifiedCppName().startsWith(QStringLiteral(u"QtJambi")) ? java_enum->name().replace(QStringLiteral(u"$"), QStringLiteral(u"::<wbr/>")) : java_enum->typeEntry()->qualifiedCppName() );
        if(!java_enum->href().isEmpty())
            commentStream << "</a>";
        commentStream << "</code></p>" << Qt::endl;
    }
    if(!java_enum->since().isEmpty()){
        commentStream << "@since This enum was introduced in Qt " << java_enum->since() << "." << Qt::endl;
    }
    if (java_enum->isDeclDeprecated()) {
        if(!java_enum->deprecatedComment().isEmpty()){
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            commentStream << "@deprecated " << encodeHtml(java_enum->deprecatedComment())
                          << Qt::endl;
        }
    }

    Indentor indentor;
    for(const CodeSnip& snip : java_enum->typeEntry()->codeSnips()){
        if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::Comment){
            snip.formattedCode(commentStream, indentor);
        }
    }

    if(!java_enum->enclosingClass() || !java_enum->enclosingClass()->isFake()){
        // Write out the QFlags if present...
        FlagsTypeEntry *flags_entry = java_enum->typeEntry()->flags();
        if (flags_entry) {
            commentStream << Qt::endl << "@see " << flags_entry->targetLangName()<< Qt::endl;
        }
    }

    comment = comment.trimmed();
    if(!comment.isEmpty()){
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

    const MetaEnumValueList &values = java_enum->values();
    EnumTypeEntry *entry = java_enum->typeEntry();
    const QMap<QString,QString>& renamedEnumValues = entry->renamedEnumValues();

    QStringList linesPos1;
    QStringList linesPos2;
    QStringList linesPos3;
    QStringList linesPos4;
    QStringList linesPos5;
    QStringList linesBegin;
    QStringList linesEnd;
    for(const CodeSnip &snip : entry->codeSnips()) {
        if (snip.language == TS::TargetLangCode) {
            if (snip.position == CodeSnip::Position1) {
                linesPos1 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position2) {
                linesPos2 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position3) {
                linesPos3 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position4) {
                linesPos4 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Position5) {
                linesPos5 << snip.code().split("\n");
            }else if (snip.position == CodeSnip::Beginning) {
                linesBegin << snip.code().split("\n");
            }else if (snip.position == CodeSnip::End){
                linesEnd << snip.code().split("\n");
            }
        }
    }

    // Check if enums in QObjects are declared in the meta object. If not
    if ((java_enum->enclosingClass()->isQObject() || java_enum->enclosingClass()->isQtNamespace())
            && !java_enum->isMetaEnum()) {
        s << INDENT << "@QtUnlistedEnum" << Qt::endl;
    }
    if (entry->isExtensible()) {
        s << INDENT << "@QtExtensibleEnum" << Qt::endl;
    }
    if (java_enum->isDeclDeprecated()) {
        s << INDENT << "@Deprecated" << Qt::endl;
    }
    QStringList rejected;
    for (int i = 0; i < values.size(); ++i) {
        MetaEnumValue *enum_value = values.at(i);
        if (java_enum->typeEntry()->isEnumValueRejected(enum_value->name()) && !java_enum->typeEntry()->isEnumValueRemoveRejected(enum_value->name())){
            rejected << QStringLiteral(u"\"%1\"").arg(renamedEnumValues.value(enum_value->name(), enum_value->name()));
        }
    }
    if(!rejected.isEmpty()){
        s << INDENT << "@QtRejectedEntries({" << rejected.join(", ") << "})" << Qt::endl;
    }

    bool force_friendly = false;
    if(java_enum->enclosingClass()){
        const MetaClass * enclosingClass = java_enum->enclosingClass();
        while(enclosingClass->enclosingClass())
            enclosingClass = enclosingClass->enclosingClass();
        force_friendly = enclosingClass->typeEntry()->isForceFriendly();
    }
    // Generates Java 1.5 type enums
    s  << INDENT;
    if(!force_friendly){
        if(java_enum->isProtected() && (java_enum->enclosingClass() && !java_enum->enclosingClass()->typeEntry()->designatedInterface()) && !java_enum->enclosingClass()->typeEntry()->isInterface()){
            s  << "protected";
        }else{
            s  << "public";
        }
    }


    QString type;
    QString enumInterface;
    switch(size){
    case 8:
        type = QStringLiteral(u"byte");
        if(java_enum->typeEntry()->flags())
            enumInterface = QStringLiteral(u"QtByteFlagEnumerator");
        else
            enumInterface = QStringLiteral(u"QtByteEnumerator");
        break;
    case 16:
        type = QStringLiteral(u"short");
        if(java_enum->typeEntry()->flags())
            enumInterface = QStringLiteral(u"QtShortFlagEnumerator");
        else
            enumInterface = QStringLiteral(u"QtShortEnumerator");
        break;
    case 32:
        type = QStringLiteral(u"int");
        if(java_enum->typeEntry()->flags())
            enumInterface = QStringLiteral(u"QtFlagEnumerator");
        else
            enumInterface = QStringLiteral(u"QtEnumerator");
        break;
    case 64:
        type = QStringLiteral(u"long");
        if(java_enum->typeEntry()->flags())
            enumInterface = QStringLiteral(u"QtLongFlagEnumerator");
        else
            enumInterface = QStringLiteral(u"QtLongEnumerator");
        break;
    default:
        type = QStringLiteral(u"int");
        if(java_enum->typeEntry()->flags())
            enumInterface = QStringLiteral(u"QtFlagEnumerator");
        else
            enumInterface = QStringLiteral(u"QtEnumerator");
        break;
    }

    s  << " enum " << java_enum->name() << " implements "+enumInterface;
    if (!java_enum->typeEntry()->implements().isEmpty()) {
        s << ", " << java_enum->typeEntry()->implements();
    }
    s << " {" << Qt::endl;

    {
        INDENTATION(INDENT);
        printExtraCode(linesPos1, s);

        MetaEnumValueList switchValues;
        bool hasStringValue = false;

        for (int i = 0; i < values.size(); ++i) {
            MetaEnumValue *enum_value = values.at(i);
            if (java_enum->typeEntry()->isEnumValueRemoveRejected(enum_value->name()))
                continue;

            //          only reject enum entries on the resolve side!
            if (!java_enum->typeEntry()->isEnumValueRejected(enum_value->name())){
                switchValues.append(enum_value);
                if(enum_value->value().userType()==QMetaType::QString){
                    hasStringValue = true;
                }
            }

            QStringList enumScope = java_enum->typeEntry()->qualifiedCppName().split("::");
            if(!java_enum->typeEntry()->isScopedEnum()){
                enumScope.removeLast();
            }
            bool hasComment = false;
            comment.clear();
            commentStream.seek(0);
            if(enum_value->deprecated()){
                if(!enum_value->deprecatedComment().isEmpty()){
                    hasComment = true;
                    commentStream << "<p>Representing <code>";
                    if(!java_enum->href().isEmpty())
                        commentStream << "<a href=\"" << docsUrl << java_enum->href() << "\">";
                    commentStream << (QStringList(enumScope) << enum_value->name()).join(QStringLiteral(u"::<wbr/>"));
                    if(!java_enum->href().isEmpty())
                        commentStream << "</a>";
                    commentStream << "</code></p>" << Qt::endl
                                  << "@deprecated " << encodeHtml(enum_value->deprecatedComment()) << Qt::endl;
                }
                s << INDENT << "@Deprecated" << Qt::endl;
            }
            if(!hasComment){
                commentStream << "<p>Representing <code>";
                if(!java_enum->href().isEmpty())
                    commentStream << "<a href=\"" << docsUrl << java_enum->href() << "\">";
                commentStream << (QStringList(enumScope) << enum_value->name()).join(QStringLiteral(u"::<wbr/>"));
                if(!java_enum->href().isEmpty())
                    commentStream << "</a>";
                commentStream << "</code></p>";
            }
            comment = comment.trimmed();
            if(!comment.isEmpty()){
                s << INDENT << "/**" << Qt::endl;
                commentStream.seek(0);
                while(!commentStream.atEnd()){
                    s << INDENT << " * " << commentStream.readLine() << Qt::endl;
                }
                s << INDENT << " */" << Qt::endl;
            }
            s << INDENT << renamedEnumValues.value(enum_value->name(), enum_value->name()) << "(";
            if(enum_value->value().userType()==QMetaType::QString){
                s << "value_" << enum_value->name() << "()";
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

        QString adds;
        {
            QTextStream s(&adds);
            printExtraCode(linesPos2, s);
        }

        s << adds.trimmed() << ";" << Qt::endl << Qt::endl;
        s << INDENT << "static {" << Qt::endl
          << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl
          << INDENT << "}" << Qt::endl;

        printExtraCode(linesBegin, s, true);

        s << Qt::endl
          << INDENT << "private " << java_enum->name() << "(" << type << " value) {" << Qt::endl
          << INDENT << "    this.value = value;" << Qt::endl
          << INDENT << "}" << Qt::endl << Qt::endl
          << INDENT << "/**" << Qt::endl
          << INDENT << " * {@inheritDoc}" << Qt::endl
          << INDENT << " */" << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "public " << type << " value() {" << Qt::endl
          << INDENT << "    return value;" << Qt::endl
          << INDENT << "}" << Qt::endl << Qt::endl;

        // Write out the create flags function if its a QFlags enum
        if (entry->flags()) {
            FlagsTypeEntry *flags_entry = entry->flags();
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * Create a " << flags_entry->targetLangName() << " of the enum entry." << Qt::endl
              << INDENT << " * @return " << flags_entry->targetLangName() << "" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "public @NonNull " << flags_entry->targetLangName() << " asFlags() {" << Qt::endl
              << INDENT << "    return new " << flags_entry->targetLangName() << "(value);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * Combines this entry with other enum entry." << Qt::endl
              << INDENT << " * @param e enum entry" << Qt::endl
              << INDENT << " * @return new flag" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public @NonNull " << flags_entry->targetLangName() << " combined(@NonNull "
              << entry->targetLangName().replace("$",".") << " e) {" << Qt::endl
              << INDENT << "    return asFlags().setFlag(e, true);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * Excludes other enum entry from a flag of this entry." << Qt::endl
              << INDENT << " * @param e enum entry" << Qt::endl
              << INDENT << " * @return new flag" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public @NonNull " << flags_entry->targetLangName() << " cleared(@NonNull "
              << entry->targetLangName().replace("$",".") << " e) {" << Qt::endl
              << INDENT << "    return asFlags().setFlag(e, false);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * Creates a new {@link " << flags_entry->targetLangName() << "} from the entries."  << Qt::endl
              << INDENT << " * @param values entries"  << Qt::endl
              << INDENT << " * @return new flag"  << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public static @NonNull " << flags_entry->targetLangName() << " flags(@Nullable "
              << entry->targetLangName().replace("$",".") << " @NonNull... values) {" << Qt::endl
              << INDENT << "    return new " << flags_entry->targetLangName() << "(values);" << Qt::endl
              << INDENT << "}" << Qt::endl << Qt::endl;
        }



        QString numberType;
        if (entry->isExtensible()) {
            switch(size){
            case 8:
                numberType = QStringLiteral(u"java.lang.Byte");
                break;
            case 16:
                numberType = QStringLiteral(u"java.lang.Short");
                break;
            case 32:
                numberType = QStringLiteral(u"java.lang.Integer");
                break;
            case 64:
                numberType = QStringLiteral(u"java.lang.Long");
                break;
            default:
                numberType = QStringLiteral(u"java.lang.Integer");
                break;
            }
        }

        s << INDENT << "/**" << Qt::endl;
        s << INDENT << " * Returns the corresponding enum entry for the given value." << Qt::endl;
        s << INDENT << " * @param value" << Qt::endl;
        s << INDENT << " * @return enum entry" << Qt::endl;
        if (entry->isExtensible()) {
            s << INDENT << " * @throws QNoSuchEnumValueException if value not existent in the enum" << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
        s << INDENT << "public static @NonNull " << java_enum->name() << " resolve(" << type << " value) {" << Qt::endl;
        {
            INDENTATION(INDENT);
            if(size==64 || hasStringValue){
                for (int i = 0; i < switchValues.size(); ++i) {
                    MetaEnumValue *e = switchValues.at(i);
                    if(i>0){
                        s << INDENT << "}else ";
                    }else{
                        s << INDENT;
                    }
                    s << "if(value==";
                    if(e->value().userType()==QMetaType::QString){
                        s << e->name() << ".value){" << Qt::endl;
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
                    MetaEnumValue *e = switchValues.at(i);

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
                s << "throw new QNoSuchEnumValueException(value);" << Qt::endl;
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
              << INDENT << " * @throws QNoSuchEnumValueException if value not existent in the enum or name does not match." << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "public static @NonNull " << java_enum->name() << " resolve(" << type << " value, @Nullable String name) {" << Qt::endl;
            {
                INDENTATION(INDENT);
                s << INDENT << "if(name==null || name.isEmpty())" << Qt::endl
                  << INDENT << "    return resolve(value);" << Qt::endl
                  << INDENT << "else" << Qt::endl
                  << INDENT << "    return QtJambi_LibraryUtilities.internal.resolveEnum(" << java_enum->name() << ".class, value, name);" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl;
        }

        bool wendl = false;
        for (int i = 0; i < values.size(); ++i) {
            MetaEnumValue *enum_value = values.at(i);
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
                s << "value_" << enum_value->name() << "();";
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
            bool is64 = java_enum->typeEntry()->size()==64 && QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)>=QT_VERSION_CHECK(6,9,0);
            const char* valueMethod = is64 ? "longValue" : "value";
            const char* valueType = is64 ? "long" : "int";
            const char* valueBoxedType = is64 ? "Long" : "Integer";
            const char* javaType = is64 ? "QLongFlags" : "QFlags";
            s << INDENT << "/**" << Qt::endl
              << INDENT << " * {@link " << javaType << "} type for enum {@link " << java_enum->name().replace("$",".") << "}" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT;
            if(!force_friendly){
                if(java_enum->isProtected() && (java_enum->enclosingClass() && !java_enum->enclosingClass()->typeEntry()->designatedInterface()) && !java_enum->enclosingClass()->typeEntry()->isInterface()){
                    s << "protected ";
                }else{
                    s << "public ";
                }
            }
            s << "static final class " << flagsName << " extends " << javaType << "<" << java_enum->name().replace("$",".") << "> implements Comparable<" << flagsName << "> {" << Qt::endl;
            {
                INDENTATION(INDENT);
                s << INDENT << "private static final long serialVersionUID = 0x" << QString::number(serialVersionUID, 16) << "L;" << Qt::endl;
                printExtraCode(linesPos1, s, true);
                s << INDENT << "static {" << Qt::endl
                  << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a new " << flagsName << "." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public " << flagsName << "(){" << Qt::endl
                  << INDENT << "    this(0);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a new " << flagsName << " where the flags in <code>args</code> are set." << Qt::endl
                  << INDENT << " * @param args enum entries" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public " << flagsName << "(@Nullable " << java_enum->name().replace("$",".") << " @NonNull... args){" << Qt::endl
                  << INDENT << "    this(0);" << Qt::endl
                  << INDENT << "    set(args);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a new " << flagsName << " with given <code>value</code>." << Qt::endl
                  << INDENT << " * @param value" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public " << flagsName << "(" << valueType << " value) {" << Qt::endl
                  << INDENT << "    super(value);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Combines this flags with enum entry." << Qt::endl
                  << INDENT << " * @param e enum entry" << Qt::endl
                  << INDENT << " * @return new " << flagsName << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public final @NonNull " << flagsName << " combined(@StrictNonNull " << java_enum->name().replace("$",".") << " e){" << Qt::endl
                  << INDENT << "    return new " << flagsName << "(" << valueMethod << "() | e.value());" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Sets the flag <code>e</code>" << Qt::endl
                  << INDENT << " * @param e enum entry" << Qt::endl
                  << INDENT << " * @return this" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public final @NonNull " << flagsName << " setFlag(@Nullable " << java_enum->name().replace("$",".") << " e){" << Qt::endl
                  << INDENT << "    return setFlag(e, true);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Sets or clears the flag <code>flag</code>" << Qt::endl
                  << INDENT << " * @param e enum entry" << Qt::endl
                  << INDENT << " * @param on set (true) or clear (false)" << Qt::endl
                  << INDENT << " * @return this" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public final @NonNull " << flagsName << " setFlag(@Nullable " << java_enum->name().replace("$",".") << " e, boolean on){" << Qt::endl
                  << INDENT << "    if (on) {" << Qt::endl
                  << INDENT << "    	setValue(" << valueMethod << "() | e.value());" << Qt::endl
                  << INDENT << "    }else {" << Qt::endl
                  << INDENT << "    	setValue(" << valueMethod << "() & ~e.value());" << Qt::endl
                  << INDENT << "    }" << Qt::endl
                  << INDENT << "    return this;" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns an array of flag objects represented by this " << flagsName << "." << Qt::endl
                  << INDENT << " * @return array of enum entries" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public final @NonNull " << java_enum->name().replace("$",".") << " @NonNull[] flags(){" << Qt::endl
                  << INDENT << "    return super.flags(" << java_enum->name().replace("$",".") << ".values());" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * {@inheritDoc}" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public final @NonNull " << flagsName << " clone(){" << Qt::endl
                  << INDENT << "        return new " << flagsName << "(" << valueMethod << "());" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * {@inheritDoc}" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public final int compareTo(@StrictNonNull " << flagsName << " other){" << Qt::endl
                  << INDENT << "    return " << valueBoxedType << ".compare(" << valueMethod << "(), other." << valueMethod << "());" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the value of this QFlags." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << valueType << " value(){" << Qt::endl;
                if(is64){
                    s << INDENT << "    return longValue();" << Qt::endl;
                }else{
                    s << INDENT << "    return intValue();" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
                if(is64){
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * See <a href=\"https://doc.qt.io/qt/qflags.html#toInt\">QFlags::toInt() const</a>" << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "public final " << valueType << " toLong(){" << Qt::endl
                      << INDENT << "    return longValue();" << Qt::endl
                      << INDENT << "}" << Qt::endl << Qt::endl;
                }
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * Sets the value of this QFlags." << Qt::endl
                  << INDENT << " * @param value new value" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final void setValue(" << valueType << " value){" << Qt::endl;
                if(is64){
                    s << INDENT << "    setLongValue(value);" << Qt::endl;
                }else{
                    s << INDENT << "    setIntValue(value);" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
                printExtraCode(linesPos4, s, true);
                printExtraCode(linesPos5, s, true);
            }
            s << INDENT << "}" << Qt::endl
              << INDENT << Qt::endl;
        }
    }
}

void JavaGenerator::writePrivateNativeFunction(QTextStream &s, const MetaFunction *java_function) {
    uint exclude_attributes = MetaAttributes::Public | MetaAttributes::Protected | MetaAttributes::Override;
    uint include_attributes = 0;

    include_attributes |= MetaAttributes::Private | MetaAttributes::Native;
    if(java_function->declaringClass()->isInterface() || java_function->isConstructor()){
        include_attributes |= MetaAttributes::Static;
    }

    writeFunctionAttributes(s, java_function, -1, {}, include_attributes, exclude_attributes,
                            Option(NoNullness | EnumAsInts | VarArgsAsArray
                            | (java_function->isConstructor() ? SkipTemplateParameters : NoOption)
                            | ( (
                                  java_function->isNormal()
                               || java_function->isSignal() ) ? NoOption : SkipReturnType)));
    if (java_function->isConstructor()){
        const QList<Parameter> addedParameterTypes = java_function->addedParameterTypes();
        if((java_function->declaringClass()->typeEntry()->isGenericClass()
                && java_function->declaringClass()->templateBaseClass()
                && java_function->declaringClass()->templateBaseClass()->templateArguments().size()>0) || !addedParameterTypes.isEmpty()){
            s << "<";
            bool first = true;
            if(java_function->declaringClass()->templateBaseClass()){
                for(TypeEntry * t : java_function->declaringClass()->templateBaseClass()->templateArguments()){
                    if(first){
                        first = false;
                    }else{
                        s << ",";
                    }
                    s << t->name();
                }
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
    bool needsComma = false;
    const MetaArgumentList& arguments = java_function->arguments();

    if(java_function->isConstructor()){
        if(needsComma)
            s << ", ";
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
        else if(java_function->implementingClass()->typeEntry()->isQMetaObjectType()){
            s << "long __this__persistentPointer";
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
        s << QString(argumentMod.modified_type).replace(u'$', u'.') << " " << argumentMod.modified_name;
        ++argumentCounter;
    }

    Option _option = java_function->isFinal() ? NoOption : VirtualCall;
    for (int i = 0; i < arguments.count(); ++i) {
        const MetaArgument *arg = arguments.at(i);

        if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
            if(java_function->isConstructor()){
                writeFunctionArgument(s, java_function, arg, needsComma, nullptr, Option(_option | NoNullness | IsNativeCall | CollectionAsCollection | VarArgsAsArray));
            }else if ((!arg->type()->hasNativeId()
                      || !java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()
                      || java_function->useArgumentAsArray(arg->argumentIndex()+1)
                      || java_function->useArgumentAsBuffer(arg->argumentIndex()+1)
                         ) && !java_function->useArgumentAsSlotContext(arg->argumentIndex()+1)){
                writeFunctionArgument(s, java_function, arg, needsComma, nullptr, Option(_option | NoNullness | IsNativeCall | EnumAsInts | UseNativeIds | CollectionAsCollection | VarArgsAsArray));
            }else{
                if (needsComma)
                    s << ", ";
                needsComma = true;
                s << "long " << arg->modifiedArgumentName();
            }
        }
        ++argumentCounter;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& argumentMod = addedArguments.first[argumentCounter];
            if(needsComma)
                s << ", ";
            needsComma = true;
            s << QString(argumentMod.modified_type).replace(u'$', u'.') << " " << argumentMod.modified_name;
            ++argumentCounter;
        }
    }
    for(const ArgumentModification& argumentMod : addedArguments.second){
        if(needsComma)
            s << ", ";
        needsComma = true;
        registerPackage(argumentMod.modified_type);
        s << QString(argumentMod.modified_type).replace(u'$', u'.') << " " << argumentMod.modified_name;
    }
    s << ")";

    QString throws = java_function->throws();
    if(!throws.isEmpty()){
        s << " throws " << throws << " ";
    }
    s << ";" << Qt::endl
      << INDENT << Qt::endl;
}

void JavaGenerator::writeFunctionCallForOwnership(QTextStream &s, const MetaFunction *, TS::Ownership owner, const QString& variable) {
    if (owner == TS::CppOwnership) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.setCppOwnership(" << variable << ");" << Qt::endl;
    } else if (owner == TS::TargetLangOwnership) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.setJavaOwnership(" << variable << ");" << Qt::endl;
    } else if (owner == TS::DefaultOwnership) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.setDefaultOwnership(" << variable << ");" << Qt::endl;
    } else if (owner == TS::Invalidate) {
        s << INDENT<< "QtJambi_LibraryUtilities.internal.invalidateObject(" << variable << ");" << Qt::endl;
    }
}

void JavaGenerator::writeOwnershipForContainer(QTextStream &s, const MetaFunction *java_function, TS::Ownership owner, MetaType *type, const QString &arg_name) {
    Q_ASSERT(type->isContainer());

    if(owner!=TS::IgnoreOwnership && owner!=TS::InvalidOwnership){
        s << INDENT << "for (" << type->instantiations().at(0)->fullName() << " i : "
          << arg_name << ")" << Qt::endl
          << "    ";
        writeFunctionCallForOwnership(s, java_function, owner, "i");
    }
}

static FunctionModificationList get_function_modifications_for_class_hierarchy(const MetaFunction *java_function, bool reverse) {
    FunctionModificationList mods;
    const MetaClass *cls = java_function->implementingClass();
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

bool JavaGenerator::hasCodeInjections(const MetaFunction *java_function,
                                      const QSet<CodeSnip::Position>& positions) {
    const MetaClass *cls = java_function->implementingClass();
    while (cls!= nullptr) {
        if(java_function->hasCodeInjections(cls, TS::TargetLangCode, positions))
            return true;
        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }
    return false;
}

void JavaGenerator::writeInjectedCode(QTextStream &s, const MetaFunction *java_function,
                                      CodeSnip::Position position) {
    FunctionModificationList mods = get_function_modifications_for_class_hierarchy(java_function,
                                                                                   position==CodeSnip::Position1
                                                                                   || position==CodeSnip::Position2
                                                                                   || position==CodeSnip::Beginning
                                                                                );

    for(const FunctionModification& mod : qAsConst(mods)) {
        if (mod.snips.count() <= 0)
            continue ;

        for(const CodeSnip& snip : mod.snips) {
            if (snip.position != position)
                continue ;

            if (snip.language != TS::TargetLangCode)
                continue ;

            QString code = snip.code();
            ArgumentMap map = snip.argumentMap;
            ArgumentMap::iterator it = map.begin();
            const MetaArgumentList& arguments = java_function->arguments();
            for (;it != map.end();++it) {
                int pos = it.key() - 1;
                QString meta_name = it.value();

                if(pos == -1){
                    code = code.replace(meta_name, QStringLiteral(u"__qt_return_value"));
                }else if (pos >= 0 && pos < arguments.count()) {
                    code = code.replace(meta_name, arguments.at(pos)->modifiedArgumentName());
                } else {
                    QString debug = QStringLiteral(u"argument map specifies invalid argument index %1 for function '%2'")
                                    .arg(pos + 1).arg(java_function->name());
                    ReportHandler::warning(debug);
                }

            }
            code = code.replace(QStringLiteral(u"%this"), QStringLiteral(u"this"));
            QStringList lines = code.split("\n");
            if(position==CodeSnip::Comment || position==CodeSnip::CommentIntro){
                INDENTATIONRESET(INDENT);
                printExtraCode(lines, s);
            }else{
                printExtraCode(lines, s);
            }
        }
    }
}

void JavaGenerator::writeJavaCallThroughContents(QTextStream &s, const MetaFunction *java_function, uint attributes) {
    {
        const MetaArgumentList& arguments = java_function->arguments();

        QString lines;
        QString setArgumentOwnership;
        {
            QTextStream s(&lines);
            QTextStream s2(&setArgumentOwnership);
            writeInjectedCode(s, java_function, CodeSnip::Beginning);

            for (int i = 0; i < arguments.count(); ++i) {
                MetaArgument *arg = arguments.at(i);
                MetaType *type = arg->type();

                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    bool nonNull = false;
                    if (arg->isNullPointerDisabled(java_function)){
                        s << INDENT << "java.util.Objects.requireNonNull(" << arg->modifiedArgumentName() << ", \"Argument '" << arg->modifiedArgumentName() << "': null not expected.\");" << Qt::endl;
                        nonNull = true;
                    }

                    OwnershipRule ownershipRule = java_function->ownership(java_function->implementingClass(), TS::TargetLangCode, i + 1);
                    if (ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership) {
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
                                    s2 << ownershipRule.condition.replace("%arg", arg->modifiedArgumentName());
                                }
                                s2 << ") {" << Qt::endl;
                                {
                                    INDENTATION(INDENT);
                                    writeOwnershipForContainer(s2, java_function, ownershipRule.ownership, arg->type(), arg->modifiedArgumentName());
                                }
                                s2 << INDENT << "}" << Qt::endl;
                            }
                        }else{
                            if(ownershipRule.condition.isEmpty()){
                                writeFunctionCallForOwnership(s2, java_function, ownershipRule.ownership, arg->modifiedArgumentName());
                            }else{
                                s2 << INDENT << "if (" << ownershipRule.condition.replace("%arg", arg->modifiedArgumentName()) << ") {" << Qt::endl;
                                {
                                    INDENTATION(INDENT);
                                    writeFunctionCallForOwnership(s2, java_function, ownershipRule.ownership, arg->modifiedArgumentName());
                                }
                                s2 << INDENT << "}" << Qt::endl;
                            }
                        }
                    }

                    if (type->isArray() && type->arrayElementCounts().size()>0) {
                        if(java_function->typeReplaced(arg->argumentIndex() + 1).isEmpty()){
                            s << INDENT << "java.util.Objects.requireNonNull(" << arg->modifiedArgumentName() << ", \"Argument '" << arg->modifiedArgumentName() << "': null not expected.\");" << Qt::endl;
                            nonNull = true;
                        }
                    }

                    if (type->isContainer() && static_cast<const ContainerTypeEntry *>(type->typeEntry())->type()==ContainerTypeEntry::std_array) {
                        if(java_function->typeReplaced(arg->argumentIndex() + 1).isEmpty()){
                            s << INDENT << "java.util.Objects.requireNonNull(" << arg->modifiedArgumentName() << ", \"Argument '" << arg->modifiedArgumentName() << "': null not expected.\");" << Qt::endl;
                            nonNull = true;
                        }
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
                    if(java_function->useArgumentAsBuffer(arg->argumentIndex()+1)){
                        int minArrayLength = java_function->arrayOrBufferLengthMinValue(arg->argumentIndex()+1);
                        int maxArrayLength = java_function->arrayOrBufferLengthMaxValue(arg->argumentIndex()+1);
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
                    }else if(java_function->useArgumentAsArray(arg->argumentIndex()+1)){
                        int minArrayLength = java_function->arrayOrBufferLengthMinValue(arg->argumentIndex()+1);
                        int maxArrayLength = java_function->arrayOrBufferLengthMaxValue(arg->argumentIndex()+1);
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
                        if(java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                            int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
                            if(lengthParameter>0 && lengthParameter<=java_function->arguments().size()){
                                const MetaArgument *lengthParam = java_function->arguments()[lengthParameter - 1];
                                if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                                    lengthParam = nullptr;
                                    for(const MetaArgument *argument : java_function->arguments()) {
                                        if(argument && argument->argumentIndex()+1==lengthParameter){
                                            lengthParam = argument;
                                        }
                                    }
                                }
                                if(lengthParam && java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                                    QString offsetName = QStringLiteral(u"offsetOf");
                                    QString modifiedArgumentName = arg->modifiedArgumentName();
                                    modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                                    offsetName += modifiedArgumentName;
                                }
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
        if(!java_function->isStatic()){
            referenceCounts[-1] = java_function->referenceCounts(java_function->implementingClass(), -1);
            for(const ReferenceCount& refCount : referenceCounts[-1]) {
                if (refCount.action != ReferenceCount::Ignore) {
                    has_argument_referenceCounts = true;
                    break;
                }
            }
        }
        if(!java_function->isConstructor()){
            referenceCounts[0] = java_function->referenceCounts(java_function->implementingClass(), 0);
            for(const ReferenceCount& refCount : referenceCounts[0]) {
                if (refCount.action != ReferenceCount::Ignore) {
                    has_argument_referenceCounts = true;
                    break;
                }
            }
        }
        QHash<const MetaArgument*,QString> availableNativeIDs;
        for (const MetaArgument* argument : qAsConst(arguments)) {
            referenceCounts[argument->argumentIndex()+1] = java_function->referenceCounts(java_function->implementingClass(), argument->argumentIndex()+1);
            for(const ReferenceCount& refCount : referenceCounts[argument->argumentIndex()+1]) {
                if (refCount.action != ReferenceCount::Ignore) {
                    has_argument_referenceCounts = true;
                    const MetaType *type = argument->type();
                    if(!java_function->typeReplaced(argument->argumentIndex()+1).isEmpty()
                        || java_function->useArgumentAsBuffer(argument->argumentIndex()+1)){
                    }else if(java_function->useArgumentAsArray(argument->argumentIndex()+1)){
                    }else if(java_function->isConstructor()){
                    }else if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                    }else if (type->hasNativeId() || java_function->useArgumentAsSlotContext(argument->argumentIndex()+1)) {
                        if(!availableNativeIDs.contains(argument)){
                            QString idVar = QStringLiteral(u"__%1__NativeIdInfo").arg(argument->modifiedArgumentName());
                            s << INDENT << "InternalAccess.NativeIdInfo " << idVar << " = QtJambi_LibraryUtilities.internal.checkedNativeIdInfo(" << argument->modifiedArgumentName() << ");" << Qt::endl;
                            availableNativeIDs[argument] = idVar;
                        }
                    }
                    if (refCount.action == ReferenceCount::Put
                            && refCount.keyArgument>0
                            && argument->argumentIndex()!=int(refCount.keyArgument)-1) {
                        for (const MetaArgument* argument : qAsConst(arguments)) {
                            if(argument->argumentIndex()==int(refCount.keyArgument)-1){
                                const MetaType *type = argument->type();
                                if(!java_function->typeReplaced(argument->argumentIndex()+1).isEmpty()
                                    || java_function->useArgumentAsBuffer(argument->argumentIndex()+1)){
                                }else if(java_function->useArgumentAsArray(argument->argumentIndex()+1)){
                                }else if(java_function->isConstructor()){
                                }else if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                                }else if (type->hasNativeId() || java_function->useArgumentAsSlotContext(argument->argumentIndex()+1)) {
                                    if(!availableNativeIDs.contains(argument)){
                                        QString idVar = QStringLiteral(u"__%1__NativeIdInfo").arg(argument->modifiedArgumentName());
                                        s << INDENT << "InternalAccess.NativeIdInfo " << idVar << " = QtJambi_LibraryUtilities.internal.checkedNativeIdInfo(" << argument->modifiedArgumentName() << ");" << Qt::endl;
                                        availableNativeIDs[argument] = idVar;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }

        // Lookup if there is a reference-count action required on the return value.
        MetaType *return_type = java_function->type();
        QString new_return_type = java_function->typeReplaced(0);
        registerPackage(new_return_type);
        new_return_type = new_return_type.replace(u'$', u'.');
        bool has_return_type = (new_return_type != QStringLiteral(u"void")
                                && (!new_return_type.isEmpty() || return_type)) || java_function->isSelfReturningFunction();
        TS::Ownership returnValueOwnership = java_function->ownership(java_function->implementingClass(), TS::TargetLangCode, 0).ownership;
        TS::Ownership thisOwnership = java_function->ownership(java_function->implementingClass(), TS::TargetLangCode, -1).ownership;

        bool has_code_injections_at_the_end = hasCodeInjections(java_function, {CodeSnip::End, CodeSnip::Position4, CodeSnip::Position3});

        bool needs_return_variable = has_return_type
                                     && ( !setArgumentOwnership.isEmpty()
                                          || (returnValueOwnership != TS::InvalidOwnership && returnValueOwnership != TS::IgnoreOwnership)
                                          || (thisOwnership != TS::InvalidOwnership && thisOwnership != TS::IgnoreOwnership)
                                          || has_argument_referenceCounts
                                          || has_code_injections_at_the_end);

        s << INDENT;
        if (has_return_type && (java_function->argumentReplaced(0).isEmpty() || java_function->argumentReplaced(0)==QStringLiteral(u"this"))) {
            if (needs_return_variable) {
                if (new_return_type.isEmpty())
                    s << qualifiedJavaType(return_type, java_function->implementingClass(), Option(IsReturnType | (m_nullness ? NoOption : NoNullness)));
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
                    s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace(u'$', u'.') << "(";
                }
            }
        }

        bool needsComma = false;
        if (attributes & SuperCall) {
            s << "super.";
        }
        s << java_function->marshalledName() << "(";
        if (java_function->isConstructor()) {
            s << "this";
            needsComma = true;
        }

        if (!java_function->isConstructor() && !java_function->isStatic()){
            if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                needsComma = true;
            }else if(java_function->implementingClass()->typeEntry()->isQMetaObjectType()){
                s << "__qt_persistentPointer";
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
            const MetaArgument *arg = arguments.at(i);
            const MetaType *type = arg->type();

            if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                if (needsComma)
                    s << ", ";
                needsComma = true;

                if(!java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()
                    || java_function->useArgumentAsBuffer(arg->argumentIndex()+1)){
                    s << arg->modifiedArgumentName();
                }else if(java_function->useArgumentAsArray(arg->argumentIndex()+1)){
                    s << arg->modifiedArgumentName();
                    if(java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                        int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
                        if(lengthParameter>0 && lengthParameter<=java_function->arguments().size()){
                            const MetaArgument *lengthParam = java_function->arguments()[lengthParameter - 1];
                            if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                                lengthParam = nullptr;
                                for(const MetaArgument *argument : java_function->arguments()) {
                                    if(argument && argument->argumentIndex()+1==lengthParameter){
                                        lengthParam = argument;
                                    }
                                }
                            }
                            if(lengthParam && java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                                s << ", offsetOf";
                                QString modifiedArgumentName = arg->modifiedArgumentName();
                                modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                                s << modifiedArgumentName;
                            }
                        }
                    }
                }else if(java_function->isConstructor()){
                    s << arg->modifiedArgumentName();
                }else if (type->isTargetLangEnum() || type->isTargetLangFlags()) {
                    s << arg->modifiedArgumentName() << ".value()";
                }else if(availableNativeIDs.contains(arg)){
                    s << availableNativeIDs[arg] << ".nativeId()";
                }else if (type->hasNativeId() || java_function->useArgumentAsSlotContext(arg->argumentIndex()+1)) {
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

        s << ")";

        // This closed the ".resolve(" or the "new MyType(" fragments
        if (return_type && new_return_type.isEmpty() && (return_type->isTargetLangEnum() || return_type->isTargetLangFlags()))
            s << ")";

        s << ";" << Qt::endl;

        if (thisOwnership != TS::InvalidOwnership && thisOwnership != TS::IgnoreOwnership){
            writeFunctionCallForOwnership(s, java_function, thisOwnership, "this");
        }

        s << setArgumentOwnership;

        if(!java_function->isStatic()){
            for(const ReferenceCount& refCount : qAsConst(referenceCounts[-1]))
                writeReferenceCount(s, refCount, -1, java_function, availableNativeIDs, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
        }

        // We must ensure we retain a Java hard-reference over the native method call
        // so that the GC will not destroy the C++ object too early.  At this point we
        // have called the native method call so can manage referenceCount issues.
        // First the input arguments
        for (const MetaArgument* argument : qAsConst(arguments)) {
            for(const ReferenceCount& refCount : referenceCounts[argument->argumentIndex()+1])
                writeReferenceCount(s, refCount, argument->argumentIndex()+1, java_function, availableNativeIDs, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
        }

        // Then the return value
        if(!java_function->isConstructor()){
            for(const ReferenceCount& referenceCount : qAsConst(referenceCounts[0]))
                writeReferenceCount(s, referenceCount, 0, java_function, availableNativeIDs, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
        }

        writeInjectedCode(s, java_function, CodeSnip::Position3);
        writeInjectedCode(s, java_function, CodeSnip::Position4);
        writeInjectedCode(s, java_function, CodeSnip::End);

        if (needs_return_variable) {
            if (returnValueOwnership != TS::InvalidOwnership && returnValueOwnership != TS::IgnoreOwnership) {
                if (return_type->isContainer()){
                    s << INDENT << "if (__qt_return_value != null) {" << Qt::endl;
                    writeOwnershipForContainer(s, java_function, returnValueOwnership, return_type, "__qt_return_value");
                    s << INDENT << "}" << Qt::endl;
                }else{
                    writeFunctionCallForOwnership(s, java_function, returnValueOwnership, "__qt_return_value");
                }
            }
            if (!java_function->argumentReplaced(0).isEmpty() && java_function->argumentReplaced(0)!="this") {
                s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
            }else{
                s << INDENT << "return __qt_return_value;" << Qt::endl;
            }
        }else if (!java_function->argumentReplaced(0).isEmpty() && java_function->argumentReplaced(0)!="this") {
            s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
        }
    }
}

void JavaGenerator::writeSignal(QTextStream &s, const MetaFunction *java_function) {
    Q_ASSERT(java_function->isSignal());

    if (java_function->isModifiedRemoved(TS::TargetLangCode) || java_function->isPrivate())
        return ;

    MetaArgumentList arguments;
    for (int i = 0; i < java_function->arguments().size(); ++i) {
        MetaArgument *arg = java_function->arguments().at(i);
        if(!java_function->argumentRemoved(arg->argumentIndex()+1)){
            arguments << arg;
        }
    }
    auto sz = arguments.size();

    QList<QString> defaultValueArgumentType;
    QList<QString> defaultValueExpressions;

    QString signalTypeName;
    if (java_function->isPrivateSignal()) {
        signalTypeName += "PrivateSignal";
    }else{
        signalTypeName += "Signal";
        for (const MetaArgument* argument : qAsConst(arguments)) {
            QString defaultValueExpression = argument->defaultValueExpression();
            if(!defaultValueExpression.isEmpty()){
                defaultValueExpressions << defaultValueExpression;

                QString type = java_function->typeReplaced(argument->argumentIndex() + 1);

                if (type.isEmpty()){
                    type = qualifiedJavaType(argument->type(), java_function->implementingClass(), java_function->isPrivateSignal() ? Option(BoxedPrimitive | VarArgsAsArray | IsReturnType | (m_nullness ? NoOption : NoNullness)) : Option(BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers | IsReturnType | (m_nullness ? NoOption : NoNullness)));
                }else{
                    registerPackage(type);
                    type = type.replace(u'$', u'.');
                }

                defaultValueArgumentType << type;
            }
        }
    }

    signalTypeName += QString::number(sz);

    QString extraArguments;
    QTextStream extraArgumentStream(&extraArguments);
    if(!defaultValueExpressions.isEmpty()){
        auto dsz = defaultValueExpressions.size();
        signalTypeName += "Default";
        signalTypeName += QString::number(dsz);
        for (int i = 0; i < dsz; ++i) {
            if (i > 0)
                extraArgumentStream << ", " << Qt::endl;
            extraArgumentStream << "()->" << defaultValueExpressions.at(i);
        }
    }
    if(hasCodeInjections(java_function, {CodeSnip::Beginning})){
        INDENTATIONRESET(INDENT);
        if(!extraArguments.isEmpty()){
            extraArgumentStream << ", " << Qt::endl;
        }
        extraArgumentStream << "(";
        for (decltype(arguments.length()) l = arguments.length(), a{0}; a < l; ++a) {
            const MetaArgument* argument = arguments[a];
            if (a > 0)
                extraArgumentStream << ", ";
            extraArgumentStream << argument->argumentName();
        }
        extraArgumentStream << ")->{" << Qt::endl;
        {
            INDENTATION(INDENT);
            writeInjectedCode(extraArgumentStream, java_function, CodeSnip::Beginning);
        }
        extraArgumentStream << "}";
    }
    m_exportedPackages.clear();
    QString constructorCall = signalTypeName;
    if (sz > 0) {
        constructorCall += "<>";
        signalTypeName += "<";
        bool begin = true;
        for (const MetaArgument* argument : qAsConst(arguments)) {
            if (begin){
                begin = false;
            }else{
                signalTypeName += ", ";
            }

            QString modifiedType = java_function->typeReplaced(argument->argumentIndex() + 1);

            QString boxedType;
            if (modifiedType.isEmpty()){
                boxedType = qualifiedJavaType(argument->type(), java_function->implementingClass(), java_function->isPrivateSignal() ? Option(BoxedPrimitive | VarArgsAsArray | IsReturnType | (m_nullness ? NoOption : NoNullness)) : Option(BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers | IsReturnType | (m_nullness ? NoOption : NoNullness)));
            }else{
                registerPackage(modifiedType);
                boxedType = modifiedType.replace(u'$', u'.');
            }
            QString typeAnnotation = QStringLiteral(u"java.lang.@QtPrimitiveType@StrictNonNull ");
            if(boxedType==QStringLiteral(u"java.lang.Integer")){
                boxedType = typeAnnotation + QStringLiteral(u"Integer");
            }else if(boxedType==QStringLiteral(u"java.lang.Short")){
                boxedType = typeAnnotation + QStringLiteral(u"Short");
            }else if(boxedType==QStringLiteral(u"java.lang.Byte")){
                boxedType = typeAnnotation + QStringLiteral(u"Byte");
            }else if(boxedType==QStringLiteral(u"java.lang.Long")){
                boxedType = typeAnnotation + QStringLiteral(u"Long");
            }else if(boxedType==QStringLiteral(u"java.lang.Double")){
                boxedType = typeAnnotation + QStringLiteral(u"Double");
            }else if(boxedType==QStringLiteral(u"java.lang.Float")){
                boxedType = typeAnnotation + QStringLiteral(u"Float");
            }else if(boxedType==QStringLiteral(u"java.lang.Boolean")){
                boxedType = typeAnnotation + QStringLiteral(u"Boolean");
            }else if(boxedType==QStringLiteral(u"java.lang.Character")){
                boxedType = typeAnnotation + QStringLiteral(u"Character");
            }
            signalTypeName += boxedType;
        }
        signalTypeName += u'>';
    }

    uint exclude_attributes = MetaAttributes::Abstract
                             | MetaAttributes::Native
                             | MetaAttributes::Private
                             | MetaAttributes::Override;
    uint include_attributes = MetaAttributes::Final;

    QString signalName = java_function->name();
    FunctionModificationList mods = java_function->modifications(java_function->implementingClass());
    for(const FunctionModification& mod : qAsConst(mods)) {
        if (mod.isAccessModifier()) {
            exclude_attributes |= MetaAttributes::Public
                                  | MetaAttributes::Protected
                                  | MetaAttributes::Private
                                  | MetaAttributes::Friendly;
            include_attributes &= ~(exclude_attributes);

            if (mod.isPublic())
                include_attributes |= MetaAttributes::Public;
            else if (mod.isProtected())
                include_attributes |= MetaAttributes::Protected;
            else if (mod.isPrivate())
                include_attributes |= MetaAttributes::Private;
            else if (mod.isFriendly())
                include_attributes |= MetaAttributes::Friendly;

            exclude_attributes &= ~(include_attributes);

        }
    }

    s << Qt::endl;
    // Insert Javadoc
    QString comment;
    QTextStream commentStream(&comment);
    bool hasIntro = hasCodeInjections(java_function, {CodeSnip::CommentIntro});
    if(hasIntro){
        writeInjectedCode(commentStream, java_function, CodeSnip::CommentIntro);
    }
    if(!hasIntro || !java_function->href().isEmpty() || !java_function->brief().isEmpty()){
        if(!java_function->brief().isEmpty()){
            commentStream << "<p>" << encodeHtml(java_function->brief()) << "</p>" << Qt::endl;
        }
        commentStream << "<p>See <code>";
        if(!java_function->href().isEmpty())
            commentStream << "<a href=\"" << docsUrl << java_function->href() << "\">";
        if(java_function->declaringClass())
            commentStream << encodeHtml(java_function->declaringClass()->qualifiedCppName()
                                             .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                             .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                             .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                          << "::<wbr/>";
        commentStream << encodeHtml(java_function->originalSignature()).replace(",", ",<wbr/>");
        if(!java_function->href().isEmpty())
            commentStream << "</a>";
        commentStream << "</code></p>" << Qt::endl;
    }
    if(!java_function->since().isEmpty()){
        commentStream << "@since This signal was introduced in Qt " << java_function->since() << "." << Qt::endl;
    }
    if(hasCodeInjections(java_function, {CodeSnip::Comment})){
        writeInjectedCode(commentStream, java_function, CodeSnip::Comment);
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

    QString ident;
    {
        QTextStream s2(&ident);
        writeFunctionAttributes(s2, java_function, -1, {}, include_attributes, exclude_attributes,
                                Option(SkipReturnType));
        if(m_nullness)
            s2 << "@NonNull ";
        s2 << signalTypeName << " " << signalName << " = new " << constructorCall << "(";
        s << ident;
        QString lastLine;
        while(!s2.atEnd()){
            lastLine = s2.readLine();
        }
        s2.seek(0);
        lastLine.fill(u' ');
        ident = lastLine;
    }
    if(!extraArguments.isEmpty()){
        extraArgumentStream.seek(0);
        bool firstLine = true;
        while(!extraArgumentStream.atEnd()){
            if(firstLine){
                s << extraArgumentStream.readLine();
                firstLine = false;
            }else{
                s << Qt::endl << ident << extraArgumentStream.readLine();
            }
        }
    }
    s << ");" << Qt::endl;
}

void JavaGenerator::writeMultiSignal(QTextStream &s, const MetaFunctionList& signalList){
    const QString& signalName = signalList.first()->name();
    QMap<qint64,QSet<QString> > signalTypesByArgs;
    QHash<MetaFunction*,QString> signalTypes;
    QMap<qint64,QList<QString>> signalParameterClassesList;
    QMap<qint64,int> argumentCountMap;
    s << Qt::endl
      << INDENT << "/**" << Qt::endl
      << INDENT << " * <p>Wrapper class for overloaded signals:</p>" << Qt::endl
      << INDENT << " * <ul>" << Qt::endl;
    for(MetaFunction* java_function : signalList){
        s << INDENT << " * <li><code>" << encodeHtml(java_function->signature()) << "</code></li>" << Qt::endl;
    }
    s << INDENT << " * </ul>" << Qt::endl
      << INDENT << " */" << Qt::endl
      << INDENT << "public final class MultiSignal_" << signalName << " extends MultiSignal{" << Qt::endl;
    {
        INDENTATION(INDENT);
        QHash<QString,const MetaFunction *> previousMethodSignatures;
        m_currentMethodSignatures.swap(previousMethodSignatures);
        s << INDENT << "private MultiSignal_" << signalName << "(){" << Qt::endl;
        {
            INDENTATION(INDENT);
            QString configurations;
            bool hasDefaults = false;
            {
                QTextStream s(&configurations);
                bool first = true;
                QSet<qint64> argCounts;
                for(MetaFunction* java_function : signalList){
                    const MetaArgumentList& arguments = java_function->arguments();
                    auto sz = arguments.count();
                    argCounts.insert(sz);
                    argumentCountMap[sz]++;

                    QList<QString> defaultValueArgumentType;
                    QList<QString> defaultValueExpressions;

                    QString constructorCall = QStringLiteral(u"Signal");
                    if (java_function->isPrivateSignal()) {
                        constructorCall = QStringLiteral(u"PrivateSignal");
                    }else{
                        for (int i = 0; i < sz; ++i) {
                            auto arg = arguments.at(i);
                            QString defaultValueExpression = arguments.at(i)->defaultValueExpression();
                            if(!defaultValueExpression.isEmpty()){
                                defaultValueExpressions << defaultValueExpression;

                                QString type = java_function->typeReplaced(arg->argumentIndex() + 1);

                                if (type.isEmpty()){
                                    type = qualifiedJavaType(arg->type(), java_function->implementingClass(), java_function->isPrivateSignal() ? Option(BoxedPrimitive | VarArgsAsArray | IsReturnType | (m_nullness ? NoOption : NoNullness)) : Option(BoxedPrimitive | VarArgsAsArray | NoQCollectionContainers | IsReturnType | (m_nullness ? NoOption : NoNullness)));
                                }else{
                                    registerPackage(type);
                                    type = type.replace(u'$', u'.');
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
                                boxedType += qualifiedJavaType(arg->type(), java_function->implementingClass(), Option(BoxedPrimitive | VarArgsAsArray | IsReturnType | (m_nullness ? NoOption : NoNullness)));
                                QString type = qualifiedJavaType(arg->type(), java_function->implementingClass(), Option(NoNullness | VarArgsAsArray | IsReturnType));
                                auto idx = type.indexOf(u'<');
                                if(idx>0){
                                    type = type.mid(0, idx);
                                    if(type==QStringLiteral(u"io.qt.core.QList"))
                                        type = QStringLiteral(u"java.util.List");
                                    else if(type==QStringLiteral(u"io.qt.core.QMap") || type==QStringLiteral(u"io.qt.core.QHash"))
                                        type = QStringLiteral(u"java.util.Map");
                                    else if(type==QStringLiteral(u"io.qt.core.QSet"))
                                        type = QStringLiteral(u"java.util.Set");
                                }
                                unboxedType += type;
                            }else{
                                registerPackage(modifiedType);
                                boxedType += modifiedType.replace(u'$', u'.');
                                unboxedType += annotationFreeTypeName(modifiedType.replace(u'$', u'.'));
                            }
                            signalObjectType += boxedType;
                            signalParameterClasses += unboxedType+".class";
                        }
                        signalObjectType += ">";
                        signalParameterClassesList[sz] << signalParameterClasses;
                    }
                    signalTypes[java_function] = signalObjectType;
                    if(!first)
                        s << ", ";
                    if (sz > 0) {
                        s << "new SignalConfiguration(" << signalParameterClasses << ", new " << constructorCall << "(" << signalDefaultArgumentExpressions << "))";
                    }else{
                        s << "new SignalConfiguration(new " << constructorCall << "(" << signalDefaultArgumentExpressions << "))";
                    }
                    first = false;
                }
            }
            s << INDENT << "super();" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
        QSet<qint64> writtenConnects;
        for(QMap<qint64,int>::const_iterator it=argumentCountMap.cbegin(); it!=argumentCountMap.cend(); it++){
            QString parameters;
            QStringList parameterClasses;
            QStringList classes;
            QStringList vars;
            QString annotations;
            {
                QTextStream s2(&annotations);
                for(const QString& signalParameterClasses : qAsConst(signalParameterClassesList[it.key()])){
                    s2 << "@QtAllowedTypeSet({" << signalParameterClasses << "})" << Qt::endl << INDENT;
                }
            }
            QStringList params;
            if(it.key()>0){
                for(int j=0; j<it.key(); j++){
                    QString letter = QString(QChar('A'+j));
                    params << letter;
                    classes << QString(QLatin1String("Class<%1> type%2")).arg(letter).arg(j+1);
                    vars << QString(QLatin1String("type%1")).arg(j+1);
                    parameterClasses << QString(QLatin1String("(Class<%1>)parameters[%2]")).arg(letter).arg(j);
                }
                parameters = QString(QLatin1String("<%1>")).arg(params.join(","));
            };
            params.prepend(QStringLiteral(u"Receiver"));
            QString rparameters = QString(QLatin1String("<%1>")).arg(params.join(","));

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
                    for(const QString& signalParameterClasses : qAsConst(signalParameterClassesList[it.key()])){
                        s << INDENT << " * <li><code>" << signalName << ".overload(" << signalParameterClasses << ")</code></li>" << Qt::endl;
                    }
                    s << INDENT << " * </ul>" << Qt::endl;
                }
                s << INDENT << " * <p>{@link QNoSuchSignalException} is thrown otherwise.</p>" << Qt::endl << Qt::endl;
                for(int j=0; j<it.key(); j++){
                    s << INDENT << " * @param <" << QChar('A'+j) << "> signal parameter type"<< Qt::endl;
                }
                for(int j=0; j<it.key(); j++){
                    s << INDENT << " * @param type" << QString::number(j+1) << " value of type " << QChar('A'+j) << Qt::endl;
                }
                s << INDENT << " * @return overloaded signal" << Qt::endl
                  << INDENT << " * @throws QNoSuchSignalException if signal is not available" << Qt::endl;
            }
            s << INDENT << " */" << Qt::endl;

            if(signalTypesByArgs[it.key()].size()==1){
                s << INDENT << annotations << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << signalTypesByArgs[it.key()].begin().operator *() << parameters << " overload(" << classes.join(", ") << ") throws QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return (" << signalTypesByArgs[it.key()].begin().operator *() << parameters << ")super.overload(" << vars.join(", ") << ");" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
            }else{
                s << INDENT << annotations << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject.AbstractPrivateSignal" << it.key() << parameters << " overload(" << classes.join(", ") << ") throws QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.overload(" << vars.join(", ") << ");" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
            }

            QString nonNull = m_nullness ? QStringLiteral(u"@NonNull ") : QString{};
            QString snonNull = m_nullness ? QStringLiteral(u"@StrictNonNull ") : QString{};
            QString arrayNonNull = m_nullness ? QStringLiteral(" @NonNull") : QString{};
            if(it.key()>0)
            {
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * Initializes a connection to the <i>slot</i>." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param slot the slot to be connected" << Qt::endl
                  << INDENT << " * @param connectionType type of connection" << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(io.qt.core.QMetaObject." << snonNull << "Slot" << it.key() << parameters << " slot, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect(slot, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Removes the connection to the given <i>slot</i>." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param slot the slot to be disconnected" << Qt::endl
                  << INDENT << " * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject." << snonNull << "Slot" << it.key() << parameters << " slot) {" << Qt::endl
                  << INDENT << "    return super.disconnect(slot);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Initializes a connection to the <i>slot</i> of <i>receiver</i>." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param <Receiver> The type of the receiver" << Qt::endl
                  << INDENT << " * @param receiver the target receiver" << Qt::endl
                  << INDENT << " * @param slot the slot to be connected" << Qt::endl
                  << INDENT << " * @param connectionType type of connection" << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << rparameters << (rparameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(@StrictNonNull Receiver receiver, io.qt.core.QMetaObject." << snonNull << "Slot" << params.size() << rparameters << " slot, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect(slot, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Removes the connection to the given <i>slot</i> of <i>receiver</i>." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param <Receiver> The type of the receiver" << Qt::endl
                  << INDENT << " * @param receiver the target receiver" << Qt::endl
                  << INDENT << " * @param slot the slot to be disconnected" << Qt::endl
                  << INDENT << " * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << rparameters << (rparameters.isEmpty() ? "" : " ") << "boolean disconnect(@StrictNonNull Receiver receiver, io.qt.core.QMetaObject." << snonNull << "Slot" << params.size() << rparameters << " slot) {" << Qt::endl
                  << INDENT << "    return super.disconnect(slot);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second" << Qt::endl
                  << INDENT << " * signal to be emitted as well." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal. This will be emitted whenever this signal is emitted." << Qt::endl
                  << INDENT << " * @param connectionType One of the connection types defined in the Qt interface." << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(io.qt.core.QMetaObject." << snonNull << "Connectable" << it.key() << parameters << " signal, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect((io.qt.core.QMetaObject.AbstractSignal)signal, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Disconnects a signal from another signal if the two were previously connected by a call to connect." << Qt::endl
                  << INDENT << " * A call to this function will assure that the emission of the first signal will not cause the emission of the second." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal." << Qt::endl
                  << INDENT << " * @return <code>true</code> if the two signals were successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject." << snonNull << "Connectable" << it.key() << parameters << " signal) {" << Qt::endl
                  << INDENT << "    return super.disconnect((io.qt.core.QMetaObject.AbstractSignal)signal);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second" << Qt::endl
                  << INDENT << " * signal to be emitted as well." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal. This will be emitted whenever this signal is emitted." << Qt::endl
                  << INDENT << " * @param connectionType One of the connection types defined in the Qt interface." << Qt::endl
                  << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                  << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(io.qt.core.QMetaObject." << snonNull << "AbstractPublicSignal" << it.key() << parameters << " signal, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
                  << INDENT << "    return super.connect((io.qt.core.QMetaObject.AbstractSignal)signal, connectionType);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Disconnects a signal from another signal if the two were previously connected by a call to connect." << Qt::endl
                  << INDENT << " * A call to this function will assure that the emission of the first signal will not cause the emission of the second." << Qt::endl
                  << INDENT << " * " << Qt::endl
                  << INDENT << " * @param signal The second signal." << Qt::endl
                  << INDENT << " * @return <code>true</code> if the two signals were successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "boolean disconnect(io.qt.core.QMetaObject." << snonNull << "AbstractPublicSignal" << it.key() << parameters << " signal) {" << Qt::endl
                  << INDENT << "    return super.disconnect((io.qt.core.QMetaObject.AbstractSignal)signal);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
                for(qint64 i = it.key()-1; i>0; --i){
                    if(!argumentCountMap.contains(i) && !writtenConnects.contains(i)){
                        writtenConnects.insert(i);
                        QString parameters;
                        QStringList parameterClasses;
                        QStringList classes;
                        QStringList vars;
                        QStringList params;
                        if(i>0){
                            for(int j=0; j<i; j++){
                                QString letter = QString(QChar('A'+j));
                                params << letter;
                                classes << QStringLiteral(u"Class<%1> type%2").arg(letter).arg(j+1);
                                vars << QStringLiteral(u"type%1").arg(j+1);
                                parameterClasses << QStringLiteral(u"(Class<%1>)parameters[%2]").arg(letter).arg(j);
                            }
                            parameters = QStringLiteral(u"<%1>").arg(params.join(QStringLiteral(u",")));
                        }
                        params.prepend(QStringLiteral(u"Receiver"));
                        QString rparameters = QString(QLatin1String("<%1>")).arg(params.join(","));
                        s << INDENT << "/**" << Qt::endl
                          << INDENT << " * Initializes a connection to the <i>slot</i>." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param slot the slot to be connected" << Qt::endl
                          << INDENT << " * @param connectionType type of connection" << Qt::endl
                          << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                          << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(io.qt.core.QMetaObject.Slot" << i << parameters << " slot, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
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
                          << INDENT << " * Initializes a connection to the <i>slot</i> of <i>receiver</i>." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param <Receiver> The type of the receiver" << Qt::endl
                          << INDENT << " * @param receiver the target receiver" << Qt::endl
                          << INDENT << " * @param slot the slot to be connected" << Qt::endl
                          << INDENT << " * @param connectionType type of connection" << Qt::endl
                          << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                          << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << rparameters << (rparameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(@StrictNonNull Receiver receiver, io.qt.core.QMetaObject." << snonNull << "Slot" << params.size() << rparameters << " slot, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
                          << INDENT << "    return super.connect(slot, connectionType);" << Qt::endl
                          << INDENT << "}" << Qt::endl << Qt::endl
                          << INDENT << "/**" << Qt::endl
                          << INDENT << " * Removes the connection to the given <i>slot</i> of <i>receiver</i>." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param <Receiver> The type of the receiver" << Qt::endl
                          << INDENT << " * @param receiver the target receiver" << Qt::endl
                          << INDENT << " * @param slot the slot to be disconnected" << Qt::endl
                          << INDENT << " * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << rparameters << (rparameters.isEmpty() ? "" : " ") << "boolean disconnect(@StrictNonNull Receiver receiver, io.qt.core.QMetaObject." << snonNull << "Slot" << params.size() << rparameters << " slot) {" << Qt::endl
                          << INDENT << "    return super.disconnect(slot);" << Qt::endl
                          << INDENT << "}" << Qt::endl << Qt::endl
                          << INDENT << "/**" << Qt::endl
                          << INDENT << " * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second" << Qt::endl
                          << INDENT << " * signal to be emitted as well." << Qt::endl
                          << INDENT << " * " << Qt::endl
                          << INDENT << " * @param signal The second signal. This will be emitted whenever this signal is emitted." << Qt::endl
                          << INDENT << " * @param connectionType One of the connection types defined in the Qt interface." << Qt::endl
                          << INDENT << " * @return connection if successful or <code>null</code> otherwise" << Qt::endl
                          << INDENT << " * @throws QMisfittingSignatureException Raised if their signatures are incompatible." << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public final " << parameters << (parameters.isEmpty() ? "" : " ") << "io.qt.core.QMetaObject." << nonNull << "Connection connect(io.qt.core.QMetaObject.AbstractPublicSignal" << i << parameters << " signal, io.qt.core.Qt." << nonNull << "ConnectionType" << arrayNonNull << "... connectionType) throws QNoSuchSignalException{" << Qt::endl
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

        for(MetaFunction* java_function : signalList){
            QString comment;
            QTextStream commentStream(&comment);
            bool hasIntro = hasCodeInjections(java_function, {CodeSnip::CommentIntro});
            if(hasIntro){
                writeInjectedCode(commentStream, java_function, CodeSnip::CommentIntro);
            }
            if(!hasIntro || !java_function->href().isEmpty() || !java_function->brief().isEmpty()){
                if(!java_function->brief().isEmpty()){
                    commentStream << "<p>" << encodeHtml(java_function->brief()) << "</p>" << Qt::endl;
                }
                commentStream << "<p>See <code>";
                if(!java_function->href().isEmpty())
                    commentStream << "<a href=\"" << docsUrl << java_function->href() << "\">";
                if(java_function->declaringClass())
                    commentStream << encodeHtml(java_function->declaringClass()->qualifiedCppName()
                                                     .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                                     .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                                     .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                                  << "::<wbr/>";
                commentStream << encodeHtml(java_function->originalSignature()).replace(",", ",<wbr/>");
                if(!java_function->href().isEmpty())
                    commentStream << "</a>";
                commentStream << "</code></p>" << Qt::endl;
            }
            if(!java_function->since().isEmpty()){
                commentStream << "@since This function was introduced in Qt " << java_function->since() << "." << Qt::endl;
            }
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            if(hasCodeInjections(java_function, {CodeSnip::Comment})){
                writeInjectedCode(commentStream, java_function, CodeSnip::Comment);
            }
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
                QString type = qualifiedJavaType(java_function->arguments().at(i)->type(), java_function->implementingClass(), Option(NoNullness | VarArgsAsArray | IsReturnType));
                auto idx = type.indexOf(u'<');
                if(idx>0){
                    type = type.mid(0, idx);
                    if(type==QStringLiteral(u"io.qt.core.QList"))
                        type = QStringLiteral(u"java.util.List");
                    else if(type==QStringLiteral(u"io.qt.core.QMap") || type==QStringLiteral(u"io.qt.core.QHash"))
                        type = QStringLiteral(u"java.util.Map");
                    else if(type==QStringLiteral(u"io.qt.core.QSet"))
                        type = QStringLiteral(u"java.util.Set");
                    hasInstantiations = true;
                }
                classes << type+QStringLiteral(u".class");
            }
            m_exportedPackages.clear();
            QString sig = functionSignature(java_function,
                                            java_function->isPrivateSignal() ? MetaAttributes::Private : MetaAttributes::Public,
                                            ( java_function->isPrivateSignal() ? MetaAttributes::Public : 0 ) | MetaAttributes::Native | MetaAttributes::Override,
                                            Option(NoSuppressExports), {}, -1, QStringLiteral(u"emit"));// << "public final void emit("
            bool exp = false;
            for(const QString& pkg : qAsConst(m_exportedPackages)){
                if(pkg!=java_function->implementingClass()->package()
                    && !pkg.isEmpty()
                    && !pkg.startsWith(QStringLiteral(u"io.qt.internal"))
                    && pkg!=QStringLiteral(u"io.qt")
                    && m_typeSystemByPackage[pkg]
                    && !m_typeSystemByPackage[pkg]->module().isEmpty()
                    && m_typeSystemByPackage[pkg]->module()!=QStringLiteral(u"qtjambi") // because it's transitive
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
            }
            //else if(java_function->isPrivateSignal())
            //    s << INDENT << "@SuppressWarnings(\"unused\")" << Qt::endl;

            s << sig;
            s << " {" << Qt::endl;
            if (java_function->isPrivateSignal()){
                s << INDENT << "    io.qt.core.QObject.emit((";
                if(hasInstantiations){
                    QString signalType = signalTypes.value(java_function);
                    auto idx = signalType.indexOf(u'<');
                    if(idx>0){
                        signalType = signalType.mid(0, idx);
                    }
                    s << signalType;
                }else{
                    s << signalTypes.value(java_function);
                }
                s << ")overload(";
                s << classes.join(QStringLiteral(u", ")) << "), ";
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
                    auto idx = signalType.indexOf(u'<');
                    if(idx>0){
                        signalType = signalType.mid(0, idx);
                    }
                    s << signalType;
                }else{
                    s << signalTypes.value(java_function);
                }
                s << ")overload(";
                s << classes.join(QStringLiteral(u", ")) << ")).emit(";
                for(int i = 0; i < java_function->arguments().size(); i++){
                    if(i!=0)
                        s << ", ";
                    s << java_function->arguments().at(i)->modifiedArgumentName();
                }
                s << ");" << Qt::endl;
            }
            s << INDENT << "}" << Qt::endl << Qt::endl;
            {
                QString javaSignature;
                QTextStream s2(&javaSignature);
                s2 << "emit(";
                writeFunctionArguments(s2, java_function, {}, -1, Option(NoNullness | SkipTemplateParameters | VarArgsAsArray | SkipName | IsReturnType));
                s2 << ")";
                s2.flush();
                if(m_currentMethodSignatures.contains(javaSignature)){
                    ReportHandler::warning(QStringLiteral(u"Duplicate Java method %1 in %2").arg(javaSignature, java_function->ownerClass()->qualifiedCppName()));
                }
                m_currentMethodSignatures.insert(javaSignature, nullptr);
            }
        }
        for(MetaFunction* java_function : signalList){
            writeFunctionOverloads(s, java_function,
                                   java_function->isPrivateSignal() ? MetaAttributes::Private : MetaAttributes::Public,
                                   ( java_function->isPrivateSignal() ? MetaAttributes::Public : 0 ) | MetaAttributes::Native,
                                   Option(IsReturnType | (m_nullness ? NoOption : NoNullness)), QStringLiteral(u"emit"));
        }
        m_currentMethodSignatures.swap(previousMethodSignatures);
    }
    s << INDENT << "};" << Qt::endl << Qt::endl;
    s << Qt::endl
      << INDENT << "/**" << Qt::endl
      << INDENT << " * <p>Overloaded signals:</p>" << Qt::endl
      << INDENT << " * <ul>" << Qt::endl;
    for(MetaFunction* java_function : signalList){
        s << INDENT << " * <li><code>" << encodeHtml(java_function->originalSignature()).replace(",", ",<wbr/>") << "</code></li>" << Qt::endl;
    }
    s << INDENT << " * </ul>" << Qt::endl
      << INDENT << " */" << Qt::endl
      << INDENT << "public final ";
    if(m_nullness)
        s << "@NonNull ";
    s << "MultiSignal_" << signalName << " " << signalName << " = new MultiSignal_" << signalName << "();" << Qt::endl << Qt::endl;
}

void JavaGenerator::retrieveModifications(const MetaFunction *java_function,
        const MetaClass *java_class,
        uint *exclude_attributes,
        uint *include_attributes, Option) {
    FunctionModificationList mods = java_function->modifications(java_class);
//     printf("name: %s has %d mods\n", qPrintable(java_function->signature()), mods.size());
    for(const FunctionModification& mod : qAsConst(mods)) {
        if (mod.isAccessModifier()) {
//             printf(" -> access mod to %x\n", mod.modifiers);
            *exclude_attributes |= MetaAttributes::Public
                                   | MetaAttributes::Protected
                                   | MetaAttributes::Private
                                   | MetaAttributes::Friendly;

            if (mod.isPublic())
                *include_attributes |= MetaAttributes::Public;
            else if (mod.isProtected())
                *include_attributes |= MetaAttributes::Protected;
            else if (mod.isPrivate())
                *include_attributes |= MetaAttributes::Private;
            else if (mod.isFriendly())
                *include_attributes |= MetaAttributes::Friendly;
        }

        if (mod.isFinal()) {
            *include_attributes |= MetaAttributes::FinalInTargetLang;
        } else if (mod.isDeclaredFinal()) {
            *include_attributes |= MetaAttributes::Final;
            *include_attributes |= MetaAttributes::DeclaredFinalInCpp;
        } else if (mod.isNonFinal()) {
            *exclude_attributes |= MetaAttributes::FinalInTargetLang;
        }
    }
    *exclude_attributes &= ~(*include_attributes);
}

QString JavaGenerator::functionSignature(const MetaFunction *java_function,
        uint included_attributes, uint excluded_attributes,
        Option option,
        const QMap<int,Replacement>& replacedArguments,
        int arg_count,
        const QString& alternativeFunctionName) {
    const MetaArgumentList& arguments = java_function->arguments();
    int argument_count = arg_count < 0 ? int(arguments.size()) : arg_count;
    QString result;
    QTextStream s(&result);
    QString functionName = java_function->name();
    if(!alternativeFunctionName.isEmpty())
        functionName = alternativeFunctionName;
    // The actual function
    if (!(java_function->isEmptyFunction() || java_function->isNormal() || java_function->isSignal()))
        option = Option(option | SkipReturnType);
    writeFunctionAttributes(s, java_function, arg_count, replacedArguments, included_attributes, excluded_attributes, option);

    s << functionName << "(";
    writeFunctionArguments(s, java_function, replacedArguments, argument_count, option);
    s << ")";
    QString throws = java_function->throws();
    if(!throws.isEmpty()){
        s << " throws " << throws;
    }
    return result;
}

void JavaGenerator::setupForFunction(const MetaFunction *java_function,
                                     uint *included_attributes,
                                     uint *excluded_attributes, Option option) {
    *excluded_attributes |= java_function->ownerClass()->isInterface() || java_function->isConstructor()
                            ? MetaAttributes::Native
                            : 0;
    *excluded_attributes |= (java_function->ownerClass()->isInterface() && (option & InFunctionComment)==0) || java_function->isConstructor()
                            ? MetaAttributes::Final
                            : 0;
    if (java_function->ownerClass()->isInterface() && (option & InFunctionComment)==0)
        *excluded_attributes |= MetaAttributes::Abstract;
    if (java_function->needsCallThrough())
        *excluded_attributes |= MetaAttributes::Native;

    const MetaClass *java_class = java_function->ownerClass();
    retrieveModifications(java_function, java_class, excluded_attributes, included_attributes, option);
}

void JavaGenerator::writeReferenceCount(QTextStream &s, const ReferenceCount &refCount,
                                        int argumentIndex, const MetaFunction *java_function, const QHash<const MetaArgument*,QString>& availableNativeIDs, const QString &thisName) {
    if (refCount.action == ReferenceCount::Ignore)
        return;

    const MetaArgumentList& arguments = java_function->arguments();
    const MetaArgument* argument = nullptr;
    QString argumentName;
    if(argumentIndex==-1){
        argumentName = thisName;
    }else if(argumentIndex==0){
        argumentName = QStringLiteral(u"__qt_return_value");
    }else{
        argument = MetaFunction::argumentByIndex(arguments, argumentIndex-1);
        if(!argument)
            return;
        argumentName = argument->modifiedArgumentName();
    }
    bool nullPointersDisabled = false;
    if(argumentIndex>0){
        for(const MetaArgument* argument : arguments){
            if(argument->argumentIndex()==argumentIndex-1 && argument->isNullPointerDisabled(java_function)){
                nullPointersDisabled = true;
                break;
            }
        }
    }

    QString condition = refCount.condition;
    condition = condition.replace(QStringLiteral(u"%in->"), argumentName+QStringLiteral(u"."));
    condition = condition.replace(QStringLiteral(u"%arg->"), argumentName+QStringLiteral(u"."));
    condition = condition.replace(QStringLiteral(u"%this->"), thisName+QStringLiteral(u"."));
    condition = condition.replace(QStringLiteral(u"%0->"), QStringLiteral(u"__qt_return_value."));
    condition = condition.replace(QStringLiteral(u"%return->"), QStringLiteral(u"__qt_return_value."));
    condition = condition.replace(QStringLiteral(u"%in"), argumentName);
    condition = condition.replace(QStringLiteral(u"%arg"), argumentName);
    condition = condition.replace(QStringLiteral(u"%this"), thisName);
    condition = condition.replace(QStringLiteral(u"%0"), QStringLiteral(u"__qt_return_value"));
    condition = condition.replace(QStringLiteral(u"%return"), QStringLiteral(u"__qt_return_value"));
    for(const MetaArgument* argument : arguments){
        condition = condition.replace(QStringLiteral(u"%")+QString::number(argument->argumentIndex()+1)+QStringLiteral(u"->"), argument->modifiedArgumentName()+QStringLiteral(u"."));
        condition = condition.replace(QStringLiteral(u"%")+QString::number(argument->argumentIndex()+1), argument->modifiedArgumentName());
    }

    QScopedPointer<Indentation> indentation;
    if(argument
            && java_function->typeReplaced(argument->argumentIndex()+1).isEmpty()
            && !java_function->useArgumentAsBuffer(argument->argumentIndex()+1)
            && !java_function->useArgumentAsArray(argument->argumentIndex()+1)
            && !argument->type()->isTargetLangEnum()
            && !argument->type()->isTargetLangFlags()
            && refCount.action != ReferenceCount::AddAll
            && refCount.action != ReferenceCount::Remove
            && refCount.action != ReferenceCount::ClearAddAll){
        s << INDENT << "if (";
        if (!condition.isEmpty())
            s << condition << " && (";
        bool hasOr = false;
        if(refCount.action == ReferenceCount::Put && int(refCount.keyArgument)-1!=argument->argumentIndex()){
            for (const MetaArgument* argument : qAsConst(arguments)) {
                if(argument->argumentIndex()==int(refCount.keyArgument)-1){
                    if(availableNativeIDs.contains(argument)){
                        s << "(" << argument->modifiedArgumentName() << " != null && "
                          << availableNativeIDs[argument] << ".needsReferenceCounting()) || ";
                        hasOr = true;
                    }
                    break;
                }
            }
        }
        if(hasOr)
            s << "(";
        if(refCount.action != ReferenceCount::Set && !nullPointersDisabled)
            s << argumentName << " != null && ";
        if(argument && availableNativeIDs.contains(argument)){
            s << availableNativeIDs[argument] << ".needsReferenceCounting()";
        }else{
            s << "QtJambi_LibraryUtilities.internal.needsReferenceCounting(" << argumentName << ")";
        }
        if(hasOr)
            s << ")";
        if (!condition.isEmpty())
            s << ")";
        s << ") {" << Qt::endl;
        indentation.reset(new Indentation(INDENT));
    }else if (!condition.isEmpty()){
        s << INDENT << "if (";
        if(refCount.action != ReferenceCount::Set && !nullPointersDisabled)
            s << argumentName << " != null && ";
        s << condition << ") {" << Qt::endl;
        indentation.reset(new Indentation(INDENT));
    }else if(refCount.action != ReferenceCount::Set && !nullPointersDisabled){
        s << INDENT << "if (" << argumentName << " != null) {" << Qt::endl;
        indentation.reset(new Indentation(INDENT));
    }
    bool isStatic = java_function->isStatic();
    QString declareVariable = refCount.declareVariable;
    {
        if (!declareVariable.isEmpty()) {
            QList<TypeEntry *> types = m_database->findTypes(declareVariable);
            if(types.size()==1){
                declareVariable = types[0]->qualifiedTargetLangName();
            }
        }else if(java_function->isAbstract()){
            if(java_function->ownerClass()->typeEntry()->lookupName().endsWith(QStringLiteral(u"$Impl$ConcreteWrapper"))){
                QString lookupName = java_function->ownerClass()->typeEntry()->lookupName();
                lookupName.chop(16);
                declareVariable = java_function->ownerClass()->typeEntry()->javaPackage() + "." + lookupName.replace(u'$', u'.');
            }else if(java_function->ownerClass()->typeEntry()->lookupName().endsWith(QStringLiteral(u"$ConcreteWrapper"))){
                QString lookupName = java_function->declaringClass()->typeEntry()->lookupName();
                if(lookupName.endsWith(QStringLiteral(u"$ConcreteWrapper")))
                    lookupName.chop(16);
                declareVariable = java_function->declaringClass()->typeEntry()->javaPackage() + "." + lookupName.replace(u'$', u'.');
            }
        }
        switch (refCount.action) {
            case ReferenceCount::Put:
                {
                    QString keyArgumentName;
                    if(argument->argumentIndex()!=int(refCount.keyArgument)-1){
                        for (const MetaArgument* argument : qAsConst(arguments)) {
                            if(argument->argumentIndex()==int(refCount.keyArgument)-1){
                                keyArgumentName = argument->modifiedArgumentName();
                                break;
                            }
                        }
                    }
                    if (declareVariable.isEmpty()) {
                        s << INDENT << "if(" << refCount.variableName << "==null)" << Qt::endl;
                        if (refCount.threadSafe)
                            s << INDENT << "    " << refCount.variableName << " = java.util.Collections.synchronizedMap(QtJambi_LibraryUtilities.internal.newRCMap());" << Qt::endl;
                        else
                            s << INDENT << "    " << refCount.variableName << " = QtJambi_LibraryUtilities.internal.newRCMap();" << Qt::endl;
                        if(keyArgumentName.isEmpty()){
                            s << INDENT << refCount.variableName << ".put(" << argumentName << ", null);" << Qt::endl;
                        }else{
                            s << INDENT << refCount.variableName << ".put(" << keyArgumentName << ", " << argumentName << ");" << Qt::endl;
                        }
                    }else{
                        if(keyArgumentName.isEmpty()){
                            s << INDENT << "QtJambi_LibraryUtilities.internal.putReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << argumentName << ", null);" << Qt::endl;
                        }else{
                            s << INDENT << "QtJambi_LibraryUtilities.internal.putReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << keyArgumentName << ", " << argumentName << ");" << Qt::endl;
                        }
                    }
                }
                break;
            case ReferenceCount::ClearAdd:
                if (declareVariable.isEmpty()) {
                    s << INDENT << "if(" << refCount.variableName << "!=null)" << Qt::endl
                      << INDENT << "    " << refCount.variableName << ".clear();" << Qt::endl;
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
                    s << INDENT << "if(" << refCount.variableName << "!=null)" << Qt::endl
                      << INDENT << "    " << refCount.variableName << ".clear();" << Qt::endl;
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
                    if(argument
                            && argument->type()->instantiations().size()==1
                            && argument->type()->instantiations()[0]->isObject()){
                        s << INDENT << "for(" << qualifiedJavaType(argument->type()->instantiations()[0], java_function->implementingClass(), Option(BoxedPrimitive | VarArgsAsArray | NoNullness))
                          << " " << argumentName << "_element : " << argumentName << "){" << Qt::endl
                          << INDENT << "    if(QtJambi_LibraryUtilities.internal.needsReferenceCounting(" << argumentName << "_element))" << Qt::endl
                          << INDENT << "        " << refCount.variableName << ".add(" << argumentName << "_element);" << Qt::endl
                          << INDENT << "}" << Qt::endl;
                    }else{
                        s << INDENT << refCount.variableName << ".addAll(" << argumentName << ");" << Qt::endl;
                    }
                }else{
                    if(argument
                            && argument->type()->instantiations().size()==1
                            && argument->type()->instantiations()[0]->isObject()){
                        s << INDENT << "for(" << qualifiedJavaType(argument->type()->instantiations()[0], java_function->implementingClass(), Option(BoxedPrimitive | VarArgsAsArray | NoNullness))
                          << " " << argumentName << "_element : " << argumentName << "){" << Qt::endl
                          << INDENT << "    if(QtJambi_LibraryUtilities.internal.needsReferenceCounting(" << argumentName << "_element))" << Qt::endl
                          << INDENT << "        QtJambi_LibraryUtilities.internal.addReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << argumentName << "_element);" << Qt::endl
                          << INDENT << "}" << Qt::endl;
                    }else{
                        s << INDENT << "QtJambi_LibraryUtilities.internal.addAllReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
                    }
                }
                break;
            case ReferenceCount::Remove:
                if (declareVariable.isEmpty()) {
                    switch(m_referenceCountContainer[java_function->implementingClass()->fullName()][refCount.variableName]){
                    case ReferenceCount::Put:
                        s << INDENT << "while (" << refCount.variableName << " != null && " << refCount.variableName << ".containsKey(" << argumentName << "))" << Qt::endl
                          << INDENT << "    " << refCount.variableName << ".remove(" << argumentName << ");" << Qt::endl;
                        break;
                    case ReferenceCount::Add:
                        s << INDENT << "while (" << refCount.variableName << " != null && " << refCount.variableName << ".remove(" << argumentName << "));" << Qt::endl;
                        break;
                    default:
                        break;
                    }
                }else{
                    switch(m_referenceCountContainer[declareVariable][refCount.variableName]){
                    case ReferenceCount::Put:
                        s << INDENT << "QtJambi_LibraryUtilities.internal.removeFromMapReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
                        break;
                    case ReferenceCount::Add:
                        s << INDENT << "QtJambi_LibraryUtilities.internal.removeFromCollectionReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ", " << argumentName << ");" << Qt::endl;
                        break;
                    default:
                        break;
                    }
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
    }
    if(!indentation.isNull()){
        indentation.reset();
        switch (refCount.action) {
        case ReferenceCount::Set: {
            s << INDENT << "}else{" << Qt::endl;
            if (declareVariable.isEmpty())
                s << INDENT << "    " << refCount.variableName << " = null;" << Qt::endl;
            else{
                s << INDENT << "    QtJambi_LibraryUtilities.internal.setReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( refCount.threadSafe ? "true" : "false") << ", " << ( isStatic ? "true" : "false") << ", null);" << Qt::endl;
            }
        }
            break;
        default:
            break;
        }
        s << INDENT << "}" << Qt::endl;
    }
    if (refCount.action == ReferenceCount::Put) {
        s << INDENT << "else{" << Qt::endl;
        {
            INDENTATION(INDENT);
            QString keyArgumentName = MetaFunction::argumentByIndex(arguments, int(refCount.keyArgument) - 1)->modifiedArgumentName();
            if (declareVariable.isEmpty()) {
                s << INDENT << "if(" << refCount.variableName << "!=null)" << Qt::endl
                  << INDENT << "    " << refCount.variableName << ".remove(" << keyArgumentName << ");" << Qt::endl;
            }else{
                s << INDENT << "removeFromMapReferenceCount(" << thisName << ", " << declareVariable << ".class, \"" << refCount.variableName << "\", " << ( isStatic ? "true" : "false") << ", " << keyArgumentName << ");" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
    }
}

void JavaGenerator::writeDeprecatedComment(QTextStream& commentStream, const MetaFunction *java_function){
    const MetaFunction *foundFun = nullptr;
    const MetaClass *declaringClass = nullptr;
    bool hasNull = false;
    QString method;
    if(java_function->deprecatedComment().toLower().startsWith(QStringLiteral(u"use "))){
        QString deprecatedComment = java_function->deprecatedComment();
        deprecatedComment = deprecatedComment.mid(4);
        if(deprecatedComment.startsWith(java_function->declaringClass()->qualifiedCppName() + "::")){
            method = deprecatedComment.mid(java_function->declaringClass()->qualifiedCppName().length()+2);
            declaringClass = java_function->declaringClass();
        }else{
            auto idx = deprecatedComment.indexOf(QStringLiteral(u"::"));
            if(idx>0){
                QString className = deprecatedComment.left(idx);
                method = deprecatedComment.mid(idx+2);
                declaringClass = m_classes.findClass(className);
            }else{
                declaringClass = java_function->declaringClass();
                method = deprecatedComment;
            }
        }
        if(method.endsWith(QStringLiteral(u" instead"))){
            method.chop(8);
        }
        if(method.contains(QStringLiteral(u"nullptr"))){
            hasNull = true;
            method.replace(QStringLiteral(u"nullptr"), QString());
        }
    }
    if(declaringClass){
        QByteArray signature = QMetaObject::normalizedSignature(qPrintable(method));
        for(const MetaFunction * fun : declaringClass->functions()){
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
                for(const MetaFunction * fun : declaringClass->functions()){
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
        commentStream << foundFun->declaringClass()->typeEntry()->qualifiedTargetLangName().replace(u'$', u'.');
        commentStream << "#" << foundFun->name() << "(";
        writeFunctionArguments(commentStream, foundFun, {}, int(foundFun->arguments().size()), Option(NoNullness | SkipName | SkipTemplateParameters | NoSuppressExports | VarArgsAsArray));
        commentStream << ")}";
        if(hasNull)
            commentStream << " with <code>null</code>";
        commentStream << " instead" << Qt::endl;
    }else{
        commentStream << "@deprecated " << encodeHtml(java_function->deprecatedComment()) << Qt::endl;
    }
}

void JavaGenerator::writeFunction(QTextStream &s, const MetaFunction *java_function,
                                  uint included_attributes, uint excluded_attributes, Option option) {
    if (java_function->isModifiedRemoved(TS::TargetLangCode))
        return ;
    if (java_function->hasUnresolvedTemplateTypes()){
        return;
    }
    if(!(option & NoNullness) && !m_nullness){
        option = Option(option | NoNullness);
    }
    setupForFunction(java_function, &included_attributes, &excluded_attributes, option);

    const MetaArgumentList& arguments = java_function->arguments();
    {
        QString javaSignature;
        QTextStream s2(&javaSignature);
        s2 << java_function->name();
        {
            QList<QString> templateArguments;
            {
                const QList<Parameter> addedParameterTypes = java_function->addedParameterTypes();
                for(const Parameter& p : addedParameterTypes){
                    if(p.extends.isEmpty())
                        templateArguments << p.name;
                    else
                        templateArguments << p.name + " extends " + p.extends;
                }
            }
            if(!templateArguments.isEmpty())
                s2 << "<" << templateArguments.join(",") << ">";
        }
        s2 << "(";
        writeFunctionArguments(s2, java_function, {}, -1, Option((java_function->isFinal() ? NoOption : VirtualCall) | NoNullness | SkipTemplateParameters | VarArgsAsArray | SkipName));
        s2 << ")";
        s2.flush();
        if(m_currentMethodSignatures.contains(javaSignature)){
            ReportHandler::warning(QStringLiteral(u"Duplicate Java method %1 in %2").arg(javaSignature, java_function->ownerClass()->qualifiedCppName()));
        }
        m_currentMethodSignatures.insert(javaSignature, nullptr);
    }


    static QRegularExpression regExp(QStringLiteral(u"^(insert|set|take|add|remove|install|uninstall).*"));
    if (regExp.match(java_function->name()).hasMatch()) {

        const MetaClass *c = java_function->implementingClass();
        bool hasObjectTypeArgument = false;
        for(MetaArgument *argument : arguments) {
            TS::Ownership cpp_ownership = java_function->ownership(c, TS::NativeCode, argument->argumentIndex() + 1).ownership;
            TS::Ownership java_ownership = java_function->ownership(c, TS::TargetLangCode, argument->argumentIndex() + 1).ownership;
            TS::Ownership shell_ownership = java_function->ownership(c, TS::ShellCode, argument->argumentIndex() + 1).ownership;

            if (argument->type()->typeEntry()->isFunctional()){
                const FunctionalTypeEntry *ftype = reinterpret_cast<const FunctionalTypeEntry *>(argument->type()->typeEntry());
                if(ftype->isFunctionPointer()
                        && cpp_ownership == TS::InvalidOwnership
                        && java_ownership == TS::InvalidOwnership
                        && shell_ownership == TS::InvalidOwnership){
                    hasObjectTypeArgument = true;
                    break;
                }
            }
            if (argument->type()->typeEntry()->isObject()
                    && argument->type()->indirections().size()==1
                    && argument->type()->getReferenceType()==MetaType::NoReference
                    && cpp_ownership == TS::InvalidOwnership
                    && java_ownership == TS::InvalidOwnership
                    && shell_ownership == TS::InvalidOwnership) {
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

    static QRegularExpression regExp2(QStringLiteral(u"^(new|create|clone|take).*"));
    static QRegularExpression regExp3(QStringLiteral(u"^(new|create|clone|take|from).*"));
    if (java_function->type()
            && !java_function->implementingClass()->typeEntry()->designatedInterface()
            && !java_function->type()->indirections().isEmpty()
        && (java_function->isStatic() ? regExp3 : regExp2).match(java_function->name()).hasMatch()) {
        const MetaClass *c = java_function->implementingClass();
        TS::Ownership cpp_ownership = java_function->ownership(c, TS::NativeCode, 0).ownership;
        TS::Ownership java_ownership = java_function->ownership(c, TS::TargetLangCode, 0).ownership;
        TS::Ownership shell_ownership = java_function->ownership(c, TS::ShellCode, 0).ownership;

        if ( !java_function->nullPointersDisabled()
                && cpp_ownership == TS::InvalidOwnership
                    && java_ownership == TS::InvalidOwnership
                    && shell_ownership == TS::InvalidOwnership ) {
            m_factory_functions.append(java_function);
        }
    }
    if((java_function->isConstructor())
        && java_function->wasPublic()
        && (java_function->ownerClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)==TypeEntry::GenerateTargetLang
        && (!java_function->isDeclExplicit() || java_function->isForcedImplicit())
        && !java_function->isCopyConstructor()
        && java_function->declaringClass()->typeEntry()->hasPublicCopyConstructor()
        && java_function->declaringClass()->typeEntry()->hasPublicDefaultConstructor()
        && java_function->declaringClass()->typeEntry()->hasPublicDefaultAssignment()
        && !java_function->declaringClass()->typeEntry()->getNoImplicitConstructors()){
        decltype(arguments.size()) i = 0;
        bool is1Arg = false;
        for(decltype(arguments.size()) l = arguments.size(); i < l; ++i){
            if(!java_function->argumentRemoved(arguments[0]->argumentIndex()+1)
                && arguments[0]->type()
                && arguments[0]->type()->indirections().isEmpty()
                && (arguments[0]->type()->isConstant() || arguments[0]->type()->getReferenceType()!=MetaType::Reference)
                && java_function->typeReplaced(arguments[0]->argumentIndex() + 1).isEmpty()
                && !arguments[0]->type()->typeEntry()->isInitializerList()
                && !arguments[0]->type()->typeEntry()->isQSpan()){
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
            if(!java_function->isDeclExplicit()){
                if(!java_function->isForcedExplicit())
                    m_implicit_constructors.append(java_function);
            }else if(java_function->isForcedImplicit()){
                m_implicit_constructors.append(java_function);
            }
        }
    }

    if(!(((java_function->attributes() & (~excluded_attributes)) | included_attributes) & MetaAttributes::Private)){
        QString comment;
        QTextStream commentStream(&comment);
        bool hasIntro = hasCodeInjections(java_function, {CodeSnip::CommentIntro});
        if(hasIntro){
            writeInjectedCode(commentStream, java_function, CodeSnip::CommentIntro);
        }
        if(!hasIntro || !java_function->href().isEmpty() || !java_function->brief().isEmpty()){
            if (!java_function->isEmptyFunction()) {
                if(!java_function->brief().isEmpty()){
                    commentStream << "<p>" << encodeHtml(java_function->brief()) << "</p>" << Qt::endl;
                }
                commentStream << "<p>See <code>";
                if(!java_function->href().isEmpty())
                    commentStream << "<a href=\"" << docsUrl << java_function->href() << "\">";
                if(java_function->declaringClass() && java_function->functionType()!=MetaFunction::GlobalScopeFunction){
                    commentStream << encodeHtml(java_function->declaringClass()->qualifiedCppName()
                                         .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                         .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                         .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q"))) << "::<wbr/>";
                }
                if(java_function->functionTemplate().first){
                    if(!java_function->functionTemplate().first->originalSignature().isEmpty()){
                        commentStream << encodeHtml(java_function->functionTemplate().first->originalSignature()).replace(",", ",<wbr/>");
                    }else if(!java_function->functionTemplate().first->minimalSignature().isEmpty()){
                        commentStream << encodeHtml(java_function->functionTemplate().first->minimalSignature()).replace(",", ",<wbr/>");
                    }else{
                        commentStream << encodeHtml(java_function->functionTemplate().first->name()) << "(...)";
                    }
                }else {
                    if(!java_function->originalSignature().isEmpty()){
                        commentStream << encodeHtml(java_function->originalSignature()).replace(",", ",<wbr/>");
                    }else if(!java_function->minimalSignature().isEmpty()){
                        commentStream << encodeHtml(java_function->minimalSignature()).replace(",", ",<wbr/>");
                    }else{
                        commentStream << encodeHtml(java_function->name()) << "(...)";
                    }
                }
                if(!java_function->href().isEmpty())
                    commentStream << "</a>";
                commentStream << "</code></p>" << Qt::endl;
                if(java_function->isSelfReturningFunction()){
                    commentStream << "<p>This member function returns the object itself.</p>" << Qt::endl;
                }
                if(!java_function->since().isEmpty()){
                    commentStream << "@since This function was introduced in Qt " << java_function->since() << "." << Qt::endl;
                }
            }
        }
        if(hasCodeInjections(java_function, {CodeSnip::Comment})){
            writeInjectedCode(commentStream, java_function, CodeSnip::Comment);
        }
        if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
            if(!comment.isEmpty())
                commentStream << Qt::endl;
            writeDeprecatedComment(commentStream, java_function);
        }
        if (java_function->isEmptyFunction() && comment.isEmpty()) {
            if(java_function->wasPrivate()){
                commentStream << "Function has no implementation because its native counterpart is private." << Qt::endl;
            }else{
                commentStream << "Function has no implementation." << Qt::endl;
            }
        }else{
            const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
            int argumentCounter = 1;
            while(addedArguments.first.contains(argumentCounter)){
                const ArgumentModification& mod = addedArguments.first[argumentCounter];
                commentStream << "@param " << mod.modified_name;
                if(!mod.comment.isEmpty())
                    commentStream << " " << mod.comment;
                commentStream << Qt::endl;
                ++argumentCounter;
            }
            for (const MetaArgument *arg : arguments){
                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    commentStream << "@param " << arg->modifiedArgumentName();
                    if(!arg->comment().isEmpty())
                        commentStream << " " << arg->comment();
                    commentStream << Qt::endl;
                }
                ++argumentCounter;
                while(addedArguments.first.contains(argumentCounter)){
                    const ArgumentModification& mod = addedArguments.first[argumentCounter];
                    commentStream << "@param " << mod.modified_name;
                    if(!mod.comment.isEmpty())
                        commentStream << " " << mod.comment;
                    commentStream << Qt::endl;
                    ++argumentCounter;
                }
            }
            for(const ArgumentModification& mod : addedArguments.second){
                commentStream << "@param " << mod.modified_name;
                if(!mod.comment.isEmpty())
                    commentStream << " " << mod.comment;
                commentStream << Qt::endl;
            }
            if(java_function->argumentReplaced(0)==u"this"){
                commentStream << "@return the object itself" << Qt::endl;
            }else{
                QString typeReplaced = java_function->typeReplaced(0);
                if(java_function->type() && typeReplaced!=u"void"){
                    if(java_function->isSelfReturningFunction()){
                        commentStream << "@return this";
                        if(!java_function->returnValueComment().isEmpty())
                            commentStream << " " << java_function->returnValueComment();
                        commentStream << Qt::endl;
                    }else{
                        commentStream << "@return";
                        if(!java_function->returnValueComment().isEmpty())
                            commentStream << " " << java_function->returnValueComment();
                        commentStream << Qt::endl;
                    }
                }
            }
        }
        if(!comment.trimmed().isEmpty() && (option & InFunctionComment)==0){
            s << INDENT << "/**" << Qt::endl;
            commentStream.seek(0);
            while(!commentStream.atEnd()){
                s << INDENT << " * " << commentStream.readLine() << Qt::endl;
            }
            s << INDENT << " */" << Qt::endl;
        }
    }

    const QPropertySpec *spec = java_function->propertySpec();
    if (spec) {
        if (java_function->isPropertyReader()) {
            s << INDENT << "@QtPropertyReader(name=\"" << spec->name() << "\")" << Qt::endl;
            if (!spec->designable().isEmpty()){
                s << INDENT << "@QtPropertyDesignable";
                if(spec->designable()!="true")
                    s << "(\"" << spec->designable() << "\")";
                s << Qt::endl;
            }
            if (!spec->scriptable().isEmpty()){
                s << INDENT << "@QtPropertyScriptable";
                if(spec->scriptable()!="true")
                    s << "(\"" << spec->scriptable() << "\")";
                s << Qt::endl;
            }
            if (!spec->stored().isEmpty()){
                s << INDENT << "@QtPropertyStored";
                if(spec->stored()!="true")
                    s << "(\"" << spec->stored() << "\")";
                s << Qt::endl;
            }
//   This seams to be unnecessary in QtJambi
//            if (!spec->revision().isEmpty())
//                s << INDENT << "@QtPropertyRevision(\"" << spec->revision() << "\")" << Qt::endl;
            if (!spec->user().isEmpty()){
                s << INDENT << "@QtPropertyUser";
                if(spec->user()!="true")
                    s << "(\"" << spec->user() << "\")";
                s << Qt::endl;
            }
            if (spec->required())
                s << INDENT << "@QtPropertyRequired" << Qt::endl;
            if (spec->constant())
                s << INDENT << "@QtPropertyConstant" << Qt::endl;
        } else if (java_function->isPropertyWriter()) {
            s << INDENT << "@QtPropertyWriter(name=\"" << spec->name() << "\")" << Qt::endl;
        } else if (java_function->isPropertyResetter()) {
            s << INDENT << "@QtPropertyResetter(name=\"" << spec->name() << "\")" << Qt::endl;
        } else if (java_function->isPropertyBindable()) {
            s << INDENT << "@QtPropertyBindable(name=\"" << spec->name() << "\")" << Qt::endl;
        }
    }

    QString interfaceDefaultImpl;
    {
        Option sigOption = (java_function->isFinal() ? NoOption : VirtualCall);
        if(!(option & NoNullness) && !m_nullness){
            sigOption = Option(sigOption | NoNullness);
        }
        if(java_function->ownerClass()->isInterface()
                && !java_function->isProtected()
                && (sigOption & InFunctionComment)==0){
            if(!java_function->isAbstract()){
                sigOption = DefaultFunction;
            }else{
                const FunctionModificationList mods = java_function->modifications(java_function->ownerClass());
                for(const FunctionModification& mod : mods) {
                    if (mod.snips.count() > 0){
                        for(const CodeSnip& snip : mod.snips) {
                            if (snip.language == TS::Interface){
                                QString code = snip.code();
                                ArgumentMap map = snip.argumentMap;
                                ArgumentMap::iterator it = map.begin();
                                for (;it != map.end();++it) {
                                    int pos = it.key() - 1;
                                    QString meta_name = it.value();
                                    if(pos == -1){
                                        code = code.replace(meta_name, QStringLiteral(u"__qt_return_value"));
                                    }else if (pos >= 0 && pos < arguments.count()) {
                                        code = code.replace(meta_name, MetaFunction::argumentByIndex(arguments, pos)->modifiedArgumentName());
                                    } else {
                                        QString debug = QStringLiteral(u"argument map specifies invalid argument index %1 for function '%2'")
                                                        .arg(pos + 1).arg(java_function->name());
                                        ReportHandler::warning(debug);
                                    }

                                }
                                code = code.replace(QStringLiteral(u"%this"), QStringLiteral(u"this"));
                                QStringList lines = code.split(QStringLiteral(u"\n"));
                                QTextStream s(&interfaceDefaultImpl);
                                {
                                    INDENTATION(INDENT);
                                    printExtraCode(lines, s);
                                }
                            }
                        }
                    }
                }
                if(!interfaceDefaultImpl.isEmpty()){
                    sigOption = DefaultFunction;
                }
            }
        }
        uint attr = (java_function->attributes() & (~excluded_attributes)) | included_attributes;
        if(java_function->name().startsWith(QStringLiteral(u"operator_"))){
            if (!(attr & MetaAttributes::Static && java_function->ownerClass()->typeEntry()->designatedInterface())
                    && !(attr & MetaAttributes::Private))
                ReportHandler::warning(QStringLiteral(u"Non-private operator function %1 in %2").arg(java_function->originalSignature(), java_function->ownerClass()->qualifiedCppName()));
        }
        s << functionSignature(java_function, included_attributes, excluded_attributes, sigOption);
    }

    if(java_function->isEmptyFunction() && !java_function->isAbstract()){
        s << " throws QNoImplementationException {" << Qt::endl
          << INDENT << "    throw new QNoImplementationException();" << Qt::endl
          << INDENT << "}" << Qt::endl
          << INDENT << Qt::endl;
    }else if(!interfaceDefaultImpl.isEmpty()){
        s << "{" << Qt::endl
          << interfaceDefaultImpl
          << INDENT << "}" << Qt::endl
          << INDENT << Qt::endl;
    }else if(java_function->ownerClass()->isInterface() && !java_function->isAbstract()){
        s << "{" << Qt::endl;
        if(!java_function->isStatic() && java_function->isPublic()){
            INDENTATION(INDENT);
            bool has_argument_referenceCounts = false;
            QMap<int,QList<ReferenceCount>> referenceCounts;
            for (int i = -1; i <= arguments.size(); ++i) {
                referenceCounts[i] = java_function->referenceCounts(java_function->implementingClass(), i);
                if (referenceCounts[i].size() > 0) {
                    for(const ReferenceCount& refCount : qAsConst(referenceCounts[i])) {
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
            MetaType *return_type = java_function->type();
            QString new_return_type = QString(java_function->typeReplaced(0)).replace(u'$', u'.');
            bool has_return_type = (new_return_type != "void"
                                    && (!new_return_type.isEmpty() || return_type))
                    || java_function->isSelfReturningFunction();
            OwnershipRule ownershipRule = java_function->ownership(java_function->implementingClass(), TS::TargetLangCode, 0);

            bool has_code_injections_at_the_end = hasCodeInjections(java_function, {CodeSnip::End, CodeSnip::Position4, CodeSnip::Position3});

            bool needs_return_variable = has_return_type
                                         && ( ( ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership)
                                              || has_argument_referenceCounts || referenceCounts[0].size() > 0 || has_code_injections_at_the_end);

            s << INDENT;
            if (has_return_type && (java_function->argumentReplaced(0).isEmpty() || java_function->argumentReplaced(0)==QStringLiteral(u"this"))) {
                if (needs_return_variable) {
                    if (new_return_type.isEmpty())
                        s << qualifiedJavaType(return_type, java_function->implementingClass(), Option(IsReturnType | (m_nullness ? NoOption : NoNullness)));
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
                        s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace(u'$', u'.') << "(";
                    }
                }
            }

            s << "Impl." << java_function->marshalledName() << "(";
            bool needsComma = false;
            if(!java_function->isStatic()){
                if(java_function->implementingClass()->typeEntry()->isNativeIdBased()){
                    s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
                }else if(java_function->implementingClass()->typeEntry()->isQMetaObjectType()){
                    s << "__qt_persistentPointer";
                }else
                    s << "this";
                needsComma = true;
            }

            for (int i = 0; i < arguments.count(); ++i) {
                const MetaArgument *arg = arguments.at(i);
                const MetaType *type = arg->type();

                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    if(needsComma)
                        s << ", ";

                    if(!java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()
                            || java_function->useArgumentAsBuffer(arg->argumentIndex()+1)){
                        s << arg->modifiedArgumentName();
                    }else if(java_function->useArgumentAsArray(arg->argumentIndex()+1)){
                        s << arg->modifiedArgumentName();
                        if(java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                            s << ", ";
                            int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
                            if(lengthParameter>0 && lengthParameter<=java_function->arguments().size()){
                                const MetaArgument *lengthParam = java_function->arguments()[lengthParameter - 1];
                                if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                                    lengthParam = nullptr;
                                    for(const MetaArgument *argument : java_function->arguments()) {
                                        if(argument && argument->argumentIndex()+1==lengthParameter){
                                            lengthParam = argument;
                                        }
                                    }
                                }
                                if(lengthParam && java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                                    s << ", offsetOf";
                                    QString modifiedArgumentName = arg->modifiedArgumentName();
                                    modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                                    s << modifiedArgumentName;
                                }
                            }
                        }
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

            for(ReferenceCount refCount : qAsConst(referenceCounts[-1])){
                refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace(u'$', u'.');
                writeReferenceCount(s, refCount, -1, java_function, {}, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
            }

            // We must ensure we retain a Java hard-reference over the native method call
            // so that the GC will not destroy the C++ object too early.  At this point we
            // have called the native method call so can manage referenceCount issues.
            // First the input arguments
            for (const MetaArgument* argument : qAsConst(arguments)) {
                for(ReferenceCount refCount : qAsConst(referenceCounts[argument->argumentIndex()+1])){
                    refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace(u'$', u'.');
                    writeReferenceCount(s, refCount, argument->argumentIndex()+1, java_function, {}, QLatin1String("this"));
                }
            }

            if (!java_function->argumentReplaced(0).isEmpty() && java_function->argumentReplaced(0)!=QStringLiteral(u"this")) {
                s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
            }else{
                // Then the return value
                for(ReferenceCount referenceCount : qAsConst(referenceCounts[0])) {
                    referenceCount.declareVariable = java_function->declaringClass()->fullName().replace(u'/', u'.').replace(u'$', u'.');
                    writeReferenceCount(s, referenceCount, 0, java_function, {}, QStringLiteral(u"this"));
                }

                writeInjectedCode(s, java_function, CodeSnip::Position3);
                writeInjectedCode(s, java_function, CodeSnip::Position4);
                writeInjectedCode(s, java_function, CodeSnip::End);

                if (needs_return_variable) {
                    if (ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership) {
                        if (return_type->isContainer()){
                            s << INDENT << "if (__qt_return_value != null";
                            if(!ownershipRule.condition.isEmpty()){
                                s << " && " << ownershipRule.condition.replace("%arg", "__qt_return_value");
                            }
                            s << ") {" << Qt::endl;
                            {
                                INDENTATION(INDENT);
                                writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, QStringLiteral(u"__qt_return_value"));
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else if(!ownershipRule.condition.isEmpty()){
                            s << INDENT << "if (" << ownershipRule.condition.replace("%arg", "__qt_return_value") << ") {" << Qt::endl;
                            {
                                INDENTATION(INDENT);
                                writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, QStringLiteral(u"__qt_return_value"));
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else{
                            writeFunctionCallForOwnership(s, java_function, ownershipRule.ownership, QStringLiteral(u"__qt_return_value"));
                        }
                    }
                    s << INDENT << "return __qt_return_value;" << Qt::endl;
                }
            }
        }else{
            INDENTATION(INDENT);
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
                MetaArgument *arg = arguments.at(i);
                if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                    if(hasArg)
                        s << ", ";
                    s << arguments.at(i)->modifiedArgumentName();
                    hasArg = true;
                    if(java_function->useArgumentAsArray(arg->argumentIndex()+1)
                        && java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                        int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
                        if(lengthParameter>0 && lengthParameter<=java_function->arguments().size()){
                            const MetaArgument *lengthParam = java_function->arguments()[lengthParameter - 1];
                            if(!lengthParam || lengthParam->argumentIndex()+1!=lengthParameter){
                                lengthParam = nullptr;
                                for(const MetaArgument *argument : java_function->arguments()) {
                                    if(argument && argument->argumentIndex()+1==lengthParameter){
                                        lengthParam = argument;
                                    }
                                }
                            }
                            if(lengthParam && java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                                s << ", offsetOf";
                                QString modifiedArgumentName = arg->modifiedArgumentName();
                                modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                                s << modifiedArgumentName;
                            }
                        }
                    }
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
            s << "{" << Qt::endl;
            {
                INDENTATION(INDENT);
                writeJavaCallThroughContents(s, java_function);
            }
            s << INDENT << "}" << Qt::endl
              << INDENT << Qt::endl;
        }

        if((java_function->isAbstract() || !(java_function->originalAttributes() & MetaAttributes::Public)) && !java_function->implementingClass()->generateShellClass()){
            // do nothing
        }else if(java_function->isAbstract() && java_function->implementingClass()->hasUnimplmentablePureVirtualFunction()){
            // do nothing
        }else{
            writePrivateNativeFunction(s, java_function);
        }
    } else {
        s << ";" << Qt::endl
          << INDENT << Qt::endl;
    }
}

void JavaGenerator::write_equals_parts(QTextStream &s, const MetaFunctionList &lst, char prefix, bool& first, bool& suppressUnchecked, bool& suppressRaw, bool &wroteNull, QList<QPair<const MetaFunction*,QString>>& javaTypesByFunction) {
    bool suppressNull = false;
    for(MetaFunction *f : lst) {
        MetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            if(arg->type()->typeEntry()->designatedInterface())
                type = arg->type()->typeEntry()->designatedInterface()->qualifiedTargetLangName();
            else
                type = arg->type()->typeEntry()->qualifiedTargetLangName();
        }
        if(type==QStringLiteral(u"java.lang.Void") || type==QStringLiteral(u"null")){
            suppressNull = true;
            break;
        }
    }
    for(MetaFunction *f : lst) {
        MetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            if(arg->type()->typeEntry()->designatedInterface())
                type = arg->type()->typeEntry()->designatedInterface()->qualifiedTargetLangName();
            else
                type = arg->type()->typeEntry()->qualifiedTargetLangName();
            registerPackage(type);
            type = type.replace(u'$', u'.');
            if(arg->type()->typeEntry()->isContainer()){
                if(type==QStringLiteral(u"java.util.List")
                        ||  type==QStringLiteral(u"java.util.LinkedList")
                        ||  type==QStringLiteral(u"java.util.Queue")
                        ||  type==QStringLiteral(u"java.util.Deque")
                        ||  type==QStringLiteral(u"java.util.ArrayList")
                        ||  type==QStringLiteral(u"java.util.Vector")
                        ||  type==QStringLiteral(u"java.util.Set")
                        ||  type==QStringLiteral(u"io.qt.core.QSet")
                        ||  type==QStringLiteral(u"io.qt.core.QList")
                        ||  type==QStringLiteral(u"io.qt.core.QQueue")
                        ||  type==QStringLiteral(u"io.qt.core.QVector")
                        ||  type==QStringLiteral(u"io.qt.core.QStack")
                        ||  type==QStringLiteral(u"io.qt.core.QLinkedList")){
                    type = QStringLiteral(u"java.util.Collection");
                }else if(type==QStringLiteral(u"java.util.Map")
                         ||  type==QStringLiteral(u"java.util.SortedMap")
                         ||  type==QStringLiteral(u"java.util.NavigableMap")
                         ||  type==QStringLiteral(u"java.util.HashMap")
                         ||  type==QStringLiteral(u"java.util.TreeMap")
                         ||  type==QStringLiteral(u"io.qt.core.QMap")
                         ||  type==QStringLiteral(u"io.qt.core.QHash")
                         ||  type==QStringLiteral(u"io.qt.core.QMultiMap")
                         ||  type==QStringLiteral(u"io.qt.core.QMultiHash")){
                    type = QStringLiteral(u"java.util.Map");
                }
            }
            const QList<const MetaType *>& instantiations = arg->type()->instantiations();
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
        if(boxedType==QStringLiteral(u"io.qt.core.QString"))
            boxedType = type = QStringLiteral(u"java.lang.CharSequence");
        else if(boxedType==QStringLiteral(u"int"))
            boxedType = QStringLiteral(u"java.lang.Integer");
        else if(boxedType==QStringLiteral(u"byte"))
            boxedType = QStringLiteral(u"java.lang.Byte");
        else if(boxedType==QStringLiteral(u"short"))
            boxedType = QStringLiteral(u"java.lang.Short");
        else if(boxedType==QStringLiteral(u"long"))
            boxedType = QStringLiteral(u"java.lang.Long");
        else if(boxedType==QStringLiteral(u"double"))
            boxedType = QStringLiteral(u"java.lang.Double");
        else if(boxedType==QStringLiteral(u"float"))
            boxedType = QStringLiteral(u"java.lang.Float");
        else if(boxedType==QStringLiteral(u"boolean"))
            boxedType = QStringLiteral(u"java.lang.Boolean");
        else if(boxedType==QStringLiteral(u"char"))
            boxedType = QStringLiteral(u"java.lang.Character");

        if(type==QStringLiteral(u"java.lang.Void")){
            wroteNull = true;
            javaTypesByFunction.append({f, "<code>null</code>"});
            s << INDENT << (first ? "if" : "else if") << " (other==null) {" << Qt::endl;
            first = false;
            s << INDENT << "    return ";
            if (prefix != 0) s << prefix;
            s << f->name() << "((java.lang.Void)null);" << Qt::endl;
        }else if(type==QStringLiteral(u"null")){
            wroteNull = true;
            javaTypesByFunction.append({f, "<code>null</code>"});
            s << INDENT << (first ? "if" : "else if") << " (other==" << type;
            first = false;
            s << ") {" << Qt::endl;
            s << INDENT << "    return ";
            if (prefix != 0) s << prefix;
            s << f->name() << "();" << Qt::endl;
        }else{
            if(boxedType.endsWith(QStringLiteral(u"[]")))
                javaTypesByFunction.append({f, "<code>"+boxedType+"</code>"});
            else
                javaTypesByFunction.append({f, "<code>{@link "+boxedType+"}</code>"});
            s << INDENT << (first ? "if" : "else if") << " (";
            bool closeParen = false;
            if(!suppressNull && !wroteNull && arg->type()->typeEntry()==f->declaringClass()->typeEntry()){
                wroteNull = true;
                s << "other==null || ";
                closeParen = true;
                if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size())
                    s << "(";
            }
            s << "other instanceof " << boxedType;
            first = false;
            if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size()){
                s << " && checkContainerType((" << type << ") other";
                for(const MetaType * instantiation : arg->type()->instantiations()){
                    s << ", " << instantiation->typeEntry()->qualifiedTargetLangName() << ".class";
                }
                s << ")";
                if(closeParen)
                    s << ")";
            }
            s << ") {" << Qt::endl;
            s << INDENT << "    return ";
            if (prefix != 0) s << prefix;
            s << f->name() << "((" << type << ") other);" << Qt::endl;
            QStringList implicitCalls = f->implicitCalls(arg->argumentIndex()+1);
            if(arg->type()->typeEntry()->isComplex()){
                implicitCalls << static_cast<const ComplexTypeEntry *>(arg->type()->typeEntry())->implicitCasts();
            }
            QSet<QString> usedImplicitCalls;
            for(QString jarg : qAsConst(implicitCalls)){
                jarg = annotationFreeTypeName(jarg);
                QString boxedType = jarg;
                if(boxedType==QStringLiteral(u"io.qt.core.QString"))
                    boxedType = type = QStringLiteral(u"java.lang.CharSequence");
                else if(boxedType==QStringLiteral(u"int"))
                    boxedType = QStringLiteral(u"java.lang.Integer");
                else if(boxedType==QStringLiteral(u"byte"))
                    boxedType = QStringLiteral(u"java.lang.Byte");
                else if(boxedType==QStringLiteral(u"short"))
                    boxedType = QStringLiteral(u"java.lang.Short");
                else if(boxedType==QStringLiteral(u"long"))
                    boxedType = QStringLiteral(u"java.lang.Long");
                else if(boxedType==QStringLiteral(u"double"))
                    boxedType = QStringLiteral(u"java.lang.Double");
                else if(boxedType==QStringLiteral(u"float"))
                    boxedType = QStringLiteral(u"java.lang.Float");
                else if(boxedType==QStringLiteral(u"boolean"))
                    boxedType = QStringLiteral(u"java.lang.Boolean");
                else if(boxedType==QStringLiteral(u"char"))
                    boxedType = QStringLiteral(u"java.lang.Character");
                if(usedImplicitCalls.contains(boxedType))
                    continue;
                usedImplicitCalls.insert(boxedType);
                if(boxedType.endsWith(QStringLiteral(u"[]")))
                    javaTypesByFunction.append({f, "<code>"+boxedType+"</code>"});
                else
                    javaTypesByFunction.append({f, "<code>{@link "+boxedType+"}</code>"});
                s << INDENT << "}" << Qt::endl
                  << INDENT << (first ? "if" : "else if") << " (other instanceof " << boxedType << ") {" << Qt::endl
                  << INDENT << "    return ";
                first = false;
                if (prefix != 0) s << prefix;
                s << f->name() << "((" << jarg << ") other);" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
    }
}

void JavaGenerator::write_compareto_parts(QTextStream &s, const MetaFunctionList &lst, int value, bool& first, bool& suppressUnchecked, bool& suppressRaw, bool &wroteNull, QList<QPair<const MetaFunction*,QString>>& javaTypesByFunction) {
    bool suppressNull = false;
    for(MetaFunction *f : lst) {
        MetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            if(arg->type()->typeEntry()->designatedInterface())
                type = arg->type()->typeEntry()->designatedInterface()->qualifiedTargetLangName();
            else
                type = arg->type()->typeEntry()->qualifiedTargetLangName();
        }
        if(type==QStringLiteral(u"java.lang.Void") || type==QStringLiteral(u"null")){
            suppressNull = true;
            break;
        }
    }
    for(MetaFunction *f : lst) {
        MetaArgument *arg = f->arguments().at(0);
        QString type = f->typeReplaced(arg->argumentIndex()+1);
        if (type.isEmpty()){
            type = arg->type()->typeEntry()->qualifiedTargetLangName();
            if(arg->type()->typeEntry()->isContainer()){
                if(type==QStringLiteral(u"java.util.List")
                       ||  type==QStringLiteral(u"java.util.LinkedList")
                       ||  type==QStringLiteral(u"java.util.Queue")
                       ||  type==QStringLiteral(u"java.util.Deque")
                       ||  type==QStringLiteral(u"java.util.ArrayList")
                       ||  type==QStringLiteral(u"java.util.Vector")
                       ||  type==QStringLiteral(u"java.util.Set")
                       ||  type==QStringLiteral(u"io.qt.core.QSet")
                       ||  type==QStringLiteral(u"io.qt.core.QList")
                       ||  type==QStringLiteral(u"io.qt.core.QQueue")
                       ||  type==QStringLiteral(u"io.qt.core.QVector")
                       ||  type==QStringLiteral(u"io.qt.core.QStack")
                       ||  type==QStringLiteral(u"io.qt.core.QLinkedList")) {
                    type = QStringLiteral(u"java.util.Collection");
                }else if(type==QStringLiteral(u"java.util.Map")
                       ||  type==QStringLiteral(u"java.util.SortedMap")
                       ||  type==QStringLiteral(u"java.util.NavigableMap")
                       ||  type==QStringLiteral(u"java.util.HashMap")
                       ||  type==QStringLiteral(u"java.util.TreeMap")
                       ||  type==QStringLiteral(u"io.qt.core.QMap")
                       ||  type==QStringLiteral(u"io.qt.core.QHash")
                       ||  type==QStringLiteral(u"io.qt.core.QMultiMap")
                       ||  type==QStringLiteral(u"io.qt.core.QMultiHash")){
                    type = QStringLiteral(u"java.util.Map");
                }
            }
            const QList<const MetaType *>& instantiations = arg->type()->instantiations();
            if(instantiations.size()>0){
                suppressRaw = true;
                suppressUnchecked = true;
            }
        }
        type = type.replace(u'$', u'.');
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
        if(boxedType==QStringLiteral(u"io.qt.core.QString"))
            boxedType = type = QStringLiteral(u"java.lang.CharSequence");
        else if(boxedType==QStringLiteral(u"int"))
            boxedType = QStringLiteral(u"java.lang.Integer");
        else if(boxedType==QStringLiteral(u"byte"))
            boxedType = QStringLiteral(u"java.lang.Byte");
        else if(boxedType==QStringLiteral(u"short"))
            boxedType = QStringLiteral(u"java.lang.Short");
        else if(boxedType==QStringLiteral(u"long"))
            boxedType = QStringLiteral(u"java.lang.Long");
        else if(boxedType==QStringLiteral(u"double"))
            boxedType = QStringLiteral(u"java.lang.Double");
        else if(boxedType==QStringLiteral(u"float"))
            boxedType = QStringLiteral(u"java.lang.Float");
        else if(boxedType==QStringLiteral(u"boolean"))
            boxedType = QStringLiteral(u"java.lang.Boolean");
        else if(boxedType==QStringLiteral(u"char"))
            boxedType = QStringLiteral(u"java.lang.Character");
        QStringList implicitCalls = f->implicitCalls(arg->argumentIndex()+1);
        if(arg->type()->typeEntry()->isComplex()){
            implicitCalls << static_cast<const ComplexTypeEntry *>(arg->type()->typeEntry())->implicitCasts();
        }
        if(value==0){
            if(boxedType.endsWith(QStringLiteral(u"[]")))
                javaTypesByFunction.append({f, "<code>"+boxedType+"</code>"});
            else
                javaTypesByFunction.append({f, "<code>{@link "+boxedType+"}</code>"});
            s << INDENT << (first ? "if" : "else if") << " (";
            bool closeParen = false;
            if(!suppressNull && !wroteNull && arg->type()->typeEntry()==f->declaringClass()->typeEntry()){
                wroteNull = true;
                s << "other==null || ";
                closeParen = true;
                if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size())
                    s << "(";
            }
            s << "other instanceof " << boxedType;
            first = false;
            if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size()){
                s << " && checkContainerType((" << type << ") other";
                for(const MetaType * instantiation : arg->type()->instantiations()){
                    s << ", " << instantiation->typeEntry()->qualifiedTargetLangName() << ".class";
                }
                s << ")";
                if(closeParen)
                    s << ")";
            }
            s << ") {" << Qt::endl
              << INDENT << "    return " << f->name() << "((" << type << ") other);" << Qt::endl
              << INDENT << "}" << Qt::endl;
            QSet<QString> usedImplicitCalls;
            for(QString jarg : qAsConst(implicitCalls)){
                jarg = annotationFreeTypeName(jarg);
                QString boxedType = jarg;
                if(boxedType==QStringLiteral(u"io.qt.core.QString"))
                    boxedType = type = QStringLiteral(u"java.lang.CharSequence");
                else if(boxedType==QStringLiteral(u"int"))
                    boxedType = QStringLiteral(u"java.lang.Integer");
                else if(boxedType==QStringLiteral(u"byte"))
                    boxedType = QStringLiteral(u"java.lang.Byte");
                else if(boxedType==QStringLiteral(u"short"))
                    boxedType = QStringLiteral(u"java.lang.Short");
                else if(boxedType==QStringLiteral(u"long"))
                    boxedType = QStringLiteral(u"java.lang.Long");
                else if(boxedType==QStringLiteral(u"double"))
                    boxedType = QStringLiteral(u"java.lang.Double");
                else if(boxedType==QStringLiteral(u"float"))
                    boxedType = QStringLiteral(u"java.lang.Float");
                else if(boxedType==QStringLiteral(u"boolean"))
                    boxedType = QStringLiteral(u"java.lang.Boolean");
                else if(boxedType==QStringLiteral(u"char"))
                    boxedType = QStringLiteral(u"java.lang.Character");
                if(usedImplicitCalls.contains(boxedType))
                    continue;
                usedImplicitCalls.insert(boxedType);
                if(boxedType.endsWith(QStringLiteral(u"[]")))
                    javaTypesByFunction.append({f, "<code>"+boxedType+"</code>"});
                else
                    javaTypesByFunction.append({f, "<code>{@link "+boxedType+"}</code>"});
                s << INDENT << (first ? "if" : "else if") << " (other instanceof " << boxedType << ") {" << Qt::endl
                  << INDENT << "    return " << f->name() << "((" << jarg << ") other);" << Qt::endl
                  << INDENT << "}" << Qt::endl;
                first = false;
            }
        }else{
            if(type==QStringLiteral(u"java.lang.Void")){
                javaTypesByFunction.append({f, QStringLiteral(u"<code>null</code>")});
                wroteNull = true;
                s << INDENT << (first ? "if" : "else if") << " (other==null) {" << Qt::endl;
                s << INDENT << "    if (" << f->name() << "((java.lang.Void)null)) return " << value << ";" << Qt::endl
                  << INDENT << "    else return " << -value << ";" << Qt::endl
                  << INDENT << "}" << Qt::endl;
                first = false;
            }else if(type==QStringLiteral(u"null")){
                javaTypesByFunction.append({f, QStringLiteral(u"<code>null</code>")});
                wroteNull = true;
                s << INDENT << (first ? "if" : "else if") << " (other==" << type;
                s << ") {" << Qt::endl;
                s << INDENT << "    if (" << f->name() << "()) return " << value << ";" << Qt::endl
                  << INDENT << "    else return " << -value << ";" << Qt::endl
                  << INDENT << "}" << Qt::endl;
                first = false;
            }else{
                if(boxedType.endsWith(QStringLiteral(u"[]")))
                    javaTypesByFunction.append({f, QStringLiteral(u"<code>")+boxedType+QStringLiteral(u"</code>")});
                else
                    javaTypesByFunction.append({f, "<code>{@link "+boxedType+"}</code>"});
                s << INDENT << (first ? "if" : "else if") << " (";
                bool closeParen = false;
                if(!suppressNull && !wroteNull && arg->type()->typeEntry()==f->declaringClass()->typeEntry()){
                    wroteNull = true;
                    s << "other==null || ";
                    closeParen = true;
                    if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size())
                        s << "(";
                }
                s << "other instanceof " << boxedType;
                first = false;
                if(arg->type()->typeEntry()->isContainer() && arg->type()->instantiations().size()){
                    s << " && checkContainerType((" << type << ") other";
                    for(const MetaType * instantiation : arg->type()->instantiations()){
                        s << ", " << instantiation->typeEntry()->qualifiedTargetLangName() << ".class";
                    }
                    s << ")";
                    if(closeParen)
                        s << ")";
                }
                s << ") {" << Qt::endl;
                s << INDENT << "    if (" << f->name() << "((" << type << ") other)) return " << value << ";" << Qt::endl
                  << INDENT << "    else return " << -value << ";" << Qt::endl
                  << INDENT << "}" << Qt::endl;
                QSet<QString> usedImplicitCalls;
                for(QString jarg : qAsConst(implicitCalls)){
                    jarg = annotationFreeTypeName(jarg);
                    QString boxedType = jarg;
                    if(boxedType==QStringLiteral(u"io.qt.core.QString"))
                        boxedType = type = QStringLiteral(u"java.lang.CharSequence");
                    else if(boxedType==QStringLiteral(u"int"))
                        boxedType = QStringLiteral(u"java.lang.Integer");
                    else if(boxedType==QStringLiteral(u"byte"))
                        boxedType = QStringLiteral(u"java.lang.Byte");
                    else if(boxedType==QStringLiteral(u"short"))
                        boxedType = QStringLiteral(u"java.lang.Short");
                    else if(boxedType==QStringLiteral(u"long"))
                        boxedType = QStringLiteral(u"java.lang.Long");
                    else if(boxedType==QStringLiteral(u"double"))
                        boxedType = QStringLiteral(u"java.lang.Double");
                    else if(boxedType==QStringLiteral(u"float"))
                        boxedType = QStringLiteral(u"java.lang.Float");
                    else if(boxedType==QStringLiteral(u"boolean"))
                        boxedType = QStringLiteral(u"java.lang.Boolean");
                    else if(boxedType==QStringLiteral(u"char"))
                        boxedType = QStringLiteral(u"java.lang.Character");
                    if(usedImplicitCalls.contains(boxedType))
                        continue;
                    usedImplicitCalls.insert(boxedType);
                    if(boxedType.endsWith(QStringLiteral(u"[]")))
                        javaTypesByFunction.append({f, "<code>"+boxedType+"</code>"});
                    else
                        javaTypesByFunction.append({f, "<code>{@link "+boxedType+"}</code>"});
                    s << INDENT << (first ? "if" : "else if") << " (other instanceof " << boxedType << ") {" << Qt::endl
                      << INDENT << "    if (" << f->name() << "((" << jarg << ") other)) return " << value << ";" << Qt::endl
                      << INDENT << "    else return " << -value << ";" << Qt::endl
                      << INDENT << "}" << Qt::endl;
                    first = false;
                }
            }
        }
    }
}

const MetaType * JavaGenerator::getIterableType(const MetaClass *cls, bool& isConst) const{
    const MetaFunctionList& begin_functions = cls->beginFunctions();
    const MetaFunctionList& end_functions = cls->endFunctions();
    for(const MetaFunction* f : begin_functions){
        if(f->name()=="constBegin"){
            isConst = true;
            break;
        }
    }
    if(begin_functions.isEmpty() || end_functions.isEmpty()){
        return nullptr;
    }
    if(begin_functions.first()->type()->typeEntry()->qualifiedCppName()==end_functions.first()->type()->typeEntry()->qualifiedCppName()){
        if (begin_functions.first()->type()->isIterator()) {
            if(!begin_functions.first()->type()->iteratorInstantiations().isEmpty()){
                return begin_functions.first()->type()->iteratorInstantiations().first();
            }
            const IteratorTypeEntry* iteratorType = static_cast<const IteratorTypeEntry*>(begin_functions.first()->type()->typeEntry());
            if(MetaClass * iteratorClass = cls->findIterator(iteratorType)){
                if(iteratorClass->templateBaseClassInstantiations().size()==1){
                    return iteratorClass->templateBaseClassInstantiations().at(0);
                }
                for(MetaFunction* function : iteratorClass->functions()){
                    if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                        return function->type();
                    }
                }
            }else if(MetaClass * iteratorClass = m_classes.findClass(iteratorType->qualifiedCppName(), MetaClassList::QualifiedCppName)){
                if(iteratorClass->templateBaseClassInstantiations().size()==1){
                    return iteratorClass->templateBaseClassInstantiations().at(0);
                }
                for(MetaFunction* function : iteratorClass->functions()){
                    if(function->originalName()=="operator*" && function->type() && function->arguments().isEmpty() && function->isConstant()){
                        return function->type();
                    }
                }
            }
        }
    }
    return nullptr;
}

bool JavaGenerator::isComparable(const MetaClass *cls) const {
    const MetaFunctionList& eq_functions = cls->equalsFunctions();
    const MetaFunctionList& neq_functions = cls->notEqualsFunctions();

    // Write the comparable functions
    const MetaFunctionList& gt_functions = cls->greaterThanFunctions();
    const MetaFunctionList& geq_functions = cls->greaterThanEqFunctions();
    const MetaFunctionList& lt_functions = cls->lessThanFunctions();
    const MetaFunctionList& leq_functions = cls->lessThanEqFunctions();
    const MetaFunctionList& compare_functions = cls->compareFunctions();

    bool hasEquals = eq_functions.size() || neq_functions.size();
    bool isComparable = compare_functions.size() || (hasEquals
                                                        ? gt_functions.size() || geq_functions.size() || lt_functions.size() || leq_functions.size()
                                                        : gt_functions.size() == 1 || lt_functions.size() == 1);

    return isComparable;
}

bool hasImplicitCalls(const MetaFunction* function){
    MetaArgument *arg = function->arguments().at(0);
    QString type = function->typeReplaced(arg->argumentIndex()+1);
    if (type.isEmpty()){
        if(!function->implicitCalls(arg->argumentIndex()+1).isEmpty()){
            return true;
        }
        if(arg->type()->typeEntry()->isComplex() && !static_cast<const ComplexTypeEntry *>(arg->type()->typeEntry())->implicitCasts().isEmpty()){
            return true;
        }
    }
    return false;
}

void JavaGenerator::writeJavaLangObjectOverrideFunctions(QTextStream &s,
        const MetaClass *cls) {
    const MetaFunctionList& eq_functions = cls->equalsFunctions();
    const MetaFunctionList& neq_functions = cls->notEqualsFunctions();

    bool writeHashEquals = true;
    if (eq_functions.size() || neq_functions.size()) {
        bool suppressRaw = false;
        bool suppressUnchecked = false;
        QList<QPair<const MetaFunction*,QString>> javaTypesByFunction;
        QString eq;
        {
            INDENTATION(INDENT);
            QTextStream _s(&eq);
            bool first = true;
            bool wroteNull = false;
            write_equals_parts(_s, eq_functions,  char(0), first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
            write_equals_parts(_s, neq_functions, '!', first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
        }
        s << Qt::endl;
        s << INDENT << "/**" << Qt::endl;
        if(javaTypesByFunction.size()==1){
            auto f = javaTypesByFunction[0].first;
            s << INDENT << " * <p>See <code>";
            if(!f->href().isEmpty()){
                s << "<a href=\"" << docsUrl << f->href() << "\">";
            }
            if(f->functionType()!=MetaFunction::GlobalScopeFunction){
                s << encodeHtml(cls->qualifiedCppName()
                         .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                         .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                         .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                  << "::<wbr/>";
            }
            s << encodeHtml(f->originalSignature()).replace(",", ",<wbr/>");
            if(!f->href().isEmpty())
                s << "</a>";
            s << "</code></p>" << Qt::endl;
        }else{
            s << INDENT << " * <p>Checks for equality depending on the type of given object.</p><ul>" << Qt::endl;
            for(const QPair<const MetaFunction*,QString>& pair : qAsConst(javaTypesByFunction)){
                auto f = pair.first;
                if(f){
                    s << INDENT << " * <li>"+pair.second+" ckecked by to <code>";
                    if(!f->href().isEmpty()){
                        s << "<a href=\"" << docsUrl << f->href() << "\">";
                    }
                    if(f->functionType()!=MetaFunction::GlobalScopeFunction){
                        s << encodeHtml(cls->qualifiedCppName()
                                 .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                 .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                 .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                          << "::<wbr/>";
                    }
                    s << encodeHtml(f->originalSignature()).replace(",", ",<wbr/>");
                    if(!f->href().isEmpty())
                        s << "</a>";
                    s << "</code></li>" << Qt::endl;
                }
            }
            s << INDENT << " * </ul>" << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
        s << INDENT << "@Override" << Qt::endl;
        s << INDENT << "@QtUninvokable" << Qt::endl;
        if(suppressRaw && suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"rawtypes\", \"unchecked\"})" << Qt::endl;
        else if(suppressRaw)
            s << INDENT << "@SuppressWarnings({\"rawtypes\"})" << Qt::endl;
        else if(suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"unchecked\"})" << Qt::endl;
        s << INDENT << "public boolean equals(Object other) {" << Qt::endl;
        {
            INDENTATION(INDENT);
            QStringList lines;
            for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::Equals){
                    lines << codeSnip.code().split("\n");
                }
            }
            if(!lines.isEmpty())
                printExtraCode(lines, s);
            s << eq << Qt::endl;
            s << INDENT << "return false;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl << Qt::endl;
        writeHashEquals = false;
    }else if(cls->typeEntry()->isQMetaObjectType()){
        QString nullable = m_nullness ? QStringLiteral(u"@Nullable ") : QString{};
        s << Qt::endl
          << INDENT << "/**" << Qt::endl
          << INDENT << " * Checks for equality depending on the type of given object." << Qt::endl
          << INDENT << " */" << Qt::endl
          << INDENT << "@QtUninvokable" << Qt::endl
          << INDENT << "public boolean equals(" << nullable << cls->typeEntry()->targetLangName() << " other) {" << Qt::endl
          << INDENT << "    return other!=null && other.__qt_persistentPointer==__qt_persistentPointer;" << Qt::endl
          << INDENT << "}" << Qt::endl;
        s << Qt::endl
          << INDENT << "/**" << Qt::endl
          << INDENT << " * Checks for equality depending on the type of given object." << Qt::endl
          << INDENT << " */" << Qt::endl
          << INDENT << "@QtUninvokable" << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "public boolean equals(Object other) {" << Qt::endl;
        QStringList lines;
        for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
            if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::Equals){
                lines << codeSnip.code().split("\n");
            }
        }
        if(!lines.isEmpty()){
            INDENTATION(INDENT);
            printExtraCode(lines, s);
        }
        if(lines.isEmpty() || !lines.last().contains("return ")){
            INDENTATION(INDENT);
            s << INDENT << "if (this instanceof " << cls->typeEntry()->targetLangName() << ")" << Qt::endl
              << INDENT << "    return equals((" << cls->typeEntry()->targetLangName() << ") other);" << Qt::endl
              << INDENT << "return false;" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl;
        writeHashEquals = false;
    }else{
        MetaClass* baseClass = cls->baseClass();
        while(baseClass){
            if(baseClass->typeEntry()->hasEquals() || baseClass->typeEntry()->isContainer()){
                break;
            }
            baseClass = baseClass->baseClass();
        }
        if(!baseClass){
            if (cls->templateBaseClass() && cls->templateBaseClass()->typeEntry()->isContainer()){
                writeHashEquals = false;
            }
        }else{
            writeHashEquals = false;
        }
    }

    if(writeHashEquals){
        if (cls->hasHashFunction()) {
            s << Qt::endl;
            s << INDENT << "/**" << Qt::endl;
            s << INDENT << " * Checks for equality by checking object's hash code." << Qt::endl;
            s << INDENT << " */" << Qt::endl;
            s << INDENT << "@Override" << Qt::endl;
            s << INDENT << "@QtUninvokable" << Qt::endl;
            s << INDENT << "public boolean equals(Object other) {" << Qt::endl;
            {
                INDENTATION(INDENT);
                QStringList lines;
                for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                    if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::Equals){
                        lines << codeSnip.code().split("\n");
                    }
                }
                if(!lines.isEmpty())
                    printExtraCode(lines, s);
                QString nonGenericType = cls->typeEntry()->qualifiedTargetLangName();
                registerPackage(nonGenericType);
                nonGenericType = nonGenericType.replace(u'$', u'.');
                auto idx = nonGenericType.indexOf("<");
                if(idx>0)
                    nonGenericType = nonGenericType.left(idx);
                if(nonGenericType==QStringLiteral(u"int"))
                    nonGenericType = QStringLiteral(u"java.lang.Integer");
                else if(nonGenericType==QStringLiteral(u"byte"))
                    nonGenericType = QStringLiteral(u"java.lang.Byte");
                else if(nonGenericType==QStringLiteral(u"short"))
                    nonGenericType = QStringLiteral(u"java.lang.Short");
                else if(nonGenericType==QStringLiteral(u"long"))
                    nonGenericType = QStringLiteral(u"java.lang.Long");
                else if(nonGenericType==QStringLiteral(u"double"))
                    nonGenericType = QStringLiteral(u"java.lang.Double");
                else if(nonGenericType==QStringLiteral(u"float"))
                    nonGenericType = QStringLiteral(u"java.lang.Float");
                else if(nonGenericType==QStringLiteral(u"boolean"))
                    nonGenericType = QStringLiteral(u"java.lang.Boolean");
                else if(nonGenericType==QStringLiteral(u"char"))
                    nonGenericType = QStringLiteral(u"java.lang.Character");
                s << INDENT << "if(other instanceof " << nonGenericType << ")" << Qt::endl
                  << INDENT << "    return hashCode()==other.hashCode();" << Qt::endl
                  << INDENT << "else return super.equals(other);";
            }
            s << INDENT << "}" << Qt::endl << Qt::endl;
        }else if(cls->typeEntry()->isValue()){
            if(!cls->baseClass())
                ReportHandler::warning(QStringLiteral(u"No ==/!= operator found for value type %1.").arg(cls->qualifiedCppName()));
        }
    }

    // Write the comparable functions
    const MetaFunctionList& gt_functions = cls->greaterThanFunctions();
    const MetaFunctionList& geq_functions = cls->greaterThanEqFunctions();
    const MetaFunctionList& lt_functions = cls->lessThanFunctions();
    const MetaFunctionList& leq_functions = cls->lessThanEqFunctions();
    const MetaFunctionList& compare_functions = cls->compareFunctions();

    bool hasEquals = eq_functions.size() || neq_functions.size();
    bool comparable = compare_functions.size() || (hasEquals
                                                        ? gt_functions.size() || geq_functions.size() || lt_functions.size() || leq_functions.size()
                                                        : gt_functions.size() == 1 || lt_functions.size() == 1);
    if (comparable) {
        QList<QPair<const MetaFunction*,QString>> javaTypesByFunction;
        bool suppressRaw = false;
        bool suppressUnchecked = false;
        bool wroteNull = false;
        QString comparableType = findComparableType(cls);
        QString comparableFunctionCode;
        {
            QTextStream s(&comparableFunctionCode);
            s << INDENT << "@QtUninvokable" << Qt::endl
              << INDENT << "public int compareTo(" << comparableType << " other) {" << Qt::endl;
            {
                INDENTATION(INDENT);
                QStringList lines;
                for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                    if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::Compare){
                        lines << codeSnip.code().split("\n");
                    }
                }
                if(!lines.isEmpty())
                    printExtraCode(lines, s);
                bool isWritten = false;
                bool first = true;
                if(compare_functions.size()){
                    isWritten = true;
                    write_compareto_parts(s, compare_functions, 0, first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
                    s << INDENT << "else throw new ClassCastException();" << Qt::endl;
                }else if (hasEquals) {
                    s << INDENT << "if (equals(other)) return 0;" << Qt::endl;
                    first = false;
                    if (lt_functions.size() == 1
                            && gt_functions.size() == 0
                            && leq_functions.size() == 0
                            && geq_functions.size() == 0
                            && !hasImplicitCalls(lt_functions[0])) {
                        javaTypesByFunction.append({lt_functions[0], ""});
                        s << INDENT << "else if (" << lt_functions[0]->name() << "(other)) return -1;" << Qt::endl
                          << INDENT << "else return 1;" << Qt::endl;
                        isWritten = true;
                    }else if (gt_functions.size() == 1
                              && lt_functions.size() == 0
                              && leq_functions.size() == 0
                              && geq_functions.size() == 0
                              && !hasImplicitCalls(gt_functions[0])) {
                        javaTypesByFunction.append({gt_functions[0], ""});
                          s << INDENT << "else if (" << gt_functions[0]->name() << "(other)) return -1;" << Qt::endl
                            << INDENT << "else return 1;" << Qt::endl;
                          isWritten = true;
                    }
                } else if (lt_functions.size() == 1 && !hasImplicitCalls(lt_functions[0])) {
                    javaTypesByFunction.append({lt_functions[0], ""});
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
                    isWritten = true;
                    s << INDENT << "if (" << lt_functions[0]->name() << "((" << className << ") other)) return -1;" << Qt::endl
                      << INDENT << "else if (((" << className << ") other)." << lt_functions[0]->name() << "(this)) return 1;" << Qt::endl
                      << INDENT << "else return 0;" << Qt::endl;

                } else if (gt_functions.size() == 1 && !hasImplicitCalls(gt_functions[0])) {
                    javaTypesByFunction.append({gt_functions[0], ""});
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
                    isWritten = true;
                    s << INDENT << "if (" << gt_functions[0]->name() << "((" << className << ") other)) return 1;" << Qt::endl
                      << INDENT << "else if (((" << className << ") other)." << gt_functions[0]->name() << "(this)) return -1;" << Qt::endl
                      << INDENT << "else return 0;" << Qt::endl;

                } else if (geq_functions.size() == 1 && leq_functions.size() && !hasImplicitCalls(geq_functions[0]) && !hasImplicitCalls(leq_functions[0])) {
                    javaTypesByFunction.append({geq_functions[0], ""});
                    QString className = cls->typeEntry()->qualifiedTargetLangName();
                    if(cls->typeEntry()->isContainer()){
                        if(className==QStringLiteral(u"java.util.List")
                               ||  className==QStringLiteral(u"java.util.LinkedList")
                               ||  className==QStringLiteral(u"java.util.Queue")
                               ||  className==QStringLiteral(u"java.util.Deque")
                               ||  className==QStringLiteral(u"java.util.ArrayList")
                               ||  className==QStringLiteral(u"java.util.Vector")
                               ||  className==QStringLiteral(u"java.util.Set")
                               ||  className==QStringLiteral(u"io.qt.core.QSet")
                               ||  className==QStringLiteral(u"io.qt.core.QList")
                               ||  className==QStringLiteral(u"io.qt.core.QQueue")
                               ||  className==QStringLiteral(u"io.qt.core.QVector")
                               ||  className==QStringLiteral(u"io.qt.core.QStack")
                               ||  className==QStringLiteral(u"io.qt.core.QLinkedList")) {
                            className = QStringLiteral(u"java.util.Collection");
                        }else if(className==QStringLiteral(u"java.util.Map")
                               ||  className==QStringLiteral(u"java.util.SortedMap")
                               ||  className==QStringLiteral(u"java.util.NavigableMap")
                               ||  className==QStringLiteral(u"java.util.HashMap")
                               ||  className==QStringLiteral(u"java.util.TreeMap")
                               ||  className==QStringLiteral(u"io.qt.core.QMap")
                               ||  className==QStringLiteral(u"io.qt.core.QHash")
                               ||  className==QStringLiteral(u"io.qt.core.QMultiMap")
                               ||  className==QStringLiteral(u"io.qt.core.QMultiHash")){
                            className = QStringLiteral(u"java.util.Map");
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
                    isWritten = true;
                    s << INDENT << "boolean less = operator_less_or_equal((" << className << ") other);" << Qt::endl
                      << INDENT << "boolean greater = operator_greater_or_equal((" << className << ") other);" << Qt::endl
                      << INDENT << "if (less && greater) return 0;" << Qt::endl
                      << INDENT << "else if (less) return -1;" << Qt::endl
                      << INDENT << "else return 1;" << Qt::endl;
                }
                if(!isWritten){
                    if (lt_functions.size()) {
                        write_compareto_parts(s, lt_functions, -1, first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
                    } else if (gt_functions.size()) {
                        write_compareto_parts(s, gt_functions, 1, first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
                    } else if (leq_functions.size()) {
                        write_compareto_parts(s, leq_functions, -1, first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
                    } else if (geq_functions.size()) {
                        write_compareto_parts(s, geq_functions, 1, first, suppressUnchecked, suppressRaw, wroteNull, javaTypesByFunction);
                    }
                    s << INDENT << "else throw new ClassCastException();" << Qt::endl;
                }
            }

            s << INDENT << "}" << Qt::endl;
        }
        s << INDENT << "/**" << Qt::endl;
        if(javaTypesByFunction.size()==1){
            auto f = javaTypesByFunction[0].first;
            s << INDENT << " * <p>See <code>";
            if(!f->href().isEmpty()){
                s << "<a href=\"" << docsUrl << f->href() << "\">";
            }
            if(f->functionType()!=MetaFunction::GlobalScopeFunction){
                s << encodeHtml(cls->qualifiedCppName()
                         .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                         .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                         .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                  << "::<wbr/>";
            }
            s << encodeHtml(f->originalSignature()).replace(",", ",<wbr/>");
            if(!f->href().isEmpty())
                s << "</a>";
            s << "</code></p>" << Qt::endl;
        }else{
            s << INDENT << " * <p>Compares to other object depending on its type.</p><ul>" << Qt::endl;
            for(const QPair<const MetaFunction*,QString>& pair : qAsConst(javaTypesByFunction)){
                auto f = pair.first;
                if(f){
                    s << INDENT << " * <li>"+pair.second+" compared by <code>";
                    if(!f->href().isEmpty()){
                        s << "<a href=\"" << docsUrl << f->href() << "\">";
                    }
                    if(f->functionType()!=MetaFunction::GlobalScopeFunction){
                        s << encodeHtml(cls->qualifiedCppName()
                                 .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                 .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                 .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                          << "::<wbr/>";
                    }
                    s << encodeHtml(f->originalSignature()).replace(",", ",<wbr/>");
                    if(!f->href().isEmpty())
                        s << "</a>";
                    s << "</code></li>" << Qt::endl;
                }
            }
            s << INDENT << " * </ul>" << Qt::endl;
        }
        s << INDENT << " */" << Qt::endl;
        if(suppressRaw && suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"rawtypes\", \"unchecked\"})" << Qt::endl;
        else if(suppressRaw)
            s << INDENT << "@SuppressWarnings({\"rawtypes\"})" << Qt::endl;
        else if(suppressUnchecked)
            s << INDENT << "@SuppressWarnings({\"unchecked\"})" << Qt::endl;
        s << comparableFunctionCode << Qt::endl;
    }

    if (!cls->isNamespace() && (cls->hasHashFunction() || eq_functions.size()+neq_functions.size() > 0 || cls->typeEntry()->isQMetaObjectType())) {
        const MetaFunctionList hashcode_functions = cls->queryFunctionsByName("hashCode");
        bool found = false;
        for(const MetaFunction* function : hashcode_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            QStringList lines;
            for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
                if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::HashCode){
                    lines << codeSnip.code().split("\n");
                }
            }
            if(lines.isEmpty() && cls->hasHashFunction() && !cls->typeEntry()->isNativeIdBased() && !cls->typeEntry()->designatedInterface()){
                s << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the objects's hash code computed by <code>qHash(" << cls->qualifiedCppName() << ")</code>." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public native int hashCode();" << Qt::endl;
            }else if(cls->hasHashFunction()
                     || cls->typeEntry()->isValue()
                     || (cls->typeEntry()->hasPublicDefaultConstructor()
                         && cls->typeEntry()->hasPublicCopyConstructor()
                         && cls->typeEntry()->isDestructorPublic()
                         && !cls->typeEntry()->skipMetaTypeRegistration())){
                s << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the objects's hash code computed by <code>qHash(" << cls->qualifiedCppName() << ")</code>." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public int hashCode() {" << Qt::endl;
                if(!lines.isEmpty()){
                    INDENTATION(INDENT);
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
                        s << INDENT << "@QtUninvokable" << Qt::endl
                          << INDENT << "private native static int hashCode_native(long __this_nativeId);" << Qt::endl;
                    }else if(cls->typeEntry()->designatedInterface()){
                        s << INDENT << "@QtUninvokable" << Qt::endl
                          << INDENT << "private native static int hashCode_native(" << cls->simpleName() << " __this);" << Qt::endl;
                    }else{
                        s << INDENT << "@QtUninvokable" << Qt::endl
                          << INDENT << "private native int hashCode_native();" << Qt::endl;
                    }
                } else { // We have equals() but no qHash(), we return 0 from hashCode() to respect
                    // contract of java.lang.Object
                    if(lines.isEmpty() || !lines.last().contains("return ")){
                        s << INDENT << "    return 0;" << Qt::endl;
                    }
                    s << INDENT << "}" << Qt::endl;
                }
            }else if(cls->typeEntry()->isQMetaObjectType()){
                s << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the objects's hash code." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public int hashCode() {" << Qt::endl;
                if(!lines.isEmpty()){
                    INDENTATION(INDENT);
                    printExtraCode(lines, s);
                }
                if(lines.isEmpty() || !lines.last().contains("return ")){
                    s << INDENT << "    return Long.hashCode(__qt_persistentPointer);" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }
        }
    }

    // Qt has a standard toString() conversion in QVariant?
    QString nonNull = m_nullness ? QStringLiteral(u"@NonNull ") : QString{};
    QStringList lines;
    for(const CodeSnip& codeSnip : cls->typeEntry()->codeSnips()){
        if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::ToString){
            lines << codeSnip.code().split("\n");
        }
    }

    QMetaType metaType;
    if (!cls->toStringCapability()
            && !cls->typeEntry()->getNoInstance()
            && (metaType = QMetaType::fromName(qPrintable(cls->qualifiedCppName()))).isValid()
            && metaType.id()!=QMetaType::QStringList
            && QMetaType::canConvert(metaType, QMetaType(QMetaType::QString))) {
        const MetaFunctionList tostring_functions = cls->queryFunctionsByName(QStringLiteral(u"toString"));
        bool found = false;
        for(const MetaFunction* function : tostring_functions) {
            if (function->actualMinimumArgumentCount() == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            if(lines.isEmpty() && !cls->typeEntry()->isNativeIdBased() && !cls->typeEntry()->designatedInterface()){
                s << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the string representation of the object given by <code>QVariant(this).toString()</code>." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public native "  << nonNull << "String toString();" << Qt::endl;
            }else{
                s << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the string representation of the object given by <code>QVariant(this).toString()</code>." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@QtUninvokable" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public "  << nonNull << "String toString() {" << Qt::endl;
                if(!lines.isEmpty()){
                    INDENTATION(INDENT);
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
                    s << INDENT << "@QtUninvokable" << Qt::endl
                      << INDENT << "private native static String toString_native(long __this_nativeId);" << Qt::endl;
                }else if(cls->typeEntry()->designatedInterface()){
                    s << INDENT << "@QtUninvokable" << Qt::endl
                      << INDENT << "private native static String toString_native(" << cls->simpleName() << " __this);" << Qt::endl;
                }else{
                    s << INDENT << "@QtUninvokable" << Qt::endl
                      << INDENT << "private native String toString_native();" << Qt::endl;
                }
            }
        }else if(!lines.isEmpty()){
            s << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * {@inheritDoc}" << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "@QtUninvokable" << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "public "  << nonNull << "String toString() {" << Qt::endl;
            {
                INDENTATION(INDENT);
                printExtraCode(lines, s);
                if(lines.isEmpty() || !lines.last().contains("return ")){
                    s << INDENT << "return super.toString();" << Qt::endl;
                }
            }
            s << INDENT << "}" << Qt::endl;
        }
    }else if(cls->typeEntry()->isQMetaObjectType() || !lines.isEmpty()){
        s << Qt::endl
          << INDENT << "/**" << Qt::endl
          << INDENT << " * {@inheritDoc}" << Qt::endl
          << INDENT << " */" << Qt::endl
          << INDENT << "@QtUninvokable" << Qt::endl
          << INDENT << "@Override" << Qt::endl
          << INDENT << "public "  << nonNull << "String toString() {" << Qt::endl;
        {
            INDENTATION(INDENT);
            printExtraCode(lines, s);
            if(lines.isEmpty() || !lines.last().contains("return ")){
                if(lines.isEmpty()){
                    s << INDENT << "if(__qt_persistentPointer!=0)" << Qt::endl
                      << INDENT << "    return \"QMetaObject(\" + className() + \")\";" << Qt::endl;
                }
                s << INDENT << "return super.toString();" << Qt::endl;
            }
        }
        s << INDENT << "}" << Qt::endl;
    }
}

QString toMetaTypeExpression(QString elementType){
    elementType = elementType.replace("? extends ", "");
    int idx = elementType.indexOf(u'<');
    if(idx>0){
        QString containerType = elementType.mid(0, idx);
        elementType = elementType.mid(idx+1).chopped(1);
        if(elementType.contains(u',')){
            const QStringList elements = elementType.split(u',');
            elementType.clear();
            for(const QString& e : elements){
                elementType = QStringLiteral(u"%1, %2").arg(elementType, toMetaTypeExpression(e));
            }
        }else{
            elementType = toMetaTypeExpression(elementType);
        }
        return QStringLiteral(u"io.qt.core.QMetaType.fromType(%1.class%2)").arg(containerType, elementType);
    }
    return QStringLiteral(u"io.qt.core.QMetaType.fromType(%1.class)").arg(elementType);
}

void JavaGenerator::writeInstantiatedType(QTextStream &s, const MetaType *abstractMetaType, bool forceBoxed) const {
    Q_ASSERT(abstractMetaType);

    const TypeEntry *type = abstractMetaType->typeEntry();
    /* avoid output like java.util.List<int>*/
    if(forceBoxed && type->isPrimitive()){
        if(type->qualifiedTargetLangName()==QStringLiteral(u"int")){
            s << "java.lang.Integer";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"boolean")){
            s << "java.lang.Boolean";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"short")){
            s << "java.lang.Short";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"char")){
            s << "java.lang.Character";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"byte")){
            s << "java.lang.Byte";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"long")){
            s << "java.lang.Long";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"double")){
            s << "java.lang.Double";
            return;
        }else if(type->qualifiedTargetLangName()==QStringLiteral(u"float")){
            s << "java.lang.Float";
            return;
        }
    }

    if(type->designatedInterface()){
        s << type->designatedInterface()->qualifiedTargetLangName().replace(u'$', u'.');
    }else if(abstractMetaType->isCharString()){
        s << "java.lang.String";
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
        case ContainerTypeEntry::std_array:
            if (abstractMetaType->hasInstantiations()) {
                const QList<const MetaType *>& instantiations = abstractMetaType->instantiations();
                QString arrayType = instantiations[0]->typeEntry()->targetLangName();
                s << arrayType << "[]";
            }
            break;
        default:
            s << type->qualifiedTargetLangName().replace(u'$', u'.'); break;
        }
    }else{
        s << type->qualifiedTargetLangName().replace(u'$', u'.');
    }

    if (abstractMetaType->hasInstantiations()) {
        s << "<";
        const QList<const MetaType *>& instantiations = abstractMetaType->instantiations();
        for (int i = 0; i < instantiations.size(); ++i) {
            if (i > 0)
                s << ", ";

            writeInstantiatedType(s, instantiations.at(i), true);
        }
        s << ">";
    }
}

QString upCaseFirst(const QString &str);

void JavaGenerator::writeFunctionOverloads(QTextStream &s, const MetaFunction *java_function,
        uint include_attributes, uint exclude_attributes, Option _option, const QString& alternativeFunctionName) {
    if (java_function->isModifiedRemoved(TS::TargetLangCode))
        return ;
    if (java_function->hasUnresolvedTemplateTypes())
        return;
    if (java_function->isCopyConstructor())
        return;

    // We only create the overloads for the class that actually declares the function
    // unless this is an interface, in which case we create the overloads for all
    // classes that directly implement the interface.
    const MetaClass *decl_class = java_function->declaringClass();

    if (java_function->implementingClass() != decl_class
         && java_function->implementingClass()->baseClass()
         && (decl_class->isInterface()
            ? java_function->implementingClass()->baseClass()->inheritsFromInterface(decl_class)
            : java_function->implementingClass()->baseClass()->inheritsFrom(decl_class)))
        return;

    if(!(_option & NoNullness) && !m_nullness){
        _option = Option(_option | NoNullness);
    }
    exclude_attributes |= MetaAttributes::Override;
    setupForFunction(java_function, &include_attributes, &exclude_attributes, _option);

    const MetaArgumentList& arguments = java_function->arguments();

    bool hasCalls = false;
    QList<QMap<int,Replacement>>  implicitCalls;
    implicitCalls << QMap<int,Replacement>();
    QMap<int,QList<Replacement>> implicitCallArgs;
    bool functionIsNoImplicitArguments = java_function->isNoImplicitArguments();
    for (const MetaArgument *arg : arguments){
        if(java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No
            && arg->argumentIndex()>=0){
            const QStringList inhibitedImplicitCalls = java_function->inhibitedImplicitCalls(arg->argumentIndex()+1);
            if((arg->type()->isConstant() || arg->type()->getReferenceType()!=MetaType::Reference)
                && java_function->typeReplaced(arg->argumentIndex() + 1).isEmpty()
                && arg->type()->indirections().isEmpty()){
                if(!java_function->isNoImplicitCalls(arg->argumentIndex()+1) && !functionIsNoImplicitArguments && arg->type()->typeEntry()->isComplex()){
                    const ComplexTypeEntry* ctype = reinterpret_cast<const ComplexTypeEntry*>(arg->type()->typeEntry());
                    const QList<MetaFunction*>& declImplicitCasts = ctype->declImplicitCasts<MetaFunction>();
                    for(MetaFunction* castFunction : declImplicitCasts){
                        MetaArgument* castArgument = castFunction->arguments()[0];
                        const FunctionModificationList mods = castFunction->modifications(castFunction->implementingClass());
                        bool isPublic = castFunction->isPublic();
                        if(isPublic){
                            for(const FunctionModification& mod : mods) {
                                if(mod.isAccessModifier()){
                                    if (!mod.isPublic()){
                                        isPublic = false;
                                        break;
                                    }
                                }
                            }
                        }
                        if(isPublic
                            && (inhibitedImplicitCalls.isEmpty()
                                || ( !inhibitedImplicitCalls.contains(castArgument->type()->typeEntry()->qualifiedCppName())
                                    && !inhibitedImplicitCalls.contains(castArgument->type()->typeEntry()->qualifiedTargetLangName())))){
                            QString impliciteCall;
                            Option options = _option;
                            if(!(options & NoNullness) && castArgument->isNullPointerDisabled(castFunction)){
                                options = Option(options | StrictNonNull);
                            }
                            if(isCharSequenceSubstitute(castArgument->type())){
                                if(options & NoNullness){
                                    impliciteCall = QStringLiteral(u"java.lang.CharSequence");
                                }else{
                                    if(options & StrictNonNull){
                                        impliciteCall = QStringLiteral(u"java.lang.@StrictNonNull CharSequence");
                                    }else if(castArgument->type()->indirections().isEmpty()){
                                        impliciteCall = QStringLiteral(u"java.lang.@NonNull CharSequence");
                                    }else{
                                        impliciteCall = QStringLiteral(u"java.lang.@Nullable CharSequence");
                                    }
                                }
                            }else{
                                impliciteCall = qualifiedJavaType(castArgument->type(), java_function->implementingClass(), Option(options));
                            }
                            implicitCallArgs[arg->argumentIndex()].append(impliciteCall);
                        }
                    }
                    for(const QString& cast : ctype->implicitCasts()){
                        if(inhibitedImplicitCalls.isEmpty()
                            || !inhibitedImplicitCalls.contains(cast)){
                            implicitCallArgs[arg->argumentIndex()].append(cast);
                        }
                    }
                }
            }
            for(const QString& type : java_function->implicitCalls(arg->argumentIndex()+1))
                implicitCallArgs[arg->argumentIndex()].append(type);
            if(arg->type()->isQSpan() && !java_function->isNoImplicitCalls(arg->argumentIndex()+1) && !functionIsNoImplicitArguments){
                bool spanPointer = !arg->type()->indirections().isEmpty();
                Q_ASSERT(arg->type()->instantiations().size()>0);
                const MetaType* instantiation = arg->type()->instantiations()[0];
                QString element_type = qualifiedJavaType(instantiation, java_function->implementingClass(), Option((_option & ~EnumAsInts & ~UseNativeIds & ~BoxedPrimitive) | NoSuppressExports | VarArgsAsArray));
                QString nonnull;
                if(!(_option & NoNullness)){
                    if(spanPointer){
                        nonnull = QStringLiteral(u"@Nullable ");
                    }else if(instantiation->isConstant()){
                        nonnull = QStringLiteral(u"@NonNull ");
                    }else{
                        nonnull = QStringLiteral(u"@StrictNonNull ");
                    }
                }

                QString comment;
                if(instantiation->isConstant()){
                    comment = QStringLiteral(u"allows read-only buffer as span.");
                }else{
                    comment = QStringLiteral(u"expects writable buffer as span.");
                }
                QStringList types;
                if(element_type==QStringLiteral(u"int")){
                    types << QStringLiteral(u"java.nio.%1IntBuffer");
                }else if(element_type==QStringLiteral(u"byte")){
                    types << QStringLiteral(u"java.nio.%1ByteBuffer");
                    types << QStringLiteral(u"io.qt.core.%1QByteArray");
                    if(instantiation->isConstant()){
                        types << QStringLiteral(u"io.qt.core.%1QByteArrayView");
                    }
                }else if(element_type==QStringLiteral(u"char")){
                    types << QStringLiteral(u"java.nio.%1CharBuffer");
                    types << QStringLiteral(u"io.qt.core.%1QString");
                    if(instantiation->isConstant()){
                        types << QStringLiteral(u"java.lang.%1String");
                        types << QStringLiteral(u"java.lang.%1CharSequence");
                    }
                }else if(element_type==QStringLiteral(u"short")){
                    types << QStringLiteral(u"java.nio.%1ShortBuffer");
                }else if(element_type==QStringLiteral(u"long")){
                    types << QStringLiteral(u"java.nio.%1LongBuffer");
                }else if(element_type==QStringLiteral(u"float")){
                    types << QStringLiteral(u"java.nio.%1FloatBuffer");
                }else if(element_type==QStringLiteral(u"double")){
                    types << QStringLiteral(u"java.nio.%1DoubleBuffer");
                }
                QString el = qualifiedJavaType(instantiation, java_function->implementingClass(),
                                            Option((_option & ~EnumAsInts & ~UseNativeIds)
                                                | BoxedPrimitive | NoSuppressExports
                                                | VarArgsAsArray));
                if(instantiation->isConstant()){
                    types << QStringLiteral(u"java.util.%1Collection<%2>").arg(QStringLiteral(u"%1"), el);
                }
                types << QStringLiteral(u"io.qt.core.%1QList<%2>").arg(QStringLiteral(u"%1"), el);
                for(const QString &type : qAsConst(types)){
                    if(!inhibitedImplicitCalls.contains(type.arg(QString()))
                        && !inhibitedImplicitCalls.contains(type.arg(nonnull)))
                        implicitCallArgs[arg->argumentIndex()].append(Replacement(type.arg(nonnull), comment));
                }
                if(!(_option & NoNullness)){
                    if(spanPointer){
                        nonnull = QStringLiteral(u"@Nullable ");
                    }else if(instantiation->isConstant()){
                        nonnull = QStringLiteral(u" @NonNull");
                    }else{
                        nonnull = QStringLiteral(u" @StrictNonNull");
                    }
                }
                QString annotationFreeElementType = QString(element_type).replace(" @Nullable", "").replace(" @NonNull", "").replace(" @StrictNonNull", "")
                        .replace("@Nullable ", "").replace("@NonNull ", "").replace("@StrictNonNull ", "")
                        .replace("@QtPrimitiveType ", "").replace(" @QtPrimitiveType", "").replace("@QtPrimitiveType", "");
                if(!inhibitedImplicitCalls.contains(QStringLiteral(u"%1[]").arg(annotationFreeElementType))
                    && !inhibitedImplicitCalls.contains(QStringLiteral(u"%1%2[]").arg(element_type, nonnull))
                    && (!(instantiation->isConstant() && arg->argumentIndex()==arguments.size()-1)
                            || (!inhibitedImplicitCalls.contains(QStringLiteral(u"%1...").arg(annotationFreeElementType))
                                && !inhibitedImplicitCalls.contains(QStringLiteral(u"%1%2...").arg(element_type, nonnull))))){
                    QString array_type;
                    if(instantiation->isConstant() && arg->argumentIndex()==arguments.size()-1){
                        array_type = QStringLiteral(u"...");
                    }else{
                        array_type = QStringLiteral(u"[]");
                    }
                    implicitCallArgs[arg->argumentIndex()].append(QStringLiteral(u"%1%2%3").arg(element_type, nonnull, array_type));
                }
            }
            if(arg->argumentIndex()==arguments.size()-1 && arg->type()->isFlags()){
                const EnumTypeEntry *originator = static_cast<const FlagsTypeEntry *>(arg->type()->typeEntry())->originator();
                QString replacement;
                {
                    QTextStream s(&replacement);
                    if(_option & NoNullness){
                        s << originator->javaPackage().replace("$",".") << ".";
                        if(!originator->javaQualifier().isEmpty())
                            s << originator->javaQualifier().replace("$",".") << ".";
                        s << originator->targetLangName().replace("$",".") << "...";
                    }else{
                        QString name = originator->javaPackage().replace("$",".") + ".";
                        if(!originator->javaQualifier().isEmpty())
                            name += originator->javaQualifier().replace("$",".") + ".";
                        name += originator->targetLangName().replace("$",".");
                        auto idx = name.indexOf(u'<');
                        QString package;
                        QString typeName = name;
                        if(idx>0){
                            typeName = name.mid(0, idx);
                        }
                        idx = typeName.lastIndexOf(u'.');
                        if(idx>0){
                            typeName = name.mid(idx+1);
                            package = name.mid(0, idx+1);
                            s << package << "@NonNull " << typeName << " @NonNull...";
                        }else{
                            s << "@NonNull " << name << " @NonNull...";
                        }
                    }
                }
                implicitCallArgs[arg->argumentIndex()].append(replacement);
            }
            bool useArgumentAsSlot = java_function->useArgumentAsSlot(arg->argumentIndex() + 1);
            bool useArgumentAsBuffer = java_function->useArgumentAsBuffer(arg->argumentIndex() + 1);
            bool useArgumentAsArray = java_function->useArgumentAsArray(arg->argumentIndex() + 1);
            if(useArgumentAsSlot){
                int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
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
                        implicitCallArgs[lengthParam->argumentIndex()].append(Replacement::ContextFromLambda);
                    }
                }
            }
            if(!useArgumentAsBuffer
                 && useArgumentAsArray
                 && java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
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
                        implicitCallArgs[lengthParam->argumentIndex()].append(Replacement::RemoveArgument);
                    }
                }
            }
            if(useArgumentAsBuffer && useArgumentAsArray){
                if(java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                    implicitCallArgs[arg->argumentIndex()].append(Replacement::ArrayWithOffsetAsBuffer);
                }
                implicitCallArgs[arg->argumentIndex()].append(Replacement::ArrayAsBuffer);
            }
        }
    }

    for (const MetaArgument *arg : arguments){
        if(implicitCallArgs.contains(arg->argumentIndex())){
            const QList<QMap<int,Replacement>>  implicitCallsCopy = implicitCalls;
            for(const Replacement& jarg : qAsConst(implicitCallArgs[arg->argumentIndex()])){
                if(jarg){
                    QList<QMap<int,Replacement>>  _implicitCalls = implicitCallsCopy;
                    _implicitCalls.detach();
                    for(QMap<int,Replacement>& call : _implicitCalls){
                        call[arg->argumentIndex()] = jarg;
                        hasCalls = true;
                    }
                    implicitCalls << _implicitCalls;
                }
            }
        }
    }

    if(implicitCalls.size()>150){
        implicitCalls.clear();
        implicitCalls << QMap<int,Replacement>();
        {
            QString originalJavaSignature;
            {
                QTextStream s2(&originalJavaSignature);
                s2 << java_function->name();
                s2 << "(";
                writeFunctionArguments(s2, java_function, {}, -1, Option(NoNullness | SkipTemplateParameters | VarArgsAsArray | SkipName));
                s2 << ")";
                s2.flush();
            }
            ReportHandler::warning(QString("Skipping implicit calls for Java method %1 in %2. Too many arguments.").arg(originalJavaSignature, java_function->ownerClass()->qualifiedCppName()));
        }
    }

    // Figure out how many functions we need to write out,
    // One extra for each default argument.
    QList<int> _argumentCounts;
    uint excluded_attributes = MetaAttributes::Abstract
                               | MetaAttributes::Native
                               | exclude_attributes;
    uint included_attributes = (java_function->isConstructor() ? 0 : MetaAttributes::Final) | include_attributes;

    for (int i = int(arguments.size()-1); i >= 0; --i) {
        const MetaArgument *arg = arguments.at(i);
        if(java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No){
            if (arg->defaultValueExpression().isEmpty()
                //&& !java_function->argumentTypeArrayVarArgs(arg->argumentIndex()+1)
                //&& !arg->type()->typeEntry()->isInitializerList()
                ){
                break;
            }
            _argumentCounts << i;
        }
    }

    QList<Delegate> delegates = java_function->delegates();
    if(delegates.isEmpty() && !m_noKotlinDelegates){
        if(java_function->attributes() & MetaAttributes::GetterFunction){
            if(const MetaField * accessedField = java_function->accessedField()){
                QString name = accessedField->name();
                if(name.startsWith("m_")){
                    name = name.mid(2);
                }
                const TS::FieldModificationList modifications = accessedField->modifications();
                bool newNoKotlinGetter = false;
                for(const FieldModification& modification : modifications) {
                    if(!modification.renamedTo().isEmpty()){
                        name = modification.renamedTo();
                    }
                    if(modification.isNoKotlinGetter())
                        newNoKotlinGetter = true;
                }
                if(!newNoKotlinGetter){
//                QString getName = ((accessedField->type()
//                                   && accessedField->type()->isPrimitive()
//                                   && accessedField->type()->typeEntry()->qualifiedTargetLangName()==QStringLiteral(u"boolean")) ? "is" : "get") + upCaseFirst(name);
                    QString getName = "get" + upCaseFirst(name);
                    if(java_function->name()!=getName){
                        delegates << Delegate{getName, false, false, 0, {}};
                    }
                }
            }
        }else if(java_function->isPropertyReader() && java_function->propertySpec()){
            bool noKotlinGetter = false;
            const TS::FunctionModificationList modifications = java_function->modifications(java_function->declaringClass());
            for(const FunctionModification& modification : modifications) {
                if(modification.isNoKotlinGetter())
                    noKotlinGetter = true;
            }
            if(!noKotlinGetter
                    && java_function->type()
                    && (java_function->type()->isPrimitive() || java_function->type()->isPrimitiveChar())
                    && java_function->type()->typeEntry()->targetLangName()==QStringLiteral(u"boolean")
                    && java_function->propertySpec()->name().startsWith("is")
                    && java_function->propertySpec()->name()==java_function->name()){
                noKotlinGetter = true;
            }
            if(!noKotlinGetter){
                QString name;
                if(java_function->propertySpec()->read()!=java_function->name()) {
                    name = java_function->name();
                }else{
                    name = java_function->propertySpec()->name();
                }
                if(name!="get"+upCaseFirst(java_function->propertySpec()->name())){
                    QString getName = "get" + upCaseFirst(name);
                    if(java_function->name()!=getName){
                        delegates << Delegate{getName, false, false, 0, {}};
                    }
                }
            }
        }
    }

    // this is for the original but argument-reduced method:
    delegates << Delegate{};
    bool hasBeenCalled = false;
    for(const Delegate& delegate : qAsConst(delegates)){
        QList<int> argumentCounts = _argumentCounts;
        if(!delegate.name.isEmpty()){
            argumentCounts << -1;
        }
        for(const QMap<int,Replacement>& replacedArguments : qAsConst(implicitCalls)){
            if(delegate.name.isEmpty()){
                bool isEmpty = true;
                for(Replacement a : replacedArguments){
                    if(a){
                        isEmpty = false;
                        break;
                    }
                }
                if(!isEmpty)
                    argumentCounts << -1;
            }
            for (int i = 0; i < argumentCounts.size(); ++i) {

                int used_arguments = argumentCounts[i];
                {
                    QString javaSignature;
                    {
                        QTextStream s2(&javaSignature);
                        if(!alternativeFunctionName.isEmpty()){
                            s2 << alternativeFunctionName;
                        }else if(delegate.name.isEmpty()){
                            s2 << java_function->name();
                        }else{
                            s2 << delegate.name;
                        }
                        s2 << "(";
                        writeFunctionArguments(s2, java_function, replacedArguments, used_arguments, Option((java_function->isFinal() ? NoOption : VirtualCall) | NoNullness | SkipTemplateParameters | VarArgsAsArray | SkipName));
                        s2 << ")";
                        s2.flush();
                    }
                    if(m_currentMethodSignatures.contains(javaSignature)){
                        const MetaFunction * other;
                        if((other = m_currentMethodSignatures[javaSignature]) && java_function!=other){
                            ReportHandler::warning(QString("Ambiguous overloaded Java method %1 in %2 could be %3 or %4").arg(javaSignature, java_function->ownerClass()->qualifiedCppName(), java_function->originalSignature().isEmpty() ? java_function->minimalSignature() : java_function->originalSignature(), other->originalSignature().isEmpty() ? other->minimalSignature() : other->originalSignature()));
                        }
                        continue;
                    }
                    m_currentMethodSignatures.insert(javaSignature, java_function);
                    hasBeenCalled = true;
                }

                uint option = uint((java_function->isFinal() ? NoOption : VirtualCall) | _option);
                if(!java_function->isEmptyFunction()
                        && !java_function->isNormal()
                        && !java_function->isSignal()){
                    option = option | SkipReturnType;
                }
                if(java_function->ownerClass()->isInterface() && (_option & InFunctionComment)==0){
                    option = option | DefaultFunction;
                }
                QString signature;
                uint _excluded_attributes = excluded_attributes;
                uint _included_attributes = included_attributes;
                if(delegate.isSelfReturning){
                    signature = functionSignature(java_function, _included_attributes,
                                                  _excluded_attributes,
                                                  Option(SkipReturnType | option),
                                                  replacedArguments, used_arguments,
                                                  java_function->ownerClass()->name()+" "+delegate.name);
                }else{
                    signature = functionSignature(java_function, _included_attributes,
                                                  _excluded_attributes,
                                                  Option(option),
                                                  replacedArguments, used_arguments,
                                                  delegate.name.isEmpty() ? alternativeFunctionName : delegate.name);
                }
                QString comment;
                QTextStream commentStream(&comment);
                if(!delegate.isDeprecated || used_arguments!=-1){
                    if(java_function->isConstructor()){
                        commentStream << "<p>Overloaded constructor for {@link #";
                        if(alternativeFunctionName.isEmpty()){
                            commentStream << java_function->name();
                        }else{
                            commentStream << alternativeFunctionName;
                        }
                    }else{
                        if(delegate.name.isEmpty()){
                            commentStream << "<p>Overloaded function for {@link #";
                            if(alternativeFunctionName.isEmpty()){
                                commentStream << java_function->name();
                            }else{
                                commentStream << alternativeFunctionName;
                            }
                        }else if(used_arguments==-1){
                            if(java_function->attributes() & MetaAttributes::GetterFunction
                                    || java_function->isPropertyReader()){
                                commentStream << "@hidden" << Qt::endl
                                              << "<p>Kotlin property getter. In Java use {@link #";
                            }else{
                                commentStream << "<p>Delegate for {@link #";
                            }
                            commentStream << java_function->name();
                        }else{
                            commentStream << "<p>Overloaded function for {@link #";
                            commentStream << delegate.name;
                        }
                    }
                    commentStream << "(";
                    writeFunctionArguments(commentStream, java_function, used_arguments==-1 ? QMap<int,Replacement>{} : replacedArguments, int(arguments.size()), Option((java_function->isFinal() ? NoOption : VirtualCall) | NoNullness | SkipName | NoSuppressExports | SkipTemplateParameters | VarArgsAsArray));
                    commentStream << ")}";
                    if(used_arguments>=0){
                        bool useList = arguments.size()-used_arguments>1;
                        if(useList)
                            commentStream << "</p>" << Qt::endl << "<p>with: </p><ul>" << Qt::endl;
                        else
                            commentStream << Qt::endl << " with ";
                        for (int j = used_arguments; j < arguments.size(); ++j) {
                            auto arg = arguments.at(j);
                            if(java_function->argumentRemoved(arg->argumentIndex() + 1)!=ArgumentRemove_No)
                                continue;
                            if(useList)
                                commentStream << "<li>";
                            commentStream << "<code>" << arg->modifiedArgumentName() << " = ";
                            QString defaultExpr = arg->defaultValueExpression();
                            auto pos = defaultExpr.indexOf(".");
                            if (pos > 0) {
                                QString someName = defaultExpr.left(pos);
                                ComplexTypeEntry *ctype =
                                    m_database->findComplexType(someName);
                                QString replacement;
                                if (ctype && ctype->isQVariant())
                                    replacement = "QVariant.";
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
                                defaultExpr = "new " + arg->type()->fullName().replace(u'$', u'.') + "(" + defaultExpr + ")";
                            }
                            commentStream << encodeHtml(defaultExpr)
                                          << "</code>";
                            if(useList)
                                commentStream << "</li>" << Qt::endl;
                        }
                        if(useList)
                            commentStream << "</ul>" << Qt::endl;
                        else
                            commentStream << ".</p>" << Qt::endl;
                    }else{
                        if(java_function->attributes() & MetaAttributes::GetterFunction
                                || java_function->isPropertyReader()){
                            commentStream << " instead";
                        }
                        commentStream << ".</p>" << Qt::endl;
                    }
                }

                if(delegate.isDeprecated){
                    if(!comment.isEmpty())
                        commentStream << Qt::endl;
                    commentStream << "@deprecated";
                    if(!java_function->isDeprecated()){
                        commentStream << " Use {@link #" << java_function->name() << "(";
                        writeFunctionArguments(commentStream, java_function, replacedArguments, int(arguments.size()), Option((option & ~StrictNonNull) | NoNullness | NoSuppressExports | SkipName | SkipTemplateParameters | VarArgsAsArray));
                        commentStream << ")} instead." << Qt::endl;
                    }
                }else if(java_function->isDeprecated() && !java_function->deprecatedComment().isEmpty()){
                    if(!comment.isEmpty())
                        commentStream << Qt::endl;
                    writeDeprecatedComment(commentStream, java_function);
                }

                s << INDENT << Qt::endl;
                if(!comment.trimmed().isEmpty()
                        && (_option & InFunctionComment)==0
                        && !java_function->isPrivateSignal()
                        && !(_included_attributes & MetaAttributes::Private)){
                    s << INDENT << "/**" << Qt::endl;
                    commentStream.seek(0);
                    while(!commentStream.atEnd()){
                        s << INDENT << " * " << commentStream.readLine() << Qt::endl;
                    }
                    s << INDENT << " */" << Qt::endl;
                }
                if(java_function->isPropertyReader() && !signature.contains("@QtPropertyReader(")){
                    s << INDENT << "@QtPropertyReader(enabled=false)" << Qt::endl;
                }
                if(delegate.isDeprecated && !signature.contains("@Deprecated")){
                    s << INDENT << "@Deprecated" << Qt::endl;
                }
                s << signature << " {" << Qt::endl;
                {
                    QString precontent;
                    QString content;
                    QString postcontent;
                    QTextStream pre(&precontent);
                    QTextStream post(&postcontent);
                    QString new_return_type = java_function->typeReplaced(0);
                    INDENTATION(INDENT);
                    {
                        QTextStream s(&content);
                        if (java_function->isConstructor())
                            s << "this";
                        else{
                            if(delegate.name.isEmpty() || used_arguments==-1){
                                if(alternativeFunctionName.isEmpty())
                                    s << java_function->name();
                                else
                                    s << alternativeFunctionName;
                            }else{
                                s << delegate.name;
                            }
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
                            const MetaArgument* arg = arguments.at(j);
                            if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                                Replacement alternativeType = replacedArguments[arg->argumentIndex()];
                                if(used_arguments!=-1
                                    && alternativeType.type==Replacement::Name){
                                    alternativeType = Replacement();
                                }
                                if(alternativeType){
                                    if (written_arguments > 0)
                                        s << ", ";
                                    if(alternativeType.type==Replacement::RemoveArgument){
                                        s << "-1";
                                    }else if(alternativeType.type==Replacement::ContextFromLambda){
                                        for(const MetaArgument* _arg : arguments){
                                            if(java_function->utilArgumentIndex(_arg->argumentIndex()+1)==arg->argumentIndex()+1){
                                                s << "QtJambi_LibraryUtilities.internal.lambdaContext(" << _arg->argumentName() << ")";
                                            }
                                        }
                                    }else{
                                        QString java_type = java_function->typeReplaced(arg->argumentIndex() + 1);
                                        if (java_type.isEmpty()){
                                            if(java_function->useArgumentAsBuffer(arg->argumentIndex()+1)){
                                                java_type = arg->type()->fullName();
                                                if(java_type==QStringLiteral(u"int")){
                                                    java_type = "java.nio.IntBuffer";
                                                }else if(java_type==QStringLiteral(u"byte")){
                                                    java_type = "java.nio.ByteBuffer";
                                                }else if(java_type==QStringLiteral(u"char")){
                                                    java_type = "java.nio.CharBuffer";
                                                }else if(java_type==QStringLiteral(u"short")){
                                                    java_type = "java.nio.ShortBuffer";
                                                }else if(java_type==QStringLiteral(u"long")){
                                                    java_type = "java.nio.LongBuffer";
                                                }else if(java_type==QStringLiteral(u"float")){
                                                    java_type = "java.nio.FloatBuffer";
                                                }else if(java_type==QStringLiteral(u"double")){
                                                    java_type = "java.nio.DoubleBuffer";
                                                }else{
                                                    java_type = "java.nio.Buffer";
                                                }
                                            }else if(java_function->useArgumentAsArray(arg->argumentIndex()+1)){
                                                java_type += "[]";
                                            }else{
                                                uint option = 0;
                                                if(!java_function->isFinal()){
                                                     option = Option(option | NoQCollectionContainers | VirtualCall);
                                                }
                                                java_type = qualifiedJavaType(arg->type(), java_function->implementingClass(), Option(option | VarArgsAsArray | NoNullness));
                                            }
                                        }
                                        java_type = java_type.replace(" @Nullable", "").replace(" @NonNull", "").replace(" @StrictNonNull", "")
                                                             .replace("@Nullable ", "").replace("@NonNull ", "").replace("@StrictNonNull ", "");
                                        //s << arg->modifiedArgumentName() << "==null ? null : ";
                                        QString annotationFreeAlternativeType = QString(alternativeType.typeName).replace(" @Nullable", "").replace(" @NonNull", "").replace(" @StrictNonNull", "")
                                                .replace("@Nullable ", "").replace("@NonNull ", "").replace("@StrictNonNull ", "")
                                                .replace("@QtPrimitiveType ", "").replace(" @QtPrimitiveType", "").replace("@QtPrimitiveType", "").replace("...", "[]");

                                        auto writeIterableToBuffer = [](const MetaFunction *java_function,
                                                                        const MetaArgument* arg,
                                                                        const QString& annotationFreeAlternativeType,
                                                                        QTextStream& pre,
                                                                        QTextStream& s,
                                                                        QTextStream& post, char c){
                                            if(java_function->isConstructor()){
                                                s << "QtJambi_LibraryUtilities.internal.mutableData" << c << "(" << arg->modifiedArgumentName() << ")";
                                            }else{
                                                bool isMutableSpan = arg->type()->isQSpan()
                                                        && !arg->type()->instantiations().isEmpty()
                                                        && !arg->type()->instantiations()[0]->isConstant();
                                                QString javaBoxedType;
                                                QString javaPrimitiveType;
                                                switch(c){
                                                case 'B':
                                                    javaBoxedType = javaPrimitiveType = QStringLiteral(u"Byte");
                                                    break;
                                                case 'S':
                                                    javaBoxedType = javaPrimitiveType = QStringLiteral(u"Short");
                                                    break;
                                                case 'I':
                                                    javaBoxedType = QStringLiteral(u"Integer");
                                                    javaPrimitiveType = QStringLiteral(u"Int");
                                                    break;
                                                case 'C':
                                                    javaBoxedType = QStringLiteral(u"Character");
                                                    javaPrimitiveType = QStringLiteral(u"Char");
                                                    break;
                                                case 'J':
                                                    javaBoxedType = javaPrimitiveType = QStringLiteral(u"Long");
                                                    break;
                                                case 'F':
                                                    javaBoxedType = javaPrimitiveType = QStringLiteral(u"Float");
                                                    break;
                                                case 'D':
                                                    javaBoxedType = javaPrimitiveType = QStringLiteral(u"Double");
                                                    break;
                                                }
                                                QString javaBufferType = QStringLiteral(u"java.nio.%1Buffer").arg(javaPrimitiveType);
                                                javaPrimitiveType = javaPrimitiveType.toLower();

                                                QString varName = arg->modifiedArgumentName();
                                                if(!annotationFreeAlternativeType.startsWith(QStringLiteral(u"io.qt.core."))){
                                                    if(isMutableSpan){
                                                        pre << "Runnable " << arg->modifiedArgumentName() << "_finalizer = null;" << Qt::endl;
                                                        post << "if(" << arg->modifiedArgumentName() << "_finalizer!=null)" << Qt::endl
                                                             << "    " << arg->modifiedArgumentName() << "_finalizer.run();" << Qt::endl;
                                                    }
                                                    pre << javaBufferType << " " << arg->modifiedArgumentName() << "_buffer;";
                                                    QString listName = varName + QStringLiteral(u"_list");
                                                    varName += QStringLiteral(u"_value");
                                                    pre << "io.qt.QtObject " << varName << ";" << Qt::endl;
                                                    if(isMutableSpan){
                                                        pre << "if(" << arg->modifiedArgumentName() << " instanceof io.qt.core.QSpan){" << Qt::endl
                                                            << "    io.qt.core.QSpan<" << javaBoxedType << "> " << arg->modifiedArgumentName() << "_span = (io.qt.core.QSpan<" << javaBoxedType << ">) " << arg->modifiedArgumentName() << ";" << Qt::endl
                                                            << "    " << arg->modifiedArgumentName() << "_buffer = io.qt.core.QSpan.as" << javaPrimitiveType << "Buffer(" << arg->modifiedArgumentName() << "_span);" << Qt::endl
                                                            << "    " << varName << " = " << arg->modifiedArgumentName() << "_span;" << Qt::endl;
                                                    }else{
                                                        pre << "if(" << arg->modifiedArgumentName() << " instanceof io.qt.core.QConstSpan){" << Qt::endl
                                                            << "    io.qt.core.QConstSpan<" << javaBoxedType << "> " << arg->modifiedArgumentName() << "_span = (io.qt.core.QConstSpan<" << javaBoxedType << ">) " << arg->modifiedArgumentName() << ";" << Qt::endl
                                                            << "    " << arg->modifiedArgumentName() << "_buffer = io.qt.core.QConstSpan.as" << javaPrimitiveType << "Buffer(" << arg->modifiedArgumentName() << "_span);" << Qt::endl
                                                            << "    " << varName << " = " << arg->modifiedArgumentName() << "_span;" << Qt::endl;
                                                    }
                                                    QString elementArray = QStringLiteral(u"io.qt.core.QByteArray");
                                                    switch(c){
                                                    case 'C':
                                                        elementArray = QStringLiteral(u"io.qt.core.QString");
                                                        Q_FALLTHROUGH();
                                                    case 'B':
                                                        pre << "}else if(" << arg->modifiedArgumentName() << " instanceof " << elementArray << "){" << Qt::endl
                                                            << "    " << elementArray << " " << arg->modifiedArgumentName() << "_container = (" << elementArray << ") " << arg->modifiedArgumentName() << ";" << Qt::endl;
                                                        if(!isMutableSpan){
                                                            pre << "    " << arg->modifiedArgumentName() << "_buffer = " << arg->modifiedArgumentName() << "_container.data();" << Qt::endl;
                                                        }else{
                                                            pre << "    " << arg->modifiedArgumentName() << "_buffer = QtJambi_LibraryUtilities.internal.mutableData(" << arg->modifiedArgumentName() << "_container);" << Qt::endl;
                                                        }
                                                        pre << "    " << varName << " = " << arg->modifiedArgumentName() << "_container;" << Qt::endl
                                                            << "}else if(" << arg->modifiedArgumentName() << " instanceof io.qt.core.QByteArrayView){" << Qt::endl;
                                                        if(!isMutableSpan){
                                                            pre << "    io.qt.core.QByteArrayView " << arg->modifiedArgumentName() << "_ba = (io.qt.core.QByteArrayView) " << arg->modifiedArgumentName() << ";" << Qt::endl
                                                                << "    " << arg->modifiedArgumentName() << "_buffer = " << arg->modifiedArgumentName() << "_ba.data();" << Qt::endl
                                                                << "    " << varName << " = " << arg->modifiedArgumentName() << "_ba;" << Qt::endl;
                                                        }else{
                                                            pre << "    throw new IllegalArgumentException(\"Cannot cast QByteArrayView to QSpan<" << javaPrimitiveType << ">\");" << Qt::endl;
                                                        }
                                                        break;
                                                    }
                                                    pre << "}else{" << Qt::endl
                                                        << "    io.qt.core.QList<" << javaBoxedType << "> " << listName << ";" << Qt::endl
                                                        << "    if(" << arg->modifiedArgumentName() << " instanceof io.qt.core.QList){" << Qt::endl
                                                        << "        " << listName << " = (io.qt.core.QList<" << javaBoxedType << ">) " << arg->modifiedArgumentName() << ";" << Qt::endl
                                                        << "    }else{" << Qt::endl
                                                        << "        " << listName << " = new io.qt.core.QList<>(" << javaPrimitiveType << ".class);" << Qt::endl;
                                                    if(isMutableSpan){
                                                        pre << "        if(" << arg->modifiedArgumentName() << " instanceof java.util.List){" << Qt::endl
                                                            << "            java.util.List<" << javaBoxedType << "> " << arg->modifiedArgumentName() << "_jlist = (java.util.List<" << javaBoxedType << ">)" << arg->modifiedArgumentName() << ";" << Qt::endl
                                                            << "            " << listName << ".addAll(" << arg->modifiedArgumentName() << "_jlist);" << Qt::endl
                                                            << "            " << arg->modifiedArgumentName() << "_finalizer = ()->{" << Qt::endl
                                                            << "                    for(int " << arg->modifiedArgumentName() << "_i=0, "
                                                            << arg->modifiedArgumentName() << "_l1=" << listName << ".size(), "
                                                            << arg->modifiedArgumentName() << "_l2=" << arg->modifiedArgumentName() << "_jlist.size(); "
                                                            << arg->modifiedArgumentName() << "_i<" << arg->modifiedArgumentName() << "_l1 && " <<
                                                               arg->modifiedArgumentName() << "_i<" << arg->modifiedArgumentName() << "_l2; ++" << arg->modifiedArgumentName() << "_i){" << Qt::endl
                                                            << "                        " << arg->modifiedArgumentName() << "_jlist.set(" << arg->modifiedArgumentName() << "_i, " << listName << ".get(" << arg->modifiedArgumentName() << "_i));" << Qt::endl
                                                            << "                    }" << Qt::endl
                                                            << "                };" << Qt::endl
                                                            << "        }else";
                                                        if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.lang.Iterable<"))){
                                                            pre << " if(" << arg->modifiedArgumentName() << " instanceof io.qt.core.QSequentialIterator){" << Qt::endl
                                                                << "            java.util.Iterator<" << javaBoxedType << "> " << arg->modifiedArgumentName() << "_iter = ((io.qt.core.QSequentialIterator<" << javaBoxedType << ">)" << arg->modifiedArgumentName() << ").iterator();" << Qt::endl
                                                                << "            if(!(" << arg->modifiedArgumentName() << "_iter instanceof java.util.ListIterator))" << Qt::endl
                                                                << "                throw new IllegalArgumentException(\"Cannot cast immutable Iterable to QSpan<" << javaPrimitiveType << ">\");" << Qt::endl
                                                                << "            java.util.ListIterator<" << javaBoxedType << "> " << arg->modifiedArgumentName() << "_mutable_iter = (java.util.ListIterator<" << javaBoxedType << ">)" << arg->modifiedArgumentName() << "_iter;" << Qt::endl
                                                                << "            while(" << arg->modifiedArgumentName() << "_mutable_iter.hasNext()) {" << Qt::endl
                                                                << "                " << arg->modifiedArgumentName() << "_list.add(" << arg->modifiedArgumentName() << "_mutable_iter.next());" << Qt::endl
                                                                << "            }" << Qt::endl
                                                                << "            " << arg->modifiedArgumentName() << "_finalizer = ()->{" << Qt::endl
                                                                << "                    int " << arg->modifiedArgumentName() << "_i=" << listName << ".size();" << Qt::endl
                                                                << "                    while(" << arg->modifiedArgumentName() << "_mutable_iter.hasPrevious() && " << arg->modifiedArgumentName() << "_i>0){" << Qt::endl
                                                                << "                        " << arg->modifiedArgumentName() << "_mutable_iter.set(" << listName << ".get(--" << arg->modifiedArgumentName() << "_i));" << Qt::endl
                                                                << "                    }" << Qt::endl
                                                                << "                };" << Qt::endl
                                                                << "        }else if(" << arg->modifiedArgumentName() << " instanceof java.util.Collection)";
                                                        }
                                                        pre << "{" << Qt::endl
                                                            << "            java.util.Collection<" << javaBoxedType << "> " << arg->modifiedArgumentName() << "_collection = (java.util.Collection<" << javaBoxedType << ">)" << arg->modifiedArgumentName() << ";" << Qt::endl
                                                            << "            " << listName << ".addAll(" << arg->modifiedArgumentName() << "_collection);" << Qt::endl
                                                            << "            " << arg->modifiedArgumentName() << "_finalizer = ()->{" << Qt::endl
                                                            << "                    " << arg->modifiedArgumentName() << "_collection.clear();" << Qt::endl
                                                            << "                    " << arg->modifiedArgumentName() << "_collection.addAll(" << listName << ");" << Qt::endl
                                                            << "                };" << Qt::endl;
                                                        if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.lang.Iterable<"))){
                                                            pre << "        }else{" << Qt::endl
                                                                << "            " << listName << ".dispose();" << Qt::endl
                                                                << "            throw new IllegalArgumentException(\"Cannot cast immutable Iterable to QSpan<" << javaPrimitiveType << ">\");" << Qt::endl;
                                                        }
                                                        pre << "        }" << Qt::endl;
                                                    }else{
                                                        pre << "        for(" << javaBoxedType << " " << arg->modifiedArgumentName() << "_item : " << arg->modifiedArgumentName() << "){" << Qt::endl
                                                            << "            " << listName << ".append(" << arg->modifiedArgumentName() << "_item);" << Qt::endl
                                                            << "        }" << Qt::endl;
                                                    }
                                                    pre << "    }" << Qt::endl
                                                        << "    " << arg->modifiedArgumentName() << "_buffer = QtJambi_LibraryUtilities.internal.mutableData" << c << "(" << listName << ");" << Qt::endl
                                                        << "    " << varName << " = " << listName << ";" << Qt::endl
                                                        << "}" << Qt::endl;
                                                }else{
                                                    pre << javaBufferType << " " << arg->modifiedArgumentName() << "_buffer = QtJambi_LibraryUtilities.internal.mutableData" << c << "(" << varName << ");" << Qt::endl;
                                                }
                                                s << "" << arg->modifiedArgumentName() << "_buffer";
                                                post << "QtJambi_LibraryUtilities.internal.truncateBuffer(" << varName << ", " << arg->modifiedArgumentName() << "_buffer);" << Qt::endl;
                                            }
                                        };

                                        if(java_type==QStringLiteral(u"int")
                                            || java_type==QStringLiteral(u"byte")
                                            || java_type==QStringLiteral(u"short")
                                            || java_type==QStringLiteral(u"long")
                                            || java_type==QStringLiteral(u"double")
                                            || java_type==QStringLiteral(u"float")
                                            || java_type==QStringLiteral(u"boolean")
                                            || java_type==QStringLiteral(u"char")){
                                            s << "(" << java_type << ")" << arg->modifiedArgumentName();
                                        }else if((java_type==QStringLiteral(u"java.nio.ByteBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Byte>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Byte>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'B');
                                        }else if((java_type==QStringLiteral(u"java.nio.ShortBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Short>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Short>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'S');
                                        }else if((java_type==QStringLiteral(u"java.nio.IntBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Integer>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Integer>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'I');
                                        }else if((java_type==QStringLiteral(u"java.nio.LongBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Long>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Long>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'J');
                                        }else if((java_type==QStringLiteral(u"java.nio.FloatBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Float>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Float>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'F');
                                        }else if((java_type==QStringLiteral(u"java.nio.DoubleBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Double>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Double>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'D');
                                        }else if((java_type==QStringLiteral(u"java.nio.CharBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QList<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QQueue<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QStack<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QVector<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QSpan<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QConstSpan<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.Iterable<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.Collection<java.lang.Character>")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.util.List<java.lang.Character>"))){
                                            writeIterableToBuffer(java_function,
                                                                  arg,
                                                                  annotationFreeAlternativeType,
                                                                  pre, s, post, u'C');
                                        }else if(java_type==QStringLiteral(u"java.lang.String")){
                                            s << arg->modifiedArgumentName() << "==null ? null : " << arg->modifiedArgumentName() << ".toString()";
                                        }else if((java_type==QStringLiteral(u"java.nio.CharBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"java.lang.CharSequence")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.String"))){
                                            s << arg->modifiedArgumentName() << "==null ? null : java.nio.CharBuffer.wrap(" << arg->modifiedArgumentName() << ")";
                                        /*}else if(java_type==QStringLiteral(u"java.nio.ByteBuffer")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.String")){
                                            s << arg->modifiedArgumentName() << "==null ? null : java.nio.ByteBuffer.wrap(" << arg->modifiedArgumentName() << ".getBytes(\"UTF-8\"))";
                                        }else if(java_type==QStringLiteral(u"java.nio.ByteBuffer")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"java.lang.CharSequence")){
                                            s << arg->modifiedArgumentName() << "==null ? null : java.nio.ByteBuffer.wrap(" << arg->modifiedArgumentName() << ".toString().getBytes(\"UTF-8\"))";*/
                                        }else if((java_type==QStringLiteral(u"java.nio.ByteBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArray")){
                                            if(java_function->isConstructor()){
                                                if(arg->type()->isConstant()){
                                                    s << arg->modifiedArgumentName() << ".data()";
                                                }else{
                                                    s << "QtJambi_LibraryUtilities.internal.mutableData(" << arg->modifiedArgumentName() << ")";
                                                }
                                            }else{
                                                if(arg->type()->isConstant()){
                                                    pre << "java.nio.ByteBuffer __" << arg->modifiedArgumentName() << "_buffer = " << arg->modifiedArgumentName() << ".data();" << Qt::endl;
                                                }else{
                                                    pre << "java.nio.ByteBuffer __" << arg->modifiedArgumentName() << "_buffer = QtJambi_LibraryUtilities.internal.mutableData(" << arg->modifiedArgumentName() << ");" << Qt::endl;
                                                }
                                                s << "__" << arg->modifiedArgumentName() << "_buffer";
                                                post << "QtJambi_LibraryUtilities.internal.truncateBuffer(" << arg->modifiedArgumentName() << ", __" << arg->modifiedArgumentName() << "_buffer);" << Qt::endl;
                                            }
                                        }else if((java_type==QStringLiteral(u"java.nio.ByteBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArrayView")){
                                            if(java_function->isConstructor()){
                                                s << arg->modifiedArgumentName() << ".data()";
                                            }else{
                                                pre << "java.nio.ByteBuffer __" << arg->modifiedArgumentName() << "_buffer = " << arg->modifiedArgumentName() << ".data();" << Qt::endl;
                                                s << "__" << arg->modifiedArgumentName() << "_buffer";
                                                post << "QtJambi_LibraryUtilities.internal.truncateBuffer(" << arg->modifiedArgumentName() << ", __" << arg->modifiedArgumentName() << "_buffer);" << Qt::endl;
                                            }
                                        }else if(java_type==QStringLiteral(u"io.qt.core.QByteArrayView")
                                                 && annotationFreeAlternativeType==QStringLiteral(u"java.lang.CharSequence")){
                                            s << arg->modifiedArgumentName() << " instanceof io.qt.core.QString ? new io.qt.core.QByteArrayView(((io.qt.core.QString)" << arg->modifiedArgumentName() << ").toUtf8()) : (" << arg->modifiedArgumentName() << "==null ? null : new io.qt.core.QByteArrayView(" << arg->modifiedArgumentName() << ".toString()))";
                                        }else if(java_type==QStringLiteral(u"io.qt.core.QByteArray")
                                                 && annotationFreeAlternativeType==QStringLiteral(u"java.lang.CharSequence")){
                                            s << arg->modifiedArgumentName() << " instanceof io.qt.core.QString ? ((io.qt.core.QString)" << arg->modifiedArgumentName() << ").toUtf8() : (" << arg->modifiedArgumentName() << "==null ? null : new io.qt.core.QByteArray(" << arg->modifiedArgumentName() << ".toString()))";
                                        }else if(java_type==QStringLiteral(u"byte[]")
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArrayView")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArray"))){
                                            s << arg->modifiedArgumentName() << ".toArray()";
                                        }else if((java_type==QStringLiteral(u"java.nio.CharBuffer")
                                                  || java_type==QStringLiteral(u"java.nio.Buffer"))
                                                 && annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QString")){
                                            if(java_function->isConstructor()){
                                                if(arg->type()->isConstant()){
                                                    s << arg->modifiedArgumentName() << ".data()";
                                                }else{
                                                    s << "QtJambi_LibraryUtilities.internal.mutableData(" << arg->modifiedArgumentName() << ")";
                                                }
                                            }else{
                                                if(arg->type()->isConstant()){
                                                    pre << "java.nio.CharBuffer __" << arg->modifiedArgumentName() << "_buffer = " << arg->modifiedArgumentName() << ".data();" << Qt::endl;
                                                }else{
                                                    pre << "java.nio.CharBuffer __" << arg->modifiedArgumentName() << "_buffer = QtJambi_LibraryUtilities.internal.mutableData(" << arg->modifiedArgumentName() << ");" << Qt::endl;
                                                }
                                                s << "__" << arg->modifiedArgumentName() << "_buffer";
                                                post << "QtJambi_LibraryUtilities.internal.truncateBuffer(" << arg->modifiedArgumentName() << ", __" << arg->modifiedArgumentName() << "_buffer);" << Qt::endl;
                                            }
                                        }else if(java_type==QStringLiteral(u"byte[]")
                                                 && (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArrayView")
                                                     || annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArray"))){
                                            s << arg->modifiedArgumentName() << ".toArray()";
                                        }else if(java_type.startsWith(QStringLiteral(u"java.nio.")) && java_type.endsWith(QStringLiteral(u"Buffer"))
                                                   && (alternativeType.type==Replacement::ArrayAsBuffer
                                                       || alternativeType.type==Replacement::ArrayWithOffsetAsBuffer
                                                       || alternativeType.typeName.endsWith(QStringLiteral(u"[]"))
                                                       || alternativeType.typeName.endsWith(QStringLiteral(u"...")))){
                                            if(!alternativeType.typeName.endsWith(QStringLiteral(u"@StrictNonNull[]"))
                                                    && !alternativeType.typeName.endsWith(QStringLiteral(u"@StrictNonNull..."))){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            if(java_type==QStringLiteral(u"java.nio.Buffer")){
                                                if(annotationFreeAlternativeType==QStringLiteral(u"char[]")){
                                                    s << "java.nio.CharBuffer.wrap(" << arg->modifiedArgumentName();
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"short[]")){
                                                    s << "java.nio.ShortBuffer.wrap(" << arg->modifiedArgumentName();
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"long[]")){
                                                    s << "java.nio.LongBuffer.wrap(" << arg->modifiedArgumentName();
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"int[]")){
                                                    s << "java.nio.IntBuffer.wrap(" << arg->modifiedArgumentName();
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"float[]")){
                                                    s << "java.nio.FloatBuffer.wrap(" << arg->modifiedArgumentName();
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"double[]")){
                                                    s << "java.nio.DoubleBuffer.wrap(" << arg->modifiedArgumentName();
                                                }else{
                                                    s << "java.nio.ByteBuffer.wrap(" << arg->modifiedArgumentName();
                                                }
                                            }else{
                                                s << java_type.replace(u'$', u'.') << ".wrap(" << arg->modifiedArgumentName();
                                            }
                                            if(alternativeType.type==Replacement::ArrayWithOffsetAsBuffer){
                                                QString modifiedArgumentName = arg->modifiedArgumentName();
                                                modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                                                s << ", offsetOf" << modifiedArgumentName << ", ";
                                                int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
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
                                                    if(lengthParam){
                                                        s << lengthParam->modifiedArgumentName();
                                                    }else{
                                                        s << arg->modifiedArgumentName() << ".length";
                                                    }
                                                }else{
                                                    s << arg->modifiedArgumentName() << ".length";
                                                }
                                            }
                                            s << ")";
                                        }else if(java_type==QStringLiteral(u"io.qt.core.QRunnable")){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            s << "io.qt.core.QRunnable.of(" << arg->modifiedArgumentName() << ")";
                                        }else if((java_type.startsWith(QStringLiteral(u"java.util.Collection<")) || java_type==QStringLiteral(u"java.util.Collection")
                                                  || java_type.startsWith(QStringLiteral(u"java.lang.Iterable<")) || java_type==QStringLiteral(u"java.lang.Iterable"))
                                                 && (alternativeType.typeName.endsWith(QStringLiteral(u"[]")) || alternativeType.typeName.endsWith(QStringLiteral(u"...")))){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            s << "java.util.Arrays.asList(" << arg->modifiedArgumentName() << ")";
                                        }else if((java_type.startsWith("io.qt.core.QConstSpan<") || java_type=="io.qt.core.QConstSpan")){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            s << "io.qt.core.QConstSpan.of";
                                            if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.nio."))
                                                     && annotationFreeAlternativeType.endsWith(QStringLiteral(u"Buffer"))){
                                                s << "Buffer(";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"java.lang.CharSequence")
                                                         || annotationFreeAlternativeType==QStringLiteral(u"java.lang.String")){
                                                s << "Char(";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArrayView")){
                                                s << "Byte(";
                                            }else{
                                                if(annotationFreeAlternativeType==QStringLiteral(u"byte[]")
                                                        || (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArray"))){
                                                    s << "Byte(";
                                                }else if((annotationFreeAlternativeType.startsWith("io.qt.core.QList<")
                                                          || annotationFreeAlternativeType.startsWith("java.util.Collection<"))){
                                                    s << "List(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"short[]")){
                                                    s << "Short(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"int[]")){
                                                    s << "Int(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"long[]")){
                                                    s << "Long(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"char[]")
                                                         || (annotationFreeAlternativeType.startsWith("io.qt.core.")
                                                             && annotationFreeAlternativeType.endsWith("QString"))){
                                                    s << "Char(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"float[]")){
                                                    s << "Float(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"double[]")){
                                                    s << "Double(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"boolean[]")){
                                                    s << "Boolean(";
                                                }else if(annotationFreeAlternativeType.endsWith(QStringLiteral(u"[]"))){
                                                    QString elementType = annotationFreeAlternativeType.chopped(2);
                                                    if(elementType.contains(u"<")){
                                                        s << "Typed(" << toMetaTypeExpression(elementType) << ", ";
                                                    }else{
                                                        s << "Typed(" << elementType << ".class, ";
                                                    }
                                                }else{
                                                    s << "(";
                                                }
                                            }
                                            s << arg->modifiedArgumentName() << ")";
                                        }else if((java_type.startsWith(QStringLiteral(u"io.qt.core.QSpan<")) || java_type==QStringLiteral(u"io.qt.core.QSpan"))){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            s << "io.qt.core.QSpan.of";
                                            if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.nio."))
                                                     && annotationFreeAlternativeType.endsWith(QStringLiteral(u"Buffer"))){
                                                s << "Buffer(";
                                            }else{
                                                if(annotationFreeAlternativeType==QStringLiteral(u"byte[]")
                                                        || (annotationFreeAlternativeType==QStringLiteral(u"io.qt.core.QByteArray"))){
                                                    s << "Byte(";
                                                }else if((annotationFreeAlternativeType.startsWith("io.qt.core.QList<"))){
                                                    s << "List(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"short[]")){
                                                    s << "Short(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"int[]")){
                                                    s << "Int(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"long[]")){
                                                    s << "Long(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"char[]")
                                                         || (annotationFreeAlternativeType.startsWith("io.qt.core.")
                                                             && annotationFreeAlternativeType.endsWith("QString"))){
                                                    s << "Char(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"float[]")){
                                                    s << "Float(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"double[]")){
                                                    s << "Double(";
                                                }else if(annotationFreeAlternativeType==QStringLiteral(u"boolean[]")){
                                                    s << "Boolean(";
                                                }else if(annotationFreeAlternativeType.endsWith(QStringLiteral(u"[]"))){
                                                    QString elementType = annotationFreeAlternativeType.chopped(2);
                                                    if(elementType.contains(u"<")){
                                                        s << "Typed(" << toMetaTypeExpression(elementType) << ", ";
                                                    }else{
                                                        s << "Typed(" << elementType << ".class, ";
                                                    }
                                                }else{
                                                    s << "(";
                                                }
                                            }
                                            s << arg->modifiedArgumentName() << ")";
                                        }else if((java_type.startsWith(QStringLiteral(u"io.qt.core.QList<")) || java_type==QStringLiteral(u"io.qt.core.QList"))
                                                 && annotationFreeAlternativeType.endsWith(QStringLiteral(u"[]"))){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            if(annotationFreeAlternativeType==QStringLiteral(u"byte[]")){
                                                s << "io.qt.core.QList.ofByte(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"short[]")){
                                                s << "io.qt.core.QList.ofShort(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"int[]")){
                                                s << "io.qt.core.QList.ofInt(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"long[]")){
                                                s << "io.qt.core.QList.ofLong(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"char[]")){
                                                s << "io.qt.core.QList.ofChar(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"float[]")){
                                                s << "io.qt.core.QList.ofFloat(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"double[]")){
                                                s << "io.qt.core.QList.ofDouble(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"boolean[]")){
                                                s << "io.qt.core.QList.ofBoolean(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.nio."))
                                                     && annotationFreeAlternativeType.endsWith(QStringLiteral(u"Buffer"))){
                                                s << "io.qt.core.QList.ofBuffer(" << arg->modifiedArgumentName() << ")";
                                            }else{
                                                QString elementType = annotationFreeAlternativeType.chopped(2);
                                                if(elementType.contains(u"<")){
                                                    s << "io.qt.core.QList.ofTyped(" << toMetaTypeExpression(elementType) << ", ";
                                                }else{
                                                    s << "io.qt.core.QList.ofTyped(" << elementType << ".class, ";
                                                }
                                                s << arg->modifiedArgumentName() << ")";
                                            }
                                        }else if((java_type.startsWith(QStringLiteral(u"io.qt.core.QSet<")) || java_type==QStringLiteral(u"io.qt.core.QSet"))
                                                 && annotationFreeAlternativeType.endsWith(QStringLiteral(u"[]"))){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            if(annotationFreeAlternativeType==QStringLiteral(u"byte[]")){
                                                s << "io.qt.core.QSet.ofByte(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"short[]")){
                                                s << "io.qt.core.QSet.ofShort(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"int[]")){
                                                s << "io.qt.core.QSet.ofInt(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"long[]")){
                                                s << "io.qt.core.QSet.ofLong(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"char[]")){
                                                s << "io.qt.core.QSet.ofChar(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"float[]")){
                                                s << "io.qt.core.QSet.ofFloat(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"double[]")){
                                                s << "io.qt.core.QSet.ofDouble(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"boolean[]")){
                                                s << "io.qt.core.QSet.ofBoolean(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.nio."))
                                                     && annotationFreeAlternativeType.endsWith(QStringLiteral(u"Buffer"))){
                                                s << "io.qt.core.QSet.ofBuffer(" << arg->modifiedArgumentName() << ")";
                                            }else{
                                                QString elementType = annotationFreeAlternativeType.chopped(2);
                                                if(elementType.contains(u"<")){
                                                    s << "io.qt.core.QSet.ofTyped(" << toMetaTypeExpression(elementType) << ", ";
                                                }else{
                                                    s << "io.qt.core.QSet.ofTyped(" << elementType << ".class, ";
                                                }
                                                s << arg->modifiedArgumentName() << ")";
                                            }
                                        }else if((java_type.startsWith("io.qt.core.QVector<") || java_type=="io.qt.core.QVector")
                                                 && annotationFreeAlternativeType.endsWith(QStringLiteral(u"[]"))){
                                            if(!arg->type()->indirections().isEmpty()){
                                                s << arg->modifiedArgumentName() << "==null ? null : ";
                                            }
                                            if(annotationFreeAlternativeType==QStringLiteral(u"byte[]")){
                                                s << "io.qt.core.QVector.ofByte(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"short[]")){
                                                s << "io.qt.core.QVector.ofShort(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"int[]")){
                                                s << "io.qt.core.QVector.ofInt(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"long[]")){
                                                s << "io.qt.core.QVector.ofLong(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"char[]")){
                                                s << "io.qt.core.QVector.ofChar(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"float[]")){
                                                s << "io.qt.core.QVector.ofFloat(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"double[]")){
                                                s << "io.qt.core.QVector.ofDouble(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType==QStringLiteral(u"boolean[]")){
                                                s << "io.qt.core.QVector.ofBoolean(" << arg->modifiedArgumentName() << ")";
                                            }else if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"java.nio."))
                                                     && annotationFreeAlternativeType.endsWith(QStringLiteral(u"Buffer"))){
                                                s << "io.qt.core.QVector.ofBuffer(" << arg->modifiedArgumentName() << ")";
                                            }else{
                                                QString elementType = annotationFreeAlternativeType.chopped(2);
                                                if(elementType.contains(u"<")){
                                                    s << "io.qt.core.QVector.ofTyped(" << toMetaTypeExpression(elementType) << ", ";
                                                }else{
                                                    s << "io.qt.core.QVector.ofTyped(" << elementType << ".class, ";
                                                }
                                                s << arg->modifiedArgumentName() << ")";
                                            }
                                        }else if(java_type==QStringLiteral(u"java.lang.Object")){
                                            if(arg->type()->typeEntry()->isQVariant()){
                                                if(!annotationFreeAlternativeType.endsWith(QStringLiteral(u"[]")) && !annotationFreeAlternativeType.contains(QStringLiteral(u"<"))){
                                                    if(annotationFreeAlternativeType.startsWith(QStringLiteral(u"io.qt."))){
                                                        if(alternativeType.typeName.contains(QStringLiteral(u"@StrictNonNull"))){
                                                            s << "java.util.Objects.<java.lang.Object>requireNonNull(" << arg->modifiedArgumentName() << ")";
                                                        }else if(alternativeType.typeName.contains(QStringLiteral(u"@Nullable"))){
                                                            s << "io.qt.core.QVariant.typedNullable(" << arg->modifiedArgumentName() << ", " << annotationFreeAlternativeType << ".class)";
                                                        }else{
                                                            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)<QT_VERSION_CHECK(6,0,0))
                                                                s << arg->modifiedArgumentName() << "==null ? new io.qt.core.QVariant(io.qt.core.QMetaType.qMetaTypeId(" << annotationFreeAlternativeType << ".class), null) : " << arg->modifiedArgumentName();
                                                            else if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)<QT_VERSION_CHECK(6,7,0))
                                                                s << arg->modifiedArgumentName() << "==null ? new io.qt.core.QVariant(io.qt.core.QMetaType.fromType(" << annotationFreeAlternativeType << ".class), null) : " << arg->modifiedArgumentName();
                                                            else
                                                                s << "io.qt.core.QVariant.fromMetaType(io.qt.core.QMetaType.fromType(" << annotationFreeAlternativeType << ".class), " << arg->modifiedArgumentName() << ")";
                                                        }
                                                    }
                                                }else{
                                                    s << "io.qt.core.QVariant.nullable(" << arg->modifiedArgumentName() << ")";
                                                }
                                            }else{
                                                s << "(" << java_type.replace(u'$', u'.') << ")" << arg->modifiedArgumentName();
                                            }
                                        }else if(arg->type()->typeEntry()->isQVariant()){
                                            if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)<QT_VERSION_CHECK(6,0,0))
                                                s << "new io.qt.core.QVariant(io.qt.core.QMetaType.qMetaTypeId(" << annotationFreeAlternativeType << ".class), " << arg->modifiedArgumentName() << ")";
                                            else if(QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)<QT_VERSION_CHECK(6,7,0))
                                                s << "new io.qt.core.QVariant(io.qt.core.QMetaType.fromType(" << annotationFreeAlternativeType << ".class), " << arg->modifiedArgumentName() << ")";
                                            else
                                                s << "io.qt.core.QVariant.fromMetaType(io.qt.core.QMetaType.fromType(" << annotationFreeAlternativeType << ".class), " << arg->modifiedArgumentName() << ")";
                                        }else{
                                            s << "new " << java_type.replace(u'$', u'.') << "(" << arg->modifiedArgumentName() << ")";
                                        }
                                    }
                                }else if (used_arguments==-1 || j < used_arguments) {
                                    if (written_arguments > 0)
                                        s << ", ";
                                    s << arg->modifiedArgumentName();
                                    if(!java_function->useArgumentAsBuffer(arg->argumentIndex() + 1)
                                        && java_function->useArgumentAsArray(arg->argumentIndex() + 1)
                                        && java_function->insertUtilArgument(arg->argumentIndex() + 1)){
                                        int lengthParameter = java_function->utilArgumentIndex(arg->argumentIndex() + 1);
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
                                            if(lengthParam){
                                                if(Replacement lengthType = replacedArguments[lengthParam->argumentIndex()]){
                                                    if(lengthType.type==Replacement::RemoveArgument){
                                                        lengthParam = nullptr;
                                                        s << ", 0";
                                                    }
                                                }
                                            }
                                            if(lengthParam && java_function->argumentRemoved(lengthParam->argumentIndex() + 1)==ArgumentRemove_No){
                                                QString modifiedArgumentName = arg->modifiedArgumentName();
                                                modifiedArgumentName[0] = modifiedArgumentName[0].toUpper();
                                                s << ", offsetOf" << modifiedArgumentName;
                                            }
                                        }
                                    }
                                } else {
                                    if (written_arguments > 0)
                                        s << ", ";
                                    QString defaultExpr = arg->defaultValueExpression();
                                    MetaType *arg_type = nullptr;
                                    QString modified_type = java_function->typeReplaced(arg->argumentIndex() + 1);
                                    if (modified_type.isEmpty()) {
                                        arg_type = arg->type();
                                        if (arg_type->isNativePointer()) {
                                            if(defaultExpr=="null"
                                                    && !java_function->useArgumentAsArray(arg->argumentIndex() + 1)
                                                && !java_function->useArgumentAsBuffer(arg->argumentIndex() + 1))
                                                s << "(QNativePointer)";
                                        } else {
                                            const MetaType *abstractMetaType = arg->type();
                                            const TypeEntry *type = abstractMetaType->typeEntry();
                                            if (type->designatedInterface())
                                                type = type->designatedInterface();
                                            if (!type->isEnum() && !type->isFlags()) {
                                                if(defaultExpr=="null" || type->isPrimitive()){
                                                    s << "(";
                                                    writeInstantiatedType(s, abstractMetaType, false);
                                                    if(java_function->useArgumentAsArray(arg->argumentIndex() + 1))
                                                        s << "[]";
                                                    s << ")";
                                                }
                                            }
                                        }
                                    } else {
                                        if(defaultExpr==QStringLiteral(u"null"))
                                            s << "(" << modified_type.replace(" @Nullable", "")
                                                                     .replace(" @NonNull", "")
                                                                     .replace(" @StrictNonNull", "")
                                                                     .replace("@Nullable ", "")
                                                                     .replace("@NonNull ", "")
                                                                     .replace("@StrictNonNull ", "")
                                                                     .replace('$', u'.') << ")";
                                    }

                                    auto pos = defaultExpr.indexOf(".");
                                    if (pos > 0) {
                                        QString someName = defaultExpr.left(pos);
                                        ComplexTypeEntry *ctype =
                                            m_database->findComplexType(someName);
                                        QString replacement;
                                        if (ctype && ctype->isQVariant())
                                            replacement = QStringLiteral(u"QVariant.");
                                        else if (ctype)
                                            if(ctype->javaPackage().isEmpty())
                                                replacement = ctype->targetLangName() + QStringLiteral(u".");
                                            else
                                                replacement = ctype->javaPackage() + "." + ctype->targetLangName() + ".";
                                        else
                                            replacement = someName + ".";
                                        defaultExpr = defaultExpr.replace(someName + ".", replacement);
                                    }

                                    if (arg_type && arg_type->isFlags()) {
                                        s << "new " << arg_type->fullName().replace(u'$', u'.') << "(" << defaultExpr << ")";
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
                        s << ")";
                    }
                    if(precontent.isEmpty() && postcontent.isEmpty()){
                        s << INDENT;
                        if (new_return_type != "void"
                                && (!new_return_type.isEmpty() || java_function->type())
                                && !delegate.isSelfReturning){
                            s << "return ";
                        }
                        s << content << ";" << Qt::endl;
                    }else{
                        if (new_return_type != "void"
                                && (!new_return_type.isEmpty() || java_function->type())
                                && !delegate.isSelfReturning){
                            if(!new_return_type.isEmpty()){
                                s << INDENT << new_return_type;
                            }else{
                                s << INDENT << qualifiedJavaType(java_function->type(), java_function->implementingClass(), Option(option));
                            }
                            s << " __java_return_value;" << Qt::endl;
                            pre.seek(0);
                            while(!pre.atEnd()){
                                s << INDENT << pre.readLine() << Qt::endl;
                            }
                            s << INDENT << "try{" << Qt::endl;
                            {
                                INDENTATION(INDENT);
                                s << INDENT << "__java_return_value = " << content << ";" << Qt::endl;
                            }
                            s << INDENT << "}finally{" << Qt::endl;
                            {
                                INDENTATION(INDENT);
                                post.seek(0);
                                while(!post.atEnd()){
                                    s << INDENT << post.readLine() << Qt::endl;
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                            s << INDENT << "return __java_return_value;" << Qt::endl;
                        }else{
                            pre.seek(0);
                            while(!pre.atEnd()){
                                s << INDENT << pre.readLine() << Qt::endl;
                            }
                            s << INDENT << "try{" << Qt::endl;
                            {
                                INDENTATION(INDENT);
                                s << INDENT << content << ";" << Qt::endl;
                            }
                            s << INDENT << "}finally{" << Qt::endl;
                            {
                                INDENTATION(INDENT);
                                post.seek(0);
                                while(!post.atEnd()){
                                    s << INDENT << post.readLine() << Qt::endl;
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }
                    }
                    if(delegate.isSelfReturning)
                        s << INDENT << "return this;" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
            }
        }
    }
    if(hasCalls && hasBeenCalled && !m_functions_with_implicit_calls.contains(java_function))
        m_functions_with_implicit_calls.append(java_function);
}

QString JavaGenerator::subDirectoryForClass(const MetaClass *java_class) const{
    QString pkgDir = java_class->typeEntry()->isGlobal() ? QString() : subDirectoryForPackage(java_class->package());
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(java_class->typeEntry()->targetTypeSystem()));
    if(!typeSystemEntry)
        typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(java_class->package()));
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
    }else if(typeSystemEntry && !typeSystemEntry->targetName().isEmpty()){
        return typeSystemEntry->targetName() + "/" + pkgDir;
    }else{
        return pkgDir;
    }
}

QString JavaGenerator::subDirectoryForFunctional(const MetaFunctional * java_class) const{
    QString pkgDir = subDirectoryForPackage(java_class->package());
    TypeSystemTypeEntry * typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(java_class->typeEntry()->targetTypeSystem()));
    if(!typeSystemEntry)
        typeSystemEntry = static_cast<TypeSystemTypeEntry *>(m_database->findType(java_class->package()));
    if(typeSystemEntry && !typeSystemEntry->qtLibrary().isEmpty()){
        if(typeSystemEntry->qtLibrary().startsWith("Qt") && !typeSystemEntry->qtLibrary().startsWith("QtJambi")){
            QString libName = typeSystemEntry->qtLibrary();
            return "QtJambi" + libName.mid(2) + "/" + pkgDir;
        }else
            return typeSystemEntry->qtLibrary() + "/" + pkgDir;
    }else if(typeSystemEntry && !typeSystemEntry->targetName().isEmpty()){
        return typeSystemEntry->targetName() + "/" + pkgDir;
    }else
        return pkgDir;
}

void JavaGenerator::write(QTextStream &s, const MetaClass *java_class, int nesting_level) {
    if(java_class->enclosingClass() && nesting_level==0){ // don't write nested classes into own file
        return;
    }
    if(java_class->typeEntry()->designatedInterface() && java_class->typeEntry()->designatedInterface()->noImpl()){
        return;
    }

    if(java_class->typeEntry()->isIterator())
        return;

    {
        const MetaFunctionList inconsistentFunctions = java_class->cppInconsistentFunctions();
        if(!inconsistentFunctions.isEmpty()){
            for(MetaFunction* f : inconsistentFunctions)
                m_inconsistent_functions << f;
            ReportHandler::warning("Unable to generate class "+java_class->fullName()+" due to inconsistent functions.");
            return;
        }
    }

    ReportHandler::debugSparse("Generating class: " + java_class->fullName());
    bool fakeClass = java_class->isFake();

    if(nesting_level==0){
        s << INDENT << "package " << java_class->package() << ";" << Qt::endl << Qt::endl;
        m_currentPackages.clear();

        QSet<QString> imports;
        imports.insert("io.qt.*");
        s << "import io.qt.*;" << Qt::endl;
        for(const Include &inc : java_class->typeEntry()->extraIncludes()) {
            if (inc.type == Include::TargetLangImport) {
                QString imp = inc.toString();
                if(!imports.contains(imp)){
                    imports.insert(imp);
                    s << imp << Qt::endl;
                }
            }
        }
        if(!imports.isEmpty())
            s << Qt::endl;
    }

    QString comment;
    QString lines;
    {
        QTextStream s(&lines);
        QTextStream commentStream(&comment);
        if(java_class->typeEntry()->designatedInterface()){
            commentStream << "@hidden" << Qt::endl;
            commentStream << "<p>Implementor class for interface {@link " << QString(java_class->typeEntry()->designatedInterface()->qualifiedTargetLangName()).replace("$", ".") << "}</p>" << Qt::endl;
        }else{
            bool hasIntro = false;
            {
                Indentor indentor;
                for(const CodeSnip& snip : java_class->typeEntry()->codeSnips()){
                    if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::CommentIntro){
                        snip.formattedCode(commentStream, indentor);
                        hasIntro = true;
                    }
                }
            }
            if(!hasIntro || !java_class->brief().isEmpty() || !java_class->href().isEmpty()){
                if(!java_class->brief().isEmpty()){
                    commentStream << "<p>" << encodeHtml(java_class->brief()) << "</p>" << Qt::endl;
                }
                commentStream << "<p>Java wrapper for Qt ";
                if(java_class->typeEntry()->isNamespace()){
                    if(reinterpret_cast<const NamespaceTypeEntry*>(java_class->typeEntry())->isHeader())
                        commentStream << "namespace ";
                    else
                        commentStream << "header file ";
                }else{
                    commentStream << "class ";
                }
                commentStream << "<code>";
                if(!java_class->href().isEmpty())
                    commentStream << "<a href=\"" << docsUrl << java_class->href() << "\">";
                commentStream << encodeHtml(
                                                 //                                     java_class->templateBaseClass()
                                                 //                                     ? java_class->templateBaseClass()->qualifiedCppName().replace("<JObjectWrapper>", "<T>")
                                                 //                                     :
                                                 java_class->qualifiedCppName() );
                if(!java_class->href().isEmpty())
                    commentStream << "</a>";
                commentStream << "</code></p>" << Qt::endl;
            }
            if(!java_class->since().isEmpty()){
                commentStream << "@since This class was introduced in Qt " << java_class->since() << "." << Qt::endl;
            }
            {
                Indentor indentor;
                for(const CodeSnip& snip : java_class->typeEntry()->codeSnips()){
                    if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::Comment){
                        snip.formattedCode(commentStream, indentor);
                    }
                }
            }
        }

        if (java_class->isDeprecated()) {
            if(!java_class->deprecatedComment().isEmpty()){
                if(!comment.isEmpty())
                    commentStream << Qt::endl;
                commentStream << "@deprecated " << encodeHtml(java_class->deprecatedComment()) << Qt::endl;
            }
            s << INDENT << "@Deprecated" << Qt::endl;
        }

        s << INDENT;

        bool force_friendly = java_class->typeEntry()->isForceFriendly();
        if(!force_friendly && java_class->enclosingClass()){
            auto enclosingClass = java_class->enclosingClass();
            while(enclosingClass->enclosingClass())
                enclosingClass = enclosingClass->enclosingClass();
            force_friendly = enclosingClass->typeEntry()->isForceFriendly();
        }
        bool force_abstract = java_class->typeEntry()->isForceAbstract();
        bool isInterface = java_class->isInterface();
        bool isFinal = false;
        if (java_class->isPublic() && !force_friendly)
            s << "public ";
        // else friendly
        if(nesting_level>0){
            if(!force_friendly){
                if (java_class->isProtected()){
                    s << "protected ";
                }else if (java_class->isPrivate()){
                    s << "private ";
                }
            }
            s << "static ";
        }
        if (isInterface) {
            s << "interface ";
        } else {
            bool force_final = java_class->typeEntry()->forceFinal();
            if(force_final){
                s << "final ";
                isFinal = true;
            }else if ((java_class->isFinal() || java_class->isNamespace())
                    && !java_class->hasSubClasses()
                    && !force_final
                    && !force_abstract
                    && !java_class->isAbstract()
            ){
                s << "final ";
                isFinal = true;
            }else if ( ( (java_class->isAbstract() && !java_class->isNamespace()) || force_abstract ) && !force_final){
                s << "abstract ";
            }
            s << "class ";
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
                    s << QString(templateArgument->name()).replace(u'$', u'.');
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
                for(const MetaType * instantiation : java_class->templateBaseClassInstantiations()){
                    if(k>0){
                        s << ", ";
                    }
                    s << qualifiedJavaType(instantiation, java_class, Option(BoxedPrimitive | IsReturnType | (m_nullness ? NoOption : NoNullness)));
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
                            s << " extends QtObject";
                    }else{
                        s << " extends " << java_class->baseClass()->fullName().replace("$",".");
                    }
                } else if(!java_class->typeEntry()->isQMetaObjectType()){
                    QString sc = QString(type->defaultSuperclass()).replace("$",".");
                    if (!sc.isEmpty())
                        s << " extends " << sc;
                    else
                        s << " extends QtObject";
                }
            } else if (java_class->isInterface()) {
                s << " extends QtObjectInterface";
            }
        }

        // implementing interfaces...
        bool implements = java_class->isInterface();
        MetaClassList interfaces = java_class->interfaces();

        if (!interfaces.isEmpty()) {
            if (java_class->isInterface())
                s << ", ";
            else {
                s << Qt::endl << INDENT << "    implements ";
                implements = true;
            }
            for (int i = 0; i < interfaces.size(); ++i) {
                MetaClass *iface = interfaces.at(i);
                m_currentPackages << iface->package();
                if (i != 0)
                    s << "," << Qt::endl << INDENT << "            ";
                s << iface->package() << "." << iface->name().replace(u'$', u'.');
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

        bool isConst = isContainer;
        const MetaType * iterableType = nullptr;
        if(!isContainer)
            iterableType = getIterableType(java_class, isConst);
        if (iterableType) {
            if (!implements) {
                implements = true;
                s << Qt::endl << INDENT << "    implements ";
            } else {
                s << "," << Qt::endl << INDENT << "            ";
            }
            s << "java.lang.Iterable<" << qualifiedJavaType(iterableType, java_class, Option(BoxedPrimitive | IsReturnType | (m_nullness ? NoOption : NoNullness)))<< ">";
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
            const QStringList clss = java_class->typeEntry()->implements().split(",");
            for(const QString& cls : clss){
                registerPackage(cls);
            }
            s << java_class->typeEntry()->implements();
        }

        s << Qt::endl << INDENT << "{" << Qt::endl;

        QHash<QString,const MetaFunction *> previousMethodSignatures;
        m_currentMethodSignatures.swap(previousMethodSignatures);

        {
            INDENTATION(INDENT);

            if (!isInterface) {
                if(!java_class->baseClass()
                    || java_class->package() != java_class->baseClass()->package()
                    || java_class->baseClass()->fullName()==java_class->fullName()){
                    s << INDENT << "static {" << Qt::endl;
                        s << INDENT << "    QtJambi_LibraryUtilities.initialize();" << Qt::endl; //" << java_class->package() << ".
                    s << INDENT << "}" << Qt::endl
                      << INDENT << Qt::endl;
                }
                if(java_class->typeEntry()->isQMetaObjectType()){
                    s << INDENT << "private final long __qt_persistentPointer;" << Qt::endl;
                }
                if (java_class->typeEntry()->expensePolicy().isValid()) {
                    s << INDENT << "private static long __qt_expenseCounter = 0;" << Qt::endl;
                }
                QMap<QString, int> variables;
                //bool isWrapperClass = java_class->typeEntry()->lookupName().endsWith("$ConcreteWrapper");
                for(MetaFunction *function : java_class->functions()) {
                    const QList<ReferenceCount> referenceCounts = function->referenceCounts(java_class);
                    for(const ReferenceCount& refCount : referenceCounts) {
                        variables[refCount.variableName] |=
                            uint(refCount.action)
                            | ( /*(isWrapperClass && function->isAbstract()) ? ReferenceCount::Friendly :*/ refCount.access )
                            | (refCount.threadSafe ? ReferenceCount::ThreadSafe : 0)
                            | (function->isStatic() ? ReferenceCount::Static : 0)
                            | (refCount.declareVariable.isEmpty() ? ReferenceCount::DeclareVariable : 0);
                    }
                }
                for(MetaField *field : java_class->fields()) {
                    const QList<ReferenceCount> referenceCounts = field->setter()->referenceCounts(java_class);
                    for(const ReferenceCount& refCount : referenceCounts) {
                        variables[refCount.variableName] |=
                            uint(refCount.action)
                            | ( /*(isWrapperClass && function->isAbstract()) ? ReferenceCount::Friendly :*/ refCount.access )
                            | (refCount.threadSafe ? ReferenceCount::ThreadSafe : 0)
                            | (field->isStatic() ? ReferenceCount::Static : 0)
                            | (refCount.declareVariable.isEmpty() ? ReferenceCount::DeclareVariable : 0);
                    }
                }

                const auto keys = variables.keys();
                for(const QString& variableName : keys) {
                    int attributes = variables[variableName];
                    int actions = attributes & ReferenceCount::ActionsMask;
                    bool threadSafe = attributes & ReferenceCount::ThreadSafe;
                    bool isStatic = attributes & ReferenceCount::Static;
                    bool declareVariable = attributes & ReferenceCount::DeclareVariable;
                    int access = attributes & ReferenceCount::AccessMask;

                    if (actions == ReferenceCount::Ignore || !declareVariable)
                        continue;

                    if ((actions & ReferenceCount::Add)
                            && !(actions & ReferenceCount::Remove)
                            && !(actions & ReferenceCount::ClearAdd)
                            && !(actions & ReferenceCount::ClearAddAll)) {
                        QString warn = QStringLiteral(u"Either add or remove specified for reference count variable '%1' in '%2' but not both.")
                                       .arg(variableName, java_class->qualifiedCppName());
                        ReportHandler::warning(warn);
                    }
                    if ((actions & ReferenceCount::Remove)
                            && !(actions & ReferenceCount::Add)
                            && !(actions & ReferenceCount::Put)
                            && !(actions & ReferenceCount::ClearAdd)
                            && !(actions & ReferenceCount::ClearAddAll)) {
                        QString warn = QStringLiteral(u"Either add or remove specified for reference count variable '%1' in '%2' but not both.")
                                       .arg(variableName, java_class->qualifiedCppName());
                        ReportHandler::warning(warn);
                    }

                    s << INDENT << "@QtPropertyMember(enabled=false)" << Qt::endl
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

                    if (actions & ReferenceCount::Put){
                        s << "java.util.Map<Object,Object> " << variableName << ";" << Qt::endl;
                        m_referenceCountContainer[java_class->fullName()][variableName] = ReferenceCount::Put;
                        if(java_class->extractInterface())
                            m_referenceCountContainer[java_class->extractInterface()->fullName()][variableName] = ReferenceCount::Put;
                    }else if (!(actions & ReferenceCount::Set) && actions != ReferenceCount::Ignore) {
                        s << "java.util.Collection<Object> " << variableName << ";" << Qt::endl;
                        m_referenceCountContainer[java_class->fullName()][variableName] = ReferenceCount::Add;
                        if(java_class->extractInterface())
                            m_referenceCountContainer[java_class->extractInterface()->fullName()][variableName] = ReferenceCount::Add;
                    } else if (actions != ReferenceCount::Ignore) {
                        if (threadSafe)
                            s << "synchronized ";
                        s << "Object " << variableName << ";" << Qt::endl;
                        m_referenceCountContainer[java_class->fullName()][variableName] = ReferenceCount::Set;
                        if(java_class->extractInterface())
                            m_referenceCountContainer[java_class->extractInterface()->fullName()][variableName] = ReferenceCount::Set;
                    }
                }
                if(!variables.isEmpty())
                    s << INDENT << Qt::endl;
            }

            if(java_class->isNamespace()){
                bool hasMetaObject = reinterpret_cast<const NamespaceTypeEntry*>(java_class->typeEntry())->hasMetaObject();
                if(hasMetaObject){
                    QString pkg;
                    if(java_class->package()!="io.qt.core"){
                        pkg = "io.qt.core.";
                    }
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * This variable stores the meta-object for the namespace." << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "public static final " << pkg;
                    if(m_nullness)
                        s << "@NonNull ";
                    s << "QMetaObject staticMetaObject = " << pkg << "QMetaObject.forType("
                      << java_class->name().replace(u'$', u'.') << ".class);" << Qt::endl
                      << INDENT << Qt::endl;
                }
                if(!isInterface){
                    s << INDENT << "private " << java_class->simpleName() << "() throws java.lang.InstantiationError {" << Qt::endl;
                    if(java_class->enums().isEmpty() && !(hasMetaObject && m_nullness))
                        s << INDENT << "    QtUtilities.class.hashCode();" << Qt::endl;
                    s << INDENT << "    throw new java.lang.InstantiationError(\"Cannot instantiate namespace " << java_class->simpleName() << ".\");" << Qt::endl
                      << INDENT << "}" << Qt::endl
                      << INDENT << Qt::endl;
                }
            }

            if(java_class->isInterface() || !java_class->typeEntry()->designatedInterface()){
                if (java_class->has_Q_GADGET() || java_class->has_Q_OBJECT()) {
                    QString pkg;
                    if(java_class->package()!="io.qt.core"){
                        pkg = "io.qt.core.";
                    }
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * This variable stores the meta-object for the class." << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "public static final " << pkg;
                    if(m_nullness)
                        s << "@NonNull ";
                    s << "QMetaObject staticMetaObject = " << pkg << "QMetaObject.forType("
                      << java_class->name().replace(u'$', u'.') << ".class);" << Qt::endl
                      << INDENT << Qt::endl;
                }
            }

            if (!java_class->isInterface() && (java_class->isAbstract() || force_abstract)) {
                s << INDENT << "@NativeAccess" << Qt::endl
                  << INDENT << "private static final class ConcreteWrapper extends " << java_class->name().replace(u'$', u'.') << " {" << Qt::endl;

                {
                    INDENTATION(INDENT);
                    s << INDENT << Qt::endl
                      << INDENT << "@NativeAccess" << Qt::endl
                      << INDENT << "private ConcreteWrapper(QPrivateConstructor p) { super(p); }" << Qt::endl;

                    const MetaFunctionList functions = java_class->queryFunctions(MetaClass::NormalFunctions | MetaClass::AbstractFunctions | MetaClass::NonEmptyFunctions | MetaClass::NotRemovedFromTargetLang);
                    for(const MetaFunction *java_function : functions) {
                        if(!java_function->isPrivate()){
                            uint exclude_attributes = MetaAttributes::Abstract | MetaAttributes::Override;
                            if(java_function->needsCallThrough()){
                                exclude_attributes |= MetaAttributes::Native;
                            }
                            uint include_attributes = 0;
                            retrieveModifications(java_function, java_class, &exclude_attributes, &include_attributes);

                            s << INDENT << Qt::endl
                              << INDENT << "@Override" << Qt::endl;
                            writeFunctionAttributes(s, java_function, -1, {}, include_attributes, exclude_attributes,
                                                    (java_function->isNormal() || java_function->isSignal()) ? Option(NoSuppressExports) : Option(SkipReturnType | NoSuppressExports));

                            s << java_function->name() << "(";
                            writeFunctionArguments(s, java_function, {}, int(java_function->arguments().count()));
                            s << ")";
                            QString throws = java_function->throws();
                            if(!throws.isEmpty()){
                                s << " throws " << throws << " ";
                            }
                            if(java_function->needsCallThrough()){
                                s << "{" << Qt::endl;
                                {
                                    INDENTATION(INDENT);
                                    writeJavaCallThroughContents(s, java_function);
                                }
                                s << INDENT << "}" << Qt::endl;
                                writePrivateNativeFunction(s, java_function);
                            }else{
                                s << ";" << Qt::endl << Qt::endl;
                            }
                        }
                    }
                    writeExtraFunctions(s, java_class, true);
                }
                s << INDENT << "}" << Qt::endl
                  << INDENT << Qt::endl;
            }

            // Enums
            int writtenEnums = 0;
            for(MetaEnum *java_enum : java_class->enums()){
                if(java_enum->enclosingClass()==java_class){
                    writeEnum(s, java_enum);
                    ++writtenEnums;
                }
            }

            // functionals
            for(MetaFunctional *java_f : java_class->functionals())
                writeFunctional(s, java_f);

            if ((writtenEnums>0 || !java_class->functionals().isEmpty()) && !java_class->enclosedClasses().isEmpty())
                s << Qt::endl;
            // write enclosed types as static embedded classes
            for(MetaClass *enclosed_java_class : java_class->enclosedClasses()){
                if ((enclosed_java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)){
                    write(s, enclosed_java_class, nesting_level+1);
                }
            }
            if (!java_class->enclosedClasses().isEmpty() && !java_class->functions().isEmpty())
                s << Qt::endl;

            // Signals
            QList<QString> signalNames;
            QMap<QString,MetaFunctionList> sortedSignals;
            const MetaFunctionList functions = java_class->queryFunctions(MetaClass::Signals
                                                                          | MetaClass::ClassImplements
                                                                          | MetaClass::NotRemovedFromTargetLang);
            for(MetaFunction* function : functions){
                QString key = function->declaringClass()->typeEntry()->qualifiedCppName() + "::" + function->name();
                if(!signalNames.contains(key))
                    signalNames.append(key);
                sortedSignals[key].append(function);
            }

            for(const QString& key : qAsConst(signalNames)){
                const MetaFunctionList& list = sortedSignals[key];
                if(list.size()==1)
                    writeSignal(s, list.first());
                else if(list.size()>1)
                    writeMultiSignal(s, list);
            }
            s << INDENT << Qt::endl;

            // Class has subclasses but also only private constructors
            if (!isInterface && java_class->hasUnimplmentablePureVirtualFunction()) {
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * This constructor is a place holder intended to prevent" << Qt::endl
                  << INDENT << " * users from subclassing the class. Certain classes can" << Qt::endl
                  << INDENT << " * unfortunately only be subclasses internally. The constructor" << Qt::endl
                  << INDENT << " * will indiscriminately throw an exception if called. If the" << Qt::endl
                  << INDENT << " * exception is ignored, any use of the constructed object will" << Qt::endl
                  << INDENT << " * cause an exception to occur." << Qt::endl << Qt::endl
                  << INDENT << " * @throws QClassCannotBeSubclassedException" << Qt::endl
                  << INDENT << " **/" << Qt::endl
                  << INDENT << "@SuppressWarnings(\"unused\")" << Qt::endl
                  << INDENT << "private " << java_class->simpleName() << "() throws QClassCannotBeSubclassedException {" << Qt::endl
                  << INDENT << "    super((QPrivateConstructor)null);" << Qt::endl
                  << INDENT << "    throw new QClassCannotBeSubclassedException(" << java_class->simpleName() << ".class);" << Qt::endl
                  << INDENT << "}" << Qt::endl << Qt::endl;
            }

            // Functions
            bool alreadyHasCloneMethod = false;
            bool generateShellClass = java_class->generateShellClass();
            MetaFunctionList privatePureVirtualFunctions;

            QList<const MetaFunction *> overloadedFunctions;
            MetaFunctionList java_funcs = java_class->functionsInTargetLang();
            for (int i = 0; i < java_funcs.size(); ++i) {
                MetaFunction *function = java_funcs.at(i);

                // If a method in an interface class is modified to be private or protected, this should
                // not be present in the interface at all, only in the implementation.
                if (java_class->isInterface()) {
                    uint includedAttributes = 0;
                    uint excludedAttributes = 0;
                    retrieveModifications(function, java_class, &excludedAttributes, &includedAttributes);
                    if (includedAttributes & MetaAttributes::Private
                            || includedAttributes & MetaAttributes::Protected
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
                if(java_class->typeEntry()->isDestructorPrivate() && function->isConstructor())
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
                overloadedFunctions << function;
            }

            if (iterableType) {
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns an iterator for this iterable object." << Qt::endl
                  << INDENT << " * @see " << (isConst ? "#constBegin()" : "#begin()") << Qt::endl
                  << INDENT << " * @see " << (isConst ? "#constEnd()" : "#end()") << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@Override" << Qt::endl
                  << INDENT << "public java.util.";
                if(m_nullness)
                    s << "@NonNull ";
                s << "Iterator<" << qualifiedJavaType(iterableType, java_class, Option(BoxedPrimitive | IsReturnType | (m_nullness ? NoOption : NoNullness))) << "> iterator() {" << Qt::endl
                  << INDENT << "    return " << (isConst ? "constBegin" : "begin") << "().iterator();" << Qt::endl
                  << INDENT << "}" << Qt::endl
                  << INDENT << Qt::endl;
            }

            // Field accessors
            MetaFieldList fields = java_class->fields();
            MetaFieldList nonPublicFields;
            for(MetaField *field : fields) {
                if (field->wasPublic() || (!java_class->isInterface() && field->wasProtected() && !java_class->isFinal())) {
                    writeFieldAccessors(s, field, Option(NoOption), &overloadedFunctions);
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
                  << INDENT << " * Constructor for internal use only." << Qt::endl;
                if(java_class->typeEntry()->isQMetaObjectType()){
                    s << INDENT << " * @param p expected to be <code>null</code>." << Qt::endl;
                }else{
                    s << INDENT << " * @param persistentPointer" << Qt::endl;
                }
                s << INDENT << " * @hidden" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "@NativeAccess" << Qt::endl
                  << INDENT << (isFinal ? "private " : "protected ") << java_class->simpleName();
                if(java_class->typeEntry()->isQMetaObjectType()){
                    s << "(long persistentPointer) { this.__qt_persistentPointer = persistentPointer; } " << Qt::endl;
                }else{
                    s << "(QPrivateConstructor p) { super(p); } " << Qt::endl;
                }
                s << INDENT << Qt::endl;
                if(!isFinal && java_class->generateShellClass() && java_class->isQObject() && java_class->hasStandardConstructor() && !java_class->hasUnimplmentablePureVirtualFunction()){
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * Constructor for internal use only." << Qt::endl
                      << INDENT << " * It is not allowed to call the declarative constructor from inside Java." << Qt::endl
                      << INDENT << " * @hidden" << Qt::endl
                      << INDENT << " * @deprecated Use {@link QtConstructInPlace} instead." << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "@SuppressWarnings(\"removal\")" << Qt::endl
                      << INDENT << "@Deprecated(forRemoval = true, since = \"6.10\")" << Qt::endl
                      << INDENT << (isFinal ? "private " : "protected ") << java_class->simpleName() << "(QDeclarativeConstructor constructor) {" << Qt::endl;
                    {
                        INDENTATION(INDENT);
                        s << INDENT << "super((QPrivateConstructor)null);" << Qt::endl;
                        s << INDENT << "constructor.initialize(this);" << Qt::endl;
                    }
                    s << INDENT << "} " << Qt::endl;
                }
            }

            if (!java_class->isInterface()) {
                writeJavaLangObjectOverrideFunctions(s, java_class);
            }
            writeExtraFunctions(s, java_class, false);
            if (!java_class->isInterface()) {
                writeToStringFunction(s, java_class);
            }

            if (java_class->hasCloneOperator() && !alreadyHasCloneMethod) {
                writeCloneFunction(s, java_class);
            }
            for(const MetaFunction *function : qAsConst(overloadedFunctions)){
                writeFunctionOverloads(s, function);
            }

            if (java_class->isInterface()) {
                const InterfaceTypeEntry* itype = static_cast<const InterfaceTypeEntry*>(java_class->typeEntry());
                MetaFunctionList nonPublicFunctions;
                MetaFunctionList restrictedFunctions;
                for(MetaFunction *java_function : qAsConst(java_funcs)) {
                    if(java_function->declaringClass()==java_class){
                        if(!java_function->isStatic()
                                && !java_function->isConstructor()
                                && !java_function->isAbstract()
                                && !java_function->isPrivate()
                                && !java_function->isPublic()
                                && !java_function->isRemovedFrom(java_class, TS::TargetLangCode)){
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
                        INDENTATIONRESET(INDENT);
                        QString comment2;
                        QTextStream commentStream2(&comment2);
                        for (MetaFunction *function : qAsConst(nonPublicFunctions)) {
                            writeFunction(commentStream2, function, 0, 0, Option(NoNullness | InFunctionComment | NoSuppressExports));
                        }
                        for(const MetaField *field : qAsConst(nonPublicFields)) {
                            writeFieldAccessors(commentStream2, field, Option(NoNullness | InFunctionComment | NoSuppressExports));
                        }
                        commentStream2.seek(0);
                        commentStream << Qt::endl
                                      << "<br>" << Qt::endl
                                      << "<code>" << Qt::endl;
                        while(!commentStream2.atEnd()){
                            QString line = encodeHtml(commentStream2.readLine());
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
                        QString targetLangName = java_class->typeEntry()->targetLangName().replace(u'$', u'.');
                        s << INDENT << Qt::endl
                          << INDENT << "/**" << Qt::endl
                          << INDENT << " * @hidden" << Qt::endl
                          << INDENT << " */" << Qt::endl
                          << INDENT << "public static class MemberAccess extends io.qt.MemberAccess<" << targetLangName << "> {" << Qt::endl;
                        {
                            INDENTATION(INDENT);
                            s << INDENT << "private MemberAccess(" << targetLangName << " instance){" << Qt::endl
                              << INDENT << "    super(instance);" << Qt::endl
                              << INDENT << "}" << Qt::endl
                              << INDENT << Qt::endl;
                            for(MetaFunction *java_function : qAsConst(restrictedFunctions)) {
                                if(java_function->isPublic()){
                                    *java_function -= MetaAttributes::Public;
                                    *java_function += MetaAttributes::Private;
                                }else{
                                    *java_function += MetaAttributes::Public;
                                }
                                *java_function -= MetaAttributes::Native;
                                *java_function -= MetaAttributes::Abstract;
                                s << functionSignature(java_function, 0, 0, Option(NoOption))
                                  << "{" << Qt::endl;
                                {
                                    INDENTATION(INDENT);
                                    s << INDENT << targetLangName << " instance = instance();" << Qt::endl;

                                    const MetaArgumentList& arguments = java_function->arguments();

                                    for (int i = 0; i < arguments.size(); ++i) {
                                        if (arguments[i]->isNullPointerDisabled(java_function)){
                                            s << INDENT << "java.util.Objects.requireNonNull(" << arguments[i]->modifiedArgumentName() << ", \"Argument '" << arguments[i]->modifiedArgumentName() << "': null not expected.\");" << Qt::endl;
                                        }
                                    }

                                    bool has_argument_referenceCounts = false;
                                    QMap<int,QList<ReferenceCount>> referenceCounts;
                                    for (int i = -1; i <= arguments.size(); ++i) {
                                        referenceCounts[i] = java_function->referenceCounts(java_function->implementingClass(), i);
                                        if (referenceCounts[i].size() > 0) {
                                            for(const ReferenceCount& refCount : qAsConst(referenceCounts[i])) {
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
                                    MetaType *return_type = java_function->type();
                                    QString new_return_type = QString(java_function->typeReplaced(0)).replace(u'$', u'.');
                                    bool has_return_type = (new_return_type != "void"
                                                            && (!new_return_type.isEmpty() || return_type)) || java_function->isSelfReturningFunction();
                                    OwnershipRule ownershipRule = java_function->ownership(java_function->implementingClass(), TS::TargetLangCode, 0);

                                    bool has_code_injections_at_the_end = hasCodeInjections(java_function, {CodeSnip::End, CodeSnip::Position4, CodeSnip::Position3});

                                    bool needs_return_variable = has_return_type
                                                                 && ( ( ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership)
                                                                      || has_argument_referenceCounts || referenceCounts[0].size() > 0 || has_code_injections_at_the_end);

                                    s << INDENT;
                                    if (has_return_type && (java_function->argumentReplaced(0).isEmpty() || java_function->argumentReplaced(0)=="this")) {
                                        if (needs_return_variable) {
                                            if (new_return_type.isEmpty())
                                                s << qualifiedJavaType(return_type, java_function->implementingClass(), Option(IsReturnType | (m_nullness ? NoOption : NoNullness)));
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
                                                s << "new " << return_type->typeEntry()->qualifiedTargetLangName().replace(u'$', u'.') << "(";
                                            }
                                        }
                                    }

                                    s << itype->origin()->targetLangName().replace(u'$', u'.')
                                      << "." << java_function->marshalledName() << "(";
                                    if(java_class->typeEntry()->isNativeIdBased()){
                                        s << "QtJambi_LibraryUtilities.internal.nativeId(instance)";
                                    }else if(java_class->typeEntry()->isQMetaObjectType()){
                                        s << "instance.__qt_persistentPointer";
                                    }else{
                                        s << "instance";
                                    }

                                    bool needsComma = false;
                                    for (int i = 0; i < arguments.count(); ++i) {
                                        const MetaArgument *arg = arguments.at(i);
                                        const MetaType *type = arg->type();

                                        if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
                                            if (needsComma || (!java_function->isStatic() && !java_function->isConstructor()))
                                                s << ", ";
                                            needsComma = true;

                                            if(!java_function->typeReplaced(arg->argumentIndex()+1).isEmpty()
                                                    || java_function->useArgumentAsArray(arg->argumentIndex()+1)
                                                || java_function->useArgumentAsBuffer(arg->argumentIndex()+1)){
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

                                    s << ")";

                                    // This closed the ".resolve(" or the "new MyType(" fragments
                                    if (return_type && new_return_type.isEmpty() && (return_type->isTargetLangEnum() || return_type->isTargetLangFlags()))
                                        s << ")";

                                    s << ";" << Qt::endl;

                                    for(ReferenceCount refCount : qAsConst(referenceCounts[-1])){
                                        refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace(u'$', u'.');
                                        writeReferenceCount(s, refCount, -1, java_function, {}, java_function->isStatic() ? QLatin1String("null") : QLatin1String("this"));
                                    }

                                    // We must ensure we retain a Java hard-reference over the native method call
                                    // so that the GC will not destroy the C++ object too early.  At this point we
                                    // have called the native method call so can manage referenceCount issues.
                                    // First the input arguments
                                    for (const MetaArgument* argument : qAsConst(arguments)) {
                                        for(ReferenceCount refCount : referenceCounts[argument->argumentIndex()+1]){
                                            refCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace(u'$', u'.');
                                            writeReferenceCount(s, refCount, argument->argumentIndex()+1, java_function, {}, QLatin1String("instance"));
                                        }
                                    }

                                    if (!java_function->argumentReplaced(0).isEmpty() && java_function->argumentReplaced(0)!="this") {
                                        s << INDENT << "return " << java_function->argumentReplaced(0) << ";" << Qt::endl;
                                    }else{
                                        // Then the return value
                                        for(ReferenceCount referenceCount : qAsConst(referenceCounts[0])) {
                                            referenceCount.declareVariable = java_function->declaringClass()->fullName().replace("/", ".").replace(u'$', u'.');
                                            writeReferenceCount(s, referenceCount, 0, java_function, {}, QLatin1String("instance"));
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
                                            if (ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership) {
                                                if (return_type->isContainer()){
                                                    s << INDENT << "if (__qt_return_value != null";
                                                    if(!ownershipRule.condition.isEmpty()){
                                                        s << " && " << ownershipRule.condition.replace("%arg", "__qt_return_value");
                                                    }
                                                    s << ") {" << Qt::endl;
                                                    {
                                                        INDENTATION(INDENT);
                                                        writeOwnershipForContainer(s, java_function, ownershipRule.ownership, return_type, "__qt_return_value");
                                                    }
                                                    s << INDENT << "}" << Qt::endl;
                                                }else if(!ownershipRule.condition.isEmpty()){
                                                    s << INDENT << "if (" << ownershipRule.condition.replace("%arg", "__qt_return_value") << ") {" << Qt::endl;
                                                    {
                                                        INDENTATION(INDENT);
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
                for(MetaFunction *function : qAsConst(privatePureVirtualFunctions)){
                    QString sig = functionSignature(function, 0, MetaAttributes::Native | MetaAttributes::Abstract, Option(InFunctionComment | NoSuppressExports));
                    if(!privatePureVirtualFunctionSignatures.contains(sig))
                        privatePureVirtualFunctionSignatures << sig;
                }
                if(privatePureVirtualFunctionSignatures.size()==1)
                    commentStream << "<p>The following private function is pure virtual in Qt and thus has to " << Qt::endl
                      << "be implemented in derived Java classes by using the &commat;QtPrivateOverride annotation:</p>" << Qt::endl;
                else
                    commentStream << "<p>The following private functions are pure virtual in Qt and thus have to " << Qt::endl
                      << "be implemented in derived Java classes by using the &commat;QtPrivateOverride annotation:</p>" << Qt::endl;
                INDENTATIONRESET(INDENT);
                QString comment2;
                QTextStream commentStream2(&comment2);
                for(const QString& signature : qAsConst(privatePureVirtualFunctionSignatures)){
                    commentStream2 << Qt::endl
                                   << "@QtPrivateOverride" << Qt::endl
                                   << signature << " {...}" << Qt::endl;
                }
                commentStream2.seek(0);
                commentStream << "<code>" << Qt::endl;
                while(!commentStream2.atEnd()){
                    QString line = encodeHtml(commentStream2.readLine());
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

        m_currentMethodSignatures.swap(previousMethodSignatures);
        s << INDENT << "}" << Qt::endl;
    }

    if(nesting_level==0){
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
        m_currentPackages.removeDuplicates();
        m_packagesByTypeSystem[m_database->findTypeSystem(java_class->targetTypeSystem())].append(m_currentPackages);
    }

    s << lines;
}

void JavaGenerator::generate() {
    AbstractGenerator::generate();

    const auto keys = m_packagesByTypeSystem.keys();
    for(TypeSystemTypeEntry * typeSystemEntry : keys){
        if(typeSystemEntry){
            m_packagesByTypeSystem[typeSystemEntry].removeDuplicates();
            for(const QString& pkg : qAsConst(m_packagesByTypeSystem[typeSystemEntry])){
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
                        if(typeSystemEntry->module()!=relatedTypeSystem->module() && !relatedTypeSystem->qtLibrary().isEmpty()){
                            if(relatedTypeSystem->requiredTypeSystems().contains(typeSystemEntry))
                                ReportHandler::warning(QString("Cyclic dependency detected between modules %1 and %2.").arg(typeSystemEntry->module(), relatedTypeSystem->module()));
                            typeSystemEntry->addRequiredTypeSystem(relatedTypeSystem);
                        }
                    }
                }
            }
            if(!typeSystemEntry->module().isEmpty()){
                for(QString forwardDeclaration : typeSystemEntry->forwardDeclarations()){
                    BufferedOutputStream stream(QFileInfo(javaOutputDirectory() + "/" + typeSystemEntry->module() + "/" + forwardDeclaration + ".java"));
                    auto idx = forwardDeclaration.lastIndexOf('/');
                    QString pkg;
                    if(idx > 0){
                        pkg = forwardDeclaration.mid(0, idx);
                        forwardDeclaration = forwardDeclaration.mid(idx + 1);
                    }
                    stream << "package " << pkg.replace('/', '.') << ";" << Qt::endl
                           << "public class " << forwardDeclaration << "{}" << Qt::endl;
                }
            }
        }
    }

    auto writer = [](const QString& logOutputDirectory, const QString& fileName, QList<const MetaFunction *>& functions, const QString& header){
        QFile file(fileName);
        if (!logOutputDirectory.isNull())
            file.setFileName(QDir(logOutputDirectory).absoluteFilePath(fileName));
        if(functions.isEmpty()){
            file.remove();
        }else if (file.open(QFile::WriteOnly)) {
            QTextStream s(&file);

            s << header.arg(functions.size()) << Qt::endl;
            for(const MetaFunction *f : qAsConst(functions)) {
                s << f->implementingClass()->qualifiedCppName() << " :: " << f->minimalSignature();
                if(f->type()){
                    s << " -> ";
                    CppGenerator::writeTypeInfo(s, f->type(), NoOption);
                }
                s << Qt::endl;
            }

            functions.clear();
        }
    };

    writer(logOutputDirectory(), QStringLiteral("nativepointer_api.log"), m_nativepointer_functions, QStringLiteral("Number of public or protected functions with QNativePointer API: %1"));
    writer(logOutputDirectory(), QStringLiteral("resettable_object_functions.log"), m_resettable_object_functions, QStringLiteral("Number of public or protected functions that return a "
                                                                                                                                      "non-QObject pointer/reference, or that are virtual and take "
                                                                                                                                      "a non-QObject pointer/reference argument: %1"));
    writer(logOutputDirectory(), QStringLiteral("reference_count_candidates.log"), m_reference_count_candidate_functions, QStringLiteral("The following functions have a signature pattern which may imply that they need to apply reference counting to their arguments (%1 functions):"));
    writer(logOutputDirectory(), QStringLiteral("factory_functions.log"), m_factory_functions, QStringLiteral("The following functions have a signature pattern which may imply that they need to apply ownership to their return value and/or need to disable null pointers (%1 functions):"));
    writer(logOutputDirectory(), QStringLiteral("inconsistent_functions.log"), m_inconsistent_functions, QStringLiteral("The following functions are inconsistent (virtual but declared final in java) (%1 functions):"));
    writer(logOutputDirectory(), QStringLiteral("implicit_constructors.log"), m_implicit_constructors, QStringLiteral("The following constructors are implicit (%1 constructors):"));
    writer(logOutputDirectory(), QStringLiteral("functions_with_implicit_calls.log"), m_functions_with_implicit_calls, QStringLiteral("The following functions are implicitly called (%1 functions):"));
}

void JavaGenerator::writeFunctionAttributes(QTextStream &s, const MetaFunction *java_function, int arg_count, const QMap<int,Replacement>& replacedArguments,
        uint included_attributes, uint excluded_attributes,
        Option options) {
    uint attr = (java_function->attributes() & (~excluded_attributes)) | included_attributes;

    m_exportedPackages.clear();
    QString returnType;
    if ((options & SkipReturnType) == 0) {
        QString modified_type;
        if(java_function->type() && java_function->useArgumentAsBuffer(0)){
            QScopedPointer<MetaType> cpy(java_function->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(MetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            MetaBuilder::decideUsagePattern(cpy.get());
            modified_type = qualifiedJavaType(cpy.get(), java_function->implementingClass(), Option((options & ~UseNativeIds) | NoNullness | IsReturnType));
            if(modified_type==QStringLiteral(u"int")){
                modified_type = "java.nio.IntBuffer";
            }else if(modified_type==QStringLiteral(u"byte")){
                modified_type = "java.nio.ByteBuffer";
            }else if(modified_type==QStringLiteral(u"char")){
                modified_type = "java.nio.CharBuffer";
            }else if(modified_type==QStringLiteral(u"short")){
                modified_type = "java.nio.ShortBuffer";
            }else if(modified_type==QStringLiteral(u"long")){
                modified_type = "java.nio.LongBuffer";
            }else if(modified_type==QStringLiteral(u"float")){
                modified_type = "java.nio.FloatBuffer";
            }else if(modified_type==QStringLiteral(u"double")){
                modified_type = "java.nio.DoubleBuffer";
            }else{
                modified_type = "java.nio.Buffer";
            }
        }else if(java_function->type() && java_function->useArgumentAsArray(0)){
            QScopedPointer<MetaType> cpy(java_function->type()->copy());
            cpy->setConstant(false);
            cpy->setReferenceType(MetaType::NoReference);
            QList<bool> indirections = cpy->indirections();
            if(!indirections.isEmpty()){
                indirections.removeLast();
                cpy->setIndirections(indirections);
            }
            MetaBuilder::decideUsagePattern(cpy.get());
            modified_type = qualifiedJavaType(cpy.get(), java_function->implementingClass(), Option((options & ~UseNativeIds) | IsReturnType)).replace(u'$', u'.');
            modified_type += "[]";
        }else{
            modified_type = java_function->typeReplaced(0);
            if(modified_type.isEmpty() && java_function->isSelfReturningFunction()){
                Q_ASSERT(java_function->ownerClass());
                modified_type = java_function->ownerClass()->typeEntry()->targetLangName();
            }
        }
        if (modified_type.isEmpty()){
            if(!java_function->isFinal()){
                 options = Option(options | NoQCollectionContainers);
            }
            returnType = qualifiedJavaType(java_function->type(), java_function->implementingClass(), Option(options | VarArgsAsArray | IsReturnType));
        }else{
            modified_type.replace(u'$', u'.').replace(QStringLiteral(u"..."), QStringLiteral(u"[]"));
            registerPackage(modified_type);
            if (!(options & NoNullness) && modified_type!="void") {
                bool np = false;
                if((np = java_function->nullPointersDisabled(java_function->declaringClass(), 0))
                        || java_function->isSelfReturningFunction()){
                    QString ann = np ? QStringLiteral(u"@StrictNonNull") : QStringLiteral(u"@NonNull");
                    if(modified_type.endsWith(QStringLiteral(u"[]"))){
                        modified_type = modified_type.mid(0, modified_type.length()-2) + " " + ann + "[]";
                    }else {
                        auto idx = modified_type.indexOf(u'<');
                        QString package;
                        QString typeName = modified_type;
                        if(idx>0){
                            typeName = modified_type.mid(0, idx);
                        }
                        idx = typeName.lastIndexOf(u'.');
                        if(idx>0){
                            typeName = modified_type.mid(idx+1);
                            package = modified_type.mid(0, idx+1);
                            modified_type = package + ann + " " + typeName;
                        }else{
                            modified_type = ann + " " + modified_type;
                        }
                    }
                }else if(modified_type.startsWith("java.nio.") && !modified_type.contains("@")){
                    QString ann = QStringLiteral(u"@Nullable ");
                    auto idx = modified_type.indexOf(u'<');
                    QString package;
                    QString typeName = modified_type;
                    if(idx>0){
                        typeName = modified_type.mid(0, idx);
                    }
                    idx = typeName.lastIndexOf(u'.');
                    if(idx>0){
                        typeName = modified_type.mid(idx+1);
                        package = modified_type.mid(0, idx+1);
                        modified_type = package + ann + typeName;
                    }else{
                        modified_type = ann + modified_type;
                    }
                }else if(modified_type=="java.lang.String" || modified_type=="String"){
                    if(java_function->type()
                            && java_function->type()->typeEntry()->qualifiedCppName()==QStringLiteral(u"char")
                            && java_function->type()->indirections().size()==1
                            && !java_function->hasConversionRule(TS::Language::NativeCode, 0)){
                        auto idx = modified_type.indexOf(u'<');
                        QString package;
                        QString typeName = modified_type;
                        if(idx>0){
                            typeName = modified_type.mid(0, idx);
                        }
                        idx = typeName.lastIndexOf(u'.');
                        QString ann = QStringLiteral(u"@Nullable ");
                        if(idx>0){
                            typeName = modified_type.mid(idx+1);
                            package = modified_type.mid(0, idx+1);
                            modified_type = package + ann + typeName;
                        }else{
                            modified_type = ann + modified_type;
                        }
                    }
                }
            }
            returnType = modified_type;
        }
    }

    if ((attr & MetaAttributes::Public) || (attr & MetaAttributes::Protected)) {

        // Does the function use native pointer API?
        bool nativePointer = false;
        if(java_function->type()
                && java_function->type()->isNativePointer()
                && !java_function->type()->typeEntry()->isNativePointer()
                && !(java_function->type()->typeEntry()->isAlias() && reinterpret_cast<const AliasTypeEntry*>(java_function->type()->typeEntry())->getAsNativePointer())){
            if(java_function->typeReplaced(0).isEmpty()
                && java_function->argumentReplaced(0)!=QStringLiteral(u"this")
                && !java_function->useArgumentAsArray(0)
                && !java_function->useArgumentAsBuffer(0)
                && java_function->type()->typeEntry()->qualifiedCppName()!=QStringLiteral(u"QMetaObject")){
                nativePointer = true;
            }
        }

        if(!nativePointer
                && java_function->type()
                && java_function->type()->arrayElementType()
                && java_function->type()->arrayElementType()->isNativePointer()
                && !java_function->type()->arrayElementType()->typeEntry()->isNativePointer()
                && !(java_function->type()->arrayElementType()->typeEntry()->isAlias() && reinterpret_cast<const AliasTypeEntry*>(java_function->type()->arrayElementType()->typeEntry())->getAsNativePointer())){
            if(java_function->typeReplaced(0).isEmpty()
                && java_function->argumentReplaced(0)!=QStringLiteral(u"this")){
                nativePointer = true;
            }
        }

        // Does the function need to be considered for resetting the Java objects after use?
        bool resettableObject = false;
        if(java_function->type()
                && !java_function->isSelfReturningFunction()
                && !java_function->useArgumentAsArray(0)
            && !java_function->useArgumentAsBuffer(0)
                && java_function->typeReplaced(0).isEmpty()
                && java_function->ownership(java_function->declaringClass(),
                                            TS::ShellCode, 0).ownership==TS::InvalidOwnership
                && java_function->ownership(java_function->declaringClass(),
                                            TS::NativeCode, 0).ownership==TS::InvalidOwnership
                && java_function->ownership(java_function->declaringClass(),
                                            TS::TargetLangCode, 0).ownership==TS::InvalidOwnership
                && java_function->ownership(java_function->declaringClass(),
                                            TS::NoLanguage, 0).ownership==TS::InvalidOwnership
                && !java_function->resetObjectAfterUse(0)
                && !java_function->hasConversionRule(TS::ShellCode, 0)
                && !java_function->hasConversionRule(TS::NativeCode, 0)
                && !m_factory_functions.contains(java_function)
                && !java_function->type()->typeEntry()->isQMetaObjectType()
                && (
                       (java_function->type()->isObject()
                         && java_function->type()->getReferenceType()==MetaType::Reference
                         && (java_function->type()->typeEntry()->isObject() || !java_function->type()->isConstant()))
                    || (java_function->type()->isValue()
                         && !java_function->type()->isConstant()
                         && java_function->type()->getReferenceType()==MetaType::Reference)
                    || (java_function->type()->typeEntry()->isComplex()
                         && !java_function->type()->isConstant()
                         && java_function->type()->getReferenceType()==MetaType::Reference)
                    || (java_function->type()->typeEntry()->isContainer()
                         && !java_function->type()->isConstant()
                         && java_function->type()->getReferenceType()==MetaType::Reference)
                    || (java_function->type()->isQVariant()
                         && !java_function->type()->isConstant()
                         && java_function->type()->actualIndirections()>0)
                    || (java_function->type()->isJavaString()
                         && !java_function->type()->isConstant()
                         && java_function->type()->actualIndirections()>0)
                    )){
            resettableObject = true;
        }

        if (!nativePointer && java_function->type() && !java_function->type()->isSmartPointer()) {
            for(const MetaType *type : java_function->type()->instantiations()) {
                if (type
                        && type->isNativePointer()
                        && !type->typeEntry()->isNativePointer()
                        && !(type->typeEntry()->isAlias() && reinterpret_cast<const AliasTypeEntry*>(type->typeEntry())->getAsNativePointer())) {
                    nativePointer = true;
                    break;
                }
            }
        }

        const MetaArgumentList& arguments = java_function->arguments();
        if (!nativePointer || (!resettableObject && !java_function->isFinal())) {
            for(const MetaArgument *argument : arguments) {
                if (java_function->argumentRemoved(argument->argumentIndex() + 1)==ArgumentRemove_No
                        && java_function->typeReplaced(argument->argumentIndex() + 1).isEmpty()
                        && !java_function->useArgumentAsArray(argument->argumentIndex() + 1)
                        && !java_function->useArgumentAsBuffer(argument->argumentIndex() + 1)
                        && !java_function->useArgumentAsSlot(argument->argumentIndex() + 1)
                        && !java_function->useArgumentAsSlotContext(argument->argumentIndex() + 1)) {

                    if (argument->type()->isNativePointer()) {
                        if(!argument->type()->typeEntry()->isNativePointer()
                                && !(argument->type()->typeEntry()->isAlias() && reinterpret_cast<const AliasTypeEntry*>(argument->type()->typeEntry())->getAsNativePointer())){
                            nativePointer = true;
                            if (resettableObject) break ;
                        }

                    } else if (argument->type()->arrayElementType() && argument->type()->arrayElementType()->isNativePointer()) {
                        if(!argument->type()->arrayElementType()->typeEntry()->isNativePointer()
                                && !(argument->type()->arrayElementType()->typeEntry()->isAlias() && reinterpret_cast<const AliasTypeEntry*>(argument->type()->arrayElementType()->typeEntry())->getAsNativePointer())){
                            nativePointer = true;
                            if (resettableObject) break ;
                        }

                    } else if (!java_function->isFinalInTargetLang()
                               && ((argument->type()->isObject() && (argument->type()->typeEntry()->isObject() || !argument->type()->isConstant()))
                                   || (argument->type()->isValue()
                                        && !argument->type()->isConstant()
                                        && argument->type()->actualIndirections()>0)
                                   || (argument->type()->isContainer()
                                        && !argument->type()->isConstant()
                                        && argument->type()->actualIndirections()>0)
                                   || (argument->type()->isQVariant()
                                        && !argument->type()->isConstant()
                                        && argument->type()->actualIndirections()>0)
                                   || (argument->type()->isJavaString()
                                        && !argument->type()->isConstant()
                                        && argument->type()->actualIndirections()>0)
                                   || argument->type()->isQSpan()
                                   )
                               && !argument->type()->isQObject()
                               && !java_function->resetObjectAfterUse(argument->argumentIndex() + 1)
                               && !java_function->hasConversionRule(TS::NativeCode, 0)
                               && !java_function->hasConversionRule(TS::ShellCode, 0)
                               && java_function->ownership(java_function->declaringClass(),
                                        TS::ShellCode, argument->argumentIndex() + 1).ownership == TS::InvalidOwnership
                               && java_function->ownership(java_function->declaringClass(),
                                        TS::NativeCode, argument->argumentIndex() + 1).ownership == TS::InvalidOwnership
                               && java_function->ownership(java_function->declaringClass(),
                                        TS::TargetLangCode, argument->argumentIndex() + 1).ownership == TS::InvalidOwnership
                               && java_function->ownership(java_function->declaringClass(),
                                        TS::NoLanguage, argument->argumentIndex() + 1).ownership == TS::InvalidOwnership) {
                        resettableObject = true;
                        if (nativePointer) break ;

                    } else if (argument->type()->hasInstantiations()) {

                        for(const MetaType *type : argument->type()->instantiations()) {
                            if (type && type->isNativePointer()) {
                                if(!argument->type()->isSmartPointer()
                                        && !type->typeEntry()->isNativePointer()
                                        && !(type->typeEntry()->isAlias() && reinterpret_cast<const AliasTypeEntry*>(type->typeEntry())->getAsNativePointer())){
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
                && (java_function->ownerClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)==TypeEntry::GenerateTargetLang
                && (java_function->ownerClass() == java_function->declaringClass() || java_function->isFinal()))
            m_nativepointer_functions.append(java_function);
        if (resettableObject
                && !java_function->ownerClass()->isFake()
                && (java_function->ownerClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)==TypeEntry::GenerateTargetLang
                && !m_resettable_object_functions.contains(java_function)
                && (java_function->ownerClass() == java_function->declaringClass() || java_function->isFinal()))
            m_resettable_object_functions.append(java_function);
    }

    if ((options & SkipAttributes) == 0) {
        if (java_function->isEmptyFunction()
                || java_function->isDeprecated()) s << INDENT << "@Deprecated" << Qt::endl;

        bool needsSuppressUnusedWarning = m_database->includeEclipseWarnings()
                                          && java_function->isSignal()
                                          && ((((excluded_attributes & MetaAttributes::Private) == 0)
                                              && (java_function->isPrivate()
                                                  || (included_attributes & MetaAttributes::Private) != 0 || java_function->isPrivateSignal())));

        bool needsSuppressExportsWarning = false;

        if(java_function->isPublic()
                && (included_attributes & MetaAttributes::Private)==0
                && (options & NoSuppressExports)!=NoSuppressExports){
            QString strg;
            QTextStream s(&strg);
            writeFunctionArguments(s, java_function, {}, arg_count, options);
            for(const QString& pkg : qAsConst(m_exportedPackages)){
                if(pkg!=java_function->implementingClass()->package()
                        && !pkg.isEmpty()
                        && m_typeSystemByPackage[pkg]
                        && m_typeSystemByPackage[pkg]->module()!=QStringLiteral(u"qtjambi") // because it's transitive
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
            s << "    @QtPropertyNotify(name=\"" << spec->name() << "\")" << Qt::endl;
        }
        if (!(options & VarArgsAsArray) && java_function->arguments().size()>0) {
            auto lastArg = arg_count>0 && arg_count<=java_function->arguments().size() ? java_function->arguments()[arg_count-1] : (java_function->arguments().isEmpty() || arg_count==0 ? nullptr : java_function->arguments().last());
            if(lastArg){
                QString typeReplaced = java_function->typeReplaced(lastArg->argumentIndex()+1);
                if(typeReplaced.isEmpty()
                    && (
                        (
                           (lastArg->type()->typeEntry()->isInitializerList()
                        ||  (lastArg->type()->typeEntry()->isQSpan() && replacedArguments[lastArg->argumentIndex()].typeName.endsWith(u"..."))
                            )
                        && lastArg->type()->instantiations().size()>0
                        && lastArg->type()->instantiations().first()->instantiations().size()>0
                        )
                        ||
                        (
                           !lastArg->type()->typeEntry()->isInitializerList()
                        && !lastArg->type()->typeEntry()->isQSpan()
                        && lastArg->type()->instantiations().size()>0
                        && java_function->useArgumentAsVarArgs(lastArg->argumentIndex()+1)
                        )
                       )
                ){
                    s << INDENT << "@SafeVarargs" << Qt::endl;
                }else if(!typeReplaced.isEmpty()
                           && typeReplaced!=QStringLiteral(u"java.lang.Object...")
                            && typeReplaced.endsWith(QStringLiteral(u"..."))){
                    s << INDENT << "@SafeVarargs" << Qt::endl;
                }
            }
        }

        if(!java_function->isConstructor()

            && (!java_function->isInGlobalScope() || (java_function->name()!=QStringLiteral(u"readFrom")
                                                      && java_function->name()!=QStringLiteral(u"writeTo")) )
                && !java_function->isSignal()
                && !java_function->isStatic()
                && java_function->isFinalInCpp() && (excluded_attributes & MetaAttributes::FinalInTargetLang)==MetaAttributes::FinalInTargetLang){
            s << INDENT << "@QtDeclaredFinal" << Qt::endl;
        }

        if (!(attr & NoBlockedSlot)
                && !java_function->isAllowedAsSlot()
                && !java_function->isInvokable()
                && !java_function->isConstructor()

                && !java_function->isSlot()
                && !java_function->isSignal()
                && !java_function->isStatic()
                && !(included_attributes & MetaAttributes::Static))
            s << INDENT << "@QtUninvokable" << Qt::endl;
        if (attr & MetaAttributes::Override)
            s << INDENT << "@Override" << Qt::endl;
        s << INDENT;
        if (attr & MetaAttributes::Static && java_function->ownerClass()->typeEntry()->designatedInterface()) s << "private ";
        else if (attr & MetaAttributes::Public) s << "public ";
        else if (attr & MetaAttributes::Protected) s << "protected ";
        else if (attr & MetaAttributes::Private) s << "private ";

        bool isStatic = (attr & MetaAttributes::Static);
        bool isDefault = !isStatic && (options & DefaultFunction);

        if (!isDefault && attr & MetaAttributes::Native) s << "native ";
        else if (!isDefault && !isStatic && (attr & MetaAttributes::FinalInTargetLang)) s << "final ";
        else if (!isDefault && !isStatic && (attr & MetaAttributes::Abstract)) s << "abstract ";

        if (isStatic) s << "static ";

        if (isDefault) s << "default ";

        if (!(options & SkipTemplateParameters)){
            QList<QString> templateArguments;
            if(isStatic){
                if(java_function->type() && java_function->type()->typeEntry()->isTemplateArgument()
                        && !templateArguments.contains(java_function->type()->typeEntry()->qualifiedCppName())){
                    templateArguments << java_function->type()->typeEntry()->qualifiedCppName();
                }
                for(const MetaArgument* arg : java_function->arguments()){
                    if(arg->type()->typeEntry()->isTemplateArgument()
                            && !templateArguments.contains(arg->type()->typeEntry()->qualifiedCppName())){
                        templateArguments << arg->type()->typeEntry()->qualifiedCppName();
                    }
                }
            }
            {
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
    }

    if ((options & SkipReturnType) == 0) {
        s << returnType << " ";
    }
}

void JavaGenerator::writeConstructorContents(QTextStream &s, const MetaFunction *java_function) {
    // Write constructor
    s << "{" << Qt::endl;
    {
        INDENTATION(INDENT);
        s << INDENT << "super((QPrivateConstructor)null);" << Qt::endl;

        writeJavaCallThroughContents(s, java_function);

        // Write out expense checks if present...
        const MetaClass *java_class = java_function->implementingClass();
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
            if (snip.language == TS::Constructors) {
                lines << snip.code().split("\n");
            }
        }
        printExtraCode(lines, s);
    }
    s << INDENT << "}" << Qt::endl
      << INDENT << Qt::endl;

    // Write native constructor
    writePrivateNativeFunction(s, java_function);
}

void JavaGenerator::writeFunctionArguments(QTextStream &s, const MetaFunction *java_function,
        const QMap<int,Replacement>& replacedArguments, int argument_count, Option options) {
    bool commaRequired = false;
    const QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments = java_function->addedArguments();
    const MetaArgumentList& arguments = java_function->arguments();
    int argumentCounter = 1;
    while(addedArguments.first.contains(argumentCounter)){
        const ArgumentModification& mod = addedArguments.first[argumentCounter];
        if(commaRequired)
            s << ", ";
        commaRequired = true;
        QString argType = QString(mod.modified_type).replace(u'$', u'.');
        if((options & SkipTemplateParameters) && argType.contains("<")){
            bool isVarArg = argType.endsWith(QStringLiteral(u"..."));
            argType = argType.chopped(3);
            int arrayCount = 0;
            while(argType.endsWith(QStringLiteral(u"[]"))){
                ++arrayCount;
                argType = argType.chopped(2);
            }
            auto idx = argType.indexOf(u'<');
            argType = argType.mid(0, idx);
            for(;arrayCount>0; --arrayCount){
                argType += "[]";
            }
            if(isVarArg)
                argType += "...";
        }
        if((options & VarArgsAsArray) && argType.endsWith(QStringLiteral(u"..."))){
            argType = argType.replace(QStringLiteral(u"..."), QStringLiteral(u"[]"));
        }
        if((options & NoNullness) && argType.contains(u'@')){
            argType = annotationFreeTypeName(argType);
        }
        s << argType;
        if ((options & SkipName) == 0) {
            s << " " << mod.modified_name;
        }
        ++argumentCounter;
    }
    if (argument_count == -1)
        argument_count = int(arguments.size());
    for (int i = 0; i < argument_count; ++i) {
        const MetaArgument *arg = arguments.at(i);
        if (java_function->argumentRemoved(arg->argumentIndex() + 1)==ArgumentRemove_No) {
            const MetaArgument *nextArg = i+1 < argument_count ? arguments.at(i+1) : nullptr;
            while(nextArg && java_function->argumentRemoved(nextArg->argumentIndex() + 1)!=ArgumentRemove_No)
                nextArg = nextArg->argumentIndex()+1 < argument_count ? MetaFunction::argumentByIndex(arguments, nextArg->argumentIndex() + 1) : nullptr;
            Option myOptions = Option(options | CollectionAsCollection);
            if(nextArg || !addedArguments.second.isEmpty()){
                myOptions = Option(options | VarArgsAsArray | CollectionAsCollection);
            }
            writeFunctionArgument(s, java_function, arg, commaRequired, &replacedArguments, myOptions);
        }
        ++argumentCounter;
        while(addedArguments.first.contains(argumentCounter)){
            const ArgumentModification& mod = addedArguments.first[argumentCounter];
            if(commaRequired)
                s << ", ";
            commaRequired = true;
            QString argType = QString(mod.modified_type).replace(u'$', u'.');
            if((options & SkipTemplateParameters) && argType.contains("<")){
                bool isVarArg = argType.endsWith(QStringLiteral(u"..."));
                argType = argType.chopped(3);
                int arrayCount = 0;
                while(argType.endsWith(QStringLiteral(u"[]"))){
                    ++arrayCount;
                    argType = argType.chopped(2);
                }
                auto idx = argType.indexOf(u'<');
                argType = argType.mid(0, idx);
                for(;arrayCount>0; --arrayCount){
                    argType += "[]";
                }
                if(isVarArg)
                    argType += "...";
            }
            if((options & VarArgsAsArray) && argType.endsWith(QStringLiteral(u"..."))){
                argType = argType.replace(QStringLiteral(u"..."), QStringLiteral(u"[]"));
            }
            if((options & NoNullness) && argType.contains(u'@')){
                argType = annotationFreeTypeName(argType);
            }
            s << argType;
            if ((options & SkipName) == 0) {
                s << " " << mod.modified_name;
            }
            ++argumentCounter;
        }
    }
    for(const ArgumentModification& argumentMod : addedArguments.second){
        if(commaRequired)
            s << ", ";
        commaRequired = true;
        QString argType = QString(argumentMod.modified_type).replace(u'$', u'.');
        if((options & SkipTemplateParameters) && argType.contains("<")){
            bool isVarArg = argType.endsWith(QStringLiteral(u"..."));
            argType = argType.chopped(3);
            int arrayCount = 0;
            while(argType.endsWith(QStringLiteral(u"[]"))){
                ++arrayCount;
                argType = argType.chopped(2);
            }
            auto idx = argType.indexOf(u'<');
            argType = argType.mid(0, idx);
            for(;arrayCount>0; --arrayCount){
                argType += "[]";
            }
            if(isVarArg)
                argType += "...";
        }
        if((options & VarArgsAsArray) && argType.endsWith(QStringLiteral(u"..."))){
            argType = argType.replace(QStringLiteral(u"..."), QStringLiteral(u"[]"));
        }
        if((options & NoNullness) && argType.contains(u'@')){
            argType = annotationFreeTypeName(argType);
        }
        s << argType;
        if ((options & SkipName) == 0) {
            s << " " << argumentMod.modified_name;
        }
    }
}


void JavaGenerator::writeExtraFunctions(QTextStream &s, const MetaClass *java_class, bool isConcreteWrapper) {
    const ComplexTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    QStringList lines;
    for(const CodeSnip &snip : class_type->codeSnips()) {
        if((snip.position==CodeSnip::Beginning
            || snip.position==CodeSnip::Position1
            || snip.position==CodeSnip::Position2
            || snip.position==CodeSnip::Position3
            || snip.position==CodeSnip::Position4
            || snip.position==CodeSnip::Position5
            || snip.position==CodeSnip::End)
            && ((isConcreteWrapper && snip.language == TS::ConctreteWrapper)
                 || (!isConcreteWrapper && !java_class->isInterface() && snip.language == TS::TargetLangCode)
                     || (!isConcreteWrapper && java_class->isInterface() && snip.language == TS::Interface))) {
            lines << snip.code().split("\n");
        }
    }
    if(!lines.isEmpty())
        printExtraCode(lines, s);
}

void JavaGenerator::writeExtraFunctions(QTextStream &s, const MetaFunctional *java_class, bool inInterface) {
    const FunctionalTypeEntry *class_type = java_class->typeEntry();
    Q_ASSERT(class_type);

    QStringList lines;
    for(const CodeSnip &snip : class_type->codeSnips()) {
        if((snip.position==CodeSnip::Beginning
            || snip.position==CodeSnip::Position1
            || snip.position==CodeSnip::Position2
            || snip.position==CodeSnip::Position3
            || snip.position==CodeSnip::Position4
            || snip.position==CodeSnip::Position5
            || snip.position==CodeSnip::End)
            && ((!inInterface && snip.language == TS::TargetLangCode)
                            || (inInterface && snip.language == TS::Interface))) {
            lines << snip.code().split("\n");
        }
    }
    if(!lines.isEmpty()){
        printExtraCode(lines, s);
        s << Qt::endl;
    }
}

void JavaGenerator::writeToStringFunction(QTextStream &s, const MetaClass *java_class) {
    bool generate = java_class->toStringCapability() && !java_class->hasDefaultToStringFunction();
    bool isQEvent = java_class->isQEvent() && java_class->qualifiedCppName()=="QEvent";

    if (generate || isQEvent) {
        QStringList lines;
        for(const CodeSnip& codeSnip : java_class->typeEntry()->codeSnips()){
            if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::ToString){
                lines << codeSnip.code().split("\n");
            }
        }
        QString nonNull = m_nullness ? QStringLiteral(u"@NonNull ") : QString{};
        if (isQEvent) {
            s << Qt::endl
              << INDENT << "/**" << Qt::endl
              << INDENT << " * Returns the string representation of the <code>QEvent</code>." << Qt::endl
              << INDENT << " */" << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "public " << nonNull << "String toString() {" << Qt::endl;
            if(!lines.isEmpty()){
                printExtraCode(lines, s);
            }
            s << INDENT << "    return \"QEvent(type=\" + type().name() + \")\";" << Qt::endl
              << INDENT << "}" << Qt::endl;
        } else {
            s << Qt::endl
              << INDENT << "/**" << Qt::endl;
            if(java_class->toStringCapability()){
                s << INDENT << " * <p>See <code>";
                if(!java_class->toStringCapability()->href().isEmpty()){
                    s << "<a href=\"" << docsUrl << java_class->toStringCapability()->href() << "\">";
                }
                if(java_class->toStringCapability()->functionType()!=MetaFunction::GlobalScopeFunction){
                    s << encodeHtml(java_class->qualifiedCppName()
                                            .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                                            .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                                            .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
                      << "::<wbr/>";
                }
                s << encodeHtml(java_class->toStringCapability()->originalSignature())
                         .replace(QStringLiteral(u","), QStringLiteral(u",<wbr/>"));
                if(!java_class->toStringCapability()->href().isEmpty())
                    s << "</a>";
                s << "</code></p>" << Qt::endl;
            }else{
                s << INDENT << " * Returns the string representation of the object given by <code>operator&lt;&lt;(QDebug, " << java_class->qualifiedCppName() << ")</code>." << Qt::endl;
            }
            s << INDENT << " */" << Qt::endl
              << INDENT << "@QtUninvokable" << Qt::endl
              << INDENT << "@Override" << Qt::endl
              << INDENT << "public ";
            bool isNative = lines.isEmpty() && !java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface();
            if(isNative)
                s << "native ";
            s << nonNull << "String toString()";
            if(!isNative){
                s << " {" << Qt::endl;
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
                  << INDENT << "@QtUninvokable" << Qt::endl
                  << INDENT << "private ";
                if(java_class->typeEntry()->isNativeIdBased() || java_class->typeEntry()->designatedInterface())
                    s << "static ";
                s << "native String toString_native(";
                if(java_class->typeEntry()->isNativeIdBased()){
                    s << "long __this_nativeId";
                }else if(java_class->typeEntry()->designatedInterface()){
                    s << java_class->simpleName() << " _this";
                }
                s << ")";
            }
            s << ";" << Qt::endl;
        }
    }
}

void JavaGenerator::writeCloneFunction(QTextStream &s, const MetaClass *java_class) {
    s << INDENT << Qt::endl
      << INDENT << "/**" << Qt::endl
      << INDENT << " * <p>Creates and returns a copy of this object.</p>" << Qt::endl;
    if(MetaFunction* f = java_class->publicCopyConstructor()){
        s << INDENT << "<p>See <code>";
        if(!f->href().isEmpty()){
            s << "<a href=\"" << docsUrl << f->href() << "\">";
        }
        s << encodeHtml(java_class->qualifiedCppName()
                 .replace(QStringLiteral(u"<JObjectWrapper>"), QString())
                 .replace(QStringLiteral(u"QtJambi"), QStringLiteral(u"Q"))
                 .replace(QStringLiteral(u"QVoid"), QStringLiteral(u"Q")))
          << "::<wbr/>";
        s << encodeHtml(f->originalSignature());
        if(!f->href().isEmpty())
            s << "</a>";
        s << "</code></p>" << Qt::endl;
    }
    s << INDENT << " */" << Qt::endl
      << INDENT << "@QtUninvokable" << Qt::endl
      << INDENT << "@Override" << Qt::endl
      << INDENT << "public ";
    QStringList lines;
    for(const CodeSnip& codeSnip : java_class->typeEntry()->codeSnips()){
        if(codeSnip.language==TS::TargetLangCode && codeSnip.position==CodeSnip::Clone){
            lines << codeSnip.code().split("\n");
        }
    }
    if(!java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface() && lines.isEmpty()){
        s << "native ";
    }
    s << java_class->simpleName();
    if(java_class->typeEntry()->isGenericClass()){
        s << "<";
        if(java_class->templateBaseClass()){
            QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
            if(templateArguments.size()>0){
                for (int i = 0; i < templateArguments.size(); ++i) {
                    if (i > 0)
                        s << ",";
                    s << QString(templateArguments.at(i)->name()).replace(u'$', u'.');
                }
            }
        }else{
            s << "T";
        }
        s << ">";
    }
    if(!java_class->typeEntry()->isNativeIdBased() && !java_class->typeEntry()->designatedInterface() && lines.isEmpty()){
        s << " clone();" << Qt::endl;
    }else{
        QString name = java_class->name();
        if(java_class->typeEntry() && java_class->typeEntry()->designatedInterface()){
            name = java_class->typeEntry()->designatedInterface()->targetLangName();
        }
        s << " clone() {" << Qt::endl;
        if(!lines.isEmpty()){
            INDENTATION(INDENT);
            s << INDENT << java_class->simpleName() << " clone = clone_native(";
            if(java_class->typeEntry()->isNativeIdBased()){
                s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
            }else{
                s << "this";
            }
            s << ");" << Qt::endl;
            printExtraCode(lines, s);
            s << INDENT << "return clone;" << Qt::endl;
        }else{
            INDENTATION(INDENT);
            s << INDENT << "return clone_native(";
            if(java_class->typeEntry()->isNativeIdBased()){
                s << "QtJambi_LibraryUtilities.internal.nativeId(this)";
            }else{
                s << "this";
            }
            s << ");" << Qt::endl;
        }
        s << INDENT << "}" << Qt::endl
          << INDENT << "private static native ";
        if(java_class->typeEntry()->isGenericClass()){
            s << "<";
            if(java_class->templateBaseClass()){
                QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
                if(templateArguments.size()>0){
                    for (int i = 0; i < templateArguments.size(); ++i) {
                        if (i > 0)
                            s << ",";
                        s << QString(templateArguments.at(i)->name()).replace(u'$', u'.');
                    }
                }
            }else{
                s << "T";
            }
            s << "> ";
        }
        s << java_class->simpleName();
        if(java_class->typeEntry()->isGenericClass()){
            s << "<";
            if(java_class->templateBaseClass()){
                QList<TypeEntry *> templateArguments = java_class->templateBaseClass()->templateArguments();
                if(templateArguments.size()>0){
                    for (int i = 0; i < templateArguments.size(); ++i) {
                        if (i > 0)
                            s << ",";
                        s << QString(templateArguments.at(i)->name()).replace(u'$', u'.');
                    }
                }
            }else{
                s << "T";
            }
            s << ">";
        }
        s << " clone_native(";
        if(java_class->typeEntry()->isNativeIdBased()){
            s << "long __this_nativeId";
        }else{
            s << java_class->simpleName() << " __this";
        }
        s << ");" << Qt::endl;
    }
    if(!java_class->publicCopyConstructor()
        && !java_class->typeEntry()->hasProtectedCopyConstructor()
        && !java_class->typeEntry()->hasPrivateCopyConstructor()
        && !java_class->typeEntry()->isContainer()){
        ReportHandler::warning(QStringLiteral("Cloneable class %1 is missing an explicit copy constructor").arg(java_class->qualifiedCppName()));
    }
}

void JavaGenerator::generateFake(const MetaClass *fake_class) {
    for(MetaFunctional* functional : fake_class->functionals()){
        if(functional->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
            QString fileName = QString("%1.java").arg(functional->name());
            ReportHandler::debugSparse(QString("generating: %1").arg(fileName));

            BufferedOutputStream stream(QFileInfo(resolveOutputDirectory() + "/" + subDirectoryForClass(fake_class) + subDirectoryForPackage(functional->package()) + "/" + fileName));
            write(stream, functional);

            if (stream.finish())
                ++m_num_generated_written;
            ++m_num_generated;
        }
    }
    for(const MetaEnum * enm : fake_class->enums()){
        if(enm->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang){
            {
                QString fileName = QString("%1.java").arg(enm->name());
                ReportHandler::debugSparse(QString("generating: %1").arg(fileName));
                BufferedOutputStream stream(QFileInfo(resolveOutputDirectory() + "/" + subDirectoryForClass(fake_class) + subDirectoryForPackage(enm->package()) + "/" + fileName));
                write(stream, enm);
                if (stream.finish())
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
                QStringList linesPos5;
                QStringList linesBegin;
                QStringList linesEnd;
                for(const CodeSnip &snip : enm->typeEntry()->codeSnips()) {
                    if (snip.language == TS::TargetLangCode) {
                        if (snip.position == CodeSnip::Position1) {
                            linesPos1 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position2) {
                            linesPos2 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position3) {
                            linesPos3 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position4) {
                            linesPos4 << snip.code().split("\n");
                        }else if (snip.position == CodeSnip::Position5) {
                            linesPos5 << snip.code().split("\n");
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
                BufferedOutputStream s(QFileInfo(resolveOutputDirectory() + "/" + subDirectoryForClass(fake_class) + subDirectoryForPackage(enm->package()) + "/" + fileName));
                s << INDENT << "package " << enm->package() << ";" << Qt::endl
                  << Qt::endl
                  << "import io.qt.*;" << Qt::endl
                  << Qt::endl;
                QString comment;
                {
                    QTextStream commentStream(&comment);
                    Indentor indentor;
                    for(const CodeSnip& snip : enm->typeEntry()->codeSnips()){
                        if(snip.language==TS::TargetLangCode && snip.position==CodeSnip::Comment){
                            snip.formattedCode(commentStream, indentor);
                        }
                    }
                }
                bool is64 = enm->typeEntry()->size()==64 && QT_VERSION_CHECK(m_qtVersionMajor,m_qtVersionMinor,m_qtVersionPatch)>=QT_VERSION_CHECK(6,9,0);
                const char* valueMethod = is64 ? "longValue" : "value";
                const char* valueType = is64 ? "long" : "int";
                const char* valueBoxedType = is64 ? "Long" : "Integer";
                const char* javaType = is64 ? "QLongFlags" : "QFlags";
                if(!comment.trimmed().isEmpty()){
                    s << INDENT << "/**" << Qt::endl;
                    QTextStream commentStream(&comment, QIODevice::ReadOnly);
                    while(!commentStream.atEnd()){
                        s << INDENT << " * " << commentStream.readLine() << Qt::endl;
                    }
                    s << INDENT << " */" << Qt::endl;
                }else{
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * {@link " << javaType << "} type for enum {@link " << enm->name().replace("$",".") << "}" << Qt::endl
                      << INDENT << " */" << Qt::endl;
                }

                s << INDENT << "public final class " << flagsName << " extends " << javaType << "<@NonNull " << enm->name().replace("$",".") << "> implements Comparable<@NonNull " << flagsName << "> {" << Qt::endl
                  << INDENT << "    private static final long serialVersionUID = 0x" << QString::number(serialVersionUID, 16) << "L;" << Qt::endl;
                printExtraCode(linesPos1, s, true);
                s << INDENT << "    static {" << Qt::endl
                  << INDENT << "        QtJambi_LibraryUtilities.initialize();" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Creates a new " << flagsName << "." << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public " << flagsName << "(){" << Qt::endl
                  << INDENT << "        this(0);" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Creates a new " << flagsName << " where the flags in <code>args</code> are set." << Qt::endl
                  << INDENT << "     * @param args enum entries" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public " << flagsName << "(@Nullable " << enm->name().replace("$",".") << " @NonNull... args){" << Qt::endl
                  << INDENT << "        this(0);" << Qt::endl
                  << INDENT << "        set(args);" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Creates a new " << flagsName << " with given <code>value</code>." << Qt::endl
                  << INDENT << "     * @param value" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public " << flagsName << "(" << valueType << " value) {" << Qt::endl
                  << INDENT << "        super(value);" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Combines this flags with enum entry." << Qt::endl
                  << INDENT << "     * @param e enum entry" << Qt::endl
                  << INDENT << "     * @return new " << flagsName << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final @NonNull " << flagsName << " combined(@StrictNonNull " << enm->name().replace("$",".") << " e){" << Qt::endl
                  << INDENT << "        return new " << flagsName << "(" << valueMethod << "() | e.value());" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Sets the flag <code>e</code>" << Qt::endl
                  << INDENT << "     * @param e enum entry" << Qt::endl
                  << INDENT << "     * @return this" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public final @NonNull " << flagsName << " setFlag(@Nullable " << enm->name().replace("$",".") << " e){" << Qt::endl
                  << INDENT << "        return setFlag(e, true);" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Sets or clears the flag <code>flag</code>" << Qt::endl
                  << INDENT << "     * @param e enum entry" << Qt::endl
                  << INDENT << "     * @param on set (true) or clear (false)" << Qt::endl
                  << INDENT << "     * @return this" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    public final @NonNull " << flagsName << " setFlag(@Nullable " << enm->name().replace("$",".") << " e, boolean on){" << Qt::endl
                  << INDENT << "        if (e!=null) {" << Qt::endl
                  << INDENT << "            if (on) {" << Qt::endl
                  << INDENT << "            	setValue(" << valueMethod << "() | e.value());" << Qt::endl
                  << INDENT << "            }else {" << Qt::endl
                  << INDENT << "        	    setValue(" << valueMethod << "() & ~e.value());" << Qt::endl
                  << INDENT << "            }" << Qt::endl
                  << INDENT << "        }" << Qt::endl
                  << INDENT << "        return this;" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Returns an array of flag objects represented by this " << flagsName << "." << Qt::endl
                  << INDENT << "     * @return array of enum entries" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final @NonNull " << enm->name().replace("$",".") << " @NonNull[] flags(){" << Qt::endl
                  << INDENT << "        return super.flags(" << enm->name().replace("$",".") << ".values());" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final @NonNull " << flagsName << " clone(){" << Qt::endl
                  << INDENT << "        return new " << flagsName << "(" << valueMethod << "());" << Qt::endl
                  << INDENT << "    }" << Qt::endl << Qt::endl
                  << INDENT << "    /**" << Qt::endl
                  << INDENT << "     * Compares this flag with the specified flag for order." << Qt::endl
                  << INDENT << "     * {@inheritDoc}" << Qt::endl
                  << INDENT << "     */" << Qt::endl
                  << INDENT << "    @Override" << Qt::endl
                  << INDENT << "    public final int compareTo(@StrictNonNull " << flagsName << " other){" << Qt::endl
                  << INDENT << "        return " << valueBoxedType << ".compare(" << valueMethod << "(), other." << valueMethod << "());" << Qt::endl
                  << INDENT << "    }" << Qt::endl
                  << INDENT << "/**" << Qt::endl
                  << INDENT << " * Returns the value of this QFlags." << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final " << valueType << " value(){" << Qt::endl;
                if(is64){
                    s << INDENT << "    return longValue();" << Qt::endl;
                }else{
                    s << INDENT << "    return intValue();" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl << Qt::endl;
                if(is64){
                    s << INDENT << "/**" << Qt::endl
                      << INDENT << " * See <a href=\"https://doc.qt.io/qt/qflags.html#toInt\">QFlags::toInt() const</a>" << Qt::endl
                      << INDENT << " */" << Qt::endl
                      << INDENT << "public final " << valueType << " toLong(){" << Qt::endl
                      << INDENT << "    return longValue();" << Qt::endl
                      << INDENT << "}" << Qt::endl << Qt::endl;
                }
                s << INDENT << "/**" << Qt::endl
                  << INDENT << " * Sets the value of this QFlags." << Qt::endl
                  << INDENT << " * @param value new value" << Qt::endl
                  << INDENT << " */" << Qt::endl
                  << INDENT << "public final void setValue(" << valueType << " value){" << Qt::endl;
                if(is64){
                    s << INDENT << "    setLongValue(value);" << Qt::endl;
                }else{
                    s << INDENT << "    setIntValue(value);" << Qt::endl;
                }
                s << INDENT << "}" << Qt::endl;
                printExtraCode(linesPos4, s, true);
                printExtraCode(linesPos5, s, true);
                s << INDENT << "}" << Qt::endl
                  << INDENT << Qt::endl;
                if (s.finish())
                    ++m_num_generated_written;
                ++m_num_generated;
            }
        }
    }
}

void JavaGenerator::write(QTextStream &s, const MetaFunctional *global_fun, int){
    ReportHandler::debugSparse("Generating functional: " + global_fun->fullName());
    s << INDENT << "package " << global_fun->package() << ";" << Qt::endl << Qt::endl;

    QSet<QString> imports;
    imports.insert(QStringLiteral(u"io.qt.*"));
    s << "import io.qt.*;" << Qt::endl;

    const QList<Include>& includes = global_fun->typeEntry()->extraIncludes();
    for(const Include &inc : includes) {
        if (inc.type == Include::TargetLangImport) {
            QString imp = inc.toString();
            if(!imports.contains(imp)){
                imports.insert(imp);
                s << imp << Qt::endl;
            }
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
}

void JavaGenerator::write(QTextStream &s, const MetaEnum *global_enum) {
    if((global_enum->typeEntry()->codeGeneration() & ~TypeEntry::InheritedByTypeSystem)==TypeEntry::GenerateNothing)
        return;
    ReportHandler::debugSparse("Generating enum: " + global_enum->fullName());
    s << INDENT << "package " << global_enum->package() << ";" << Qt::endl << Qt::endl;

    s << "import io.qt.*;" << Qt::endl << Qt::endl;

    QString lines;
    {
        QTextStream s(&lines);
        writeEnum(s, global_enum);
    }
    s << lines;
}
