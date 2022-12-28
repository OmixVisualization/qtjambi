/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "injectedcode.h"

#ifndef QTJAMBI_NO_WIDGETS
void GraphicsSceneSubclass::drawItems(QPainter *painter,
                                     int numItems,
                                     QGraphicsItem *items[],
                                     const QStyleOptionGraphicsItem options[],
                                     QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(widget)
    this->numItems = numItems;

    firstBoundingRect = items[0]->boundingRect();
    secondBoundingRect = items[1]->boundingRect();

    firstStyleOptionType = options[0].type;
    firstStyleOptionVersion = options[0].version;

    secondStyleOptionType = options[1].type;
    secondStyleOptionVersion = options[1].version;

    firstItem = items[0];
    secondItem = items[1];

    firstStyleOption = options[0];
    secondStyleOption = options[1];
}
#endif

void AccessibleTextInterfaceSubclass::selection(int selectionIndex, int *startOffset, int *endOffset) const
{
    if (startOffset) *startOffset = selectionIndex + 1;
    if (endOffset) *endOffset = selectionIndex + 2;
}

void AccessibleTextInterfaceSubclass::callSelection(AccessibleTextInterfaceSubclass *obj, int selectionIndex, int *startOffset, int *endOffset)
{
    Q_ASSERT(obj);
    obj->selection(selectionIndex, startOffset, endOffset);
}

QString AccessibleTextInterfaceSubclass::attributes(int offset, int *startOffset, int *endOffset) const
{
    if (startOffset) *startOffset = offset + 1;
    if (endOffset) *endOffset = offset + 2;
    return QString("AccessibleTextInterfaceSubclass::attributes");
}

QString AccessibleTextInterfaceSubclass::callAttributes(AccessibleTextInterfaceSubclass *obj, int offset, int *startOffset, int *endOffset)
{
    Q_ASSERT(obj);
    return obj->attributes(offset, startOffset, endOffset);
}

QString AccessibleTextInterfaceSubclass::textBeforeOffset(int offset, QAccessible::TextBoundaryType,
                                int *startOffset, int *endOffset) const
{
    if (startOffset) *startOffset = offset + 1;
    if (endOffset) *endOffset = offset + 2;
    return QString("AccessibleTextInterfaceSubclass::textBeforeOffset");
}

QString AccessibleTextInterfaceSubclass::callTextBeforeOffset(AccessibleTextInterfaceSubclass *obj, int offset, QAccessible::TextBoundaryType boundaryType,
                                 int *startOffset, int *endOffset)
{
    Q_ASSERT(obj);
    return obj->textBeforeOffset(offset, boundaryType, startOffset, endOffset);
}

QString AccessibleTextInterfaceSubclass::textAfterOffset(int offset, QAccessible::TextBoundaryType,
                                int *startOffset, int *endOffset) const
{
    if (startOffset) *startOffset = offset + 1;
    if (endOffset) *endOffset = offset + 2;
    return QString("AccessibleTextInterfaceSubclass::textAfterOffset");
}

QString AccessibleTextInterfaceSubclass::callTextAfterOffset(AccessibleTextInterfaceSubclass *obj, int offset, QAccessible::TextBoundaryType boundaryType,
                                int *startOffset, int *endOffset)
{
    Q_ASSERT(obj);
    return obj->textAfterOffset(offset, boundaryType, startOffset, endOffset);
}

QString AccessibleTextInterfaceSubclass::textAtOffset(int offset, QAccessible::TextBoundaryType,
                             int *startOffset, int *endOffset) const
{
    if (startOffset) *startOffset = offset + 1;
    if (endOffset) *endOffset = offset + 2;
    return QString("AccessibleTextInterfaceSubclass::textAtOffset");
}

QString AccessibleTextInterfaceSubclass::callTextAtOffset(AccessibleTextInterfaceSubclass *obj, int offset, QAccessible::TextBoundaryType boundaryType,
                             int *startOffset, int *endOffset)
{
    Q_ASSERT(obj);
    return obj->textAtOffset(offset, boundaryType, startOffset, endOffset);
}

#ifndef QTJAMBI_NO_NETWORK
void AbstractSocketSubclass::connectProxyAuthenticationRequired(QAbstractSocket *socket)
{
    QObject::connect(socket, &QAbstractSocket::proxyAuthenticationRequired, this, &AbstractSocketSubclass::aSlot);
}

void AbstractSocketSubclass::emitProxyAuthenticationRequired(QAbstractSocket *socket, const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    static_cast<AbstractSocketSubclass *>(socket)->emitSignalAccessor(proxy, authenticator);
}

void AbstractSocketSubclass::aSlot(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    if (authenticator) {
        authenticator->setUser(proxy.user());
        authenticator->setPassword(proxy.password());
    }
}
#endif // QTJAMBI_NO_NETWORK

namespace Java{
Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
namespace QtXml{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/xml,QXmlEntityResolver$ResolvedEntity,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLio/qt/xml/QXmlInputSource;)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(error,Z)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(inputSource,Lio/qt/xml/QXmlInputSource;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/xml,QXmlNamespaceSupport$ProcessedName,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/xml,QXmlNamespaceSupport$SplitName,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;Ljava/lang/String;)
)
}
#endif

#ifndef QTJAMBI_NO_WIDGETS
namespace QtWidgets{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QGraphicsItem$BlockedByModalPanelInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLio/qt/widgets/QGraphicsItem;)
)
}
#endif
}

bool ImageIOHandlerSubclass::callRead(QImage *image) {
    return read(image);
}

bool ImageIOHandlerSubclass::read(QImage *image) {
    return image != nullptr ? image->load(":io/qt/autotests/svgcards-example.png") : true;
}
