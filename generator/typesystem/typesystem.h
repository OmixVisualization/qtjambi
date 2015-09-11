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

#ifndef TYPESYSTEM_H_
#define TYPESYSTEM_H_

#include <QHash>
#include <QString>

class TemplateEntry;

typedef QHash<QString, TemplateEntry *> TemplateEntryHash;

class TemplateInstance {
    public:
        TemplateInstance(const QString &name)
                : m_name(name) {}

        void addReplaceRule(const QString &name, const QString &value) {
            replaceRules[name] = value;
        }

        QString expandCode() const;

        QString name() const {
            return m_name;
        }

    private:
        const QString m_name;
        QHash<QString, QString> replaceRules;
};

namespace TypeSystem {
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

        // masks
        All                 = TargetLangCode
        | NativeCode
        | ShellCode
        | ShellDeclaration
        | PackageInitializer
        | Constructors
        | Interface
        | DestructorFunction
        | Signal,

        JavaAndNativeCode   = TargetLangCode | NativeCode,
        TargetLangAndNativeCode   = TargetLangCode | NativeCode
    };

    enum Ownership {
        InvalidOwnership,
        DefaultOwnership,
        TargetLangOwnership,
        CppOwnership
    };
};

struct Include {
    enum IncludeType {
        IncludePath,
        LocalPath,
        TargetLangImport
    };

    Include() : type(IncludePath) { }
    Include(IncludeType t, const QString &nam) : type(t), name(nam) { }

    bool isValid() { return !name.isEmpty(); }

    IncludeType type;
    QString name;

    QString toString() const;

    bool operator<(const Include &other) const { return name < other.name; }
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

        Padding     = 0xffffffff
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
    QString conditional;
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

QString fixCppTypeName(const QString &name);
#endif
