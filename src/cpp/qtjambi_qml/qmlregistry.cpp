/****************************************************************************
**
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


#include <QtQml>
#include "qmlregistry.h"
#include <QtQml/qqmlprivate.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambitypemanager_p.h>

class QQmlParserStatus;
class QQmlPropertyValueSource;
class QQmlPropertyValueInterceptor;

typedef void(*CreatorFunction)(void*);

CreatorFunction creatorFunction(JNIEnv * env, jclass clazz){
    return 0;
}

QQmlAttachedPropertiesFunc attachedPropertiesFunc(JNIEnv * env, jclass clazz){
    // QQmlPrivate::attachedPropertiesFunc<T>()
    return 0;
}

QMetaObject * attachedPropertiesMetaObject(JNIEnv * env, jclass clazz){
    //QQmlPrivate::attachedPropertiesMetaObject<T>(),
    return 0;
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2
  (JNIEnv * env, jclass, jclass clazz){
    QString javaName = qtjambi_class_name(env, clazz).replace('.', '/');
    QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
    QString className = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);

    QString listName("QQmlListProperty<" + className + '>');
    className += "*";

    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, 0);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, 0);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
        }
    }

    QQmlPrivate::RegisterType type = {
        0,

        qtjambiQmlRegisterNormalizedMetaType(className.toLatin1().constData(), env, clazz),
        qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),
        0, 0,
        QString(),

        0, 0, 0, 0, meta_object,

        attachedPropertiesFunc(env, clazz),
        attachedPropertiesMetaObject(env, clazz),

        QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlParserStatus>::cast(),
        QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueSource>::cast(),
        QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueInterceptor>::cast(),

        0, 0,

        0,
        0
    };

    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterTypeNotAvailable
 * Signature: (Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterTypeNotAvailable
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring message){
    QString _uri = qtjambi_to_qstring(env, uri);
    QString _qmlName = qtjambi_to_qstring(env, qmlName);
    QString _message = qtjambi_to_qstring(env, message);
    return qmlRegisterTypeNotAvailable(_uri.toLatin1().constData(), (int) versionMajor, (int) versionMinor, _qmlName.toLatin1().constData(), _message);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
      QString javaName = qtjambi_class_name(env, clazz).replace('.', '/');
      QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
      QString className = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);
      javaName = javaName.replace("/", "::");

      QString _uri = qtjambi_to_qstring(env, uri);
      QString _qmlName = qtjambi_to_qstring(env, qmlName);
      QString _reason = qtjambi_to_qstring(env, reason);

      QString listName("QQmlListProperty<" + className + '>');
      className += "*";

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, 0);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, 0);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }

      QQmlPrivate::RegisterType type = {
          1,

          qtjambiQmlRegisterNormalizedMetaType(className.toLatin1().constData(), env, clazz),
          qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),
          0, 0,
          _reason,

          _uri.toLatin1().constData(), (int)versionMajor, (int)versionMinor, _qmlName.toLatin1().constData(), meta_object,

          attachedPropertiesFunc(env, clazz),
          attachedPropertiesMetaObject(env, clazz),

          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlParserStatus>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueSource>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueInterceptor>::cast(),

          0, 0,

          0,
          0
      };

      return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
      QString javaName = qtjambi_class_name(env, clazz).replace('.', '/');
      QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
      QString className = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);
      javaName = javaName.replace("/", "::");

      QString _uri = qtjambi_to_qstring(env, uri);
      QString _qmlName = qtjambi_to_qstring(env, qmlName);
      QString _reason = qtjambi_to_qstring(env, reason);

      QString listName("QQmlListProperty<" + className + '>');
      className += "*";

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, 0);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, 0);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }

      QQmlPrivate::RegisterType type = {
          1,

          qtjambiQmlRegisterNormalizedMetaType(className.toLatin1().constData(), env, clazz),
          qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),
          0, 0,
          _reason,

          _uri.toLatin1().constData(), (int)versionMajor, (int)versionMinor, _qmlName.toLatin1().constData(), meta_object,

          attachedPropertiesFunc(env, clazz),
          attachedPropertiesMetaObject(env, clazz),

          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlParserStatus>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueSource>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueInterceptor>::cast(),

          0, 0,

          0,
          (int)metaObjectRevision
      };

      return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
      QString _qmlName = qtjambi_to_qstring(env, qmlName);
      QString _uri = qtjambi_to_qstring(env, uri);

      QString javaName = qtjambi_class_name(env, clazz).replace('.', '/');
      QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
      QString className = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);
      javaName = javaName.replace("/", "::");

      QString listName("QQmlListProperty<" + className + '>');
      className += "*";

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, 0);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, 0);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }

      QQmlPrivate::RegisterType type = {
          1,

          qtjambiQmlRegisterNormalizedMetaType(className.toLatin1().constData(), env, clazz),
          qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),
          sizeof(void*), creatorFunction(env, clazz),
          QString(),

          _uri.toLatin1().constData(), (int)versionMajor, (int)versionMinor, _qmlName.toLatin1().constData(), meta_object,

          attachedPropertiesFunc(env, clazz),
          attachedPropertiesMetaObject(env, clazz),

          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlParserStatus>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueSource>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueInterceptor>::cast(),

          0, 0,

          0,
          0
      };

      return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
      QString _qmlName = qtjambi_to_qstring(env, qmlName);
      QString _uri = qtjambi_to_qstring(env, uri);

      QString javaName = qtjambi_class_name(env, clazz).replace('.', '/');
      QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
      QString className = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);
      javaName = javaName.replace("/", "::");

      QString listName("QQmlListProperty<" + className + '>');
      className += "*";

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, 0);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, 0);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }

      QQmlPrivate::RegisterType type = {
          1,

          qtjambiQmlRegisterNormalizedMetaType(className.toLatin1().constData(), env, clazz),
          qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),
          sizeof(void*), creatorFunction(env, clazz),
          QString(),

          _uri.toLatin1().constData(), (int)versionMajor, (int)versionMinor, _qmlName.toLatin1().constData(), meta_object,

          attachedPropertiesFunc(env, clazz),
          attachedPropertiesMetaObject(env, clazz),

          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlParserStatus>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueSource>::cast(),
          QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueInterceptor>::cast(),

          0, 0,

          0,
          (int)metaObjectRevision
      };

      return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterRevision
 * Signature: (Ljava/lang/Class;ILjava/lang/String;II)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterRevision
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor){

    QString javaName = qtjambi_class_name(env, clazz).replace('.', '/');
    QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
    QString className = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);
    javaName = javaName.replace("/", "::");

    QString _uri = qtjambi_to_qstring(env, uri);

    QString listName("QQmlListProperty<" + className + '>');
    className += "*";

    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, 0);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, 0);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
        }
    }

    QQmlPrivate::RegisterType type = {
        1,

        qtjambiQmlRegisterNormalizedMetaType(className.toLatin1().constData(), env, clazz),
        qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),
        sizeof(void*), creatorFunction(env, clazz),
        QString(),

        _uri.toLatin1().constData(), (int)versionMajor, (int)versionMinor, 0, meta_object,

        attachedPropertiesFunc(env, clazz),
        attachedPropertiesMetaObject(env, clazz),

        QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlParserStatus>::cast(),
        QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueSource>::cast(),
        QQmlPrivate::StaticCastSelector<JObjectWrapper,QQmlPropertyValueInterceptor>::cast(),

        0, 0,

        0,
        (int)metaObjectRevision
    };

    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz){
    env->ThrowNew(env->FindClass("java/lang/Exception"), "Not yet implemented");
    return -1;
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring, jint, jint, jstring){
    env->ThrowNew(env->FindClass("java/lang/Exception"), "Not yet implemented");
    return -1;
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterInterface
 * Signature: (Ljava/lang/Class;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterInterface
  (JNIEnv * env, jclass, jclass clazz, jstring name){
    QString typeName = qtjambi_to_qstring(env, name);
    QString listName("QQmlListProperty<" + typeName + '>');
    typeName += "*";

    QQmlPrivate::RegisterInterface qmlInterface = {
        0,

        qtjambiQmlRegisterNormalizedMetaType(typeName.toLatin1().constData(), env, clazz),
        qRegisterNormalizedMetaType<QQmlListProperty<JObjectWrapper> >(listName.toLatin1()),

        0//qobject_interface_iid<T *>()
    };

    return QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlAttachedPropertiesObject
 * Signature: (Lorg/qtjambi/qt/core/QObject;Z)Lorg/qtjambi/qt/core/QObject;
 */
