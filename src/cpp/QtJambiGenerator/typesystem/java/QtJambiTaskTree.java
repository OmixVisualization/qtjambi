/****************************************************************************
**
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

class runner_start_overloads{
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.util.function.@NonNull Consumer<@NonNull QTaskTree> setupHandler, java.util.function.@NonNull Consumer<@NonNull QTaskTree> doneHandler) {
   start(FIRSTARG%recipe, setupHandler, doneHandler==null ? null : (t,w)->doneHandler.accept(t));
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.util.function.@NonNull Consumer<@NonNull QTaskTree> setupHandler, java.util.function.@NonNull Consumer<@NonNull QTaskTree> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    start(FIRSTARG%recipe, setupHandler, doneHandler==null ? null : (t,w)->doneHandler.accept(t), callDone);
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.util.function.@NonNull Consumer<@NonNull QTaskTree> setupHandler, java.util.function.@NonNull Consumer<@NonNull QTaskTree> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    start(FIRSTARG%recipe, setupHandler, doneHandler==null ? null : (t,w)->doneHandler.accept(t), new QtTaskTree.CallDone(callDone));
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.util.function.@NonNull Consumer<@NonNull QTaskTree> setupHandler, java.lang.@NonNull Runnable doneHandler) {
    start(FIRSTARG%recipe, setupHandler, doneHandler==null ? null : (t,w)->doneHandler.run());
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.util.function.@NonNull Consumer<@NonNull QTaskTree> setupHandler, java.lang.@NonNull Runnable doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    start(FIRSTARG%recipe, setupHandler, doneHandler==null ? null : (t,w)->doneHandler.run(), callDone);
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.util.function.@NonNull Consumer<@NonNull QTaskTree> setupHandler, java.lang.@NonNull Runnable doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    start(FIRSTARG%recipe, setupHandler, doneHandler==null ? null : (t,w)->doneHandler.run(), new QtTaskTree.CallDone(callDone));
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.util.function.@NonNull BiConsumer<@NonNull QTaskTree, QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler, new QtTaskTree.CallDone(callDone));
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.util.function.@NonNull BiConsumer<@NonNull QTaskTree, QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler, callDone);
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.util.function.@NonNull BiConsumer<@NonNull QTaskTree, QtTaskTree.@NonNull DoneWith> doneHandler) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler);
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.util.function.@NonNull Consumer<@NonNull QTaskTree> doneHandler) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler==null ? null : (t,w)->doneHandler.accept(t));
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.util.function.@NonNull Consumer<@NonNull QTaskTree> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler==null ? null : (t,w)->doneHandler.accept(t), callDone);
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.util.function.@NonNull Consumer<@NonNull QTaskTree> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler==null ? null : (t,w)->doneHandler.accept(t), new QtTaskTree.CallDone(callDone));
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.lang.@NonNull Runnable doneHandler) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler==null ? null : (t,w)->doneHandler.run());
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.lang.@NonNull Runnable doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler==null ? null : (t,w)->doneHandler.run(), callDone);
}
/**
 * <p>Overloaded function for {@link #start(FIRSTLINK%Group, java.util.function.Consumer, java.util.function.BiConsumer, QtTaskTree.CallDone)}.</p>
 */
@QtUninvokable
public final void start(FIRSTDECL%@StrictNonNull Group recipe, java.lang.@NonNull Runnable setupHandler, java.lang.@NonNull Runnable doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    start(FIRSTARG%recipe, setupHandler==null ? null : t->setupHandler.run(), doneHandler==null ? null : (t,w)->doneHandler.run(), new QtTaskTree.CallDone(callDone));
}
}// class

