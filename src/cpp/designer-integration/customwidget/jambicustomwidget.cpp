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


#include "jambicustomwidget.h"
#include <jambilanguageextension.h>

#include <qtjambi/qtjambi_core.h>

#include <QtCore/QtPlugin>
#include <QtCore/QtDebug>
#include <QtCore/QMetaObject>

#include <qapplication.h>

static jclass class_CustomWidget;
static jmethodID method_createWidget;
static jmethodID method_group;
static jmethodID method_icon;
static jmethodID method_includeFile;
static jmethodID method_isContainer;
static jmethodID method_name;
static jmethodID method_tooltip;
static jmethodID method_whatsThis;
static jmethodID method_pluginClass;

static void resolve(JNIEnv *env) {
    if (class_CustomWidget)
        return;

    jclass cls = qtjambi_find_class(env, "org/qtjambi/tools/designer/CustomWidget");
    if (qtjambi_exception_check(env))
        return;
    class_CustomWidget = (jclass) env->NewGlobalRef(cls);
    Q_ASSERT(class_CustomWidget);
    if (qtjambi_exception_check(env))
        return;

    method_createWidget = env->GetMethodID(class_CustomWidget, "createWidget", "(Lorg/qtjambi/qt/gui/QWidget;)Lorg/qtjambi/qt/gui/QWidget;");
    Q_ASSERT(method_createWidget);
    if (qtjambi_exception_check(env))
        return;

    method_group = env->GetMethodID(class_CustomWidget, "group", "()Ljava/lang/String;");
    Q_ASSERT(method_group);
    if (qtjambi_exception_check(env))
        return;

    method_icon = env->GetMethodID(class_CustomWidget, "icon", "()Lorg/qtjambi/qt/gui/QIcon;");
    Q_ASSERT(method_icon);
    if (qtjambi_exception_check(env))
        return;

    method_includeFile = env->GetMethodID(class_CustomWidget, "includeFile", "()Ljava/lang/String;");
    Q_ASSERT(method_includeFile);
    if (qtjambi_exception_check(env))
        return;

    method_isContainer = env->GetMethodID(class_CustomWidget, "isContainer", "()Z");
    Q_ASSERT(method_isContainer);
    if (qtjambi_exception_check(env))
        return;

    method_name = env->GetMethodID(class_CustomWidget, "name", "()Ljava/lang/String;");
    Q_ASSERT(method_name);
    if (qtjambi_exception_check(env))
        return;

    method_tooltip = env->GetMethodID(class_CustomWidget, "tooltip", "()Ljava/lang/String;");
    Q_ASSERT(method_tooltip);
    if (qtjambi_exception_check(env))
        return;

    method_whatsThis = env->GetMethodID(class_CustomWidget, "whatsThis", "()Ljava/lang/String;");
    Q_ASSERT(method_whatsThis);
    if (qtjambi_exception_check(env))
        return;

    method_pluginClass = env->GetMethodID(class_CustomWidget, "pluginClass", "()Ljava/lang/Class;");
    Q_ASSERT(method_whatsThis);
    if (qtjambi_exception_check(env))
        return;
}

JambiCustomWidget::JambiCustomWidget(jobject object):
    m_core(0),
    m_object(0)
{
    Q_ASSERT(object);

    JNIEnv *env = qtjambi_current_environment();
    resolve(env);
    m_object = env->NewGlobalRef(object);
    qtjambi_exception_check(env);
}

JambiCustomWidget::~JambiCustomWidget()
{
    JNIEnv *env = qtjambi_current_environment();
    QTJAMBI_EXCEPTION_CHECK(env);
    env->DeleteGlobalRef(m_object);
    QTJAMBI_EXCEPTION_CHECK(env);
}

QDesignerLanguageExtension *JambiCustomWidget::language() const
{
    if (! m_core)
        return 0;

    QExtensionManager *mgr = m_core->extensionManager();
    return qt_extension<QDesignerLanguageExtension*> (mgr, m_core);
}

bool JambiCustomWidget::isInitialized() const
{
    return m_core != 0;
}

void JambiCustomWidget::initialize(QDesignerFormEditorInterface *core)
{
    if (m_core)
        return;

    m_core = core;
}

QString JambiCustomWidget::callStringMethod(jmethodID id) const
{
    JNIEnv *env = qtjambi_current_environment();
    Q_ASSERT(env);
    QTJAMBI_EXCEPTION_CHECK(env);

    jstring str = (jstring) env->CallObjectMethod(m_object, id);

    if (str == 0)
        return QString();

    QTJAMBI_EXCEPTION_CHECK(env);
    QString qstring = qtjambi_to_qstring(env, str);
    QTJAMBI_EXCEPTION_CHECK(env);
    return qstring;
}

