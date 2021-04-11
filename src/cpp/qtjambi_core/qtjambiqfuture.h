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

#ifndef QT_JAMBI_RUN
#define QFUTURE_TEST
#endif

#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>
#include <QtCore/QFutureSynchronizer>
#include <QtCore/QAtomicInteger>
#include <QtCore/QThreadStorage>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QVariant>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QPromise>
#endif

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#endif //def QT_JAMBI_RUN

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QPromise<void> QVoidPromise;
typedef QPromise<jbyte> QBytePromise;
typedef QPromise<jint> QIntPromise;
typedef QPromise<jshort> QShortPromise;
typedef QPromise<jlong> QLongPromise;
typedef QPromise<jfloat> QFloatPromise;
typedef QPromise<jdouble> QDoublePromise;
typedef QPromise<jchar> QCharPromise;
typedef QPromise<jboolean> QBooleanPromise;
typedef QPromise<JObjectWrapper> QtJambiPromise;
#endif

typedef QFuture<void> QVoidFuture;
typedef QFuture<jbyte> QByteFuture;
typedef QFuture<jint> QIntFuture;
typedef QFuture<jshort> QShortFuture;
typedef QFuture<jlong> QLongFuture;
typedef QFuture<jfloat> QFloatFuture;
typedef QFuture<jdouble> QDoubleFuture;
typedef QFuture<jchar> QCharFuture;
typedef QFuture<jboolean> QBooleanFuture;
typedef QFuture<JObjectWrapper> QtJambiFuture;

typedef QFutureIterator<JObjectWrapper> QtJambiFutureIterator;
typedef QFutureIterator<jbyte> QByteFutureIterator;
typedef QFutureIterator<jint> QIntFutureIterator;
typedef QFutureIterator<jshort> QShortFutureIterator;
typedef QFutureIterator<jlong> QLongFutureIterator;
typedef QFutureIterator<jfloat> QFloatFutureIterator;
typedef QFutureIterator<jdouble> QDoubleFutureIterator;
typedef QFutureIterator<jchar> QCharFutureIterator;
typedef QFutureIterator<jboolean> QBooleanFutureIterator;

typedef QFutureWatcher<void> QVoidFutureWatcher;
typedef QFutureWatcher<jbyte> QByteFutureWatcher;
typedef QFutureWatcher<jint> QIntFutureWatcher;
typedef QFutureWatcher<jshort> QShortFutureWatcher;
typedef QFutureWatcher<jlong> QLongFutureWatcher;
typedef QFutureWatcher<jfloat> QFloatFutureWatcher;
typedef QFutureWatcher<jdouble> QDoubleFutureWatcher;
typedef QFutureWatcher<jchar> QCharFutureWatcher;
typedef QFutureWatcher<jboolean> QBooleanFutureWatcher;
typedef QFutureWatcher<JObjectWrapper> QtJambiFutureWatcher;

typedef QFutureSynchronizer<void> QVoidFutureSynchronizer;
typedef QFutureSynchronizer<jbyte> QByteFutureSynchronizer;
typedef QFutureSynchronizer<jint> QIntFutureSynchronizer;
typedef QFutureSynchronizer<jshort> QShortFutureSynchronizer;
typedef QFutureSynchronizer<jlong> QLongFutureSynchronizer;
typedef QFutureSynchronizer<jfloat> QFloatFutureSynchronizer;
typedef QFutureSynchronizer<jdouble> QDoubleFutureSynchronizer;
typedef QFutureSynchronizer<jchar> QCharFutureSynchronizer;
typedef QFutureSynchronizer<jboolean> QBooleanFutureSynchronizer;
typedef QFutureSynchronizer<JObjectWrapper> QtJambiFutureSynchronizer;

typedef QFutureInterface<void> QVoidFutureInterface;
typedef QFutureInterface<jbyte> QByteFutureInterface;
typedef QFutureInterface<jint> QIntFutureInterface;
typedef QFutureInterface<jshort> QShortFutureInterface;
typedef QFutureInterface<jlong> QLongFutureInterface;
typedef QFutureInterface<jfloat> QFloatFutureInterface;
typedef QFutureInterface<jdouble> QDoubleFutureInterface;
typedef QFutureInterface<jchar> QCharFutureInterface;
typedef QFutureInterface<jboolean> QBooleanFutureInterface;
typedef QFutureInterface<JObjectWrapper> QtJambiFutureInterface;

