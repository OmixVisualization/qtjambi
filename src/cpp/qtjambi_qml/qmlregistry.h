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

/* Header for class org_qtjambi_qt_qml_QtQml */

#ifndef _Included_qmlregistry_h
#define _Included_qmlregistry_h

#include <QtCore/qglobal.h>
#ifdef Q_OS_MAC
#  include <JavaVM/jni.h>
#else
#  include <jni.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2
  (JNIEnv *, jclass, jclass);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterTypeNotAvailable
 * Signature: (Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterTypeNotAvailable
  (JNIEnv *, jclass, jstring, jint, jint, jstring, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jclass, jclass, jstring, jint, jint, jstring, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterUncreatableType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterUncreatableType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jclass, jclass, jint, jstring, jint, jint, jstring, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv *, jclass, jclass, jstring, jint, jint, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Ljava/lang/Class;ILjava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Ljava_lang_Class_2ILjava_lang_String_2IILjava_lang_String_2
  (JNIEnv *, jclass, jclass, jint, jstring, jint, jint, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterRevision
 * Signature: (Ljava/lang/Class;ILjava/lang/String;II)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterRevision
  (JNIEnv *, jclass, jclass, jint, jstring, jint, jint);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2
  (JNIEnv *, jclass, jclass, jclass);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterExtendedType
 * Signature: (Ljava/lang/Class;Ljava/lang/Class;Ljava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterExtendedType__Ljava_lang_Class_2Ljava_lang_Class_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv *, jclass, jclass, jclass, jstring, jint, jint, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterInterface
 * Signature: (Ljava/lang/Class;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterInterface
  (JNIEnv *, jclass, jclass, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlAttachedPropertiesObject
 * Signature: (Lorg/qtjambi/qt/core/QObject;Z)Lorg/qtjambi/qt/core/QObject;
 */
JNIEXPORT jobject JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlAttachedPropertiesObject
  (JNIEnv *, jclass, jobject, jboolean);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterBaseTypes
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterBaseTypes
  (JNIEnv *, jclass, jstring, jint, jint);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlRegisterType
 * Signature: (Lorg/qtjambi/qt/core/QUrl;Ljava/lang/String;IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlRegisterType__Lorg_qtjambi_qt_core_QUrl_2Ljava_lang_String_2IILjava_lang_String_2
  (JNIEnv *, jclass, jobject, jstring, jint, jint, jstring);

/*
 * Class:     org_qtjambi_qt_qml_QtQml
 * Method:    qmlProtectModule
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_org_qtjambi_qt_qml_QtQml_qmlProtectModule
  (JNIEnv *, jclass, jstring, jint);


#ifdef __cplusplus
}
#endif
#endif
