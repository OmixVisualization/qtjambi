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

#ifndef VARIANTS_H
#define VARIANTS_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/jobjectwrapper.h>
#endif
#include <QtCore/QtCore>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#include <QtJambiCore/hashes.h>
#include <QtJambiGui/hashes.h>
#include "namespace.h"

class Variants
{
public:
    static QObject* createInternalObject(QObject* parent = nullptr);

    QVariant pushThrough(const QVariant &variant);

    qint64 currentQInt64() const;
    quint64 currentQUInt64() const;

    qint32 currentQInt32() const;
    quint32 currentQUInt32() const;

    qint16 currentQInt16() const;
    quint16 currentQUInt16() const;

    qint8 currentQInt8() const;
    quint8 currentQUInt8() const;

    QChar currentQChar() const;

    float currentFloat() const;
    double currentDouble() const;

    bool currentBool() const;

    QString currentString() const;

    QVariantList currentQVariantList() const;
    QVariantMap currentQVariantMap() const;

    QPixmap currentQPixmap() const;
    QImage currentQImage() const;

    QSize currentQSize() const;
    QSizeF currentQSizeF() const;

    QPoint currentQPoint() const;
    QPointF currentQPointF() const;

    QLine currentQLine() const;
    QLineF currentQLineF() const;

    QRect currentQRect() const;
    QRectF currentQRectF() const;

    int currentType() const;

    bool isValid() const;

    QString currentToString();

    int currentToInt();
	
    double currentToDouble();
	
    bool currentToBoolean();

    static QVariant getDockWidgetArea();
	
    static QVariant getDockWidgetAreas();
	
    static QVariant getPageSize();
	
    static QVariant getSharedPointerOfWidget();

    static QVariant getIntMap();

    static QVariant getColorHash();

    static QVariant getIntMultiMap();

    static QVariant getIntVector();

    static QVariant getIntLinkedList();

    static QVariant getIntList();

    static QVariant getIntSet();

    static QVariant getQObjectSet();

    static QVariant getIntQueue();

    static QVariant getIntStack();

    static QVariant getCharMultiMap();

    static QVariant getCharMultiHash();

    static QVariant getInt16MultiMap();

    static QVariant getInt16MultiHash();

    static QVariant getInt64MultiMap();

    static QVariant getInt64MultiHash();

    static QVariant getStringVector();

    static QVariant getStringLinkedList();

    static QVariant getStringList();

    static QVariant getStringSet();

    static QVariant getStringQueue();

    static QVariant getStringStack();

    static QVariant getUnknownEnumVector();

    static QVariant getUnknownEnumLinkedList();

    static QVariant getUnknownEnumList();

    static QVariant getUnknownEnumSet();

    static QVariant getUnknownEnumQueue();

    static QVariant getUnknownEnumStack();

    static QVariant getUnknownValueVector();

    static QVariant getUnknownValueLinkedList();

    static QVariant getUnknownValueList();

    static QVariant getUnknownValueSet();

    static QVariant getUnknownValueQueue();

    static QVariant getUnknownValueStack();

    static QVariant getDirVector();

    static QVariant getDirLinkedList();

    static QVariant getDirList();

    static QVariant getDirSet();

    static QVariant getDirQueue();

    static QVariant getDirStack();

    static QVariant getTestObjectVector();

    static QVariant getTestObjectLinkedList();

    static QVariant getTestObjectList();

    static QVariant getTestObjectSet();

    static QVariant getTestObjectQueue();

    static QVariant getTestObjectStack();

    static QVariant getTestObjectMap();

    static QVariant getTestObjectHash();

    static QVariant getStringMultiMap();

    static QVariant getStringMultiHash();

    static QVariant getString64MultiHash();

    static QVariant getString16MultiHash();

    static QVariant getString8MultiHash();

    static QVariant getTestObjectMultiMap();

    static QVariant getTestObjectMultiHash();

    static QVariant getDirHash();

    static QVariant getUnknownEnumMap();

    static QVariant getUnknownEnumHash();

    static QVariant getUnknownEnumMultiMap();

    static QVariant getUnknownEnumMultiHash();

    static QVariant getColorMultiHash();

    static QVariant getColorMultiMap();

    static QVariant getRectFMultiHash();

    static QVariant getRectFMultiMap();

    static QVariant getUnknownValueMap();

    static QVariant getUnknownValueHash();

    static QVariant getDirPair();
	
    static QVariant getSpinBox();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static QMetaType propertyType(const QObject* object, const QString& propertyName);
#else
    static int propertyType(const QObject* object, const QString& propertyName);
#endif

    static QColor fetchColorProperty(const QObject* object, const QString& propertyName);

    static QObject* fetchObjectProperty(const QObject* object, const QString& propertyName);

    static QEvent* fetchEventProperty(const QObject* object, const QString& propertyName);

    static QPaintDevice* fetchPaintDeviceProperty(const QObject* object, const QString& propertyName);

    static QRunnable* fetchRunnableProperty(const QObject* object, const QString& propertyName);

    static QStringList fetchStringListProperty(const QObject* object, const QString& propertyName);

    static void analyze(const JObjectWrapper& object);

    static bool compare(QObject* object, const QString& property, const QList<QObject*>& list);

    static bool compare(QObject* object, const QString& property, const QPair<QObject*,QObject*>& pair);

#ifndef QTJAMBI_NO_WIDGETS
    static QGraphicsItem* convertInterface(QGraphicsObject* object);
#endif

    static QObject* convertJavaObject(jobject obj);
private:
    QVariant current;
};

#endif // VARIANTS_H
