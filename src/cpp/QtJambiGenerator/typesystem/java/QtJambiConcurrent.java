/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

class QtConcurrent___ extends QtConcurrent {
    
    public static final class ThreadEngineStarter<T> extends io.qt.QtObject{
        private ThreadEngineStarter(QPrivateConstructor p){
            super(p);
        }

        public final QFuture<T> startAsynchronously(){
            QFuture<T> result = startAsynchronously(QtJambi_LibraryUtilities.internal.nativeId(this));
            dispose();
            return result;
        }
        
        @io.qt.QtUninvokable
        private static native final <T> QFuture<T> startAsynchronously(long nativeId);
    }
    
    /**
     * An implemetation of this interface is given one to QtConcurrent's map() methods.
     * The map() method of this interface is called for each object in a java.util.Collection.
     *
     */
    @FunctionalInterface
    public interface MapFunctor<T> {
    
        /**
         * This function is called for each item in the Collection. The function is then free to alter <code>object</code> as it see fit.
         */
        @io.qt.NativeAccess
        public void map(T object);
    }
    
    /**
     * Implement this interface to perform a mapped operation. An implementation of the interface is sendt
     * to ome of the mapped methods of QtConcurrent, which applies the MappedFunctor.map() method to all elements in a collection,
     * and returns the result.
     */
    @FunctionalInterface
    public interface MappedFunctor<U, T> {
    
        /**
         * This method is called for each object in a collection. It should returned a new altered
         * object.
         */
        @io.qt.NativeAccess
        public U map(T object);
    }
    
    /**
     * An implementation of this interface is given to one of QtConcurrent's filtered() methods.
     * The filter method if this interface is called for each item in a java.util.Collection.
     *
     */
    public interface FilteredFunctor<T> {
    
        /**
         *  This method is called for each item in a java.util.Collection. The items for which
         *  this method returns true are removed from the collection.
         */
        public boolean filter(T object);
    }
    
    /**
     * Implement this interface in order to perform a reduce operation.
     * <p>
     * The reduce method will be called once per intermediate result (the result of the mapping of the data)
     * and the very first time the reduce() method is called for the particular data set, the result is set to
     * the returned value of the defaultResult() method.
     */
    @FunctionalInterface
    public interface ReduceFunctor<U, T> extends java.io.Serializable{
        /**
         * Performs a reduce operation on <code>intermediate</code>.
         * Returns the result of the reduction.
         */
        @io.qt.NativeAccess
        public U reduce(U result, T intermediate);
    }
    
