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

#ifndef UTILS_H_
#define UTILS_H_

#include <QHash>
#include <QMap>
#include <QString>
#include <QException>

namespace TS {
    class TemplateEntry;
    class TemplateTypeEntry;

    typedef QMap<QString, TemplateEntry *> TemplateEntryHash;
    typedef QMap<QString, TemplateTypeEntry *> TemplateTypeEntryHash;

    class TemplateInstance {
        public:
            TemplateInstance(const QString &name)
                    : m_name(name) {}

            void addReplaceRule(const QString &name, const QString &value) {
                replaceRules[name] = value;
            }

            QString expandCode() const;
            bool hasCode() const;

            QString name() const {
                return m_name;
            }

            void setIndent(const QString& indent){
                m_indent = indent;
            }

        private:
            const QString m_name;
            QMap<QString, QString> replaceRules;
            QString m_indent;
    };

    enum Language {
        NoLanguage          = 0x0000,
        TargetLangCode      = 0x0001,
        NativeCode          = 0x0002,
        ShellCode           = 0x0004,
        ShellDeclaration    = 0x0008,
        PackageInitializer  = 0x0010,
        DestructorFunction  = 0x0020,
        Constructors        = 0x0040,
        Interface           = 0x0080,
        Signal              = 0x0100,
        MetaInfo            = 0x0200,
        ModuleInfo          = 0x0400,
        Getter              = 0x1000,
        Setter              = 0x2000,
        DeleterFunction     = 0x4000,

        // masks
        All                 = TargetLangCode
        | NativeCode
        | Getter
        | Setter
        | ShellCode
        | ShellDeclaration
        | PackageInitializer
        | Constructors
        | Interface
        | DestructorFunction
        | DeleterFunction
        | Signal
        | MetaInfo
        | ModuleInfo,

        JavaAndNativeCode   = TargetLangCode | NativeCode,
        TargetLangAndNativeCode   = TargetLangCode | NativeCode
    };

    enum Ownership {
        InvalidOwnership,
        DefaultOwnership,
        TargetLangOwnership,
        CppOwnership,
        IgnoreOwnership,
        RegisterDependency,
        Invalidate
    };

    struct Include {
        enum IncludeType {
            IncludePath,
            LocalPath,
            TargetLangImport
        };

            Include() : type(IncludePath), name(), inherited(false), suppressed(false), ckeckAvailability(false) { }
        Include(IncludeType t, const QString &nam, bool _inherited = false, bool _suppressed = false, bool _ckeckAvailability = false) : type(t), name(nam), inherited(_inherited), suppressed(_suppressed), ckeckAvailability(_ckeckAvailability) { }
        Include(IncludeType t, const QString &nam, const QMap<QString,QString>& _requiredFeatures, bool _inherited = false, bool _suppressed = false, bool _ckeckAvailability = false) : type(t), name(nam), requiredFeatures(_requiredFeatures), inherited(_inherited), suppressed(_suppressed), ckeckAvailability(_ckeckAvailability) { }

        bool isValid() const { return !name.isEmpty(); }

        QString toString() const;

        bool operator<(const Include &other) const { return name < other.name; }
        IncludeType type;
        QString name;
        QMap<QString,QString> requiredFeatures;
        bool inherited;
        bool suppressed;
        bool ckeckAvailability;
    };

    struct OwnershipRule {
        TS::Ownership ownership;
        QString condition;
    };

    struct ReferenceCount {
        ReferenceCount() : threadSafe(false), access(Public) { }
        enum Action { // 0x01 - 0xff
            Invalid     = 0x00,
            Add         = 0x01,
            AddAll      = 0x02,
            Remove      = 0x04,
            Set         = 0x08,
            Ignore      = 0x10,
            ClearAdd    = 0x20,
            ClearAddAll = 0x40,
            Put         = 0x80,

            ActionsMask = 0xff,

            Padding     = 0xfffffff
        };

        enum Flag { // 0x100 - 0xf00
            ThreadSafe      = 0x100,
            Static          = 0x200,
            DeclareVariable = 0x400,

            FlagsMask       = 0xf00
        };

        enum Access { // 0x1000 - 0xf000
            Private     = 0x1000,
            Protected   = 0x2000,
            Friendly    = 0x3000,
            Public      = 0x4000,

            AccessMask  = 0xf000
        };

        Action action;
        QString variableName;
        QString condition;
        QString declareVariable;

    uint threadSafe : 1;

        uint access;

        uint keyArgument;
    };

    struct TypeRejection {
        QString class_name;
        QString function_name;
        QString field_name;
        QString enum_name;
    };

}

class TypesystemException : public QException{
public:
    ~TypesystemException() = default;
    TypesystemException *clone() const override;
    char const* what() const noexcept override;
    void raise() const override;
    static void raise(const QString& message);
private:
    TypesystemException(const QByteArray& message);
    QByteArray m_message;
};

#endif
