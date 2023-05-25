/****************************************************************************
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

#ifndef CODESNIP_H_
#define CODESNIP_H_

#include <QMap>
#include <QTextStream>
#include "utils.h"

class Indentor;

namespace TS{

typedef QMap<int, QString> ArgumentMap;

class CodeSnipFragment {
    private:
        QString m_code;
        TemplateInstance *m_instance;
        friend class CodeSnipAbstract;

    public:
        CodeSnipFragment(const QString &code)
                : m_code(code),
                m_instance(nullptr) {}

        CodeSnipFragment(TemplateInstance *instance)
                : m_instance(instance) {}

        QString code() const;
        bool hasCode() const;
};

class CodeSnipAbstract {
    public:
        QString code() const;
        bool hasCode() const;

        void addCode(const QString &code);

        void addTemplateInstance(TemplateInstance *ti);

        QList<CodeSnipFragment*> codeList;
};

class CustomFunction : public CodeSnipAbstract {
    public:
        CustomFunction(const QString &n = QString()) : name(n) { }

        QString name;
        QString param_name;
        QString placement_name;
};

class TemplateEntry : public CodeSnipAbstract {
    public:
        TemplateEntry(const QString &name)
                : m_name(name) {
        }

        QString name() const {
            return m_name;
        }

    private:
        QString m_name;
};

class CodeSnip : public CodeSnipAbstract {
    public:
        enum Position {
            Beginning = 0,
            Position1,
            Position2,
            Position3,
            Position4,
            Position5,
            Equals,
            Compare,
            HashCode,
            ToString,
            Clone,
            Comment,
            End = 1024
        };

        CodeSnip() : language(TS::TargetLangCode) { }
        CodeSnip(TS::Language lang) : language(lang) { }

        // Very simple, easy to make code ugly if you try
        QTextStream &formattedCode(QTextStream &s, Indentor &indentor) const;

        TS::Language language;
        Position position;
        ArgumentMap argumentMap;
};
typedef QList<CodeSnip> CodeSnipList;

}

#endif