    @SuppressWarnings("unchecked")
    @io.qt.QtUninvokable
    private static <U> U computeDefaultResult(ReduceFunctor<U,?> functor) {
        int[] types = QtJambi_LibraryUtilities.internal.lambdaMetaTypes(ReduceFunctor.class, functor);
        if(types!=null && types.length>0) {
            return (U)new QMetaType(types[0]).create();
        }
        return null;
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#map">QtConcurrent::map(Sequence, MapFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<Void> map(Collection<T> sequence, MapFunctor<T> functor){
        return startMap(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor)).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMap">QtConcurrent::blockingMap(Sequence, MapFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> void blockingMap(Collection<T> sequence, MapFunctor<T> functor){
        QFuture<Void> future = map(sequence, functor);
        future.waitForFinished();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mapped">QtConcurrent::mapped(Sequence, MappedFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> mapped(Collection<T> sequence, MappedFunctor<U, T> functor){
        return startMapped(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor)).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMapped">QtConcurrent::blockingMapped(Sequence, MappedFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QList<U> blockingMapped(Collection<T> sequence, MappedFunctor<U, T> functor){
        QFuture<U> future = mapped(sequence, functor);
        return future.results();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor) {
        return mappedReduced(sequence, functor, reduceFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption ... options) {
        return mappedReduced(sequence, functor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
           return startMappedReduced(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value()).startAsynchronously();
     }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor) {
        QFuture<U> future = mappedReduced(sequence, functor, reduceFunctor);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption ... options) {
        QFuture<U> future = mappedReduced(sequence, functor, reduceFunctor, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
        QFuture<U> future = mappedReduced(sequence, functor, reduceFunctor, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filter">QtConcurrent::filter(Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<Void> filter(Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return filter(0, Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor));
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilter">QtConcurrent::blockingFilter(Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> void blockingFilter(Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        QFuture<Void> future = filter(sequence, filteredFunctor);
        future.waitForFinished();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filtered">QtConcurrent::filtered(Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> filtered(Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return startFiltered(0, Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor)).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFiltered">QtConcurrent::blockingFiltered(Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QList<T> blockingFiltered(Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        QFuture<T> future = filtered(sequence, filteredFunctor);
        return future.results();
    }
        
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        return startFilteredReduced(0, Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value()).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor) {
        QFuture<U> future = filteredReduced(sequence, filteredFunctor, reduceFunctor);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        QFuture<U> future = filteredReduced(sequence, filteredFunctor, reduceFunctor, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        QFuture<U> future = filteredReduced(sequence, filteredFunctor, reduceFunctor, options);
        return future.result();
    }
    
    @io.qt.QtUninvokable
    private native static <T> QFuture<Void> filter(long threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor);
    
    @io.qt.QtUninvokable
    private native static <T> ThreadEngineStarter<Void> startMap(long threadPool, Collection<T> sequence, MapFunctor<T> functor);
    
    @io.qt.QtUninvokable
    private native static <U, T> ThreadEngineStarter<U> startMapped(long threadPool, Collection<T> sequence, MappedFunctor<U, T> functor);
    
    @io.qt.QtUninvokable
    private native static <U, V, T> ThreadEngineStarter<U> startMappedReduced(long threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, int options);
    
    @io.qt.QtUninvokable
    private native static <U, T> ThreadEngineStarter<U> startFilteredReduced(long threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, int options);
    
    @io.qt.QtUninvokable
    private native static <T> ThreadEngineStarter<T> startFiltered(long threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> run(Callable<T> callable){
        return run0(null, Objects.requireNonNull(callable));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> run(QThreadPool threadPool, Callable<T> callable){
        return run0(threadPool, Objects.requireNonNull(callable));
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> run0(QThreadPool threadPool, Callable<T> callable);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    @io.qt.QtUninvokable
    public static QFuture<Void> run(Runnable runnable) {
        return runVoid0(null, Objects.requireNonNull(runnable));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    @io.qt.QtUninvokable
    public static QFuture<Void> run(QThreadPool threadPool, Runnable runnable) {
        return runVoid0(threadPool, Objects.requireNonNull(runnable));
    }
    @io.qt.QtUninvokable
    private native static QFuture<Void> runVoid0(QThreadPool threadPool, Runnable runnable);
    
    @FunctionalInterface
    public interface Runnable1<A>{
        public void run(A a);
    }
    
    @FunctionalInterface
    public interface Runnable2<A,B>{
        public void run(A a, B b);
    }
    
    @FunctionalInterface
    public interface Runnable3<A,B,C>{
        public void run(A a, B b, C c);
    }
    
    @FunctionalInterface
    public interface Runnable4<A,B,C,D>{
        public void run(A a, B b, C c, D d);
    }
    
    @FunctionalInterface
    public interface Runnable5<A,B,C,D,E>{
        public void run(A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A> QFuture<Void> run(Runnable1<A> runnable, A a) {
        return runVoid1(null, runnable, a);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A> QFuture<Void> run(QThreadPool threadPool, Runnable1<A> runnable, A a) {
        return runVoid1(threadPool, Objects.requireNonNull(runnable), a);
    }
    private native static <A> QFuture<Void> runVoid1(QThreadPool threadPool, Runnable1<A> runnable, A a);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B> QFuture<Void> run(Runnable2<A,B> runnable, A a, B b) {
        return runVoid2(null, Objects.requireNonNull(runnable), a, b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B> QFuture<Void> run(QThreadPool threadPool, Runnable2<A,B> runnable, A a, B b) {
        return runVoid2(threadPool, Objects.requireNonNull(runnable), a, b);
    }
    private native static <A,B> QFuture<Void> runVoid2(QThreadPool threadPool, Runnable2<A,B> runnable, A a, B b);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C> QFuture<Void> run(Runnable3<A,B,C> runnable, A a, B b, C c) {
        return runVoid3(null, Objects.requireNonNull(runnable), a, b, c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C> QFuture<Void> run(QThreadPool threadPool, Runnable3<A,B,C> runnable, A a, B b, C c) {
        return runVoid3(threadPool, Objects.requireNonNull(runnable), a, b, c);
    }
    private native static <A,B,C> QFuture<Void> runVoid3(QThreadPool threadPool, Runnable3<A,B,C> runnable, A a, B b, C c);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C,D> QFuture<Void> run(Runnable4<A,B,C,D> runnable, A a, B b, C c, D d) {
        return runVoid4(null, Objects.requireNonNull(runnable), a, b, c, d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C,D> QFuture<Void> run(QThreadPool threadPool, Runnable4<A,B,C,D> runnable, A a, B b, C c, D d) {
        return runVoid4(threadPool, Objects.requireNonNull(runnable), a, b, c, d);
    }
    private native static <A,B,C,D> QFuture<Void> runVoid4(QThreadPool threadPool, Runnable4<A,B,C,D> runnable, A a, B b, C c, D d);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C,D,E> QFuture<Void> run(Runnable5<A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return runVoid5(null, Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C,D,E> QFuture<Void> run(QThreadPool threadPool, Runnable5<A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return runVoid5(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    private native static <A,B,C,D,E> QFuture<Void> runVoid5(QThreadPool threadPool, Runnable5<A,B,C,D,E> runnable, A a, B b, C c, D d, E e);
    
    
    
    @FunctionalInterface
    public interface Callable1<T,A>{
        public T call(A a);
    }
    
    @FunctionalInterface
    public interface Callable2<T,A,B>{
        public T call(A a, B b);
    }
    
    @FunctionalInterface
    public interface Callable3<T,A,B,C>{
        public T call(A a, B b, C c);
    }
    
    @FunctionalInterface
    public interface Callable4<T,A,B,C,D>{
        public T call(A a, B b, C c, D d);
    }
    
    @FunctionalInterface
    public interface Callable5<T,A,B,C,D,E>{
        public T call(A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A> QFuture<T> run(Callable1<T,A> runnable, A a) {
        return run1(null, Objects.requireNonNull(runnable), a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A> QFuture<T> run(QThreadPool threadPool, Callable1<T,A> runnable, A a) {
        return run1(threadPool, Objects.requireNonNull(runnable), a);
    }
    private native static <T,A> QFuture<T> run1(QThreadPool threadPool, Callable1<T,A> runnable, A a);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B> QFuture<T> run(Callable2<T,A,B> runnable, A a, B b) {
        return run2(null, Objects.requireNonNull(runnable), a, b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B> QFuture<T> run(QThreadPool threadPool, Callable2<T,A,B> runnable, A a, B b) {
        return run2(threadPool, Objects.requireNonNull(runnable), a, b);
    }
    private native static <T,A,B> QFuture<T> run2(QThreadPool threadPool, Callable2<T,A,B> runnable, A a, B b);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C> QFuture<T> run(Callable3<T,A,B,C> runnable, A a, B b, C c) {
        return run3(null, Objects.requireNonNull(runnable), a, b, c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C> QFuture<T> run(QThreadPool threadPool, Callable3<T,A,B,C> runnable, A a, B b, C c) {
        return run3(threadPool, Objects.requireNonNull(runnable), a, b, c);
    }
    private native static <T,A,B,C> QFuture<T> run3(QThreadPool threadPool, Callable3<T,A,B,C> runnable, A a, B b, C c);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C,D> QFuture<T> run(Callable4<T,A,B,C,D> runnable, A a, B b, C c, D d) {
        return run4(null, Objects.requireNonNull(runnable), a, b, c, d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C,D> QFuture<T> run(QThreadPool threadPool, Callable4<T,A,B,C,D> runnable, A a, B b, C c, D d) {
        return run4(threadPool, Objects.requireNonNull(runnable), a, b, c, d);
    }
    private native static <T,A,B,C,D> QFuture<T> run4(QThreadPool threadPool, Callable4<T,A,B,C,D> runnable, A a, B b, C c, D d);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E> QFuture<T> run(Callable5<T,A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return run5(null, Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E> QFuture<T> run(QThreadPool threadPool, Callable5<T,A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return run5(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    private native static <T,A,B,C,D,E> QFuture<T> run5(QThreadPool threadPool, Callable5<T,A,B,C,D,E> runnable, A a, B b, C c, D d, E e);
    
}// class

class QtConcurrent_5__ extends QtConcurrent {
    
    public static <T> ThreadEngineStarter<Void> startMap(Collection<T> sequence, MapFunctor<T> functor) {
        return startMap(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startMapped(Collection<T> sequence, MappedFunctor<U, T> functor){
        return startMapped(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
        return startMappedReduced(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption... options) {
        return startMappedReduced(sequence, functor, reduceFunctor, new ReduceOptions(options));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        return startFilteredReduced(sequence, filteredFunctor, reduceFunctor, new ReduceOptions(options));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        return startFilteredReduced(0, Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    public static <T> ThreadEngineStarter<T> startFiltered(Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return startFiltered(0, Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor));
    }
    
}// class

class QtConcurrent_6__ extends QtConcurrent {
    
    @FunctionalInterface
    public interface Runnable6<A,B,C,D,E,F>{
        public void run(A a, B b, C c, D d, E e, F f);
    }
    
    @FunctionalInterface
    public interface Runnable7<A,B,C,D,E,F,G>{
        public void run(A a, B b, C c, D d, E e, F f, G g);
    }
    
    @FunctionalInterface
    public interface Runnable8<A,B,C,D,E,F,G,H>{
        public void run(A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    @FunctionalInterface
    public interface Runnable9<A,B,C,D,E,F,G,H,I>{
        public void run(A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    @FunctionalInterface
    public interface Callable6<T,A,B,C,D,E,F>{
        public T call(A a, B b, C c, D d, E e, F f);
    }
    
    @FunctionalInterface
    public interface Callable7<T,A,B,C,D,E,F,G>{
        public T call(A a, B b, C c, D d, E e, F f, G g);
    }
    
    @FunctionalInterface
    public interface Callable8<T,A,B,C,D,E,F,G,H>{
        public T call(A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    @FunctionalInterface
    public interface Callable9<T,A,B,C,D,E,F,G,H,I>{
        public T call(A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F> QFuture<Void> run(Runnable6<A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return runVoid6(null, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F> QFuture<Void> run(QThreadPool threadPool, Runnable6<A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return runVoid6(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }
    private native static <A,B,C,D,E,F> QFuture<Void> runVoid6(QThreadPool threadPool, Runnable6<A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F,G> QFuture<Void> run(Runnable7<A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return runVoid7(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F,G> QFuture<Void> run(QThreadPool threadPool, Runnable7<A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return runVoid7(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }
    private native static <A,B,C,D,E,F,G> QFuture<Void> runVoid7(QThreadPool threadPool, Runnable7<A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F,G,H> QFuture<Void> run(Runnable8<A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runVoid8(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F,G,H> QFuture<Void> run(QThreadPool threadPool, Runnable8<A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runVoid8(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }
    private native static <A,B,C,D,E,F,G,H> QFuture<Void> runVoid8(QThreadPool threadPool, Runnable8<A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F,G,H,I> QFuture<Void> run(Runnable9<A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runVoid9(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A,B,C,D,E,F,G,H,I> QFuture<Void> run(QThreadPool threadPool, Runnable9<A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runVoid9(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    private native static <A,B,C,D,E,F,G,H,I> QFuture<Void> runVoid9(QThreadPool threadPool, Runnable9<A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F> QFuture<T> run(Callable6<T,A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return run6(null, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F> QFuture<T> run(QThreadPool threadPool, Callable6<T,A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return run6(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }
    private native static <T,A,B,C,D,E,F> QFuture<T> run6(QThreadPool threadPool, Callable6<T,A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F,G> QFuture<T> run(Callable7<T,A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return run7(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F,G> QFuture<T> run(QThreadPool threadPool, Callable7<T,A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return run7(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }
    private native static <T,A,B,C,D,E,F,G> QFuture<T> run7(QThreadPool threadPool, Callable7<T,A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F,G,H> QFuture<T> run(Callable8<T,A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return run8(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F,G,H> QFuture<T> run(QThreadPool threadPool, Callable8<T,A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return run8(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }
    private native static <T,A,B,C,D,E,F,G,H> QFuture<T> run8(QThreadPool threadPool, Callable8<T,A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F,G,H,I> QFuture<T> run(Callable9<T,A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return run9(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T,A,B,C,D,E,F,G,H,I> QFuture<T> run(QThreadPool threadPool, Callable9<T,A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return run9(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    private native static <T,A,B,C,D,E,F,G,H,I> QFuture<T> run9(QThreadPool threadPool, Callable9<T,A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#map">QtConcurrent::map(QThreadPool*, Sequence, MapFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<Void> map(QThreadPool threadPool, Collection<T> sequence, MapFunctor<T> functor){
        return startMap(threadPool, sequence, functor).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMap">QtConcurrent::blockingMap(QThreadPool*, Sequence, MapFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> void blockingMap(QThreadPool threadPool, Collection<T> sequence, MapFunctor<T> functor){
        QFuture<Void> future = map(threadPool, sequence, functor);
        future.waitForFinished();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mapped">QtConcurrent::mapped(QThreadPool*, Sequence, MappedFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> mapped(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<U, T> functor){
        return startMapped(threadPool, sequence, functor).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMapped">QtConcurrent::blockingMapped(QThreadPool*, Sequence, MappedFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QList<U> blockingMapped(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<U, T> functor){
        QFuture<U> future = mapped(threadPool, sequence, functor);
        return future.results();
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor) {
        return mappedReduced(threadPool, sequence, functor, reduceFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption ... options) {
        return mappedReduced(threadPool, sequence, functor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, options).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue) {
        return mappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOption ... options) {
        return mappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOptions options) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, options).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue) {
        return mappedReduced(sequence, functor, reduceFunctor, initialValue, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOption ... options) {
        return mappedReduced(sequence, functor, reduceFunctor, initialValue, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#mappedReduced">QtConcurrent::mappedReduced(Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOptions options) {
        return startMappedReduced(0, Objects.requireNonNull(sequence), Objects.requireNonNull(functor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value()).startAsynchronously();
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor) {
        QFuture<U> future = mappedReduced(threadPool, sequence, functor, reduceFunctor);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption ... options) {
        QFuture<U> future = mappedReduced(threadPool, sequence, functor, reduceFunctor, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
        QFuture<U> future = mappedReduced(threadPool, sequence, functor, reduceFunctor, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue) {
        QFuture<U> future = mappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOption ... options) {
        QFuture<U> future = mappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(QThreadPool*, Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOptions options) {
        QFuture<U> future = mappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue) {
        QFuture<U> future = mappedReduced(sequence, functor, reduceFunctor, initialValue);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOption ... options) {
        QFuture<U> future = mappedReduced(sequence, functor, reduceFunctor, initialValue, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingMappedReduced">QtConcurrent::blockingMappedReduced(Sequence, MappedFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOptions options) {
        QFuture<U> future = mappedReduced(sequence, functor, reduceFunctor, initialValue, options);
        return future.result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filter">QtConcurrent::filter(QThreadPool*, Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<Void> filter(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return filter(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), filteredFunctor);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filtered">QtConcurrent::filtered(QThreadPool*, Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> filtered(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return startFiltered(threadPool, sequence, filteredFunctor).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFiltered">QtConcurrent::blockingFiltered(QThreadPool*, Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> QList<T> blockingFiltered(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        QFuture<T> future = filtered(threadPool, sequence, filteredFunctor);
        return future.results();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilter">QtConcurrent::blockingFilter(QThreadPool*, Sequence, FilteredFunctor)</a></p>
     */
    @io.qt.QtUninvokable
    public static <T> void blockingFilter(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        QFuture<Void> future = filter(threadPool, sequence, filteredFunctor);
        future.waitForFinished();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, options).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOption ... options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOptions options) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, options).startAsynchronously();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, initialValue, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOption ... options) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, initialValue, new ReduceOptions(options));
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#filteredReduced">QtConcurrent::filteredReduced(Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOptions options) {
        return startFilteredReduced(0, Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), initialValue, options.value()).startAsynchronously();
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, options).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, options).result();
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOption ... options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, options).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(QThreadPool*, Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOptions options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, options).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, initialValue).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOption ... options) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, initialValue, options).result();
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#blockingFilteredReduced">QtConcurrent::blockingFilteredReduced(Sequence, FilteredFunctor, ReduceFunctor, InitialValueType, QtConcurrent::ReduceOptions)</a></p>
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOptions options) {
        return filteredReduced(sequence, filteredFunctor, reduceFunctor, initialValue, options).result();
    }
    
    @io.qt.QtUninvokable
    public static <T> ThreadEngineStarter<Void> startMap(QThreadPool threadPool, Collection<T> sequence, MapFunctor<T> functor) {
        return startMap(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startMapped(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<U, T> functor){
        return startMapped(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, computeDefaultResult(reduceFunctor), ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
        return startMappedReduced(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(functor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption... options) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, new ReduceOptions(options));
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOptions options) {
        return startMappedReduced(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(functor), Objects.requireNonNull(reduceFunctor), initialValue, options.value());
    }
    
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, U initialValue, ReduceOption... options) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, initialValue, new ReduceOptions(options));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, new ReduceOptions(options));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        return startFilteredReduced(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), computeDefaultResult(reduceFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOption ... options) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, initialValue, new ReduceOptions(options));
    }
    
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, U initialValue, ReduceOptions options) {
        return startFilteredReduced(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), initialValue, options.value());
    }
    
    @io.qt.QtUninvokable
    public static <T> ThreadEngineStarter<T> startFiltered(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return startFiltered(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(threadPool)), Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor));
    }
    
    @FunctionalInterface
    public interface RunnableWithPromise<T> {
        public void run(QPromise<T> promise);
    }
    
    @FunctionalInterface
    public interface RunnableWithPromise1<T, A> {
        public void run(QPromise<T> promise, A a);
    }

    @FunctionalInterface
    public interface RunnableWithPromise2<T, A, B> {
        public void run(QPromise<T> promise, A a, B b);
    }

    @FunctionalInterface
    public interface RunnableWithPromise3<T, A, B, C> {
        public void run(QPromise<T> promise, A a, B b, C c);
    }

    @FunctionalInterface
    public interface RunnableWithPromise4<T, A, B, C, D> {
        public void run(QPromise<T> promise, A a, B b, C c, D d);
    }

    @FunctionalInterface
    public interface RunnableWithPromise5<T, A, B, C, D, E> {
        public void run(QPromise<T> promise, A a, B b, C c, D d, E e);
    }

    @FunctionalInterface
    public interface RunnableWithPromise6<T, A, B, C, D, E, F> {
        public void run(QPromise<T> promise, A a, B b, C c, D d, E e, F f);
    }

    @FunctionalInterface
    public interface RunnableWithPromise7<T, A, B, C, D, E, F, G> {
        public void run(QPromise<T> promise, A a, B b, C c, D d, E e, F f, G g);
    }

    @FunctionalInterface
    public interface RunnableWithPromise8<T, A, B, C, D, E, F, G, H> {
        public void run(QPromise<T> promise, A a, B b, C c, D d, E e, F f, G g, H h);
    }

    @FunctionalInterface
    public interface RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> {
        public void run(QPromise<T> promise, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    @FunctionalInterface
    public interface RunnableWithVoidPromise {
        public void run(QPromise<Void> promise);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise1<A> {
        public void run(QPromise<Void> promise, A a);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise2<A, B> {
        public void run(QPromise<Void> promise, A a, B b);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise3<A, B, C> {
        public void run(QPromise<Void> promise, A a, B b, C c);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise4<A, B, C, D> {
        public void run(QPromise<Void> promise, A a, B b, C c, D d);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise5<A, B, C, D, E> {
        public void run(QPromise<Void> promise, A a, B b, C c, D d, E e);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise6<A, B, C, D, E, F> {
        public void run(QPromise<Void> promise, A a, B b, C c, D d, E e, F f);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise7<A, B, C, D, E, F, G> {
        public void run(QPromise<Void> promise, A a, B b, C c, D d, E e, F f, G g);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> {
        public void run(QPromise<Void> promise, A a, B b, C c, D d, E e, F f, G g, H h);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> {
        public void run(QPromise<Void> promise, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T> QFuture<T> run(RunnableWithPromise<T> runnable) {
        return runWithPromise0(null, runnable);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise<T> runnable) {
        return runWithPromise0(threadPool, runnable);
    }

    private native static <T> QFuture<T> runWithPromise0(QThreadPool threadPool, RunnableWithPromise<T> runnable);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A> QFuture<T> run(RunnableWithPromise1<T, A> runnable, A a) {
        return runWithPromise1(null, Objects.requireNonNull(runnable), a);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise1<T, A> runnable, A a) {
        return runWithPromise1(threadPool, Objects.requireNonNull(runnable), a);
    }

    private native static <T, A> QFuture<T> runWithPromise1(QThreadPool threadPool, RunnableWithPromise1<T, A> runnable, A a);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B> QFuture<T> run(RunnableWithPromise2<T, A, B> runnable, A a, B b) {
        return runWithPromise2(null, Objects.requireNonNull(runnable), a, b);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable, A a, B b) {
        return runWithPromise2(threadPool, Objects.requireNonNull(runnable), a, b);
    }

    private native static <T, A, B> QFuture<T> runWithPromise2(QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable, A a, B b);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C> QFuture<T> run(RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c) {
        return runWithPromise3(null, Objects.requireNonNull(runnable), a, b, c);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c) {
        return runWithPromise3(threadPool, Objects.requireNonNull(runnable), a, b, c);
    }

    private native static <T, A, B, C> QFuture<T> runWithPromise3(QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a, B b,
            C c);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C, D> QFuture<T> run(RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d) {
        return runWithPromise4(null, Objects.requireNonNull(runnable), a, b, c, d);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C, D> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b,
            C c, D d) {
        return runWithPromise4(threadPool, Objects.requireNonNull(runnable), a, b, c, d);
    }

    private native static <T, A, B, C, D> QFuture<T> runWithPromise4(QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable,
            A a, B b, C c, D d);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E> QFuture<T> run(RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
        return runWithPromise5(null, Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a,
            B b, C c, D d, E e) {
        return runWithPromise5(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    private native static <T, A, B, C, D, E> QFuture<T> runWithPromise5(QThreadPool threadPool,
            RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F> QFuture<T> run(RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e,
            F f) {
        return runWithPromise6(null, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable,
            A a, B b, C c, D d, E e, F f) {
        return runWithPromise6(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    private native static <T, A, B, C, D, E, F> QFuture<T> runWithPromise6(QThreadPool threadPool,
            RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F, G> QFuture<T> run(RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c,
            D d, E e, F f, G g) {
        return runWithPromise7(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F, G> QFuture<T> run(QThreadPool threadPool,
            RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return runWithPromise7(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    private native static <T, A, B, C, D, E, F, G> QFuture<T> runWithPromise7(QThreadPool threadPool,
            RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H> QFuture<T> run(RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b,
            C c, D d, E e, F f, G g, H h) {
        return runWithPromise8(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H> QFuture<T> run(QThreadPool threadPool,
            RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runWithPromise8(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    private native static <T, A, B, C, D, E, F, G, H> QFuture<T> runWithPromise8(QThreadPool threadPool,
            RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H, I> QFuture<T> run(RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a,
            B b, C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromise9(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H, I> QFuture<T> run(QThreadPool threadPool,
            RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromise9(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> runWithPromise9(QThreadPool threadPool,
            RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static QFuture<Void> run(RunnableWithVoidPromise runnable) {
        return runWithPromiseVoid0(null, runnable);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise runnable) {
        return runWithPromiseVoid0(threadPool, runnable);
    }

    private native static QFuture<Void> runWithPromiseVoid0(QThreadPool threadPool, RunnableWithVoidPromise runnable);
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A> QFuture<Void> run(RunnableWithVoidPromise1<A> runnable, A a) {
        return runWithPromiseVoid1(null, Objects.requireNonNull(runnable), a);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise1<A> runnable, A a) {
        return runWithPromiseVoid1(threadPool, Objects.requireNonNull(runnable), a);
    }

    private native static <A> QFuture<Void> runWithPromiseVoid1(QThreadPool threadPool, RunnableWithVoidPromise1<A> runnable, A a);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B> QFuture<Void> run(RunnableWithVoidPromise2<A, B> runnable, A a, B b) {
        return runWithPromiseVoid2(null, Objects.requireNonNull(runnable), a, b);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable, A a, B b) {
        return runWithPromiseVoid2(threadPool, Objects.requireNonNull(runnable), a, b);
    }

    private native static <A, B> QFuture<Void> runWithPromiseVoid2(QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable, A a, B b);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C> QFuture<Void> run(RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c) {
        return runWithPromiseVoid3(null, Objects.requireNonNull(runnable), a, b, c);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c) {
        return runWithPromiseVoid3(threadPool, Objects.requireNonNull(runnable), a, b, c);
    }

    private native static <A, B, C> QFuture<Void> runWithPromiseVoid3(QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b,
            C c);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C, D> QFuture<Void> run(RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d) {
        return runWithPromiseVoid4(null, Objects.requireNonNull(runnable), a, b, c, d);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C, D> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c,
            D d) {
        return runWithPromiseVoid4(threadPool, Objects.requireNonNull(runnable), a, b, c, d);
    }

    private native static <A, B, C, D> QFuture<Void> runWithPromiseVoid4(QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a,
            B b, C c, D d);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C, D, E> QFuture<Void> run(RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
        return runWithPromiseVoid5(null, Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C, D, E> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b,
            C c, D d, E e) {
        return runWithPromiseVoid5(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    private native static <A, B, C, D, E> QFuture<Void> runWithPromiseVoid5(QThreadPool threadPool,
            RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F> QFuture<Void> run(RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e,
            F f) {
        return runWithPromiseVoid6(null, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a,
            B b, C c, D d, E e, F f) {
        return runWithPromiseVoid6(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    private native static <A, B, C, D, E, F> QFuture<Void> runWithPromiseVoid6(QThreadPool threadPool,
            RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F, G> QFuture<Void> run(RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d,
            E e, F f, G g) {
        return runWithPromiseVoid7(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F, G> QFuture<Void> run(QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable,
            A a, B b, C c, D d, E e, F f, G g) {
        return runWithPromiseVoid7(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    private native static <A, B, C, D, E, F, G> QFuture<Void> runWithPromiseVoid7(QThreadPool threadPool,
            RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F, G, H> QFuture<Void> run(RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c,
            D d, E e, F f, G g, H h) {
        return runWithPromiseVoid8(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F, G, H> QFuture<Void> run(QThreadPool threadPool,
            RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runWithPromiseVoid8(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    private native static <A, B, C, D, E, F, G, H> QFuture<Void> runWithPromiseVoid8(QThreadPool threadPool,
            RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F, G, H, I> QFuture<Void> run(RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b,
            C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromiseVoid9(null, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#run">QtConcurrent::run(QThreadPool*, Function, ...)</a></p>
     */
    public static <A, B, C, D, E, F, G, H, I> QFuture<Void> run(QThreadPool threadPool,
            RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromiseVoid9(threadPool, Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    private native static <A, B, C, D, E, F, G, H, I> QFuture<Void> runWithPromiseVoid9(QThreadPool threadPool,
            RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static abstract class QTaskBuilder<T>{
        QTaskBuilder(int priority, QThreadPool threadPool) {
            super();
            this.priority = priority;
            this.threadPool = threadPool;
        }
        private int priority;
        private QThreadPool threadPool;
        
        final int priority() {return priority;}
        final QThreadPool threadPool() { return threadPool; }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        public QFuture<T> spawn() {
            throw new RuntimeException("Unable to spawn due to missing arguments.");
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn-1">QtConcurrent::QTaskBuilder::spawn(QtConcurrent::FutureResult)</a></p>
         */
        public final void spawn(QtConcurrent.FutureResult result) {
            spawn();
        }

        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder::withPriority(int)</a></p>
         */
        public QTaskBuilder<T> withPriority(int newPriority) {
            this.priority = newPriority;
            return this;
        }
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder::onThreadPool(QThreadPool&)</a></p>
         */
        public QTaskBuilder<T> onThreadPool(QThreadPool newThreadPool) {
            this.threadPool = Objects.requireNonNull(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static abstract class QTypedTaskBuilder<T> extends QTaskBuilder<T>{
        public QTypedTaskBuilder(int priority, QThreadPool threadPool) {
            super(priority, threadPool);
        }
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static QTaskBuilderVoid0Arg0 task(Runnable runnable) {
        return new QTaskBuilderVoid0Arg0(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTaskBuilderVoid0Arg0 extends QTaskBuilder<Void>{
        QTaskBuilderVoid0Arg0(int newPriority, QThreadPool threadPool, Runnable runnable) {
            super(newPriority, threadPool);
            this.runnable = runnable;
        }
        private final Runnable runnable;
        
        @Override
        public QTaskBuilderVoid0Arg0 withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid0Arg0 onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable);
        }
        
        private native static QFuture<Void> spawn(QThreadPool threadPool, int priority, Runnable runnable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A> QTaskBuilderVoid1Arg0<A> task(Runnable1<A> runnable) {
        return new QTaskBuilderVoid1Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid1Arg0<A> extends QTaskBuilder<Void>{
        private final Runnable1<A> runnable;
        
        QTaskBuilderVoid1Arg0(int priority, QThreadPool threadPool, Runnable1<A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid1Arg1<A> withArguments(A a) {
            return new QTaskBuilderVoid1Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        @Override
        public QTaskBuilderVoid1Arg0<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid1Arg0<A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTaskBuilderVoid1Arg1<A> extends QTaskBuilder<Void>{
        QTaskBuilderVoid1Arg1(int newPriority, QThreadPool threadPool, Runnable1<A> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable1<A> runnable;
        private final A a;
        
        @Override
        public QTaskBuilderVoid1Arg1<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid1Arg1<A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a);
        }
        
        private native static <A> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable1<A> runnable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B> QTaskBuilderVoid2Arg0<A, B> task(Runnable2<A, B> runnable) {
        return new QTaskBuilderVoid2Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid2Arg0<A, B> extends QTaskBuilder<Void>{
        private final Runnable2<A, B> runnable;
        
        QTaskBuilderVoid2Arg0(int priority, QThreadPool threadPool, Runnable2<A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid2Arg1<A, B> withArguments(A a) {
            return new QTaskBuilderVoid2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid2Arg2<A, B> withArguments(A a, B b) {
            return new QTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        @Override
        public QTaskBuilderVoid2Arg0<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid2Arg0<A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTaskBuilderVoid2Arg1<A, B> extends QTaskBuilder<Void>{
        QTaskBuilderVoid2Arg1(int newPriority, QThreadPool threadPool, Runnable2<A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable2<A, B> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid2Arg2<A, B> withArguments(B b) {
            return new QTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        @Override
        public QTaskBuilderVoid2Arg1<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid2Arg1<A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid2Arg2<A, B> extends QTaskBuilder<Void>{
        QTaskBuilderVoid2Arg2(int newPriority, QThreadPool threadPool, Runnable2<A, B> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable2<A, B> runnable;
        private final A a;
        private final B b;
        
        @Override
        public QTaskBuilderVoid2Arg2<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid2Arg2<A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b);
        }
        
        private native static <A, B> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable2<A, B> runnable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C> QTaskBuilderVoid3Arg0<A, B, C> task(Runnable3<A, B, C> runnable) {
        return new QTaskBuilderVoid3Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid3Arg0<A, B, C> extends QTaskBuilder<Void>{
        private final Runnable3<A, B, C> runnable;
        
        QTaskBuilderVoid3Arg0(int priority, QThreadPool threadPool, Runnable3<A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid3Arg1<A, B, C> withArguments(A a) {
            return new QTaskBuilderVoid3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid3Arg2<A, B, C> withArguments(A a, B b) {
            return new QTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid3Arg3<A, B, C> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QTaskBuilderVoid3Arg0<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid3Arg0<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTaskBuilderVoid3Arg1<A, B, C> extends QTaskBuilder<Void>{
        QTaskBuilderVoid3Arg1(int newPriority, QThreadPool threadPool, Runnable3<A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable3<A, B, C> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid3Arg2<A, B, C> withArguments(B b) {
            return new QTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid3Arg3<A, B, C> withArguments(B b, C c) {
            return new QTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QTaskBuilderVoid3Arg1<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid3Arg1<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid3Arg2<A, B, C> extends QTaskBuilder<Void>{
        QTaskBuilderVoid3Arg2(int newPriority, QThreadPool threadPool, Runnable3<A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable3<A, B, C> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid3Arg3<A, B, C> withArguments(C c) {
            return new QTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QTaskBuilderVoid3Arg2<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid3Arg2<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid3Arg3<A, B, C> extends QTaskBuilder<Void>{
        QTaskBuilderVoid3Arg3(int newPriority, QThreadPool threadPool, Runnable3<A, B, C> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable3<A, B, C> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        @Override
        public QTaskBuilderVoid3Arg3<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid3Arg3<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c);
        }
        
        private native static <A, B, C> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable3<A, B, C> runnable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D> QTaskBuilderVoid4Arg0<A, B, C, D> task(Runnable4<A, B, C, D> runnable) {
        return new QTaskBuilderVoid4Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid4Arg0<A, B, C, D> extends QTaskBuilder<Void>{
        private final Runnable4<A, B, C, D> runnable;
        
        QTaskBuilderVoid4Arg0(int priority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg1<A, B, C, D> withArguments(A a) {
            return new QTaskBuilderVoid4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg2<A, B, C, D> withArguments(A a, B b) {
            return new QTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTaskBuilderVoid4Arg0<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid4Arg0<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTaskBuilderVoid4Arg1<A, B, C, D> extends QTaskBuilder<Void>{
        QTaskBuilderVoid4Arg1(int newPriority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable4<A, B, C, D> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg2<A, B, C, D> withArguments(B b) {
            return new QTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(B b, C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTaskBuilderVoid4Arg1<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid4Arg1<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid4Arg2<A, B, C, D> extends QTaskBuilder<Void>{
        QTaskBuilderVoid4Arg2(int newPriority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(C c, D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTaskBuilderVoid4Arg2<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid4Arg2<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid4Arg3<A, B, C, D> extends QTaskBuilder<Void>{
        QTaskBuilderVoid4Arg3(int newPriority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTaskBuilderVoid4Arg3<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid4Arg3<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid4Arg4<A, B, C, D> extends QTaskBuilder<Void>{
        QTaskBuilderVoid4Arg4(int newPriority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        @Override
        public QTaskBuilderVoid4Arg4<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid4Arg4<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d);
        }
        
        private native static <A, B, C, D> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable4<A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E> QTaskBuilderVoid5Arg0<A, B, C, D, E> task(Runnable5<A, B, C, D, E> runnable) {
        return new QTaskBuilderVoid5Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid5Arg0<A, B, C, D, E> extends QTaskBuilder<Void>{
        private final Runnable5<A, B, C, D, E> runnable;
        
        QTaskBuilderVoid5Arg0(int priority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg1<A, B, C, D, E> withArguments(A a) {
            return new QTaskBuilderVoid5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(A a, B b) {
            return new QTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTaskBuilderVoid5Arg0<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid5Arg0<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid5Arg1<A, B, C, D, E> extends QTaskBuilder<Void>{
        QTaskBuilderVoid5Arg1(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(B b) {
            return new QTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(B b, C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTaskBuilderVoid5Arg1<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid5Arg1<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid5Arg2<A, B, C, D, E> extends QTaskBuilder<Void>{
        QTaskBuilderVoid5Arg2(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTaskBuilderVoid5Arg2<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid5Arg2<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid5Arg3<A, B, C, D, E> extends QTaskBuilder<Void>{
        QTaskBuilderVoid5Arg3(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid5Arg4<A, B, C, D, E> extends QTaskBuilder<Void>{
        QTaskBuilderVoid5Arg4(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid5Arg5<A, B, C, D, E> extends QTaskBuilder<Void>{
        QTaskBuilderVoid5Arg5(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        @Override
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid5Arg5<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <A, B, C, D, E> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F> QTaskBuilderVoid6Arg0<A, B, C, D, E, F> task(Runnable6<A, B, C, D, E, F> runnable) {
        return new QTaskBuilderVoid6Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg0<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        private final Runnable6<A, B, C, D, E, F> runnable;
        
        QTaskBuilderVoid6Arg0(int priority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg1<A, B, C, D, E, F> withArguments(A a) {
            return new QTaskBuilderVoid6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTaskBuilderVoid6Arg0<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg0<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg1<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QTaskBuilderVoid6Arg1(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(B b) {
            return new QTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTaskBuilderVoid6Arg1<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg1<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg2<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QTaskBuilderVoid6Arg2(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg2<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg3<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QTaskBuilderVoid6Arg3(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg4<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QTaskBuilderVoid6Arg4(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg5<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QTaskBuilderVoid6Arg5(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid6Arg6<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QTaskBuilderVoid6Arg6(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        @Override
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid6Arg6<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <A, B, C, D, E, F> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F, G> QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> task(Runnable7<A, B, C, D, E, F, G> runnable) {
        return new QTaskBuilderVoid7Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        
        QTaskBuilderVoid7Arg0(int priority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withArguments(A a) {
            return new QTaskBuilderVoid7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg1(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(B b) {
            return new QTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg2(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg3(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg4(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
    
        @Override
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg5(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg6(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QTaskBuilderVoid7Arg7(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        @Override
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <A, B, C, D, E, F, G> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F, G, H> QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> task(Runnable8<A, B, C, D, E, F, G, H> runnable) {
        return new QTaskBuilderVoid8Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        
        QTaskBuilderVoid8Arg0(int priority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTaskBuilderVoid8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg1(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg2(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg3(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg4(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg5(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg6(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg7(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QTaskBuilderVoid8Arg8(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        @Override
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }

        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <A, B, C, D, E, F, G, H> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F, G, H, I> QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> task(Runnable9<A, B, C, D, E, F, G, H, I> runnable) {
        return new QTaskBuilderVoid9Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        
        QTaskBuilderVoid9Arg0(int priority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTaskBuilderVoid9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg1(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg2(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg3(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg4(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg5(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg6(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg7(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg8(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QTaskBuilderVoid9Arg9(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
            this.i = i;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        private final I i;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <A, B, C, D, E, F, G, H, I> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T> QTypedTaskBuilder0Arg0<T> task(java.util.concurrent.Callable<T> callable) {
        return new QTypedTaskBuilder0Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder0Arg0<T> extends QTaskBuilder<T>{
        QTypedTaskBuilder0Arg0(int newPriority, QThreadPool threadPool, java.util.concurrent.Callable<T> callable) {
            super(newPriority, threadPool);
            this.callable = callable;
        }
        private final java.util.concurrent.Callable<T> callable;
        
        @Override
        public QTypedTaskBuilder0Arg0<T> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder0Arg0<T> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable);
        }
        
        private native static <T> QFuture<T> spawn(QThreadPool threadPool, int priority, java.util.concurrent.Callable<T> callable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A> QTypedTaskBuilder1Arg0<T, A> task(Callable1<T, A> callable) {
        return new QTypedTaskBuilder1Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder1Arg0<T, A> extends QTaskBuilder<T>{
        private final Callable1<T, A> callable;
        
        QTypedTaskBuilder1Arg0(int priority, QThreadPool threadPool, Callable1<T, A> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder1Arg1<T, A> withArguments(A a) {
            return new QTypedTaskBuilder1Arg1<>(priority(), threadPool(), callable, a);
        }
        
        @Override
        public QTypedTaskBuilder1Arg0<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder1Arg0<T, A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder1Arg1<T, A> extends QTaskBuilder<T>{
        QTypedTaskBuilder1Arg1(int newPriority, QThreadPool threadPool, Callable1<T, A> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable1<T, A> callable;
        private final A a;
        
        @Override
        public QTypedTaskBuilder1Arg1<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder1Arg1<T, A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a);
        }
        
        private native static <T, A> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable1<T, A> callable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B> QTypedTaskBuilder2Arg0<T, A, B> task(Callable2<T, A, B> callable) {
        return new QTypedTaskBuilder2Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder2Arg0<T, A, B> extends QTaskBuilder<T>{
        private final Callable2<T, A, B> callable;
        
        QTypedTaskBuilder2Arg0(int priority, QThreadPool threadPool, Callable2<T, A, B> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder2Arg1<T, A, B> withArguments(A a) {
            return new QTypedTaskBuilder2Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder2Arg2<T, A, B> withArguments(A a, B b) {
            return new QTypedTaskBuilder2Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        @Override
        public QTypedTaskBuilder2Arg0<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder2Arg0<T, A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder2Arg1<T, A, B> extends QTaskBuilder<T>{
        QTypedTaskBuilder2Arg1(int newPriority, QThreadPool threadPool, Callable2<T, A, B> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable2<T, A, B> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder2Arg2<T, A, B> withArguments(B b) {
            return new QTypedTaskBuilder2Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        @Override
        public QTypedTaskBuilder2Arg1<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder2Arg1<T, A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder2Arg2<T, A, B> extends QTaskBuilder<T>{
        QTypedTaskBuilder2Arg2(int newPriority, QThreadPool threadPool, Callable2<T, A, B> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable2<T, A, B> callable;
        private final A a;
        private final B b;
        
        @Override
        public QTypedTaskBuilder2Arg2<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder2Arg2<T, A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b);
        }
        
        private native static <T, A, B> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable2<T, A, B> callable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C> QTypedTaskBuilder3Arg0<T, A, B, C> task(Callable3<T, A, B, C> callable) {
        return new QTypedTaskBuilder3Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder3Arg0<T, A, B, C> extends QTaskBuilder<T>{
        private final Callable3<T, A, B, C> callable;
        
        QTypedTaskBuilder3Arg0(int priority, QThreadPool threadPool, Callable3<T, A, B, C> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder3Arg1<T, A, B, C> withArguments(A a) {
            return new QTypedTaskBuilder3Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder3Arg2<T, A, B, C> withArguments(A a, B b) {
            return new QTypedTaskBuilder3Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder3Arg3<T, A, B, C> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder3Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        @Override
        public QTypedTaskBuilder3Arg0<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder3Arg0<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder3Arg1<T, A, B, C> extends QTaskBuilder<T>{
        QTypedTaskBuilder3Arg1(int newPriority, QThreadPool threadPool, Callable3<T, A, B, C> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable3<T, A, B, C> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder3Arg2<T, A, B, C> withArguments(B b) {
            return new QTypedTaskBuilder3Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder3Arg3<T, A, B, C> withArguments(B b, C c) {
            return new QTypedTaskBuilder3Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        @Override
        public QTypedTaskBuilder3Arg1<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder3Arg1<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder3Arg2<T, A, B, C> extends QTaskBuilder<T>{
        QTypedTaskBuilder3Arg2(int newPriority, QThreadPool threadPool, Callable3<T, A, B, C> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable3<T, A, B, C> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder3Arg3<T, A, B, C> withArguments(C c) {
            return new QTypedTaskBuilder3Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        @Override
        public QTypedTaskBuilder3Arg2<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder3Arg2<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder3Arg3<T, A, B, C> extends QTaskBuilder<T>{
        QTypedTaskBuilder3Arg3(int newPriority, QThreadPool threadPool, Callable3<T, A, B, C> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable3<T, A, B, C> callable;
        private final A a;
        private final B b;
        private final C c;
        
        @Override
        public QTypedTaskBuilder3Arg3<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder3Arg3<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c);
        }
        
        private native static <T, A, B, C> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable3<T, A, B, C> callable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D> QTypedTaskBuilder4Arg0<T, A, B, C, D> task(Callable4<T, A, B, C, D> callable) {
        return new QTypedTaskBuilder4Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder4Arg0<T, A, B, C, D> extends QTaskBuilder<T>{
        private final Callable4<T, A, B, C, D> callable;
        
        QTypedTaskBuilder4Arg0(int priority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg1<T, A, B, C, D> withArguments(A a) {
            return new QTypedTaskBuilder4Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg2<T, A, B, C, D> withArguments(A a, B b) {
            return new QTypedTaskBuilder4Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        @Override
        public QTypedTaskBuilder4Arg0<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder4Arg0<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder4Arg1<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg1(int newPriority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable4<T, A, B, C, D> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg2<T, A, B, C, D> withArguments(B b) {
            return new QTypedTaskBuilder4Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(B b, C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        @Override
        public QTypedTaskBuilder4Arg1<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder4Arg1<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder4Arg2<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg2(int newPriority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(C c, D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        @Override
        public QTypedTaskBuilder4Arg2<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder4Arg2<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder4Arg3<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg3(int newPriority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        @Override
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder4Arg4<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg4(int newPriority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        @Override
        public QTypedTaskBuilder4Arg4<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder4Arg4<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d);
        }
        
        private native static <T, A, B, C, D> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable4<T, A, B, C, D> callable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E> QTypedTaskBuilder5Arg0<T, A, B, C, D, E> task(Callable5<T, A, B, C, D, E> callable) {
        return new QTypedTaskBuilder5Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder5Arg0<T, A, B, C, D, E> extends QTaskBuilder<T>{
        private final Callable5<T, A, B, C, D, E> callable;
        
        QTypedTaskBuilder5Arg0(int priority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg1<T, A, B, C, D, E> withArguments(A a) {
            return new QTypedTaskBuilder5Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(A a, B b) {
            return new QTypedTaskBuilder5Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        @Override
        public QTypedTaskBuilder5Arg0<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder5Arg0<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder5Arg1<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg1(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(B b) {
            return new QTypedTaskBuilder5Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(B b, C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        @Override
        public QTypedTaskBuilder5Arg1<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder5Arg1<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder5Arg2<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg2(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        @Override
        public QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder5Arg2<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder5Arg3<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg3(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        @Override
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder5Arg4<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg4(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        @Override
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder5Arg5<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg5(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        @Override
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder5Arg5<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e);
        }
        
        private native static <T, A, B, C, D, E> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F> QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> task(Callable6<T, A, B, C, D, E, F> callable) {
        return new QTypedTaskBuilder6Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        private final Callable6<T, A, B, C, D, E, F> callable;
        
        QTypedTaskBuilder6Arg0(int priority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> withArguments(A a) {
            return new QTypedTaskBuilder6Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTypedTaskBuilder6Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg1(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(B b) {
            return new QTypedTaskBuilder6Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg2(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg3(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg4(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg5(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg6(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        @Override
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f);
        }
        
        private native static <T, A, B, C, D, E, F> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F, G> QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> task(Callable7<T, A, B, C, D, E, F, G> callable) {
        return new QTypedTaskBuilder7Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        
        QTypedTaskBuilder7Arg0(int priority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withArguments(A a) {
            return new QTypedTaskBuilder7Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTypedTaskBuilder7Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg1(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(B b) {
            return new QTypedTaskBuilder7Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg2(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg3(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg4(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
    
        @Override
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg5(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg6(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg7(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        @Override
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f, g);
        }
        
        private native static <T, A, B, C, D, E, F, G> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H> QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> task(Callable8<T, A, B, C, D, E, F, G, H> callable) {
        return new QTypedTaskBuilder8Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        
        QTypedTaskBuilder8Arg0(int priority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTypedTaskBuilder8Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTypedTaskBuilder8Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg1(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTypedTaskBuilder8Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg2(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg3(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg4(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg5(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg6(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg7(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg8(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        @Override
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f, g, h);
        }
        
        private native static <T, A, B, C, D, E, F, G, H> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H, I> QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> task(Callable9<T, A, B, C, D, E, F, G, H, I> callable) {
        return new QTypedTaskBuilder9Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        
        QTypedTaskBuilder9Arg0(int priority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTypedTaskBuilder9Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTypedTaskBuilder9Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg1(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTypedTaskBuilder9Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg2(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg3(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg4(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg5(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg6(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg7(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg8(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg9(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
            this.i = i;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        private final I i;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> spawn(QThreadPool threadPool, int priority,
                Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
        
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static QPromiseTaskBuilderVoid0Arg0 task(RunnableWithVoidPromise runnable) {
        return new QPromiseTaskBuilderVoid0Arg0(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid0Arg0 extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid0Arg0(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise runnable) {
            super(newPriority, threadPool);
            this.runnable = runnable;
        }
        private final RunnableWithVoidPromise runnable;
        
        @Override
        public QPromiseTaskBuilderVoid0Arg0 withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid0Arg0 onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable);
        }
        
        private native static QFuture<Void> spawn(QThreadPool threadPool, int priority, RunnableWithVoidPromise runnable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A> QPromiseTaskBuilderVoid1Arg0<A> task(RunnableWithVoidPromise1<A> runnable) {
        return new QPromiseTaskBuilderVoid1Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid1Arg0<A> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise1<A> runnable;
        
        QPromiseTaskBuilderVoid1Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise1<A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid1Arg1<A> withArguments(A a) {
            return new QPromiseTaskBuilderVoid1Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        @Override
        public QPromiseTaskBuilderVoid1Arg0<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid1Arg0<A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid1Arg1<A> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid1Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise1<A> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise1<A> runnable;
        private final A a;
        
        @Override
        public QPromiseTaskBuilderVoid1Arg1<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid1Arg1<A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a);
        }
        
        private native static <A> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise1<A> runnable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B> QPromiseTaskBuilderVoid2Arg0<A, B> task(RunnableWithVoidPromise2<A, B> runnable) {
        return new QPromiseTaskBuilderVoid2Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid2Arg0<A, B> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise2<A, B> runnable;
        
        QPromiseTaskBuilderVoid2Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid2Arg1<A, B> withArguments(A a) {
            return new QPromiseTaskBuilderVoid2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid2Arg2<A, B> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        @Override
        public QPromiseTaskBuilderVoid2Arg0<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid2Arg0<A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid2Arg1<A, B> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid2Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise2<A, B> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid2Arg2<A, B> withArguments(B b) {
            return new QPromiseTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        @Override
        public QPromiseTaskBuilderVoid2Arg1<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid2Arg1<A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid2Arg2<A, B> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid2Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise2<A, B> runnable;
        private final A a;
        private final B b;
        
        @Override
        public QPromiseTaskBuilderVoid2Arg2<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid2Arg2<A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b);
        }
        
        private native static <A, B> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise2<A, B> runnable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C> QPromiseTaskBuilderVoid3Arg0<A, B, C> task(RunnableWithVoidPromise3<A, B, C> runnable) {
        return new QPromiseTaskBuilderVoid3Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid3Arg0<A, B, C> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        
        QPromiseTaskBuilderVoid3Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> withArguments(A a) {
            return new QPromiseTaskBuilderVoid3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QPromiseTaskBuilderVoid3Arg0<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid3Arg0<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid3Arg1<A, B, C> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid3Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withArguments(B b) {
            return new QPromiseTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid3Arg2<A, B, C> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid3Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withArguments(C c) {
            return new QPromiseTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid3Arg3<A, B, C> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid3Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        @Override
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c);
        }
        
        private native static <A, B, C> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D> QPromiseTaskBuilderVoid4Arg0<A, B, C, D> task(RunnableWithVoidPromise4<A, B, C, D> runnable) {
        return new QPromiseTaskBuilderVoid4Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid4Arg0<A, B, C, D> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        
        QPromiseTaskBuilderVoid4Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> withArguments(A a) {
            return new QPromiseTaskBuilderVoid4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QPromiseTaskBuilderVoid4Arg0<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid4Arg0<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid4Arg1<A, B, C, D> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid4Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withArguments(B b) {
            return new QPromiseTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid4Arg2<A, B, C, D> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid4Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid4Arg3<A, B, C, D> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid4Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid4Arg4<A, B, C, D> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid4Arg4(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        @Override
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d);
        }
        
        private native static <A, B, C, D> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E> QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> task(RunnableWithVoidPromise5<A, B, C, D, E> runnable) {
        return new QPromiseTaskBuilderVoid5Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        
        QPromiseTaskBuilderVoid5Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> withArguments(A a) {
            return new QPromiseTaskBuilderVoid5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid5Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(B b) {
            return new QPromiseTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid5Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid5Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid5Arg4(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid5Arg5(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        @Override
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <A, B, C, D, E> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F> QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> task(RunnableWithVoidPromise6<A, B, C, D, E, F> runnable) {
        return new QPromiseTaskBuilderVoid6Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        
        QPromiseTaskBuilderVoid6Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> withArguments(A a) {
            return new QPromiseTaskBuilderVoid6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid6Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(B b) {
            return new QPromiseTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid6Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid6Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid6Arg4(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid6Arg5(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid6Arg6(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        @Override
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <A, B, C, D, E, F> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F, G> QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> task(RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable) {
        return new QPromiseTaskBuilderVoid7Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        
        QPromiseTaskBuilderVoid7Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withArguments(A a) {
            return new QPromiseTaskBuilderVoid7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(B b) {
            return new QPromiseTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg4(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
    
        @Override
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg5(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg6(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid7Arg7(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        @Override
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <A, B, C, D, E, F, G> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F, G, H> QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> task(RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable) {
        return new QPromiseTaskBuilderVoid8Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        
        QPromiseTaskBuilderVoid8Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QPromiseTaskBuilderVoid8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QPromiseTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg4(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg5(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg6(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg7(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid8Arg8(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        @Override
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <A, B, C, D, E, F, G, H> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <A, B, C, D, E, F, G, H, I> QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> task(RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable) {
        return new QPromiseTaskBuilderVoid9Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        
        QPromiseTaskBuilderVoid9Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QPromiseTaskBuilderVoid9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QPromiseTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg3(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg4(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg5(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg6(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg7(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg8(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<Void>{
        QPromiseTaskBuilderVoid9Arg9(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
            this.i = i;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        private final I i;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <A, B, C, D, E, F, G, H, I> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T> QTypedPromiseTaskBuilder0Arg0<T> task(RunnableWithPromise<T> callable) {
        return new QTypedPromiseTaskBuilder0Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder0Arg0<T> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder0Arg0(int newPriority, QThreadPool threadPool, RunnableWithPromise<T> runnable) {
            super(newPriority, threadPool);
            this.runnable = runnable;
        }
        private final RunnableWithPromise<T> runnable;
        
        @Override
        public QTypedPromiseTaskBuilder0Arg0<T> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder0Arg0<T> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable);
        }
        
        private native static <T> QFuture<T> spawn(QThreadPool threadPool, int priority, RunnableWithPromise<T> runnable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A> QTypedPromiseTaskBuilder1Arg0<T, A> task(RunnableWithPromise1<T, A> runnable) {
        return new QTypedPromiseTaskBuilder1Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder1Arg0<T, A> extends QTaskBuilder<T>{
        private final RunnableWithPromise1<T, A> runnable;
        
        QTypedPromiseTaskBuilder1Arg0(int priority, QThreadPool threadPool, RunnableWithPromise1<T, A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder1Arg1<T, A> withArguments(A a) {
            return new QTypedPromiseTaskBuilder1Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        @Override
        public QTypedPromiseTaskBuilder1Arg0<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder1Arg0<T, A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder1Arg1<T, A> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder1Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise1<T, A> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise1<T, A> runnable;
        private final A a;
        
        @Override
        public QTypedPromiseTaskBuilder1Arg1<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder1Arg1<T, A> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a);
        }
        
        private native static <T, A> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise1<T, A> runnable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B> QTypedPromiseTaskBuilder2Arg0<T, A, B> task(RunnableWithPromise2<T, A, B> runnable) {
        return new QTypedPromiseTaskBuilder2Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder2Arg0<T, A, B> extends QTaskBuilder<T>{
        private final RunnableWithPromise2<T, A, B> runnable;
        
        QTypedPromiseTaskBuilder2Arg0(int priority, QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> withArguments(A a) {
            return new QTypedPromiseTaskBuilder2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        @Override
        public QTypedPromiseTaskBuilder2Arg0<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder2Arg0<T, A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder2Arg1<T, A, B> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder2Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise2<T, A, B> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> withArguments(B b) {
            return new QTypedPromiseTaskBuilder2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        @Override
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder2Arg2<T, A, B> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder2Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise2<T, A, B> runnable;
        private final A a;
        private final B b;
        
        @Override
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b);
        }
        
        private native static <T, A, B> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise2<T, A, B> runnable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C> QTypedPromiseTaskBuilder3Arg0<T, A, B, C> task(RunnableWithPromise3<T, A, B, C> runnable) {
        return new QTypedPromiseTaskBuilder3Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder3Arg0<T, A, B, C> extends QTaskBuilder<T>{
        private final RunnableWithPromise3<T, A, B, C> runnable;
        
        QTypedPromiseTaskBuilder3Arg0(int priority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> withArguments(A a) {
            return new QTypedPromiseTaskBuilder3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QTypedPromiseTaskBuilder3Arg0<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder3Arg0<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder3Arg1<T, A, B, C> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withArguments(B b) {
            return new QTypedPromiseTaskBuilder3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder3Arg2<T, A, B, C> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withArguments(C c) {
            return new QTypedPromiseTaskBuilder3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        @Override
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder3Arg3<T, A, B, C> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        @Override
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c);
        }
        
        private native static <T, A, B, C> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D> QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> task(RunnableWithPromise4<T, A, B, C, D> runnable) {
        return new QTypedPromiseTaskBuilder4Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> extends QTaskBuilder<T>{
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        
        QTypedPromiseTaskBuilder4Arg0(int priority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> withArguments(A a) {
            return new QTypedPromiseTaskBuilder4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withArguments(B b) {
            return new QTypedPromiseTaskBuilder4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        @Override
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg4(int newPriority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        @Override
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d);
        }
        
        private native static <T, A, B, C, D> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E> QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> task(RunnableWithPromise5<T, A, B, C, D, E> runnable) {
        return new QTypedPromiseTaskBuilder5Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> extends QTaskBuilder<T>{
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        
        QTypedPromiseTaskBuilder5Arg0(int priority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> withArguments(A a) {
            return new QTypedPromiseTaskBuilder5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(B b) {
            return new QTypedPromiseTaskBuilder5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg4(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        @Override
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg5(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        @Override
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <T, A, B, C, D, E> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F> QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> task(RunnableWithPromise6<T, A, B, C, D, E, F> runnable) {
        return new QTypedPromiseTaskBuilder6Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        
        QTypedPromiseTaskBuilder6Arg0(int priority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> withArguments(A a) {
            return new QTypedPromiseTaskBuilder6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(B b) {
            return new QTypedPromiseTaskBuilder6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg4(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg5(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        @Override
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg6(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        @Override
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <T, A, B, C, D, E, F> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F, G> QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> task(RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable) {
        return new QTypedPromiseTaskBuilder7Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        
        QTypedPromiseTaskBuilder7Arg0(int priority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withArguments(A a) {
            return new QTypedPromiseTaskBuilder7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(B b) {
            return new QTypedPromiseTaskBuilder7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg4(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
    
        @Override
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg5(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg6(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        @Override
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg7(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        @Override
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <T, A, B, C, D, E, F, G> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H> QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> task(RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable) {
        return new QTypedPromiseTaskBuilder8Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        
        QTypedPromiseTaskBuilder8Arg0(int priority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTypedPromiseTaskBuilder8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTypedPromiseTaskBuilder8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg4(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg5(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        @Override
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg6(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg7(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        @Override
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg8(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        @Override
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <T, A, B, C, D, E, F, G, H> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task">QtConcurrent::task(Task&&)</a></p>
     */
    public static <T, A, B, C, D, E, F, G, H, I> QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> task(RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable) {
        return new QTypedPromiseTaskBuilder9Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        
        QTypedPromiseTaskBuilder9Arg0(int priority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTypedPromiseTaskBuilder9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static class QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTypedPromiseTaskBuilder9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg3(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg4(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg5(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg6(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg7(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg8(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &&...)</a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        @Override
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder</a></p>
     */
    public static final class QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg9(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
            this.g = g;
            this.h = h;
            this.i = i;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        private final I i;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder::spawn()</a></p>
         */
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        @Override
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> onThreadPool(QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> spawn(QThreadPool threadPool, int priority,
                RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
}// class


class QtConcurrent_DISMISS__ extends QtConcurrent {
    
    static class ThreadEngine<T> extends ThreadEngineBase
    {
        ThreadEngine(QThreadPool pool) {
            super(pool);
            Objects.requireNonNull(pool);
        }

        @io.qt.QtUninvokable
        T result() { return null; }

        @SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
        final QFutureInterface<T> futureInterfaceTyped()
        {
            return (QFutureInterface<T>)futureInterface();
        }

        // Runs the user algorithm using a single thread.
        @io.qt.QtUninvokable
        final T startSingleThreaded()
        {
            super._startSingleThreaded();
            return result();
        }

        // Runs the user algorithm using multiple threads.
        // Does not block, returns a future.
        @io.qt.QtUninvokable
        final QFuture<T> startAsynchronously()
        {
            QFutureInterface<T> futureInterface = new QFutureInterface<>();
            setFutureInterface(futureInterface);

            // reportStart() must be called before starting threads, otherwise the
            // user algorithm might finish while reportStart() is running, which
            // is very bad.
            futureInterface.reportStarted();
            QFuture<T> future = new QFuture<T>(futureInterface);
            start();

            acquireBarrierSemaphore();
            threadPool().start(this);
            return future;
        }

        @io.qt.QtPrivateOverride
        @io.qt.QtUninvokable
        final void asynchronousFinish()
        {
            finish();
            QFutureInterface<T> futureInterface = futureInterfaceTyped();
            futureInterface.reportFinished(result());
            setFutureInterface(null);
            futureInterface.dispose();
            dispose();
        }

        @io.qt.QtUninvokable
        final void reportResult(T _result) {
            reportResult(_result, -1);
        }
        
        @io.qt.QtUninvokable
        final void reportResult(T _result, int index)
        {
            QFutureInterface<T> futureInterface = futureInterfaceTyped();
            if (futureInterface!=null)
                futureInterface.reportResult(_result, index);
        }
        
        @io.qt.QtUninvokable
        final void reportResults(Collection<T> _result) {
            reportResults(_result, -1, -1);
        }
        
        @io.qt.QtUninvokable
        final void reportResults(Collection<T> _result, int index) {
            reportResults(_result, index, -1);
        }

        @io.qt.QtUninvokable
        final void reportResults(Collection<T> _result, int index, int count)
        {
            QFutureInterface<T> futureInterface = futureInterfaceTyped();
            if (futureInterface!=null)
                futureInterface.reportResults(_result, index, count);
        }
    }
    
    public static final class ThreadEngineStarter<T> {
        ThreadEngineStarter(ThreadEngine<T> _threadEngine){
            threadEngine = Objects.requireNonNull(_threadEngine);
        }

        public final QFuture<T> startAsynchronously(){
            return threadEngine.startAsynchronously();
        }
        
        private ThreadEngine<T> threadEngine;
    }
    
    static class IterateKernel<M,T> extends ThreadEngine<T>{
        IterateKernel(QThreadPool pool, Collection<M> collection)
        {
            super(pool);
            this.collection = collection;
            this.iterationCount = collection instanceof List ? this.collection.size() : 0;
            this.forIteration = collection instanceof List;
            this.progressReportingEnabled = true;
        }

        boolean runIteration(M value, int index, Object[] values) { return false; }
        boolean runIterations(List<M> collection, int index, int count, Object[] values) { return false; }

        @Override
        public void start()
        {
            progressReportingEnabled = isProgressReportingEnabled();
            if (progressReportingEnabled && iterationCount > 0)
                setProgressRange(0, iterationCount);
        }

        @Override
        public boolean shouldStartThread()
        {
            if (forIteration)
                return (currentIndex.get() < iterationCount) && !this.shouldThrottleThread();
            else // whileIteration
                return (iteratorThreads.get() == 0);
        }

        @Override
        public final ThreadFunctionResult threadFunction()
        {
            if (forIteration)
                return forThreadFunction();
            else // whileIteration
                return whileThreadFunction();
        }

        final ThreadFunctionResult forThreadFunction()
        {
            BlockSizeManager blockSizeManager = new BlockSizeManager(threadPool(), iterationCount);
            ResultReporter<T> resultReporter = createResultsReporter();

            for(;;) {
                if (this.isCanceled())
                    break;

                final int currentBlockSize = blockSizeManager.blockSize();

                if (currentIndex.get() >= iterationCount)
                    break;

                // Atomically reserve a block of iterationCount for this thread.
                final int beginIndex = currentIndex.getAndAdd(currentBlockSize);
                final int endIndex = Math.min(beginIndex + currentBlockSize, iterationCount);

                if (beginIndex >= endIndex) {
                    // No more work
                    break;
                }

                this.waitForResume(); // (only waits if the qfuture is paused.)

                if (shouldStartThread())
                    this.startThread();

                final int finalBlockSize = endIndex - beginIndex; // block size adjusted for possible end-of-range
                resultReporter.reserveSpace(finalBlockSize);

                // Call user code with the current iteration range.
                blockSizeManager.timeBeforeUser();
                final boolean resultsAvailable = this.runIterations((List<M>)collection, beginIndex, endIndex, resultReporter.getPointer());
                blockSizeManager.timeAfterUser();

                if (resultsAvailable)
                    resultReporter.reportResults(beginIndex);

                // Report progress if progress reporting enabled.
                if (progressReportingEnabled) {
                    completed.getAndAdd(finalBlockSize);
                    this.setProgressValue(this.completed.get());
                }

                if (this.shouldThrottleThread())
                    return ThreadFunctionResult.ThrottleThread;
            }
            return ThreadFunctionResult.ThreadFinished;
        }

        final ThreadFunctionResult whileThreadFunction()
        {
            if (iteratorThreads.compareAndSet(0, 1) == false)
                return ThreadFunctionResult.ThreadFinished;

            ResultReporter<T> resultReporter = createResultsReporter();
            resultReporter.reserveSpace(1);

            for(M value : collection) {
                // The following two lines breaks support for input iterators according to
                // the sgi docs: dereferencing prev after calling ++current is not allowed
                // on input iterators. (prev is dereferenced inside user.runIteration())
                int index = currentIndex.getAndAdd(1);
                if(index>collection.size())
                    break;
                iteratorThreads.compareAndSet(1, 0);

                this.waitForResume(); // (only waits if the qfuture is paused.)

                if (shouldStartThread())
                    this.startThread();

                final boolean resultAavailable = this.runIteration(value, index, resultReporter.getPointer());
                if (resultAavailable)
                    resultReporter.reportResults(index);

                if (this.shouldThrottleThread())
                    return ThreadFunctionResult.ThrottleThread;

                if (iteratorThreads.compareAndSet(0, 1) == false)
                    return ThreadFunctionResult.ThreadFinished;
            }

            return ThreadFunctionResult.ThreadFinished;
        }

        private ResultReporter<T> createResultsReporter()
        {
            return new ResultReporter<>(this, defaultValue);
        }

        public Collection<M> collection;
        public AtomicInteger currentIndex;
        public AtomicInteger iteratorThreads;
        public AtomicInteger completed;
        public final int iterationCount;
        public final boolean forIteration;
        public boolean progressReportingEnabled;
        public T defaultValue;
    }
    
    static final class ResultReporter<T>
    {
        ResultReporter(ThreadEngine<T> _threadEngine, T _defaultValue)
        {
            this.threadEngine = _threadEngine;
            this.defaultValue = _defaultValue;
        }

        void reserveSpace(int resultCount)
        {
            currentResultCount = resultCount;
            resizeList(Math.max(resultCount, vector.length));
        }

        @SuppressWarnings({ "unchecked", "rawtypes" })
        void reportResults(int begin)
        {
            final int useVectorThreshold = 4; // Tunable parameter.
            if (currentResultCount > useVectorThreshold) {
                resizeList(currentResultCount);
                threadEngine.reportResults((Collection<T>)(Collection)Arrays.asList(vector), begin);
            } else {
                for (int i = 0; i < currentResultCount; ++i)
                    threadEngine.reportResult((T)vector[i], begin + i);
            }
        }

        Object[] getPointer()
        {
            return vector;
        }

        public int currentResultCount;
        public ThreadEngine<T> threadEngine;
        public Object[] vector = new Object[0];

        private void resizeList(int size)
        {
            int oldSize = vector.length;
            vector = Arrays.copyOf(vector, size);
            for(int i=oldSize; i<size; ++i) {
                vector[i] = defaultValue;
            }
        }

        private T defaultValue;
    }
    
    static final class MapKernel<T> extends IterateKernel<T, Void>
    {
        MapKernel(QThreadPool pool, Collection<T> collection, MapFunctor<T> map) {
            super(pool, collection);
            this.map = map;
        }

        private final MapFunctor<T> map;

        @Override
        boolean runIteration(T value, int index, Object[] values) {
            map.map(value);
            return false;
        }

        @Override
        boolean runIterations(List<T> collection, int index, int count, Object[] values) {
            for (; index < count; index++) {
                runIteration(collection.get(index), index, values);
            }
            return false;
        }
    }
    
    static final class MapEachKernel<U,T> extends IterateKernel<T,U>
    {
        MapEachKernel(QThreadPool pool, Collection<T> collection, MappedFunctor<U,T> map) {
            super(pool, collection);
            this.map = map;
        }

        private final MappedFunctor<U,T> map;

        @Override
        boolean runIteration(T value, int index, Object[] values) {
            values[index] = map.map(value);
            return true;
        }

        @Override
        boolean runIterations(List<T> collection, int index, int count, Object[] values) {
            for (; index < count; index++) {
                runIteration(collection.get(index), index, values);
            }
            return true;
        }
    }
    
    static final class IntermediateResults<T>
    {
        IntermediateResults(int begin, int end) {
            this.begin = begin;
            this.end = end;
            this.vector = new ArrayList<>();
        }
        IntermediateResults(int begin, int end, List<T> vector) {
            this.begin = begin;
            this.end = end;
            this.vector = vector;
        }
        public final int begin, end;
        public final List<T> vector;
    }
    
    private final static int ReduceQueueStartLimit = 20;
    private final static int ReduceQueueThrottleLimit = 30;
    
    static final class ReduceKernel<U,T>
    {
        ReduceKernel(QThreadPool pool, ReduceOptions reduceOptions) {
            this.reduceOptions = reduceOptions;
            this.threadCount = pool.maxThreadCount();
        }

        private final ReduceOptions reduceOptions;
        
        private final QMutex mutex = new QMutex();
        private int progress, resultsMapSize;
        private final int threadCount;
        private final Map<Integer,IntermediateResults<T>> resultsMap = new TreeMap<>();
        
        private final boolean canReduce(int begin) {
            return ((reduceOptions.testFlag(ReduceOption.UnorderedReduce)
                     && progress == 0)
                    || (reduceOptions.testFlag(ReduceOption.OrderedReduce)
                        && progress == begin));
        }
        
        private U reduceResult(ReduceFunctor<U,T> reduce,
                U r,
                IntermediateResults<T> result) {
          for (int i = 0; i < result.vector.size(); ++i) {
              r = reduce.reduce(r, result.vector.get(i));
          }
          return r;
        }
        
        private U reduceResults(ReduceFunctor<U,T> reduce,
                U r,
                Map<Integer,IntermediateResults<T>> resultsMap){
            for(IntermediateResults<T> results : resultsMap.values()) {
                r = reduceResult(reduce, r, results);
            }
            return r;
        }
        
        U runReduce(ReduceFunctor<U,T> reduce,
                U r,
                IntermediateResults<T> result) {
            mutex.lock();
            try {
                if (!canReduce(result.begin)) {
                    ++resultsMapSize;
                    resultsMap.put(result.begin, result);
                    return r;
                }
                if (reduceOptions.testFlag(ReduceOption.UnorderedReduce)) {
                    progress = -1;
                    mutex.unlock();
                    try {
                        reduceResult(reduce, r, result);
                    }finally {
                        mutex.lock();
                    }
                    while (!resultsMap.isEmpty()) {
                        Map<Integer,IntermediateResults<T>> resultsMapCopy = new TreeMap<>(resultsMap);
                        resultsMap.clear();

                        mutex.unlock();
                        try {
                            reduceResults(reduce, r, resultsMapCopy);
                        }finally {
                            mutex.lock();
                        }

                        resultsMapSize -= resultsMapCopy.size();
                    }
                    progress = 0;
                }else {
                    // reduce this result
                    mutex.unlock();
                    try {
                        reduceResult(reduce, r, result);
                    }finally {
                        mutex.lock();
                    }
                    
                    // OrderedReduce
                    progress += result.end - result.begin;
                    
                    // reduce as many other results as possible
                    for(Map.Entry<Integer,IntermediateResults<T>> entry : new HashSet<>(resultsMap.entrySet())) {
                        if(entry.getValue().begin != progress)
                            break;
                        mutex.unlock();
                        try {
                            r = reduceResult(reduce, r, entry.getValue());
                        }finally {
                            mutex.lock();
                        }

                        --resultsMapSize;
                        progress += entry.getValue().end - entry.getValue().begin;
                        resultsMap.remove(entry.getKey());
                    }
                }
            }finally {
                mutex.unlock();
            }
            return r;
        }
        
        // final reduction
        U finish(ReduceFunctor<U,T> reduce, U r)
        {
            return reduceResults(reduce, r, resultsMap);
        }
        
        boolean shouldThrottle()
        {
            mutex.lock();
            try {
                return (resultsMapSize > (ReduceQueueThrottleLimit * threadCount));
            }finally {
                mutex.unlock();
            }
        }

        boolean shouldStartThread()
        {
            mutex.lock();
            try {
                return (resultsMapSize <= (ReduceQueueStartLimit * threadCount));
            }finally {
                mutex.unlock();
            }
        }
    }
    
    static final class MappedReducedKernel<U,V,T> extends IterateKernel<T,U>
    {
        MappedReducedKernel(QThreadPool pool, Collection<T> collection, MappedFunctor<V,T> map, ReduceFunctor<U,V> reduce, ReduceOptions reduceOptions) {
            super(pool, collection);
            this.map = map;
            this.reduce = reduce;
            super.defaultValue = reduce.defaultResult();
            this.reducedResult = super.defaultValue;
            this.reducer = new ReduceKernel<>(pool, reduceOptions);
        }

        private U reducedResult;
        private final MappedFunctor<V,T> map;
        private final ReduceFunctor<U,V> reduce;
        private final ReduceKernel<U,V> reducer;

        @Override
        boolean runIteration(T value, int index, Object[] values) {
            IntermediateResults<V> results = new IntermediateResults<>(index, index + 1);
            results.vector.add(map.map(value));
            reducedResult = reducer.runReduce(reduce, reducedResult, results);
            return false;
        }

        @Override
        boolean runIterations(List<T> collection, int index, int count, Object[] values) {
            IntermediateResults<V> results = new IntermediateResults<>(index, index + count);
            for (; index < count; index++) {
                results.vector.add(map.map(collection.get(index)));
            }
            reducedResult = reducer.runReduce(reduce, reducedResult, results);
            return false;
        }
        
        @Override
        protected void finish()
        {
            reducedResult = reducer.finish(reduce, reducedResult);
        }
        
        @Override
        protected boolean shouldThrottleThread()
        {
            return super.shouldThrottleThread() || reducer.shouldThrottle();
        }
        
        @Override
        public boolean shouldStartThread()
        {
            return super.shouldStartThread() && reducer.shouldStartThread();
        }
        
        @Override
        protected U result()
        {
            return reducedResult;
        }
    }
    
    static final class FilteredReducedKernel<T,U> extends IterateKernel<T,U>{
        private U reducedResult;
        private final FilteredFunctor<T> keep;
        private final ReduceFunctor<U,T> reduce;
        private final ReduceKernel<U,T> reducer;

        FilteredReducedKernel(QThreadPool pool, Collection<T> sequence, FilteredFunctor<T> keep, ReduceFunctor<U, T> reduce, ReduceOptions options) {
            super(pool, sequence);
            this.keep = keep;
            this.reduce = reduce;
            super.defaultValue = reduce.defaultResult();
            this.reducedResult = super.defaultValue;
            this.reducer = new ReduceKernel<>(pool, options);
        }

        @Override
        boolean runIteration(T value, int index, Object[] values) {
            IntermediateResults<T> results = new IntermediateResults<>(index, index+1);
            if(keep.filter(value))
                results.vector.add(value);
            reducedResult = reducer.runReduce(reduce, reducedResult, results);
            return false;
        }

        @Override
        boolean runIterations(List<T> collection, int index, int count, Object[] values) {
            IntermediateResults<T> results = new IntermediateResults<>(index, count);
            for (; index < count; ++index) {
                T value = collection.get(index);
                if(keep.filter(value))
                    results.vector.add(value);                
            }
            reducedResult = reducer.runReduce(reduce, reducedResult, results);
            return false;
        }

        @Override
        public boolean shouldStartThread() {
            return super.shouldStartThread() && reducer.shouldStartThread();
        }

        @Override
        U result() {
            return reducedResult;
        }

        @Override
        protected void finish() {
            reducedResult = reducer.finish(reduce, reducedResult);
        }

        @Override
        protected boolean shouldThrottleThread() {
            return super.shouldThrottleThread() || reducer.shouldThrottle();
        }
    }
    
    static final class FilteredEachKernel<T> extends IterateKernel<T,T>{
        private final FilteredFunctor<T> keep;

        FilteredEachKernel(QThreadPool pool, Collection<T> sequence, FilteredFunctor<T> keep) {
            super(pool, sequence);
            this.keep = keep;
        }
        
        @Override
        public void start()
        {
            QFutureInterfaceBase futureInterface = this.futureInterface();
            if (futureInterface!=null)
                futureInterface.setFilterMode(true);
            super.start();
        }

        @Override
        boolean runIteration(T value, int index, Object[] values) {
            if (keep.filter(value))
                reportResult(value, index);
            else
                reportResult(null, index);
            return false;
        }

        @Override
        boolean runIterations(List<T> collection, int index, int count, Object[] values) {
            IntermediateResults<T> results = new IntermediateResults<>(index, count);

            for (; index < count; ++index) {
                T value = collection.get(index);
                if (keep.filter(value))
                    results.vector.add(value);
            }

            reportResults(results.vector, index, count);
            return false;
        }
    }
    
    static final class FilterKernel<T> extends IterateKernel<T,Void>{
        private final Collection<T> sequence;
        private Collection<T> reducedResult = new ArrayList<>();
        private final FilteredFunctor<T> keep;
        private final ReduceFunctor<Collection<T>,T> reduce;
        private final ReduceKernel<Collection<T>,T> reducer;

        FilterKernel(QThreadPool pool, Collection<T> sequence, FilteredFunctor<T> keep, ReduceFunctor<Collection<T>, T> reduce, ReduceOptions options) {
            super(pool, sequence);
            this.sequence = sequence;
            this.keep = keep;
            this.reduce = reduce;
            this.reducer = new ReduceKernel<>(pool, options);
        }

        @Override
        boolean runIteration(T value, int index, Object[] values) {
            IntermediateResults<T> results = new IntermediateResults<>(index, index + 1);
            if (keep.filter(value))
                results.vector.add(value);
            reducedResult = reducer.runReduce(reduce, reducedResult, results);
            return false;
        }

        @Override
        boolean runIterations(List<T> collection, int index, int count, Object[] values) {
            IntermediateResults<T> results = new IntermediateResults<>(index, count);
            for (; index < count; ++index) {
                T value = collection.get(index);
                if (keep.filter(value))
                    results.vector.add(value);
            }
            reducedResult = reducer.runReduce(reduce, reducedResult, results);
            return false;
        }

        @Override
        public boolean shouldStartThread() {
            return super.shouldStartThread() && reducer.shouldStartThread();
        }

        @Override
        protected boolean shouldThrottleThread() {
            return super.shouldThrottleThread() || reducer.shouldThrottle();
        }

        @Override
        protected void finish() {
            reducedResult = reducer.finish(reduce, reducedResult);
            sequence.clear();
            sequence.addAll(reducedResult);
        }
    }
    
    /**
     *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
     */
    public static <T> ThreadEngineStarter<Void> startMap(QThreadPool threadPool, Collection<T> sequence, MapFunctor<T> functor) {
        return new ThreadEngineStarter<>(new MapKernel<>(Objects.requireNonNull(threadPool), Objects.requireNonNull(sequence), Objects.requireNonNull(functor)));
    }
    
    /**
     * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
     *
     */
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startMapped(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<U, T> functor){
        return new ThreadEngineStarter<>(new MapEachKernel<>(Objects.requireNonNull(threadPool), Objects.requireNonNull(sequence), Objects.requireNonNull(functor)));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOptions options) {
        return new ThreadEngineStarter<>(new MappedReducedKernel<>(Objects.requireNonNull(threadPool), Objects.requireNonNull(sequence), Objects.requireNonNull(functor), Objects.requireNonNull(reduceFunctor), options));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> ThreadEngineStarter<U> startMappedReduced(QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, ReduceFunctor<U, V> reduceFunctor, ReduceOption... options) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOption ... options) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> ThreadEngineStarter<U> startFilteredReduced(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<U, T> reduceFunctor, ReduceOptions options) {
        return new ThreadEngineStarter<>(new FilteredReducedKernel<>(Objects.requireNonNull(threadPool), Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), options));
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> ThreadEngineStarter<T> startFiltered(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return new ThreadEngineStarter<>(new FilteredEachKernel<>(Objects.requireNonNull(threadPool), Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor)));
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    static <T> ThreadEngineStarter<Void> filterInternal(QThreadPool threadPool, Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReduceFunctor<Collection<T>, T> reduceFunctor){
        return new ThreadEngineStarter<>(new FilterKernel<>(Objects.requireNonNull(threadPool), Objects.requireNonNull(sequence), Objects.requireNonNull(filteredFunctor), Objects.requireNonNull(reduceFunctor), new ReduceOptions()));
    }
    
    private static class PushBackWrapper<T> implements ReduceFunctor<Collection<T>, T>{
        @Override
        public Collection<T> reduce(Collection<T> result, T intermediate) {
            result.add(intermediate);
            return result;
        }
    }
}// class


