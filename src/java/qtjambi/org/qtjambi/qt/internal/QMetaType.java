/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.internal;

/**
 * This class brings the corresponding c++ type QMetaType and enum QMetaType::Type to Java.
 * @author Peter Droste <info@omix-visualization.com>
 */
class QMetaType{
	enum Type{
		UnknownType(0), Bool(1), Int(2), UInt(3), LongLong(4), ULongLong(5),
        Double(6), Long(32), Short(33), Char(34), ULong(35), UShort(36),
        UChar(37), Float(38),
        VoidStar(31),
        QChar(7), QString(10), QStringList(11), QByteArray(12),
        QBitArray(13), QDate(14), QTime(15), QDateTime(16), QUrl(17),
        QLocale(18), QRect(19), QRectF(20), QSize(21), QSizeF(22),
        QLine(23), QLineF(24), QPoint(25), QPointF(26), QRegExp(27),
        QEasingCurve(29), QUuid(30), QVariant(41), QModelIndex(42),
        QRegularExpression(44),
        QJsonValue(45), QJsonObject(46), QJsonArray(47), QJsonDocument(48),
        QObjectStar(39), SChar(40),
        Void(43),
        QVariantMap(8), QVariantList(9), QVariantHash(28),
        QFont(64), QPixmap(65), QBrush(66), QColor(67), QPalette(68),
        QIcon(69), QImage(70), QPolygon(71), QRegion(72), QBitmap(73),
        QCursor(74), QKeySequence(75), QPen(76), QTextLength(77), QTextFormat(78),
        QMatrix(79), QTransform(80), QMatrix4x4(81), QVector2D(82),
        QVector3D(83), QVector4D(84), QQuaternion(85), QPolygonF(86),
        QSizePolicy(121),
        User(1024);
		public final int value;
		private Type(int value){
			this.value = value;
		}
	}
	
	static Type type(String typeName){
		if("double".equals(typeName)){
			return Type.Double;
		}
		if("int".equals(typeName)){
			return Type.Int;
		}
		if("float".equals(typeName)){
			return Type.Float;
		}
		if("boolean".equals(typeName) || "bool".equals(typeName)){
			return Type.Bool;
		}
		if("short".equals(typeName)){
			return Type.Short;
		}
		if("long".equals(typeName)){
			return Type.Long;
		}
		if("byte".equals(typeName) || "char".equals(typeName)){
			return Type.Char;
		}
		if("void".equals(typeName)){
			return Type.Void;
		}
		for(Type t : Type.values()){
			if(typeName.endsWith(t.name())){
				return t;
			}
		}
		return Type.User;
	}
}
