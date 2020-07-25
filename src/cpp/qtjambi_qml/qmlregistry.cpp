/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtQml>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include "qtjambi_jarimport.h"
#include "qmlcreateparentfunction.h"
#include "qmlattachedpropertiesfunction.h"
#include "qmlcreatorfunction.h"

#include <QtQml/qqmlprivate.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include <qtjambi/qtjambi_qml.h>
#include "qtjambi_qml_repository.h"
#include <qtjambi/qtjambi_interfaces.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_functionpointer.h>

class QQmlParserStatus;
class QQmlPropertyValueSource;
class QQmlPropertyValueInterceptor;

const QMetaObject * attachedPropertiesMetaObject(JNIEnv * env, jobject method){
    if(!method)
        return nullptr;
    jclass returnType = jclass(Java::Private::Runtime::Method.getReturnType(env, method));
    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, returnType, nullptr);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, returnType);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
            meta_object = qtjambi_metaobject_for_class(env, returnType, original_meta_object);
        }
    }
    return meta_object;
}

int parserStatusCast(JNIEnv * env, jclass clazz){
    return qtjambi_interface_offset(env, clazz, typeid(QQmlParserStatus));
}

int valueSourceCast(JNIEnv * env, jclass clazz){
    return qtjambi_interface_offset(env, clazz, typeid(QQmlPropertyValueSource));
}

int valueInterceptorCast(JNIEnv *, jclass){
    return -1;
}

int registerQmlListType(QString javaName){
    QString listName("QQmlListProperty<" + javaName.replace(".", "::").replace("/", "::") + '>');
    return QMetaType::registerNormalizedType(listName.toLatin1(),
                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<QQmlListProperty<QObject> >::Destruct,
                                      [](void * ptr, const void * copy)->void*{
                                            const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                            QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                            if(_copy){
                                                list->append = _copy->append;
                                                list->at = _copy->at;
                                                list->clear = _copy->clear;
                                                list->count = _copy->count;
                                                list->data = _copy->data;
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                                                list->dummy1 = _copy->dummy1;
                                                list->dummy2 = _copy->dummy2;
#else
                                                list->replace = _copy->replace;
                                                list->removeLast = _copy->removeLast;
#endif
                                                list->object = _copy->object;
                                            }
                                            return list;
                                      },
                                      sizeof(QQmlListProperty<QObject>),
                                      QMetaType::MovableType | QMetaType::NeedsConstruction | QMetaType::NeedsDestruction,
                                      nullptr
                                      );
}

/**
 * this method is used by the qml wrapper
 */
int registerQmlMetaType(QString javaName, const QMetaObject *meta_object)
{
    javaName = javaName.replace(".", "::")+"*";
    QByteArray _javaName = javaName.toLatin1();

    int definedType = QMetaType::type(_javaName);
    if(definedType!=QMetaType::UnknownType){
        return definedType;
    }
    return QMetaType::registerNormalizedType(_javaName,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Destruct,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Construct,
                                     sizeof(QObject*),
                                     QMetaType::TypeFlags( QMetaType::MovableType | QMetaType::PointerToQObject ),
                                     meta_object);
}

