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

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
namespace QtJambiPrivate{
template<>
struct supports_qHash<QMap<QString,QPair<size_t,size_t>>> : std::false_type{};
template<>
struct supports_qHash<QList<QPair<size_t,size_t>>> : std::false_type{};
}
#endif

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_DeployerUtility_getRegisteredTypeSizesAndAlignments__(JNIEnv *env, jclass){
    try{
        return qtjambi_cast<jobject>(env, getRegisteredTypeSizesAndAlignments());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_DeployerUtility_getInterfaceIID(JNIEnv *env, jclass, jclass cls){
    try{
        const char* iid = CoreAPI::getInterfaceIID(env, cls);
        return iid ? env->NewStringUTF(iid) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

