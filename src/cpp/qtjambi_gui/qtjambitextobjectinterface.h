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

#ifndef QTJAMBITEXTOBJECTINTERFACE_H
#define QTJAMBITEXTOBJECTINTERFACE_H

#include <QtCore/QObject>
#include <QtGui/QTextObjectInterface>

// NOTE:
// This class intentionally inherits QTextObjectInterface, which is a
// class not included in the Qt Jambi type system. This is part of a trick
// to get a Java API which is as similar as possible to the unportable
// Q_DECLARE_INTERFACE() API which is used in Qt. The pure virtual functions
// are redeclared here solely so that the generator will generate code for them.
class QtJambiTextObjectInterface: public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)
public:
    virtual QSizeF intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format) = 0;
    virtual void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format) = 0;
};

#endif // QTJAMBITEXTOBJECTINTERFACE_H
