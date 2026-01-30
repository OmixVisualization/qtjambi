/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_MODELAPI_H
#define QTJAMBI_MODELAPI_H

#include "qtjambiapi.h"

class QAbstractItemModel;
struct ModelDataPrivate;

class ModelData
{
public:
    ModelData();
    ~ModelData();
    QTJAMBI_EXPORT bool get_rowCount(const QAbstractItemModel* model, int& value, const QModelIndex& parent) const;
    QTJAMBI_EXPORT void set_rowCount(int value, const QModelIndex& parent);
    QTJAMBI_EXPORT bool get_columnCount(const QAbstractItemModel* model, int& value, const QModelIndex& parent) const;
    QTJAMBI_EXPORT void set_columnCount(int value, const QModelIndex& parent);
private:
    ModelDataPrivate* d;
    Q_DISABLE_COPY_MOVE(ModelData)
};

class QtJambiModelShell{
public:
    typedef QtJambiAPI::ConstructorFn ConstructorFunction;
    virtual ModelData* modelData() = 0;
    static QTJAMBI_EXPORT void initialize(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction, size_t size, size_t alignment, const std::type_info& typeId, uint returnScopeRequired, const QMetaObject& originalMetaObject, SafeBool isShell, SafeBool hasCustomMetaObject, jvalue* arguments = nullptr);
};

#endif // QTJAMBI_MODELAPI_H
