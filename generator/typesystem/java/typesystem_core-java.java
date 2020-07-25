/****************************************************************************
 **
 ** Copyright (C) 1992-2009 Nokia. All rights reserved.
 ** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

    @io.qt.QtUninvokable
    public final QObject findChild() {
        return findChild(QObject.class);
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl) {
        return findChild(cl, (String)null, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl, String name) {
        return findChild(cl, name, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl, String name, Qt.FindChildOption... options) {
        return findChild(cl, name, new Qt.FindChildOptions(options));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> T findChild(Class < T > cl, String name, Qt.FindChildOptions options) {
        return findChild(nativeId(this), java.util.Objects.requireNonNull(cl), QMetaObject.forType(cl).metaObjectPointer, name, options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> T findChild(long nativeId, Class < T > cl, long metaObjectPointer, String name, int options);
    
    @io.qt.QtUninvokable
    public final java.util.List<QObject> findChildren() {
        return findChildren(QObject.class, (String)null, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl) {
        return findChildren(cl, (String)null, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, String name) {
        return findChildren(cl, name, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, String name, Qt.FindChildOption... options) {
        return findChildren(cl, name, new Qt.FindChildOptions(options));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, String name, Qt.FindChildOptions options){
        return findChildrenString(nativeId(this), java.util.Objects.requireNonNull(cl), QMetaObject.forType(cl).metaObjectPointer, name, options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> java.util.List<T> findChildrenString(long nativeId, Class < T > cl, long metaObjectPointer, String name, int options);
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, QRegularExpression re) {
        return findChildren(cl, re, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, QRegularExpression re, Qt.FindChildOption... options) {
        return findChildren(cl, re, new Qt.FindChildOptions(options));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, QRegularExpression re, Qt.FindChildOptions options){
        return findChildrenQRegularExpression(nativeId(this), java.util.Objects.requireNonNull(cl), QMetaObject.forType(cl).metaObjectPointer, nativeId(re), options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> java.util.List<T> findChildrenQRegularExpression(long nativeId, Class < T > cl, long metaObjectPointer, long re, int options);
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, QRegExp regExp) {
        return findChildren(cl, regExp, new Qt.FindChildOptions(Qt.FindChildOption.FindChildrenRecursively));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, QRegExp regExp, Qt.FindChildOption... options) {
        return findChildren(cl, regExp, new Qt.FindChildOptions(options));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QObject> java.util.List<T> findChildren(Class < T > cl, QRegExp regExp, Qt.FindChildOptions options){
        return findChildrenQRegExp(nativeId(this), java.util.Objects.requireNonNull(cl), QMetaObject.forType(cl).metaObjectPointer, nativeId(regExp), options.value());
    }
    
    @io.qt.QtUninvokable
    private native final <T extends QObject> java.util.List<T> findChildrenQRegExp(long nativeId, Class < T > cl, long metaObjectPointer, long regExp, int options);
    
    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes no
     * parameters.
     */
    public final class Signal0 extends QMetaObject.AbstractPublicSignal0 {
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes one
     * parameter.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public final class Signal1<A> extends QMetaObject.AbstractPublicSignal1<A> {
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes two
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class Signal2<A, B> extends QMetaObject.AbstractPublicSignal2<A, B> {
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes three
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class Signal3<A, B, C> extends QMetaObject.AbstractPublicSignal3<A, B, C> {
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes four
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */

    public final class Signal4<A, B, C, D> extends QMetaObject.AbstractPublicSignal4<A, B, C, D> {
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes five
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5<A, B, C, D, E> extends QMetaObject.AbstractPublicSignal5<A, B, C, D, E> {
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes six
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
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes seven
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
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes eight
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
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes nine
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
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal2Default1<A, B> extends QMetaObject.AbstractSignal2Default1<A, B>{
        public Signal2Default1(Supplier<B> arg2Default) {
            super(arg2Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal3Default1<A, B, C> extends QMetaObject.AbstractSignal3Default1<A, B, C> {
        public Signal3Default1(Supplier<C> arg3Default) {
            super(arg3Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal3Default2<A, B, C> extends QMetaObject.AbstractSignal3Default2<A, B, C> {
        public Signal3Default2(Supplier<B> arg2Default, Supplier<C> arg3Default){
            super(arg2Default, arg3Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal4Default1<A, B, C, D> extends QMetaObject.AbstractSignal4Default1<A, B, C, D>{
        public Signal4Default1(Supplier<D> arg4Default){
            super(arg4Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal4Default2<A, B, C, D> extends QMetaObject.AbstractSignal4Default2<A, B, C, D>{
        public Signal4Default2(Supplier<C> arg3Default, Supplier<D> arg4Default){
            super(arg3Default, arg4Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal4Default3<A, B, C, D> extends QMetaObject.AbstractSignal4Default3<A, B, C, D>{
        public Signal4Default3(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default){
            super(arg2Default, arg3Default, arg4Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal5Default1<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default1<A, B, C, D, E>{
        public Signal5Default1(Supplier<E> arg5Default){
            super(arg5Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal5Default2<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default2<A, B, C, D, E>{
        public Signal5Default2(Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg4Default, arg5Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal5Default3<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default3<A, B, C, D, E>{
        public Signal5Default3(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg3Default, arg4Default, arg5Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal5Default4<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default4<A, B, C, D, E>{
        public Signal5Default4(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal6Default1<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default1<A, B, C, D, E, F>{
        public Signal6Default1(Supplier<F> arg6Default){
            super(arg6Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal6Default2<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default2<A, B, C, D, E, F>{
        public Signal6Default2(Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg5Default, arg6Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal6Default3<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default3<A, B, C, D, E, F>{
        public Signal6Default3(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg4Default, arg5Default, arg6Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal6Default4<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default4<A, B, C, D, E, F>{
        public Signal6Default4(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal6Default5<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default5<A, B, C, D, E, F>{
        public Signal6Default5(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal7Default1<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default1<A, B, C, D, E, F, G>{
        public Signal7Default1(Supplier<G> arg7Default){
            super(arg7Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal7Default2<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default2<A, B, C, D, E, F, G> {
        
        public Signal7Default2(Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg6Default, arg7Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal7Default3<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default3<A, B, C, D, E, F, G> {
        
        public Signal7Default3(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg5Default, arg6Default, arg7Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal7Default4<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default4<A, B, C, D, E, F, G> {
        
        public Signal7Default4(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal7Default5<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default5<A, B, C, D, E, F, G> {
        
        public Signal7Default5(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal7Default6<A, B, C, D, E, F, G> extends QMetaObject.AbstractSignal7Default6<A, B, C, D, E, F, G> {
        
        public Signal7Default6(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default1<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default1<A, B, C, D, E, F, G, H>{
        public Signal8Default1(Supplier<H> arg8Default){
            super(arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default2<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default2<A, B, C, D, E, F, G, H> {
        public Signal8Default2(Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg7Default, arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default3<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default3<A, B, C, D, E, F, G, H> {
        public Signal8Default3(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg6Default, arg7Default, arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default4<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default4<A, B, C, D, E, F, G, H> {
        public Signal8Default4(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default5<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default5<A, B, C, D, E, F, G, H> {
        public Signal8Default5(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default6<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default6<A, B, C, D, E, F, G, H> {
        public Signal8Default6(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal8Default7<A, B, C, D, E, F, G, H> extends QMetaObject.AbstractSignal8Default7<A, B, C, D, E, F, G, H> {
        public Signal8Default7(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default1<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default1<A, B, C, D, E, F, G, H, I>{
        public Signal9Default1(Supplier<I> arg9Default){
            super(arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default2<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> {
        public Signal9Default2(Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default3<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> {
        public Signal9Default3(Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg7Default, arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default4<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> {
        public Signal9Default4(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default5<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> {
        public Signal9Default5(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default6<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> {
        public Signal9Default6(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default7<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> {
        public Signal9Default7(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    public final class Signal9Default8<A, B, C, D, E, F, G, H, I> extends QMetaObject.AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> {
        public Signal9Default8(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes no
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes one
     * parameter.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes two
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes three
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes four
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes five
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes six
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes seven
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes eight
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    }
    
    protected static void emit(PrivateSignal0 signal) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit();
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A> void emit(PrivateSignal1<A> signal, A arg1) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B> void emit(PrivateSignal2<A,B> signal, A arg1, B arg2) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C> void emit(PrivateSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C,D> void emit(PrivateSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C,D,E> void emit(PrivateSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C,D,E,F> void emit(PrivateSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C,D,E,F,G> void emit(PrivateSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C,D,E,F,G,H> void emit(PrivateSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected static <A,B,C,D,E,F,G,H,I> void emit(PrivateSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws io.qt.QSignalAccessException {
        Class<?> callerClass = callerClassProvider().get();
        Class<?> signalDeclaringClass = signalDeclaringClass(signal);
        if(callerClass==signalDeclaringClass) {
            signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }else {
            throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
        }
    }
    
    protected final static class SignalConfiguration extends QMetaObject.SignalConfiguration{
        public SignalConfiguration(QMetaObject.AbstractPrivateSignal0 signal) {
            super(signal);
        }
        
        public <A> SignalConfiguration(Class<A> type1, QMetaObject.AbstractPrivateSignal1<A> signal) {
            super(signal, type1);
        }
        
        public <A,B> SignalConfiguration(Class<A> type1, Class<B> type2, QMetaObject.AbstractPrivateSignal2<A,B> signal) {
            super(signal, type1, type2);
        }
        
        public <A,B,C> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, QMetaObject.AbstractPrivateSignal3<A,B,C> signal) {
            super(signal, type1, type2, type3);
        }
        
        public <A,B,C,D> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal) {
            super(signal, type1, type2, type3, type4);
        }
        
        public <A,B,C,D,E> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal) {
            super(signal, type1, type2, type3, type4, type5);
        }
        
        public <A,B,C,D,E,F> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal) {
            super(signal, type1, type2, type3, type4, type5, type6);
        }
        
        public <A,B,C,D,E,F,G> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, Class<G> type7, QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal) {
            super(signal, type1, type2, type3, type4, type5, type6, type7);
        }
        
        public <A,B,C,D,E,F,G,H> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, Class<G> type7, Class<H> type8, QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal) {
            super(signal, type1, type2, type3, type4, type5, type6, type7, type8);
        }
        
        public <A,B,C,D,E,F,G,H,I> SignalConfiguration(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
                Class<F> type6, Class<G> type7, Class<H> type8, Class<I> type9, QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal) {
            super(signal, type1, type2, type3, type4, type5, type6, type7, type8, type9);
        }
        
        public SignalConfiguration(QDeclarableSignals.AbstractPrivateGenericSignal signal) {
            super(signal, (Class<?>[])null);
        }
    }
    
    public class MultiSignal extends QMetaObject.AbstractMultiSignal {
        @SuppressWarnings("exports")
        public MultiSignal(SignalConfiguration signal1, SignalConfiguration signal2, SignalConfiguration... furtherSignals){
            super(signal1, signal2, furtherSignals);
        }
        
        @Override
        public QObject containingObject() {
            return QObject.this;
        }
    
        /**
         * Removes the given connection from this signal.
         *
         * @param connection the connection to be removed
         * @return  True if the disconnection was successful.
         */
        public final boolean disconnect(QMetaObject.Connection connection) {
            return super.disconnect(connection);
        }
        
        public final boolean disconnectAll() {
            return super.disconnectAll();
        }
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal0 signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal1<?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal1<A> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal2<?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal2<A,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal2<A,B> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<A,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<A,B,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal3<A,B,C> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,B,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,B,C,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,C,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F,G> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F,G> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F,G,H> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<?,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,?,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,?,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,?,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,?,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,?,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F,G> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,?,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F,G,H> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,?> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static <A,B,C,D,E,F,G,H,I> QMetaObject.Connection connect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, QMetaObject.Slot9<A,B,C,D,E,F,G,H,I> slot, Qt.ConnectionType... connectionType) {
        return signal.connect(slot, connectionType);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal0 signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal1<?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal1<A> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal2<?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal2<A,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal2<A,B> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal3<?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal3<A,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal3<A,B,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal3<A,B,C> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal4<?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,B,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,B,C,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal4<A,B,C,D> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal5<?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,C,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal6<?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal7<?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F,G> boolean disconnect(QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal8<?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F,G> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F,G,H> boolean disconnect(QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot) {
        return signal.disconnect(slot);
    }
    
    public static boolean disconnect(QMetaObject.AbstractPrivateSignal9<?,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot0 slot) {
        return signal.disconnect(slot);
    }
    
    public static <A> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,?,?,?,?,?,?,?,?> signal, QMetaObject.Slot1<A> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,?,?,?,?,?,?,?> signal, QMetaObject.Slot2<A,B> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,?,?,?,?,?,?> signal, QMetaObject.Slot3<A,B,C> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,?,?,?,?,?> signal, QMetaObject.Slot4<A,B,C,D> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,?,?,?,?> signal, QMetaObject.Slot5<A,B,C,D,E> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,?,?,?> signal, QMetaObject.Slot6<A,B,C,D,E,F> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F,G> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,?,?> signal, QMetaObject.Slot7<A,B,C,D,E,F,G> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F,G,H> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,?> signal, QMetaObject.Slot8<A,B,C,D,E,F,G,H> slot) {
        return signal.disconnect(slot);
    }
    
    public static <A,B,C,D,E,F,G,H,I> boolean disconnect(QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, QMetaObject.Slot9<A,B,C,D,E,F,G,H,I> slot) {
        return signal.disconnect(slot);
    }
    
    /**
     * Creates a connection of the given @param type from the @param signal in
     * the @param sender object to the @param method in the @param receiver object.
     * Returns a handle to the connection that can be used to disconnect
     * it later.
     * 
     * <p><tt>
     * QLabel label = new QLabel();
     * QScrollBar scrollBar = new QScrollBar();
     * QObject.connect(scrollBar, "valueChanged(int)", label,  "setNum(int)");
     * </tt></p>
     * 
     * This example ensures that the label always displays the current
     * scroll bar value. Note that the signal and slots parameters must not
     * contain any variable names, only the type. E.g. the following would
     * not work and return false:
     * 
     * <p><tt>
     * QObject.connect(scrollBar, "valueChanged(int value)", label,  "setNum(int value)");
     * </tt></p>
     * 
     * A signal can also be connected to another signal:
     * 
     * <p><tt>
     * public class MyWidget extends QWidget
     * {
     *     public final Signal0 buttonClicked = new Signal0();
     * 
     *     public MyWidget(){
     *         myButton = new QPushButton(this);
     *         connect(myButton, "clicked()", this, "buttonClicked()");
     *     }
     * 
     *     private QPushButton myButton;
     * }
     * </tt></p>
     * 
     * or alternatively:
     * 
     * <p><tt>
     * clicked.connect(myButton.buttonClicked);
     * </tt></p>
     * 
     * In this example, the @code MyWidget constructor relays a signal from
     * a private member variable, and makes it available under a name
     * that relates to @code MyWidget.
     * 
     * A signal can be connected to many slots and signals. Many signals
     * can be connected to one slot.
     * 
     * If a signal is connected to several slots, the slots are activated
     * in the same order in which the connections were made, when the
     * signal is emitted.
     * 
     * In case the connection was successfully initialized the function returns 
     * a QMetaObject.Connection that represents a handle to this connection. 
     * The connection handle will be invalid
     * if it cannot create the connection, for example, if QObject is unable
     * to verify the existence of either @param signal or @param method, or if their
     * signatures aren't compatible.
     * In this case, the function returns null.
     * 
     * By default, a signal is emitted for every connection you make;
     * two signals are emitted for duplicate connections. You can break
     * all of these connections with a single disconnect() call.
     * If you pass the Qt.ConnectionType.UniqueConnection @param type, the connection will only
     * be made if it is not a duplicate. If there is already a duplicate
     * (exact same signal to the exact same slot on the same objects),
     * the connection will fail and connect will return null.
     * 
     * Note: Qt.ConnectionType.UniqueConnections do not work for lambdas; 
     * they only apply to connecting to member functions.
     * 
     * The optional @param type parameter describes the type of connection
     * to establish. In particular, it determines whether a particular
     * signal is delivered to a slot immediately or queued for delivery
     * at a later time. If the signal is queued, the parameters must be
     * of types that are known to Qt's meta-object system, because Qt
     * needs to copy the arguments to store them in an event behind the
     * scenes. If you try to use a queued connection and get the error
     * message
     * 
     * <p><tt>
     * QObject::connect: Cannot queue arguments of type 'MyType'
     * (Make sure 'MyType' is registered using qRegisterMetaType().)
     * </tt></p>
     * 
     * call QMetaType.qRegisterMetaType() to register the data type before you
     * establish the connection.
     * 
     * @see #disconnect()
     * @see #sender()
     * @see QMetaType#qRegisterMetaType()
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
     * Creates a connection of the given <tt>type</tt> from the <tt>signal</tt> in
     * the <tt>sender</tt> object to the <tt>method</tt> in the <tt>receiver</tt> object.
     * Returns a handle to the connection that can be used to disconnect it later.
     *
     * The Connection handle will be invalid  if it cannot create the
     * connection, for example, the parameters were invalid.
     * You can check if a QMetaObject.Connection is returned.
     *
     * This function works in the same way as
     * {@link #connect(QObject, String, QObject, String, io.qt.core.Qt.ConnectionType...)}
     * but it uses QMetaMethod to specify signal and method.
     *
     * @see #connect(QObject, String, QObject, String, io.qt.core.Qt.ConnectionType...)
     */
    public static QMetaObject.Connection connect(QObject sender, QMetaMethod signal, QObject receiver, QMetaMethod slot, Qt.ConnectionType... connectionType) {
        java.util.Objects.requireNonNull(signal);
        long sender_id = checkedNativeId(sender);
        long receiver_id = checkedNativeId(receiver);
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
     * Disconnects <i>signal</i> in object sender from method in object
     * <i>receiver</i>. Returns <tt>true</tt> if the connection is successfully broken;
     * otherwise returns <tt>false</tt>.
     * 
     * A signal-slot connection is removed when either of the objects
     * involved are destroyed.
     * 
     * disconnect() is typically used in three ways, as the following
     * examples demonstrate.
     * <ol>
     * <li>Disconnect everything connected to an object's signals:
     * 
     *    <p><tt>disconnect(myObject, null, null, null);</tt></p>
     * 
     *    equivalent to the non-static overloaded function
     * 
     *    <p><tt>myObject.disconnect();</tt></p>
     * </li>
     * <li>Disconnect everything connected to a specific signal:
     * 
     *    <p><tt>disconnect(myObject, "mySignal()", null, null);</tt></p>
     * 
     *    equivalent to the non-static overloaded function
     * 
     *    <p><tt>myObject.mySignal.disconnect();</tt></p>
     * </li>
     * <li>Disconnect a specific receiver:
     * 
     *    <p><tt>disconnect(myObject, null, myReceiver, null);</tt></p>
     * 
     *    equivalent to the non-static overloaded function
     * 
     *    <p><tt>myObject.disconnect(myReceiver);</tt></p>
     * </li>
     * </ol>
     * 
     * 0 may be used as a wildcard, meaning "any signal", "any receiving
     * object", or "any slot in the receiving object", respectively.
     * 
     * The <i>sender</i> may never be 0. (You cannot disconnect signals from
     * more than one object in a single call.)
     * 
     * If <i>signal</i> is 0, it disconnects <i>receiver</i> and <i>method</i> from
     * any signal. If not, only the specified signal is disconnected.
     * 
     * If <i>receiver</i> is 0, it disconnects anything connected to
     * <i>signal</i>. If not, QMetaObject.Slots in objects other than <i>receiver</i> are not
     * disconnected.
     * 
     * If <i>method</i> is 0, it disconnects anything that is connected to
     * <i>receiver</i>. If not, only slots named <i>method</i> will be disconnected,
     * and all other slots are left alone. The <i>method</i> must be 0 if 
     * <i>receiver</i> is left out, so you cannot disconnect a
     * specifically-named slot on all objects.
     * 
     * @see connect()
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
     * Disconnects <i>signal</i> in object <i>sender</i> from <i>method</i> in object
     * <i>receiver</i> receiver. Returns <tt>true</tt> if the connection is successfully broken;
     * otherwise returns <tt>false</tt>.
     *
     * This function provides the same possibilities like
     * {@link #disconnect(QObject, String, QObject, String)}
     * but uses QMetaMethod to represent the signal and the method to be disconnected.
     *
     * Additionally this function returns false and no signals and slots disconnected
     * if:
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
     * QMetaMethod() may be used as wildcard in the meaning "any signal" or "any slot in receiving object".
     * In the same way 0 can be used for <i>receiver</i> in the meaning "any receiving object". In this case
     * method should also be QMetaMethod(). <i>sender</i> parameter should be never 0.

     * @see #disconnect(QObject, String, QObject, String)
     */
    public static boolean disconnect(QObject sender, QMetaMethod signal, QObject receiver, QMetaMethod slot) {
        long sender_id = checkedNativeId(sender);
        long receiver_id = 0;
        if(receiver!=null) {
            receiver_id = checkedNativeId(receiver);
        }
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
     * Disconnect a connection.
     * 
     * If the connection is invalid or has already been disconnected, do nothing
     * and return false.
     * 
        * @see connect()
     */
    public static boolean disconnect(QMetaObject.Connection connection) {
        return io.qt.internal.QtJambiInternal.disconnect(connection);
    }

    /**
     * Disconnect all connections originating in this object.
     */
    public final boolean disconnect() {
        return disconnectAll(this, null);
    }

    /**
    * Disconnect all connections made from this signal emitter to a specific object.
    *
    * @param other The receiver to disconnect, or null to disconnect all receivers
    */
    public final boolean disconnect(Object receiver) {
        if(receiver instanceof QMetaObject.Connection)
            return disconnect((QMetaObject.Connection)receiver);
        else return disconnectAll(this, receiver);
    }

    @io.qt.QtUninvokable
    public final QMetaObject metaObject() {
        return QMetaObject.forQObject(checkedNativeId(this));
    }
    
    @io.qt.QtUninvokable
    public final <T extends QtObjectInterface> T qt_metacast(Class<T> targetType) {
        return QMetaObject.cast(targetType, this);
    }
    
    @io.qt.QtUninvokable
    public final boolean inherits(Class<?> targetType){
        return targetType.isInstance(this) || inherits(io.qt.internal.MetaObjectTools.internalNameOfArgumentType(targetType).replace("*", ""));
    }
    
    @io.qt.QtUninvokable
    protected final int receivers(io.qt.core.QMetaObject.AbstractSignal signalObject){
        String signal = io.qt.internal.MetaObjectTools.cppSignalSignature(signalObject);
        if(signal.isEmpty())
          return 0;
        return __qt_QObject_receivers_const_char_ptr_constfct(checkedNativeId(this), signal);
    }
}// class

class QObject_native__{

// QObject::findChild(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChild__JLjava_lang_Class_2JLjava_lang_String_2I)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jclass cl0,
 jlong metaObjectPointer1,
 jobject name2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChild(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        QString __qt_name2(qtjambi_to_qstring(__jni_env, jstring(name2)));
        QObject * __qt_return = qt_qFindChild_helper(__qt_this, __qt_name2, *__qt_metaObjectPointer1, Qt::FindChildOptions(options3));
        return qtjambi_from_qobject(__jni_env, __qt_return, cl0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

// QObject::findChildren(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenString__JLjava_lang_Class_2JLjava_lang_String_2I)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jclass cl0,
 jlong metaObjectPointer1,
 jobject name2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QString &name = QString(), Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        QString __qt_name2(qtjambi_to_qstring(__jni_env, jstring(name2)));
        QList<void *> list;
        qt_qFindChildren_helper(__qt_this, __qt_name2, *__qt_metaObjectPointer1, &list, Qt::FindChildOptions(options3));
        JObjectWrapper cl0Wrapper(__jni_env, cl0);
        return qtjambi_to_ArrayList<QList, void *>(
                __jni_env, 
                list, 
                [cl0Wrapper](JNIEnv *__jni_env, void *const& p) -> jobject {
                    return qtjambi_from_qobject(__jni_env, reinterpret_cast<const QObject *>(p), jclass(cl0Wrapper.object()));
                });
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

// QObject::findChildren(const QRegExp &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenQRegExp__JLjava_lang_Class_2JJI)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jclass cl0,
 jlong metaObjectPointer1,
 QtJambiNativeID regExp2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QRegExp &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        const QRegExp& __qt_regExp2 = qtjambi_value_from_nativeId<QRegExp>(regExp2);
        QList<void *> list;
        qt_qFindChildren_helper(__qt_this, __qt_regExp2, *__qt_metaObjectPointer1, &list, Qt::FindChildOptions(options3));
        JObjectWrapper cl0Wrapper(__jni_env, cl0);
        return qtjambi_to_ArrayList<QList, void *>(
                __jni_env, 
                list, 
                [cl0Wrapper](JNIEnv *__jni_env, void * const & p) -> jobject {
                    return qtjambi_from_qobject(__jni_env, reinterpret_cast<const QObject *>(p), jclass(cl0Wrapper.object()));
                });
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

// QObject::findChildren(const QRegularExpression &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QObject_findChildrenQRegularExpression__JLjava_lang_Class_2JJI)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jclass cl0,
 jlong metaObjectPointer1,
 QtJambiNativeID regExp2,
 jint options3)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QObject::findChildren(const QRegularExpression &regExp, Qt::FindChildOptions options = Qt::FindChildrenRecursively) const")
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QMetaObject *__qt_metaObjectPointer1 = reinterpret_cast<const QMetaObject *>(metaObjectPointer1);
        const QRegularExpression& __qt_regExp2 = qtjambi_value_from_nativeId<QRegularExpression>(regExp2);
        QList<void *> list;
        qt_qFindChildren_helper(__qt_this, __qt_regExp2, *__qt_metaObjectPointer1, &list, Qt::FindChildOptions(options3));
        JObjectWrapper cl0Wrapper(__jni_env, cl0);
        return qtjambi_to_ArrayList<QList, void *>(
                __jni_env, 
                list, 
                [cl0Wrapper](JNIEnv *__jni_env, void * const & p) -> jobject {
                    return qtjambi_from_qobject(__jni_env, reinterpret_cast<const QObject *>(p), jclass(cl0Wrapper.object()));
                });
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}
 
}// class

class QSignalTransition___{
    
    public QSignalTransition(io.qt.core.QMetaObject.AbstractSignal signal) {
        this(signal, null);
    }
    
    public QSignalTransition(io.qt.core.QMetaObject.AbstractSignal signal, io.qt.core.QState sourceState) {
        super((QPrivateConstructor)null);
        constructorThreadCheck(this);
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            String signalSignature = io.qt.internal.MetaObjectTools.cppSignalSignature(signal);
            __qt_QSignalTransition_new_const_QObject_ptr_const_char_ptr_QState_ptr(this, sender, signalSignature, sourceState);
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }
    
    @io.qt.QtUninvokable
    public final void setSignal(io.qt.core.QMetaObject.AbstractSignal signal){
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            String signalSignature = io.qt.internal.MetaObjectTools.cppSignalSignature(signal);
            __qt_QSignalTransition_setSenderObject_const_QObject_ptr(checkedNativeId(this), nativeId(sender));
            __qt_QSignalTransition_setSignal_cref_QByteArray(checkedNativeId(this), nativeId(new QByteArray(signalSignature)));
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }
    
}// class

class QState___{
    
    @io.qt.QtUninvokable
    public final io.qt.core.QSignalTransition addTransition(io.qt.core.QMetaObject.AbstractSignal signal, io.qt.core.QAbstractState target) {
        if(signal.containingObject() instanceof QObject) {
            QObject sender = (QObject)signal.containingObject();
            String signalSignature = io.qt.internal.MetaObjectTools.cppSignalSignature(signal);
            return addTransition(sender, signalSignature, target);
        }else {
            throw new IllegalArgumentException("Signal is not owned by a QObject.");
        }
    }
    
}// class

abstract class QUrl___ extends QUrl{
    
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

    public static java.util.List<java.lang.String> toStringList(java.util.List<io.qt.core.QUrl> uris, FormattingOptions options){
        return toStringList(uris, options.value());
    }
    
    public static java.util.List<java.lang.String> toStringList(java.util.List<io.qt.core.QUrl> uris, FormattingOption... options){
        return toStringList(uris, new FormattingOptions(options).value());
    }
    
    public static java.util.List<java.lang.String> toStringList(java.util.List<io.qt.core.QUrl> uris){
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

    public static QCoreApplication initialize(String args[]) {
        return initialize(null, args, QCoreApplication::new);
    }

    public static QCoreApplication initialize(String applicationName, String args[]) {
        return initialize(applicationName, args, QCoreApplication::new);
    }

    public static <T extends QCoreApplication> T initialize(String args[], java.util.function.Function<String[],T> constructor) {
        return initialize(null, args, constructor);
    }
    
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
    
    public native static void addPreRoutine(Runnable startUpFunction);
    
    public native static void addPostRoutine(Runnable cleanUpFunction);
    
    public native static void removePostRoutine(Runnable cleanUpFunction);
}// class

class QCoreApplication_native__ {


extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_preinit)
(JNIEnv *__jni_env, jclass)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCoreApplication::preinit()")
    try{
#ifdef Q_OS_DARWIN
        if (!pthread_main_np()) {
            qWarning("WARNING: QtJambi does not appear to be running on the main thread and will "
                     "most likely be unstable and crash. "
                     "Please make sure to launch your 'java' command with the "
                     "'-XstartOnFirstThread' command line option. For instance: "
                     "java -XstartOnFirstThread any.vendor.MainClass");
             JavaException::raiseError(__jni_env,
                     "QtJambi does not appear to be running on the main() thread." QTJAMBI_STACKTRACEINFO );
        }
#endif
        QCoreApplicationPrivate::theMainThread = QThread::currentThread();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

typedef QList<JObjectWrapper> RoutineList;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gRoutineLock, (QReadWriteLock::Recursive))
Q_GLOBAL_STATIC(RoutineList, gPreRoutines)
Q_GLOBAL_STATIC(RoutineList, gPostRoutines)

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_addPreRoutine)
(JNIEnv *__jni_env, jclass, jobject cleanUpFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qAddPostRoutine(QtCleanUpFunction)")
    
    if (QCoreApplication::instance()){
        Java::Runtime::Runnable.run(__jni_env, cleanUpFunction);
    }else if(!gPreRoutines.isDestroyed()){
        QWriteLocker locker(gRoutineLock());
        gPreRoutines->append(JObjectWrapper(__jni_env, cleanUpFunction));
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_addPostRoutine)
(JNIEnv *__jni_env, jclass, jobject cleanUpFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qAddPostRoutine(QtCleanUpFunction)")
    if(!gPostRoutines.isDestroyed()){
        QWriteLocker locker(gRoutineLock());
        gPostRoutines->prepend(JObjectWrapper(__jni_env, cleanUpFunction));
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_removePostRoutine)
(JNIEnv *__jni_env, jclass, jobject cleanUpFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qAddPostRoutine(QtCleanUpFunction)")
    if(!gPostRoutines.isDestroyed()){
        QWriteLocker locker(gRoutineLock());
        gPostRoutines->removeAll(JObjectWrapper(__jni_env, cleanUpFunction));
    }
}

void initialize_pre_and_post_routines()
{
    qAddPreRoutine([](){
        if(JNIEnv *__jni_env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
            for(const JObjectWrapper& cleanUpFunction : *gPreRoutines){
                try{
                    Java::Runtime::Runnable.run(__jni_env, cleanUpFunction.object());
                }catch(const JavaException& exn){
                    exn.report(__jni_env);
                }
            }
        }
        gPreRoutines->clear();
    });
    qAddPostRoutine([](){
        if(JNIEnv *__jni_env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
            for(const JObjectWrapper& cleanUpFunction : *gPreRoutines){
                try{
                    Java::Runtime::Runnable.run(__jni_env, cleanUpFunction.object());
                }catch(const JavaException& exn){
                    exn.report(__jni_env);
                }
            }
        }
        gPostRoutines->clear();
    });
}

}// class

class QTranslator___ extends QTranslator {
}// class

class QItemSelection___ extends QItemSelection {
    
    public QItemSelection(java.util.Collection<io.qt.core.QItemSelectionRange> collection){
        this();
        append(collection);
    }
}// class

class QXmlStreamAttributes___ extends QXmlStreamAttributes {
    
    public QXmlStreamAttributes(java.util.Collection<io.qt.core.QXmlStreamAttribute> collection){
        this();
        append(collection);
    }
}// class

class QTextStream___ extends QTextStream {
    public CharSequence string() {
        QIODevice device = __qt_QTextStream_device_constfct(checkedNativeId(this));
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
    private static final class StringDevice<String extends Object & Appendable & CharSequence> extends QIODevice{
        public StringDevice(String string, io.qt.core.QIODevice.OpenMode openMode) {
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
        boolean[] res = new boolean[size()];

        for (int i = 0; i < size(); i++) {
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
    
    public final int compare(byte[] b) {
        return compare(java.nio.ByteBuffer.wrap(b));
    }

    public final int compare(byte[] b, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(java.nio.ByteBuffer.wrap(b), cs);
    }
    
    public final int compare(String str) {
        return compare(new QByteArray(str));
    }

    public final int compare(String str, io.qt.core.Qt.CaseSensitivity cs) {
        return compare(new QByteArray(str), cs);
    }
    
}// class

class QByteArray_5_15__ extends QByteArray {
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
    
    public QByteArray(String s) {
        this();
        append(s);
    }

    public QByteArray(byte data[]) {
        this(java.nio.ByteBuffer.wrap(data), data.length);
    }

    public QByteArray(byte data[], int size) {
        this(java.nio.ByteBuffer.wrap(data), size==-1 ? data.length : Math.min(size, data.length));
    }

    public final boolean contains(String str) {
        return contains(new QByteArray(str));
    }

    public final boolean contains(byte data[]) {
        return contains(java.nio.ByteBuffer.wrap(data));
    }

    public final int count(String str) {
        return count(new QByteArray(str));
    }

    public final int count(byte data[]) {
        return count(java.nio.ByteBuffer.wrap(data));
    }

    public final boolean endsWith(String str) {
        return endsWith(new QByteArray(str));
    }

    public final QByteArray prepend(String str) {
        return prepend(new QByteArray(str));
    }

    public final QByteArray replace(QByteArray before, String after) {
        return replace(before, new QByteArray(after));
    }

    public final QByteArray replace(String before, String after) {
        return replace(new QByteArray(before), new QByteArray(after));
    }

    public final boolean startsWith(String str) {
        return startsWith(new QByteArray(str));
    }

    public final QByteArray prepend(byte[] data) {
        this.prepend(java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }

    public final QByteArray replace(int index, int len, byte[] data) {
        this.replace(index, len, java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }

    public final QByteArray insert(int i, byte[] data) {
        this.insert(i, java.nio.ByteBuffer.wrap(data), data.length);
        return this;
    }
    
    public native static QByteArray join(java.util.Collection<QByteArray> stringList);
    
    public native static QByteArray join(java.util.Collection<QByteArray> stringList, byte sep);
    
    public static QByteArray join(java.util.Collection<QByteArray> stringList, QByteArray sep) {
        return join(stringList, checkedNativeId(sep));
    }
    
    private native static QByteArray join(java.util.Collection<QByteArray> stringList, long sep);
    
}// class

class QByteArray__native_ extends QByteArray {

// QByteArrayList::join()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 jobject list0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    try{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        QByteArray __qt_return_value = __qt_list0.join();
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QByteArrayList::join(char sep)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2B)
(JNIEnv *__jni_env,
 jclass,
 jobject list0,
 jbyte sep1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    try{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        QByteArray __qt_return_value = __qt_list0.join(char(sep1));
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QByteArrayList::join(const QByteArray & sep)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QByteArray_join__Ljava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 jobject list0,
 QtJambiNativeID sep1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QByteArrayList::join(const QByteArray & sep)")
    try{
        QtJambiScope __qtjambi_scope;
        const QByteArrayList&  __qt_list0 = qtjambi_cast<const QByteArrayList& >(__jni_env, __qtjambi_scope, list0);
        const QByteArray&  __qt_sep1 = qtjambi_value_from_nativeId<QByteArray>(sep1);
        QByteArray __qt_return_value = __qt_list0.join(__qt_sep1);
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

}// class

class QIODevice_java__ extends QIODevice {
        public native static QIODevice fromDirectBuffer(java.nio.Buffer buffer);
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

class QCalendar__native_ extends QCalendar {
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCalendar_unspecified__)
(JNIEnv *, jclass)
{
    return jint(QCalendar::Unspecified);
}
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

class QFutureWatcher___ extends QFutureWatcher {
    
    public final QFuture<T> future() {
        long nativeId = nativeId(this);
        if (nativeId == 0)
            throw new io.qt.QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
        return __qt_future(nativeId);
    }
    private native QFuture<T> __qt_future(long nativeId);

}// class

class QFutureWatcherVoid___ extends QFutureWatcherVoid {
    
    public final QFutureVoid future() {
        long nativeId = nativeId(this);
        if (nativeId == 0)
            throw new io.qt.QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
        return __qt_future(nativeId);
    }

    private native QFutureVoid __qt_future(long nativeId);

}// class

class QFutureSynchronizer___ extends QFutureSynchronizer {
    
    public final java.util.List<QFuture<T>> futures() {
        long nativeId = nativeId(this);
        if (nativeId == 0)
            throw new io.qt.QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
        return __qt_futures(nativeId);
    }
    private native java.util.List<QFuture<T>> __qt_futures(long nativeId);

}// class

class QFutureSynchronizerVoid___ extends QFutureSynchronizerVoid {
    
    public final java.util.List<QFutureVoid> futures() {
        long nativeId = nativeId(this);
        if (nativeId == 0)
            throw new io.qt.QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
        return __qt_futures(nativeId);
    }
    private native java.util.List<QFutureVoid> __qt_futures(long nativeId);

}// class

class QXmlStreamWriter___ extends QXmlStreamWriter {
}// class


class QJsonDocument___{
    
    public static final class FromJsonResult{
        private FromJsonResult(QJsonDocument document, QJsonParseError error) {
            super();
            this.document = document;
            this.error = error;
        }
        public final io.qt.core.QJsonDocument document;
        public final io.qt.core.QJsonParseError error;
    }
    
    public static io.qt.core.QJsonDocument fromRawData(String data) {
        return fromRawData(data, io.qt.core.QJsonDocument.DataValidation.Validate);
    }
    
    public static io.qt.core.QJsonDocument fromRawData(String data, io.qt.core.QJsonDocument.DataValidation validation) {
        return fromRawData(data.getBytes(), validation);
    }
    
    public static io.qt.core.QJsonDocument fromRawData(byte[] data) {
        return fromRawData(data, io.qt.core.QJsonDocument.DataValidation.Validate);
    }
    
    public static io.qt.core.QJsonDocument fromRawData(byte[] data, io.qt.core.QJsonDocument.DataValidation validation) {
        java.nio.ByteBuffer buffer = java.nio.ByteBuffer.wrap(data);
        return fromRawData(buffer, buffer.capacity(), validation);
    }
    
}// class

class QLockFile__{
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
    
    public QPropertyAnimation(io.qt.core.QObject target, String propertyName){
        this(target, new io.qt.core.QByteArray(propertyName), null);
    }
    
    public QPropertyAnimation(io.qt.core.QObject target, String propertyName, io.qt.core.QObject parent){
        this(target, new io.qt.core.QByteArray(propertyName), parent);
    }
    
    public final void setPropertyName(String propertyName)    {
        setPropertyName(new io.qt.core.QByteArray(propertyName));
    }
    
}// class

class QMetaType___ extends QMetaType {
    
    public static int registerMetaType(Class<?> clazz){
        return io.qt.internal.QtJambiInternal.registerMetaType(clazz);
    }
    
    public static int metaTypeId(Class<?> clazz){
        return io.qt.internal.QtJambiInternal.metaTypeId(clazz);
    }

    public interface GenericTypeInterface{
        @io.qt.QtUninvokable
        public int metaType();
    }
    
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
    
    public static final class GenericFlags extends io.qt.QFlags<GenericFlag> implements GenericTypeInterface{
        private static final long serialVersionUID = -7659504264600507749L;
    
        @io.qt.internal.NativeAccess
        private GenericFlags(int type, int value) {
            super(value);
            this.type = type;
        }
        
        private final int type;
        
        @Override
        public final int metaType() {
            return type;
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
        public final int metaType() {
            return type;
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
    
    public final static class GenericValue extends io.qt.QtObject implements GenericTypeInterface{
        private GenericValue(QPrivateConstructor p) { super(p); }
        
        @io.qt.internal.NativeAccess
        private final int type = QMetaType.Type.UnknownType.value();
        
        @io.qt.QtUninvokable
        public int metaType() {
            return type;
        }
        
        public java.nio.ByteBuffer buffer(){
            return QMetaType.buffer(checkedNativeId(this), type);
        }
    }
    
    public final static class GenericGadget extends io.qt.QtGadget implements GenericTypeInterface{
        private GenericGadget(QPrivateConstructor p) {super(p);}
        
        @io.qt.internal.NativeAccess
        private final int type = QMetaType.Type.UnknownType.value();
        
        @io.qt.QtUninvokable
        public int metaType() {
            return type;
        }
        
        public java.nio.ByteBuffer buffer(){
            return QMetaType.buffer(checkedNativeId(this), type);
        }
    }

    private static native java.nio.ByteBuffer buffer(long __this__nativeId, int type);
}// class

class QMetaType_native__ extends QMetaType {

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaType_buffer)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID nativeId,
 jint type0)
{
    try{
        if(void* ptr = qtjambi_from_nativeId(nativeId)){
            jlong size = QMetaType::sizeOf(type0);
            if(size>0){
                return __jni_env->NewDirectByteBuffer(ptr, size);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}
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
        __qt_QCborValue_setValue_cref_QString(checkedNativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborValue_setValue_cref_QString(long __this__nativeId, java.lang.String key, io.qt.core.QCborValue value);
    
    @io.qt.QtUninvokable
    public final void setValue(long key, io.qt.core.QCborValue value){
        __qt_QCborValue_setValue_long_long(checkedNativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborValue_setValue_long_long(long __this__nativeId, long key, io.qt.core.QCborValue value);
    
}// class

class QCborValue_native__{

// QCborValue::operator[](const QString & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborValue__1_1qt_1QCborValue_1setValue_1cref_1QString__JLjava_lang_String_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborValue::operator[](const QString & key)")
    try{
        QtJambiScope __qtjambi_scope(__this_nativeId);
        Q_UNUSED(__qtjambi_scope)
        QCborValue *__qt_this = qtjambi_object_from_nativeId<QCborValue>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QString&  __qt_key0 = qtjambi_cast<QString >(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborValue::operator[](long long key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborValue__1_1qt_1QCborValue_1setValue_1long_1long__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborValue::operator[](long long key)")
    try{
        QtJambiScope __qtjambi_scope(__this_nativeId);
        Q_UNUSED(__qtjambi_scope)
        QCborValue *__qt_this = qtjambi_object_from_nativeId<QCborValue>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[static_cast<qlonglong>(key0)] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

}// class

class QCborArray_java__{

    @io.qt.QtUninvokable
    public final void setValue(long i, io.qt.core.QCborValue value){
        __qt_QCborArray_setValue_qsizetype(checkedNativeId(this), i, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborArray_setValue_qsizetype(long __this__nativeId, long i, io.qt.core.QCborValue value);

}// class

class QCborArray_native__{

// QCborArray::operator[](qsizetype i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborArray__1_1qt_1QCborArray_1setValue_1qsizetype__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong i0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborArray::operator[](qsizetype i)")
    try{
        QtJambiScope __qtjambi_scope(__this_nativeId);
        Q_UNUSED(__qtjambi_scope)
        QCborArray *__qt_this = qtjambi_object_from_nativeId<QCborArray>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[static_cast<qsizetype>(i0)] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}


}// class

class QCborMap_java__{
    
    @io.qt.QtUninvokable
    public final void setValue(io.qt.core.QCborValue key, io.qt.core.QCborValue value){
        __qt_QCborMap_setValue_cref_QCborValue(checkedNativeId(this), nativeId(key), value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborMap_setValue_cref_QCborValue(long __this__nativeId, long key, io.qt.core.QCborValue value);

    @io.qt.QtUninvokable
    public final void setValue(java.lang.String key, io.qt.core.QCborValue value){
        __qt_QCborMap_setValue_cref_QString(checkedNativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborMap_setValue_cref_QString(long __this__nativeId, java.lang.String key, io.qt.core.QCborValue value);

    @io.qt.QtUninvokable
    public final void setValue(long key, io.qt.core.QCborValue value){
        __qt_QCborMap_setValue_long_long(checkedNativeId(this), key, value);
    }
    @io.qt.QtUninvokable
    private native void __qt_QCborMap_setValue_long_long(long __this__nativeId, long key, io.qt.core.QCborValue value);
    
}// class

class QCborMap_native__{

// QCborMap::operator[](const QCborValue & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap__1_1qt_1QCborMap_1setValue_1cref_1QCborValue__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](const QCborValue & key)")
    try{
        QCborMap *__qt_this = qtjambi_object_from_nativeId<QCborMap>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QCborValue&  __qt_key0 = qtjambi_object_from_nativeId_deref<QCborValue>(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborMap::operator[](const QString & key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap__1_1qt_1QCborMap_1setValue_1cref_1QString__JLjava_lang_String_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](const QString & key)")
    try{
        QCborMap *__qt_this = qtjambi_object_from_nativeId<QCborMap>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QString&  __qt_key0 = qtjambi_cast<QString >(__jni_env, key0);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[__qt_key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QCborMap::operator[](long long key)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborMap__1_1qt_1QCborMap_1setValue_1long_1long__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong key0,
 jobject value1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborMap::operator[](long long key)")
    try{
        QCborMap *__qt_this = qtjambi_object_from_nativeId<QCborMap>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborValue __qt_value1 = qtjambi_cast<QCborValue>(__jni_env, value1);
        (*__qt_this)[key0] = __qt_value1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

}// class

class QCborStreamReader_java__{
    
    public QCborStreamReader(byte[] data){
        this(new io.qt.core.QByteArray(data));
    }
    
    @io.qt.QtUninvokable
    public final void addData(byte[] data){
        addData(new io.qt.core.QByteArray(data));
    }
    
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
        return __qt_QCborStreamReader_readByteArray(checkedNativeId(this));
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QCborStreamReader.StringResult<io.qt.core.QByteArray> __qt_QCborStreamReader_readByteArray(long __this__nativeId);

    @io.qt.QtUninvokable
    public final io.qt.core.QCborStreamReader.StringResult<String> readString(){
        return __qt_QCborStreamReader_readString(checkedNativeId(this));
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QCborStreamReader.StringResult<String> __qt_QCborStreamReader_readString(long __this__nativeId);

    @io.qt.QtUninvokable
    public final io.qt.core.QCborStreamReader.StringResult<Long> readChunk(java.nio.ByteBuffer buffer){
        return __qt_QCborStreamReader_readChunk(checkedNativeId(this), buffer);
    }
    @io.qt.QtUninvokable
    private native io.qt.core.QCborStreamReader.StringResult<Long> __qt_QCborStreamReader_readChunk(long __this__nativeId, java.nio.ByteBuffer buffer);
    
}// class

class QCborStreamReader_native__{

// QCborStreamReader::readByteArray()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader__1_1qt_1QCborStreamReader_1readByteArray__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readByteArray()")
    try{
        QCborStreamReader *__qt_this = qtjambi_object_from_nativeId<QCborStreamReader>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborStreamReader::StringResult<QByteArray> __qt_return_value = __qt_this->readByteArray();
        return Java::Private::QtCore::QCborStreamReader$StringResult.newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QCborStreamReader::readString()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader__1_1qt_1QCborStreamReader_1readString__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readString()")
    try{
        QCborStreamReader *__qt_this = qtjambi_object_from_nativeId<QCborStreamReader>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QCborStreamReader::StringResult<QString> __qt_return_value = __qt_this->readString();
        return Java::Private::QtCore::QCborStreamReader$StringResult.newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QCborStreamReader::readChunk()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCborStreamReader__1_1qt_1QCborStreamReader_1readChunk__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject buffer)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCborStreamReader::readChunk()")
    try{
        QCborStreamReader *__qt_this = qtjambi_object_from_nativeId<QCborStreamReader>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        JBufferData __qt_buffer(__jni_env, buffer);
        QCborStreamReader::StringResult<qsizetype> __qt_return_value = __qt_this->readStringChunk(__qt_buffer, __qt_buffer.size());
        return Java::Private::QtCore::QCborStreamReader$StringResult.newInstance(__jni_env, qtjambi_cast<jobject>(__jni_env, __qt_return_value.data), qtjambi_cast<jobject>(__jni_env, __qt_return_value.status));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

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
    
    public static void registerPluginInterface(Class<? extends QtObjectInterface> factoryClass){
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
        return instance(checkedNativeId(this));
    }
    
    private static native final io.qt.core.QObject instance(long nativeId);
}// class

class QStaticPlugin_native__{

// QStaticPlugin::instance()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QStaticPlugin_instance)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QStaticPlugin::instance()")
    try{
        QStaticPlugin *__qt_this = qtjambi_object_from_nativeId<QStaticPlugin>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->instance());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

}// class

class QThread___{
    
    @io.qt.QtUninvokable
    static native void initializeCurrentThread();
    
    public QThread(ThreadGroup group, String name, long stackSize, io.qt.core.QObject parent) {
        super((QPrivateConstructor)null);        
        constructorThreadCheck(this);
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
        initialize(checkedNativeId(this), group);
    }
    
    public final ThreadGroup getThreadGroup() {
        if(javaThread!=null) {
            return javaThread.getThreadGroup();
        }
        return getThreadGroup(checkedNativeId(this));
    }
    
    private native ThreadGroup getThreadGroup(long __this__nativeId);
    
    public void setName(String name) {
        if(!isRunning() && javaThread==null)
            setName(checkedNativeId(this), name);
    }
    
    private native void setName(long __this__nativeId, String name);
    
    public final String getName() {
        if(javaThread!=null) {
            return javaThread.getName();
        }
        return getName(checkedNativeId(this));
    }
    
    private native String getName(long __this__nativeId);
    
    public void setDaemon(boolean daemon) {
        if(!isRunning() && javaThread==null)
            setDaemon(checkedNativeId(this), daemon);
    }
    
    private native void setDaemon(long __this__nativeId, boolean daemon);
    
    public final boolean isDaemon() {
        if(javaThread!=null) {
            return javaThread.isDaemon();
        }
        return isDaemon(checkedNativeId(this));
    }
    
    private native boolean isDaemon(long __this__nativeId);
    
    public void setUncaughtExceptionHandler(Thread.UncaughtExceptionHandler handler) {
        if(javaThread!=null) {
            javaThread.setUncaughtExceptionHandler(handler);
        }else {
            setUncaughtExceptionHandler(checkedNativeId(this), handler);
        }
    }
    
    private native void setUncaughtExceptionHandler(long __this__nativeId, Thread.UncaughtExceptionHandler handler);
    
    public final Thread.UncaughtExceptionHandler getUncaughtExceptionHandler() {
        if(javaThread!=null) {
            return javaThread.getUncaughtExceptionHandler();
        }
        return getUncaughtExceptionHandler(checkedNativeId(this));
    }
    
    private native Thread.UncaughtExceptionHandler getUncaughtExceptionHandler(long __this__nativeId);
    
    public void setContextClassLoader(ClassLoader cl) {
        if(javaThread!=null) {
            javaThread.setContextClassLoader(cl);
        }else {
            setContextClassLoader(checkedNativeId(this), cl);
        }
    }
    
    private native void setContextClassLoader(long __this__nativeId, ClassLoader cl);
    
    public final ClassLoader getContextClassLoader() {
        if(javaThread!=null) {
            return javaThread.getContextClassLoader();
        }
        return getContextClassLoader(checkedNativeId(this));
    }
    
    private native ClassLoader getContextClassLoader(long __this__nativeId);
    
    private native void initialize(long nativeId, ThreadGroup group);
    private final Thread javaThread = null;
    
    public final Thread javaThread() { return javaThread==null ? javaThread(checkedNativeId(this)) : javaThread; }
    
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
                                        qthread.__qt_QThread_requestInterruption(checkedNativeId(qthread));
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

class QThread_native__{
    
#if !defined(QT_QTJAMBI_PORT)

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_javaThread)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_from_thread(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_thread)
(JNIEnv *__jni_env, jclass, jobject thread)
{
    try{
        return qtjambi_cast<jobject>(__jni_env, qtjambi_to_thread(__jni_env, thread));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getThreadGroup)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_thread_get_group(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setUncaughtExceptionHandler)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jobject handler)
{
    try{
        qtjambi_thread_set_UncaughtExceptionHandler(__jni_env, __this_nativeId, handler);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getUncaughtExceptionHandler)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_thread_get_UncaughtExceptionHandler(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setContextClassLoader)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jobject cl)
{
    try{
        qtjambi_thread_set_ContextClassLoader(__jni_env, __this_nativeId, cl);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getContextClassLoader)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_thread_get_ContextClassLoader(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setName)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jstring name)
{
    try{
        qtjambi_thread_set_name(__jni_env, __this_nativeId, name);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getName)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_thread_get_name(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setDaemon)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jboolean daemon)
{
    try{
        qtjambi_thread_set_daemon(__jni_env, __this_nativeId, daemon);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_isDaemon)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_thread_is_daemon(__qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return false;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_initialize)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jobject group)
{
    try{
        qtjambi_initialize_thread(__jni_env, __this_nativeId, group);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

#endif // !defined(QT_QTJAMBI_PORT)

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

class QPersistentModelIndex__{
    
    @io.qt.QtUninvokable
    public final io.qt.core.QModelIndex toIndex(){
        return __qt_QPersistentModelIndex_toIndex(checkedNativeId(this));
    }
    
    @io.qt.QtUninvokable
    private native io.qt.core.QModelIndex __qt_QPersistentModelIndex_toIndex(long __this__nativeId);
    
}// class

class QPersistentModelIndex_native_{

// QPersistentModelIndex::operator const QModelIndex &() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QPersistentModelIndex__1_1qt_1QPersistentModelIndex_1toIndex__J)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPersistentModelIndex::operator const QModelIndex &() const")
    try{
#if QT_CONFIG(itemmodel)
        const QPersistentModelIndex *__qt_this = qtjambi_object_from_nativeId<QPersistentModelIndex>(__this_nativeId);
        Q_ASSERT(__qt_this);
        const QModelIndex& __qt_return_value = *__qt_this;
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(row0)
        Q_UNUSED(column1)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(itemmodel)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

}// class

class QDir__{
    @io.qt.QtUninvokable
    public final java.util.List<io.qt.core.QFileInfo> entryInfoList(io.qt.core.QDir.Filter... filters) {
        return entryInfoList(new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
    
    @io.qt.QtUninvokable
    public final java.util.List<java.lang.String> entryList(io.qt.core.QDir.Filter... filters) {
        return entryList(new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
    
    @io.qt.QtUninvokable
    public final java.util.List<java.lang.String> entryList(java.util.Collection<java.lang.String> nameFilters, io.qt.core.QDir.Filter... filters) {
        return entryList(nameFilters, new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
    
    @io.qt.QtUninvokable
    public final java.util.List<io.qt.core.QFileInfo> entryInfoList(java.util.Collection<java.lang.String> nameFilters, io.qt.core.QDir.Filter... filters) {
        return entryInfoList(nameFilters, new io.qt.core.QDir.Filters(filters), new io.qt.core.QDir.SortFlags(-1));
    }
}// class

class QFile__{
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

