/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    
class QTest__{

    @io.qt.QtUninvokable
    public static void QBENCHMARK(Runnable runnable) {
        for (QBenchmarkIterationController __iteration_controller = new QBenchmarkIterationController();
                        __iteration_controller.isDone() == false; __iteration_controller.next())
            runnable.run();
    }
    
    @io.qt.QtUninvokable
    public static void QBENCHMARK_ONCE(Runnable runnable) {
        for (QBenchmarkIterationController __iteration_controller = new QBenchmarkIterationController(QBenchmarkIterationController.RunMode.RunOnce);
                        __iteration_controller.isDone() == false; __iteration_controller.next())
            runnable.run();
    }
    
    @io.qt.QtUninvokable
    public static <T extends Comparable<T>> boolean qCompare(T t1, T t2, java.lang.String actual, java.lang.String expected, java.lang.String file, int line){
        return QTest.compare_helper(t1==null ? t1!=null : t1.compareTo(t2)==0, "Compared values are not the same", 
                "value1", 
                "value2", 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                file, line);
    }
    
    @io.qt.QtUninvokable
    public static <T> boolean qCompare(T t1, T t2, java.lang.String actual, java.lang.String expected, java.lang.String file, int line){
        return QTest.compare_helper(java.util.Objects.equals(t1, t2), "Compared values are not the same", 
                "value1", 
                "value2", 
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                file, line);
    }
    
