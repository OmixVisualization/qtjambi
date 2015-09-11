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

class QPrinter___ extends QPrinter {

        /**
         * Returns the page margins for this printer.
         * The unit of the returned margins are specified with the unit parameter.
         * This function was introduced in Qt 4.4.
         *
         * @param unit Specifies the unit of the returned margins.
         * @return The page margins for this printer.
         */
        public final org.qtjambi.qt.core.QMarginsF getPageMargins(QPrinter.Unit unit) {
            QNativePointer left = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer top = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer right = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer bottom = new QNativePointer(QNativePointer.Type.Double);

            getPageMargins(left, top, right, bottom, unit);
            return new org.qtjambi.qt.core.QMarginsF(left.doubleValue(), top.doubleValue(), right.doubleValue(), bottom.doubleValue());
        }
}// class


