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

package generator;

import io.qt.*;
import io.qt.internal.QtJambiObject.QPrivateConstructor;
import io.qt.core.*;

class QObject___ extends QObject {
    
    /**
     * Parameter type for declarative constructors, i.e. constructors being
     * called by QML only.
     */
    protected static final class QDeclarativeConstructor { 
        private QDeclarativeConstructor(Class<?> cls, long placement) { this.placement = placement; this.cls = cls; }
        @io.qt.internal.NativeAccess
        private final long placement;
        @io.qt.internal.NativeAccess
        private final Class<?> cls;
    }
    
    private static String classToScope(Class<?> cls) {
        if(cls.isEnum()) {
            if(cls.getDeclaringClass()!=null) {
                return classToScope(cls.getDeclaringClass());
            }else {
                return cls.getPackage().getName();
            }
        }else if(cls.isAnonymousClass()) {
            if(cls.getEnclosingClass()!=null) {
                return classToScope(cls.getEnclosingClass());
            }
        }else if(cls.getEnclosingClass()!=null) {
            return classToScope(cls.getEnclosingClass())+"$"+cls.getSimpleName();
        }
        return cls.getName();
    }
    
    /**
     * Returns a translated version of source, or source itself if there is no appropriate translated version.
     * Note that if you need to translate in a static context, you can use QCoreApplication::translate(),
     * which is a static method.
     *
     * @param source the source text to translate.
     * @return translated version of the source text.
     */
    public static String tr(String source) {
        String scope = classToScope(callerClassProvider().get());
        return QCoreApplication.translate(scope, source);
    }

    /**
     * Returns a translated version of source, or source itself if there is no appropriate translated version.
     * The comment is used to help translators translate the source text.
     * Note that if you need to translate in a static context, you can use QCoreApplication::translate(),
     * which is a static method.
     *
     * @param source the source text to translate.
     * @param comment helps the translator translate the source text.
     * @return translated version of the source text.
     */
    public static String tr(String source, String comment) {
        String scope = classToScope(callerClassProvider().get());
        return QCoreApplication.translate(scope, source, comment);
    }

    /**
     * Returns a translated version of source, or source itself if there is no appropriate translated version.
     * If %n occurs in the source text, it will be substituted with count.
     * The comment is used to help translators translate the source text.
     * Note that if you need to translate in a static context, you can use QCoreApplication::translate(),
     * which is a static method.
     *
     * @param source the source text to translate.
     * @param comment helps the translator translate the source text.
     * @param count in source %n will be substituted by count.
     * @return translated version of the source text.
     */
    public static String tr(String source, String comment, int count) {
        String scope = classToScope(callerClassProvider().get());
        return QCoreApplication.translate(scope, source, comment, count);
    }

    /**
     * <p>Overloaded function for {@link #findChild(Class)}
     *  with <code>cl = QObject.class</code>.</p>
     * @return found child
     */
    @io.qt.QtUninvokable
    public final QObject findChild() {
        return findChild(QObject.class);
    }
    
