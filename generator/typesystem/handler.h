/****************************************************************************
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

#ifndef HANDLER_H_
#define HANDLER_H_

#include <qxml.h>
#include "typeentry.h"
#include "stackelement.h"

class TypeDatabase;

class Handler : public QXmlDefaultHandler {
    public:
        Handler(TypeDatabase *database, bool generate)
                : m_database(database),
                  current(nullptr),
                  m_generate(generate ? TypeEntry::GenerateAll : TypeEntry::GenerateForSubclass),
                  tagNames{
                      {"rejection",                  StackElement::Rejection},
                      {"primitive-type",             StackElement::PrimitiveTypeEntry},
                      {"object-type",                StackElement::ObjectTypeEntry},
                      {"template-type",              StackElement::TemplateTypeEntry},
                      {"functional-type",            StackElement::FunctionalTypeEntry},
                      {"iterator-type",              StackElement::IteratorTypeEntry},
                      {"value-type",                 StackElement::ValueTypeEntry},
                      {"interface-type",             StackElement::InterfaceTypeEntry},
                      {"namespace-type",             StackElement::NamespaceTypeEntry},
                      {"enum-type",                  StackElement::EnumTypeEntry},
                      {"extra-includes",             StackElement::ExtraIncludes},
                      {"include",                    StackElement::Include},
                      {"inject-code",                StackElement::InjectCode},
                      {"interface-polymorphy",       StackElement::InterfacePolymorphicId},
                      {"modify-function",            StackElement::ModifyFunction},
                      {"modify-field",               StackElement::ModifyField},
                      {"instantiate-template",       StackElement::InstantiateTemplate},
                      {"access",                     StackElement::Access},
                      {"remove",                     StackElement::Removal},
                      {"rename",                     StackElement::Rename},
                      {"typesystem",                 StackElement::Root},
                      {"custom-constructor",         StackElement::CustomMetaConstructor},
                      {"custom-destructor",          StackElement::CustomMetaDestructor},
                      {"argument-map",               StackElement::ArgumentMap},
                      {"suppress-warning",           StackElement::SuppressedWarning},
                      {"load-typesystem",            StackElement::LoadTypesystem},
                      {"define-ownership",           StackElement::DefineOwnership},
                      {"replace-default-expression", StackElement::ReplaceDefaultExpression},
                      {"reject-enum-value",          StackElement::RejectEnumValue},
                      {"rename-enum-value",          StackElement::RenameEnumValue},
                      {"replace-type",               StackElement::ReplaceType},
                      {"array-type",                 StackElement::ArrayType},
                      {"conversion-rule",            StackElement::ConversionRule},
                      {"modify-argument",            StackElement::ModifyArgument},
                      {"remove-argument",            StackElement::RemoveArgument},
                      {"add-argument",               StackElement::AddArgument},
                      {"remove-default-expression",  StackElement::RemoveDefaultExpression},
                      {"template",                   StackElement::Template},
                      {"insert-template",            StackElement::TemplateInstanceEnum},
                      {"replace",                    StackElement::Replace},
                      {"no-null-pointer",            StackElement::NoNullPointers},
                      {"reference-count",            StackElement::ReferenceCount},
                      {"import",                     StackElement::ImportTemplate}
                  } {
        }

        bool startElement(const QString &namespaceURI, const QString &localName,
                          const QString &qName, const QXmlAttributes &atts);
        bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);

        QString errorString() const {
            return m_error;
        }
        bool error(const QXmlParseException &exception);
        bool fatalError(const QXmlParseException &exception);
        bool warning(const QXmlParseException &exception);

        bool characters(const QString &ch);

        void setImportInputDirectoryList(const QStringList &importInputDirectoryList) {
            m_importInputDirectoryList = importInputDirectoryList;
        }

    private:
        void fetchAttributeValues(const QString &name, const QXmlAttributes &atts,
                                  QHash<QString, QString> *acceptedAttributes);

        bool importFileElement(const QXmlAttributes &atts);
        bool convertBoolean(const QString &, const QString &, bool);

        uint parseQtVersion(QString strg, uint defaultValue);

        TypeDatabase *m_database;
        StackElement* current;
        QString m_defaultPackage;
        QString m_defaultSuperclass;
        QString m_error;
        QStringList m_importInputDirectoryList;
        TypeEntry::CodeGeneration m_generate;

        /**
         * Returns attributes corresponding element type
         * StackElement::ElementType.
         */
        QHash<QString, QString> setStackElementAttributes(StackElement::ElementType type);

        CodeSnipList m_code_snips;
        FunctionModificationList m_function_mods;
        FieldModificationList m_field_mods;

        const QHash<QString, StackElement::ElementType> tagNames;
};

#endif
