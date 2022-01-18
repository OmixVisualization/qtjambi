/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>
#include <QtSql/QSqlRelationalDelegate>

void initialize_meta_info_QSqlRelationalDelegate(){
#if QT_CONFIG(sqlmodel)
#ifdef QT_WIDGETS_LIB
    const std::type_info& typeId = registerQObjectTypeInfo<QSqlRelationalDelegate>("QSqlRelationalDelegate", "io/qt/sql/QSqlRelationalDelegate");
    Q_UNUSED(typeId)
#endif //QT_WIDGETS_LIB
#endif //QT_CONFIG(sqlmodel)
}

extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_sql_QSqlDriverCreator_getConstructorHandle)(JNIEnv *env, jobject, jclass type){
    return jlong(env->GetMethodID(type, "<init>", "()V"));
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_sql_QSqlDriverCreator_create)(JNIEnv *env, jobject, jclass type, jlong methodId){
    return env->NewObject(type, jmethodID(methodId));
}
