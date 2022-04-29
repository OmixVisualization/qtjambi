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

#ifndef ABSTRACTMETABUILDER_H
#define ABSTRACTMETABUILDER_H

#include "codemodel.h"
#include "abstractmetalang.h"
#include "typesystem/typesystem.h"
#include "typeparser.h"

#include <QtCore/QSet>
#include "typesystem/typedatabase.h"
#include "docindex/docindexreader.h"

struct Operator;

class AbstractMetaBuilder {
    public:
        enum RejectReason {
            NotInTypeSystem,
            GenerationDisabled,
            RedefinedToNotClass,
            UnmatchedArgumentType,
            UnmatchedReturnType,
            IsPrivate,
            NoReason
        };

        AbstractMetaBuilder();
        virtual ~AbstractMetaBuilder();

        const AbstractMetaClassList& classes() const { return m_meta_classes; }
        const AbstractMetaFunctionalList& functionals() const { return m_meta_functionals; }
        AbstractMetaClassList classesTopologicalSorted() const;

        FileModelItem model() const { return m_dom; }
        void setModel(FileModelItem item) { m_dom = item; }

        ScopeModelItem popScope() { return m_scopes.takeLast(); }
        void pushScope(ScopeModelItem item) { m_scopes << item; }
        ScopeModelItem currentScope() const { return m_scopes.last(); }

        const QString& fileName() const { return m_file_name; }
        void setFileName(const QString &fileName) { m_file_name = fileName; }

        void dumpLog();

        bool build();

        void applyDocs(const DocModel* model);

        void figureOutEnumValuesForClass(AbstractMetaClass *meta_class, QSet<AbstractMetaClass *> *classes, QSet<AbstractMetaClass *> *repeatClasses, QSet<QString> *warnings = nullptr);
        QVariant figureOutEnumValue(const uint size, const QString &name, QVariant value, AbstractMetaClass *global, AbstractMetaEnum *meta_enum, AbstractMetaFunction *meta_function = nullptr, QSet<QString> *warnings = nullptr);
        void figureOutEnumValues();
        void figureOutDefaultEnumArguments();
        void figureOutFunctionsInNamespace(const NamespaceModelItem &item);

        void addAbstractMetaClass(AbstractMetaClass *cls);
        void addAbstractMetaFunctional(AbstractMetaFunctional *cls);
        AbstractMetaClass *traverseTypeAlias(TypeAliasModelItem item);
        AbstractMetaFunctional *traverseFunctional(TypeAliasModelItem item);
        AbstractMetaClass *traverseClass(ClassModelItem item);
        bool setupInheritance(AbstractMetaClass *meta_class);
        bool setupTemplateInstantiations(AbstractMetaClass *meta_class);
        AbstractMetaClass *traverseNamespace(NamespaceModelItem item);
        AbstractMetaEnum *traverseEnum(EnumModelItem item, AbstractMetaClass *enclosing, const QSet<QString> &enumsDeclarations);
        AbstractMetaClass * instantiateIterator(IteratorTypeEntry *iteratorTypeEntry, AbstractMetaClass *subclass, const QList<const AbstractMetaType *>& template_types, const QHash<const TypeEntry *,const AbstractMetaType *>& template_types_by_name);
        void traverseEnums(ScopeModelItem item, AbstractMetaClass *parent, const QStringList &enumsDeclarations);
        void traverseFunctions(ScopeModelItem item, AbstractMetaClass *parent);
        void traverseFields(ScopeModelItem item, AbstractMetaClass *parent);
        void traverseStreamOperator(FunctionModelItem function_item);
        void traverseCompareOperator(FunctionModelItem item);
        AbstractMetaFunction *traverseFunction(FunctionModelItem function);
        AbstractMetaField *traverseField(VariableModelItem field, const AbstractMetaClass *cls);
        void checkFunctionModifications();
        void registerHashFunction(FunctionModelItem function_item);
        void registerToStringCapability(FunctionModelItem function_item);

        void parseQ_Property(AbstractMetaClass *meta_class, const QStringList &declarations);
        void setupEquals(AbstractMetaClass *meta_class);
        void setupBeginEnd(AbstractMetaClass *meta_class);
        void setupComparable(AbstractMetaClass *meta_class);
        void setupClonable(AbstractMetaClass *cls);
        void setupFunctionDefaults(AbstractMetaFunction *meta_function, AbstractMetaClass *meta_class);

        QString translateDefaultValue(const QString& defaultValueExpression, AbstractMetaType *type,
                                      AbstractMetaFunction *fnc, AbstractMetaClass *,
                                      int argument_index);
        AbstractMetaType *translateType(const TypeInfo& type_info, bool* ok, const QString &contextString = QString(),
                                      bool resolveType = true, bool resolveScope = true, bool prependScope = true);

        static void decideUsagePattern(AbstractMetaType *type);

