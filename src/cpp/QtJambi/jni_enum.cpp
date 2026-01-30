/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "pch_p.h"

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_EnumUtility_resolveLongEnum(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jlong value, jstring entryName){
    try{
        return resolveLongEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_EnumUtility_resolveByteEnum(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jbyte value, jstring entryName){
    try{
        return resolveByteEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_EnumUtility_resolveShortEnum(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jshort value, jstring entryName){
    try{
        return resolveShortEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_EnumUtility_resolveIntEnum(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jint value, jstring entryName){
    try{
        return resolveIntEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