class group_overloads{
public static final @NonNull GroupItem onGroupSetup(java.lang.@StrictNonNull Runnable handler) {
    return onGroupSetup(()->{handler.run(); return QtTaskTree.SetupResult.Continue;});
}

public static final @NonNull GroupItem onGroupSetup(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> handler) {
    return ONGROUPSETUP%(handler);
}

public static final @NonNull GroupItem onGroupDone(java.lang.@StrictNonNull Runnable handler) {
    return onGroupDone(handler, QtTaskTree.CallDoneFlag.Always.asFlags());
}

public static final @NonNull GroupItem onGroupDone(java.lang.@StrictNonNull Runnable handler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    return onGroupDone(handler, new QtTaskTree.CallDone(callDone));
}

public static final @NonNull GroupItem onGroupDone(java.lang.@StrictNonNull Runnable handler, QtTaskTree.@NonNull CallDone callDone) {
    return onGroupDone(dw->{handler.run(); return QtTaskTree.DoneResult.Success;}, callDone);
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> handler) {
    return onGroupDone(handler, QtTaskTree.CallDoneFlag.Always.asFlags());
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> handler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    return onGroupDone(handler, new QtTaskTree.CallDone(callDone));
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> handler, QtTaskTree.@NonNull CallDone callDone) {
    return onGroupDone(dw->{handler.accept(dw); return QtTaskTree.DoneResult.Success;}, callDone);
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> handler) {
    return onGroupDone(handler, QtTaskTree.CallDoneFlag.Always.asFlags());
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> handler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    return onGroupDone(handler, new QtTaskTree.CallDone(callDone));
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> handler, QtTaskTree.@NonNull CallDone callDone) {
    return onGroupDone((java.util.function.Function<QtTaskTree.DoneWith,QtTaskTree.DoneResult>)dw->handler.get(), callDone);
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Function<QtTaskTree.@NonNull DoneWith,QtTaskTree.@NonNull DoneResult> handler) {
    return onGroupDone(handler, QtTaskTree.CallDoneFlag.Always.asFlags());
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Function<QtTaskTree.@NonNull DoneWith,QtTaskTree.@NonNull DoneResult> handler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    return onGroupDone(handler, new QtTaskTree.CallDone(callDone));
}

public static final @NonNull GroupItem onGroupDone(java.util.function.@StrictNonNull Function<QtTaskTree.@NonNull DoneWith,QtTaskTree.@NonNull DoneResult> handler, QtTaskTree.@NonNull CallDone callDone) {
    return ONGROUPDONE%(handler, callDone);
}
}// class