    @io.qt.QtUninvokable
    public static <T extends Comparable<T>> void QCOMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2,
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static <T> void QCOMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2,
                java.util.Objects.toString(t2), 
                java.util.Objects.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static void QCOMPARE(int t1, int t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Integer.toString(t2), 
                java.lang.Integer.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static void QCOMPARE(double t1, double t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Double.toString(t2), 
                java.lang.Double.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static void QCOMPARE(float t1, double t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Double.toString(t2), 
                java.lang.Float.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static void QCOMPARE(double t1, float t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Float.toString(t2), 
                java.lang.Double.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static void QCOMPARE(float t1, float t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                java.lang.Float.toString(t2), 
                java.lang.Float.toString(t1), 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
    public static void QCOMPARE(String t1, String t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qCompare(t1, t2, 
                t2, 
                t1, 
                info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError("Compared values are not the same");
    }
    
    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
    public static void QEXPECT_FAIL(String dataIndex, String comment, QTest.TestFailMode mode) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qExpectFail(dataIndex, comment, mode, info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError(comment);
    }
    
    @io.qt.QtUninvokable
    public static void QFAIL(String message) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTest.qFail(message, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
        throw new AssertionError(message);
    }
    
    @io.qt.QtUninvokable
    public static <T> T QFETCH(Class<T> type, String name) {
        return io.qt.core.QVariant.convert(qData(name, io.qt.core.QMetaType.fromType(type).id()), type);
    }
    
    @io.qt.QtUninvokable
    public static <T> T QFETCH_GLOBAL(Class<T> type, String name) {
        return io.qt.core.QVariant.convert(qGlobalData(name, io.qt.core.QMetaType.fromType(type).id()), type);
    }

    @io.qt.QtUninvokable
    public static String QFINDTESTDATA(String basepath){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        return QTest.qFindTestData(basepath, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
    }
    
    @io.qt.QtUninvokable
    public static String QFINDTESTDATA(String basepath, java.lang.String builddir){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        return QTest.qFindTestData(basepath, info.declaringClass.getName()+"."+info.methodName, info.lineNumber, builddir);
    }
    
    @io.qt.QtUninvokable
    public static void QSKIP(String statement){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTest.qSkip(statement, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
    }
    
    @io.qt.QtUninvokable
    public static <T> boolean qTest(T actual, String elementName, String actualStr, String expected, String file, int line) {
        @SuppressWarnings("unchecked")
        Class<T> cls = (Class<T>)actual.getClass();
        return qCompare(actual, io.qt.core.QVariant.convert(QTest.qElementData(elementName, io.qt.core.QMetaType.fromType(cls).id()), cls), actualStr, expected, file, line);
    }
    
    @io.qt.QtUninvokable
    public static <T> void QTEST(T actual, String testElement) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        if(!QTest.qTest(actual, testElement, java.util.Objects.toString(actual), java.util.Objects.toString(actual), info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError();
    }
    
    @io.qt.QtUninvokable
    public static int qExec(io.qt.core.QObject testObject, String...args){
        return qExec(testObject, java.util.Arrays.asList(args));
    }
    
    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
    private static void QTRY_IMPL(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier expr, int timeout) throws AssertionError {
        int qt_test_step = timeout < 350 ? timeout / 7 + 1 : 50;
        int qt_test_timeoutValue = timeout;
        { QTRY_LOOP_IMPL(()->currentTestFailed() || (expr.getAsBoolean()), qt_test_timeoutValue, qt_test_step); }
        QTRY_TIMEOUT_DEBUG_IMPL(info, ()->currentTestFailed() || (expr.getAsBoolean()), qt_test_timeoutValue, qt_test_step);
    }

    @io.qt.QtUninvokable
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
    
    @io.qt.QtUninvokable
    public static <T extends Comparable<T>> void QTRY_COMPARE_WITH_TIMEOUT(T t1, T t2, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->t1.compareTo(t2)==0, timeout);
    }

    @io.qt.QtUninvokable
    public static <T extends Comparable<T>> void QTRY_COMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->t1.compareTo(t2)==0, 5000);
    }
    
    @io.qt.QtUninvokable
    public static <T> void QTRY_COMPARE_WITH_TIMEOUT(T t1, T t2, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->java.util.Objects.equals(t1, t2), timeout);
    }

    @io.qt.QtUninvokable
    public static <T> void QTRY_COMPARE(T t1, T t2) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_COMPARE_WITH_TIMEOUT_IMPL(info, ()->t1!=null, ()->java.util.Objects.equals(t1, t2), 5000);
    }
    
    @io.qt.QtUninvokable
    public static void QTRY_VERIFY_WITH_TIMEOUT(java.util.function.BooleanSupplier expr, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_IMPL(info, expr, timeout);
        QVERIFY(info, expr);
    }

    @io.qt.QtUninvokable
    public static void QTRY_VERIFY_WITH_TIMEOUT(java.util.function.BooleanSupplier expr, String messageExpression, int timeout) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTRY_IMPL(info, expr, timeout);
        QVERIFY(info, expr, messageExpression);
    }
    
    @io.qt.QtUninvokable
    public static void QVERIFY(java.util.function.BooleanSupplier statement) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QVERIFY(info, statement);
    }
    
    @io.qt.QtUninvokable
    private static void QVERIFY(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier statement) throws AssertionError {
        if (!qVerify(statement.getAsBoolean(), "", "", info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
            throw new AssertionError();
    }
    
    @io.qt.QtUninvokable
    public static void QVERIFY(java.util.function.BooleanSupplier statement, String description) throws AssertionError {
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QVERIFY(info, statement, description);
    }
    
    @io.qt.QtUninvokable
    private static void QVERIFY(io.qt.InternalAccess.CallerContext info, java.util.function.BooleanSupplier statement, String description) throws AssertionError {
        if(statement.getAsBoolean()) {
            if (!qVerify(true, "", description, info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError(description);
        }else {
            if (!qVerify(false, "", description, info.declaringClass.getName()+"."+info.methodName, info.lineNumber))
                throw new AssertionError(description);
        }
    }
    
    @io.qt.QtUninvokable
    public static void QWARN(String message){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        QTest.qWarn(message, info.declaringClass.getName()+"."+info.methodName, info.lineNumber);
    }
    
    @io.qt.QtUninvokable
    public static io.qt.core.QTemporaryDir QEXTRACTTESTDATA(String resourcePath){
        return QTest.qExtractTestData(resourcePath);
    }
    
    @io.qt.QtUninvokable
    public static void addColumn(Class<?> type, String name){
        addColumnInternal(io.qt.core.QMetaType.fromType(type).id(), name);
    }
    
}// class

class QTest_5__{

    @io.qt.QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QTouchDevice device) {
        return touchEvent(window, device, true);
    }
    
    @io.qt.QtUninvokable
    public static native io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QTouchDevice device, boolean autoCommit);
    
    @io.qt.QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QTouchDevice device) {
        return touchEvent(window, device, true);
    }
    
    @io.qt.QtUninvokable
    public static native io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QTouchDevice device, boolean autoCommit);
    
}// class

class QTest_6__{
    
    @io.qt.QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QPointingDevice device) {
        return touchEvent(window, device, true);
    }
    @io.qt.QtUninvokable
    public static io.qt.test.QTest.QTouchEventSequence touchEvent(io.qt.gui.QWindow window, io.qt.gui.QPointingDevice device, boolean autoCommit){
        return new io.qt.test.QTest.QTouchEventSequence(window, device, autoCommit);
    }
    @io.qt.QtUninvokable
    public static io.qt.test.QTest.QTouchEventWidgetSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QPointingDevice device) {
        return touchEvent(window, device, true);
    }
    @io.qt.QtUninvokable
    public static native io.qt.test.QTest.QTouchEventWidgetSequence touchEvent(io.qt.widgets.QWidget window, io.qt.gui.QPointingDevice device, boolean autoCommit);
        
