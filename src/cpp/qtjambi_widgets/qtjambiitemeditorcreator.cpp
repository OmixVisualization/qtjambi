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

#include <QtWidgets/QItemEditorCreator>
#include <QtWidgets/QWidget>

#include <qtjambi/qtjambilink.h>
#include <qtjambi/qtjambi_core.h>

class JavaWidgetCreator
{
public:
    JavaWidgetCreator(JNIEnv *env, jclass widgetType)
        : m_widget_type(reinterpret_cast<jclass>(env->NewGlobalRef(widgetType))), m_constructor(0)
    {
        m_constructor = env->GetMethodID(m_widget_type, "<init>", "(Lorg/qtjambi/qt/widgets/QWidget;)V");
    }

    virtual ~JavaWidgetCreator()
    {
        JNIEnv *env = qtjambi_current_environment();
        if (env != 0 && m_widget_type != 0)
            env->DeleteGlobalRef(m_widget_type);
    }

    QWidget *createJavaWidget(JNIEnv *env, QWidget *parent) const
    {
        if (m_constructor == 0)
            return 0;

        jobject java_parent = qtjambi_from_QWidget(env, parent);
        jobject java_widget = env->NewObject(m_widget_type, m_constructor, java_parent);

        return static_cast<QWidget *>(qtjambi_to_qobject(env, java_widget));
    }

private:
    jclass m_widget_type;
    jmethodID m_constructor;
};

template <typename T>
class QtJambiItemEditorCreatorBase: public T, public JavaWidgetCreator
{
public:
    QtJambiItemEditorCreatorBase(const QByteArray &valuePropertyName, JNIEnv *env, jclass widgetType, bool createWidgetOverride, bool valuePropertyNameOverride)
        : T(valuePropertyName),
          JavaWidgetCreator(env, widgetType),
          m_create_widget_overridden(createWidgetOverride),
          m_value_property_name_overridden(valuePropertyNameOverride)
    {
    }


    virtual QWidget *createWidget(QWidget *parent) const
    {
        JNIEnv *env = qtjambi_current_environment();
        if (env == 0)
            return T::createWidget(parent);

        if (m_create_widget_overridden) {

            StaticCache *sc = StaticCache::instance();
            sc->resolveQItemEditorCreatorBase();

            env->PushLocalFrame(100);
            jobject __java_parent = qtjambi_from_QWidget(env, parent);
            jobject __java_return_value = env->CallObjectMethod(m_link->javaObject(env), sc->QItemEditorCreatorBase.createWidget, __java_parent);
            qtjambi_exception_check(env);

            QWidget *__qt_return_value = static_cast<QWidget *>(qtjambi_to_qobject(env, __java_return_value));
            env->PopLocalFrame(0);
            return __qt_return_value;
        } else {
            QWidget *w = createJavaWidget(env, parent);
            qtjambi_exception_check(env);

            return w;
        }
    }

    virtual QByteArray valuePropertyName() const
    {
        if (m_value_property_name_overridden) {
            JNIEnv *env = qtjambi_current_environment();
            if (env == 0)
                return T::valuePropertyName();

            StaticCache *sc = StaticCache::instance();
            sc->resolveQItemEditorCreatorBase();

            env->PushLocalFrame(100);

            jobject __java_return_value = env->CallObjectMethod(m_link->javaObject(env), sc->QItemEditorCreatorBase.valuePropertyName);
            qtjambi_exception_check(env);

            QByteArray __qt_return_value = * (QByteArray *) qtjambi_to_object(env, __java_return_value);

            env->PopLocalFrame(0);
            return __qt_return_value;
        } else {
            return T::valuePropertyName();
        }
    }

public:
    QtJambiLink *m_link;

private:
    uint m_create_widget_overridden : 1;
    uint m_value_property_name_overridden : 1;
    uint m_reserved : 30;

};

class QtJambiItemEditorCreator: public QtJambiItemEditorCreatorBase<QItemEditorCreator<QWidget> >
{
public:
     QtJambiItemEditorCreator(const QByteArray &valuePropertyName, JNIEnv *env, jclass widgetType, bool createWidgetOverride, bool valuePropertyNameOverride)
         : QtJambiItemEditorCreatorBase<QItemEditorCreator<QWidget> > (valuePropertyName, env, widgetType, createWidgetOverride, valuePropertyNameOverride)
     {
     }
};

// Quick-n-easy to match the constructor requirement of QtJambiItemEditorCreatorBase
template <typename T>
class __StandardItemEditorCreator: public QStandardItemEditorCreator<QWidget>
{
public:
    __StandardItemEditorCreator(const QByteArray &)
        : QStandardItemEditorCreator<QWidget>()
    { }

};

class QtJambiStandardItemEditorCreator: public QtJambiItemEditorCreatorBase<__StandardItemEditorCreator<QWidget> >
{
public:
     QtJambiStandardItemEditorCreator(JNIEnv *env, jclass widgetType, bool createWidgetOverride, bool valuePropertyNameOverride)
         : QtJambiItemEditorCreatorBase<__StandardItemEditorCreator<QWidget> > (QByteArray(), env, widgetType, createWidgetOverride, valuePropertyNameOverride)
     {
     }

};

