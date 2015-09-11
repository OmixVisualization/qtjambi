/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

        static {
            org.qtjambi.qt.QtJambi_LibraryInitializer.init();
            org.qtjambi.qt.concurrent.QtJambi_LibraryInitializer.init();
        }

        /**
         * An implemetation of this interface is given one to QtConcurrent's map() methods.
         * The map() method of this interface is called for each object in a java.util.Collection.
         *
         */
        public interface MapFunctor<T> {

            /**
             * This function is called for each item in the Collection. The function is then free to alter <tt>object</tt> as it see fit.
             */
            public void map(T object);
        }
        /**
         *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
         */
        public static native <T> QFutureVoid map(java.util.Collection<T> sequence, MapFunctor<T> functor);

        /**
         *  Calls function once for each item in sequence. The function is passed a reference to the item, so that any modifications done to the item will appear in sequence.
         */
        public static native <T> void blockingMap(java.util.Collection<T> sequence, MapFunctor<T> functor);

        /**
         * Implement this interface to perform a mapped operation. An implementation of the interface is sendt
         * to ome of the mapped methods of QtConcurrent, which applies the MappedFunctor.map() method to all elements in a collection,
         * and returns the result.
         */
        public interface MappedFunctor<U, T> {

            /**
             * This method is called for each object in a collection. It should returned a new altered
             * object.
             */
            public U map(T object);
        }

        /**
         * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
         *
         */
        public static native <U, T> QFuture<U> mapped(java.util.Collection<T> sequence, MappedFunctor<U, T> functor);

        /**
         * Calls function once for each item in sequence and returns a future with each mapped item as a result. You can QFutureIterator to iterate through the results.
         */
        public static native <U, T> java.util.List<U> blockingMapped(java.util.Collection<T> sequence, MappedFunctor<U, T> functor);

        /**
         * Implement this interface in order to perform a reduce operation.
         * <p>
         * The reduce method will be called once per intermediate result (the result of the mapping of the data)
         * and the very first time the reduce() method is called for the particular data set, the result is set to
         * the returned value of the defaultResult() method.
         */
        public interface ReducedFunctor<U, T> {
            public U defaultResult();

            /**
             *  Performs a reduce operation on <tt>intermediate</tt>. <tt>result</tt> is the result of the reduction.
             */
            public void reduce(U result, T intermediate);
        }

        /**
         * This is an overloaded method provided for convenience.
         * <p>
         * It is equivalent of mappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
         */
        public static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor) {
            return mappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
        }


        /**
         * This is an overloaded method provided for convenience.
         * <p>
         * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
         *
         */
        public static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOption ... options) {
            return mappedReduced(sequence, functor, reducedFunctor, new ReduceOptions(options));
        }

        /**
         * Calls mapFunction once for each item in sequence. The return value of each mapFunction is passed to reduceFunction.
         * <p>
         * Note that while mapFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is determined by reduceOptions.
         *
         */
        public static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOptions options) {
            return mappedReduced(sequence, functor, reducedFunctor, options.value());
        }

        private native static <U, V, T> QFuture<U> mappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, int options);


        /**
         * This is an overloaded method provided for convenience.
         * <p>
         * It is equivalent of calling blockingMappedReduced(sequence, functor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
         *
         */
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
        public static <U, V, T> U blockingMappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, ReduceOptions options) {
            return blockingMappedReduced(sequence, functor, reducedFunctor, options.value());
        }

        private native static <U, V, T> U blockingMappedReduced(java.util.Collection<T> sequence, MappedFunctor<V, T> functor, ReducedFunctor<U, V> reducedFunctor, int options);

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
         * Calls filterFunctor's filtered() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
         */
        public native static <T> QFuture<T> filtered(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor);

        /**
         * Calls filterFunctor's filtered() method once for each item in sequence and returns a new Sequence of kept items. If filterFunction returns true, a copy of the item is put in the new Sequence. Otherwise, the item will not appear in the new Sequence.
         */
        public native static <T> java.util.List<T> blockingFiltered(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor);


        /**
         * This is an overloaded method provided for convenience. It is equivalent of calling filteredReduced(sequence, filteredFunctor, ReduceOption.UnorderedReduce, ReduceOption.Seq This is an overloaded method provided for convenience. It is equivalent of calling filteredReduced)
         */
        public static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor) {
            return filteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
        }

        /**
         * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
         * <p>
         * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
         */
        public static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOption ... options) {
            return filteredReduced(sequence, filteredFunctor, reducedFunctor, new ReduceOptions(options));
        }

        /**
         * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
         * <p>
         * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
         */
        public static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOptions options) {
            return filteredReduced(sequence, filteredFunctor, reducedFunctor, options.value());
        }
        private native static <U, T> QFuture<U> filteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, int options);

        /**
         * This is an overloaded method provided for convenience. It is the equivalent of calling blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce)
         */
        public static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor) {
            return blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, ReduceOption.UnorderedReduce, ReduceOption.SequentialReduce);
        }

        /**
         * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
         * <p>
         * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
         */
        public static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOption ... options) {
            return blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, new ReduceOptions(options));
        }

        /**
         * Calls filterFunction once for each item in sequence. If filterFunction returns true for an item, that item is then passed to reduceFunction. In other words, the return value is the result of reduceFunction for each item where filterFunction returns true.
         * <p>
         * Note that while filterFunction is called concurrently, only one thread at a time will call reduceFunction. The order in which reduceFunction is called is undefined if reduceOptions is QtConcurrent::UnorderedReduce. If reduceOptions is QtConcurrent::OrderedReduce, reduceFunction is called in the order of the original sequence.
         */
        public static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, ReduceOptions options) {
            return blockingFilteredReduced(sequence, filteredFunctor, reducedFunctor, options.value());
        }

        private native static <U, T> U blockingFilteredReduced(java.util.Collection<T> sequence, FilteredFunctor<T> filteredFunctor, ReducedFunctor<U, T> reducedFunctor, int options);

        /**
         * Executes the method <tt>m</tt> through the QtConcurrent framework with the given arguments. The returned QFuture object's result wil be the
         * return value of <tt>m</tt>. Note that this method does not accept function that return void. use runVoidMethod() for this.
         */
        public static <T> QFuture<T> run(Object _this, java.lang.reflect.Method m, Object ... args) {
            if (m.getReturnType() == null || m.getReturnType().equals(Void.TYPE))
                throw new IllegalArgumentException("Cannot call run on method returning void. Use 'runVoidMethod' instead.");

            return runPrivate(_this, m.getDeclaringClass(), m, args, org.qtjambi.qt.internal.QtJambiInternal.resolveConversionSchema(m.getParameterTypes(), m.getParameterTypes()), org.qtjambi.qt.internal.QtJambiInternal.typeConversionCode(m.getReturnType()));
        }
        private native static <T> QFuture<T> runPrivate(Object _this, Class < ? > declaringClass, java.lang.reflect.Method m, Object args[], int conversionScheme[], byte returnType);

        /**
         * Executes the method <tt>m</tt> with the given arguments using the QtConcurrent framework. Notice that runVoidMethod() does not
         * accept methods that has a return value. Use the run() method for this purpose.
         */
        public static QFutureVoid runVoidMethod(Object _this, java.lang.reflect.Method m, Object ... args) {
            if (m.getReturnType() != null && !m.getReturnType().equals(Void.TYPE))
                throw new IllegalArgumentException("Cannot call runVoidMethod on method returning non-void type. Use 'run' instead.");

            return runVoidMethodPrivate(_this, m.getDeclaringClass(), m, args, org.qtjambi.qt.internal.QtJambiInternal.resolveConversionSchema(m.getParameterTypes(), m.getParameterTypes()));
        }
        private native static QFutureVoid runVoidMethodPrivate(Object _this, Class < ? > declaringClass, java.lang.reflect.Method m, Object args[], int conversionScheme[]);


}// class


