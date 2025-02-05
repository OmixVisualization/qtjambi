/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#include <QtJambi/QtJambiAPI>
#include <QtScxml/QScxmlStateMachine>
#include <QtJambi/JavaAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/qtjambi_cast.h>

#if QT_VERSION < QT_VERSION_CHECK(5,12,0)
#include <qscxmldatamodel.h>

// QScxmlDataModel::ForeachLoopBody::run(bool * ok)
extern "C" JNIEXPORT void JNICALL Java_io_qt_scxml_QScxmlDataModel_00024ForeachLoopBody_run
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jbooleanArray ok0)
{
    QTJAMBI_TRY{
        QScxmlDataModel::ForeachLoopBody *__qt_this = QtJambiAPI::objectFromNativeId<QScxmlDataModel::ForeachLoopBody>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QScxmlDataModel::ForeachLoopBody::run(bool * ok)", __qt_this)
        JBooleanArrayPointer boolPointer(__jni_env, ok0);
        __qt_this->run(boolPointer.pointer());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_ForeachLoopBody(void* ptr){
    delete reinterpret_cast<QScxmlDataModel::ForeachLoopBody*>(ptr);
}

void initialize_meta_info_ForeachLoopBody(){
    using namespace RegistryAPI;
    const std::type_info& typeId = registerUnspecificTypeInfo<QScxmlDataModel::ForeachLoopBody>("QScxmlDataModel::ForeachLoopBody", "io/qt/scxml/QScxmlDataModel$ForeachLoopBody");
    registerDeleter(typeId, &deleter_ForeachLoopBody);
}
#endif
