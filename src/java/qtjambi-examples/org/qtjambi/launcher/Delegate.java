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

package org.qtjambi.launcher;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.widgets.*;

public class Delegate extends QItemDelegate {

    public static final int MARGE_HOR = 20; // Extra pixels on each side of text...
    public static final int MARGE_VER = 6;

    public Delegate(QSortFilterProxyModel model) {
        super(model);
        m_proxyModel = model;
    }

    private static String stripName(String s) {
        String[] tmp = RetroTranslatorHelper.split(s, "\\.");
        return tmp[tmp.length - 1];
    }

    @Override
    public void paint(QPainter p, QStyleOptionViewItem option, QModelIndex index) {
        index = m_proxyModel.mapToSource(index);
        Launchable l = ((LaunchableListModel)m_proxyModel.sourceModel()).at(index);
        String text = stripName(l.name());
        boolean selected = (option.state().isSet(QStyle.StateFlag.State_Selected));
        QRectF rect = new QRectF(option.rect());
        rect.adjust(3, 1, -3, -1);
        if (selected) {
            Style.drawShadeButton(p, rect, new QStyle.State(QStyle.StateFlag.State_Enabled));
            Style.drawButtonOutline(p, rect, new QStyle.State(Style.StateFlag.State_Enabled));
        }

        Style.drawShadowText(p, rect.translated(0, -1), text, 2, 2);
    }

    @Override
    public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
        index = m_proxyModel.mapToSource(index);
        Launchable l = ((LaunchableListModel)m_proxyModel.sourceModel()).at(index);

        String text = stripName(l.name());

        QRect rect = option.fontMetrics().boundingRect(text);

        return new QSize(rect.width() + MARGE_HOR * 2, rect.height() + MARGE_VER * 2);
    }

    // Member variables...
    private QSortFilterProxyModel m_proxyModel;
}
