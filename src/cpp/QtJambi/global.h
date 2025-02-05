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

#if !defined(QTJAMBI_GLOBAL_H)
#define QTJAMBI_GLOBAL_H

#define _CRT_SECURE_NO_DEPRECATE

#include <QtCore/qglobal.h>
#include <QtCore/qhashfunctions.h>

//TODO: rewrite
#if defined(Q_OS_WIN)
#  if !defined(QTJAMBI_EXPORT) && !defined(QT_QTJAMBI_IMPORT)
#    define QTJAMBI_EXPORT
#    define QTJAMBI_STATIC_EXPORT __declspec(dllimport)
#  elif defined(QT_QTJAMBI_IMPORT)
#    if defined(QTJAMBI_EXPORT)
#      undef QTJAMBI_EXPORT
#    endif
#    define QTJAMBI_EXPORT __declspec(dllimport)
#    define QTJAMBI_STATIC_EXPORT
#  elif defined(QTJAMBI_EXPORT)
#    undef QTJAMBI_EXPORT
#    define QTJAMBI_EXPORT __declspec(dllexport)
#    define QTJAMBI_STATIC_EXPORT
#  endif
#elif defined (QT_VISIBILITY_AVAILABLE)
#  if !defined(QTJAMBI_EXPORT) && !defined(QT_QTJAMBI_IMPORT)
#    define QTJAMBI_EXPORT
#    define QTJAMBI_STATIC_EXPORT
#  elif defined(QT_QTJAMBI_IMPORT)
#    if defined(QTJAMBI_EXPORT)
#      undef QTJAMBI_EXPORT
#    endif
#    define QTJAMBI_EXPORT
#    define QTJAMBI_STATIC_EXPORT
#  elif defined(QTJAMBI_EXPORT)
#    undef QTJAMBI_EXPORT
#    define QTJAMBI_EXPORT __attribute__((visibility("default")))
#    define QTJAMBI_STATIC_EXPORT
#  endif
#else
# if defined(QTJAMBI_EXPORT)
#   undef QTJAMBI_EXPORT
# endif
#  define QTJAMBI_EXPORT
#    define QTJAMBI_STATIC_EXPORT
#endif

#  include <jni.h>
#ifndef Q_OS_ANDROID
#  include <jni_md.h>
#endif

#ifndef QTJAMBI_GENERATOR_RUNNING

typedef void (*PtrDeleterFunction)(void *,bool);

typedef void* (*CopyFunction)(const void *);

typedef const class QObject* (*PtrOwnerFunction)(const void *);

using hash_type = decltype(qHash(std::declval<char>()));

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

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define MIN_QT6(MACRO, ...)\
MACRO(__VA_ARGS__)
#else
#define MIN_QT6(...)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
#define MIN_QT7(MACRO, ...)\
MACRO(__VA_ARGS__)
#else
#define MIN_QT7(...)
#endif

#endif // QTJAMBI_GLOBAL_H