QWidget *JambiCustomWidget::createWidget(QWidget *parent)
{
    JNIEnv *env = qtjambi_current_environment();
    Q_ASSERT (env != 0);
    QTJAMBI_EXCEPTION_CHECK(env);

    jobject javaParent = qtjambi_from_qobject(env, parent, "QWidget", "org/qtjambi/qt/gui/");
    QTJAMBI_EXCEPTION_CHECK(env);

    jobject widget = env->CallObjectMethod(m_object, method_createWidget, javaParent);
    QTJAMBI_EXCEPTION_CHECK(env);

    QWidget *qwidget = qobject_cast<QWidget*>(qtjambi_to_qobject (env, widget));
    QTJAMBI_EXCEPTION_CHECK(env);

    // Designer assumes the widget always has a parent (if a parent is actually passed in.)
    if (qwidget->parent() == 0 && parent != 0)
        qwidget->setParent(parent, qwidget->windowFlags() & ~Qt::Window);

    return qwidget;
}

QString JambiCustomWidget::domXml() const
{
    QString objectName = callStringMethod(method_name);
    objectName[0] = objectName[0].toLower();

    const char *xml_data = "<widget class=\"%1\"  name=\"%2\">"
                           "  <property name=\"objectName\">"
                           "    <string notr=\"true\">%3</string>"
                           "  </property>"
                           "</widget>";

    return QString::fromUtf8(xml_data)
        .arg(name())
        .arg(objectName)
        .arg(objectName);
}

QString JambiCustomWidget::name() const
{
    JNIEnv *env = qtjambi_current_environment();
    jclass cl = (jclass) env->CallObjectMethod(m_object, method_pluginClass);
    return qtjambi_class_name(env, cl).split(".").last();
}

bool JambiCustomWidget::isContainer() const
{
    JNIEnv *env = qtjambi_current_environment();
    return env->CallBooleanMethod(m_object, method_isContainer);
}

QString JambiCustomWidget::group() const
{
    return callStringMethod(method_group);
}

QString JambiCustomWidget::toolTip() const
{
    return callStringMethod(method_tooltip);
}

QString JambiCustomWidget::whatsThis() const
{
    return callStringMethod(method_whatsThis);
}

QString JambiCustomWidget::includeFile() const
{
    return callStringMethod(method_includeFile);
}

QIcon JambiCustomWidget::icon() const
{
    JNIEnv *env = qtjambi_current_environment();
    jobject javaIcon = env->CallObjectMethod(m_object, method_icon);

    qtjambi_exception_check(env);

    if (javaIcon == 0)
        return QIcon();

    QIcon i = * (QIcon *) qtjambi_to_object(env, javaIcon);
    return i;
}

JambiCustomWidgetCollection::JambiCustomWidgetCollection()
{
    if(qtjambi_initialize_vm()){

        JNIEnv *env = qtjambi_current_environment();
        Q_ASSERT (env != 0);

        jclass cl = qtjambi_find_class(env, "org/qtjambi/tools/designer/CustomWidgetManager");
        if (qtjambi_exception_check(env))
            return;

        jmethodID method_instance = env->GetStaticMethodID(cl, "instance", "()Lorg/qtjambi/tools/designer/CustomWidgetManager;");
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(method_instance);

        m_id_customWidgets = env->GetMethodID(cl, "customWidgets", "()Ljava/util/List;");
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(m_id_customWidgets);

        m_manager = env->NewGlobalRef(env->CallStaticObjectMethod(cl, method_instance));
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(m_manager);

        initializeWidgets(env);
        m_id_loadPlugins = env->GetMethodID(cl, "loadPlugins", "(Ljava/lang/String;)V");
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(m_id_loadPlugins);

        env->DeleteLocalRef(cl);
    }
}

void JambiCustomWidgetCollection::initializeWidgets(JNIEnv *env)
{
    m_widgets.clear();

    jobject widgetList = env->CallObjectMethod(m_manager, m_id_customWidgets);
    jobjectArray widgetArray = qtjambi_collection_toArray(env, widgetList);

    int length = env->GetArrayLength(widgetArray);
    for (int i=0; i<length; ++i) {
        jobject widget = env->GetObjectArrayElement(widgetArray, i);
        m_widgets << new JambiCustomWidget(widget);
    }

    env->DeleteLocalRef(widgetArray);
    env->DeleteLocalRef(widgetList);
}

void JambiCustomWidgetCollection::loadPlugins(const QString &path, QObject *widgetFactory)
{
    JNIEnv *env = qtjambi_current_environment();

    jstring jPath = qtjambi_from_qstring(env, path);
    Q_ASSERT(jPath);

    env->CallVoidMethod(m_manager, m_id_loadPlugins, jPath);
    initializeWidgets(env);

    env->DeleteLocalRef(jPath);

    // Make sure widget factory is updated. This depends on an internal implementation, but
    // there doesn't seem to be any public api doing the same.
    QMetaObject::invokeMethod(widgetFactory, "loadPlugins");
}

JambiCustomWidgetCollection::~JambiCustomWidgetCollection()
{
    JNIEnv *env = qtjambi_current_environment();
    env->DeleteGlobalRef(m_manager);
}


QList<QDesignerCustomWidgetInterface *> JambiCustomWidgetCollection::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(JambiCustomWidgetCollection)
#endif

// Q_EXPORT_PLUGIN2(JambiCustomWidget, JambiCustomWidget)
