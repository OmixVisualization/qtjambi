/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef QTJAMBICONCURRENT_H
#define QTJAMBICONCURRENT_H

#ifndef QT_NO_QFUTURE

#include <QtCore/QtCore>
#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_jobjectwrapper.h>
#endif //def QT_JAMBI_RUN

typedef QFutureWatcher<void> QtJambiVoidFutureWatcher;
typedef QFuture<void> QtJambiVoidFuture;
typedef QFutureSynchronizer<void> QtJambiVoidFutureSynchronizer;
typedef QFuture<JObjectWrapper> QtJambiFuture;
typedef QFutureWatcher<JObjectWrapper> QtJambiFutureWatcher;
typedef QFutureSynchronizer<JObjectWrapper> QtJambiFutureSynchronizer;
typedef QFutureIterator<JObjectWrapper> QtJambiFutureIterator;
typedef QThreadStorage<JObjectWrapper> QtJambiThreadStorage;
typedef QAtomicInteger<jint> QtJambiAtomicInteger;
typedef QAtomicInteger<jbyte> QAtomicByte;
typedef QAtomicInteger<jshort> QAtomicShort;
typedef QAtomicInteger<jlong> QAtomicLong;

namespace QtJambiStringUtil{

inline QString join(const QStringList& stringList, const QString &sep)
{return stringList.join(sep);}

inline QString join(const QStringList& stringList, QChar sep)
{return stringList.join(sep);}

#if QT_VERSION_CHECK(5,15,0)
#define Q_SPLITBEHAVIOUR_PREFIX(e) Qt::e
#define Q_SPLIT_PREFIX(e) Qt::e
#else
#ifdef QT_JAMBI_RUN
enum SplitBehavior { KeepEmptyParts, SkipEmptyParts };
#define Q_SPLIT_PREFIX(e) e
#define Q_SPLITBEHAVIOUR_PREFIX(e) e
#else
#define Q_SPLITBEHAVIOUR_PREFIX(e) QString::e
#define Q_SPLIT_PREFIX(e) QString::e
#endif
#endif

#ifndef QT_JAMBI_RUN
#define Q_STRING_PREFIX(e) QString::e
#else
#define Q_STRING_PREFIX(e) e
#endif

inline QStringList split(const QString& string, const QString &sep, Q_SPLITBEHAVIOUR_PREFIX(SplitBehavior) behavior = Q_SPLIT_PREFIX(KeepEmptyParts), Qt::CaseSensitivity cs = Qt::CaseSensitive)
{ return string.split(sep, behavior, cs); }

inline QStringList split(const QString& string, QChar sep, Q_SPLITBEHAVIOUR_PREFIX(SplitBehavior) behavior = Q_SPLIT_PREFIX(KeepEmptyParts), Qt::CaseSensitivity cs = Qt::CaseSensitive)
{ return string.split(sep, behavior, cs); }
#ifndef QT_NO_REGEXP
inline QStringList split(const QString& string, const QRegExp &sep, Q_SPLITBEHAVIOUR_PREFIX(SplitBehavior) behavior = Q_SPLIT_PREFIX(KeepEmptyParts))
{ return string.split(sep, behavior); }
#endif
#if QT_CONFIG(regularexpression)
inline QStringList split(const QString& string, const QRegularExpression &sep, Q_SPLITBEHAVIOUR_PREFIX(SplitBehavior) behavior = Q_SPLIT_PREFIX(KeepEmptyParts))
{ return string.split(sep, behavior); }
#endif

inline QString repeated(const QString& string, int times)
{ return string.repeated(times); }

#ifdef QT_JAMBI_RUN
enum SectionFlag {
    SectionDefault             = 0x00,
    SectionSkipEmpty           = 0x01,
    SectionIncludeLeadingSep   = 0x02,
    SectionIncludeTrailingSep  = 0x04,
    SectionCaseInsensitiveSeps = 0x08
};
Q_DECLARE_FLAGS(SectionFlags, SectionFlag)
#else
typedef QString::SectionFlag SectionFlag;
typedef QString::SectionFlags SectionFlags;
#endif

inline QString section(const QString& string, QChar sep, int start, int end = -1, SectionFlags flags = Q_STRING_PREFIX(SectionDefault))
{ return string.section(sep, start, end, flags); }

inline QString section(const QString& string, const QString &in_sep, int start, int end = -1, SectionFlags flags = Q_STRING_PREFIX(SectionDefault))
{ return string.section(in_sep, start, end, flags); }
#ifndef QT_NO_REGEXP
inline QString section(const QString& string, const QRegExp &reg, int start, int end = -1, SectionFlags flags = Q_STRING_PREFIX(SectionDefault))
{ return string.section(reg, start, end, flags); }
#endif
#if QT_CONFIG(regularexpression)
inline QString section(const QString& string, const QRegularExpression &re, int start, int end = -1, SectionFlags flags = Q_STRING_PREFIX(SectionDefault))
{ return string.section(re, start, end, flags); }
#endif
inline QString left(const QString& string, int n)
{ return string.left(n); }

inline QString right(const QString& string, int n)
{ return string.right(n); }

inline QString mid(const QString& string, int position, int n = -1)
{ return string.mid(position, n); }

inline QString chopped(const QString& string, int n)
{ return string.chopped(n); }

inline QString arg(QString string, std::initializer_list<QString> args)
{
    for(const QString& arg : args){
        string = string.arg(arg);
    }
    return string;
}

};

#undef Q_STRING_PREFIX

#endif // QT_NO_QFUTURE

#endif
