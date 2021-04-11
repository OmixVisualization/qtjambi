/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

/**
The QtConcurrent class contains static methods for running computations in parallel (using separate threads)
on the items in a java.util.Collection, such as a Vector or LinkedList. We will now describe these methods.

The QtConcurrent::map(), QtConcurrent::mapped() and QtConcurrent::mappedReduced() functions run computations in parallel on the items in a sequence such as a QList or a QVector. QtConcurrent::map() modifies a sequence in-place, QtConcurrent::mapped() returns a new sequence containing the modified content, and QtConcurrent::mappedReduced() returns a single result.
Concurrent Map
<p>
QtConcurrent::mapped() takes an input sequence and a map function. This map function is then called for each item in the sequence, and a new sequence containing the return values from the map function is returned.
<p>
The map function must be of the form:
<pre>
     U function(const T &amp;t);
</pre>
T and U can be any type (and they can even be the same type), but T must match the type stored in the sequence. The function returns the modified or mapped content.
<p>
This example shows how to apply a scale function to all the items in a sequence:
<pre>
     QImage scaled(const QImage &amp;image)
     {
         return image.scaled(100, 100);
     }

     QList &lt;QImage&gt; images = ...;
     QFuture&lt;QImage&gt; thumbnails = QtConcurrent::mapped(images, scaled);
</pre>
The results of the map are made available through QFuture. See the QFuture and QFutureWatcher documentation for more information on how to use QFuture in your applications.
<p>
If you want to modify a sequence in-place, use QtConcurrent::map(). The map function must then be of the form:
<pre>
     U function(T &amp;t);
</pre>
Note that the return value and return type of the map function are not used.
<p>
Using QtConcurrent::map() is similar to using QtConcurrent::mapped():
<pre>
     void scale(QImage &amp;image)
     {
         image = image.scaled(100, 100);
     }

     QList&lt;QImage&gt; images = ...;
     QFuture&lt;void&gt; future = QtConcurrent::map(images, scale);
</pre>
Since the sequence is modified in place, QtConcurrent::map() does not return any results via QFuture. However, you can still use QFuture and QFutureWatcher to monitor the status of the map.
Concurrent Map-Reduce
<p>
QtConcurrent::mappedReduced() is similar to QtConcurrent::mapped(), but instead of returning a sequence with the new results, the results are combined into a single value using a reduce function.
<p>
The reduce function must be of the form:
<pre>
     V function(T &amp;result, const U &amp;intermediate)
</pre>
T is the type of the final result, U is the return type of the map function. Note that the return value and return type of the reduce function are not used.
<p>
Call QtConcurrent::mappedReduced() like this:
<pre>
     void addToCollage(QImage &amp;collage, const QImage &amp;thumbnail)
     {
         QPainter p(&amp;collage);
         static QPoint offset = QPoint(0, 0);
         p.drawImage(offset, thumbnail);
         offset += ...;
     }

     QList&lt;QImage&gt; images = ...;
     QFuture&lt;QImage&gt; collage = QtConcurrent::mappedReduced(images, scaled, addToCollage);
</pre>
The reduce function will be called once for each result returned by the map function, and should merge the intermediate into the result variable. QtConcurrent::mappedReduced() guarantees that only one thread will call reduce at a time, so using a mutex to lock the result variable is not neccesary. The QtConcurrent::ReduceOptions enum provides a way to control the order in which the reduction is done. If QtConcurrent::UnorderedReduce is used (the default), the order is undefined, while QtConcurrent::OrderedReduce ensures that the reduction is done in the order of the original sequence.
Additional API Features
Using Iterators instead of Sequence
<p>
Each of the above functions has a variant that takes an iterator range instead of a sequence. You use them in the same way as the sequence variants:
<pre>
     QList&lt;QImage&gt; images = ...;

     QFuture&lt;QImage&gt; thumbnails = QtConcurrent::mapped(images.constBegin(), images.constEnd(), scaled);

     // map in-place only works on non-const iterators
     QFuture&lt;void&gt; future = QtConcurrent::map(images.begin(), images.end(), scale);

     QFuture&lt;QImage&gt; collage = QtConcurrent::mappedReduced(images.constBegin(), images.constEnd(), scaled, addToCollage);
</pre>
Blocking Variants
<p>
Each of the above functions has a blocking variant that returns the final result instead of a QFuture. You use them in the same way as the asynchronous variants.
<pre>
     QList&lt;QImage&gt; images = ...;

     // each call blocks until the entire operation is finished
     QList&lt;QImage&gt; future = QtConcurrent::blockingMapped(images, scaled);

     QtConcurrent::blockingMap(images, scale);

     QImage collage = QtConcurrent::blockingMappedReduced(images, scaled, addToCollage);
</pre>
Note that the result types above are not QFuture objects, but real result types (in this case, QList&lt;QImage&gt; and QImage).
Using Member Functions
<p>
QtConcurrent::map(), QtConcurrent::mapped(), and QtConcurrent::mappedReduced() accept pointers to member functions. The member function class type must match the type stored in the sequence:
<pre>
     // squeeze all strings in a QStringList
     QStringList strings = ...;
     QFuture&lt;void&gt; squeezedStrings = QtConcurrent::map(strings, &amp;QString::squeeze);

     // swap the rgb values of all pixels on a list of images
     QList&lt;QImage&gt; images = ...;
     QFuture&lt;QImage&gt; bgrImages = QtConcurrent::mapped(images, &amp;QImage::rgbSwapped);

     // create a set of the lengths of all strings in a list
     QStringList strings = ...;
     QFuture&lt;QSet&lt;int&gt; &gt; wordLengths = QtConcurrent::mappedReduced(string, &amp;QString::length, &amp;QSet&lt;int&gt;::insert);
</pre>
Note that when using QtConcurrent::mappedReduced(), you can mix the use of normal and member functions freely:
<p>
<pre>
     // can mix normal functions and member functions with QtConcurrent::mappedReduced()

     // compute the average length of a list of strings
     extern void computeAverage(int &amp;average, int length);
     QStringList strings = ...;
     QFuture&lt;int&gt; averageWordLength = QtConcurrent::mappedReduced(strings, &amp;QString::length, computeAverage);

     // create a set of the color distribution of all images in a list
     extern int colorDistribution(const QImage &amp;string);
     QList&lt;QImage&gt; images = ...;
     QFuture&lt;QSet&lt;int&gt; &gt; totalColorDistribution = QtConcurrent::mappedReduced(images, colorDistribution, QSet&lt;int&gt;::insert);
</pre>
Using Function Objects
<p>
QtConcurrent::map(), QtConcurrent::mapped(), and QtConcurrent::mappedReduced() accept function objects, which can be used to add state to a function call. The result_type typedef must define the result type of the function call operator:
<pre>
     struct Scaled
     {
         Scaled(int size)
         : m_size(size) { }

         typedef QImage result_type;

         QImage operator()(const QImage &amp;image)
         {
             return image.scaled(m_size, m_size);
         }

         int m_size;
     };

     QList&lt;QImage&gt; images = ...;
     QFuture&lt;QImage&gt; thumbnails = QtConcurrent::mapped(images, Scaled(100));
</pre>
Using Bound Function Arguments
<p>
Note that Qt does not provide support for bound functions. This is provided by 3rd party libraries like Boost or C++ TR1 Library Extensions.
<p>
If you want to use a map function that takes more than one argument you can use boost::bind() or std::tr1::bind() to transform it onto a function that takes one argument.
<p>
As an example, we'll use QImage::scaledToWidth():
<pre>
     QImage QImage::scaledToWidth(int width, Qt::TransformationMode) const;
</pre>
scaledToWidth takes three arguments (including the "this" pointer) and can't be used with QtConcurrent::mapped() directly, because QtConcurrent::mapped() expects a function that takes one argument. To use QImage::scaledToWidth() with QtConcurrent::mapped() we have to provide a value for the width and the transformation mode:
<pre>
     boost::bind(&amp;QImage::scaledToWidth, 100 Qt::SmoothTransformation)
</pre>
The return value from boost::bind() is a function object (functor) with the following signature:
<pre>
     QImage scaledToWith(const QImage &amp;image)
</pre>
This matches what QtConcurrent::mapped() expects, and the complete example becomes:
<pre>
     QList&gt;QImage&lt; images = ...;
     QFuture&gt;QImage&lt; thumbnails = QtConcurrent::mapped(images, boost::bind(&amp;QImage::scaledToWidth, 100 Qt::SmoothTransformation));
</pre>
*/
class QtConcurrent___ extends QtConcurrent {
    
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
        @io.qt.internal.NativeAccess
        public void map(T object);
    }
    /**
     *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
     */
    @io.qt.QtUninvokable
    public static <T> QVoidFuture map(java.util.Collection<T> sequence, MapFunctor<T> functor){
        return _map(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    private static native <T> QVoidFuture _map(long threadPoolId, java.util.Collection<T> sequence, MapFunctor<T> functor);
    
    /**
     *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
     */
    @io.qt.QtUninvokable
    public static <T> void blockingMap(java.util.Collection<T> sequence, MapFunctor<T> functor){
        _blockingMap(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    private static native <T> void _blockingMap(long threadPoolId, java.util.Collection<T> sequence, MapFunctor<T> functor);
    
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
        @io.qt.internal.NativeAccess
        public U map(T object);
    }
    
    /**
     * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
     *
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> mapped(java.util.Collection<T> sequence, MappedFunctor<U, T> functor){
        return _mapped(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    private static native <U, T> QFuture<U> _mapped(long threadPoolId, java.util.Collection<T> sequence, MappedFunctor<U, T> functor);
    
    /**
     * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
     */
    @io.qt.QtUninvokable
    public static <U, T> java.util.List<U> blockingMapped(java.util.Collection<T> sequence, MappedFunctor<U, T> functor){
        return _blockingMapped(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    @io.qt.QtUninvokable
    private static native <U, T> java.util.List<U> _blockingMapped(long threadPoolId, java.util.Collection<T> sequence, MappedFunctor<U, T> functor);
    
    /**
     * Implement this interface in order to perform a reduce operation.
     * <p>
     * The reduce method will be called once per intermediate result (the result of the mapping of the data)
     * and the very first time the reduce() method is called for the particular data set, the result is set to
     * the returned value of the defaultResult() method.
     */
    @FunctionalInterface
    public interface ReducedFunctor<U, T> {
        @io.qt.internal.NativeAccess
        public default U defaultResult() { return null; }
    
        /**
         *  Performs a reduce operation on <code>intermediate</code>. <code>result</code> is the result of the reduction.
         */
        @io.qt.internal.NativeAccess
        public void reduce(U result, T intermediate);
    }
    
    /**
     * This is an overloaded method provided for convenience.
     * <p>
     * It is equivalent of mappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor) {
        return mappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    
    /**
     * This is an overloaded method provided for convenience.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOption ... options) {
        return mappedReduced(sequence, functor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOptions options) {
        return _mappedReduced(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    private native static <U, V, T> QFuture<U> _mappedReduced(long threadPoolId, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, int options);
    
    
    /**
     * This is an overloaded method provided for convenience.
     * <p>
     * It is equivalent of calling blockingMappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor) {
        return blockingMappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     * <p>
     * Note: This function will block until all items in the sequence have been processed.
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOption ... options) {
        return blockingMappedReduced(sequence, functor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     * <p>
     * Note: This function will block until all items in the sequence have been processed.
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOptions options) {
        return _blockingMappedReduced(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    private native static <U, V, T> U _blockingMappedReduced(long threadPoolId, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, int options);
    
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
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> QVoidFuture filter(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return _filter(0, java.util.Objects.requireNonNull(sequence), filteredFunctor);
    }
    
    @io.qt.QtUninvokable
    private native static <T> QVoidFuture _filter(long threadPoolId, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor);
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> filtered(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return _filtered(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor));
    }
    
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> _filtered(long threadPoolId, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor);
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> java.util.List<T> blockingFiltered(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return _blockingFiltered(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor));
    }

    @io.qt.QtUninvokable
    private native static <T> java.util.List<T> _blockingFiltered(long threadPoolId, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor);
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true, the item is kept in sequence; otherwise, the item is removed from sequence.
     * Note: This function will block until all items in the sequence have been processed.
     */
    @io.qt.QtUninvokable
    public static <T> void blockingFilter(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        _blockingFilter(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor));
    }
    
    @io.qt.QtUninvokable
    private native static <T> void _blockingFilter(long threadPoolId, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor);
    
    /**
     * This is an overloaded method provided for convenience. It is equivalent of calling filteredReduced(sequence, filteredFunctor, ReduceOption.UnorderedReduce, ReduceOption.Seq This is an overloaded method provided for convenience. It is equivalent of calling filteredReduced)
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor) {
        return filteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOption ... options) {
        return filteredReduced(sequence, filteredFunctor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOptions options) {
        return _filteredReduced(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }
    @io.qt.QtUninvokable
    private native static <U, T> QFuture<U> _filteredReduced(long threadPoolId, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, int options);
    
    /**
     * This is an overloaded method provided for convenience. It is the equivalent of calling blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor) {
        return blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOption ... options) {
        return blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOptions options) {
        return _blockingFilteredReduced(0, java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }
    
    @io.qt.QtUninvokable
    private native static <U, T> U _blockingFilteredReduced(long threadPoolId, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, int options);
    
    /**
     * Executes the Callable <code>callable</code> through the QtConcurrent framework. The returned QFuture object's result will be the
     * return value of <code>callable</code>.
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> run(java.util.concurrent.Callable<T> callable){
        return run0(0, java.util.Objects.requireNonNull(callable));
    }
    
    /**
     * Executes the Callable <code>callable</code> through the QtConcurrent framework. The thread is taken from the QThreadPool pool. 
     * The returned QFuture object's result will be the return value of <code>callable</code>.
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> run(QThreadPool threadPool, java.util.concurrent.Callable<T> callable){
        return run0(checkedNativeId(threadPool), java.util.Objects.requireNonNull(callable));
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> run0(long threadPoolId, java.util.concurrent.Callable<T> callable);
    
    /**
     * Executes the Runnable <code>Void</code> using the QtConcurrent framework.
     */
    @io.qt.QtUninvokable
    public static QVoidFuture run(Runnable runnable) {
        return runVoid0(0, java.util.Objects.requireNonNull(runnable));
    }
    
    /**
     * Executes the Runnable <code>Void</code> using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    @io.qt.QtUninvokable
    public static QVoidFuture run(QThreadPool threadPool, Runnable runnable) {
        return runVoid0(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable));
    }
    @io.qt.QtUninvokable
    private native static QVoidFuture runVoid0(long threadPoolId, Runnable runnable);
    
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
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A> QVoidFuture run(Runnable1<A> runnable, A a) {
        return runVoid1(0, runnable, a);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A> QVoidFuture run(QThreadPool threadPool, Runnable1<A> runnable, A a) {
        return runVoid1(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a);
    }
    private native static <A> QVoidFuture runVoid1(long threadPoolId, Runnable1<A> runnable, A a);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B> QVoidFuture run(Runnable2<A,B> runnable, A a, B b) {
        return runVoid2(0, java.util.Objects.requireNonNull(runnable), a, b);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B> QVoidFuture run(QThreadPool threadPool, Runnable2<A,B> runnable, A a, B b) {
        return runVoid2(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b);
    }
    private native static <A,B> QVoidFuture runVoid2(long threadPoolId, Runnable2<A,B> runnable, A a, B b);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C> QVoidFuture run(Runnable3<A,B,C> runnable, A a, B b, C c) {
        return runVoid3(0, java.util.Objects.requireNonNull(runnable), a, b, c);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C> QVoidFuture run(QThreadPool threadPool, Runnable3<A,B,C> runnable, A a, B b, C c) {
        return runVoid3(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c);
    }
    private native static <A,B,C> QVoidFuture runVoid3(long threadPoolId, Runnable3<A,B,C> runnable, A a, B b, C c);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C,D> QVoidFuture run(Runnable4<A,B,C,D> runnable, A a, B b, C c, D d) {
        return runVoid4(0, java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C,D> QVoidFuture run(QThreadPool threadPool, Runnable4<A,B,C,D> runnable, A a, B b, C c, D d) {
        return runVoid4(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }
    private native static <A,B,C,D> QVoidFuture runVoid4(long threadPoolId, Runnable4<A,B,C,D> runnable, A a, B b, C c, D d);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C,D,E> QVoidFuture run(Runnable5<A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return runVoid5(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C,D,E> QVoidFuture run(QThreadPool threadPool, Runnable5<A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return runVoid5(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    private native static <A,B,C,D,E> QVoidFuture runVoid5(long threadPoolId, Runnable5<A,B,C,D,E> runnable, A a, B b, C c, D d, E e);
    
    
    
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
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A> QFuture<T> run(Callable1<T,A> runnable, A a) {
        return run1(0, java.util.Objects.requireNonNull(runnable), a);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A> QFuture<T> run(QThreadPool threadPool, Callable1<T,A> runnable, A a) {
        return run1(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a);
    }
    private native static <T,A> QFuture<T> run1(long threadPoolId, Callable1<T,A> runnable, A a);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B> QFuture<T> run(Callable2<T,A,B> runnable, A a, B b) {
        return run2(0, java.util.Objects.requireNonNull(runnable), a, b);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B> QFuture<T> run(QThreadPool threadPool, Callable2<T,A,B> runnable, A a, B b) {
        return run2(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b);
    }
    private native static <T,A,B> QFuture<T> run2(long threadPoolId, Callable2<T,A,B> runnable, A a, B b);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C> QFuture<T> run(Callable3<T,A,B,C> runnable, A a, B b, C c) {
        return run3(0, java.util.Objects.requireNonNull(runnable), a, b, c);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C> QFuture<T> run(QThreadPool threadPool, Callable3<T,A,B,C> runnable, A a, B b, C c) {
        return run3(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c);
    }
    private native static <T,A,B,C> QFuture<T> run3(long threadPoolId, Callable3<T,A,B,C> runnable, A a, B b, C c);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C,D> QFuture<T> run(Callable4<T,A,B,C,D> runnable, A a, B b, C c, D d) {
        return run4(0, java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C,D> QFuture<T> run(QThreadPool threadPool, Callable4<T,A,B,C,D> runnable, A a, B b, C c, D d) {
        return run4(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }
    private native static <T,A,B,C,D> QFuture<T> run4(long threadPoolId, Callable4<T,A,B,C,D> runnable, A a, B b, C c, D d);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C,D,E> QFuture<T> run(Callable5<T,A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return run5(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C,D,E> QFuture<T> run(QThreadPool threadPool, Callable5<T,A,B,C,D,E> runnable, A a, B b, C c, D d, E e) {
        return run5(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }
    private native static <T,A,B,C,D,E> QFuture<T> run5(long threadPoolId, Callable5<T,A,B,C,D,E> runnable, A a, B b, C c, D d, E e);
    
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
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C,D,E,F> QVoidFuture run(Runnable6<A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return runVoid6(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C,D,E,F> QVoidFuture run(QThreadPool threadPool, Runnable6<A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return runVoid6(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }
    private native static <A,B,C,D,E,F> QVoidFuture runVoid6(long threadPoolId, Runnable6<A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C,D,E,F,G> QVoidFuture run(Runnable7<A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return runVoid7(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C,D,E,F,G> QVoidFuture run(QThreadPool threadPool, Runnable7<A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return runVoid7(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }
    private native static <A,B,C,D,E,F,G> QVoidFuture runVoid7(long threadPoolId, Runnable7<A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C,D,E,F,G,H> QVoidFuture run(Runnable8<A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runVoid8(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C,D,E,F,G,H> QVoidFuture run(QThreadPool threadPool, Runnable8<A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runVoid8(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }
    private native static <A,B,C,D,E,F,G,H> QVoidFuture runVoid8(long threadPoolId, Runnable8<A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A,B,C,D,E,F,G,H,I> QVoidFuture run(Runnable9<A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runVoid9(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <A,B,C,D,E,F,G,H,I> QVoidFuture run(QThreadPool threadPool, Runnable9<A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runVoid9(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    private native static <A,B,C,D,E,F,G,H,I> QVoidFuture runVoid9(long threadPoolId, Runnable9<A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C,D,E,F> QFuture<T> run(Callable6<T,A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return run6(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C,D,E,F> QFuture<T> run(QThreadPool threadPool, Callable6<T,A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f) {
        return run6(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }
    private native static <T,A,B,C,D,E,F> QFuture<T> run6(long threadPoolId, Callable6<T,A,B,C,D,E,F> runnable, A a, B b, C c, D d, E e, F f);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C,D,E,F,G> QFuture<T> run(Callable7<T,A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return run7(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C,D,E,F,G> QFuture<T> run(QThreadPool threadPool, Callable7<T,A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return run7(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }
    private native static <T,A,B,C,D,E,F,G> QFuture<T> run7(long threadPoolId, Callable7<T,A,B,C,D,E,F,G> runnable, A a, B b, C c, D d, E e, F f, G g);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C,D,E,F,G,H> QFuture<T> run(Callable8<T,A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return run8(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C,D,E,F,G,H> QFuture<T> run(QThreadPool threadPool, Callable8<T,A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return run8(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }
    private native static <T,A,B,C,D,E,F,G,H> QFuture<T> run8(long threadPoolId, Callable8<T,A,B,C,D,E,F,G,H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T,A,B,C,D,E,F,G,H,I> QFuture<T> run(Callable9<T,A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return run9(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken from the QThreadPool pool.
     */
    public static <T,A,B,C,D,E,F,G,H,I> QFuture<T> run(QThreadPool threadPool, Callable9<T,A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return run9(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    private native static <T,A,B,C,D,E,F,G,H,I> QFuture<T> run9(long threadPoolId, Callable9<T,A,B,C,D,E,F,G,H,I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

    /**
     *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
     */
    @io.qt.QtUninvokable
    public static <T> QVoidFuture map(QThreadPool threadPool, java.util.Collection<T> sequence, MapFunctor<T> functor){
        return _map(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    /**
     *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
     */
    @io.qt.QtUninvokable
    public static <T> void blockingMap(QThreadPool threadPool, java.util.Collection<T> sequence, MapFunctor<T> functor){
        _blockingMap(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    /**
     * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
     *
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> mapped(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<U, T> functor){
        return _mapped(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }
    
    /**
     * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
     */
    @io.qt.QtUninvokable
    public static <U, T> java.util.List<U> blockingMapped(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<U, T> functor){
        return _blockingMapped(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor));
    }

    /**
     * This is an overloaded method provided for convenience.
     * <p>
     * It is equivalent of mappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor) {
        return mappedReduced(threadPool, sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    
    /**
     * This is an overloaded method provided for convenience.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOption ... options) {
        return mappedReduced(threadPool, sequence, functor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> QFuture<U> mappedReduced(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOptions options) {
        return _mappedReduced(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }

    /**
     * This is an overloaded method provided for convenience.
     * <p>
     * It is equivalent of calling blockingMappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
     *
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor) {
        return blockingMappedReduced(threadPool, sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     * <p>
     * Note: This function will block until all items in the sequence have been processed.
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOption ... options) {
        return blockingMappedReduced(threadPool, sequence, functor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
     * <p>
     * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
     * <p>
     * Note: This function will block until all items in the sequence have been processed.
     */
    @io.qt.QtUninvokable
    public static <U, V, T> U blockingMappedReduced(QThreadPool threadPool, java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOptions options) {
        return _blockingMappedReduced(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(functor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> QVoidFuture filter(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return _filter(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), filteredFunctor);
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> QFuture<T> filtered(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return _filtered(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor));
    }
    
    /**
     * Calls filterFunctor's filter() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
     */
    @io.qt.QtUninvokable
    public static <T> java.util.List<T> blockingFiltered(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        return _blockingFiltered(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true, the item is kept in sequence; otherwise, the item is removed from sequence.
     * Note: This function will block until all items in the sequence have been processed.
     */
    @io.qt.QtUninvokable
    public static <T> void blockingFilter(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor){
        _blockingFilter(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor));
    }
    
    /**
     * This is an overloaded method provided for convenience. It is equivalent of calling filteredReduced(sequence, filteredFunctor, ReduceOption.UnorderedReduce, ReduceOption.Seq This is an overloaded method provided for convenience. It is equivalent of calling filteredReduced)
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOption ... options) {
        return filteredReduced(threadPool, sequence, filteredFunctor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> QFuture<U> filteredReduced(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOptions options) {
        return _filteredReduced(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
    }

    /**
     * This is an overloaded method provided for convenience. It is the equivalent of calling blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor) {
        return blockingFilteredReduced(threadPool, sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOption ... options) {
        return blockingFilteredReduced(threadPool, sequence, filteredFunctor, reducedFunctor, new ReduceOptions(options));
    }
    
    /**
     * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
     * <p>
     * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
     */
    @io.qt.QtUninvokable
    public static <U, T> U blockingFilteredReduced(QThreadPool threadPool, java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOptions options) {
        return _blockingFilteredReduced(checkedNativeId(threadPool), java.util.Objects.requireNonNull(sequence), java.util.Objects.requireNonNull(filteredFunctor), java.util.Objects.requireNonNull(reducedFunctor), options.value());
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
        public void run(QVoidPromise promise);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise1<A> {
        public void run(QVoidPromise promise, A a);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise2<A, B> {
        public void run(QVoidPromise promise, A a, B b);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise3<A, B, C> {
        public void run(QVoidPromise promise, A a, B b, C c);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise4<A, B, C, D> {
        public void run(QVoidPromise promise, A a, B b, C c, D d);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise5<A, B, C, D, E> {
        public void run(QVoidPromise promise, A a, B b, C c, D d, E e);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise6<A, B, C, D, E, F> {
        public void run(QVoidPromise promise, A a, B b, C c, D d, E e, F f);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise7<A, B, C, D, E, F, G> {
        public void run(QVoidPromise promise, A a, B b, C c, D d, E e, F f, G g);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> {
        public void run(QVoidPromise promise, A a, B b, C c, D d, E e, F f, G g, H h);
    }

    @FunctionalInterface
    public interface RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> {
        public void run(QVoidPromise promise, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T> QFuture<T> run(RunnableWithPromise<T> runnable) {
        return runWithPromise0(0, runnable);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise<T> runnable) {
        return runWithPromise0(checkedNativeId(threadPool), runnable);
    }

    private native static <T> QFuture<T> runWithPromise0(long threadPoolId, RunnableWithPromise<T> runnable);
    
    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A> QFuture<T> run(RunnableWithPromise1<T, A> runnable, A a) {
        return runWithPromise1(0, java.util.Objects.requireNonNull(runnable), a);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise1<T, A> runnable, A a) {
        return runWithPromise1(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a);
    }

    private native static <T, A> QFuture<T> runWithPromise1(long threadPoolId, RunnableWithPromise1<T, A> runnable, A a);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B> QFuture<T> run(RunnableWithPromise2<T, A, B> runnable, A a, B b) {
        return runWithPromise2(0, java.util.Objects.requireNonNull(runnable), a, b);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable, A a, B b) {
        return runWithPromise2(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b);
    }

    private native static <T, A, B> QFuture<T> runWithPromise2(long threadPoolId, RunnableWithPromise2<T, A, B> runnable, A a, B b);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B, C> QFuture<T> run(RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c) {
        return runWithPromise3(0, java.util.Objects.requireNonNull(runnable), a, b, c);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c) {
        return runWithPromise3(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c);
    }

    private native static <T, A, B, C> QFuture<T> runWithPromise3(long threadPoolId, RunnableWithPromise3<T, A, B, C> runnable, A a, B b,
            C c);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B, C, D> QFuture<T> run(RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d) {
        return runWithPromise4(0, java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C, D> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b,
            C c, D d) {
        return runWithPromise4(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }

    private native static <T, A, B, C, D> QFuture<T> runWithPromise4(long threadPoolId, RunnableWithPromise4<T, A, B, C, D> runnable,
            A a, B b, C c, D d);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B, C, D, E> QFuture<T> run(RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
        return runWithPromise5(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C, D, E> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a,
            B b, C c, D d, E e) {
        return runWithPromise5(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    private native static <T, A, B, C, D, E> QFuture<T> runWithPromise5(long threadPoolId,
            RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B, C, D, E, F> QFuture<T> run(RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e,
            F f) {
        return runWithPromise6(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C, D, E, F> QFuture<T> run(QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable,
            A a, B b, C c, D d, E e, F f) {
        return runWithPromise6(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    private native static <T, A, B, C, D, E, F> QFuture<T> runWithPromise6(long threadPoolId,
            RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);

    /**
     * Executes the RunnableWithPromise using the QtConcurrent framework.
     */
    public static <T, A, B, C, D, E, F, G> QFuture<T> run(RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c,
            D d, E e, F f, G g) {
        return runWithPromise7(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C, D, E, F, G> QFuture<T> run(QThreadPool threadPool,
            RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g) {
        return runWithPromise7(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    private native static <T, A, B, C, D, E, F, G> QFuture<T> runWithPromise7(long threadPoolId,
            RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B, C, D, E, F, G, H> QFuture<T> run(RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b,
            C c, D d, E e, F f, G g, H h) {
        return runWithPromise8(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C, D, E, F, G, H> QFuture<T> run(QThreadPool threadPool,
            RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runWithPromise8(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    private native static <T, A, B, C, D, E, F, G, H> QFuture<T> runWithPromise8(long threadPoolId,
            RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);

    /**
     * Executes the Callable using the QtConcurrent framework.
     */
    public static <T, A, B, C, D, E, F, G, H, I> QFuture<T> run(RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a,
            B b, C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromise9(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    /**
     * Executes the Callable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <T, A, B, C, D, E, F, G, H, I> QFuture<T> run(QThreadPool threadPool,
            RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromise9(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> runWithPromise9(long threadPoolId,
            RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static QVoidFuture run(RunnableWithVoidPromise runnable) {
        return runWithPromiseVoid0(0, runnable);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise runnable) {
        return runWithPromiseVoid0(checkedNativeId(threadPool), runnable);
    }

    private native static QVoidFuture runWithPromiseVoid0(long threadPoolId, RunnableWithVoidPromise runnable);
    
    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A> QVoidFuture run(RunnableWithVoidPromise1<A> runnable, A a) {
        return runWithPromiseVoid1(0, java.util.Objects.requireNonNull(runnable), a);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise1<A> runnable, A a) {
        return runWithPromiseVoid1(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a);
    }

    private native static <A> QVoidFuture runWithPromiseVoid1(long threadPoolId, RunnableWithVoidPromise1<A> runnable, A a);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B> QVoidFuture run(RunnableWithVoidPromise2<A, B> runnable, A a, B b) {
        return runWithPromiseVoid2(0, java.util.Objects.requireNonNull(runnable), a, b);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable, A a, B b) {
        return runWithPromiseVoid2(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b);
    }

    private native static <A, B> QVoidFuture runWithPromiseVoid2(long threadPoolId, RunnableWithVoidPromise2<A, B> runnable, A a, B b);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C> QVoidFuture run(RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c) {
        return runWithPromiseVoid3(0, java.util.Objects.requireNonNull(runnable), a, b, c);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c) {
        return runWithPromiseVoid3(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c);
    }

    private native static <A, B, C> QVoidFuture runWithPromiseVoid3(long threadPoolId, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b,
            C c);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C, D> QVoidFuture run(RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d) {
        return runWithPromiseVoid4(0, java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C, D> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c,
            D d) {
        return runWithPromiseVoid4(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d);
    }

    private native static <A, B, C, D> QVoidFuture runWithPromiseVoid4(long threadPoolId, RunnableWithVoidPromise4<A, B, C, D> runnable, A a,
            B b, C c, D d);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C, D, E> QVoidFuture run(RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e) {
        return runWithPromiseVoid5(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C, D, E> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b,
            C c, D d, E e) {
        return runWithPromiseVoid5(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e);
    }

    private native static <A, B, C, D, E> QVoidFuture runWithPromiseVoid5(long threadPoolId,
            RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C, D, E, F> QVoidFuture run(RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e,
            F f) {
        return runWithPromiseVoid6(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C, D, E, F> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a,
            B b, C c, D d, E e, F f) {
        return runWithPromiseVoid6(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f);
    }

    private native static <A, B, C, D, E, F> QVoidFuture runWithPromiseVoid6(long threadPoolId,
            RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C, D, E, F, G> QVoidFuture run(RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d,
            E e, F f, G g) {
        return runWithPromiseVoid7(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C, D, E, F, G> QVoidFuture run(QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable,
            A a, B b, C c, D d, E e, F f, G g) {
        return runWithPromiseVoid7(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g);
    }

    private native static <A, B, C, D, E, F, G> QVoidFuture runWithPromiseVoid7(long threadPoolId,
            RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C, D, E, F, G, H> QVoidFuture run(RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c,
            D d, E e, F f, G g, H h) {
        return runWithPromiseVoid8(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C, D, E, F, G, H> QVoidFuture run(QThreadPool threadPool,
            RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h) {
        return runWithPromiseVoid8(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h);
    }

    private native static <A, B, C, D, E, F, G, H> QVoidFuture runWithPromiseVoid8(long threadPoolId,
            RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);

    /**
     * Executes the Runnable using the QtConcurrent framework.
     */
    public static <A, B, C, D, E, F, G, H, I> QVoidFuture run(RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b,
            C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromiseVoid9(0, java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }
    
    /**
     * Executes the Runnable using the QtConcurrent framework. The thread is taken
     * from the QThreadPool pool.
     */
    public static <A, B, C, D, E, F, G, H, I> QVoidFuture run(QThreadPool threadPool,
            RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        return runWithPromiseVoid9(checkedNativeId(threadPool), java.util.Objects.requireNonNull(runnable), a, b, c, d, e, f, g, h, i);
    }

    private native static <A, B, C, D, E, F, G, H, I> QVoidFuture runWithPromiseVoid9(long threadPoolId,
            RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);

    public static abstract class QTaskBuilder{
        QTaskBuilder(int priority, QThreadPool threadPool) {
            super();
            this.priority = priority;
            this.threadPool = threadPool;
        }
        private int priority;
        private QThreadPool threadPool;
        
        final int priority() {return priority;}
        final QThreadPool threadPool() { return threadPool; }
        final long threadPoolId() { return threadPool==null ? 0 : checkedNativeId(threadPool); }
        
        public abstract void spawn(QtConcurrent.FutureResult result);
        public QTaskBuilder withPriority(int newPriority) {
            this.priority = newPriority;
            return this;
        }
        public QTaskBuilder onThreadPool(QThreadPool newThreadPool) {
            this.threadPool = java.util.Objects.requireNonNull(newThreadPool);
            return this;
        }
    }
    
    public static abstract class QTaskBuilderVoid extends QTaskBuilder{
        QTaskBuilderVoid(int priority, QThreadPool threadPool) {
            super(priority, threadPool);
        }
        public QVoidFuture spawn() {
            throw new RuntimeException("Unable to spawn due to missing arguments.");
        }
        public final void spawn(QtConcurrent.FutureResult result) {
            spawn();
        }
    }
    
    public static abstract class QTypedTaskBuilder<T> extends QTaskBuilder{
        public QTypedTaskBuilder(int priority, QThreadPool threadPool) {
            super(priority, threadPool);
        }
        public QFuture<T> spawn() {
            throw new RuntimeException("Unable to spawn due to missing arguments.");
        }
        public final void spawn(QtConcurrent.FutureResult result) {
            spawn();
        }
    }
    
    public static QTaskBuilderVoid0Arg0 task(Runnable runnable) {
        return new QTaskBuilderVoid0Arg0(0, null, runnable);
    }
    
    public static class QTaskBuilderVoid0Arg0 extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable);
        }
        
        private native static QVoidFuture spawn(long threadPoolId, int priority, Runnable runnable);
    }
    
    public static <A> QTaskBuilderVoid1Arg0<A> task(Runnable1<A> runnable) {
        return new QTaskBuilderVoid1Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid1Arg0<A> extends QTaskBuilderVoid{
        private final Runnable1<A> runnable;
        
        QTaskBuilderVoid1Arg0(int priority, QThreadPool threadPool, Runnable1<A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
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
    
    public static class QTaskBuilderVoid1Arg1<A> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a);
        }
        
        private native static <A> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable1<A> runnable, A a);
    }
    
    public static <A, B> QTaskBuilderVoid2Arg0<A, B> task(Runnable2<A, B> runnable) {
        return new QTaskBuilderVoid2Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid2Arg0<A, B> extends QTaskBuilderVoid{
        private final Runnable2<A, B> runnable;
        
        QTaskBuilderVoid2Arg0(int priority, QThreadPool threadPool, Runnable2<A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid2Arg1<A, B> withArguments(A a) {
            return new QTaskBuilderVoid2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
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
    
    public static class QTaskBuilderVoid2Arg1<A, B> extends QTaskBuilderVoid{
        QTaskBuilderVoid2Arg1(int newPriority, QThreadPool threadPool, Runnable2<A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable2<A, B> runnable;
        private final A a;
        
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
    
    public static final class QTaskBuilderVoid2Arg2<A, B> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b);
        }
        
        private native static <A, B> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable2<A, B> runnable, A a, B b);
    }
    
    public static <A, B, C> QTaskBuilderVoid3Arg0<A, B, C> task(Runnable3<A, B, C> runnable) {
        return new QTaskBuilderVoid3Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid3Arg0<A, B, C> extends QTaskBuilderVoid{
        private final Runnable3<A, B, C> runnable;
        
        QTaskBuilderVoid3Arg0(int priority, QThreadPool threadPool, Runnable3<A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid3Arg1<A, B, C> withArguments(A a) {
            return new QTaskBuilderVoid3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid3Arg2<A, B, C> withArguments(A a, B b) {
            return new QTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
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
    
    public static class QTaskBuilderVoid3Arg1<A, B, C> extends QTaskBuilderVoid{
        QTaskBuilderVoid3Arg1(int newPriority, QThreadPool threadPool, Runnable3<A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable3<A, B, C> runnable;
        private final A a;
        
        public QTaskBuilderVoid3Arg2<A, B, C> withArguments(B b) {
            return new QTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
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
    
    public static final class QTaskBuilderVoid3Arg2<A, B, C> extends QTaskBuilderVoid{
        QTaskBuilderVoid3Arg2(int newPriority, QThreadPool threadPool, Runnable3<A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable3<A, B, C> runnable;
        private final A a;
        private final B b;
        
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
    
    public static final class QTaskBuilderVoid3Arg3<A, B, C> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c);
        }
        
        private native static <A, B, C> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable3<A, B, C> runnable, A a, B b, C c);
    }
    
    public static <A, B, C, D> QTaskBuilderVoid4Arg0<A, B, C, D> task(Runnable4<A, B, C, D> runnable) {
        return new QTaskBuilderVoid4Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid4Arg0<A, B, C, D> extends QTaskBuilderVoid{
        private final Runnable4<A, B, C, D> runnable;
        
        QTaskBuilderVoid4Arg0(int priority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid4Arg1<A, B, C, D> withArguments(A a) {
            return new QTaskBuilderVoid4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid4Arg2<A, B, C, D> withArguments(A a, B b) {
            return new QTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static class QTaskBuilderVoid4Arg1<A, B, C, D> extends QTaskBuilderVoid{
        QTaskBuilderVoid4Arg1(int newPriority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable4<A, B, C, D> runnable;
        private final A a;
        
        public QTaskBuilderVoid4Arg2<A, B, C, D> withArguments(B b) {
            return new QTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(B b, C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static final class QTaskBuilderVoid4Arg2<A, B, C, D> extends QTaskBuilderVoid{
        QTaskBuilderVoid4Arg2(int newPriority, QThreadPool threadPool, Runnable4<A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        public QTaskBuilderVoid4Arg3<A, B, C, D> withArguments(C c) {
            return new QTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static final class QTaskBuilderVoid4Arg3<A, B, C, D> extends QTaskBuilderVoid{
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
    
    public static final class QTaskBuilderVoid4Arg4<A, B, C, D> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d);
        }
        
        private native static <A, B, C, D> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable4<A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    public static <A, B, C, D, E> QTaskBuilderVoid5Arg0<A, B, C, D, E> task(Runnable5<A, B, C, D, E> runnable) {
        return new QTaskBuilderVoid5Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid5Arg0<A, B, C, D, E> extends QTaskBuilderVoid{
        private final Runnable5<A, B, C, D, E> runnable;
        
        QTaskBuilderVoid5Arg0(int priority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid5Arg1<A, B, C, D, E> withArguments(A a) {
            return new QTaskBuilderVoid5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(A a, B b) {
            return new QTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTaskBuilderVoid5Arg1<A, B, C, D, E> extends QTaskBuilderVoid{
        QTaskBuilderVoid5Arg1(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        
        public QTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(B b) {
            return new QTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(B b, C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTaskBuilderVoid5Arg2<A, B, C, D, E> extends QTaskBuilderVoid{
        QTaskBuilderVoid5Arg2(int newPriority, QThreadPool threadPool, Runnable5<A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        public QTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(C c) {
            return new QTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(C c, D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTaskBuilderVoid5Arg3<A, B, C, D, E> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(D d) {
            return new QTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTaskBuilderVoid5Arg4<A, B, C, D, E> extends QTaskBuilderVoid{
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
    
    public static final class QTaskBuilderVoid5Arg5<A, B, C, D, E> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <A, B, C, D, E> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    public static <A, B, C, D, E, F> QTaskBuilderVoid6Arg0<A, B, C, D, E, F> task(Runnable6<A, B, C, D, E, F> runnable) {
        return new QTaskBuilderVoid6Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid6Arg0<A, B, C, D, E, F> extends QTaskBuilderVoid{
        private final Runnable6<A, B, C, D, E, F> runnable;
        
        QTaskBuilderVoid6Arg0(int priority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid6Arg1<A, B, C, D, E, F> withArguments(A a) {
            return new QTaskBuilderVoid6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTaskBuilderVoid6Arg1<A, B, C, D, E, F> extends QTaskBuilderVoid{
        QTaskBuilderVoid6Arg1(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        
        public QTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(B b) {
            return new QTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTaskBuilderVoid6Arg2<A, B, C, D, E, F> extends QTaskBuilderVoid{
        QTaskBuilderVoid6Arg2(int newPriority, QThreadPool threadPool, Runnable6<A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        public QTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(C c) {
            return new QTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTaskBuilderVoid6Arg3<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(D d) {
            return new QTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTaskBuilderVoid6Arg4<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(E e) {
            return new QTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTaskBuilderVoid6Arg5<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
    
    public static final class QTaskBuilderVoid6Arg6<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <A, B, C, D, E, F> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    public static <A, B, C, D, E, F, G> QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> task(Runnable7<A, B, C, D, E, F, G> runnable) {
        return new QTaskBuilderVoid7Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        
        QTaskBuilderVoid7Arg0(int priority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withArguments(A a) {
            return new QTaskBuilderVoid7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
        QTaskBuilderVoid7Arg1(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        
        public QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(B b) {
            return new QTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
        QTaskBuilderVoid7Arg2(int newPriority, QThreadPool threadPool, Runnable7<A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        public QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(C c) {
            return new QTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(D d) {
            return new QTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(E e) {
            return new QTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(F f) {
            return new QTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
    
    public static final class QTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <A, B, C, D, E, F, G> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }

    public static <A, B, C, D, E, F, G, H> QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> task(Runnable8<A, B, C, D, E, F, G, H> runnable) {
        return new QTaskBuilderVoid8Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        
        QTaskBuilderVoid8Arg0(int priority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTaskBuilderVoid8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
        QTaskBuilderVoid8Arg1(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        public QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
        QTaskBuilderVoid8Arg2(int newPriority, QThreadPool threadPool, Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        public QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
    
    public static final class QTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <A, B, C, D, E, F, G, H> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    public static <A, B, C, D, E, F, G, H, I> QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> task(Runnable9<A, B, C, D, E, F, G, H, I> runnable) {
        return new QTaskBuilderVoid9Arg0<>(0, null, runnable);
    }
    
    public static final class QTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        
        QTaskBuilderVoid9Arg0(int priority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTaskBuilderVoid9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
        QTaskBuilderVoid9Arg1(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        public QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
        QTaskBuilderVoid9Arg2(int newPriority, QThreadPool threadPool, Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final Runnable9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        public QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
    
    public static final class QTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g, h, i);
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
        private native static <A, B, C, D, E, F, G, H, I> QVoidFuture spawn(long threadPoolId, int priority,
                Runnable9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    public static <T> QTypedTaskBuilder0Arg0<T> task(java.util.concurrent.Callable<T> callable) {
        return new QTypedTaskBuilder0Arg0<>(0, null, callable);
    }
    
    public static class QTypedTaskBuilder0Arg0<T> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable);
        }
        
        private native static <T> QFuture<T> spawn(long threadPoolId, int priority, java.util.concurrent.Callable<T> callable);
    }
    
    public static <T, A> QTypedTaskBuilder1Arg0<T, A> task(Callable1<T, A> callable) {
        return new QTypedTaskBuilder1Arg0<>(0, null, callable);
    }
    
    public static final class QTypedTaskBuilder1Arg0<T, A> extends QTypedTaskBuilder<T>{
        private final Callable1<T, A> callable;
        
        QTypedTaskBuilder1Arg0(int priority, QThreadPool threadPool, Callable1<T, A> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
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
    
    public static class QTypedTaskBuilder1Arg1<T, A> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a);
        }
        
        private native static <T, A> QFuture<T> spawn(long threadPoolId, int priority,
                Callable1<T, A> callable, A a);
    }
    
    public static <T, A, B> QTypedTaskBuilder2Arg0<T, A, B> task(Callable2<T, A, B> callable) {
        return new QTypedTaskBuilder2Arg0<>(0, null, callable);
    }
    
    public static final class QTypedTaskBuilder2Arg0<T, A, B> extends QTypedTaskBuilder<T>{
        private final Callable2<T, A, B> callable;
        
        QTypedTaskBuilder2Arg0(int priority, QThreadPool threadPool, Callable2<T, A, B> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder2Arg1<T, A, B> withArguments(A a) {
            return new QTypedTaskBuilder2Arg1<>(priority(), threadPool(), callable, a);
        }
        
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
    
    public static class QTypedTaskBuilder2Arg1<T, A, B> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder2Arg1(int newPriority, QThreadPool threadPool, Callable2<T, A, B> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable2<T, A, B> callable;
        private final A a;
        
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
    
    public static final class QTypedTaskBuilder2Arg2<T, A, B> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b);
        }
        
        private native static <T, A, B> QFuture<T> spawn(long threadPoolId, int priority,
                Callable2<T, A, B> callable, A a, B b);
    }
    
    public static <T, A, B, C> QTypedTaskBuilder3Arg0<T, A, B, C> task(Callable3<T, A, B, C> callable) {
        return new QTypedTaskBuilder3Arg0<>(0, null, callable);
    }
    
    public static final class QTypedTaskBuilder3Arg0<T, A, B, C> extends QTypedTaskBuilder<T>{
        private final Callable3<T, A, B, C> callable;
        
        QTypedTaskBuilder3Arg0(int priority, QThreadPool threadPool, Callable3<T, A, B, C> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder3Arg1<T, A, B, C> withArguments(A a) {
            return new QTypedTaskBuilder3Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder3Arg2<T, A, B, C> withArguments(A a, B b) {
            return new QTypedTaskBuilder3Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
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
    
    public static class QTypedTaskBuilder3Arg1<T, A, B, C> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder3Arg1(int newPriority, QThreadPool threadPool, Callable3<T, A, B, C> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable3<T, A, B, C> callable;
        private final A a;
        
        public QTypedTaskBuilder3Arg2<T, A, B, C> withArguments(B b) {
            return new QTypedTaskBuilder3Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
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
    
    public static final class QTypedTaskBuilder3Arg2<T, A, B, C> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder3Arg2(int newPriority, QThreadPool threadPool, Callable3<T, A, B, C> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable3<T, A, B, C> callable;
        private final A a;
        private final B b;
        
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
    
    public static final class QTypedTaskBuilder3Arg3<T, A, B, C> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c);
        }
        
        private native static <T, A, B, C> QFuture<T> spawn(long threadPoolId, int priority,
                Callable3<T, A, B, C> callable, A a, B b, C c);
    }
    
    public static <T, A, B, C, D> QTypedTaskBuilder4Arg0<T, A, B, C, D> task(Callable4<T, A, B, C, D> callable) {
        return new QTypedTaskBuilder4Arg0<>(0, null, callable);
    }
    
    public static final class QTypedTaskBuilder4Arg0<T, A, B, C, D> extends QTypedTaskBuilder<T>{
        private final Callable4<T, A, B, C, D> callable;
        
        QTypedTaskBuilder4Arg0(int priority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder4Arg1<T, A, B, C, D> withArguments(A a) {
            return new QTypedTaskBuilder4Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder4Arg2<T, A, B, C, D> withArguments(A a, B b) {
            return new QTypedTaskBuilder4Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
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
    
    public static class QTypedTaskBuilder4Arg1<T, A, B, C, D> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder4Arg1(int newPriority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable4<T, A, B, C, D> callable;
        private final A a;
        
        public QTypedTaskBuilder4Arg2<T, A, B, C, D> withArguments(B b) {
            return new QTypedTaskBuilder4Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(B b, C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
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
    
    public static final class QTypedTaskBuilder4Arg2<T, A, B, C, D> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder4Arg2(int newPriority, QThreadPool threadPool, Callable4<T, A, B, C, D> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable4<T, A, B, C, D> callable;
        private final A a;
        private final B b;
        
        public QTypedTaskBuilder4Arg3<T, A, B, C, D> withArguments(C c) {
            return new QTypedTaskBuilder4Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
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
    
    public static final class QTypedTaskBuilder4Arg3<T, A, B, C, D> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedTaskBuilder4Arg4<T, A, B, C, D> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c, d);
        }
        
        private native static <T, A, B, C, D> QFuture<T> spawn(long threadPoolId, int priority,
                Callable4<T, A, B, C, D> callable, A a, B b, C c, D d);
    }
    
    public static <T, A, B, C, D, E> QTypedTaskBuilder5Arg0<T, A, B, C, D, E> task(Callable5<T, A, B, C, D, E> callable) {
        return new QTypedTaskBuilder5Arg0<>(0, null, callable);
    }
    
    public static final class QTypedTaskBuilder5Arg0<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
        private final Callable5<T, A, B, C, D, E> callable;
        
        QTypedTaskBuilder5Arg0(int priority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder5Arg1<T, A, B, C, D, E> withArguments(A a) {
            return new QTypedTaskBuilder5Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(A a, B b) {
            return new QTypedTaskBuilder5Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
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
    
    public static class QTypedTaskBuilder5Arg1<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder5Arg1(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        
        public QTypedTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(B b) {
            return new QTypedTaskBuilder5Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(B b, C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
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
    
    public static final class QTypedTaskBuilder5Arg2<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder5Arg2(int newPriority, QThreadPool threadPool, Callable5<T, A, B, C, D, E> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable5<T, A, B, C, D, E> callable;
        private final A a;
        private final B b;
        
        public QTypedTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(C c) {
            return new QTypedTaskBuilder5Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(C c, D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
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
    
    public static final class QTypedTaskBuilder5Arg3<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(D d) {
            return new QTypedTaskBuilder5Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
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
    
    public static final class QTypedTaskBuilder5Arg4<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedTaskBuilder5Arg5<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c, d, e);
        }
        
        private native static <T, A, B, C, D, E> QFuture<T> spawn(long threadPoolId, int priority,
                Callable5<T, A, B, C, D, E> callable, A a, B b, C c, D d, E e);
    }
    
    public static <T, A, B, C, D, E, F> QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> task(Callable6<T, A, B, C, D, E, F> callable) {
        return new QTypedTaskBuilder6Arg0<>(0, null, callable);
    }
    
    public static class QTypedTaskBuilder6Arg0<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
        private final Callable6<T, A, B, C, D, E, F> callable;
        
        QTypedTaskBuilder6Arg0(int priority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> withArguments(A a) {
            return new QTypedTaskBuilder6Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTypedTaskBuilder6Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
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
    
    public static class QTypedTaskBuilder6Arg1<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder6Arg1(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        
        public QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(B b) {
            return new QTypedTaskBuilder6Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedTaskBuilder6Arg2<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder6Arg2(int newPriority, QThreadPool threadPool, Callable6<T, A, B, C, D, E, F> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable6<T, A, B, C, D, E, F> callable;
        private final A a;
        private final B b;
        
        public QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(C c) {
            return new QTypedTaskBuilder6Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedTaskBuilder6Arg3<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(D d) {
            return new QTypedTaskBuilder6Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedTaskBuilder6Arg4<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(E e) {
            return new QTypedTaskBuilder6Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedTaskBuilder6Arg5<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedTaskBuilder6Arg6<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c, d, e, f);
        }
        
        private native static <T, A, B, C, D, E, F> QFuture<T> spawn(long threadPoolId, int priority,
                Callable6<T, A, B, C, D, E, F> callable, A a, B b, C c, D d, E e, F f);
    }
    
    public static <T, A, B, C, D, E, F, G> QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> task(Callable7<T, A, B, C, D, E, F, G> callable) {
        return new QTypedTaskBuilder7Arg0<>(0, null, callable);
    }
    
    public static class QTypedTaskBuilder7Arg0<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        
        QTypedTaskBuilder7Arg0(int priority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withArguments(A a) {
            return new QTypedTaskBuilder7Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTypedTaskBuilder7Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
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
    
    public static class QTypedTaskBuilder7Arg1<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder7Arg1(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        
        public QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(B b) {
            return new QTypedTaskBuilder7Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedTaskBuilder7Arg2<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder7Arg2(int newPriority, QThreadPool threadPool, Callable7<T, A, B, C, D, E, F, G> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable7<T, A, B, C, D, E, F, G> callable;
        private final A a;
        private final B b;
        
        public QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(C c) {
            return new QTypedTaskBuilder7Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedTaskBuilder7Arg3<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(D d) {
            return new QTypedTaskBuilder7Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedTaskBuilder7Arg4<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(E e) {
            return new QTypedTaskBuilder7Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedTaskBuilder7Arg5<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(F f) {
            return new QTypedTaskBuilder7Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedTaskBuilder7Arg6<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedTaskBuilder7Arg7<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c, d, e, f, g);
        }
        
        private native static <T, A, B, C, D, E, F, G> QFuture<T> spawn(long threadPoolId, int priority,
                Callable7<T, A, B, C, D, E, F, G> callable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    public static <T, A, B, C, D, E, F, G, H> QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> task(Callable8<T, A, B, C, D, E, F, G, H> callable) {
        return new QTypedTaskBuilder8Arg0<>(0, null, callable);
    }
    
    public static class QTypedTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        
        QTypedTaskBuilder8Arg0(int priority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTypedTaskBuilder8Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTypedTaskBuilder8Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static class QTypedTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder8Arg1(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        
        public QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTypedTaskBuilder8Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder8Arg2(int newPriority, QThreadPool threadPool, Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable8<T, A, B, C, D, E, F, G, H> callable;
        private final A a;
        private final B b;
        
        public QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTypedTaskBuilder8Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTypedTaskBuilder8Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTypedTaskBuilder8Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTypedTaskBuilder8Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTypedTaskBuilder8Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c, d, e, f, g, h);
        }
        
        private native static <T, A, B, C, D, E, F, G, H> QFuture<T> spawn(long threadPoolId, int priority,
                Callable8<T, A, B, C, D, E, F, G, H> callable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    public static <T, A, B, C, D, E, F, G, H, I> QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> task(Callable9<T, A, B, C, D, E, F, G, H, I> callable) {
        return new QTypedTaskBuilder9Arg0<>(0, null, callable);
    }
    
    public static class QTypedTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        
        QTypedTaskBuilder9Arg0(int priority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable) {
            super(priority, threadPool);
            this.callable = callable;
        }
        
        public QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTypedTaskBuilder9Arg1<>(priority(), threadPool(), callable, a);
        }
        
        public QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTypedTaskBuilder9Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static class QTypedTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder9Arg1(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        
        public QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTypedTaskBuilder9Arg2<>(priority(), threadPool(), callable, a, b);
        }
        
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
        QTypedTaskBuilder9Arg2(int newPriority, QThreadPool threadPool, Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b) {
            super(newPriority, threadPool);
            this.callable = callable;
            this.a = a;
            this.b = b;
        }
        private final Callable9<T, A, B, C, D, E, F, G, H, I> callable;
        private final A a;
        private final B b;
        
        public QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTypedTaskBuilder9Arg3<>(priority(), threadPool(), callable, a, b, c);
        }
        
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTypedTaskBuilder9Arg4<>(priority(), threadPool(), callable, a, b, c, d);
        }
        
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTypedTaskBuilder9Arg5<>(priority(), threadPool(), callable, a, b, c, d, e);
        }
        
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTypedTaskBuilder9Arg6<>(priority(), threadPool(), callable, a, b, c, d, e, f);
        }
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTypedTaskBuilder9Arg7<>(priority(), threadPool(), callable, a, b, c, d, e, f, g);
        }
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QTypedTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTypedTaskBuilder9Arg8<>(priority(), threadPool(), callable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QTypedTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), callable, a, b, c, d, e, f, g, h, i);
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
        private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> spawn(long threadPoolId, int priority,
                Callable9<T, A, B, C, D, E, F, G, H, I> callable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
        
    public static QPromiseTaskBuilderVoid0Arg0 task(RunnableWithVoidPromise runnable) {
        return new QPromiseTaskBuilderVoid0Arg0(0, null, runnable);
    }
    
    public static class QPromiseTaskBuilderVoid0Arg0 extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable);
        }
        
        private native static QVoidFuture spawn(long threadPoolId, int priority, RunnableWithVoidPromise runnable);
    }
    
    public static <A> QPromiseTaskBuilderVoid1Arg0<A> task(RunnableWithVoidPromise1<A> runnable) {
        return new QPromiseTaskBuilderVoid1Arg0<>(0, null, runnable);
    }
    
    public static final class QPromiseTaskBuilderVoid1Arg0<A> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise1<A> runnable;
        
        QPromiseTaskBuilderVoid1Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise1<A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
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
    
    public static class QPromiseTaskBuilderVoid1Arg1<A> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a);
        }
        
        private native static <A> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise1<A> runnable, A a);
    }
    
    public static <A, B> QPromiseTaskBuilderVoid2Arg0<A, B> task(RunnableWithVoidPromise2<A, B> runnable) {
        return new QPromiseTaskBuilderVoid2Arg0<>(0, null, runnable);
    }
    
    public static final class QPromiseTaskBuilderVoid2Arg0<A, B> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise2<A, B> runnable;
        
        QPromiseTaskBuilderVoid2Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid2Arg1<A, B> withArguments(A a) {
            return new QPromiseTaskBuilderVoid2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid2Arg1<A, B> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid2Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise2<A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise2<A, B> runnable;
        private final A a;
        
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
    
    public static final class QPromiseTaskBuilderVoid2Arg2<A, B> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b);
        }
        
        private native static <A, B> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise2<A, B> runnable, A a, B b);
    }
    
    public static <A, B, C> QPromiseTaskBuilderVoid3Arg0<A, B, C> task(RunnableWithVoidPromise3<A, B, C> runnable) {
        return new QPromiseTaskBuilderVoid3Arg0<>(0, null, runnable);
    }
    
    public static final class QPromiseTaskBuilderVoid3Arg0<A, B, C> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        
        QPromiseTaskBuilderVoid3Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid3Arg1<A, B, C> withArguments(A a) {
            return new QPromiseTaskBuilderVoid3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid3Arg1<A, B, C> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid3Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid3Arg2<A, B, C> withArguments(B b) {
            return new QPromiseTaskBuilderVoid3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid3Arg2<A, B, C> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid3Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise3<A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise3<A, B, C> runnable;
        private final A a;
        private final B b;
        
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
    
    public static final class QPromiseTaskBuilderVoid3Arg3<A, B, C> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c);
        }
        
        private native static <A, B, C> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise3<A, B, C> runnable, A a, B b, C c);
    }
    
    public static <A, B, C, D> QPromiseTaskBuilderVoid4Arg0<A, B, C, D> task(RunnableWithVoidPromise4<A, B, C, D> runnable) {
        return new QPromiseTaskBuilderVoid4Arg0<>(0, null, runnable);
    }
    
    public static final class QPromiseTaskBuilderVoid4Arg0<A, B, C, D> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        
        QPromiseTaskBuilderVoid4Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid4Arg1<A, B, C, D> withArguments(A a) {
            return new QPromiseTaskBuilderVoid4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid4Arg1<A, B, C, D> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid4Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid4Arg2<A, B, C, D> withArguments(B b) {
            return new QPromiseTaskBuilderVoid4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid4Arg2<A, B, C, D> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid4Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise4<A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        public QPromiseTaskBuilderVoid4Arg3<A, B, C, D> withArguments(C c) {
            return new QPromiseTaskBuilderVoid4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid4Arg3<A, B, C, D> extends QTaskBuilderVoid{
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
    
    public static final class QPromiseTaskBuilderVoid4Arg4<A, B, C, D> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d);
        }
        
        private native static <A, B, C, D> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise4<A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    public static <A, B, C, D, E> QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> task(RunnableWithVoidPromise5<A, B, C, D, E> runnable) {
        return new QPromiseTaskBuilderVoid5Arg0<>(0, null, runnable);
    }
    
    public static final class QPromiseTaskBuilderVoid5Arg0<A, B, C, D, E> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        
        QPromiseTaskBuilderVoid5Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> withArguments(A a) {
            return new QPromiseTaskBuilderVoid5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid5Arg1<A, B, C, D, E> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid5Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> withArguments(B b) {
            return new QPromiseTaskBuilderVoid5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid5Arg2<A, B, C, D, E> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid5Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise5<A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        public QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> withArguments(C c) {
            return new QPromiseTaskBuilderVoid5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid5Arg3<A, B, C, D, E> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> withArguments(D d) {
            return new QPromiseTaskBuilderVoid5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid5Arg4<A, B, C, D, E> extends QTaskBuilderVoid{
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
    
    public static final class QPromiseTaskBuilderVoid5Arg5<A, B, C, D, E> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <A, B, C, D, E> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise5<A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    public static <A, B, C, D, E, F> QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> task(RunnableWithVoidPromise6<A, B, C, D, E, F> runnable) {
        return new QPromiseTaskBuilderVoid6Arg0<>(0, null, runnable);
    }
    
    public static class QPromiseTaskBuilderVoid6Arg0<A, B, C, D, E, F> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        
        QPromiseTaskBuilderVoid6Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> withArguments(A a) {
            return new QPromiseTaskBuilderVoid6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid6Arg1<A, B, C, D, E, F> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid6Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> withArguments(B b) {
            return new QPromiseTaskBuilderVoid6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid6Arg2<A, B, C, D, E, F> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid6Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise6<A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        public QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> withArguments(C c) {
            return new QPromiseTaskBuilderVoid6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid6Arg3<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> withArguments(D d) {
            return new QPromiseTaskBuilderVoid6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid6Arg4<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> withArguments(E e) {
            return new QPromiseTaskBuilderVoid6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid6Arg5<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
    
    public static final class QPromiseTaskBuilderVoid6Arg6<A, B, C, D, E, F> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <A, B, C, D, E, F> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise6<A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    public static <A, B, C, D, E, F, G> QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> task(RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable) {
        return new QPromiseTaskBuilderVoid7Arg0<>(0, null, runnable);
    }
    
    public static class QPromiseTaskBuilderVoid7Arg0<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        
        QPromiseTaskBuilderVoid7Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> withArguments(A a) {
            return new QPromiseTaskBuilderVoid7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid7Arg1<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid7Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> withArguments(B b) {
            return new QPromiseTaskBuilderVoid7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid7Arg2<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid7Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        public QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> withArguments(C c) {
            return new QPromiseTaskBuilderVoid7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid7Arg3<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> withArguments(D d) {
            return new QPromiseTaskBuilderVoid7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid7Arg4<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> withArguments(E e) {
            return new QPromiseTaskBuilderVoid7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid7Arg5<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> withArguments(F f) {
            return new QPromiseTaskBuilderVoid7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid7Arg6<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
    
    public static final class QPromiseTaskBuilderVoid7Arg7<A, B, C, D, E, F, G> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <A, B, C, D, E, F, G> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise7<A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    public static <A, B, C, D, E, F, G, H> QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> task(RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable) {
        return new QPromiseTaskBuilderVoid8Arg0<>(0, null, runnable);
    }
    
    public static class QPromiseTaskBuilderVoid8Arg0<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        
        QPromiseTaskBuilderVoid8Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QPromiseTaskBuilderVoid8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static class QPromiseTaskBuilderVoid8Arg1<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid8Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QPromiseTaskBuilderVoid8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid8Arg2<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid8Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        public QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QPromiseTaskBuilderVoid8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid8Arg3<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QPromiseTaskBuilderVoid8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid8Arg4<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QPromiseTaskBuilderVoid8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid8Arg5<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QPromiseTaskBuilderVoid8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid8Arg6<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QPromiseTaskBuilderVoid8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid8Arg7<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
    
    public static final class QPromiseTaskBuilderVoid8Arg8<A, B, C, D, E, F, G, H> extends QTaskBuilderVoid{
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
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <A, B, C, D, E, F, G, H> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise8<A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    public static <A, B, C, D, E, F, G, H, I> QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> task(RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable) {
        return new QPromiseTaskBuilderVoid9Arg0<>(0, null, runnable);
    }
    
    public static class QPromiseTaskBuilderVoid9Arg0<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        
        QPromiseTaskBuilderVoid9Arg0(int priority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QPromiseTaskBuilderVoid9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QPromiseTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static class QPromiseTaskBuilderVoid9Arg1<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid9Arg1(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        public QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QPromiseTaskBuilderVoid9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QPromiseTaskBuilderVoid9Arg2<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
        QPromiseTaskBuilderVoid9Arg2(int newPriority, QThreadPool threadPool, RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        public QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QPromiseTaskBuilderVoid9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QPromiseTaskBuilderVoid9Arg3<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QPromiseTaskBuilderVoid9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QPromiseTaskBuilderVoid9Arg4<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QPromiseTaskBuilderVoid9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QPromiseTaskBuilderVoid9Arg5<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QPromiseTaskBuilderVoid9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QPromiseTaskBuilderVoid9Arg6<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QPromiseTaskBuilderVoid9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid9Arg7<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        public QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QPromiseTaskBuilderVoid9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QPromiseTaskBuilderVoid9Arg8<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
    
    public static final class QPromiseTaskBuilderVoid9Arg9<A, B, C, D, E, F, G, H, I> extends QTaskBuilderVoid{
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
        
        @Override
        public QVoidFuture spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g, h, i);
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
        private native static <A, B, C, D, E, F, G, H, I> QVoidFuture spawn(long threadPoolId, int priority,
                RunnableWithVoidPromise9<A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    public static <T> QTypedPromiseTaskBuilder0Arg0<T> task(RunnableWithPromise<T> callable) {
        return new QTypedPromiseTaskBuilder0Arg0<>(0, null, callable);
    }
    
    public static class QTypedPromiseTaskBuilder0Arg0<T> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable);
        }
        
        private native static <T> QFuture<T> spawn(long threadPoolId, int priority, RunnableWithPromise<T> runnable);
    }
    
    public static <T, A> QTypedPromiseTaskBuilder1Arg0<T, A> task(RunnableWithPromise1<T, A> runnable) {
        return new QTypedPromiseTaskBuilder1Arg0<>(0, null, runnable);
    }
    
    public static final class QTypedPromiseTaskBuilder1Arg0<T, A> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise1<T, A> runnable;
        
        QTypedPromiseTaskBuilder1Arg0(int priority, QThreadPool threadPool, RunnableWithPromise1<T, A> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
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
    
    public static class QTypedPromiseTaskBuilder1Arg1<T, A> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a);
        }
        
        private native static <T, A> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise1<T, A> runnable, A a);
    }
    
    public static <T, A, B> QTypedPromiseTaskBuilder2Arg0<T, A, B> task(RunnableWithPromise2<T, A, B> runnable) {
        return new QTypedPromiseTaskBuilder2Arg0<>(0, null, runnable);
    }
    
    public static final class QTypedPromiseTaskBuilder2Arg0<T, A, B> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise2<T, A, B> runnable;
        
        QTypedPromiseTaskBuilder2Arg0(int priority, QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder2Arg1<T, A, B> withArguments(A a) {
            return new QTypedPromiseTaskBuilder2Arg1<>(priority(), threadPool(), runnable, a);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder2Arg1<T, A, B> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder2Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise2<T, A, B> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise2<T, A, B> runnable;
        private final A a;
        
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
    
    public static final class QTypedPromiseTaskBuilder2Arg2<T, A, B> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b);
        }
        
        private native static <T, A, B> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise2<T, A, B> runnable, A a, B b);
    }
    
    public static <T, A, B, C> QTypedPromiseTaskBuilder3Arg0<T, A, B, C> task(RunnableWithPromise3<T, A, B, C> runnable) {
        return new QTypedPromiseTaskBuilder3Arg0<>(0, null, runnable);
    }
    
    public static final class QTypedPromiseTaskBuilder3Arg0<T, A, B, C> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise3<T, A, B, C> runnable;
        
        QTypedPromiseTaskBuilder3Arg0(int priority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder3Arg1<T, A, B, C> withArguments(A a) {
            return new QTypedPromiseTaskBuilder3Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder3Arg1<T, A, B, C> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder3Arg2<T, A, B, C> withArguments(B b) {
            return new QTypedPromiseTaskBuilder3Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder3Arg2<T, A, B, C> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder3Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise3<T, A, B, C> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise3<T, A, B, C> runnable;
        private final A a;
        private final B b;
        
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
    
    public static final class QTypedPromiseTaskBuilder3Arg3<T, A, B, C> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c);
        }
        
        private native static <T, A, B, C> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise3<T, A, B, C> runnable, A a, B b, C c);
    }
    
    public static <T, A, B, C, D> QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> task(RunnableWithPromise4<T, A, B, C, D> runnable) {
        return new QTypedPromiseTaskBuilder4Arg0<>(0, null, runnable);
    }
    
    public static final class QTypedPromiseTaskBuilder4Arg0<T, A, B, C, D> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        
        QTypedPromiseTaskBuilder4Arg0(int priority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> withArguments(A a) {
            return new QTypedPromiseTaskBuilder4Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder4Arg1<T, A, B, C, D> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> withArguments(B b) {
            return new QTypedPromiseTaskBuilder4Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder4Arg2<T, A, B, C, D> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder4Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise4<T, A, B, C, D> runnable;
        private final A a;
        private final B b;
        
        public QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> withArguments(C c) {
            return new QTypedPromiseTaskBuilder4Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder4Arg3<T, A, B, C, D> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedPromiseTaskBuilder4Arg4<T, A, B, C, D> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d);
        }
        
        private native static <T, A, B, C, D> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise4<T, A, B, C, D> runnable, A a, B b, C c, D d);
    }
    
    public static <T, A, B, C, D, E> QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> task(RunnableWithPromise5<T, A, B, C, D, E> runnable) {
        return new QTypedPromiseTaskBuilder5Arg0<>(0, null, runnable);
    }
    
    public static final class QTypedPromiseTaskBuilder5Arg0<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        
        QTypedPromiseTaskBuilder5Arg0(int priority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> withArguments(A a) {
            return new QTypedPromiseTaskBuilder5Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder5Arg1<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> withArguments(B b) {
            return new QTypedPromiseTaskBuilder5Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder5Arg2<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder5Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise5<T, A, B, C, D, E> runnable;
        private final A a;
        private final B b;
        
        public QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> withArguments(C c) {
            return new QTypedPromiseTaskBuilder5Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder5Arg3<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> withArguments(D d) {
            return new QTypedPromiseTaskBuilder5Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder5Arg4<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedPromiseTaskBuilder5Arg5<T, A, B, C, D, E> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e);
        }
        
        private native static <T, A, B, C, D, E> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise5<T, A, B, C, D, E> runnable, A a, B b, C c, D d, E e);
    }
    
    public static <T, A, B, C, D, E, F> QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> task(RunnableWithPromise6<T, A, B, C, D, E, F> runnable) {
        return new QTypedPromiseTaskBuilder6Arg0<>(0, null, runnable);
    }
    
    public static class QTypedPromiseTaskBuilder6Arg0<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        
        QTypedPromiseTaskBuilder6Arg0(int priority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> withArguments(A a) {
            return new QTypedPromiseTaskBuilder6Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder6Arg1<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> withArguments(B b) {
            return new QTypedPromiseTaskBuilder6Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder6Arg2<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder6Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise6<T, A, B, C, D, E, F> runnable;
        private final A a;
        private final B b;
        
        public QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> withArguments(C c) {
            return new QTypedPromiseTaskBuilder6Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder6Arg3<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> withArguments(D d) {
            return new QTypedPromiseTaskBuilder6Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder6Arg4<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> withArguments(E e) {
            return new QTypedPromiseTaskBuilder6Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder6Arg5<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedPromiseTaskBuilder6Arg6<T, A, B, C, D, E, F> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f);
        }
        
        private native static <T, A, B, C, D, E, F> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise6<T, A, B, C, D, E, F> runnable, A a, B b, C c, D d, E e, F f);
    }
    
    public static <T, A, B, C, D, E, F, G> QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> task(RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable) {
        return new QTypedPromiseTaskBuilder7Arg0<>(0, null, runnable);
    }
    
    public static class QTypedPromiseTaskBuilder7Arg0<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        
        QTypedPromiseTaskBuilder7Arg0(int priority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> withArguments(A a) {
            return new QTypedPromiseTaskBuilder7Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder7Arg1<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> withArguments(B b) {
            return new QTypedPromiseTaskBuilder7Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder7Arg2<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder7Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable;
        private final A a;
        private final B b;
        
        public QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> withArguments(C c) {
            return new QTypedPromiseTaskBuilder7Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder7Arg3<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> withArguments(D d) {
            return new QTypedPromiseTaskBuilder7Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder7Arg4<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> withArguments(E e) {
            return new QTypedPromiseTaskBuilder7Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder7Arg5<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> withArguments(F f) {
            return new QTypedPromiseTaskBuilder7Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder7Arg6<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedPromiseTaskBuilder7Arg7<T, A, B, C, D, E, F, G> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g);
        }
        
        private native static <T, A, B, C, D, E, F, G> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise7<T, A, B, C, D, E, F, G> runnable, A a, B b, C c, D d, E e, F f, G g);
    }
    
    public static <T, A, B, C, D, E, F, G, H> QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> task(RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable) {
        return new QTypedPromiseTaskBuilder8Arg0<>(0, null, runnable);
    }
    
    public static class QTypedPromiseTaskBuilder8Arg0<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        
        QTypedPromiseTaskBuilder8Arg0(int priority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> withArguments(A a) {
            return new QTypedPromiseTaskBuilder8Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static class QTypedPromiseTaskBuilder8Arg1<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> withArguments(B b) {
            return new QTypedPromiseTaskBuilder8Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder8Arg2<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder8Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable;
        private final A a;
        private final B b;
        
        public QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> withArguments(C c) {
            return new QTypedPromiseTaskBuilder8Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder8Arg3<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> withArguments(D d) {
            return new QTypedPromiseTaskBuilder8Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder8Arg4<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> withArguments(E e) {
            return new QTypedPromiseTaskBuilder8Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder8Arg5<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> withArguments(F f) {
            return new QTypedPromiseTaskBuilder8Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder8Arg6<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> withArguments(G g) {
            return new QTypedPromiseTaskBuilder8Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder8Arg7<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedPromiseTaskBuilder8Arg8<T, A, B, C, D, E, F, G, H> extends QTypedTaskBuilder<T>{
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
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g, h);
        }
        
        private native static <T, A, B, C, D, E, F, G, H> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise8<T, A, B, C, D, E, F, G, H> runnable, A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    public static <T, A, B, C, D, E, F, G, H, I> QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> task(RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable) {
        return new QTypedPromiseTaskBuilder9Arg0<>(0, null, runnable);
    }
    
    public static class QTypedPromiseTaskBuilder9Arg0<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        
        QTypedPromiseTaskBuilder9Arg0(int priority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable) {
            super(priority, threadPool);
            this.runnable = runnable;
        }
        
        public QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> withArguments(A a) {
            return new QTypedPromiseTaskBuilder9Arg1<>(priority(), threadPool(), runnable, a);
        }
        
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b) {
            return new QTypedPromiseTaskBuilder9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static class QTypedPromiseTaskBuilder9Arg1<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg1(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        
        public QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> withArguments(B b) {
            return new QTypedPromiseTaskBuilder9Arg2<>(priority(), threadPool(), runnable, a, b);
        }
        
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(B b, C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedPromiseTaskBuilder9Arg2<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
        QTypedPromiseTaskBuilder9Arg2(int newPriority, QThreadPool threadPool, RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b) {
            super(newPriority, threadPool);
            this.runnable = runnable;
            this.a = a;
            this.b = b;
        }
        private final RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable;
        private final A a;
        private final B b;
        
        public QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> withArguments(C c) {
            return new QTypedPromiseTaskBuilder9Arg3<>(priority(), threadPool(), runnable, a, b, c);
        }
        
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(C c, D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedPromiseTaskBuilder9Arg3<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> withArguments(D d) {
            return new QTypedPromiseTaskBuilder9Arg4<>(priority(), threadPool(), runnable, a, b, c, d);
        }
        
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(D d, E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedPromiseTaskBuilder9Arg4<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> withArguments(E e) {
            return new QTypedPromiseTaskBuilder9Arg5<>(priority(), threadPool(), runnable, a, b, c, d, e);
        }
        
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(E e, F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedPromiseTaskBuilder9Arg5<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> withArguments(F f) {
            return new QTypedPromiseTaskBuilder9Arg6<>(priority(), threadPool(), runnable, a, b, c, d, e, f);
        }
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(F f, G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
    
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
    
    public static final class QTypedPromiseTaskBuilder9Arg6<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> withArguments(G g) {
            return new QTypedPromiseTaskBuilder9Arg7<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g);
        }
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(G g, H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder9Arg7<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        public QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> withArguments(H h) {
            return new QTypedPromiseTaskBuilder9Arg8<>(priority(), threadPool(), runnable, a, b, c, d, e, f, g, h);
        }
        
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
    
    public static final class QTypedPromiseTaskBuilder9Arg8<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
    
    public static final class QTypedPromiseTaskBuilder9Arg9<T, A, B, C, D, E, F, G, H, I> extends QTypedTaskBuilder<T>{
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
        
        @Override
        public QFuture<T> spawn() {
            return spawn(threadPoolId(), priority(), runnable, a, b, c, d, e, f, g, h, i);
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
        private native static <T, A, B, C, D, E, F, G, H, I> QFuture<T> spawn(long threadPoolId, int priority,
                RunnableWithPromise9<T, A, B, C, D, E, F, G, H, I> runnable, A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }

}// class