struct Group{
QtTaskTree::GroupItem groupHandler(const QtTaskTree::GroupItem::GroupSetupHandler& setupHandler, const QtTaskTree::GroupItem::GroupDoneHandler& doneHandler = {}, QtTaskTree::CallDone callDoneFlags = QtTaskTree::CallDoneFlag::Always);

// QtTaskTree::Group::onGroupSetup<Handler>(Handler &&)
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_tasktree_Group_onGroupSetupImpl
    (JNIEnv *__jni_env,
     jclass,
     jobject handler0)
{
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::Group::onGroupSetup<Handler>(Handler &&)")
        QtTaskTree::GroupItem::GroupSetupHandler __qt_handler0;
        if(handler0){
            __qt_handler0 = [wrapper = JObjectWrapper(__jni_env, handler0)]() -> QtTaskTree::SetupResult{
                if(JniEnvironment env{200}){
                    QTJAMBI_TRY{
                        return qtjambi_cast<QtTaskTree::SetupResult>(env, Java::Runtime::Supplier::get(env, wrapper.object(env)));
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
                return QtTaskTree::SetupResult::StopWithError;
            };
        }
        QtTaskTree::GroupItem __qt_return_value = groupHandler(__qt_handler0);
        __java_return_value = qtjambi_cast<jobject>(__jni_env, std::move(__qt_return_value));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

// QtTaskTree::Group::onGroupDone<Handler>(Handler &&, CallDone)
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_tasktree_Group_onGroupDoneImpl
    (JNIEnv *__jni_env,
     jclass,
     jobject handler0,
     jobject callDone1)
{
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::Group::onGroupDone<Handler>(Handler &&, CallDone)")
        QtTaskTree::GroupItem::GroupDoneHandler __qt_handler0;
        if(handler0){
            __qt_handler0 = [wrapper = JObjectWrapper(__jni_env, handler0)](QtTaskTree::DoneWith dw) -> QtTaskTree::DoneResult{
                if(JniEnvironment env{200}){
                    QTJAMBI_TRY{
                        return qtjambi_cast<QtTaskTree::DoneResult>(env, Java::Runtime::Function::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, dw)));
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
                return QtTaskTree::DoneResult::Error;
            };
        }
        QtTaskTree::GroupItem __qt_return_value = groupHandler({}, __qt_handler0, qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone1));
        __java_return_value = qtjambi_cast<jobject>(__jni_env, std::move(__qt_return_value));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}
}// class

class ExecutableItem{
private static java.util.function.Supplier<io.qt.core.QPair<io.qt.core.QObject,io.qt.core.QMetaMethod>> convert(java.util.function.@NonNull Supplier<io.qt.core.QMetaObject.@StrictNonNull AbstractSignal> signalGetter) {
    return ()->{
        io.qt.core.QMetaObject.AbstractSignal signal = signalGetter.get();
        if(signal.containingObject() instanceof io.qt.core.QObject) {
            io.qt.core.QObject object = (io.qt.core.QObject)signal.containingObject();
            io.qt.core.QMetaMethod sg = io.qt.core.QMetaMethod.fromSignal(signal);
            if(sg!=null && sg.isValid())
                return new io.qt.core.QPair<>(object, sg);
        }
        return new io.qt.core.QPair<>(null,null);
    };
}
public final Group withCancel(java.util.function.@NonNull Supplier<io.qt.core.QMetaObject.@StrictNonNull AbstractSignal> signalGetter, @NonNull GroupItem @NonNull... postCancelRecipe) {
    return withCancel(QtJambi_LibraryUtilities.internal.nativeId(this), convert(signalGetter), postCancelRecipe);
}
@QtUninvokable
private native Group withCancel(long __this__nativeId, java.util.function.@NonNull Supplier<io.qt.core.QPair<io.qt.core.QObject,io.qt.core.QMetaMethod>> signalGetter, GroupItem[] postCancelRecipe);

public final Group withAccept(java.util.function.@NonNull Supplier<io.qt.core.QMetaObject.@StrictNonNull AbstractSignal> signalGetter) {
    return withAccept(QtJambi_LibraryUtilities.internal.nativeId(this), convert(signalGetter));
}
@QtUninvokable
private native Group withAccept(long __this__nativeId, java.util.function.Supplier<io.qt.core.QPair<io.qt.core.QObject,io.qt.core.QMetaMethod>> signalGetter);
}// class

struct ExecutableItem{
QtTaskTree::Group withCancel(QtTaskTree::ExecutableItem* ei, const std::function<void(QObject *, const std::function<void()> &)> &connectWrapper,
                             const QtTaskTree::GroupItems &postCancelRecipe);
QtTaskTree::Group withAccept(QtTaskTree::ExecutableItem* ei, const std::function<void(QObject *, const std::function<void()> &)> &connectWrapper);

// QtTaskTree::ExecutableItem::withCancel(ObjectSignalGetter &&,std::initializer_list<GroupItem>)const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_tasktree_ExecutableItem_withCancel
    (JNIEnv *__jni_env,
     jobject __this,
     QtJambiNativeID __this_nativeId,
     jobject signalGetter0,
     jobjectArray postCancelRecipe1)
{
    Q_UNUSED(__this)
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QtTaskTree::ExecutableItem *__qt_this = qtjambi_cast<QtTaskTree::ExecutableItem*>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QtTaskTree::ExecutableItem::withCancel(ObjectSignalGetter &&,std::initializer_list<GroupItem>)const", __this_nativeId)
        std::function<void(QObject *, const std::function<void()> &)> __qt_signalGetter0;
        if(signalGetter0){
            __qt_signalGetter0 = [wrapper = JObjectWrapper(__jni_env, signalGetter0)](QObject *guard, const std::function<void()> &trigger) {
                if(JniEnvironment env{200}){
                    QTJAMBI_TRY{
                        QPair<QObject*,QMetaMethod> signalInfo = qtjambi_cast<QPair<QObject*,QMetaMethod>>(env, Java::Runtime::Supplier::get(env, wrapper.object(env)));
                        if(signalInfo.first){
                            QMetaObject::connect(signalInfo.first, signalInfo.second, guard, trigger,
                                             static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::SingleShotConnection));
                        }
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
            };
        }
        QtTaskTree::Group __qt_return_value = withCancel(__qt_this, __qt_signalGetter0, convertGroupItems(__jni_env, postCancelRecipe1));
        __java_return_value = qtjambi_cast<jobject>(__jni_env, std::move(__qt_return_value));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

// QtTaskTree::ExecutableItem::withAccept(ObjectSignalGetter &&)const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_tasktree_ExecutableItem_withAccept
    (JNIEnv *__jni_env,
     jobject __this,
     QtJambiNativeID __this_nativeId,
     jobject signalGetter0)
{
    Q_UNUSED(__this)
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QtTaskTree::ExecutableItem *__qt_this = qtjambi_cast<QtTaskTree::ExecutableItem*>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QtTaskTree::ExecutableItem::withAccept(ObjectSignalGetter &&,std::initializer_list<GroupItem>)const", __this_nativeId)
        std::function<void(QObject *, const std::function<void()> &)> __qt_signalGetter0;
        if(signalGetter0){
            __qt_signalGetter0 = [wrapper = JObjectWrapper(__jni_env, signalGetter0)](QObject *guard, const std::function<void()> &trigger) {
                if(JniEnvironment env{200}){
                    QTJAMBI_TRY{
                        QPair<QObject*,QMetaMethod> signalInfo = qtjambi_cast<QPair<QObject*,QMetaMethod>>(env, Java::Runtime::Supplier::get(env, wrapper.object(env)));
                        if(signalInfo.first){
                            QMetaObject::connect(signalInfo.first, signalInfo.second, guard, trigger,
                                                 static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::SingleShotConnection));
                        }
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
            };
        }
        QtTaskTree::Group __qt_return_value = withAccept(__qt_this, __qt_signalGetter0);
        __java_return_value = qtjambi_cast<jobject>(__jni_env, std::move(__qt_return_value));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}
}// class

class When{
public <Task extends io.qt.core.QObject> When(@StrictNonNull QCustomTask<Task, java.util.function.@NonNull BiConsumer<@Nullable Task,@Nullable QTaskInterface>> task, @StrictNonNull String signalName, QtTaskTree.@NonNull WorkflowPolicy policy) {
    super((QPrivateConstructor)null);
    io.qt.core.QMetaObject mo = io.qt.core.QMetaObject.forType(task.taskType);
    if(mo==null)
        throw new QNoSuchSignalException(task.taskType.getTypeName() + "." + signalName);
    io.qt.core.QMetaMethod signal = mo.method(signalName);
    if(!signal.isValid() || signal.methodType()!=io.qt.core.QMetaMethod.MethodType.Signal)
        throw new QNoSuchSignalException(task.taskType.getTypeName() + "." + signalName);
    if(task.taskType==QTaskTree.class && ((Class<?>)task.adapterType)==QTaskTreeTaskAdapter.class) {
        initialize_native_QTaskTree(this, task, signal, policy);
    }else if(QBarrier.class==task.taskType && ((Class<?>)task.adapterType)==QDefaultTaskAdapter.class) {
        initialize_native_QBarrier(this, task, signal, policy);
    }else if(io.qt.core.QProcess.class==task.taskType && ((Class<?>)task.adapterType)==QProcessTaskAdapter.class) {
        initialize_native_QProcess(this, task, signal, policy);
    }else {
        initialize_native_custom(this, task, signal, policy);
    }
}

public static final When when(java.util.function.@NonNull Function<io.qt.tasktree.@NonNull QStoredBarrier, io.qt.tasktree.@NonNull ExecutableItem> kicker) {
    return new When(kicker);
}
public static final When when(java.util.function.@NonNull Function<io.qt.tasktree.@NonNull QStoredBarrier, io.qt.tasktree.@NonNull ExecutableItem> kicker, io.qt.tasktree.QtTaskTree.@NonNull WorkflowPolicy policy) {
    return new When(kicker, policy);
}
public static final <Task extends io.qt.core.QObject> When when(@StrictNonNull QCustomTask<Task, java.util.function.@NonNull BiConsumer<@Nullable Task,@Nullable QTaskInterface>> task, @StrictNonNull String signalName, io.qt.tasktree.QtTaskTree.@NonNull WorkflowPolicy policy) {
    return new When(task, signalName, policy);
}

private native static <Task extends io.qt.core.QObject> void initialize_native_QTaskTree(When instance, QCustomTask<Task, java.util.function.BiConsumer<Task,QTaskInterface>> task, io.qt.core.QMetaMethod signal, QtTaskTree.WorkflowPolicy policy);
private native static <Task extends io.qt.core.QObject> void initialize_native_QBarrier(When instance, QCustomTask<Task, java.util.function.BiConsumer<Task,QTaskInterface>> task, io.qt.core.QMetaMethod signal, QtTaskTree.WorkflowPolicy policy);
private native static <Task extends io.qt.core.QObject> void initialize_native_QProcess(When instance, QCustomTask<Task, java.util.function.BiConsumer<Task,QTaskInterface>> task, io.qt.core.QMetaMethod signal, QtTaskTree.WorkflowPolicy policy);
private native static <Task extends io.qt.core.QObject> void initialize_native_custom(When instance, QCustomTask<Task, java.util.function.BiConsumer<Task,QTaskInterface>> task, io.qt.core.QMetaMethod signal, QtTaskTree.WorkflowPolicy policy);
}// class

class For{
public static final @NonNull For iter(Iterator iterator) {
    return new For(iterator);
}
public static final <T> @NonNull ForEachItem<T> each(io.qt.core.@StrictNonNull QList<T> list) {
    return new ForEachItem<>(list);
}
public static final @NonNull ForItem repeat(long count) {
    return new ForItem(count);
}
public static final @NonNull ForItem until(java.util.function.@NonNull LongPredicate condition) {
    return new ForItem(condition);
}
}// class

class If{
public static If success(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> handler){
    return new If(handler);
}
public static If test(java.util.function.@StrictNonNull BooleanSupplier handler){
    return new If(handler);
}
}// class

class ThenItem{
public @NonNull ElseIfItem elif(@StrictNonNull ElseIf elseIfItem){
    return QtTaskTree.elif(this, elseIfItem);
}
public ElseIfItem elif(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> handler){
    return QtTaskTree.elif(this, new ElseIf(handler));
}
public ElseIfItem elif(java.util.function.@StrictNonNull BooleanSupplier handler){
    return QtTaskTree.elif(this, new ElseIf(handler));
}
}// class

class ThenFunctions{
public @NonNull ThenItem then(@StrictNonNull Then thenItem){
    return QtTaskTree.then(this, thenItem);
}
public @NonNull ThenItem then(java.util.@NonNull Collection<? extends @NonNull GroupItem> children){
    return QtTaskTree.then(this, new Then(children));
}
public @NonNull ThenItem then(@NonNull GroupItem@NonNull  ... children){
    return QtTaskTree.then(this, new Then(children));
}
public @NonNull ThenItem then(java.lang.@StrictNonNull Runnable item) {
    java.util.Objects.requireNonNull(item);
    return QtTaskTree.then(this, new Then(new QSyncTask(item)));
}
public @NonNull ThenItem then(java.lang.@StrictNonNull Runnable item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.lang.@StrictNonNull Runnable item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.lang.@StrictNonNull Runnable item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, QtTaskTree.CallDoneFlag.Always.asFlags())));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.lang.@StrictNonNull Runnable item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.lang.@StrictNonNull Runnable item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.lang.@StrictNonNull Runnable item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, QtTaskTree.CallDoneFlag.Always.asFlags())));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.lang.@StrictNonNull Runnable item) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull BooleanSupplier item) {
    java.util.Objects.requireNonNull(item);
    return QtTaskTree.then(this, new Then(new QSyncTask(item)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull BooleanSupplier item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull BooleanSupplier item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull BooleanSupplier item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, QtTaskTree.CallDoneFlag.Always.asFlags())));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull BooleanSupplier item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull BooleanSupplier item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull BooleanSupplier item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, QtTaskTree.CallDoneFlag.Always.asFlags())));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull BooleanSupplier item) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item) {
    java.util.Objects.requireNonNull(item);
    return QtTaskTree.then(this, new Then(new QSyncTask(item)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    return QtTaskTree.then(this, new Then(new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, QtTaskTree.CallDoneFlag.Always.asFlags())));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, callDone)));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item, java.util.function.@StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(doneHandler);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item), QtTaskTree.onGroupDone(doneHandler, QtTaskTree.CallDoneFlag.Always.asFlags())));
}
public @NonNull ThenItem then(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> item) {
    java.util.Objects.requireNonNull(item);
    java.util.Objects.requireNonNull(setupHandler);
    return QtTaskTree.then(this, new Then(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(item)));
}
}// class

