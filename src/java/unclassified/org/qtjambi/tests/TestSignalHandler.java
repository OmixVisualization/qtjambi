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

package org.qtjambi.tests;

import org.qtjambi.qt.*;
import org.qtjambi.extensions.signalhandler.*;

public class TestSignalHandler extends QSignalEmitter {

    public Signal0 zero = new Signal0();
    public Signal1<String> one = new Signal1<String>();
    public Signal2<String, String> two = new Signal2<String, String>();
    public Signal3<String, String, String> three = new Signal3<String, String, String>();
    public Signal4<String, String, String, String> four = new Signal4<String, String, String, String>();
    public Signal5<String, String, String, String, String> five = new Signal5<String, String, String, String, String>();
    public Signal6<String, String, String, String, String, String> six = new Signal6<String, String, String, String, String, String>();
    public Signal7<String, String, String, String, String, String, String> seven = new Signal7<String, String, String, String, String, String, String>();
    public Signal8<String, String, String, String, String, String, String, String> eight = new Signal8<String, String, String, String, String, String, String, String>();
    public Signal9<String, String, String, String, String, String, String, String, String> nine = new Signal9<String, String, String, String, String, String, String, String, String>();


    public static void main(String args[]) {
        TestSignalHandler tester = new TestSignalHandler();

        new QSignalHandler0(tester.zero) {
            @Override
            public void handle() {
                System.out.println("got zero signal...");
            }
        };

        new QSignalHandler1<String>(tester.one) {
            @Override
            public void handle(String arg) {
                System.out.println("got 'one' signal: " + arg);
            }
        };

        new QSignalHandler2<String, String>(tester.two) {
            @Override
            public void handle(String arg1, String arg2) {
                System.out.println("got 'two' signal: " + arg1 + ", " + arg2);
            }
        };

        new QSignalHandler3<String, String, String>(tester.three) {
            @Override
            public void handle(String arg1, String arg2, String arg3) {
                System.out.println("got 'three' signal: " + arg1 + ", " + arg2 + ", " + arg3);
            }
        };

        new QSignalHandler4<String, String, String, String>(tester.four) {
            @Override
            public void handle(String a, String b, String c, String d) {
                System.out.println("got 'four' signal: " + a + b + c + d);
            }
        };

        new QSignalHandler5<String, String, String, String, String>(tester.five) {
            @Override
            public void handle(String a, String b, String c, String d, String e) {
                System.out.println("got 'five' signal: " + a + b + c + d + e);
            }
        };

        new QSignalHandler6<String, String, String, String, String, String>(tester.six) {
            @Override
            public void handle(String a, String b, String c, String d, String e, String f) {
                System.out.println("got 'six' signal: " + a + b + c + d + e + f);
            }
        };


        new QSignalHandler7<String, String, String, String, String, String, String>(tester.seven) {
            @Override
            public void handle(String a, String b, String c, String d, String e, String f, String g) {
                System.out.println("got 'seven' signal: " + a + b + c + d + e + f + g);
            }
        };

        new QSignalHandler8<String, String, String, String, String, String, String, String>(tester.eight) {
            @Override
            public void handle(String a, String b, String c, String d, String e, String f, String g, String h) {
                System.out.println("got 'eight' signal: " + a + b + c + d + e + f + g + h);
            }
        };


        new QSignalHandler9<String, String, String, String, String, String, String, String, String>(tester.nine) {
            @Override
            public void handle(String a, String b, String c, String d, String e, String f, String g, String h, String i) {
                System.out.println("got 'nine' signal: " + a + b + c + d + e + f + g + h + i);
            }
        };

        // The actual test...
        tester.zero.emit();
        tester.one.emit("un");
        tester.two.emit("un", "deux");
        tester.three.emit("un", "deux", "trois");
        tester.four.emit("un", "deux", "trois", "quatre");
        tester.five.emit("un", "deux", "trois", "quatre", "cinq");
        tester.six.emit("un", "deux", "trois", "quatre", "cinq", "six");
        tester.seven.emit("un", "deux", "trois", "quatre", "cinq", "six", "sept");
        tester.eight.emit("un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit");
        tester.nine.emit("un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf");

    }
}