jmethodID findConstructor(JNIEnv * env, jclass clazz, const QString& javaName){
    jmethodID constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
    if(env->ExceptionCheck())
        env->ExceptionClear();
    jmethodID superClassConstructor = nullptr;
    jclass generatedSuperclass = qtjambi_find_generated_superclass(env, clazz);
    if(generatedSuperclass){
        superClassConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
        if(env->ExceptionCheck())
            env->ExceptionClear();
    }else{
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class %1 cannot be registered as Qml type since it does not inherit a Qt class.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
    }
    if(!constructor){
        if(superClassConstructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QDeclarativeConstructor) to register as Qml type.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }else{
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))).arg(superClassName)) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if(!superClassConstructor){
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))).arg(superClassName)) QTJAMBI_STACKTRACEINFO );
        }
    }
    return constructor;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2
  (JNIEnv * env, jclass, jclass clazz){
    env->EnsureLocalCapacity(300);
    try{
        int objectSize = int(qtjambi_extended_size_for_class(env, clazz));
        QString javaName = qtjambi_class_name(env, clazz);
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ objectSize,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ nullptr,
            /*int versionMajor*/ 0,
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterTypeNotAvailable
 * Signature: (Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterTypeNotAvailable
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring message){
    try{
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QString _message = qtjambi_to_qstring(env, message);
        return qmlRegisterTypeNotAvailable(_uri.constData(), int(versionMajor), int(versionMinor), _qmlName.constData(), _message);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    try{
      QString javaName = qtjambi_class_name(env, clazz);
      QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
      QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }
      jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

      QQmlPrivate::RegisterType type = {
          /* int version */ 1,

          /*int typeId*/ registerQmlMetaType(javaName, meta_object),
          /*int listId*/ registerQmlListType(javaName),
          /*int objectSize*/ 0,
          /*void (*create)(void *)*/ nullptr,
          /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

          /*const char *uri*/ _uri.constData(),
          /*int versionMajor*/ int(versionMajor),
          /*int versionMinor*/ int(versionMinor),
          /*const char *elementName*/ _qmlName.constData(),
          /*const QMetaObject *metaObject*/ meta_object,

          /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
          /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

          /*int parserStatusCast*/ parserStatusCast(env, clazz),
          /*int valueSourceCast*/ valueSourceCast(env, clazz),
          /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

          /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
          /*const QMetaObject *extensionMetaObject*/ nullptr,

          /*QQmlCustomParser *customParser*/ nullptr,
          /*int revision*/ 0
      };

      return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    try{
      QString javaName = qtjambi_class_name(env, clazz);
      QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
      QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }
      jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

      QQmlPrivate::RegisterType type = {
          /* int version */ 1,

          /*int typeId*/ registerQmlMetaType(javaName, meta_object),
          /*int listId*/ registerQmlListType(javaName),
          /*int objectSize*/ 0,
          /*void (*create)(void *)*/ nullptr,
          /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

          /*const char *uri*/ _uri.constData(),
          /*int versionMajor*/ int(versionMajor),
          /*int versionMinor*/ int(versionMinor),
          /*const char *elementName*/ _qmlName.constData(),
          /*const QMetaObject *metaObject*/ meta_object,

          /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
          /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

          /*int parserStatusCast*/ parserStatusCast(env, clazz),
          /*int valueSourceCast*/ valueSourceCast(env, clazz),
          /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

          /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
          /*const QMetaObject *extensionMetaObject*/ nullptr,

          /*QQmlCustomParser *customParser*/ nullptr,
          /*int revision*/ int(metaObjectRevision)
      };

      return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    try{
          size_t objectSize = qtjambi_extended_size_for_class(env, clazz);
          QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
          QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

          QString javaName = qtjambi_class_name(env, clazz);
          jmethodID constructor = findConstructor(env, clazz, javaName);

          const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
          if(!meta_object){
              jclass closestClass = resolveClosestQtSuperclass(env, clazz);
              if(closestClass){
                  const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                  meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
              }
          }
          jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
          int psCast = parserStatusCast(env, clazz);
          int vsCast = valueSourceCast(env, clazz);
          int viCast = valueInterceptorCast(env, clazz);

          QQmlPrivate::RegisterType type = {
              /* int version */ 1,

              /*int typeId*/ registerQmlMetaType(javaName, meta_object),
              /*int listId*/ registerQmlListType(javaName),
              /*int objectSize*/ int(objectSize),
              /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
              /*QString noCreationReason*/ QString(),

              /*const char *uri*/ _uri.constData(),
              /*int versionMajor*/ int(versionMajor),
              /*int versionMinor*/ int(versionMinor),
              /*const char *elementName*/ _qmlName.constData(),
              /*const QMetaObject *metaObject*/ meta_object,

              /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
              /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

              psCast,
              vsCast,
              viCast,

              /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
              /*const QMetaObject *extensionMetaObject*/ nullptr,

              /*QQmlCustomParser *customParser*/ nullptr,
              /*int revision*/ 0
          };

          return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    try{
          size_t objectSize = qtjambi_extended_size_for_class(env, clazz);
          QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
          QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

          QString javaName = qtjambi_class_name(env, clazz);
          jmethodID constructor = findConstructor(env, clazz, javaName);

          const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
          if(!meta_object){
              jclass closestClass = resolveClosestQtSuperclass(env, clazz);
              if(closestClass){
                  const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                  meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
              }
          }
          jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
          int psCast = parserStatusCast(env, clazz);
          int vsCast = valueSourceCast(env, clazz);
          int viCast = valueInterceptorCast(env, clazz);

          QQmlPrivate::RegisterType type = {
              /* int version */ 1,

              /*int typeId*/ registerQmlMetaType(javaName, meta_object),
              /*int listId*/ registerQmlListType(javaName),
              /*int objectSize*/ int(objectSize),
              /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
              /*QString noCreationReason*/ QString(),

              /*const char *uri*/ _uri.constData(),
              /*int versionMajor*/ int(versionMajor),
              /*int versionMinor*/ int(versionMinor),
              /*const char *elementName*/ _qmlName.constData(),
              /*const QMetaObject *metaObject*/ meta_object,

              /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
              /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

              psCast,
              vsCast,
              viCast,

              /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
              /*const QMetaObject *extensionMetaObject*/ nullptr,

              /*QQmlCustomParser *customParser*/ nullptr,
              /*int revision*/ int(metaObjectRevision)
          };

          return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterRevision
 * Signature: (Ljava/lang/Class;ILjava/lang/String;II)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterRevision
  (JNIEnv * env, jclass, jclass clazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor){
    try{
        size_t objectSize = qtjambi_extended_size_for_class(env, clazz);
        QString javaName = qtjambi_class_name(env, clazz);
        jmethodID constructor = findConstructor(env, clazz, javaName);
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
        int psCast = parserStatusCast(env, clazz);
        int vsCast = valueSourceCast(env, clazz);
        int viCast = valueInterceptorCast(env, clazz);

        QQmlPrivate::RegisterType type = {
            /* int version */ 1,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ int(objectSize),
            /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            psCast,
            vsCast,
            viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ int(metaObjectRevision)
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz){
    try{
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ nullptr,
            /*int versionMajor*/ 0,
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;I)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2I
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring uri, jint versionMajor){
    try{
        QString _uri = qtjambi_to_qstring(env, uri);
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ qPrintable(_uri),
            /*int versionMajor*/ versionMajor,
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    try{
        size_t objectSize = qtjambi_extended_size_for_class(env, clazz);

        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor = findConstructor(env, clazz, javaName);
        jmethodID econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jclass declClass = clazz;
        jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
        if(!method){
            declClass = extendedClazz;
            method = qtjambi_find_QmlAttachedProperties(env, declClass);
        }

        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        int psCast = parserStatusCast(env, clazz);
        int vsCast = valueSourceCast(env, clazz);
        int viCast = valueInterceptorCast(env, clazz);

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ int(objectSize),
            /*void (*create)(void *)*/ creatorFunction(env, meta_object, clazz, constructor, objectSize, psCast, vsCast, viCast),
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            psCast,
            vsCast,
            viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterInterface
 * Signature: (Ljava/lang/Class;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterInterface1
  (JNIEnv * env, jclass, jclass clazz, jstring name){
    try{
        QString typeName = qtjambi_to_qstring(env, name);
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const char* iid = getInterfaceIID(env, clazz);
        if(!iid){
            iid = registerInterfaceID(env, clazz);
        }

        QQmlPrivate::RegisterInterface qmlInterface = {
            /* int version */ 1,

            /* int typeId */ registerQmlMetaType(typeName, meta_object),
            /* int listId */ registerQmlListType(typeName),

            /* const char *iid */ iid,
            
            /* const char *uri */ "",
            /* int versionMajor */ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterInterface
 * Signature: (Ljava/lang/Class;Ljava/lang/String;I)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterInterface2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor){
    try{
        QString _uri = qtjambi_to_qstring(env, uri);
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        QString typeName;
        if(meta_object){
            typeName = meta_object->className();
        }

        const char* iid = getInterfaceIID(env, clazz);
        if(!iid){
            iid = registerInterfaceID(env, clazz);
        }
        QQmlPrivate::RegisterInterface qmlInterface = {
            /* int version */ 1,

            /* int typeId */ registerQmlMetaType(typeName, meta_object),
            /* int listId */ registerQmlListType(typeName),

            /* const char *iid */ iid,
            
            /* const char *uri */ qPrintable(_uri),
            /* int versionMajor */ versionMajor
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlAttachedPropertiesObject
 * Signature: (Lio/qt/core/QObject;Z)Lio/qt/core/QObject;
 */
extern "C" Q_DECL_EXPORT jobject JNICALL Java_io_qt_qml_QtQml_qmlAttachedPropertiesObject
  (JNIEnv * env, jclass, jclass clazz, jobject object, jboolean create){
    try{
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        static int idx = -1;
        QObject* result = QtQml::qmlAttachedPropertiesObject(&idx, qtjambi_to_qobject(env, object), meta_object, create);
        return qtjambi_from_QObject(env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterBaseTypes
 * Signature: (Ljava/lang/String;II)V
 */
extern "C" Q_DECL_EXPORT void JNICALL Java_io_qt_qml_QtQml_qmlRegisterBaseTypes
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor){
    try{
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        qmlRegisterBaseTypes(qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor));
#else
        Q_UNUSED(uri)
        Q_UNUSED(versionMajor)
        Q_UNUSED(versionMinor)
        JavaException::raiseQNoImplementationException(env, "qmlRegisterBaseTypes() is no longer available since Qt 5.14" QTJAMBI_STACKTRACEINFO);
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterAnonymousType
 * Signature: (Ljava/lang/String;I)V
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterAnonymousType
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor){
    try{
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        Q_UNUSED(clazz)
        Q_UNUSED(uri)
        Q_UNUSED(versionMajor)
        JavaException::raiseQNoImplementationException(env, "qmlRegisterAnonymousType() is not available in Qt versions older than 5.14" QTJAMBI_STACKTRACEINFO);
#else
        QString javaName = qtjambi_class_name(env, clazz);
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, clazz, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Lio/qt/core/QUrl;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterType__Lio_qt_core_QUrl_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jobject url, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    env->EnsureLocalCapacity(300);
    try{
        QUrl* _url = reinterpret_cast<QUrl*>(qtjambi_to_object(env, url));
        QString _qmlName = qtjambi_to_qstring(env, qmlName);
        return qmlRegisterType(_url ? *_url : QUrl(), qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor), _qmlName.isEmpty() ? nullptr : qPrintable(_qmlName));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlProtectModule
 * Signature: (Ljava/lang/String;I)Z
 */
extern "C" Q_DECL_EXPORT jboolean JNICALL Java_io_qt_qml_QtQml_qmlProtectModule
  (JNIEnv * env, jclass, jstring uri, jint majVersion){
    try{
        return qmlProtectModule(qPrintable(qtjambi_to_qstring(env, uri)), int(majVersion));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlClearTypeRegistrations
 * Signature: ()V
 */
extern "C" Q_DECL_EXPORT void JNICALL Java_io_qt_qml_QtQml_qmlClearTypeRegistrations
  (JNIEnv *, jclass){
    qmlClearTypeRegistrations();
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterModule
 * Signature: (Ljava/lang/String;II)V
 */
extern "C" Q_DECL_EXPORT void Java_io_qt_qml_QtQml_qmlRegisterModule__Ljava_lang_String_2II
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor){
    try{
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        qmlRegisterModule(_uri.constData(), int(versionMajor), int(versionMinor));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedUncreatableType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    try{
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jclass declClass = clazz;
        jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
        if(!method){
            declClass = extendedClazz;
            method = qtjambi_find_QmlAttachedProperties(env, declClass);
        }

        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterExtendedUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterExtendedUncreatableType__Ljava_lang_Class_2Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jclass extendedClazz, jint metaObjectRevision, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    try{
        QString javaName = qtjambi_class_name(env, clazz);
        QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
        jmethodID constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!constructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }

        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
        if(!extended_meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
            }
        }
        jclass declClass = clazz;
        jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
        if(!method){
            declClass = extendedClazz;
            method = qtjambi_find_QmlAttachedProperties(env, declClass);
        }

        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();

        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ registerQmlMetaType(javaName, meta_object),
            /*int listId*/ registerQmlListType(javaName),
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ qtjambi_to_qstring(env, reason),

            /*const char *uri*/ _uri.constData(),
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ int(versionMinor),
            /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /*const QMetaObject *metaObject*/ meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ attachedPropertiesFunc(env, declClass, method),
            /*const QMetaObject *attachedPropertiesMetaObject*/ attachedPropertiesMetaObject(env, method),

            /*int parserStatusCast*/ parserStatusCast(env, clazz),
            /*int valueSourceCast*/ valueSourceCast(env, clazz),
            /*int valueInterceptorCast*/ valueInterceptorCast(env, clazz),

            /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, constructor),
            /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ int(metaObjectRevision)
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterSingletonType
 * Signature: (Lio/qt/core/QUrl;Ljava/lang/String;IILjava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterSingletonType__Lio_qt_core_QUrl_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv * env, jclass, jobject url, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName){
    try{
        QUrl _url = qtjambi_cast<QUrl>(env, url);
        QString _qmlName = qtjambi_to_qstring(env, qmlName);
        return qmlRegisterSingletonType(_url, qPrintable(qtjambi_to_qstring(env, uri)), int(versionMajor), int(versionMinor), _qmlName.isEmpty() ? nullptr : qPrintable(_qmlName));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterSingletonType
 * Signature: (Ljava/lang/String;IILjava/lang/String;Lio/qt/qml/QtQml$ValueCallback;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterSingletonType__Ljava_lang_String_2IILjava_lang_String_2Lio_qt_qml_QtQml_00024ValueCallback_2
  (JNIEnv * env, jclass, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jobject callback){
    try{
        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        QQmlPrivate::RegisterSingletonType api = {
            /* int version */     0,
            /* const char *uri */ _uri.constData(),
            /* int versionMajor */ int(versionMajor),
            /* int versionMinor */ int(versionMinor),
            /* const char *typeName */ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /* scriptApi */ qtjambi_to_functional< QJSValue (*)(QQmlEngine *, QJSEngine *) >(env, callback, "io/qt/qml/QtQml$ValueCallback"),
            /* qobjectApi */ nullptr,
            /* const QMetaObject *instanceMetaObject */ nullptr,
            /* int typeId */ 0,
            /* int revision */ 0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            ,/* generalizedQobjectApi */ {}
#endif
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterSingletonType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Lio/qt/qml/QtQml$ObjectCallback;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterSingletonType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Lio_qt_qml_QtQml_00024ObjectCallback_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jobject callback){
    try{
        QString javaName = qtjambi_class_name(env, clazz);

        QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
        QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, clazz);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
                meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
            }
        }
        QQmlPrivate::RegisterSingletonType api = {
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            /* int version */     3,
#else
            /* int version */     2,
#endif
            /* const char *uri */ _uri.constData(),
            /* int versionMajor */ int(versionMajor),
            /* int versionMinor */ int(versionMinor),
            /* const char *typeName */ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
            /* scriptApi */ nullptr,
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            /* qobjectApi */ nullptr,
#else
            /* qobjectApi */ qtjambi_to_functional< QObject* (*)(QQmlEngine *, QJSEngine *) >(env, callback, "io/qt/qml/QtQml$ObjectCallback"),
#endif
            /* const QMetaObject *instanceMetaObject */ meta_object,
            /* int typeId */ registerQmlMetaType(javaName, meta_object),
            /* int revision */ 0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
                ,/* generalizedQobjectApi */ qtjambi_to_functional< std::function<QObject*(QQmlEngine *, QJSEngine *)> >(env, callback, "io/qt/qml/QtQml$ObjectCallback")
#endif
        };

        return QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

/*
 * Class:     io_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableClass
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL Java_io_qt_qml_QtQml_qmlRegisterUncreatableClass__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv * env, jclass, jclass clazz, jstring uri, jint versionMajor, jint versionMinor, jstring qmlName, jstring reason){
    try{
      QByteArray _uri = qtjambi_to_qstring(env, uri).toLocal8Bit();
      QByteArray _qmlName = qtjambi_to_qstring(env, qmlName).toLocal8Bit();

      const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz, nullptr);
      if(!meta_object){
          jclass closestClass = resolveClosestQtSuperclass(env, clazz);
          if(closestClass){
              const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass, nullptr);
              meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
          }
      }
      return !meta_object ? -1 : qmlRegisterUncreatableMetaObject(*meta_object,
                                              /*const char *uri*/ _uri.constData(),
                                              /*int versionMajor*/ int(versionMajor),
                                              /*int versionMinor*/ int(versionMinor),
                                              /*const char *elementName*/ _qmlName.isEmpty() ? nullptr : _qmlName.constData(),
                                              /*QString noCreationReason*/ qtjambi_to_qstring(env, reason));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

void qtjambi_qml_jarimport(const QUrl& baseUrl, const char * uri){
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        Java::QtQml::QmlTypes.registerModule(env, qtjambi_cast<jobject>(env, baseUrl), env->NewStringUTF(uri));
    }
}
