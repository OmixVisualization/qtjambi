/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef MULTISIGNALTEST_H
#define MULTISIGNALTEST_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QObject>

class MultiSignalTest : public QObject
{
    Q_OBJECT
public:
    explicit MultiSignalTest(QObject *parent = nullptr);

signals:
    void multiSignal1();
    void multiSignal1(int i);
    void multiSignal1(int i, const QString& s);
    void multiSignal1(double d, QPrivateSignal);
    void multiSignal2(const QString& s);
    void multiSignal2(int i);
    void multiSignal2(bool b);
    void multiSignal3(int i);
    void multiSignal3(bool b);
    void multiSignal3(const QString& s = "TEST");
    void multiSignal4(const QString& s, int i = 147);
    void multiSignal4(int i, bool b = true);
    void multiSignal4(bool b, const QString& s = "TEST");
    void multiSignal4(bool b, int i, const QString& s = "TEST");
public slots:

public:
    void emit_multiSignal1();
    void emit_multiSignal1(int i);
    void emit_multiSignal1(int i, const QString& s);
    void emit_multiSignal1(double d);
    void emit_multiSignal2(const QString& s);
    void emit_multiSignal2(int i);
    void emit_multiSignal2(bool b);
    void emit_multiSignal3();
    void emit_multiSignal3(int i);
    void emit_multiSignal3(bool b);
    void emit_multiSignal3(const QString& s);
    void emit_multiSignal4(const QString& s);
    void emit_multiSignal4(int i);
    void emit_multiSignal4(bool b);
    void emit_multiSignal4(const QString& s, int i);
    void emit_multiSignal4(int i, bool b);
    void emit_multiSignal4(bool b, const QString& s);
    void emit_multiSignal4(bool b, int i, const QString& s);
};

#endif // MULTISIGNALTEST_H
