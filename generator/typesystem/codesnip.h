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

#ifndef CODESNIP_H_
#define CODESNIP_H_

#include <QMap>
#include <QTextStream>
#include "typesystem.h"

class Indentor;
typedef QMap<int, QString> ArgumentMap;

class CodeSnipFragment {
    private:
        const QString m_code;
        TemplateInstance *m_instance;

    public:
        CodeSnipFragment(const QString &code)
                : m_code(code),
                m_instance(0) {}

        CodeSnipFragment(TemplateInstance *instance)
                : m_instance(instance) {}

        QString code() const;
};

class CodeSnipAbstract {
    public:
        QString code() const;

        void addCode(const QString &code) {
            codeList.append(new CodeSnipFragment(code));
        }

        void addTemplateInstance(TemplateInstance *ti) {
            codeList.append(new CodeSnipFragment(ti));
        }

        QList<CodeSnipFragment*> codeList;
};

class CustomFunction : public CodeSnipAbstract {
    public:
        CustomFunction(const QString &n = QString()) : name(n) { }

        QString name;
        QString param_name;
        QString where_name;
};

class TemplateEntry : public CodeSnipAbstract {
    public:
        TemplateEntry(const QString &name)
                : m_name(name) {
        };

        QString name() const {
            return m_name;
        };

    private:
        QString m_name;
};

class CodeSnip : public CodeSnipAbstract {
    public:
        enum Position {
            Beginning,
            End,
            AfterThis
        };

        CodeSnip() : language(TypeSystem::TargetLangCode) { }
        CodeSnip(TypeSystem::Language lang) : language(lang) { }

        // Very simple, easy to make code ugly if you try
        QTextStream &formattedCode(QTextStream &s, Indentor &indentor) const;

        TypeSystem::Language language;
        Position position;
        ArgumentMap argumentMap;
};
typedef QList<CodeSnip> CodeSnipList;

#endif
