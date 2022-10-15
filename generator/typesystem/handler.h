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

#include <QtXml/QtXml>
#include "typeentry.h"

class TypeDatabase;

class TypesystemException : public QException{
public:
    char const* what() const noexcept override;
    static void raise(const QString& message);
private:
    TypesystemException(const QString& message);
    QByteArray m_message;
};

struct NamespacePrefix{
    enum NamingPolicy{
        Cut,
        Preserve
    };

    QString prefix;
    QString _namespace;
    NamingPolicy namingPolicy;
};

class Handler {
    public:
        Handler(TypeDatabase *database, bool generate, uint qtVersion)
                : m_database(database),
                  m_qtVersion(qtVersion),
                  m_generate(generate ? TypeEntry::GenerateAll : TypeEntry::GenerateForSubclass) {}

        void parse(const QString &filepath);

        void setImportInputDirectoryList(const QStringList &importInputDirectoryList) {
            m_importInputDirectoryList = importInputDirectoryList;
        }

        void setTypeystemsDirectory(const QString &typeystem_directory) {
            m_typeystem_directory = typeystem_directory;
        }

    private:
        bool convertBoolean(const QString &, const QString &, bool);

        QString attributeValue(const QDomNode& domNode, const QString & defaultValue = QString());

        uint parseQtVersion(QString strg, uint defaultValue);
        bool checkQtVersion(QDomNamedNodeMap& attributes);
        void parseTypeSystem(const QDomElement &typeSystemElement, const QString& fileName);
        void parseLoadTypeSystem(const QDomElement &element);
        void parseSuppressedWarning(const QDomElement &element);
        void parseInjectCode(const QDomElement &element, ComplexTypeEntry* entry);
        void parseInjectCode(const QDomElement &element, FunctionalTypeEntry* entry);
        void parseInjectCode(const QDomElement &element,const QHash<QString, TypeSystem::Language>& languageNames,
                             const QHash<QString, CodeSnip::Position>& positionNames,
                             const std::function<void(const QString&,const CodeSnip&)>& appendCodeSnip, bool argumentMapAllowed = false, bool allowPackage = false);
        void parseConversionRule(const QHash<QString, TypeSystem::Language>& languageNames, const QDomElement &element, const std::function<void(const CodeSnip&)>& appendCodeSnip);
        void parseImportFile(const QDomElement &element, const std::function<void(const QString&)>& consumeCode);
        void parseInsertTemplate(const QDomElement &element, const std::function<void(TemplateInstance*)>& consumeTemplateInstance);
        void parseExtraIncludes(const QDomElement &element, TypeEntry* entry);
        Include parseInclude(const QDomElement &element);
        void parseRejection(const QDomElement &element);
        void parseTemplate(const QDomElement &element);
        void parseInstantiation(const QDomElement &element, ComplexTypeEntry* entry);
        void parsePrimitiveType(const QDomElement &element);
        void parseObjectType(const QDomElement &element);
        void parseAttributesOfComplexType(const QDomElement &element, QDomNamedNodeMap& attributes, ComplexTypeEntry* entry);
        QList<QDomElement> parseChildrenOfComplexType(const QDomElement &element, ComplexTypeEntry* entry);
        QString parseTypeName(QDomNamedNodeMap& attributes, const QDomElement &element);
        void parseTemplateType(const QDomElement &element);
        void parseFunctionalType(const QDomElement &element);
        void parseIteratorType(const QDomElement &element);
        void parseValueType(const QDomElement &element);
        void parseTypeAliasType(const QDomElement &element);
        void parseInterfaceType(const QDomElement &element);
        void parseNamespaceType(const QDomElement &element);
        void parseModifyArgument(const QDomElement &element, AbstractFunctionModification& mod);
        void parseModifyFunction(const QDomElement &element, TypeEntry* entry);
        TemplateInstantiation parseInstantiation(const QDomElement &element);
        void parseModifyField(const QDomElement &element, ComplexTypeEntry* entry);
        void parseEnumType(const QDomElement &element);
        CustomFunction parseCustomStructor(const QDomElement &element);
        void ensureNoChildren(const QDomElement &element);

        TypeDatabase *m_database;
        uint m_qtVersion;
        QString m_defaultPackage;
        QString m_defaultSuperclass;
        QStringList m_importInputDirectoryList;
        QString m_typeystem_directory;
        TypeEntry::CodeGeneration m_generate;
        QList<NamespacePrefix> m_namespacePrefixes;
        QMultiMap<TypeSystemTypeEntry*,QString> m_requiredModules;
};

#endif
