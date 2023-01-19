/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.autotests;

import static io.qt.core.QObject.tr;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QObject;
import io.qt.core.QResource;
import io.qt.core.QTranslator;
import io.qt.widgets.QApplication;

public class TestI18N extends ApplicationInitializer {

    @BeforeClass
    public static void testInitialize() throws Exception {
    	Assert.assertTrue("Prerequisite 1 of 1: -encoding UTF-8 is required.  This test class can only be expected to work as intended when javac used -encoding UTF-8 for compling the source code.", "Hello \u00e8".compareTo("Hello è") == 0);
        ApplicationInitializer.testInitialize();
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
    	QResource.addClassPath(".");
        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        QApplication.installTranslator(translator);

        SimpleNotTranslated test = new SimpleNotTranslated();
        test.testNotTranslated();

        QApplication.removeTranslator(translator);  // tearDown() static cleanup
    }
    
    public class SuperClass{
    	public void testTranslation() {
    		assertEquals("SuperClass.text", tr("text"));
    	}
    }
    
    public class SubClass extends SuperClass{
    	public void testTranslation() {
    		super.testTranslation();
    		assertEquals("SubClass.text", tr("text"));
    	}
    }
    
    @Test
    public void testInheritedTranslate() {
    	QResource.addClassPath(".");

        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        QApplication.installTranslator(translator);

        SubClass test = new SubClass();
        test.testTranslation();

        QApplication.removeTranslator(translator);  // tearDown() static cleanup
    }

    public class SimpleNotTranslated extends QObject {
        public void testNotTranslated() {
            assertEquals("test", tr("test"));
//            assertEquals("\u03c0", tr("\u03c0")); // Pi
//            assertEquals("\u06a0", tr("\u06a0")); // Arabic
            assertEquals("not translated ø", tr("not translated ø"));
            assertEquals("not translated è", tr("not translated \u00e8"));
//            assertEquals("Hello \u00e8", tr("Hello è"));
            assertEquals("not translated \n123", tr("not translated \n123"));
        }
    }

    @Test
    public void TestSimpleTranslated() {
    	QResource.addClassPath(".");

        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        QApplication.installTranslator(translator);

        SimpleTranslated test = new SimpleTranslated();
        test.testTranslated();

        QApplication.removeTranslator(translator);  // tearDown() static cleanup
    }

    @Test
    public void TestSimpleTranslatedWithTranslationsLoaded() {
    	QResource.addClassPath(".");

        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
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
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        assertEquals(translator.translate("my context", "do not translate this"), "");
        assertEquals(translator.translate("my context", "do not translate this æøå"), "");
        assertEquals(translator.translate("my context", "do not translate this \u06a0"), "");
    }

    @Test
    public void TestQTranslatorTranslated() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        assertEquals(translator.translate("my context", "translate this"), "oversett dette");
        assertEquals(translator.translate("my context", "translate this æøå"), "oversett dette æøå");
        assertEquals(translator.translate("my context", "translate this \u06a0"), "oversett dette \u06a0");
    }

    @Test
    public void TestQTranslatorTranslatedContext() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
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
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        assertEquals(translator.translate("øæå", "translate this"), "OVERSETT DETTE");
        assertEquals(translator.translate("\u06a0", "translate this æøå"), "OVERSETT DETTE ÆØÅ");
        assertEquals(translator.translate("\u03c0", "translate this \u03c0", "Comment"), "OVERSETT DETTE Π");
    }

    @Test
    public void TestQTranslatorAdvancedStrings() {
        QTranslator translator = new QTranslator();
        assertTrue(translator.load(":io/qt/autotests/i18n.qm"));
        assertEquals(translator.translate("strings", "a" + "b" + "c"), "abc");
        assertEquals(translator.translate("strings", "c"
                    +"a" + "" +
                    "b"), "cab");

    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestI18N.class.getName());
    }
}