    /**
     * <p>Overloaded function for {@link #findChild(Class,String,Qt.FindChildOptions)}</p>
     * <p>with: <ul>
     * <li><code>name = null</code></li>
     * <li><code>options = FindChildrenRecursively</code></li>
     * </ul>
     * @param <T> type of child
     * @param cl type of child
     * @return found child
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl) {
        return findChild(cl, (String)null, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChild(Class,String,Qt.FindChildOptions)}
     * with: <code>options = FindChildrenRecursively</code>.</p>
     * @param <T> type of child
     * @param cl type of child
     * @param name name of child
     * @return found child
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl, String name) {
        return findChild(cl, name, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChild(Class,String,Qt.FindChildOptions)}.</p>
     * @param <T> type of child
     * @param cl type of child
     * @param name name of child
     * @param options search options
     * @return found child
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl, String name, Qt.FindChildOption... options) {
        return findChild(cl, name, new Qt.FindChildOptions(options));
    }
    
    /**
     * <p>Returns the child of this object that instane of <i>cl</i> and
     * that is called <i>name</i>, or <code>null</code> if there is no such object.
     * Omitting the <i>name</i> argument causes all object names to be matched.
     * The search is performed recursively, unless <i>options</i> specifies the
     * option <code>FindDirectChildrenOnly</code>.</p>
     * 
     * <p>If there is more than one child matching the search, the most
     * direct ancestor is returned. If there are several direct
     * ancestors, it is undefined which one will be returned. In that
     * case, {@link #findChildren()} should be used.</p>
     * 
     * <p>This example returns a child <code>QPushButton</code> of <code>parentWidget</code>
     * named <code>"button1"</code>, even if the button isn't a direct child of
     * the parent:</p>
     * <p>
     * <code>QPushButton button = parentWidget.findChild(QPushButton.class, "button1");</code>
     * </p>
     * <p>This example returns a <code>QListWidget</code> child of <code>parentWidget</code>:</p>
     * <p>
     * <code>QListWidget list = parentWidget.findChild(QListWidget.class);</code>
     * </p>
     * <p>This example returns a child <code>QPushButton</code> of <code>parentWidget</code>
     * (its direct parent) named <code>"button1"</code>:</p>
     * <p>
     * <code>QPushButton button = parentWidget.findChild(QPushButton.class, "button1", Qt.FindChildOption.FindDirectChildrenOnly);</code>
     * </p>
     * <p>This example returns a <code>QListWidget</code> child of <code>parentWidget</code>,
     * its direct parent:</p>
     * <p>
     * <code>QListWidget list = parentWidget.findChild(QListWidget.class, null, Qt.FindChildOption.FindDirectChildrenOnly);</code>
     * </p>
     * @param <T> type of child
     * @param cl type of child
     * @param name name of child
     * @param options search options
     * @return found child
     * @see #findChildren()
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl, String name, Qt.FindChildOptions options) {
        return findChild(nativeId(this), java.util.Objects.requireNonNull(cl), QMetaObject.forType(cl).metaObjectPointer, name, options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> T findChild(long nativeId, Class < T > cl, long metaObjectPointer, String name, int options);
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class)}
     *  with <code>cl = QObject.class</code>.</p>
     * @return found children
     */
    @io.qt.QtUninvokable
    public final QList<QObject> findChildren() {
        return findChildren(QObject.class, (String)null, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,String,Qt.FindChildOptions)}</p>
     * <p>with: <ul>
     * <li><code>name = null</code></li>
     * <li><code>options = FindChildrenRecursively</code></li>
     * </ul>
     * @param <T> type of children
     * @param cl type of children
     * @return found children
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl) {
        return findChildren(cl, (String)null, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,String,Qt.FindChildOptions)}
     * with: <code>options = FindChildrenRecursively</code>.</p>
     * @param <T> type of children
     * @param cl type of children
     * @param name name of children
     * @return found children
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, String name) {
        return findChildren(cl, name, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,String,Qt.FindChildOptions)}.</p>
     * @param <T> type of children
     * @param cl type of children
     * @param name name of children
     * @param options search options
     * @return found children
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, String name, Qt.FindChildOption... options) {
        return findChildren(cl, name, new Qt.FindChildOptions(options));
    }
    
    /**
     * <p>Returns all children of this object with the given <i>name</i> that are
     * instance of <i>cl</i>, or an empty list if there are no such objects.
     * Omitting the <i>name</i> argument causes all object names to be matched.
     * The search is performed recursively, unless <i>options</i> specifies the
     * option <i>FindDirectChildrenOnly</i>.</p>
     * 
     * <p>The following example shows how to find a list of child <code>QWidget</code>s of
     * the specified <code>parentWidget</code> named <code>widgetname</code>:</p>
     * <p>
     * <code>List&lt;QWidget> widgets = parentWidget.findChildren(QWidget.class, "widgetname");</code>
     * </p>
     * <p>This example returns all <code>QPushButton</code>s that are children of <code>parentWidget</code>:</p>
     * <p>
     * <code>List&lt;QPushButton> allPButtons = parentWidget.findChildren(QPushButton.class);</code>
     * </p>
     * <p>This example returns all <code>QPushButton</code>s that are immediate children of <code>parentWidget</code>:</p>
     * <p>
     * <code>List&lt;QPushButton> childButtons = parentWidget.findChildren(QPushButton.class, null, Qt.FindChildOption.FindDirectChildrenOnly);</code>
     * </p>
     * @param <T> type of children
     * @param cl type of children
     * @param name name of children
     * @param options search options
     * @return found children
     * @see #findChild()
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, String name, Qt.FindChildOptions options){
        return findChildrenString(nativeId(this), QMetaObject.forType(java.util.Objects.requireNonNull(cl)).metaObjectPointer, name, options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> QList<T> findChildrenString(long nativeId, long metaObjectPointer, String name, int options);
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,QRegularExpression,Qt.FindChildOptions)}
     * with: <code>options = FindChildrenRecursively</code>.</p>
     * @param <T> type of children
     * @param cl type of children
     * @param re regular expression
     * @return found children
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, QRegularExpression re) {
        return findChildren(cl, re, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,QRegularExpression,Qt.FindChildOptions)}.</p>
     * @param <T> type of children
     * @param cl type of children
     * @param re regular expression
     * @param options search options
     * @return found children
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, QRegularExpression re, Qt.FindChildOption... options) {
        return findChildren(cl, re, new Qt.FindChildOptions(options));
    }
    
    /**
     * <p>This function overloads {@link #findChildren()}.</p>
     * <p>Returns the children of this object that are instance of <i>cl</i> 
     * and that have names matching the regular expression <i>re</i>, 
     * or an empty list if there are no such objects. 
     * The search is performed recursively, unless <i>options</i> specifies the option <i>FindDirectChildrenOnly</i>.</p>
     * @param <T> type of children
     * @param cl type of children
     * @param re regular expression
     * @param options search options
     * @return found children
     * @see #findChildren()
     */
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, QRegularExpression re, Qt.FindChildOptions options){
        return findChildrenQRegularExpression(nativeId(this), QMetaObject.forType(java.util.Objects.requireNonNull(cl)).metaObjectPointer, nativeId(re), options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> QList<T> findChildrenQRegularExpression(long nativeId, long metaObjectPointer, long re, int options);
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes no
     * parameters.
     */
    public final class Signal0 extends QMetaObject.AbstractPublicSignal0 {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes one
     * parameter.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public final class Signal1<A> extends QMetaObject.AbstractPublicSignal1<A> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes two
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class Signal2<A, B> extends QMetaObject.AbstractPublicSignal2<A, B> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes three
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class Signal3<A, B, C> extends QMetaObject.AbstractPublicSignal3<A, B, C> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes four
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */

    public final class Signal4<A, B, C, D> extends QMetaObject.AbstractPublicSignal4<A, B, C, D> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5<A, B, C, D, E> extends QMetaObject.AbstractPublicSignal5<A, B, C, D, E> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6<A, B, C, D, E, F> extends QMetaObject.AbstractPublicSignal6<A, B, C, D, E, F> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7<A, B, C, D, E, F, G> extends QMetaObject.AbstractPublicSignal7<A, B, C, D, E, F, G> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractPublicSignal8<A, B, C, D, E, F, G, H> {
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractPublicSignal9<A, B, C, D, E, F, G, H, I>{
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes one
     * parameters and providing default value for the parameter.
     *
     * @param <A> The type of the first parameter of the signal.
     */
    public final class Signal1Default1<A> extends QMetaObject.AbstractPublicSignal1<A> {
        public Signal1Default1(Supplier<A> arg1Default){
            super();
            if(arg1Default!=null){
                this.arg1Default = arg1Default;
            }else{
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes two
     * parameters and providing a default value for parameter no. 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class Signal2Default1<A, B> extends QMetaObject.AbstractSignal2Default1<A, B>{
        public Signal2Default1(Supplier<B> arg2Default) {
            super(arg2Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes two
     * parameters and providing default values for parameters 1 and 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class Signal2Default2<A, B> extends QMetaObject.AbstractSignal2Default1<A, B> {
        
        public Signal2Default2(Supplier<A> arg1Default, Supplier<B> arg2Default){
            super(arg2Default);
            if(arg1Default!=null){
                this.arg1Default = arg1Default;
            }else{
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes three
     * parameters and providing a default value for parameter no. 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class Signal3Default1<A, B, C> extends QMetaObject.AbstractSignal3Default1<A, B, C> {
        public Signal3Default1(Supplier<C> arg3Default) {
            super(arg3Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes three
     * parameters and providing default values for parameters 2 and 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class Signal3Default2<A, B, C> extends QMetaObject.AbstractSignal3Default2<A, B, C> {
        public Signal3Default2(Supplier<B> arg2Default, Supplier<C> arg3Default){
            super(arg2Default, arg3Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes three
     * parameters and providing default values for parameters 1 to 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class Signal3Default3<A, B, C> extends QMetaObject.AbstractSignal3Default2<A, B, C> {
        
        public Signal3Default3(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default){
            super(arg2Default, arg3Default);
            if(arg2Default!=null){
                this.arg1Default = arg1Default;
            }else{
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes four
     * parameters and providing a default value for parameter no. 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public final class Signal4Default1<A, B, C, D> extends QMetaObject.AbstractSignal4Default1<A, B, C, D>{
        public Signal4Default1(Supplier<D> arg4Default){
            super(arg4Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes four
     * parameters and providing default values for parameters 3 and 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public final class Signal4Default2<A, B, C, D> extends QMetaObject.AbstractSignal4Default2<A, B, C, D>{
        public Signal4Default2(Supplier<C> arg3Default, Supplier<D> arg4Default){
            super(arg3Default, arg4Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes four
     * parameters and providing default values for parameters 2 to 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public final class Signal4Default3<A, B, C, D> extends QMetaObject.AbstractSignal4Default3<A, B, C, D>{
        public Signal4Default3(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default){
            super(arg2Default, arg3Default, arg4Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes four
     * parameters and providing default values for parameters 1 to 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public final class Signal4Default4<A, B, C, D> extends QMetaObject.AbstractSignal4Default3<A, B, C, D> {
        
        public Signal4Default4(Supplier<A> arg1Default, Supplier<B> arg2Default, 
                Supplier<C> arg3Default, Supplier<D> arg4Default){
            super(arg2Default, arg3Default, arg4Default);
            if(arg1Default!=null){
                this.arg1Default = arg1Default;
            }else{
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters and providing a default value for parameter no. 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5Default1<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default1<A, B, C, D, E>{
        public Signal5Default1(Supplier<E> arg5Default){
            super(arg5Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters and providing default values for parameters 4 and 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5Default2<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default2<A, B, C, D, E>{
        public Signal5Default2(Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg4Default, arg5Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters and providing default values for parameters 3 to 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5Default3<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default3<A, B, C, D, E>{
        public Signal5Default3(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg3Default, arg4Default, arg5Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters and providing default values for parameters 2 to 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5Default4<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default4<A, B, C, D, E>{
        public Signal5Default4(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters and providing default values for parameters 1 to 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5Default5<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default4<A, B, C, D, E>{
        
        public Signal5Default5(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default);
            if(arg1Default!=null){
                this.arg1Default = arg1Default;
            }else{
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters and providing a default value for parameter no. 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6Default1<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default1<A, B, C, D, E, F>{
        public Signal6Default1(Supplier<F> arg6Default){
            super(arg6Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters and providing default values for parameters 5 and 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6Default2<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default2<A, B, C, D, E, F>{
        public Signal6Default2(Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg5Default, arg6Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters and providing default values for parameters 4 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6Default3<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default3<A, B, C, D, E, F>{
        public Signal6Default3(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg4Default, arg5Default, arg6Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters and providing default values for parameters 3 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6Default4<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default4<A, B, C, D, E, F>{
        public Signal6Default4(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters and providing default values for parameters 2 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6Default5<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default5<A, B, C, D, E, F>{
        public Signal6Default5(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters and providing default values for parameters 1 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6Default6<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default5<A, B, C, D, E, F>{
        public Signal6Default6(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing a default value for parameter no. 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default1<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default1<A, B, C, D, E, F, G>{
        public Signal7Default1(Supplier<G> arg7Default){
            super(arg7Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing default values for parameters 6 and 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default2<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default2<A, B, C, D, E, F, G> {
        
        public Signal7Default2(Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg6Default, arg7Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing default values for parameters 5 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default3<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default3<A, B, C, D, E, F, G> {
        
        public Signal7Default3(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg5Default, arg6Default, arg7Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing default values for parameters 4 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default4<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default4<A, B, C, D, E, F, G> {
        
        public Signal7Default4(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing default values for parameters 3 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default5<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default5<A, B, C, D, E, F, G> {
        
        public Signal7Default5(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing default values for parameters 2 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default6<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default6<A, B, C, D, E, F, G> {
        
        public Signal7Default6(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters and providing default values for parameters 1 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7Default7<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default6<A, B, C, D, E, F, G> {
        
        public Signal7Default7(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing a default value for parameter no. 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default1<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default1<A, B, C, D, E, F, G, H>{
        public Signal8Default1(Supplier<H> arg8Default){
            super(arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 7 and 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default2<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default2<A, B, C, D, E, F, G, H> {
        public Signal8Default2(Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg7Default, arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 6 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default3<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default3<A, B, C, D, E, F, G, H> {
        public Signal8Default3(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg6Default, arg7Default, arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 5 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default4<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default4<A, B, C, D, E, F, G, H> {
        public Signal8Default4(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 4 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default5<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default5<A, B, C, D, E, F, G, H> {
        public Signal8Default5(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 3 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default6<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default6<A, B, C, D, E, F, G, H> {
        public Signal8Default6(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 2 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default7<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default7<A, B, C, D, E, F, G, H> {
        public Signal8Default7(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters and providing default values for parameters 1 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class Signal8Default8<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default7<A, B, C, D, E, F, G, H> {
        public Signal8Default8(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        public Signal8Default8(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean dummy){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, dummy);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing a default value for parameter no. 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default1<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default1<A, B, C, D, E, F, G, H, I>{
        public Signal9Default1(Supplier<I> arg9Default){
            super(arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 8 and 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default2<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> {
        public Signal9Default2(Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 7 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default3<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> {
        public Signal9Default3(Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg7Default, arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 6 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default4<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> {
        public Signal9Default4(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 5 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default5<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> {
        public Signal9Default5(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 4 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default6<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> {
        public Signal9Default6(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 3 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default7<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> {
        public Signal9Default7(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 2 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default8<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> {
        public Signal9Default8(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters and providing default values for parameters 1 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class Signal9Default9<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> {
        
        public Signal9Default9(Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<A> arg1Default;
        
        public void emit() {
            emit(arg1Default.get());
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes no
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     */
    public final class PrivateSignal0 extends QMetaObject.AbstractPrivateSignal0 {
        
        public PrivateSignal0() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit() {
            emitSignal();
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes one
     * parameter.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public final class PrivateSignal1<A> extends QMetaObject.AbstractPrivateSignal1<A> {
        
        public PrivateSignal1() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1) {
            emitSignal(arg1);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes two
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class PrivateSignal2<A, B> extends QMetaObject.AbstractPrivateSignal2<A, B> {
        
        public PrivateSignal2() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2) {
            emitSignal(arg1, arg2);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes three
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class PrivateSignal3<A, B, C> extends QMetaObject.AbstractPrivateSignal3<A, B, C> {
        
        public PrivateSignal3() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3) {
            emitSignal(arg1, arg2, arg3);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes four
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */

    public final class PrivateSignal4<A, B, C, D> extends QMetaObject.AbstractPrivateSignal4<A, B, C, D> {
        
        public PrivateSignal4() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4) {
            emitSignal(arg1, arg2, arg3, arg4);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes five
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class PrivateSignal5<A, B, C, D, E> extends QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> {
        
        public PrivateSignal5() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            emitSignal(arg1, arg2, arg3, arg4, arg5);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes six
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class PrivateSignal6<A, B, C, D, E, F> extends QMetaObject.AbstractPrivateSignal6<A, B, C, D, E, F> {
        
        public PrivateSignal6() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes seven
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class PrivateSignal7<A, B, C, D, E, F, G> extends QMetaObject.AbstractPrivateSignal7<A, B, C, D, E, F, G> {
        
        public PrivateSignal7() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes eight
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public final class PrivateSignal8<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractPrivateSignal8<A, B, C, D, E, F, G, H> {
        
        public PrivateSignal8() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6,
                G arg7, H arg8) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QObject subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public final class PrivateSignal9<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractPrivateSignal9<A, B, C, D, E, F, G, H, I> {
        
        public PrivateSignal9() {
            super();
        }
        
        /**
         * Emits the signal.
         */
        @io.qt.QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6,
                G arg7, H arg8, I arg9) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param signal the signal to be emitted
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static void emit(PrivateSignal0 signal) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit();
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A> void emit(PrivateSignal1<A> signal, A arg1) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B> void emit(PrivateSignal2<A,B> signal, A arg1, B arg2) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C> void emit(PrivateSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @param arg4 The argument for the fourth parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C,D> void emit(PrivateSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @param arg4 The argument for the fourth parameter of the signal.
     * @param arg5 The argument for the fifth parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C,D,E> void emit(PrivateSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @param arg4 The argument for the fourth parameter of the signal.
     * @param arg5 The argument for the fifth parameter of the signal.
     * @param arg6 The argument for the sixth parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C,D,E,F> void emit(PrivateSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @param arg4 The argument for the fourth parameter of the signal.
     * @param arg5 The argument for the fifth parameter of the signal.
     * @param arg6 The argument for the sixth parameter of the signal.
     * @param arg7 The argument for the seventh parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C,D,E,F,G> void emit(PrivateSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @param arg4 The argument for the fourth parameter of the signal.
     * @param arg5 The argument for the fifth parameter of the signal.
     * @param arg6 The argument for the sixth parameter of the signal.
     * @param arg7 The argument for the seventh parameter of the signal.
     * @param arg8 The argument for the eighth parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C,D,E,F,G,H> void emit(PrivateSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Emits a private signal. This method may only be called from inside the signal owning object.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     * @param signal the signal to be emitted
     * @param arg1 The argument for the first parameter of the signal.
     * @param arg2 The argument for the second parameter of the signal.
     * @param arg3 The argument for the third parameter of the signal.
     * @param arg4 The argument for the fourth parameter of the signal.
     * @param arg5 The argument for the fifth parameter of the signal.
     * @param arg6 The argument for the sixth parameter of the signal.
     * @param arg7 The argument for the seventh parameter of the signal.
     * @param arg8 The argument for the eighth parameter of the signal.
     * @param arg9 The argument for the ninth parameter of the signal.
     * @throws io.qt.QSignalAccessException if signal is emitted from outside the declaring class.
     */
    protected static <A,B,C,D,E,F,G,H,I> void emit(PrivateSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    /**
     * Container type for {@link MultiSignal} initialization.
     */
    protected final static class SignalConfiguration extends QMetaObject.SignalConfiguration{
        /**
         * Declares a parameterless signal.
         * @param signal the declared signal
         */
        public SignalConfiguration(QMetaObject.AbstractPrivateSignal0 signal) {
            super(signal);
        }
        
        /**
         * Declares a signal with onw parameter.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param signal the declared signal
         */
        public <A> SignalConfiguration(Class<A> type1, QMetaObject.AbstractPrivateSignal1<A> signal) {
            super(signal, type1);
        }
        
        /**
         * Declares a signal with two parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B> SignalConfiguration(Class<A> type1, Class<B> type2, QMetaObject.AbstractPrivateSignal2<A,B> signal) {
            super(signal, type1, type2);
        }
        
        /**
         * Declares a signal with three parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, QMetaObject.AbstractPrivateSignal3<A,B,C> signal) {
            super(signal, type1, type2, type3);
        }
        
        /**
         * Declares a signal with four parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param <D> The type of the fourth parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param type4 The type of the fourth parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C,D> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal) {
            super(signal, type1, type2, type3, type4);
        }
        
        /**
         * Declares a signal with five parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param <D> The type of the fourth parameter of the signal.
         * @param <E> The type of the fifth parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param type4 The type of the fourth parameter of the signal.
         * @param type5 The type of the fifth parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C,D,E> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal) {
            super(signal, type1, type2, type3, type4, type5);
        }
        
        /**
         * Declares a signal with six parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param <D> The type of the fourth parameter of the signal.
         * @param <E> The type of the fifth parameter of the signal.
         * @param <F> The type of the sixth parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param type4 The type of the fourth parameter of the signal.
         * @param type5 The type of the fifth parameter of the signal.
         * @param type6 The type of the sixth parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C,D,E,F> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal) {
            super(signal, type1, type2, type3, type4, type5, type6);
        }
        
        /**
         * Declares a signal with seven parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param <D> The type of the fourth parameter of the signal.
         * @param <E> The type of the fifth parameter of the signal.
         * @param <F> The type of the sixth parameter of the signal.
         * @param <G> The type of the seventh parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param type4 The type of the fourth parameter of the signal.
         * @param type5 The type of the fifth parameter of the signal.
         * @param type6 The type of the sixth parameter of the signal.
         * @param type7 The type of the seventh parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C,D,E,F,G> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, Class<G> type7, QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal) {
            super(signal, type1, type2, type3, type4, type5, type6, type7);
        }
        
        /**
         * Declares a signal with eight parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param <D> The type of the fourth parameter of the signal.
         * @param <E> The type of the fifth parameter of the signal.
         * @param <F> The type of the sixth parameter of the signal.
         * @param <G> The type of the seventh parameter of the signal.
         * @param <H> The type of the eighth parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param type4 The type of the fourth parameter of the signal.
         * @param type5 The type of the fifth parameter of the signal.
         * @param type6 The type of the sixth parameter of the signal.
         * @param type7 The type of the seventh parameter of the signal.
         * @param type8 The type of the eighth parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C,D,E,F,G,H> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, Class<G> type7, Class<H> type8, QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal) {
            super(signal, type1, type2, type3, type4, type5, type6, type7, type8);
        }
        
        /**
         * Declares a signal with nine parameters.
         * 
         * @param <A> The type of the first parameter of the signal.
         * @param <B> The type of the second parameter of the signal.
         * @param <C> The type of the third parameter of the signal.
         * @param <D> The type of the fourth parameter of the signal.
         * @param <E> The type of the fifth parameter of the signal.
         * @param <F> The type of the sixth parameter of the signal.
         * @param <G> The type of the seventh parameter of the signal.
         * @param <H> The type of the eighth parameter of the signal.
         * @param <I> The type of the ninth parameter of the signal.
         * @param type1 The type of the first parameter of the signal.
         * @param type2 The type of the second parameter of the signal.
         * @param type3 The type of the third parameter of the signal.
         * @param type4 The type of the fourth parameter of the signal.
         * @param type5 The type of the fifth parameter of the signal.
         * @param type6 The type of the sixth parameter of the signal.
         * @param type7 The type of the seventh parameter of the signal.
         * @param type8 The type of the eighth parameter of the signal.
         * @param type9 The type of the ninth parameter of the signal.
         * @param signal the declared signal
         */
        public <A,B,C,D,E,F,G,H,I> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, Class<G> type7, Class<H> type8, Class<I> type9, QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal) {
            super(signal, type1, type2, type3, type4, type5, type6, type7, type8, type9);
        }
        
        /**
         * Declares a signal with generic parameters.
         * 
         * @param signal the declared signal
         */
        public SignalConfiguration(QDeclarableSignals.AbstractPrivateGenericSignal signal) {
            super(signal, (Class<?>[])null);
        }
    }
    
    /**
     * Wrapper class supporting the definition of overloaded signal in a class.
     */
    public class MultiSignal extends QMetaObject.AbstractMultiSignal {
        @SuppressWarnings("exports")
        public MultiSignal(SignalConfiguration signal1, SignalConfiguration signal2, SignalConfiguration... furtherSignals){
            super(signal1, signal2, furtherSignals);
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    
        /**
         * Removes the given connection from this signal.
         *
         * @param connection the connection to be removed
         * @return <code>true</code> if the disconnection was successful.
         */
        @Override
        public final boolean disconnect(QMetaObject.Connection connection) {
            return super.disconnect(connection);
        }
        
        /**
         * Disconnects all overloaded signals.
         * @return <code>true</code> if any disconnection was successful.
         */
        @Override
        public final boolean disconnectAll() {
            return super.disconnectAll();
        }
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal0 signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal1<?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal1<A> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal2<?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal2<A,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal2<A,B> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<A,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<A,B,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<A,B,C> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,B,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,B,C,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,C,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F,G> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F,G> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F,G,H> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<?,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,?,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,?,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,?,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,?,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,?,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F,G> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,?,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F,G,H> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,?> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Initializes a connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     * @param signal the signal to be connected
     * @param slot the slot to be connected
     * @param connectionType type of connection
     * @return connection if successful or <code>null</code> otherwise
     * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
     * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
     */
    public static <A,B,C,D,E,F,G,H,I> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, QMetaObject.Slot9<A,B,C,D,E,F,G,H,I> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal0 signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal1<?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal1<A> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal2<?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal2<A,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal2<A,B> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal3<?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal3<A,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal3<A,B,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal3<A,B,C> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal4<?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,B,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,B,C,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal5<?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,C,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal6<?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal7<?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F,G> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal8<?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F,G> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F,G,H> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal9<?,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,?,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,?,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,?,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,?,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,?,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F,G> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,?,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F,G,H> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,?> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Removes the connection between the given <i>signal</i> and <i>slot</i>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     * @param signal the signal to be disconnected
     * @param slot the slot to be disconnected
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static <A,B,C,D,E,F,G,H,I> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, QMetaObject.Slot9<A,B,C,D,E,F,G,H,I> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * <p>Creates a connection of the given <i>type</i> from the <i>signal</i> in
     * the <i>sender</i> object to the <i>method</i> in the <i>receiver</i> object.</p>
     * <p>Returns a handle to the connection that can be used to disconnect
     * it later.</p>
     * 
     * <p><code>
     * QLabel label = new QLabel();<br>
     * QScrollBar scrollBar = new QScrollBar();<br>
     * QObject.connect(scrollBar, "valueChanged(int)", label,  "setNum(int)");<br>
     * </code></p>
     * 
     * <p>This example ensures that the label always displays the current
     * scroll bar value. Note that the signal and slots parameters must not
     * contain any variable names, only the type. E.g. the following would
     * not work and throw {@link QNoSuchSignalException}/{@link QNoSuchSlotException}:</p>
     * 
     * <p><code>
     * QObject.connect(scrollBar, "valueChanged(int value)", label,  "setNum(int value)");
     * </code></p>
     * 
     * <p>A signal can also be connected to another signal:</p>
     * 
     * <p><code>
     * public class MyWidget extends QWidget {<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public final Signal0 buttonClicked = new Signal0();<br>
     * <br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public MyWidget(){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;myButton = new QPushButton(this);<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;connect(myButton, "clicked()", this, "buttonClicked()");<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * <br>
     * &nbsp;&nbsp;&nbsp;&nbsp;private QPushButton myButton;<br>
     * }
     * </code></p>
     * 
     * <p>or alternatively:</p>
     * 
     * <p><code>
     * clicked.connect(myButton.buttonClicked);
     * </code></p>
     * 
     * <p>In this example, the <i>MyWidget</i> constructor relays a signal from
     * a private member variable, and makes it available under a name
     * that relates to <i>MyWidget</i>.</p>
     * 
     * <p>A signal can be connected to many slots and signals. Many signals
     * can be connected to one slot.</p>
     * 
     * <p>If a signal is connected to several slots, the slots are activated
     * in the same order in which the connections were made, when the
     * signal is emitted.</p>
     * 
     * <p>In case the connection was successfully initialized the function returns 
     * a QMetaObject.Connection that represents a handle to this connection. 
     * The connection handle will be invalid
     * if it cannot create the connection, for example, if QObject is unable
     * to verify the existence of either <i>signal</i> or <i>method</i>, or if their
     * signatures aren't compatible.
     * In this case, the function returns null.</p>
     * 
     * <p>By default, a signal is emitted for every connection you make;
     * two signals are emitted for duplicate connections. You can break
     * all of these connections with a single disconnect() call.
     * If you pass the Qt.ConnectionType.UniqueConnection <i>type</i>, the connection will only
     * be made if it is not a duplicate. If there is already a duplicate
     * (exact same signal to the exact same slot on the same objects),
     * the connection will fail and connect will return null.</p>
     * 
     * <p>Note: Qt.ConnectionType.UniqueConnections do not work for lambdas; 
     * they only apply to connecting to member functions.</p>
     * 
     * <p>The optional <i>type</i> parameter describes the type of connection
     * to establish. In particular, it determines whether a particular
     * signal is delivered to a slot immediately or queued for delivery
     * at a later time. If the signal is queued, the parameters must be
     * of types that are known to Qt's meta-object system, because Qt
     * needs to copy the arguments to store them in an event behind the
     * scenes. If you try to use a queued connection and get the error
     * message</p>
     * 
     * <p><code>
     * QObject::connect: Cannot queue arguments of type 'MyType'
     * (Make sure 'MyType' is registered using qRegisterMetaType().)
     * </code></p>
     * 
     * <p>call {@link QMetaType#registerMetaType(Class)} to register the data type before you
     * establish the connection.</p>
     * 
     * @see #disconnect()
     * @see #sender()
     * @see QMetaType#registerMetaType(Class)
     */
    public static QMetaObject.Connection connect(QObject sender, String signal, QObject receiver, String slot, Qt.ConnectionType... connectionType) {
        if(signal.isEmpty())
            throw new IllegalArgumentException("Empty string not allowed as signal.");
        if(slot.isEmpty())
            throw new IllegalArgumentException("Empty string not allowed as slot.");
        QMetaMethod signalMethod = sender.metaObject().method(signal);
        if(signalMethod==null)
            throw new QNoSuchSignalException(signal);
        QMetaMethod slotMethod = receiver.metaObject().method(slot);
        if(slotMethod==null)
            throw new QNoSuchSlotException(receiver, slot);
        return connect(
                sender,
                signalMethod,
                receiver,
                slotMethod,
                connectionType
            );
    }
    
    /**
     * <p>Creates a connection of the given <code>type</code> from the <code>signal</code> in
     * the <code>sender</code> object to the <code>method</code> in the <code>receiver</code> object.
     * Returns a handle to the connection that can be used to disconnect it later.</p>
     *
     * <p>The Connection handle will be invalid  if it cannot create the
     * connection, for example, the parameters were invalid.
     * You can check if a QMetaObject.Connection is returned.</p>
     *
     * <p>This function works in the same way as
     * {@link #connect(QObject, String, QObject, String, io.qt.core.Qt.ConnectionType...)}
     * but it uses QMetaMethod to specify signal and method.</p>
     *
     * @see #connect(QObject, String, QObject, String, io.qt.core.Qt.ConnectionType...)
     */
    public static QMetaObject.Connection connect(QObject sender, QMetaMethod signal, QObject receiver, QMetaMethod slot, Qt.ConnectionType... connectionType) {
        java.util.Objects.requireNonNull(signal);
        long sender_id = checkedNativeId(java.util.Objects.requireNonNull(sender));
        long receiver_id = checkedNativeId(java.util.Objects.requireNonNull(receiver));
        java.util.Objects.requireNonNull(slot);
        byte flags = 0;
        if(connectionType!=null && connectionType.length>0) {
            for (Qt.ConnectionType c : connectionType) {
                flags |= c.value();
            }
        }
        return QMetaObject.connectMethods(
                sender_id,
                signal.methodIndex(),
                signal.enclosingMetaObject().metaObjectPointer,
                receiver_id,
                slot.methodIndex(),
                slot.enclosingMetaObject().metaObjectPointer,
                flags
            );
    }
    
    /**
     * <p>Disconnects <i>signal</i> in object sender from method in object
     * <i>receiver</i>. Returns <code>true</code> if the connection is successfully broken;
     * otherwise returns <code>false</code>.</p>
     * 
     * <p>A signal-slot connection is removed when either of the objects
     * involved are destroyed.</p>
     * 
     * <p>disconnect() is typically used in three ways, as the following
     * examples demonstrate.</p>
     * <ol>
     * <li>Disconnect everything connected to an object's signals:
     * 
     *    <p><code>disconnect(myObject, null, null, null);</code></p>
     * 
     *    equivalent to the non-static overloaded function
     * 
     *    <p><code>myObject.disconnect();</code></p>
     * </li>
     * <li>Disconnect everything connected to a specific signal:
     * 
     *    <p><code>disconnect(myObject, "mySignal()", null, null);</code></p>
     * 
     *    equivalent to the non-static overloaded function
     * 
     *    <p><code>myObject.mySignal.disconnect();</code></p>
     * </li>
     * <li>Disconnect a specific receiver:
     * 
     *    <p><code>disconnect(myObject, null, myReceiver, null);</code></p>
     * 
     *    equivalent to the non-static overloaded function
     * 
     *    <p><code>myObject.disconnect(myReceiver);</code></p>
     * </li>
     * </ol>
     * 
     * <p>0 may be used as a wildcard, meaning "any signal", "any receiving
     * object", or "any slot in the receiving object", respectively.</p>
     * 
     * <p>The <i>sender</i> may never be 0. (You cannot disconnect signals from
     * more than one object in a single call.)</p>
     * 
     * <p>If <i>signal</i> is 0, it disconnects <i>receiver</i> and <i>method</i> from
     * any signal. If not, only the specified signal is disconnected.</p>
     * 
     * <p>If <i>receiver</i> is 0, it disconnects anything connected to
     * <i>signal</i>. If not, QMetaObject.Slots in objects other than <i>receiver</i> are not
     * disconnected.</p>
     * 
     * <p>If <i>method</i> is 0, it disconnects anything that is connected to
     * <i>receiver</i>. If not, only slots named <i>method</i> will be disconnected,
     * and all other slots are left alone. The <i>method</i> must be 0 if 
     * <i>receiver</i> is left out, so you cannot disconnect a
     * specifically-named slot on all objects.</p>
     * 
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QObject sender, String signal, QObject receiver, String slot) {
        if(signal!=null && signal.isEmpty())
            throw new IllegalArgumentException("Empty string not allowed as signal.");
        if(slot!=null && slot.isEmpty())
            throw new IllegalArgumentException("Empty string not allowed as slot.");
        QMetaMethod signalMethod = signal==null ? null : sender.metaObject().method(signal);
        if(signal!=null && signalMethod==null)
            throw new QNoSuchSignalException(signal);
        QMetaMethod slotMethod = slot==null ? null : receiver.metaObject().method(slot);
        if(slot!=null && slotMethod==null)
            throw new QNoSuchSlotException(receiver, slot);
        return disconnect(
                sender,
                signalMethod,
                receiver,
                slotMethod
            );
    }
    
    /**
     * <p>Disconnects <i>signal</i> in object <i>sender</i> from <i>method</i> in object
     * <i>receiver</i> receiver. Returns <code>true</code> if the connection is successfully broken;
     * otherwise returns <code>false</code>.</p>
     *
     * <p>This function provides the same possibilities like
     * {@link #disconnect(QObject, String, QObject, String)}
     * but uses QMetaMethod to represent the signal and the method to be disconnected.</p>
     *
     * <p>Additionally this function returns false and no signals and slots disconnected
     * if:</p>
     * <ol>
     *
     *      <li><i>signal</i> is not a member of sender class or one of its parent classes.</li>
     *
     *      <li><i>method</i> is not a member of receiver class or one of its parent classes.</li>
     *
     *      <li><i>signal</i> instance represents not a signal.</li>
     *
     * </ol>
     *
     * <p><code>null</code> may be used as wildcard in the meaning "any signal" or "any slot in receiving object".
     * In the same way 0 can be used for <i>receiver</i> in the meaning "any receiving object". In this case
     * method should also be <code>null</code>. <i>sender</i> parameter should be never 0.</p>
     *
     * @see #disconnect(QObject, String, QObject, String)
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public static boolean disconnect(QObject sender, QMetaMethod signal, QObject receiver, QMetaMethod slot) {
        long sender_id = checkedNativeId(java.util.Objects.requireNonNull(sender));
        long receiver_id = checkedNativeId(receiver);
        return QMetaObject.disconnectMethods(
                sender_id,
                signal==null ? -1 : signal.methodIndex(),
                signal==null ? 0 : signal.enclosingMetaObject().metaObjectPointer,
                receiver_id,
                slot==null ? -1 : slot.methodIndex(),
                slot==null ? 0 : slot.enclosingMetaObject().metaObjectPointer
            );
    }
    
    /**
     * <p>Disconnect a connection.</p>
     * 
     * <p>If the connection is invalid or has already been disconnected, do nothing
     * and return false.</p>
     * @param connection the connection
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     * 
     */
    public static boolean disconnect(QMetaObject.Connection connection) {
        return io.qt.internal.QtJambiInternal.disconnect(connection);
    }

    /**
     * Disconnect all connections originating in this object.
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public final boolean disconnect() {
        return disconnectAll(this, null);
    }

    /**
     * Disconnect all connections made from this signal emitter to a specific object.
     *
     * @param receiver The receiver to disconnect, or null to disconnect all receivers
     * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
     */
    public final boolean disconnect(Object receiver) {
        if(receiver instanceof QMetaObject.Connection)
            return disconnect((QMetaObject.Connection)receiver);
        else return disconnectAll(this, receiver);
    }

    /**
     * Returns a pointer to the meta-object of this object.
     * @return the object's meta-object
     */
    @io.qt.QtUninvokable
    public final QMetaObject metaObject() {
        return metaObject(nativeId(this));
    }
    
    @io.qt.QtUninvokable
    private static native QMetaObject metaObject(long nativeId);
    
    /**
     * Casts an object to the given <i>targetType</i>. Returns null if object is not instance of <i>targetType</i>.
     * @param <T> type
     * @param targetType
     * @return the object as targetType or null
     */
    @io.qt.QtUninvokable
    public final <T extends QtObjectInterface> T qt_metacast(Class<T> targetType) {
        return QMetaObject.cast(targetType, this);
    }
    
    /**
     * Returns true if this object is an instance of a class that inherits className or a QObject subclass that inherits className; otherwise returns false.
     * A class is considered to inherit itself.
     * @param targetType
     * @return <code>true</code> if object instance of targetType
     */
    @io.qt.QtUninvokable
    public final boolean inherits(Class<?> targetType){
        return targetType.isInstance(this) || inherits(internalNameOfArgumentType(targetType).replace("*", ""));
    }
    
    /**
     * @see #receivers(String)
     */
    @io.qt.QtUninvokable
    protected final int receivers(io.qt.core.QMetaObject.AbstractSignal signalObject){
        String signal = cppSignalSignature(signalObject);
        if(signal.isEmpty())
          return 0;
        return __qt_QObject_receivers_const_char_ptr_constfct(nativeId(this), signal);
    }
}// class

class QObject_6__ extends QObject {

    private static Runnable NO_SIGNAL = ()->{};
    
    private static QMetaMethod findNotifySignalByBindables(QObject object, java.lang.reflect.Field reflectedField, QUntypedPropertyData property) {
        for(QMetaProperty metaProperty : object.metaObject().properties()) {
            if(metaProperty.isBindable()) {
                QUntypedBindable bindable = metaProperty.bindable(object);
                if(bindable.data()==property) {
                    QMetaObject.AbstractSignal.registerPropertyField(metaProperty, reflectedField);
                    QMetaMethod notifySignal = metaProperty.notifySignal();
                    if(notifySignal.isValid() && notifySignal.parameterCount()==0) {
                        return notifySignal;
                    }else {
                        return null;
                    }
                }
            }
        }
        return null;
    }
    
    private class EmitSignal implements Runnable{
        @Override
        public void run() {
            try {
                QMetaObject.AbstractSignal.emitNativeSignal(checkedNativeId(QObject.this), methodIndex); 
            }catch(QNoNativeResourcesException e){}
        }

        private final int methodIndex;

        public EmitSignal(int methodIndex) {
            super();
            this.methodIndex = methodIndex;
        }
    }
    
    private static abstract class AbstractPropertyCore<T>{
        abstract QMetaType valueMetaType(QProperty<T> property);
        abstract void initialize(QProperty<T> property);
        abstract void emitSignal(QProperty<T> property);
        abstract boolean hasSignal(QProperty<T> property);
        abstract T value(QBindingStorage bindingStorage, QProperty<T> property);
        abstract QPropertyBindingData bindingData(QBindingStorage bindingStorage, QProperty<T> property, boolean create);
        abstract boolean setValueBypassingBindings(QProperty<T> property, Object val);
    }
    
    private static class PropertyCore<T> extends AbstractPropertyCore<T>{
        PropertyCore(QMetaType metaType) {
            super();
            this.metaType = metaType;
        }
        final QMetaType valueMetaType(QProperty<T> property) { return metaType; }
        final void initialize(QProperty<T> property) {}
        void emitSignal(QProperty<T> property) {}
        boolean hasSignal(QProperty<T> property) { return false; }
        final T value(QBindingStorage bindingStorage, QProperty<T> property){
            bindingStorage.maybeUpdateBindingAndRegister(property);
            return io.qt.core.QProperty.getValueBypassingBindings(nativeId(property), checkedNativeId(metaType));
        }
        final QPropertyBindingData bindingData(QBindingStorage bindingStorage, QProperty<T> property, boolean create) {
            return bindingStorage.bindingData(property, create);
        }
        boolean setValueBypassingBindings(QProperty<T> property, Object val) {
            return io.qt.core.QProperty.setValueBypassingBindings(nativeId(property), checkedNativeId(metaType), val);
        }
        private final QMetaType metaType;
    }
    
    private final static class SignalPropertyCore<T> extends PropertyCore<T>{
        public SignalPropertyCore(QMetaType metaType, int methodIndex) {
            super(metaType);
            this.methodIndex = methodIndex;
        }
        void emitSignal(QProperty<T> property) {
            try {
                QMetaObject.AbstractSignal.emitNativeSignal(checkedNativeId(java.util.Objects.requireNonNull(property.owner())), methodIndex); 
            }catch(QNoNativeResourcesException e){}
        }
        boolean hasSignal(QProperty<T> property) { return true; }
        private final int methodIndex;
    }
    
    private static class RCPropertyCore<T> extends PropertyCore<T>{
        @SuppressWarnings("unused")
        private Object __rcValue;
        RCPropertyCore(QMetaType metaType) {
            super(metaType);
        }
        final boolean setValueBypassingBindings(QProperty<T> property, Object val) {
            if(super.setValueBypassingBindings(property, val)) {
                __rcValue = val;
                return true;
            }
            else return false;
        }
    }
    
    private final static class RCSignalPropertyCore<T> extends RCPropertyCore<T>{
        public RCSignalPropertyCore(QMetaType metaType, int methodIndex) {
            super(metaType);
            this.methodIndex = methodIndex;
        }
        void emitSignal(QProperty<T> property) {
            try {
                QMetaObject.AbstractSignal.emitNativeSignal(checkedNativeId(java.util.Objects.requireNonNull(property.owner())), methodIndex); 
            }catch(QNoNativeResourcesException e){}
        }
        boolean hasSignal(QProperty<T> property) { return true; }
        private final int methodIndex;
    }
    
    private static class InitializingPropertyCore<T> extends AbstractPropertyCore<T>{
        QMetaType valueMetaType(QProperty<T> property) {
            initialize(property);
            return property.core.valueMetaType(property);
        }
        void initialize(QProperty<T> property) {
            initialize(property, null);
        }
        final void initialize(QProperty<T> property, T val) {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(property.owner(), property);
            if(result.property!=null) {
                if(result.notifySignal==null) {
                    if(result.metaType.flags().isSet(QMetaType.TypeFlag.IsPointer) || result.metaType.name().contains("*")) {
                        property.core = new RCPropertyCore<>(result.metaType);
                    }else {
                        property.core = new PropertyCore<>(result.metaType);
                    }
                }else {
                    if(result.metaType.flags().isSet(QMetaType.TypeFlag.IsPointer) || result.metaType.name().contains("*")) {
                        property.core = new RCSignalPropertyCore<>(result.metaType, result.notifySignal.methodIndex());
                    }else {
                        property.core = new SignalPropertyCore<>(result.metaType, result.notifySignal.methodIndex());
                    }
                }
                io.qt.core.QProperty.__qt_new(property, result.metaType, val);
            }else {
                property.core = new PropertyCore<>(result.metaType);
                io.qt.core.QProperty.__qt_new(property, result.metaType, val);
                QMetaMethod notifySignal = findNotifySignalByBindables(property.owner(), result.reflectedField, property);
                if(notifySignal!=null) {
                    if(result.metaType.flags().isSet(QMetaType.TypeFlag.IsPointer) || result.metaType.name().contains("*")) {
                        property.core = new RCSignalPropertyCore<>(result.metaType, notifySignal.methodIndex());
                    }else {
                        property.core = new SignalPropertyCore<>(result.metaType, notifySignal.methodIndex());
                    }
                }
            }
        }
        final T value(QBindingStorage bindingStorage, QProperty<T> property){
            initialize(property);
            return property.core.value(bindingStorage, property);
        }
        final QPropertyBindingData bindingData(QBindingStorage bindingStorage, QProperty<T> property, boolean create) {
            return property.core.bindingData(bindingStorage, property, create);
        }
        void emitSignal(QProperty<T> property) {
            initialize(property);
            property.core.emitSignal(property);
        }
        boolean hasSignal(QProperty<T> property) {
            initialize(property);
            return property.core.hasSignal(property);
        }
        boolean setValueBypassingBindings(QProperty<T> property, Object val) {
            initialize(property);
            return property.core.setValueBypassingBindings(property, val);
        }
    }
    
    private final static class ValueInitializingPropertyCore<T> extends InitializingPropertyCore<T>{
        public ValueInitializingPropertyCore(T val) {
            super();
            this.val = val;
        }
        
        void initialize(QProperty<T> property) {
            super.initialize(property, val);
        }
    
        private final T val;
    }
    
    private final static class BindingInitializingPropertyCore<T> extends InitializingPropertyCore<T>{
        public BindingInitializingPropertyCore(QUntypedPropertyBinding binding) {
            super();
            this.binding = binding;
        }
        
        void initialize(QProperty<T> property) {
            super.initialize(property, null);
            property.bindingData().setBinding(binding, property);
        }
    
        private final QUntypedPropertyBinding binding;
    }
    
    /**
     * <p>The <code>QProperty</code> class enables automatic property bindings.
     * The Java type <code>QProperty</code> corresponds to the C++ type <code><a href="https://doc.qt.io/qt/qobjectbindableproperty.html">QObjectBindableProperty</a></code>.</p>
     * <p>It is only allowed to use <code>QProperty</code> as <code>final</code>-declared member variable of a <code>QObject</code> subtype.</p>
     * <p>Example:</p>
     * <code>
     * public class MyObject extends QObject{<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;private final QProperty&lt;QColor> color = new QProperty&lt;>();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public final Signal0 colorChanged = new Signal0();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public QColor color(){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return color.value();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public void setColor(QColor color){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;color.setValue(color);<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public QBindable&lt;QColor> bindableColor(){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return new QBindable&lt;>(color);<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * }
     * </code>
     * <p><code>QProperty</code> fields should meet the following naming conventions to let
     * metaobject system identify them as accessible property. For a property called "<code>foo</code>"
     * call the <code>QProperty</code> field either "<code>foo</code>", "<code>fooProp</code>" or "<code>fooProperty</code>".
     * Then, QtJambi will identify the method "<code>public T foo()</code>" as it's getter,
     * "<code>public void setFoo(T)</code>" as it's setter,
     * "<code>public final Signal0 fooChanged</code>" as it's notify signal and 
     * "<code>public QBindable&lt;T> bindableFoo()</code>" as it's bindable supplier.
     * </p>
     * <p>Instead of meeting these naming conventions you can use the <code>io.qt.QtProperty...</code> annotations
     * to make the fields and methods identified as property:</p>
     * <code>
     * public class MyObject extends QObject{<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;@QtPropertyMember(name="color")<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;private final QProperty&lt;QColor> _c = new QProperty&lt;>();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;@QtPropertyNotify(name="color")<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public final Signal0 colorChangeAppeared = new Signal0();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;@QtPropertyReader(name="color")<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public QColor get_color(){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return _c.value();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;@QtPropertyWriter(name="color")<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public void change_color(QColor color){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_c.setValue(color);<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;@QtPropertyBindable(name="color")<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public QBindable&lt;QColor> get_bindable_color(){<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return new QBindable&lt;>(_c);<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
     * }
     * </code>
     * <p>By declaring a <code>QProperty</code> field <code>public</code> 
     * or by using the <code>QtPropertyMember</code> annotation
     * QtJambi identifies a readable, writable and bindable property 
     * without the need to specify getter, setter and bindable methods:</p>
     * <code>
     * public class MyObject extends QObject{<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public final QProperty&lt;QColor> color = new QProperty&lt;>();<br>
     * &nbsp;&nbsp;&nbsp;&nbsp;public final Signal0 colorChanged = new Signal0();<br>
     * }
     * </code>
     * <p>For primitive-typed implementations see:</p>
     * <ul>
     * <li>{@link QBooleanProperty}</li>
     * <li>{@link QByteProperty}</li>
     * <li>{@link QShortProperty}</li>
     * <li>{@link QIntProperty}</li>
     * <li>{@link QLongProperty}</li>
     * <li>{@link QFloatProperty}</li>
     * <li>{@link QDoubleProperty}</li>
     * <li>{@link QCharProperty}</li>
     * </ul>
     */
    public final class QProperty<T> extends QPropertyData<T> {
        
        QObject owner() {
            return QObject.this;
        }
    
        /**
         * <p>Constructs a property whose type is taken from it's field declaration.</p>
         */
        public QProperty() {
            super(null);
            core = new InitializingPropertyCore<>();
        }
    
        /**
         * <p>Constructs a property with the provided <code>initialValue</code>.</p>
         * <p>The property type is taken from it's field declaration. 
         * The <code>initialValue</code> has to be assignable to the property type. Otherwise, a default value is used.</p>
         * @param initialValue
         */
        public QProperty(T initialValue) {
            super(null);
            core = new ValueInitializingPropertyCore<>(initialValue);
        }
        
        /**
         * <p>Constructs a property with the provided binding.</p>
         * <p>The property type is taken from it's field declaration. 
         * The <code>binding</code>'s type has to be assignable to the property type. Otherwise, a default value is used.</p>
         * @param binding
         */
        public QProperty(QUntypedPropertyBinding binding) {
            super(null);
            core = new BindingInitializingPropertyCore<>(binding);
        }
        
        /**
         * <p>Constructs a property bound to the provided <code>functor</code>.</p>
         * <p>The property type is taken from it's field declaration. 
         * The <code>functor</code>'s return type ({@link QtUtilities.Supplier#get()}) has to be assignable to the property type. 
         * Otherwise, a default value is used.</p>
         * @param functor
         */
        public QProperty(QtUtilities.Supplier<T> functor) {
            this(new QPropertyBinding<>(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public T value()
        {
            return core.value(bindingStorage(), this);
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @SuppressWarnings("unchecked")
        @QtUninvokable
        public void setValue(T newValue)
        {
            QMetaType valueMetaType = core.valueMetaType(this);
            if(newValue!=null && !QVariant.canConvert(newValue, valueMetaType))
                throw new ClassCastException("Cannot cast value to "+valueMetaType.name());
            
            if(newValue==null && !valueMetaType.flags().isSet(QMetaType.TypeFlag.IsPointer))
                newValue = (T)valueMetaType.create();
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (core.setValueBypassingBindings(this, newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be equals to the property's type <code>T</code>, 
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @SuppressWarnings("unchecked")
        @QtUninvokable
        public QPropertyBinding<T> setBinding(QPropertyBinding<T> newBinding)
        {
            if(newBinding!=null && !io.qt.core.QProperty.checkType(core.valueMetaType(this), newBinding.valueMetaType()))
                return new QPropertyBinding<>();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QPropertyBinding)
                return (QPropertyBinding<T>)oldBinding;
            else
                return new QPropertyBinding<>(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be equals to the property's type <code>T</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(newBinding!=null && !io.qt.core.QProperty.checkType(core.valueMetaType(this), newBinding.valueMetaType()))
                return false;
            QPropertyBindingData bd = bindingData();
            bd.setBinding(newBinding, this);
            notifyProperty(bd);
            return true;
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link QtUtilities.Supplier#get()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QPropertyBinding<T> setBinding(QtUtilities.Supplier<T> functor)
        {
            return setBinding(new QPropertyBinding<>(functor));
        }
        
        @QtUninvokable
        private QPropertyBinding<T> makeBinding()
        {
            core.initialize(this);
            return new QPropertyBinding<>(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() {
            QPropertyBindingData bd = core.bindingData(bindingStorage(), this, false);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QPropertyBinding&lt;T>} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QPropertyBinding<T> binding()
        {
            return new QPropertyBinding<>(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QPropertyBinding<T> takeBinding()
        {
            return setBinding(new QPropertyBinding<>());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() {
            return core.bindingData(bindingStorage(), this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            core.emitSignal(this);
        }
        
        @QtUninvokable
        private void emitSignal() {
            core.emitSignal(this);
        }
        
        @QtUninvokable
        private boolean hasSignal() {
            return core.hasSignal(this);
        }
        
        QMetaType valueMetaType() {
            return core.valueMetaType(this);
        }
        
        /**
         * {@inheritDoc}
         */
        @QtUninvokable
        @Override
        public final T getValueBypassingBindings() {
            long metaTypeId = checkedNativeId(core.valueMetaType(this));
            return io.qt.core.QProperty.getValueBypassingBindings(nativeId(this), metaTypeId);
        }
        
        /**
         * {@inheritDoc}
         */
        @QtUninvokable
        @Override
        public final boolean setValueBypassingBindings(T val) {
            return core.setValueBypassingBindings(this, val);
        }
        
        private AbstractPropertyCore<T> core;
    }
    
    /**
     * QBooleanProperty is primitive-typed version of QProperty&lt;Boolean&gt;.
     * @see QProperty
     */
    public class QBooleanProperty extends QBooleanPropertyData {
    
        /**
         * Constructs a <code>boolean</code>-typed property.
         */
        public QBooleanProperty() {
            super();
        }
    
        /**
         * Constructs a <code>boolean</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QBooleanProperty(boolean initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>boolean</code>-typed property with the provided binding.
         * @param binding
         */
        public QBooleanProperty(QBooleanPropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>boolean</code>-typed property with the provided binding.
         * @param binding
         */
        public QBooleanProperty(QPropertyBinding<@QtPrimitiveType Boolean> binding) {
            super();
            if(io.qt.core.QBooleanProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>boolean</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QBooleanProperty(java.util.function.BooleanSupplier functor) {
            this(new QBooleanPropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public boolean value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(boolean newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QBooleanPropertyBinding setBinding(QBooleanPropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QBooleanPropertyBinding)
                return (QBooleanPropertyBinding)oldBinding;
            else
                return new QBooleanPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>boolean</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QBooleanPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Boolean> newBinding)
        {
            if(!io.qt.core.QBooleanProperty.checkType(newBinding.valueMetaType()))
                return new QBooleanPropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QBooleanPropertyBinding)
                return (QBooleanPropertyBinding)oldBinding;
            else
                return new QBooleanPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>boolean</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QBooleanProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link java.util.function.BooleanSupplier#getAsBoolean()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QBooleanPropertyBinding setBinding(java.util.function.BooleanSupplier functor)
        {
            return setBinding(new QBooleanPropertyBinding(functor));
        }
        
        @QtUninvokable
        private QBooleanPropertyBinding makeBinding()
        {
            return new QBooleanPropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QBooleanPropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QBooleanPropertyBinding binding()
        {
            return new QBooleanPropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QBooleanPropertyBinding takeBinding()
        {
            return setBinding(new QBooleanPropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QByteProperty is primitive-typed version of QProperty&lt;Byte&gt;.
     * @see QProperty
     */
    public class QByteProperty extends QBytePropertyData {
        
        /**
         * Constructs a <code>byte</code>-typed property.
         */
        public QByteProperty() {
            super();
        }
    
        /**
         * Constructs a <code>byte</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QByteProperty(byte initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>byte</code>-typed property with the provided binding.
         * @param binding
         */
        public QByteProperty(QBytePropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>byte</code>-typed property with the provided binding.
         * @param binding
         */
        public QByteProperty(QPropertyBinding<@QtPrimitiveType Byte> binding) {
            super();
            if(io.qt.core.QByteProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>byte</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QByteProperty(io.qt.QtUtilities.ByteSupplier functor) {
            this(new QBytePropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public byte value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(byte newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QBytePropertyBinding setBinding(QBytePropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QBytePropertyBinding)
                return (QBytePropertyBinding)oldBinding;
            else
                return new QBytePropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>byte</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QBytePropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Byte> newBinding)
        {
            if(!io.qt.core.QByteProperty.checkType(newBinding.valueMetaType()))
                return new QBytePropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QBytePropertyBinding)
                return (QBytePropertyBinding)oldBinding;
            else
                return new QBytePropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>byte</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QByteProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link java.util.function.ByteSupplier#getAsByte()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QBytePropertyBinding setBinding(io.qt.QtUtilities.ByteSupplier functor)
        {
            return setBinding(new QBytePropertyBinding(functor));
        }
        
        @QtUninvokable
        private QBytePropertyBinding makeBinding()
        {
            return new QBytePropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QBytePropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QBytePropertyBinding binding()
        {
            return new QBytePropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QBytePropertyBinding takeBinding()
        {
            return setBinding(new QBytePropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QShortProperty is primitive-typed version of QProperty&lt;Short&gt;.
     * @see QProperty
     */
    public class QShortProperty extends QShortPropertyData {
        
        /**
         * Constructs a <code>short</code>-typed property.
         */
        public QShortProperty() {
            super();
        }
    
        /**
         * Constructs a <code>short</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QShortProperty(short initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>short</code>-typed property with the provided binding.
         * @param binding
         */
        public QShortProperty(QShortPropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>short</code>-typed property with the provided binding.
         * @param binding
         */
        public QShortProperty(QPropertyBinding<@QtPrimitiveType Short> binding) {
            super();
            if(io.qt.core.QShortProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>short</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QShortProperty(QtUtilities.ShortSupplier functor) {
            this(new QShortPropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public short value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(short newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QShortPropertyBinding setBinding(io.qt.QtUtilities.ShortSupplier functor)
        {
            return setBinding(new QShortPropertyBinding(functor));
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>short</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QShortPropertyBinding setBinding(QShortPropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QShortPropertyBinding)
                return (QShortPropertyBinding)oldBinding;
            else
                return new QShortPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>short</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QShortPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Short> newBinding)
        {
            if(!io.qt.core.QShortProperty.checkType(newBinding.valueMetaType()))
                return new QShortPropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QShortPropertyBinding)
                return (QShortPropertyBinding)oldBinding;
            else
                return new QShortPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>short</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QShortProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        @QtUninvokable
        private QShortPropertyBinding makeBinding()
        {
            return new QShortPropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QShortPropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QShortPropertyBinding binding()
        {
            return new QShortPropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QShortPropertyBinding takeBinding()
        {
            return setBinding(new QShortPropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QIntProperty is primitive-typed version of QProperty&lt;Integer&gt;.
     * @see QProperty
     */
    public class QIntProperty extends QIntPropertyData {
        
        /**
         * Constructs an <code>int</code>-typed property.
         */
        public QIntProperty() {
            super();
        }
    
        /**
         * Constructs an <code>int</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QIntProperty(int initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs an <code>int</code>-typed property with the provided binding.
         * @param binding
         */
        public QIntProperty(QIntPropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs an <code>int</code>-typed property with the provided binding.
         * @param binding
         */
        public QIntProperty(QPropertyBinding<@QtPrimitiveType Integer> binding) {
            super();
            if(io.qt.core.QIntProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs an <code>int</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QIntProperty(java.util.function.IntSupplier functor) {
            this(new QIntPropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public int value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(int newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QIntPropertyBinding setBinding(QIntPropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QIntPropertyBinding)
                return (QIntPropertyBinding)oldBinding;
            else
                return new QIntPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>int</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QIntPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Integer> newBinding)
        {
            if(!io.qt.core.QIntProperty.checkType(newBinding.valueMetaType()))
                return new QIntPropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QIntPropertyBinding)
                return (QIntPropertyBinding)oldBinding;
            else
                return new QIntPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>int</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QIntProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link java.util.function.IntSupplier#getAsInt()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QIntPropertyBinding setBinding(java.util.function.IntSupplier functor)
        {
            return setBinding(new QIntPropertyBinding(functor));
        }
        
        @QtUninvokable
        private QIntPropertyBinding makeBinding()
        {
            return new QIntPropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QIntPropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QIntPropertyBinding binding()
        {
            return new QIntPropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QIntPropertyBinding takeBinding()
        {
            return setBinding(new QIntPropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QLongProperty is primitive-typed version of QProperty&lt;Long&gt;.
     * @see QProperty
     */
    public class QLongProperty extends QLongPropertyData {
        
        /**
         * Constructs a <code>long</code>-typed property.
         */
        public QLongProperty() {
            super();
        }
    
        /**
         * Constructs a <code>long</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QLongProperty(long initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>long</code>-typed property with the provided binding.
         * @param binding
         */
        public QLongProperty(QLongPropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>long</code>-typed property with the provided binding.
         * @param binding
         */
        public QLongProperty(QPropertyBinding<@QtPrimitiveType Long> binding) {
            super();
            if(io.qt.core.QLongProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>long</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QLongProperty(java.util.function.LongSupplier functor) {
            this(new QLongPropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public long value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(long newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QLongPropertyBinding setBinding(QLongPropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QLongPropertyBinding)
                return (QLongPropertyBinding)oldBinding;
            else
                return new QLongPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>long</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QLongPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Long> newBinding)
        {
            if(!io.qt.core.QLongProperty.checkType(newBinding.valueMetaType()))
                return new QLongPropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QLongPropertyBinding)
                return (QLongPropertyBinding)oldBinding;
            else
                return new QLongPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>long</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QLongProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link java.util.function.LongSupplier#getAsLong()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QLongPropertyBinding setBinding(java.util.function.LongSupplier functor)
        {
            return setBinding(new QLongPropertyBinding(functor));
        }
        
        @QtUninvokable
        private QLongPropertyBinding makeBinding()
        {
            return new QLongPropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QLongPropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QLongPropertyBinding binding()
        {
            return new QLongPropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QLongPropertyBinding takeBinding()
        {
            return setBinding(new QLongPropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QFloatProperty is primitive-typed version of QProperty&lt;Float&gt;.
     * @see QProperty
     */
    public class QFloatProperty extends QFloatPropertyData {
        
        /**
         * Constructs a <code>float</code>-typed property.
         */
        public QFloatProperty() {
            super();
        }
    
        /**
         * Constructs a <code>float</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QFloatProperty(float initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>float</code>-typed property with the provided binding.
         * @param binding
         */
        public QFloatProperty(QFloatPropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>float</code>-typed property with the provided binding.
         * @param binding
         */
        public QFloatProperty(QPropertyBinding<@QtPrimitiveType Float> binding) {
            super();
            if(io.qt.core.QFloatProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>float</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QFloatProperty(io.qt.QtUtilities.FloatSupplier functor) {
            this(new QFloatPropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public float value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(float newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QFloatPropertyBinding setBinding(QFloatPropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QFloatPropertyBinding)
                return (QFloatPropertyBinding)oldBinding;
            else
                return new QFloatPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>float</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QFloatPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Float> newBinding)
        {
            if(!io.qt.core.QFloatProperty.checkType(newBinding.valueMetaType()))
                return new QFloatPropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QFloatPropertyBinding)
                return (QFloatPropertyBinding)oldBinding;
            else
                return new QFloatPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>float</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QFloatProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link io.qt.QtUtilities.FloatSupplier#getAsFloat()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QFloatPropertyBinding setBinding(io.qt.QtUtilities.FloatSupplier functor)
        {
            return setBinding(new QFloatPropertyBinding(functor));
        }
        
        @QtUninvokable
        private QFloatPropertyBinding makeBinding()
        {
            return new QFloatPropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QFloatPropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QFloatPropertyBinding binding()
        {
            return new QFloatPropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QFloatPropertyBinding takeBinding()
        {
            return setBinding(new QFloatPropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QDoubleProperty is primitive-typed version of QProperty&lt;Double&gt;.
     * @see QProperty
     */
    public class QDoubleProperty extends QDoublePropertyData {
        
        /**
         * Constructs a <code>double</code>-typed property.
         */
        public QDoubleProperty() {
            super();
        }
    
        /**
         * Constructs a <code>double</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QDoubleProperty(double initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>double</code>-typed property with the provided binding.
         * @param binding
         */
        public QDoubleProperty(QDoublePropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>double</code>-typed property with the provided binding.
         * @param binding
         */
        public QDoubleProperty(QPropertyBinding<@QtPrimitiveType Double> binding) {
            super();
            if(io.qt.core.QDoubleProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>double</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QDoubleProperty(java.util.function.DoubleSupplier functor) {
            this(new QDoublePropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public double value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(double newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QDoublePropertyBinding setBinding(QDoublePropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QDoublePropertyBinding)
                return (QDoublePropertyBinding)oldBinding;
            else
                return new QDoublePropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>double</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QDoublePropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Double> newBinding)
        {
            if(!io.qt.core.QDoubleProperty.checkType(newBinding.valueMetaType()))
                return new QDoublePropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QDoublePropertyBinding)
                return (QDoublePropertyBinding)oldBinding;
            else
                return new QDoublePropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>double</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QDoubleProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link java.util.function.DoubleSupplier#getAsDouble()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QDoublePropertyBinding setBinding(java.util.function.DoubleSupplier functor)
        {
            return setBinding(new QDoublePropertyBinding(functor));
        }
        
        @QtUninvokable
        private QDoublePropertyBinding makeBinding()
        {
            return new QDoublePropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QDoublePropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QDoublePropertyBinding binding()
        {
            return new QDoublePropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QDoublePropertyBinding takeBinding()
        {
            return setBinding(new QDoublePropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    /**
     * QCharProperty is primitive-typed version of QProperty&lt;Character&gt;.
     * @see QProperty
     */
    public class QCharProperty extends QCharPropertyData {
        
        /**
         * Constructs a <code>char</code>-typed property.
         */
        public QCharProperty() {
            super();
        }
    
        /**
         * Constructs a <code>char</code>-typed property with the provided initialValue.
         * @param initialValue
         */
        public QCharProperty(char initialValue) {
            super(initialValue);
        }
        
        /**
         * Constructs a <code>char</code>-typed property with the provided binding.
         * @param binding
         */
        public QCharProperty(QCharPropertyBinding binding) {
            super();
            bindingData().setBinding(binding, this);
        }
        
        /**
         * Constructs a <code>char</code>-typed property with the provided binding.
         * @param binding
         */
        public QCharProperty(QPropertyBinding<@QtPrimitiveType Character> binding) {
            super();
            if(io.qt.core.QCharProperty.checkType(binding.valueMetaType()))
                bindingData().setBinding(binding, this);
        }
        
        /**
         * <p>Constructs a <code>char</code>-typed property bound to the provided <code>functor</code>.</p>
         * @param functor
         */
        public QCharProperty(io.qt.QtUtilities.CharSupplier functor) {
            this(new QCharPropertyBinding(functor));
        }
        
        /**
         * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
         * @return value
         */
        @QtUninvokable
        public char value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return this.getValueBypassingBindings();
        }
        
        /**
         * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
         * @param newValue
         */
        @QtUninvokable
        public void setValue(char newValue)
        {
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            if(bd!=null)
                bd.removeBinding();
            if (this.setValueBypassingBindings(newValue))
                notifyProperty(bd);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QCharPropertyBinding setBinding(QCharPropertyBinding newBinding)
        {
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QCharPropertyBinding)
                return (QCharPropertyBinding)oldBinding;
            else
                return new QCharPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>char</code>,
         * otherwise the property remains unchanged.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param newBinding
         * @return oldBinding
         */
        @QtUninvokable
        public QCharPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Character> newBinding)
        {
            if(!io.qt.core.QCharProperty.checkType(newBinding.valueMetaType()))
                return new QCharPropertyBinding();
            QPropertyBindingData bd = bindingData();
            QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
            notifyProperty(bd);
            if(oldBinding instanceof QCharPropertyBinding)
                return (QCharPropertyBinding)oldBinding;
            else
                return new QCharPropertyBinding(oldBinding);
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
         * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>char</code>, 
         * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
         * <p>The first time the property value is read, the binding is evaluated. 
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
         * @param newBinding
         * @return true if types match, false otherwise.
         */
        @QtUninvokable
        public boolean setBinding(QUntypedPropertyBinding newBinding)
        {
            if(io.qt.core.QCharProperty.checkType(newBinding.valueMetaType())) {
                QPropertyBindingData bd = bindingData();
                bd.setBinding(newBinding, this);
                notifyProperty(bd);
                return true;
            }else {
                return false;
            }
        }
        
        /**
         * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
         * <p>The first time the property value is read, the binding is evaluated by invoking {@link io.qt.QtUtilities.CharSupplier#getAsChar()} of <code>functor</code>.
         * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
         * @param functor
         * @return oldBinding
         */
        @QtUninvokable
        public QCharPropertyBinding setBinding(io.qt.QtUtilities.CharSupplier functor)
        {
            return setBinding(new QCharPropertyBinding(functor));
        }
        
        @QtUninvokable
        private QCharPropertyBinding makeBinding()
        {
            return new QCharPropertyBinding(this::value);
        }
        
        /**
         * Checks if the property has a binding.
         * @return true if the property has a binding, false otherwise.
         */
        @QtUninvokable
        public boolean hasBinding() { 
            QPropertyBindingData bd = bindingStorage().bindingData(this);
            return bd!=null && bd.hasBinding(); 
        }
        
        /**
         * Returns the binding expression that is associated with this property. 
         * A default constructed {@link QCharPropertyBinding} will be returned if no such association exists.
         * @return binding
         */
        @QtUninvokable
        public QCharPropertyBinding binding()
        {
            return new QCharPropertyBinding(this);
        }
        
        /**
         * <p>Disassociates the binding expression from this property and returns it.</p>
         * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
         * @return the removed binding
         */
        @QtUninvokable
        public QCharPropertyBinding takeBinding()
        {
            return setBinding(new QCharPropertyBinding());
        }
        
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        private void notifyProperty(QPropertyBindingData binding){
            if (binding!=null)
                binding.notifyObservers(this);
            emitSignal();
        }
        
        private Runnable signal = () -> {
            io.qt.internal.QtJambiPropertyInfo result = QMetaObject.AbstractSignal.analyzeProperty(QObject.this, this);
            QMetaMethod notifySignal;
            if(result.property!=null) {
                notifySignal = result.notifySignal;
            }else {
                notifySignal = findNotifySignalByBindables(QObject.this, result.reflectedField, this);
            }
            if(notifySignal==null) {
                this.signal = NO_SIGNAL;
            }else{
                int methodIndex = notifySignal.methodIndex();
                this.signal = new EmitSignal(methodIndex);
            }
            this.signal.run();
        };
        
        @QtUninvokable
        private boolean hasSignal() {
            return this.signal != NO_SIGNAL;
        }
        
        @QtUninvokable
        private void emitSignal() {
            this.signal.run();
        }
    }
    
    private abstract class QAbstractComputedProperty extends QUntypedPropertyData {
        @QtUninvokable
        public final boolean hasBinding() { return false; }
                
        /**
         * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
         * <p>The returned property change handler object keeps track of the registration. 
         * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
         * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         */
        @QtUninvokable
        public final QPropertyChangeHandler onValueChanged(Runnable f)
        {
            return new QPropertyChangeHandler(bindingData(), f);
        }
    
        /**
         * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
         * @param f
         * @return property change handler
         * @see QPropertyChangeHandler
         * @see #onValueChanged(Runnable)
         */
        @QtUninvokable
        public final QPropertyChangeHandler subscribe(Runnable f)
        {
            f.run();
            return onValueChanged(f);
        }
        
        @QtUninvokable
        final QPropertyBindingData bindingData() { 
            return bindingStorage().bindingData(this, true);
        }
        
        @QtUninvokable
        public abstract QMetaType valueMetaType();
    }
    
    /**
     * <p>The <code>QComputedProperty</code> class is a template class for externally computed/stored values.
     * Create an instance of <code>QComputedProperty</code> by providing a value supplier.</p>
     * <p>Example:</p>
     * <code>
     * private final QComputedProperty&lt;QColor> color = new QComputedProperty&lt;>(this::computeOptimalColor);
     * </code>
     * <p>The Java type <code>QComputedProperty</code> corresponds to the C++ type <code>QObjectComputedProperty</code>.</p>
     * <p>For primitive-typed implementations see:</p>
     * <ul>
     * <li>{@link QComputedBooleanProperty}</li>
     * <li>{@link QComputedByteProperty}</li>
     * <li>{@link QComputedShortProperty}</li>
     * <li>{@link QComputedIntProperty}</li>
     * <li>{@link QComputedLongProperty}</li>
     * <li>{@link QComputedFloatProperty}</li>
     * <li>{@link QComputedDoubleProperty}</li>
     * <li>{@link QComputedCharProperty}</li>
     * </ul>
     */
    public final class QComputedProperty<T> extends QAbstractComputedProperty {
        
        private final QtUtilities.Supplier<T> getter;
        
        /**
         * Constructs a computed-value property whose type corresponds to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedProperty(QtUtilities.Supplier<T> getter) {
            super();
            QPropertyBinding.analyzeMetaType(getter);
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public T value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.get();
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return QPropertyBinding.analyzeMetaType(getter);
        }
    }
    
    /**
     * QComputedBooleanProperty is primitive-typed version of QComputedProperty&lt;Boolean&gt;.
     * @see QComputedProperty
     */
    public class QComputedBooleanProperty extends QAbstractComputedProperty {
        
        private final java.util.function.BooleanSupplier getter;
    
        /**
         * Constructs a <code>boolean</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedBooleanProperty(java.util.function.BooleanSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public boolean value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsBoolean();
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.Bool);
        }
    }
    
    /**
     * QComputedByteProperty is primitive-typed version of QComputedProperty&lt;Byte&gt;.
     * @see QComputedProperty
     */
    public class QComputedByteProperty extends QAbstractComputedProperty {
        
        private final QtUtilities.ByteSupplier getter;
    
        /**
         * Constructs a <code>byte</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedByteProperty(QtUtilities.ByteSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public byte value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsByte();
        }
        
        @QtUninvokable
        private QBytePropertyBinding makeBinding()
        {
            return new QBytePropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.SChar);
        }
    }
    
    /**
     * QComputedShortProperty is primitive-typed version of QComputedProperty&lt;Short&gt;.
     * @see QComputedProperty
     */
    public class QComputedShortProperty extends QAbstractComputedProperty {
        
        private final QtUtilities.ShortSupplier getter;
    
        /**
         * Constructs a <code>short</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedShortProperty(QtUtilities.ShortSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public short value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsShort();
        }
        
        @QtUninvokable
        private QShortPropertyBinding makeBinding()
        {
            return new QShortPropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.Short);
        }
    }
    
    /**
     * QComputedIntProperty is primitive-typed version of QComputedProperty&lt;Integer&gt;.
     * @see QComputedProperty
     */
    public class QComputedIntProperty extends QAbstractComputedProperty {
        
        private final java.util.function.IntSupplier getter;
    
        /**
         * Constructs a <code>int</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedIntProperty(java.util.function.IntSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public int value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsInt();
        }
        
        @QtUninvokable
        private QIntPropertyBinding makeBinding()
        {
            return new QIntPropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.Int);
        }
    }
    
    /**
     * QComputedLongProperty is primitive-typed version of QComputedProperty&lt;Long&gt;.
     * @see QComputedProperty
     */
    public class QComputedLongProperty extends QAbstractComputedProperty {
        
        private final java.util.function.LongSupplier getter;
    
        /**
         * Constructs a <code>long</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedLongProperty(java.util.function.LongSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public long value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsLong();
        }
        
        @QtUninvokable
        private QLongPropertyBinding makeBinding()
        {
            return new QLongPropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.LongLong);
        }
    }
    
    /**
     * QComputedFloatProperty is primitive-typed version of QComputedProperty&lt;Float&gt;.
     * @see QComputedProperty
     */
    public class QComputedFloatProperty extends QAbstractComputedProperty {
        
        private final QtUtilities.FloatSupplier getter;
    
        /**
         * Constructs a <code>float</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedFloatProperty(QtUtilities.FloatSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public float value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsFloat();
        }
        
        @QtUninvokable
        private QFloatPropertyBinding makeBinding()
        {
            return new QFloatPropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.Float);
        }
    }
    
    /**
     * QComputedDoubleProperty is primitive-typed version of QComputedProperty&lt;Double&gt;.
     * @see QComputedProperty
     */
    public class QComputedDoubleProperty extends QAbstractComputedProperty {
        
        private final java.util.function.DoubleSupplier getter;
    
        /**
         * Constructs a <code>double</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedDoubleProperty(java.util.function.DoubleSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public double value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsDouble();
        }
        
        @QtUninvokable
        private QDoublePropertyBinding makeBinding()
        {
            return new QDoublePropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.Double);
        }
    }
    
    /**
     * QComputedCharProperty is primitive-typed version of QComputedProperty&lt;Character&gt;.
     * @see QComputedProperty
     */
    public class QComputedCharProperty extends QAbstractComputedProperty {
        
        private final QtUtilities.CharSupplier getter;
    
        /**
         * Constructs a <code>char</code>-typed computed-value property according to the given value supplier.
         * @param getter supplying the computed value
         */
        public QComputedCharProperty(QtUtilities.CharSupplier getter) {
            super();
            this.getter = getter;
        }
    
        /**
         * Returns the value of the property computed by the given getter.
         * @return value
         */
        @QtUninvokable
        public char value()
        {
            bindingStorage().maybeUpdateBindingAndRegister(this);
            return getter.getAsChar();
        }
        
        @QtUninvokable
        private QCharPropertyBinding makeBinding()
        {
            return new QCharPropertyBinding(this::value);
        }
        
        @QtUninvokable
        public final QMetaType valueMetaType() {
            return new QMetaType(QMetaType.Type.QChar);
        }
    }

}// class
    
class QObject_5__ extends QObject {
    /**
     * <p>This function overloads {@link #findChildren()}.</p>
     * <p>Returns the children of this object that are instance of <i>cl</i> 
     * and that have names matching the regular expression <i>regExp</i>, 
     * or an empty list if there are no such objects. 
     * The search is performed recursively, unless <i>options</i> specifies the option <i>FindDirectChildrenOnly</i>.</p>
     * @deprecated Use {@link #findChildren(Class, QRegularExpression, Qt.FindChildOptions)} instead.
     * @param <T> type of children
     * @param cl type of children
     * @param regExp regular expression
     * @param options search options
     * @return found children
     * @see #findChildren()
     */
    @Deprecated
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, QRegExp regExp, Qt.FindChildOptions options){
        return findChildrenQRegExp(nativeId(this), QMetaObject.forType(java.util.Objects.requireNonNull(cl)).metaObjectPointer, nativeId(regExp), options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> QList<T> findChildrenQRegExp(long nativeId, long metaObjectPointer, long regExp, int options);
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,QRegExp,Qt.FindChildOptions)}.</p>
     * @deprecated Use {@link #findChildren(Class, QRegularExpression, Qt.FindChildOptions)} instead.
     * @param <T> type of children
     * @param cl type of children
     * @param regExp regular expression
     * @return found children
     */
    @Deprecated
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, QRegExp regExp) {
        return findChildren(cl, regExp, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    /**
     * <p>Overloaded function for {@link #findChildren(Class,QRegExp,Qt.FindChildOptions)}
     * with: <code>options = FindChildrenRecursively</code>.</p>
     * @deprecated Use {@link #findChildren(Class, QRegularExpression, Qt.FindChildOptions)} instead.
     * @param <T> type of children
     * @param cl type of children
     * @param regExp regular expression
     * @param options search options
     * @return found children
     */
    @Deprecated
    @io.qt.QtUninvokable
    public final <T extends QObject> QList<T> findChildren(Class < T > cl, QRegExp regExp, Qt.FindChildOption... options) {
        return findChildren(cl, regExp, new Qt.FindChildOptions(options));
    }
    
}// class

class QSignalTransition___{
    
    /**
     * Overloaded constructor for {@link #QSignalTransition(io.qt.core.QMetaObject.AbstractSignal, io.qt.core.QState)}.
     */
    public QSignalTransition(io.qt.core.QMetaObject.AbstractSignal signal) {
        this(signal, null);
    }
    
    /**
     * Constructs a new signal transition associated with the given signal of the given sender object and with the given sourceState.
     */
    public QSignalTransition(io.qt.core.QMetaObject.AbstractSignal signal, io.qt.core.QState sourceState) {
        super((QPrivateConstructor)null);
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            String signalSignature = cppSignalSignature(signal);
            __qt_QSignalTransition_new_const_QObject_ptr_const_char_ptr_QState_ptr(this, sender, signalSignature, sourceState);
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }
    
    /**
     * Sets the signal associated with this signal transition.
     */
    @io.qt.QtUninvokable
    public final void setSignal(io.qt.core.QMetaObject.AbstractSignal signal){
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            String signalSignature = cppSignalSignature(signal);
            __qt_QSignalTransition_setSenderObject_const_QObject_ptr(nativeId(this), checkedNativeId(sender));
            __qt_QSignalTransition_setSignal_cref_QByteArray(nativeId(this), nativeId(new QByteArray(signalSignature)));
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }
    
}// class

class QState___{
    
    /**
     * Adds a transition associated with the given signal of the given sender object, and returns the new QSignalTransition object. 
     * The transition has this state as the source, and the given target as the target state.
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QSignalTransition addTransition(io.qt.core.QMetaObject.AbstractSignal signal, io.qt.core.QAbstractState target) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            String signalSignature = cppSignalSignature(signal);
            return __qt_QState_addTransition_const_QObject_ptr_const_char_ptr_QAbstractState_ptr(nativeId(this), checkedNativeId(sender), signalSignature, checkedNativeId(target));
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }
    
}// class

abstract class QUrl___ extends QUrl{
    
    /**
     * Supertype interface for {@link UrlFormattingOption} and {@link ComponentFormattingOption}
     * to be joined in QFiags type {@link FormattingOptions}
     */
    public interface FormattingOption extends io.qt.QtFlagEnumerator{
        
        public final static FormattingOption None                   = UrlFormattingOption.None;
        public final static FormattingOption RemoveScheme           = UrlFormattingOption.RemoveScheme;
        public final static FormattingOption RemovePassword         = UrlFormattingOption.RemovePassword;
        public final static FormattingOption RemoveUserInfo         = UrlFormattingOption.RemoveUserInfo;
        public final static FormattingOption RemovePort             = UrlFormattingOption.RemovePort;
        public final static FormattingOption RemoveAuthority        = UrlFormattingOption.RemoveAuthority;
        public final static FormattingOption RemovePath             = UrlFormattingOption.RemovePath;
        public final static FormattingOption RemoveQuery            = UrlFormattingOption.RemoveQuery;
        public final static FormattingOption RemoveFragment         = UrlFormattingOption.RemoveFragment;
        public final static FormattingOption PreferLocalFile        = UrlFormattingOption.PreferLocalFile;
        public final static FormattingOption StripTrailingSlash     = UrlFormattingOption.StripTrailingSlash;
        public final static FormattingOption RemoveFilename         = UrlFormattingOption.RemoveFilename;
        public final static FormattingOption NormalizePathSegments  = UrlFormattingOption.NormalizePathSegments;
        public final static FormattingOption EncodeSpaces           = ComponentFormattingOption.EncodeSpaces;
        public final static FormattingOption EncodeUnicode          = ComponentFormattingOption.EncodeUnicode;
        public final static FormattingOption EncodeDelimiters       = ComponentFormattingOption.EncodeDelimiters;
        public final static FormattingOption EncodeReserved         = ComponentFormattingOption.EncodeReserved;
        public final static FormattingOption DecodeReserved         = ComponentFormattingOption.DecodeReserved;
        public final static FormattingOption FullyEncoded           = ComponentFormattingOption.FullyEncoded;
        public final static FormattingOption FullyDecoded           = ComponentFormattingOption.FullyDecoded;
            
        public static FormattingOptions flags(FormattingOption... values) {
            return new FormattingOptions(values);
        }
        
        public default io.qt.QFlags<?> asFlags() {
            return new FormattingOptions(value());
        }
        
        public static FormattingOption resolve(int value) {
            try {
                return UrlFormattingOption.resolve(value);
            }catch(io.qt.QNoSuchEnumValueException e){
                return ComponentFormattingOption.resolve(value);
            }
        }
        
        public static FormattingOption[] values() {
            UrlFormattingOption[] v1 = UrlFormattingOption.values();
            ComponentFormattingOption[] v2 = ComponentFormattingOption.values();
            FormattingOption[] values = new FormattingOption[v1.length + v2.length];
            System.arraycopy(v1, 0, values, 0, v1.length);
            System.arraycopy(v2, 0, values, v1.length, v2.length);
            return values;
        }
    }
    
    /**
     * QFiags type for joining {@link UrlFormattingOption} and {@link ComponentFormattingOption}.
     */
    public static class FormattingOptions extends io.qt.QFlags<FormattingOption> {
        private static final long serialVersionUID = -4458464052834800982L;

        public FormattingOptions(FormattingOption ... args) {
            super(args); 
            for (FormattingOption arg : args) {
                if( !(arg instanceof UrlFormattingOption) 
                        && !(arg instanceof ComponentFormattingOption) ) {
                    throw new IllegalArgumentException("Custom implementations of FormattingOption (" + arg.getClass().getName() + ") are not allowed.");
                }
            }
        }
        
        public FormattingOptions(int value) { 
            super(value); 
        }
        
        @Override
        public FormattingOptions clone() {
            return new FormattingOptions(value());
        }
        
        @Override
        public FormattingOptions combined(FormattingOption flag) {
            if( !(flag instanceof UrlFormattingOption) 
                    && !(flag instanceof ComponentFormattingOption) ) {
                throw new IllegalArgumentException("Custom implementations of FormattingOption (" + flag.getClass().getName() + ") are not allowed.");
            }
            return new FormattingOptions(value() | flag.value());
        }
        
        @Override
        public FormattingOption[] flags() {
            return super.flags(FormattingOption.values());
        }
    }

    @io.qt.QtUninvokable
    public final java.lang.String toDisplayString()    {
        return toDisplayString(ComponentFormattingOption.PrettyDecoded.value());
    }

    @io.qt.QtUninvokable
    public final java.lang.String toDisplayString(FormattingOption... options)    {
        return toDisplayString(new FormattingOptions(options).value());
    }

    @io.qt.QtUninvokable
    public final java.lang.String toDisplayString(FormattingOptions options)    {
        return toDisplayString(options.value());
    }

    @io.qt.QtUninvokable
    public final java.lang.String toString(FormattingOptions options)    {
        return toString(options.value());
    }
    
    @io.qt.QtUninvokable
    public final java.lang.String toString(FormattingOption... options)    {
        return toString(new FormattingOptions(options).value());
    }
    
    @io.qt.QtUninvokable
    public final java.lang.String toString()    {
        return toString(ComponentFormattingOption.PrettyDecoded.value());
    }

    @io.qt.QtUninvokable
    public final java.lang.String url()    {
        return url(ComponentFormattingOption.PrettyDecoded.value());
    }

    @io.qt.QtUninvokable
    public final java.lang.String url(FormattingOptions options)    {
        return url(options.value());
    }

    @io.qt.QtUninvokable
    public final java.lang.String url(FormattingOption... options)    {
        return url(new FormattingOptions(options).value());
    }

    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray toEncoded()    {
        return toEncoded(ComponentFormattingOption.PrettyDecoded.value());
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray toEncoded(FormattingOption... options)    {
        return toEncoded(new FormattingOptions(options).value());
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray toEncoded(FormattingOptions options)    {
        return toEncoded(options.value());
    }

    public static QStringList toStringList(java.util.Collection<io.qt.core.QUrl> uris, FormattingOptions options){
        return toStringList(uris, options.value());
    }
    
    public static QStringList toStringList(java.util.Collection<io.qt.core.QUrl> uris, FormattingOption... options){
        return toStringList(uris, new FormattingOptions(options).value());
    }
    
    public static QStringList toStringList(java.util.Collection<io.qt.core.QUrl> uris){
        return toStringList(uris, ComponentFormattingOption.PrettyDecoded.value());
    }
    
    public static io.qt.core.QUrl fromClassPath(java.lang.String classspath){
        return fromLocalFile("/:classpath:"+classspath);
    }
}// class

abstract class QAbstractItemModel___ extends QAbstractItemModel {
    
        private native boolean setData_native(long id, int row, int col, Object value, int role);

        public final boolean setData(int row, int col, Object value) {
            return setData_native(nativeId(this), row, col, value, io.qt.core.Qt.ItemDataRole.DisplayRole);
        }

        public final boolean setData(int row, int col, Object value, int role) {
            return setData_native(nativeId(this), row, col, value, role);
        }

        private native Object data_native(long id, int row, int col, int role);

        public final Object data(int row, int col, int role) {
            return data_native(nativeId(this), row, col, role);
        }

        public final Object data(int row, int col) {
            return data_native(nativeId(this), row, col, Qt.ItemDataRole.DisplayRole);
        }
        
        @io.qt.QtUninvokable
        protected final io.qt.core.QModelIndex createIndex(int row, int column){
            return createIndex(row, column, 0);
        }
}// class

class QTimer___ extends QTimer {
    
    private static class QSingleShotTimer extends QObject {
        private int timerId;
        public final Signal0 timeout = new Signal0();
        private final QObject receiver;
        private final QMetaObject.Slot0 slot;

        private QSingleShotTimer(int msec, Qt.TimerType timeType, QObject obj, String method) {
            super(QAbstractEventDispatcher.instance());
            this.slot = null;
            receiver = obj;
            if(QAbstractEventDispatcher.instance()==null){
                dispose();
                return;
            }
            setCppOwnership(this);
            timeout.connect(obj, method);
            timerId = startTimer(msec, timeType);
        }

        private QSingleShotTimer(int msec, Qt.TimerType timeType, QObject obj, QMetaObject.Slot0 slot) {
            super(QAbstractEventDispatcher.instance());
            this.slot = slot;
            receiver = obj;
            if(obj!=null && thread()!=obj.thread()) {
                QCoreApplication.instance().aboutToQuit.connect(this::disposeLater);
                setParent(null);
                this.moveToThread(obj.thread());
            }else if(QAbstractEventDispatcher.instance()==null){
                dispose();
                return;
            }
            setCppOwnership(this);
            timerId = startTimer(msec, timeType);
        }
    
        protected void timerEvent(QTimerEvent e) {
            if (timerId > 0)
                killTimer(timerId);
            timerId = -1;
            if(slot!=null) {
                if(receiver==null || !receiver.isDisposed()) {
                    try {
                        slot.invoke();
                    } catch (Throwable e1) {
                    }
                }
            }else {
                timeout.emit();
            }
            disposeLater();
        }
    }

    /**
     * This static function calls a slot after a given time interval.
     * 
     * It is very convenient to use this function because you do not need
     * to bother with a timerEvent or create a local QTimer object.
     * 
     * The receiver is the receiving object and the member is the slot. The
     * time interval is msec milliseconds.
     * 
     * @see #start()
     */
     public static void singleShot(int msec, QObject obj, String method) {
        singleShot(msec, defaultTypeFor(msec), obj, method);
    }

    /**
     * This static function calls slot after a given time interval.
     * 
     * It is very convenient to use this function because you do not need
     * to bother with a timerEvent or create a local QTimer object.
     * 
     * The time interval is msec milliseconds.
     * 
     * If context is destroyed before the interval occurs, the method will not be called.
     * The function will be run in the thread of context. The context's thread must have
     * a running Qt event loop.
     * 
     * @see #start()
     * @since 5.4
     */    
    public static void singleShot(int msec, QObject context, QMetaObject.Slot0 slot) {
        singleShot(msec, defaultTypeFor(msec), context, slot);
    }

    /**
     * This static function calls a slot after a given time interval.
     * 
     * It is very convenient to use this function because you do not need
     * to bother with a timerEvent or create a local QTimer object.
     * 
     * The receiver is the receiving object and the member is the slot. The
     * time interval is msec milliseconds. The timerType affects the
     * accuracy of the timer.
     * 
     * @see #start()
     */
    public static void singleShot(int msec, Qt.TimerType timeType, QObject obj, String method) {
        new QSingleShotTimer(msec, timeType, obj, method);
    }

    /**
     * This static function calls slot after a given time interval.
     * 
     * It is very convenient to use this function because you do not need
     * to bother with a timerEvent or
     * create a local QTimer object.
     * 
     * The time interval is msec milliseconds.
     * 
     * @see #start()
     * @since 5.4
     */
    public static void singleShot(int msec, QMetaObject.Slot0 slot) {
        singleShot(msec, defaultTypeFor(msec), null, slot);
    }

    /**
     * This static function calls slot after a given time interval.
     * 
     * It is very convenient to use this function because you do not need
     * to bother with a timerEvent or
     * create a local QTimer object.
     * 
     * The time interval is msec milliseconds. The timerType affects the
     * accuracy of the timer.
     * 
     * @see #start()
     * @since 5.4
     */
    public static void singleShot(int msec, Qt.TimerType timeType, QMetaObject.Slot0 slot) {
        singleShot(msec, timeType, null, slot);
    }

    /**
     * This static function calls slot after a given time interval.
     * 
     * It is very convenient to use this function because you do not need
     * to bother with a timerEvent or
     * create a local QTimer object.
     * 
     * The time interval is msec milliseconds. The timerType affects the
     * accuracy of the timer.
     * 
     * If context is destroyed before the interval occurs, the method will not be called.
     * The function will be run in the thread of context. The context's thread must have
     * a running Qt event loop.
     * 
     * @see #start()
     * @since 5.4
     */
    public static void singleShot(int msec, Qt.TimerType timeType, QObject context, QMetaObject.Slot0 slot) {
        new QSingleShotTimer(msec, timeType, context, slot);
    }
    
    private static Qt.TimerType defaultTypeFor(int msecs){ 
        return msecs >= 2000 ? Qt.TimerType.CoarseTimer : Qt.TimerType.PreciseTimer; 
    }
    
}// class

class QCoreApplication___ extends QCoreApplication {
    
    private static boolean __qt_isInitializing = false;

    private static native void preinit();

    /**
     * <p>Overloaded function for {@link #sendPostedEvents(io.qt.core.QObject, int)}</p>
     */
    public static void sendPostedEvents(io.qt.core.QObject receiver, QEvent.Type event_type){
        sendPostedEvents(receiver, event_type.value());
    }

    /**
     * Initializes the QCoreApplication with the given arguments.
     * @param args
     * @return QCoreApplication instance
     */
    public static QCoreApplication initialize(String args[]) {
        return initialize(null, args, QCoreApplication::new);
    }

    /**
     * Initializes the QCoreApplication with the given arguments and applicationName.
     * @param applicationName
     * @param args
     * @return QCoreApplication instance
     */
    public static QCoreApplication initialize(String applicationName, String args[]) {
        return initialize(applicationName, args, QCoreApplication::new);
    }

    /**
     * Initializes a custom QCoreApplication subclass with the given arguments.
     * @param <T> QCoreApplication subclass
     * @param args
     * @param constructor
     * @return T instance
     */
    public static <T extends QCoreApplication> T initialize(String args[], java.util.function.Function<String[],T> constructor) {
        return initialize(null, args, constructor);
    }
    
    /**
     * Initializes a custom QCoreApplication subclass with the given arguments and applicationName.
     * @param <T> QCoreApplication subclass
     * @param applicationName
     * @param args
     * @param constructor
     * @return T instance
     */
    public static <T extends QCoreApplication> T initialize(String applicationName, String args[], java.util.function.Function<String[],T> constructor) {
        if (instance() != null)
            throw new RuntimeException("QCoreApplication can only be initialized once");
        preinit();
        String[] _args = new String[args.length+1];
        if(applicationName!=null && !applicationName.isEmpty()){
            _args[0] = applicationName;
        }else{
            _args[0] = applicationName();
        }
        System.arraycopy(args, 0, _args, 1, args.length);
        args = _args;
        __qt_isInitializing = true;
        try {
            T application = java.util.Objects.requireNonNull(constructor.apply(args), "Constructor function does not instantiate QCoreApplication.");
            setCppOwnership(application);
            return application;
        } catch (Error e) {
            throw e;
        } catch (RuntimeException e) {
            throw e;
        } catch (Throwable e) {
            throw new RuntimeException("Cannot initialize QCoreApplication", e);
        }finally{
            __qt_isInitializing = false;
        }
    }

    /**
     * Destroys the QCoreApplication instance and purges Qt.
     */
    public static void shutdown() {
        QCoreApplication app = instance();
        if(app != null) {
            app.disposeLater();
            app = null;        // discard hard-reference
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());    // allow deleteLater() to work some magic
            quit();            // finish up cause an aboutToQuit() to fire
            processEvents();    // process quit
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());    // allow deleteLater() to work some magic
        }
        if (instance() != null)    // this should be a QtJambi qWarning()
            java.util.logging.Logger.getLogger("io.qt.core").log(java.util.logging.Level.WARNING, "WARNING: QCoreApplication.shutdown() QCoreApplication.instance()!=null");
        System.gc();
        System.runFinalization();
    }

    /**
     * Enters the main event loop and waits until exit() is called. 
     * 
     * @return the value that was passed to exit() (which is 0 if exit() is called via quit()).
     */
    public static int exec() {
        QCoreApplication instance = QCoreApplication.instance();
        if (instance == null)
            throw new RuntimeException("QCoreApplication has not been initialized with QCoreApplication.initialize()");
        else if(instance.thread()!=QThread.currentThread())
            throw new RuntimeException("exec() must be called from the main thread.");
        else if(io.qt.internal.QtJambiInternal.countEventLoops(instance.thread())>0)
            throw new RuntimeException("The event loop is already running.");
        else if(instance instanceof io.qt.widgets.QApplication)
            return io.qt.widgets.QApplication.exec();
        else if(instance instanceof io.qt.gui.QGuiApplication)
            return io.qt.gui.QGuiApplication.exec();
        else 
            return exec_internal();
    }
    
    /**
     * Adds a pre-routine to be executed before initializing QCoreApplication.
     * @see initialize(String[])
     */
    public native static void addPreRoutine(Runnable startUpFunction);
    
    /**
     * Adds a post-routine to be executed when deleting QCoreApplication.
     * @see shutdown()
     */
    public native static void addPostRoutine(Runnable cleanUpFunction);
    
    /**
     * Removes a previously added post-routine.
     * @see addPostRoutine(Runnable)
     * @see shutdown()
     */
    public native static void removePostRoutine(Runnable cleanUpFunction);
}// class

class QTranslator___ extends QTranslator {
}// class

class QItemSelection_6__ extends QItemSelection {
    
    public QItemSelection() {
        super((QPrivateConstructor)null);
        __qt_QItemSelection_new(this, null);
    }
    
}// class

class QItemSelection___ extends QItemSelection {
    
    public QItemSelection(java.util.Collection<QItemSelectionRange> other) {
        super((QPrivateConstructor)null);
        __qt_QItemSelection_new(this, other);
    }
    
    private native static <T> void __qt_QItemSelection_new(Object instance, java.util.Collection<QItemSelectionRange> other);
}// class

class QXmlStreamAttributes___ extends QXmlStreamAttributes {
    
    public QXmlStreamAttributes(java.util.Collection<io.qt.core.QXmlStreamAttribute> collection){
        this();
        append(collection);
    }
}// class

class autoclosedelete {
    @Override
    public void close(){
        dispose();
    }
}// class

class QDebug___ extends QDebug {
    
    /**
     * <p>See <a href="@docRoot/qt.html#endl">Qt::endl</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug endl(){
        endl(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void endl(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#flush">Qt::flush</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug flush(){
        flush(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void flush(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#reset">Qt::reset</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug reset(){
        reset(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void reset(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#bin">Qt::bin</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug bin(){
        bin(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void bin(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#oct">Qt::oct</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug oct(){
        oct(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void oct(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#hex">Qt::hex</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug hex(){
        hex(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void hex(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#dec">Qt::dec</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug dec(){
        dec(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void dec(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#showbase">Qt::showbase</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug showbase(){
        showbase(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void showbase(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#forcesign">Qt::forcesign</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug forcesign(){
        forcesign(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void forcesign(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#forcepoint">Qt::forcepoint</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug forcepoint(){
        forcepoint(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void forcepoint(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#noshowbase">Qt::noshowbase</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug noshowbase(){
        noshowbase(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void noshowbase(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#noforcesign">Qt::noforcesign</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug noforcesign(){
        noforcesign(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void noforcesign(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#noforcepoint">Qt::noforcepoint</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug noforcepoint(){
        noforcepoint(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void noforcepoint(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#uppercasebase">Qt::uppercasebase</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug uppercasebase(){
        uppercasebase(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void uppercasebase(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#uppercasedigits">Qt::uppercasedigits</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug uppercasedigits(){
        uppercasedigits(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void uppercasedigits(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#lowercasebase">Qt::lowercasebase</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug lowercasebase(){
        lowercasebase(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void lowercasebase(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#lowercasedigits">Qt::lowercasedigits</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug lowercasedigits(){
        lowercasedigits(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void lowercasedigits(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#fixed">Qt::fixed</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug fixed(){
        fixed(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void fixed(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#scientific">Qt::scientific</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug scientific(){
        scientific(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void scientific(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#left">Qt::left</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug left(){
        left(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void left(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#right">Qt::right</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug right(){
        right(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void right(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#center">Qt::center</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug center(){
        center(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void center(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#bom">Qt::bom</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug bom(){
        bom(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void bom(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qt.html#ws">Qt::ws</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug ws(){
        ws(nativeId(this));
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void ws(long __this__nativeId);
    
    /**
     * <p>See <a href="@docRoot/qtextstream.html#qSetFieldWidth">qSetFieldWidth(int width)</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug qSetFieldWidth(int width){
        setFieldWidth(nativeId(this), width);
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void setFieldWidth(long __this__nativeId, int width);
    
    /**
     * <p>See <a href="@docRoot/qtextstream.html#qSetRealNumberPrecision">qSetRealNumberPrecision(int precision)</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug qSetRealNumberPrecision(int precision){
        setRealNumberPrecision(nativeId(this), precision);
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void setRealNumberPrecision(long __this__nativeId, int precision);
    
    /**
     * <p>See <a href="@docRoot/qtextstream.html#qSetPadChar">qSetPadChar(QChar ch)</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug qSetPadChar(char ch){
        setPadChar(nativeId(this), ch);
        return this;
    }
    
    @io.qt.QtUninvokable
    private native void setPadChar(long __this__nativeId, char ch);
    
    /**
     * <p>See <a href="@docRoot/qdebug.html#QDebug-1">QDebug::QDebug(QString*)</a></p>
     */
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> QDebug(String string){
        this(new QTextStream.StringDevice<String>(java.util.Objects.requireNonNull(string, "Argument 'string': null not expected."), io.qt.core.QIODevice.OpenModeFlag.WriteOnly.asFlags()));
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QDebug append(java.lang.Object obj){
        QMetaType metaType = QList.getMetaType(obj);
        debugStream(nativeId(this), nativeId(metaType), obj);
        return this;
    }
    
    @io.qt.QtUninvokable
    private native static void debugStream(long debug, long metaType, Object value);
}// class

class QTextStream___ extends QTextStream {
    public CharSequence string() {
        QIODevice device = __qt_QTextStream_device_constfct(nativeId(this));
        if(device instanceof StringDevice) {
            flush();
            return ((StringDevice<?>) device).string;
        }
        return null;
    }
    
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> void setString(String string, io.qt.core.QIODevice.OpenModeFlag ... openMode) {
        setString(string, new io.qt.core.QIODevice.OpenMode(openMode));
    }
    
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> void setString(String string) {
        setString(string, new io.qt.core.QIODevice.OpenMode(4));
    }
    
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> void setString(String string, io.qt.core.QIODevice.OpenMode openMode) {
        flush();
        setDevice(new StringDevice<String>(java.util.Objects.requireNonNull(string, "Argument 'string': null not expected."), openMode));
    }
    
    public void setString(String string, io.qt.core.QIODevice.OpenModeFlag ... openMode) {
        setString(string, new io.qt.core.QIODevice.OpenMode(openMode));
    }
    
    public void setString(String string) {
        setString(string, new io.qt.core.QIODevice.OpenMode(4));
    }
    
    public void setString(String string, io.qt.core.QIODevice.OpenMode openMode) {
        setString(string==null ? new StringBuilder() : new StringBuilder(string), openMode);
    }
    
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> QTextStream(String string, io.qt.core.QIODevice.OpenModeFlag ... openMode) {
        this(string, new io.qt.core.QIODevice.OpenMode(openMode));
    }
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> QTextStream(String string) {
        this(string, new io.qt.core.QIODevice.OpenMode(3));
    }
    @SuppressWarnings("hiding")
    public <String extends Object & Appendable & CharSequence> QTextStream(String string, io.qt.core.QIODevice.OpenMode openMode){
        this(new StringDevice<String>(java.util.Objects.requireNonNull(string, "Argument 'string': null not expected."), openMode));
    }
    
    public QTextStream(String string, io.qt.core.QIODevice.OpenModeFlag ... openMode) {
        this(string, new io.qt.core.QIODevice.OpenMode(openMode));
    }
    public QTextStream(String string) {
        this(string, new io.qt.core.QIODevice.OpenMode(3));
    }
    public QTextStream(String string, io.qt.core.QIODevice.OpenMode openMode){
        this(string==null ? new StringBuilder() : new StringBuilder(string), openMode);
    }
    
    @SuppressWarnings("hiding")
    static final class StringDevice<String extends Object & Appendable & CharSequence> extends QIODevice{
        StringDevice(String string, io.qt.core.QIODevice.OpenModeFlag... openMode) {
            this(string, io.qt.core.QIODevice.OpenModeFlag.flags(openMode));
        }
        
        StringDevice(String string, io.qt.core.QIODevice.OpenMode openMode) {
            super();
            this.string = string;
            this.open(openMode);
        }

        private final String string;
        private int cursor;

        @Override
        protected int readData(byte[] data) {
            int i = -1;
            if(data!=null && openMode().isSet(QIODevice.OpenModeFlag.ReadOnly) && data.length>0){
                i = 0;
                while(i < data.length && cursor<string.length()) {
                    char c = string.charAt(cursor++);
                    byte[] _data = java.lang.String.valueOf(c).getBytes();
                    if(_data.length < i+data.length) {
                        System.arraycopy(_data, 0, data, i, _data.length);
                        ++i;
                    }else {
                        --cursor;
                        break;
                    }
                }
            }
            return i;
        }

        @Override
        protected int writeData(byte[] data) {
            if(data!=null && openMode().isSet(QIODevice.OpenModeFlag.WriteOnly) && data.length>0){
                java.lang.String strg = new java.lang.String(data);
                try {
                    string.append(strg);
                    cursor = string.length();
                    return data.length;
                } catch (java.io.IOException e) {
                    throw new RuntimeException(e);
                }
            }else return -1;
        }

        @Override
        public boolean isSequential() {
            return true;
        }

        @Override
        public boolean seek(long pos) {
            if(pos==0) {
                cursor = 0;
            }
            return super.seek(pos);
        }
    }
}// class

class QBitArray___ extends QBitArray {
    
    @io.qt.QtUninvokable
    public final boolean[] toBitArray() {
        long size = size();
        if(size>Integer.MAX_VALUE)
            throw new RuntimeException("Size of QBitArray exceeds max array size.");
        boolean[] res = new boolean[(int)size];

        for (int i = 0; i < size; i++) {
            res[i] = testBit(i);
        }
        return res;
    }
    
    public static io.qt.core.QBitArray fromBits(boolean[] data){
        io.qt.core.QBitArray array = new io.qt.core.QBitArray(data.length);
        for (int i = 0; i < data.length; i++) {
            if(data[i])
                array.setBit(i);
        }
        return array;
    }
    
    public static io.qt.core.QBitArray fromBits(byte[] data){
        return fromBits(java.nio.ByteBuffer.wrap(data), data.length*8);
    }
}// class

// hfr

class QDate___ extends QDate {
    
    /**
     * Result class for {@link QDate#weekNumber()}
     */
    public final static class Week{
        private Week(int weekNumber, int yearNumber){
            this.weekNumber = weekNumber;
            this.yearNumber = yearNumber;
        }
        public final int weekNumber;
        public final int yearNumber;
    }
}// class

class QByteArray_5_12__ extends QByteArray {

    @io.qt.QtUninvokable
    public final int compare(byte[] b) {
        return compare(new QByteArray(b));
    }

    @io.qt.QtUninvokable
    public final int compare(byte[] b, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(new QByteArray(b), cs);
    }
    
    @io.qt.QtUninvokable
    public final int compare(String str) {
        return compare(new QByteArray(str));
    }

    @io.qt.QtUninvokable
    public final int compare(String str, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(new QByteArray(str), cs);
    }
}// class

class QByteArray_5_15__ extends QByteArray {
    /**
     * Result class for {@link #fromBase64Encoding(io.qt.core.QByteArray, io.qt.core.QByteArray.Base64Options)}
     */
    public static class FromBase64Result{
        public final io.qt.core.QByteArray decoded;
        public final io.qt.core.QByteArray.Base64DecodingStatus decodingStatus;
        private FromBase64Result(QByteArray decoded, int decodingStatus) {
            super();
            this.decoded = decoded;
            this.decodingStatus = Base64DecodingStatus.resolve(decodingStatus);
        }
    }
}// class

class QByteArray___ extends QByteArray {
    
    public QByteArray(byte data[]) {
        this(java.nio.ByteBuffer.wrap(data), data.length);
    }

    @io.qt.QtUninvokable
    public final QByteArray prepend(byte[] data) {
        this.prepend(java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }
    
    @io.qt.QtUninvokable
    public native static QByteArray join(java.util.Collection<QByteArray> stringList);
    
    @io.qt.QtUninvokable
    public native static QByteArray join(java.util.Collection<QByteArray> stringList, byte sep);
    
    @io.qt.QtUninvokable
    public static QByteArray join(java.util.Collection<QByteArray> stringList, QByteArray sep) {
        return join(stringList, checkedNativeId(sep));
    }
    
    @io.qt.QtUninvokable
    private native static QByteArray join(java.util.Collection<QByteArray> stringList, long sep);
    
}// class

class QByteArray_5__ extends QByteArray {
    
    public QByteArray(String s) {
        this();
        append(s);
    }

    public QByteArray(byte data[], int size) {
        this(java.nio.ByteBuffer.wrap(data), size==-1 ? data.length : Math.min(size, data.length));
    }
    
    @io.qt.QtUninvokable
    public final int count(byte data[]) {
        return count(java.nio.ByteBuffer.wrap(data));
    }

    @io.qt.QtUninvokable
    public final boolean contains(byte data[]) {
        return contains(java.nio.ByteBuffer.wrap(data));
    }

    @io.qt.QtUninvokable
    public final boolean startsWith(String str) {
        return startsWith(new QByteArray(str));
    }

    @io.qt.QtUninvokable
    public final boolean contains(String str) {
        return contains(new QByteArray(str));
    }

    @io.qt.QtUninvokable
    public final int count(String str) {
        return count(new QByteArray(str));
    }

    @io.qt.QtUninvokable
    public final boolean endsWith(String str) {
        return endsWith(new QByteArray(str));
    }

    @io.qt.QtUninvokable
    public final QByteArray prepend(String str) {
        return prepend(new QByteArray(str));
    }

    @io.qt.QtUninvokable
    public final QByteArray replace(int index, int len, byte[] data) {
        this.replace(index, len, java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }

    @io.qt.QtUninvokable
    public final QByteArray insert(int i, byte[] data) {
        this.insert(i, java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }
    
    @io.qt.QtUninvokable
    public final QByteArray replace(QByteArray before, String after) {
        return replace(before, new QByteArray(after));
    }

    @io.qt.QtUninvokable
    public final QByteArray replace(String before, String after) {
        return replace(new QByteArray(before), new QByteArray(after));
    }

}// class

class QByteArray_6__ extends QByteArray {

    private static byte[] getBytes(String s) {
        try {
            return s.getBytes("UTF-8");
        } catch (java.io.UnsupportedEncodingException e) {
            return s.getBytes();
        }
    }

    public QByteArray(String s) {
        this(getBytes(s));
    }

    public QByteArray(byte data[], long size) {
        this(java.nio.ByteBuffer.wrap(data), size==-1 ? data.length : Math.min(size, data.length));
    }

    @io.qt.QtUninvokable
    public final boolean contains(String str) {
        return contains(new QByteArrayView(str));
    }

    @io.qt.QtUninvokable
    public final long count(String str) {
        return count(new QByteArrayView(str));
    }

    @io.qt.QtUninvokable
    public final boolean endsWith(String str) {
        return endsWith(new QByteArrayView(str));
    }
    
    @io.qt.QtUninvokable
    public final boolean endsWith(byte[] str) {
        return endsWith(new QByteArrayView(str));
    }
    
    @io.qt.QtUninvokable
    public final boolean endsWith(java.nio.ByteBuffer str) {
        return endsWith(new QByteArrayView(str));
    }

    @io.qt.QtUninvokable
    public final QByteArray prepend(String str) {
        return prepend(getBytes(str));
    }

    @io.qt.QtUninvokable
    public final boolean startsWith(String str) {
        return startsWith(new QByteArrayView(str));
    }
    
    @io.qt.QtUninvokable
    public final boolean startsWith(byte[] str) {
        return startsWith(new QByteArrayView(str));
    }
    
    @io.qt.QtUninvokable
    public final boolean startsWith(java.nio.ByteBuffer str) {
        return startsWith(new QByteArrayView(str));
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray append(byte[] a){
        return append(java.nio.ByteBuffer.wrap(a), a.length);
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray append(String a){
        return append(getBytes(a));
    }
    
    @io.qt.QtUninvokable
    public final int compare(String str) {
        return compare(str, io.qt.core.Qt.CaseSensitivity.CaseSensitive);
    }

    @io.qt.QtUninvokable
    public final int compare(String str, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(new QByteArrayView(str), cs);
    }
    
    @io.qt.QtUninvokable
    public final long count(byte data[]) {
        return count(new QByteArrayView(data));
    }

    @io.qt.QtUninvokable
    public final boolean contains(byte data[]) {
        return contains(new QByteArrayView(data));
    }
    
    @io.qt.QtUninvokable
    public final int compare(byte[] b) {
        return compare(b, io.qt.core.Qt.CaseSensitivity.CaseSensitive);
    }

    @io.qt.QtUninvokable
    public final int compare(byte[] b, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(new QByteArrayView(b), cs);
    }
    
    @io.qt.QtUninvokable
    public final long count(java.nio.ByteBuffer data) {
        return count(new QByteArrayView(data));
    }

    @io.qt.QtUninvokable
    public final boolean contains(java.nio.ByteBuffer data) {
        return contains(new QByteArrayView(data));
    }
    
    @io.qt.QtUninvokable
    public final int compare(java.nio.ByteBuffer b) {
        return compare(b, io.qt.core.Qt.CaseSensitivity.CaseSensitive);
    }

    @io.qt.QtUninvokable
    public final int compare(java.nio.ByteBuffer b, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(new QByteArrayView(b), cs);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(String strg) {
        return indexOf(strg, (int)0);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(String strg, long from){
        return indexOf(new io.qt.core.QByteArrayView(strg), from);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(byte[] other) {
        return indexOf(other, (int)0);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(byte[] other, long from){
        return indexOf(new io.qt.core.QByteArrayView(other), from);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(java.nio.ByteBuffer other) {
        return indexOf(other, (int)0);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(java.nio.ByteBuffer other, long from){
        return indexOf(new io.qt.core.QByteArrayView(other), from);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(QByteArray other) {
        return indexOf(other, (long)0);
    }
    
    @io.qt.QtUninvokable
    public final long indexOf(QByteArray other, long from){
        return indexOf(new io.qt.core.QByteArrayView(other), from);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(String strg) {
        return lastIndexOf(strg, (long)0);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(String strg, long from){
        return lastIndexOf(new io.qt.core.QByteArrayView(strg), from);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(byte[] other) {
        return lastIndexOf(other, (long)0);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(byte[] other, long from){
        return lastIndexOf(new io.qt.core.QByteArrayView(other), from);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(java.nio.ByteBuffer other) {
        return lastIndexOf(other, (long)0);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(java.nio.ByteBuffer other, long from){
        return lastIndexOf(new io.qt.core.QByteArrayView(other), from);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(QByteArray other) {
        return lastIndexOf(other, (long)0);
    }
    
    @io.qt.QtUninvokable
    public final long lastIndexOf(QByteArray other, long from){
        return lastIndexOf(new io.qt.core.QByteArrayView(other), from);
    }

    @io.qt.QtUninvokable
    public final QByteArray replace(long index, long len, byte[] data) {
        this.replace(index, len, java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }

    @io.qt.QtUninvokable
    public final QByteArray insert(long i, byte[] data) {
        this.insert(i, java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }
    
    @io.qt.QtUninvokable
    public final QByteArray replace(QByteArray before, String after) {
        return replace(new QByteArrayView(before), new QByteArrayView(after));
    }

    @io.qt.QtUninvokable
    public final QByteArray replace(String before, String after) {
        return replace(new QByteArrayView(before), new QByteArrayView(after));
    }

}// class

class QByteArrayView___ extends QByteArray {
    
    public QByteArrayView(byte[] data){
        super((QPrivateConstructor)null);
        Long[] pointerOut = {null};
        __qt_QByteArrayView_newBytes(this, data, pointerOut);
        if(pointerOut[0]!=null) {
            long pointer = pointerOut[0];
            purgeTask = ()->__qt_QByteArrayView_purgeBytes(pointer);
            getSignalOnDispose(this, true).connect(purgeTask::run);        		
        }
    }
    private native static void __qt_QByteArrayView_newBytes(Object instance, byte[] data, Long[] pointerOut);
    private native static void __qt_QByteArrayView_purgeBytes(long pointer);
    
    public QByteArrayView(String data){
        super((QPrivateConstructor)null);
        Long[] pointerOut = {null};
        __qt_QByteArrayView_newString(this, data, pointerOut);
        if(pointerOut[0]!=null) {
            long pointer = pointerOut[0];
            purgeTask = ()->__qt_QByteArrayView_purgeString(pointer);
            getSignalOnDispose(this, true).connect(purgeTask::run);        		
        }
    }
    private native static void __qt_QByteArrayView_newString(Object instance, String data, Long[] pointerOut);
    private native static void __qt_QByteArrayView_purgeString(long pointer);
    
    public QByteArrayView(java.nio.ByteBuffer data){
        super((QPrivateConstructor)null);
        if(data.isDirect()) {
            __qt_QByteArrayView_newDirectBuffer(this, data);
            purgeTask = data::hashCode;
            getSignalOnDispose(this, true).connect(purgeTask::run);
        }else {
            Long[] pointerOut = {null};
            __qt_QByteArrayView_newBuffer(this, data, pointerOut);
            if(pointerOut[0]!=null) {
                long pointer = pointerOut[0];
                purgeTask = ()->__qt_QByteArrayView_purgeBuffer(pointer);
                getSignalOnDispose(this, true).connect(purgeTask::run);
            }
        }
    }
    private native static void __qt_QByteArrayView_newDirectBuffer(Object instance, java.nio.ByteBuffer data);
    private native static void __qt_QByteArrayView_newBuffer(Object instance, java.nio.Buffer data, Long[] pointerOut);
    private native static void __qt_QByteArrayView_purgeBuffer(long pointer);
    
    public QByteArrayView(QByteArray data){
        super((QPrivateConstructor)null);
        if(data!=null) {
            __qt_QByteArrayView_newByteArray(this, data);
            purgeTask = data::isDisposed;
            getSignalOnDispose(this, true).connect(purgeTask::run);
        }else {
            __qt_QByteArrayView_new(this);
        }
    }
    private native static void __qt_QByteArrayView_newByteArray(Object instance, QByteArray data);
    
    private Runnable purgeTask;
    
}// class

class QIODevice_prefix__ extends QIODevice {
        public native static QIODevice fromDirectBuffer(java.nio.Buffer buffer);
        
        public static QIODevice fromInputStream(java.io.InputStream stream) {
            return new QIODevice() {
                @Override
                public boolean atEnd() {
                    return super.atEnd();
                }

                @Override
                public long bytesAvailable() {
                    try {
                        return stream.available();
                    } catch (java.io.IOException e) {
                        return 0;
                    }
                }

                @Override
                public long bytesToWrite() {
                    return 0;
                }

                @Override
                public boolean canReadLine() {
                    return false;
                }

                @Override
                public void close() {
                    try {
                        stream.close();
                    } catch (java.io.IOException e) {
                    }
                    super.close();
                }

                @Override
                public boolean isSequential() {
                    return true;
                }

                @Override
                public boolean open(OpenMode mode) {
                    if(mode.value()==OpenModeFlag.ReadOnly.value())
                        return super.open(mode);
                    else return false;
                }

                @Override
                public long pos() {
                    return super.pos();
                }

                @Override
                protected int readLineData(byte[] data) {
                    return 0;
                }

                @Override
                public boolean reset() {
                    try {
                        stream.reset();
                    } catch (java.io.IOException e) {
                        return false;
                    }
                    return super.reset();
                }

                @Override
                public boolean seek(long pos) {
                    return false;
                }

                @Override
                public long size() {
                    return super.size();
                }
}// QIODevice_prefix__

class QIODevice_6_1__ {

                @Override
                protected long skipData(long maxSize) {
                    try {
                        stream.skip(maxSize);
                    } catch (java.io.IOException e) {
                        return 0;
                    }
                    return super.skipData(maxSize);
                }
}// QIODevice_6_1__

class QIODevice_infix__ {

                @Override
                protected int writeData(byte[] data) {
                    return 0;
                }
                
                @Override
                protected int readData(byte[] data) {
                    try {
                        return stream.read(data);
                    } catch (java.io.IOException e) {
                        return 0;
                    }
                }
            };
        }
        
        public static QIODevice fromOutputStream(java.io.OutputStream stream) {
            return new QIODevice() {
                @Override
                public boolean atEnd() {
                    return super.atEnd();
                }

                @Override
                public long bytesAvailable() {
                    return 0;
                }

                @Override
                public long bytesToWrite() {
                    return 0;
                }

                @Override
                public boolean canReadLine() {
                    return false;
                }

                @Override
                public void close() {
                    try {
                        stream.close();
                    } catch (java.io.IOException e) {
                    }
                    super.close();
                }

                @Override
                public boolean isSequential() {
                    return true;
                }

                @Override
                public boolean open(OpenMode mode) {
                    if(mode.value()==OpenModeFlag.WriteOnly.value())
                        return super.open(mode);
                    else return false;
                }

                @Override
                public long pos() {
                    return super.pos();
                }

                @Override
                protected int readLineData(byte[] data) {
                    return 0;
                }

                @Override
                public boolean reset() {
                    return false;
                }

                @Override
                public boolean seek(long pos) {
                    return false;
                }

                @Override
                public long size() {
                    return super.size();
                }
}// QIODevice_infix__

class QIODevice_6_2__ {

                @Override
                protected long skipData(long maxSize) {
                    return 0;
                }
}// QIODevice_6_2__

class QIODevice_suffix__ {

                @Override
                protected int writeData(byte[] data) {
                    try {
                        stream.write(data);
                        return data.length;
                    } catch (java.io.IOException e) {
                        return 0;
                    }
                }
                
                @Override
                protected int readData(byte[] data) {
                    return 0;
                }
            };
        }
}// class

class QIODevice_cpp__{
}// class

class QTextCodec___ extends QTextCodec {
}// class

class QBuffer___ extends QBuffer {
}// class

class QCalendar___ extends QCalendar {
    public static final int Unspecified = unspecified();
    private static native int unspecified();
}// class

class QSignalMapper___ extends QSignalMapper {
        private java.util.HashMap<Object, Object> __rcObjectForObject;
}// class

class QAbstractFileEngine_MapExtensionReturn___ extends QAbstractFileEngine_MapExtensionReturn {
    
    private io.qt.QNativePointer currentAddressNativePointer; // don't garbage collect while in use
    public final void setAddress(String address) {
        currentAddressNativePointer = address != null ? io.qt.QNativePointer.createCharPointer(address) : null;
        address_private(currentAddressNativePointer);
    }

    public final String address() {
        io.qt.QNativePointer np = address_private();
        return np != null ? charPointerToString(np) : null;
    }
}// class

class QAbstractFileEngine_UnMapExtensionOption___ extends QAbstractFileEngine_UnMapExtensionOption {
    
    private io.qt.QNativePointer currentAddressNativePointer; // don't garbage collect while in use
    public final void setAddress(String address) {
        currentAddressNativePointer = address != null ? io.qt.QNativePointer.createCharPointer(address) : null;
        address_private(currentAddressNativePointer);
    }

    public final String address() {
        io.qt.QNativePointer np = address_private();
        return np != null ? charPointerToString(np) : null;
    }
}// class

class QFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QFutureInterface<T> futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QFutureInterface<T> futureInterface(long __this__nativeId);
}// class

class QVoidFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QFutureInterfaceBase futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QFutureInterfaceBase futureInterface(long __this__nativeId);
}// class

class QBooleanFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QBooleanFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QBooleanFutureInterface futureInterface(long __this__nativeId);
}// class

class QByteFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QByteFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QByteFutureInterface futureInterface(long __this__nativeId);
}// class

class QShortFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QShortFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QShortFutureInterface futureInterface(long __this__nativeId);
}// class

class QIntFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QIntFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QIntFutureInterface futureInterface(long __this__nativeId);
}// class

class QLongFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QLongFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QLongFutureInterface futureInterface(long __this__nativeId);
}// class

class QFloatFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QFloatFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QFloatFutureInterface futureInterface(long __this__nativeId);
}// class

class QDoubleFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QDoubleFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QDoubleFutureInterface futureInterface(long __this__nativeId);
}// class

class QCharFuture___ extends QFuture {
    @io.qt.QtUninvokable
    final io.qt.core.QCharFutureInterface futureInterface() {
    	return futureInterface(nativeId(this));
	}
    
    @io.qt.QtUninvokable
    private native io.qt.core.QCharFutureInterface futureInterface(long __this__nativeId);
}// class

class QFuture_6__ extends QFuture {

    @io.qt.QtUninvokable
    public <R> QFuture<R> then(java.util.function.Function<QFuture<T>, R> function){
        return then(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> then(long __this_nativeId, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public <R> QFuture<R> then(QtFuture.Launch policy, java.util.function.Function<QFuture<T>, R> function){
        return thenLaunch(nativeId(this), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenLaunch(long __this_nativeId, int policy, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public <R> QFuture<R> then(QThreadPool pool, java.util.function.Function<QFuture<T>, R> function){
        return thenPool(nativeId(this), nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenPool(long __this_nativeId, long pool, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(java.util.function.Consumer<QFuture<T>> function){
        return thenVoid(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenVoid(long __this_nativeId, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(QtFuture.Launch policy, java.util.function.Consumer<QFuture<T>> function){
        return thenLaunchVoid(nativeId(this), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenLaunchVoid(long __this_nativeId, int policy, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(QThreadPool pool, java.util.function.Consumer<QFuture<T>> function){
        return thenPoolVoid(nativeId(this), nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenPoolVoid(long __this_nativeId, long pool, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public QFuture<T> onFailed(java.util.function.Function<Throwable, T> function){
        return onFailed(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailed(long __this_nativeId, java.util.function.Function<Throwable, T> function);
    
    @io.qt.QtUninvokable
    public QFuture<T> onCanceled(java.util.function.Supplier<T> function){
        return onCanceled(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceled(long __this_nativeId, java.util.function.Supplier<T> function);
}// class

class QFuture_6_1__ extends QFuture {
    @io.qt.QtUninvokable
    public <R> QFuture<R> then(QObject context, java.util.function.Function<QFuture<T>, R> function){
        return thenPool(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenContext(long __this_nativeId, long contextId, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(QObject context, java.util.function.Consumer<QFuture<T>> function){
        return thenPoolVoid(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenContextVoid(long __this_nativeId, long contextId, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public QFuture<T> onFailed(QObject context, java.util.function.Function<Throwable, T> function){
        return onFailedContext(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailedContext(long __this_nativeId, long contextId, java.util.function.Function<Throwable, T> function);
    
    @io.qt.QtUninvokable
    public QFuture<T> onCanceled(QObject context, java.util.function.Supplier<T> function){
        return onCanceledContext(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceledContext(long __this_nativeId, long contextId, java.util.function.Supplier<T> function);
}// class

class QVoidFuture_6_1__ extends QFuture {
    @io.qt.QtUninvokable
    public <R> QFuture<R> then(QObject context, java.util.function.Function<QVoidFuture,R> function){
        return thenContext(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<R> thenContext(long __this_nativeId, long contextId, java.util.function.Function<QVoidFuture,R> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(QObject context, java.util.function.Consumer<QVoidFuture> function){
        return thenContextVoid(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenContextVoid(long __this_nativeId, long contextId, java.util.function.Consumer<QVoidFuture> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture onFailed(QObject context, java.util.function.Consumer<Throwable> function){
        return onFailedContext(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static QVoidFuture onFailedContext(long __this_nativeId, long contextId, java.util.function.Consumer<Throwable> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture onCanceled(QObject context, java.lang.Runnable function){
        return onCanceledContext(nativeId(this), checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static QVoidFuture onCanceledContext(long __this_nativeId, long contextId, java.lang.Runnable function);
}// class

class QVoidFuture_6__ extends QFuture {

    @io.qt.QtUninvokable
    public <R> QFuture<R> then(java.util.function.Function<QVoidFuture,R> function){
        return then(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<R> then(long __this_nativeId, java.util.function.Function<QVoidFuture,R> function);
    
    @io.qt.QtUninvokable
    public <R> QFuture<R> then(QtFuture.Launch policy, java.util.function.Function<QVoidFuture,R> function){
        return thenLaunch(nativeId(this), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<R> thenLaunch(long __this_nativeId, int policy, java.util.function.Function<QVoidFuture,R> function);
    
    @io.qt.QtUninvokable
    public <R> QFuture<R> then(QThreadPool pool, java.util.function.Function<QVoidFuture,R> function){
        return thenPool(nativeId(this), nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<R> thenPool(long __this_nativeId, long pool, java.util.function.Function<QVoidFuture,R> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(java.util.function.Consumer<QVoidFuture> function){
        return thenVoid(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenVoid(long __this_nativeId, java.util.function.Consumer<QVoidFuture> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(QtFuture.Launch policy, java.util.function.Consumer<QVoidFuture> function){
        return thenLaunchVoid(nativeId(this), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenLaunchVoid(long __this_nativeId, int policy, java.util.function.Consumer<QVoidFuture> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture then(QThreadPool pool, java.util.function.Consumer<QVoidFuture> function){
        return thenPoolVoid(nativeId(this), nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QVoidFuture thenPoolVoid(long __this_nativeId, long pool, java.util.function.Consumer<QVoidFuture> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture onFailed(java.util.function.Consumer<Throwable> function){
        return onFailed(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static QVoidFuture onFailed(long __this_nativeId, java.util.function.Consumer<Throwable> function);
    
    @io.qt.QtUninvokable
    public QVoidFuture onCanceled(java.lang.Runnable function){
        return onCanceled(nativeId(this), function);
    }
    @io.qt.QtUninvokable
    private native static QVoidFuture onCanceled(long __this_nativeId, java.lang.Runnable function);
}// class

class QFutureInterfaceBase___ {
    
    @io.qt.QtUninvokable
    public final void reportException(Throwable e) {
        reportException(nativeId(this), java.util.Objects.requireNonNull(e));
    }
    
    @io.qt.QtUninvokable
    private native void reportException(long nativeId, Throwable e);
    
    @io.qt.QtUninvokable
    protected final void setContinuation(java.util.function.Consumer<QFutureInterfaceBase> continuation) {
        setContinuation(nativeId(this), continuation);
    }
    @io.qt.QtUninvokable
    private native void setContinuation(long nativeId, java.util.function.Consumer<QFutureInterfaceBase> continuation);
}// class

class QtJambiFutureWatcher__{
    
    /**
     * <p>See <a href="@docRoot/qfuturewatcher.html#future">QFutureWatcher::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QFuture<T> future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QFuture<T> __qt_future(long nativeId);
    
    /**
     * <p>See <a href="@docRoot/qfuturewatcher.html#result">QFutureWatcher::result() const</a></p>
     */
    @io.qt.QtUninvokable
    public final T result(){
        return future().result();
    }
    
    /**
     * <p>See <a href="@docRoot/qfuturewatcher.html#resultAt">QFutureWatcher::resultAt(int index) const</a></p>
     */
    @io.qt.QtUninvokable
    public final T resultAt(int index){
        return future().resultAt(index);
    }
    
}// class

class QtJambiFutureInterface__{
    @io.qt.QtUninvokable
    public final boolean reportFinished(T result){
        boolean resultReported = reportResult(result);
        reportFinished();
        return resultReported;
    }
}// class

function future__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QFuture<T> future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QFuture<T> __qt_future(long nativeId);

}// class

function futurevoid__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QVoidFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QVoidFuture __qt_future(long nativeId);

}// class

function futureboolean__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QBooleanFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QBooleanFuture __qt_future(long nativeId);

}// class

function futurebyte__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QByteFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QByteFuture __qt_future(long nativeId);

}// class

function futureshort__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QShortFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QShortFuture __qt_future(long nativeId);

}// class

function futureint__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QIntFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QIntFuture __qt_future(long nativeId);

}// class

function futurelong__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QLongFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QLongFuture __qt_future(long nativeId);

}// class

function futurefloat__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QFloatFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QFloatFuture __qt_future(long nativeId);

}// class

function futurechar__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QCharFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QCharFuture __qt_future(long nativeId);

}// class

function futuredouble__ {
    
    /**
     * <p>See <a href="@docRoot/qfuture.html#future">QFuture::future() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QDoubleFuture future() {
        return __qt_future(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QDoubleFuture __qt_future(long nativeId);

}// class

class QtFuture___ {

    @io.qt.QtUninvokable
    public static QVoidFuture connect(QMetaObject.AbstractPrivateSignal0 signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QVoidFutureInterface promise = new QVoidFutureInterface();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect(()->{
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A> QFuture<A> connect(QMetaObject.AbstractPrivateSignal1<A> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<A> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect(value->{
                promise.reportResult(value);
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B> QFuture<QPair<A,B>> connect(QMetaObject.AbstractPrivateSignal2<A,B> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<QPair<A,B>> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b)->{
                promise.reportResult(new QPair<>(a,b));
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal3<A,B,C> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c)->{
                promise.reportResult(new Object[] {a,b,c});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C,D> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c,d)->{
                promise.reportResult(new Object[] {a,b,c,d});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C,D,E> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c,d,e)->{
                promise.reportResult(new Object[] {a,b,c,d,e});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C,D,E,F> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c,d,e,f)->{
                promise.reportResult(new Object[] {a,b,c,d,e,f});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C,D,E,F,G> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c,d,e,f,g)->{
                promise.reportResult(new Object[] {a,b,c,d,e,f,g});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C,D,E,F,G,H> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c,d,e,f,g,h)->{
                promise.reportResult(new Object[] {a,b,c,d,e,f,g,h});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> QFuture<Object[]> connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            QFutureInterface<Object[]> promise = new QFutureInterface<>();
            promise.reportStarted();
            QMetaObject.Connection[] connections = {null, null};
            connections[0] = signal.connect((a,b,c,d,e,f,g,h,i)->{
                promise.reportResult(new Object[] {a,b,c,d,e,f,g,h,i});
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            connections[1] = sender.destroyed.connect(()->{
                promise.reportCanceled();
                promise.reportFinished();
                QObject.disconnect(connections[0]);
                QObject.disconnect(connections[1]);
            });
            return promise.future();
        }
        return null;
    }
    
}// class

class QtFuture_6_1__ {
    @io.qt.QtUninvokable
    public static <T> QFuture<T> makeReadyFuture(java.util.Collection<T> values)
    {
        QFutureInterface<T> promise = new QFutureInterface<>();
        promise.reportStarted();
        promise.reportResults(values);
        promise.reportFinished();
        return promise.future();
    }
    
    @io.qt.QtUninvokable
    public static QVoidFuture makeReadyFuture()
    {
        QVoidFutureInterface promise = new QVoidFutureInterface();
        promise.reportStarted();
        promise.reportFinished();
        return promise.future();
    }
    
    @io.qt.QtUninvokable
    public static QVoidFuture makeExceptionalFuture(Throwable exception)
    {
        QVoidFutureInterface promise = new QVoidFutureInterface();
        promise.reportStarted();
        promise.reportException(exception);
        promise.reportFinished();
        return promise.future();
    }
}// class

class QFutureSynchronizer___ extends QFutureSynchronizer {
    
    /**
     * <p>See <a href="@docRoot/qfuturesynchronizer.html#futures">QFutureSynchronizer::futures() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QList<QFuture<T>> futures() {
        return __qt_futures(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QList<QFuture<T>> __qt_futures(long nativeId);

}// class

class QFutureSynchronizerVoid___ extends QFutureSynchronizerVoid {
    
    /**
     * <p>See <a href="@docRoot/qfuturesynchronizer.html#futures">QFutureSynchronizer::futures() const</a></p>
     */
    @io.qt.QtUninvokable
    public final QList<QVoidFuture> futures() {
        return __qt_futures(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QList<QVoidFuture> __qt_futures(long nativeId);

}// class

class QVoidPromise___ {
    
    @io.qt.QtUninvokable
    public final void setException(Throwable e) {
        setException(nativeId(this), java.util.Objects.requireNonNull(e));
    }
    
    @io.qt.QtUninvokable
    private native void setException(long nativeId, Throwable e);
    
}// class

class QPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(T result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(T result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, T result, int index);
    
}// class

class QBooleanPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(boolean result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(boolean result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, boolean result, int index);
    
}// class

class QBytePromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(byte result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(byte result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, byte result, int index);
    
}// class

class QShortPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(short result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(short result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, short result, int index);
    
}// class

class QIntPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(int result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(int result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, int result, int index);
    
}// class

class QLongPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(long result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(long result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, long result, int index);
    
}// class

class QCharPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(char result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(char result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, char result, int index);
    
}// class

class QFloatPromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(float result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(float result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, float result, int index);
    
}// class

class QDoublePromise___ {
    
    @io.qt.QtUninvokable
    public final boolean addResult(double result) {
        return addResult(nativeId(this), result, -1);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(double result, int index) {
        return addResult(nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    private native boolean addResult(long nativeId, double result, int index);
    
}// class

class QXmlStreamWriter___ extends QXmlStreamWriter {
}// class


class QJsonDocument___{
    
    /**
     * Result class for {@link #fromJson(io.qt.core.QByteArray)}
     */
    public static final class FromJsonResult{
        private FromJsonResult(QJsonDocument document, QJsonParseError error) {
            super();
            this.document = document;
            this.error = error;
        }
        public final io.qt.core.QJsonDocument document;
        public final io.qt.core.QJsonParseError error;
    }
}// class


class QJsonDocument_5__{
    
    /**
     * <p>Overloaded function for {@link #fromRawData(java.nio.ByteBuffer, io.qt.core.QJsonDocument.DataValidation)}.</p>
     */
    public static io.qt.core.QJsonDocument fromRawData(String data) {
        return fromRawData(data, io.qt.core.QJsonDocument.DataValidation.Validate);
    }
    
    /**
     * <p>Overloaded function for {@link #fromRawData(java.nio.ByteBuffer, io.qt.core.QJsonDocument.DataValidation)}.</p>
     */
    public static io.qt.core.QJsonDocument fromRawData(String data, io.qt.core.QJsonDocument.DataValidation validation) {
        return fromRawData(data.getBytes(), validation);
    }
    
    /**
     * <p>Overloaded function for {@link #fromRawData(java.nio.ByteBuffer, io.qt.core.QJsonDocument.DataValidation)}.</p>
     */
    public static io.qt.core.QJsonDocument fromRawData(byte[] data) {
        return fromRawData(data, io.qt.core.QJsonDocument.DataValidation.Validate);
    }
    
    /**
     * <p>Overloaded function for {@link #fromRawData(java.nio.ByteBuffer, io.qt.core.QJsonDocument.DataValidation)}.</p>
     */
    public static io.qt.core.QJsonDocument fromRawData(byte[] data, io.qt.core.QJsonDocument.DataValidation validation) {
        java.nio.ByteBuffer buffer = java.nio.ByteBuffer.wrap(data);
        return fromRawData(buffer, buffer.capacity(), validation);
    }
    
}// class

class QLockFile__{

    /**
     * Result class for {@link #getLockInfo()}
     */
    public static class LockInfo {
            private LockInfo(long pid, String hostname, String appname) {
                this.pid = pid;
                this.hostname = hostname;
                this.appname = appname;
            }
            
            public final long pid;
            public final String hostname;
            public final String appname;
    }
}// class


class QPropertyAnimation___ extends QPropertyAnimation {
    
    /**
     * <p>Overloaded constructor for {@link #QPropertyAnimation(io.qt.core.QObject,io.qt.core.QByteArray,io.qt.core.QObject)}
     * with <code>parent = null</code>.</p>
     */
    public QPropertyAnimation(io.qt.core.QObject target, String propertyName){
        this(target, new io.qt.core.QByteArray(propertyName), null);
    }
    
    /**
     * <p>Overloaded constructor for {@link #QPropertyAnimation(io.qt.core.QObject,io.qt.core.QByteArray,io.qt.core.QObject)}.</p>
     */
    public QPropertyAnimation(io.qt.core.QObject target, String propertyName, io.qt.core.QObject parent){
        this(target, new io.qt.core.QByteArray(propertyName), parent);
    }
    
    /**
     * <p>Overloaded function for {@link #setPropertyName(io.qt.core.QByteArray)}.</p>
     */
    public final void setPropertyName(String propertyName)    {
        setPropertyName(new io.qt.core.QByteArray(propertyName));
    }
    
}// class

class QMetaType_6__ extends QMetaType {
    
    public static io.qt.core.QMetaType fromName(String name){
        return fromName(new io.qt.core.QByteArrayView(name));
    }
    
    public static io.qt.core.QMetaType fromName(QByteArray name){
        return fromName(new io.qt.core.QByteArrayView(name));
    }
    
    public static io.qt.core.QMetaType fromName(byte[] name){
        return fromName(new io.qt.core.QByteArrayView(name));
    }
    
}// class

class QMetaType___ extends QMetaType {
    
    /**
     * Overloaded constructor for {@link #QMetaType(int)}.
     * @param type
     */
    public QMetaType(QMetaType.Type type){
        this(type.value());
    }
    
    /**
     * <p>Overloaded function for {@link #create(Object)}
     * with <code>copy = null</code>.</p>
     * @return new instance
     */
    public final Object create() {
        return io.qt.internal.QtJambiInternal.createMetaType(id(), javaType(), null);
    }
    
    /**
     * <p>Returns a copy of <i>copy</i>, assuming it is of the type that this <code>QMetaType</code> instance was created for.</p>
     * <p>If copy is <code>null</code>, creates a default constructed instance.</p>
     * <p>See <a href="@docRoot/qmetatype.html#create">QMetaType::create(int, const void *)</a></p>
     * @param copy
     * @return new instance
     */
    public final Object create(Object copy) {
        return io.qt.internal.QtJambiInternal.createMetaType(id(), javaType(), copy);
    }
    
    /**
     * Returns the type name associated with this <code>QMetaType</code> as {@link String}.
     * @return type name
     */
    @Override
    @io.qt.QtUninvokable
    public final String toString() {
        return ""+name();
    }
    
    /**
     * <p>Returns the <code>QMetaType</code> corresponding to the given class.</p>
     * <p>If the class has never been registered as meta type <code>UnknownType</code> is returned.</p>
     * <p>If given class is generic (e.g. {@link io.qt.core.QList}, {@link io.qt.core.QPair}, {@link io.qt.core.QMap}) 
     * specify the template instantiations to be used in the registered template type.</p>
     * <p>See <a href="@docRoot/qmetatype.html#fromType">QMetaMethod::fromType&lt;T&gt;()</a></p>
     * @param clazz the class to be registered
     * @param instantiations optional instantiations for generics (templates)
     * @return meta type ID
     */
    public static QMetaType fromType(Class<?> clazz, QMetaType... instantiations){
        return new QMetaType(registerMetaType(clazz, instantiations));
    }
    
    /**
     * <p>Registers the given class as meta type. Returns the internal ID used by <code>QMetaType</code>.</p>
     * <p>After a type has been registered, you can create and destroy objects of that type dynamically at run-time.</p>
     * <p>If given class is generic (e.g. {@link io.qt.core.QList}, {@link io.qt.core.QPair}, {@link io.qt.core.QMap}) 
     * specify the template instantiations to be used in the registered template type.</p>
     * <p>See <a href="@docRoot/qmetatype.html#qRegisterMetaType">qRegisterMetaType&lt;T&gt;()</a></p>
     * @param clazz the class to be registered
     * @param instantiations optional instantiations for generics (templates)
     * @return meta type ID
     */
    public static int registerMetaType(Class<?> clazz, QMetaType... instantiations){
        int registeredId = metaTypeId(clazz, instantiations);
        if(registeredId!=0)
            return registeredId;
        Object[] typeParameters = clazz.getTypeParameters();
        if(typeParameters.length>0) {
            if(typeParameters.length!=instantiations.length) {
                throw new IllegalArgumentException("Number of instantiations does not correspond to number of type parameters.");
            }
            if(QList.class.isAssignableFrom(clazz)) {
                if(instantiations[0].id()==Type.QString.value()) {
                    return Type.QStringList.value();
                }else if(instantiations[0].id()==Type.QByteArray.value()) {
                    return Type.QByteArrayList.value();
                }else{
                    if(instantiations[0].id()==Type.QVariant.value()) {
                        return Type.QVariantList.value();
                    }
                }
            }else if(QMap.class.isAssignableFrom(clazz)) {
                if(instantiations[0].id()==Type.QString.value()) {
                    if(instantiations[1].id()==Type.QVariant.value()) {
                        return Type.QVariantMap.value();
                    }
                }
            }
            else if(QHash.class.isAssignableFrom(clazz)) {
                if(instantiations[0].id()==Type.QString.value()) {
                    if(instantiations[1].id()==Type.QVariant.value()) {
                        return Type.QVariantHash.value();
                    }
                }
            }
            int[] _instantiations = new int[instantiations.length];
            for (int i = 0; i < _instantiations.length; i++) {
                _instantiations[i] = instantiations[i].id();
                if(_instantiations[i]==0)
                    throw new IllegalArgumentException("Invalid instantiation.");
            }
            return io.qt.internal.QtJambiInternal.registerMetaType(clazz, _instantiations);
        }else {
            if(instantiations.length>0) {
                throw new IllegalArgumentException("Type "+clazz.getName()+" does not accept instantiations.");
            }
            return io.qt.internal.QtJambiInternal.registerMetaType(clazz);
        }
    }
    
    /**
     * <p>Returns the meta type id of goven class at compile time.</p>
     * <p>If the class has never been registered as meta type <code>UnknownType</code> is returned.</p>
     * <p>If given class is generic (e.g. {@link io.qt.core.QList}, {@link io.qt.core.QPair}, {@link io.qt.core.QMap}) 
     * specify the template instantiations to be used in the registered template type.</p>
     * <p>See <a href="@docRoot/qmetatype.html#qMetaTypeId">qMetaTypeId&lt;T&gt;()</a></p>
     * @param clazz the class to be registered
     * @param instantiations optional instantiations for generics (templates)
     * @return meta type ID
     */
    public static int metaTypeId(Class<?> clazz, QMetaType... instantiations){
        if(clazz==null)
            return QMetaType.Type.Nullptr.value();
        initializePackage(clazz);
        Object[] typeParameters = clazz.getTypeParameters();
        if(typeParameters.length>0) {
            if(typeParameters.length!=instantiations.length) {
                if(instantiations.length==0) {
                    int result = io.qt.internal.QtJambiInternal.metaTypeId(clazz);
                    if(result!=0)
                        return result;
                }
                throw new IllegalArgumentException("Number of instantiations does not correspond to number of type parameters.");
            }
            for (QMetaType instantiation : instantiations) {
                if(instantiation.id()==0)
                    throw new IllegalArgumentException("Invalid instantiation.");
            }
            switch(typeParameters.length) {
            case 1:
                if(instantiations[0].id()!=Type.Void.value()) {
                    if(clazz==java.util.Set.class) {
                        return type(String.format("QSet<%1$s>", instantiations[0].name()));
                    }else if(clazz==java.util.Queue.class) {
                        return type(String.format("QQueue<%1$s>", instantiations[0].name()));
                    }else if(clazz==java.util.Deque.class) {
                        return type(String.format("QStack<%1$s>", instantiations[0].name()));
                    }else if(clazz.isInterface() && java.util.List.class.isAssignableFrom(clazz)) {
                        return type(String.format("QList<%1$s>", instantiations[0].name()));
                    }
                }
                break;
            case 2:
                if(instantiations[0].id()!=QMetaType.Type.Void.value() && instantiations[1].id()!=QMetaType.Type.Void.value()) {
                    if(clazz==java.util.Map.class) {
                        return type(String.format("QHash<%1$s,%2$s>", instantiations[0].name(), instantiations[1].name()));
                    }else if(clazz==java.util.TreeMap.class) {
                        return type(String.format("QMap<%1$s,%2$s>", instantiations[0].name(), instantiations[1].name()));
                    }else if(clazz==java.util.HashMap.class) {
                        return type(String.format("QHash<%1$s>", instantiations[0].name(), instantiations[1].name()));
                    }else if(clazz.isInterface() && java.util.NavigableMap.class.isAssignableFrom(clazz)) {
                        return type(String.format("QMap<%1$s,%2$s>", instantiations[0].name(), instantiations[1].name()));
                    }
                }
                break;
            }
            if(io.qt.QtObjectInterface.class.isAssignableFrom(clazz)) {
                String name = clazz.getSimpleName();
                QStringList _instantiations = new QStringList();
                for (QMetaType instantiation : instantiations) {
                    _instantiations.add(instantiation.name().toString());
                }
                return type(String.format("%1$s<%2$s>", name, _instantiations.join(',')));
            }
        }else {
            if(instantiations.length>0) {
                throw new IllegalArgumentException("Type "+clazz.getName()+" does not accept instantiations.");
            }
        }
        return io.qt.internal.QtJambiInternal.metaTypeId(clazz);
    }
    
    /**
     * Returns the Java class for this meta type.
     * @return java class
     */
    public final Class<?> javaType(){
        return io.qt.internal.QtJambiInternal.javaTypeForMetaTypeId(id());
    }
    
    /**
     * Returns the Java class for the given meta type ID.
     * @return java class
     */
    public static Class<?> javaType(int metaTypeId){
        return io.qt.internal.QtJambiInternal.javaTypeForMetaTypeId(metaTypeId);
    }

    /**
     * Interface super type for all generic classes representing a QMetaType-registered but Java-unknown value type.
     */
    public interface GenericTypeInterface{
        @io.qt.QtUninvokable
        public QMetaType metaType();
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown enumerator type which can be used in QFlags.
     */
    public static final class GenericFlag implements io.qt.QtFlagEnumerator{
        private GenericFlag(int value) {
            super();
            this.value = value;
        }
    
        private final int value;
        
        @Override 
        public int value() { 
            return value; 
        }
        
        @Override 
        public int ordinal() { return -1; }
        
        @Override 
        public String name() { return "unknown"; }
        
        @Override 
        public Class<? extends io.qt.QtFlagEnumerator> getDeclaringClass() { return GenericFlag.class; }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown QFlags type.
     */
    public static final class GenericFlags extends io.qt.QFlags<GenericFlag> implements GenericTypeInterface{
        private static final long serialVersionUID = -7659504264600507749L;
    
        @io.qt.internal.NativeAccess
        private GenericFlags(int type, int value) {
            super(value);
            this.type = type;
        }
        
        private final int type;
        
        @Override
        public final QMetaType metaType() {
            return new QMetaType(type);
        }
        
        public GenericFlag[] flags(){
            return new GenericFlag[]{new GenericFlag(value())};
        }
    
        @Override
        public GenericFlags clone() {
            return new GenericFlags(type, value());
        }
    
        @Override
        public GenericFlags combined(GenericFlag flag) {
            return new GenericFlags(type, value() | flag.value());
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown enumerator.
     */
    private abstract static class AbstractGenericEnumerator implements io.qt.QtAbstractEnumerator, GenericTypeInterface{
        @io.qt.internal.NativeAccess
        private AbstractGenericEnumerator(int type, int index, String name) {
            super();
            this.type = type;
            this.index = index;
            this.name = name;
        }
    
        private final int type;
        private final int index;
        private final String name;
    
        
        @Override
        public final QMetaType metaType() {
            return new QMetaType(type);
        }
    
        @Override
        public final int ordinal() {
            return index;
        }
    
        @Override
        public final String name() {
            return name==null ? "unknown" : name;
        }
        
        abstract String valueString();
    
        @Override
        public String toString() {
            if(name==null) {
                return String.format("%1$s(%2$s)", typeName(type), valueString());
            }else {
                return String.format("%1$s(%2$s:%3$s)", typeName(type), name, valueString());
            }
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + ((name == null) ? 0 : name.hashCode());
            result = prime * result + type;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if (obj == null)
                return false;
            if (getClass() != obj.getClass())
                return false;
            AbstractGenericEnumerator other = (AbstractGenericEnumerator) obj;
            if (name == null) {
                if (other.name != null)
                    return false;
            } else if (!name.equals(other.name))
                return false;
            if (type != other.type)
                return false;
            return true;
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown 32 Bit enumerator.
     */
    public final static class GenericEnumerator extends AbstractGenericEnumerator implements io.qt.QtEnumerator{
        @io.qt.internal.NativeAccess
        private GenericEnumerator(int type, int index, int value, String name) {
            super(type, index, name);
            this.value = value;
        }
        
        private final int value;
        
        @Override
        public int value() {
            return value;
        }
    
        @Override
        public Class<? extends io.qt.QtEnumerator> getDeclaringClass() {
            return GenericEnumerator.class;
        }
    
        String valueString() {
            return Integer.toString(value);
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = super.hashCode();
            result = prime * result + value;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if (!super.equals(obj))
                return false;
            if (getClass() != obj.getClass())
                return false;
            GenericEnumerator other = (GenericEnumerator) obj;
            if (value != other.value)
                return false;
            return true;
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown 8 Bit enumerator.
     */
    public final static class GenericByteEnumerator extends AbstractGenericEnumerator implements io.qt.QtByteEnumerator{
        @io.qt.internal.NativeAccess
        private GenericByteEnumerator(int type, int index, byte value, String name) {
            super(type, index, name);
            this.value = value;
        }
        
        private final byte value;
        
        @Override
        public byte value() {
            return value;
        }
    
        @Override
        public Class<? extends io.qt.QtByteEnumerator> getDeclaringClass() {
            return GenericByteEnumerator.class;
        }
    
        String valueString() {
            return Byte.toString(value);
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = super.hashCode();
            result = prime * result + value;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if (!super.equals(obj))
                return false;
            if (getClass() != obj.getClass())
                return false;
            GenericByteEnumerator other = (GenericByteEnumerator) obj;
            if (value != other.value)
                return false;
            return true;
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown 16 Bit enumerator.
     */
    public final static class GenericShortEnumerator extends AbstractGenericEnumerator implements io.qt.QtShortEnumerator{
        @io.qt.internal.NativeAccess
        private GenericShortEnumerator(int type, int index, short value, String name) {
            super(type, index, name);
            this.value = value;
        }
        
        private final short value;
        
        @Override
        public short value() {
            return value;
        }
    
        @Override
        public Class<? extends io.qt.QtShortEnumerator> getDeclaringClass() {
            return GenericShortEnumerator.class;
        }
    
        String valueString() {
            return Short.toString(value);
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = super.hashCode();
            result = prime * result + value;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if (!super.equals(obj))
                return false;
            if (getClass() != obj.getClass())
                return false;
            GenericShortEnumerator other = (GenericShortEnumerator) obj;
            if (value != other.value)
                return false;
            return true;
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown 64 Bit enumerator.
     */
    public final static class GenericLongEnumerator extends AbstractGenericEnumerator implements io.qt.QtLongEnumerator{
        @io.qt.internal.NativeAccess
        private GenericLongEnumerator(int type, int index, long value, String name) {
            super(type, index, name);
            this.value = value;
        }
        
        private final long value;
        
        @Override
        public long value() {
            return value;
        }
    
        @Override
        public Class<? extends io.qt.QtLongEnumerator> getDeclaringClass() {
            return GenericLongEnumerator.class;
        }
    
        String valueString() {
            return Long.toString(value);
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = super.hashCode();
            result = prime * result + (int) (value ^ (value >>> 32));
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if (!super.equals(obj))
                return false;
            if (getClass() != obj.getClass())
                return false;
            GenericLongEnumerator other = (GenericLongEnumerator) obj;
            if (value != other.value)
                return false;
            return true;
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown value type.
     */
    public final static class GenericValue extends io.qt.QtObject implements Cloneable, GenericTypeInterface{
        @io.qt.internal.NativeAccess
        private GenericValue(QPrivateConstructor p) { super(p); }
        
        private static native GenericValue clone(long _this, int type);
        
        @Override
        public GenericValue clone() {
            return clone(nativeId(this), type);
        }

        @io.qt.internal.NativeAccess
        private final int type = QMetaType.Type.UnknownType.value();
        
        @io.qt.QtUninvokable
        public QMetaType metaType() {
            return new QMetaType(type);
        }
        
        public java.nio.ByteBuffer buffer(){
            return QMetaType.buffer(nativeId(this), type);
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown object type.
     */
    public final static class GenericObject extends io.qt.QtObject implements GenericTypeInterface{
        private GenericObject(QPrivateConstructor p) { super(p); }
        
        @io.qt.internal.NativeAccess
        private final int type = QMetaType.Type.UnknownType.value();
        
        @io.qt.QtUninvokable
        public QMetaType metaType() {
            return new QMetaType(type);
        }
    }
    
    /**
     * This class represents a QMetaType-registered but Java-unknown gadget type.
     */
    public final static class GenericGadget extends io.qt.QtGadget implements GenericTypeInterface{
        private GenericGadget(QPrivateConstructor p) {super(p);}
        
        @io.qt.internal.NativeAccess
        private final int type = QMetaType.Type.UnknownType.value();
        
        @io.qt.QtUninvokable
        public QMetaType metaType() {
            return new QMetaType(type);
        }
    }

    private static native java.nio.ByteBuffer buffer(long __this__nativeId, int type);
}// class

class QMetaMethod___ {
    private Class<?> returnClassType;
    private java.util.List<Class<?>> parameterClassTypes;
    
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#parameterTypes">QMetaMethod::parameterTypes() const</a></p>
     */
    @io.qt.QtUninvokable
    public final java.util.List<Class<?>> parameterClassTypes() {
        if(parameterClassTypes==null) {
            parameterClassTypes = java.util.Collections.unmodifiableList(parameterClassTypes(nativeId(this)));
        }
        return parameterClassTypes;
    }
    
    @io.qt.QtUninvokable
    private static native final java.util.List<Class<?>> parameterClassTypes(long __this_nativeId);
    
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#parameterType">QMetaMethod::parameterType(int index) const</a></p>
     */
    @io.qt.QtUninvokable
    public final Class<?> parameterClassType(int index) {
        return parameterClassTypes().get(index);
    }
    
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#returnType">QMetaMethod::returnType() const</a></p>
     */
    @io.qt.QtUninvokable
    public final Class<?> returnClassType(){
        if(returnClassType==null)
            returnClassType = returnClassType(nativeId(this));
        return returnClassType;
    }
    
    @io.qt.QtUninvokable
    private static native final Class<?> returnClassType(long __this_nativeId);
        
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#methodSignature">QMetaMethod::methodSignature() const</a></p>
     */
    @io.qt.QtUninvokable
    public final String methodSignature() {
        StringBuilder args = new StringBuilder();
        java.util.List<Class<?>> parameterClassTypes = parameterClassTypes();
        for(int i=0; i<parameterClassTypes.size(); ++i) {
            if(i!=0)
                args.append(',');
            Class<?> parameterType = parameterClassTypes.get(i);
            if(parameterType!=null){
                if(parameterType.isArray()) {
                    int arrayDepth = 1;
                    Class<?> componentType = parameterType.getComponentType();
                    while(true) {
                        if(componentType.isArray()) {
                            ++arrayDepth;
                            componentType = componentType.getComponentType();
                        }else {
                            args.append(componentType.getName());
                            for (int j = 0; j < arrayDepth; j++) {
                                args.append("[]");
                            }
                            break;
                        }
                    }
                }else {
                    args.append(parameterType.getName());
                }
            }else {
                args.append("<unknown type>");
            }
        }
        return String.format("%1$s(%2$s)", name(), args);
    }
    
    @Override
    @io.qt.QtUninvokable
    public final String toString() {
        return methodSignature();
    }
    
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#invoke-1">QMetaMethod::invoke(QObject *object, QGenericReturnArgument returnValue, QGenericArgument val0 = QGenericArgument(nullptr), QGenericArgument val1 = QGenericArgument(), QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(), QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(), QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(), QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument()) const</a></p>
     */
    @io.qt.QtUninvokable
    public final Object invoke(QObject object, Object... args) throws IllegalArgumentException, io.qt.QUnsuccessfulInvocationException {
        return invoke(object, Qt.ConnectionType.AutoConnection, args);
    }
    
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#invoke">QMetaMethod::invoke(QObject *object, Qt::ConnectionType connectionType, QGenericReturnArgument returnValue, QGenericArgument val0 = QGenericArgument(nullptr), QGenericArgument val1 = QGenericArgument(), QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(), QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(), QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(), QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument()) const</a></p>
     */
    @io.qt.QtUninvokable
    public final Object invoke(QObject object, Qt.ConnectionType connection, Object... args) throws IllegalArgumentException, io.qt.QUnsuccessfulInvocationException {
        if(!isValid())
            throw new io.qt.QUnsuccessfulInvocationException("Cannot invoke an invalid QMetaMethod.");
        long object_id = checkedNativeId(java.util.Objects.requireNonNull(object));
        if(parameterCount() != args.length) {
            throw new IllegalArgumentException(String.format("Wrong number of arguments. expected: %2$s, given: %1$s", args.length, parameterCount()));
        }
        {
            QMetaObject enclosingMetaObject = enclosingMetaObject();
            QMetaObject objectMO = object.metaObject();
            if(objectMO!=enclosingMetaObject && !objectMO.inherits(enclosingMetaObject)) {
                throw new IllegalArgumentException(String.format("Given object is not an instance of %1$s", enclosingMetaObject.className()));
            }
        }
        if(connection==Qt.ConnectionType.BlockingQueuedConnection && object.thread()==QThread.currentThread()) {
            throw new io.qt.QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
        }
        Class<?>[] argClassTypes = new Class<?>[1+parameterCount()];
        Class<?> returnType = returnClassType();
        argClassTypes[0] = returnType==null ? void.class : returnType;
        if(argClassTypes[0]!=void.class && argClassTypes[0]!=Void.class) {
            if(connection==Qt.ConnectionType.QueuedConnection) {
                throw new io.qt.QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
            }else if(connection==Qt.ConnectionType.AutoConnection) {
                if(QThread.currentThread() != object.thread()) {
                    throw new io.qt.QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
                }
            }
        }
        java.util.List<Class<?>> parameterClassTypes = parameterClassTypes();
        for(int i=0; i<parameterClassTypes.size(); ++i) {
            argClassTypes[i+1] = parameterClassTypes.get(i);
        }
        return _invoke(object_id, nativeId(this), argClassTypes, connection.ordinal(), args);
    }
    
    @io.qt.QtUninvokable
    private static native Object _invoke(long object__id, long metaMethodId, Class<?>[] argClassTypes, int connection, Object[] args);
    
    /**
     * <p>See <a href="@docRoot/qmetamethod.html#invokeOnGadget">QMetaMethod::invokeOnGadget(void *gadget, QGenericReturnArgument returnValue, QGenericArgument val0 = QGenericArgument(nullptr), QGenericArgument val1 = QGenericArgument(), QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(), QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(), QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(), QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument()) const</a></p>
     */
    @io.qt.QtUninvokable
    public final Object invokeOnGadget(Object object, Object... args) throws IllegalArgumentException, io.qt.QUnsuccessfulInvocationException {
        if(!isValid())
            throw new io.qt.QUnsuccessfulInvocationException("Cannot invoke an invalid QMetaMethod.");
        if(parameterCount() != args.length) {
            throw new IllegalArgumentException(String.format("Wrong number of arguments. expected: %2$s, given: %1$s", args.length, parameterCount()));
        }
        long object_id = 0;
        if(object instanceof io.qt.QtObjectInterface){
            object_id = checkedNativeId((io.qt.QtObjectInterface)object);
        }
        {
            QMetaObject enclosingMetaObject = enclosingMetaObject();
            QMetaObject objectMO = QMetaObject.forGadget(object);
            if(objectMO!=enclosingMetaObject && !objectMO.inherits(enclosingMetaObject)) {
                throw new IllegalArgumentException(String.format("Given gadget is not an instance of %1$s", enclosingMetaObject.className()));
            }
        }
        Class<?>[] argClassTypes = new Class<?>[1+parameterCount()];
        Class<?> returnType = returnClassType();
        argClassTypes[0] = returnType==null ? void.class : returnType;
        java.util.List<Class<?>> parameterClassTypes = parameterClassTypes();
        for(int i=0; i<parameterClassTypes.size(); ++i) {
            argClassTypes[i+1] = parameterClassTypes.get(i);
        }
        return _invokeOnGadget(object_id, object, nativeId(this), argClassTypes, args);
    }
    
    @io.qt.QtUninvokable
    private static native Object _invokeOnGadget(long object_id, Object object, long metaMethodId, Class<?>[] argClassTypes, Object[] args);
    
    @io.qt.QtUninvokable
    public final java.lang.reflect.Method toReflectedMethod() {
        if(isValid()) {
            switch(methodType()) {
            case Method:
            case Slot:
                java.lang.reflect.AccessibleObject reflected = toReflected(nativeId(this));
                if(reflected instanceof java.lang.reflect.Method){
                    return (java.lang.reflect.Method)reflected;
                }
                try {
                    java.util.List<Class<?>> parameterTypes = parameterClassTypes();
                    return this.enclosingMetaObject().type().getDeclaredMethod(name().toString(), parameterTypes.toArray(new Class<?>[parameterTypes.size()]));
                } catch (NoSuchMethodException | SecurityException e) {
                }
                break;
            default:
                throw new IllegalArgumentException("Method " + this + " is a " + methodType().name().toLowerCase() + ".");
            }
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    private static native java.lang.reflect.AccessibleObject toReflected(long object_id);
    
    @io.qt.QtUninvokable
    public final java.lang.reflect.Constructor<?> toReflectedConstructor() {
        if(isValid()) {
            if(methodType()==MethodType.Constructor) {
                java.lang.reflect.AccessibleObject reflected = toReflected(nativeId(this));
                if(reflected instanceof java.lang.reflect.Constructor<?>){
                    return (java.lang.reflect.Constructor<?>)reflected;
                }
                try {
                    java.util.List<Class<?>> parameterTypes = parameterClassTypes();
                    return this.enclosingMetaObject().type().getDeclaredConstructor(parameterTypes.toArray(new Class<?>[parameterTypes.size()]));
                } catch (NoSuchMethodException | SecurityException e) {
                }
            }else {
                throw new IllegalArgumentException("Method " + this + " is not a constructor.");
            }
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public final QMetaObject.AbstractSignal toSignal(QObject sender) {
        if(isValid()) {
            if(methodType()==MethodType.Signal) {
                return findSignal(sender, name().toString(), parameterClassTypes().toArray(new Class[parameterCount()]));
            }else {
                throw new IllegalArgumentException("Method " + this + " is not a signal.");
            }
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public static QMetaMethod fromSignal(QMetaObject.AbstractSignal signal) {
        QMetaMethod method = null;
        io.qt.QtSignalEmitterInterface containingObject = signal.containingObject();
        if(containingObject instanceof QObject) {
            if(signal.methodIndex()>=0) {
                method = ((QObject)containingObject).metaObject().methodByIndex(signal.methodIndex());
            }else{
                java.util.List<Class<?>> signalTypeClasses = signal.argumentTypes();
                method = ((QObject)containingObject).metaObject().method(signal.name(), signalTypeClasses.toArray(new Class[signalTypeClasses.size()]));
            }
        }
        return method;
    }
    
    public static QMetaMethod fromReflectedConstructor(java.lang.reflect.Constructor<?> constructor) {
        QMetaObject mo = QMetaObject.forType(constructor.getDeclaringClass());
        if(mo!=null) {
            return mo.constructor(constructor.getParameterTypes());
        }
        return null;
    }
    
    public static QMetaMethod fromReflectedMethod(java.lang.reflect.Method method) {
        Class<?> declaringClass = method.getDeclaringClass();
        QMetaMethod qmethod = null;
        QMetaObject mo = QMetaObject.forType(declaringClass);
        if(mo!=null) {
            Object[] ok = {method};
            qmethod = methodFromMethod(mo.metaObjectPointer, ok);
            if(qmethod==null && ok[0]==null) {
                qmethod = mo.method(method.getName(), method.getParameterTypes());
            }
        }
        return qmethod;
    }
    
    private static native QMetaMethod methodFromMethod(long metaObjectPointer, Object[] method);
    
    private static QMetaMethod fromMethodImpl(QMetaObject.AbstractSlot method) {
        LambdaInfo info = lamdaInfo(method);
        if(info!=null && info.lambdaArgs.isEmpty()) {
            if(info.reflectiveMethod!=null)
                return fromReflectedMethod(info.reflectiveMethod);
            if(info.reflectiveConstructor!=null)
                return fromReflectedConstructor(info.reflectiveConstructor);
        }
        return null;
    }
    
    public static <R> QMetaMethod fromMethod(QMetaObject.Method0<R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,R> QMetaMethod fromMethod(QMetaObject.Method1<A,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,R> QMetaMethod fromMethod(QMetaObject.Method2<A,B,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,R> QMetaMethod fromMethod(QMetaObject.Method3<A,B,C,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,R> QMetaMethod fromMethod(QMetaObject.Method4<A,B,C,D,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,R> QMetaMethod fromMethod(QMetaObject.Method5<A,B,C,D,E,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,R> QMetaMethod fromMethod(QMetaObject.Method6<A,B,C,D,E,F,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,G,R> QMetaMethod fromMethod(QMetaObject.Method7<A,B,C,D,E,F,G,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,G,H,R> QMetaMethod fromMethod(QMetaObject.Method8<A,B,C,D,E,F,G,H,R> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,G,H,I,R> QMetaMethod fromMethod(QMetaObject.Method9<A,B,C,D,E,F,G,H,I,R> method) {
        return fromMethodImpl(method);
    }
    
    public static QMetaMethod fromMethod(QMetaObject.Slot0 method) {
        return fromMethodImpl(method);
    }
    
    public static <A> QMetaMethod fromMethod(QMetaObject.Slot1<A> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B> QMetaMethod fromMethod(QMetaObject.Slot2<A,B> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C> QMetaMethod fromMethod(QMetaObject.Slot3<A,B,C> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D> QMetaMethod fromMethod(QMetaObject.Slot4<A,B,C,D> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E> QMetaMethod fromMethod(QMetaObject.Slot5<A,B,C,D,E> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F> QMetaMethod fromMethod(QMetaObject.Slot6<A,B,C,D,E,F> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,G> QMetaMethod fromMethod(QMetaObject.Slot7<A,B,C,D,E,F,G> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,G,H> QMetaMethod fromMethod(QMetaObject.Slot8<A,B,C,D,E,F,G,H> method) {
        return fromMethodImpl(method);
    }
    
    public static <A,B,C,D,E,F,G,H,I> QMetaMethod fromMethod(QMetaObject.Slot9<A,B,C,D,E,F,G,H,I> method) {
        return fromMethodImpl(method);
    }
}// class

class QMetaProperty___{
    
    @io.qt.QtUninvokable
    public final QMetaObject.AbstractSignal notifySignal(QObject object) {
        QMetaMethod notifySignal = notifySignal();
        return notifySignal==null ? null : notifySignal.toSignal(object);
    }
    
    /**
     * <p>See <a href="@docRoot/qmetaproperty.html#resetOnGadget">QMetaProperty::resetOnGadget(const void *) const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean resetOnGadget(Object object){
        return resetOnGadget(nativeId(this), object);
    }
    
    /**
     * <p>See <a href="@docRoot/qmetaproperty.html#readOnGadget">QMetaProperty::readOnGadget(const void *) const</a></p>
     */
    @io.qt.QtUninvokable
    public final Object readOnGadget(Object object) {
        return readOnGadget(nativeId(this), object);
    }
    
    /**
     * <p>See <a href="@docRoot/qmetaproperty.html#writeOnGadget">QMetaProperty::writeOnGadget(void *, const QVariant &) const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean writeOnGadget(Object object, Object value) {
        return writeOnGadget(nativeId(this), object, value);
    }
    
    @io.qt.QtUninvokable
    private static native boolean resetOnGadget(long nativeId, Object object);
    
    @io.qt.QtUninvokable
    private static native boolean writeOnGadget(long nativeId, Object object, Object value);
    
    @io.qt.QtUninvokable
    private static native Object readOnGadget(long nativeId, Object object);
    
    @Override
    @io.qt.QtUninvokable
    public final String toString() {
        return name();
    }
    
    private Class<?> classType;
    
    /**
     * <p>See <a href="@docRoot/qmetaproperty.html#metaType">QMetaProperty::metaType() const</a></p>
     */
    @io.qt.QtUninvokable
    public final Class<?> classType(){
        if(classType==null)
            classType = classType(nativeId(this));
        if(classType==null)
            classType = QMetaType.javaType(this.userType());
        return classType;
    }
    
    @io.qt.QtUninvokable
    private static native final Class<?> classType(long __this_nativeId);
}// class

class QMetaEnum___{
    
    @Override
    @io.qt.QtUninvokable
    public final String toString() {
        return name();
    }

    @io.qt.QtUninvokable
    public Integer keysToValue(String... keys) {
        return keysToValue(new QStringList(keys).join('|'));
    }
    
    @io.qt.QtUninvokable
    public Integer keysToValue(java.util.Collection<String> keys) {
        return keysToValue((keys instanceof QStringList ? (QStringList)keys : new QStringList(keys)).join('|'));
    }

    @io.qt.QtUninvokable
    public io.qt.QtAbstractEnumerator[] entries() {
        return enclosingMetaObject().enumEntries(this);
    }
    
    @io.qt.QtUninvokable
    public io.qt.QtAbstractEnumerator entry(int index) {
        return enclosingMetaObject().enumEntry(this, index);
    }
    
    @io.qt.QtUninvokable
    public io.qt.QtAbstractEnumerator entry(String name) {
        return enclosingMetaObject().enumEntry(this, name);
    }
    
    @io.qt.QtUninvokable
    public Class<?> type() {
        return enclosingMetaObject().enumType(this);
    }
    
    @io.qt.QtUninvokable
    @io.qt.internal.NativeAccess
    private static Class<?> flagsType(Class<?> enumType){
        try {
            Class<?> result = enumType.getMethod("asFlags").getReturnType();
            if(result==io.qt.QFlags.class && enumType.getDeclaringClass()!=null) {
                for(Class<?> cls : enumType.getDeclaringClass().getDeclaredClasses()) {
                    if(io.qt.QFlags.class.isAssignableFrom(cls)) {
                        if(cls.getGenericSuperclass() instanceof java.lang.reflect.ParameterizedType) {
                            java.lang.reflect.ParameterizedType pt = (java.lang.reflect.ParameterizedType)cls.getGenericSuperclass();
                            if(pt.getRawType()==io.qt.QFlags.class) {
                                java.lang.reflect.Type[] types = pt.getActualTypeArguments();
                                if(types.length==1 && types[0]==enumType)
                                    return cls;
                            }
                        }
                    }
                }
            }
            return result;
        } catch (Exception e) {
            return null;
        }
    }
    
    @io.qt.QtUninvokable
    public io.qt.QFlags<?> flags(int value) {
        Class<?> enumType = type();
        if(io.qt.QtFlagEnumerator.class.isAssignableFrom(enumType)) {
            Class<?> flagsType = flagsType(enumType);
            if(flagsType!=null)
                return flags(flagsType, value);
        }else if(io.qt.QFlags.class.isAssignableFrom(enumType)) {
            return flags(enumType, value);
        }
        return null;
    }
    
    @io.qt.QtUninvokable
    public io.qt.QFlags<?> flags(String... names) {
        @SuppressWarnings("unchecked")
        io.qt.QFlags<io.qt.QtFlagEnumerator> flags = (io.qt.QFlags<io.qt.QtFlagEnumerator>)flags(0);
        for (String name : names) {
            io.qt.QtAbstractEnumerator entry = entry(name);
            if(entry instanceof io.qt.QtFlagEnumerator) {
                flags.set((io.qt.QtFlagEnumerator)entry);
            }else {
                throw new io.qt.QNoSuchEnumValueException(name);
            }
        }
        return flags;
    }
    
    @io.qt.QtUninvokable
    private static native io.qt.QFlags<? extends io.qt.QtFlagEnumerator> flags(Class<?> cls, int value);
    
    @io.qt.QtUninvokable
    public io.qt.QtAbstractEnumerator resolve(int value) {
        Class<?> type = type();
        if(type.isEnum()) {
            byte bitSize = 4;
            if(io.qt.QtByteEnumerator.class.isAssignableFrom(type)) {
                bitSize = 1;
            }else if(io.qt.QtShortEnumerator.class.isAssignableFrom(type)) {
                bitSize = 2;
            }else if(io.qt.QtLongEnumerator.class.isAssignableFrom(type)) {
                bitSize = 8;
            }
            return resolveEntry(type, value, bitSize);
        }else {
            return enclosingMetaObject().getEnumEntry(this, value);
        }
    }
    
    @io.qt.QtUninvokable
    private static native io.qt.QtAbstractEnumerator resolveEntry(Class<?> cls, int value, byte bitSize);

}// class

class Qt___ extends Qt {
    
    public final static QMetaObject staticMetaObject = QMetaObject.forType(Qt.class);
    
}// class

class QCollatorSortKey___{
    
    @io.qt.QtUninvokable
    @Override
    public final int compareTo(io.qt.core.QCollatorSortKey key){
        return compare(key);
    }
    
}// class

class QCborValue_java__{
    
    /**
     * Result class for {@link #fromCbor(io.qt.core.QByteArray)}
     */
    public static final class FromCborResult{
        private FromCborResult(io.qt.core.QCborValue value, io.qt.core.QCborParserError error) {
            super();
            this.value = value;
            this.error = error;
        }
        public final io.qt.core.QCborValue value;
        public final io.qt.core.QCborParserError error;
    }
    
    @io.qt.QtUninvokable
    public final void setValue(java.lang.String key, io.qt.core.QCborValue value){
        __qt_QCborValue_setValue_cref_QString(nativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborValue_setValue_cref_QString(long __this__nativeId, java.lang.String key, io.qt.core.QCborValue value);
    
    @io.qt.QtUninvokable
    public final void setValue(long key, io.qt.core.QCborValue value){
        __qt_QCborValue_setValue_long_long(nativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborValue_setValue_long_long(long __this__nativeId, long key, io.qt.core.QCborValue value);
    
}// class

class QCborArray_java__{

    @io.qt.QtUninvokable
    public final void setValue(long i, io.qt.core.QCborValue value){
        __qt_QCborArray_setValue_qsizetype(nativeId(this), i, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborArray_setValue_qsizetype(long __this__nativeId, long i, io.qt.core.QCborValue value);

}// class

class QCborMap_java__{
    
    @io.qt.QtUninvokable
    public final void setValue(io.qt.core.QCborValue key, io.qt.core.QCborValue value){
        __qt_QCborMap_setValue_cref_QCborValue(nativeId(this), nativeId(key), value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborMap_setValue_cref_QCborValue(long __this__nativeId, long key, io.qt.core.QCborValue value);

    @io.qt.QtUninvokable
    public final void setValue(java.lang.String key, io.qt.core.QCborValue value){
        __qt_QCborMap_setValue_cref_QString(nativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborMap_setValue_cref_QString(long __this__nativeId, java.lang.String key, io.qt.core.QCborValue value);

    @io.qt.QtUninvokable
    public final void setValue(long key, io.qt.core.QCborValue value){
        __qt_QCborMap_setValue_long_long(nativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborMap_setValue_long_long(long __this__nativeId, long key, io.qt.core.QCborValue value);
    
}// class

class QCborStreamReader_java__{
    
    public QCborStreamReader(byte[] data){
        this(new io.qt.core.QByteArray(data));
    }
    
    @io.qt.QtUninvokable
    public final void addData(byte[] data){
        addData(new io.qt.core.QByteArray(data));
    }
    
    /**
     * Result class for {@link #readByteArray()} and {@link #readString()}
     */
    public static final class StringResult<Container> {
        private StringResult(Container data, StringResultCode status) {
            super();
            this.data = data;
            this.status = status;
        }
        public final Container data;
        public final StringResultCode status;
    };
    
    @io.qt.QtUninvokable
    public final io.qt.core.QCborStreamReader.StringResult<io.qt.core.QByteArray> readByteArray(){
        return __qt_QCborStreamReader_readByteArray(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QCborStreamReader.StringResult<io.qt.core.QByteArray> __qt_QCborStreamReader_readByteArray(long __this__nativeId);

    @io.qt.QtUninvokable
    public final io.qt.core.QCborStreamReader.StringResult<String> readString(){
        return __qt_QCborStreamReader_readString(nativeId(this));
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QCborStreamReader.StringResult<String> __qt_QCborStreamReader_readString(long __this__nativeId);

    @io.qt.QtUninvokable
    public final io.qt.core.QCborStreamReader.StringResult<Long> readChunk(java.nio.ByteBuffer buffer){
        return __qt_QCborStreamReader_readChunk(nativeId(this), buffer);
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QCborStreamReader.StringResult<Long> __qt_QCborStreamReader_readChunk(long __this__nativeId, java.nio.ByteBuffer buffer);
    
}// class

class QFactoryLoader__{

    public static void registerFactory(Class<? extends QtObjectInterface> factoryClass){
        Method createMethod = null;
        for(Method method : factoryClass.getDeclaredMethods()) {
            if(!Modifier.isStatic(method.getModifiers())
                    && Modifier.isPublic(method.getModifiers())
                    && method.getReturnType()!=void.class) {
                createMethod = method;
                break;
            }
        }
        if(createMethod==null) {
            throw new IllegalArgumentException("Missing factory method in interface "+factoryClass.getName());
        }
        qRegisterPluginInterface(factoryClass);
    }
    
    public <P extends QtObjectInterface, R> R loadPlugin(QMetaObject.Method1<P, R> create, String key) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory);
                        } catch (Throwable e) {
                            Logger.getLogger(QFactoryLoader.class.getName()).throwing(QFactoryLoader.class.getName(), "loadPlugin", e);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, R> R loadPlugin(QMetaObject.Method2<P, A, R> create, String key, A a) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, R> R loadPlugin(QMetaObject.Method3<P, A, B, R> create, String key, A a, B b) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, C, R> R loadPlugin(QMetaObject.Method4<P, A, B, C, R> create, String key, A a, B b, C c) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b, c);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, C, D, R> R loadPlugin(QMetaObject.Method5<P, A, B, C, D, R> create, String key, A a, B b, C c, D d) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b, c, d);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, C, D, E, R> R loadPlugin(QMetaObject.Method6<P, A, B, C, D, E, R> create, String key, A a, B b, C c, D d, E e) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b, c, d, e);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, C, D, E, F, R> R loadPlugin(QMetaObject.Method7<P, A, B, C, D, E, F, R> create, String key, A a, B b, C c, D d, E e, F f) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b, c, d, e, f);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, C, D, E, F, G, R> R loadPlugin(QMetaObject.Method8<P, A, B, C, D, E, F, G, R> create, String key, A a, B b, C c, D d, E e, F f, G g) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b, c, d, e, f, g);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public <P extends QtObjectInterface, A, B, C, D, E, F, G, H, R> R loadPlugin(QMetaObject.Method9<P, A, B, C, D, E, F, G, H, R> create, String key, A a, B b, C c, D d, E e, F f, G g, H h) {
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<P> factoryClass = getFactoryClass(create);
                if(factoryClass!=null && factoryClass==qRegisteredPluginInterface(iid)) {
                    QObject factoryObject = instance(index);
                    P factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        try {
                            return create.invoke(factory, a, b, c, d, e, f, g, h);
                        } catch (RuntimeException | Error ex) {
                            throw ex;
                        } catch (Throwable ex) {
                            throw new RuntimeException(ex);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public QObject loadPlugin(String key, Object... args){
        return loadPlugin(QObject.class, key, args);
    }
    
    public <P extends QtObjectInterface> P loadPlugin(Class<P> pluginClass, String key, Object... args){
        int index = indexOf(key);
        if (index != -1) {
            QJsonValue iidValue = metaData().get(index).value("IID");
            if(iidValue.isString()) {
                QByteArray iid = new QByteArray(iidValue.toString());
                Class<? extends QtObjectInterface> factoryClass = qRegisteredPluginInterface(iid);
                if(factoryClass!=null) {
                    QObject factoryObject = instance(index);
                    QtObjectInterface factory = QMetaObject.cast(factoryClass, factoryObject);
                    if(factory!=null){
                        Method createMethod = null;
                        for(Method method : factoryClass.getDeclaredMethods()) {
                            if(method.getParameterCount()==args.length
                                    && !Modifier.isStatic(method.getModifiers())
                                    && Modifier.isPublic(method.getModifiers())
                                    && pluginClass==method.getReturnType()) {
                                Class<?>[] argClassTypes = method.getParameterTypes();
                                createMethod = method;
                                for (int i = 0; i < method.getParameterCount(); i++) {
                                    Class<?> argClassType = argClassTypes[i];
                                    if(argClassType.isPrimitive()) {
                                        if(args[i]==null
                                                || (argClassType==int.class && !(args[i] instanceof Integer))
                                                || (argClassType==short.class && !(args[i] instanceof Short))
                                                || (argClassType==byte.class && !(args[i] instanceof Byte))
                                                || (argClassType==long.class && !(args[i] instanceof Long))
                                                || (argClassType==boolean.class && !(args[i] instanceof Boolean))
                                                || (argClassType==float.class && !(args[i] instanceof Float))
                                                || (argClassType==char.class && !(args[i] instanceof Character))
                                                || (argClassType==double.class && !(args[i] instanceof Double)) 
                                            ) {
                                            createMethod = null;
                                            break;
                                        }
                                    }else if(args[i]!=null && !argClassType.isInstance(args[i])){
                                        createMethod = null;
                                        break;
                                    }
                                }
                                if(createMethod!=null)
                                    break;
                            }
                        }
                        if(createMethod!=null) {
                            try {
                                Object result = invokeMethod(createMethod, factory, args);
                                if(pluginClass.isInterface() && result instanceof QObject) {
                                    return QMetaObject.cast(pluginClass, (QObject)result);
                                }else if(result!=null){
                                    return pluginClass.cast(result);
                                }
                            } catch (RuntimeException | Error ex) {
                                throw ex;
                            } catch (Throwable ex) {
                                throw new RuntimeException(ex);
                            }
                        }
                    }
                }
            }
        }
        return null;
    }
    
}// class

class QPluginLoader_java__{
    
    public static void registerStaticPluginFunction(QObject instance){
        qRegisterStaticPluginFunction(instance, (QJsonObject)null);
    }
    
    public static void registerStaticPluginFunction(QObject instance, QJsonObject metaData){
        qRegisterStaticPluginFunction(instance, metaData);
    }
    
    public static void registerStaticPluginFunction(QObject instance, java.util.Map<String, Object> metaData){
        qRegisterStaticPluginFunction(instance, QJsonObject.fromVariantHash(metaData));
    }
    
    public static void registerStaticPluginFunction(Class<? extends QObject> pluginClass){
        qRegisterStaticPluginFunction(pluginClass, (QJsonObject)null);
    }
    
    public static void registerStaticPluginFunction(Class<? extends QObject> pluginClass, QJsonObject metaData){
        qRegisterStaticPluginFunction(pluginClass, metaData);
    }
    
    public static void registerStaticPluginFunction(Class<? extends QObject> pluginClass, java.util.Map<String, Object> metaData){
        qRegisterStaticPluginFunction(pluginClass, QJsonObject.fromVariantHash(metaData));
    }
    
    public static void registerPluginInterface(Class<? extends io.qt.QtObjectInterface> factoryClass){
        qRegisterPluginInterface(factoryClass);
    }
    
    @io.qt.QtUninvokable
    public final <T extends io.qt.core.QObject> T instance(Class<T> type){
        return QMetaObject.cast(type, instance());
    }
}// class

class QStaticPlugin_java__{
    @io.qt.QtUninvokable
    public final io.qt.core.QObject instance(){
        return instance(nativeId(this));
    }
    
    private static native final io.qt.core.QObject instance(long nativeId);
}// class

class QThread___{
    
    @io.qt.QtUninvokable
    static native void initialize();
    
    public QThread(ThreadGroup group, String name, long stackSize, io.qt.core.QObject parent) {
        super((QPrivateConstructor)null);        
        __qt_QThread_new_QObject_ptr(this, parent);
        initialize(group);
        if(name!=null)
            setName(name);
        if(stackSize>=0 && stackSize <= 0x0ffffffffL)
            setStackSize( (int)(stackSize & 0x0ffffffffL) );
    }
    
    public static QThread create(Runnable runnable) {
        return create(null, runnable, null, -1, null);
    }
    
    public static QThread create(Runnable runnable, io.qt.core.QObject parent) {
        return create(null, runnable, null, -1, parent);
    }
    
    public static QThread create(Runnable runnable, long stackSize) {
        return create(null, runnable, null, stackSize, null);
    }
    
    public static QThread create(Runnable runnable, long stackSize, io.qt.core.QObject parent) {
        return create(null, runnable, null, stackSize, parent);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable, long stackSize) {
        return create(group, runnable, null, stackSize, null);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable, long stackSize, io.qt.core.QObject parent) {
        return create(group, runnable, null, stackSize, parent);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable, String name) {
        return create(group, runnable, name, -1, null);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable, String name, io.qt.core.QObject parent) {
        return create(group, runnable, name, -1, parent);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable, String name, long stackSize) {
        return create(group, runnable, name, stackSize, null);
    }
    
    public static QThread create(Runnable runnable, String name, long stackSize, io.qt.core.QObject parent) {
        return create(null, runnable, name, stackSize, parent);
    }
    
    public static QThread create(Runnable runnable, String name, long stackSize) {
        return create(null, runnable, name, stackSize, null);
    }
    
    public static QThread create(Runnable runnable, String name, io.qt.core.QObject parent) {
        return create(null, runnable, name, -1, parent);
    }
    
    public static QThread create(Runnable runnable, String name) {
        return create(null, runnable, name, -1, null);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable) {
        return create(group, runnable, null, -1, null);
    }
    
    public static QThread create(ThreadGroup group, Runnable runnable, String name, long stackSize, io.qt.core.QObject parent) {
        return new Runner(group, runnable, name, stackSize, parent);
    }
    
    private final static class Runner extends QThread{
        Runner(ThreadGroup group, Runnable runnable, String name, long stackSize, QObject parent) {
            super(group, name, stackSize, parent);
            this.runnable = runnable;
        }
        private final Runnable runnable;
        
        @Override
        protected void run() {
            if(runnable!=null){
                Thread thread = javaThread();
                try{
                    runnable.run();
                }catch(Throwable exn){
                    if(thread!=null) {
                        Thread.UncaughtExceptionHandler handler = thread.getUncaughtExceptionHandler();
                        if(handler==null) {
                            handler = Thread.getDefaultUncaughtExceptionHandler();
                        }
                        if(handler==null) {
                            exn.printStackTrace();
                        }else {
                            handler.uncaughtException(thread, exn);
                        }
                    }else {
                        exn.printStackTrace();
                    }
                }
            }
        }
    }
    
    public QThread(String name) {
        this(null, name, -1, null);
    }
    
    public QThread(ThreadGroup group) {
        this(group, null, -1, null);
    }
    
    public QThread(String name, io.qt.core.QObject parent) {
        this(null, name, -1, parent);
    }
    
    public QThread(ThreadGroup group, io.qt.core.QObject parent) {
        this(group, null, -1, parent);
    }
    
    public QThread(String name, long stackSize) {
        this(null, name, stackSize, null);
    }
    
    public QThread(ThreadGroup group, long stackSize) {
        this(group, null, stackSize, null);
    }
    
    public QThread(String name, long stackSize, io.qt.core.QObject parent) {
        this(null, name, stackSize, parent);
    }
    
    public QThread(ThreadGroup group, long stackSize, io.qt.core.QObject parent) {
        this(group, null, stackSize, parent);
    }
    
    public QThread(ThreadGroup group, String name, long stackSize) {
        this(group, name, stackSize, null);
    }
    
    public QThread(ThreadGroup group, String name, io.qt.core.QObject parent) {
        this(group, name, -1, parent);
    }
    
    public QThread(ThreadGroup group, String name) {
        this(group, name, -1, null);
    }
    
    private void initialize(ThreadGroup group) {
        if(group==null) {
            Thread parent = Thread.currentThread();
            SecurityManager security = System.getSecurityManager();
            if (security != null) {
                group = security.getThreadGroup();
            }
            if (group == null) {
                group = parent.getThreadGroup();
            }
        }
        initialize(nativeId(this), group);
    }
    
    public final ThreadGroup getThreadGroup() {
        if(javaThread!=null) {
            return javaThread.getThreadGroup();
        }
        return getThreadGroup(nativeId(this));
    }
    
    private native ThreadGroup getThreadGroup(long __this__nativeId);
    
    public void setName(String name) {
        if(!isRunning() && javaThread==null)
            setName(nativeId(this), name);
    }
    
    private native void setName(long __this__nativeId, String name);
    
    public final String getName() {
        if(javaThread!=null) {
            return javaThread.getName();
        }
        return getName(nativeId(this));
    }
    
    private native String getName(long __this__nativeId);
    
    public void setDaemon(boolean daemon) {
        if(!isRunning() && javaThread==null)
            setDaemon(nativeId(this), daemon);
    }
    
    private native void setDaemon(long __this__nativeId, boolean daemon);
    
    public final boolean isDaemon() {
        if(javaThread!=null) {
            return javaThread.isDaemon();
        }
        return isDaemon(nativeId(this));
    }
    
    private native boolean isDaemon(long __this__nativeId);
    
    public void setUncaughtExceptionHandler(Thread.UncaughtExceptionHandler handler) {
        if(javaThread!=null) {
            javaThread.setUncaughtExceptionHandler(handler);
        }else {
            setUncaughtExceptionHandler(nativeId(this), handler);
        }
    }
    
    private native void setUncaughtExceptionHandler(long __this__nativeId, Thread.UncaughtExceptionHandler handler);
    
    public final Thread.UncaughtExceptionHandler getUncaughtExceptionHandler() {
        if(javaThread!=null) {
            return javaThread.getUncaughtExceptionHandler();
        }
        return getUncaughtExceptionHandler(nativeId(this));
    }
    
    private native Thread.UncaughtExceptionHandler getUncaughtExceptionHandler(long __this__nativeId);
    
    public void setContextClassLoader(ClassLoader cl) {
        if(javaThread!=null) {
            javaThread.setContextClassLoader(cl);
        }else {
            setContextClassLoader(nativeId(this), cl);
        }
    }
    
    private native void setContextClassLoader(long __this__nativeId, ClassLoader cl);
    
    public final ClassLoader getContextClassLoader() {
        if(javaThread!=null) {
            return javaThread.getContextClassLoader();
        }
        return getContextClassLoader(nativeId(this));
    }
    
    private native ClassLoader getContextClassLoader(long __this__nativeId);
    
    private static native void initialize(long nativeId, ThreadGroup group);
    private final Thread javaThread = null;
    
    public final Thread javaThread() { return javaThread==null ? javaThread(nativeId(this)) : javaThread; }
    
    private native Thread javaThread(long nativeId);
    public static native QThread thread(Thread thread);
    
    public final boolean isAlive() {
        return isRunning();
    }
    
    public final boolean isInterrupted() {
        if(javaThread!=null && javaThread.isInterrupted())
            return true;
        return isInterruptionRequested();
    }
    
    public final void interrupt() {
        requestInterruption();
    }
    
    private final static Object interruptible;
    
    static{
        Object _interruptible = null;
        try {
            _interruptible = java.lang.reflect.Proxy.newProxyInstance(
                    QThread.class.getClassLoader(), 
                    new Class[] { Class.forName("sun.nio.ch.Interruptible") }, 
                    (proxy, method, args)->{
                        if(args.length==1 && args[0] instanceof Thread) {
                            Thread thread = (Thread)args[0];
                            if(thread.isAlive()) {
                                QThread qthread = thread(thread);
                                try(Monitor monitor = synchronizedNativeId(qthread)){
                                    if(qthread!=null && !qthread.isDisposed() && !qthread.isInterruptionRequested()){
                                        qthread.__qt_QThread_requestInterruption(nativeId(qthread));
                                    }
                                }
                            }
                        }
                        return null;
                    });
        } catch (Throwable e) {
        }
        interruptible = _interruptible;
    }
}// class

class QSharedMemory_java__{
    
    private static class CleanTask implements Runnable{
        private CleanTask(QSharedMemory sharedMemory) {
            super();
            this.sharedMemory = sharedMemory;
        }
        
        @Override
        public void run() {
            if(!isClosed) {
                isClosed = true;
                try{
                    if(!sharedMemory.isDisposed())
                        sharedMemory.unlock();
                }finally {
                    sharedMemory.__qt_isInUse = false;
                }
            }
        }
        private final QSharedMemory sharedMemory;
        private boolean isClosed = false;
    }
    
    public static class DataAccess implements AutoCloseable{
        
        private final QSharedMemory sharedMemory;
        private final Cleanable cleanable;
        
        DataAccess(QSharedMemory sharedMemory){
            this.sharedMemory = sharedMemory;
            cleanable = registerCleaner(this, new CleanTask(sharedMemory));
        }
        
        @Override
        public void close(){
            cleanable.clean();
        }
        
        @io.qt.QtUninvokable
        public java.nio.ByteBuffer data() {
            if(!sharedMemory.__qt_isInUse) {
                throw new IllegalStateException();
            }
            java.nio.ByteBuffer result = sharedMemory.data();
            if(sharedMemory.__qt_accessMode==AccessMode.ReadOnly.value()){
                result = result.asReadOnlyBuffer();
            }
            return result;
        }
        
    }
    
    private int __qt_accessMode = 0;
    private boolean __qt_isInUse;
    
    public DataAccess access() throws IllegalStateException{
        if(this.isAttached() && !__qt_isInUse && lock()) {
            __qt_isInUse = true;
            return new DataAccess(this);
        }
        throw new IllegalStateException();
    }
    
}// class

class QSettings__{

}// class

class QModelRoleData___{
    
    @io.qt.QtUninvokable
    public final void setData(Object data){
        __qt_QModelRoleData_setData(nativeId(this), data);
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QModelRoleData_setData(long __this__nativeId, Object data);
    
}// class

class QDir__{
    @io.qt.QtUninvokable
    public final io.qt.core.QList<io.qt.core.QFileInfo> entryInfoList(io.qt.core.QDir.Filter... filters) {
        return entryInfoList(new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QList<java.lang.String> entryList(io.qt.core.QDir.Filter... filters) {
        return entryList(new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QList<java.lang.String> entryList(java.util.Collection<java.lang.String> nameFilters, io.qt.core.QDir.Filter... filters) {
        return entryList(nameFilters, new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
    
    @io.qt.QtUninvokable
    public final io.qt.core.QList<io.qt.core.QFileInfo> entryInfoList(java.util.Collection<java.lang.String> nameFilters, io.qt.core.QDir.Filter... filters) {
        return entryInfoList(nameFilters, new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
}// class

class QFile__{
    /**
     * Result class for {@link #moveToTrash(java.lang.String)}
     */
    public static class TrashResult{
        public final boolean success;
        public final String pathInTrash;
        private TrashResult(boolean success, String pathInTrash) {
            super();
            this.success = success;
            this.pathInTrash = pathInTrash;
        }
    }
}// class

class QUntypedPropertyBinding_java__{
    QUntypedPropertyBinding(Object functor){
        __qt_QUntypedPropertyBinding_new_functor(this, java.util.Objects.requireNonNull(functor));
    }
    private native static void __qt_QUntypedPropertyBinding_new_functor(Object instance, Object functor);
    
    QUntypedPropertyBinding(QPropertyBindingData bindingData) {
        __qt_QUntypedPropertyBinding_new_bindingData(this, bindingData);
    }
    private native static void __qt_QUntypedPropertyBinding_new_bindingData(Object instance, QPropertyBindingData bindingData);
    
    QUntypedPropertyBinding(QUntypedPropertyBinding other, boolean copy){
        super((QPrivateConstructor)null);
        __qt_QUntypedPropertyBinding_new_copy(this, other);
    }
    
    private native static void __qt_QUntypedPropertyBinding_new_copy(Object instance, QUntypedPropertyBinding other);
}// class

class QPropertyObserver_java__{
    QPropertyObserver(io.qt.core.QUntypedPropertyData aliasedPropertyPtr, boolean isAlias){
        super((QPrivateConstructor)null);
        __qt_QPropertyObserver_newAlias(this, aliasedPropertyPtr);
    }
    
    private native static void __qt_QPropertyObserver_newAlias(Object instance, io.qt.core.QUntypedPropertyData aliasedPropertyPtr);
    
    QPropertyObserver(boolean isPropertyChangeHandler){
        super((QPrivateConstructor)null);
        __qt_QPropertyObserver_newPropertyChangeHandler(this);
    }
    
    private native static void __qt_QPropertyObserver_newPropertyChangeHandler(Object instance);
}// class

class QPropertyObserver_shell__{
public:
    QPropertyObserver_shell(QPropertyObserverBase::ChangeHandler);
}// class

class QPropertyObserver_native__{

QPropertyObserver_shell::QPropertyObserver_shell(QPropertyObserverBase::ChangeHandler changeHandler0)
    : QPropertyObserver(changeHandler0)
{
    QTJAMBI_DEBUG_METHOD_PRINT_WHERE("shell", "QPropertyObserver_shell::QPropertyObserver_shell(QPropertyObserverBase::ChangeHandler changeHandler0)", __shell())
    __shell()->constructed(typeid(QPropertyObserver));
}

// new QPropertyChangeHandler()
void __qt_construct_QPropertyChangeHandler(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QPropertyChangeHandler()")
    new(__qtjambi_ptr) QPropertyObserver_shell([](QPropertyObserver* self, QUntypedPropertyData *){
            try{
                if(JNIEnv *env = qtjambi_current_environment()) {
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    try{
                        jobject _self = qtjambi_cast<jobject>(env, self);
                        Java::QtCore::QPropertyChangeHandler::invoke(env, _self);
                    }catch(const JavaException& exn){
                        exn.report(env);
                    }
                }
            }catch(...){}
        });
}

}// class

class QUntypedBindable_java__{
    QUntypedBindable(io.qt.core.QUntypedPropertyData d, io.qt.core.QBindableInterface i, boolean copy){
        super((QPrivateConstructor)null);
        __qt_QUntypedBindable_new_copy(this, d, i);
    }
    
    private native static void __qt_QUntypedBindable_new_copy(Object instance, io.qt.core.QUntypedPropertyData d, io.qt.core.QBindableInterface i);
    
    /**
     * <p>Registers the given functor f as a callback that shall be called whenever the value of the bindable changes.</p>
     * <p>The returned property change handler object keeps track of the registration. 
     * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
     * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
     * @param f
     * @return property change handler
     * @see QPropertyChangeHandler
     */
    @io.qt.QtUninvokable
    public final QPropertyChangeHandler onValueChanged(Runnable f)
    {
        QPropertyChangeHandler handler = new QPropertyChangeHandler(f);
        observe(handler);
        return handler;
    }

    /**
     * Subscribes the given functor f as a callback that is called immediately and whenever the value of the bindable changes in the future.
     * @param f
     * @return property change handler
     * @see QPropertyChangeHandler
     * @see #onValueChanged(Runnable)
     */
    @io.qt.QtUninvokable
    public final QPropertyChangeHandler subscribe(Runnable f)
    {
        f.run();
        return onValueChanged(f);
    }
    
    @io.qt.QtUninvokable
    final io.qt.core.QUntypedPropertyBinding overrideBinding(io.qt.core.QUntypedPropertyBinding binding){
        return overrideBinding(checkedNativeId(this.iface()), checkedNativeId(this.data()), checkedNativeId(binding));
    }
    
    @io.qt.QtUninvokable
    private native io.qt.core.QUntypedPropertyBinding overrideBinding(long ifaceId, long dataId, long binding);
    
}// class

class QBindableInterface__{
    @io.qt.QtUninvokable
    final QMetaType metaType() { return metaType(io.qt.internal.QtJambiInternal.nativeId(this)); }
    
    @io.qt.QtUninvokable
    private static native QMetaType metaType(long nativeId);
}// class

class QLoggingCategory__{
    
    public QLoggingCategory(java.lang.String category){
        this(category==null ? null : new QByteArray(category));
    }
    
    public QLoggingCategory(java.lang.String category, io.qt.core.QtMsgType severityLevel){
        this(category==null ? null : new QByteArray(category), severityLevel);
    }
    
}// class

