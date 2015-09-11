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

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.BeforeClass;
import org.junit.Test;

import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QTranslator;
import org.qtjambi.qt.widgets.QApplication;

public class TestI18N extends QApplicationTest {

    @BeforeClass
    public static void testInitialize() throws Exception {
        if("Hello è".compareTo("Hello \u00e8") != 0)
            throw new RuntimeException("Prerequisite 1 of 1: -encoding UTF-8 is required.  This test class can only be expected to work as intended when javac used -encoding UTF-8 for compling the source code.");
        QApplicationTest.testInitialize();
    }

    @Test
    public void testCompilerUsedUtf8WhenCompling() {
        // If this test fails you did not tell the compiler that the source code is UTF-8
        // during the compiling process to fix recompile the project with UTF-8 set.
        assertEquals("Compiler used -encoding UTF-8 check", "Hello è", "Hello \u00e8");
    }

    @Test
    public void TestSimpleNotTranslated() {
        QApplication.installTranslator(null);
        SimpleNotTranslated test = new SimpleNotTranslated();
        test.testNotTranslated();
    }

    @Test
    public void TestSimpleNotTranslatedWithTranslationsLoaded() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");
        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        QApplication.installTranslator(translator);

        SimpleNotTranslated test = new SimpleNotTranslated();
        test.testNotTranslated();

        QApplication.removeTranslator(translator);  // tearDown() static cleanup
    }

    public class SimpleNotTranslated extends QObject {
        public void testNotTranslated() {
            assertEquals(tr("test"), "test");
            assertEquals(tr("\u03c0"), "\u03c0"); // Pi
            assertEquals(tr("\u06a0"), "\u06a0"); // Arabic
            assertEquals(tr("not translated ø"), "not translated ø");
            assertEquals(tr("not translated \u00e8"), "not translated è");
            assertEquals(tr("Hello è"), "Hello \u00e8");
            assertEquals(tr("not translated \n123"), "not translated \n123");
        }
    }

    @Test
    public void TestSimpleTranslated() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");

        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        QApplication.installTranslator(translator);

        SimpleTranslated test = new SimpleTranslated();
        test.testTranslated();

        QApplication.removeTranslator(translator);  // tearDown() static cleanup
    }

    @Test
    public void TestSimpleTranslatedWithTranslationsLoaded() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");

        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        QApplication.installTranslator(translator);

        SimpleTranslated test = new SimpleTranslated();
        test.testTranslated();

        QApplication.removeTranslator(translator);  // tearDown() static cleanup
    }

    public class SimpleTranslated extends QObject {
        public void testTranslated() {
            assertEquals(tr("test"), "test");
            assertEquals(tr("one\ntwo"), "en\nto");
            assertEquals(tr("abc-ø-abc"), "abc-ø-abc"); // e8
            assertEquals(tr("\u00e8"), "ok"); // e8
            assertEquals(tr("\u03c0"), "ok Pi"); // Pi
            assertEquals(tr("\u06a0"), "ok Arabisk"); // Arabic
            assertEquals(tr("Pi"), "\u03c0"); // Pi
            assertEquals(tr("Arabisk"), "\u06a0"); // Arabic
            assertEquals(tr("è"), "ok"); // e8
            assertEquals(tr("not translated abc"), "ikke oversatt æøå");
            assertEquals(tr("not translated æøå"), "ikke oversatt æøå");
            assertEquals(tr("not translated 2 \u00e8"), "ikke oversatt 2 è");
            assertEquals(tr("Hello è"), "Halloys \u00e8");
            assertEquals(tr("My mother 123"), "Min mor 123");
        }
    }

    @Test
    public void TestQTranslatorNotTranslated() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        assertEquals(translator.translate("my context", "do not translate this"), "");
        assertEquals(translator.translate("my context", "do not translate this æøå"), "");
        assertEquals(translator.translate("my context", "do not translate this \u06a0"), "");
    }

    @Test
    public void TestQTranslatorTranslated() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        assertEquals(translator.translate("my context", "translate this"), "oversett dette");
        assertEquals(translator.translate("my context", "translate this æøå"), "oversett dette æøå");
        assertEquals(translator.translate("my context", "translate this \u06a0"), "oversett dette \u06a0");
    }

    @Test
    public void TestQTranslatorTranslatedContext() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        assertEquals(translator.translate("øæå", "translate this"), "oversett dette");
        assertEquals(translator.translate("\u06a0", "translate this æøå"), "oversett dette æøå");
        assertEquals(translator.translate("\u03c0", "translate this \u03c0"), "oversett dette \u03c0");
    }


    @Test
    public void TestReimplementQTranslator() {
        QTranslator translator = new QTranslator() {

            @Override
            public String translate(String context, String sourceText, String comment, int n) {
                return super.translate(context, sourceText, comment, n).toUpperCase();
            }

        };
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        assertEquals(translator.translate("øæå", "translate this"), "OVERSETT DETTE");
        assertEquals(translator.translate("\u06a0", "translate this æøå"), "OVERSETT DETTE ÆØÅ");
        assertEquals(translator.translate("\u03c0", "translate this \u03c0", "Comment"), "OVERSETT DETTE Π");
    }

    @Test
    public void TestQTranslatorAdvancedStrings() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load("classpath:org/qtjambi/autotests/i18n.qm"));
        assertEquals(translator.translate("strings", "a" + "b" + "c"), "abc");
        assertEquals(translator.translate("strings", "c"
                    +"a" + "" +
                    "b"), "cab");

    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestI18N.class.getName());
    }
}
