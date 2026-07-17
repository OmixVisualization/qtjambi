/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

class QtConcurrent___ extends QtConcurrent {
    /**
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent.html"><code>QtConcurrent::ThreadEngineStarter</code></a></p>
     */
    public static final class ThreadEngineStarter<T> extends QtObject{
        private ThreadEngineStarter(QPrivateConstructor p){
            super(p);
        }

        @QtUninvokable
        public final io.qt.core.@NonNull QFuture<T> startAsynchronously(){
            io.qt.core.QFuture<T> result = startAsynchronously(QtJambi_LibraryUtilities.internal.nativeId(this));
            dispose();
            return result;
        }
        
        @QtUninvokable
        private static native final <T> io.qt.core.QFuture<T> startAsynchronously(long nativeId);
    }
    
    /**
     * An implemetation of this interface is given one to QtConcurrent's map() methods.
     * The map() method of this interface is called for each object in a java.util.Collection.
     *
     */
    @FunctionalInterface
    public interface MapFunctor<T> extends io.qt.core.QtFuture.Runnable1<T>{
    }
    
    /**
     * Implement this interface to perform a mapped operation. An implementation of the interface is sendt
     * to ome of the mapped methods of QtConcurrent, which applies the MappedFunctor.map() method to all elements in a collection,
     * and returns the result.
     */
    @FunctionalInterface
    public interface MappedFunctor<U, T> extends io.qt.core.QtFuture.@StrictNonNull Callable1<U,T>{
    }
    
    /**
     * An implementation of this interface is given to one of QtConcurrent's filtered() methods.
     * The filter method if this interface is called for each item in a java.util.Collection.
     *
     */
     @FunctionalInterface
    public interface KeepFunctor<T> extends io.qt.core.QtFuture.Predicate1<T>{
    }
    
    /**
     * Implement this interface in order to perform a reduce operation.
     * <p>
     * The reduce method will be called once per intermediate result (the result of the mapping of the data)
     * and the very first time the reduce() method is called for the particular data set.
     */
    @FunctionalInterface
    public interface ReduceFunctor<U, T> extends io.qt.core.QtFuture.Callable2<U, U, T>, java.io.Serializable{
    }

