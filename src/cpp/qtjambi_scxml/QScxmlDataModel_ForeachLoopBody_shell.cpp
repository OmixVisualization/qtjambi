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

#include <QtCore/QVariant>
#include <QtCore/QVariant>

#include <qscxmldatamodel.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

// QScxmlDataModel::ForeachLoopBody::run(bool * ok)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_scxml_QScxmlDataModel_00024ForeachLoopBody__1_1qt_1QScxmlDataModel_00024ForeachLoopBody_1run_1nativepointer)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jbooleanArray ok0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QScxmlDataModel::ForeachLoopBody::run(bool * ok)")
    try{
        QScxmlDataModel::ForeachLoopBody *__qt_this = qtjambi_object_from_nativeId<QScxmlDataModel::ForeachLoopBody>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        JBooleanArrayPointer boolPointer(__jni_env, ok0);
        __qt_this->run(boolPointer.pointer());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

void deleter_ForeachLoopBody(void* ptr){
    delete reinterpret_cast<QScxmlDataModel::ForeachLoopBody*>(ptr);
}

void initialize_meta_info_ForeachLoopBody(){
    const std::type_info& typeId = registerTypeInfo<QScxmlDataModel::ForeachLoopBody>("QScxmlDataModel::ForeachLoopBody", "io/qt/scxml/QScxmlDataModel$ForeachLoopBody");
    registerDeleter(typeId, &deleter_ForeachLoopBody);
}
