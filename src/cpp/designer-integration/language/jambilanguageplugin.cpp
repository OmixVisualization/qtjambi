/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of the $MODULE$ of the Qt Toolkit.
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

#include "jambilanguageplugin.h"

#include "qtjambiintrospection_p.h"

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_utils.h>

#include <QtCore/QtDebug>
#include <QtCore/QtPlugin>

#include <private/ui4_p.h>

#include <QtGui/QtGui>
#if QT_VERSION < 0x050000
#include <QtGui/QMessageBox>
#else
#include <QtWidgets/QMessageBox>
#endif

jclass class_ResourceBrowser;

jmethodID method_ResourceBrowser;
jmethodID method_signalMatchesSlot;

static ClassData jni_class_table[] = {
    { &class_ResourceBrowser, "org/qtjambi/tools/designer/ResourceBrowser" },
    { 0, 0 }
};

static MethodData jni_method_table[] = {
    { &class_ResourceBrowser, &method_ResourceBrowser, "<init>", "(Lorg/qtjambi/qt/gui/QWidget;)V" },
    { 0, 0, 0, 0 }
};

static MethodData jni_static_method_table[] = {
    { 0, 0, 0, 0 }
};

JambiLanguagePlugin::JambiLanguagePlugin():
    m_core(0),
    m_vmLoaded(false)
{
    if (qtjambi_initialize_vm()) {
        JNIEnv *env = qtjambi_current_environment();

        bool rv_c  = qtjambi_resolve_classes(env, jni_class_table);
        bool rv_m  = qtjambi_resolve_methods(env, false, jni_method_table);
        bool rv_sm = qtjambi_resolve_methods(env, true, jni_static_method_table);
        if(rv_c && rv_m && rv_sm)
            m_vmLoaded = true;

        if(!m_vmLoaded)
            qWarning("Qt Jambi: Cannot load JambiLanguagePlugin due to missing class files");
    } else {
       QMessageBox::information(0, tr("Qt Jambi Plugin"),
                          tr("Could not locate the java virtual machine.\n\nQt Jambi plugins have been disabled."),
                          QMessageBox::Close);
    }
}

JambiLanguagePlugin::~JambiLanguagePlugin()
{
}

bool JambiLanguagePlugin::isInitialized() const
{
    return m_core != 0;
}

void JambiLanguagePlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (!m_vmLoaded)
        return;

    if (m_core)
        return;

    m_core = core;
    m_core->setIntrospection(new QtJambiIntrospection);

    QExtensionManager *mgr = m_core->extensionManager();
    Q_ASSERT (mgr != 0);

    mgr->registerExtensions(new JambiExtensionFactory(this, mgr), Q_TYPEID(QDesignerLanguageExtension));
    mgr->registerExtensions(new JambiExtensionFactory(this, mgr), Q_TYPEID(QDesignerExtraInfoExtension));
}

QAction *JambiLanguagePlugin::action() const
{
    return 0;
}

QDesignerFormEditorInterface *JambiLanguagePlugin::core() const
{
    return m_core;
}

JambiLanguage::JambiLanguage(QObject *parent)
    : QObject(parent)
{
}

JambiLanguage::~JambiLanguage()
{
}

static QMessageBox *no_feature(QWidget *parent)
{
    QMessageBox *box = new QMessageBox(QMessageBox::Information,
                                       QLatin1String("Promote to Custom Widget"),
                                       QLatin1String("This feature is not availble in Qt Jambi"),
                                       QMessageBox::Ok,
                                       parent);
    return box;

}

QDialog *JambiLanguage::createPromotionDialog(QDesignerFormEditorInterface *,
                                              const QString &,
                                              QString *,
                                              QWidget *parent)
{
    return no_feature(parent);
}

QDialog *JambiLanguage::createPromotionDialog(QDesignerFormEditorInterface *,QWidget *parent)
{
    return no_feature(parent);
}


QDialog *JambiLanguage::createFormWindowSettingsDialog(QDesignerFormWindowInterface *, QWidget *)
{
    return 0;
}

QString JambiLanguage::classNameOf(QObject *object) const
{
    if (object == 0)
        return QString();

    QtJambiLink *link = QtJambiLink::findLinkForQObject(object);
    if (link && link->createdByJava()) {
        JNIEnv *env = qtjambi_current_environment();
        jobject jobj = link->javaObject(env);
        QString name = qtjambi_class_name(env, env->GetObjectClass(jobj)).split(".").last();
        return name;
    }
    return object->metaObject()->className();
}

QString JambiLanguage::enumerator(const QString &name) const
{
    return name;
}

QString JambiLanguage::neutralEnumerator(const QString &name) const
{
    return name;
}

QDesignerResourceBrowserInterface *JambiLanguage::createResourceBrowser(QWidget *parentWidget)
{
    JNIEnv *env = qtjambi_current_environment();
    jobject jParent = qtjambi_from_QWidget(env, parentWidget);
    QTJAMBI_EXCEPTION_CHECK(env);

    if (class_ResourceBrowser == 0 || method_ResourceBrowser == 0)
        return 0;

    jobject jWidget = env->NewObject(class_ResourceBrowser, method_ResourceBrowser, jParent);
    QTJAMBI_EXCEPTION_CHECK(env);

    QObject *widget = qtjambi_to_qobject(env, jWidget);
    QTJAMBI_EXCEPTION_CHECK(env);

    QDesignerResourceBrowserInterface *iface =
        qobject_cast<QDesignerResourceBrowserInterface *>(widget);
    Q_ASSERT(iface);

    return iface;
}