    @io.qt.QtUninvokable
    public static String QFINDTESTDATA(String basepath, java.lang.String builddir, java.lang.String sourcedir){
        io.qt.InternalAccess.CallerContext info = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
        return QTest.qFindTestData(basepath, info.declaringClass.getName()+"."+info.methodName, info.lineNumber, builddir, sourcedir);
    }
    
}// class

class QSignalSpy__{
    
    public QSignalSpy(QMetaObject.AbstractSignal signal) {
        super((QPrivateConstructor)null);
        if(!(signal.containingObject() instanceof QObject)) {
            throw new IllegalArgumentException("Signal spy can only operate on QObject signals.");
        }
        QObject obj = (QObject)signal.containingObject();
        QMetaMethod signalMethod = QMetaMethod.fromSignal(signal);
        initialize_native(this, obj, signalMethod);
    }

    @io.qt.QtUninvokable
    public final void forEach(Consumer<? super java.util.List<java.lang.Object>> action) {
        list().forEach(action);
    }

    @io.qt.QtUninvokable
    public boolean addAll(Collection<? extends java.util.List<java.lang.Object>> c) {
        return list().addAll(c);
    }

    @io.qt.QtUninvokable
    public boolean addAll(int index, Collection<? extends java.util.List<java.lang.Object>> c) {
        return list().addAll(index, c);
    }

    @io.qt.QtUninvokable
    public java.util.List<java.lang.Object> remove(int index) {
        return list().remove(index);
    }

    @io.qt.QtUninvokable
    public ListIterator<java.util.List<java.lang.Object>> listIterator() {
        return list().listIterator();
    }

    @io.qt.QtUninvokable
    public ListIterator<java.util.List<java.lang.Object>> listIterator(int index) {
        return list().listIterator(index);
    }

    @io.qt.QtUninvokable
    public List<java.util.List<java.lang.Object>> subList(int fromIndex, int toIndex) {
        return list().subList(fromIndex, toIndex);
    }

    @io.qt.QtUninvokable
    public Object[] toArray() {
        return list().toArray();
    }

    @io.qt.QtUninvokable
    public <T> T[] toArray(T[] a) {
        return list().toArray(a);
    }

    @io.qt.QtUninvokable
    public boolean containsAll(Collection<?> c) {
        return list().containsAll(c);
    }

