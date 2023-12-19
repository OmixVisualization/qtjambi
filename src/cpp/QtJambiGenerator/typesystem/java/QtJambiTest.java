/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    
class QTest__{

    /**
     * <p>See <a href="@docRoot/qtest.html#QBENCHMARK"><code>QBENCHMARK</code></a></p>
     */
    @QtUninvokable
    public static void QBENCHMARK(Runnable runnable) {
        for (QBenchmarkIterationController __iteration_controller = new QBenchmarkIterationController();
                        __iteration_controller.isDone() == false; __iteration_controller.next())
            runnable.run();
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QBENCHMARK_ONCE"><code>QBENCHMARK_ONCE</code></a></p>
     */
    @QtUninvokable
    public static void QBENCHMARK_ONCE(Runnable runnable) {
        for (QBenchmarkIterationController __iteration_controller = new QBenchmarkIterationController(QBenchmarkIterationController.RunMode.RunOnce);
                        __iteration_controller.isDone() == false; __iteration_controller.next())
            runnable.run();
    }
    
    @QtUninvokable
    public static <T extends Comparable<T>> boolean qCompare(T t1, T t2, java.lang.String actual, java.lang.String expected, java.lang.String file, int line){
        return QTest.compare_helper(t1==null ? t1!=null : t1.compareTo(t2)==0, "Compared values are not the same", 
                "value1", 
                "value2", 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                file, line);
    }
    
    @QtUninvokable
    public static <T> boolean qCompare(T t1, T t2, java.lang.String actual, java.lang.String expected, java.lang.String file, int line){
        return QTest.compare_helper(java.util.Objects.equals(t1, t2), "Compared values are not the same", 
                "value1", 
                "value2", 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                file, line);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2,
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static <T> void QCOMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2,
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(int t1, int t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Integer.toString(t2), 
                java.lang.Integer.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(double t1, double t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Double.toString(t2), 
                java.lang.Double.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(float t1, double t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Double.toString(t2), 
                java.lang.Float.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(double t1, float t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Float.toString(t2), 
                java.lang.Double.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(float t1, float t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Float.toString(t2), 
                java.lang.Float.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(String t1, String t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                t2, 
                t1, 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(io.qt.gui.QIcon t1, io.qt.gui.QIcon t2) throws AssertionError {
        if(t1==null)
            t1 = new io.qt.gui.QIcon();
        if(t2==null)
            t2 = new io.qt.gui.QIcon();
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(io.qt.gui.QPixmap t1, io.qt.gui.QPixmap t2) throws AssertionError {
        if(t1==null)
            t1 = new io.qt.gui.QPixmap();
        if(t2==null)
            t2 = new io.qt.gui.QPixmap();
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE"><code>QCOMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE(io.qt.gui.QImage t1, io.qt.gui.QImage t2) throws AssertionError {
        if(t1==null)
            t1 = new io.qt.gui.QImage();
        if(t2==null)
            t2 = new io.qt.gui.QImage();
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError("Compared values are not the same");
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QEXPECT_FAIL"><code>QEXPECT_FAIL(dataIndex, comment, mode)</code></a></p>
     */
    @QtUninvokable
    public static void QEXPECT_FAIL(String dataIndex, String comment, QTest.TestFailMode mode) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qExpectFail(dataIndex, comment, mode, info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError(comment);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QFAIL"><code>QFAIL(message)</code></a></p>
     */
    @QtUninvokable
    public static void QFAIL(String message) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTest.qFail(message, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
        throw new AssertionError(message);
    }
    
    /**
     * @deprecated Use {@link #QFETCH(String, Class, io.qt.core.QMetaType...)} instead.
     */
    @QtUninvokable
    @Deprecated
    public static <T> T QFETCH(Class<T> type, String name) {
        return io.qt.core.QVariant.convert(qData(name, io.qt.core.QMetaType.fromType(type).id()), type);
    }
    
    /**
     * @deprecated Use {@link #QFETCH_GLOBAL(String, Class, io.qt.core.QMetaType...)} instead.
     */
    @QtUninvokable
    @Deprecated
    public static <T> T QFETCH_GLOBAL(Class<T> type, String name) {
        return io.qt.core.QVariant.convert(qGlobalData(name, io.qt.core.QMetaType.fromType(type).id()), type);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QFETCH"><code>QFETCH(type, name)</code></a></p>
     */
    @QtUninvokable
    public static <T> T QFETCH(String name, Class<T> type, io.qt.core.QMetaType...instantiations) {
        return io.qt.core.QVariant.convert(qData(name, io.qt.core.QMetaType.fromType(type, instantiations).id()), type);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QFETCH_GLOBAL"><code>QFETCH_GLOBAL(type, name)</code></a></p>
     */
    @QtUninvokable
    public static <T> T QFETCH_GLOBAL(String name, Class<T> type, io.qt.core.QMetaType...instantiations) {
        return io.qt.core.QVariant.convert(qGlobalData(name, io.qt.core.QMetaType.fromType(type, instantiations).id()), type);
    }

    /**
     * <p>See <a href="@docRoot/qtest.html#QFINDTESTDATA"><code>QFINDTESTDATA(filename)</code></a></p>
     */
    @QtUninvokable
    public static String QFINDTESTDATA(String basepath){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        return QTest.qFindTestData(basepath, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QFINDTESTDATA"><code>QFINDTESTDATA(filename,builddir)</code></a></p>
     */
    @QtUninvokable
    public static String QFINDTESTDATA(String basepath, java.lang.String builddir){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        return QTest.qFindTestData(basepath, info.declaringClass.getName()+"."+info.methodName, info.lineNumber, builddir);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QSKIP"><code>QSKIP(description)</code></a></p>
     */
    @QtUninvokable
    public static void QSKIP(String statement){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTest.qSkip(statement, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
    }
    
    @QtUninvokable
    public static <T> boolean qTest(T actual, String elementName, String actualStr, String expected, String file, int line) {
        @SuppressWarnings("unchecked")
        Class<T> cls = (Class<T>)actual.getClass();
        return qCompare(actual, io.qt.core.QVariant.convert(QTest.qElementData(elementName, io.qt.core.QMetaType.fromType(cls).id()), cls), actualStr, expected, file, line);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QTEST"><code>QTEST(actual, testElement)</code></a></p>
     */
    @QtUninvokable
    public static <T> void QTEST(T actual, String testElement) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qTest(actual, testElement, java.util.Objects.toString(actual), java.util.Objects.toString(actual), info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError();
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#qExec"><code>QTest::qExec(QObject *, int, char **)</code></a></p>
     */
    @QtUninvokable
    public static int qExec(io.qt.core.QObject testObject, String...args){
        return qExec(testObject, java.util.Arrays.asList(args));
    }
    
    @QtUninvokable
    private static int QTRY_LOOP_IMPL(java.util.function.BooleanSupplier expr, int timeoutValue, int step) {
        if (!(expr.getAsBoolean())) {
            qWait(0);
        }
        int qt_test_i = 0;
        for (; qt_test_i < timeoutValue && !(expr.getAsBoolean()); qt_test_i += step) {
            qWait(step);
        }
        return qt_test_i;
    }
    
    @QtUninvokable
    private static void QTRY_TIMEOUT_DEBUG_IMPL(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier expr, int timeoutValue, int step) throws AssertionError {
        if (!(expr.getAsBoolean())) {
            int qt_test_i = QTRY_LOOP_IMPL(expr, (2 * timeoutValue), step);
            if (expr.getAsBoolean()) {
                String message = String.format("QTestLib: This test case check failed because the requested timeout (%1$s ms) was too short, %2$s ms would have been sufficient this time.", timeoutValue, timeoutValue + qt_test_i);
                QTest.qFail(message, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
                throw new AssertionError(message);
            }
        }
    }
    
    @QtUninvokable
    private static void QTRY_IMPL(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier expr, int timeout) throws AssertionError {
        int qt_test_step = timeout < 350 ? timeout / 7 + 1 : 50;
        int qt_test_timeoutValue = timeout;
        { QTRY_LOOP_IMPL(()->currentTestFailed() || (expr.getAsBoolean()), qt_test_timeoutValue, qt_test_step); }
        QTRY_TIMEOUT_DEBUG_IMPL(info, ()->currentTestFailed() || (expr.getAsBoolean()), qt_test_timeoutValue, qt_test_step);
    }

    @QtUninvokable
    private static void QTRY_COMPARE_WITH_TIMEOUT_IMPL(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier expr, java.util.function.BooleanSupplier equals, int timeout) throws AssertionError {
        int qt_test_step = timeout < 350 ? timeout / 7 + 1 : 50;
        int qt_test_timeoutValue = timeout;
        { 
            QTRY_LOOP_IMPL(()->currentTestFailed() || (expr.getAsBoolean()), qt_test_timeoutValue, qt_test_step); 
        }
        QTRY_TIMEOUT_DEBUG_IMPL(info, ()->currentTestFailed() || (expr.getAsBoolean()), qt_test_timeoutValue, qt_test_step);
        QTRY_IMPL(info, equals, timeout);
        if(!QTest.compare_helper(equals.getAsBoolean(), "Compared values are not the same", 
                "value1", 
                "value2", 
                "value1", 
                "value2", 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber)) {
            throw new AssertionError("Compared values are not the same");
        }
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QTRY_COMPARE_WITH_TIMEOUT"><code>QTRY_COMPARE_WITH_TIMEOUT(actual, expected, timeout)</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QTRY_COMPARE_WITH_TIMEOUT(T t1, T t2, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->t1.compareTo(t2)==0, timeout);
    }

    /**
     * <p>See <a href="@docRoot/qtest.html#QTRY_COMPARE"><code>QTRY_COMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QTRY_COMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->t1.compareTo(t2)==0, 5000);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QTRY_COMPARE_WITH_TIMEOUT"><code>QTRY_COMPARE_WITH_TIMEOUT(actual, expected, timeout)</code></a></p>
     */
    @QtUninvokable
    public static <T> void QTRY_COMPARE_WITH_TIMEOUT(T t1, T t2, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->java.util.Objects.equals(t1, t2), timeout);
    }

    /**
     * <p>See <a href="@docRoot/qtest.html#QTRY_COMPARE"><code>QTRY_COMPARE(actual, expected)</code></a></p>
     */
    @QtUninvokable
    public static <T> void QTRY_COMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->java.util.Objects.equals(t1, t2), 5000);
    }
    
    @QtUninvokable
    public static void QTRY_VERIFY_WITH_TIMEOUT(java.util.function.BooleanSupplier expr, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_IMPL(info, expr, timeout);
        QVERIFY(info, expr);
    }

    /**
     * <p>See <a href="@docRoot/qtest.html#QTRY_VERIFY_WITH_TIMEOUT"><code>QTRY_VERIFY_WITH_TIMEOUT(condition, timeout)</code></a></p>
     */
    @QtUninvokable
    public static void QTRY_VERIFY_WITH_TIMEOUT(java.util.function.BooleanSupplier expr, String messageExpression, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_IMPL(info, expr, timeout);
        QVERIFY(info, expr, messageExpression);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QVERIFY"><code>QVERIFY(condition)</code></a></p>
     */
    @QtUninvokable
    public static void QVERIFY(java.util.function.BooleanSupplier statement) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QVERIFY(info, statement);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QVERIFY"><code>QVERIFY(condition)</code></a></p>
     */
    @QtUninvokable
    private static void QVERIFY(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier statement) throws AssertionError {
        if (!qVerify(statement.getAsBoolean(), "", "", info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError();
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QVERIFY2"><code>QVERIFY2(condition,message)</code></a></p>
     */
    @QtUninvokable
    public static void QVERIFY(java.util.function.BooleanSupplier statement, String description) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QVERIFY(info, statement, description);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QVERIFY2"><code>QVERIFY2(condition,message)</code></a></p>
     */
    @QtUninvokable
    private static void QVERIFY(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier statement, String description) throws AssertionError {
        if(statement.getAsBoolean()) {
            if (!qVerify(true, "", description, info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError(description);
        }else {
            if (!qVerify(false, "", description, info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError(description);
        }
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html"><code>QTest</code></a></p>
     */
    @QtUninvokable
    public static void QWARN(String message){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTest.qWarn(message, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html"><code>QTest</code></a></p>
     */
    @QtUninvokable
    public static io.qt.core.QTemporaryDir QEXTRACTTESTDATA(String resourcePath){
        return QTest.qExtractTestData(resourcePath);
    }
    
    /**
     * Use {@link #addColumn(String, Class, QMetaType...)} instead.
     */
    @Deprecated
    @QtUninvokable
    public static void addColumn(Class<?> type, String name){
        addColumn(name, type);
    }

    /**
     * Place this macro-like method in the main method of the test class.
     * <p>See <a href="@docRoot/qtest.html#QTEST_MAIN"><code>QTEST_MAIN(TestClass)</code></a></p>
     */
    @QtUninvokable
    @SuppressWarnings("unchecked")
    public static int QTEST_MAIN(String... args) {
        io.qt.InternalAccess.CallerContext context = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!io.qt.core.QObject.class.isAssignableFrom(context.declaringClass) || !"main".equals(context.methodName))
            throw new RuntimeException("QTEST_MAIN() can only be called from inside main method of QObject-based type.");
        return QTEST_MAIN_WRAPPER((Class<? extends io.qt.core.QObject>)context.declaringClass, args, io.qt.widgets.QApplication::initialize);
    }
    
    /**
     * Place this macro-like method in the main method of the test class.
     * <p>See <a href="@docRoot/qtest.html#QTEST_MAIN"><code>QTEST_MAIN(TestClass)</code></a></p>
     */
    @QtUninvokable
    @SuppressWarnings("unchecked")
    public static int QTEST_WIDGETLESS_MAIN(String... args) {
        io.qt.InternalAccess.CallerContext context = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!io.qt.core.QObject.class.isAssignableFrom(context.declaringClass) || !"main".equals(context.methodName))
            throw new RuntimeException("QTEST_MAIN() can only be called from inside main method of QObject-based type.");
        return QTEST_MAIN_WRAPPER((Class<? extends io.qt.core.QObject>)context.declaringClass, args, io.qt.gui.QGuiApplication::initialize);
    }
    
    /**
     * Place this macro-like method in the main method of the test class.
     * <p>See <a href="@docRoot/qtest.html#QTEST_GUILESS_MAIN"><code>QTEST_GUILESS_MAIN(TestClass)</code></a></p>
     */
    @QtUninvokable
    @SuppressWarnings("unchecked")
    public static int QTEST_GUILESS_MAIN(String... args) {
        io.qt.InternalAccess.CallerContext context = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!io.qt.core.QObject.class.isAssignableFrom(context.declaringClass) || !"main".equals(context.methodName))
            throw new RuntimeException("QTEST_MAIN() can only be called from inside main method of QObject-based type.");
        return QTEST_MAIN_WRAPPER((Class<? extends io.qt.core.QObject>)context.declaringClass, args, io.qt.core.QCoreApplication::initialize);
    }
    
    /**
     * Place this macro-like method in the main method of the test class.
     * <p>See <a href="@docRoot/qtest.html#QTEST_APPLESS_MAIN"><code>QTEST_APPLESS_MAIN(TestClass)</code></a></p>
     */
    @QtUninvokable
    @SuppressWarnings("unchecked")
    public static int QTEST_APPLESS_MAIN(String... args) {
        io.qt.InternalAccess.CallerContext context = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!io.qt.core.QObject.class.isAssignableFrom(context.declaringClass) || !"main".equals(context.methodName))
            throw new RuntimeException("QTEST_MAIN() can only be called from inside main method of QObject-based type.");
        return QTEST_MAIN_WRAPPER((Class<? extends io.qt.core.QObject>)context.declaringClass, args, null);
    }
    
    private static int QTEST_MAIN_WRAPPER(Class<? extends io.qt.core.QObject> testClass, String[] args, java.util.function.BiConsumer<String,String[]> init) {
        java.lang.reflect.Method initMain = null;
        try {
            initMain = testClass.getMethod("initMain");
        } catch (Throwable e) {}
        if(initMain!=null && java.lang.reflect.Modifier.isStatic(initMain.getModifiers())) {
            try {
                QtJambi_LibraryUtilities.internal.invokeMethod(initMain, null);
            } catch (Throwable e) {
                throw new AssertionError("Error in "+testClass.getName()+".initMain()", e);
            }              
        }
        if(init!=null) {
            init.accept(testClass.getSimpleName(), args);
            io.qt.core.QCoreApplication.setAttribute(io.qt.core.Qt.ApplicationAttribute.AA_Use96Dpi, true);
            if(io.qt.widgets.QApplication.instance()!=null) {
                try {
                    io.qt.widgets.QApplication.setNavigationMode(io.qt.core.Qt.NavigationMode.NavigationModeNone);
                } catch (QNoImplementationException e) {}
            }
        }else {
            String[] newArgs = new String[args.length+1];
            newArgs[0] = testClass.getSimpleName();
            System.arraycopy(args, 0, newArgs, 1, args.length);
            args = newArgs;
        }
        io.qt.core.QObject testObject;
        try {
            testObject = QtJambi_LibraryUtilities.internal.invokeContructor(testClass.getConstructor());
        } catch (NoSuchMethodException e) {
            throw new AssertionError(testClass.getName()+": no default constructor available.", e);
        } catch (Throwable e) {
            throw new AssertionError("Unable to create "+testClass.getName(), e);
        }
        QTest.setMainSourcePath("");
        int result = QTest.qExec(testObject, init!=null ? io.qt.core.QCoreApplication.arguments() : java.util.Arrays.asList(args));
        if(init!=null)
            io.qt.core.QCoreApplication.shutdown();
        return result;
    }
    
}// class

class QTest_5__{

    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent-1"><code>QTest::touchEvent(QWindow *window, QTouchDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QTouchDevice device) {
        return touchEvent(window, device, true);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent-1"><code>QTest::touchEvent(QWindow *window, QTouchDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static native io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QTouchDevice device, boolean autoCommit);
    
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent"><code>QTest::touchEvent(QWidget *widget, QTouchDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QTouchDevice device) {
        return touchEvent(window, device, true);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent"><code>QTest::touchEvent(QWidget *widget, QTouchDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static native io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QTouchDevice device, boolean autoCommit);
    
}// class

class QTest_6__{
    
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent"><code>QTest::touchEvent(QWidget *widget, QPointingDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QPointingDevice device) {
        return touchEvent(window, device, true);
    }
     
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent"><code>QTest::touchEvent(QWidget *widget, QPointingDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QPointingDevice device, boolean autoCommit){
        return new io.qt.test.QTest.QTouchEventSequence(window, device, autoCommit);
    }
     
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent-1"><code>QTest::touchEvent(QWindow *window, QPointingDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static io.qt.test.QTest.QTouchEventWidgetSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QPointingDevice device) {
        return touchEvent(window, device, true);
    }
     
    /**
     * <p>See <a href="@docRoot/qtest.html#touchEvent-1"><code>QTest::touchEvent(QWindow *window, QPointingDevice *device, bool autoCommit = true)</code></a></p>
     */
    @QtUninvokable
    public static native io.qt.test.QTest.QTouchEventWidgetSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QPointingDevice device, boolean autoCommit);
        
    /**
     * <p>See <a href="@docRoot/qtest.html#QFINDTESTDATA"><code>QFINDTESTDATA(filename,builddir)</code></a></p>
     */
    @QtUninvokable
    public static String QFINDTESTDATA(String basepath, java.lang.String builddir, java.lang.String sourcedir){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        return QTest.qFindTestData(basepath, info.declaringClass.getName()+"."+info.methodName, info.lineNumber, builddir, sourcedir);
    }
    
}// class

class QTest_64__{
    @QtUninvokable
    private static <T> void QCOMPARE_OP_IMPL(java.util.function.BiPredicate<T, T> predicate, T lhs, T rhs, ComparisonOperation op) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if (reportResult(predicate.test(lhs, rhs),
                ()->java.util.Objects.toString(lhs),
                ()->java.util.Objects.toString(rhs),
                "leftValue", "rightValue", op, info.declaringClass.getName()+"."+info.methodName, info.lineNumber)) {
            throw new AssertionError("Compared values are not the same");
        }
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_EQ"><code>QCOMPARE_EQ</code></a></p>
     */
    @QtUninvokable
    public static <T> void QCOMPARE_EQ(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL(java.util.Objects::equals, lhs, rhs, ComparisonOperation.Equal);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_NE"><code>QCOMPARE_NE</code></a></p>
     */
    @QtUninvokable
    public static <T> void QCOMPARE_NE(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->!java.util.Objects.equals(l,r), lhs, rhs, ComparisonOperation.Equal);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_EQ"><code>QCOMPARE_EQ</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE_EQ(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l.compareTo(r)==0, lhs, rhs, ComparisonOperation.Equal);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_EQ"><code>QCOMPARE_EQ</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_EQ(int lhs, int rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l==r, lhs, rhs, ComparisonOperation.Equal);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_EQ"><code>QCOMPARE_EQ</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_EQ(double lhs, double rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l==r, lhs, rhs, ComparisonOperation.Equal);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_EQ"><code>QCOMPARE_EQ</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_EQ(long lhs, long rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l==r, lhs, rhs, ComparisonOperation.Equal);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_NE"><code>QCOMPARE_NE</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE_NE(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l.compareTo(r)!=0, lhs, rhs, ComparisonOperation.NotEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_NE"><code>QCOMPARE_NE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_NE(int lhs, int rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l!=r, lhs, rhs, ComparisonOperation.NotEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_NE"><code>QCOMPARE_NE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_NE(double lhs, double rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l!=r, lhs, rhs, ComparisonOperation.NotEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_NE"><code>QCOMPARE_NE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_NE(long lhs, long rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l!=r, lhs, rhs, ComparisonOperation.NotEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LT"><code>QCOMPARE_LT</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE_LT(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l.compareTo(r)<0, lhs, rhs, ComparisonOperation.LessThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LT"><code>QCOMPARE_LT</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_LT(int lhs, int rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l<r, lhs, rhs, ComparisonOperation.LessThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LT"><code>QCOMPARE_LT</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_LT(double lhs, double rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l<r, lhs, rhs, ComparisonOperation.LessThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LT"><code>QCOMPARE_LT</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_LT(long lhs, long rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l<r, lhs, rhs, ComparisonOperation.LessThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LE"><code>QCOMPARE_LE</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE_LE(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l.compareTo(r)<=0, lhs, rhs, ComparisonOperation.LessThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LE"><code>QCOMPARE_LE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_LE(int lhs, int rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l<=r, lhs, rhs, ComparisonOperation.LessThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LE"><code>QCOMPARE_LE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_LE(double lhs, double rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l<=r, lhs, rhs, ComparisonOperation.LessThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_LE"><code>QCOMPARE_LE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_LE(long lhs, long rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l<=r, lhs, rhs, ComparisonOperation.LessThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GT"><code>QCOMPARE_GT</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE_GT(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l.compareTo(r)>0, lhs, rhs, ComparisonOperation.GreaterThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GT"><code>QCOMPARE_GT</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_GT(int lhs, int rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l>r, lhs, rhs, ComparisonOperation.GreaterThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GT"><code>QCOMPARE_GT</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_GT(double lhs, double rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l>r, lhs, rhs, ComparisonOperation.GreaterThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GT"><code>QCOMPARE_GT</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_GT(long lhs, long rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l>r, lhs, rhs, ComparisonOperation.GreaterThan);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GE"><code>QCOMPARE_GE</code></a></p>
     */
    @QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE_GE(T lhs, T rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l.compareTo(r)>=0, lhs, rhs, ComparisonOperation.GreaterThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GE"><code>QCOMPARE_GE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_GE(int lhs, int rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l>=r, lhs, rhs, ComparisonOperation.GreaterThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GE"><code>QCOMPARE_GE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_GE(double lhs, double rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l>=r, lhs, rhs, ComparisonOperation.GreaterThanOrEqual);
    }
    
    /**
     * <p>See <a href="@docRoot/qtest.html#QCOMPARE_GE"><code>QCOMPARE_GE</code></a></p>
     */
    @QtUninvokable
    public static void QCOMPARE_GE(long lhs, long rhs) throws AssertionError {
        QCOMPARE_OP_IMPL((l,r)->l>=r, lhs, rhs, ComparisonOperation.GreaterThanOrEqual);
    }

}// class

class QSignalSpy__{
    
    /**
     * <p>See <a href="@docRoot/qsignalspy.html#QSignalSpy-1"><code>QSignalSpy::QSignalSpy(const QObject *, PointerToMemberFunction)</code></a></p>
     */
    public QSignalSpy(QMetaObject.AbstractSignal signal) {
        super((QPrivateConstructor)null);
        if(!(signal.containingObject() instanceof QObject)) {
            throw new IllegalArgumentException("Signal spy can only operate on QObject signals.");
        }
        QObject obj = (QObject)signal.containingObject();
        QMetaMethod signalMethod = QMetaMethod.fromSignal(signal);
        initialize_native(this, obj, signalMethod);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final void forEach(Consumer<? super java.util.List<java.lang.Object>> action) {
        list().forEach(action);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean addAll(Collection<? extends java.util.List<java.lang.Object>> c) {
        return list().addAll(c);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean addAll(int index, Collection<? extends java.util.List<java.lang.Object>> c) {
        return list().addAll(index, c);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public java.util.List<java.lang.Object> remove(int index) {
        return list().remove(index);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public ListIterator<java.util.List<java.lang.Object>> listIterator() {
        return list().listIterator();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public ListIterator<java.util.List<java.lang.Object>> listIterator(int index) {
        return list().listIterator(index);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public List<java.util.List<java.lang.Object>> subList(int fromIndex, int toIndex) {
        return list().subList(fromIndex, toIndex);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public Object[] toArray() {
        return list().toArray();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public <T> T[] toArray(T[] a) {
        return list().toArray(a);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean containsAll(Collection<?> c) {
        return list().containsAll(c);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public QList<java.util.List<java.lang.Object>> clone() {
        return list().clone();
    }

    /**
     * @see io.qt.core.QList#append(java.lang.Object)
     */
    @QtUninvokable
    public final void append(java.util.Collection<java.util.List<java.lang.Object>> t) {
        list().append(t);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean removeAll(Collection<?> c) {
        return list().removeAll(c);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean retainAll(Collection<?> c) {
        return list().retainAll(c);
    }

    /**
     * @see io.qt.core.QList#append(java.lang.Object)
     */
    @QtUninvokable
    public final void append(java.util.List<java.lang.Object> t) {
        list().append(t);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public Iterator<java.util.List<java.lang.Object>> iterator() {
        return list().iterator();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public String toString() {
        return list().toString();
    }

    /**
     * @see io.qt.core.QList#at(int)
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> at(int i) {
        return list().at(i);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final void clear() {
        list().clear();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final boolean contains(Object t) {
        return list().contains(t);
    }

    /**
     * @see io.qt.core.QList#count()
     */
    @QtUninvokable
    public final int count() {
        return list().count();
    }

    /**
     * @see io.qt.core.QList#count(java.lang.Object)
     */
    @QtUninvokable
    public final int count(java.util.List<java.lang.Object> t) {
        return list().count(t);
    }

    /**
     * @see io.qt.core.QList#endsWith(java.lang.Object)
     */
    @QtUninvokable
    public final boolean endsWith(java.util.List<java.lang.Object> t) {
        return list().endsWith(t);
    }

    /**
     * @see io.qt.core.QList#first()
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> first() {
        return list().first();
    }

    /**
     * @see io.qt.core.QList#indexOf(java.lang.Object,int)
     */
    @QtUninvokable
    public final int indexOf(java.util.List<java.lang.Object> t, int from) {
        return list().indexOf(t, from);
    }

    /**
     * @see io.qt.core.QList#insert(int,java.lang.Object)
     */
    @QtUninvokable
    public final void insert(int i, java.util.List<java.lang.Object> t) {
        list().insert(i, t);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final boolean isEmpty() {
        return list().isEmpty();
    }

    /**
     * @see io.qt.core.QList#last()
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> last() {
        return list().last();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final int lastIndexOf(Object t) {
        return list().lastIndexOf(t);
    }

    /**
     * @see io.qt.core.QList#lastIndexOf(java.lang.Object,int)
     */
    @QtUninvokable
    public final int lastIndexOf(java.util.List<java.lang.Object> t, int from) {
        return list().lastIndexOf(t, from);
    }

    /**
     * @see io.qt.core.QList#length()
     */
    @QtUninvokable
    public final int length() {
        return list().length();
    }

    /**
     * @see io.qt.core.QList#mid(int)
     */
    @QtUninvokable
    public final QList<java.util.List<java.lang.Object>> mid(int pos) {
        return list().mid(pos);
    }

    /**
     * @see io.qt.core.QList#mid(int,int)
     */
    @QtUninvokable
    public final QList<java.util.List<java.lang.Object>> mid(int pos, int length) {
        return list().mid(pos, length);
    }

    /**
     * @see io.qt.core.QList#move(int,int)
     */
    @QtUninvokable
    public final void move(int from, int to) {
        list().move(from, to);
    }

    /**
     * @see io.qt.core.QList#prepend(java.lang.Object)
     */
    @QtUninvokable
    public final void prepend(java.util.List<java.lang.Object> t) {
        list().prepend(t);
    }

    /**
     * @see io.qt.core.QList#removeAll(java.lang.Object)
     */
    @QtUninvokable
    public final int removeAll(java.util.List<java.lang.Object> t) {
        return list().removeAll(t);
    }

    /**
     * @see io.qt.core.QList#removeAt(int)
     */
    @QtUninvokable
    public final void removeAt(int i) {
        list().removeAt(i);
    }

    /**
     * @see io.qt.core.QList#removeOne(java.lang.Object)
     */
    @QtUninvokable
    public final boolean removeOne(java.util.List<java.lang.Object> t) {
        return list().removeOne(t);
    }

    /**
     * @see io.qt.core.QList#replace(int,java.lang.Object)
     */
    @QtUninvokable
    public final void replace(int i, java.util.List<java.lang.Object> t) {
        list().replace(i, t);
    }

    /**
     * @see io.qt.core.QList#reserve(int)
     */
    @QtUninvokable
    public final void reserve(int size) {
        list().reserve(size);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final int size() {
        return list().size();
    }

    /**
     * @see io.qt.core.QList#startsWith(java.lang.Object)
     */
    @QtUninvokable
    public final boolean startsWith(java.util.List<java.lang.Object> t) {
        return list().startsWith(t);
    }

    /**
     * @see io.qt.core.QList#takeAt(int)
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> takeAt(int i) {
        return list().takeAt(i);
    }

    /**
     * @see io.qt.core.QList#takeFirst()
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> takeFirst() {
        return list().takeFirst();
    }

    /**
     * @see io.qt.core.QList#takeLast()
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> takeLast() {
        return list().takeLast();
    }

    /**
     * @see io.qt.core.QList#value(int)
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> value(int i) {
        return list().value(i);
    }

    /**
     * @see io.qt.core.QList#value(int,java.lang.Object)
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> value(int i, java.util.List<java.lang.Object> defaultValue) {
        return list().value(i, defaultValue);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        return list().equals(other);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public int hashCode() {
        return list().hashCode();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean add(java.util.List<java.lang.Object> e) {
        return list().add(e);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public void add(int index, java.util.List<java.lang.Object> e) {
        list().add(index, e);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public java.util.List<java.lang.Object> set(int index, java.util.List<java.lang.Object> e) {
        return list().set(index, e);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public boolean remove(Object e) {
        return list().remove(e);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public int indexOf(Object e) {
        return list().indexOf(e);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public java.util.List<java.lang.Object> get(int index) {
        return list().get(index);
    }

    /**
     * @see io.qt.core.QList#swapItemsAt(int,int)
     */
    @QtUninvokable
    public final void swapItemsAt(int i, int j) {
        list().swapItemsAt(i, j);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final void replaceAll(UnaryOperator<java.util.List<java.lang.Object>> operator) {
        list().replaceAll(operator);
    }

    /**
     * @see io.qt.core.QList#writeTo(QDataStream)
     */
    @QtUninvokable
    public void writeTo(QDataStream stream) {
        list().writeTo(stream);
    }

    /**
     * @see io.qt.core.QList#readFrom(QDataStream)
     */
    @QtUninvokable
    public void readFrom(QDataStream stream) {
        list().readFrom(stream);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final void sort(Comparator<? super java.util.List<java.lang.Object>> c) {
        list().sort(c);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final boolean removeIf(Predicate<? super java.util.List<java.lang.Object>> filter) {
        return list().removeIf(filter);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final Spliterator<java.util.List<java.lang.Object>> spliterator() {
        return list().spliterator();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final Stream<java.util.List<java.lang.Object>> stream() {
        return list().stream();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    @QtUninvokable
    public final Stream<java.util.List<java.lang.Object>> parallelStream() {
        return list().parallelStream();
    }

    /**
     * @see io.qt.core.QList#removeFirst()
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> removeFirst() {
        return list().removeFirst();
    }

    /**
     * @see io.qt.core.QList#removeLast()
     */
    @QtUninvokable
    public final java.util.List<java.lang.Object> removeLast() {
        return list().removeLast();
    }
}// class

class QSignalSpy_5_{
    /**
     * @see io.qt.core.QList#toSet()
     */
    @QtUninvokable
    public final QSet<java.util.List<java.lang.Object>> toSet()    {
        return list().toSet();
    }
}// class

class QSignalSpy_6_{
    /**
     * @see io.qt.core.QList#fill(java.lang.Object)
     */
    @QtUninvokable
    public final void fill(java.util.List<java.lang.Object> t) {
        list().fill(t);
    }

    /**
     * @see io.qt.core.QList#fill(java.lang.Object,int)
     */
    @QtUninvokable
    public final void fill(java.util.List<java.lang.Object> t, int size) {
        list().fill(t, size);
    }

    /**
     * @see io.qt.core.QList#insert(int,int,java.lang.Object)
     */
    @QtUninvokable
    public final void insert(int i, int n, java.util.List<java.lang.Object> t) {
        list().insert(i, n, t);
    }

    /**
     * @see io.qt.core.QList#remove(int,int)
     */
    @QtUninvokable
    public final void remove(int i, int n) {
        list().remove(i, n);
    }

    /**
     * @see io.qt.core.QList#capacity()
     */
    @QtUninvokable
    public final int capacity() {
        return list().capacity();
    }

    /**
     * @see io.qt.core.QList#resize(int)
     */
    @QtUninvokable
    public final void resize(int size) {
        list().resize(size);
    }

    /**
     * @see io.qt.core.QList#squeeze()
     */
    @QtUninvokable
    public final void squeeze() {
        list().squeeze();
    }
}// class
