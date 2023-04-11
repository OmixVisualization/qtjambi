/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtJambi/QtJambiAPI>

class QAbstractGrpcChannel;
class QGrpcOperation;

namespace QtJambiPrivate{

void deleteGrpcChannel(QAbstractGrpcChannel* pointer);

template<>
struct SmartPointerHelper<std::shared_ptr,QAbstractGrpcChannel>{
    static void deletePointer(void* pointer,bool){
        std::shared_ptr<QAbstractGrpcChannel>* _pointer = reinterpret_cast<std::shared_ptr<QAbstractGrpcChannel>*>(pointer);
        delete _pointer;
    }

    static void* createPointer(void* pointer){
        return new std::shared_ptr<QAbstractGrpcChannel>(reinterpret_cast<QAbstractGrpcChannel*>(pointer), &deleteGrpcChannel);
    }

    static void* getFromPointer(const void* pointer){
        const std::shared_ptr<QAbstractGrpcChannel>& _pointer = *reinterpret_cast<const std::shared_ptr<QAbstractGrpcChannel>*>(pointer);
        return &*_pointer;QSharedPointer<int> i;
    }
};

}

jobject qtjambi_QGrpcOperation_read(JNIEnv *__jni_env, const QGrpcOperation *__qt_this, jclass type, jobjectArray instantiations);

#endif // UTILS_P_H
