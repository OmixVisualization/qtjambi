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


#ifndef JAMBI_CUSTOM_WIDGET_H
#define JAMBI_CUSTOM_WIDGET_H

#include <QtDesigner/QtDesigner>
#include <qtjambi/qtjambi_global.h>

class JambiCustomWidget: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    JambiCustomWidget(jobject object);
    virtual ~JambiCustomWidget();

    virtual bool isInitialized() const;
    virtual void initialize(QDesignerFormEditorInterface *core);

    virtual QWidget *createWidget(QWidget *parent);

    virtual bool isContainer() const;
    virtual QString group() const;
    virtual QString name() const;
    virtual QString toolTip() const;
    virtual QString whatsThis() const;
    virtual QString includeFile() const;
    virtual QIcon icon() const;

    virtual QString domXml() const;


    QDesignerLanguageExtension *language() const;

private:
    QString callStringMethod(jmethodID method) const;

    QDesignerFormEditorInterface *m_core;
    jobject m_object;


};


class JambiCustomWidgetCollection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(JambiCustomWidgetCollection)
#endif
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    JambiCustomWidgetCollection();
    ~JambiCustomWidgetCollection();

    QList<QDesignerCustomWidgetInterface*> customWidgets() const;

public slots:
    void loadPlugins(const QString &path, QObject *widgetFactory);

private:
    void initializeWidgets(JNIEnv *env);

    jobject m_manager;
    jmethodID m_id_customWidgets;
    jmethodID m_id_loadPlugins;

    QList<QDesignerCustomWidgetInterface *> m_widgets;
};
#endif // JAMBI_CUSTOM_WIDGET_H