struct When{
template <typename Task, typename Adapter, typename Deleter>
std::function<QtTaskTree::ExecutableItem(const QtTaskTree::QStoredBarrier &)> kickerForSignal(const QtTaskTree::QCustomTask<Task, Adapter, Deleter> &task,
                                                                                              QMetaMethod signal)
{
    return [taskHandler = QtTaskTree::GroupItemPrivate::taskHandler(task), signal](const QtTaskTree::QStoredBarrier &barrier) {
        auto handler = std::move(taskHandler);
        const auto wrappedSetupHandler
            = [originalSetupHandler = std::move(handler.m_taskAdapterSetupHandler),
               barrier, signal](void *taskAdapter) {
                  const QtTaskTree::SetupResult setupResult = std::invoke(originalSetupHandler, taskAdapter);
                  QObject* task;
                  if constexpr(std::is_same_v<QtTaskTree::QCustomTask<Task, Adapter, Deleter>,QtTaskTree::QBarrierTask>){
                      task = taskOfQBarrierTask(taskAdapter);
                  }else if constexpr(std::is_same_v<QtTaskTree::QCustomTask<Task, Adapter, Deleter>,QtTaskTree::QTaskTreeTask>){
                      task = taskOfQTaskTreeTask(taskAdapter);
                  }else if constexpr(std::is_same_v<QtTaskTree::QCustomTask<Task, Adapter, Deleter>,QtTaskTree::QProcessTask>){
                      task = taskOfQProcessTask(taskAdapter);
                  }else{
                      using TaskAdapter = typename QtTaskTree::QCustomTask<JObjectWrapper>::TaskAdapter;
                      task = static_cast<TaskAdapter *>(taskAdapter)->object;
                  }
                  QMetaObject::connect(task, signal,
                                       barrier.activeStorage(), &QtTaskTree::QStartedBarrier::advance);
                  return setupResult;
              };
        handler.m_taskAdapterSetupHandler = std::move(wrappedSetupHandler);
        return executableItem(std::move(handler));
    };
}

// new When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
void __qt_construct_QtTaskTree_When_QTaskTree(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    jobject task0 = __java_arguments[0].l;
    jobject signal1 = __java_arguments[1].l;
    jobject policy2 = __java_arguments[2].l;
    const QtTaskTree::QTaskTreeTask& task = QtJambiAPI::convertJavaObjectToNativeReference<const QtTaskTree::QTaskTreeTask>(__jni_env, task0);
    std::function<QtTaskTree::ExecutableItem(const QtTaskTree::QStoredBarrier&)> kicker = kickerForSignal(task,
                                                                                                           qtjambi_cast<QMetaMethod>(__jni_env, signal1));
    QtTaskTree::WorkflowPolicy __qt_policy2 = qtjambi_cast<QtTaskTree::WorkflowPolicy>(__jni_env, policy2);
    QtTaskTree::When *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::When(std::move(kicker), QtTaskTree::WorkflowPolicy(__qt_policy2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_When_initialize_1native_1QTaskTree
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject task0,
     jobject signal1,
     jobject policy2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = task0;
        arguments[1].l = signal1;
        arguments[2].l = policy2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QtTaskTree_When_QTaskTree, sizeof(QtTaskTree::When), alignof(QtTaskTree::When), typeid(QtTaskTree::When), 0, false, &__qt_delete_QtTaskTree_When, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
void __qt_construct_QtTaskTree_When_QBarrier(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    jobject task0 = __java_arguments[0].l;
    jobject signal1 = __java_arguments[1].l;
    jobject policy2 = __java_arguments[2].l;
    const QtTaskTree::QBarrierTask& task = QtJambiAPI::convertJavaObjectToNativeReference<const QtTaskTree::QBarrierTask>(__jni_env, task0);
    std::function<QtTaskTree::ExecutableItem(const QtTaskTree::QStoredBarrier&)> kicker = kickerForSignal(task,
                                                                                                           qtjambi_cast<QMetaMethod>(__jni_env, signal1));
    QtTaskTree::WorkflowPolicy __qt_policy2 = qtjambi_cast<QtTaskTree::WorkflowPolicy>(__jni_env, policy2);
    QtTaskTree::When *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::When(std::move(kicker), QtTaskTree::WorkflowPolicy(__qt_policy2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_When_initialize_1native_1QBarrier
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject task0,
     jobject signal1,
     jobject policy2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = task0;
        arguments[1].l = signal1;
        arguments[2].l = policy2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QtTaskTree_When_QBarrier, sizeof(QtTaskTree::When), alignof(QtTaskTree::When), typeid(QtTaskTree::When), 0, false, &__qt_delete_QtTaskTree_When, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
void __qt_construct_QtTaskTree_When_QProcess(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    jobject task0 = __java_arguments[0].l;
    jobject signal1 = __java_arguments[1].l;
    jobject policy2 = __java_arguments[2].l;
    const QtTaskTree::QProcessTask& task = QtJambiAPI::convertJavaObjectToNativeReference<const QtTaskTree::QProcessTask>(__jni_env, task0);
    std::function<QtTaskTree::ExecutableItem(const QtTaskTree::QStoredBarrier&)> kicker = kickerForSignal(task,
                                                                                                          qtjambi_cast<QMetaMethod>(__jni_env, signal1));
    QtTaskTree::WorkflowPolicy __qt_policy2 = qtjambi_cast<QtTaskTree::WorkflowPolicy>(__jni_env, policy2);
    QtTaskTree::When *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::When(std::move(kicker), QtTaskTree::WorkflowPolicy(__qt_policy2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_When_initialize_1native_1QProcess
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject task0,
     jobject signal1,
     jobject policy2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = task0;
        arguments[1].l = signal1;
        arguments[2].l = policy2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QtTaskTree_When_QProcess, sizeof(QtTaskTree::When), alignof(QtTaskTree::When), typeid(QtTaskTree::When), 0, false, &__qt_delete_QtTaskTree_When, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
void __qt_construct_QtTaskTree_When_custom(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    jobject task0 = __java_arguments[0].l;
    jobject signal1 = __java_arguments[1].l;
    jobject policy2 = __java_arguments[2].l;
    const QtTaskTree::QCustomTask<JObjectWrapper>& task = QtJambiAPI::convertJavaObjectToNativeReference<const QtTaskTree::QCustomTask<JObjectWrapper>>(__jni_env, task0);
    std::function<QtTaskTree::ExecutableItem(const QtTaskTree::QStoredBarrier&)> kicker = kickerForSignal(task,
                                                                                                           qtjambi_cast<QMetaMethod>(__jni_env, signal1));
    QtTaskTree::WorkflowPolicy __qt_policy2 = qtjambi_cast<QtTaskTree::WorkflowPolicy>(__jni_env, policy2);
    QtTaskTree::When *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::When(std::move(kicker), QtTaskTree::WorkflowPolicy(__qt_policy2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_When_initialize_1native_1custom
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject task0,
     jobject signal1,
     jobject policy2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::When::When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = task0;
        arguments[1].l = signal1;
        arguments[2].l = policy2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QtTaskTree_When_custom, sizeof(QtTaskTree::When), alignof(QtTaskTree::When), typeid(QtTaskTree::When), 0, false, &__qt_delete_QtTaskTree_When, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
}// class

class Storage{
public Storage(@StrictNonNull Class<StorageStruct> structType) {
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(structType);
    try {
        initialize_native(this, structType, QtJambi_LibraryUtilities.internal.getFactory0(structType.getConstructor()));
    } catch (NoSuchMethodException e) {
        throw new QNoImplementationException(String.format("Type %1$s is missing a default constructor.", structType.getName()));
    }
}
public Storage(io.qt.QtUtilities.@StrictNonNull Supplier<StorageStruct> supplier) {
    super((QPrivateConstructor)null);
    @SuppressWarnings("unchecked")
    Class<StorageStruct> structType = (Class<StorageStruct>)QtJambi_LibraryUtilities.internal.lambdaReturnType(io.qt.QtUtilities.Supplier.class, supplier);
    if(structType==null)
        throw new NullPointerException("Unable to detect return type of lambda expression");
    initialize_native(this, structType, java.util.Objects.requireNonNull(supplier));
}

public static class ActiveStorage<StorageStruct extends java.lang.Object> {
    private final io.qt.core.QVariant activeStorage;
    private final Class<StorageStruct> structType;
    private ActiveStorage(Class<StorageStruct> structType, io.qt.core.QVariant activeStorage) {
        super();
        this.structType = structType;
        this.activeStorage = activeStorage;
    }
    public final StorageStruct getStorage() {
        return structType.cast(activeStorage.value());
    }
    public final void setStorage(StorageStruct value) {
        if(io.qt.core.QObject.class.isAssignableFrom(structType))
            throw new QNoImplementationException(String.format("Unable to replace object of type %1$s.", structType.getName()));
        activeStorage.assign(structType.cast(value));
    }
}
}// class

class QTaskTree{
public final <StorageStruct> void onStorageSetup(@StrictNonNull Storage<StorageStruct> storage, java.util.function.@StrictNonNull Consumer<Storage.@NonNull ActiveStorage<StorageStruct>> handler) {
    onStorageSetup(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.checkedNativeId(storage), handler);
}
public static native final void onStorageSetup(long __this__nativeId, long storage, java.util.function.@StrictNonNull Consumer<?> handler);

public final <StorageStruct> void onStorageDone(@StrictNonNull Storage<StorageStruct> storage, java.util.function.@StrictNonNull Consumer<@NonNull StorageStruct> handler) {
    onStorageDone(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.checkedNativeId(storage), handler);
}
public static native final void onStorageDone(long __this__nativeId, long storage, java.util.function.Consumer<?> handler);

public final void onStorageSetup(@StrictNonNull QStoredBarrier storage, java.util.function.@StrictNonNull Consumer<@NonNull QStartedBarrier> handler) {
    onStorageSetupBarrier(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.checkedNativeId(storage), handler);
}
public static native final void onStorageSetupBarrier(long __this__nativeId, long storage, java.util.function.@StrictNonNull Consumer<?> handler);

public final void onStorageDone(@StrictNonNull QStoredBarrier storage, java.util.function.@StrictNonNull Consumer<@NonNull QStartedBarrier> handler) {
    onStorageDoneBarrier(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.checkedNativeId(storage), handler);
}
public static native final void onStorageDoneBarrier(long __this__nativeId, long storage, java.util.function.Consumer<?> handler);
}// class

class IF_ELSE{
public IF_ELSE(java.lang.@StrictNonNull Runnable handler){
    this(new QSyncTask(handler));
}

public IF_ELSE(java.util.function.@StrictNonNull Supplier<QtTaskTree.@NonNull DoneResult> handler){
    this(new QSyncTask(handler));
}

public IF_ELSE(java.util.function.@StrictNonNull BooleanSupplier handler){
    this(new QSyncTask(handler));
}
}// class

class QThreadFunction{
@SuppressWarnings("unchecked")
static <ResultType> Class<QThreadFunction<ResultType>> typedClass(){
    return (Class<QThreadFunction<ResultType>>)(Class<?>)QThreadFunction.class;
}
}// class

struct QThreadFunction{
}// class
