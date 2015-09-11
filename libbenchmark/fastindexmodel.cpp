/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "jni.h"

#include "qtjambi_core.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>

#include <QtGui/QStandardItemModel>

struct QModelIndexAccessor {
    int row;
    int col;
    void *ptr;
    QAbstractItemModel *model;
};

QStandardItemModel *sillyModel;

_declspec(noinline) static void doStuff(const QModelIndex &index) {
    if (index.row() == index.column() + 1)
        printf("tada!!!\n");
}

jclass class_FastIndexArray;
jfieldID id_FastIndexArray_data;
jmethodID id_FastIndexArray_constructor;

jclass class_FastIndexMembers;
jfieldID id_FastIndexMembers_row;
jfieldID id_FastIndexMembers_col;
jfieldID id_FastIndexMembers_model;
jfieldID id_FastIndexMembers_object;
jmethodID id_FastIndexMembers_constructor;


extern "C" JNIEXPORT void JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_setupNative
(JNIEnv *env, jclass)
{
    sillyModel = new QStandardItemModel(1000, 1000);

    class_FastIndexArray = (jclass) env->NewGlobalRef(env->FindClass("org/qtjambi/benchmarks/itemview/FastIndexArray"));
    Q_ASSERT(class_FastIndexArray);


    id_FastIndexArray_data = env->GetFieldID(class_FastIndexArray, "data", "[J");
    Q_ASSERT(id_FastIndexArray_data);
    id_FastIndexArray_constructor = env->GetMethodID(class_FastIndexArray, "<init>", "(IILorg/qtjambi/qt/gui/QAbstractItemModel;)V");
    Q_ASSERT(id_FastIndexArray_constructor);

    class_FastIndexMembers = (jclass) env->NewGlobalRef(env->FindClass("org/qtjambi/benchmarks/itemview/FastIndexMembers"));
    Q_ASSERT(class_FastIndexMembers);


    id_FastIndexMembers_row = env->GetFieldID(class_FastIndexMembers, "row", "I");
    id_FastIndexMembers_col = env->GetFieldID(class_FastIndexMembers, "col", "I");
    id_FastIndexMembers_model = env->GetFieldID(class_FastIndexMembers, "model", "Lorg/qtjambi/qt/gui/QAbstractItemModel;");
    id_FastIndexMembers_object = env->GetFieldID(class_FastIndexMembers, "object", "Ljava/lang/Object;");
    Q_ASSERT(id_FastIndexMembers_row);
    Q_ASSERT(id_FastIndexMembers_col);
    Q_ASSERT(id_FastIndexMembers_model);
    Q_ASSERT(id_FastIndexMembers_object);

    id_FastIndexMembers_constructor = env->GetMethodID(class_FastIndexMembers, "<init>", "(IILorg/qtjambi/qt/gui/QAbstractItemModel;)V");
}

extern "C" JNIEXPORT void JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_doNativeStuff
(JNIEnv *, jobject)
{
    QModelIndex index = sillyModel->index(500, 500);
}

extern "C" JNIEXPORT void JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_takeQModelIndex
(JNIEnv *, jobject, jlong indexPointer, jobject, jint)
{
    QModelIndex *index = (QModelIndex *) qtjambi_from_jlong(indexPointer);
    doStuff(*index);
}


extern "C" JNIEXPORT jobject JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_createFastIndexArray
(JNIEnv *env, jobject, jint row, jint col)
{
    sillyModel->index(row, col);
    return env->NewObject(class_FastIndexArray, id_FastIndexArray_constructor, row, col, 0);
}


extern "C" JNIEXPORT jobject JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_createFastIndexMembers
(JNIEnv *env, jobject, jint row, jint col)
{
    sillyModel->index(row, col);
    return env->NewObject(class_FastIndexMembers, id_FastIndexMembers_constructor, row, col, 0);
}


extern "C" JNIEXPORT void JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_takeFastIndexArray
(JNIEnv *env, jobject, jobject index, jobject, jint)
{
    jlongArray array = (jlongArray) env->GetObjectField(index, id_FastIndexArray_data);
    jlong *data = env->GetLongArrayElements(array, 0);

    QModelIndexAccessor modelIndex = {
        (int) data[0],
        (int) data[1],
        (void *) data[2],
        (QAbstractItemModel *) data[3]
    };
    env->ReleaseLongArrayElements(array, data, JNI_ABORT);

    doStuff(*(QModelIndex *) &modelIndex);
}


extern "C" JNIEXPORT void JNICALL
Java_org_qtjambi_benchmarks_itemview_FastIndexModel_takeFastIndexMembers
(JNIEnv *env, jobject, jobject index, jobject, jint)
{
    QModelIndexAccessor modelIndex = {
        env->GetIntField(index, id_FastIndexMembers_row),
        env->GetIntField(index, id_FastIndexMembers_col),
        (void *) env->GetObjectField(index, id_FastIndexMembers_object),
        (QAbstractItemModel *) env->GetObjectField(index, id_FastIndexMembers_model)
    };

    doStuff(*(QModelIndex *)&modelIndex);
}