typedef QThreadStorage<JObjectWrapper> QtJambiThreadStorage;
typedef QThreadStorage<jbyte> QByteThreadStorage;
typedef QThreadStorage<jint> QIntThreadStorage;
typedef QThreadStorage<jshort> QShortThreadStorage;
typedef QThreadStorage<jlong> QLongThreadStorage;
typedef QThreadStorage<jfloat> QFloatThreadStorage;
typedef QThreadStorage<jdouble> QDoubleThreadStorage;
typedef QThreadStorage<jchar> QCharThreadStorage;
typedef QThreadStorage<jboolean> QBooleanThreadStorage;
typedef QAtomicInteger<jint> QtJambiAtomicInteger;
typedef QAtomicInteger<jbyte> QAtomicByte;
typedef QAtomicInteger<jshort> QAtomicShort;
typedef QAtomicInteger<jlong> QAtomicLong;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline bool operator==(const QtJambiFuture &f1, const QtJambiFuture &f2) {
    struct Future{
        QFutureInterfaceBase d;
    };
    return reinterpret_cast<const Future*>(&f1)->d==reinterpret_cast<const Future*>(&f2)->d;
}

inline bool operator==(const QVoidFuture &f1, const QVoidFuture &f2) {
    struct Future{
        QFutureInterfaceBase d;
    };
    return reinterpret_cast<const Future*>(&f1)->d==reinterpret_cast<const Future*>(&f2)->d;
}
#endif

namespace QtJambiStringListUtil{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline QString join(const QStringList& stringList, const QString &sep)
{return stringList.join(sep);}

inline QString join(const QStringList& stringList, QChar sep)
{return stringList.join(sep);}

inline QStringList filter(const QStringList& stringList, const QString &str, Qt::CaseSensitivity cs){
    return stringList.filter(str, cs);
}

inline QStringList filter(const QStringList& stringList, const QRegularExpression  &re){
    return stringList.filter(re);
}

inline int removeDuplicates(QStringList& stringList){
    return int(stringList.removeDuplicates());
}

inline void replaceInStrings(QStringList& stringList, const QString &before, const QString &after, Qt::CaseSensitivity cs){
    stringList.replaceInStrings(before, after, cs);
}

inline void replaceInStrings(QStringList& stringList, const QRegularExpression &re, const QString &after){
    stringList.replaceInStrings(re, after);
}

inline void sort(QStringList& stringList, Qt::CaseSensitivity cs){
    stringList.sort(cs);
}

#else
inline void sort(QStringList& self, Qt::CaseSensitivity cs)
{ QtPrivate::QStringList_sort(&self, cs); }
inline qsizetype removeDuplicates(QStringList& self)
{ return QtPrivate::QStringList_removeDuplicates(&self); }

inline QString join(const QStringList& self, QStringView sep)
{ return QtPrivate::QStringList_join(&self, sep); }
inline QString join(const QStringList& self, QChar sep)
{ return QtPrivate::QStringList_join(&self, &sep, 1); }

inline QStringList filter(const QStringList& self, QStringView str, Qt::CaseSensitivity cs)
{ return QtPrivate::QStringList_filter(&self, str, cs); }
inline void replaceInStrings(QStringList& self, QStringView before, QStringView after, Qt::CaseSensitivity cs)
{
    QtPrivate::QStringList_replaceInStrings(&self, before, after, cs);
}

inline bool contains(const QStringList& self, QStringView str, Qt::CaseSensitivity cs) noexcept
{ return QtPrivate::QStringList_contains(&self, str, cs); }

#if QT_CONFIG(regularexpression)
inline QStringList filter(const QStringList& self, const QRegularExpression &re)
{ return QtPrivate::QStringList_filter(&self, re); }
inline void replaceInStrings(QStringList& self, const QRegularExpression &re, const QString &after)
{
    QtPrivate::QStringList_replaceInStrings(&self, re, after);
}
inline qsizetype indexOf(const QStringList& self, const QRegularExpression &re, qsizetype from)
{ return QtPrivate::QStringList_indexOf(&self, re, from); }
inline qsizetype lastIndexOf(const QStringList& self, const QRegularExpression &re, qsizetype from)
{ return QtPrivate::QStringList_lastIndexOf(&self, re, from); }
#endif // QT_CONFIG(regularexpression)
#endif
}

namespace QtJambiStringUtil{

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifndef QT_NO_REGEXP
inline QStringList split(const QString& string, const QRegExp &sep, Q_SPLITBEHAVIOUR_PREFIX(SplitBehavior) behavior = Q_SPLIT_PREFIX(KeepEmptyParts))
{ return string.split(sep, behavior); }
#endif
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifndef QT_NO_REGEXP
inline QString section(const QString& string, const QRegExp &reg, int start, int end = -1, SectionFlags flags = Q_STRING_PREFIX(SectionDefault))
{ return string.section(reg, start, end, flags); }
#endif
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

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

inline QString format(QString string, std::initializer_list<QVariant> args)
{
    for(const QVariant& arg : args){
        string = string.arg(arg.toString());
    }
    return string;
}

};

#undef Q_STRING_PREFIX

#endif // QT_NO_QFUTURE

#endif