bool JambiLanguage::isLanguageResource(const QString &path) const
{
    return path.startsWith("classpath:");
}


bool JambiLanguage::signalMatchesSlot(const QString &signal, const QString &slot) const
{
    // Similar logic to the qdesigner_membersheet.cpp which allows
    // "partial" connections to be valid in the signal/slot editor
    // window.
    if (signal == QLatin1String("<signal>") || slot == QLatin1String("<slot>"))
        return true;

    JNIEnv *env = qtjambi_current_environment();

    jstring signalString = qtjambi_from_qstring(env, signal);
    QTJAMBI_EXCEPTION_CHECK(env);

    jstring slotString = qtjambi_from_qstring(env, slot);
    QTJAMBI_EXCEPTION_CHECK(env);

    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiInternal();

    return env->CallStaticBooleanMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.signalMatchesSlot,
                                        signalString, slotString);
}

QString JambiLanguage::widgetBoxContents() const
{
    QFile f(":/jambiwidgetbox.xml");
    if (f.open(QFile::ReadOnly)) {
        return QString::fromLatin1(f.readAll());
    }
    qWarning("JambiLanguage::widgetBoxContents(), Failed to load widget box contents...");
    return QString();
}


JambiExtraInfoExtension::JambiExtraInfoExtension(QWidget *widget,
                                                 QDesignerFormEditorInterface *core)
    : m_widget(widget),
      m_core(core)
{
}


bool JambiExtraInfoExtension::saveUiExtraInfo(DomUI *ui)
{
    ui->setAttributeLanguage(QLatin1String("jambi"));
    return false;
}

void JambiExtraInfoExtension::fiddleWith(DomProperty *property)
{
    if (property->hasAttributeName() && property->attributeName() == QLatin1String("orientation")
        && property->kind() == DomProperty::Enum) {
        if (property->elementEnum() == QLatin1String("Qt::Horizontal")) {
            property->setElementEnum(QLatin1String("org.qtjambi.qt.core.Qt.Orientation.Horizontal"));
        } else if (property->elementEnum() == QLatin1String("Qt::Vertical")) {
            property->setElementEnum(QLatin1String("org.qtjambi.qt.core.Qt.Orientation.Vertical"));
        }
    }
}

void JambiExtraInfoExtension::fiddleWith(DomSpacer *spacer)
{
    QList<DomProperty *> properties = spacer->elementProperty();
    foreach (DomProperty *property, properties)
        fiddleWith(property);
}

void JambiExtraInfoExtension::fiddleWith(DomWidget *widget)
{
    QList<DomWidget *> widgets = widget->elementWidget();
    foreach (DomWidget *w, widgets)
        fiddleWith(w);

    QList<DomLayout *> layouts = widget->elementLayout();
    foreach (DomLayout *layout, layouts)
        fiddleWith(layout);
}

void JambiExtraInfoExtension::fiddleWith(DomLayoutItem *layoutItem)
{
    DomSpacer *spacer = layoutItem->elementSpacer();
    if (spacer != 0)
        fiddleWith(spacer);

    DomWidget *widget = layoutItem->elementWidget();
    if (widget != 0)
        fiddleWith(widget);

    DomLayout *layout = layoutItem->elementLayout();
    if (layout != 0)
        fiddleWith(layout);
}

void JambiExtraInfoExtension::fiddleWith(DomLayout *layout)
{
    QList<DomLayoutItem *> layoutItems = layout->elementItem();
    foreach (DomLayoutItem *layoutItem, layoutItems)
        fiddleWith(layoutItem);
}

bool JambiExtraInfoExtension::loadUiExtraInfo(DomUI *ui)
{
    if (ui->hasAttributeLanguage()
        && ui->attributeLanguage().toLower() != QLatin1String("jambi")) {
        QMessageBox::warning(0,
                             QLatin1String("Incompatible UI file"),
                             QLatin1String("The UI file that is being loaded does not contain a "
                                           "language attribute or the language attribute is "
                                           "not <code>jambi</code>. The form may not load "
                                           "properly")
                             );
        return false;
    }

    if (ui->hasElementWidget() && ui->elementWidget() != 0)
        fiddleWith(ui->elementWidget());

    return true;
}


bool JambiExtraInfoExtension::saveWidgetExtraInfo(DomWidget *ui_widget)
{
    Q_UNUSED(ui_widget)
    return false;
}


bool JambiExtraInfoExtension::loadWidgetExtraInfo(DomWidget *ui_widget)
{
    Q_UNUSED(ui_widget)
    return false;
}



JambiExtensionFactory::JambiExtensionFactory(JambiLanguagePlugin *plugin, QExtensionManager *parent):
    QExtensionFactory(parent),
    m_jambi(plugin)
{
}


JambiExtensionFactory::~JambiExtensionFactory()
{
}


QObject *JambiExtensionFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    if (iid == Q_TYPEID(QDesignerLanguageExtension) && qobject_cast<QDesignerFormEditorInterface*> (object)) {
        return new JambiLanguage(parent);

    } else if (iid == Q_TYPEID(QDesignerExtraInfoExtension)) {
        QWidget *w = qobject_cast<QWidget *>(object);
        return new JambiExtraInfoExtension(w, m_jambi->core());
    }

    return 0;
}
