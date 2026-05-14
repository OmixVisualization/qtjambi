/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_BUFFER_H
#define QTJAMBI_CAST_BUFFER_H

#include "javabuffers.h"
#include "qtjambi_cast_util.h"

namespace QtJambiPrivate {

template<bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_buffer{
    static auto JBufferPointer(JniType in, Args...args){
        PersistentJBufferData * data = nullptr;
        if constexpr(std::is_same_v<JniType, jobject>){
            auto env = cast_var_args<Args...>::env(args...);
            if (JBufferData::isBuffer(env, in)) {
                data = new PersistentJBufferData(env, in);
            }
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            Q_UNUSED(in)
        }
        return data;
    }
};

template<typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>{
    static auto JBufferPointer(JniType in, Args...args){
        PersistentJBufferConstData * data = nullptr;
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(std::is_same_v<JniType, jobject>){
            if (JBufferConstData::isBuffer(env, in)) {
                data = new PersistentJBufferConstData(env, in);
            }
        }else{
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return data;
    }
};


} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_BUFFER_H
