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

#if !defined(QTJAMBI_GLOBAL_H)
#define QTJAMBI_GLOBAL_H

#define _CRT_SECURE_NO_DEPRECATE

#include <QtCore/qglobal.h>
#include <QtCore/qhashfunctions.h>

#if defined(QT_SHARED) || !defined(QT_STATIC)
#  if defined(QTJAMBI_BUILD_LIB)
#    define QTJAMBI_EXPORT Q_DECL_EXPORT
#    define QTJAMBI_TEMPLATE_EXPORT
#  else
#    define QTJAMBI_EXPORT Q_DECL_IMPORT
#    define QTJAMBI_TEMPLATE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QTJAMBI_EXPORT
#  define QTJAMBI_TEMPLATE_EXPORT
#endif

#  include <jni.h>
#ifndef Q_OS_ANDROID
#  include <jni_md.h>
#endif

#ifndef QTJAMBI_GENERATOR_RUNNING

typedef void (*PtrDeleterFunction)(void *,bool);

typedef void* (*CopyFunction)(const void *);

typedef const class QObject* (*PtrOwnerFunction)(const void *);

struct SafeBool{
    SafeBool() = delete;
    template<typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, bool>>>
    SafeBool(T) = delete;
    inline SafeBool(bool _value) : value(_value){}
    inline operator bool() const { return value; }
    inline bool operator!() const { return !value; }
    friend inline void swap(SafeBool& a, SafeBool& b){
        std::swap(a.value, b.value);
    }
private:
    bool value;
};

#endif // QTJAMBI_GENERATOR_RUNNING

#if defined(QT_OVERLOADED_MACRO) && QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_OVERLOADED_MACRO QT_OVERLOADED_MACRO
#else
#define QTJAMBI_VA_ARGS_CHOOSE(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define QTJAMBI_VA_ARGS_EXPAND(...) __VA_ARGS__
#define QTJAMBI_VA_ARGS_COUNT(...) QTJAMBI_VA_ARGS_EXPAND(QTJAMBI_VA_ARGS_CHOOSE(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define QTJAMBI_OVERLOADED_MACRO_EXPAND(MACRO, ARGC) MACRO##_##ARGC
#define QTJAMBI_OVERLOADED_MACRO_IMP(MACRO, ARGC) QTJAMBI_OVERLOADED_MACRO_EXPAND(MACRO, ARGC)
#define QTJAMBI_OVERLOADED_MACRO(MACRO, ...) QTJAMBI_VA_ARGS_EXPAND(QTJAMBI_OVERLOADED_MACRO_IMP(MACRO, QTJAMBI_VA_ARGS_COUNT(__VA_ARGS__))(__VA_ARGS__))
#endif

#define MIN_QT6(MACRO, ...)\
MACRO(__VA_ARGS__)

#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
#define MIN_QT7(MACRO, ...)\
MACRO(__VA_ARGS__)
#else
#define MIN_QT7(...)
#endif

#endif // QTJAMBI_GLOBAL_H
