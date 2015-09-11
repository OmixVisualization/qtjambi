/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.benchmarks;

import org.qtjambi.qt.core.*;

public class SetObjectName {
    public static void main(String args[]) {

        int runningTime = 8000;
        int timesPerRound = 100;

        {
            QTime t = new QTime();
            QObject o = new QObject();
            long counter = 0;
            t.start();
            while (t.elapsed() < runningTime) {
                for (int i=0; i<timesPerRound; ++i) {
                    o.setObjectName("abcde");
                }
                counter += timesPerRound;
            }

            double ops = ((double) counter) / t.elapsed() * 1000;
            System.out.printf("Calling QObject::setObjectName(): %.2fM times pr second\n", ops / 1000000.);
        }

        {
            QTime t = new QTime();
            QFile f = new QFile();
            long counter = 0;
            t.start();
            while (t.elapsed() < runningTime) {
                for (int i=0; i<timesPerRound; ++i) {
                    f.unsetError();
                }
                counter += timesPerRound;
            }

            double ops = ((double) counter) / t.elapsed() * 1000;
            System.out.printf("Calling QFile::unsetError(): %.2fM times pr second\n", ops / 1000000.);
        }


    }
}