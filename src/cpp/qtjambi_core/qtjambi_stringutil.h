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

#include <qtjambi/qtjambi_global.h>
#include <QtCore/QString>
#include <QtCore/QStringList>
#if QT_CONFIG(regularexpression)
#include <qregularexpression.h>
#endif //QT_CONFIG(regularexpression)
#include <QtCore/QList>
#include <QtCore/QVariant>

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_core.h>
#else
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
struct QtJambiItemSelection{
    QtJambiItemSelection(std::initializer_list<QItemSelectionRange>);
    QtJambiItemSelection();
};
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool operator<(const QString &s1, const QString &s2) noexcept{return false;}
bool operator<(const QChar &s1, const QChar &s2) noexcept{return false;}
bool operator==(const QChar &s1, const QChar &s2) noexcept{return false;}
#endif

struct QtJambiStringList{
    QtJambiStringList() = delete;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QString join(const QString &sep);
    QString join(QChar sep);
    QStringList filter(const QString &str, Qt::CaseSensitivity cs);
    QStringList filter(const QRegularExpression  &re);
    int removeDuplicates();
    void replaceInStrings(const QString &before, const QString &after, Qt::CaseSensitivity cs);
    void replaceInStrings(const QRegularExpression &re, const QString &after);
    void sort(Qt::CaseSensitivity cs);
#else
    QtJambiStringList(std::initializer_list<QString>);
    QtJambiStringList(QList<QString>);
    void sort(Qt::CaseSensitivity cs);
    qsizetype removeDuplicates();
    QString join(QStringView sep);
    QString join(QChar sep);
    QStringList filter(const QStringView str, Qt::CaseSensitivity cs);
    void replaceInStrings(QStringView before, QStringView after, Qt::CaseSensitivity cs);
    bool contains(const QStringView str, Qt::CaseSensitivity cs) noexcept;
    #if QT_CONFIG(regularexpression)
    QStringList filter(const QRegularExpression &re);
    void replaceInStrings(const QRegularExpression &re, const QString &after);
    qsizetype indexOf(const QRegularExpression &re, qsizetype from);
    qsizetype lastIndexOf(const QRegularExpression &re, qsizetype from);
#endif // QT_CONFIG(regularexpression)
#endif
};
#endif //def QT_JAMBI_RUN

#endif // QT_NO_QFUTURE

#endif