        bool inheritHiddenBaseType(AbstractMetaClass *subclass,
                             const AbstractMetaClass *template_class,
                             const TypeParser::Info &info);
        AbstractMetaType *inheritTemplateType(const QList<const AbstractMetaType *> &template_types, const AbstractMetaType *meta_type, bool *ok = nullptr);

        bool isClass(const QString &qualified_name, const QString& className);
        bool isEnum(const QStringList &qualified_name);

        void fixQObjectForScope(TypeDatabase *types,
                                NamespaceModelItem item);

        const QString& outputDirectory() const { return m_out_dir; }
        void setOutputDirectory(const QString &outDir) { m_out_dir = outDir; }
        void setFeatures(const QMap<QString, QString>& features){ m_features = &features; }
        void setQtVersion(uint qtVersionMajor, uint qtVersionMinor, uint qtVersionPatch, uint qtjambiVersionPatch) {
            m_qtVersionMajor = qtVersionMajor;
            m_qtVersionMinor = qtVersionMinor;
            m_qtVersionPatch = qtVersionPatch;
            m_qtjambiVersionPatch = qtjambiVersionPatch;
        }
        const QMap<QString,TypeSystemTypeEntry *>& typeSystemByPackage() const { return m_typeSystemByPackage; }
    protected:
        AbstractMetaClass *argumentToClass(ArgumentModelItem, const QString &contextString);

        virtual AbstractMetaClass *createMetaClass() = 0;
        virtual AbstractMetaEnum *createMetaEnum() = 0;
        virtual AbstractMetaFunctional *createMetaFunctional() = 0;
        virtual AbstractMetaEnumValue *createMetaEnumValue() = 0;
        virtual AbstractMetaField *createMetaField() = 0;
        virtual AbstractMetaFunction *createMetaFunction() = 0;
        virtual AbstractMetaArgument *createMetaArgument() = 0;
        virtual AbstractMetaType *createMetaType() = 0;

    private:
        TypeInfo analyzeTypeInfo(AbstractMetaClass *cls, QString strg);
        AbstractMetaFunctional * findFunctional(AbstractMetaClass *cls, const FunctionalTypeEntry * fentry);
        void fixMissingIterator();
        void sortLists();
        Operator findOperator(const uint size, QString *s,
                              AbstractMetaClass *global,
                              AbstractMetaEnum *meta_enum,
                              AbstractMetaFunction *meta_function, QSet<QString> *warnings = nullptr);

        struct RenamedOperator{
            QString newName;
            TypeEntry *castType;
            bool skip;
        };

        RenamedOperator rename_operator(const QString &oper);

        QString m_file_name;
        QString m_out_dir;

        AbstractMetaClassList m_meta_classes;
        AbstractMetaFunctionalList m_meta_functionals;
        AbstractMetaClassList m_templates;
        AbstractMetaClassList m_template_iterators;
        FileModelItem m_dom;

        QSet<const TypeEntry *> m_used_types;

        QMap<QPair<QString,QString>, RejectReason> m_rejected_classes;
        QMap<QPair<QString,QString>, RejectReason> m_rejected_functionals;
        QMap<QPair<QString,QString>, RejectReason> m_rejected_enums;
        QMap<QPair<QString,QString>, RejectReason> m_rejected_functions;
        QMap<QPair<QString,QString>, RejectReason> m_rejected_fields;

        QList<AbstractMetaEnum *> m_enums;

        QHash<QString, AbstractMetaEnumValue *> m_enum_values;

        AbstractMetaClass *m_current_class;
        QList<ScopeModelItem> m_scopes;
        QString m_namespace_prefix;

        QSet<AbstractMetaClass *> m_setup_inheritance_done;

        struct MissingIterator{
            MissingIterator(const IteratorTypeEntry* _iteratorType,
                            AbstractMetaType *_meta_type,
            AbstractMetaClass * _current_class) :
                iteratorType(_iteratorType),
                meta_type(_meta_type),
                current_class(_current_class)
            {}
            const IteratorTypeEntry* iteratorType;
            AbstractMetaType *meta_type;
            AbstractMetaClass * current_class;
        };
        QList<MissingIterator> m_missing_iterators;
        const QMap<QString, QString>* m_features;
        QMap<QString,TypeSystemTypeEntry *> m_typeSystemByPackage;
        uint m_qtVersionMajor;
        uint m_qtVersionMinor;
        uint m_qtVersionPatch;
        uint m_qtjambiVersionPatch;
};

struct Operator {
    enum Type { Plus, Minus, ShiftLeft, Not, None };

    Operator(const uint _size) : type(None), size(_size), value() { }

    QVariant calculate(QVariant x);

    QString toString(QString x);

    Type type;
    const uint size;
    QVariant value;
};

#endif // ABSTRACTMETBUILDER_H
