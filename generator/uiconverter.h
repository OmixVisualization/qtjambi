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

#ifndef UICONVERTER_H
#define UICONVERTER_H

#include "metajava.h"

#include <QtXml>

class UiConverter {
    public:
        enum SearchType {
            SignalSearch,
            SlotSearch
        };

        void setClasses(const AbstractMetaClassList &classes) { m_java_classes = classes; }
        AbstractMetaClassList classes() const { return m_java_classes; }

        void convertToJui(const QString &uiFile, const QString &customWidgetFiles);

    private:
        typedef QPair<QString, AbstractMetaClass *> CustomWidget;

        void traverse(QDomNode node, QDomDocument *doc);
        void fixUiNode(QDomElement node, QDomDocument *doc);
        void fixSetNode(QDomElement node, QDomDocument *doc);
        void fixEnumNode(QDomElement node, QDomDocument *doc);
        void fixConnectionNode(QDomElement node, QDomDocument *doc);
        void fixWidgetNode(QDomElement, QDomDocument *doc);
        void fixCustomWidgetNode(QDomElement, QDomDocument *doc);

        void traverseCustomWidgets(const QString &customWidgetFiles);
        void traverseCustomWidgetFile(const QString &customWidgetFile);
        QString translateEnumValue(const QString &enumValue);
        const AbstractMetaFunction *findFunction(AbstractMetaClass *javaClass, const QString &signature,
                SearchType type);

        AbstractMetaClassList m_java_classes;
        QHash<QString, AbstractMetaClass *> m_named_widgets;
        QMultiMap<QString, CustomWidget> m_custom_widgets;
};

#endif // UICONVERTER_H