//
// QItemEditorCreator
//

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_widgets_QItemEditorCreator__1_1qt_1QItemEditorCreator)
(JNIEnv *__jni_env,
 jobject __jni_object,
 jlong valuePropertyNameId,
 jclass widgetType,
 jboolean createWidgetOverride,
 jboolean valuePropertyNameOverride)
{
    QByteArray *valuePropertyName = reinterpret_cast<QByteArray *>(qtjambi_from_jlong(valuePropertyNameId));

    QtJambiItemEditorCreator *__qt_this = new QtJambiItemEditorCreator(*valuePropertyName, __jni_env, widgetType,
        bool(createWidgetOverride), bool(valuePropertyNameOverride));
    QtJambiLink *__qt_java_link = qtjambi_construct_object(__jni_env,
                                                           __jni_object,
                                                           __qt_this,
#if QT_VERSION < 0x050000
                                                           QMetaType::Void,
#else
                                                           QMetaType::UnknownType,
#endif
                                                           QLatin1String("org/qtjambi/qt/widgets/QItemEditorCreator"),
                                                           true);
    if (!__qt_java_link) {
        qWarning("object construction failed for type: QItemEditorCreator");
        return;
    }
    __qt_this->m_link = __qt_java_link;
    __qt_this->m_link->setCreatedByJava(true);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_widgets_QItemEditorCreator__1_1qt_1createWidget)
(JNIEnv *__jni_env,
 jobject ,
 jlong nativeId,
 jlong parentId)
{
    QWidget *parent = reinterpret_cast<QWidget *>(qtjambi_from_jlong(parentId));
    QtJambiItemEditorCreator *__qt_this = reinterpret_cast<QtJambiItemEditorCreator *>(qtjambi_from_jlong(nativeId));

    QWidget *widget = __qt_this->createJavaWidget(__jni_env, parent);
    return qtjambi_from_QWidget(__jni_env, widget);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_widgets_QItemEditorCreator__1_1qt_1valuePropertyName)
(JNIEnv *__jni_env,
 jobject ,
 jlong nativeId)
{
    QtJambiItemEditorCreator *__qt_this = reinterpret_cast<QtJambiItemEditorCreator *>(qtjambi_from_jlong(nativeId));

    QByteArray valuePropertyName = __qt_this->QItemEditorCreator<QWidget>::valuePropertyName();
    return qtjambi_from_object(__jni_env, &valuePropertyName, "QByteArray", "org/qtjambi/qt/core/", true);
}

//
// QStandardItemEditorCreator
//

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_widgets_QStandardItemEditorCreator__1_1qt_1QStandardItemEditorCreator)
(JNIEnv *__jni_env,
 jobject __jni_object,
 jclass widgetType,
 jboolean createWidgetOverride,
 jboolean valuePropertyNameOverride)
{
    QtJambiStandardItemEditorCreator *__qt_this = new QtJambiStandardItemEditorCreator(__jni_env, widgetType,
        bool(createWidgetOverride), bool(valuePropertyNameOverride));
    QtJambiLink *__qt_java_link = qtjambi_construct_object(__jni_env,
                                                           __jni_object,
                                                           __qt_this,
#if QT_VERSION < 0x050000
                                                           QMetaType::Void,
#else
                                                           QMetaType::UnknownType,
#endif
                                                           QLatin1String("org/qtjambi/qt/widgets/QStandardItemEditorCreator"),
                                                           true);
    if (!__qt_java_link) {
        qWarning("object construction failed for type: QStandardItemEditorCreator");
        return;
    }
    __qt_this->m_link = __qt_java_link;
    __qt_this->m_link->setCreatedByJava(true);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_widgets_QStandardItemEditorCreator__1_1qt_1createWidget)
(JNIEnv *__jni_env,
 jobject ,
 jlong nativeId,
 jlong parentId)
{
    QWidget *parent = reinterpret_cast<QWidget *>(qtjambi_from_jlong(parentId));
    QtJambiStandardItemEditorCreator *__qt_this = reinterpret_cast<QtJambiStandardItemEditorCreator *>(qtjambi_from_jlong(nativeId));

    QWidget *widget = __qt_this->createJavaWidget(__jni_env, parent);
    return qtjambi_from_QWidget(__jni_env, widget);
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_widgets_QStandardItemEditorCreator__1_1qt_1valuePropertyName)
(JNIEnv *__jni_env,
 jobject ,
 jlong nativeId)
{
    QtJambiStandardItemEditorCreator *__qt_this = reinterpret_cast<QtJambiStandardItemEditorCreator *>(qtjambi_from_jlong(nativeId));

    QByteArray valuePropertyName = __qt_this->QStandardItemEditorCreator<QWidget>::valuePropertyName();
    return qtjambi_from_object(__jni_env, &valuePropertyName, "QByteArray", "org/qtjambi/qt/core/", true);
}