    @io.qt.QtUninvokable
    public QList<java.util.List<java.lang.Object>> clone() {
        return list().clone();
    }

    @io.qt.QtUninvokable
    public final void append(Collection<java.util.List<java.lang.Object>> t) {
        list().append(t);
    }

    @io.qt.QtUninvokable
    public boolean removeAll(Collection<?> c) {
        return list().removeAll(c);
    }

    @io.qt.QtUninvokable
    public boolean retainAll(Collection<?> c) {
        return list().retainAll(c);
    }

    @io.qt.QtUninvokable
    public final void append(java.util.List<java.lang.Object> t) {
        list().append(t);
    }

    @io.qt.QtUninvokable
    public Iterator<java.util.List<java.lang.Object>> iterator() {
        return list().iterator();
    }

    @io.qt.QtUninvokable
    public String toString() {
        return list().toString();
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> at(int i) {
        return list().at(i);
    }

    @io.qt.QtUninvokable
    public final void clear() {
        list().clear();
    }

    @io.qt.QtUninvokable
    public final boolean contains(Object t) {
        return list().contains(t);
    }

    @io.qt.QtUninvokable
    public final int count() {
        return list().count();
    }

    @io.qt.QtUninvokable
    public final int count(java.util.List<java.lang.Object> t) {
        return list().count(t);
    }

    @io.qt.QtUninvokable
    public final boolean endsWith(java.util.List<java.lang.Object> t) {
        return list().endsWith(t);
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> first() {
        return list().first();
    }

    @io.qt.QtUninvokable
    public final int indexOf(java.util.List<java.lang.Object> t, int from) {
        return list().indexOf(t, from);
    }

    @io.qt.QtUninvokable
    public final void insert(int i, java.util.List<java.lang.Object> t) {
        list().insert(i, t);
    }

    @io.qt.QtUninvokable
    public final boolean isEmpty() {
        return list().isEmpty();
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> last() {
        return list().last();
    }

    @io.qt.QtUninvokable
    public final int lastIndexOf(Object t) {
        return list().lastIndexOf(t);
    }

    @io.qt.QtUninvokable
    public final int lastIndexOf(java.util.List<java.lang.Object> t, int from) {
        return list().lastIndexOf(t, from);
    }

    @io.qt.QtUninvokable
    public final int length() {
        return list().length();
    }

    @io.qt.QtUninvokable
    public final QList<java.util.List<java.lang.Object>> mid(int pos) {
        return list().mid(pos);
    }

    @io.qt.QtUninvokable
    public final QList<java.util.List<java.lang.Object>> mid(int pos, int length) {
        return list().mid(pos, length);
    }

    @io.qt.QtUninvokable
    public final void move(int from, int to) {
        list().move(from, to);
    }

    @io.qt.QtUninvokable
    public final void prepend(java.util.List<java.lang.Object> t) {
        list().prepend(t);
    }

    @io.qt.QtUninvokable
    public final int removeAll(java.util.List<java.lang.Object> t) {
        return list().removeAll(t);
    }

    @io.qt.QtUninvokable
    public final void removeAt(int i) {
        list().removeAt(i);
    }

    @io.qt.QtUninvokable
    public final boolean removeOne(java.util.List<java.lang.Object> t) {
        return list().removeOne(t);
    }

    @io.qt.QtUninvokable
    public final void replace(int i, java.util.List<java.lang.Object> t) {
        list().replace(i, t);
    }

    @io.qt.QtUninvokable
    public final void reserve(int size) {
        list().reserve(size);
    }

    @io.qt.QtUninvokable
    public final int size() {
        return list().size();
    }

    @io.qt.QtUninvokable
    public final boolean startsWith(java.util.List<java.lang.Object> t) {
        return list().startsWith(t);
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> takeAt(int i) {
        return list().takeAt(i);
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> takeFirst() {
        return list().takeFirst();
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> takeLast() {
        return list().takeLast();
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> value(int i) {
        return list().value(i);
    }

    @io.qt.QtUninvokable
    public final java.util.List<java.lang.Object> value(int i, java.util.List<java.lang.Object> defaultValue) {
        return list().value(i, defaultValue);
    }

    @io.qt.QtUninvokable
    public boolean equals(Object other) {
        return list().equals(other);
    }

    @io.qt.QtUninvokable
    public int hashCode() {
        return list().hashCode();
    }

    @io.qt.QtUninvokable
    public boolean add(java.util.List<java.lang.Object> e) {
        return list().add(e);
    }

    @io.qt.QtUninvokable
    public void add(int index, java.util.List<java.lang.Object> e) {
        list().add(index, e);
    }

    @io.qt.QtUninvokable
    public java.util.List<java.lang.Object> set(int index, java.util.List<java.lang.Object> e) {
        return list().set(index, e);
    }

    @io.qt.QtUninvokable
    public boolean remove(Object e) {
        return list().remove(e);
    }

    @io.qt.QtUninvokable
    public int indexOf(Object e) {
        return list().indexOf(e);
    }

    @io.qt.QtUninvokable
    public java.util.List<java.lang.Object> get(int index) {
        return list().get(index);
    }

    @io.qt.QtUninvokable
    public final void swapItemsAt(int i, int j) {
        list().swapItemsAt(i, j);
    }

    @io.qt.QtUninvokable
    public final void replaceAll(UnaryOperator<java.util.List<java.lang.Object>> operator) {
        list().replaceAll(operator);
    }

    @io.qt.QtUninvokable
    public void writeTo(QDataStream stream) {
        list().writeTo(stream);
    }

    @io.qt.QtUninvokable
    public void readFrom(QDataStream stream) {
        list().readFrom(stream);
    }

    @io.qt.QtUninvokable
    public final void sort(Comparator<? super java.util.List<java.lang.Object>> c) {
        list().sort(c);
    }

    @io.qt.QtUninvokable
    public final boolean removeIf(Predicate<? super java.util.List<java.lang.Object>> filter) {
        return list().removeIf(filter);
    }

    @io.qt.QtUninvokable
    public final Spliterator<java.util.List<java.lang.Object>> spliterator() {
        return list().spliterator();
    }

    @io.qt.QtUninvokable
    public final Stream<java.util.List<java.lang.Object>> stream() {
        return list().stream();
    }

    @io.qt.QtUninvokable
    public final Stream<java.util.List<java.lang.Object>> parallelStream() {
        return list().parallelStream();
    }
    
    @io.qt.QtUninvokable
    public final void removeFirst() {
        list().removeFirst();
    }

    @io.qt.QtUninvokable
    public final void removeLast() {
        list().removeLast();
    }
}// class

class QSignalSpy_5_{
    @io.qt.QtUninvokable
    public final QSet<java.util.List<java.lang.Object>> toSet()    {
        return list().toSet();
    }
}// class

class QSignalSpy_6_{
    @io.qt.QtUninvokable
    public final void fill(java.util.List<java.lang.Object> t) {
        list().fill(t);
    }

    @io.qt.QtUninvokable
    public final void fill(java.util.List<java.lang.Object> t, int size) {
        list().fill(t, size);
    }

    @io.qt.QtUninvokable
    public final void insert(int i, int n, java.util.List<java.lang.Object> t) {
        list().insert(i, n, t);
    }

    @io.qt.QtUninvokable
    public final void remove(int i, int n) {
        list().remove(i, n);
    }

    @io.qt.QtUninvokable
    public final int capacity() {
        return list().capacity();
    }

    @io.qt.QtUninvokable
    public final void resize(int size) {
        list().resize(size);
    }

    @io.qt.QtUninvokable
    public final void squeeze() {
        list().squeeze();
    }
}// class
