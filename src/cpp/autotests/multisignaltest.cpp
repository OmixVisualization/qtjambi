/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "multisignaltest.h"

MultiSignalTest::MultiSignalTest(QObject *parent) : QObject(parent)
{

}


void MultiSignalTest::emit_multiSignal1(){ emit multiSignal1(); }
void MultiSignalTest::emit_multiSignal1(double d){ emit multiSignal1(d, QPrivateSignal()); }
void MultiSignalTest::emit_multiSignal1(int i){ emit multiSignal1(i); }
void MultiSignalTest::emit_multiSignal1(int i, const QString& s){ emit multiSignal1(i,s); }
void MultiSignalTest::emit_multiSignal2(const QString& s){ emit multiSignal2(s); }
void MultiSignalTest::emit_multiSignal2(int i){ emit multiSignal2(i); }
void MultiSignalTest::emit_multiSignal2(bool b){ emit multiSignal2(b); }
void MultiSignalTest::emit_multiSignal3(){ emit multiSignal3(); }
void MultiSignalTest::emit_multiSignal3(int i){ emit multiSignal3(i); }
void MultiSignalTest::emit_multiSignal3(bool b){ emit multiSignal3(b); }
void MultiSignalTest::emit_multiSignal3(const QString& s){ emit multiSignal3(s); }
void MultiSignalTest::emit_multiSignal4(const QString& s){ emit multiSignal4(s); }
void MultiSignalTest::emit_multiSignal4(int i){ emit multiSignal4(i); }
void MultiSignalTest::emit_multiSignal4(bool b){ emit multiSignal4(b); }
void MultiSignalTest::emit_multiSignal4(const QString& s, int i){ emit multiSignal4(s,i); }
void MultiSignalTest::emit_multiSignal4(int i, bool b){ emit multiSignal4(i,b); }
void MultiSignalTest::emit_multiSignal4(bool b, const QString& s){ emit multiSignal4(b,s); }
void MultiSignalTest::emit_multiSignal4(bool b, int i, const QString& s){ emit multiSignal4(b,i,s); }