JNIEXPORT jobject JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlAttachedPropertiesObject
  (JNIEnv * env, jclass, jobject, jboolean){
    env->ThrowNew(env->FindClass("java/lang/Exception"), "Not yet implemented");
    return jobject(0);
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterBaseTypes
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterBaseTypes
  (JNIEnv * env, jclass, jstring, jint, jint){
    env->ThrowNew(env->FindClass("java/lang/Exception"), "Not yet implemented");
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Lorg/qtjambi/qt/core/QUrl;Ljava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Lorg_qtjambi_qt_core_QUrl_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jobject, jstring, jint, jint, jstring){
	return -1;
}

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlProtectModule
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlProtectModule
  (JNIEnv * env, jclass, jstring, jint){
	return false;
}


/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (JLjava/lang/String;IILjava/lang/String;)I
 */
jint Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2JLjava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jlong url, jstring uri, jint versionMajor, jint versionMinor, jstring typeName){
    QString _typeName = qtjambi_to_qstring(env, typeName);
    QString _uri = qtjambi_to_qstring(env, uri);
    QUrl *_url = (QUrl *) qtjambi_from_jlong(url);

    return qmlRegisterType(*_url, _uri.toLatin1().constData(), (int)versionMajor, (int)versionMinor, _typeName.toLatin1().constData());
}