    @SuppressWarnings("unchecked")
    @QtUninvokable
    private static <U> U computeDefaultResult(ReduceFunctor<U,?> reduce) {
        java.util.Objects.requireNonNull(reduce, "Argument 'reduce': null not expected.");
        int[] types = QtJambi_LibraryUtilities.internal.lambdaMetaTypes(ReduceFunctor.class, reduce);
        if(types!=null && types.length>0) {
            return (U)new io.qt.core.QMetaType(types[0]).create();
        }
        return null;
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,V,T> U blockingMappedReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
        return blockingMappedReduced(pool, sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T> U blockingMappedReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return blockingMappedReduced(pool, sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T> U blockingMappedReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return blockingMappedReduced(pool, sequence, map, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,V,T> U blockingMappedReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
        return blockingMappedReduced(sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T> U blockingMappedReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return blockingMappedReduced(sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T> U blockingMappedReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return blockingMappedReduced(sequence, map, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingMappedReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
        return blockingMappedReduced(pool, begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingMappedReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return blockingMappedReduced(pool, begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingMappedReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return blockingMappedReduced(pool, begin, end, map, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingMappedReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
        return blockingMappedReduced(begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingMappedReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return blockingMappedReduced(begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #blockingMappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingMappedReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return blockingMappedReduced(begin, end, map, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,T> io.qt.core.@NonNull QFuture<U> filteredReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
        return filteredReduced(pool, sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> io.qt.core.@NonNull QFuture<U> filteredReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return filteredReduced(pool, sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> io.qt.core.@NonNull QFuture<U> filteredReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return filteredReduced(pool, sequence, keep, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,T> io.qt.core.@NonNull QFuture<U> filteredReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
        return filteredReduced(sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> io.qt.core.@NonNull QFuture<U> filteredReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return filteredReduced(sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> io.qt.core.@NonNull QFuture<U> filteredReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return filteredReduced(sequence, keep, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> filteredReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
        return filteredReduced(pool, begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> filteredReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return filteredReduced(pool, begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> filteredReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return filteredReduced(pool, begin, end, keep, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> filteredReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
        return filteredReduced(begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> filteredReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return filteredReduced(begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #filteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> filteredReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return filteredReduced(begin, end, keep, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #blockingFilteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,T> U blockingFilteredReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
        return blockingFilteredReduced(pool, sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #blockingFilteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> U blockingFilteredReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return blockingFilteredReduced(pool, sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #blockingFilteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> U blockingFilteredReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return blockingFilteredReduced(pool, sequence, keep, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #blockingFilteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
     */
    public static <U,T> U blockingFilteredReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
        return blockingFilteredReduced(sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #blockingFilteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> U blockingFilteredReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return blockingFilteredReduced(sequence, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #blockingFilteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
     */
    public static <U,T> U blockingFilteredReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
        return blockingFilteredReduced(sequence, keep, reduce, computeDefaultResult(reduce), options);
    }

/**
 * <p>Overloaded function for {@link #blockingFilteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
 */
public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingFilteredReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
    return blockingFilteredReduced(pool, begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
}

/**
 * <p>Overloaded function for {@link #blockingFilteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
 */
public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingFilteredReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
    return blockingFilteredReduced(pool, begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
}

/**
 * <p>Overloaded function for {@link #blockingFilteredReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
 */
public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingFilteredReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
    return blockingFilteredReduced(pool, begin, end, keep, reduce, computeDefaultResult(reduce), options);
}

/**
 * <p>Overloaded function for {@link #blockingFilteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 *  with <code>options = SequentialReduce | UnorderedReduce</code>.</p>
 */
public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingFilteredReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce) {
    return blockingFilteredReduced(begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
}

/**
 * <p>Overloaded function for {@link #blockingFilteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
 */
public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingFilteredReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
    return blockingFilteredReduced(begin, end, keep, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
}

/**
 * <p>Overloaded function for {@link #blockingFilteredReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.KeepFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}.</p>
 */
public static <U,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> U blockingFilteredReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull KeepFunctor<T> keep, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U, T> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options){
    return blockingFilteredReduced(begin, end, keep, reduce, computeDefaultResult(reduce), options);
}

    /**
     * <p>Overloaded function for {@link #mappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     */
    public static <U,V,T> io.qt.core.@NonNull QFuture<U> mappedReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
        return mappedReduced(pool, sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #mappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     */
    public static <U,V,T> io.qt.core.@NonNull QFuture<U> mappedReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return mappedReduced(pool, sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #mappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     */
    public static <U,V,T> io.qt.core.@NonNull QFuture<U> mappedReduced(io.qt.core.@Nullable QThreadPool pool, java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options) {
        return mappedReduced(pool, sequence, map, reduce, computeDefaultResult(reduce), options);
    }

    /**
     * <p>Overloaded function for {@link #mappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     */
    public static <U,V,T> io.qt.core.@NonNull QFuture<U> mappedReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
        return mappedReduced(sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
    }

    /**
     * <p>Overloaded function for {@link #mappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     */
    public static <U,V,T> io.qt.core.@NonNull QFuture<U> mappedReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
        return mappedReduced(sequence, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
    }

    /**
     * <p>Overloaded function for {@link #mappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
     */
    public static <U,V,T> io.qt.core.@NonNull QFuture<U> mappedReduced(java.util.@StrictNonNull Collection<T> sequence, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options) {
        return mappedReduced(sequence, map, reduce, computeDefaultResult(reduce), options);
    }

/**
 * <p>Overloaded function for {@link #mappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 */
public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> mappedReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
    return mappedReduced(pool, begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
}

/**
 * <p>Overloaded function for {@link #mappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 */
public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> mappedReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
    return mappedReduced(pool, begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
}

/**
 * <p>Overloaded function for {@link #mappedReduced(io.qt.core.QThreadPool, java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 */
public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> mappedReduced(io.qt.core.@Nullable QThreadPool pool, @StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options) {
    return mappedReduced(pool, begin, end, map, reduce, computeDefaultResult(reduce), options);
}

/**
 * <p>Overloaded function for {@link #mappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 */
public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> mappedReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce) {
    return mappedReduced(begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(5));
}

/**
 * <p>Overloaded function for {@link #mappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 */
public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> mappedReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOption @NonNull... options) {
    return mappedReduced(begin, end, map, reduce, new io.qt.concurrent.QtConcurrent.ReduceOptions(options));
}

/**
 * <p>Overloaded function for {@link #mappedReduced(java.util.Collection, io.qt.concurrent.QtConcurrent.MappedFunctor, io.qt.concurrent.QtConcurrent.ReduceFunctor, java.lang.Object, io.qt.concurrent.QtConcurrent.ReduceOptions)}
 */
public static <U,V,T,Container extends QtObjectInterface,Iterator extends io.qt.core.QSequentialConstIterator<T,Container>> io.qt.core.@NonNull QFuture<U> mappedReduced(@StrictNonNull Iterator begin, @StrictNonNull Iterator end, io.qt.concurrent.QtConcurrent.@StrictNonNull MappedFunctor<V,T> map, io.qt.concurrent.QtConcurrent.@StrictNonNull ReduceFunctor<U,V> reduce, io.qt.concurrent.QtConcurrent.@NonNull ReduceOptions options) {
    return mappedReduced(begin, end, map, reduce, computeDefaultResult(reduce), options);
}
}// class

class QtConcurrent_DISMISS_1__ extends QtConcurrent {
    /**
     * The QTaskBuilder class is used for adjusting task parameters. Call {@code withArguments(...)} to assign arguments to the task.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     */
    public static abstract class QTaskBuilder<T>{
        QTaskBuilder(int priority, QThreadPool threadPool) {
            super();
            this.priority = priority;
            this.threadPool = threadPool;
        }
        private int priority;
        private QThreadPool threadPool;
        
        @QtUninvokable
        final int priority() {return priority;}
        @QtUninvokable
        final QThreadPool threadPool() { return threadPool; }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn"><code>QtConcurrent::QTaskBuilder::spawn()</code></a></p>
         * @throws RuntimeException if there are still missing arguments. Therfore, call {@code withArguments(...)} to assign arguments to the task.
         */
        @QtUninvokable
        public @NonNull QFuture<T> spawn() {
            throw new RuntimeException("Unable to spawn due to missing arguments.");
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#spawn-1"><code>QtConcurrent::QTaskBuilder::spawn(QtConcurrent::FutureResult)</code></a></p>
         * @throws RuntimeException if there are still missing arguments. Therfore, call {@code withArguments(...)} to assign arguments to the task.
         */
        @QtUninvokable
        public void spawn(QtConcurrent.FutureResult result) {
            throw new RuntimeException("Unable to spawn due to missing arguments.");
        }

        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withPriority"><code>QtConcurrent::QTaskBuilder::withPriority(int)</code></a></p>
         */
        @QtUninvokable
        public @NonNull QTaskBuilder<T> withPriority(int newPriority) {
            this.priority = newPriority;
            return this;
        }
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#onThreadPool"><code>QtConcurrent::QTaskBuilder::onThreadPool(QThreadPool&amp;)</code></a></p>
         */
        @QtUninvokable
        public @NonNull QTaskBuilder<T> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            this.threadPool = Objects.requireNonNull(newThreadPool, "Argument 'newThreadPool': null not expected.");
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     */
    public static abstract class QTypedTaskBuilder<T> extends QTaskBuilder<T>{
        QTypedTaskBuilder(int priority, QThreadPool threadPool) {
            super(priority, threadPool);
        }
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static QTaskBuilderVoid0Arg0 task(QtFuture.@StrictNonNull Runnable runnable) {
        return new QTaskBuilderVoid0Arg0(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static class QTaskBuilderVoid0Arg0 extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid0Arg0(int newPriority, QThreadPool threadPool, QtFuture.Runnable runnable) {
            super(newPriority, threadPool);
            this.runnable = runnable;
        }
        private final QtFuture.Runnable runnable;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid0Arg0 withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid0Arg0 onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable);
        }
        
        private native static QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable runnable);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable);
        }

        private native static void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable runnable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A> QTaskBuilderVoid1Arg0<A> task(QtFuture.@StrictNonNull Runnable1<A> runnable) {
        return new QTaskBuilderVoid1Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid1Arg0<A> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable1<A> runnable;
        
        QTaskBuilderVoid1Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable1<A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid1Arg1<A> withArguments(A a) {
            return new QTaskBuilderVoid1Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid1Arg0<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid1Arg0<A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid1Arg1<A> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid1Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable1<A> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable1<A> runnable;
        private final A a;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid1Arg1<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid1Arg1<A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a);
        }
        
        private native static <A> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable1<A> runnable, A a);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a);
        }

        private native static <A> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable1<A> runnable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B> QTaskBuilderVoid2Arg0<A, B> task(QtFuture.@StrictNonNull Runnable2<A, B> runnable) {
        return new QTaskBuilderVoid2Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid2Arg0<A, B> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable2<A, B> runnable;
        
        QTaskBuilderVoid2Arg0(int priority, QThreadPool threadPool, QtFuture.@StrictNonNull Runnable2<A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid2Arg1<A, B> withArguments(A a) {
            return new QTaskBuilderVoid2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid2Arg2<A, B> withArguments(A a, B b) {
            return new QTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid2Arg0<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid2Arg0<A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid2Arg1<A, B> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid2Arg1(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull Runnable2<A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable2<A, B> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid2Arg2<A, B> withArguments(B b) {
            return new QTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid2Arg1<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid2Arg1<A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid2Arg2<A, B> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid2Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable2<A, B> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable2<A, B> runnable;
        private final A a;
        private final B b;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid2Arg2<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid2Arg2<A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b);
        }
        
        private native static <A, B> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable2<A, B> runnable, A a, B b);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b);
        }

        private native static <A, B> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable2<A, B> runnable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C> QTaskBuilderVoid3Arg0<A, B, C> task(QtFuture.@StrictNonNull Runnable3<A, B, C> runnable) {
        return new QTaskBuilderVoid3Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid3Arg0<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable3<A, B, C> runnable;
        
        QTaskBuilderVoid3Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable3<A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid3Arg1<A, B, C> withArguments(A a) {
            return new QTaskBuilderVoid3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid3Arg2<A, B, C> withArguments(A a, B b) {
            return new QTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid3Arg3<A, B, C> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg0<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg0<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid3Arg1<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid3Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable3<A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable3<A, B, C> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid3Arg2<A, B, C> withArguments(B b) {
            return new QTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid3Arg3<A, B, C> withArguments(B b, C c) {
            return new QTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg1<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg1<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid3Arg2<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid3Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable3<A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable3<A, B, C> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid3Arg3<A, B, C> withArguments(C c) {
            return new QTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg2<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg2<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid3Arg3<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid3Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable3<A, B, C> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable3<A, B, C> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg3<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid3Arg3<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c);
        }
        
        private native static <A, B, C> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable3<A, B, C> runnable, A a, B b, C c);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c);
        }

        private native static <A, B, C> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable3<A, B, C> runnable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D> QTaskBuilderVoid4Arg0<A, B, C, D> task(QtFuture.@StrictNonNull Runnable4<A, B, C, D> runnable) {
        return new QTaskBuilderVoid4Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid4Arg0<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable4<A, B, C, D> runnable;
        
        QTaskBuilderVoid4Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable4<A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg1<A, B, C, D> withArguments(A a) {
            return new QTaskBuilderVoid4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg2<A, B, C, D> withArguments(A a, B b) {
            return new QTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg0<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg0<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid4Arg1<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid4Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable4<A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable4<A, B, C, D> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg2<A, B, C, D> withArguments(B b) {
            return new QTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(B b, C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg1<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg1<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid4Arg2<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid4Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable4<A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(C c, D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg2<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg2<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid4Arg3<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid4Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable4<A, B, C, D> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid4Arg4<A, B, C, D> withArguments(D d) {
            return new QTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg3<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg3<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid4Arg4<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid4Arg4(int newPriority, QThreadPool threadPool, QtFuture.Runnable4<A, B, C, D> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg4<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid4Arg4<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d);
        }
        
        private native static <A, B, C, D> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable4<A, B, C, D> runnable, A a, B b, C c, D d);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c, d);
        }

        private native static <A, B, C, D> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable4<A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E> QTaskBuilderVoid5Arg0<A, B, C, D, E> task(QtFuture.@StrictNonNull Runnable5<A, B, C, D, E> runnable) {
        return new QTaskBuilderVoid5Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid5Arg0<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable5<A, B, C, D, E> runnable;
        
        QTaskBuilderVoid5Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable5<A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg1<A, B, C, D, E> withArguments(A a) {
            return new QTaskBuilderVoid5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(A a, B b) {
            return new QTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg0<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg0<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid5Arg1<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid5Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable5<A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable5<A, B, C, D, E> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(B b) {
            return new QTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(B b, C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg1<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg1<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid5Arg2<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid5Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable5<A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg2<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg2<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid5Arg3<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid5Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable5<A, B, C, D, E> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(D d, E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg3<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg3<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid5Arg4<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid5Arg4(int newPriority, QThreadPool threadPool, QtFuture.Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(E e) {
            return new QTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg4<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg4<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid5Arg5<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid5Arg5(int newPriority, QThreadPool threadPool, QtFuture.Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid5Arg5<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <A, B, C, D, E> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c, d, e);
        }

        private native static <A, B, C, D, E> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F> QTaskBuilderVoid6Arg0<A, B, C, D, E, F> task(QtFuture.@StrictNonNull Runnable6<A, B, C, D, E, F> runnable) {
        return new QTaskBuilderVoid6Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg0<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        
        QTaskBuilderVoid6Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg1<A, B, C, D, E, F> withArguments(A a) {
            return new QTaskBuilderVoid6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg0<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg0<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg1<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid6Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(B b) {
            return new QTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg1<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg1<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg2<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid6Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg2<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg3<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid6Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg3<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg4<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid6Arg4(int newPriority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(E e, F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg4<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg5<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid6Arg5(int newPriority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(F f) {
            return new QTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg5<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid6Arg6<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid6Arg6(int newPriority, QThreadPool threadPool, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid6Arg6<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <A, B, C, D, E, F> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }

        private native static <A, B, C, D, E, F> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F, G> QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> task(QtFuture.@StrictNonNull Runnable7<A, B, C, D, E, F, G> runnable) {
        return new QTaskBuilderVoid7Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        
        QTaskBuilderVoid7Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withArguments(A a) {
            return new QTaskBuilderVoid7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(B b) {
            return new QTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg4(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg5(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg6(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(G g) {
            return new QTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid7Arg7(int newPriority, QThreadPool threadPool, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <A, B, C, D, E, F, G> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }

        private native static <A, B, C, D, E, F, G> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }

    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F, G, H> QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> task(QtFuture.@StrictNonNull Runnable8<A, B, C, D, E, F, G, H> runnable) {
        return new QTaskBuilderVoid8Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        
        QTaskBuilderVoid8Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTaskBuilderVoid8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg4(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg5(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg6(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg7(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid8Arg8(int newPriority, QThreadPool threadPool, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <A, B, C, D, E, F, G, H> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }

        private native static <A, B, C, D, E, F, G, H> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F, G, H, I> QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> task(QtFuture.@StrictNonNull Runnable9<A, B, C, D, E, F, G, H, I> runnable) {
        return new QTaskBuilderVoid9Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        
        QTaskBuilderVoid9Arg0(int priority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTaskBuilderVoid9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg1(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg2(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg3(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg4(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg5(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg6(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg7(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg8(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QTaskBuilderVoid9Arg9(int newPriority, QThreadPool threadPool, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
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
        private final QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable;
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
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <A, B, C, D, E, F, G, H, I> QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        private native static <A, B, C, D, E, F, G, H, I> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     */
    public static <T> @NonNull QTypedTaskBuilder0Arg0<T> task(QtFuture.@StrictNonNull Callable<T> callable) {
        return new QTypedTaskBuilder0Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder0Arg0<T> extends QTaskBuilder<T>{
        QTypedTaskBuilder0Arg0(int newPriority, QThreadPool threadPool, QtFuture.Callable<T> callable) {
            super(newPriority, threadPool);
            this.callable = callable;
        }
        private final QtFuture.Callable<T> callable;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder0Arg0<T> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder0Arg0<T> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable);
        }
        
        private native static <T> QFuture<T> spawn(QThreadPool threadPool, int priority, QtFuture.Callable<T> callable);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable);
        }

        private native static void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable<?> callable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A> @NonNull QTypedTaskBuilder1Arg0<T, A> task(QtFuture.@StrictNonNull Callable1<T, A> callable) {
        return new QTypedTaskBuilder1Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder1Arg0<T, A> extends QTaskBuilder<T>{
        private final QtFuture.Callable1<T, A> callable;
        
        QTypedTaskBuilder1Arg0(int priority, QThreadPool threadPool, QtFuture.Callable1<T, A> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder1Arg1<T, A> withArguments(A a) {
            return new QTypedTaskBuilder1Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder1Arg0<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder1Arg0<T, A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder1Arg1<T, A> extends QTaskBuilder<T>{
        QTypedTaskBuilder1Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable1<T, A> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable1<T, A> callable;
        private final A a;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder1Arg1<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder1Arg1<T, A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a);
        }
        
        private native static <T, A> QFuture<T> spawn(QThreadPool threadPool, int priority, QtFuture.Callable1<T, A> callable, A a);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a);
        }

        private native static <A> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable1<?, A> callable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B> @NonNull QTypedTaskBuilder2Arg0<T, A, B> task(QtFuture.@StrictNonNull Callable2<T, A, B> callable) {
        return new QTypedTaskBuilder2Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder2Arg0<T, A, B> extends QTaskBuilder<T>{
        private final QtFuture.Callable2<T, A, B> callable;
        
        QTypedTaskBuilder2Arg0(int priority, QThreadPool threadPool, QtFuture.Callable2<T, A, B> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder2Arg1<T, A, B> withArguments(A a) {
            return new QTypedTaskBuilder2Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder2Arg2<T, A, B> withArguments(A a, B b) {
            return new QTypedTaskBuilder2Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder2Arg0<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder2Arg0<T, A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder2Arg1<T, A, B> extends QTaskBuilder<T>{
        QTypedTaskBuilder2Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable2<T, A, B> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable2<T, A, B> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder2Arg2<T, A, B> withArguments(B b) {
            return new QTypedTaskBuilder2Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder2Arg1<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder2Arg1<T, A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder2Arg2<T, A, B> extends QTaskBuilder<T>{
        QTypedTaskBuilder2Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable2<T, A, B> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable2<T, A, B> callable;
        private final A a;
        private final B b;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder2Arg2<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder2Arg2<T, A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b);
        }
        
        private native static <T, A, B> QFuture<T> spawn(QThreadPool threadPool, int priority, QtFuture.Callable2<T, A, B> callable, A a, B b);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b);
        }

        private native static <A, B> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable2<?, A, B> callable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C> @NonNull QTypedTaskBuilder3Arg0<T, A, B, C> task(QtFuture.@StrictNonNull Callable3<T, A, B, C> callable) {
        return new QTypedTaskBuilder3Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder3Arg0<T, A, B, C> extends QTaskBuilder<T>{
        private final QtFuture.Callable3<T, A, B, C> callable;
        
        QTypedTaskBuilder3Arg0(int priority, QThreadPool threadPool, QtFuture.Callable3<T, A, B, C> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder3Arg1<T, A, B, C> withArguments(A a) {
            return new QTypedTaskBuilder3Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder3Arg2<T, A, B, C> withArguments(A a, B b) {
            return new QTypedTaskBuilder3Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder3Arg3<T, A, B, C> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder3Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg0<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg0<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder3Arg1<T, A, B, C> extends QTaskBuilder<T>{
        QTypedTaskBuilder3Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable3<T, A, B, C> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable3<T, A, B, C> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder3Arg2<T, A, B, C> withArguments(B b) {
            return new QTypedTaskBuilder3Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder3Arg3<T, A, B, C> withArguments(B b, C c) {
            return new QTypedTaskBuilder3Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg1<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg1<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder3Arg2<T, A, B, C> extends QTaskBuilder<T>{
        QTypedTaskBuilder3Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable3<T, A, B, C> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable3<T, A, B, C> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder3Arg3<T, A, B, C> withArguments(C c) {
            return new QTypedTaskBuilder3Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg2<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg2<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder3Arg3<T, A, B, C> extends QTaskBuilder<T>{
        QTypedTaskBuilder3Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable3<T, A, B, C> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable3<T, A, B, C> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg3<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder3Arg3<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c);
        }
        
        private native static <T, A, B, C> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable3<T, A, B, C> callable, A a, B b, C c);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c);
        }

        private native static <A, B, C> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable3<?, A, B, C> callable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D> @NonNull QTypedTaskBuilder4Arg0<T, A, B, C, D> task(QtFuture.@StrictNonNull Callable4<T, A, B, C, D> callable) {
        return new QTypedTaskBuilder4Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder4Arg0<T, A, B, C, D> extends QTaskBuilder<T>{
        private final QtFuture.Callable4<T, A, B, C, D> callable;
        
        QTypedTaskBuilder4Arg0(int priority, QThreadPool threadPool, QtFuture.Callable4<T, A, B, C, D> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg1<T, A, B, C, D> withArguments(A a) {
            return new QTypedTaskBuilder4Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg2<T, A, B, C, D> withArguments(A a, B b) {
            return new QTypedTaskBuilder4Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg0<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg0<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder4Arg1<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable4<T, A, B, C, D> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable4<T, A, B, C, D> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg2<T, A, B, C, D> withArguments(B b) {
            return new QTypedTaskBuilder4Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(B b, C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg1<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg1<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder4Arg2<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable4<T, A, B, C, D> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(C c, D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg2<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg2<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder4Arg3<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable4<T, A, B, C, D> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder4Arg4<T, A, B, C, D> withArguments(D d) {
            return new QTypedTaskBuilder4Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg3<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg3<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder4Arg4<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedTaskBuilder4Arg4(int newPriority, QThreadPool threadPool, QtFuture.Callable4<T, A, B, C, D> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg4<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder4Arg4<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d);
        }
        
        private native static <T, A, B, C, D> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable4<T, A, B, C, D> callable, A a, B b, C c, D d);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c, d);
        }

        private native static <A, B, C, D> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable4<?, A, B, C, D> callable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E> @NonNull QTypedTaskBuilder5Arg0<T, A, B, C, D, E> task(QtFuture.@StrictNonNull Callable5<T, A, B, C, D, E> callable) {
        return new QTypedTaskBuilder5Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder5Arg0<T, A, B, C, D, E> extends QTaskBuilder<T>{
        private final QtFuture.Callable5<T, A, B, C, D, E> callable;
        
        QTypedTaskBuilder5Arg0(int priority, QThreadPool threadPool, QtFuture.Callable5<T, A, B, C, D, E> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg1<T, A, B, C, D, E> withArguments(A a) {
            return new QTypedTaskBuilder5Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(A a, B b) {
            return new QTypedTaskBuilder5Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg0<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg0<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder5Arg1<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable5<T, A, B, C, D, E> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable5<T, A, B, C, D, E> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(B b) {
            return new QTypedTaskBuilder5Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(B b, C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg1<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg1<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder5Arg2<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable5<T, A, B, C, D, E> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg2<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder5Arg3<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable5<T, A, B, C, D, E> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(D d, E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg3<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder5Arg4<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg4(int newPriority, QThreadPool threadPool, QtFuture.Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(E e) {
            return new QTypedTaskBuilder5Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg4<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder5Arg5<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedTaskBuilder5Arg5(int newPriority, QThreadPool threadPool, QtFuture.Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder5Arg5<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e);
        }
        
        private native static <T, A, B, C, D, E> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d, E e);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c, d, e);
        }

        private native static <A, B, C, D, E> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable5<?, A, B, C, D, E> callable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F> @NonNull QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> task(QtFuture.@StrictNonNull Callable6<T, A, B, C, D, E, F> callable) {
        return new QTypedTaskBuilder6Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        
        QTypedTaskBuilder6Arg0(int priority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> withArguments(A a) {
            return new QTypedTaskBuilder6Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTypedTaskBuilder6Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(B b) {
            return new QTypedTaskBuilder6Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg4(int newPriority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(E e, F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg5(int newPriority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(F f) {
            return new QTypedTaskBuilder6Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedTaskBuilder6Arg6(int newPriority, QThreadPool threadPool, QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f);
        }
        
        private native static <T, A, B, C, D, E, F> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e, F f);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c, d, e, f);
        }

        private native static <A, B, C, D, E, F> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable6<?, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F, G> @NonNull QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> task(QtFuture.@StrictNonNull Callable7<T, A, B, C, D, E, F, G> callable) {
        return new QTypedTaskBuilder7Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        
        QTypedTaskBuilder7Arg0(int priority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withArguments(A a) {
            return new QTypedTaskBuilder7Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTypedTaskBuilder7Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(B b) {
            return new QTypedTaskBuilder7Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg4(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg5(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg6(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(G g) {
            return new QTypedTaskBuilder7Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedTaskBuilder7Arg7(int newPriority, QThreadPool threadPool, QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f, g);
        }
        
        private native static <T, A, B, C, D, E, F, G> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f, G g);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c, d, e, f, g);
        }

        private native static <A, B, C, D, E, F, G> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable7<?, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F, G, H> @NonNull QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> task(QtFuture.@StrictNonNull Callable8<T, A, B, C, D, E, F, G, H> callable) {
        return new QTypedTaskBuilder8Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        
        QTypedTaskBuilder8Arg0(int priority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTypedTaskBuilder8Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTypedTaskBuilder8Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTypedTaskBuilder8Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg4(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg5(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg6(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg7(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QTypedTaskBuilder8Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedTaskBuilder8Arg8(int newPriority, QThreadPool threadPool, QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f, g, h);
        }
        
        private native static <T, A, B, C, D, E, F, G, H> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g, H h);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c, d, e, f, g, h);
        }

        private native static <A, B, C, D, E, F, G, H> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable8<?, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F, G, H, I> @NonNull QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> task(QtFuture.@StrictNonNull Callable9<T, A, B, C, D, E, F, G, H, I> callable) {
        return new QTypedTaskBuilder9Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        
        QTypedTaskBuilder9Arg0(int priority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTypedTaskBuilder9Arg1<>(priority(), threadPool(), callable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTypedTaskBuilder9Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg1(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTypedTaskBuilder9Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg2(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg3(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg4(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg5(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg6(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg7(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg8(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QTypedTaskBuilder9Arg9<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedTaskBuilder9Arg9(int newPriority, QThreadPool threadPool, QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
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
        private final QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable;
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
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), callable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

        /**
         * {@inheritDoc}
         */
        @Override
        public void spawn(QtConcurrent.FutureResult result) {
            spawnIgnore(threadPool(), priority(), callable, a, b, c, d, e, f, g, h, i);
        }

        private native static <A, B, C, D, E, F, G, H, I> void spawnIgnore(QThreadPool threadPool, int priority, QtFuture.Callable9<?, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
        
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static QPromiseTaskBuilderVoid0Arg0 task(QtFuture.@StrictNonNull RunnableWithVoidPromise runnable) {
        return new QPromiseTaskBuilderVoid0Arg0(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid0Arg0 extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid0Arg0(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise runnable) {
            super(newPriority, threadPool);
            this.runnable = runnable;
        }
        private final QtFuture.RunnableWithVoidPromise runnable;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid0Arg0 withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid0Arg0 onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable);
        }
        
        private native static QFuture<Void> spawn(QThreadPool threadPool, int priority, QtFuture.RunnableWithVoidPromise runnable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A> QPromiseTaskBuilderVoid1Arg0<A> task(QtFuture.@StrictNonNull RunnableWithVoidPromise1<A> runnable) {
        return new QPromiseTaskBuilderVoid1Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid1Arg0<A> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise1<A> runnable;
        
        QPromiseTaskBuilderVoid1Arg0(int priority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise1<A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid1Arg1<A> withArguments(A a) {
            return new QPromiseTaskBuilderVoid1Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid1Arg0<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid1Arg0<A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid1Arg1<A> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid1Arg1(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise1<A> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise1<A> runnable;
        private final A a;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid1Arg1<A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid1Arg1<A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a);
        }
        
        private native static <A> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise1<A> runnable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B> QPromiseTaskBuilderVoid2Arg0<A, B> task(QtFuture.@StrictNonNull RunnableWithVoidPromise2<A, B> runnable) {
        return new QPromiseTaskBuilderVoid2Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid2Arg0<A, B> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise2<A, B> runnable;
        
        QPromiseTaskBuilderVoid2Arg0(int priority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise2<A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid2Arg1<A, B> withArguments(A a) {
            return new QPromiseTaskBuilderVoid2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid2Arg2<A, B> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid2Arg0<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid2Arg0<A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid2Arg1<A, B> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid2Arg1(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise2<A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise2<A, B> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid2Arg2<A, B> withArguments(B b) {
            return new QPromiseTaskBuilderVoid2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid2Arg1<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid2Arg1<A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid2Arg2<A, B> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid2Arg2(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise2<A, B> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise2<A, B> runnable;
        private final A a;
        private final B b;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid2Arg2<A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid2Arg2<A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b);
        }
        
        private native static <A, B> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise2<A, B> runnable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C> QPromiseTaskBuilderVoid3Arg0<A, B, C> task(QtFuture.@StrictNonNull RunnableWithVoidPromise3<A, B, C> runnable) {
        return new QPromiseTaskBuilderVoid3Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid3Arg0<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise3<A, B, C> runnable;
        
        QPromiseTaskBuilderVoid3Arg0(int priority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise3<A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> withArguments(A a) {
            return new QPromiseTaskBuilderVoid3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg0<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg0<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid3Arg1<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid3Arg1(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise3<A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withArguments(B b) {
            return new QPromiseTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid3Arg2<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid3Arg2(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise3<A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withArguments(C c) {
            return new QPromiseTaskBuilderVoid3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid3Arg3<A, B, C> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid3Arg3(int newPriority, QThreadPool threadPool, QtFuture.@StrictNonNull RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid3Arg3<A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c);
        }
        
        private native static <A, B, C> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D> QPromiseTaskBuilderVoid4Arg0<A, B, C, D> task(QtFuture.@StrictNonNull RunnableWithVoidPromise4<A, B, C, D> runnable) {
        return new QPromiseTaskBuilderVoid4Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid4Arg0<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable;
        
        QPromiseTaskBuilderVoid4Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> withArguments(A a) {
            return new QPromiseTaskBuilderVoid4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg0<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg0<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid4Arg1<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid4Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withArguments(B b) {
            return new QPromiseTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid4Arg2<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid4Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid4Arg3<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid4Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withArguments(D d) {
            return new QPromiseTaskBuilderVoid4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid4Arg4<A, B, C, D> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid4Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid4Arg4<A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d);
        }
        
        private native static <A, B, C, D> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E> QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> task(QtFuture.@StrictNonNull RunnableWithVoidPromise5<A, B, C, D, E> runnable) {
        return new QPromiseTaskBuilderVoid5Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        
        QPromiseTaskBuilderVoid5Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> withArguments(A a) {
            return new QPromiseTaskBuilderVoid5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid5Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(B b) {
            return new QPromiseTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid5Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid5Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid5Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withArguments(E e) {
            return new QPromiseTaskBuilderVoid5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid5Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <A, B, C, D, E> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F> QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> task(QtFuture.@StrictNonNull RunnableWithVoidPromise6<A, B, C, D, E, F> runnable) {
        return new QPromiseTaskBuilderVoid6Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        
        QPromiseTaskBuilderVoid6Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> withArguments(A a) {
            return new QPromiseTaskBuilderVoid6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid6Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(B b) {
            return new QPromiseTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid6Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid6Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid6Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid6Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withArguments(F f) {
            return new QPromiseTaskBuilderVoid6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid6Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <A, B, C, D, E, F> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F, G> QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> task(QtFuture.@StrictNonNull RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable) {
        return new QPromiseTaskBuilderVoid7Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        
        QPromiseTaskBuilderVoid7Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withArguments(A a) {
            return new QPromiseTaskBuilderVoid7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(B b) {
            return new QPromiseTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withArguments(G g) {
            return new QPromiseTaskBuilderVoid7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid7Arg7(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <A, B, C, D, E, F, G> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F, G, H> QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> task(QtFuture.@StrictNonNull RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable) {
        return new QPromiseTaskBuilderVoid8Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        
        QPromiseTaskBuilderVoid8Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QPromiseTaskBuilderVoid8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QPromiseTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg7(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QPromiseTaskBuilderVoid8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid8Arg8(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <A, B, C, D, E, F, G, H> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <A, B, C, D, E, F, G, H, I> QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> task(QtFuture.@StrictNonNull RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable) {
        return new QPromiseTaskBuilderVoid9Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        
        QPromiseTaskBuilderVoid9Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QPromiseTaskBuilderVoid9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QPromiseTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg7(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg8(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QPromiseTaskBuilderVoid9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> extends QTaskBuilder<@QtPrimitiveType Void>{
        QPromiseTaskBuilderVoid9Arg9(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
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
        private final QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
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
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<@QtPrimitiveType Void> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <A, B, C, D, E, F, G, H, I> QFuture<Void> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T> QTypedPromiseTaskBuilder0Arg0<T> task(QtFuture.@StrictNonNull RunnableWithPromise<T> callable) {
        return new QTypedPromiseTaskBuilder0Arg0<>(0, null, callable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder0Arg0<T> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder0Arg0(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise<T> runnable) {
            super(newPriority, threadPool);
            this.runnable = runnable;
        }
        private final QtFuture.RunnableWithPromise<T> runnable;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder0Arg0<T> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder0Arg0<T> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable);
        }
        
        private native static <T> QFuture<T> spawn(QThreadPool threadPool, int priority, QtFuture.RunnableWithPromise<T> runnable);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A> QTypedPromiseTaskBuilder1Arg0<T, A> task(QtFuture.@StrictNonNull RunnableWithPromise1<T, A> runnable) {
        return new QTypedPromiseTaskBuilder1Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder1Arg0<T, A> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise1<T, A> runnable;
        
        QTypedPromiseTaskBuilder1Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise1<T, A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder1Arg1<T, A> withArguments(A a) {
            return new QTypedPromiseTaskBuilder1Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder1Arg0<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder1Arg0<T, A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder1Arg1<T, A> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder1Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise1<T, A> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise1<T, A> runnable;
        private final A a;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder1Arg1<T, A> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder1Arg1<T, A> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a);
        }
        
        private native static <T, A> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise1<T, A> runnable, A a);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B> QTypedPromiseTaskBuilder2Arg0<T, A, B> task(QtFuture.@StrictNonNull RunnableWithPromise2<T, A, B> runnable) {
        return new QTypedPromiseTaskBuilder2Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder2Arg0<T, A, B> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise2<T, A, B> runnable;
        
        QTypedPromiseTaskBuilder2Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise2<T, A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> withArguments(A a) {
            return new QTypedPromiseTaskBuilder2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder2Arg0<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder2Arg0<T, A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder2Arg1<T, A, B> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder2Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise2<T, A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise2<T, A, B> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> withArguments(B b) {
            return new QTypedPromiseTaskBuilder2Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder2Arg2<T, A, B> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder2Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise2<T, A, B> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise2<T, A, B> runnable;
        private final A a;
        private final B b;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder2Arg2<T, A, B> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b);
        }
        
        private native static <T, A, B> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise2<T, A, B> runnable, A a, B b);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C> QTypedPromiseTaskBuilder3Arg0<T, A, B, C> task(QtFuture.@StrictNonNull RunnableWithPromise3<T, A, B, C> runnable) {
        return new QTypedPromiseTaskBuilder3Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder3Arg0<T, A, B, C> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise3<T, A, B, C> runnable;
        
        QTypedPromiseTaskBuilder3Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise3<T, A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> withArguments(A a) {
            return new QTypedPromiseTaskBuilder3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg0<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg0<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder3Arg1<T, A, B, C> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise3<T, A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withArguments(B b) {
            return new QTypedPromiseTaskBuilder3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder3Arg2<T, A, B, C> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise3<T, A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withArguments(C c) {
            return new QTypedPromiseTaskBuilder3Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder3Arg3<T, A, B, C> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder3Arg3<T, A, B, C> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c);
        }
        
        private native static <T, A, B, C> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D> QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> task(QtFuture.@StrictNonNull RunnableWithPromise4<T, A, B, C, D> runnable) {
        return new QTypedPromiseTaskBuilder4Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable;
        
        QTypedPromiseTaskBuilder4Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> withArguments(A a) {
            return new QTypedPromiseTaskBuilder4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withArguments(B b) {
            return new QTypedPromiseTaskBuilder4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withArguments(D d) {
            return new QTypedPromiseTaskBuilder4Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d);
        }
        
        private native static <T, A, B, C, D> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E> QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> task(QtFuture.@StrictNonNull RunnableWithPromise5<T, A, B, C, D, E> runnable) {
        return new QTypedPromiseTaskBuilder5Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable;
        
        QTypedPromiseTaskBuilder5Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> withArguments(A a) {
            return new QTypedPromiseTaskBuilder5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(B b) {
            return new QTypedPromiseTaskBuilder5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withArguments(E e) {
            return new QTypedPromiseTaskBuilder5Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <T, A, B, C, D, E> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F> QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> task(QtFuture.@StrictNonNull RunnableWithPromise6<T, A, B, C, D, E, F> runnable) {
        return new QTypedPromiseTaskBuilder6Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        
        QTypedPromiseTaskBuilder6Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> withArguments(A a) {
            return new QTypedPromiseTaskBuilder6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(B b) {
            return new QTypedPromiseTaskBuilder6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withArguments(F f) {
            return new QTypedPromiseTaskBuilder6Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <T, A, B, C, D, E, F> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F, G> QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> task(QtFuture.@StrictNonNull RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable) {
        return new QTypedPromiseTaskBuilder7Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        
        QTypedPromiseTaskBuilder7Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withArguments(A a) {
            return new QTypedPromiseTaskBuilder7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(B b) {
            return new QTypedPromiseTaskBuilder7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withArguments(G g) {
            return new QTypedPromiseTaskBuilder7Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg7(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <T, A, B, C, D, E, F, G> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F, G, H> QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> task(QtFuture.@StrictNonNull RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable) {
        return new QTypedPromiseTaskBuilder8Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        
        QTypedPromiseTaskBuilder8Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTypedPromiseTaskBuilder8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTypedPromiseTaskBuilder8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(F f, G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(G g, H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg7(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withArguments(H h) {
            return new QTypedPromiseTaskBuilder8Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg8(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <T, A, B, C, D, E, F, G, H> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * <p>See <a href="@docRoot/qtconcurrent.html#task"><code>QtConcurrent::task(Task&amp;&amp;)</code></a></p>
     * @return {@link QTaskBuilder}
     */
    public static <T, A, B, C, D, E, F, G, H, I> QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> task(QtFuture.@StrictNonNull RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable) {
        return new QTypedPromiseTaskBuilder9Arg0<>(0, null, runnable);
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        
        QTypedPromiseTaskBuilder9Arg0(int priority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTypedPromiseTaskBuilder9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg1(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTypedPromiseTaskBuilder9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg2(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg3(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
        }
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg4(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
        }
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg5(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
        }
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg6(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
            this.c = c;
            this.d = d;
            this.e = e;
            this.f = f;
        }
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg7(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g) {
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
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(H h, I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg8(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
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
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        private final C c;
        private final D d;
        private final E e;
        private final F f;
        private final G g;
        private final H h;

        /**
         * <p>See <a href="@docRoot/qtconcurrent-qtaskbuilder.html#withArguments"><code>QtConcurrent::QTaskBuilder::withArguments(ExtraArgs &amp;&amp;...)</code></a></p>
         */
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withArguments(I i) {
            return new QTypedPromiseTaskBuilder9Arg9<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h, i);
        }
        
        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
    }
    
    /**
     * The QTaskBuilder class is used for adjusting task parameters.
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent-qtaskbuilder.html"><code>QtConcurrent::QTaskBuilder</code></a></p>
     * @hidden
     */
    public static final class QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> extends QTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg9(int newPriority, QThreadPool threadPool, QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
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
        private final QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
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
         * {@inheritDoc}
         */
        @Override
        public @NonNull QFuture<T> spawn() {
            return spawn(threadPool(), priority(), runnable, a, b, c, d, e, f, g, h, i);
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> withPriority(int newPriority) {
            super.withPriority(newPriority);
            return this;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> onThreadPool(@StrictNonNull QThreadPool newThreadPool) {
            super.onThreadPool(newThreadPool);
            return this;
        }
        private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> spawn(QThreadPool threadPool, int priority,
                QtFuture.RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
}// class


class QtConcurrent_DISMISS__ extends QtConcurrent {
    
    static class ThreadEngine<T> extends ThreadEngineBase
    {
        ThreadEngine(@Nullable QThreadPool pool) {
            super(pool);
            Objects.requireNonNull(pool, "Argument 'pool': null not expected.");
        }

        @QtUninvokable
        T result() { return null; }

        @SuppressWarnings("unchecked")
        @QtUninvokable
        final QFutureInterface<T> futureInterfaceTyped()
        {
            return (QFutureInterface<T>)futureInterface();
        }

        // Runs the user algorithm using a single thread.
        @QtUninvokable
        final T startSingleThreaded()
        {
            super._startSingleThreaded();
            return result();
        }

        // Runs the user algorithm using multiple threads.
        // Does not block, returns a future.
        @QtUninvokable
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

        @QtPrivateOverride
        @QtUninvokable
        final void asynchronousFinish()
        {
            finish();
            QFutureInterface<T> futureInterface = futureInterfaceTyped();
            futureInterface.reportFinished(result());
            setFutureInterface(null);
            futureInterface.dispose();
            dispose();
        }

        @QtUninvokable
        final void reportResult(T _result) {
            reportResult(_result, -1);
        }
        
        @QtUninvokable
        final void reportResult(T _result, int index)
        {
            QFutureInterface<T> futureInterface = futureInterfaceTyped();
            if (futureInterface!=null)
                futureInterface.reportResult(_result, index);
        }
        
        @QtUninvokable
        final void reportResults(Collection<T> _result) {
            reportResults(_result, -1, -1);
        }
        
        @QtUninvokable
        final void reportResults(Collection<T> _result, int index) {
            reportResults(_result, index, -1);
        }

        @QtUninvokable
        final void reportResults(Collection<T> _result, int index, int count)
        {
            QFutureInterface<T> futureInterface = futureInterfaceTyped();
            if (futureInterface!=null)
                futureInterface.reportResults(_result, index, count);
        }
    }
    
    /**
     * <p>Java wrapper for Qt's class <a href="@docRoot/qtconcurrent.html"><code>QtConcurrent::ThreadEngineStarter</code></a></p>
     */
    public static final class ThreadEngineStarter<T> {
        ThreadEngineStarter(ThreadEngine<T> _threadEngine){
            threadEngine = Objects.requireNonNull(_threadEngine, "Argument 'threadEngine': null not expected.");
        }

        public final QFuture<T> startAsynchronously(){
            return threadEngine.startAsynchronously();
        }
        
        private ThreadEngine<T> threadEngine;
    }
    
    static class IterateKernel<M,T> extends ThreadEngine<T>{
        IterateKernel(@Nullable QThreadPool pool, Collection<M> collection)
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
        MapKernel(@Nullable QThreadPool pool, Collection<T> collection, MapFunctor<T> map) {
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
        MapEachKernel(@Nullable QThreadPool pool, Collection<T> collection, MappedFunctor<U,T> map) {
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
        ReduceKernel(@Nullable QThreadPool pool, @NonNull ReduceOptions reduceOptions) {
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
        MappedReducedKernel(@Nullable QThreadPool pool, Collection<T> collection, MappedFunctor<V,T> map, @StrictNonNull ReduceFunctor<U,V> reduce, @NonNull ReduceOptions reduceOptions) {
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
        private final KeepFunctor<T> keep;
        private final ReduceFunctor<U,T> reduce;
        private final ReduceKernel<U,T> reducer;

        FilteredReducedKernel(@Nullable QThreadPool pool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> keep, @StrictNonNull ReduceFunctor<U, T> reduce, @NonNull ReduceOptions options) {
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
        private final KeepFunctor<T> keep;

        FilteredEachKernel(@Nullable QThreadPool pool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> keep) {
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
        private final KeepFunctor<T> keep;
        private final ReduceFunctor<Collection<T>,T> reduce;
        private final ReduceKernel<Collection<T>,T> reducer;

        FilterKernel(@Nullable QThreadPool pool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> keep, @StrictNonNull ReduceFunctor<Collection<T>, T> reduce, @NonNull ReduceOptions options) {
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
    public static <T> @NonNull ThreadEngineStarter<@QtPrimitiveType Void> startMap(@Nullable QThreadPool threadPool, Collection<T> sequence, MapFunctor<T> functor) {
        return new ThreadEngineStarter<>(new MapKernel<>(Objects.requireNonNull(threadPool, "Argument 'threadPool': null not expected."), Objects.requireNonNull(sequence, "Argument 'sequence': null not expected."), Objects.requireNonNull(functor, "Argument 'functor': null not expected.")));
    }
    
    /**
     * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
     *
     */
    @QtUninvokable
    public static <U, T> @NonNull ThreadEngineStarter<U> startMapped(@Nullable QThreadPool threadPool, Collection<T> sequence, MappedFunctor<U, T> functor){
        return new ThreadEngineStarter<>(new MapEachKernel<>(Objects.requireNonNull(threadPool, "Argument 'threadPool': null not expected."), Objects.requireNonNull(sequence, "Argument 'sequence': null not expected."), Objects.requireNonNull(functor, "Argument 'functor': null not expected.")));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @QtUninvokable
    public static <U, V, T> @NonNull ThreadEngineStarter<U> startMappedReduced(@Nullable QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, @StrictNonNull ReduceFunctor<U, V> reduceFunctor, @NonNull ReduceOptions options) {
        return new ThreadEngineStarter<>(new MappedReducedKernel<>(Objects.requireNonNull(threadPool, "Argument 'threadPool': null not expected."), Objects.requireNonNull(sequence, "Argument 'sequence': null not expected."), Objects.requireNonNull(functor, "Argument 'functor': null not expected."), Objects.requireNonNull(reduceFunctor, "Argument 'reduceFunctor': null not expected."), options));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @QtUninvokable
    public static <U, V, T> @NonNull ThreadEngineStarter<U> startMappedReduced(@Nullable QThreadPool threadPool, Collection<T> sequence, MappedFunctor<V, T> functor, @StrictNonNull ReduceFunctor<U, V> reduceFunctor, ReduceOption... options) {
        return startMappedReduced(threadPool, sequence, functor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @QtUninvokable
    public static <U, T> @NonNull ThreadEngineStarter<U> startFilteredReduced(@Nullable QThreadPool threadPool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> filteredFunctor, @StrictNonNull ReduceFunctor<U, T> reduceFunctor, @NonNull ReduceOption @NonNull... options) {
        return startFilteredReduced(threadPool, sequence, filteredFunctor, reduceFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @QtUninvokable
    public static <U, T> @NonNull ThreadEngineStarter<U> startFilteredReduced(@Nullable QThreadPool threadPool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> filteredFunctor, @StrictNonNull ReduceFunctor<U, T> reduceFunctor, @NonNull ReduceOptions options) {
        return new ThreadEngineStarter<>(new FilteredReducedKernel<>(Objects.requireNonNull(threadPool, "Argument 'threadPool': null not expected."), Objects.requireNonNull(sequence, "Argument 'sequence': null not expected."), Objects.requireNonNull(filteredFunctor, "Argument 'filteredFunctor': null not expected."), Objects.requireNonNull(reduceFunctor, "Argument 'reduceFunctor': null not expected."), options));
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @QtUninvokable
    public static <T> @NonNull ThreadEngineStarter<T> startFiltered(@Nullable QThreadPool threadPool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> filteredFunctor){
        return new ThreadEngineStarter<>(new FilteredEachKernel<>(Objects.requireNonNull(threadPool, "Argument 'threadPool': null not expected."), Objects.requireNonNull(sequence, "Argument 'sequence': null not expected."), Objects.requireNonNull(filteredFunctor, "Argument 'filteredFunctor': null not expected.")));
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @QtUninvokable
    static <T> @NonNull ThreadEngineStarter<@QtPrimitiveType Void> filterInternal(@Nullable QThreadPool threadPool, Collection<T> sequence, @StrictNonNull KeepFunctor<T> filteredFunctor, @StrictNonNull ReduceFunctor<Collection<T>, T> reduceFunctor){
        return new ThreadEngineStarter<>(new FilterKernel<>(Objects.requireNonNull(threadPool, "Argument 'threadPool': null not expected."), Objects.requireNonNull(sequence, "Argument 'sequence': null not expected."), Objects.requireNonNull(filteredFunctor, "Argument 'filteredFunctor': null not expected."), Objects.requireNonNull(reduceFunctor, "Argument 'reduceFunctor': null not expected."), new ReduceOptions()));
    }
    
    private static class PushBackWrapper<T> implements ReduceFunctor<Collection<T>, T>{
        @Override
        public Collection<T> reduce(Collection<T> result, T intermediate) {
            result.add(intermediate);
            return result;
        }
    }
}// class


