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

#include "codesnip.h"

namespace TS{

void CodeSnipAbstract::addTemplateInstance(TemplateInstance *ti) {
    if(!codeList.isEmpty()){
        CodeSnipFragment* codeFragment = codeList.last();
        if(!codeFragment->m_instance
                && !codeFragment->m_code.isEmpty()){
            QStringList lines = codeFragment->m_code.split("\n");
            if(!lines.isEmpty()){
                if(!lines.last().isEmpty()
                        && lines.last().trimmed().isEmpty()){
                    ti->setIndent(lines.takeLast());
                    codeFragment->m_code = lines.join("\n")+"\n";
                }
            }
        }
    }
    codeList.append(new CodeSnipFragment(ti));
}

void CodeSnipAbstract::addCode(const QString &code) {
    if(codeList.isEmpty() && code.trimmed().isEmpty())
        return;
    codeList.append(new CodeSnipFragment(code));
}

QString CodeSnipAbstract::code() const {
    QString res;
    for(CodeSnipFragment *codeFrag : codeList) {
        res.append(codeFrag->code());
    }
    return res;
}

bool CodeSnipAbstract::hasCode() const {
    for(CodeSnipFragment *codeFrag : codeList) {
        if(codeFrag->hasCode())
            return true;
    }
    return false;
}

QString CodeSnipFragment::code() const {
    if (m_instance)
        return m_instance->expandCode();
    else
        return m_code;
}

bool CodeSnipFragment::hasCode() const {
    if (m_instance)
        return m_instance->hasCode();
    else
        return !m_code.isEmpty();
}

}

